/*
  xsns_18_pms5003.ino - PMS3003, PMS5003, PMS7003 particle concentration sensor support for Tasmota

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

#ifdef USE_PMS5003
/*********************************************************************************************\
 * PlanTower PMS3003, PMS5003, PMS7003 particle concentration sensor
 * For background information see http://aqicn.org/sensor/pms5003-7003/ or
 * http://aqicn.org/sensor/pms3003/
 *
 * Hardware Serial will be selected if GPIO3 = [PMS5003]
 * You can either support PMS3003 or PMS5003-7003 at one time. To enable the PMS3003 support
 * you must enable the define PMS_MODEL_PMS3003 on your configuration file.
\*********************************************************************************************/

#define XSNS_18             18

#include <TasmotaSerial.h>

#ifndef WARMUP_PERIOD
#define WARMUP_PERIOD 30          // Turn on PMSX003 XX-seconds before read in passive mode
#endif

#ifndef MIN_INTERVAL_PERIOD
#define MIN_INTERVAL_PERIOD 60    // minimum interval period in seconds required for passive mode
#endif
#define PMS3003FRAMELEN 20		//framelen
#define PMS5003FRAMELEN 28

TasmotaSerial *PmsSerial;

struct PMS5003 {
  uint16_t time = 0;
  uint8_t type = 1;
  uint8_t valid = 0;
  uint8_t wake_mode = 1;
  uint8_t ready = 1;
  bool discovery_triggered = false;
} Pms;

enum PmsCommands
{
  CMD_MODE_ACTIVE,
  CMD_SLEEP,
  CMD_WAKEUP,
  CMD_MODE_PASSIVE,
  CMD_READ_DATA
};

const uint8_t kPmsCommands[][7] PROGMEM = {
    //  0     1    2    3     4     5     6
    {0x42, 0x4D, 0xE1, 0x00, 0x01, 0x01, 0x71},  // pms_set_active_mode
    {0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x73},  // pms_sleep
    {0x42, 0x4D, 0xE4, 0x00, 0x01, 0x01, 0x74},  // pms_wake
    {0x42, 0x4D, 0xE1, 0x00, 0x00, 0x01, 0x70},  // pms_set_passive_mode
    {0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71}}; // pms_passive_mode_read

struct pmsX003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  //When PMS3003, the following line are reserved
  uint16_t particles_03um, particles_05um, particles_10um;
  //When PMS3003, the following two line are non-existing
  uint16_t particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  //Except for the checksum
  uint16_t checksum;
  uint8_t ok;
} pms_data;

/*********************************************************************************************/

size_t PmsSendCmd(uint8_t command_id)
{
  return PmsSerial->write(kPmsCommands[command_id], sizeof(kPmsCommands[command_id]));
}

/*********************************************************************************************/

void MarshallFirstRead(void)
{
	pms_data.framelen=ntohs(pms_data.framelen);
	pms_data.pm10_standard=ntohs(pms_data.pm10_standard);
	pms_data.pm25_standard=ntohs(pms_data.pm25_standard);
	pms_data.pm100_standard=ntohs(pms_data.pm100_standard);
	pms_data.pm10_env=ntohs(pms_data.pm10_env);
	pms_data.pm25_env=ntohs(pms_data.pm25_env);
	pms_data.pm100_env=ntohs(pms_data.pm100_env);
	pms_data.particles_03um=ntohs(pms_data.particles_03um);
	pms_data.particles_05um=ntohs(pms_data.particles_05um);
	pms_data.particles_10um=ntohs(pms_data.particles_10um);
	//The checksum for the pms3003 is stored in this section before being copied to checksum
	pms_data.particles_25um=ntohs(pms_data.particles_25um);
}
void MarshallSecondRead(void)
{
	pms_data.particles_50um=ntohs(pms_data.particles_50um);
	pms_data.particles_100um=ntohs(pms_data.particles_100um);
	pms_data.unused=ntohs(pms_data.unused);
	pms_data.checksum=ntohs(pms_data.checksum);
}
bool PmsReadData(void)
{
  if (! PmsSerial->available()) {
    return false;
  }
  while ((PmsSerial->peek() != 0x42) && PmsSerial->available()) {
    PmsSerial->read();
  }

  //4 : 2 byte header + 2 byte checksum
  if (PmsSerial->available() < PMS3003FRAMELEN+4) {
	  return false;
  }
  uint8_t header[2];
  PmsSerial->readBytes(header,2);
  if (header[0]!=0x42 or header[1]!=0x4d)
  {
	  return false;
  }
  pms_data.ok=0;
  PmsSerial->readBytes((uint8_t*)&pms_data,PMS3003FRAMELEN+2);
  MarshallFirstRead();
  if (pms_data.framelen ==PMS3003FRAMELEN) //PMS3003
  {
	  pms_data.checksum=pms_data.particles_25um;
	  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t*)&pms_data, PMS3003FRAMELEN+2);
  }else if(pms_data.framelen == PMS5003FRAMELEN) //PMS5003-PMS7003
  {
	  if (PmsSerial->available()<8)
	  {
		  //Not enought data
    AddLog(LOG_LEVEL_DEBUG, PSTR("PMS: Not enough data for %d bytes "),pms_data.framelen);
		  return false;
	  }else{
		  PmsSerial->readBytes(((uint8_t*)&pms_data)+PMS3003FRAMELEN+2,8);
		  MarshallSecondRead();
		  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t*)&pms_data, PMS5003FRAMELEN+2);
	  }
  }else{
    AddLog(LOG_LEVEL_DEBUG, PSTR("PMS: OUT bad framelen2  %d"),pms_data.framelen);
	  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t*)&pms_data, PMS5003FRAMELEN+2);
	  return false;
  }

  uint16_t sum = 0x42;
  sum+=0x4d;
  PmsSerial->flush();  // Make room for another burst
  uint8_t* pms_data_in_byte=(uint8_t*)&pms_data;
  //We check against PMS5003FRAMELEN to prevent any incorrect framelen
  for (uint16_t i=0; i<pms_data.framelen && pms_data.framelen<=PMS5003FRAMELEN;++i)
  {
	  sum+=pms_data_in_byte[i];
  }
