#include <Arduino.h>
#ifdef ESP32
  #include "SPIFFS.h"
#endif
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioOutputSPDIF.h"
#include "AudioGeneratorMP3.h"

// To run, set your ESP8266 build to 160MHz, and include a SPIFFS partition 
// big enough to hold your MP3 file. Find suitable MP3 file from i.e.
// https://docs.espressif.com/projects/esp-adf/en/latest/design-guide/audio-samples.html
// and download it into 'data' directory. Use the "Tools->ESP8266/ESP32 Sketch Data Upload" 
// menu to write the MP3 to SPIFFS. Then upload the sketch normally. 

AudioFileSourceSPIFFS *file;
AudioFileSourceID3 *id3;
AudioOutputSPDIF *out;
AudioGeneratorMP3 *mp3;

// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
  Serial.printf("ID3 callback for: %s = '", type);

  if (isUnicode) {
    string += 2;
  }
  
  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
    Serial.printf("%c", a);
  }
  Serial.printf("'\n");
  Serial.flush();
}


void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  audioLogger = &Serial;
  SPIFFS.begin();
  file = new AudioFileSourceSPIFFS(); 
  id3 = NULL; 
  out = new AudioOutputSPDIF();
  mp3 = new AudioGeneratorMP3();
  String fileName = "";

  // Find first MP3 file in SPIFF and play it

#ifdef ESP32
  File dir, root = SPIFFS.open("/");  
  while ((dir = root.openNextFile())) {
    if (String(dir.name()).endsWith(".mp3")) {
      if (file->open(dir.name())) {
        fileName = String(dir.name());
        break;
      }
    }
    dir = root.openNextFile();
  }
#else
  Dir dir = SPIFFS.openDir("");
  while (dir.next()) {
    if (dir.fileName().endsWith(".mp3")) {
      if (file->open(dir.fileName().c_str())) {
        fileName = dir.fileName();
        break;
      }
    }
  }
#endif

  if (fileName.length() > 0) {
    id3 = new AudioFileSourceID3(file);
    id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
    mp3->begin(id3, out);
    Serial.printf("Playback of '%s' begins...\n", fileName.c_str());
  } else {
    Serial.println("Can't find .mp3 file in SPIFFS");
  }
}

void loop()
{
  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  } else {
    Serial.println("MP3 done");
    delay(1000);
  }
}
