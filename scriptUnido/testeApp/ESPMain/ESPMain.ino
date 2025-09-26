#include "toApp.h"


void setup() {
  
  iniciarBluetooth();  

}

void loop() {
  
  receberDadosApp();
  enviarKeepAlive();
  enviarAppTeste();

}
