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

bool temMedida() {
  return medidaPronta;
}

float getDistancia() {
  medidaPronta = false;
  return duracaoPulso / 58.0; // converte para cm
}

// Faz uma leitura via pulseIn (sem interrupção)
float medirSensor(int direcao) {

  long duracao;

  switch(direcao){

    case 0:

    duracao = pulseIn(ECHO_FRENTE, HIGH, 30000); // timeout 30ms
    if (duracao == 0) return -1;  // sem eco
    return duracao / 58.0;        // cm

    break;

    case 1:

      duracao = pulseIn(ECHO_ESQUERDA, HIGH, 30000); // timeout 30ms
      if (duracao == 0) return -1;  // sem eco
      return duracao / 58.0;        // cm

    break;


    case 2:

    duracao = pulseIn(ECHO_DIREITA, HIGH, 30000); // timeout 30ms
    if (duracao == 0) return -1;  // sem eco
    return duracao / 58.0;        // cm

    break;
  }

  return 0;
  
}

// Média de leituras para reduzir ruído
float mediaUltrassonico(int n, int direcao) {
  float soma = 0;
  int count = 0;
  for (int i = 0; i < n; i++) {
    float d = medirSensor(direcao); // 0 - frente, 1 - esquerda, 2 - direita
    if (d > 0) { soma += d; count++; }
    delay(5);
  }
  return (count == 0) ? -1 : soma / count;
}

//função para chamar diferentes modos para disparar os sensores, possivelmente juntos, em série ou alternado
float chamaMedirSensor(){
//adicionar média das leiturar baseado na variavel: leiturasUltrassonico = 5;
  
  distanciaFrente = mediaUltrassonico(leiturasUltrassonico, 0);
  distanciaDireita = mediaUltrassonico(leiturasUltrassonico, 2);
  distanciaEsquerda = mediaUltrassonico(leiturasUltrassonico, 0);

  return distanciaFrente;

}
