#ifndef DISTANCIA_H
#define DISTANCIA_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "mapa.h"

#define FRONTEIRA 3


typedef struct {
    uint16_t x;
    uint16_t y;
} pos_t;

typedef struct {
    bool finished;
    uint16_t destinoX;
    uint16_t destinoY;
} result_t;


// ------------------------------------------------------------
// Configurações e utilitários
// ------------------------------------------------------------


// ------------------------------------------------------------
// BFS com parada por camada (raio)
// ------------------------------------------------------------

result_t bfs_raio(Mapa* mapa, uint16_t startX, uint16_t startY);

#endif
