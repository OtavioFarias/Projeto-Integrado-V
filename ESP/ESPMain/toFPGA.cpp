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


void enviarDadosFPGA(int msg){

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

      if(client.available()) {
        String msg = client.readStringUntil('\n');
        msg.trim();

        Serial.print("Recebido do FPGA: ");
        Serial.println(msg);

        if(separarBits(msg, 0, 0) == "1"){

          receberMapa(separarBits(msg, 1, 8).toInt());

          return "acabou";
          
        }
        
        if(msg.toInt() == 0) {
          
          enviarDadosFPGA(1);
          esperarFPGA();

        }

        return msg;

      }

    client.stop();

  }

  return "1";

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

    int x = mensagem.substring(pos1 + 1, pos2).toInt();
    int y = mensagem.substring(pos2 + 1, pos3).toInt();
    int DistanciaX = mensagem.substring(pos3 + 1, pos4).toInt();
    int DistanciaY = mensagem.substring(pos4 + 1, pos5).toInt();
    int direcao = mensagem.substring(pos5 + 1, pos6).toInt();


    String valorEnvio = String(0);

    valorEnvio = escreverBits(valorEnvio, 0, 0, 1);
    valorEnvio = escreverBits(valorEnvio, 1, 4, x);
    valorEnvio = escreverBits(valorEnvio, 5, 8, y);
    valorEnvio = escreverBits(valorEnvio, 9, 16, DistanciaX);
    valorEnvio = escreverBits(valorEnvio, 17, 24, DistanciaY);
    valorEnvio = escreverBits(valorEnvio, 25, 25, direcao);

    enviarDadosFPGA(valorEnvio.toInt()); 

  }else 
  if (cabecalho == "fNovo_Trajeto") {

    Serial.println("Novo Trajeto");

    int x = mensagem.substring(pos1 + 1, pos2).toInt();
    int y = mensagem.substring(pos2 + 1, pos3).toInt();

    Serial.println(x);
    Serial.println(y);

    String valorEnvio = String(0);

    valorEnvio = escreverBits(valorEnvio, 0, 0, 0);
    valorEnvio = escreverBits(valorEnvio, 1, 4, x);
    valorEnvio = escreverBits(valorEnvio, 5, 8, y);

    enviarDadosFPGA(valorEnvio.toInt());    

    esperarCaminho();

  }
  else{

    Serial.println("Destino não Identificado");

  }

}

void esperarCaminho(){

  Serial.println("Esperando novo caminho");

  int tamanho = -1;

  while(tamanho < 0){
    
    tamanho = receberDadosFPGA().toInt();

  }

  Serial.print("Tamanho: ");
  Serial.println(tamanho);

  int counter = 0;
  int counter2 = 0;

  while(counter < tamanho / 16){

    counter = 0;

    counter++;

    String valor = " ";

    while(valor == " ") {

      valor = receberDadosFPGA();

    }

    while(counter2 < 16){

      enviarDadosVariaveis(separarBits(valor, 0 + counter2*2, 3 + counter2*2));

      counter2++;

    }

  }

  enviarDadosVariaveis("acabou");

}

void receberMapa(int tamanho){

  Serial.println("Enviando mapa");

  int counter = 0;
  int counter2 = 0;


  while(counter < tamanho/3){

    String valor = " ";
    
    while(valor == 0){ 
      
      valor = receberDadosFPGA();

    }

    counter2 = 0;

    while(counter2 < 3){

      enviarDadosAppV2("m" + separarBits(valor, 0 + counter2*10, 3 + counter2*10) + ";" + separarBits(valor, 4 + counter2*10, 7 + counter2*10)  + ";" + separarBits(valor, 8 + counter2*10, 9 + counter2*10));

      counter2++;

    }

    counter++;

  }
  

}

void esperarFPGA(){

  while(receberDadosFPGA() != "acabou");

}