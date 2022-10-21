#include <Arduino.h>
#ifdef ESP32
    void setup() {
        Serial.begin(115200);
        Serial.printf("ERROR - ESP32 does not support LittleFS\n");
    }
    void loop() {}
#else
#include <ESP8266WiFi.h>
#include <AudioOutputI2S.h>
#include <AudioGeneratorMIDI.h>
#include <AudioFileSourceLittleFS.h>

AudioFileSourceLittleFS *sf2;
AudioFileSourceLittleFS *mid;
AudioOutputI2S *dac;
AudioGeneratorMIDI *midi;

void setup()
{
  const char *soundfont = "/1mgm.sf2";
  const char *midifile = "/furelise.mid";

  WiFi.mode(WIFI_OFF); 

  Serial.begin(115200);
  Serial.println("Starting up...\n");

  audioLogger = &Serial;
  sf2 = new AudioFileSourceLittleFS(soundfont);
  mid = new AudioFileSourceLittleFS(midifile);
  
  dac = new AudioOutputI2S();
  midi = new AudioGeneratorMIDI();
  midi->SetSoundfont(sf2);
  midi->SetSampleRate(22050);
  Serial.printf("BEGIN...\n");
  midi->begin(mid, dac);
}

void loop()
{
  if (midi->isRunning()) {
    if (!midi->loop()) {
      uint32_t e = millis();
      midi->stop();
    }
  } else {
    Serial.printf("MIDI done\n");
    delay(1000);
  }
}

#endif
