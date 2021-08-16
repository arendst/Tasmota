/*
  AudioOutputSPDIF
  
  S/PDIF output via I2S
  
  Needs transciever from CMOS level to either optical or coaxial interface
  See: https://www.epanorama.net/documents/audio/spdif.html

  Original idea and sources: 
    Forum thread dicussing implementation
      https://forum.pjrc.com/threads/28639-S-pdif
    Teensy Audio Library 
      https://github.com/PaulStoffregen/Audio/blob/master/output_spdif2.cpp
   
  Adapted for ESP8266Audio

  Copyright (C) 2020 Ivan Kostoski

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

#if defined(ESP32) || defined(ESP8266)
#pragma once

#include "AudioOutput.h"

#if defined(ESP32)
#define SPDIF_OUT_PIN_DEFAULT  27
#define DMA_BUF_COUNT_DEFAULT  8
#define DMA_BUF_SIZE_DEFAULT   256
#elif defined(ESP8266)
#define SPDIF_OUT_PIN_DEFAULT  3
#define DMA_BUF_COUNT_DEFAULT  32
#define DMA_BUF_SIZE_DEFAULT   64
#endif

class AudioOutputSPDIF : public AudioOutput
{
  public:
    AudioOutputSPDIF(int dout_pin=SPDIF_OUT_PIN_DEFAULT, int port=0, int dma_buf_count = DMA_BUF_COUNT_DEFAULT);
    virtual ~AudioOutputSPDIF() override;
    bool SetPinout(int bclkPin, int wclkPin, int doutPin);
    virtual bool SetRate(int hz) override;
    virtual bool SetBitsPerSample(int bits) override;
    virtual bool SetChannels(int channels) override;
    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual bool stop() override;

    bool SetOutputModeMono(bool mono);  // Force mono output no matter the input

    const uint32_t VUCP_PREAMBLE_B = 0xCCE80000; // 11001100 11101000
    const uint32_t VUCP_PREAMBLE_M = 0xCCE20000; // 11001100 11100010
    const uint32_t VUCP_PREAMBLE_W = 0xCCE40000; // 11001100 11100100

  protected:
    virtual inline int AdjustI2SRate(int hz) { return rate_multiplier * hz; }
    uint8_t portNo;
    bool mono;
    bool i2sOn;
    uint8_t frame_num;
    uint8_t rate_multiplier;
};

#endif // _AUDIOOUTPUTSPDIF_H
