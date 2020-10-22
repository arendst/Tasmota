/*
  AudioGeneratorOpus
  Audio output generator that plays Opus audio files
    
  Copyright (C) 2020  Earle F. Philhower, III

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

#include <AudioGeneratorOpus.h>

AudioGeneratorOpus::AudioGeneratorOpus()
{
  of = nullptr;
  buff = nullptr;
  buffPtr = 0;
  buffLen = 0;
  running = false;
}

AudioGeneratorOpus::~AudioGeneratorOpus()
{
  if (of) op_free(of);
  of = nullptr;
  free(buff);
  buff = nullptr;
}

#define OPUS_BUFF 1024

bool AudioGeneratorOpus::begin(AudioFileSource *source, AudioOutput *output)
{
  buff = (int16_t*)malloc(OPUS_BUFF * sizeof(int16_t));
  if (!buff) return false;

  if (!source) return false;
  file = source;
  if (!output) return false;
  this->output = output;
  if (!file->isOpen()) return false; // Error

  of = op_open_callbacks((void*)this, &cb, nullptr, 0, nullptr);
  if (!of) return false;

  prev_li = -1;
  lastSample[0] = 0;
  lastSample[1] = 0;

  buffPtr = 0;
  buffLen = 0;

  output->begin();

  // These are fixed by Opus
  output->SetRate(48000);
  output->SetBitsPerSample(16);
  output->SetChannels(2);

  running = true;
  return true;
}

bool AudioGeneratorOpus::loop()
{

  if (!running) goto done;

  if (!output->ConsumeSample(lastSample)) goto done; // Try and send last buffered sample

  do {
    if (buffPtr == buffLen) {
      int ret = op_read_stereo(of, (opus_int16 *)buff, OPUS_BUFF);
      if (ret == OP_HOLE) {
        // fprintf(stderr,"\nHole detected! Corrupt file segment?\n");
        continue;
      } else if (ret <= 0) {
        running = false;
        goto done;
      }
     buffPtr = 0;
     buffLen = ret * 2;
    }

    lastSample[AudioOutput::LEFTCHANNEL] = buff[buffPtr] & 0xffff; 
    lastSample[AudioOutput::RIGHTCHANNEL] = buff[buffPtr+1] & 0xffff; 
    buffPtr += 2;
  } while (running && output->ConsumeSample(lastSample));

done:
  file->loop();
  output->loop();

  return running;
}

bool AudioGeneratorOpus::stop()
{
  if (of) op_free(of);
  of = nullptr;
  free(buff);
  buff = nullptr;
  running = false;
  output->stop();
  return true;
}

bool AudioGeneratorOpus::isRunning()
{
  return running;
}

int AudioGeneratorOpus::read_cb(unsigned char *_ptr, int _nbytes) {
  if (_nbytes == 0) return 0;
  _nbytes = file->read(_ptr, _nbytes);
  if (_nbytes == 0) return -1;
  return _nbytes;
}

int AudioGeneratorOpus::seek_cb(opus_int64 _offset, int _whence) {
  if (!file->seek((int32_t)_offset, _whence)) return -1;
  return 0;
}

opus_int64 AudioGeneratorOpus::tell_cb() {
  return file->getPos();
}

int AudioGeneratorOpus::close_cb() {
  // NO OP, we close in main loop
  return 0;
}
