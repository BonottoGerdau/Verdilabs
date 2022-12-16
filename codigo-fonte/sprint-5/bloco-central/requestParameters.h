/* Módulo para requisitar parâmetros do servidor */
#include <WiFi.h>        // Biblioteca básica para trabalhar com WiFi no ESP
#include <HTTPClient.h>  // Biblioteca para fazer requisições HTTP

// Definições dos endpoints para cada parâmetro
const char* tempMinAddress = "https://greener-g6it.onrender.com/tempMin";
const char* tempMaxAddress = "https://greener-g6it.onrender.com/tempMax";
const char* humidityMinAddress = "https://greener-g6it.onrender.com/humidityMin";
const char* humidityMaxAddress = "https://greener-g6it.onrender.com/humidityMax";

// Esta função faz a requisição dos parâmetros para cada um dos endpoints, totalizando quatro requests HTTP separadas, e os salva num array 
// passado como argumento
void requestParameters(double* parameters) {
  HTTPClient http;  // Cria cliente HTTP

  // Inicia requisição para o endpoint desejado
  http.begin(tempMinAddress);

  // Envia HTTP POST
  int httpResponseCode = http.sendRequest("GET");

  // Cria variável para salvar dados retornados da requisição
  String payload = "{}";

  // Se a requisição funcionar (código > 0)...
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: "); // Printa código
    Serial.println(httpResponseCode);
    payload = http.getString(); // Pega retorno
    parameters[0] = payload.toDouble(); // Guarda retorno como double no array
  } else {
    Serial.print("Error code: "); // Printa erro, se houver
    Serial.println(httpResponseCode);
  }
  // Finaliza requisição
  http.end();

  // Inicia próxima requisição. A mesma lógica explicada acima se aplica às próximas requisições.
  http.begin(tempMaxAddress);

  httpResponseCode = http.sendRequest("GET");

  payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    parameters[1] = payload.toDouble();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  http.begin(humidityMinAddress);
  httpResponseCode = http.sendRequest("GET");

  payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    parameters[2] = payload.toDouble();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  http.begin(humidityMaxAddress);
  httpResponseCode = http.sendRequest("GET");

  payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    parameters[3] = payload.toDouble();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}