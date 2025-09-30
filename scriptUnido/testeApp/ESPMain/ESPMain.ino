#include "toApp.h"
#include "toArduino.h"


void setup() {
  
  iniciarBluetooth(); 
  iniciarComunicacaoArduino(); 

}

void loop() {
  
  receberDadosApp();
  enviarKeepAlive();
  //enviarAppTeste();
  lerArduino();

}
const int LED_PIN = 23;
