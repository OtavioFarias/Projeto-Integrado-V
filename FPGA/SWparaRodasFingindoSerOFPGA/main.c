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

  printf("=== Sistema de Mapeamento ===\n");

  uint8_t caminho_saida[TAMANHO_MALHA_TOTAL];
  
  printf("Iniciando Comunicação com o ESP\n");
  
  int socket = iniciar_conexao_socket();
  
  while (1) {

    recebeDado(socket);
      
  }
  
    return 0;
  
}
