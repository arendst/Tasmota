/*
 * This example shows how to read from a seesaw encoder module.
 * The available encoder API is:
 *      int32_t getEncoderPosition();
        int32_t getEncoderDelta();
        void enableEncoderInterrupt();
        void disableEncoderInterrupt();
        void setEncoderPosition(int32_t pos);
 */

#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

void setup() {
  Serial.begin(9600);
  
  if(!ss.begin(0x36)){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("seesaw started");

}

void loop() {
  Serial.println(ss.getEncoderPosition());
  delay(400);
}
