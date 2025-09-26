#ifndef ULTRASSONICOS_H
#define ULTRASSONICOS_H

#pragma once


#include <Arduino.h>

#define TRIG_FRENTE 52
#define TRIG_OUTROS 22
#define ECHO_FRENTE 19   // com interrupção
#define ECHO_DIREITA 28
#define ECHO_ESQUERDA 26
#define ECHO_TRAS 30

extern volatile unsigned long tempoInicio;
extern volatile unsigned long duracaoPulso;
extern volatile bool medidaPronta;

extern float distanciaFrente;
extern float distanciaDireita;
extern float distanciaEsquerda;
extern float distanciaTras;

extern float distanciaParaVirar;    // cm
extern int leiturasUltrassonico;    // média de leituras

void disparaSensor();

void capturaPulso();

bool temMedida();

float getDistancia();

float medirSensor(int echoPin);

float mediaUltrassonico(int echoPin, int n);

#endif
