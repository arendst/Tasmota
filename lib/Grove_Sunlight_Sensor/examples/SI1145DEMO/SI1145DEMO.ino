/*
    This is a demo to test Grove - Sunlight Sensor library

*/

#include <Wire.h>

#include "Arduino.h"
#include "SI114X.h"

SI114X SI1145 = SI114X();

void setup() {

  Serial.begin(115200);
  Serial.println("Beginning Si1145!");

  while (!SI1145.Begin()) {
    Serial.println("Si1145 is not ready!");
    delay(1000);
  }
  Serial.println("Si1145 is ready!");
}

void loop() {
  Serial.print("//--------------------------------------//\r\n");
  Serial.print("Vis: "); Serial.println(SI1145.ReadVisible());
  Serial.print("IR: "); Serial.println(SI1145.ReadIR());
  //the real UV value must be div 100 from the reg value , datasheet for more information.
  Serial.print("UV: ");  Serial.println((float)SI1145.ReadUV()/100);
  delay(1000);
}
