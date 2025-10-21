//falta adicionar um controle para saber onde o robo está, possivelmente faça isso usando um variavel de 2 bits que indica a direção atual e ao chamar a função de andar para o próximo bloco, ele atualiza a posição atual

#include "motores.h"

// Pino do sensor Hall
const int pinoSensor = 2;  

float distanciaDesejada = 42;

int tamanhoQuadradoEmPulsos = distanciaDesejada/(6.5*3.14);

AF_DCMotor motor1(1); 
AF_DCMotor motor2(2);
AF_DCMotor motor3(3); 
AF_DCMotor motor4(4);

int velocidade = 255;
float toleranciaErroRotacao = 10;
float distanciaParaVirar = 20;    // cm

uint8_t direcaoAtual = 0; //00 - frente, 01 - esquerda, 10 - direita, 11 - tras

//posicao do carrinho na malha
int posicaoAtualX = 0;
int posicaoAtualY = 0;

cppQueue	filaDestino(sizeof(uint8_t), 10, FIFO, true);

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


  int distancia = chamaMedirSensor();

  Serial.print("Distância: ");
  Serial.println(distancia);

  enviarDadosLaterais();

  if (distancia > 0 && distancia < distanciaParaVirar) {

    //manda requisição para o FPGA, se não apenas manda os dados
    parar();   // obstáculo detectado
    irParaCoordenada();
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



void irParaCoordenada(){

  //precisa de um tratamento para colocar o robo na posição correta

  //chamar função que envia requisição para o FPGA e espera

  esperarFPGA();

  while (!filaDestino.isEmpty()) {

    uint8_t valor; 
    filaDestino.pop(&valor);


    int distancia = chamaMedirSensor();

    Serial.print("Distância: ");
    Serial.println(distancia);

    enviarDadosLaterais();
    
    if (distancia > 0 && distancia < distanciaParaVirar) {

      //manda requisição para o FPGA, se não apenas manda os dados
      parar();   // obstáculo detectado
      irParaCoordenada();
      
    }
    else{


      andarQuadrado(valor);


    }

  }

}

void andarQuadrado(int direcao){

  ajustarDirecao(direcao);

  int contagem = 0; // zera a contagem

  while (contagem < tamanhoQuadradoEmPulsos) {
    
    contagem++;

    frente();

    mudarPosicaoAtual();

  }

  parar();

  

}

void ajustarDirecao(int direcao){ //direçao para qua, precisa ir

  if(direcao != direcaoAtual) {

    virarCoordenado(1);
    ajustarDirecao(direcao);

  }

  direcaoAtual = direcao;

}

// Função principal de virar coordenado
void virarCoordenado(int direcao) {

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

    if (fabs(erro) < 3) { // tolerância de 3 graus
      parar();
      break;
    }

    if (erro > 0) {
      passoEsquerda(20); // gira em passos pequenos
    } else {
      passoDireita(20);
    }
  }
}


void mudarPosicaoAtual(){

  //atualizar posição atual, será chamado sempre que andar, só faz a lógica de atualizar a variavel

  switch(direcaoAtual) {

    //frente
    case 00:

      posicaoAtualY++;    

    break;

    //esquerda
    case 01:

      posicaoAtualX--;

    break;

    //direita
    case 10:

      posicaoAtualX++;

    break;

    //tras
    case 11:

      posicaoAtualY--;

    break;

  }

}