#include "comunicacao.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

// Inicializa RNG apenas uma vez
static bool rng_inicializado = false;

uint8_t ler_sensor_dir() {
    if (!rng_inicializado) {
        srand(time(NULL));
        rng_inicializado = true;
    }
    // Retorna valor de 0 a 10 (simulando distância em cm, por exemplo)
    return rand() % 11;
}

uint8_t ler_sensor_esq() {
    if (!rng_inicializado) {
        srand(time(NULL));
        rng_inicializado = true;
    }
    // Retorna valor de 0 a 10
    return rand() % 11;
}

uint8_t ler_pos_x() {
    if (!rng_inicializado) {
        srand(time(NULL));
        rng_inicializado = true;
    }
    // Simula posição X na malha de teste (0 a 9)
    return rand() % 10;
}

uint8_t ler_pos_y() {
    if (!rng_inicializado) {
        srand(time(NULL));
        rng_inicializado = true;
    }
    // Simula posição Y na malha de teste (0 a 9)
    return rand() % 10;
}

bool sinal_nova_rota() {
    // Sempre retorna true para testar cálculo de rota
    return true;
}

bool sinal_mapear() {
    // Sempre retorna true para simular que sensor está pronto
    return true;
}

void caminho(uint8_t* caminho_saida, int tamanho) {
	int i = 0;

    // Apenas para debug
    printf("Caminho gerado para teste: ");
    for (i = 0; i < tamanho; i++) {
        printf("%u ", caminho_saida[i]);
    }
    printf("\n");
}

/*
void receberMapa(Mapa* mapa) {
    // Preenche mapa de teste com valores aleatórios
	int y = 0;
	int x = 0;
    for (y = 0; y < TAMANHO_MALHA_TOTAL; y++) {
        for (x = 0; x < TAMANHO_MALHA_TOTAL; x++) {

        }
    }
    printf("[DEBUG] Mapa de teste gerado.\n");
}
*/
