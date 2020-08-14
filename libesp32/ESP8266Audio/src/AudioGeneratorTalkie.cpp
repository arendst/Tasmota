/*
  AudioGeneratorTalkie
  Audio output generator that speaks using the LPC code in old TI speech chips
  Output is locked at 8khz as that's that the hardcoded LPC coefficients are built around

  Based on the Talkie Arduino library by Peter Knight, https://github.com/going-digital/Talkie
    
  Copyright (C) 2020 Earle F. Philhower, III

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


#include "AudioGeneratorTalkie.h"

AudioGeneratorTalkie::AudioGeneratorTalkie()
{
  running = false;
  lastFrame = false;
  file = nullptr;
  output = nullptr;
  buff = nullptr;
}

AudioGeneratorTalkie::~AudioGeneratorTalkie()
{
  free(buff);
}

bool AudioGeneratorTalkie::say(const uint8_t *data, size_t len, bool async) {
  // Finish saying anything in the pipe
  while (running) {
    loop();
    delay(0);
  }
  buff = (uint8_t*)realloc(buff, len);
  if (!buff) return false;
  memcpy_P(buff, data, len);

  // Reset the interpreter to the start of the stream
  ptrAddr = buff;
  ptrBit = 0;
  frameLeft = 0;
  lastFrame = false;

  running = true;

  if (!async) {
    // Finish saying anything in the pipe
    while (running) {
      loop();
      delay(0);
    }
  }

  return true;
}

bool AudioGeneratorTalkie::begin(AudioFileSource *source, AudioOutput *output)
{
  if (!output) return false;
  this->output = output;
  if (source) {
    file = source;
    if (!file->isOpen()) return false; // Error
    auto len = file->getSize();
    uint8_t *temp = (uint8_t *)malloc(len);
    if (!temp) return false;
    if (file->read(temp, len) != (uint32_t)len) return false;
    say(temp, len);
    free(temp);
  } else {
    // Reset the interpreter to the start of the stream
    ptrAddr = buff;
    ptrBit = 0;
    frameLeft = 0;
    running = false;
  }
  
  if (!output->SetRate( 8000 )) return false;
  if (!output->SetBitsPerSample( 16 )) return false;
  if (!output->SetChannels( 2 )) return false;
  if (!output->begin()) return false;



  return true;
}


bool AudioGeneratorTalkie::stop()
{
  if (!running) return true;
  running = false;
  output->stop();
  return file ? file->close() : true;
}

bool AudioGeneratorTalkie::isRunning()
{
  return running;
}

bool AudioGeneratorTalkie::loop()
{
  if (!running) goto done; // Nothing to do here!

  if (!frameLeft) {
    if (lastFrame) {
      running = false;
      goto done;
    }
    lastFrame = genOneFrame();
  }

  if (frameLeft) {
    for ( ; frameLeft; frameLeft--) {
      auto res = genOneSample();
      int16_t r[2] = {res, res};
      if (!output->ConsumeSample(r)) break;
    }
  }
  
done:
  if (file) file->loop();
  output->loop();

  return running;
}

// The ROMs used with the TI speech were serial, not byte wide.
// Here's a handy routine to flip ROM data which is usually reversed.
uint8_t AudioGeneratorTalkie::rev(uint8_t a)
{
  // 76543210
  a = (a>>4) | (a<<4); // Swap in groups of 4
  // 32107654
  a = ((a & 0xcc)>>2) | ((a & 0x33)<<2); // Swap in groups of 2
  // 10325476
  a = ((a & 0xaa)>>1) | ((a & 0x55)<<1); // Swap bit pairs
  // 01234567
  return a;
}


uint8_t AudioGeneratorTalkie::getBits(uint8_t bits) {
	uint8_t value;
	uint16_t data;
	data = rev(ptrAddr[0])<<8;
	if (ptrBit+bits > 8) {
		data |= rev(ptrAddr[1]);
	}
	data <<= ptrBit;
	value = data >> (16-bits);
	ptrBit += bits;
	if (ptrBit >= 8) {
		ptrBit -= 8;
		ptrAddr++;
	}
	return value;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
// Constant LPC coefficient tables
static const uint8_t tmsEnergy[0x10]  = {0x00,0x02,0x03,0x04,0x05,0x07,0x0a,0x0f,0x14,0x20,0x29,0x39,0x51,0x72,0xa1,0xff};
static const uint8_t tmsPeriod[0x40]  = {0x00,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2D,0x2F,0x31,0x33,0x35,0x36,0x39,0x3B,0x3D,0x3F,0x42,0x45,0x47,0x49,0x4D,0x4F,0x51,0x55,0x57,0x5C,0x5F,0x63,0x66,0x6A,0x6E,0x73,0x77,0x7B,0x80,0x85,0x8A,0x8F,0x95,0x9A,0xA0};
static const int16_t tmsK1[0x20]      = {0x82C0,0x8380,0x83C0,0x8440,0x84C0,0x8540,0x8600,0x8780,0x8880,0x8980,0x8AC0,0x8C00,0x8D40,0x8F00,0x90C0,0x92C0,0x9900,0xA140,0xAB80,0xB840,0xC740,0xD8C0,0xEBC0,0x0000,0x1440,0x2740,0x38C0,0x47C0,0x5480,0x5EC0,0x6700,0x6D40};
static const int16_t tmsK2[0x20]      = {0xAE00,0xB480,0xBB80,0xC340,0xCB80,0xD440,0xDDC0,0xE780,0xF180,0xFBC0,0x0600,0x1040,0x1A40,0x2400,0x2D40,0x3600,0x3E40,0x45C0,0x4CC0,0x5300,0x5880,0x5DC0,0x6240,0x6640,0x69C0,0x6CC0,0x6F80,0x71C0,0x73C0,0x7580,0x7700,0x7E80};
static const int8_t tmsK3[0x10]       = {0x92,0x9F,0xAD,0xBA,0xC8,0xD5,0xE3,0xF0,0xFE,0x0B,0x19,0x26,0x34,0x41,0x4F,0x5C};
static const int8_t tmsK4[0x10]       = {0xAE,0xBC,0xCA,0xD8,0xE6,0xF4,0x01,0x0F,0x1D,0x2B,0x39,0x47,0x55,0x63,0x71,0x7E};
static const int8_t tmsK5[0x10]       = {0xAE,0xBA,0xC5,0xD1,0xDD,0xE8,0xF4,0xFF,0x0B,0x17,0x22,0x2E,0x39,0x45,0x51,0x5C};
static const int8_t tmsK6[0x10]       = {0xC0,0xCB,0xD6,0xE1,0xEC,0xF7,0x03,0x0E,0x19,0x24,0x2F,0x3A,0x45,0x50,0x5B,0x66};
static const int8_t tmsK7[0x10]       = {0xB3,0xBF,0xCB,0xD7,0xE3,0xEF,0xFB,0x07,0x13,0x1F,0x2B,0x37,0x43,0x4F,0x5A,0x66};
static const int8_t tmsK8[0x08]       = {0xC0,0xD8,0xF0,0x07,0x1F,0x37,0x4F,0x66};
static const int8_t tmsK9[0x08]       = {0xC0,0xD4,0xE8,0xFC,0x10,0x25,0x39,0x4D};
static const int8_t tmsK10[0x08]      = {0xCD,0xDF,0xF1,0x04,0x16,0x20,0x3B,0x4D};

// The chirp we active the filter using
static const int8_t chirp[] = {0x00,0x2a,0xd4,0x32,0xb2,0x12,0x25,0x14,0x02,0xe1,0xc5,0x02,0x5f,0x5a,0x05,0x0f,0x26,0xfc,0xa5,0xa5,0xd6,0xdd,0xdc,0xfc,0x25,0x2b,0x22,0x21,0x0f,0xff,0xf8,0xee,0xed,0xef,0xf7,0xf6,0xfa,0x00,0x03,0x02,0x01};
#pragma GCC diagnostic pop


bool AudioGeneratorTalkie::genOneFrame() {
  uint8_t energy;
  uint8_t repeat;

  // Read speech data, processing the variable size frames.

  energy = getBits(4);
  if (energy == 0) {
    // Energy = 0: rest frame
    synthEnergy = 0;
  } else if (energy == 0xf) {
    // Energy = 15: stop frame. Silence the synthesiser.
    synthEnergy = 0;
    synthK1 = 0;
    synthK2 = 0;
    synthK3 = 0;
    synthK4 = 0;
    synthK5 = 0;
    synthK6 = 0;
    synthK7 = 0;
    synthK8 = 0;
    synthK9 = 0;
    synthK10 = 0;
  } else {
    synthEnergy = tmsEnergy[energy];
    repeat = getBits(1);
    synthPeriod = tmsPeriod[getBits(6)];
    // A repeat frame uses the last coefficients
    if (!repeat) {
      // All frames use the first 4 coefficients
      synthK1 = tmsK1[getBits(5)];
      synthK2 = tmsK2[getBits(5)];
      synthK3 = tmsK3[getBits(4)];
      synthK4 = tmsK4[getBits(4)];
      if (synthPeriod) {
        // Voiced frames use 6 extra coefficients.
        synthK5 = tmsK5[getBits(4)];
        synthK6 = tmsK6[getBits(4)];
        synthK7 = tmsK7[getBits(4)];
        synthK8 = tmsK8[getBits(3)];
        synthK9 = tmsK9[getBits(3)];
        synthK10 = tmsK10[getBits(3)];
      }
    }
  }

  frameLeft = 8000 / 40;
  
  return (energy == 0xf); // Last frame will return true
}

int16_t AudioGeneratorTalkie::genOneSample()
{
  static uint8_t periodCounter;
  static int16_t x0,x1,x2,x3,x4,x5,x6,x7,x8,x9;
  int16_t u0,u1,u2,u3,u4,u5,u6,u7,u8,u9,u10;

  if (synthPeriod) {
    // Voiced source
    if (periodCounter < synthPeriod) {
      periodCounter++;
    } else {
      periodCounter = 0;
    }
    if (periodCounter < sizeof(chirp)) {
      u10 = ((chirp[periodCounter]) * (uint32_t) synthEnergy) >> 8;
    } else {
      u10 = 0;
    }
  } else {
    // Unvoiced source
    static uint16_t synthRand = 1;
    synthRand = (synthRand >> 1) ^ ((synthRand & 1) ? 0xB800 : 0);
    u10 = (synthRand & 1) ? synthEnergy : -synthEnergy;
  }
  // Lattice filter forward path
  u9 = u10 - (((int16_t)synthK10*x9) >> 7);
  u8 = u9 - (((int16_t)synthK9*x8) >> 7);
  u7 = u8 - (((int16_t)synthK8*x7) >> 7);
  u6 = u7 - (((int16_t)synthK7*x6) >> 7);
  u5 = u6 - (((int16_t)synthK6*x5) >> 7);
  u4 = u5 - (((int16_t)synthK5*x4) >> 7);
  u3 = u4 - (((int16_t)synthK4*x3) >> 7);
  u2 = u3 - (((int16_t)synthK3*x2) >> 7);
  u1 = u2 - (((int32_t)synthK2*x1) >> 15);
  u0 = u1 - (((int32_t)synthK1*x0) >> 15);

  // Output clamp
  if (u0 > 511) u0 = 511;
  if (u0 < -512) u0 = -512;
  
  // Lattice filter reverse path
  x9 = x8 + (((int16_t)synthK9*u8) >> 7);
  x8 = x7 + (((int16_t)synthK8*u7) >> 7);
  x7 = x6 + (((int16_t)synthK7*u6) >> 7);
  x6 = x5 + (((int16_t)synthK6*u5) >> 7);
  x5 = x4 + (((int16_t)synthK5*u4) >> 7);
  x4 = x3 + (((int16_t)synthK4*u3) >> 7);
  x3 = x2 + (((int16_t)synthK3*u2) >> 7);
  x2 = x1 + (((int32_t)synthK2*u1) >> 15);
  x1 = x0 + (((int32_t)synthK1*u0) >> 15);
  x0 = u0;

  uint16_t v = u0; // 10 bits
  v <<= 6; // Now full 16
  return v;
}
