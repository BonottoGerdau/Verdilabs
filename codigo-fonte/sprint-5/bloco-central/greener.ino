/* 
Este é o código-fonte para ESP32-S3-Box de Greener, um assistente de monitoramento de temperatura e umidade para casas de vegetação.
Através da conexão com os módulos sensor.h e display.h, este programa checa a temperatura e umidade relativa do ar medidas por um sensor AHT10
a cada 10 segundos, interpreta esses dados segundo regras do negócio e coordena feedbacks através de um display LCD e um código de cores em seis LEDs.
Essas medidas são enviadas a cada 10 segundos. Por fim, o displaySymbols.h guarda o mapeamento dos ícones de temperatura e umidade para serem exibidos no display; por ora, 
isso ainda não está implementado no código principal, mas já temos a biblioteca pronta para ser utilizada na próxima sprint.

Nesta Sprint 4, alcançamos os seguintes objetivos, cumprindo os requisitos da Adalove:

1. Conexão de múltiplos ESPs com o servidor externo;
2. Servidor colocado em nuvem (Render);
3. Implementação de rotina de erros e verificação de periféricos para sensor e conexão Wi-Fi.

Para a última sprint, pretendemos desenvolver as seguintes metas:
1. Detecção do timestamp local no momento de coleta das medidas de temperatura e umidade, com exibição no LCD e envio do mesmo para o banco de dados
2. Salvamento automático dos dados no armazenamento interno do ESP no caso de falta de conexão;
3. Possibilidade de alterar os parâmetros de temperatura e umidade ideais, além de modificar o intervalo entre medições;
4. Adicionar detecção de LCD através da varredura de I2C para a rotina de erros.
*/

// Importação dos módulos necessários. Não foi possível modularizar mais devido à necessidade de se utilizar o LCD nas funções deste arquivo,
// pois não é possível importar a mesma biblioteca em mais de uma aba. Logo, tudo que exigia LCD precisou ser concentrado neste .ino.
#include "sensor.h"       // Inicializa e coordena leituras de temperatura e umidade
#include "display.h"      // Inicializa e coordena exibição de mensagens e rotinas no LCD
#include <WiFiManager.h>  // Gerencia configuração de Wi-Fi através de página Web e hotspot
#include "wi-fi.h"        // Módulo para coordenar envio de requisições HTTP
#include "i2cScanner.h"
#include "data.h"

// Definição de pinos do RGB de feedback para temperatura
#define greenTemp 13
#define blueTemp 12
#define redTemp 11

// Definição de pinos do RGB de feedback para temperatura
#define greenHumidity 46
#define blueHumidity 8
#define redHumidity 17

// Variáveis que guardam estado atual do sistema para exibição (ou não) de alertas e mensagens no display
bool tempAlert = true;       // Referente a temperatura no extremo da zona de tolerância
bool humidityAlert = true;   // Referente a umidade no extremo da zona de tolerância
bool tempDanger = true;      // Referente a temperatura fora da zona de tolerância
bool humidityDanger = true;  // Referente a umidade fora da zona de tolerância

void checkSensor();  // Esta função checa se o sensor está funcionando e exibe o resultado no LCD
void connectWiFi();  // Esta função gerencia a configuração de Wi-Fi para conexão
void checkSystem();  // Esta função gerencia o processo de detecção e tratamento de erros no sistema

WiFiManager wm;  // Cria objeto para configuração de Wi-Fi

void setup() {
  Serial.begin(115200);  // Define velocidade da comunicação serial com monitor segundo o padrão do ESP-32
  // Inicializa os pinos dos RGBs
  pinMode(redTemp, OUTPUT);
  pinMode(blueTemp, OUTPUT);
  pinMode(greenTemp, OUTPUT);
  pinMode(blueHumidity, OUTPUT);
  pinMode(redHumidity, OUTPUT);
  pinMode(greenHumidity, OUTPUT);
  setupDisplay();  // Inicializa display LCD
  welcome();       // Exibe boas-vindas no LCD
  checkSensor();   // Checa se o sensor está funcionando
  connectWiFi();   // Faz conexão com o Wi-Fi
  initCurrentParameters();
  Serial.print("temp max: ");
  Serial.println(tempMax);
  Serial.print("temp min: ");
  Serial.println(tempMin);
  Serial.print("h max: ");
  Serial.println(humidityMax);
  Serial.print("h min: ");
  Serial.println(humidityMin);
  checkLeds();     // Realiza teste de cores nos RGB
}

