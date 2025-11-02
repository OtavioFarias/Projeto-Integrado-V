#include "comunicacao.h"

uint8_t ler_sensor_dir(){

	return 0;

};
uint8_t ler_sensor_esq(){

	return 0;

};
uint8_t ler_pos_x(){

	return 0;

};

uint8_t ler_pos_y(){

	return 0;

};

bool sinal_nova_rota(){

	return true;

};

bool sinal_mapear(){

	return true;

};
void caminho(uint8_t* caminho_saida){};//função recebe novo caminho para enviar ao arduino
void receberMapa(Mapa* mapa){}; //função para passar mapa para o App
