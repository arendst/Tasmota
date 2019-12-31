/*
  xsns_15_mhz19.ino - MH-Z19(B) CO2 sensor support for Tasmota

  Copyright (C) 2020  Theo Arends

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

#ifdef USE_MHZ19
/*********************************************************************************************\
 * MH-Z19 - CO2 sensor
 *
 * Adapted from EspEasy plugin P049 by Dmitry (rel22 ___ inbox.ru)
 *
 * Hardware Serial will be selected if GPIO1 = [MHZ Rx] and GPIO3 = [MHZ Tx]
 **********************************************************************************************
 * Filter usage
 *
 * Select filter usage on low stability readings
\*********************************************************************************************/

#define XSNS_15                      15

enum MhzFilterOptions {MHZ19_FILTER_OFF, MHZ19_FILTER_OFF_ALLSAMPLES, MHZ19_FILTER_FAST, MHZ19_FILTER_MEDIUM, MHZ19_FILTER_SLOW};

#define MHZ19_FILTER_OPTION          MHZ19_FILTER_FAST

/*********************************************************************************************\
 * Source: http://www.winsen-sensor.com/d/files/infrared-gas-sensor/mh-z19b-co2-ver1_0.pdf
 *
 * Automatic Baseline Correction (ABC logic function) is enabled by default but may be disabled with command
 * Sensor15 0
 * and enabled again with command
 * Sensor15 1
 *
 * ABC logic function refers to that sensor itself do zero point judgment and automatic calibration procedure
 * intelligently after a continuous operation period. The automatic calibration cycle is every 24 hours after powered on.
 *
 * The zero point of automatic calibration is 400ppm.
 *
 * This function is usually suitable for indoor air quality monitor such as offices, schools and homes,
 * not suitable for greenhouse, farm and refrigeratory where this function should be off.
 *
 * Please do zero calibration timely, such as manual or commend calibration.
\*********************************************************************************************/

#include <TasmotaSerial.h>

#ifndef CO2_LOW
#define CO2_LOW                      800     // Below this CO2 value show green light
#endif
#ifndef CO2_HIGH
#define CO2_HIGH                     1200    // Above this CO2 value show red light
#endif

#define MHZ19_READ_TIMEOUT           400     // Must be way less than 1000 but enough to read 9 bytes at 9600 bps
#define MHZ19_RETRY_COUNT            8

TasmotaSerial *MhzSerial;

const char kMhzModels[] PROGMEM = "|B";

const char ABC_ENABLED[] = "ABC is Enabled";
const char ABC_DISABLED[] = "ABC is Disabled";

enum MhzCommands { MHZ_CMND_READPPM, MHZ_CMND_ABCENABLE, MHZ_CMND_ABCDISABLE, MHZ_CMND_ZEROPOINT, MHZ_CMND_RESET, MHZ_CMND_RANGE_1000, MHZ_CMND_RANGE_2000, MHZ_CMND_RANGE_3000, MHZ_CMND_RANGE_5000 };
const uint8_t kMhzCommands[][4] PROGMEM = {
//  2     3    6    7
  {0x86,0x00,0x00,0x00},   // mhz_cmnd_read_ppm
  {0x79,0xA0,0x00,0x00},   // mhz_cmnd_abc_enable
  {0x79,0x00,0x00,0x00},   // mhz_cmnd_abc_disable
  {0x87,0x00,0x00,0x00},   // mhz_cmnd_zeropoint
  {0x8D,0x00,0x00,0x00},   // mhz_cmnd_reset
  {0x99,0x00,0x03,0xE8},   // mhz_cmnd_set_range_1000
  {0x99,0x00,0x07,0xD0},   // mhz_cmnd_set_range_2000
  {0x99,0x00,0x0B,0xB8},   // mhz_cmnd_set_range_3000
  {0x99,0x00,0x13,0x88}};  // mhz_cmnd_set_range_5000

uint8_t mhz_type = 1;
uint16_t mhz_last_ppm = 0;
uint8_t mhz_filter = MHZ19_FILTER_OPTION;
bool mhz_abc_must_apply = false;

float mhz_temperature = 0;
uint8_t mhz_retry = MHZ19_RETRY_COUNT;
uint8_t mhz_received = 0;
uint8_t mhz_state = 0;

/*********************************************************************************************/

uint8_t MhzCalculateChecksum(uint8_t *array)
{
  uint8_t checksum = 0;
  for (uint32_t i = 1; i < 8; i++) {
    checksum += array[i];
  }
  checksum = 255 - checksum;
  return (checksum +1);
}

