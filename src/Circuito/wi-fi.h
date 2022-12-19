/* Módulo para enviar requisições de medições e erros */
#include <WiFi.h>                                                        // Biblioteca básica para trabalhar com WiFi no ESP
#include <HTTPClient.h>                                                  // Biblioteca para fazer requisições HTTP
#include <Arduino_JSON.h>

const char* readingAddress = "https://greener-g6it.onrender.com/insert_reading";  // Guarda endereço do endpoint para enviar dados de leitura
const char* errorAddress = "https://greener-g6it.onrender.com/insert_error";      // Guarda endereço do endpoint para enviar dados de erro

// Esta função envia dados de temperatura, umidade e estufa de origem para o servidor externo
void sendReading(float temperature, float humidity, int greenhouse) {
  // Checa status da conexão Wi-Fi. Se ela estiver ativa, realiza a requisição
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;  // Cria cliente
    HTTPClient http;    // Cria objeto http

    // Inicia requisição para o endpoint desejado
    http.begin(readingAddress);

    // Adiciona header de json, conforme especificado no servidor
    http.addHeader("Content-Type", "application/json");

    // Cria string json para requisição
    String httpRequestData = "{\"datetime\":\"" + String(0) + "\",\"temperature\":\"" + String(temperature) + "\",\"humidity\":\"" + String(humidity) + "\",\"greenhouse\":\"" + String(greenhouse) + "\"}";

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

// Devolve valor booleano de acordo com estado da conexão com Wi-Fi (1 para conectado, 0 para não conectado)
bool getWiFiStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    return 1;
  }
  return 0;
}

// Envia erro de sensor ao servidor
void sendSensorError() {
  // Checa status da conexão Wi-Fi. Se ela estiver ativa, realiza a requisição
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;  // Cria cliente
    HTTPClient http;    // Cria objeto http

    // Inicia requisição para o endpoint desejado
    http.begin(errorAddress);

    // Adiciona header de json, conforme especificado no servidor
    http.addHeader("Content-Type", "application/json");

    // Cria string json para requisição
    String httpRequestData = "{\"error\":\"sensor\"}";
    Serial.println(httpRequestData);

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

// Informa o servidor que nenhum erro foi detectado ("0")
void sendNoError() {
  // Checa status da conexão Wi-Fi. Se ela estiver ativa, realiza a requisição
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;  // Cria cliente
    HTTPClient http;    // Cria objeto http

    // Inicia requisição para o endpoint desejado
    http.begin(errorAddress);

    // Adiciona header de json, conforme especificado no servidor
    http.addHeader("Content-Type", "application/json");

    // Cria string json para requisição
    String httpRequestData = "{\"error\":\"0\"}";
    Serial.println(httpRequestData);

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

