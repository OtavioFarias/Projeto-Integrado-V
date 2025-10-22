#include "toApp.h"
#include "toArduino.h"


void setup() {

  Serial.begin(115200);

  iniciarBluetooth(); 
  //iniciarComunicacaoArduino(); 

}

void loop() {

  receberDadosApp();
  enviarKeepAlive();
  //enviarAppTeste();
  lerArduino();

}