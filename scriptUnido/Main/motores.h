#ifndef MOTORES_H
#define MOTORES_H

#pragma once

#include <AFMotor_R4.h>
#include "ultrassonicos.h"

extern int velocidade;

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


#endif

