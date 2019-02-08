#include "Adafruit_Crickit.h"
#include "seesaw_servo.h"

Adafruit_Crickit crickit;
seesaw_Servo myservo(&crickit);  // create servo object to control a servo

void setup() {
  Serial.begin(115200);
  
  if(!crickit.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("Crickit started");
  
  myservo.attach(CRICKIT_SERVO1);  // attaches the servo to CRICKIT_SERVO1 pin
}

void loop() {
  myservo.write(0);
  delay(1000);
  myservo.write(90);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo.write(90);
  delay(1000);
}

