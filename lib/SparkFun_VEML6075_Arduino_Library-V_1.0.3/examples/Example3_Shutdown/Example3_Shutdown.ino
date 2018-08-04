/*
  Shutting down/powering on the VEML6075
  By: Jim Lindblom
  SparkFun Electronics
  Date: May 23, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14748
  
  This example demonstrates how to shutdown and power-back-up the VEML6075. In case you need to save some power.ee
  
  Quick-start:
  - Use a SparkFun BlackBoard -or- attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  - Check through comments to configure your sensor, then upload example sketch
  - Plug the Qwiic UV Sensor onto the BlackBoard/shield
  - Open the serial monitor and set the baud rate to 9600
  - Sensor readings will stream every ~250ms. Every ~10 seconds the sensor will shut down or power-on.
    - In the shutdown state, readings will contain the last-read-value
*/
// Include the SparkFun VEML6075 library.
// Click here to get the library: http://librarymanager/All#SparkFun_VEML6075
#include <SparkFun_VEML6075_Arduino_Library.h>

VEML6075 uv; // Create a VEML6075 object 

void setup() {
  Serial.begin(9600);
  if (uv.begin() == false) {
    Serial.println("Unable to communicate with VEML6075.");
    while (1) ;
  }
}

const unsigned int READINGS_BETWEEN_SHUTDOWN = 50;

void loop() {
  static unsigned int numReadings = 1;
  static boolean powerOnState = true;
  
  if ((numReadings % READINGS_BETWEEN_SHUTDOWN) == 0) {
    if (powerOnState) {
      // Use shutdown to disable sensor readings. The sensor will consume less power in this state.
      uv.shutdown(); 
      Serial.println("Shut down");
      powerOnState = false;   
    } else {
      // Use powerOn to enable sensor readings.
      uv.powerOn();
      Serial.println("Power up!");
      powerOnState = true;
    }
  }
  Serial.println(String((float)millis() / 1000.0) + ": " + String(uv.uva()) + ", " + String(uv.uvb()) + ", " + String(uv.index()));
  numReadings++;
  
  delay(200);
}