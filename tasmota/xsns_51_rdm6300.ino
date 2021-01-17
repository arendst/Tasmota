/*
  xsns_51_rdm6300.ino - Support for RDM630(0) 125kHz NFC Tag Reader on Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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

#ifdef USE_RDM6300
/*********************************************************************************************\
 * Seeed studio Grove / RDM630 / RDM6300 125kHz rfid reader
 *
 * Expected 14 byte data:
 *  0  1  2  3  4  5  6  7  8  9 10 11 12 13
 * Hd ------ ASCII characters ----- Chksm Tl
 * 02 31 34 30 30 38 45 43 37 39 33 43 45 03 = 02-14-008EC793-CE-03
 *    Versn --------- Tag ---------
\*********************************************************************************************/

#define XSNS_51            51

#define RDM6300_BAUDRATE   9600
#define RDM_TIMEOUT        100
#define RDM6300_BLOCK      2 * 10   // 2 seconds block time

#include <TasmotaSerial.h>
TasmotaSerial *RDM6300Serial = nullptr;

struct {
  uint32_t uid = 0;
  uint8_t block_time = 0;
} Rdm;

/********************************************************************************************/

uint8_t RDM6300HexNibble(char chr) {
  uint8_t rVal = 0;
  if (isdigit(chr)) { rVal = chr - '0'; }
  else if (chr >= 'A' && chr <= 'F') { rVal = chr + 10 - 'A'; }
  else if (chr >= 'a' && chr <= 'f') { rVal = chr + 10 - 'a'; }
  return rVal;
}

// Convert hex string to int array
void RDM6300HexStringToArray(uint8_t array[], uint8_t len, char buffer[]) {
  char *cp = buffer;
  for (uint32_t i = 0; i < len; i++) {
    uint8_t val = RDM6300HexNibble(*cp++) << 4;
    array[i] = val | RDM6300HexNibble(*cp++);
  }
}

/********************************************************************************************/

void RDM6300Init() {
  if (PinUsed(GPIO_RDM6300_RX)) {
    RDM6300Serial = new TasmotaSerial(Pin(GPIO_RDM6300_RX), -1, 1);
    if (RDM6300Serial->begin(RDM6300_BAUDRATE)) {
      if (RDM6300Serial->hardwareSerial()) {
        ClaimSerial();
      }
    }
  }
}

void RDM6300ScanForTag() {
  if (!RDM6300Serial) { return; }

  if (Rdm.block_time > 0) {
    Rdm.block_time--;
    while (RDM6300Serial->available()) {
      RDM6300Serial->read();               // Flush serial buffer
    }
    return;
  }

  if (RDM6300Serial->available()) {

    char c = RDM6300Serial->read();
    if (c != 2) { return; }                // Head marker

    // read rest of message 11 more bytes
    char rdm_buffer[14];
    uint8_t rdm_index = 0;

    rdm_buffer[rdm_index++] = c;

    uint32_t cmillis = millis();
    while (1) {
      if (RDM6300Serial->available()) {
        c = RDM6300Serial->read();
        rdm_buffer[rdm_index++] = c;

        if (3 == c) { break; }             // Tail marker
        if (rdm_index > 14) { break; }     // Illegal message
      }
      if ((millis() - cmillis) > RDM_TIMEOUT) {
        return;                            // Timeout
      }
    }

    AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t*)rdm_buffer, sizeof(rdm_buffer));

    if (rdm_buffer[13] != 3) { return; }   // Tail marker

    Rdm.block_time = RDM6300_BLOCK;        // Block for 2 seconds

    uint8_t rdm_array[6];
    RDM6300HexStringToArray(rdm_array, sizeof(rdm_array), (char*)rdm_buffer +1);
    uint8_t accu = 0;
    for (uint32_t count = 0; count < 5; count++) {
      accu ^= rdm_array[count];            // Calc checksum,
    }
    if (accu != rdm_array[5]) { return; }  // Checksum error

    rdm_buffer[11] = '\0';
    uint32_t uid = strtoul(rdm_buffer +3, nullptr, 16);
    if (uid > 0) {                         // Ignore false positive all zeros
      Rdm.uid = uid;
      ResponseTime_P(PSTR(",\"RDM6300\":{\"UID\":\"%08X\"}}"), Rdm.uid);
      MqttPublishTeleSensor();
    }
  }
}

#ifdef USE_WEBSERVER
void RDM6300Show(void) {
  if (!RDM6300Serial) { return; }
  WSContentSend_PD(PSTR("{s}RDM6300 UID{m}%08X {e}"), Rdm.uid);
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns51(byte function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      RDM6300Init();
      break;
    case FUNC_EVERY_100_MSECOND:
      RDM6300ScanForTag();
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      RDM6300Show();
      break;
#endif  // USE_WEBSERVER
  }
  return result;
}

#endif  // USE_RDM6300
