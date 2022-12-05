#include <Wire.h>
 
#define SDA 5
#define SCL 4
 
void I2Cscan() {
  Wire.begin(SDA, SCL);
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  String addresses[2];
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      displayMessage("HI", "HI");
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(1000);          
}