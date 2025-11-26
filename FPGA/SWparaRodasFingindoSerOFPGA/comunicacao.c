#include "comunicacao.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

static int global_sockfd = -1;
static MapUpdate buffer_updates[MAX_BUFFER_MAPA];
static int qtd_updates = 0;

static volatile bool flag_pedido_nova_rota = false;

// === CORREÇÃO: Variáveis para guardar a posição do robô ===
static volatile uint8_t ultima_pos_x = 0;
static volatile uint8_t ultima_pos_y = 0;

// ============================================================================
// FUNÇÕES AUXILIARES DE BITS
// ============================================================================

void comm_escrever_bits(uint32_t *buffer, int *pos_bit, int val, int nbits) {
    uint32_t mask = (1 << nbits) - 1;
    uint32_t val_limpo = val & mask;
    int shift = 32 - (*pos_bit) - nbits;
    if (shift >= 0) {
        *buffer |= (val_limpo << shift);
        *pos_bit += nbits;
    }
}

int comm_ler_bits(uint32_t buffer, int *pos_bit, int nbits) {
    int shift = 32 - (*pos_bit) - nbits;
    uint32_t mask = (1 << nbits) - 1;
    int valor = (buffer >> shift) & mask;
    *pos_bit += nbits;
    return valor;
}

void comm_debug_print_bits(uint32_t val, const char* label) {
    printf("[DEBUG NET] %s: ", label);
    for (int i = 31; i >= 0; i--) {
        printf("%d", (val >> i) & 1);
        if (i % 4 == 0 && i != 0) printf(" ");
    }
    printf(" (Hex: 0x%08X)\n", val);
}

void bufferizar_update(uint8_t x, uint8_t y) {
    if (qtd_updates >= MAX_BUFFER_MAPA) return;
    if (x >= TAMANHO_MALHA || y >= TAMANHO_MALHA) return;

    int indice = idx(x, y);
    int valor_bruto = readMalha(indice);

    // Extração manual (Lógica 29 bits do mapa.c)
    const int TOTAL_BITS = 29;
    int inicio_fisico = (TOTAL_BITS - 1) - 9;
    int largura = 2;
    int valor_cell = (valor_bruto >> inicio_fisico) & ((1 << largura) - 1);

    buffer_updates[qtd_updates].x = x;
    buffer_updates[qtd_updates].y = y;
    buffer_updates[qtd_updates].valor = valor_cell;
    qtd_updates++;
}

// ============================================================================
// CONEXÃO
// ============================================================================

int iniciar_conexao_socket() {
    int sockfd = 0;
    struct sockaddr_in serv_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[COMMS] Erro criar socket");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORTA_SERVIDOR);

    if (inet_pton(AF_INET, IP_SERVIDOR, &serv_addr.sin_addr) <= 0) return -1;

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sockfd);
        return -1;
    }
    printf("[COMMS] Conectado.\n");
    global_sockfd = sockfd;
    return sockfd;
}

int aguardar_esp_livre(int sockfd) {
    int32_t req = 0;
    int32_t resp_rede;
    int resp_local;

    while (1) {
        // Handshake PC->ESP: Envia 0, espera 1
        if (send(sockfd, &req, sizeof(req), 0) < 0) return -1;

        fd_set readfds;
        struct timeval tv;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        tv.tv_sec = TIMEOUT_HANDSHAKE_SEC;
        tv.tv_usec = 0;

        int activity = select(sockfd + 1, &readfds, NULL, NULL, &tv);

        if (activity <= 0) continue;

        int bytes = recv(sockfd, &resp_rede, sizeof(resp_rede), 0);
        if (bytes <= 0) return -1;

        resp_local = ntohl(resp_rede);
        if (resp_local == 1) return 1;

        usleep(500000);
    }
}

// ============================================================================
// ENVIO: CAMINHO PARA ARDUINO (SEM HANDSHAKE)
// ============================================================================

void enviar_caminho_arduino(int sockfd, uint8_t* caminho_saida, int tamanho) {
    if (tamanho <= 0) return;

    printf("[COMMS] Enviando Caminho (%d passos) [Sem Handshake]...\n", tamanho);

    // 1. Pacote de Tamanho (8 bits no MSB)
    uint32_t pct_tamanho = 0;
    int pos = 0;
    comm_escrever_bits(&pct_tamanho, &pos, tamanho, 8);

    uint32_t net_pkg = htonl(pct_tamanho);
    send(sockfd, &net_pkg, sizeof(net_pkg), 0);

    usleep(5000);

    // 2. Pacotes de Dados
    uint32_t buffer_dados = 0;
    int bits_ocupados = 0;
    pos = 0;

    for (int i = 0; i < tamanho; i++) {
        comm_escrever_bits(&buffer_dados, &pos, caminho_saida[i], 2);
        bits_ocupados += 2;

        if (bits_ocupados == 32 || i == tamanho - 1) {
            net_pkg = htonl(buffer_dados);
            send(sockfd, &net_pkg, sizeof(net_pkg), 0);

            buffer_dados = 0;
            pos = 0;
            bits_ocupados = 0;
            usleep(5000);
        }
    }
}

// ============================================================================
// ENVIO: MAPA PARA APP (COM HANDSHAKE)
// ============================================================================

