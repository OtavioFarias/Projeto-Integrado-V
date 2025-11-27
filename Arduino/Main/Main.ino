#include "espComunicacao.h"
#include "motores.h"
#include "mpu.h"
#include "outros.h"
#include "ultrassonicos.h"
#include <EnableInterrupt.h>

int andarAutonomo = 1; //1 - anda sozinho, 0 - Controle via ESP

void setup() {
  
  //digitalWrite(16, HIGH);

  Serial.begin(9600);    // Serial para monitor

  inciarComunicacaoESP();
  enviarDadosESP("Comunicação ESP inicializada");

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

  enviarDadosESP(" MPU inicializado com sucesso");

  calibrarGyroZ();
  ultimoTempoMPU = micros();

  /*
  if (!mpu.testConnection()) {
    Serial.println("Erro: MPU6050 não encontrado!");
  }else{
  Serial.println("MPU6050 encontrado!");
  }
  */

  Serial.println("Setup Finalizado");

  

  enviarDadosESPparaFPGA(String("Atualizar_Mapa:") + " "  + String(5) + " "  + String(5) + " "  + String((int)(2)) + " "  + String(2) + " "  + String(0));

  esperarFPGA();
  


}

void loop() {

  atualizarAnguloZ_ComFiltro();

  Serial.println("To aqui");

  //virarCoordenado(1);
  
  receberDadosESP();

  /*
  if(debug == 1){
    enviarDadosESP("andarAutonomo" + andarAutonomo);
  }
  */
  
  if(andarAutonomo == 1){

    Serial.println("Andar Autonomo Ativado");
    andarAutomatico();
    //Serial.println("Andando");
    //frente();

    }
  else{

    andarESP();

  }


}