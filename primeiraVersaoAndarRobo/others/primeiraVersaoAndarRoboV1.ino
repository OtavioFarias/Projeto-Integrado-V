// Ultrassônicos
#define TRIG 12 // vai ser o mesmo para os 4 ultrassônicos
#define ECHO_frente 2 // ultrassônico da frente que gera interrupcção
#define ECHO_tras 8 
#define ECHO_direita 9 
#define ECHO_esquerda 10 

// Motores - vamos usar a protoboard para ecomizar pinos do arduino
#define Motor_Direita_1 4
#define Motor_Direita_2 5
#define Motor_Esquerda__1 6
#define Motor_Esquerda__2 7

float distanciaParaVirar = 20;
float IntervaloMinimoEntreMedidaDistancia = 60;

volatile unsigned long tempoInicio = 0;
volatile unsigned long duracaoPulso = 0;
volatile bool medidaPronta = false;

unsigned long ultimoDisparo = 0;
float distanciaFrente;
float distanciaDireita; 
float distanciaEsquerda;
float distanciaTras;

// Movimento
enum Estado { ANDANDO, VIRANDO, PARADO };
Estado estado = ANDANDO;

unsigned long tempoAcao = 0;
int direcaoVirar = 0;  // 0=esquerda, 1=direita
unsigned long ultimoMovimentoAleatorio = 0;

const unsigned long DURACAO_GIRO = 500; // tempo aproximado para girar 90º (ms) mudar para considerar o giroscópio

void setup() {

  Serial.begin(9600);    // Para monitorar no PC
  Serial1.begin(9600);   // Comunicação com o ESP32 (pinos 18=TX1, 19=RX1)

  pinMode(Motor_Direita_1, OUTPUT); pinMode(Motor_Direita_2, OUTPUT);
  pinMode(Motor_Esquerda__1, OUTPUT); pinMode(Motor_Esquerda__2, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO_tras, INPUT);
  pinMode(ECHO_frente, INPUT);
  pinMode(ECHO_direita, INPUT);
  pinMode(ECHO_esquerda, INPUT);

  attachInterrupt(digitalPinToInterrupt(ECHO), capturaPulso, CHANGE);

  frente();
  

}

void loop() {
  unsigned long agora = millis();

  // Sensor
  if (agora - ultimoDisparo > IntervaloMinimoEntreMedidaDistancia) {
    disparaSensor();
    ultimoDisparo = agora;
  }

  if (temMedida()) {
    distancia = getDistancia();
  }

  // Obstáculo
  if (distancia < distanciaParaVirar && estado == ANDANDO) {
    decidirMudancaTrajeto()
  }


  // Executa ação atual
  if (estado == ANDANDO) {
    frente();
  } 
  else if (estado == VIRANDO) {
    if (direcaoVirar == 0) esquerda();
    else direita();

    if (agora >= tempoAcao) {
      estado = PARADO; 
      parar();
      tempoAcao = agora + 200; // tempo de parada antes de seguir em frente
    }
  }
  else if (estado == PARADO) {
    if (agora >= tempoAcao) {
      estado = ANDANDO;
      frente();
    }
  }
}

// ========================
// Funções do sensor
// ========================
void disparaSensor() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
}

void capturaPulso() {
  if (digitalRead(ECHO) == HIGH) {
    tempoInicio = micros();
  } else {
    duracaoPulso = micros() - tempoInicio;
    medidaPronta = true;
  }
}

bool temMedida() {
  return medidaPronta;
}

float getDistancia() {
  medidaPronta = false;
  return duracaoPulso / 58.0;
}

// ========================
// Funções dos motores
// ========================
void frente() {
  digitalWrite(Motor_Direita_1, HIGH); digitalWrite(Motor_Direita_2, LOW);
  digitalWrite(Motor_Esquerda__1, HIGH); digitalWrite(Motor_Esquerda__2, LOW);
}

void tras() {
  digitalWrite(Motor_Direita_1, LOW); digitalWrite(Motor_Direita_2, HIGH);
  digitalWrite(Motor_Esquerda__1, LOW); digitalWrite(Motor_Esquerda__2, HIGH);
}

void esquerda() {
  digitalWrite(Motor_Direita_1, LOW); digitalWrite(Motor_Direita_2, HIGH);
  digitalWrite(Motor_Esquerda__1, HIGH); digitalWrite(Motor_Esquerda__2, LOW);
}

void direita() {
  digitalWrite(Motor_Direita_1, HIGH); digitalWrite(Motor_Direita_2, LOW);
  digitalWrite(Motor_Esquerda__1, LOW); digitalWrite(Motor_Esquerda__2, HIGH);
}

void parar() {
  digitalWrite(Motor_Direita_1, LOW); digitalWrite(Motor_Direita_2, LOW);
  digitalWrite(Motor_Esquerda__1, LOW); digitalWrite(Motor_Esquerda__2, LOW);
}


// ========================
// Função para iniciar giro
// ========================
void iniciarGiro(int direcao) {
  estado = VIRANDO;
  direcaoVirar = direcao;
  tempoAcao = millis() + DURACAO_GIRO;
  parar(); // garante que comece do estado parado
}

void decidirMudancaTrajeto(){

  iniciarGiro(0); // 0=esquerda, 1=direita

}


// ==================================
// Funções para comunicação com o ESP
// ==================================

//TX1 (pino 18 no Mega) → RX do ESP32 (ex: GPIO16 se você usar RXD2)
//RX1 (pino 19 no Mega) ← TX do ESP32 (ex: GPIO17 se você usar TXD2)
//GND ↔ GND
//Divisor de tensão no fio Mega → ESP32 (pois Mega manda 5V e ESP32 é 3.3V)


void enviarDadosUltrassonicoESP(float distanciaFrente, float distanciaDireita, float distanciaEsquerda, float distanciaTras) {

  // Envia a variável para o ESP32
  Serial1.println(distanciaFrente);

  
  // Envia a variável para o ESP32
  Serial1.println(distanciaDireita);

  
  // Envia a variável para o ESP32
  Serial1.println(distanciaEsquerda);

  
  // Envia a variável para o ESP32
  Serial1.println(distanciaTras);

}
