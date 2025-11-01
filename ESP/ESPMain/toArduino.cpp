/*
  Comunicação UART entre ESP32 e Arduino Mega
  ESP32 usa UART2 (pinos 16 RX, 17 TX)
*/

#include "toArduino.h"
#include <HardwareSerial.h>

HardwareSerial SerialESP(2);  // UART2

void iniciarComunicacaoArduino() {               // Monitor serial
  SerialESP.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  Serial.println("ESP32 pronto para comunicar com Mega...");
}

void lerArduino(){
  if (SerialESP.available()) {
    String recebido = SerialESP.readStringUntil('\n');
    Serial.print("Recebido do MEGA: ");
    Serial.println(recebido);
    enviarDadosAppV2(recebido);
  }
}

void enviarArduinoTeste(){
  // Envia mensagem a cada 3 segundos
  static unsigned long ultimoEnvio = 0;
  if (millis() - ultimoEnvio > 3000) {
    ultimoEnvio = millis();
    SerialESP.println("Ola MEGA, aqui é o ESP32!");
  }
}

void enviarDadosVariaveis(String msg){

  SerialESP.println(msg);
  Serial.print("Mensageme enviada para o arduino: ");
  Serial.println(msg);

}