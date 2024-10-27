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
 * For PMSx003T models that report temperature and humidity define PMS_MODEL_PMS5003T
 * This module can also support de Winsen ZH03x series of dust particle sensors,
 * To support those sensors, you must define PMS_MODEL_ZH03X in the confuguration file.
\*********************************************************************************************/

#define XSNS_18             18

#include <TasmotaSerial.h>

#ifndef WARMUP_PERIOD
#define WARMUP_PERIOD 30          // Turn on PMSX003 XX-seconds before read in passive mode
#endif

#ifndef MIN_INTERVAL_PERIOD
#define MIN_INTERVAL_PERIOD 60    // minimum interval period in seconds required for passive mode
#endif

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

#ifdef PMS_MODEL_ZH03X
const uint8_t kPmsCommands[][9] PROGMEM = {
    //  0     1    2    3     4     5     6     7     8
    {0xFF, 0x01, 0x78, 0x40, 0x00, 0x00, 0x00, 0x00, 0x47},  // pms_set_active_mode
    {0xFF, 0x01, 0xA7, 0x01, 0x00, 0x00, 0x00, 0x00, 0x57},  // pms_sleep
    {0xFF, 0x01, 0xA7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58},  // pms_wake
    {0xFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46},  // pms_set_passive_mode
    {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79}}; // pms_passive_mode_read
#else
const uint8_t kPmsCommands[][7] PROGMEM = {
    //  0     1    2    3     4     5     6
    {0x42, 0x4D, 0xE1, 0x00, 0x01, 0x01, 0x71},  // pms_set_active_mode
    {0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x73},  // pms_sleep
    {0x42, 0x4D, 0xE4, 0x00, 0x01, 0x01, 0x74},  // pms_wake
    {0x42, 0x4D, 0xE1, 0x00, 0x00, 0x01, 0x70},  // pms_set_passive_mode
    {0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71}}; // pms_passive_mode_read
#endif // PMS_MODEL_ZH03X

struct pmsX003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
#if defined(PMS_MODEL_PMS3003) || defined(PMS_MODEL_ZH03X)
  uint16_t reserved1, reserved2, reserved3;
#else
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um;
#ifdef PMS_MODEL_PMS5003T
  uint16_t temperature10x, humidity10x;
#else
  uint16_t particles_50um, particles_100um;
#endif // PMS_MODEL_PMS5003T
  uint16_t unused;
#endif  // PMS_MODEL_PMS3003
  uint16_t checksum;
} pms_data;

/*********************************************************************************************/

size_t PmsSendCmd(uint8_t command_id)
{
  return PmsSerial->write(kPmsCommands[command_id], sizeof(kPmsCommands[command_id]));
}

/*********************************************************************************************/

bool PmsReadData(void)
{
  if (! PmsSerial->available()) {
    return false;
  }
  while ((PmsSerial->peek() != 0x42) && PmsSerial->available()) {
    PmsSerial->read();
  }
#if defined(PMS_MODEL_PMS3003) || defined(PMS_MODEL_ZH03X)
  if (PmsSerial->available() < 24) {
#else
  if (PmsSerial->available() < 32) {
#endif  // PMS_MODEL_PMS3003
    return false;
  }

#if defined(PMS_MODEL_PMS3003) || defined(PMS_MODEL_ZH03X)
  uint8_t buffer[24];
  PmsSerial->readBytes(buffer, 24);
#else
  uint8_t buffer[32];
  PmsSerial->readBytes(buffer, 32);
#endif  // PMS_MODEL_PMS3003
  uint16_t sum = 0;
  PmsSerial->flush();  // Make room for another burst

#if defined(PMS_MODEL_PMS3003) || defined(PMS_MODEL_ZH03X)
  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, 24);
#else
  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, 32);
#endif  // PMS_MODEL_PMS3003

  // get checksum ready
