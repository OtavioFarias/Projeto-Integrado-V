#include "espComunicacao.h"

extern int andarAutonomo; //1 - anda sozinho 0 - Controle via ESP

String buffer = ""; // Armazena dados recebidos
int ativarEnvioDados = 0; // 1- envia 0 - não envia

void enviarDadosUltrassonicoESP(float frente, float direita, float esquerda, float tras) {
  Serial2.print("F:"); Serial2.print(frente);
  Serial2.print(" D:"); Serial2.print(direita);
  Serial2.print(" E:"); Serial2.print(esquerda);
  Serial2.print(" T:"); Serial2.println(tras);
}

void inciarComunicacaoESP(){
  Serial2.begin(9600);   // UART com ESP32 define quais usar, vamos usar 16 e 17
  Serial2.setTimeout(100); // 100 ms em vez de 1000
  Serial.println("Mega pronto para comunicar com ESP32...");

}

void enviarDadosESP(){
  // Envia uma mensagem a cada 2 segundos
  static unsigned long ultimoEnvio = 0;
  if (millis() - ultimoEnvio > 2000) {
    ultimoEnvio = millis();
    Serial2.println("Ola ESP32, aqui é o MEGA!");
  }
}

String bufferESP = "";

void receberDadosESP() {
  while (Serial2.available()) {
    char c = Serial2.read();

    if (c == '\n') {  
      bufferESP.trim();
      if (bufferESP.length() > 0) {
        Serial.print("Recebido do ESP32: ");
        Serial.println(bufferESP);
        processarMensagem(bufferESP);
      }
      bufferESP = ""; // limpa buffer
    } else {
      bufferESP += c;
    }
  }
}

// Função para processar a mensagem recebida
void processarMensagem(String msg) {
  Serial.print("Recebido: ");
  Serial.println(msg);

  // Divide os pares separados por ';'
  int start = 0;
  int end = msg.indexOf(';');

  while (end != -1) {

    String par = msg.substring(start, end);

    int sep = par.indexOf(':');
    if (sep != -1) {

      String chave = par.substring(0, sep);
      String valor = par.substring(sep + 1);
      Serial.print("Chave: "); 
      Serial.println(chave);
      
      Serial.print("valor: "); 
      Serial.println(valor.toInt());

      // Atualiza variáveis conforme a chave
      if (chave == "velocidade"){velocidade = valor.toInt(); iniciarMotores();}
      else if (chave == "anguloObjetivo") anguloObjetivo = valor.toInt();
      else if (chave == "distanciaParaVirar") distanciaParaVirar = valor.toInt();
      else if (chave == "leiturasUltrassonico") leiturasUltrassonico = valor.toInt();
      else if (chave == "andarAutonomo") andarAutonomo = valor.toInt();
      else if (chave == "frente") frente();
      else if (chave == "tras") tras();
      else if (chave == "direita") direita();
      else if (chave == "esquerda") esquerda();
      else if (chave == "parar") parar();
      else if (chave == "passoFrente") passoFrente(valor.toInt());
      else if (chave == "passoTras") passoTras(valor.toInt());
      else if (chave == "passoDireita") passoDireita(valor.toInt());
      else if (chave == "passoEsquerda") passoEsquerda(valor.toInt());
      else if (chave == "virarCoordenado") virarCoordenado(valor.toInt());
      else if (chave == "ativarEnvioDados") ativarEnvioDados = valor.toInt();
      else if (chave == "andarQuadrado") andarQuadrado(valor.toInt());
      else if (chave == "debug") debug = valor.toInt();
      else if (chave == "tamanhoPasso") tamanhoPasso = valor.toInt();

      enviarDadosESP("Recebido com sucesso: " + msg);

    }

    start = end + 1;
    end = msg.indexOf(';', start);
    //enviarDadosESP();
  }
}


void andarESP(){

  delay(100);

  //Serial.println("Andar com controle do App");
/*
  int distancia = medirSensor(0);

  if(ativarEnvioDados == 1){

    Serial.print("Distância medida pelo sensor da frente: ");
    Serial.println(distancia);

  }

*/
}

void esperarFPGA(){

  if(debug){
    enviarDadosESP("Função: esperaraFPGA");
  }

  // ESP espera receber esses dois dados depois envia a resposta
  enviarDadosESPparaFPGA(String("Novo_Trajeto") + " "  + String(posicaoAtualX) + " "  + String(posicaoAtualX));

  String mensagem;

  while((mensagem = receberDadosESPdoFPGA()) != "acabou"){
    
    uint8_t valor = mensagem.toInt();    
    filaDestino.push(&valor);  

  }

  Serial.println("Novo Trajeto Recebido");

}

void enviarDadosLaterais(){

  enviarDadosESPparaFPGA(String("Atualizar_Mapa:") + " "  + String(posicaoAtualX) + " "  + String(posicaoAtualX) + " "  + String((int)(distanciaDireita / distanciaDesejada)) + " "  + String((int) (distanciaEsquerda/ distanciaDesejada)) + " "  + String(direcaoAtual));

}

//Sempre para o App
void enviarDadosESP(String msg) {

  Serial.print("Enviando para o ESP: ");
  Serial.println(msg);
  Serial2.println("a" + msg); //mensagem deve ir para o app

}

void enviarDadosESPparaFPGA(String msg) {

  Serial.print("Enviando para o FPGA: ");
  Serial.println(msg);
  Serial2.println("f" + msg); //mensagem deve ir para o FPGA

}

String receberDadosESPdoFPGA() {
  //Serial.println("Tentando ler dados ESP: ");
  
  //Serial.println("Tentando Ler ESP");
  while(1){
    if (Serial2.available()) { // Verifica se há dados
      Serial.println(Serial2.available());
      String mensagem = Serial2.readStringUntil('\n'); // Lê a string completa até '\n'
      //String mensagem = String(Serial2.read());
      mensagem.trim(); // Remove espaços ou caracteres invisíveis
      if (mensagem.length() > 0) {
        return mensagem;
      }
    }
  }
  
}