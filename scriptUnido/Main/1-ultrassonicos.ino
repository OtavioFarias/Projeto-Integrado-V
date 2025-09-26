#define TRIG_FRENTE 52
#define TRIG_OUTROS 22
#define ECHO_FRENTE 19   // com interrupção
#define ECHO_DIREITA 28
#define ECHO_ESQUERDA 26
#define ECHO_TRAS 30

volatile unsigned long tempoInicio = 0;
volatile unsigned long duracaoPulso = 0;
volatile bool medidaPronta = false;

float distanciaFrente = 0;
float distanciaDireita = 0;
float distanciaEsquerda = 0;
float distanciaTras = 0;

float distanciaParaVirar = 20;    // cm
int leiturasUltrassonico = 5;    // média de leituras

void disparaSensor() {
  digitalWrite(TRIG_OUTROS, LOW);
  digitalWrite(TRIG_FRENTE, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_OUTROS, HIGH);
  digitalWrite(TRIG_FRENTE, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_OUTROS, LOW);
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
