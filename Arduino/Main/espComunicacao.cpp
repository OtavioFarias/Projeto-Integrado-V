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



void receberDadosESP() {
  //Serial.println("Tentando ler dados ESP: ");
  
  if (Serial2.available()) { // Verifica se há dados
    String mensagem = Serial2.readStringUntil('\n'); // Lê a string completa até '\n'
    mensagem.trim(); // Remove espaços ou caracteres invisíveis
    if (mensagem.length() > 0) {
      Serial.print("Recebido do ESP32: ");
      Serial.println(mensagem);
      processarMensagem(mensagem); // envia para a função que processa a mensagem
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
      else if (chave == "virarCoordenado") virarCoordenado();
      else if (chave == "ativarEnvioDados") ativarEnvioDados = valor.toInt();

    }

    start = end + 1;
    end = msg.indexOf(';', start);
    enviarDadosESP();
  }
}


void andarESP(){

  //Serial.println("Andar com controle do App");

  int distancia = medirSensor();

  if(ativarEnvioDados == 1){

    Serial.print("Distância medida pelo sensor da frente: ");
    Serial.println(distancia);

  }

}