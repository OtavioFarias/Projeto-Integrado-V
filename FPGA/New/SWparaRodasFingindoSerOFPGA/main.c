#include "distancia.h"
#include "mapa.h"
#include "a_star.h"
#include "comunicacao.h"
#include "acelerador.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

//#include "altera_avalon_performance_counter.h"
//#include "system.h"

int main(void)
{


    mapa_reset();


    //mapa_reset();

    printf("=== Sistema de Mapeamento ===\n");

    //Mapa mapa;

    //mapa_reset(&mapa);

    uint8_t posX = 5, posY = 5;
    uint8_t distDir = 2, distEsq = 2;
    bool direcaoHorizontal = true;

    result_t resultadoBusca;
    uint8_t caminho_saida[TAMANHO_MALHA_TOTAL];

    //debug
    int i = 0;
    int numeroDeRepeticoes = 5;

    //traduzirValorMalha(readMalha(91));

    while (1) {

        // Inicializa medição de performance
    	// PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
    	// PERF_START_MEASURING(PERFORMANCE_COUNTER_0_BASE);

        // 1. Ler posição atual e sensores
        while(!sinal_mapear());

        /*
        posX = ler_pos_x();
        posY = ler_pos_y();
        distDir = ler_sensor_dir();
        distEsq = ler_sensor_esq();
        */

        //printf("Posição X: %d \tPosição Y: %d \tDistância Direita: %d\t Distância Esquerda: %d\n", posX, posY, distDir, distEsq);


        // 2. Atualizar mapa com as novas leituras
        // PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 1); // Seção 1 = mapa_atualizar
        //mapa_atualizar(&mapa, posX, posY, direcaoHorizontal, distDir, distEsq);

        mapa_atualizar(posX, posY, direcaoHorizontal, distDir, distEsq);
        //mapa_atualizar(posX, posY - 1, direcaoHorizontal, distDir, distEsq);
        //mapa_atualizar(posX, posY + 1, direcaoHorizontal, distDir, distEsq);

        //mapa_atualizar(5, 5, 0, 2, 2);

        mapa_print();

        //PERF_END(PERFORMANCE_COUNTER_0_BASE, 1);

        //mapa_print(&mapa);


        // 3. Verifica se foi solicitado novo destino
        if (sinal_nova_rota()) {
            //printf("[INFO] Solicitada nova rota. Buscando alvo...\n");

            // 3.1 Primeiro, encontra célula alvo mais próxima (via BFS)
        //    PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 2); // Seção 2 = bfs_raio
            //resultadoBusca = bfs_raio(&mapa, posX, posY);
            resultadoBusca = bfs_raio(posX, posY);

        //    PERF_END(PERFORMANCE_COUNTER_0_BASE, 2);

            //mapa_print(&mapa);

            if (!resultadoBusca.finished) {
                //printf("[AVISO] Nenhum alvo encontrado.\n");
            } else {
                printf("[INFO] Alvo encontrado em (%u, %u)\n",
                        resultadoBusca.destinoX, resultadoBusca.destinoY);

                // 3.2 Executa A* para achar menor caminho até o alvo
           //     PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 3); // Seção 3 = aStar_direct



               /* int caminho_len = aStar_direct(&mapa,
                                               posX, posY,
                                               resultadoBusca.destinoX, resultadoBusca.destinoY,
                                               caminho_saida);

				*/

                //printf("Enviando para o A* x: %d\t y:%d\n", posX, posY);

					printf("===============\n");
					printf("Chamando A*\n");
					printf("Origem \t x: %d \t y: %d \n", posX, posY);
					printf("Destino \t x: %d \t y: %d \n", resultadoBusca.destinoX, resultadoBusca.destinoY);
					printf("===============\n");

					int caminhoLen = aStar_direct(
												  posX, posY,
												  resultadoBusca.destinoX, resultadoBusca.destinoY,
												  caminho_saida
												  );


				   //printf("Caminho Encontrado, quantidade de passos: %d\n", caminhoLen);


              //  PERF_END(PERFORMANCE_COUNTER_0_BASE, 3);

                if (caminhoLen > 0) {
                    // 3.3 Executa a função caminho
                    caminho(caminho_saida, caminhoLen);

                } else {
                    //printf("[ERRO] Falha ao calcular caminho.\n");
                }
            }
        }

        // Receber mapa atualizado via comunicação
          //receberMapa(&mapa);


       // PERF_STOP_MEASURING(PERFORMANCE_COUNTER_0_BASE);
/*
	// Imprime relatório de performance
        perf_print_formatted_report((void*)PERFORMANCE_COUNTER_0_BASE,
                                    ALT_CPU_FREQ, 3,
                                    "Mapa Atualizar",
                                    "BFS",
                                    "A*");

        */

        //debug
        i++;
        if(i == numeroDeRepeticoes) break;

        //break; // só para teste
    }


    return 0;
}
