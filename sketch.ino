// ==========================================================
// GLOBAL SOLUTION 1.2026
// Solução de sistema embarcado para monitoramento e gestão de Nanossatélite
//
// Disciplinas contempladas:
// COA (Computer Organization and Architecture)
// SERS (Soluções em Energias Renováveis e Sustentabilidade)
// ==========================================================

#include "DHTesp.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ==========================================================
// SERS - SISTEMA ENERGÉTICO
// ----------------------------------------------------------
// O LDR simula a incidência solar recebida por um painel
// fotovoltaico. Os valores obtidos são utilizados para
// estimar a potência gerada e alimentar o modelo de gestão
// energética do nanossatélite.
// ==========================================================

int ldrPin = 1;
float luz;
String statusSolar;

float potenciaGerada;
float capacidadeBateria = 1;
float energiaBateria = 0.1;
float saldoEnergetico;
float potenciaConsumida;
float eficiencia;
float deltaTempo = 0.2 / 3600;
float percentualBateria;
float autonomiaHoras;
bool alertSaldoEnergetico;
bool alertPercentualBateria;

int barraLedPins[] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42};
int barraLedQtdPins = 10;

// ==========================================================
// COA - MONITORAMENTO TÉRMICO
// ----------------------------------------------------------
// Variáveis utilizadas para aquisição, processamento e
// classificação dos dados de temperatura.
// ==========================================================

int dhtPin = 2;
float temp;
float tempAnterior;
float variacaoTemp;
String tendencia;
String statusTemperatura;
bool alertTemp;
bool alertTempVar;
bool mostrouTemp;
bool firstTempCycle = true;
DHTesp dhtSensor;

// ==========================================================
// COA - MONITORAMENTO DE VIBRAÇÃO E IMPACTOS
// ----------------------------------------------------------
// Utiliza o MPU6050 para detectar acelerações anormais,
// turbulências e possíveis impactos durante a missão.
// ==========================================================

int SDApin = 9;
int SCLpin = 10;
bool alertVibracao;
float aceleracaoTotal;
String vibracaoStatus;
Adafruit_MPU6050 mpu;

// ==========================================================
// VARIÁVEIS DO SISTEMA - ALERTAS E DISPLAY
// ==========================================================

int alertTempPin = 19;
int alertVibracaoPin = 20;
int alertSaldoEnergeticoPin = 21;
int alertPercentualBateriaPin = 18;

int paginaAtual;
unsigned long ultimaTrocaPagina;

LiquidCrystal_I2C lcd(0x27, 20, 4);

// ==========================================================
// INICIALIZAÇÃO DO SISTEMA
// ----------------------------------------------------------
// Configura sensores, display LCD, LEDs de alerta e demais
// periféricos do computador de bordo.
// ==========================================================

void setup() {

  
  pinMode(ldrPin, INPUT);
  pinMode(alertVibracaoPin, OUTPUT);
  pinMode(alertSaldoEnergeticoPin, OUTPUT);
  pinMode(alertPercentualBateriaPin, OUTPUT);
  pinMode(alertTempPin, OUTPUT);

  dhtSensor.setup(dhtPin, DHTesp::DHT22);

  Wire.setPins(SDApin, SCLpin);
  mpu.begin();

  for (int i = 0; i < barraLedQtdPins; i++) {
    pinMode(barraLedPins[i], OUTPUT);
  }

  Serial.begin(115200);
  
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("====================");
  lcd.setCursor(0, 1);
  lcd.print("      Iniciando     ");
  lcd.setCursor(0, 2);
  lcd.print("      Sistemas      ");
  lcd.setCursor(0, 3);
  lcd.print("====================");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====================");
  lcd.setCursor(0, 1);
  lcd.print(" Todos os Sistemas ");
  lcd.setCursor(0, 2);
  lcd.print("       Ativos       ");
  lcd.setCursor(0, 3);
  lcd.print("====================");
  delay(1000);
  lcd.clear();

}

