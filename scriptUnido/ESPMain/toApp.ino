#include "BluetoothSerial.h"

BluetoothSerial SerialBT;


void iniciarBluetooth(){
  SerialBT.begin("ESP32_BT"); // Nome que aparecerá no scan do Linux
  Serial.println("ESP32 pronto! Use Bluetooth para conectar.");
}

void receberDadosApp(){
  // Se receber dados do Bluetooth, repassa para o Serial
  if (SerialBT.available()) {
    char c = SerialBT.read();
    Serial.write(c); // mostra no monitor serial
    SerialBT.print("Echo: "); 
    SerialBT.println(c); // devolve ao PC
  }
}

void enviarAppTeste(){
  // Também pode enviar mensagens periódicas
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 5000) {
    lastSend = millis();
    SerialBT.println("Mensagem do ESP32 via Bluetooth!");
  }
}
