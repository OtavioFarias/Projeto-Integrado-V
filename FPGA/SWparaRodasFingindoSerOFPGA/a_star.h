#ifndef A_STAR_H
#define A_STAR_H

#include <stdint.h>
#include <limits.h>

#include "mapa.h"
#include "acelerador.h"
#include "utilitarios.h"

/* ---------------- A* ---------------- */
int aStar_direct(/*Mapa *mapa,*/
                 int sx, int sy,
                 int dx_t, int dy_t,
                 uint8_t *saida);

#endif

