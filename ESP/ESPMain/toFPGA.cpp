#include "toFPGA.h"

const char* ap_ssid = "ESP32_REDE";
const char* ap_password = "12345678";

WiFiServer server(5000);

void iniciarWIFI() {
  Serial.begin(115200);

  Serial.println("Criando Access Point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  Serial.println("Servidor TCP iniciado na porta 5000");
}


void enviarDadosFPGA(String msg){

  WiFiClient client = server.available();

  if (client) {
    Serial.println("PC conectado!");

      if (client.available()) {
        
        Serial.print("Enviando para o FPGA: ");
        Serial.println(msg);
        client.println(msg);

      }

    client.stop();

  }

}

void receberDadosFPGA(){

  WiFiClient client = server.available();

  if (client) {

    Serial.println("PC conectado!");

    if (client.available()) {
      String msg = client.readStringUntil('\n');
      Serial.print("Recebido do FPGA: ");
      Serial.println(msg);

    }

    client.stop();

  }

}


