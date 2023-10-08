#include <Arduino.h>
#include <ESP8266SAM.h>
#include "AudioOutputI2SNoDAC.h"

AudioOutputI2SNoDAC *out = NULL;

void setup()
{
  out = new AudioOutputI2SNoDAC();
  out->begin();
}

void loop()
{
  ESP8266SAM *sam = new ESP8266SAM;
  sam->Say(out, "Can you hear me now?");
  delay(500);
  sam->Say(out, "I can't hear you!");
  delete sam;
}
