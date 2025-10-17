#ifndef MOTORES_H
#define MOTORES_H

#pragma once

#include <AFMotor_R4.h>
#include "ultrassonicos.h"
#include "mpu.h"
#include "espComunicacao.h"

extern toleranciaErroRotacao
extern int velocidade;
extern float distanciaParaVirar;    // cm

void iniciarMotores();

void frente();

void parar();

void tras();

void esquerda();

void direita();

void testeMotor1();

void testeMotor2();

void testeMotor3();

void testeMotor4();

void girarAngulo();

void andarAutomatico();

void passoFrente(int duracao);

void passoTras(int duracao);

void passoDireita(int duracao); 

void passoEsquerda(int duracao);

void virarCoordenado();

int direcaoIndicadaFPGA();


#endif

