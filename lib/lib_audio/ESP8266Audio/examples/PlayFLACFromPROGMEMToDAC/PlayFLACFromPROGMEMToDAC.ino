#include <Arduino.h>
#include <AudioOutputI2S.h>
#include <AudioFileSourcePROGMEM.h>
#include <AudioGeneratorFLAC.h>

#include "sample.h"

AudioOutputI2S *out;
AudioFileSourcePROGMEM *file;
AudioGeneratorFLAC *flac;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up...\n");

  audioLogger = &Serial;
  file = new AudioFileSourcePROGMEM( sample_flac, sizeof(sample_flac) );
  out = new AudioOutputI2S();
  flac = new AudioGeneratorFLAC();
  flac->begin(file, out);
}

void loop()
{
  if (flac->isRunning()) {
    if (!flac->loop()) flac->stop();
  } else {
    Serial.printf("FLAC done\n");
    delay(1000);
  }
}

