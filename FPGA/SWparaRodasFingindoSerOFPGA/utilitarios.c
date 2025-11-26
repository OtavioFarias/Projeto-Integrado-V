#include "utilitarios.h"

int separarBits(int n, int inicio, int fim) {
    const int TOTAL_BITS = 29;

    // Converte índices lógicos para índices físicos
    int inicio_fisico = (TOTAL_BITS - 1) - fim;
    int fim_fisico    = (TOTAL_BITS - 1) - inicio;

    int largura = fim_fisico - inicio_fisico + 1;

    int valor = (n >> inicio_fisico) & ((1 << largura) - 1);
    return valor;
}

int escreverBits(int n, int inicio, int fim, int v) {
    const int TOTAL_BITS = 29;

    // Converte índices lógicos (0 = MSB) para índices físicos (0 = LSB)
    int inicio_fisico = (TOTAL_BITS - 1) - fim;
    int fim_fisico    = (TOTAL_BITS - 1) - inicio;

    int largura = fim_fisico - inicio_fisico + 1;

    // 1. Zera a faixa de bits
    n &= ~(((1 << largura) - 1) << inicio_fisico);

    // 2. Escreve o novo valor
    n |= (v & ((1 << largura) - 1)) << inicio_fisico;

    return n;
}


void printBits29(int num){

	int i;

	for (i = 28; i >= 0; i--) {

		int bit = (num >> i) & 1;
		printf("%d", bit);

	}

	printf("\n");

}
