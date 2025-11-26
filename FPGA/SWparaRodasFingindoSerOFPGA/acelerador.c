#include "acelerador.h"

// Fila simples para simulação

#define MAX 8

static inline uint32_t heuristica(int x1, int y1, int x2, int y2) {
    int dxh = x1 - x2; if (dxh < 0) dxh = -dxh;
    int dyh = y1 - y2; if (dyh < 0) dyh = -dyh;
    return (uint32_t)(dxh + dyh);
}

int fila[MAX];
int fim = 0;   // próxima posição livre

// Verifica se está cheia
int cheia() {
    return fim == MAX;
}

// Verifica se está vazia
int vazia() {
    return fim == 0;
}

// Insere no final
void enqueue(int valor) {
    if (cheia()) {
        printf("Fila cheia!\n");
        return;
    }
    fila[fim] = valor;
    fim++;
}

// Remove do início
int dequeue() {
    if (vazia()) {
        printf("Fila vazia!\n");
        return -1;
    }

    int removido = fila[0];

    // desloca os elementos para a esquerda
    for (int i = 0; i < fim - 1; i++) {
        fila[i] = fila[i + 1];
    }

    fim--;
  
    return removido;
}
 
//
 

int malha[101];

void writeMalha(int address, int value){

  malha[address] = value;
  
}


int readMalha(int address){

	return malha[address];

}

int readFIFO(){

  int valor = dequeue();
  printf("Lendo Fila, valor %d\n", valor);
  if(valor == -1) return 255;
  return valor;
  
}

void requestAcelerador(int address, int value, int dx_t, int dy_t){

  int cur = address;

  // Converte índice linear de volta para coordenadas (x, y)

  int cx = cur / TAMANHO_MALHA;

  int cy = cur % TAMANHO_MALHA;

  uint32_t gcur = g_cost[cur]; // custo acumulado até o nó atual


  // Explora os 4 vizinhos (N, S, L, O)

  int d;
  for (d = 0; d < 4; d++) {
      int nx = cx + dx[d]; // coordenada x do vizinho

      int ny = cy + dy[d]; // coordenada y do vizinho



      // Ignora se o vizinho está fora dos limites do mapa
      if (nx < 0 || nx >= TAMANHO_MALHA || ny < 0 || ny >= TAMANHO_MALHA)

          continue;



      int nidx = idx(nx, ny); // índice linear do vizinho

      //uint8_t v = mapa->malha[nidx];         // valor da célula do mapa
      uint8_t v = readMalha(nidx);         // valor da célula do mapa


      // Ignora células desconhecidas ou ocupadas (obstáculos)

      if (v == FRONTEIRA || v == LIVRE)

          continue;



      // Ignora se o nó já foi fechado

      if (closed_flag[nidx])

          continue;



      // Custo temporário = custo atual + 1 (movimento simples)

      uint32_t tent_g = gcur + 1;



      // Se o nó ainda não está aberto OU encontramos um caminho melhor

      if (!open_flag[nidx] || tent_g < g_cost[nidx]) {

          parent_dir[nidx] = (uint8_t)d; // registra a direção do pai

          g_cost[nidx] = tent_g;         // atualiza custo g

          f_cost[nidx] = tent_g + heuristica(nx, ny, dx_t, dy_t); // custo total estimado

          int valor = nidx + 100000000*open_flag[nidx]; 
          
          printf("Open: %d\n", open_flag[nidx]);
          printf("Nidx: %d\n", nidx);
          printf("Valor: %d\n", valor);
        
          enqueue(valor);
        
      }
  }
}