// ==========================================================
// CICLO PRINCIPAL DE EXECUÇÃO
// ----------------------------------------------------------
// Executa continuamente:
// 1. Leitura dos sensores
// 2. Processamento dos dados
// 3. Gestão energética
// 4. Atualização de alertas
// 5. Atualização da interface visual
// ==========================================================

void loop() {

  painelSolar();

  temperatura();
  mpuLoop();

  calcularConsumo();
  atualizarEnergia();

  alertasBoard();

  gerenciarPaginas();
  delay(200);
  
}

// ==========================================================
// COA - PROCESSAMENTO DE ACELERAÇÃO
// ----------------------------------------------------------
// Calcula a aceleração resultante do MPU6050 e classifica
// o estado operacional em Normal, Anormal, Turbulência
// ou Impacto.
// ==========================================================

void mpuLoop() {

  sensors_event_t a, g, tempmpu;

  mpu.getEvent(&a, &g, &tempmpu);

  aceleracaoTotal = sqrt(
    a.acceleration.x * a.acceleration.x +
    a.acceleration.y * a.acceleration.y +
    a.acceleration.z * a.acceleration.z
  );

  if (aceleracaoTotal > 20) {
    alertVibracao = true;
    vibracaoStatus = "Impacto";
  }
  else if (aceleracaoTotal > 16) {
      alertVibracao = true;
      vibracaoStatus = "Turbulencia";
  }
  else if (aceleracaoTotal >= 12) {
      alertVibracao = true;
      vibracaoStatus = "Anormal";
  }
  else {
      alertVibracao = false;
      vibracaoStatus = "Normal";
  }

}

// ==========================================================
// COA - PROCESSAMENTO TÉRMICO
// ----------------------------------------------------------
// Realiza a leitura do DHT22, calcula tendências e
// classifica o estado operacional da temperatura.
// ==========================================================

void temperatura() {

  tempAnterior = temp;
  temp = dhtSensor.getTemperature();

  if (temp > tempAnterior) {
    tendencia = "Subindo";
    variacaoTemp = temp - tempAnterior;
    mostrouTemp = false;
  } else if (temp < tempAnterior) {
    tendencia = "Descendo";
    variacaoTemp = tempAnterior - temp;
    mostrouTemp = false;
  } else if (mostrouTemp == true) {
    tendencia = "Estavel";
    mostrouTemp = false;
    variacaoTemp = 0;
  }

  if (variacaoTemp > 10 and !firstTempCycle) {
    alertTempVar = true;
  } else {
    alertTempVar = false;
  }

  if (firstTempCycle == true) {
    tempAnterior = temp;
    variacaoTemp = 0;
    firstTempCycle = false;
    tendencia = "Estavel";
  }

  if (temp < 0) {
  statusTemperatura = "CRITICO";
  alertTemp = true;
  }
  else if (temp < 10) {
    statusTemperatura = "ALERTA";
    alertTemp = true;
  }
  else if (temp <= 35) {
    statusTemperatura = "OPERACIONAL";
    alertTemp = false;
  }
  else if (temp <= 50) {
    statusTemperatura = "ATENCAO";
    alertTemp = true;
  }
  else {
    statusTemperatura = "CRITICO";
    alertTemp = true;
  }

}

// ==========================================================
// COA + SERS - SISTEMA DE ALERTAS
// ----------------------------------------------------------
// Responsável por sinalizar visualmente condições críticas
// relacionadas aos sensores e ao subsistema energético.
// ==========================================================

void alertasBoard() {

  if (alertTemp == true) {
    digitalWrite(alertTempPin, HIGH);
  } else  {
    digitalWrite(alertTempPin, LOW);
  }

  if (alertVibracao) {
    digitalWrite(alertVibracaoPin, HIGH);
  } else {
    digitalWrite(alertVibracaoPin, LOW);
  }

  if (alertSaldoEnergetico) {
    digitalWrite(alertSaldoEnergeticoPin, HIGH); 
  } else {
    digitalWrite(alertSaldoEnergeticoPin, LOW);
  }

  if (alertPercentualBateria) {
    digitalWrite(alertPercentualBateriaPin, HIGH);
  } else {
    digitalWrite(alertPercentualBateriaPin, LOW);
  }

}

