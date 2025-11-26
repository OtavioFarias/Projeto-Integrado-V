#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

#include "mapa.h"
#include "acelerador.h"

// ============================================================================
// CONFIGURAÇÕES DE REDE (ATUALIZADO PARA O ESP32 REAL)
// ============================================================================
// O IP padrão do ESP32 em modo SoftAP (Access Point) é 192.168.4.1
// Se você estiver conectando o ESP32 no Roteador da sua casa, verifique o IP no Serial Monitor
#define IP_SERVIDOR "192.168.4.1"
#define PORTA_SERVIDOR 5000

// ----------------------------------------------------------------------------
// CABEÇALHOS DE RECEBIMENTO (2 BITS) - VINDOS DO ESP
// ----------------------------------------------------------------------------
#define HEADER_REQ_ROTA       0 // 00: Pedido de Rota
#define HEADER_DADOS_MAPA     1 // 01: Dados de Mapa
#define HEADER_HANDSHAKE_REQ  2 // 10: Pedido de Permissão (Handshake)

// ----------------------------------------------------------------------------
// RESPOSTAS DO PC PARA O HANDSHAKE DO ESP
// ----------------------------------------------------------------------------
#define RESP_PC_LIVRE    2 // PC diz: "Pode mandar"
#define RESP_PC_OCUPADO  3 // PC diz: "Estou ocupado, tente depois"

// Configurações Gerais
#define MAX_BUFFER_MAPA 50
#define TIMEOUT_HANDSHAKE_SEC 2

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t valor;
} MapUpdate;

// Funções de Conexão
int iniciar_conexao_socket();

// Funções de Protocolo
int aguardar_esp_livre(int sockfd);

// Envio
void enviar_caminho_arduino(int sockfd, uint8_t* caminho_saida, int tamanho);
void enviar_buffer_mapa_app(int sockfd);

// Recepção
void recebeDado(int sockfd);

// Controle e Leitura de Estado
bool sinal_nova_rota();
bool sinal_mapear();

// Funções para ler a última posição recebida
uint8_t ler_pos_x();
uint8_t ler_pos_y();

// Wrapper
void caminho(uint8_t* caminho_saida, int tamanho);

#endif
