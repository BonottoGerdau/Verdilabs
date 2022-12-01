const int Pin_Red_Temp = 11;
const int Pin_Blue_Temp = 12;
const int Pin_Green_Temp = 13;
const int Pin_Red_Umi = 17;
const int Pin_Blue_Umi = 8;
const int Pin_Green_Umi = 46;

void temErro() {
 //  bool erroArmazenamento; ainda não implementado
  bool erroLeituraDisplay;
  bool erroLed;
  bool erroDisplay;
  bool erroSensor;
}

bool erroLed(){

}

bool erroSensor() {  // detecção o sensor conectado 
  Serial.begin(115200);
  Serial.println("Sensor de temperatura não conectado"); 
}

//bool // github, commit da Elisa


void setup() {
  // put your setup code here, to run once:
  pinMode(Pin_Red_Temp, OUTPUT);
  pinMode(Pin_Blue_Temp, OUTPUT);
  pinMode(Pin_Green_Temp, OUTPUT);
  pinMode(Pin_Red_Umi, OUTPUT);
  pinMode(Pin_Blue_Umi, OUTPUT);
  pinMode(Pin_Green_Umi, OUTPUT);
}

void loop() {
  //piscaLed('r', 't', 1);
  //piscaLed('r', 'u', 1);
  piscaLed('y', 't', 1);
  piscaLed('y', 'u', 1);
}


// Switch case que recebe um caractere r g ou b e acende
//o led na cor correspondente
void piscaLed(char ledColor, char reading, int estado){
  if (ledColor == 'r' && reading == 't' && estado == 1){
    analogWrite(Pin_Red_Temp, 0);
    analogWrite(Pin_Blue_Temp, 255);
    analogWrite(Pin_Green_Temp, 255);
    analogWrite(Pin_Red_Umi, 0);
    analogWrite(Pin_Blue_Umi, 255);
    analogWrite(Pin_Green_Umi, 255);
    delay(500);

    Serial.println("Erro grave!");
  }
  if (ledColor == 'r' && reading == 't' && estado == 0){
    analogWrite(Pin_Red_Temp, 255);
    analogWrite(Pin_Blue_Temp, 255);
    analogWrite(Pin_Green_Temp, 255);
    analogWrite(Pin_Red_Umi, 255);
    analogWrite(Pin_Blue_Umi, 255);
    analogWrite(Pin_Green_Umi, 255);
    delay(500);
    Serial.println("Erro na leitura ");
  } 
  if (ledColor == 'y' && reading == 't' && estado == 1){
    analogWrite(Pin_Red_Temp, 0);
    analogWrite(Pin_Blue_Temp, 255);
    analogWrite(Pin_Green_Temp, 0);
    analogWrite(Pin_Red_Umi, 0);
    analogWrite(Pin_Blue_Umi, 255);
    analogWrite(Pin_Green_Umi, 0);
    delay(500);
  }
  if(ledColor == 'y' && reading == 't' && estado == 0){
    analogWrite(Pin_Red_Temp, 255);
    analogWrite(Pin_Blue_Temp, 255);
    analogWrite(Pin_Green_Temp, 255);
    analogWrite(Pin_Red_Umi, 255);
    analogWrite(Pin_Blue_Umi, 255);
    analogWrite(Pin_Green_Umi, 255);
    delay(500); 
  }
}

/*void acendeLedUmi(char ledUmi, int estado){
  if (ledUmi == 'r' && estado == 1){

  
  if (ledUmi == 'y' && estado == 1){
    analogWrite(Pin_Red_Umi, 0);
    analogWrite(Pin_Blue_Umi, 255);
    analogWrite(Pin_Green_Umi, 0);
   }
  }
}
*/



















