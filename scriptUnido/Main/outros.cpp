#include "outros.h"

void resetMega() {
  asm volatile ("  jmp 0"); // pula para o endereço 0, reiniciando
}
