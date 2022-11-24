/* Este módulo agrega funções para exibir mensagens no LCD no sistema de IoT Greener */

// Importa bibliotecas necessárias
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <string>

// Define pinos I2C do circuito. 
// Nas próximas sprints, pretendemos centralizar a definição desses pinos no código-fonte principal,
// para minimizar erros de pino
#define SDA_PIN 10
#define SLC_PIN 9

// Define dimensões do LCD
int lcdColumns = 16;
int lcdRows = 2;

// Cria objeto de LCD
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Inicializa o LCD
void startDisplay() {
  // Inicializa o I2C
  Wire.begin(SDA_PIN, SLC_PIN);
  // Inicializa o objeto de LCD
  lcd.init();
  // Liga o backlight do LCD
  lcd.backlight();
}

// Mostra rotina de boas-vindas
void welcome() {
  // Coloca cursor no início da 1ª linha e mostra primeira parte da mensagem
  lcd.setCursor(1, 0);
  lcd.print("Bem-vindo ao");
  
  // Coloca cursor na 2ª linha e mostra o resto da mensagem, tentando centralizar os caracteres
  lcd.setCursor(2, 1);
  lcd.print("< Greener >");
  
  // Espera 5 segundos
  delay(5000);
  // Limpa LCD
  lcd.clear();
}

// Mostra duas strings na primeira e segunda linhas do LCD, respectivamente
void displayMessage(String upper, String lower) {
  // Limpa LCD
  lcd.clear();
  
  // Coloca cursor no início da 1ª linha e mostra primeira parte da mensagem
  lcd.setCursor(0, 0);
  lcd.print(upper);
  
  // Coloca cursor no início da 2ª linha e mostra o resto da mensagem
  lcd.setCursor(0, 1);
  lcd.print(lower);
}