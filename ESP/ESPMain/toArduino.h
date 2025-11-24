#ifndef TOARDUINO_H
#define TOARDUINO_H

#pragma once
#include <HardwareSerial.h>
#include "toApp.h"
#include "toFPGA.h"

void iniciarComunicacaoArduino();

void lerArduino();

void enviarArduinoTeste();

void enviarDadosVariaveis(String msg);

void enviarDadosAppV2(String msg);

#endif