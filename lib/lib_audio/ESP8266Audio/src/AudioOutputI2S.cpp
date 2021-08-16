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
#elif defined(ARDUINO_ARCH_RP2040) || defined(ESP8266)
  #ifdef ARDUINO_ESP8266_MAJOR    //this define was added in ESP8266 Arduino Core version v3.0.1
    #include "core_esp8266_i2s.h" //for Arduino core >= 3.0.1
  #else
    #include "i2s.h"              //for Arduino core <= 3.0.0
  #endif
#endif
#include "AudioOutputI2S.h"

#if defined(ESP32) || defined(ESP8266)
AudioOutputI2S::AudioOutputI2S(int port, int output_mode, int dma_buf_count, int use_apll)
{
  this->portNo = port;
  this->i2sOn = false;
  this->dma_buf_count = dma_buf_count;
  if (output_mode != EXTERNAL_I2S && output_mode != INTERNAL_DAC && output_mode != INTERNAL_PDM) {
    output_mode = EXTERNAL_I2S;
  }
  this->output_mode = output_mode;
  this->use_apll = use_apll;

  //set defaults
  mono = false;
  bps = 16;
  channels = 2;
  hertz = 44100;
  bclkPin = 26;
  wclkPin = 25;
  doutPin = 22;
  SetGain(1.0);
}

bool AudioOutputI2S::SetPinout()
{
  #ifdef ESP32
    if (output_mode == INTERNAL_DAC || output_mode == INTERNAL_PDM)
      return false; // Not allowed

    i2s_pin_config_t pins = {
        .bck_io_num = bclkPin,
        .ws_io_num = wclkPin,
        .data_out_num = doutPin,
        .data_in_num = I2S_PIN_NO_CHANGE};
    i2s_set_pin((i2s_port_t)portNo, &pins);
    return true;
  #else
    (void)bclkPin;
    (void)wclkPin;
    (void)doutPin;
    return false;
  #endif
}

bool AudioOutputI2S::SetPinout(int bclk, int wclk, int dout)
{
  bclkPin = bclk;
  wclkPin = wclk;
  doutPin = dout;
  if (i2sOn)
    return SetPinout();

  return true;
}
#elif defined(ARDUINO_ARCH_RP2040)
AudioOutputI2S::AudioOutputI2S(long sampleRate, pin_size_t sck, pin_size_t data) {
    i2sOn = false;
    mono = false;
    bps = 16;
    channels = 2;
    hertz = sampleRate;
    bclkPin = sck;
    doutPin = data;
    SetGain(1.0);
}
#endif

AudioOutputI2S::~AudioOutputI2S()
{
  #ifdef ESP32
    if (i2sOn) {
      audioLogger->printf("UNINSTALL I2S\n");
      i2s_driver_uninstall((i2s_port_t)portNo); //stop & destroy i2s driver
    }
  #elif defined(ESP8266)
    if (i2sOn)
      i2s_end();
  #elif defined(ARDUINO_ARCH_RP2040)
    stop();
  #endif
  i2sOn = false;
}

