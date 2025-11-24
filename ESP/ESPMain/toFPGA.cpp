#include "toFPGA.h"

const char* ap_ssid = "ESP32_REDE";
const char* ap_password = "12345678";

WiFiServer server(5000);

void iniciarWIFI() {

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

String receberDadosFPGA(){

  WiFiClient client = server.available();

  if (client) {

    Serial.println("PC conectado!");

    if (client.available()) {
      String msg = client.readStringUntil('\n');
      Serial.print("Recebido do FPGA: ");
      Serial.println(msg);

      if(msg == "mapa") receberMapa();

      return msg;

    }

    client.stop();

  }

  return "-1";

}

// Ajusta os dados para o padrão esperado do FPGA
void comunicarFPGA(String mensagem){

  Serial.println("Separando Mensagem Para o FPGA");

  int pos1 = mensagem.indexOf(' '); //separa o "cabeçalho" que indica para onde o cabeçalho deve ir
  int pos2 = mensagem.indexOf(' ', pos1 + 1);
  int pos3 = mensagem.indexOf(' ', pos2 + 1);
  int pos4 = mensagem.indexOf(' ', pos3 + 1);
  int pos5 = mensagem.indexOf(' ', pos4 + 1);
  int pos6 = mensagem.indexOf(' ', pos5 + 1);

  String cabecalho = mensagem.substring(0, pos1);

  Serial.print("Cabeçalho: ");
  Serial.println(cabecalho);

  if(cabecalho == "fAtualizar_Mapa:"){
    
    Serial.println("atualizar Mapa");

    String x = mensagem.substring(pos1 + 1, pos2);
    String y = mensagem.substring(pos2 + 1, pos3);
    String DistanciaX = mensagem.substring(pos3 + 1, pos4);
    String DistanciaY = mensagem.substring(pos4 + 1, pos5);
    String direcao = mensagem.substring(pos5 + 1, pos6);

    Serial.println(x);
    Serial.println(y);
    Serial.println(DistanciaX);
    Serial.println(DistanciaY);
    Serial.println(direcao);

    enviarDadosFPGA("Atualiza o mapa");    

  }else 
  if (cabecalho == "fNovo_Trajeto") {

    Serial.println("Novo Trajeto");

    String x = mensagem.substring(pos1 + 1, pos2);
    String y = mensagem.substring(pos2 + 1, pos3);

    Serial.println(x);
    Serial.println(y);

    enviarDadosFPGA("Quero o caminho novo");    

    esperarCaminho();

  }
  else{

    Serial.println("Destino não Identificado");

  }

  /*
      int sep = par.indexOf(':');
    if (sep != -1) {

      String chave = par.substring(0, sep);
      String valor = par.substring(sep + 1);
      Serial.print("Chave: "); 
      Serial.println(chave);
      
      Serial.print("valor: "); 
      Serial.println(valor.toInt());
      */

}

void esperarCaminho(){

  int tamanho = receberDadosFPGA().toInt();

  int counter = 0;

  while(counter < tamanho){

    counter++;

    enviarDadosVariaveis(receberDadosFPGA());

  }

  enviarDadosVariaveis("acabou");

}

void receberMapa(){

  Serial.println("Enviando mapa");
  enviarDadosAppV2(receberDadosFPGA());

}
