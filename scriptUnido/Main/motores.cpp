#include "motores.h"

AF_DCMotor motor1(1); 
AF_DCMotor motor2(2);
AF_DCMotor motor3(3); 
AF_DCMotor motor4(4);

extern int velocidade = 255;
extern float toleranciaErroRotacao = 10
float distanciaParaVirar = 20;    // cm

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


  int distancia = medirSensor();

  Serial.print("Distância: ");
  Serial.println(distancia);

  if (distancia > 0 && distancia < distanciaParaVirar) {
    parar();   // obstáculo detectado
    virarCoordenado();
    frente();
  } 

  delay(100);

}


void passoFrente(int duracao){

  frente();
  delay(duracao);
  parar();

}

void passoTras(int duracao){

  tras();
  delay(duracao);
  parar();

}

void passoDireita(int duracao){

  direita();
  delay(duracao);
  parar();

}

void passoEsquerda(int duracao){

  esquerda();
  delay(duracao);
  parar();

}

// Função principal de virar coordenado
void virarCoordenado() {
  Serial.println("Virar");
  int direcao = direcaoIndicadaFPGA();

  Serial.println(direcao);

  // define objetivo conforme direção recebida
  if (direcao == 0) { 
    anguloObjetivo = 0; 
  } else if (direcao == 1) { 
    anguloObjetivo = 90; 
  } else if (direcao == 2) { 
    anguloObjetivo = 180; 
  } else if (direcao == 3) { 
    anguloObjetivo = -90; 
  }

  // loop até atingir objetivo
  while (true) {

    atualizarAnguloZ_ComFiltro(); // mantém anguloZ atualizado
    float erro = erroDeRotacao();

    Serial.println("erro de rotação");   
    Serial.println(erro);


    if (fabs(erro) < toleranciaErroRotacao) { // tolerância de 3 graus
      parar();
      break;
    }

    if (erro > 0) {
      passoEsquerda(200); // gira em passos pequenos
    } else {
      passoDireita(200);
    }
  }
}

int direcaoIndicadaFPGA(){
  // 0 = frente, 1 = direita, 2 = trás, 3 = esquerda)
  return 1;

}
