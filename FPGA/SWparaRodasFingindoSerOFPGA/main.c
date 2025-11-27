#include "distancia.h"
#include "mapa.h"
#include "a_star.h"
#include "comunicacao.h"
#include "acelerador.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

int main(void)
{
    mapa_reset();
    printf("================================================\n");
    printf("   SIMULADOR FPGA (ENTRADA DE BITS REAIS)       \n");
    printf("================================================\n");
    comunicacao_iniciar();
  
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, ESP_IP, &addr.sin_addr);

    printf("Conectando ao ESP32...\n");

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Erro ao conectar");
        return 1;
    }

    printf("Conectado ao ESP32!\n");

    while (1) {
        int evento = recebeDado(sock);

        if (evento == EVENTO_PEDIDO_ROTA) {
            calcular_e_enviar_rota(sock);
        }
        else if (evento == EVENTO_MAPA_ATUALIZADO) {
            printf("[MAIN] Mapa Atualizado. Visualização:\n");
            mapa_print();
        }

        usleep(50000);
    }

    comunicacao_encerrar();
    return 0;
}

