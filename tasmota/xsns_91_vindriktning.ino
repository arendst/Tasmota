/*
  xsns_91_vindriktning.ino - IKEA vindriktning particle concentration sensor support for Tasmota

  Copyright (C) 2021  Marcel Ritter and Theo Arends

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

#ifdef USE_VINDRIKTNING
/*********************************************************************************************\
 * IKEA VINDRIKTNING PM2.5 particle concentration sensor
 *
 * This sensor uses a subset of the PM1006K LED particle sensor
 * To use Tasmota the user needs to add an ESP8266 or ESP32
\*********************************************************************************************/

#define XSNS_91                   91

//#define XSNS_91_XTRA_INFO              // Display unverified data for PM1.0 and PM10

#include <TasmotaSerial.h>

#define VINDRIKTNING_DATASET_SIZE 20

TasmotaSerial *VindriktningSerial;

struct VINDRIKTNING {
  uint16_t pm2_5 = 0;
#ifdef XSNS_91_XTRA_INFO
  uint16_t pm1_0 = 0;
  uint16_t pm10 = 0;
#endif  // XSNS_91_XTRA_INFO
  uint8_t type = 1;
  uint8_t valid = 0;
  bool discovery_triggered = false;
} Vindriktning;

bool VindriktningReadData(void) {
  if (!VindriktningSerial->available()) {
    return false;
  }

  uint8_t buffer[VINDRIKTNING_DATASET_SIZE];
  VindriktningSerial->readBytes(buffer, VINDRIKTNING_DATASET_SIZE);
  VindriktningSerial->flush();  // Make room for another burst

  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, VINDRIKTNING_DATASET_SIZE);

  uint8_t crc = 0;
  for (uint32_t i = 0; i < VINDRIKTNING_DATASET_SIZE; i++) {
    crc += buffer[i];
  }
  if (crc != 0) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("VDN: " D_CHECKSUM_FAILURE));
    return false;
  }

  // sample data:
  // 16 11 0b 00 00 00 0c 00 00 03 cb 00 00 00 0c 01 00 00 00 e7
  //               |pm2_5|     |pm1_0|     |pm10 |        | CRC |
  Vindriktning.pm2_5 = (buffer[5] << 8) | buffer[6];
#ifdef XSNS_91_XTRA_INFO
  Vindriktning.pm1_0 = (buffer[9] << 8) | buffer[10];
  Vindriktning.pm10 = (buffer[13] << 8) | buffer[14];
#endif  // XSNS_91_XTRA_INFO

  if (!Vindriktning.discovery_triggered) {
    TasmotaGlobal.discovery_counter = 1;      // force TasDiscovery()
    Vindriktning.discovery_triggered = true;
  }
  return true;
}

/*********************************************************************************************/

void VindriktningSecond(void) {                // Every second
  if (VindriktningReadData()) {
    Vindriktning.valid = 60;
  } else {
    if (Vindriktning.valid) {
      Vindriktning.valid--;
    }
  }
}

/*********************************************************************************************/

void VindriktningInit(void) {
  Vindriktning.type = 0;
  if (PinUsed(GPIO_VINDRIKTNING_RX)) {
    VindriktningSerial = new TasmotaSerial(Pin(GPIO_VINDRIKTNING_RX), -1, 1);
    if (VindriktningSerial->begin(9600)) {
      if (VindriktningSerial->hardwareSerial()) { ClaimSerial(); }
      Vindriktning.type = 1;
    }
  }
}

#ifdef XSNS_91_XTRA_INFO

#ifdef USE_WEBSERVER
const char HTTP_VINDRIKTNING_SNS[] PROGMEM =
  "{s}VINDRIKTNING " D_ENVIRONMENTAL_CONCENTRATION " 1.0 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
  "{s}VINDRIKTNING " D_ENVIRONMENTAL_CONCENTRATION " 2.5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
  "{s}VINDRIKTNING " D_ENVIRONMENTAL_CONCENTRATION " 10 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void VindriktningShow(bool json) {
  if (Vindriktning.valid) {
    if (json) {
      ResponseAppend_P(PSTR(",\"VINDRIKTNING\":{\"PM1\":%d,\"PM2.5\":%d,\"PM10\":%d}"), Vindriktning.pm1_0, Vindriktning.pm2_5, Vindriktning.pm10);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_COUNT, Vindriktning.pm1_0);	   // PM1.0
        DomoticzSensor(DZ_VOLTAGE, Vindriktning.pm2_5);	 // PM2.5
        DomoticzSensor(DZ_CURRENT, Vindriktning.pm10);	 // PM10
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
        WSContentSend_PD(HTTP_VINDRIKTNING_SNS, Vindriktning.pm1_0, Vindriktning.pm2_5, Vindriktning.pm10);
#endif  // USE_WEBSERVER
    }
  }
}

#else  // No XSNS_91_XTRA_INFO

#ifdef USE_WEBSERVER
const char HTTP_VINDRIKTNING_SNS[] PROGMEM =
  "{s}VINDRIKTNING " D_ENVIRONMENTAL_CONCENTRATION " 2.5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}";
#endif  // USE_WEBSERVER

void VindriktningShow(bool json) {
  if (Vindriktning.valid) {
    if (json) {
      ResponseAppend_P(PSTR(",\"VINDRIKTNING\":{\"PM2.5\":%d}"), Vindriktning.pm2_5);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_VOLTAGE, Vindriktning.pm2_5);	 // PM2.5
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
        WSContentSend_PD(HTTP_VINDRIKTNING_SNS, Vindriktning.pm2_5);
#endif  // USE_WEBSERVER
    }
  }
}

#endif  // XSNS_91_XTRA_INFO

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns91(uint8_t function) {
  bool result = false;

  if (Vindriktning.type) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        VindriktningSecond();
        break;
      case FUNC_JSON_APPEND:
        VindriktningShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
	      VindriktningShow(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_INIT:
        VindriktningInit();
        break;
    }
  }
  return result;
}

#endif  // USE_VINDRIKTNING