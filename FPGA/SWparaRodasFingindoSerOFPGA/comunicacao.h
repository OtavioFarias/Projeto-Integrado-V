#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include <stdint.h>
#include <stdbool.h>
#include "mapa.h"
#include "acelerador.h"
#include "a_star.h"
#include "distancia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


// Eventos
#define EVENTO_NENHUM          0
#define EVENTO_MAPA_ATUALIZADO 1
#define EVENTO_PEDIDO_ROTA     2

// Protocolo Binário
#define HEADER_REQ_ROTA       3 // 11
#define HEADER_DADOS_MAPA     2 // 10

#define MAX_BUFFER_MAPA 50

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t valor;
} MapUpdate;


#define ESP_IP "192.168.4.1"
#define PORT   5000

void enviarDadosESP(int sock, const char *msg);
uint32_t receberDadosESP(int sock);


// Funções
bool comunicacao_iniciar();
void comunicacao_encerrar();
int recebeDado(int sock);
void caminho(uint8_t* caminho_saida, int tamanho, int sock);
uint8_t ler_pos_x();
uint8_t ler_pos_y();
bool sinal_nova_rota();
bool sinal_mapear();

void calcular_e_enviar_rota(int sock);
const char *u32_to_bin(uint32_t value);

#endif
