/*
  xsns_20_novasds.ino - Nova SDS011/SDS021 particle concentration sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#ifdef USE_NOVA_SDS
/*********************************************************************************************\
 * Nova Fitness SDS011 (and possibly SDS021) particle concentration sensor
 * For background information see http://aqicn.org/sensor/sds011/
 *
 * Hardware Serial will be selected if GPIO3 = [SDS0X01]
\*********************************************************************************************/

#include <TasmotaSerial.h>

TasmotaSerial *NovaSdsSerial;

uint8_t novasds_type = 1;
uint8_t novasds_valid = 0;
uint8_t novasds_running = 1;
uint8_t novasds_read_tick = 30;
uint8_t novasds_wakup_tick = 179;
uint8_t novasds_ticker = 0;

struct sds011data {
  uint16_t pm100;
  uint16_t pm25;
} novasds_data;


bool NovaSdsReadData(bool publish)
{
  if (! NovaSdsSerial->available()) return false;

  while ((NovaSdsSerial->peek() != 0xAA) && NovaSdsSerial->available()) {
    NovaSdsSerial->read();
  }

  byte d[8] = { 0 };
  NovaSdsSerial->read();  // skip 0xAA
  NovaSdsSerial->readBytes(d, 8);
  NovaSdsSerial->flush();

  AddLogSerial(LOG_LEVEL_DEBUG_MORE, d, 8);
  if (!publish){
    return false;
  }
  if (d[7] == ((d[1] + d[2] + d[3] + d[4] + d[5] + d[6]) & 0xFF)) {
    novasds_data.pm25 = (d[1] + 256 * d[2]);
    novasds_data.pm100 = (d[3] + 256 * d[4]);
  } else {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("SDS: " D_CHECKSUM_FAILURE));
    return false;
  }
  return true;
}

/*********************************************************************************************/

void NovaSdsSecond()                 // Every second
{
  if (novasds_ticker < novasds_read_tick) {
    // wake up the sensor and wait read ticks to stabalize the sensor
    if (!novasds_running) {
      NovaSdsStart();
      novasds_running = 1;
    }

    // drain the serial without publishing data
    NovaSdsReadData(false);
    novasds_ticker++;

  } else if (novasds_ticker == novasds_read_tick) {

    // try to take a single stable reading and sleep the sensor
    if (NovaSdsReadData(true)) {
      novasds_valid = 1;
      NovaSdsStop();
      novasds_running = 0;
      novasds_ticker++;
    } else {
      novasds_valid = 0;
    }

  } else if (novasds_ticker >= novasds_wakup_tick) {
    // reset the counter
    novasds_ticker = 0;
  } else {
    // sensor is sleeping keep waiting
    novasds_ticker++;
  }
}

/*********************************************************************************************/

void NovaSdsInit()
{
  novasds_type = 0;
  if (pin[GPIO_SDS0X1_RX] < 99 && pin[GPIO_SDS0X1_TX] < 99) {
    NovaSdsSerial = new TasmotaSerial(pin[GPIO_SDS0X1_RX], pin[GPIO_SDS0X1_TX], 1);

    if (NovaSdsSerial->begin(9600)) {
      if (NovaSdsSerial->hardwareSerial()) {
        ClaimSerial();
      }
      novasds_type = 1;
    }

  }
}

void NovaSdsStart()
{
  AddLog_P(LOG_LEVEL_DEBUG, "SDS: start");
  const uint8_t novasds_start_cmd[] = {0xAA, 0xB4, 0x06, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x06, 0xAB};
  NovaSdsSerial->write(novasds_start_cmd, sizeof(novasds_start_cmd));
  NovaSdsSerial->flush();
}

void NovaSdsStop()
{
  AddLog_P(LOG_LEVEL_DEBUG, "SDS: stop");
  const uint8_t novasds_stop_cmd[] = {0xAA, 0xB4, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x05, 0xAB};
  NovaSdsSerial->write(novasds_stop_cmd, sizeof(novasds_stop_cmd));
  NovaSdsSerial->flush();
  // drain any old data
  while (NovaSdsSerial->available()) {
    NovaSdsSerial->read();
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SDS0X1_SNS[] PROGMEM = "%s"
  "{s}SDS0X1 " D_ENVIRONMENTAL_CONCENTRATION " 2.5 " D_UNIT_MICROMETER "{m}%s " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
  "{s}SDS0X1 " D_ENVIRONMENTAL_CONCENTRATION " 10 " D_UNIT_MICROMETER "{m}%s " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void NovaSdsShow(boolean json)
{
  if (novasds_valid) {
    char pm10[10];
    char pm2_5[10];
    float pm10f = (float)(novasds_data.pm100) / 10.0f;
    float pm2_5f = (float)(novasds_data.pm25) / 10.0f;
    dtostrfd(pm10f, 1, pm10);
    dtostrfd(pm2_5f, 1, pm2_5);
    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"SDS0X1\":{\"PM2.5\":%s,\"PM10\":%s}"), mqtt_data, pm2_5, pm10);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_VOLTAGE, pm2_5);  // PM2.5
        DomoticzSensor(DZ_CURRENT, pm10);   // PM10
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SDS0X1_SNS, mqtt_data, pm2_5, pm10);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_20

boolean Xsns20(byte function)
{
  boolean result = false;

  if (novasds_type) {
    switch (function) {
      case FUNC_INIT:
        NovaSdsInit();
        break;
      case FUNC_EVERY_SECOND:
        NovaSdsSecond();
        break;
      case FUNC_JSON_APPEND:
        NovaSdsShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        NovaSdsShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_NOVA_SDS
