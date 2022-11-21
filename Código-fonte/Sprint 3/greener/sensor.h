/* Este módulo controla o sensor de temperatura AHT10 para o código-fonte do sistema em IoT Greener */

// Importa bibliotecas necessárias
#include <Adafruit_AHTX0.h> // Biblioteca padrão do sensor
#include <Wire.h> // Biblioteca para I2C

// Define pinos de I2C do sistema
#define SDA_PIN 4
#define SLC_PIN 5

// Cria objeto de sensor
Adafruit_AHTX0 aht;

// Inicializa sensor
int startSensor() {
  Wire.begin(SDA_PIN, SLC_PIN); // Configura pinos de I2C do sistema
  // Checa se sensor foi localizado no circuito. Caso não, envia erro.
  if (!aht.begin()) {
    Serial.println("Sensor não encontrado"); 
	// Esta é uma medida provisória para demarcar o erro. Nas próximas sprints, pretendemos isto por um erro de fato visível ao usuário
	// final no LCD, através do redirecionamento de exceções e integração de bibliotecas
  }
  Serial.print("found seensor");
}

// Retorna temperatura medida pelo sensor
float getTemp() {
  sensors_event_t humidity, temp; // Cria objetos de temperatura e umidade
  aht.getEvent(&humidity, &temp);  // Atualiza valor das medições
  return temp.temperature; // Retorna valor da temperatura
}

// Retorna umidade medida pelo sensor
float getHumidity() {
  sensors_event_t humidity, temp; // Cria objetos de temperatura e umidade
  aht.getEvent(&humidity, &temp); // Atualiza valor das medições
  return humidity.relative_humidity; // Retorna valor da umidade
}