//All tasmota platform are network endian (arm/cortex/extensa), so uint16_t comparison are OK
if (pms_data.checksum!=sum)
{
    AddLog(LOG_LEVEL_DEBUG, PSTR("PMS: checksum %d %d" ),pms_data.checksum,sum);
    AddLog(LOG_LEVEL_DEBUG, PSTR("PMS: " D_CHECKSUM_FAILURE));
    return false;
}
 
  Pms.valid = 10;
 pms_data.ok=1;
  if (!Pms.discovery_triggered) {
    TasmotaGlobal.discovery_counter = 1;      // Force discovery
    Pms.discovery_triggered = true;
  }

  return true;
}

/*********************************************************************************************\
 * Command Sensor18
 *
 * Warmup time for sensor is 30 seconds, therfore setting interval time to less than 60
 * seconds doesn't really make sense.
 *
 * 0 - 59   -   Active Mode (continuous sensor readings)
 * 60 .. 65535 -  Passive Mode (read sensor every x seconds)
\*********************************************************************************************/

bool PmsCommandSensor(void)
{
  if (PinUsed(GPIO_PMS5003_TX) && (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32001)) {
    if (XdrvMailbox.payload < MIN_INTERVAL_PERIOD) {
      // Set Active Mode if interval is less than 60 seconds
      Settings->pms_wake_interval = 0;
      Pms.wake_mode = 1;
      Pms.ready = 1;
      PmsSendCmd(CMD_MODE_ACTIVE);
      PmsSendCmd(CMD_WAKEUP);
    } else {
      // Set Passive Mode and schedule read once per interval time
      Settings->pms_wake_interval = XdrvMailbox.payload;
      PmsSendCmd(CMD_MODE_PASSIVE);
      PmsSendCmd(CMD_SLEEP);
      Pms.wake_mode = 0;
      Pms.ready = 0;
    }
  }

  Response_P(S_JSON_SENSOR_INDEX_NVALUE, XSNS_18, Settings->pms_wake_interval);

  return true;
}

/*********************************************************************************************/

void PmsSecond(void)                 // Every second
{
  if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
    // Passive Mode
    Pms.time++;
    if ((Settings->pms_wake_interval - Pms.time <= WARMUP_PERIOD) && !Pms.wake_mode) {
      // wakeup sensor WARMUP_PERIOD before read interval
      Pms.wake_mode = 1;
      PmsSendCmd(CMD_WAKEUP);
    }
    if (Pms.time >= Settings->pms_wake_interval) {
      // sensor is awake and warmed up, set up for reading
      PmsSendCmd(CMD_READ_DATA);
      Pms.ready = 1;
      Pms.time = 0;
    }
  }

  if (Pms.ready) {
    if (PmsReadData()) {
      Pms.valid = 10;
      if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
        PmsSendCmd(CMD_SLEEP);
        Pms.wake_mode = 0;
        Pms.ready = 0;
      }
    } else {
      if (Pms.valid) {
        Pms.valid--;
        if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
          PmsSendCmd(CMD_READ_DATA);
          Pms.ready = 1;
        }
      }
    }
  }
}

/*********************************************************************************************/