// ==========================================================
// SERS - SIMULAÇÃO DE GERAÇÃO FOTOVOLTAICA
// ----------------------------------------------------------
// Converte a luminosidade medida pelo LDR em uma estimativa
// de potência elétrica gerada pelo painel solar.
// Também classifica as condições orbitais simuladas.
// ==========================================================

void painelSolar() {

  // Simula a incidência de luz solar sobre o painel fotovoltaico
  luz = analogRead(ldrPin);

  // Quanto menor o valor do LDR, maior a incidência solar
  potenciaGerada = ((8126 - luz) / 8126) * 10;

  if (luz >= 6000) {
    statusSolar = "ECLIPSE ORBITAL";
  }
  else if (luz >= 3000) {
    statusSolar = "TRANSICAO";
  }
  else {
    statusSolar = "SOL PLENO";
  }

}

// ==========================================================
// SERS - ESTIMATIVA DE CONSUMO ENERGÉTICO
// ----------------------------------------------------------
// Calcula a potência consumida pelo sistema considerando
// consumo base, sensores e alertas ativos.
// ==========================================================

void calcularConsumo() {

  // SIMULAÇÃO de consumo energético do módulo (em W).

  float consumoBase = 5.0;
  float consumoSensores = 2.0;
  float consumoAlertas = 0;

  if (alertTemp) consumoAlertas += 0.7;
  if (alertVibracao) consumoAlertas +=0.7;

  potenciaConsumida = consumoBase + consumoAlertas + consumoSensores;
}

// ==========================================================
// SERS - GESTÃO DE ENERGIA E BATERIA
// ----------------------------------------------------------
// Calcula:
// - Saldo energético
// - Energia armazenada (Wh)
// - Percentual da bateria
// - Autonomia estimada
//
// Esta função concentra os principais requisitos da
// disciplina de Energias Renováveis.
// ==========================================================

void atualizarEnergia() {
  
  // SIMULAÇÃO de atualização de dados relacionados a energia do módulo.

  saldoEnergetico = potenciaGerada - potenciaConsumida;

  energiaBateria += saldoEnergetico * deltaTempo;

  if (energiaBateria > capacidadeBateria) energiaBateria = capacidadeBateria;
  if (energiaBateria < 0) energiaBateria = 0;

  percentualBateria = (energiaBateria / capacidadeBateria) * 100;
  autonomiaHoras = energiaBateria / potenciaConsumida;

  int ledsAcesos = (percentualBateria * barraLedQtdPins) / 100;

  for (int i = 0; i < barraLedQtdPins; i++) {
    if (i < ledsAcesos) {
      digitalWrite(barraLedPins[i], HIGH);
    } else {
      digitalWrite(barraLedPins[i], LOW);
    }
  }

  alertSaldoEnergetico = (saldoEnergetico < 0) ? true : false;
  alertPercentualBateria = (percentualBateria < 20) ? true : false;

}

// ==========================================================
// INTERFACE / DISPLAY
// ----------------------------------------------------------
// Controla a navegação automática entre as páginas do LCD.
// ==========================================================

void gerenciarPaginas() {
  if (millis() - ultimaTrocaPagina >= 5000) {
    
    paginaAtual++;

    if (paginaAtual > 6) {
      paginaAtual = 0;
    }

    ultimaTrocaPagina = millis();

    atualizarDisplay();
  }
}

// ==========================================================
// APRESENTAÇÃO DOS DADOS
// ----------------------------------------------------------
// Exibe os resultados processados facilitando a análise do 
// comportamento do sistema.
// ==========================================================

