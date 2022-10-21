#include <Arduino.h>
#include "AudioFileSourceFunction.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"

float hz = 440.f;

// pre-defined function can also be used to generate the wave
float sine_wave(const float time) {
  float v = sin(TWO_PI * hz * time);  // C
  v *= fmod(time, 1.f);               // change linear
  v *= 0.5;                           // scale
  return v;
};

AudioGeneratorWAV* wav;
AudioFileSourceFunction* file;
AudioOutputI2SNoDAC* out;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // ===== create instance with length of song in [sec] =====
  file = new AudioFileSourceFunction(8.);
  //
  // you can set (sec, channels, hz, bit/sample) but you should care about
  // the trade-off between performance and the audio quality
  //
  // file = new AudioFileSourceFunction(sec, channels, hz, bit/sample);
  // channels   : default = 1
  // hz         : default = 8000 (8000, 11025, 22050, 44100, 48000, etc.)
  // bit/sample : default = 16 (8, 16, 32)

  // ===== set your sound function =====
  file->addAudioGenerators([&](const float time) {
    float v = sin(TWO_PI * hz * time);  // generate sine wave
    v *= fmod(time, 1.f);               // change linear
    v *= 0.5;                           // scale
    return v;
  });
  //
  // sound function should have one argument(float) and one return(float)
  // param  : float (current time [sec] of the song)
  // return : float (the amplitude of sound which varies from -1.f to +1.f)
  //
  // sound function can be registered only one or the same number with channels
  // if the channels > 1 && the number of function == 1,
  // same function are used to generate the sound in every channel
  //
  // file = new AudioFileSourceFunction(8., 2);
  // file->addAudioGenerators(
  //   // L (channel 0)
  //   [](const float time) {
  //     return 0.25 * sin(TWO_PI * 440.f * time) * fmod(time, 1.f); // C
  //   },
  //   // R (channel 1)
  //   [](const float time) {
  //     return 0.25 * sin(TWO_PI * 550.f * time) * fmod(time, 1.f); // E
  //   }
  // );
  //
  // you can also use the pre-defined function
  // file->addAudioGenerators(sine_wave);

  out = new AudioOutputI2SNoDAC();
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);
}

void loop() {
  if (wav->isRunning()) {
    if (!wav->loop()) wav->stop();
  } else {
    Serial.println("function done!");
    delay(1000);
  }
}
