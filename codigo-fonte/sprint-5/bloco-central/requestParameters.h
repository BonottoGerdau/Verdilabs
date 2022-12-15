#include <WiFi.h>                                                        // Biblioteca básica para trabalhar com WiFi no ESP
#include <HTTPClient.h>                                                  // Biblioteca para fazer requisições HTTP
#include <Arduino_JSON.h>

const char* tempMinAddress = "http://10.128.65.52:1234/tempMin";  
const char* tempMaxAddress = "http://10.128.65.52:1234/tempMax";  
const char* humidityMinAddress = "http://10.128.65.52:1234/humidityMin";  
const char* humidityMaxAddress = "http://10.128.65.52:1234/humidityMax";  

void requestParameters(double* parameters) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path 
  http.begin(tempMinAddress);


  // Send HTTP POST request
  int httpResponseCode = http.sendRequest("GET");

  String payload = "{}";   

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    parameters[0] = payload.toDouble();
    Serial.println(parameters[0]);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  http.begin(tempMaxAddress);


  // Send HTTP POST request
  httpResponseCode = http.sendRequest("GET");

  payload = "{}";   

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    parameters[1] = payload.toDouble();
    Serial.println(parameters[1]);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  http.begin(humidityMinAddress);


  // Send HTTP POST request
  httpResponseCode = http.sendRequest("GET");

  payload = "{}";   

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    parameters[2] = payload.toDouble();
    Serial.println(parameters[2]);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  http.begin(humidityMaxAddress);


  // Send HTTP POST request
  httpResponseCode = http.sendRequest("GET");

  payload = "{}";   

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    parameters[3] = payload.toDouble();
    Serial.println(parameters[3]);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}