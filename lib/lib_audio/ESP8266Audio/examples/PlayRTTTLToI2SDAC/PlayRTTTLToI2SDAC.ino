#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorRTTTL.h"
#include "AudioOutputI2S.h"

const char rudolph[] PROGMEM = 
"Rudolph the Red Nosed Raindeer:d=8,o=5,b=250:g,4a,g,4e,4c6,4a,2g.,g,a,g,a,4g,4c6,2b.,4p,f,4g,f,4d,4b,4a,2g.,g,a,g,a,4g,4a,2e.,4p,g,4a,a,4e,4c6,4a,2g.,g,a,g,a,4g,4c6,2b.,4p,f,4g,f,4d,4b,4a,2g.,g,a,g,a,4g,4d6,2c.6,4p,4a,4a,4c6,4a,4g,4e,2g,4d,4e,4g,4a,4b,4b,2b,4c6,4c6,4b,4a,4g,4f,2d,g,4a,g,4e,4c6,4a,2g.,g,a,g,a,4g,4c6,2b.,4p,f,4g,f,4d,4b,4a,2g.,4g,4a,4g,4a,2g,2d6,1c.6.";
// Plenty more at: http://mines.lumpylumpy.com/Electronics/Computers/Software/Cpp/MFC/RingTones.RTTTL

AudioGeneratorRTTTL *rtttl;
AudioFileSourcePROGMEM *file;
AudioOutputI2S *out;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.printf("RTTTL start\n");

  audioLogger = &Serial;
  file = new AudioFileSourcePROGMEM( rudolph, strlen_P(rudolph) );
  out = new AudioOutputI2S();
  rtttl = new AudioGeneratorRTTTL();
  rtttl->begin(file, out);
}

void loop()
{
  if (rtttl->isRunning()) {
    if (!rtttl->loop()) rtttl->stop();
  } else {
    Serial.printf("RTTTL done\n");
    delay(1000);
  }
}

