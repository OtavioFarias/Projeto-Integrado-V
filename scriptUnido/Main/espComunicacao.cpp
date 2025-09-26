#include "espComunicacao.h"

extern int andarAutonomo; //1 - anda sozinho 0 - Controle via ESP

String buffer = ""; // Armazena dados recebidos

void enviarDadosUltrassonicoESP(float frente, float direita, float esquerda, float tras) {
  Serial2.print("F:"); Serial1.print(frente);
  Serial2.print(" D:"); Serial1.print(direita);
  Serial2.print(" E:"); Serial1.print(esquerda);
  Serial2.print(" T:"); Serial1.println(tras);
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
  // Recebe caracteres da UART e forma uma string até '\n'
  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') {
      processarMensagem(buffer);
      buffer = "";
    } else {
      buffer += c;
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

      // Atualiza variáveis conforme a chave
      if (chave == "velocidade") velocidade = valor.toInt();
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
    }

    start = end + 1;
    end = msg.indexOf(';', start);
  }
}


void andarESP(){

   Serial.print("Implementação Pendente: ");

}