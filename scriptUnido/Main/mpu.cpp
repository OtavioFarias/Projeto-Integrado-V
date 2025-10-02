#include "mpu.h"


MPU6050 mpu;
float anguloZ = 0;
float anguloObjetivo = 90; // graus
unsigned long ultimoTempoMPU = 0;

// fator do filtro (0.98 = mais confiança no gyro, 0.02 = correção do accel)
#define ALPHA 0.98  

void atualizarAnguloZ_ComFiltro() {

  int16_t gx, gy, gz;
  int16_t ax, ay, az;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  unsigned long agora = micros();

  float dt = (agora - ultimoTempoMPU) / 1000000.0;
  ultimoTempoMPU = agora;

  // giroscópio em °/s
  float gZ = gz / 131.0;

  // integração do giroscópio
  float anguloGyro = anguloZ + gZ * dt;

  // ângulo absoluto pelo acelerômetro (plano X-Y -> eixo Z)
  float anguloAccel = atan2((float)ay, (float)ax) * 180.0 / M_PI;

  // filtro complementar
  anguloZ = ALPHA * anguloGyro + (1 - ALPHA) * anguloAccel;
}

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

float erroDeRotacao() {
  float erro = anguloObjetivo - anguloZ;

  // normaliza para -180° ... +180°
  while (erro > 180) erro -= 360;
  while (erro < -180) erro += 360;

  return erro; // positivo = virar para a esquerda, negativo = direita
}

