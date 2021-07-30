/*
  AudioFileSourceFunction
  Audio ouptut generator which can generate WAV file data from function

  Copyright (C) 2021  Hideaki Tai

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

#include "AudioFileSourceFunction.h"

AudioFileSourceFunction::AudioFileSourceFunction(float sec, uint16_t channels, uint32_t sample_per_sec, uint16_t bits_per_sample) {
  uint32_t bytes_per_sec = sample_per_sec * channels * bits_per_sample / 8;
  uint32_t len = uint32_t(sec * (float)bytes_per_sec);

  // RIFF chunk
  strncpy(wav_header.riff.chunk_id, "RIFF", 4);
  wav_header.riff.chunk_size = 4         // size of riff chunk w/o chunk_id and chunk_size
                             + 8 + 16    // size of format chunk
                             + 8 + len;  // size of data chunk
  strncpy(wav_header.riff.format, "WAVE", 4);

  // format chunk
  strncpy(wav_header.format.chunk_id, "fmt ", 4);
  wav_header.format.chunk_size = 16;
  wav_header.format.format_tag = 0x0001;  // PCM
  wav_header.format.channels = channels;
  wav_header.format.sample_per_sec = sample_per_sec;
  wav_header.format.avg_bytes_per_sec = bytes_per_sec;
  wav_header.format.block_align = channels * bits_per_sample / 8;
  wav_header.format.bits_per_sample = bits_per_sample;

  // data chunk
  strncpy(wav_header.data.chunk_id, "data", 4);
  wav_header.data.chunk_size = len;

  funcs.reserve(channels);
  pos = 0;
  size = sizeof(WavHeader) + len;
  is_ready = false;
  is_unique = false;
}

AudioFileSourceFunction::~AudioFileSourceFunction() {
  close();
}

uint32_t AudioFileSourceFunction::read(void* data, uint32_t len) {
  // callback size must be 1 or equal to channels
  if (!is_ready)
    return 0;

  uint8_t* d = reinterpret_cast<uint8_t*>(data);
  uint32_t i = 0;
  while (i < len) {
    uint32_t p = pos + i;
    if (p < sizeof(WavHeader)) {
      // header bytes
      d[i] = wav_header.bytes[p];
      i += 1;
    } else {
      // data bytes
      float time = (float)(p - sizeof(WavHeader)) / (float)wav_header.format.avg_bytes_per_sec;
      float v = funcs[0](time);
      for (size_t ch = 0; ch < wav_header.format.channels; ++ch) {
        if (!is_unique && ch > 0)
          v = funcs[ch](time);

        switch (wav_header.format.bits_per_sample) {
          case 8: {
            Uint8AndInt8 vs {int8_t(v * (float)0x7F)};
            d[i] = vs.u;
            break;
          }
          case 32: {
            Uint8AndInt32 vs {int32_t(v * (float)0x7FFFFFFF)};
            d[i + 0] = vs.u[0];
            d[i + 1] = vs.u[1];
            d[i + 2] = vs.u[2];
            d[i + 3] = vs.u[3];
            break;
          }
          case 16:
          default: {
            Uint8AndInt16 vs {int16_t(v * (float)0x7FFF)};
            d[i + 0] = vs.u[0];
            d[i + 1] = vs.u[1];
            break;
          }
        }
      }
      i += wav_header.format.block_align;
    }
  }
  pos += i;
  return (pos >= size) ? 0 : i;
}

bool AudioFileSourceFunction::seek(int32_t pos, int dir) {
  if (dir == SEEK_SET) {
    if (pos < 0 || (uint32_t)pos >= size)
      return false;
    this->pos = pos;
  } else if (dir == SEEK_CUR) {
    int32_t p = (int32_t)this->pos + pos;
    if (p < 0 || (uint32_t)p >= size)
      return false;
    this->pos = p;
  } else {
    int32_t p = (int32_t)this->size + pos;
    if (p < 0 || (uint32_t)p >= size)
      return false;
    this->pos = p;
  }
  return true;
}

bool AudioFileSourceFunction::close() {
  funcs.clear();
  pos = 0;
  size = 0;
  is_ready = false;
  is_unique = false;
  return true;
}

bool AudioFileSourceFunction::isOpen() {
  return is_ready;
}

uint32_t AudioFileSourceFunction::getSize() {
  return size;
}

uint32_t AudioFileSourceFunction::getPos() {
  return pos;
}
