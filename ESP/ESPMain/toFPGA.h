#ifndef TOFPGA_H
#define TOFPGA_H

#pragma once

#include <WiFi.h>
#include <Arduino.h>
#include "toArduino.h"

void iniciarWIFI();

void enviarDadosFPGA(String msg);

String receberDadosFPGA();

void comunicarFPGA(String mensagem);

void esperarCaminho();

void receberMapa();

#endif
