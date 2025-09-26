#ifndef TOAPP_H
#define TOAPP_H

#pragma once
#include "BluetoothSerial.h"

extern BluetoothSerial SerialBT;


void iniciarBluetooth();

void receberDadosApp();

void enviarAppTeste();

void enviarKeepAlive();

#endif