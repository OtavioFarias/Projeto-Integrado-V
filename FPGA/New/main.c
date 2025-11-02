#include "distancia.h"
#include "mapa.h"
#include "a_star.h"
#include "comunicacao.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
//#include "io.h"


int main(void)
{
    printf("=== Sistema de Mapeamento e Navegação (NIOS II) ===\n");

    Mapa mapa;

    mapa_reset(&mapa);

    uint8_t posX = 0, posY = 0;
    uint8_t distDir = 0, distEsq = 0;
    bool direcaoHorizontal = true;

    result_t resultadoBusca;
    uint8_t caminho_saida[TAMANHO_MALHA_TOTAL];

    while (1) {

        // 1. Ler posição atual e sensores
        while(!sinal_mapear());
        posX = ler_pos_x();
        posY = ler_pos_y();
        distDir = ler_sensor_dir();
        distEsq = ler_sensor_esq();

        // 2. Atualizar mapa com as novas leituras
        mapa_atualizar(&mapa, posX, posY, direcaoHorizontal, distDir, distEsq);

        // 3. Verifica se foi solicitado novo destino
        if (sinal_nova_rota()) {
            printf("[INFO] Solicitada nova rota. Buscando alvo...\n");

            // 3.1 Primeiro, encontra célula alvo mais próxima (via BFS)
            resultadoBusca = bfs_raio(&mapa, posX, posY);

            if (!resultadoBusca.finished) {
                printf("[AVISO] Nenhum alvo encontrado.\n");
            } else {
                printf("[INFO] Alvo encontrado em (%u, %u)\n", resultadoBusca.destinoX, resultadoBusca.destinoY);

                // 3.2 Executa A* para achar menor caminho até o alvo
                int caminho_len = aStar_direct(&mapa,
                                               posX, posY,
                                               resultadoBusca.destinoX, resultadoBusca.destinoY,
                                               &caminho_saida);

                if (caminho_len > 0) {
                    printf("[SUCESSO] Caminho calculado com %d passos.\n", caminho_len);
                    caminho(caminho_saida);
                } else {
                    printf("[ERRO] Falha ao calcular caminho.\n");
                }
            }


        }

        receberMapa(&mapa);
        break;
    }
    return 0;
}
