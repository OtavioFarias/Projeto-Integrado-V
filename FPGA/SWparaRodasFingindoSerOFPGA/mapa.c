
#include "mapa.h"

int idx(int x, int y) {

	//printf("Valor X: %d \t Valor Y: %d\t Resultado: %d \n", x, y, y * TAMANHO_MALHA + x);

    return y * TAMANHO_MALHA + x;
}


// Marca vizinhos desconhecidos como fronteira							 // 1=horizontal, 0=vertical
static inline void marcar_vizinhos_fronteira(/*Mapa *m, */int x, int y, bool direcaoHorizontal) {

	int x1 = x + 1;
	int x2 = x - 1;
	int y1 = y + 1;
	int y2 = y - 1;

	//printf("direcaoHorizontal: %d\n", direcaoHorizontal);

	//printf("Y1: %d\tY2:%d\n", y1, y2);

	switch(direcaoHorizontal) {

		case 0:

			//printf("Marcando Vertical \tx1: %d \tx2: %d\n", x1, x2);

			if ((unsigned)x1 < TAMANHO_MALHA && (unsigned)y < TAMANHO_MALHA) {

				int id = idx(x1, y);

				int posicaoAtual;

				posicaoAtual = readMalha(id);

				//traduzirValorMalha(posicaoAtual);

				if(separarBits(posicaoAtual, 8, 9) == 0) {
					posicaoAtual = escreverBits(posicaoAtual, 8, 9, 3);

					//printf("Salvando Valor na posi��o: %d\n", id);

					writeMalha(id, posicaoAtual);

				}
			}

			if ((unsigned)x2 >= 0 && (unsigned)y < TAMANHO_MALHA) {

				int id = idx(x2, y);

				int posicaoAtual;

				posicaoAtual = readMalha(id);

				//traduzirValorMalha(posicaoAtual);

				if(separarBits(posicaoAtual, 8, 9) == 0) {
					posicaoAtual = escreverBits(posicaoAtual, 8, 9, 3);

					//printf("Salvando Valor na posi��o: %d\n", id);

					writeMalha(id, posicaoAtual);

				}
			}


			break;


		case 1:


			//printf("Marcando Horizontal \ty1: %d \ty2: %d\n", y1, y2);

			if ((unsigned)x < TAMANHO_MALHA && (unsigned)y1 < TAMANHO_MALHA) {

				//printf("Marcando em cima\n");


				int id = idx(x, y1);

				int posicaoAtual;

				//printf("Entrei Aqui\n");

				posicaoAtual = readMalha(id);

				//traduzirValorMalha(posicaoAtual);

				if(separarBits(posicaoAtual, 8, 9) == 0) {
					posicaoAtual = escreverBits(posicaoAtual, 8, 9, 3);

					//printf("Salvando Valor na posi��o: %d\n", id);

					writeMalha(id, posicaoAtual);

				}
			}

			if ((unsigned)x < TAMANHO_MALHA && (unsigned)y2 >= 0) {

				//printf("Marcando em baixo\n");

				int id = idx(x, y2);

				int posicaoAtual;

				posicaoAtual = readMalha(id);

				//traduzirValorMalha(posicaoAtual);


				if(separarBits(posicaoAtual, 8, 9) == 0) {
					posicaoAtual = escreverBits(posicaoAtual, 8, 9, 3);

					//printf("Salvando Valor na posi��o: %d\n", id);

					writeMalha(id, posicaoAtual);

				}
			}

			break;

	}
}

/*
// Marca vizinhos desconhecidos como fronteira
static inline void marcar_vizinhos_fronteira(//Mapa *m, int x, int y) {
    static const int dx[4] = {1, -1, 0, 0};
    static const int dy[4] = {0, 0, 1, -1};

    int i;
    for (i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if ((unsigned)nx < TAMANHO_MALHA && (unsigned)ny < TAMANHO_MALHA) {
            int id = idx(nx, ny);
            //
            if (m->malha[id] == DESCONHECIDO)
                m->malha[id] = FRONTEIRA;
			//

            int posicaoAtual;

            if((posicaoAtual = readMalha(id)) == 0)
				escreverBits(posicaoAtual, 8, 9, 3);
				writeMalha(id, posicaoAtual);

        }
    }
}
*/


