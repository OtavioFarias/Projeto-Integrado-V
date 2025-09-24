#include <Wire.h>
#include <MPU6050.h>

// === Ultrassônicos ===
#define TRIG 52
#define ECHO_frente 19   // mudou para pino 19 (interrupção)
#define ECHO_direita 48
#define ECHO_esquerda 46
#define ECHO_tras 44

// === Motores ===
#define Motor_Direita_1 4
#define Motor_Direita_2 5
#define Motor_Esquerda_1 6
#define Motor_Esquerda_2 7

// === Configurações ===
float distanciaParaVirar = 20;    // cm
float anguloObjetivo = 90;       // graus a girar
int leiturasUltrassonico = 5;    // número de leituras para média

// === Variáveis sensoriamento ===
volatile unsigned long tempoInicio = 0;
volatile unsigned long duracaoPulso = 0;
volatile bool medidaPronta = false;

float distanciaFrente = 0;
float distanciaDireita = 0;
float distanciaEsquerda = 0;
float distanciaTras = 0;

// === Movimento ===
enum Estado { ANDANDO, VIRANDO, PARADO };
Estado estado = ANDANDO;
int direcaoVirar = 0; // 0=esquerda, 1=direita

// === MPU6050 ===
MPU6050 mpu;
float anguloZ = 0;
unsigned long ultimoTempoMPU = 0;


void setup() {

  Serial.begin(9600);
  Serial1.begin(9600);

  // Motores
  pinMode(Motor_Direita_1, OUTPUT);
  pinMode(Motor_Direita_2, OUTPUT);
  pinMode(Motor_Esquerda_1, OUTPUT);
  pinMode(Motor_Esquerda_2, OUTPUT);

  // Sensores
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO_frente, INPUT);
  pinMode(ECHO_direita, INPUT);
  pinMode(ECHO_esquerda, INPUT);
  pinMode(ECHO_tras, INPUT);

  // Interrupção para o sensor da frente
  attachInterrupt(digitalPinToInterrupt(ECHO_frente), capturaPulso, CHANGE);

  // Inicializa MPU6050
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("Erro ao conectar MPU6050");
    while (1);
  }

  frente();
  ultimoTempoMPU = micros();
}

void loop() {
  // Dispara TRIG
  disparaSensor();

  // Mede sensores
  if (temMedida()) distanciaFrente = getDistancia();
  distanciaDireita = mediaUltrassonico(ECHO_direita, leiturasUltrassonico);
  distanciaEsquerda = mediaUltrassonico(ECHO_esquerda, leiturasUltrassonico);
  distanciaTras = mediaUltrassonico(ECHO_tras, leiturasUltrassonico);

  // Envia dados ao ESP32
  enviarDadosUltrassonicoESP(distanciaFrente, distanciaDireita, distanciaEsquerda, distanciaTras);

  // Checa obstáculo frente
  if (distanciaFrente > 0 && distanciaFrente < distanciaParaVirar && estado == ANDANDO) {
    decidirMudancaTrajeto();
  }

  // Atualiza giro usando MPU6050
  if (estado == VIRANDO) {
    atualizarAnguloZ();
    if (abs(anguloZ - anguloObjetivo) >= 0) { // chegou ao ângulo
      parar();
      estado = ANDANDO;
      anguloZ = 0; // reseta para próxima medição
    } else {
      // Continua girando
      if (direcaoVirar == 0) esquerda();
      else direita();
    }
  }

  delay(20); // pequeno atraso para estabilidade do loop
}

// ========================
// Funções Ultrassônicos
// ========================
void disparaSensor() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
}

// ISR para captura do pulso do ECHO_frente
void capturaPulso() {
  if (digitalRead(ECHO_frente) == HIGH) {
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
  return duracaoPulso / 58.0; // em cm
}

float medirSensor(int echoPin) {
  long duracao = pulseIn(echoPin, HIGH, 30000);
  if (duracao == 0) return -1;
  return duracao / 58.0;
}

// Faz média de múltiplas leituras para reduzir ruído
float mediaUltrassonico(int echoPin, int n) {
  float soma = 0;
  int count = 0;
  for (int i = 0; i < n; i++) {
    float d;
    if (echoPin == ECHO_frente) d = getDistancia();
    else d = medirSensor(echoPin);

    if (d > 0) { soma += d; count++; }
    delay(5);
  }
  if (count == 0) return -1;
  return soma / count;
}

// ========================
// Motores
// ========================
void frente() {
  digitalWrite(Motor_Direita_1, HIGH); digitalWrite(Motor_Direita_2, LOW);
  digitalWrite(Motor_Esquerda_1, HIGH); digitalWrite(Motor_Esquerda_2, LOW);
}

void parar() {
  digitalWrite(Motor_Direita_1, LOW); digitalWrite(Motor_Direita_2, LOW);
  digitalWrite(Motor_Esquerda_1, LOW); digitalWrite(Motor_Esquerda_2, LOW);
}

void esquerda() {
  digitalWrite(Motor_Direita_1, LOW); digitalWrite(Motor_Direita_2, HIGH);
  digitalWrite(Motor_Esquerda_1, HIGH); digitalWrite(Motor_Esquerda_2, LOW);
}

void direita() {
  digitalWrite(Motor_Direita_1, HIGH); digitalWrite(Motor_Direita_2, LOW);
  digitalWrite(Motor_Esquerda_1, LOW); digitalWrite(Motor_Esquerda_2, HIGH);
}

// ========================
// Decisão de movimento
// ========================
void decidirMudancaTrajeto() {
  estado = VIRANDO;
  direcaoVirar = decidirDirecao();
}

int decidirDirecao() {
  // Futuro: pegar dados de FPGA
  esquerda(); // por enquanto esquerda
  return 0;
}

// ========================
// MPU6050
// ========================
void atualizarAnguloZ() {
  int16_t gx, gy, gz;
  int16_t ax, ay, az;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  unsigned long agora = micros();
  float dt = (agora - ultimoTempoMPU) / 1000000.0;
  ultimoTempoMPU = agora;

  float gZ = gz / 131.0;
  anguloZ += gZ * dt;
}

// ========================
// Comunicação ESP32
// ========================
void enviarDadosUltrassonicoESP(float frente, float direita, float esquerda, float tras) {
  Serial1.print("F:"); Serial1.print(frente);
  Serial1.print(" D:"); Serial1.print(direita);
  Serial1.print(" E:"); Serial1.print(esquerda);
  Serial1.print(" T:"); Serial1.println(tras);
}

