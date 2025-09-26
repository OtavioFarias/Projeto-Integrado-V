#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
float anguloZ = 0;
float anguloObjetivo = 90; // graus
unsigned long ultimoTempoMPU = 0;

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