// Atualiza o mapa conforme leitura de sensores
void mapa_atualizar(/*Mapa *m,*/
                    uint8_t posX, uint8_t posY,
                    bool direcaoHorizontal,
                    uint8_t distDir, uint8_t distEsq) {

    // Marca posiÃ§Ã£o atual como livre e ajusta fronteira
    int base = idx(posX, posY);

    //m->malha[base] = LIVRE;
    int posicaoAtual = readMalha(base);

    //printf("Coordenadas desejadas - x: %d y: %d, base: %d\n", posX, posY, base);
    //printBits29(posicaoAtual);

    posicaoAtual = escreverBits(posicaoAtual, 8, 9, 1);

    //printBits29(posicaoAtual);

    //printf("Marcando- x: %d \t y: %d \t End: %d \n", posX, posY, base);

    writeMalha(base, posicaoAtual);

    //printf("Base: %d\n", base);
    //printf("Marcar Posi��o Atual como Livre: ");
    //debugPosicaoMalha(posicaoAtual);

    //marcar_vizinhos_fronteira(m, posX, posY);
    marcar_vizinhos_fronteira(posX, posY, direcaoHorizontal);

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
       // m->malha[id] = LIVRE;

        posicaoAtual = readMalha(id);
        posicaoAtual = escreverBits(posicaoAtual, 8, 9, 1);
        writeMalha(id, posicaoAtual);

        //printf("Marcando- x: %d \t y: %d \t End: %d \n", x, y, id);

        //marcar_vizinhos_fronteira(m, x, y);
        marcar_vizinhos_fronteira(x, y, direcaoHorizontal);
    }

    // Marca direÃ§Ã£o â€œesquerdaâ€�
    for (d = 1; d <= distEsq; d++) {
        int x = posX - d * dirX;
        int y = posY - d * dirY;
        if ((unsigned)x >= TAMANHO_MALHA || (unsigned)y >= TAMANHO_MALHA)
            break;
        int id = idx(x, y);
        //m->malha[id] = LIVRE;

        posicaoAtual = readMalha(id);
        posicaoAtual = escreverBits(posicaoAtual, 8, 9, 1);
        writeMalha(id, posicaoAtual);

        //printf("Marcando- x: %d \t y: %d \t End: %d \n", x, y, id);

        //marcar_vizinhos_fronteira(m, x, y);
        marcar_vizinhos_fronteira(x, y, direcaoHorizontal);

    }

    // Marca obstÃ¡culos nas extremidades
    int xDirFim = posX + distDir * dirX + dirX;
    int yDirFim = posY + distDir * dirY + dirY;
    int xEsqFim = posX - distEsq * dirX - dirX;
    int yEsqFim = posY - distEsq * dirY - dirY;

    int id_;

    if ((unsigned)xDirFim < TAMANHO_MALHA && (unsigned)yDirFim < TAMANHO_MALHA)
        //m->malha[idx(xDirFim, yDirFim)] = OCUPADO;
    	id_ = idx(xDirFim, yDirFim);
        posicaoAtual = readMalha(id_);
        posicaoAtual = escreverBits(posicaoAtual, 8, 9, 2);
        writeMalha(id_, posicaoAtual);


    if ((unsigned)xEsqFim < TAMANHO_MALHA && (unsigned)yEsqFim < TAMANHO_MALHA)
        //m->malha[idx(xEsqFim, yEsqFim)] = OCUPADO;
    	id_ = idx(xEsqFim, yEsqFim);
        posicaoAtual = readMalha(id_);
        posicaoAtual = escreverBits(posicaoAtual, 8, 9, 2);
        writeMalha(id_, posicaoAtual);

}

void mapa_print(){

	int i;
	for(i = 0; i < TAMANHO_MALHA_TOTAL; i++){

		if((i % TAMANHO_MALHA) == 0) printf("\n");
		//printf("Endere�o: %d \t %d\t\n", i, separarBits(readMalha(i), 8, 9));
		printf("%d\t", separarBits(readMalha(i), 8, 9));

	}

	printf("\n");

}

void mapa_print_coordenadas(){

	int i;
	for(i = 0; i < TAMANHO_MALHA_TOTAL; i++){

		printf("Endere�o: %d\n", i);
		//debugPosicaoMalha(readMalha(i));
		//debugSinaisMalha();
	}

}

void mapa_reset(){

	int x,y;
	int value;

	printf("\nReset\n");

	for(x = 0; x < TAMANHO_MALHA; x++){
		for(y = 0; y < TAMANHO_MALHA; y++){

			value = 0;
			value = escreverBits(value, 0, 3, x);
			value = escreverBits(value, 4, 7, y);

			writeMalha(idx(x,y), value);

		}
	}

}
