/*
  Using the VEML6075 -- UVA, UVB, and UV Index monitoring
  By: Jim Lindblom
  SparkFun Electronics
  Date: May 23, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14748
  
  This example demonstrates how to initialize the VEML6075, then poll it for UVA, UVB, and UV index readings.
  
  Quick-start:
  - Use a SparkFun BlackBoard -or- attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  - Upload example sketch
  - Plug the Qwiic UV Sensor onto the BlackBoard/shield
  - Open the serial monitor and set the baud rate to 9600
  - Sensor readings will stream every ~250ms. Output is: <UVA>, <UVB>, <UV index>
*/
// Include the SparkFun VEML6075 library.
// Click here to get the library: http://librarymanager/All#SparkFun_VEML6075
#include <SparkFun_VEML6075_Arduino_Library.h>

VEML6075 uv; // Create a VEML6075 object 

void setup() {
  Serial.begin(9600);
  // the VEML6075's begin function can take no parameters
  // It will return true on success or false on failure to communicate
  if (uv.begin() == false) {
    Serial.println("Unable to communicate with VEML6075.");
    while (1) ;
  }
  Serial.println("UVA, UVB, UV Index");
}

void loop() {
  // Use the uva, uvb, and index functions to read calibrated UVA and UVB values and a 
  // calculated UV index value between 0-11.
  Serial.println(String(uv.uva()) + ", " + String(uv.uvb()) + ", " + String(uv.index()));
  delay(250);
}