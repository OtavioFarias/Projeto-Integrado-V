#ifndef TOAPP_H
#define TOAPP_H

#pragma once
#include <map>
#include <BluetoothSerial.h>
void iniciarBluetooth();

void receberDadosApp();

void enviarAppTeste();

void enviarKeepAlive();

#endif