size_t MhzSendCmd(uint8_t command_id)
{
  uint8_t mhz_send[9] = { 0 };

  mhz_send[0] = 0xFF;  // Start byte, fixed
  mhz_send[1] = 0x01;  // Sensor number, 0x01 by default
  memcpy_P(&mhz_send[2], kMhzCommands[command_id], sizeof(uint16_t));
/*
  mhz_send[4] = 0x00;
  mhz_send[5] = 0x00;
*/
  memcpy_P(&mhz_send[6], kMhzCommands[command_id] + sizeof(uint16_t), sizeof(uint16_t));
  mhz_send[8] = MhzCalculateChecksum(mhz_send);

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Final MhzCommand: %x %x %x %x %x %x %x %x %x"),mhz_send[0],mhz_send[1],mhz_send[2],mhz_send[3],mhz_send[4],mhz_send[5],mhz_send[6],mhz_send[7],mhz_send[8]);

  return MhzSerial->write(mhz_send, sizeof(mhz_send));
}

/*********************************************************************************************/

bool MhzCheckAndApplyFilter(uint16_t ppm, uint8_t s)
{
  if (1 == s) {
    return false;            // S==1 => "A" version sensor bootup, do not use values.
  }
  if (mhz_last_ppm < 400 || mhz_last_ppm > 5000) {
    // Prevent unrealistic values during start-up with filtering enabled.
    // Just assume the entered value is correct.
    mhz_last_ppm = ppm;
    return true;
  }
  int32_t difference = ppm - mhz_last_ppm;
  if (s > 0 && s < 64 && mhz_filter != MHZ19_FILTER_OFF) {
    // Not the "B" version of the sensor, S value is used.
    // S==0 => "B" version, else "A" version
    // The S value is an indication of the stability of the reading.
    // S == 64 represents a stable reading and any lower value indicates (unusual) fast change.
    // Now we increase the delay filter for low values of S and increase response time when the
    // value is more stable.
    // This will make the reading useful in more turbulent environments,
    // where the sensor would report more rapid change of measured values.
    difference *= s;
    difference /= 64;
  }
  if (MHZ19_FILTER_OFF == mhz_filter) {
    if (s != 0 && s != 64) {
      return false;
    }
  } else {
    difference >>= (mhz_filter -1);
  }
  mhz_last_ppm = static_cast<uint16_t>(mhz_last_ppm + difference);
  return true;
}

void MhzEverySecond(void)
{
  mhz_state++;
  if (8 == mhz_state) {                   // Every 8 sec start a MH-Z19 measuring cycle (which takes 1005 +5% ms)
    mhz_state = 0;

    if (mhz_retry) {
      mhz_retry--;
      if (!mhz_retry) {
        mhz_last_ppm = 0;
        mhz_temperature = 0;
      }
    }

    MhzSerial->flush();                    // Sync reception
    MhzSendCmd(MHZ_CMND_READPPM);
    mhz_received = 0;
  }

  if ((mhz_state > 2) && !mhz_received) {  // Start reading response after 3 seconds every second until received
    uint8_t mhz_response[9];

    unsigned long start = millis();
    uint8_t counter = 0;
    while (((millis() - start) < MHZ19_READ_TIMEOUT) && (counter < 9)) {
      if (MhzSerial->available() > 0) {
        mhz_response[counter++] = MhzSerial->read();
      } else {
        delay(5);
      }
    }

    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, mhz_response, counter);

    if (counter < 9) {
//      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "MH-Z19 comms timeout"));
      return;
    }

    uint8_t crc = MhzCalculateChecksum(mhz_response);
    if (mhz_response[8] != crc) {
//      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "MH-Z19 crc error"));
      return;
    }
    if (0xFF != mhz_response[0] || 0x86 != mhz_response[1]) {
//      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "MH-Z19 bad response"));
      return;
    }

    mhz_received = 1;

    uint16_t u = (mhz_response[6] << 8) | mhz_response[7];
    if (15000 == u) {      // During (and only ever at) sensor boot, 'u' is reported as 15000
      if (Settings.SensorBits1.mhz19b_abc_disable) {
        // After bootup of the sensor the ABC will be enabled.
        // Thus only actively disable after bootup.
        mhz_abc_must_apply = true;
      }
    } else {
      uint16_t ppm = (mhz_response[2] << 8) | mhz_response[3];
      mhz_temperature = ConvertTemp((float)mhz_response[4] - 40);
      uint8_t s = mhz_response[5];
      mhz_type = (s) ? 1 : 2;
      if (MhzCheckAndApplyFilter(ppm, s)) {
        mhz_retry = MHZ19_RETRY_COUNT;
        LightSetSignal(CO2_LOW, CO2_HIGH, mhz_last_ppm);

        if (0 == s || 64 == s) {  // Reading is stable.
          if (mhz_abc_must_apply) {
            mhz_abc_must_apply = false;
            if (!Settings.SensorBits1.mhz19b_abc_disable) {
              MhzSendCmd(MHZ_CMND_ABCENABLE);
            } else {
              MhzSendCmd(MHZ_CMND_ABCDISABLE);
            }
          }
        }

      }
    }

  }
}

