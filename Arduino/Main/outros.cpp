#include "outros.h"

int debug = 0;

void resetMega() {
  //asm volatile ("  jmp 0"); // pula para o endereço 0, reiniciando
}


void testarVoltagemTx2(){

  pinMode(16, OUTPUT);    
  digitalWrite(16, HIGH);    

}