void loop() {
  checkSystem();                                                         // Faz varredura por erros no sistema
  float temp = getTemp();                                                // Pega temperatura atual
  float humidity = getHumidity();                                        // Pega umidade atual
  String tempMessage = "Temp: " + String(temp) + " " + (char)223 + "C";  // Cria mensagem de temperatura para LCD
  String humidityMessage = "Umidade: " + String(humidity) + "%";         // Cria mensagem de umidade para LCD

  sendReading(temp, humidity, 1);                // Envia medidas para o servidor externo como coletadas pelo sensor na estufa '1'
  giveLightFeedback(temp, humidity);             // Acende cor dos RGB segundo medidas e seu encaixe nas faixas permitidas para cada indicador
  displayMessage(tempMessage, humidityMessage);  // Mostra medidas no LCD
  delay(4000);                                   // Espera 4 segundos

  showAlerts(temp, humidity);  // Mostra alertas e sugestões no LCD, se as medidas estiverem fora do desejado
}

// Esta função gerencia a criação de um hotspot, o oferecimento da página Web para configuração do Wi-Fi e conexão com a rede escolhida
void connectWiFi() {
  WiFi.mode(WIFI_STA);  // Define Wi-Fi como Station Mode, permitindo a conexão com uma rede estabelecida a partir de um ponto de acesso

  displayMessage("Conecte-se a", "rede Greener");
  //wm.resetSettings();

  // Este método condição inicia um blocking loop para 1. iniciar um hotspot; 2. conectar-se automaticamente com a rede local, se as credenciais estiverem
  // salvas, ou esperar a configuração do nome e senha da rede local pela interface no IP 192.168.4.1, salvando essas credencias para uso posterior
  bool res = wm.autoConnect("Greener");  // password protected ap

  // Se não conectar, mostra mensagem de erro no display e no serial monitor, espera dois segundos e dá restart no ESP
  if (!res) {
    Serial.println("Failed to connect");         // Printa no monitor
    displayMessage("Falha em", "conectar");      // Mostra no display
    delay(2000);                                 // Espera dois segundos
    ESP.restart();                               // Dá restart no ESP
  } else {                                       // Se conectar, mostra mensagens de sucesso no monitor e no display por dois segundos
    Serial.println("Conectado!");                // Printa no monitor
    displayMessage("Conectado", "com sucesso");  // Mostra no display
    delay(2000);                                 // Espera dois segundos
  }
}

// Recebe medidas de temperatura e umidade e, dependendo de sua adequação com a faixa de tolerância, exibe alertas no display
void showAlerts(float temp, float humidity) {
  // Se a temperatura está abaixo do permitido, mostra mensagem de urgência e sugestão de intervenção
  if (temp < tempMin * 0.95) {
    displayMessage("TEMP BAIXA!", "Fechar janelas");
    // Se a temperatura está na zona de tolerância inferior, mostra mensagem de atenção
  } else if (temp < tempMin) {
    displayMessage("Atencao!", "Temp baixando");
    // Se a temperatura está abaixo do permitido, mostra mensagens de urgência e sugestões de intervenção
  } else if (temp > tempMax * 1.1) {
    displayMessage("TEMP ALTA!", "Abrir todas");
  } else if (temp > tempMax * 1.05) {
    displayMessage("TEMP ALTA!", "Laterais 100%");
  } else if (temp > tempMax * 1.03) {
    displayMessage("TEMP ALTA!", "Laterais 50%");
  } else if (temp > tempMax) {
    // Se a temperatura está na zona de tolerância superior, mostra mensagem de atenção
    displayMessage("Atencao!", "Temp aumentando");
  }
  delay(3000);  // Espera três segundos independentemente das medidas. Se elas estiverem dentro do desejado, a mensagem do loop principal,
  // que mostra apenas as leituras puras, será exibida por 6 segundos em vez de 3

  // Se a umidade está abaixo do permitido, mostra mensagem de urgência (no caso de umidade, não há propostas de intervenção)
  if (humidity < humidityMin * 0.95) {
    displayMessage("PERIGO!", "Umidade baixa!");
    // Se a temperatura está na zona de tolerância, mostra mensagem de atenção
  } else if (humidity < humidityMin) {
    displayMessage("ATENÇAO!", "Umidade baixando!");
    // Se a temperatura está acima do permitido, mostra mensagem de urgência
  } else if (humidity > humidityMax) {
    displayMessage("PERIGO!", "Umidade alta!");
  }
  delay(3000);  // Espera 3 segundos independentemente das medidas. Se elas estiverem dentro do desejado, a última mensagem exibida
  // permanecerá no visor por mais 3 segundos
}

