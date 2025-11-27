#ifndef TOFPGA_H
#define TOFPGA_H

#pragma once

#include <WiFi.h>
#include <Arduino.h>
#include "toArduino.h"
#include "others.h"

void iniciarWIFI();

void enviarDadosFPGA(String msg);

String receberDadosFPGA();

void comunicarFPGA(String mensagem);

void esperarCaminho();

void receberMapa(int tamanho);

void esperarFPGA();

void aceitarConexao();

#endif