void PmsInit(void)
{
  Pms.type = 0;
  if (PinUsed(GPIO_PMS5003_RX)) {
    PmsSerial = new TasmotaSerial(Pin(GPIO_PMS5003_RX), (PinUsed(GPIO_PMS5003_TX)) ? Pin(GPIO_PMS5003_TX) : -1, 1);
    if (PmsSerial->begin(9600)) {
      if (PmsSerial->hardwareSerial()) { ClaimSerial(); }

      if (!PinUsed(GPIO_PMS5003_TX)) {  // setting interval not supported if TX pin not connected
        Settings->pms_wake_interval = 0;
        Pms.ready = 1;
      } else {
        if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
          // Passive Mode
          PmsSendCmd(CMD_MODE_PASSIVE);
          Pms.wake_mode = 0;
          Pms.ready = 0;
          Pms.time = Settings->pms_wake_interval - WARMUP_PERIOD; // Let it wake up in the next second
        }
      }

      Pms.type = 1;
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_PMSX003_SNS[] PROGMEM =
//  "{s}PMS3003 " D_STANDARD_CONCENTRATION " 1 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
//  "{s}PMS3003 " D_STANDARD_CONCENTRATION " 2.5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
//  "{s}PMS3003 " D_STANDARD_CONCENTRATION " 10 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
  "{s}PMSX003 " D_ENVIRONMENTAL_CONCENTRATION " 1 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
  "{s}PMSX003 " D_ENVIRONMENTAL_CONCENTRATION " 2.5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
  "{s}PMSX003 " D_ENVIRONMENTAL_CONCENTRATION " 10 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}";
const char HTTP_PMS5003_SNS[] PROGMEM =
//  "{s}PMS5003 " D_STANDARD_CONCENTRATION " 1 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
//  "{s}PMS5003 " D_STANDARD_CONCENTRATION " 2.5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
//  "{s}PMS5003 " D_STANDARD_CONCENTRATION " 10 " D_UNIT_MICROMETER "{m}%d " D_UNIT_MICROGRAM_PER_CUBIC_METER "{e}"
  "{s}PMS5003 " D_PARTICALS_BEYOND " 0.3 " D_UNIT_MICROMETER "{m}%d " D_UNIT_PARTS_PER_DECILITER "{e}"
  "{s}PMS5003 " D_PARTICALS_BEYOND " 0.5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_PARTS_PER_DECILITER "{e}"
  "{s}PMS5003 " D_PARTICALS_BEYOND " 1 " D_UNIT_MICROMETER "{m}%d " D_UNIT_PARTS_PER_DECILITER "{e}"
  "{s}PMS5003 " D_PARTICALS_BEYOND " 2.5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_PARTS_PER_DECILITER "{e}"
  "{s}PMS5003 " D_PARTICALS_BEYOND " 5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_PARTS_PER_DECILITER "{e}"
  "{s}PMS5003 " D_PARTICALS_BEYOND " 10 " D_UNIT_MICROMETER "{m}%d " D_UNIT_PARTS_PER_DECILITER "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void PmsShow(bool json)
{
  if (Pms.valid and pms_data.ok == 1) {
    if (json) {
		if (pms_data.framelen == PMS3003FRAMELEN )
		{

		  ResponseAppend_P(PSTR(",\"PMS3003\":{\"CF1\":%d,\"CF2.5\":%d,\"CF10\":%d,\"PM1\":%d,\"PM2.5\":%d,\"PM10\":%d}"),
			pms_data.pm10_standard, pms_data.pm25_standard, pms_data.pm100_standard,
			pms_data.pm10_env, pms_data.pm25_env, pms_data.pm100_env);
		}
		else{

		  ResponseAppend_P(PSTR(",\"PMS5003\":{\"CF1\":%d,\"CF2.5\":%d,\"CF10\":%d,\"PM1\":%d,\"PM2.5\":%d,\"PM10\":%d,\"PB0.3\":%d,\"PB0.5\":%d,\"PB1\":%d,\"PB2.5\":%d,\"PB5\":%d,\"PB10\":%d}"),
			pms_data.pm10_standard, pms_data.pm25_standard, pms_data.pm100_standard,
			pms_data.pm10_env, pms_data.pm25_env, pms_data.pm100_env,
			pms_data.particles_03um, pms_data.particles_05um, pms_data.particles_10um, pms_data.particles_25um, pms_data.particles_50um, pms_data.particles_100um);
		}
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_COUNT, pms_data.pm10_env);     // PM1
        DomoticzSensor(DZ_VOLTAGE, pms_data.pm25_env);   // PM2.5
        DomoticzSensor(DZ_CURRENT, pms_data.pm100_env);  // PM10
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
        WSContentSend_PD(HTTP_PMSX003_SNS,
        pms_data.pm10_env, pms_data.pm25_env, pms_data.pm100_env);
		if (pms_data.framelen == PMS5003FRAMELEN )
        WSContentSend_PD(HTTP_PMS5003_SNS,
        pms_data.particles_03um, pms_data.particles_05um, pms_data.particles_10um, pms_data.particles_25um, pms_data.particles_50um, pms_data.particles_100um);
		}

#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns18(uint8_t function)
{
  bool result = false;

  if (Pms.type) {
    switch (function) {
      case FUNC_INIT:
        PmsInit();
        break;
      case FUNC_EVERY_SECOND:
        PmsSecond();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_18 == XdrvMailbox.index) {
          result = PmsCommandSensor();
        }
        break;
      case FUNC_JSON_APPEND:
        PmsShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        PmsShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_PMS5003
