#include <Preferences.h>
#include "requestParameters.h"

int tempMin, tempMax, humidityMin, humidityMax;
double parameters[] = {0, 0, 0, 0};

void initCurrentParameters() {
  Preferences preferences;
  preferences.begin("greener", false);
  if (getWiFiStatus) {
    requestParameters(parameters);
    Serial.print("solicitei!");
    Serial.println(parameters[0]);
    tempMin = parameters[0];
    tempMax =  parameters[1];
    humidityMin = parameters[2];
    humidityMax = parameters[3];

  } else {
    tempMin = preferences.getInt("tempMin");
    tempMax = preferences.getInt("tempMax");
    humidityMin = preferences.getInt("humidityMin");
    humidityMax = preferences.getInt("humidityMax");
  }
}