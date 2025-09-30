#include "ultrassonicos.h"


volatile unsigned long tempoInicio = 0;
volatile unsigned long duracaoPulso = 0;
volatile bool medidaPronta = false;

float distanciaFrente = 0;
float distanciaDireita = 0;
float distanciaEsquerda = 0;
float distanciaTras = 0;

int leiturasUltrassonico = 5;    // média de leituras


void disparaSensor() {
  digitalWrite(TRIG_TRAS, LOW);
  digitalWrite(TRIG_DIREITA, LOW);
  digitalWrite(TRIG_ESQUERDA, LOW);
  digitalWrite(TRIG_FRENTE, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_TRAS, HIGH);
  digitalWrite(TRIG_DIREITA, HIGH);
  digitalWrite(TRIG_ESQUERDA, HIGH);
  digitalWrite(TRIG_FRENTE, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_TRAS, LOW);
  digitalWrite(TRIG_DIREITA, LOW);
  digitalWrite(TRIG_ESQUERDA, LOW);
  digitalWrite(TRIG_FRENTE, LOW);
}

void capturaPulso() {
  if (digitalRead(ECHO_FRENTE) == HIGH) {
    tempoInicio = micros();
  } else {
    duracaoPulso = micros() - tempoInicio;
    medidaPronta = true;
  }
}

bool temMedida() {
  return medidaPronta;
}

float getDistancia() {
  medidaPronta = false;
  return duracaoPulso / 58.0;
}

float medirSensor(int echoPin) {

  disparaSensor();

  long duracao = pulseIn(echoPin, HIGH, 30000);
  if (duracao == 0) return -1;
  return duracao / 58.0;
}

float mediaUltrassonico(int echoPin, int n) {
  float soma = 0;
  int count = 0;
  for (int i = 0; i < n; i++) {
    float d = (echoPin == ECHO_FRENTE) ? getDistancia() : medirSensor(echoPin);
    if (d > 0) { soma += d; count++; }
    delay(5);
  }
  return (count == 0) ? -1 : soma / count;
}

// Funções para medir cada sensor separadamente
float medirFrente(int n) {
  return mediaUltrassonico(ECHO_FRENTE, n);
}

float medirTras(int n) {
  return mediaUltrassonico(ECHO_TRAS, n);
}

float medirDireita(int n) {
  return mediaUltrassonico(ECHO_DIREITA, n);
}

float medirEsquerda(int n) {
  return mediaUltrassonico(ECHO_ESQUERDA, n);
}

// Função para medir todos de uma vez com tempo entre disparos
void medirTodos(int n, int tempoEntreDisparosMs) {
  distanciaFrente  = medirFrente(n);
  delay(tempoEntreDisparosMs);

  distanciaTras    = medirTras(n);
  delay(tempoEntreDisparosMs);

  distanciaDireita = medirDireita(n);
  delay(tempoEntreDisparosMs);

  distanciaEsquerda = medirEsquerda(n);
  delay(tempoEntreDisparosMs);
}

// Função para disparar em série (sem medir, só gerar o pulso em cada)
void dispararSerie(int tempoEntreDisparosMs) {
  // Frente
  digitalWrite(TRIG_FRENTE, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_FRENTE, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_FRENTE, LOW);
  delay(tempoEntreDisparosMs);

  // Trás
  digitalWrite(TRIG_TRAS, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_TRAS, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_TRAS, LOW);
  delay(tempoEntreDisparosMs);

  // Direita
  digitalWrite(TRIG_DIREITA, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_DIREITA, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_DIREITA, LOW);
  delay(tempoEntreDisparosMs);

  // Esquerda
  digitalWrite(TRIG_ESQUERDA, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_ESQUERDA, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_ESQUERDA, LOW);
  delay(tempoEntreDisparosMs);
}

