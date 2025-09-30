#ifndef ULTRASSONICOS_H
#define ULTRASSONICOS_H

#pragma once


#include <Arduino.h>

#define TRIG_FRENTE 52
#define TRIG_DIREITA 32
#define TRIG_ESQUERDA 27
#define TRIG_TRAS 31
#define ECHO_FRENTE 19   // com interrupção
#define ECHO_DIREITA 33
#define ECHO_ESQUERDA 26
#define ECHO_TRAS 30

extern volatile unsigned long tempoInicio;
extern volatile unsigned long duracaoPulso;
extern volatile bool medidaPronta;

extern float distanciaFrente;
extern float distanciaDireita;
extern float distanciaEsquerda;
extern float distanciaTras;

extern int leiturasUltrassonico;    // média de leituras

void disparaSensor();

void capturaPulso();

bool temMedida();

float getDistancia();

float medirSensor(int echoPin);

float mediaUltrassonico(int echoPin, int n);

#endif
