#ifndef ULTRASSONICOS_H
#define ULTRASSONICOS_H

#pragma once


#include <Arduino.h>

#define TRIG_FRENTE 52
#define ECHO_FRENTE 19   // com interrupção


#define TRIG_ESQUERDA 52
#define ECHO_ESQUERDA 19   // com interrupção


#define TRIG_DIREITA 52
#define ECHO_DIREITA 19   // com interrupção

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
