#ifndef TOARDUINO_H
#define TOARDUINO_H

#pragma once
#include <HardwareSerial.h>

void iniciarComunicacaoArduino();

void lerArduino();

void enviarArduinoTeste();

void enviarDadosVariaveis(String Mensagem);

#endif