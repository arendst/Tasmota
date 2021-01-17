/***************************************************************************
  This is a example for ENS160 flash programming
  
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

#include "clare_fw_app_production_v2.3.1.h"

/*--------------------------------------------------------------------------
  SETUP function
  initiate sensor
 --------------------------------------------------------------------------*/
void setup() {
  bool ok;

  //-------------------------------------------------------------
  // General setup steps
  //-------------------------------------------------------------

  Serial.begin(115200);

  while (!Serial) {}

  //Switch on LED for init
  pinMode(ArduinoLED, OUTPUT);
  digitalWrite(ArduinoLED, LOW);

  Serial.println("ENS160 flash example");
  delay(1000);


  //-------------------------------------------------------------
  //ENS160 related items
  //-------------------------------------------------------------
  Serial.print("Init ");
  ok = ens160.begin(1,1);
  Serial.println();
  
  if (ens160.available()) {
    // Print ENS160 version
	Serial.print("Current rev: "); Serial.print(ens160.getMajorRev());
	Serial.print("."); Serial.print(ens160.getMinorRev());
	Serial.print("."); Serial.println(ens160.getBuild());

	ok = ens160.flash(image_data, sizeof(image_data));

	if (ok) {
		// Print ENS160 version
		Serial.print("Updated rev: "); Serial.print(ens160.getMajorRev());
		Serial.print("."); Serial.print(ens160.getMinorRev());
		Serial.print("."); Serial.println(ens160.getBuild());
	} else {
		Serial.println("--- FAILED ---");
	}
  }
}

/*--------------------------------------------------------------------------
  MAIN LOOP FUNCTION
  Cylce every 1000ms and perform action
 --------------------------------------------------------------------------*/

void loop() {
  
}
