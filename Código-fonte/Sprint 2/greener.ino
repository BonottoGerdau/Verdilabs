/* 
Este é o código-fonte para ESP32-S3-Box de Greener, um assistente de monitoramento de temperatura e umidade para casas de vegetação.
Através da conexão com os módulos sensor.h e display.h, este programa checa a temperatura e umidade relativa do ar medidas por um sensr AHT10,
interpreta esses dados segundo regras do negócio e coordena feedbacks através de um display LCD e um código de cores.
Até o momento, não é possível alterar dinamicamente os parâmetros de temperatura e umidade ideias (faixas permitidas), isto é, esses
valores estão hardcoded, pois esta é apenas a primeira versão do código-fonte.
Ademais, as medidas estão sendo feitas a cada 3-9 segundos, dependendo das leituras obtidas, para fins de demonstração e teste.
Na versão final, o intervalo entre as medições será regular e definido pelo usuário.
Pretendemos oferecer essa funcionalidade nas próximas sprints.
*/


// Importa módulos criados para este projeto de sensor e display
#include "sensor.h"
#include "display.h"

// Define pinos dos leds de temperatura e umidade
#define greenTemp 41
#define yellowTemp 38
#define blueTemp 1
#define redTemp 35
#define greenHumidity 7
#define blueHumidity 6
#define redHumidity 5

// Variáveis para checar se temperatura e/ou umidade estão fora do desejado
bool tempIssueExists = true;
bool humidityIssueExists = true;

void checkTemp(float temp); // Brilha LEDs segundo valores de temperatura
void checkHumidity(float humidity); // Brilha LEDs segundo valores de umidade

// Inicialização do programa
void setup() {
  Serial.begin(115200); // Define velocidade de comunicação serial a 115200 bits por segundo, valor padrão do ESP32
  
  // Configura LEDs como output
  pinMode(redTemp, OUTPUT);
  pinMode(yellowTemp, OUTPUT);
  pinMode(greenTemp, OUTPUT);
  pinMode(blueTemp, OUTPUT);
  pinMode(blueHumidity, OUTPUT);
  pinMode(redHumidity, OUTPUT);
  pinMode(greenHumidity, OUTPUT);
  
  // Inicializa sensor
  startSensor();
  // Inicializa display
  startDisplay();
  
  // Inicia rotina de boas-vindas
  welcome();
}

void loop() {
  float temp = getTemp(); // Pega temperatura atual
  float humidity = getHumidity(); // Pega umidade atual

  String tempMessage = "Temp: " + String(temp) + " " + (char)223 + "C"; // Cria mensagem de temperatura para LCD
  String humidityMessage = "Umidade: " + String(humidity) + "%"; // Cria mensagem de umidade para LCD

  // Interpreta valores segundo as regras do negócio, flaggeando variáveis booleanas "issueExists" em caso afirmativo
  checkTemp(temp);
  checkHumidity(humidity);

  // Mostra medidas atuais no display
  displayMessage(tempMessage, humidityMessage);
  delay(3000); // Espera 3 segundos

  // Se temperatura estiver fora do desejado, mostra mensagem de alerta apropriada por três segundos
  if (tempIssueExists) {
    if (temp < 23) {
      displayMessage("Temp baixa", "Fechar janelas");
    } else if (temp > 46) {
      displayMessage("Temp muito alta", "Abra tudo");
    } else if (temp > 41) {
      displayMessage("Temp alta", "Laterais 100%");
    } else if (temp > 39) {
      displayMessage("Temp alta", "Laterais 50%");
    }
    delay(3000);
  }

  // Se umidade estiver fora do desejado, mostra mensagem de alerta apropriada por três segundos
  if (humidityIssueExists) {
    if (humidity < 65) {
      displayMessage("Umidade baixa");
    } else if (humidity > 95) {
      displayMessage("Umidade alta");
	}
    delay(3000);
  }
}

void checkTemp(float temp) {
  tempIssueExists = true; // Inicializa boolean como true, para que ela seja alterada para false se necessário
  
  // Apaga todos os leds de temperatura
  digitalWrite(redTemp, LOW);
  digitalWrite(blueTemp, LOW);
  digitalWrite(yellowTemp, LOW);
  digitalWrite(greenTemp, LOW);

  // Situação de baixa temperatura; brilha led azul
  if (temp < 23) {
    digitalWrite(redTemp, LOW);
    digitalWrite(blueTemp, HIGH);
    digitalWrite(yellowTemp, LOW);
    digitalWrite(greenTemp, LOW);
  } else if (temp > 46) { // Situação de altíssima temperatura; brilha led vermelho
    digitalWrite(redTemp, HIGH);
    digitalWrite(blueTemp, LOW);
    digitalWrite(yellowTemp, LOW);
    digitalWrite(greenTemp, LOW);
  } else if (temp > 36) { // Situação de temperatura no limite da tolerância; brilha led amarelo
    digitalWrite(redTemp, LOW);
    digitalWrite(blueTemp, LOW);
    digitalWrite(yellowTemp, HIGH);
    digitalWrite(greenTemp, LOW);
  } else { // Situação de temperatura apropriada; brilha led verde
    tempIssueExists = false; // Define booleana como falsa, pois não há problemas na temperatura
    digitalWrite(redTemp, LOW);
    digitalWrite(blueTemp, LOW);
    digitalWrite(yellowTemp, LOW);
    digitalWrite(greenTemp, HIGH);
  }
}

void checkHumidity(float humidity) {
  humidityIssueExists = true; // Inicializa boolean como true, para que ela seja alterada para false se necessário
  
  if (humidity < 65) { // Situação de umidade baixa; brilha led azul
    digitalWrite(redHumidity, LOW);
    digitalWrite(blueHumidity, HIGH);
    digitalWrite(greenHumidity, LOW);
  } else if (humidity < 95) { // Situação de umidade apropriada; brilha led verde
    digitalWrite(redHumidity, LOW);
    digitalWrite(blueHumidity, LOW);
    digitalWrite(greenHumidity, HIGH);
    humidityIssueExists = false;
  } else { // Situação de umidade elevada; brilha led vermelho
	digitalWrite(redHumidity, HIGH);
    digitalWrite(blueHumidity, LOW);
    digitalWrite(greenHumidity, LOW);
  }
}