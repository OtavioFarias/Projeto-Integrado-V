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

  enviarDadosAppV2("MPA0100;0101;01");

  aceitarConexao();
  
  receberDadosAppV2();
  lerArduino();
  receberDadosFPGA();

}
