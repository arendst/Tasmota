/*
  TasmotaLED.cpp - Lightweight implementation for adressable leds.

  Copyright (C) 2024  Stephan Hadinger

  This library is free software: you can redistribute it and/or modify
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

#include "TasmotaLEDPusher.h"
#include "TasmotaLED.h"

// DRAM_ATTR to force in IRAM because we use this in show loop
static const DRAM_ATTR uint8_t TASMOTALED_CHANNEL_ORDERS[8][3] = {
    {1, 0, 2},  // Def=GRB (0)
    {1, 0, 2},  // GRB (1)
    {0, 1, 2},  // RGB (2)
    {0, 2, 1},  // RBG (3)
    {2, 1, 0},  // BRG (4)
    {1, 2, 0},  // BGR (5)
    {2, 0, 1},  // GBR (6)
    {1, 0, 2}   // GRB (7)  // fallback if erroneous value
};

static const TasmotaLED_Timing TasmotaLED_Timings[] = {
  // WS2812
  // RmtBit0 0x00228010 RmtBit1 0x00128020 RmtReset 0x800207D0
  {
    .T0H = 400,
    .T0L = 850,
    .T1H = 800,
    .T1L = 450,
    .Reset = 80000      // it is 50000 for WS2812, but for compatibility with SK6812, we raise to 80000
  },
  // SK6812
  // RmtBit0 0x0024800C RmtBit1 0x00188018 RmtReset 0x80020C80
  {
    .T0H = 300,
    .T0L = 900,
    .T1H = 600,
    .T1L = 600,
    .Reset = 80000
  },
};

// enable AddLog
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};


TasmotaLED::TasmotaLED(uint16_t type, uint16_t num_leds) :
  _type(type),
  _timing((type >> 8) & 0xFF),
  _started(false),
  _dirty(true),
  _raw_format(false),
  _pixel_count(num_leds),
  _buf_work(nullptr),
  _buf_show(nullptr),
  _pusher(nullptr)
{
  _adjustSubType();   // compute values for _pixel_order, _w_before, _pixel_matrix
  if (_timing > (TasmotaLed_TimingEnd >> 8)) {
    _timing = 0;
  }
  switch (_type & 0x0F) {
    case TasmotaLed_4_WRGB:
      _pixel_size = 4;
      break;
    case TasmotaLed_1_Def:
    case TasmotaLed_3_RGB:
    default:  // fallback
      _pixel_size = 3;
      break;
  }

  _pixel_matrix = &TASMOTALED_CHANNEL_ORDERS[_pixel_order];

  _buf_work = new uint8_t[_pixel_count * _pixel_size];
  memset(_buf_work, 0, _pixel_count * _pixel_size);
  _buf_show = new uint8_t[_pixel_count * _pixel_size];
  memset(_buf_show, 0, _pixel_count * _pixel_size);
  // AddLog(LOG_LEVEL_DEBUG, "LED: type=0x%04X pixel_order=0x%02X _timing=%i ", _type, _pixel_order, _timing);
}

TasmotaLED::~TasmotaLED() {
  if (_pusher) {
    delete _pusher;
    _pusher = nullptr;
  }
  delete _buf_work;
  _buf_work = nullptr;
  delete _buf_show;
  _buf_show = nullptr;
}

// Adjust all internal parameters accouring to sub-type
void TasmotaLED::_adjustSubType(void) {
  _pixel_order = (_type >> 4) & 0x07;
  _pixel_matrix = &TASMOTALED_CHANNEL_ORDERS[_pixel_order];
  _w_before = _type & 0x08;
}

void TasmotaLED::SetPixelCount(uint16_t num_leds) {
  if (num_leds != _pixel_count) {
    _pixel_count = num_leds;
    delete _buf_work;
    _buf_work = new uint8_t[_pixel_count * _pixel_size];
    memset(_buf_work, 0, _pixel_count * _pixel_size);
    delete _buf_show;
    _buf_show = new uint8_t[_pixel_count * _pixel_size];
    memset(_buf_show, 0, _pixel_count * _pixel_size);
    if (_pusher) {
      _pusher->SetPixelCount(_pixel_count);
    }
  }
}

void TasmotaLED::SetPixelSubType(uint8_t subtype) {
  // subtype is only the 8 lower bits of _type
  _type = (_type & 0xFF00) | (subtype & 0xFF);
  _adjustSubType();
}


// Color is passed as 0xWWRRGGBB and copied as WWRRGGBB in _buf_work
void TasmotaLED::ClearTo(uint32_t wrgb, int32_t first, int32_t last) {
  // adjust first and last to be in range of 0 to _pixel_count-1
  if (first <0) { first += _pixel_count; }
  if (last <0) { last += _pixel_count; }
  if (first < 0) { first = 0; }
  if (last >= _pixel_count) { last = _pixel_count - 1; }
  if (first > last) { return; }
  // adjust to pixel format
  uint8_t b0 = (wrgb >> 24) & 0xFF;
  uint8_t b1 = (wrgb >> 16) & 0xFF;
  uint8_t b2 = (wrgb >>  8) & 0xFF;
  uint8_t b3 = (wrgb      ) & 0xFF;

  if ((b0 | b1 | b2 | b3) == 0) {
    // special version for clearing to black
    memset(_buf_work + first * _pixel_size, 0, (last - first + 1) * _pixel_size);
  } else {
    // fill sub-buffer with RRGGBB or WWRRGGBB (or raw)
    uint8_t *buf = _buf_work + first * _pixel_size;
    for (uint32_t i = first; i <= last; i++) {
      if (_pixel_size == 4) { *buf++ = b0;}
      *buf++ = b1;
      *buf++ = b2;
      *buf++ = b3;
    }
  }
}

void TasmotaLED::Show(void) {
  if (_pusher) {
    _dirty = false;                                               // we don't use the _dirty attribute and always show

    // copy the input buffer to the work buffer in format to be understood by LED strip
    if (_raw_format) {
      memmove(_buf_show, _buf_work, _pixel_count * _pixel_size);    // copy buffer in next buffer so we start with the current content
    } else {
      uint8_t *buf_from = _buf_work;
      uint8_t *buf_to = _buf_show;
      if (_pixel_size == 3) {
        // copying with swapping 512 pixels (1536 bytes) takes 124 microseconds to copy, so it's negligeable
        for (uint32_t i = 0; i < _pixel_count; i++) {
          buf_to[(*_pixel_matrix)[0]] = buf_from[0];   // R
          buf_to[(*_pixel_matrix)[1]] = buf_from[1];   // G
          buf_to[(*_pixel_matrix)[2]] = buf_from[2];   // B
          buf_to += 3;
          buf_from += 3;
        }
      } else if (_pixel_size == 4) {
        for (uint32_t i = 0; i < _pixel_count; i++) {
          if (_w_before) { *buf_to++ = buf_from[3]; }
          buf_to[(*_pixel_matrix)[0]] = buf_from[0];   // R
          buf_to[(*_pixel_matrix)[1]] = buf_from[1];   // G
          buf_to[(*_pixel_matrix)[2]] = buf_from[2];   // B
          if (!_w_before) { *buf_to++ = buf_from[3]; }
          buf_to += 3;    // one increment already happened
          buf_from += 4;
        }
      }
    }
    _pusher->Push(_buf_show);          // push to leds
  }
}

void TasmotaLED::SetPixelColor(int32_t index, uint32_t wrgb) {
  if (index < 0) { index += _pixel_count; }
  if ((index >= 0) && (index < _pixel_count)) {
    uint8_t *buf = _buf_work + index * _pixel_size;
    uint8_t b0 = (wrgb >> 24) & 0xFF;
    uint8_t b1 = (wrgb >> 16) & 0xFF;
    uint8_t b2 = (wrgb >>  8) & 0xFF;
    uint8_t b3 = (wrgb      ) & 0xFF;

    if (_pixel_size == 4) { *buf++ = b0;}
    *buf++ = b1;
    *buf++ = b2;
    *buf++ = b3;
    _dirty = true;
  }
}

uint32_t TasmotaLED::GetPixelColor(int32_t index) {
  if (index < 0) { index += _pixel_count; }
  if ((index >= 0) && (index < _pixel_count)) {
    uint8_t *buf = _buf_work + index * _pixel_size;
    uint32_t wrgb = 0;
    if (_pixel_size == 4) { wrgb = (*buf++) << 24; }
    wrgb |= (*buf++) << 16;
    wrgb |= (*buf++) <<  8;
    wrgb |= (*buf++);
    return wrgb;
  } else {
    return 0;
  }
}

void TasmotaLED::SetPusher(TasmotaLEDPusher *pusher) {
  if (_pusher) {
    delete _pusher;
  }
  _pusher = pusher;
  _started = false;
}

bool TasmotaLED::Begin(void) {
  if (_pusher) {
    if (_started) {
      return true;
    } else {
      const TasmotaLED_Timing * timing = &TasmotaLED_Timings[_timing];
      // AddLog(LOG_LEVEL_DEBUG, "LED: T0H=%i T0L=%i T1H=%i T1L=%i Reset=%i", timing.T0H, timing.T0L, timing.T1H, timing.T1L, timing.Reset);
      return _pusher->Begin(_pixel_count, _pixel_size, timing);
    }
  } else {
    return false;
  }
}

bool TasmotaLED::CanShow(void) const {
  if (_pusher) {
    return _pusher->CanShow();
  }
  return false;
}

#endif // ESP32
