// Pinos de direção dos motores (Ponte H)
const int IN1 = 5;
const int IN2 = 6;
const int IN3 = 9;
const int IN4 = 10;
// Pinos de velocidade (PWM)
const int ENA = 7;   // velocidade motor esquerdo
const int ENB = 8;   // velocidade motor direito

// Sensores infravermelhos (borda)
const int pinIR_Esq = 2;
const int pinIR_Dir = 3;
// Sensor ultrassônico (inimigo)
const int pinTrig = 11;
const int pinEcho = 12;

// Protótipos
void frente();
void recuar();
void virarEsquerda();
void procurar();
void parar();

// Variáveis
long duracao;
int distancia;
int linhaEsq, linhaDir;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(pinIR_Esq, INPUT);
  pinMode(pinIR_Dir, INPUT);
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  delay(3000); // Segurança
}

void loop() {
  linhaEsq = digitalRead(pinIR_Esq);
  linhaDir = digitalRead(pinIR_Dir);

  // Leitura ultrassônica
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  duracao = pulseIn(pinEcho, HIGH, 30000); // timeout p/ não travar o loop
  distancia = duracao * 0.034 / 2;

  if (linhaEsq == LOW || linhaDir == LOW) {
    recuar();
    delay(200);
    virarEsquerda();
    delay(200);
  }
  else if (distancia > 0 && distancia < 20) {
    frente();
  }
  else {
    procurar();
  }
}

// Motor esquerdo: IN1/IN2 + ENA | Motor direito: IN3/IN4 + ENB
void frente() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void recuar() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void procurar() {
  // Pivô sobre a esquerda: só roda direita gira
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 0);
  analogWrite(ENB, 120);
}

void virarEsquerda() {
  // Giro no próprio eixo: esquerda p/ trás, direita p/ frente
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void parar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}