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

  Serial.println("ENS160 example");
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
}

/*--------------------------------------------------------------------------
  MAIN LOOP FUNCTION
  Cylce every 1000ms and perform action
 --------------------------------------------------------------------------*/

void loop() {
  
  if (ens160.available()) {
    ens160.measure();
  
    Serial.print("TVOC (0x22): ");Serial.print(ens160.getTVOC());Serial.print("ppb\t");
    Serial.print("eCO2 (0x24): ");Serial.print(ens160.geteCO2());Serial.print("ppm\t");
    Serial.print("R HP0: ");Serial.print(ens160.getHP0());Serial.print("Ohm\t");
    Serial.print("R HP1: ");Serial.print(ens160.getHP1());Serial.print("Ohm\t");
    Serial.print("R HP2: ");Serial.print(ens160.getHP2());Serial.print("Ohm\t");
    Serial.print("R HP3: ");Serial.print(ens160.getHP3());Serial.println("Ohm");
  }
  delay(1000);
}
