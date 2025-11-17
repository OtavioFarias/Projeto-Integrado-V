#include "toApp.h"
#include "toArduino.h"
#include "toFPGA.h"

void setup() {
  
  Serial.begin(115200);
  iniciarBluetoothV2();

  Serial.println("Bluetooth iniciado");
  iniciarComunicacaoArduino();
  enviarDadosVariaveis("Comunicação UART inicializada");
  Serial.println("UART iniciado");

}

void loop() {
  
  receberDadosAppV2();
  lerArduino();
  //enviarDadosVariaveis("Tá vivo");
  //enviarKeepAlive();
  //enviarAppTeste();

}