bool AudioOutputI2S::SetRate(int hz)
{
  // TODO - have a list of allowable rates from constructor, check them
  this->hertz = hz;
  if (i2sOn)
  {
  #ifdef ESP32
      i2s_set_sample_rates((i2s_port_t)portNo, AdjustI2SRate(hz));
  #elif defined(ESP8266)
      i2s_set_rate(AdjustI2SRate(hz));
  #elif defined(ARDUINO_ARCH_RP2040)
      I2S.setFrequency(hz);
  #endif
  }
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

bool AudioOutputI2S::begin(bool txDAC)
{
  #ifdef ESP32
    if (!i2sOn)
    {
      if (use_apll == APLL_AUTO)
      {
        // don't use audio pll on buggy rev0 chips
        use_apll = APLL_DISABLE;
        esp_chip_info_t out_info;
        esp_chip_info(&out_info);
        if (out_info.revision > 0)
        {
          use_apll = APLL_ENABLE;
        }
      }

      i2s_mode_t mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
      if (output_mode == INTERNAL_DAC)
      {
        mode = (i2s_mode_t)(mode | I2S_MODE_DAC_BUILT_IN);
      }
      else if (output_mode == INTERNAL_PDM)
      {
        mode = (i2s_mode_t)(mode | I2S_MODE_PDM);
      }

      i2s_comm_format_t comm_fmt = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB);
      if (output_mode == INTERNAL_DAC)
      {
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
      if (i2s_driver_install((i2s_port_t)portNo, &i2s_config_dac, 0, NULL) != ESP_OK)
      {
        audioLogger->println("ERROR: Unable to install I2S drives\n");
      }
      if (output_mode == INTERNAL_DAC || output_mode == INTERNAL_PDM)
      {
        i2s_set_pin((i2s_port_t)portNo, NULL);
        i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
      }
      else
      {
        SetPinout();
      }
      i2s_zero_dma_buffer((i2s_port_t)portNo);
    }
  #elif defined(ESP8266)
    (void)dma_buf_count;
    (void)use_apll;
    if (!i2sOn)
    {
      orig_bck = READ_PERI_REG(PERIPHS_IO_MUX_MTDO_U);
      orig_ws = READ_PERI_REG(PERIPHS_IO_MUX_GPIO2_U);
    #ifdef I2S_HAS_BEGIN_RXTX_DRIVE_CLOCKS
      if (!i2s_rxtxdrive_begin(false, true, false, txDAC)) {
        return false;
      }
    #else
      if (!i2s_rxtx_begin(false, true)) {
        return false;
      }
      if (!txDAC) {
        audioLogger->printf_P(PSTR("I2SNoDAC: esp8266 arduino core should be upgraded to avoid conflicts with SPI\n"));
      }
    #endif
    }
  #elif defined(ARDUINO_ARCH_RP2040)
    (void)txDAC;
    if (!i2sOn) {
        I2S.setBCLK(bclkPin);
	I2S.setDOUT(doutPin);
        I2S.begin(hertz);
    }
  #endif
  i2sOn = true;
  SetRate(hertz); // Default
  return true;
}

bool AudioOutputI2S::ConsumeSample(int16_t sample[2])
{

  //return if we haven't called ::begin yet
  if (!i2sOn)
    return false;

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
    if (output_mode == INTERNAL_DAC)
    {
      int16_t l = Amplify(ms[LEFTCHANNEL]) + 0x8000;
      int16_t r = Amplify(ms[RIGHTCHANNEL]) + 0x8000;
      s32 = (r << 16) | (l & 0xffff);
    }
    else
    {
      s32 = ((Amplify(ms[RIGHTCHANNEL])) << 16) | (Amplify(ms[LEFTCHANNEL]) & 0xffff);
    }
// Deprecated. Use i2s_write
//    return i2s_write_bytes((i2s_port_t)portNo, (const char *)&s32, sizeof(uint32_t), 0);
    size_t bytes_written;
    i2s_write((i2s_port_t)portNo, (const char*)&s32, sizeof(uint32_t), &bytes_written, 0);
    return bytes_written;
  #elif defined(ESP8266)
    uint32_t s32 = ((Amplify(ms[RIGHTCHANNEL])) << 16) | (Amplify(ms[LEFTCHANNEL]) & 0xffff);
    return i2s_write_sample_nb(s32); // If we can't store it, return false.  OTW true
  #elif defined(ARDUINO_ARCH_RP2040)
    return !!I2S.write((void*)ms, 4);
  #endif
}

void AudioOutputI2S::flush()
{
  #ifdef ESP32
    // makes sure that all stored DMA samples are consumed / played
    int buffersize = 64 * this->dma_buf_count;
    int16_t samples[2] = {0x0, 0x0};
    for (int i = 0; i < buffersize; i++)
    {
      while (!ConsumeSample(samples))
      {
        delay(10);
      }
    }
  #elif defined(ARDUINO_ARCH_RP2040)
    I2S.flush();
  #endif
}

bool AudioOutputI2S::stop()
{
  if (!i2sOn)
    return false;

  #ifdef ESP32
    i2s_zero_dma_buffer((i2s_port_t)portNo);
  #elif defined(ARDUINO_ARCH_RP2040)
    I2S.end();
  #endif
  i2sOn = false;
  return true;
}
