/*
  xsns_126_aox3000z01.ino - AOX3000Z01 oxygen sensor support for Tasmota

  Copyright (C) 2026 by Sven Arke

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

  Based in part on xsns_102_ld2410s.ino by:
  SPDX-FileCopyrightText: 2022 Theo Arends, 2024 md5sum-as (https://github.com/md5sum-as)

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_AOX3000Z01
/*********************************************************************************************\
 * AOX3000Z01 - Oxygen sensor (UART streaming)
 *
 * Important: 
 * Uses HardwareSerial (Serial2) on ESP32 for stability
 * GPIO4 = [AOX Rx]
 *
 * JSON output: "AOX3000Z01":{"Oxygen":20.9,"Status":"Normal"}
 *
 * Protocol: 2400 baud, Frame: 0x78 + BC(len) + DATA + FCC(checksum)
 * Oxygen: DATA0..DATA1 as uint16 BE, unit = 0.1 %vol O2
 * Status: 0x01=Error, 0x02=Warmup, 0x03=Normal
\*********************************************************************************************/

#define XSNS_126 126

#ifdef ESP32
  #include <HardwareSerial.h>
  HardwareSerial *AoxSerial = nullptr;
#else
  #include <TasmotaSerial.h>
  TasmotaSerial *AoxSerial = nullptr;
#endif

float    Aox_oxygen = NAN;
uint8_t  Aox_status = 0;
uint32_t Aox_last_valid = 0;
bool     Aox_ready = false;
int8_t   Aox_rx_pin = -1;
uint32_t Aox_last_data = 0;
int      Aox_reinit_count = 0;

// Frame parser state
uint8_t  Aox_buf[32];
uint8_t  Aox_buf_idx = 0;
uint8_t  Aox_expected_len = 0;

void AoxLoop(void) {
  if (!Aox_ready || !AoxSerial) return;

  while (AoxSerial->available()) {
    uint8_t b = AoxSerial->read();
    Aox_last_data = millis();

    if (Aox_buf_idx == 0) {
      // Wait for header 0x78
      if (b == 0x78) {
        Aox_buf[Aox_buf_idx++] = b;
      }
      continue;
    }

    if (Aox_buf_idx == 1) {
      // BC (data length)
      if (b == 0 || b > 20) {
        Aox_buf_idx = 0;  // Invalid, reset
        continue;
      }
      Aox_buf[Aox_buf_idx++] = b;
      Aox_expected_len = 2 + b + 1;  // CC + BC + DATA + FCC
      continue;
    }

    // Collect remaining bytes
    Aox_buf[Aox_buf_idx++] = b;

    if (Aox_buf_idx >= Aox_expected_len) {
      // Frame complete - verify checksum
      uint16_t sum = 0;
      for (int i = 0; i < Aox_expected_len - 1; i++) sum += Aox_buf[i];
      uint8_t fcc_calc = (~sum + 1) & 0xFF;
      uint8_t fcc_got = Aox_buf[Aox_expected_len - 1];

      if (fcc_calc == fcc_got) {
        // Valid frame - parse
        uint8_t bc = Aox_buf[1];
        if (bc >= 2) {
          uint16_t raw_o2 = (Aox_buf[2] << 8) | Aox_buf[3];
          Aox_oxygen = raw_o2 / 10.0f;
        }
        if (bc >= 7) {
          Aox_status = Aox_buf[8];
        }
        Aox_last_valid = millis();
      }
      Aox_buf_idx = 0;  // Reset for next frame
    }
  }
  // Auto-reinit if 5s no data (AOX3000 serial hangs)
  if ((millis() - Aox_last_data > 5000) && (Aox_last_valid == 0)) {
    Aox_reinit_count++;
    AddLog(LOG_LEVEL_INFO, PSTR("AOX: No data - reinit #%d..."), Aox_reinit_count);
    Aox_ready = false;
    if (AoxSerial) {
      AoxSerial->end();
      delay(50);
      delete AoxSerial;
      AoxSerial = nullptr;
    }
    AoxInit();
    Aox_last_data = millis();
  }
}

