//falta adicionar um controle para saber onde o robo está, possivelmente faça isso usando um variavel de 2 bits que indica a direção atual e ao chamar a função de andar para o próximo bloco, ele atualiza a posição atual

#include "motores.h"

// Pino do sensor Hall
const int pinoSensorHall = 48;  

int estadoAnteriorHall = HIGH;   // guarda estado anterior do sensor
int contadorPulsos = 0;          // contador de pulsos

float distanciaDesejada = 42;

int tamanhoQuadradoEmPulsos = distanciaDesejada/(6.5*3.14);

AF_DCMotor motor1(1); 
AF_DCMotor motor2(2);
AF_DCMotor motor3(3); 
AF_DCMotor motor4(4);

int velocidade = 255;
float toleranciaErroRotacao = 10;
float distanciaParaVirar = 20;    // cm

Direcao direcaoAtual = FRENTE;

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

    Serial.println("Vou Bater");

    //manda requisição para o FPGA, se não apenas manda os dados
    parar();   // obstáculo detectado
    irParaCoordenada();
    //frente();
    //andarQuadrado(direcaoAtual);
  }
  else{

    andarQuadrado(direcaoAtual);

  }

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

  Serial.println("Novo Trajeto Recebido do FPGA");

  while (!filaDestino.isEmpty()) {

    uint8_t valor; 
    filaDestino.pop(&valor);

    andarQuadrado(valor);


  }

}

Direcao ajustarRequisicao(Direcao direcaoFPGA) {
  // O FPGA envia a direção absoluta (na malha)
  // Esta função converte para a direção relativa ao carrinho

  // Exemplo:
  // Se o robô está virado para a ESQUERDA (1)
  // e o FPGA manda DIREITA (2)
  // => resultado será TRAS (3)

  int direcaoVerdadeira = (direcaoFPGA - direcaoAtual + 4) % 4;

  return (Direcao)direcaoVerdadeira;
}

void andarQuadrado(Direcao direcao) {
  Serial.println("Andar um Quadrado");

  ajustarDirecao(ajustarRequisicao(direcao));

  contadorPulsos = 0;        // zera contador antes de começar
  estadoAnteriorHall = HIGH; // inicializa estado anterior

  while (contadorPulsos < tamanhoQuadradoEmPulsos) {
    frente();                 // anda para frente
    atualizarContadorHall();  // atualiza contador de pulsos do sensor

    Serial.print("Contagem de Pulsos: ");
    Serial.print(contadorPulsos);
    Serial.print(" / ");
    Serial.println(tamanhoQuadradoEmPulsos);

    //delay(500);
    
  }


  mudarPosicaoAtual(); 

  parar();                     // para o robô
  Serial.println("Lado do quadrado completo!");
}


void ajustarDirecao(Direcao direcao){ //direçao para qua, precisa ir

  Serial.println("Ajustando Direção");
  Serial.print("Direção Atual: ");
  Serial.println(direcaoAtual);
  Serial.print("Nova Direção: ");
  Serial.println(direcao);

  if(direcao != direcaoAtual) {

    virarCoordenado(direcao);

    //arrumar essa mudança de direção
    direcaoAtual = direcao;

  }

}

// Função principal de virar coordenado
void virarCoordenado(Direcao direcao) {

  Serial.println("Estou Virando");

  // define objetivo conforme direção recebida
  if (direcao == FRENTE) { 
    anguloObjetivo = 0; 
  } else if (direcao == ESQUERDA) { 
    anguloObjetivo = 90; 
  } else if (direcao == DIREITA) { 
    anguloObjetivo = 180; 
  } else if (direcao == TRAS) { 
    anguloObjetivo = -90; 
  }


  //int erro = 30;

  // loop até atingir objetivo
  while (true) {
    atualizarAnguloZ_ComFiltro(); // mantém anguloZ atualizado
    float erro = erroDeRotacao();

    Serial.println("Girando");

    if (erro < 10) { 
      parar();
      break;
    }

    if (erro > 0) {
      passoEsquerda(500); // gira em passos pequenos
    } else {
      passoDireita(500);
    }

    delay(500);

    Serial.print("Erro:");
    Serial.println(erro);

    //erro = erro - 10;

  }
  Serial.println("Nova Direção Alcançada: ");
  direcaoAtual = direcao;
  Serial.println(direcaoAtual);


  

}


void mudarPosicaoAtual(){

  //atualizar posição atual, será chamado sempre que andar, só faz a lógica de atualizar a variavel

  switch(direcaoAtual) {

    //frente
    case FRENTE:

      posicaoAtualY++;    

    break;

    //esquerda
    case ESQUERDA:

      posicaoAtualX--;

    break;

    //direita
    case DIREITA:

      posicaoAtualX++;

    break;

    //tras
    case TRAS:

      posicaoAtualY--;

    break;

  }

}

void atualizarContadorHall() {
  int estadoAtual = digitalRead(pinoSensorHall);

  // Detecta borda HIGH → LOW (ímã aproximou)
  if (estadoAnteriorHall == HIGH && estadoAtual == LOW) {
    contadorPulsos++;
    Serial.print("Pulso detectado! Contagem: ");
    Serial.println(contadorPulsos);
  }

  estadoAnteriorHall = estadoAtual;
}
