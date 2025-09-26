String buffer = ""; // Armazena dados recebidos

void enviarDadosUltrassonicoESP(float frente, float direita, float esquerda, float tras) {
  Serial1.print("F:"); Serial1.print(frente);
  Serial1.print(" D:"); Serial1.print(direita);
  Serial1.print(" E:"); Serial1.print(esquerda);
  Serial1.print(" T:"); Serial1.println(tras);
}

void inciarComunicacaoESP(){
  Serial1.begin(9600);   // UART com ESP32
  Serial.println("Mega pronto para comunicar com ESP32...");
}

void enviarDadosESP(){
  // Envia uma mensagem a cada 2 segundos
  static unsigned long ultimoEnvio = 0;
  if (millis() - ultimoEnvio > 2000) {
    ultimoEnvio = millis();
    Serial1.println("Ola ESP32, aqui é o MEGA!");
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
    }

    start = end + 1;
    end = msg.indexOf(';', start);
  }
}


void andarESP(){}