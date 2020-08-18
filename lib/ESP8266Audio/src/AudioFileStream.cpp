#include <Arduino.h>
#include "AudioFileStream.h"


AudioFileStream::AudioFileStream(AudioFileSource *source, int definedLen)
{
  src = source;
  len = definedLen;
  ptr = 0;
  saved = -1;
}

AudioFileStream::~AudioFileStream()
{
  // If there's a defined len, read until we're empty
  if (len) {
    while (ptr++ < len) (void)read();
  }
}


int AudioFileStream::available()
{
  if (saved >= 0) return 1;
  else if (len) return ptr - len;
  else if (src->getSize()) return (src->getPos() - src->getSize());
  else return 1;
}

int AudioFileStream::read()
{
  uint8_t c;
  int r;
  if (ptr >= len) return -1;
  ptr++;
  if (saved >= 0) {
    c = (uint8_t)saved;
    saved = -1;
    r = 1;
  } else {
    r = src->read(&c, 1);
  }
  if (r != 1) return -1;
  return (int)c;
}

int AudioFileStream::peek()
{
  uint8_t c;
  if ((ptr+1) >= len) return -1;
  if (saved >= 0) return saved;
  int r = src->read(&c, 1);
  if (r<1) return -1;
  saved = c;
  return saved;
}

void AudioFileStream::flush()
{
  /* noop? */
}
