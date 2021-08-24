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

#ifndef WARMUP_PERIOD
#define WARMUP_PERIOD 30          // Turn on PMSX003 XX-seconds before read in passive mode
#endif

#ifndef MIN_INTERVAL_PERIOD
#define MIN_INTERVAL_PERIOD 60    // minimum interval period in seconds required for passive mode
#endif

TasmotaSerial *VindriktningSerial;

uint16_t vindriktning_data;

struct VINDRIKTNING {
  uint16_t time = 0;
  uint8_t type = 1;
  uint8_t valid = 0;
  uint8_t wake_mode = 1;
  uint8_t ready = 1;
} Vindriktning;


bool VindriktningReadData(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("VindriktningReadData"));
  if (! VindriktningSerial->available()) {
    return false;
  }
  while (VindriktningSerial->available()) {
    VindriktningSerial->read();
    delay(15);
  }

  if (VindriktningSerial->available() < 6) {		// FIXME: 6 real data length?
    return false;
  }

  uint8_t buffer[6];
  VindriktningSerial->readBytes(buffer, 6);
  vindriktning_data = (buffer[5] << 8) | buffer[6];
  VindriktningSerial->flush();  // Make room for another burst

  AddLog(LOG_LEVEL_INFO, PSTR("PMS: %d"), vindriktning_data);

  Vindriktning.valid = 10;

  return true;
}

bool VindriktningCommandSensor(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("VindriktningCommandSensor"));
  // Set Passive Mode and schedule read once per interval time
  Settings->pms_wake_interval = XdrvMailbox.payload;
  //PmsSendCmd(CMD_MODE_PASSIVE);
  //PmsSendCmd(CMD_SLEEP);
  Vindriktning.wake_mode = 0;
  Vindriktning.ready = 0;

  Response_P(S_JSON_SENSOR_INDEX_NVALUE, XSNS_18, Settings->pms_wake_interval);

  return true;
}

/*********************************************************************************************/

void VindriktningSecond(void)                 // Every second
{
  AddLog(LOG_LEVEL_INFO, PSTR("VindriktningSecond"));
  if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
    // Passive Mode
    Vindriktning.time++;
    if ((Settings->pms_wake_interval - Vindriktning.time <= WARMUP_PERIOD) && !Vindriktning.wake_mode) {
      // wakeup sensor WARMUP_PERIOD before read interval
      Vindriktning.wake_mode = 1;
    }
    if (Vindriktning.time >= Settings->pms_wake_interval) {
      // sensor is awake and warmed up, set up for reading
      // FIXME VindriktningSendCmd(CMD_READ_DATA);
      Vindriktning.ready = 1;
      Vindriktning.time = 0;
    }
  }

  if (Vindriktning.ready) {
    if (VindriktningReadData()) {
      Vindriktning.valid = 10;
      if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
        // FIXME VindriktningSendCmd(CMD_SLEEP);
        Vindriktning.wake_mode = 0;
        Vindriktning.ready = 0;
      }
    } else {
      if (Vindriktning.valid) {
        Vindriktning.valid--;
        if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
          // FIXME VindriktningSendCmd(CMD_READ_DATA);
          Vindriktning.ready = 1;
        }
      }
    }
  }
}

/*********************************************************************************************/

void VindriktningInit(void)
{
  Vindriktning.type = 0;
  AddLog(LOG_LEVEL_INFO, PSTR("VindriktningInit"));
  if (PinUsed(GPIO_VINDRIKTNING_RX)) {
    AddLog(LOG_LEVEL_INFO, PSTR("VindriktningInit: PinUsed(GPIO_VINDRIKTNING_RX) TRUE"));
    VindriktningSerial = new TasmotaSerial(Pin(GPIO_VINDRIKTNING_RX), -1, 1);
    if (VindriktningSerial->begin(9600)) {
      AddLog(LOG_LEVEL_INFO, PSTR("Serial initialization OK"));
      if (VindriktningSerial->hardwareSerial()) { ClaimSerial(); }
      Settings->pms_wake_interval = 0;
      Vindriktning.ready = 1;
      // Vindriktning.type = 1;
    } else {
      AddLog(LOG_LEVEL_INFO, PSTR("Serial initialization Failed"));
    }
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR("VindriktningInit: PinUsed(GPIO_VINDRIKTNING_RX) FALSE"));
  }
}

#ifdef USE_WEBSERVER
const char HTTP_VINDRIKTNING_SNS[] PROGMEM =
  "{s}VINDRIKTNING " D_PARTICALS_BEYOND " 2.5 " D_UNIT_MICROMETER "{m}%d " D_UNIT_PARTS_PER_DECILITER "{e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void VindriktningShow(bool json)
{
  AddLog(LOG_LEVEL_INFO, PSTR("VindriktningShow"));
  if (Vindriktning.valid) {
    if (json) {
      ResponseAppend_P(PSTR(",\"VINDRIKTNING\":{\"CF1\":%d}"), vindriktning_data);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_VOLTAGE, vindriktning_data);	// PM2.5
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
        WSContentSend_PD(HTTP_VINDRIKTNING_SNS, vindriktning_data);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns90(uint8_t function)
{
  bool result = false;

  // AddLog(LOG_LEVEL_INFO, PSTR("Xsns90(%d)"), function);

  //  Xsns90(5)
  //  Xsns90(6)
  //  Xsns90(7)
  //  Xsns90(9)
  //  Xsns90(10)
  //  Xsns90(11)

  // 0 FUNC_SETTINGS_OVERRIDE
  // 1 FUNC_PIN_STATE
  // 2 FUNC_MODULE_INIT
  // 3 FUNC_PRE_INIT
  // 4 FUNC_INIT
  // 5 FUNC_LOOP
  // 6 FUNC_EVERY_50_MSECOND
  // 7 FUNC_EVERY_100_MSECOND
  // 8 FUNC_EVERY_200_MSECOND
  // 9 FUNC_EVERY_250_MSECOND
  // 10 FUNC_EVERY_SECOND,
  // 11 FUNC_SAVE_SETTINGS
  // 12 FUNC_SAVE_AT_MIDNIGHT
  // 13 FUNC_SAVE_BEFORE_RESTART,
  if (Vindriktning.type) {
    switch (function) {
      case FUNC_INIT:
        VindriktningInit();
        break;
      case FUNC_EVERY_SECOND:
        VindriktningSecond();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_90 == XdrvMailbox.index) {
          result = VindriktningCommandSensor();
        }
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
