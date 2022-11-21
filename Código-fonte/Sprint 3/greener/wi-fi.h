#include <WiFi.h> // Biblioteca básica para trabalhar com WiFi no ESP
#include <HTTPClient.h> // Biblioteca para fazer requisições HTTP
#include <WiFiManager.h> // Biblioteca para configurar credenciais para Wi-Fi local a partir de uma interface em hotspot 

WiFiManager wm; // Inicializa WiFiManager
const char* serverName = "http://192.168.1.108:1234/insert_reading"; // Guarda endereço do endpoint para enviar dados

// Esta função faz a conexão com a rede local a partir do hotspot e retorna verdadeiro ou falso conforme resultado dessa ação
int connect() {
  WiFi.mode(WIFI_STA); // Define modo de WiFi, diferente do padrão do ESP
  bool res;
  res = wm.autoConnect("Greener", "verdilabs");  // Cria hotspot chamado "Greener" com senha "verdilabs". 
  // Se conexão com rede local através dele der certo, res será true; caso contrário, será falso
  return res;
}

// Esta função envia dados de temperatura, umidade e estufa de origem para o servidor externo
void sendReading(float temperature, float humidity, int greenhouse) {
  // Checa status da conexão Wi-Fi. Se ela estiver ativa, realiza a requisição
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client; // Cria cliente 
    HTTPClient http; // Cria objeto http

    // Inicia requisição para o endpoint desejado
    http.begin(serverName);

    // Adiciona header de json, conforme especificado no servidor
    http.addHeader("Content-Type", "application/json");

    // Cria string json para requisição
    String httpRequestData = "{\"datetime\":\"" + String(0) + "\",\"temperature\":\"" + String(temperature) + "\",\"humidity\":\"" +
    String(humidity) + "\",\"greenhouse\":\"" + String(greenhouse) + "\"}";

    // Envia HTTP POST 
    int httpResponseCode = http.POST(httpRequestData);

    // Printa código de resposta
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Libera recursos
    http.end();

  } else {
    // Informa que WiFi está desconectado
    Serial.println("WiFi desconectado");
  }
}