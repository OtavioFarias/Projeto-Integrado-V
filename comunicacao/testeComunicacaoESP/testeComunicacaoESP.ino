#define RXD2 16  // Pino de recepção (conectado ao TX do Arduino)
#define TXD2 17  // Pino de transmissão (conectado ao RX do Arduino)

void setup() {
  Serial.begin(115200);                   // Monitor serial do PC
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  // Comunicação com o Arduino
  Serial.println("ESP32 iniciado. Aguardando mensagens do Arduino...");
}

void loop() {
  // Recebe do Arduino e mostra no Monitor Serial
  if (Serial2.available()) {
    String msgFromArduino = Serial2.readStringUntil('\n');
    Serial.print("Arduino → ");
    Serial.println(msgFromArduino);
  }

  // Envia mensagem de volta ao Arduino (exemplo a cada 2 segundos)
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 2000) {
    lastSend = millis();
    String msgToArduino = "Oi Arduino! Tempo=" + String(millis() / 1000) + "s";
    Serial2.println(msgToArduino);
    Serial.print("ESP32 → ");
    Serial.println(msgToArduino);
  }
}
