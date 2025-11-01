#ifndef TOAPP_H
#define TOAPP_H

#pragma once
#include "BluetoothSerial.h"
#include "toArduino.h"
#include "esp_bt_device.h"

void iniciarBluetooth();

void receberDadosApp();

void enviarAppTeste();

void enviarKeepAlive();

void iniciarBluetoothV2();

void receberDadosAppV2();

#endif