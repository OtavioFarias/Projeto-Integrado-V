#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define RXD2 16 //pino de recepção
#define TXD2 17 //pino de transmissão

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  SerialBT.begin("ESP32_BT");
  Serial.println("Bluetooth iniciado como ESP32_BT");
}

void loop() {
  // Recebe do Arduino e manda para o App
  if (Serial2.available()) {
    String msgFromMega = Serial2.readStringUntil('\n');
    Serial.print("Mega → ");
    Serial.println(msgFromMega);
    SerialBT.println(msgFromMega);
  }

  // Recebe do App e manda para o Arduino
  if (SerialBT.available()) {
    String msgFromApp = "";
    unsigned long start = millis();
    while (millis() - start < 100) {  // timeout 100ms
      if (SerialBT.available()) {
        char c = SerialBT.read();
        if (c == '\n') break;
        msgFromApp += c;
      }
    }
    
    if (msgFromApp.length() > 0) {
      Serial.print("App → ");
      Serial.println(msgFromApp);
      Serial2.println(msgFromApp);
    }
  }
}

#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);   // Monitor serial do PC
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // UART2 para o Mega
}

void loop() {
  if (Serial2.available()) {
    String dado = Serial2.readStringUntil('\n');
    Serial.print("Recebido do Mega: ");
    Serial.println(dado);
  }
}

