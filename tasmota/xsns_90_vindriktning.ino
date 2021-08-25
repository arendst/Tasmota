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

#define VINDRIKTNING_DATASET_SIZE 20

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
  int serial_vindriktning_in_byte_counter = 0;
  uint8_t buffer[VINDRIKTNING_DATASET_SIZE];

  //AddLog(LOG_LEVEL_INFO, PSTR("VindriktningReadData"));
  //AddLog(LOG_LEVEL_INFO, PSTR("VindriktningReadData: Available data: %d"), VindriktningSerial->available());

  if (! VindriktningSerial->available()) {
    return false;
  }

  while (VindriktningSerial->available()) {
    uint8_t serial_in_byte = VindriktningSerial->read();
    if (serial_vindriktning_in_byte_counter <= VINDRIKTNING_DATASET_SIZE -1) {
	buffer[serial_vindriktning_in_byte_counter++] = serial_in_byte;
    }
    serial_bridge_polling_window = millis();
  }

  if (serial_vindriktning_in_byte_counter < VINDRIKTNING_DATASET_SIZE) {
    AddLog(LOG_LEVEL_INFO, PSTR("VindriktningReadData: Insufficient data set: length: %d < 20"), serial_vindriktning_in_byte_counter);
    return false;
  }

  vindriktning_data = (buffer[5] << 8) | buffer[6];
  VindriktningSerial->flush();  // Make room for another burst

  AddLog(LOG_LEVEL_INFO, PSTR("VindriktningReadData: PMS=%d (Data %02d: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x)"), vindriktning_data, serial_vindriktning_in_byte_counter, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11], buffer[12], buffer[13], buffer[14], buffer[15], buffer[16], buffer[17], buffer[18], buffer[19]);

  Vindriktning.valid = 10;

  return true;
}

/*********************************************************************************************/

void VindriktningSecond(void)                 // Every second
{
  //AddLog(LOG_LEVEL_INFO, PSTR("VindriktningSecond"));
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
  //AddLog(LOG_LEVEL_INFO, PSTR("VindriktningInit"));
  if (PinUsed(GPIO_VINDRIKTNING_RX)) {
    //AddLog(LOG_LEVEL_INFO, PSTR("VindriktningInit: PinUsed(GPIO_VINDRIKTNING_RX) TRUE"));
    VindriktningSerial = new TasmotaSerial(Pin(GPIO_VINDRIKTNING_RX), -1, 1);
    if (VindriktningSerial->begin(9600)) {
      //AddLog(LOG_LEVEL_INFO, PSTR("Serial initialization OK"));
      if (VindriktningSerial->hardwareSerial()) { ClaimSerial(); }
      Settings->pms_wake_interval = 0;
      Vindriktning.ready = 1;
      Vindriktning.type = 1;
    } else {
      AddLog(LOG_LEVEL_INFO, PSTR("Serial initialization Failed"));
    }
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
