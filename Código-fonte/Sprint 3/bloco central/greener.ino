/* 
Este é o código-fonte para ESP32-S3-Box de Greener, um assistente de monitoramento de temperatura e umidade para casas de vegetação.

Através da conexão com os módulos sensor.h e display.h, este programa checa a temperatura e umidade relativa do ar medidas por um sensor AHT10
a cada 10 segundos, interpreta esses dados segundo regras do negócio e coordena feedbacks através de um display LCD e um código de cores em seis LEDs.
Essas medidas são enviadas a cada 10 

Nesta Sprint 3, alcançamos os seguintes objetivos, cumprindo os requisitos da Adalove:

1. Conexão do ESP-32 com a rede local através da definição de um hotspot utilizando o módulo "wi-fi.h" e a biblioteca "WiFiManager";
2. Envio de leituras de umidade e temperatura para um servidor externo (no momento, ainda rodando em localhost no notebook) 
através de requisições HTTP do método POST. Escolhemos esse caminho em vez de um servidor embarcado seguindo a recomendação do professor
de programação da turma. Pretendemos implementar o servidor embarcado como fallback da interface gráfica nas próximas sprints para casos
de falta de conexão à internet nas estufas.
3. Melhorias as rotinas de inicialização, com teste de sensores, LEDs e conexão.
4. Padronização do tempo de exibição de feedback na tela entre coletas. No momento, cada leitura é feita a cada 10 segundos, sendo as medidas
exibidas por no mínimo 4 segundos. Os seis segundos subsequentes são divididos entre alertas de temperatura e/ou umidade, se necessário.

Para as próximas sprints, pretendemos desenvolver as seguintes metas:

1. Detecção do timestamp local no momento de coleta das medidas de temperatura e umidade, com exibição no LCD e envio do mesmo para o banco de dados
2. Adição de um push button ao circuito que, ao ser apertado, reative o hotspot do ESP-32 para o usuário conectá-lo a outra rede sem precisar
reiniciar o sistema
3. Salvamento automático dos dados no armazenamento interno do ESP no caso de falta de conexão
4. Possibilidade de alterar os parâmetros de temperatura e umidade ideais, além de modificar o intervalo entre medições.
*/

// Importação dos módulos necessários. Não foi possível modularizar mais devido à necessidade de se utilizar o LCD nas funções deste arquivo,
// pois não é possível importar a mesma biblioteca em mais de uma aba. Logo, tudo que exigia LCD precisou ser concentrado neste .ino.
#include "sensor.h"   // Inicializa e coordena leituras de temperatura e umidade
#include "display.h"  // Inicializa e coordena exibição de mensagens e rotinas no LCD
#include "wi-fi.h"    // Inicializa hotspot, conecta ESP com a rede local e coordena envio de requisições POST com dados coletados

// Definição de pinos dos LEDs de feedback para temperatura
#define greenTemp 13   // Led para medidas ideais
#define yellowTemp 12  // Led para medidas na zona de tolerância
#define redTemp 11     // Led para medidas fora da zona permitida

// Definição de pinos dos LEDs de feedback para umidade
#define greenHumidity 46  // Led para medidas ideais
#define yellowHumidity 8  // Led para medidas na zona de tolerância
#define redHumidity 18    // Led para medidas fora da zona permitida

const tempMax = 36;
const tempMin = 28;
const humidityMax = 95;
const humidityMin = 70;

void checkSensor();                            // Esta função checa se o sensor está funcionando e exibe o resultado no LCD
void checkLeds();                              // Esta função testa todos os LEDs e exibe o resultado no LCD
void lightUpLeds(float temp, float humidity);  // Esta função coordena o acioanemtno dos LEDs de acordo com as medidas de temperatura e umidade
void showAlerts(float temp, float humidity);   // Esta função coordena as mensagens exibidas no LCD de acordo com as medidas de temperatura e umidade

// O setup inicializa todos os componentes do circuito e performa rotinas básicas de boas-vindas, checagem e conexão Wi-Fi
void setup() {
  // Inicializa pinos de LEDs
  pinMode(redTemp, OUTPUT);
  pinMode(yellowTemp, OUTPUT);
  pinMode(greenTemp, OUTPUT);
  pinMode(yellowHumidity, OUTPUT);
  pinMode(redHumidity, OUTPUT);
  pinMode(greenHumidity, OUTPUT);

  setupDisplay();  // Inicializa display LCD
  welcome();       // Exibe boas-vindas no LCD
  checkSensor();   // Checa se o sensor está funcionando

  displayMessage("Conecte-se a", "rede Greener"); 
  // Esta condição inicia um blocking loop para 1. iniciar um hotspot; 2. conectar-se automaticamente com a rede local, se as credenciais estiverem
  // salvas, ou esperar a configuração do nome e senha da rede local pela interface no IP 192.168.4.1, salvando essas credencias para uso posterior
  if (!connect()) {                          // Se não conseguir conectar, mostra mensagem de erro no LCD, espera 2 segundos e reinicia o ESP
    displayMessage("Falha em", "conectar");  // Esta função exibe as strings passadas na primeira e segunda linhas do LCD, respectivamente
    delay(2000);
    ESP.restart();
  } else {  // Se conseguir conectar, exibe mensagem de sucesso no LCD por 2 segundos
    displayMessage("Conectado", "com sucesso");
    delay(2000);
  }
  checkLeds();  // Faz checagem de todos os LEDs do circuito
}

