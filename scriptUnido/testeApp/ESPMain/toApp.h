#ifndef TOAPP_H
#define TOAPP_H

#pragma once
#include "BluetoothSerial.h"

void iniciarBluetooth();

void receberDadosApp();

void enviarAppTeste();

void enviarKeepAlive();

void atualizarLEDConexao();

#endif