/*********************************************************************************************\
 * Command Sensor15
 *
 * 0    - ABC Off
 * 1    - ABC On (Default)
 * 2    - Manual start = ABC Off
 * 3    - (Not implemented) Optional filter settings
 * 9    - Reset
 * 1000 - Range
 * 2000 - Range
 * 3000 - Range
 * 5000 - Range
\*********************************************************************************************/

#define D_JSON_RANGE_1000 "1000 ppm range"
#define D_JSON_RANGE_2000 "2000 ppm range"
#define D_JSON_RANGE_3000 "3000 ppm range"
#define D_JSON_RANGE_5000 "5000 ppm range"

bool MhzCommandSensor(void)
{
  bool serviced = true;

  switch (XdrvMailbox.payload) {
    case 0:
      Settings.SensorBits1.mhz19b_abc_disable = true;
      MhzSendCmd(MHZ_CMND_ABCDISABLE);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_15, ABC_DISABLED);
      break;
    case 1:
      Settings.SensorBits1.mhz19b_abc_disable = false;
      MhzSendCmd(MHZ_CMND_ABCENABLE);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_15, ABC_ENABLED);
      break;
    case 2:
      MhzSendCmd(MHZ_CMND_ZEROPOINT);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_15, D_JSON_ZERO_POINT_CALIBRATION);
      break;
    case 9:
      MhzSendCmd(MHZ_CMND_RESET);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_15, D_JSON_RESET);
      break;
    case 1000:
      MhzSendCmd(MHZ_CMND_RANGE_1000);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_15, D_JSON_RANGE_1000);
      break;
    case 2000:
      MhzSendCmd(MHZ_CMND_RANGE_2000);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_15, D_JSON_RANGE_2000);
      break;
    case 3000:
      MhzSendCmd(MHZ_CMND_RANGE_3000);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_15, D_JSON_RANGE_3000);
      break;
    case 5000:
      MhzSendCmd(MHZ_CMND_RANGE_5000);
      Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_15, D_JSON_RANGE_5000);
      break;
    default:
      if (!Settings.SensorBits1.mhz19b_abc_disable) {
        Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_15, ABC_ENABLED);
      } else {
        Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_15, ABC_DISABLED);
      }
  }

  return serviced;
}

/*********************************************************************************************/

void MhzInit(void)
{
  mhz_type = 0;
  if ((pin[GPIO_MHZ_RXD] < 99) && (pin[GPIO_MHZ_TXD] < 99)) {
    MhzSerial = new TasmotaSerial(pin[GPIO_MHZ_RXD], pin[GPIO_MHZ_TXD], 1);
    if (MhzSerial->begin(9600)) {
      if (MhzSerial->hardwareSerial()) { ClaimSerial(); }
      mhz_type = 1;
    }

  }
}

void MhzShow(bool json)
{
  char types[7] = "MHZ19B";  // MHZ19B for legacy reasons. Prefered is MHZ19
  char temperature[33];
  dtostrfd(mhz_temperature, Settings.flag2.temperature_resolution, temperature);
  char model[3];
  GetTextIndexed(model, sizeof(model), mhz_type -1, kMhzModels);

  if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_MODEL "\":\"%s\",\"" D_JSON_CO2 "\":%d,\"" D_JSON_TEMPERATURE "\":%s}"), types, model, mhz_last_ppm, temperature);
#ifdef USE_DOMOTICZ
    if (0 == tele_period) {
      DomoticzSensor(DZ_AIRQUALITY, mhz_last_ppm);
      DomoticzSensor(DZ_TEMP, temperature);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_CO2, types, mhz_last_ppm);
    WSContentSend_PD(HTTP_SNS_TEMP, types, temperature, TempUnit());
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns15(uint8_t function)
{
  bool result = false;

  if (mhz_type) {
    switch (function) {
      case FUNC_INIT:
        MhzInit();
        break;
      case FUNC_EVERY_SECOND:
        MhzEverySecond();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_15 == XdrvMailbox.index) {
          result = MhzCommandSensor();
        }
        break;
      case FUNC_JSON_APPEND:
        MhzShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MhzShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MHZ19