// O loop principal lê medidas de temperatura e umidade a cada 10 segundos, mostra-as no LCD (4 segundos) e apresenta o feedback por meio dos LEDs,
// segundo as regras do negócio, e de alertas e sugestões de intervenção no display (3 segundos para temperatura,
// 3 segundos para umidade, se necessário)
void loop() {
  float temp = getTemp();                                                // Pega temperatura atual
  float humidity = getHumidity();                                        // Pega umidade atual
  String tempMessage = "Temp: " + String(temp) + " " + (char)223 + "C";  // Cria mensagem de temperatura para LCD
  String humidityMessage = "Umidade: " + String(humidity) + "%";         // Cria mensagem de umidade para LCD

  sendReading(temp, humidity, 1);  // Envia medidas para o servidor externo como coletadas pelo sensor na estufa '1'

  lightUpLeds(temp, humidity);                   // Acende leds segundo medidas e seu encaixe nas faixas permitidas para cada indicador
  displayMessage(tempMessage, humidityMessage);  // Mostra medidas no LCD

  delay(4000);  // Espera 4 segundos

  showAlerts(temp, humidity);  // Mostra alertas e sugestões no LCD, se as medidas estiverem fora do desejado
}

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
  delay(3000); // Espera três segundos independentemente das medidas. Se elas estiverem dentro do desejado, a mensagem do loop principal,
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
  delay(3000); // Espera 3 segundos independentemente das medidas. Se elas estiverem dentro do desejado, a última mensagem exibida
  // permanecerá no visor por mais 3 segundos
}

// Acende LEDs de acordo com as medidas coletadas
void lightUpLeds(float temp, float humidity) {
  // Desliga todos os LEDs para reiniciar estado do sistema
  digitalWrite(redTemp, LOW);
  digitalWrite(yellowTemp, LOW);
  digitalWrite(greenTemp, LOW);
  digitalWrite(redHumidity, LOW);
  digitalWrite(yellowHumidity, LOW);
  digitalWrite(greenHumidity, LOW);

  // Quando a temperatura está fora do permitido, brilha o LED vermelho de temperatura
  if (temp < tempMin * 0.95) {
    digitalWrite(redTemp, HIGH);
  // Quando a temperatura está na zona de tolerância, brilha o LED amarelo de temperatura
  } else if (temp < tempMin) {
    digitalWrite(yellowTemp, HIGH);
  // Quando a temperatura está fora do permitido, brilha o LED vermelho de temperatura
  } else if (temp > tempMax * 1.1) {
    digitalWrite(redTemp, HIGH);
  // Quando a temperatura está na zona de tolerância, brilha o LED amarelo de temperatura
  } else if (temp > tempMax) {
    digitalWrite(yellowTemp, HIGH);
  // Quando a temperatura está dentro do permitido, brilha o LED verde de temperatura
  } else {
    digitalWrite(greenTemp, HIGH);
  }

  // Quando a umidade está fora do permitido, brilha o LED vermelho de umidade
  if (humidity < humidityMin * 0.95) {
    digitalWrite(redHumidity, HIGH);
  // Quando a umidade está na zona de tolerância, brilha o LED amarelo de umidade
  } else if (humidity < humidityMin) {
    digitalWrite(yellowHumidity, HIGH);
  } else if (humidity > humidityMax) {
    digitalWrite(redHumidity, HIGH);
  // Quando a umidade está dentro do permitido, brilha o LED verde de umidade
  } else {
    digitalWrite(greenHumidity, HIGH);
  }
}

// Checa se sensor está funcionando
void checkSensor() {
  while (true) { // Fica testando sensor até funcionar
    if (setupSensor()) { // Checa se sensor foi identificado no circuito. Se sim, exibe mensagem de sucesso e sai do loop
      displayMessage("Sensor foi", "encontrado");
      delay(2000);
      break;
    } else { // Se não, exibe mensagem de falha e tenta novamente
      displayMessage("Sensor não", "encontrado");
      delay(2000);
    }
  }
}

// Coordena rotina de teste dos LEDs
void checkLeds() {
  // Informa que teste vai começar
  displayMessage("Iniciando teste", "de LEDs");
  delay(3000);

  // Acende cada LED, com sua respectiva explicação no LCD, por 3 segundos  
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