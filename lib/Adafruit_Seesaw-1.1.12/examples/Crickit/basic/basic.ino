#include "Adafruit_Crickit.h"

Adafruit_Crickit crickit;

void setup() {
  Serial.begin(9600);
  
  if(!crickit.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("seesaw started");

  //set the PWM freq for all the servo pins
  crickit.setPWMFreq(CRICKIT_SERVO1, 50);
}

void loop() {

  //set some PWMS
  crickit.analogWrite(CRICKIT_SERVO1, 10000);
  crickit.analogWrite(CRICKIT_SERVO2, 5000);
  crickit.analogWrite(CRICKIT_SERVO3, 20000);
  crickit.analogWrite(CRICKIT_SERVO4, 45000);

  // read an ADC
  Serial.print(crickit.analogRead(CRICKIT_SIGNAL4));
  Serial.print(",");

  // read a captouch
  Serial.println(crickit.touchRead(CRICKIT_TOUCH2));
  
  delay(1);  
}
