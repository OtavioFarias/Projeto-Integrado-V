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

  iniciarWIFI();
  enviarDadosVariaveis("WIFI iniciado");
  Serial.println("WIFI iniciado");

}

void loop() {
  

  enviarDadosFPGA(15);
  receberDadosAppV2();
  lerArduino();
  receberDadosFPGA();
  //enviarDadosVariaveis("Tá vivo");
  //enviarKeepAlive();
  //enviarAppTeste();

}
