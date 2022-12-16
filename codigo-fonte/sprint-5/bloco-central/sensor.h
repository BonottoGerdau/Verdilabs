#include <Wire.h>            // Biblioteca para I2C
#include <Arduino.h>         // Biblioteca para utilizar Serial e delay
#include <Adafruit_AHTX0.h>  // Biblioteca para utilizar sensor AHT10

Adafruit_AHTX0 aht;  // Inicializa objeto para o sensor

// Define pinos I2C
#define SDA 4
#define SCL 5

// Esta função inicializa o sensor e retorna verdadeiro se ele for localizado com sucesso; falso caso contrário
bool setupSensor() {
  Wire.begin(SDA, SCL);  // Inicia comunicação I2C
  return aht.begin();
}

// Retorna umidade medida pelo sensor
float getHumidity() {
  sensors_event_t humidity, temp;     // Cria objetos de temperatura e umidade
  aht.getEvent(&humidity, &temp);     // Atualiza valor das medições
  return humidity.relative_humidity;  // Retorna valor da umidade
}

// Retorna temperatura medida pelo sensor
float getTemp() {
  sensors_event_t humidity, temp;  // Cria objetos de temperatura e umidade
  aht.getEvent(&humidity, &temp);  // Atualiza valor das medições
  return temp.temperature;         // Retorna valor da temperatura
}