#include "toApp.h"
#include "toArduino.h"


void setup() {
  
  iniciarBluetooth(); 
  iniciarComunicacaoArduino(); 

}

void loop() {
  
  receberDadosApp();
  enviarKeepAlive();
  enviarAppTeste();

}

