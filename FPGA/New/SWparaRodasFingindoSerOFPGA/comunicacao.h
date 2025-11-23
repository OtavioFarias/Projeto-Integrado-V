#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include <stdint.h>
#include <stdbool.h>

#include "mapa.h"

#define NOVA_ROTA_SIGNAL 1 // sinal recebido de algum registrador de controle, por ex.

#define SENSOR_ADDR       0x00000100 // endereços hipotéticos
#define POSICAO_ADDR      0x00000200
#define COMANDO_ADDR      0x00000300

// ------------------------------------------------------------
// Simulações de leitura (substituir por leitura de registradores no Nios II)
// ------------------------------------------------------------
uint8_t ler_sensor_dir();
uint8_t ler_sensor_esq();
uint8_t ler_pos_x();
uint8_t ler_pos_y();
bool sinal_nova_rota();
bool sinal_mapear();
void caminho(uint8_t* caminho_saida, int tamanho);//função recebe novo caminho para enviar ao arduino
//void receberMapa(Mapa* mapa); //função para passar mapa para o App

#endif

