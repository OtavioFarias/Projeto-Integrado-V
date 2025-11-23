
#ifndef ACELERADOR_H
#define ACELERADOR_H

//#include "system.h"
#include "utilitarios.h"
#include "mapa.h"

#include <stdio.h>

static uint32_t g_cost[100];
static uint32_t f_cost[100];
static uint8_t parent_dir[100];
static uint8_t open_flag[100];
static uint8_t closed_flag[100];

static const int dx[4] = {-1, 1, 0, 0};
static const int dy[4] = {0, 0, -1, 1};

void writeMalha(int address, int value);

int readMalha(int address);

int readFIFO();

void requestAcelerador(int address, int value, int dx_t, int dy_t);


void debugSinaisMalha();

void traduzirValorMalha(int value);

void debugPosicaoMalha(int value);

void debugFIFO();

void myClock();
void debugVizinhos();

#endif


