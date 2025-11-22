#import "ultrassonicos.h"

volatile unsigned long tempoInicio = 0;
volatile unsigned long duracaoPulso = 0;
volatile bool medidaPronta = false;

int leiturasUltrassonico = 5;    // média de leituras

float distanciaFrente = 0;
float distanciaDireita = 0;
float distanciaEsquerda = 0;

// Dispara o sensor frontal
void disparaSensor() {

  digitalWrite(TRIG_FRENTE, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_FRENTE, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_FRENTE, LOW);
}

// Rotina chamada pela interrupção no pino ECHO_FRENTE
void capturaPulso() {
  if (digitalRead(ECHO_FRENTE) == HIGH) {
    tempoInicio = micros();       // início do pulso
  } else {
    duracaoPulso = micros() - tempoInicio; // fim do pulso
    medidaPronta = true;
  }
}


// Faz uma leitura via pulseIn (sem interrupção)
float medirSensor(int direcao) {

  Serial.print("Medir Sensor: ");
  Serial.println(direcao);


  if(debug){
   enviarDadosESP("Funcao: medirSensor");
    enviarDadosESP("Direcao: " + direcao);
  }


  long duracao;
  int trigPin, echoPin;

  // Define qual sensor será usado
  switch (direcao) {
    case 0:
      //Serial.println("Ativando Sensor Frente");
      trigPin = TRIG_FRENTE;
      echoPin = ECHO_FRENTE;
      break;
    case 1:
      //Serial.println("Ativando Sensor Esquerda");
      trigPin = TRIG_ESQUERDA;
      echoPin = ECHO_ESQUERDA;
      break;
    case 2:
      //Serial.println("Ativando Sensor Direita");
      trigPin = TRIG_DIREITA;
      echoPin = ECHO_DIREITA;
      break;
    default:
      return -1;
  }

  int tempo = 30000;

  // Dispara o pulso ultrassônico
  //Inicializa
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede o tempo do eco
  duracao = pulseIn(echoPin, HIGH, tempo);

  Serial.print("Duração Atual");
  Serial.println(duracao);

  if (duracao == 0) return -1; // sem eco

  float distancia = duracao / 58.0; // converte em cm
  /*
  Serial.print("Distância ");
  Serial.print(direcao == 0 ? "Frente" : (direcao == 1 ? "Esquerda" : "Direita"));
  Serial.print(": ");
  Serial.println(distancia);
  */

  Serial.print("Distancia Medida");
  Serial.println(distancia);

  return distancia;

  if(debug){
    enviarDadosESP("Distancia: " + String(distancia));
  }
}


// Média de leituras para reduzir ruído
float mediaUltrassonico(int n, int direcao) {

  Serial.print("Medindo Ultrassonico: ");
  Serial.println(direcao);

  if(debug){
    enviarDadosESP("Funcao: mediaUltrassonico");
  
  }

  float soma = 0;
  int count = 0;
  for (int i = 0; i < n; i++) {

    //Serial.println("Mendindo Média da Distância");

    float d = medirSensor(direcao); // 0 - frente, 1 - esquerda, 2 - direita
    if (d > 0) { soma += d; count++; }
    delay(5);
  }
  return (count == 0) ? -1 : soma / count;
}

//função para chamar diferentes modos para disparar os sensores, possivelmente juntos, em série ou alternado
float chamaMedirSensor(){

  Serial.println("Medindo Sensores");

  if(debug){
    enviarDadosESP("Funcao: chamaMedirSensor");
  }

//adicionar média das leiturar baseado na variavel: leiturasUltrassonico = 5;

  //Serial.println("Chamando Medir Distância Frente");
  distanciaFrente = mediaUltrassonico(leiturasUltrassonico, 0);
  Serial.print("Distância Frente:");
  Serial.println(distanciaFrente);
/*
  //Serial.println("Chamando Medir Distância Direita");
  distanciaDireita = mediaUltrassonico(leiturasUltrassonico, 2);
  Serial.print("Distância Direita:");
  Serial.println(distanciaDireita);


  //Serial.println("Chamando Medir Distância Esquerda");
  distanciaEsquerda = mediaUltrassonico(leiturasUltrassonico, 1);
  Serial.print("Distância Esquerda:");
  Serial.println(distanciaEsquerda);
*/
  return distanciaFrente;

}
