#ifndef TOFPGA_H
#define TOFPGA_H

#pragma once

#include <WiFi.h>
#include <Arduino.h>

void iniciarWIFI();

void enviarDadosFPGA(String msg);

void receberDadosFPGA();

#endif