// Faz varredura para identificar erros e mostrar alertas.
void checkSystem() {
  checkSensor();
  // Checa se o sensor está funcionando
  checkConnection();  // Checa se o ESP está conectado ao Wi-Fi
  sendNoError();
}

// Função para controlar a cor de cada led RGB. Recebe o estado, correspondente à cor desejada ('g', 'y', 'r'), estado de "alerta",
// em que ambos os leds piscam para quando há erro, ou '0', para desligar o RGB. Recebe também um argumento especificando o RGB desejado:
// 't' para temperatura e 'h' para umidade. No caso do estado de alerta ('a'), como ambos os RGBs piscam, o segundo argumento é default como '0'.
void turnOnRGB(char state, char led = '0') {
  // Define estado de alerta com RGBs piscando em intervalos de meio segundo
  if (state == 'a') {
    // Acende os dois RGBs na cor de alerta
    analogWrite(redTemp, 0);
    analogWrite(blueTemp, 255);
    analogWrite(greenTemp, 255);
    analogWrite(redHumidity, 0);
    analogWrite(blueHumidity, 255);
    analogWrite(greenHumidity, 255);
    delay(500);  // Espera 0,5 segundos

    // Apaga os dois RGBs
    analogWrite(redTemp, 255);
    analogWrite(blueTemp, 255);
    analogWrite(greenTemp, 255);
    analogWrite(redHumidity, 255);
    analogWrite(blueHumidity, 255);
    analogWrite(greenHumidity, 255);
    delay(500);  // Espera 0,5 segundos
  } else {
    if (led == 't') {      // Define cores para led de temperatura, se o argumento passado for 't'
      if (state == 'g') {  // Se for verde, acende combinação de RGB que gera verde no LED
        analogWrite(redTemp, 255);
        analogWrite(blueTemp, 0);
        analogWrite(greenTemp, 255);
      } else if (state == 'y') {  // Se for amarelo, acende combinação de RGB que gera amarelo no LED
        analogWrite(redTemp, 0);
        analogWrite(blueTemp, 255);
        analogWrite(greenTemp, 0);
      } else if (state == 'r') {  // Se for vermelho, acende combinação de RGB que gera vermelho no LED
        analogWrite(redTemp, 0);
        analogWrite(blueTemp, 255);
        analogWrite(greenTemp, 255);
      } else if (state == '0') {  // Se for '0', apaga LED (nesse caso, o apagado equivale a 255)
        analogWrite(redTemp, 255);
        analogWrite(blueTemp, 255);
        analogWrite(greenTemp, 255);
      }
    } else if (led == 'h') {  // Faz o mesmo se o argumento passado for para umidade
      if (state == 'g') {
        analogWrite(redHumidity, 0);
        analogWrite(blueHumidity, 255);
        analogWrite(greenHumidity, 0);
      } else if (state == 'y') {
        analogWrite(redHumidity, 255);
        analogWrite(blueHumidity, 0);
        analogWrite(greenHumidity, 255);
      } else if (state == 'r') {
        analogWrite(redHumidity, 255);
        analogWrite(blueHumidity, 0);
        analogWrite(greenHumidity, 0);
      } else if (state == '0') {
        analogWrite(redHumidity, 255);
        analogWrite(blueHumidity, 255);
        analogWrite(greenHumidity, 255);
      }
    }
  }
}