#if defined(PMS_MODEL_PMS3003) || defined(PMS_MODEL_ZH03X)
  for (uint32_t i = 0; i < 22; i++) {
#else
  for (uint32_t i = 0; i < 30; i++) {
#endif  // PMS_MODEL_PMS3003
    sum += buffer[i];
  }
  // The data comes in endian'd, this solves it so it works on all platforms
#if defined(PMS_MODEL_PMS3003) || defined(PMS_MODEL_ZH03X)
  uint16_t buffer_u16[12];
  for (uint32_t i = 0; i < 12; i++) {
#else
  uint16_t buffer_u16[15];
  for (uint32_t i = 0; i < 15; i++) {
#endif  // PMS_MODEL_PMS3003
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }

#if defined(PMS_MODEL_PMS3003) || defined(PMS_MODEL_ZH03X)
  if (sum != buffer_u16[10]) {
#else
  if (sum != buffer_u16[14]) {
#endif  // PMS_MODEL_PMS3003
    AddLog(LOG_LEVEL_DEBUG, PSTR("PMS: " D_CHECKSUM_FAILURE));
    return false;
  }

#if defined(PMS_MODEL_PMS3003) || defined(PMS_MODEL_ZH03X)
  memcpy((void *)&pms_data, (void *)buffer_u16, 22);
#else
  memcpy((void *)&pms_data, (void *)buffer_u16, 30);
#endif  // PMS_MODEL_PMS3003
  Pms.valid = 10;

  if (!Pms.discovery_triggered) {
    TasmotaGlobal.discovery_counter = 1;      // Force discovery
    Pms.discovery_triggered = true;
  }

  return true;
}
#ifdef PMS_MODEL_ZH03X
bool ZH03ReadDataPassive() // process the passive mode response of the ZH03x sensor
{
  if (! PmsSerial->available()) {
    return false;
  }
  while ((PmsSerial->peek() != 0xFF) && PmsSerial->available()) {
    PmsSerial->read();
  }
  if (PmsSerial->available() < 9) {
    return false;
  }
  uint8_t buffer[9];
  PmsSerial->readBytes(buffer, 9);
  if (buffer[1] != 0x86) {
     return false;
  }
  PmsSerial->flush();  // Make room for another burst

  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, 9);
 
  uint8_t sum = 0;
  for (uint32_t i = 1; i < 7; i++) {
    sum += buffer[i];
  }
  sum=(~sum)+1;
  if (sum != buffer[8]) { 
    AddLog(LOG_LEVEL_DEBUG, PSTR("ZH03x: " D_CHECKSUM_FAILURE));
    return false;
  }

  uint16_t buffer_u16[12];
  for (uint32_t i = 1; i < 4; i++) { 
    buffer_u16[i] = buffer[i*2 + 1];
    buffer_u16[i] += (buffer[i*2] << 8);
    buffer_u16[i+3] = buffer[i*2 + 1];      // Direct and Environment values identical
    buffer_u16[i+3] += (buffer[i*2] << 8);  // Direct and Environment values identical
    buffer_u16[0] = 20;                     // set dummy framelength
    buffer_u16[11] = buffer[8];             // copy checksum
  }
  
    memcpy((void *)&pms_data, (void *)buffer_u16, 22); 
  
  Pms.valid = 10;

  if (!Pms.discovery_triggered) {
    TasmotaGlobal.discovery_counter = 1;      // Force discovery
    Pms.discovery_triggered = true;
  }

  return true;

}
#endif  // PMS_MODEL_ZH03X

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
#ifdef PMS_MODEL_ZH03X
    bool validread;
    if (Settings->pms_wake_interval >= MIN_INTERVAL_PERIOD) {
      validread = ZH03ReadDataPassive();  // in passive mode, the response is different from the PMS sensors
    }
      else {
        validread = PmsReadData();  // In active mode the rsponse is identical to the PMS sensors
      }
    if (validread) {
#else
    if (PmsReadData()) {
#endif  // PMS_MODEL_ZH03X
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

void PmsInit(void) {
  Pms.type = 0;
  if (PinUsed(GPIO_PMS5003_RX)) {
    PmsSerial = new TasmotaSerial(Pin(GPIO_PMS5003_RX), (PinUsed(GPIO_PMS5003_TX)) ? Pin(GPIO_PMS5003_TX) : -1, 1);
    if (PmsSerial->begin(9600)) {
      if (PmsSerial->hardwareSerial()) { ClaimSerial(); }
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR("PMS: Serial UART%d"), PmsSerial->getUart());
#endif

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

// This gives more accurate data for forest fire smoke.  PurpleAir gives you this conversion option labeled "US EPA"
// https://cfpub.epa.gov/si/si_public_record_report.cfm?dirEntryId=353088&Lab=CEMM
/*
Copy-paste from the PDF Slide 26
y={0 ≤ x <30: 0.524*x - 0.0862*RH + 5.75}
y={30≤ x <50: (0.786*(x/20 - 3/2) + 0.524*(1 - (x/20 - 3/2)))*x -0.0862*RH + 5.75}
y={50 ≤ x <210: 0.786*x - 0.0862*RH + 5.75}
y={210 ≤ x <260: (0.69*(x/50 – 21/5) + 0.786*(1 - (x/50 – 21/5)))*x - 0.0862*RH*(1 - (x/50 – 21/5)) + 2.966*(x/50 – 21/5) + 5.75*(1 - (x/50 – 21/5)) + 8.84*(10^{-4})*x^{2}*(x/50 – 21/5)}
y={260 ≤ x: 2.966 + 0.69*x + 8.84*10^{-4}*x^2}

y= corrected PM2.5 µg/m3
x= PM2.5 cf_atm (lower)
RH= Relative humidity as measured by the PurpleAir
*/
int usaEpaStandardPm2d5Adjustment(int pm25_standard, int relative_humidity)
{
  // Rename to use the same variables from the paper
  float x = pm25_standard;
  float RH = relative_humidity;
  if (x<30) {
    return 0.524f * x - 0.0862f * RH + 5.75f;
  } else if(x<50) {
    return (0.786f * (x/20.0f - 3.0f/2.0f) + 0.524f * (1.0f - (x/20.0f - 3.0f/2.0f))) * x - 0.0862f * RH + 5.75f;
  } else if(x<210) {
    return 0.786f * x - 0.0862f * RH + 5.75f;
  } else if(x<260) {
    return (0.69f * (x/50.0f - 21.0f/5.0f) + 0.786f * (1.0f - (x/50.0f - 21.0f/5.0f))) * x - 0.0862f * RH * (1.0f - (x/50.0f - 21.0f/5.0f)) + 2.966f * (x/50.0f - 21.0f/5.0f) + 5.75f * (1.0f - (x/50.0f - 21.0f/5.0f)) + 8.84f * FastPrecisePowf(10.0f, -4.0f) * FastPrecisePowf(x,2.0f) * (x/50.0f - 21.0f/5.0f);
  } else {
    return 2.966f + 0.69f * x + 8.84f * FastPrecisePowf(10.0f, -4.0f) * FastPrecisePowf(x, 2.0f);
  }
}

// Compute US AQI using the 2024+ table
// https://forum.airnowtech.org/t/the-aqi-equation-2024-valid-beginning-may-6th-2024/453
int compute_us_aqi(int pm25_standard)
{
  if (pm25_standard <= 9) {
    return map_float(pm25_standard, 0, 9, 0, 50);
  } else if (pm25_standard <= 35) {
    return map_float(pm25_standard, 9.1f, 35.4f, 51, 100);
  } else if (pm25_standard <= 55) {
    return map_float(pm25_standard, 35.5f, 55.4f, 101, 150);
  } else if (pm25_standard <= 125) {
    return map_float(pm25_standard, 55.5f, 125.4f, 151, 200);
  } else if (pm25_standard <= 225) {
    return map_float(pm25_standard, 125.5f, 225.4f, 201, 300);
  } else if (pm25_standard <= 325) {
    return map_float(pm25_standard, 225.5f, 325.4f, 301, 500);
  } else {
    return 500;
  }
}

void PmsShow(bool json) {
  if (Pms.valid) {
    char types[10];

#ifdef PMS_MODEL_ZH03X
    strcpy_P(types, PSTR("ZH03x"));
#elif defined(PMS_MODEL_PMS3003)
    strcpy_P(types, PSTR("PMS3003"));
#elif defined(PMS_MODEL_PMS5003T)
    strcpy_P(types, PSTR("PMS5003T"));
#else
    strcpy_P(types, PSTR("PMS5003"));
#endif

#ifdef PMS_MODEL_PMS5003T
    float temperature = ConvertTemp(pms_data.temperature10x/10.0);
    float humidity = ConvertHumidity(pms_data.humidity10x/10.0);
    int epa_us_aqi;
    // When in Fahrenheit include US AQI
    if (Settings->flag.temperature_conversion) {    // Fahrenheit - US, Liberia, Cayman Islands
      epa_us_aqi = compute_us_aqi(usaEpaStandardPm2d5Adjustment(pms_data.pm25_standard, humidity));
    }
#endif // PMS_MODEL_PMS5003T
    int us_aqi;
    // Use US AQI for Fahrenheit, EAQI (European Air Quality Index) for Celsius
    if (Settings->flag.temperature_conversion) {    // Fahrenheit - US, Liberia, Cayman Islands
      us_aqi = compute_us_aqi(pms_data.pm25_standard);
    }

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"CF1\":%d,\"CF2.5\":%d,\"CF10\":%d,\"PM1\":%d,\"PM2.5\":%d,\"PM10\":%d"),
        types,
        pms_data.pm10_standard, pms_data.pm25_standard, pms_data.pm100_standard,
        pms_data.pm10_env, pms_data.pm25_env, pms_data.pm100_env);
      if (Settings->flag.temperature_conversion) {    // Fahrenheit - US, Liberia, Cayman Islands
        ResponseAppend_P(PSTR(",\"US_AQI\":%d"), us_aqi);
      }
#if !(defined(PMS_MODEL_PMS3003) || defined(PMS_MODEL_ZH03X))
      ResponseAppend_P(PSTR(",\"PB0.3\":%d,\"PB0.5\":%d,\"PB1\":%d,\"PB2.5\":%d"),
        pms_data.particles_03um, pms_data.particles_05um, pms_data.particles_10um, pms_data.particles_25um);
#ifdef PMS_MODEL_PMS5003T
      ResponseAppend_P(PSTR(","));
      ResponseAppendTHD(temperature, humidity);
      if (Settings->flag.temperature_conversion) {    // Fahrenheit - US, Liberia, Cayman Islands
        ResponseAppend_P(PSTR(",\"US_EPA_AQI\":%d"), epa_us_aqi);
      }
#else
      ResponseAppend_P(PSTR(",\"PB5\":%d,\"PB10\":%d"),
        pms_data.particles_50um, pms_data.particles_100um);
#endif  // PMS_MODEL_PMS5003T
#endif  // No PMS_MODEL_PMS3003
      ResponseJsonEnd();

#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_COUNT, pms_data.pm10_env);     // PM1
        DomoticzSensor(DZ_VOLTAGE, pms_data.pm25_env);   // PM2.5
        DomoticzSensor(DZ_CURRENT, pms_data.pm100_env);  // PM10
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "1", pms_data.pm10_standard);
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "2.5", pms_data.pm25_standard);
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "10", pms_data.pm100_standard);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "1", pms_data.pm10_env);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "2.5", pms_data.pm25_env);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "10", pms_data.pm100_env);
#if !(defined(PMS_MODEL_PMS3003) || defined(PMS_MODEL_ZH03X))
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "0.3", pms_data.particles_03um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "0.5", pms_data.particles_05um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "1", pms_data.particles_10um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "2.5", pms_data.particles_25um);
      if (Settings->flag.temperature_conversion) {    // Fahrenheit - US, Liberia, Cayman Islands
        WSContentSend_PD(HTTP_SNS_US_AQI, types, us_aqi);
      }
#ifdef PMS_MODEL_PMS5003T
      WSContentSend_THD(types, temperature, humidity);
      if (Settings->flag.temperature_conversion) {    // Fahrenheit - US, Liberia, Cayman Islands
        WSContentSend_PD(HTTP_SNS_US_EPA_AQI, types, epa_us_aqi);
      }
#else
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "5", pms_data.particles_50um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "10", pms_data.particles_100um);
#endif  // PMS_MODEL_PMS5003T
#endif  // No PMS_MODEL_PMS3003
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns18(uint32_t function)
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