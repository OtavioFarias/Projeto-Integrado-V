#include "comunicacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static MapUpdate buffer_updates[MAX_BUFFER_MAPA];
static int qtd_updates = 0;
static uint8_t ultima_pos_x = 0;
static uint8_t ultima_pos_y = 0;

// Protótipos
void enviar_buffer_mapa_app_simulado(int socket);
void bufferizar_update(uint8_t x, uint8_t y);
void comm_escrever_bits(uint32_t *buffer, int *pos_bit, int val, int nbits);
int comm_ler_bits(uint32_t buffer, int *pos_bit, int nbits);
int idx(int x, int y);

// ============================================================================
// AUXILIAR: Imprime 32 bits (Int -> String)
// ==========================================================comm_escrever_bits==================
void print_binary_string(uint32_t n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

// ============================================================================
// AUXILIAR: Converte String "1010..." -> Int32
// ============================================================================
uint32_t string_to_binary(char* str) {
    uint32_t val = 0;

    // REMOVE whitespace: \r \n \t e espaços
    char clean[40];
    int w = 0;

    for (int i = 0; str[i] != 0 && w < 32; i++) {
        if (str[i] == '0' || str[i] == '1') {
            clean[w++] = str[i];
        }
    }
    clean[w] = 0;

    // Agora sim converte corretamente
    for (int i = 0; i < w; i++) {
        if (clean[i] == '1') {
            val |= (1u << (31 - i));
        }
    }

    printf("Limpo: %s \t Int: %u\n", clean, val);
    return val;
}


// ============================================================================
// INICIALIZAÇÃO
// ============================================================================
bool comunicacao_iniciar() {
    printf("[SIMULADOR BIT-A-BIT] Pronto.\n");
    return true;
}

void comunicacao_encerrar() {
    printf("[SISTEMA] Encerrando.\n");
}

// ============================================================================
// RECEBIMENTO (STRING DE BITS -> DECODIFICAÇÃO -> LÓGICA)
// ============================================================================
int recebeDado(int sock) {
 
    /*
    printf("\n[ENTRADA] Cole os 32 bits: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return EVENTO_NENHUM;
    */
  
    /*
    // Limpeza básica da string
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) < 30) return EVENTO_NENHUM; // Ignora lixo curto
    */
  
    // 1. CONVERTE STRING DE BITS PARA INTEIRO 32 BITS REAL
    uint32_t pacote = receberDadosESP(sock);

    printf("\n=== ANALISANDO PACOTE 0x%08X ===\n", pacote);

    int pos = 0;

    // 2. LÊ HEADER (2 bits)
    int header = comm_ler_bits(pacote, &pos, 2);

    printf(" [31-30] Header: %d ", header);
    if (header == HEADER_DADOS_MAPA) printf("(DADOS DE MAPA)\n");
    else if (header == HEADER_REQ_ROTA) printf("(PEDIDO DE ROTA)\n");
    else { printf("(DESCONHECIDO)\n"); return EVENTO_NENHUM; }

    printf("Indo para separação\n");
                           
    // --- LÓGICA PARA DADOS DE MAPA (Header 2 / '10') ---
    if (header == HEADER_DADOS_MAPA) {
        // Protocolo: 10 [X 4b] [Y 4b] [H 1b] [Dir 4b] [Esq 4b] ...
        uint8_t x    = comm_ler_bits(pacote, &pos, 4);
        uint8_t y    = comm_ler_bits(pacote, &pos, 4);
        uint8_t hv   = comm_ler_bits(pacote, &pos, 1);
        uint8_t dDir = comm_ler_bits(pacote, &pos, 4);
        uint8_t dEsq = comm_ler_bits(pacote, &pos, 4);

        printf(" [29-26] Pos X : %d\n", x);
        printf(" [25-22] Pos Y : %d\n", y);
        printf(" [21-21] Orient: %d (%s)\n", hv, hv ? "Horizontal" : "Vertical");
        printf(" [20-17] D. Dir: %d blocos\n", dDir);
        printf(" [16-13] D. Esq: %d blocos\n", dEsq);
        printf("----------------------------------\n");

        // Atualiza Estado
        ultima_pos_x = x;
        ultima_pos_y = y;
        bool horizontal = (hv == 1);

        // Lógica Real
        mapa_atualizar(x, y, horizontal, dDir, dEsq);

        // Bufferiza resposta pro App
        qtd_updates = 0;
        bufferizar_update(x, y);
        int dirX = horizontal ? 1 : 0;
        int dirY = horizontal ? 0 : 1;

        for (int d = 1; d <= dDir; d++) bufferizar_update(x + d*dirX, y + d*dirY);
        bufferizar_update(x + (dDir*dirX) + dirX, y + (dDir*dirY) + dirY);

        for (int d = 1; d <= dEsq; d++) bufferizar_update(x - d*dirX, y - d*dirY);
        bufferizar_update(x - (dEsq*dirX) - dirX, y - (dEsq*dirY) - dirY);

        enviar_buffer_mapa_app_simulado(sock);
        return EVENTO_MAPA_ATUALIZADO;
    }

    // --- LÓGICA PARA PEDIDO DE ROTA (Header 0 / '00') ---
    else if (header == HEADER_REQ_ROTA) {
      
        printf("Separando nova rota\n");
      
        // Protocolo: 00 [X 4b] [Y 4b] ...
        
        printf("Pos: %d\n", pos);
        printf("Pacote %d\n", pacote);
      
        uint8_t x = comm_ler_bits(pacote, &pos, 4);
      
        printf("Batata 0\n");
        printf("Pos: %d \t Pacote %d\n", pos, pacote);
      
        uint8_t y = comm_ler_bits(pacote, &pos, 4);
      
        printf("Batata\n");
      
        printf("Pos: %d \t Pacote %d\n", pos, pacote);
      
        /*uint8_t verificacao = comm_ler_bits(pacote, &pos, 1);
      
        printf("Verificação: %d", verificacao);
      
        if(verificacao != 1){
           
          return EVENTO_NENHUM;
          
        }
*/
        printf(" [29-26] Origem X: %d\n", x);
        printf(" [25-22] Origem Y: %d\n", y);
        printf("----------------------------------\n");

        ultima_pos_x = x;
        ultima_pos_y = y;
      
        return EVENTO_PEDIDO_ROTA;
    }

    return EVENTO_NENHUM;
}

// ============================================================================
// SAÍDA BINÁRIA (RESPOSTAS)
// ============================================================================

void caminho(uint8_t* caminho_saida, int tamanho, int sock) {
    if (tamanho <= 0) return;

    printf("\n>>> RESPOSTA GERADA (CAMINHO) >>>\n");

    // 1. Tamanho
    uint32_t pct_tamanho = 0;
    int pos = 0;
    comm_escrever_bits(&pct_tamanho, &pos, tamanho, 8);
    enviarDadosESP(sock, u32_to_bin(pct_tamanho));

    printf("Pacote Tamanho (%d): ", tamanho);
    print_binary_string(pct_tamanho);

    // 2. Dados
    uint32_t buffer_dados = 0;
    int bits_ocupados = 0;
    pos = 0;

    for (int i = 0; i < tamanho; i++) {
        uint32_t val = caminho_saida[i] & 0x03;
        int shift = 32 - bits_ocupados - 2;
        buffer_dados |= (val << shift);
        bits_ocupados += 2;

        if (bits_ocupados == 32 || i == tamanho - 1) {
            printf("Pacote Passos:      ");
            print_binary_string(buffer_dados);
            enviarDadosESP(sock, u32_to_bin(buffer_dados));
            sleep(10); 
            buffer_dados = 0;
            bits_ocupados = 0;
        }
    }
    printf("<<< FIM RESPOSTA <<<\n");
}

void enviar_buffer_mapa_app_simulado(int sock) {
    if (qtd_updates == 0) return;

    printf("\n>>> RESPOSTA GERADA (ATUALIZAÇÃO APP) >>>\n");

    // Header + Qtd
    uint32_t pct_controle = 0;
    int pos = 0;
    comm_escrever_bits(&pct_controle, &pos, 1, 1);
    comm_escrever_bits(&pct_controle, &pos, qtd_updates, 8);

    printf("Pacote Controle (Qtd %d): ", qtd_updates);
    print_binary_string(pct_controle);
    enviarDadosESP(sock, u32_to_bin(pct_controle));

    printf("Preparando para enviar o mapa\n");
  
    // Dados
    uint32_t buffer_dados = 0;
    int bits_ocupados = 0;
    pos = 0;

    for (int i = 0; i < qtd_updates; i++) {
        MapUpdate u = buffer_updates[i];

        // Monta pacote manualmente para visualização
        int shift = 32 - bits_ocupados - 4;
        buffer_dados |= ((u.x & 0xF) << shift);
        bits_ocupados += 4;

        shift = 32 - bits_ocupados - 4;
        buffer_dados |= ((u.y & 0xF) << shift);
        bits_ocupados += 4;

        shift = 32 - bits_ocupados - 2;
        buffer_dados |= ((u.valor & 0x3) << shift);
        bits_ocupados += 2;

        if (bits_ocupados >= 30 || i == qtd_updates - 1) {
            printf("Pacote Dados Mapa:        ");
            print_binary_string(buffer_dados);
            enviarDadosESP(sock, u32_to_bin(buffer_dados));
            buffer_dados = 0;
            bits_ocupados = 0;
        }
    }
    printf("<<< FIM RESPOSTA <<<\n");
}

// ============================================================================
// AUXILIARES DE BITS
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
    // Se for pedir bits além do limite, trunque
    if (*pos_bit + nbits > 32)
        nbits = 32 - *pos_bit;

    int shift = 32 - (*pos_bit) - nbits;
    if (shift < 0) shift = 0;

    uint32_t mask = (nbits == 32) ? 0xFFFFFFFF : ((1u << nbits) - 1);

    uint32_t valor = (buffer >> shift) & mask;
    *pos_bit += nbits;

    return valor;
}


