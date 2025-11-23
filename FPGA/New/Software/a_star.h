#ifndef A_STAR_H
#define A_STAR_H

#include <stdint.h>
#include <limits.h>

#include "mapa.h"

#define INF32 0x7FFFFFFF
#define IDX(x, y, cols) ((x) * (cols) + (y))

/* ---------------- A* ---------------- */
int aStar_direct(Mapa *mapa,
                 int sx, int sy,
                 int dx_t, int dy_t,
                 uint8_t *saida);

#endif


