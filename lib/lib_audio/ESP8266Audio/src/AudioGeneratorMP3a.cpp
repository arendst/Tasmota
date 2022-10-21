/*
  AudioGeneratorMP3
  Audio output generator using the Helix MP3 decoder
  
  Copyright (C) 2017  Earle F. Philhower, III

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma GCC optimize ("O3")

#include "AudioGeneratorMP3a.h"


AudioGeneratorMP3a::AudioGeneratorMP3a()
{
  running = false;
  file = NULL;
  output = NULL;
  hMP3Decoder = MP3InitDecoder();
  if (!hMP3Decoder) {
    audioLogger->printf_P(PSTR("Out of memory error! hMP3Decoder==NULL\n"));
    Serial.flush();
  }
  // For sanity's sake...
  memset(buff, 0, sizeof(buff));
  memset(outSample, 0, sizeof(outSample));
  buffValid = 0;
  lastFrameEnd = 0;
  validSamples = 0;
  curSample = 0;
  lastRate = 0;
  lastChannels = 0;
}

AudioGeneratorMP3a::~AudioGeneratorMP3a()
{
  MP3FreeDecoder(hMP3Decoder);
}

bool AudioGeneratorMP3a::stop()
{
  if (!running) return true;
  running = false;
  output->stop();
  return file->close();
}

bool AudioGeneratorMP3a::isRunning()
{
  return running;
}

bool AudioGeneratorMP3a::FillBufferWithValidFrame()
{
  buff[0] = 0; // Destroy any existing sync word @ 0
  int nextSync;
  do {
    nextSync = MP3FindSyncWord(buff + lastFrameEnd, buffValid - lastFrameEnd);
    if (nextSync >= 0) nextSync += lastFrameEnd;
    lastFrameEnd = 0;
    if (nextSync == -1) {
      if (buff[buffValid-1]==0xff) { // Could be 1st half of syncword, preserve it...
        buff[0] = 0xff;
        buffValid = file->read(buff+1, sizeof(buff)-1);
        if (buffValid==0) return false; // No data available, EOF
      } else { // Try a whole new buffer
        buffValid = file->read(buff, sizeof(buff));
        if (buffValid==0) return false; // No data available, EOF
      }
    }
  } while (nextSync == -1);

  // Move the frame to start at offset 0 in the buffer
  buffValid -= nextSync; // Throw out prior to nextSync
  memmove(buff, buff+nextSync, buffValid);

  // We have a sync word at 0 now, try and fill remainder of buffer
  buffValid += file->read(buff + buffValid, sizeof(buff) - buffValid);

  return true;
}

bool AudioGeneratorMP3a::loop()
{
  if (!running) goto done; // Nothing to do here!

  // If we've got data, try and pump it out...
  while (validSamples) {
    lastSample[0] = outSample[curSample*2];
    lastSample[1] = outSample[curSample*2 + 1];
    if (!output->ConsumeSample(lastSample)) goto done; // Can't send, but no error detected
    validSamples--;
    curSample++;
  }

  // No samples available, need to decode a new frame
  if (FillBufferWithValidFrame()) {
    // buff[0] start of frame, decode it...
    unsigned char *inBuff = reinterpret_cast<unsigned char *>(buff);
    int bytesLeft = buffValid;
    int ret = MP3Decode(hMP3Decoder, &inBuff, &bytesLeft, outSample, 0);
   if (ret) {
      // Error, skip the frame...
      char buff[48];
      sprintf(buff, "MP3 decode error %d", ret);
      cb.st(ret, buff);
    } else {
      lastFrameEnd = buffValid - bytesLeft;
      MP3FrameInfo fi;
      MP3GetLastFrameInfo(hMP3Decoder, &fi);
      if ((int)fi.samprate!= (int)lastRate) {
        output->SetRate(fi.samprate);
        lastRate = fi.samprate;
      }
      if (fi.nChans != lastChannels) {
        output->SetChannels(fi.nChans);
        lastChannels = fi.nChans;
      }
      curSample = 0;
      validSamples = fi.outputSamps / lastChannels;
    }
  } else {
    running = false; // No more data, we're done here...
  }

done:
  file->loop();
  output->loop();

  return running;
}

bool AudioGeneratorMP3a::begin(AudioFileSource *source, AudioOutput *output)
{
  if (!source) return false;
  file = source;
  if (!output) return false;
  this->output = output;
  if (!file->isOpen()) return false; // Error

  output->begin();
  
  // AAC always comes out at 16 bits
  output->SetBitsPerSample(16);
  
  running = true;
  
  return true;
}

