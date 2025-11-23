#ifndef MAPA_H
#define MAPA_H

#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>

#include "acelerador.h"
#include "utilitarios.h"

#define TAMANHO_MALHA 10
#define TAMANHO_MALHA_TOTAL TAMANHO_MALHA*TAMANHO_MALHA

#define DESCONHECIDO 0
#define LIVRE 1
#define OCUPADO 2
#define FRONTEIRA 3

int idx(int x, int y);

// Função para atualizar o mapa com novas leituras
void mapa_atualizar(/*Mapa *m, */
                    uint8_t posX, uint8_t posY,
                    bool direcaoHorizontal,  // 1=horizontal, 0=vertical
                    uint8_t distDir, uint8_t distEsq);

void mapa_print();

void mapa_print_coordenadas();

void mapa_reset();

#endif
