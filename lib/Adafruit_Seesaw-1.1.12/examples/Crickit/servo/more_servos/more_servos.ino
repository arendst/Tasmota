#include "Adafruit_Crickit.h"
#include "seesaw_servo.h"

Adafruit_Crickit crickit;

#define NUM_SERVOS 4

//create an array of 4 servos with our crickit object
seesaw_Servo servos[] = { seesaw_Servo(&crickit), 
                          seesaw_Servo(&crickit), 
                          seesaw_Servo(&crickit),
                          seesaw_Servo(&crickit) };

//these are the pins they will be attached to
int servoPins[] = { CRICKIT_SERVO1, CRICKIT_SERVO2, CRICKIT_SERVO3, CRICKIT_SERVO4 };

void setup() {
  Serial.begin(115200);

  //begin the crickit
  if(!crickit.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("Crickit started");

  //attach the servos to their pins
  for(int i=0; i<NUM_SERVOS; i++)
    servos[i].attach(servoPins[i]);  // attaches the servo to the pin
}

void loop() {
  //repeat for all 4 servos
  for(int i=0; i<NUM_SERVOS; i++){
    servos[i].write(0);
    delay(1000);
    servos[i].write(90);
    delay(1000);
    servos[i].write(180);
    delay(1000);
    servos[i].write(90);
    delay(1000);
  }
}