void enviar_buffer_mapa_app(int sockfd) {
    if (qtd_updates == 0) return;

    if (aguardar_esp_livre(sockfd) != 1) return;

    printf("[COMMS] Enviando Mapa (%d updates)...\n", qtd_updates);

    // Header 1 (App) + Qtd
    uint32_t pct_controle = 0;
    int pos = 0;
    comm_escrever_bits(&pct_controle, &pos, 1, 1);
    comm_escrever_bits(&pct_controle, &pos, qtd_updates, 8);

    uint32_t net_pkg = htonl(pct_controle);
    send(sockfd, &net_pkg, sizeof(net_pkg), 0);

    // Dados
    uint32_t buffer_dados = 0;
    int updates_no_pacote = 0;
    pos = 0;

    for (int i = 0; i < qtd_updates; i++) {
        MapUpdate u = buffer_updates[i];
        comm_escrever_bits(&buffer_dados, &pos, u.x, 4);
        comm_escrever_bits(&buffer_dados, &pos, u.y, 4);
        comm_escrever_bits(&buffer_dados, &pos, u.valor, 2);

        updates_no_pacote++;

        if (updates_no_pacote == 3 || i == qtd_updates - 1) {
            net_pkg = htonl(buffer_dados);
            send(sockfd, &net_pkg, sizeof(net_pkg), 0);
            buffer_dados = 0;
            pos = 0;
            updates_no_pacote = 0;
            usleep(5000);
        }
    }
    qtd_updates = 0;
}

// ============================================================================
// RECEPÇÃO: TRATA HANDSHAKE (2) E CABEÇALHOS (0, 1)
// ============================================================================

void recebeDado(int sockfd) {
    int32_t buffer_rede;

    // Bloqueia esperando pacote
    int bytes = recv(sockfd, &buffer_rede, sizeof(buffer_rede), 0);
    if (bytes <= 0) return;

    uint32_t pacote = ntohl(buffer_rede);
    int pos = 0;
  
    printf("Recebido: %d\n", pacote);

    // Lê 2 BITS de cabeçalho
    int header = comm_ler_bits(pacote, &pos, 2);

    // ------------------------------------------------------------------------
    // CASO 2: PEDIDO DE PERMISSÃO (HANDSHAKE)
    // ------------------------------------------------------------------------
    if (header == HEADER_HANDSHAKE_REQ) { // 10 (bin) = 2 (dec)
        printf("[COMMS] Handshake Recebido: ESP quer enviar dados.\n");

        if (flag_pedido_nova_rota) {
            printf("[COMMS] Handshake Resp: 3 (OCUPADO - Processando Rota Anterior).\n");
            int32_t resp = RESP_PC_OCUPADO; // Envia 3
            int32_t resp_net = htonl(resp);
            send(sockfd, &resp_net, sizeof(resp_net), 0);
        } else {
            printf("[COMMS] Handshake Resp: 2 (LIVRE - Pode mandar).\n");
            int32_t resp = RESP_PC_LIVRE; // Envia 2
            int32_t resp_net = htonl(resp);
            send(sockfd, &resp_net, sizeof(resp_net), 0);
        }
        return;
    }

    // ------------------------------------------------------------------------
    // CASO 0: PEDIDO DE NOVA ROTA
    // ------------------------------------------------------------------------
    else if (header == HEADER_REQ_ROTA) { // 00 (bin) = 0 (dec)
        printf("[COMMS] Recebido Header 0: Pedido de Novo Trajeto.\n");
      /*
      result_t resultadoBusca = bfs_raio(posX, posY);
      int caminhoLen = aStar_direct(posX, posY, resultadoBusca.destinoX, resultadoBusca.destinoY, caminho_saida);
      
      //falta coisa aqui
      */
    }

    // ------------------------------------------------------------------------
    // CASO 1: DADOS DE SENSOR (MAPA)
    // ------------------------------------------------------------------------
    else if (header == HEADER_DADOS_MAPA) { // 01 (bin) = 1 (dec)
        // [X(4)] [Y(4)] [HV(1)] [Dir(4)] [Esq(4)]
        uint8_t posX = comm_ler_bits(pacote, &pos, 4);
        uint8_t posY = comm_ler_bits(pacote, &pos, 4);

        // === CORREÇÃO: Salva a posição recebida nas variáveis globais ===
        ultima_pos_x = posX;
        ultima_pos_y = posY;

        uint8_t hv   = comm_ler_bits(pacote, &pos, 1);
        uint8_t dDir = comm_ler_bits(pacote, &pos, 4);
        uint8_t dEsq = comm_ler_bits(pacote, &pos, 4);

        printf("[COMMS] Header 1 (Mapa): X:%d Y:%d\n", posX, posY);

        mapa_atualizar(posX, posY, (hv == 1), dDir, dEsq);

        // Preencher Buffer para o App
        qtd_updates = 0;
        bufferizar_update(posX, posY);

        int dirX = (hv == 1) ? 1 : 0;
        int dirY = (hv == 1) ? 0 : 1;

        for (int d = 1; d <= dDir; d++) bufferizar_update(posX + d*dirX, posY + d*dirY);
        bufferizar_update(posX + (dDir*dirX) + dirX, posY + (dDir*dirY) + dirY);

        for (int d = 1; d <= dEsq; d++) bufferizar_update(posX - d*dirX, posY - d*dirY);
        bufferizar_update(posX - (dEsq*dirX) - dirX, posY - (dEsq*dirY) - dirY);

        enviar_buffer_mapa_app(sockfd);
    }
}

// ============================================================================
// CONTROLE E LEITURA DE ESTADO
// ============================================================================

bool sinal_nova_rota() {
    if (flag_pedido_nova_rota) {
        flag_pedido_nova_rota = false;
        return true;
    }
    return false;
}

bool sinal_mapear() { return true; }

// === CORREÇÃO: Implementação das funções de leitura ===
uint8_t ler_pos_x() {
    return ultima_pos_x;
}

uint8_t ler_pos_y() {
    return ultima_pos_y;
}

void caminho(uint8_t* caminho_saida, int tamanho) {
    if (global_sockfd != -1) {
        enviar_caminho_arduino(global_sockfd, caminho_saida, tamanho);
    }
}
