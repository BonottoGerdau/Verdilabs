const int pin_Red = 16;
const int pin_Yellow = 15;
bool erroSensor;
bool erroMedicaoDisplay;
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

bool erroSensor() { 

  if (!aht.begin()) {  
    Serial.println("Sensor não conectado");
    return 1;
  }
  Serial.println("Sensor conectado");
  return 0;

}

bool erroMedicaoDisplay() {
  if 
}



void temErro() {
  erroSensor = erroSensor();
  erroMedicaoDisplay = erroMedicaoDisplay(); 
}

int checaErro() {
  temErro();
  if(erroSensor || erroMedicaoDisplay) {

  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Sensor de temperatura não conectado");
  pinMode(pin_Red, OUTPUT);
  pinMode(pin_Yellow, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  piscaLed('r', 't', 1);
  piscaLed('r', 'u', 1);
  piscaLed('y', 't', 1);
  piscaLed('y', 'u', 1);
}

void piscaLed(char ledColor, char reading, int estado) {
  if (ledColor == 'r', reading == 't', estado == 1) {
    digitalWrite(pin_Red, HIGH);
    delay(500);
    digitalWrite(pin_Red, LOW);
    delay(500);
    digitalWrite(pin_Yellow, HIGH);
    delay(250);
    digitalWrite(pin_Yellow, LOW);
    delay(250);
  }
}