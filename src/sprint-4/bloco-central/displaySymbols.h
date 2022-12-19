// Este módulo guarda os arrays de bytes para exibir ícones de Wi-Fi, termômetro e umidade no LCD. Ainda não foi implementado no código principal.

// Símbolo de wifi
byte wifiSymbol[8] = {
  B00000,
  B01110,
  B10001,
  B00000,
  B00100,
  B01010,
  B00000,
  B00100,
};

// Símbolo do termômetro
extern byte thermometerSymbol[8] = {
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

// Símbolo da umidade
extern byte humiditySymbol[8] = {
    B00100,
    B00100,
    B01010,
    B01010,
    B10001,
    B10001,
    B10001,
    B01110,
};