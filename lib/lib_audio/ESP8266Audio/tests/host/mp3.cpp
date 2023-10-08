#include <Arduino.h>
#include "AudioFileSourceSTDIO.h"
#include "AudioOutputSTDIO.h"
#include "AudioGeneratorMP3.h"
#include "AudioFileSourceID3.h"
#include "AudioFileSourceBuffer.h"
#include "AudioOutputMixer.h"

#define MP3 "../../examples/PlayMP3FromSPIFFS/data/pno-cs.mp3"

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


// Called when there's a warning or error (like a buffer underflow or decode hiccup)
void StatusCallback(void *cbData, int code, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  // Note that the string may be in PROGMEM, so copy it to RAM for printf
  char s1[64];
  strncpy_P(s1, string, sizeof(s1));
  s1[sizeof(s1)-1]=0;
  Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
  Serial.flush();
}


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    AudioFileSourceSTDIO *in = new AudioFileSourceSTDIO(MP3);
    AudioFileSourceBuffer *buff = new AudioFileSourceBuffer(in, 2048);
    buff->RegisterStatusCB(StatusCallback, (void*)"buffer");
    AudioFileSourceID3 *id3 = new AudioFileSourceID3(buff);
    id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
    AudioOutputSTDIO *out = new AudioOutputSTDIO();
    out->SetFilename("jamonit.wav");
    AudioOutputMixer *mix = new AudioOutputMixer(17, out);
    AudioOutputMixerStub *stub = mix->NewInput();
    void *space = malloc(29192);
    AudioGeneratorMP3 *mp3 = new AudioGeneratorMP3(space, 29192);

    mp3->begin(id3, stub);
    while (mp3->loop()) { /*noop*/ }
    mp3->stop();
    out->stop();

    free(space);
    delete stub;
    delete mix;
    delete mp3;
    delete out;
    delete id3;
    delete buff;
    delete in;
}
