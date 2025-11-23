#include "distancia.h"
// ------------------------------------------------------------
// ConfiguraÃ§Ãµes e utilitÃ¡rios
// ------------------------------------------------------------

// ------------------------------------------------------------
// BFS com parada por camada (raio)
// ------------------------------------------------------------

result_t bfs_raio(/*Mapa* mapa, */uint16_t startX, uint16_t startY)
{
    // Vetores estÃ¡ticos para economia de stack
    static bool visited[TAMANHO_MALHA_TOTAL];
    static pos_t queue[TAMANHO_MALHA_TOTAL];

    uint16_t front = 0, back = 0;
    result_t result = { .finished = false, .destinoX = 0, .destinoY = 0 };

    // Reset visitados
    int i;

    for (i = 0; i < TAMANHO_MALHA_TOTAL; i++)
        visited[i] = false;

    // DireÃ§Ãµes (N, S, L, O)
    const int8_t dx[4] = {  0,  0,  1, -1 };
    const int8_t dy[4] = {  1, -1,  0,  0 };

    // Ã�ndice inicial
    const int startIdx = startY * TAMANHO_MALHA + startX;
    queue[back++] = (pos_t){ startX, startY };
    visited[startIdx] = true;

    bool found_in_this_layer = false;
    uint16_t bestX = 0, bestY = 0;
    uint32_t bestDist = 0xFFFF;

    while (front != back && !result.finished) {
        uint16_t layer_size = back - front;
        found_in_this_layer = false;

        // Processa uma camada de BFS (â€œraioâ€�)
        uint16_t i;
        for (i = 0; i < layer_size; i++) {
            pos_t curr = queue[front++];

            int dir;
            for (dir = 0; dir < 4; dir++) {
                int nx = curr.x + dx[dir];
                int ny = curr.y + dy[dir];

                // Limites
                if ((unsigned)nx >= TAMANHO_MALHA || (unsigned)ny >= TAMANHO_MALHA)
                    continue;

                int nidx = ny * TAMANHO_MALHA + nx;
                if (visited[nidx]) continue;

                visited[nidx] = true;

                // Verifica cÃ©lula alvo (exemplo: fronteira)
                //if (mapa->malha[nidx] == FRONTEIRA) {
                if(separarBits(readMalha(nidx), 8, 9) == 3){
                    uint32_t dist = (nx > startX ? nx - startX : startX - nx)
                                  + (ny > startY ? ny - startY : startY - ny);
                    if (!found_in_this_layer || dist < bestDist ||
                        (dist == bestDist && (ny < bestY || (ny == bestY && nx < bestX)))) {
                        bestDist = dist;
                        bestX = nx;
                        bestY = ny;
                        found_in_this_layer = true;
                    }
                }

                // Adiciona Ã  fila
                queue[back++] = (pos_t){ nx, ny };
            }
        }

        // Se achou cÃ©lula na camada atual, termina
        if (found_in_this_layer) {
            result.finished = true;
            result.destinoX = bestX;
            result.destinoY = bestY;
        }
    }

    return result;

}

