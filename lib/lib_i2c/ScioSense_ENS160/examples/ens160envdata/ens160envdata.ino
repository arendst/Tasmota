/***************************************************************************
  This is a example for ENS160 basic reading 
    
  Written by Christoph Friese for Sciosense / 8-April-2020
 ***************************************************************************/

#include <Wire.h>
int ArduinoLED = 13;

//-------------------------------------------------------------
//ENS160 related items
//-------------------------------------------------------------
#include "ScioSense_ENS160.h"  // ENS160 library
ScioSense_ENS160      ens160(ENS160_I2CADDR_0);
//sciosense_ENS160      ens160(ENS160_I2CADDR_1);

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

  Serial.println("ENS160 example with RH & T compensation");
  delay(1000);

  //-------------------------------------------------------------
  //ENS160 related items
  //-------------------------------------------------------------
  Serial.print("ENS160...");
  ok = ens160.begin();
  Serial.println(ens160.available() ? "done." : "failed!");
  if (ens160.available()) {
    // Print ENS160 versions
    Serial.print("Rev: "); Serial.print(ens160.getMajorRev());
    Serial.print("."); Serial.print(ens160.getMinorRev());
    Serial.print("."); Serial.println(ens160.getBuild());

    Serial.print("setup: ENS160 standard mode ");
    if (!ens160.setMode(ENS160_OPMODE_STD) ) {
      Serial.println("FAILED");
    } else {
      Serial.println("successful");
    }
  }
  
  //-------------------------------------------------------------
  //ENS210 related items
  //-------------------------------------------------------------
  Serial.print("ENS210...");
  ok = ens210.begin();
  Serial.println(ens210.available() ? "done." : "failed!");
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

  
  if (ens160.available()) {
    if (ens210.available()) {
      ens210.measure();
      ens160.set_envdata210(ens210.getDataT(),ens210.getDataH());
    }
    ens160.measure();
  }
  
  Serial.print("TVOC (0x22): ");Serial.print(ens160.getTVOC());Serial.print("ppb\t");
  Serial.print("eCO2 (0x24): ");Serial.print(ens160.getEtOH());Serial.print("ppm\t");
  Serial.print("R HP0: ");Serial.print(ens160.getHP0());Serial.print("Ohm\t");
  Serial.print("R HP1: ");Serial.print(ens160.getHP1());Serial.print("Ohm\t");
  Serial.print("R HP2: ");Serial.print(ens160.getHP2());Serial.print("Ohm\t");
  Serial.print("R HP3: ");Serial.print(ens160.getHP3());Serial.print("Ohm\t");
  
  Serial.print("Temperature: ");Serial.print(ens210.getTempCelsius());Serial.print("°C\t");
  Serial.print("Humidity: "); Serial.print(ens210.getHumidityPercent());Serial.println("%");

  delay(1000);
}
