#include <Arduino.h>
#include "AudioFileSourceSD.h"
#include "AudioOutputSPDIF.h"
#include "AudioGeneratorFLAC.h"

// For this sketch, you need connected SD card with '.flac' music files in the root
// directory. Some samples with various sampling rates are available from i.e. 
// Espressif Audio Development Framework at:
// https://docs.espressif.com/projects/esp-adf/en/latest/design-guide/audio-samples.html
//
// On ESP8266 you might need to reencode FLAC files with max '-2' compression level 
// (i.e. 1152 maximum block size) or you will run out of memory. FLAC files will be 
// slightly bigger but you don't loose audio quality with reencoding (lossles codec).

// You may need a fast SD card. Set this as high as it will work (40MHz max).
#define SPI_SPEED SD_SCK_MHZ(40)

// On ESP32 you can adjust the SPDIF_OUT_PIN (GPIO number). 
// On ESP8266 it is fixed to GPIO3/RX0 and this setting has no effect
#define SPDIF_OUT_PIN 27

File dir;
AudioFileSourceSD *source = NULL;
AudioOutputSPDIF *output = NULL;
AudioGeneratorFLAC *decoder = NULL;

void setup() {
  Serial.begin(115200);
  Serial.println();
  delay(1000);

  audioLogger = &Serial;  
  source = new AudioFileSourceSD();
  output = new AudioOutputSPDIF(SPDIF_OUT_PIN);
  decoder = new AudioGeneratorFLAC();

  // NOTE: SD.begin(...) should be called AFTER AudioOutputSPDIF() 
  //       to takover the the SPI pins if they share some with I2S
  //       (i.e. D8 on Wemos D1 mini is both I2S BCK and SPI SS)
  #if defined(ESP8266)
    SD.begin(SS, SPI_SPEED);
  #else
    SD.begin();
  #endif
  dir = SD.open("/"); 
}

void loop() {
  if ((decoder) && (decoder->isRunning())) {
    if (!decoder->loop()) decoder->stop();
  } else {
    File file = dir.openNextFile();
    if (file) {      
      if (String(file.name()).endsWith(".flac")) {
        source->close();
        if (source->open(file.name())) { 
          Serial.printf_P(PSTR("Playing '%s' from SD card...\n"), file.name());
          decoder->begin(source, output);
        } else {
          Serial.printf_P(PSTR("Error opening '%s'\n"), file.name());
        }
      } 
    } else {
      Serial.println(F("Playback form SD card done\n"));
      delay(1000);
    }       
  }
}
