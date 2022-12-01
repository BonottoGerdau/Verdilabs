#include "sensor.h"
#include "display.h"
#include <WiFiManager.h>
#include "wi-fi.h"
#include <iostream>

#define wifiButton 1
#define greenTemp 13
#define yellowTemp 12
#define redTemp 11
#define greenHumidity 46
#define yellowHumidity 8
#define redHumidity 18


 using namespace std;
 // Driver code 
 int main()
 {
 const char *s = "1234"; 
 int x; 
 sscanf(s, "%d", &x);
 cout << "\nThe integer value of x : " << x;
 return 0;
 }



bool tempAlert = true;
bool humidityAlert = true;
bool tempDanger = true;
bool humidityDanger = true;
WiFiManager wm;

void checkSensor();

void setup() {
  Serial.begin(115200);
  pinMode(wifiButton, INPUT_PULLUP);
  pinMode(redTemp, OUTPUT);
  pinMode(yellowTemp, OUTPUT);
  pinMode(greenTemp, OUTPUT);
  pinMode(yellowHumidity, OUTPUT);
  pinMode(redHumidity, OUTPUT);
  pinMode(greenHumidity, OUTPUT);
  Serial.println("#########AQUI#########");
  setupDisplay();
  welcome();
  checkSensor();

  wm.resetSettings();
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  displayMessage("Conecte-se a", "rede Greener");
  bool res;
  res = wm.autoConnect("Greener", "verdilabs");  // password protected ap

  if (!res) {
    Serial.println("Failed to connect");
    displayMessage("Falha em", "conectar");
    delay(2000);
    ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    displayMessage("Conectado", "com sucesso");
    delay(2000);
  }
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

void lightUpLeds(float temp, float humidity) {
  tempDanger = false;
  tempAlert = false;
  humidityDanger = false;
  humidityAlert = false;

  digitalWrite(redTemp, LOW);
  digitalWrite(yellowTemp, LOW);
  digitalWrite(greenTemp, LOW);
  digitalWrite(redHumidity, LOW);
  digitalWrite(yellowHumidity, LOW);
  digitalWrite(greenHumidity, LOW);

  if (temp < 28 * 0.95) {
    tempDanger = true;
    digitalWrite(redTemp, HIGH);
  } else if (temp < 28) {
    digitalWrite(yellowTemp, HIGH);
  } else if (temp > 36 * 1.1) {
    tempDanger = true;
    digitalWrite(redTemp, HIGH);
  } else if (temp > 36) {
    digitalWrite(yellowTemp, HIGH);
  } else {
    digitalWrite(greenTemp, HIGH);
  }

  if (humidity < 70 * 0.95) {
    digitalWrite(redHumidity, HIGH);
  } else if (humidity < 70) {
    digitalWrite(yellowHumidity, HIGH);
  } else if (humidity > 95) {
    digitalWrite(redHumidity, HIGH);
  } else {
    digitalWrite(greenHumidity, HIGH);
  }
}

// função: erro grave 1 - temp e humi estão lidos de forma equívocada 

void erro1 (float temp, float humidity) {
  if () {
    displayMessage("Leitura errada");
  } 
  IF(ISNUMBER(erro1); "Leitura errada");

  delay(3000);









  if (! aht.begin()) // 
  {
    Serial.println("Sensor desconectado, tente novamente.");
    return 0;
  }
  Serial.println("AHT10 conectado");
  return 1;













void checkSensor() {
  while (true) {
    if (setupSensor()) {
      displayMessage("Sensor foi", "encontrado");
      delay(2000);
      break;
    } else {
      displayMessage("Sensor não", "encontrado");
      delay(2000);
    }
  }
}

void checkLeds() {
  displayMessage("Iniciando teste", "de LEDs");
  delay(3000);

  digitalWrite(greenTemp, HIGH);
  displayMessage("TEMP [VERDE]", "    OPERANTE    ");
  delay(3000);
  digitalWrite(greenTemp, LOW);

  digitalWrite(yellowTemp, HIGH);
  displayMessage("TEMP [AMARELO]", "    OPERANTE    ");
  delay(3000);
  digitalWrite(yellowTemp, LOW);

  digitalWrite(redTemp, HIGH);
  displayMessage("TEMP [VERMELHO]", "    OPERANTE    ");
  delay(3000);
  digitalWrite(redTemp, LOW);

  digitalWrite(greenHumidity, HIGH);  
  displayMessage("UMI [VERDE]", "    OPERANTE    ");
  delay(3000);
  digitalWrite(greenHumidity, LOW);

  digitalWrite(yellowHumidity, HIGH);
  displayMessage("UMI [AMARELO]", "    OPERANTE    ");
  delay(3000);
  digitalWrite(yellowHumidity, LOW);

  digitalWrite(redHumidity, HIGH);
  displayMessage("UMI [VERMELHO]", "    OPERANTE    ");
  delay(3000);
  digitalWrite(redHumidity, LOW);
}