// Adafruit Crickit Capacitive Touch Demo for Arduino
//
// Displays the value of Adafruit Crickit touchpad values when touched
//
// Tested with the Crickit + micro:bit, all good

#include "Adafruit_Crickit.h"

Adafruit_Crickit crickit;

#define CRICKIT_NUM_TOUCH   4
#define CAPTOUCH_THRESH   500

void setup() {
  Serial.begin(9600);  // Set up serial monitor - be sure it is set to 9600 
  Serial.println("Cap Touch Demo");
  if(!crickit.begin()) {                         // Check if Crickit is attached
     Serial.println("ERROR Starting crickit");   // If an error, print and
     while(1) ;                                  //   go to a infinite loop to stop                       
  }
  else Serial.println("seesaw started");         // success, we have a Crickit
}

void loop() {

  for(int i=0; i<CRICKIT_NUM_TOUCH; i++){  // check each touch input
    uint16_t val = crickit.touchRead(i);   // read the touch input

    if(val > CAPTOUCH_THRESH){             // if the value read is > the threshold
      Serial.print("CT");                  //  print info to serial monitor
      Serial.print(i + 1);
      Serial.print(" touched! value: ");
      Serial.println(val);
    }
  }
  delay(100);  // wait tiny bit between checks
}
