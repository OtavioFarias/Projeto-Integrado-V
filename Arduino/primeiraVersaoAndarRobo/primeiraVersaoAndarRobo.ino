#include <AFMotor_R4.h>
#include <Wire.h>
#include <MPU6050.h>

// ========================
// Ultrassônicos
// ========================
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

// ========================
// Motores
// ========================
AF_DCMotor motor1(1); 
AF_DCMotor motor2(2);
AF_DCMotor motor3(3); 
AF_DCMotor motor4(4);

int speed = 100;// 0 até 255

// ========================
// Movimento
// ========================
enum Estado { ANDANDO, VIRANDO, PARADO };
Estado estado = ANDANDO;
int direcaoVirar = 0; // 0=esquerda, 1=direita

// ========================
// MPU6050
// ========================
MPU6050 mpu;
float anguloZ = 0;
float anguloObjetivo = 90; // graus
unsigned long ultimoTempoMPU = 0;

// ========================
// Funções ultrassônicos
// ========================

// ========================
// Funções motores
// ========================
void frente() {
  motor1.run(FORWARD); motor2.run(FORWARD);
  motor3.run(FORWARD); motor4.run(FORWARD);
}

void tras() {
  motor1.run(BACKWARD); motor2.run(BACKWARD);
  motor3.run(BACKWARD); motor4.run(BACKWARD);
}

void esquerda() {
  motor1.run(FORWARD); motor2.run(FORWARD);
  motor3.run(BACKWARD); motor4.run(BACKWARD);
}

void direita() {
  motor1.run(BACKWARD); motor2.run(BACKWARD);
  motor3.run(FORWARD); motor4.run(FORWARD);
}

void parar() {
  motor1.run(RELEASE); motor2.run(RELEASE);
  motor3.run(RELEASE); motor4.run(RELEASE);
}

// ========================
// Funções de decisão
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
// ========================
// Comunicação ESP32
// ========================
void enviarDadosUltrassonicoESP(float frente, float direita, float esquerda, float tras) {
  Serial1.print("F:"); Serial1.print(frente);
  Serial1.print(" D:"); Serial1.print(direita);
  Serial1.print(" E:"); Serial1.print(esquerda);
  Serial1.print(" T:"); Serial1.println(tras);
}

// ========================
// Setup
// ========================
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  // Motores
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);

  // Sensores
  pinMode(TRIG_FRENTE, OUTPUT);
  pinMode(TRIG_OUTROS, OUTPUT);
  pinMode(ECHO_FRENTE, INPUT);
  pinMode(ECHO_DIREITA, INPUT);
  pinMode(ECHO_ESQUERDA, INPUT);
  pinMode(ECHO_TRAS, INPUT);

  attachInterrupt(digitalPinToInterrupt(ECHO_FRENTE), capturaPulso, CHANGE);

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

// ========================
// Loop
// ========================
void loop() {
  // Dispara sensores
  disparaSensor();

  // Lê sensores
  if (temMedida()) distanciaFrente = getDistancia();
  distanciaDireita = mediaUltrassonico(ECHO_DIREITA, leiturasUltrassonico);
  distanciaEsquerda = mediaUltrassonico(ECHO_ESQUERDA, leiturasUltrassonico);
  distanciaTras = mediaUltrassonico(ECHO_TRAS, leiturasUltrassonico);

  // Envia para ESP32
  enviarDadosUltrassonicoESP(distanciaFrente, distanciaDireita, distanciaEsquerda, distanciaTras);

  // Obstáculo
  if (distanciaFrente > 0 && distanciaFrente < distanciaParaVirar && estado == ANDANDO) {
    decidirMudancaTrajeto();
  }

  // Atualiza giro com MPU6050
  if (estado == VIRANDO) {
    atualizarAnguloZ();
    if (abs(anguloZ - anguloObjetivo) >= 0) { 
      parar();
      estado = ANDANDO;
      anguloZ = 0; // reseta
    } else {
      if (direcaoVirar == 0) esquerda();
      else direita();
    }
  }

  delay(20);
}
