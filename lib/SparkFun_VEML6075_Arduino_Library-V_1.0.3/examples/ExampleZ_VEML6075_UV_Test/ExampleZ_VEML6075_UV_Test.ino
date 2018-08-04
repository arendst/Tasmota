#include <SparkFun_VEML6075_Arduino_Library.h>

VEML6075 uv;

void setup() {
  Serial.begin(9600);
  
  if (!uv.begin())
  {
    Serial.println("Unable to communicate with VEML6075.");
    while (1) ;
  }
}

void loop() {
  Serial.println(String(uv.a()) + ", " + String(uv.b()) + ", " +
                String(uv.uvcomp1()) + ", " + String(uv.uvcomp2()) + ", " + 
                String(uv.index()));
  delay(250);
}