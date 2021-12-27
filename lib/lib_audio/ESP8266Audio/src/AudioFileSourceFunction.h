/*
  AudioFileSourceFunction
  Audio output generator which can generate WAV file data from function

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

#ifndef _AUDIOFILESOURCEFUNCTION_H
#define _AUDIOFILESOURCEFUNCTION_H

#include <Arduino.h>
#include <vector>
#include <functional>

#include "AudioFileSource.h"

class AudioFileSourceFunction : public AudioFileSource {
  union WavHeader {
    struct {
      // RIFF chunk
      struct {
        char chunk_id[4];     // "RIFF"
        uint32_t chunk_size;  // 4 + (8 + sizeof(format_chunk)(16)) + (8 + sizeof(data_chunk))
        char format[4];       // "WAVE"
      } riff;
      // format chunk
      struct {
        char chunk_id[4];            // "fmt "
        uint32_t chunk_size;         // 16
        uint16_t format_tag;         // 1: PCM
        uint16_t channels;           // 1: MONO, 2: STEREO
        uint32_t sample_per_sec;     // 8000, 11025, 22050, 44100, 48000
        uint32_t avg_bytes_per_sec;  // sample_per_sec * channels * bits_per_sample / 8
        uint16_t block_align;        // channels * bits_per_sample / 8
        uint16_t bits_per_sample;    // 8, 16, 32
      } format;
      // data chunk
      struct {
        char chunk_id[4];     // "data"
        uint32_t chunk_size;  // num_samples * channels * bytes_per_sample
        // audio data follows here...
      } data;
    };
    uint8_t bytes[44];
  } wav_header;

  union Uint8AndInt8 {
    int8_t i;
    uint8_t u;
  };

  union Uint8AndInt16 {
    int16_t i;
    uint8_t u[2];
  };

  union Uint8AndInt32 {
    int32_t i;
    uint8_t u[4];
  };

  using callback_t = std::function<float(float)>;
  std::vector<callback_t> funcs;
  uint32_t pos;
  uint32_t size;
  bool is_ready;
  bool is_unique;

public:
  AudioFileSourceFunction(float sec, uint16_t channels = 1, uint32_t sample_per_sec = 8000, uint16_t bits_per_sample = 16);
  virtual ~AudioFileSourceFunction() override;

  template <typename F, typename... Fs>
  bool addAudioGenerators(const F& f, Fs&&... fs) {
    funcs.emplace_back(f);
    return addAudioGenerators(std::forward<Fs>(fs)...);
  }
  bool addAudioGenerators() {
    funcs.shrink_to_fit();
    if (funcs.size() == 1) {
      is_ready = true;
      is_unique = true;
      return true;
    } else if (funcs.size() == wav_header.format.channels) {
      is_ready = true;
      is_unique = false;
      return true;
    } else {
      is_ready = false;
      is_unique = false;
      funcs.clear();
      return false;
    }
  }

  virtual uint32_t read(void* data, uint32_t len) override;
  virtual bool seek(int32_t pos, int dir) override;

  virtual bool close() override;
  virtual bool isOpen() override;

  virtual uint32_t getSize() override;
  virtual uint32_t getPos() override;
};

#endif  // _AUDIOFILESOURCEFUNCTION_H
