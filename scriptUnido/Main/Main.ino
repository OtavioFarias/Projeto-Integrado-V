#include "espComunicacao.h"
#include "motores.h"
#include "mpu.h"
#include "outros.h"
#include "ultrassonicos.h"

int andarAutonomo = 0; //1 - anda sozinho 0 - Controle via ESP

void setup() {
  
  Serial.begin(9600);    // Serial para monitor

  inciarComunicacaoESP();

  iniciarMotores();
  //frente();
  parar();

}

void loop() {
  // put your main code here, to run repeatedly:

  receberDadosESP();

  if(andarAutonomo == 1){andarAutomatico();}
  else{andarESP();}

}

