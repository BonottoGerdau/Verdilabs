#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

#define SDA 5
#define SCL 4

int setupSensor() 
{
  Wire.begin(SDA, SCL);
  Serial.println("Adafruit AHT10/AHT20 demo!");
 
  if (! aht.begin()) 
  {
    Serial.println("Could not find AHT? Check wiring");
    return 0;
  }
  Serial.println("AHT10 or AHT20 found");
  return 1;
}

// Retorna umidade medida pelo sensor
float getHumidity() {
  sensors_event_t humidity, temp; // Cria objetos de temperatura e umidade
  aht.getEvent(&humidity, &temp); // Atualiza valor das medições
  return humidity.relative_humidity; // Retorna valor da umidade
}

float getTemp() {
  sensors_event_t humidity, temp; // Cria objetos de temperatura e umidade
  aht.getEvent(&humidity, &temp);  // Atualiza valor das medições
  return temp.temperature; // Retorna valor da temperatura
}

 