void bufferizar_update(uint8_t x, uint8_t y) {
    if (qtd_updates >= MAX_BUFFER_MAPA) return;
    int indice = idx(x, y);
    int valor_bruto = readMalha(indice);
    // Lógica de extração de valor (bits 8 e 9)
    const int TOTAL_BITS = 29;
    int inicio_fisico = (TOTAL_BITS - 1) - 9;
    int largura = 2;
    int valor_cell = (valor_bruto >> inicio_fisico) & ((1 << largura) - 1);
    buffer_updates[qtd_updates].x = x;
    buffer_updates[qtd_updates].y = y;
    buffer_updates[qtd_updates].valor = valor_cell;
    qtd_updates++;
}

uint8_t ler_pos_x() { return ultima_pos_x; }
uint8_t ler_pos_y() { return ultima_pos_y; }
bool sinal_nova_rota() { return false; }
bool sinal_mapear() { return true; }
                                   
void enviarDadosESP(int sock, const char *msg){
  
  sleep(1);
  
  send(sock, msg, strlen(msg), 0);

  printf("Enviado para o ESP: %s\n", msg);

  sleep(1);   

}

uint32_t receberDadosESP(int sock){
  
  char buffer[256];
  int len;
  
  len = recv(sock, buffer, sizeof(buffer) - 1, 0);

  if (len > 0) {
      buffer[len] = '\0';   // finaliza a string
      printf("Recebido do ESP32: %s\n", buffer);
      //enviarDadosESP(sock, "10101010101010101010101010101010");
  }
  else if (len == 0) {
      printf("Conexão fechada pelo ESP32\n");
  }
  else {
      perror("Erro no recv");
  }
          
  usleep(10000);
  
  return string_to_binary(buffer);

}
    

void calcular_e_enviar_rota( int sock) {
    uint8_t caminho_saida[TAMANHO_MALHA_TOTAL];
    result_t alvo;

    uint8_t cx = ler_pos_x();
    uint8_t cy = ler_pos_y();

    printf("CX: %d\n", cx);
    printf("CY: %d\n", cy);
  
  
    alvo = bfs_raio(cx, cy);

    if (!alvo.finished) {
        printf("[MAIN] Mapa completo.\n");
        return;
    }

    int passos = aStar_direct(cx, cy, alvo.destinoX, alvo.destinoY, caminho_saida);
  
    printf("Passos até o destino: %d\n", passos);
  
    if (passos > 0) {
        caminho(caminho_saida, passos, sock);
    } else {
        printf("[MAIN] Erro: Sem caminho.\n");
    }
}

const char *u32_to_bin(uint32_t value) {
    // String estática: 32 bits + '\0'
    static char buffer[33];
    buffer[32] = '\0';

    for (int i = 31; i >= 0; i--) {
        buffer[31 - i] = (value & (1u << i)) ? '1' : '0';
    }

    return buffer;
}



