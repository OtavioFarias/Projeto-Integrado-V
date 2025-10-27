#include "mpu.h"


MPU6050 mpu;
float anguloZ = 0;
float anguloObjetivo = 90; // graus
unsigned long ultimoTempoMPU = 0;

// fator do filtro (0.98 = mais confiança no gyro, 0.02 = correção do accel)
#define ALPHA 0.98  

float gz_offset = 0;  // declare como variável global


// --- Calibração simples do giroscópio (chamar no setup) ---
void calibrarGyroZ() {
  long soma = 0;
  const int N = 500;

  for (int i = 0; i < N; i++) {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    soma += gz;
    delay(5);
  }

  gz_offset = soma / (float)N;  // média do offset
  Serial.print("Offset Z calibrado: ");
  Serial.println(gz_offset);
}

// --- Função ajustada ---
void atualizarAnguloZ_ComFiltro() {
  int16_t gx, gy, gz;
  int16_t ax, ay, az;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  unsigned long agora = micros();
  float dt = (agora - ultimoTempoMPU) / 1000000.0;
  ultimoTempoMPU = agora;

  // Compensa o offset e converte para °/s
  float gZ = (gz - gz_offset) / 131.0;

    //ao tentar usar o antigo   anguloZ += gZ * dt; o carrinho mexia apenas metade do ângulo necessário para virar, então alterei o comportamento dessa medida de angulo

  // Integra o giroscópio (sem acelerômetro no eixo Z)
  anguloZ += gZ * dt/2;

  /*
  Serial.print("Ângulo Z: ");
  Serial.println(anguloZ);
  */
}


void atualizarAnguloZ() {
  int16_t gx, gy, gz;
  int16_t ax, ay, az;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  unsigned long agora = micros();
  float dt = (agora - ultimoTempoMPU) / 1000000.0;
  ultimoTempoMPU = agora;

  float gZ = gz / 131.0;

  //ao tentar usar o antigo   anguloZ += gZ * dt; o carrinho mexia apenas metade do ângulo necessário para virar, então alterei o comportamento dessa medida de angulo
  anguloZ += gZ * dt / 6;
}

float erroDeRotacao() {
  float erro = anguloObjetivo + anguloZ;

  // normaliza para -180° ... +180°
  while (erro > 180) erro -= 360;
  while (erro < -180) erro += 360;

  return erro; // positivo = virar para a esquerda, negativo = direita
}

