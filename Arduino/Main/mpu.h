#ifndef MPU_H
#define MPU_H

#pragma once

#include <Wire.h>
#include <MPU6050.h>
#include "outros.h"
#include "espComunicacao.h"

extern MPU6050 mpu;
extern float anguloZ;
extern float anguloObjetivo; // graus
extern unsigned long ultimoTempoMPU;

void atualizarAnguloZ();
float erroDeRotacao();
void atualizarAnguloZ_ComFiltro();
void calibrarGyroZ();
#endif