// Acende RGBs de acordo com a adequação das medidas de temperatura e umidade às faixas de tolerância estabelecidas com as constantes
void giveLightFeedback(float temp, float humidity) {
  // Apaga RGBs
  turnOnRGB('0', 't');
  turnOnRGB('0', 'h');

  // Se a temperatura for menor do que 95% do mínimo, acende LED vermelho (perigo)
  if (temp < tempMin * 0.95) {
    turnOnRGB('r', 't');
  } else if (temp < tempMin) {  // Se a temperatura for menor do que o mínimo, mas acima de 95% desse valor, acende LED amarelo (atenção)
    turnOnRGB('y', 't');
  } else if (temp > tempMax * 1.1) {  // Se a temperatura for maior do que 10% acima do máximo, acende LED vermelho (perigo)
    turnOnRGB('r', 't');
  } else if (temp > tempMax) {  // Se a temperatura for maior do que o máximo, mas menor do que 1.1 do máximo, acende LED amarelo (atenção)
    turnOnRGB('y', 't');
  } else {  // Se a temperatura estiver normal, acende LED verde
    turnOnRGB('g', 't');
  }

  // Se a umidade for menor do que 95% do mínimo, acende LED vermelho (perigo)
  if (humidity < humidityMin * 0.95) {
    turnOnRGB('r', 'h');
    // Se a temperatura for menor do que o mínimo, mas acima de 95% desse valor, acende LED amarelo (atenção)
  } else if (humidity < humidityMin) {
    turnOnRGB('y', 'h');
    // Se a temperatura for maior do que o máximo, acende LED vermelho (perigo)
  } else if (humidity > humidityMax) {
    turnOnRGB('r', 'h');
  } else {  // Se a temperatura estiver normal, acende LED verde
    turnOnRGB('g', 'h');
  }
}

// Checa se sensor está funcionando e exibe sinais de erro caso não
void checkSensor() {
  while (true) {          // Checa continuamente até sensor ser encontrado
    if (setupSensor()) {  // Se sensor for encontrado
                          // Reinicializa o display, pois erros de comunicação I2C em um periférico podem afetas outros. Logo, caso o sensor tenha falhado
                          // e afetado a conexão do LCD, ela pode ser reestabelecida antes que mais mensagens sejam exibidas
      setupDisplay();
      return;  // Sai da função
    } else {   // Caso sensor não seja encontrado, liga RGBs em estado de alerta e mostra mensagem de erro
      Serial.println("Sensor não encontrado");
      sendSensorError();
      displayMessage("Sensor não", "encontrado");
      turnOnRGB('a');
      delay(500);
    }
  }
}

// Rotina de teste das cores do RGB na inicialização do sistema
void checkLeds() {
  // Apaga ambos os RGBs
  turnOnRGB('0', 't');
  turnOnRGB('0', 'h');

  // Apresenta processo no LCD por 3 segundos
  displayMessage("Iniciando teste", "de LEDs");
  delay(3000);

  // Liga o RGB de temperatura no verde por 3 segundos e mostra esse estado no display
  turnOnRGB('g', 't');
  displayMessage("TEMP [VERDE]", "    OPERANTE    ");
  delay(3000);
  // Apaga RGB de temperatura
  turnOnRGB('0', 't');

  // Liga o RGB de temperatura no amarelo por 3 segundos e mostra esse estado no display
  turnOnRGB('y', 't');
  displayMessage("TEMP [AZUL]", "    OPERANTE    ");
  delay(3000);
  // Apaga RGB de temperatura
  turnOnRGB('0', 't');

  // Liga o RGB de temperatura no vermelho por 3 segundos e mostra esse estado no display
  turnOnRGB('r', 't');
  displayMessage("TEMP [VERMELHO]", "    OPERANTE    ");
  delay(3000);
  // Apaga RGB de temperatura
  turnOnRGB('0', 't');

  // Liga o RGB de umidade no verde por 3 segundos e mostra esse estado no display
  turnOnRGB('g', 'h');
  displayMessage("UMI [VERDE]", "    OPERANTE    ");
  delay(3000);
  // Apaga RGB de umidade
  turnOnRGB('0', 'h');

  // Liga o RGB de umidade no amarelo por 3 segundos e mostra esse estado no display
  turnOnRGB('y', 'h');
  displayMessage("UMI [AZUL]", "    OPERANTE    ");
  delay(3000);
  // Apaga RGB de umidade
  turnOnRGB('0', 'h');

  // Liga o RGB de umidade no vermelho por 3 segundos e mostra esse estado no display
  turnOnRGB('r', 'h');
  displayMessage("UMI [VERMELHO]", "    OPERANTE    ");
  delay(3000);
  // Apaga RGB de umidade
  turnOnRGB('0', 'h');
}

// Checa se ESP está conectado à internet
void checkConnection() {
  // Se não estiver conectado (valor booleano retornado pela função nativa do WiFi.h getWiFiStatus()), pisca LEDs no estado de alerta
  // e mostra mensagem de erro no LCD
  while (!getWiFiStatus()) {
    turnOnRGB('a');                             // RGBs em estado de alerta
    displayMessage("Problemas", "de conexao");  // Mostra mensagem no LCD
  }
}