void atualizarDisplay() {

  lcd.clear();
  lcd.setCursor(0, 0);

  switch (paginaAtual) {

  case 0: { // ALERTAS

    lcd.print("===== ALERTAS =====");

    if (!alertTemp && !alertVibracao) {

      lcd.setCursor(0,1);
      lcd.print("Status Geral:");

      lcd.setCursor(0,2);
      lcd.print("Sem Alertas");

    } else {

      if (alertTemp) {
        lcd.setCursor(0,1);
        lcd.print("Temp: ");
        lcd.print(statusTemperatura);
      }

      if (alertVibracao) {
        lcd.setCursor(0,2);
        lcd.print("Vib: ");
        lcd.print(vibracaoStatus);
      }
    }

    break;
  }

  case 1: { // TEMPERATURA

    lcd.print("=== TEMPERATURA ===");

    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(String(temp,1));
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0,2);
    lcd.print("Var: ");
    lcd.print(String(variacaoTemp,1));
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0,3);
    lcd.print(tendencia);

    mostrouTemp = true;

    break;
  }

  case 2: { // PAINEL SOLAR

    lcd.print("====== SOLAR ======");

    lcd.setCursor(0,1);
    lcd.print("LDR: ");
    lcd.print((int)luz);

    lcd.setCursor(0,2);
    lcd.print("Pot: ");
    lcd.print(String(potenciaGerada,1));
    lcd.print("W");

    lcd.setCursor(0,3);
    lcd.print(statusSolar);

    break;
  }

  case 3: { // ACELERACAO

    lcd.print("=== ACELERACAO ===");

    lcd.setCursor(0,1);
    lcd.print("Acel: ");
    lcd.print(String(aceleracaoTotal,1));
    lcd.print("m/s2");

    lcd.setCursor(0,3);

    if (alertVibracao) {
      lcd.print(vibracaoStatus);
    } else {
      lcd.print("Normal");
    }

    break;
  }

  case 4: { // ENERGIA

    lcd.print("===== ENERGIA =====");

    lcd.setCursor(0,1);
    lcd.print("Ger:");
    lcd.print(String(potenciaGerada,1));
    lcd.print("W");

    lcd.setCursor(11,1);
    lcd.print("Con:");
    lcd.print(String(potenciaConsumida,1));
    lcd.print("W");

    lcd.setCursor(0,2);
    lcd.print("Saldo:");

    if (saldoEnergetico >= 0) {
      lcd.print("+");
    }

    lcd.print(String(saldoEnergetico,1));
    lcd.print("W");

    lcd.setCursor(0,3);

    if (saldoEnergetico > 0) {
      lcd.print("Carregando");
    }
    else if (saldoEnergetico < 0) {
      lcd.print("Descarregando");
    }
    else {
      lcd.print("Equilibrado");
    }

    break;
  }

  case 5: { // BATERIA

    lcd.print("===== BATERIA =====");

    lcd.setCursor(0,1);
    lcd.print("Carga: ");
    lcd.print(String(percentualBateria,0));
    lcd.print("%");

    lcd.setCursor(0,2);
    lcd.print(String(energiaBateria,1));
    lcd.print("/");
    lcd.print(String(capacidadeBateria,0));
    lcd.print("Wh");

    lcd.setCursor(0,3);

    if (percentualBateria > 60) {
      lcd.print("Modo: NORMAL");
    }
    else if (percentualBateria > 30) {
      lcd.print("Modo: ECONOMIA");
    }
    else {
      lcd.print("Modo: CRITICO");
    }

    break;
  }

  case 6: { // AUTONOMIA

    lcd.print("==== AUTONOMIA ====");

    lcd.setCursor(0,1);
    lcd.print("Restante:");

    lcd.setCursor(0,2);
    lcd.print(String(autonomiaHoras,1));
    lcd.print(" horas");

    lcd.setCursor(0,3);

    if (autonomiaHoras > 24) {
      lcd.print("Longa Duracao");
    }
    else if (autonomiaHoras > 8) {
      lcd.print("Operacao Normal");
    }
    else {
      lcd.print("Recarga Necess.");
    }

    break;
  }
}
}