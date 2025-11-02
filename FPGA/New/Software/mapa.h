#ifndef MAPA_H
#define MAPA_H

#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>

#define TAMANHO_MALHA 1
#define TAMANHO_MALHA_TOTAL TAMANHO_MALHA*TAMANHO_MALHA

#define DESCONHECIDO 0
#define LIVRE 1
#define OCUPADO 2
#define FRONTEIRA 3

typedef struct {
    uint8_t malha[TAMANHO_MALHA_TOTAL];
} Mapa;

// Função de reset
void mapa_reset(Mapa *m);

// Função para atualizar o mapa com novas leituras
void mapa_atualizar(Mapa *m,
                    uint8_t posX, uint8_t posY,
                    bool direcaoHorizontal,  // 1=horizontal, 0=vertical
                    uint8_t distDir, uint8_t distEsq);

// Função para imprimir o mapa (debug)
void mapa_print(const Mapa *m);

#endif
