#include "espComunicacao.h"
#include "motores.h"
#include "mpu.h"
#include "outros.h"
#include "ultrassonicos.h"

int andarAutonomo = 1; //1 - anda sozinho, 0 - Controle via ESP

void setup() {
  
  Serial.begin(9600);    // Serial para monitor

  inciarComunicacaoESP();

  iniciarMotores();
  //frente();
  //passoFrente(500);

  pinMode(TRIG_FRENTE, OUTPUT);
  pinMode(ECHO_FRENTE, INPUT);

  // se quiser usar interrupção:
  attachInterrupt(digitalPinToInterrupt(ECHO_FRENTE), capturaPulso, CHANGE);

  Wire.begin();
  mpu.initialize();
  Serial.print("2");
  if (!mpu.testConnection()) {
    Serial.println("Erro: MPU6050 não encontrado!");
  }else{
  Serial.println("MPU6050 encontrado!");
  }


  frente();

}

void loop() {
  // put your main code here, to run repeatedly:

  //passoFrente(500);

  //receberDadosESP();


  if(andarAutonomo == 1){

    andarAutomatico();
    
    }
  else{

    andarESP();

  }

}

