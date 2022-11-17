#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"
#include <Arduino.h>

const char* ssid = "SHARE-RESIDENTE";
const char* password = "Share@residente";

// NTP server to request epoch time
const char* ntpServer = "pool.ntp.org";
const char* serverName = "http://10.254.18.137:1234/insert_reading";

// Variable to save current epoch time
unsigned long epochTime; 

unsigned long timerDelay = 10000;

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void sendReading(float temperature, float humidity, int greenhouse) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(serverName);

      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

      // Specify content-type header
      http.addHeader("Content-Type", "application/json");
      // Data to send with HTTP POST

      String httpRequestData = "{\"datetime\":\"" + String(getTime()) + "\",\"temperature\":\"" +
       String(temperature) + "\",\"humidity\":\"" + String(humidity) + "\",\"greenhouse\":\"" + String(greenhouse) + "\"}";
      Serial.println(httpRequestData);
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      

      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
      initWiFi();
    }
}

void setupInternet() {
  Serial.begin(115200);
  initWiFi();
  configTime(0, 0, ntpServer);
}