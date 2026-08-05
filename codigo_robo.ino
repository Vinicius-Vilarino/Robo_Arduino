/*
* VERSÃO REFINADA: EQUILÍBRIO DE TORQUE E FLUIDEZ
*/

// --- 1. MAPEAMENTO DE PINOS ---
const int pinoENA = 5;
const int pinoIN1 = 4;
const int pinoIN2 = 7;

const int pinoENB = 6;
const int pinoIN3 = 8;
const int pinoIN4 = 9;

const int Sensor_E = A2;
const int Sensor_C = A1;
const int Sensor_D = A0;

// --- 2. VELOCIDADES RECALIBRADAS (Menos velocidade, mais precisão) ---
const int velRetaEsq = 160; // Baixamos de 190 para 160
const int velRetaDir = 205; // Baixamos de 230 para 205 (Mantendo o boost de ~45 para o lado direito)
const int velSuaveInt = 110; // Roda de dentro na curva suave
const int velForteRe = 130; // Aumentamos a ré para 130 (Para "ancorar" o robô na curva fechada)

int leituraE, leituraC, leituraD, ultimaDirecao = 0;

void setup() {
pinMode(pinoENA, OUTPUT); pinMode(pinoIN1, OUTPUT); pinMode(pinoIN2, OUTPUT);
pinMode(pinoENB, OUTPUT); pinMode(pinoIN3, OUTPUT); pinMode(pinoIN4, OUTPUT);
pinMode(Sensor_E, INPUT); pinMode(Sensor_C, INPUT); pinMode(Sensor_D, INPUT);
Serial.begin(9600);
pararMotores();
delay(2000);
}

void loop() {
leituraE = digitalRead(Sensor_E);
leituraC = digitalRead(Sensor_C);
leituraD = digitalRead(Sensor_D);

// --- MARCHA 1: ALINHADO (Retas balanceadas) ---
if (leituraE == 1 && leituraC == 0 && leituraD == 1) {
moverFrente();
ultimaDirecao = 0;
}

// --- MARCHA 2: CORREÇÕES SUAVES ---
else if (leituraE == 1 && leituraC == 0 && leituraD == 0) { // Linha na Dir
virarSuaveDireita();
ultimaDirecao = 2;
}
else if (leituraE == 0 && leituraC == 0 && leituraD == 1) { // Linha na Esq
virarSuaveEsquerda();
ultimaDirecao = 1;
}

// --- MARCHA 3: CORREÇÕES FORTES ---
else if (leituraE == 0 && leituraC == 1 && leituraD == 1) { // Linha SÓ Esq
virarForteEsquerda();
ultimaDirecao = 1;
}
else if (leituraE == 1 && leituraC == 1 && leituraD == 0) { // Linha SÓ Dir
virarForteDireita();
ultimaDirecao = 2;
}

// --- 4. MODO BUSCA (Tudo Escuro) ---
else if (leituraE == 1 && leituraC == 1 && leituraD == 1) {
if (ultimaDirecao == 1) virarForteEsquerda();
else if (ultimaDirecao == 2) virarForteDireita();
else moverFrente();
}

// --- 5. ENCRUZILHADA (Tudo branco) ---
else if (leituraE == 0 && leituraC == 0 && leituraD == 0) {
moverFrente();
}
}

// =================================================================
// FUNÇÕES DE MOVIMENTO REFINADAS
// =================================================================

void pararMotores() {
digitalWrite(pinoIN1, LOW); digitalWrite(pinoIN2, LOW); analogWrite(pinoENA, 0);
digitalWrite(pinoIN3, LOW); digitalWrite(pinoIN4, LOW); analogWrite(pinoENB, 0);
}

void moverFrente() {
digitalWrite(pinoIN1, HIGH); digitalWrite(pinoIN2, LOW); analogWrite(pinoENA, velRetaEsq);
digitalWrite(pinoIN3, HIGH); digitalWrite(pinoIN4, LOW); analogWrite(pinoENB, velRetaDir);
}

// --- CURVAS PARA A ESQUERDA (Ajudando o Lado Direito Adaptado) ---
void virarSuaveEsquerda() {
digitalWrite(pinoIN1, HIGH); digitalWrite(pinoIN2, LOW); analogWrite(pinoENA, velSuaveInt);
digitalWrite(pinoIN3, HIGH); digitalWrite(pinoIN4, LOW); analogWrite(pinoENB, 255); // Turbo
}

void virarForteEsquerda() {
digitalWrite(pinoIN1, LOW); digitalWrite(pinoIN2, HIGH); analogWrite(pinoENA, velForteRe); // Ré suave
digitalWrite(pinoIN3, HIGH); digitalWrite(pinoIN4, LOW); analogWrite(pinoENB, 255); // Turbo
}

// --- CURVAS PARA A DIREITA (Lado Esquerdo é o forte) ---
void virarSuaveDireita() {
digitalWrite(pinoIN1, HIGH); digitalWrite(pinoIN2, LOW); analogWrite(pinoENA, 200);
digitalWrite(pinoIN3, HIGH); digitalWrite(pinoIN4, LOW); analogWrite(pinoENB, velSuaveInt);
}

void virarForteDireita() {
digitalWrite(pinoIN1, HIGH); digitalWrite(pinoIN2, LOW); analogWrite(pinoENA, 200);
digitalWrite(pinoIN3, LOW); digitalWrite(pinoIN4, HIGH); analogWrite(pinoENB, velForteRe); // Ré suave
}