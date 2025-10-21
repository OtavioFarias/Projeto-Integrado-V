#include "toApp.h"
#include "toArduino.h"
//#include "toFPGA.h"

void setup() {

  Serial.begin(115200);

  iniciarBluetooth(); 
  
  //iniciarComunicacaoArduino();
  //Serial.println("UART iniciado com sucesso"); 
  
  //iniciarWIFI();
  //Serial.print("WIFI iniciado com sucesso");

}

void loop() {

  receberDadosApp();
  enviarKeepAlive();
  enviarAppTeste();
  lerArduino();

}