#ifndef MOTORES_H
#define MOTORES_H

#pragma once

#include <cppQueue.h>

#include <AFMotor_R4.h>
#include "ultrassonicos.h"
#include "mpu.h"
#include "espComunicacao.h"
#include "outros.h"

enum Direcao {
  FRENTE = 0,
  ESQUERDA = 1,
  DIREITA = 2,
  TRAS = 3
};

extern const int pinoSensorHall;


extern cppQueue	filaDestino;

extern int velocidade;
extern float toleranciaErroRotacao;
extern float distanciaParaVirar;    // cm

extern int posicaoAtualX;
extern int posicaoAtualY;

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

void virarCoordenado(Direcao direcao);

void irParaCoordenada();

void mudarPosicaoAtual();

void andarQuadrado(Direcao direcao);

void ajustarDirecao(Direcao direcao);

void contarPulso();

void atualizarContadorHall();

Direcao ajustarRequisicao(Direcao direcao);

#endif

