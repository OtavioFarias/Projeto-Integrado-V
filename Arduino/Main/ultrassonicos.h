#ifndef ULTRASSONICOS_H
#define ULTRASSONICOS_H

#pragma once


#include <Arduino.h>

#define TRIG_FRENTE 52
#define ECHO_FRENTE 19   // com interrupção

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

float medirSensor();

float mediaUltrassonico(int echoPin, int n);

#endif
