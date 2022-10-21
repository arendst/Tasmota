/*
  AudioOutputSPDIF
  
  S/PDIF output via I2S
  
  Needs transceiver from CMOS level to either optical or coaxial interface
  See: https://www.epanorama.net/documents/audio/spdif.html

  Original idea and sources: 
    Forum thread dicussing implementation
      https://forum.pjrc.com/threads/28639-S-pdif
    Teensy Audio Library 
      https://github.com/PaulStoffregen/Audio/blob/master/output_spdif2.cpp
   
  Adapted for ESP8266Audio

  NOTE: This module operates I2S at 4x sampling rate, as it needs to 
        send out each bit as two output symbols, packed into 
        32-bit words. Even for mono sound, S/PDIF is specified minimum
        for 2 channels, each as 32-bits sub-frame. This drains I2S 
        buffers 4x more quickly so you may need 4x bigger output 
        buffers than usual, configurable with 'dma_buf_count' 
        constructor parameter.

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

#include <Arduino.h>
#if defined(ESP32)
  #include "driver/i2s.h"
  #include "soc/rtc.h"
#elif defined(ESP8266)
  #include "driver/SinglePinI2SDriver.h"
#endif
#include "AudioOutputSPDIF.h"

// BMC (Biphase Mark Coded) values (bit order reversed, i.e. LSB first)
static const uint16_t spdif_bmclookup[256] PROGMEM = { 
	0xcccc, 0x4ccc, 0x2ccc, 0xaccc, 0x34cc, 0xb4cc, 0xd4cc, 0x54cc,
	0x32cc, 0xb2cc, 0xd2cc, 0x52cc, 0xcacc, 0x4acc, 0x2acc, 0xaacc,
	0x334c, 0xb34c, 0xd34c, 0x534c, 0xcb4c, 0x4b4c, 0x2b4c, 0xab4c,
	0xcd4c, 0x4d4c, 0x2d4c, 0xad4c, 0x354c, 0xb54c, 0xd54c, 0x554c,
	0x332c, 0xb32c, 0xd32c, 0x532c, 0xcb2c, 0x4b2c, 0x2b2c, 0xab2c,
	0xcd2c, 0x4d2c, 0x2d2c, 0xad2c, 0x352c, 0xb52c, 0xd52c, 0x552c,
	0xccac, 0x4cac, 0x2cac, 0xacac, 0x34ac, 0xb4ac, 0xd4ac, 0x54ac,
	0x32ac, 0xb2ac, 0xd2ac, 0x52ac, 0xcaac, 0x4aac, 0x2aac, 0xaaac,
	0x3334, 0xb334, 0xd334, 0x5334, 0xcb34, 0x4b34, 0x2b34, 0xab34,
	0xcd34, 0x4d34, 0x2d34, 0xad34, 0x3534, 0xb534, 0xd534, 0x5534,
	0xccb4, 0x4cb4, 0x2cb4, 0xacb4, 0x34b4, 0xb4b4, 0xd4b4, 0x54b4,
	0x32b4, 0xb2b4, 0xd2b4, 0x52b4, 0xcab4, 0x4ab4, 0x2ab4, 0xaab4,
	0xccd4, 0x4cd4, 0x2cd4, 0xacd4, 0x34d4, 0xb4d4, 0xd4d4, 0x54d4,
	0x32d4, 0xb2d4, 0xd2d4, 0x52d4, 0xcad4, 0x4ad4, 0x2ad4, 0xaad4,
	0x3354, 0xb354, 0xd354, 0x5354, 0xcb54, 0x4b54, 0x2b54, 0xab54,
	0xcd54, 0x4d54, 0x2d54, 0xad54, 0x3554, 0xb554, 0xd554, 0x5554,
	0x3332, 0xb332, 0xd332, 0x5332, 0xcb32, 0x4b32, 0x2b32, 0xab32,
	0xcd32, 0x4d32, 0x2d32, 0xad32, 0x3532, 0xb532, 0xd532, 0x5532,
	0xccb2, 0x4cb2, 0x2cb2, 0xacb2, 0x34b2, 0xb4b2, 0xd4b2, 0x54b2,
	0x32b2, 0xb2b2, 0xd2b2, 0x52b2, 0xcab2, 0x4ab2, 0x2ab2, 0xaab2,
	0xccd2, 0x4cd2, 0x2cd2, 0xacd2, 0x34d2, 0xb4d2, 0xd4d2, 0x54d2,
	0x32d2, 0xb2d2, 0xd2d2, 0x52d2, 0xcad2, 0x4ad2, 0x2ad2, 0xaad2,
	0x3352, 0xb352, 0xd352, 0x5352, 0xcb52, 0x4b52, 0x2b52, 0xab52,
	0xcd52, 0x4d52, 0x2d52, 0xad52, 0x3552, 0xb552, 0xd552, 0x5552,
	0xccca, 0x4cca, 0x2cca, 0xacca, 0x34ca, 0xb4ca, 0xd4ca, 0x54ca,
	0x32ca, 0xb2ca, 0xd2ca, 0x52ca, 0xcaca, 0x4aca, 0x2aca, 0xaaca,
	0x334a, 0xb34a, 0xd34a, 0x534a, 0xcb4a, 0x4b4a, 0x2b4a, 0xab4a,
	0xcd4a, 0x4d4a, 0x2d4a, 0xad4a, 0x354a, 0xb54a, 0xd54a, 0x554a,
	0x332a, 0xb32a, 0xd32a, 0x532a, 0xcb2a, 0x4b2a, 0x2b2a, 0xab2a,
	0xcd2a, 0x4d2a, 0x2d2a, 0xad2a, 0x352a, 0xb52a, 0xd52a, 0x552a,
	0xccaa, 0x4caa, 0x2caa, 0xacaa, 0x34aa, 0xb4aa, 0xd4aa, 0x54aa,
	0x32aa, 0xb2aa, 0xd2aa, 0x52aa, 0xcaaa, 0x4aaa, 0x2aaa, 0xaaaa
};

AudioOutputSPDIF::AudioOutputSPDIF(int dout_pin, int port, int dma_buf_count)
{
  this->portNo = port;
#if defined(ESP32)
  // Configure ESP32 I2S to roughly compatible to ESP8266 peripheral
  i2s_config_t i2s_config_spdif = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 88200, // 2 x sampling_rate 
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, // 32bit words
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, // Right than left
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 2, 0)
	  .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
#else
	  .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
#endif
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // lowest interrupt priority
    .dma_buf_count = dma_buf_count,
    .dma_buf_len = DMA_BUF_SIZE_DEFAULT, // bigger buffers, reduces interrupts
    .use_apll = true // Audio PLL is needed for low clock jitter
  };
  if (i2s_driver_install((i2s_port_t)portNo, &i2s_config_spdif, 0, NULL) != ESP_OK) {
    audioLogger->println(F("ERROR: Unable to install I2S drivers"));
    return;
  }
  i2s_zero_dma_buffer((i2s_port_t)portNo);
  SetPinout(I2S_PIN_NO_CHANGE, I2S_PIN_NO_CHANGE, dout_pin);
  rate_multiplier = 2; // 2x32bit words
#elif defined(ESP8266)
  (void) dout_pin;
  if (!I2SDriver.begin(dma_buf_count, DMA_BUF_SIZE_DEFAULT)) {
    audioLogger->println(F("ERROR: Unable to start I2S driver"));
    return;
  }
  rate_multiplier = 4; // 4x16 bit words
#endif
  i2sOn = true;
  mono = false;
  bps = 16;
  channels = 2;
  frame_num = 0;
  SetGain(1.0);
  hertz = 0;
  SetRate(44100);
}

AudioOutputSPDIF::~AudioOutputSPDIF()
{
#if defined(ESP32)
  if (i2sOn) {
    i2s_stop((i2s_port_t)this->portNo);
    audioLogger->printf("UNINSTALL I2S\n");
    i2s_driver_uninstall((i2s_port_t)this->portNo); //stop & destroy i2s driver
  }
#elif defined(ESP8266)
  if (i2sOn) I2SDriver.stop();
#endif
  i2sOn = false;
}

bool AudioOutputSPDIF::SetPinout(int bclk, int wclk, int dout)
{
#if defined(ESP32)
  i2s_pin_config_t pins = {
    .bck_io_num = bclk,
    .ws_io_num = wclk,
    .data_out_num = dout,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  if (i2s_set_pin((i2s_port_t)portNo, &pins) != ESP_OK) {
    audioLogger->println("ERROR setting up S/PDIF I2S pins\n");
    return false;
  }
  return true;
#else
  (void) bclk;
  (void) wclk;
  (void) dout;
  return false;
#endif
}

bool AudioOutputSPDIF::SetRate(int hz)
{
  if (!i2sOn) return false;
  if (hz < 32000) return false;
  if (hz == this->hertz) return true;
  this->hertz = hz;
  int adjustedHz = AdjustI2SRate(hz);
#if defined(ESP32)
  if (i2s_set_sample_rates((i2s_port_t)portNo, adjustedHz) == ESP_OK) {
    if (adjustedHz == 88200) {
      // Manually fix the APLL rate for 44100. 
      // See: https://github.com/espressif/esp-idf/issues/2634
      // sdm0 = 28, sdm1 = 8, sdm2 = 5, odir = 0 -> 88199.977
      rtc_clk_apll_enable(1, 28, 8, 5, 0); 
    }
  } else {
    audioLogger->println("ERROR changing S/PDIF sample rate");
  } 
#elif defined(ESP8266)
  I2SDriver.setRate(adjustedHz);
  audioLogger->printf_P(PSTR("S/PDIF rate set: %.3f\n"), I2SDriver.getActualRate()/4);
#endif
  return true;
}

bool AudioOutputSPDIF::SetBitsPerSample(int bits)
{
  if ( (bits != 16) && (bits != 8) ) return false;
  this->bps = bits;
  return true;
}

bool AudioOutputSPDIF::SetChannels(int channels)
{
  if ( (channels < 1) || (channels > 2) ) return false;
  this->channels = channels;
  return true;
}

bool AudioOutputSPDIF::SetOutputModeMono(bool mono)
{
  this->mono = mono;
  // Just use the left channel for mono
  if (mono) SetChannels(1);
  return true;
}

bool AudioOutputSPDIF::begin()
{
  return true;
}

bool AudioOutputSPDIF::ConsumeSample(int16_t sample[2])
{
  if (!i2sOn) return true; // Sink the data
  int16_t ms[2];
  uint16_t hi, lo, aux;
  uint32_t buf[4];

  ms[0] = sample[0];
  ms[1] = sample[1];
  MakeSampleStereo16(ms);

  // S/PDIF encoding: 
  //   http://www.hardwarebook.info/S/PDIF
  // Original sources: Teensy Audio Library 
  //   https://github.com/PaulStoffregen/Audio/blob/master/output_spdif2.cpp
  // 
  // Order of bits, before BMC encoding, from the definition of SPDIF format
  //   PPPP AAAA  SSSS SSSS  SSSS SSSS  SSSS VUCP
  // are sent rearanged as
  //   VUCP PPPP  AAAA 0000  SSSS SSSS  SSSS SSSS
  // This requires a bit less shifting as 16 sample bits align and can be 
  // BMC encoded with two table lookups (and at the same time flipped to LSB first).
  // There is no separate word-clock, so hopefully the receiver won't notice.

  uint16_t sample_left = Amplify(ms[LEFTCHANNEL]);
  // BMC encode and flip left channel bits
  hi = pgm_read_word(&spdif_bmclookup[(uint8_t)(sample_left >> 8)]);
  lo = pgm_read_word(&spdif_bmclookup[(uint8_t)sample_left]);
  // Low word is inverted depending on first bit of high word
  lo ^= (~((int16_t)hi) >> 16);
  buf[0] = ((uint32_t)lo << 16) | hi;
  // Fixed 4 bits auxillary-audio-databits, the first used as parity
  // Depending on first bit of low word, invert the bits
  aux = 0xb333 ^ (((uint32_t)((int16_t)lo)) >> 17);
  // Send 'B' preamble only for the first frame of data-block
  if (frame_num == 0) {
    buf[1] = VUCP_PREAMBLE_B | aux;
  } else {
    buf[1] = VUCP_PREAMBLE_M | aux;
  }

  uint16_t sample_right = Amplify(ms[RIGHTCHANNEL]); 
  // BMC encode right channel, similar as above
  hi = pgm_read_word(&spdif_bmclookup[(uint8_t)(sample_right >> 8)]);
  lo = pgm_read_word(&spdif_bmclookup[(uint8_t)sample_right]);
  lo ^= (~((int16_t)hi) >> 16);
  buf[2] = ((uint32_t)lo << 16) | hi;
  aux = 0xb333 ^ (((uint32_t)((int16_t)lo)) >> 17);
  buf[3] = VUCP_PREAMBLE_W | aux;

#if defined(ESP32)
  // Assume DMA buffers are multiples of 16 bytes. Either we write all bytes or none.
  size_t bytes_written;
  esp_err_t ret = i2s_write((i2s_port_t)portNo, (const char*)&buf, 8 * channels, &bytes_written, 0);
  // If we didn't write all bytes, return false early and do not increment frame_num
  if ((ret != ESP_OK) || (bytes_written != (8 * channels))) return false;  
#elif defined(ESP8266)
  if (!I2SDriver.writeInterleaved(buf)) return false;
#endif
  // Increment and rotate frame number
  if (++frame_num > 191) frame_num = 0;
  return true;
}

bool AudioOutputSPDIF::stop()
{
#if defined(ESP32)
  i2s_zero_dma_buffer((i2s_port_t)portNo);
#elif defined(ESP8266)
  I2SDriver.stop();
#endif
  frame_num = 0;
  return true;
}

#endif
