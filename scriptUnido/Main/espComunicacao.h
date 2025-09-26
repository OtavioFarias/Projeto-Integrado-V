#ifndef ESPCOMUNICACAO_H
#define ESPCOMUNICACAO_H

#include <Arduino.h>
#include "motores.h"
#include "mpu.h"
#include "outros.h"
#include "ultrassonicos.h"


extern String buffer; // Armazena dados recebidos

void enviarDadosUltrassonicoESP();

void inciarComunicacaoESP();

void enviarDadosESP();

void receberDadosESP();

// Função para processar a mensagem recebida
void processarMensagem(String msg);

void andarESP();

#endif