void AoxEverySecond(void) {
  if (!Aox_ready) return;

  // Debug: show bytes available
  int avail = AoxSerial ? AoxSerial->available() : 0;
  AddLog(LOG_LEVEL_DEBUG, PSTR("AOX: avail=%d last=%dms"), avail, Aox_last_valid ? (millis() - Aox_last_valid) : -1);

  // Log current value once per second
  if (Aox_last_valid && (millis() - Aox_last_valid < 5000)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("AOX: O2=%.1f%% Status=%d"), Aox_oxygen, Aox_status);
  }

  // Invalidate if no valid data for 15 seconds
  if (Aox_last_valid && (millis() - Aox_last_valid > 15000)) {
    Aox_oxygen = NAN;
    Aox_status = 0;
    Aox_last_valid = 0;
    AddLog(LOG_LEVEL_DEBUG, PSTR("AOX: No data - invalidated"));
  }
}

void AoxInit(void) {
  if (!PinUsed(GPIO_AOX_RX)) return;

  Aox_rx_pin = Pin(GPIO_AOX_RX);

  #ifdef ESP32
    // Set pin HIGH immediately (UART idle state)
    pinMode(Aox_rx_pin, INPUT);
    if (AoxSerial) {
      AoxSerial->end();
      delay(50);
      delete AoxSerial;
      AoxSerial = nullptr;
    }
    // Use HardwareSerial (Serial2) on ESP32
    AoxSerial = new HardwareSerial(2);
  
    // Force GPIO mux to a known state (helps on ESP32 in some environments)
        pinMode(Aox_rx_pin, INPUT);
        delay(50);

    AoxSerial->setRxBufferSize(512);  // Set buffer BEFORE begin()
    AoxSerial->begin(2400, SERIAL_8N1, Aox_rx_pin, -1);  // RX only, no TX
    // Flush any garbage
    while(AoxSerial->available()) AoxSerial->read();
    Aox_ready = true;
    AddLog(LOG_LEVEL_INFO, PSTR("AOX: HardwareSerial2 on GPIO%d @ 2400 baud"), Aox_rx_pin);
  #else
  // ESP8266: use TasmotaSerial
  AoxSerial = new TasmotaSerial(Aox_rx_pin, -1, 1, 0, 256);
  if (AoxSerial && AoxSerial->begin(2400)) {
    if (AoxSerial->hardwareSerial()) { ClaimSerial(); }
    Aox_ready = true;
    AddLog(LOG_LEVEL_INFO, PSTR("AOX: TasmotaSerial on GPIO%d @ 2400 baud"), Aox_rx_pin);
  }
#endif
}

void AoxShow(bool json) {
  if (!Aox_ready) return;

  const char *status_str = (Aox_status == 0x01) ? "Error" :
                           (Aox_status == 0x02) ? "Warmup" :
                           (Aox_status == 0x03) ? "Normal" : "Unknown";

  if (json) {
    if (!isnan(Aox_oxygen)) {
      ResponseAppend_P(PSTR(",\"AOX3000Z01\":{\"Oxygen\":%1_f,\"Status\":\"%s\"}"), &Aox_oxygen, status_str);
    } else {
      ResponseAppend_P(PSTR(",\"AOX3000Z01\":{\"Oxygen\":null,\"Status\":\"%s\"}"), status_str);
    }
#ifdef USE_WEBSERVER
  } else {
    if (!isnan(Aox_oxygen)) {
      WSContentSend_PD(PSTR("{s}AOX3000Z01 O2{m}%1_f " D_UNIT_PERCENT "{e}"), &Aox_oxygen);
      WSContentSend_PD(PSTR("{s}AOX3000Z01 Status{m}%s{e}"), status_str);
    }
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns126(uint32_t function) {
  bool result = false;
  switch (function) {
    case FUNC_INIT:
      AoxInit();
      break;
    case FUNC_LOOP:
      AoxLoop();
      break;
    case FUNC_EVERY_SECOND:
      AoxEverySecond();
      break;
    case FUNC_JSON_APPEND:
      AoxShow(true);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      AoxShow(false);
      break;
#endif
  }
  return result;
}

#endif  // USE_AOX3000Z01
