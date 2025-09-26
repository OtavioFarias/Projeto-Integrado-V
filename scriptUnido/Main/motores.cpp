#include "motores.h"

AF_DCMotor motor1(1); 
AF_DCMotor motor2(2);
AF_DCMotor motor3(3); 
AF_DCMotor motor4(4);

extern int velocidade = 100;

void iniciarMotores(){

  motor1.setSpeed(velocidade);
  motor2.setSpeed(velocidade);
  motor3.setSpeed(velocidade);
  motor4.setSpeed(velocidade);  

}

void frente() {
  motor1.run(FORWARD); 
  motor2.run(FORWARD);
  motor3.run(FORWARD); 
  motor4.run(FORWARD);
}

void parar() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE); 
  motor4.run(RELEASE);
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

void testeMotor1(){
  motor1.run(FORWARD);
}

void testeMotor2(){
  motor2.run(FORWARD);
}

void testeMotor3(){
  motor3.run(FORWARD);
}

void testeMotor4(){
  motor4.run(FORWARD);
}

void girarAngulo(){}

void andarAutomatico(){


  int distancia = medirSensor(ECHO_FRENTE);

  if (distancia > 0 && distancia < distanciaParaVirar) {
    parar();   // obstáculo detectado
    esquerda();
    delay(2500);
    frente();
  } 

  delay(100);

}

