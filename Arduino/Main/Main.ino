#include "espComunicacao.h"
#include "motores.h"
#include "mpu.h"
#include "outros.h"
#include "ultrassonicos.h"
#include <EnableInterrupt.h>

int andarAutonomo = 1; //1 - anda sozinho, 0 - Controle via ESP

void setup() {
  
  Serial.begin(9600);    // Serial para monitor

  inciarComunicacaoESP();

  iniciarMotores();
  //frente();
  //passoFrente(500);

  pinMode(TRIG_FRENTE, OUTPUT);
  pinMode(ECHO_FRENTE, INPUT);

  pinMode(TRIG_ESQUERDA, OUTPUT);
  pinMode(ECHO_ESQUERDA, INPUT);

  pinMode(TRIG_DIREITA, OUTPUT);
  pinMode(ECHO_DIREITA, INPUT);

  pinMode(pinoSensorHall, INPUT_PULLUP);

  Wire.begin();
  mpu.initialize();

  calibrarGyroZ();
  ultimoTempoMPU = micros();


  /*
  if (!mpu.testConnection()) {
    Serial.println("Erro: MPU6050 não encontrado!");
  }else{
  Serial.println("MPU6050 encontrado!");
  }

  */
  //frente();

}

void loop() {

  atualizarAnguloZ_ComFiltro();
  
  receberDadosESP();

  if(debug){
    enviarDadosESP("andarAutonomo" + andarAutonomo);
  }

  if(andarAutonomo == 1){

    //Serial.println("Andar Autonomo Ativado");
    andarAutomatico();
    //Serial.println("Andando");
    //frente();

    }
  else{

    andarESP();

  }

}

