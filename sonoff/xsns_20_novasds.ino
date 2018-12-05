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
 * For protocol specification see 
 *   https://cdn.sparkfun.com/assets/parts/1/2/2/7/5/Laser_Dust_Sensor_Control_Protocol_V1.3.pdf
 *
 * Hardware Serial will be selected if GPIO3 = [SDS0X01]
\*********************************************************************************************/

#define XSNS_20             20

#include <TasmotaSerial.h>

#ifndef WORKING_PERIOD
#define WORKING_PERIOD                5       // NodaSDS sleep working period in minutes
#endif
#ifndef NOVA_SDS_REINIT_CHECK
#define NOVA_SDS_REINIT_CHECK         80      // NodaSDS reinitalized check in seconds
#endif
#ifndef NOVA_SDS_QUERY_INTERVAL
#define NOVA_SDS_QUERY_INTERVAL       3       // NodaSDS query interval in seconds
#endif
#ifndef NOVA_SDS_RECDATA_TIMEOUT
#define NOVA_SDS_RECDATA_TIMEOUT      150     // NodaSDS query data timeout in ms
#endif
#ifndef NOVA_SDS_DEVICE_ID
#define NOVA_SDS_DEVICE_ID            0xFFFF  // NodaSDS all sensor response
#endif


TasmotaSerial *NovaSdsSerial;

uint8_t novasds_type = 1;
uint8_t novasds_valid = 0;


struct sds011data {
  uint16_t pm100;
  uint16_t pm25;
} novasds_data;

// NovaSDS commands
#define NOVA_SDS_REPORTING_MODE       2       // Cmnd "data reporting mode"
#define NOVA_SDS_QUERY_DATA           4       // Cmnd "Query data"
#define NOVA_SDS_SET_DEVICE_ID        5       // Cmnd "Set Device ID"
#define NOVA_SDS_SLEEP_AND_WORK       6       // Cmnd "sleep and work mode"
#define NOVA_SDS_WORKING_PERIOD       8       // Cmnd "working period"
#define NOVA_SDS_CHECK_FIRMWARE_VER   7       // Cmnd "Check firmware version"
  #define NOVA_SDS_QUERY_MODE           0       // Subcmnd "query mode"
  #define NOVA_SDS_SET_MODE             1       // Subcmnd "set mode"
  #define NOVA_SDS_REPORT_ACTIVE        0       // Subcmnd "report active mode" - Sensor received query data command to report a measurement data
  #define NOVA_SDS_REPORT_QUERY         1       // Subcmnd "report query mode" - Sensor automatically reports a measurement data in a work period
  #define NOVA_SDS_WORK                 0       // Subcmnd "work mode"
  #define NOVA_SDS_SLEEP                1       // Subcmnd "sleep mode"


bool NovaSdsCommand(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint16_t sensorid, byte *buffer)
{
  uint8_t novasds_cmnd[19] = {0xAA, 0xB4, byte1, byte2, byte3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (uint8_t)(sensorid & 0xFF), (uint8_t)((sensorid>>8) & 0xFF), 0x00, 0xAB};

  // calc crc
  for (byte i = 2; i < 17; i++) {
    novasds_cmnd[17] += novasds_cmnd[i];
  }
  //~ snprintf_P(log_data, sizeof(log_data), PSTR("SDS: Send %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X"),
    //~ novasds_cmnd[0],novasds_cmnd[1],novasds_cmnd[2],novasds_cmnd[3],novasds_cmnd[4],novasds_cmnd[5],novasds_cmnd[6],novasds_cmnd[7],novasds_cmnd[8],novasds_cmnd[9],
    //~ novasds_cmnd[10],novasds_cmnd[11],novasds_cmnd[12],novasds_cmnd[13],novasds_cmnd[14],novasds_cmnd[15],novasds_cmnd[16],novasds_cmnd[17],novasds_cmnd[18]);
  //~ AddLog(LOG_LEVEL_DEBUG);
  // send cmnd
  NovaSdsSerial->write(novasds_cmnd, sizeof(novasds_cmnd));
  NovaSdsSerial->flush();

  // wait for any response
  unsigned long cmndtime = millis();
  while ( (TimePassedSince(cmndtime) < NOVA_SDS_RECDATA_TIMEOUT) && ( ! NovaSdsSerial->available() ) );
  if ( ! NovaSdsSerial->available() ) {
    // timeout
    return false;
  }
  byte recbuf[10];
  memset(recbuf, 0, sizeof(recbuf));
  // sync to 0xAA header
  while ( (TimePassedSince(cmndtime) < NOVA_SDS_RECDATA_TIMEOUT) && ( NovaSdsSerial->available() > 0) && (0xAA != (recbuf[0] = NovaSdsSerial->read())) );
  if ( 0xAA != recbuf[0] ) {
    // no head found
    return false;
  }

  // read rest (9 of 10 bytes) of message
  NovaSdsSerial->readBytes(&recbuf[1], 9);
  AddLogSerial(LOG_LEVEL_DEBUG_MORE, recbuf, sizeof(recbuf));

  if ( NULL != buffer ) {
    // return data to buffer
    memcpy(buffer, recbuf, sizeof(recbuf));
  }

  // checksum & tail check
  if ((0xAB != recbuf[9] ) || (recbuf[8] != ((recbuf[2] + recbuf[3] + recbuf[4] + recbuf[5] + recbuf[6] + recbuf[7]) & 0xFF))) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("SDS: " D_CHECKSUM_FAILURE));
    return false;
  }

  return true;
}

void NovaSdsSetWorkPeriod(void)
{
  // set sensor working period
  NovaSdsCommand(NOVA_SDS_WORKING_PERIOD, NOVA_SDS_SET_MODE, WORKING_PERIOD,        NOVA_SDS_DEVICE_ID, NULL);
  // set sensor report only on query
  NovaSdsCommand(NOVA_SDS_REPORTING_MODE, NOVA_SDS_SET_MODE, NOVA_SDS_REPORT_QUERY, NOVA_SDS_DEVICE_ID, NULL);
}

bool NovaSdsReadData(void)
{
  byte d[10];
  if ( ! NovaSdsCommand(NOVA_SDS_QUERY_DATA, 0, 0, NOVA_SDS_DEVICE_ID, d) ) {
    return false;
  }
  novasds_data.pm25 = (d[2] + 256 * d[3]);
  novasds_data.pm100 = (d[4] + 256 * d[5]);

  return true;
}

/*********************************************************************************************/

void NovaSdsSecond(void)                 // Every second
{
  if (0 == (uptime % NOVA_SDS_REINIT_CHECK)) {
    if (!novasds_valid) {
      NovaSdsSetWorkPeriod();
    }
  } else if (0 == (uptime % NOVA_SDS_QUERY_INTERVAL)) {
    if (NovaSdsReadData()) {
      novasds_valid = 10;
    } else {
      if (novasds_valid) {
        novasds_valid--;
      }
    }
  }
}

/*********************************************************************************************/

void NovaSdsInit(void)
{
  novasds_type = 0;
  if (pin[GPIO_SDS0X1_RX] < 99 && pin[GPIO_SDS0X1_TX] < 99) {
    NovaSdsSerial = new TasmotaSerial(pin[GPIO_SDS0X1_RX], pin[GPIO_SDS0X1_TX], 1);
    if (NovaSdsSerial->begin(9600)) {
      if (NovaSdsSerial->hardwareSerial()) {
        ClaimSerial();
      }
      novasds_type = 1;
      NovaSdsSetWorkPeriod();
    }
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
