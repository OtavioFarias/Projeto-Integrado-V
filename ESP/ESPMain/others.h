#ifndef OTHERS_H
#define OTHERS_H

#pragma once

#include "Arduino.h"

String intToBin(int valor);

int binToInt(String valor);

String separarBits(String value, int inicio, int fim);

String escreverBits(String value, int inicio, int fim, int valorParaEscrever);

#endif
