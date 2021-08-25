/*
  xsns_90_vindriktning.ino - IKEA vindriktning particle concentration sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

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
 * IKEA VINDRIKTNING particle concentration sensor
\*********************************************************************************************/

#define XSNS_90             90

#include <TasmotaSerial.h>

#ifndef MIN_INTERVAL_PERIOD
#define MIN_INTERVAL_PERIOD 60    // minimum interval period in seconds required for passive mode
#endif

#define VINDRIKTNING_DATASET_SIZE 20

TasmotaSerial *VindriktningSerial;

uint16_t vindriktning_pm2_5 = 0;
uint16_t vindriktning_pm1_0 = 0;
uint16_t vindriktning_pm10 = 0;

struct VINDRIKTNING {
  uint8_t type = 1;
  uint8_t valid = 0;
} Vindriktning;


bool VindriktningReadData(void)
{
  int serial_vindriktning_in_byte_counter = 0;
  uint8_t buffer[VINDRIKTNING_DATASET_SIZE];
  uint8_t crc = 0;

  //AddLog(LOG_LEVEL_DEBUG, PSTR("VindriktningReadData"));
  //AddLog(LOG_LEVEL_DEBUG, PSTR("VindriktningReadData: Available data: %d"), VindriktningSerial->available());

  if (! VindriktningSerial->available()) {
    return false;
  }

  while (VindriktningSerial->available()) {
    uint8_t serial_in_byte = VindriktningSerial->read();
    if (serial_vindriktning_in_byte_counter <= VINDRIKTNING_DATASET_SIZE -1) {
	buffer[serial_vindriktning_in_byte_counter++] = serial_in_byte;
	crc += serial_in_byte;
    }
    serial_bridge_polling_window = millis();
  }

  if (serial_vindriktning_in_byte_counter < VINDRIKTNING_DATASET_SIZE) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("VindriktningReadData: Insufficient data set: length: %d < 20"), serial_vindriktning_in_byte_counter);
    return false;
  }

  if (crc != 0) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("VindriktningReadData: Incorrect checksum"));
    return false;
  }

  // sample data:
  // 16 11 0b 00 00 00 0c 00 00 03 cb 00 00 00 0c 01 00 00 00 e7
  //               |pm2_5|     |pm1_0|        |pm10 |     | CRC |
  vindriktning_pm2_5 = (buffer[5] << 8) | buffer[6];
  vindriktning_pm1_0 = (buffer[9] << 8) | buffer[10];
  vindriktning_pm10 = (buffer[13] << 8) | buffer[14];
  VindriktningSerial->flush();  // Make room for another burst

  AddLog(LOG_LEVEL_DEBUG, PSTR("VindriktningReadData: PMS=1.0: %d 2.5: %d, 10: %d (Data %02d (CRC: %02x): %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x)"), vindriktning_pm1_0, vindriktning_pm2_5, vindriktning_pm10, serial_vindriktning_in_byte_counter, crc, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11], buffer[12], buffer[13], buffer[14], buffer[15], buffer[16], buffer[17], buffer[18], buffer[19]);

  TasmotaGlobal.discovery_counter = 1;    // force TasDiscovery()
  return true;
}

/*********************************************************************************************/

void VindriktningSecond(void)                 // Every second
{
  if (VindriktningReadData()) {
    Vindriktning.valid = MIN_INTERVAL_PERIOD;
  } else {
    if (Vindriktning.valid) {
      Vindriktning.valid--;
    }
  }
}

/*********************************************************************************************/

void VindriktningInit(void)
{
  Vindriktning.type = 0;
  //AddLog(LOG_LEVEL_DEBUG, PSTR("VindriktningInit"));
  if (PinUsed(GPIO_VINDRIKTNING_RX)) {
    //AddLog(LOG_LEVEL_DEBUG, PSTR("VindriktningInit: PinUsed(GPIO_VINDRIKTNING_RX) TRUE"));
    VindriktningSerial = new TasmotaSerial(Pin(GPIO_VINDRIKTNING_RX), -1, 1);
    if (VindriktningSerial->begin(9600)) {
      //AddLog(LOG_LEVEL_DEBUG, PSTR("Serial initialization OK"));
      if (VindriktningSerial->hardwareSerial()) { ClaimSerial(); }
      Vindriktning.type = 1;
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("Serial initialization Failed"));
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_VINDRIKTNING_SNS[] PROGMEM =
  "{s}VINDRIKTNING " D_ENVIRONMENTAL_CONCENTRATION " 1.0 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
  "{s}VINDRIKTNING " D_ENVIRONMENTAL_CONCENTRATION " 2.5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
  "{s}VINDRIKTNING " D_ENVIRONMENTAL_CONCENTRATION " 10 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void VindriktningShow(bool json)
{
  //AddLog(LOG_LEVEL_DEBUG, PSTR("VindriktningShow"));
  if (PinUsed(GPIO_VINDRIKTNING_RX)) {
    if (Vindriktning.valid) {
      if (json) {
        ResponseAppend_P(PSTR(",\"VINDRIKTNING\":{\"PM1.0\":%d,\"PM2.5\":%d,\"PM10\":%d}"), vindriktning_pm1_0, vindriktning_pm2_5, vindriktning_pm10);
#ifdef USE_DOMOTICZ
        if (0 == TasmotaGlobal.tele_period) {
          DomoticzSensor(DZ_VOLTAGE, vindriktning_pm1_0);	// PM1.0
          DomoticzSensor(DZ_VOLTAGE, vindriktning_pm2_5);	// PM2.5
          DomoticzSensor(DZ_VOLTAGE, vindriktning_pm10);	// PM10
        }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
      } else {
          WSContentSend_PD(HTTP_VINDRIKTNING_SNS, vindriktning_pm1_0, vindriktning_pm2_5, vindriktning_pm10);
#endif  // USE_WEBSERVER
      }
    } else {
        ResponseAppend_P(PSTR(",\"VINDRIKTNING\":{\"PM1.0\":null,\"PM2.5\":null,\"PM10\":null}"));
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns90(uint8_t function)
{
  bool result = false;

  // AddLog(LOG_LEVEL_DEBUG, PSTR("Xsns90(%d)"), function);
  if (Vindriktning.type) {
    switch (function) {
      case FUNC_INIT:
        VindriktningInit();
        break;
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
    }
  }
  return result;
}

#endif  // USE_VINDRIKTNING
