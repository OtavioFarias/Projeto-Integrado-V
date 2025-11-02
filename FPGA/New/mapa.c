#include "mapa.h"

static inline int idx(int x, int y) {
    return y * TAMANHO_MALHA + x;
}

// FunÃ§Ã£o inline de marcaÃ§Ã£o segura (sem if aninhado)
static inline void marcar(Mapa *m, int x, int y, uint8_t valor) {
    if ((unsigned)x < TAMANHO_MALHA && (unsigned)y < TAMANHO_MALHA)
        m->malha[idx(x, y)] = valor;
}

static inline bool in_bounds(int x, int y) {
    return ((unsigned)x < TAMANHO_MALHA && (unsigned)y < TAMANHO_MALHA);
}

// Marca vizinhos desconhecidos como fronteira
static inline void marcar_vizinhos_fronteira(Mapa *m, int x, int y) {
    static const int dx[4] = {1, -1, 0, 0};
    static const int dy[4] = {0, 0, 1, -1};

    int i;
    for (i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if ((unsigned)nx < TAMANHO_MALHA && (unsigned)ny < TAMANHO_MALHA) {
            int id = idx(nx, ny);
            if (m->malha[id] == DESCONHECIDO)
                m->malha[id] = FRONTEIRA;
        }
    }
}

// Reset total do mapa
void mapa_reset(Mapa *m) {

	int i;
    for (i = 0; i < TAMANHO_MALHA_TOTAL; i++)
        m->malha[i] = DESCONHECIDO;

}

// Atualiza o mapa conforme leitura de sensores
void mapa_atualizar(Mapa *m,
                    uint8_t posX, uint8_t posY,
                    bool direcaoHorizontal,
                    uint8_t distDir, uint8_t distEsq) {

    // Marca posiÃ§Ã£o atual como livre e ajusta fronteira
    int base = idx(posX, posY);
    m->malha[base] = LIVRE;
    marcar_vizinhos_fronteira(m, posX, posY);

    // Direita e esquerda (horizontal ou vertical)
    int dirX = direcaoHorizontal ? 1 : 0;
    int dirY = direcaoHorizontal ? 0 : 1;

    // Marca direÃ§Ã£o â€œdireitaâ€�
    int d;
    for (d = 1; d <= distDir; d++) {
        int x = posX + d * dirX;
        int y = posY + d * dirY;
        if ((unsigned)x >= TAMANHO_MALHA || (unsigned)y >= TAMANHO_MALHA)
            break;
        int id = idx(x, y);
        m->malha[id] = LIVRE;
        marcar_vizinhos_fronteira(m, x, y);
    }

    // Marca direÃ§Ã£o â€œesquerdaâ€�
    for (d = 1; d <= distEsq; d++) {
        int x = posX - d * dirX;
        int y = posY - d * dirY;
        if ((unsigned)x >= TAMANHO_MALHA || (unsigned)y >= TAMANHO_MALHA)
            break;
        int id = idx(x, y);
        m->malha[id] = LIVRE;
        marcar_vizinhos_fronteira(m, x, y);
    }

    // Marca obstÃ¡culos nas extremidades
    int xDirFim = posX + distDir * dirX + dirX;
    int yDirFim = posY + distDir * dirY + dirY;
    int xEsqFim = posX - distEsq * dirX - dirX;
    int yEsqFim = posY - distEsq * dirY - dirY;

    if ((unsigned)xDirFim < TAMANHO_MALHA && (unsigned)yDirFim < TAMANHO_MALHA)
        m->malha[idx(xDirFim, yDirFim)] = OCUPADO;

    if ((unsigned)xEsqFim < TAMANHO_MALHA && (unsigned)yEsqFim < TAMANHO_MALHA)
        m->malha[idx(xEsqFim, yEsqFim)] = OCUPADO;

}
/*
void mapa_print(const Mapa *m) {
	int y,x;
    for (y = 0; y < TAMANHO_MALHA; y++) {
        for (x = 0; x < TAMANHO_MALHA; x++)
            printf("%d ", m->malha[idx(x, y)]);
        printf("\n");
    }
}
*/

