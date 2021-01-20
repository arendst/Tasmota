/***************************************************************************
  This is a example for ENS210 basic reading 
    
  Written by Christoph Friese for Sciosense / 8-April-2020
 ***************************************************************************/

#include <Wire.h>
#define ArduinoIDE true 
int ArduinoLED = 13;

//-------------------------------------------------------------
//ENS210 related items
//-------------------------------------------------------------
#include "ScioSense_ENS210.h"
ScioSense_ENS210      ens210;

/*--------------------------------------------------------------------------
  SETUP function
  initiate sensor
 --------------------------------------------------------------------------*/
void setup() {
  bool ok;

  //-------------------------------------------------------------
  // General setup steps
  //-------------------------------------------------------------

  Serial.begin(9600);

  while (!Serial) {}

  //Switch on LED for init
  pinMode(ArduinoLED, OUTPUT);
  digitalWrite(ArduinoLED, LOW);

  if (ArduinoIDE) {
    Serial.println("ENS210 example");
    delay(1000);
  }

  if (ArduinoIDE) Serial.print("ENS210...");
  ok = ens210.begin();
  if (ArduinoIDE) Serial.println(ens210.available() ? "done." : "failed!");
  ens210.setSingleMode(false);

  //blink LED
  digitalWrite(ArduinoLED, HIGH);
  delay(100);
  digitalWrite(ArduinoLED, LOW);
  
}

/*--------------------------------------------------------------------------
  MAIN LOOP FUNCTION
  Cylce every 1000ms and perform action
 --------------------------------------------------------------------------*/

void loop() {
  
  if (ens210.available()) ens210.measure();
    
  if (ArduinoIDE) {
    Serial.print("Temperature: ");Serial.print(ens210.getTempCelsius());Serial.print("°C\t");
    Serial.print("Humidity: "); Serial.print(ens210.getHumidityPercent());Serial.println("%");
  } else {
    Serial.print(ens210.getTempCelsius());Serial.print(",");
    Serial.print(ens210.getHumidityPercent());Serial.println("");
  }

  delay(1000);
}
