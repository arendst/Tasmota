/* this example fades an rgb led based on a read encoder position */

#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

void setup() {
  Serial.begin(9600);
  
  if(!ss.begin(0x36)){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("seesaw started");
  // turn off red and green leds
  ss.analogWrite(5, 255);
  ss.analogWrite(6, 255);
}

void loop() {
  uint32_t val = abs(ss.getEncoderPosition());
  val = constrain(val*2, 0, 255);
  ss.analogWrite(4, 255-val); //write to the led
  delay(10);
}
