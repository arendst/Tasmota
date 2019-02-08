#include "Adafruit_Crickit.h"

Adafruit_Crickit crickit;

#define NUM_DRIVES 4
int drives[] = {CRICKIT_DRIVE1, CRICKIT_DRIVE2, CRICKIT_DRIVE3, CRICKIT_DRIVE4};

void setup() {
  Serial.begin(115200);
  Serial.println("4 Drive demo!");
  
  if(!crickit.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("Crickit started");

  //our default frequency is 1khz
  for(int i=0; i<NUM_DRIVES; i++)
    crickit.setPWMFreq(drives[i], 1000);
}

void loop() {
  for(int i=0; i<NUM_DRIVES; i++){
    //turn all the way on
    crickit.analogWrite(drives[i], CRICKIT_DUTY_CYCLE_OFF);
    delay(100);
    
    //turn all the way off
    crickit.analogWrite(drives[i], CRICKIT_DUTY_CYCLE_MAX);
    delay(100);
  }
}

