#ifndef TOARDUINO_H
#define TOARDUINO_H

#pragma once
#include <HardwareSerial.h>

extern HardwareSerial SerialESP(2);  // UART2

void iniciarComunicacaoArduino();

void lerArduino();

void enviarArduinoTeste();

void enviarDadosVariaveis();

#endif