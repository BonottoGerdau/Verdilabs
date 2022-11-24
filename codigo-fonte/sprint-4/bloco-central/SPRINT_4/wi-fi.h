#include <WiFi.h>
#include <HTTPClient.h>
const char* serverName = "http://10.128.1.222:80/insert_reading";

void sendReading(float temperature, float humidity, int greenhouse) {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);

    http.addHeader("Content-Type", "application/json");

    String httpRequestData = "{\"datetime\":\"" + String(0) + "\",\"temperature\":\"" + String(temperature) + "\",\"humidity\":\"" + String(humidity) + "\",\"greenhouse\":\"" + String(greenhouse) + "\"}";
    Serial.println(httpRequestData);
    // Send HTTP POST request
    // int httpResponseCode = http.POST(httpRequestData);
    int httpResponseCode = http.sendRequest("POST", httpRequestData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode,DEC);
    Serial.println();

    // Free resources
    http.end();

  } else {
    Serial.println("WiFi Disconnected");
  }
}