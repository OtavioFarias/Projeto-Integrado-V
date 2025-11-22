#ifndef ULTRASSONICOS_H
#define ULTRASSONICOS_H

#pragma once


#include <Arduino.h>
#include "outros.h"
#include "espComunicacao.h"

#define TRIG_FRENTE 49
#define ECHO_FRENTE 53   


#define TRIG_ESQUERDA 31
#define ECHO_ESQUERDA 33 


#define TRIG_DIREITA 36
#define ECHO_DIREITA 34   

extern volatile unsigned long tempoInicio;
extern volatile unsigned long duracaoPulso;
extern volatile bool medidaPronta;

extern float distanciaFrente;
extern float distanciaDireita;
extern float distanciaEsquerda;

extern int leiturasUltrassonico;    // média de leituras

void disparaSensor();

void capturaPulso();

bool temMedida();

float getDistancia();

float medirSensor(int direcao);

float mediaUltrassonico(int n,  int direcao);

float chamaMedirSensor();

#endif
