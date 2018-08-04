/*
  Using and configuring the VEML6075
  By: Jim Lindblom
  SparkFun Electronics
  Date: May 23, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14748
  
  This example demonstrates how to initialize and configure the VEML6075.
  Then poll it for UVA, UVB, and UV index readings.
  
  Quick-start:
  - Use a SparkFun BlackBoard -or- attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  - Check through comments to configure your sensor, then upload example sketch
  - Plug the Qwiic UV Sensor onto the BlackBoard/shield
  - Open the serial monitor and set the baud rate to 9600
  - Sensor readings will stream every ~100ms.
*/
// Include the SparkFun VEML6075 library.
// Click here to get the library: http://librarymanager/All#SparkFun_VEML6075
#include <SparkFun_VEML6075_Arduino_Library.h>

VEML6075 uv; // Create a VEML6075 object 

void setup() {
  Serial.begin(9600);
  // The begin function can take a TwoWire port as a parameter -- in case your platform has more than
  // the one standard "Wire" port.
  // begin will return VEML6075_SUCCESS on success, otherwise it will return an error code.
  if (uv.begin(Wire) != VEML6075_SUCCESS) {
    Serial.println("Unable to communicate with VEML6075.");
    while (1) ;
  }

  // Integration time: The VEML6075 features five selectable integration times. This is the amount of
  // time the sensor takes to sample UVA/UVB values, before integrating the readings into averages.
  // Valid integration times are:
  //      VEML6075::IT_50MS -- 50ms
  //      VEML6075::IT_100MS -- 100ms
  //      VEML6075::IT_200MS -- 200ms
  //      VEML6075::IT_400MS -- 400ms
  //      VEML6075::IT_800MS -- 800ms
  // The library defaults integration time to 100ms. (Set on every call to begin().)
  uv.setIntegrationTime(VEML6075::IT_200MS);

  // High dynamnic: The VEML6075 can either be set to normal dynamic or high dynamic mode.
  // In high dynamic mode, the resolution is increased by about a factor of two.
  // Valid dynamic settings are:
  //      VEML6075::DYNAMIC_NORMAL -- Normal dynamic mode
  //      VEML6075::DYNAMIC_HIGH -- High dynamic mode
  // The library defaults the dynamic to normal
  uv.setHighDynamic(VEML6075::DYNAMIC_HIGH);
}

void loop() {
  // In addition to uva, uvb, and index, the library also supports reading the raw
  // 16-bit unsigned UVA and UVB values, and visible-light and infrared compensation values with
  // the functions rawUva, rawUvb, visibleCompensation, and irCompensation. These values,
  // in addition to pre-calculated scalars, are used to generate the calculated UVA, UVB and index values.
  Serial.println(String((float)millis() / 1000.0) + ": " + 
                 String(uv.rawUva()) + ", " + String(uv.rawUvb()) + ", " + 
                 String(uv.visibleCompensation()) + ", " + String(uv.irCompensation()) + ", " +
                 String(uv.uva()) + ", " + String(uv.uvb()) + ", " + String(uv.index()));
  delay(100);
}