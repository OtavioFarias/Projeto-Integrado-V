#import "ultrassonicos.h"

volatile unsigned long tempoInicio = 0;
volatile unsigned long duracaoPulso = 0;
volatile bool medidaPronta = false;

float distanciaFrente = 0;
int leiturasUltrassonico = 5;    // média de leituras


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
float medirSensor() {
  disparaSensor();
  long duracao = pulseIn(ECHO_FRENTE, HIGH, 30000); // timeout 30ms
  if (duracao == 0) return -1;  // sem eco
  return duracao / 58.0;        // cm
}

// Média de leituras para reduzir ruído
float mediaUltrassonico(int n) {
  float soma = 0;
  int count = 0;
  for (int i = 0; i < n; i++) {
    float d = medirSensor();
    if (d > 0) { soma += d; count++; }
    delay(5);
  }
  return (count == 0) ? -1 : soma / count;
}
