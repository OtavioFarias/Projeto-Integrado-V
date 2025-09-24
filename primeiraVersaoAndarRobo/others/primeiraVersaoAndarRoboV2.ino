#include <Wire.h>
#include <MPU6050.h>

// === Ultrassônicos ===
#define TRIG 12
#define ECHO_frente 2    // interrupção
#define ECHO_tras 8
#define ECHO_direita 9
#define ECHO_esquerda 10

// === Motores ===
#define Motor_Direita_1 4
#define Motor_Direita_2 5
#define Motor_Esquerda_1 6
#define Motor_Esquerda_2 7

// === Configurações ===
float distanciaParaVirar = 20; // cm
float anguloObjetivo = 90;    // ângulo a girar (graus)
float anguloInicial = 0;
bool girando = false;

// === Variáveis dos sensores ===
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
int direcaoVirar = 0;  // 0=esquerda, 1=direita

// === MPU6050 ===
MPU6050 mpu;
float anguloZ = 0;

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

  attachInterrupt(digitalPinToInterrupt(ECHO_frente), capturaPulso, CHANGE);

  // Inicializa MPU6050
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("Erro ao conectar MPU6050");
    while (1);
  }

  frente();
}

void loop() {
  // Dispara TRIG
  disparaSensor();

  // Mede sensores
  if (temMedida()) distanciaFrente = getDistancia();
  distanciaDireita = medirSensor(ECHO_direita);
  distanciaEsquerda = medirSensor(ECHO_esquerda);
  distanciaTras = medirSensor(ECHO_tras);

  // Envia ao ESP32
  enviarDadosUltrassonicoESP(distanciaFrente, distanciaDireita, distanciaEsquerda, distanciaTras);

  // Checa obstáculo
  if (distanciaFrente < distanciaParaVirar && estado == ANDANDO) {
    decidirMudancaTrajeto();
  }

  // Gira até ângulo objetivo
  if (estado == VIRANDO) {
    atualizarAnguloZ();
    if (abs(anguloZ - anguloInicial) >= anguloObjetivo) {
      parar();
      estado = ANDANDO;
    } else {
      // Continua girando na direção definida
      if (direcaoVirar == 0) esquerda();
      else direita();
    }
  }
}

// ========================
// Funções dos sensores
// ========================
void disparaSensor() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
}

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
  return duracaoPulso / 58.0;
}

float medirSensor(int echoPin) {
  long duracao = pulseIn(echoPin, HIGH, 30000);
  if (duracao == 0) return -1;
  return duracao / 58.0;
}

// ========================
// Funções dos motores
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
// Funções de decisão
// ========================
void decidirMudancaTrajeto() {
  estado = VIRANDO;
  anguloInicial = anguloZ; // guarda ângulo atual
  direcaoVirar = decidirDirecao(); // por enquanto retorna esquerda()
}

int decidirDirecao() {
  // Corpo da função deixado para o futuro
  esquerda(); // por enquanto só esquerda
  return 0;
}

// ========================
// MPU6050
// ========================
void atualizarAnguloZ() {
  int16_t gx, gy, gz;
  int16_t ax, ay, az;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // converte para graus/s
  float gZ = gz / 131.0; // taxa de rotação em °/s
  static unsigned long ultimoTempo = 0;
  unsigned long agora = millis();
  float dt = (agora - ultimoTempo) / 1000.0;
  anguloZ += gZ * dt;
  ultimoTempo = agora;
}

// ========================
// Comunicação com ESP32
// ========================
void enviarDadosUltrassonicoESP(float frente, float direita, float esquerda, float tras) {
  Serial1.print("F:"); Serial1.print(frente);
  Serial1.print(" D:"); Serial1.print(direita);
  Serial1.print(" E:"); Serial1.print(esquerda);
  Serial1.print(" T:"); Serial1.println(tras);
}
