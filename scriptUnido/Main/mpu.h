#ifndef MPU_H
#define MPU_H

#pragma once

#include <Wire.h>
#include <MPU6050.h>


extern MPU6050 mpu;
extern float anguloZ;
extern float anguloObjetivo; // graus
extern unsigned long ultimoTempoMPU;

void atualizarAnguloZ();

#endif