

//#include <WiFi.h>
/*
// Configuração do Access Point
const char* ssid = "ESP32_AP";
const char* password = "12345678";

// Porta para comunicação TCP
WiFiServer server(80);
WiFiClient client;

void iniciarWIFI(){
  Serial.begin(115200);

  // Inicia o AP
  WiFi.softAP(ssid, password);

  // Inicia o servidor
  server.begin();
}


// -------- FUNÇÃO PARA ENVIAR DADOS --------
void enviarDados(String mensagem) {
  if (client && client.connected()) {
    client.println(mensagem);
  }
}

// -------- FUNÇÃO PARA RECEBER DADOS --------
void receberDados() {
  if (client && client.available()) {
    String dados = client.readStringUntil('\n');
  }
}
*/

