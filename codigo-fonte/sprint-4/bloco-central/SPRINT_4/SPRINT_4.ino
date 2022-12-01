#include "sensor.h"
#include "display.h"
#include <WiFiManager.h>
#include "wi-fi.h"

#define greenTemp 13
#define blueTemp 12
#define redTemp 11
#define greenHumidity 46
#define blueHumidity 8
#define redHumidity 17

bool tempAlert = true;
bool humidityAlert = true;
bool tempDanger = true;
bool humidityDanger = true;
WiFiManager wm;

void checkSensor();
void connectWiFi();

void setup() {
  Serial.begin(115200);
  pinMode(redTemp, OUTPUT);
  pinMode(blueTemp, OUTPUT);
  pinMode(greenTemp, OUTPUT);
  pinMode(blueHumidity, OUTPUT);
  pinMode(redHumidity, OUTPUT);
  pinMode(greenHumidity, OUTPUT);
  setupDisplay();
  welcome();
  checkSensor();
  connectWiFi();
  checkLeds();
}

void loop() {
  
  float temp = getTemp();                                                // Pega temperatura atual
  float humidity = getHumidity();                                        // Pega umidade atual
  String tempMessage = "Temp: " + String(temp) + " " + (char)223 + "C";  // Cria mensagem de temperatura para LCD
  String humidityMessage = "Umidade: " + String(humidity) + "%";         // Cria mensagem de umidade para LCD

  sendReading(temp, humidity, 1);
  lightUpLeds(temp, humidity);
  displayMessage(tempMessage, humidityMessage);
  delay(4000);

  showAlerts(temp, humidity);
}

void connectWiFi() {
  wm.resetSettings();
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  displayMessage("Conecte-se a", "rede Greener");
  bool res;
  res = wm.autoConnect("Greener");  // password protected ap

  if (!res) {
    Serial.println("Failed to connect");
    displayMessage("Falha em", "conectar");
    delay(2000);
    ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("Conectado!");
    displayMessage("Conectado", "com sucesso");
    delay(2000);
  }
}

void showAlerts(float temp, float humidity) {
  if (temp < 28 * 0.95) {
    displayMessage("TEMP BAIXA!", "Fechar janelas");
  } else if (temp < 28) {
    displayMessage("Atencao!", "Temp baixando");
  } else if (temp > 36 * 1.1) {
    displayMessage("TEMP ALTA!", "Abrir todas");
  } else if (temp > 36 * 1.05) {
    displayMessage("TEMP ALTA!", "Laterais 100%");
  } else if (temp > 36 * 1.03) {
    displayMessage("TEMP ALTA!", "Laterais 50%");
  } else if (temp > 36) {
    displayMessage("Atencao!", "Temp aumentando");
  }
  delay(3000);

  // Se umidade estiver fora do desejado, mostra mensagem de alerta apropriada por três segundos
  if (humidity < 70 * 0.95) {
    displayMessage("PERIGO!", "Umidade baixa!");
  } else if (humidity < 70) {
    displayMessage("ATENÇAO!", "Umidade baixando!");
  } else if (humidity > 95) {
    displayMessage("PERIGO!", "Umidade alta!");
  }
  delay(3000);
}

void checkSystem() {
  scan();
  checkSensor();
  checkConnection();
}

void turnOnRGB(char state, char led = '0') {
  if (state == 'a') {
    analogWrite(redTemp, 0);
    analogWrite(blueTemp, 255);
    analogWrite(greenTemp, 255);
    analogWrite(redHumidity, 0);
    analogWrite(blueHumidity, 255);
    analogWrite(greenHumidity, 255);
    delay(500);
    analogWrite(redTemp, 255);
    analogWrite(blueTemp, 255);
    analogWrite(greenTemp, 255);
    analogWrite(redHumidity, 255);
    analogWrite(blueHumidity, 255);
    analogWrite(greenHumidity, 255);
    delay(500);
  } else {
    if (led == 't') {
      if (state == 'g') {
        analogWrite(redTemp, 255);
        analogWrite(blueTemp, 0);
        analogWrite(greenTemp, 255);
      } else if (state == 'y') {
        analogWrite(redTemp, 0);
        analogWrite(blueTemp, 255);
        analogWrite(greenTemp, 0);
      } else if (state == 'r') {
        analogWrite(redTemp, 0);
        analogWrite(blueTemp, 255);
        analogWrite(greenTemp, 255);
      } else if (state == '0') {
        analogWrite(redTemp, 255);
        analogWrite(blueTemp, 255);
        analogWrite(greenTemp, 255);
      }
    } else if (led == 'h') {
      if (state == 'g') {
        analogWrite(redHumidity, 255);
        analogWrite(blueHumidity, 0);
        analogWrite(greenHumidity, 255);
      } else if (state == 'y') {
        analogWrite(redHumidity, 0);
        analogWrite(blueHumidity, 255);
        analogWrite(greenHumidity, 0);
      } else if (state == 'r') {
        analogWrite(redHumidity, 0);
        analogWrite(blueHumidity, 255);
        analogWrite(greenHumidity, 255);
      } else if (state == '0') {
        analogWrite(redHumidity, 255);
        analogWrite(blueHumidity, 255);
        analogWrite(greenHumidity, 255);
      }
    }
  }
}

void lightUpLeds(float temp, float humidity) {
  tempDanger = false;
  tempAlert = false;
  humidityDanger = false;
  humidityAlert = false;

  turnOnRGB('0', 't');
  turnOnRGB('0', 'h');

  if (temp < 28 * 0.95) {
    tempDanger = true;
    turnOnRGB('r', 't');
  } else if (temp < 28) {
    turnOnRGB('y', 't');
  } else if (temp > 36 * 1.1) {
    tempDanger = true;
    turnOnRGB('r', 't');
  } else if (temp > 36) {
    turnOnRGB('y', 't');
  } else {
    turnOnRGB('g', 't');
  }

  if (humidity < 70 * 0.95) {
    turnOnRGB('r', 'h');
  } else if (humidity < 70) {
    turnOnRGB('y', 'h');
  } else if (humidity > 95) {
    turnOnRGB('r', 'h');
  } else {
    turnOnRGB('g', 'h');
  }
}

void checkSensor() {
  while (true) {
    if (setupSensor()) {
      setupDisplay();
      return;
    } else {
      displayMessage("Sensor não", "encontrado");
      turnOnRGB('a');
    }
  }
}

void checkLeds() {
  turnOnRGB('0', 't');
  turnOnRGB('0', 'h');

  displayMessage("Iniciando teste", "de LEDs");
  delay(3000);

  turnOnRGB('g', 't');
  displayMessage("TEMP [VERDE]", "    OPERANTE    ");
  delay(3000);
  turnOnRGB('0', 't');

  turnOnRGB('y', 't');
  displayMessage("TEMP [AMARELO]", "    OPERANTE    ");
  delay(3000);
  turnOnRGB('0', 't');

  turnOnRGB('r', 't');
  displayMessage("TEMP [VERMELHO]", "    OPERANTE    ");
  delay(3000);
  turnOnRGB('0', 't');

  turnOnRGB('g', 'h');
  displayMessage("UMI [VERDE]", "    OPERANTE    ");
  delay(3000);
  turnOnRGB('0', 'h');

  turnOnRGB('y', 'h');
  displayMessage("UMI [AMARELO]", "    OPERANTE    ");
  delay(3000);
  turnOnRGB('0', 'h');

  turnOnRGB('r', 'h');
  displayMessage("UMI [VERMELHO]", "    OPERANTE    ");
  delay(3000);
  turnOnRGB('0', 'h');
}