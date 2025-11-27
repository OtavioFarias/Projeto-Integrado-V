#include "toFPGA.h"

const char* ap_ssid = "ESP32_REDE";
const char* ap_password = "12345678";

WiFiServer server(5000);

WiFiClient client;


void iniciarWIFI() {

  Serial.println("Criando Access Point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);


  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  Serial.println("Servidor TCP iniciado na porta 5000");

}
void enviarDadosFPGA(String msg) {

  client.flush();

  if (client && client.connected()) {
    client.println(msg);          // envia sempre
    client.flush();               // força envio imediato
    Serial.print("Enviado: ");
    Serial.println(msg);
  }
}

String receberDadosFPGA(){

  if (client && client.connected()) {

      if(client.available()) {
        String msg = client.readStringUntil('\n');
        msg.trim();

        if(msg != "batata") {

          Serial.print("Recebido do FPGA: ");
          Serial.println(msg);

          if(separarBits(msg, 0, 0) == "1"){

            Serial.println("Esperar Mapa");

            int tamanho = binToInt(separarBits(msg, 1, 8));
            Serial.print("Tamanho do Mapa: ");
            Serial.println(tamanho);

            receberMapa(tamanho);

            return "acabou";
            
          }
          
          if(msg.toInt() == 0) {
            
            Serial.println("Esperar caminho");

            enviarDadosFPGA("1");
            esperarFPGA();

          }
        }

        client.flush();

        return msg;

      }

  }

  return "1";

}

// Ajusta os dados para o padrão esperado do FPGA
void comunicarFPGA(String mensagem){

  delay(1000);

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

    String valorEnvio = "00000000000000000000000000000000";

    valorEnvio = escreverBits(valorEnvio, 0, 0, 1);
    valorEnvio = escreverBits(valorEnvio, 2, 5, x);
    valorEnvio = escreverBits(valorEnvio, 6, 9, y);
    valorEnvio = escreverBits(valorEnvio, 10, 10, direcao);
    valorEnvio = escreverBits(valorEnvio, 11, 14, DistanciaX);
    valorEnvio = escreverBits(valorEnvio, 15, 18, DistanciaY);
    

    Serial.print("Enviando para o FPGA do atualizar Mapa: ");
    Serial.println(valorEnvio);

    enviarDadosFPGA(valorEnvio); 

    esperarFPGA();

  }else 
  if (cabecalho == "fNovo_Trajeto") {

    Serial.println("Novo Trajeto");

    int x = mensagem.substring(pos1 + 1, pos2).toInt();
    int y = mensagem.substring(pos2 + 1, pos3).toInt();

    Serial.println(x);
    Serial.println(y);

    String valorEnvio = "00000000000000000000000000000000";

    valorEnvio = escreverBits(valorEnvio, 0, 1, 3);
    valorEnvio = escreverBits(valorEnvio, 2, 5, x);
    valorEnvio = escreverBits(valorEnvio, 6, 9, y);
    valorEnvio = escreverBits(valorEnvio, 10, 10, 1);


    Serial.print("Enviando para o FPGA do novo trajeto: ");
    Serial.println(valorEnvio);

    enviarDadosFPGA(valorEnvio);    

    esperarCaminho();

  }
  else{

    Serial.println("Destino não Identificado");

  }

}

void esperarCaminho(){

  Serial.println("Esperando novo caminho");

  int tamanho = -1;

  String binTamanho, binDados;

  while(tamanho < 0 || binTamanho.equals("1")){
    
    binTamanho = receberDadosFPGA();
    tamanho = binToInt(separarBits(binTamanho, 0, 7));

  }

  Serial.print("Tamanho: ");
  Serial.println(tamanho);

  int tamanho1 = -1;

  while((binDados = receberDadosFPGA()).equals(binTamanho) || binDados.equals("1"));

  Serial.println("Bins");
  Serial.println(binDados);
  Serial.println(binTamanho);

  int counter = 0;
  int counter2 = 0;


  while(counter <= (int)tamanho / 16){

  
    String valor = " ";

    if(counter == 0){

        valor = binDados;
        Serial.println("Dados vindo de Cima");
        Serial.println(valor);

    }else{

      while(valor.equals(" ") || valor.equals("1")) {

        valor = receberDadosFPGA();

      }

    }

    Serial.print("Caminho recebido do FPGA: ");
    Serial.println(valor);

    while(counter2 < 16){

      delay(500);
      enviarDadosVariaveis(separarBits(valor, 0 + counter2*2, 1 + counter2*2));

      counter2++;

      if(counter*16 + counter2 >= tamanho) break;

    }

    counter++;

  }

  enviarDadosVariaveis("acabou");

}

void receberMapa(int tamanho){

  Serial.println("Enviando mapa");

  int counter = 0;
  int counter2 = 0;


  while(counter <= (int)tamanho/3){

    String valor = " ";
    
    while(valor.equals(" ") || valor.equals("1")){ 
      
      valor = receberDadosFPGA();

    }

    Serial.print("Dado Recebido: ");
    Serial.println(valor);

    counter2 = 0;

    while(counter2 < 3){

      String mapaEnviar = "MPA" + separarBits(valor, 0 + counter2*10, 3 + counter2*10) + ";" + separarBits(valor, 4 + counter2*10, 7 + counter2*10)  + ";" + separarBits(valor, 8 + counter2*10, 9 + counter2*10);

      enviarDadosAppV2(mapaEnviar);

      counter2++;

    }

    counter++;

  }
  
  Serial.println("Mapa enviado");

}

void esperarFPGA(){

  while(receberDadosFPGA() != "acabou");

}

void aceitarConexao() {
  if (!client || !client.connected()) {
    WiFiClient novo = server.available();

    if (novo) {
      client = novo;   // guarda o cliente que conectou
      Serial.println("PC conectado ao ESP32!");
    }
  }
}


