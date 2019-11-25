/**
 * @file example.ino
 * @author Felix Galindo
 * @date June 2017
 * @brief Example using HPMA115S0 sensor library on a Feather 32u4
 * @license MIT
 */

#include <hpma115S0.h>
#include <SoftwareSerial.h>

//Create an instance of software serial
SoftwareSerial hpmaSerial(10, 11); // Feather TX, Feather RX

//Create an instance of the hpma115S0 library
HPMA115S0 hpma115S0(hpmaSerial);

void setup() {
  Serial.begin(57600);
  hpmaSerial.begin(9600);
  delay(5000);
  Serial.println("Starting...");
  hpma115S0.Init();
  hpma115S0.StartParticleMeasurement();
}

void loop() {
  unsigned int pm2_5, pm10;
  if (hpma115S0.ReadParticleMeasurement(&pm2_5, &pm10)) {
    Serial.println("PM 2.5: " + String(pm2_5) + " ug/m3" );
    Serial.println("PM 10: " + String(pm10) + " ug/m3" );
  }
  delay(1000);
}