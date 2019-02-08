#include "Adafruit_Crickit.h"

Adafruit_Crickit crickit;

void setup() {
  Serial.begin(115200);
  Serial.println("1 Drive demo!");
  
  if(!crickit.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("Crickit started");

  //our default frequency is 1khz
  crickit.setPWMFreq(CRICKIT_DRIVE1, 1000);
}

void loop() {
  //turn all the way on
  crickit.analogWrite(CRICKIT_DRIVE1, CRICKIT_DUTY_CYCLE_OFF);
  delay(500);
  
  //turn all the way off
  crickit.analogWrite(CRICKIT_DRIVE1, CRICKIT_DUTY_CYCLE_MAX);
  delay(500);
}
