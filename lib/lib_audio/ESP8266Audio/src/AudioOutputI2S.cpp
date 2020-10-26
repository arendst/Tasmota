/*
  AudioOutputI2S
  Base class for I2S interface port
  
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

#include <Arduino.h>
#ifdef ESP32
  #include "driver/i2s.h"
#else
  #include <i2s.h>
#endif
#include "AudioOutputI2S.h"

AudioOutputI2S::AudioOutputI2S(int port, int output_mode, int dma_buf_count, int use_apll)
{
  this->portNo = port;
  this->i2sOn = false;
  this->dma_buf_count = dma_buf_count;
  if (output_mode != EXTERNAL_I2S && output_mode != INTERNAL_DAC && output_mode != INTERNAL_PDM) {
    output_mode = EXTERNAL_I2S;
  }
  this->output_mode = output_mode;
#ifdef ESP32
  if (!i2sOn) {
    if (use_apll == APLL_AUTO) {
      // don't use audio pll on buggy rev0 chips
      use_apll = APLL_DISABLE;
      esp_chip_info_t out_info;
      esp_chip_info(&out_info);
      if(out_info.revision > 0) {
        use_apll = APLL_ENABLE;
      }
    }

    i2s_mode_t mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
    if (output_mode == INTERNAL_DAC) {
      mode = (i2s_mode_t)(mode | I2S_MODE_DAC_BUILT_IN);
    } else if (output_mode == INTERNAL_PDM) {
      mode = (i2s_mode_t)(mode | I2S_MODE_PDM);
    }

    i2s_comm_format_t comm_fmt = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB);
    if (output_mode == INTERNAL_DAC) {
      comm_fmt = (i2s_comm_format_t)I2S_COMM_FORMAT_I2S_MSB;
    }

    i2s_config_t i2s_config_dac = {
      .mode = mode,
      .sample_rate = 44100,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = comm_fmt,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // lowest interrupt priority
      .dma_buf_count = dma_buf_count,
      .dma_buf_len = 64,
      .use_apll = use_apll // Use audio PLL
    };
    audioLogger->printf("+%d %p\n", portNo, &i2s_config_dac);
    if (i2s_driver_install((i2s_port_t)portNo, &i2s_config_dac, 0, NULL) != ESP_OK) {
      audioLogger->println("ERROR: Unable to install I2S drives\n");
    }
    if (output_mode == INTERNAL_DAC || output_mode == INTERNAL_PDM) {
      i2s_set_pin((i2s_port_t)portNo, NULL);
      i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
    } else {
      SetPinout(26, 25, 22);
    }
    i2s_zero_dma_buffer((i2s_port_t)portNo);
  } 
#else
  (void) dma_buf_count;
  (void) use_apll;
  if (!i2sOn) {
    orig_bck = READ_PERI_REG(PERIPHS_IO_MUX_MTDO_U);
    orig_ws = READ_PERI_REG(PERIPHS_IO_MUX_GPIO2_U);
    i2s_begin();
  }
#endif
  i2sOn = true;
  mono = false;
  bps = 16;
  channels = 2;
  SetGain(1.0);
  SetRate(44100); // Default
}

AudioOutputI2S::~AudioOutputI2S()
{
#ifdef ESP32
  if (i2sOn) {
    audioLogger->printf("UNINSTALL I2S\n");
    i2s_driver_uninstall((i2s_port_t)portNo); //stop & destroy i2s driver
  }
#else
  if (i2sOn) i2s_end();
#endif
  i2sOn = false;
}

bool AudioOutputI2S::SetPinout(int bclk, int wclk, int dout)
{
#ifdef ESP32
  if (output_mode == INTERNAL_DAC || output_mode == INTERNAL_PDM) return false; // Not allowed

  i2s_pin_config_t pins = {
    .bck_io_num = bclk,
    .ws_io_num = wclk,
    .data_out_num = dout,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  i2s_set_pin((i2s_port_t)portNo, &pins);
  return true;
#else
  (void) bclk;
  (void) wclk;
  (void) dout;
  return false;
#endif
}

bool AudioOutputI2S::SetRate(int hz)
{
  // TODO - have a list of allowable rates from constructor, check them
  this->hertz = hz;
#ifdef ESP32
  i2s_set_sample_rates((i2s_port_t)portNo, AdjustI2SRate(hz)); 
#else
  i2s_set_rate(AdjustI2SRate(hz));
#endif
  return true;
}

bool AudioOutputI2S::SetBitsPerSample(int bits)
{
  if ( (bits != 16) && (bits != 8) ) return false;
  this->bps = bits;
  return true;
}

bool AudioOutputI2S::SetChannels(int channels)
{
  if ( (channels < 1) || (channels > 2) ) return false;
  this->channels = channels;
  return true;
}

bool AudioOutputI2S::SetOutputModeMono(bool mono)
{
  this->mono = mono;
  return true;
}

bool AudioOutputI2S::begin()
{
  return true;
}

bool AudioOutputI2S::ConsumeSample(int16_t sample[2])
{
  int16_t ms[2];

  ms[0] = sample[0];
  ms[1] = sample[1];
  MakeSampleStereo16( ms );

  if (this->mono) {
    // Average the two samples and overwrite
    int32_t ttl = ms[LEFTCHANNEL] + ms[RIGHTCHANNEL];
    ms[LEFTCHANNEL] = ms[RIGHTCHANNEL] = (ttl>>1) & 0xffff;
  }
#ifdef ESP32
  uint32_t s32;
  if (output_mode == INTERNAL_DAC) {
    int16_t l = Amplify(ms[LEFTCHANNEL]) + 0x8000;
    int16_t r = Amplify(ms[RIGHTCHANNEL]) + 0x8000;
    s32 = (r<<16) | (l&0xffff);
  } else {
    s32 = ((Amplify(ms[RIGHTCHANNEL]))<<16) | (Amplify(ms[LEFTCHANNEL]) & 0xffff);
  }
  return i2s_write_bytes((i2s_port_t)portNo, (const char*)&s32, sizeof(uint32_t), 0);
#else
  uint32_t s32 = ((Amplify(ms[RIGHTCHANNEL]))<<16) | (Amplify(ms[LEFTCHANNEL]) & 0xffff);
  return i2s_write_sample_nb(s32); // If we can't store it, return false.  OTW true
#endif
}

void AudioOutputI2S::flush() {
#ifdef ESP32
  // makes sure that all stored DMA samples are consumed / played
  int buffersize = 64 * this->dma_buf_count;
  int16_t samples[2] = {0x0,0x0};
  for (int i=0;i<buffersize; i++) {
    while (!ConsumeSample(samples)) {
      delay(10);
    }
  }
#endif
}

bool AudioOutputI2S::stop()
{
#ifdef ESP32
  i2s_zero_dma_buffer((i2s_port_t)portNo);
#endif
  return true;
}


