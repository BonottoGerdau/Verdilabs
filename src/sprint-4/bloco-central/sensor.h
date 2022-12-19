#include <Wire.h>            // Biblioteca para I2C
#include <Arduino.h>         // Biblioteca para utilizar Serial e delay
#include <Adafruit_AHTX0.h>  // Biblioteca para utilizar sensor AHT10

Adafruit_AHTX0 aht;  // Inicializa objeto para o sensor

// Define pinos I2C
#define SDA 5
#define SCL 4

// Esta função inicializa o sensor e retorna verdadeiro se ele for localizado com sucesso; falso caso contrário
int setupSensor() {
  Wire.begin(SDA, SCL);  // Inicia comunicação I2C
  if (!aht.begin()) {    // Tenta localizar sensor. Se não conseguir, exibe mensagem de erro no monitor e retorna 0.
    Serial.println("Sensor não encontrado");
    return 0;
  }
  // Se conseguir, exibe mensagem de sucesso e retorna 1
  Serial.println("Sensor encontrado");
  return 1;
}

// Retorna umidade medida pelo sensor
float getHumidity() {
  sensors_event_t humidity, temp;     // Cria objetos de temperatura e umidade
  aht.getEvent(&humidity, &temp);     // Atualiza valor das medições
  return humidity.relative_humidity;  // Retorna valor da umidade
}

float getTemp() {
  sensors_event_t humidity, temp;  // Cria objetos de temperatura e umidade
  aht.getEvent(&humidity, &temp);  // Atualiza valor das medições
  return temp.temperature;         // Retorna valor da temperatura
}