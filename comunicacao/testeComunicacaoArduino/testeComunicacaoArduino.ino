// Comunicação com ESP32 via Serial2
// RX2 = pino 19, TX2 = pino 18 (no Arduino Mega)

#define RXD2 19
#define TXD2 18

void setup() {
  Serial.begin(115200);     // Comunicação com o PC
  Serial2.begin(9600);    // Comunicação com o ESP32
  Serial.println("Arduino Mega iniciado e pronto para comunicar com o ESP32");
}

void loop() {
  /*
  // Recebe do ESP32 (por exemplo, vindo do App Bluetooth)
  if (Serial2.available()) {
    String msgFromESP = Serial2.readStringUntil('\n');
    Serial.print("ESP32 → ");
    Serial.println(msgFromESP);

    // Aqui você pode tratar comandos vindos do app
    if (msgFromESP == "LED_ON") {
      digitalWrite(13, HIGH);
      Serial2.println("LED ligado");
    } else if (msgFromESP == "LED_OFF") {
      digitalWrite(13, LOW);
      Serial2.println("LED desligado");
    } else {
      Serial2.println("Comando desconhecido");
    }
  }

  // Envia dados do Mega para o ESP32 (simulando sensores, etc.)
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 2000) {  // a cada 2 segundos
    lastSend = millis();
    String sensorValue = String(random(0, 100));  // dado simulado
    Serial2.println("Sensor=" + sensorValue);
    //Serial.print("Enviado ao ESP32: ");
    //Serial.println(sensorValue);
  }
  */

  if (Serial2.available()) {
  while (Serial2.available()) {
    char c = Serial2.read();
    Serial.write(c);
  }
}

}
