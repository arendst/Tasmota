/*
  xsns_92_hm330x.ino - Driver for Seedstudio Grove HM3301 particle sensor

  Copyright (C) 2021  Barbudor, SeedStudio

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

#ifdef USE_I2C
#ifdef USE_HM330X
/**************************************************************************************************
 * Driver for Particule sensor SeedStudio Grove HM3301
 * ================================================================================================
 * This driver use the I2C mode. Only address 0x40 is supported by the sensor so it is hardcoded in
 * the driver without any possibility to change it (without editing and recompiling)
 * By default the sensor remains active all the time but no data are provided for the first
 * HM330X_WARMUP_DELAY seconds (to allow the sensor to warm up). During warmup, no data are
 * returned in the GUI or through SENSOR message in order to provide invalid data. In case
 * of any doubt, logs with level 3 will confirm right at boot if the sensor has been detected.
 *
 * Low power / life extension
 * --------------------------
 * It is possible to connect a GPIO to pin SET of the module and assign it to "HM330X SET" for
 * automatic sleep in order to extend the life time of the sensor. When this is used, activity of
 * the sensor in synced with Teleperiod as follow
 * 1) At start, sensor is immediately enabled by setting "HM330X SET" HI and start warmup
 *    If any Telemetry occurs before the end of the warmup, no data are provided and warmup
 *    goes on.
 * 2) After HM330X_WARMUP_DELAY, data are available and reported in web and SENSOR
 * 3) Immediately after the next teleperiod SENSOR message, if Teleperiod is greater than
 *    (HM330X_WARMUP_DELAY + 2) the sensor is put to sleep by taking the pin "HM330X SET" LOW
 *    and a new cycle start
 * New cycle
 * 4) (HM330X_WARMUP_DELAY + 2) seconds before the next teleperiod, the sensor is awaken by taking
 *    the pin "HM330X SET" HI for warmup. No data are available yet
 * 5) 2 seconds before Telemetry, warmup is completed and data are available to Web until
 *    Telemetry messages are sent
 * 6) Back to 2)
 *
 * Overridables (in user_config_override.h)
 * ----------------------------------------
 * HM330X_DEFAULT_ADDRESS       0x40    in case SeedStudio makes this changeable in the future
 * HM330X_WARMUP_DELAY          30      time for warmup, data are not pulled before that delay
 * HM330X_HIDE_OUT_OF_DATE      false   if changed to true web data and sensor will not be
 *                                      published when outdated
 *************************************************************************************************/

//#define HM330X_SIM

#define XSNS_93        93
#define XI2C_63        63  // See I2CDEVICES.md

#ifndef HM330X_DEFAULT_ADDRESS
#define HM330X_DEFAULT_ADDRESS  0x40
#endif

#define HM330X_SELECT_COMM_CMD   0X88

#define HM330X_FRAME_SIZE      29

#ifndef HM330X_WARMUP_DELAY
#define HM330X_WARMUP_DELAY 30                // Ignore HM330X data for XX seconds after start
#endif

#define HM330X_PRE_TELEPERIOD_ENABLE    3     // Sensor made active before teleperiod

#ifndef HM330X_HIDE_OUT_OF_DATE
#define HM330X_HIDE_OUT_OF_DATE   false       // override to true to disable display when the data are out of date
#endif

enum {
    HM330X_NO_ERROR = 0,
    HM330X_ERROR_PARAM = -1,
    HM330X_ERROR_NOT_FOUND = -2,
    HM330X_ERROR_COMM = -3,
    HM330X_ERROR_TIMEOUT = -4,
    HM330X_ERROR_CHECKSUM = -5,
    HM330X_ERROR_MEMALLOC = -6,
    HM330X_ERROR_OTHERS = -128
};

struct HM330XFrame {
  uint16_t    reserved;
  uint16_t    sensor_num;
  uint16_t    pm1_0_standard, pm2_5_standard, pm10_0_standard;
  uint16_t    pm1_0_env, pm2_5_env, pm10_0_env;
  uint16_t    particles_0_3um, particles_0_5um, particles_1_0um, particles_2_5um, particles_5_0um, particles_10_0um;
  uint8_t     checksum;
};

enum HM330X_State {
  HM330X_SLEEP = 0,     // sensor is sleeping
  HM330X_WARMUP = 1,    // sensor is in warmup
  HM330X_ACTIVE = 2     // sensor is active and can be polled for data
};

struct HM330XDATA {
  uint8_t   warmup_counter;   // count for warmup
  bool      valid;            // valid data have been retrieved from the sensor
  uint8_t   state;            // state machine
  int8_t    set_pin;        // pin to set the device asleep or active
  struct HM330XFrame rx_buffer;
};

struct HM330XDATA *HM330Xdata = nullptr;


int HM330XUpdate() {
#ifdef HM330X_SIM
  HM330Xdata->rx_buffer.pm1_0_standard   =   10;
  HM330Xdata->rx_buffer.pm2_5_standard   =   25;
  HM330Xdata->rx_buffer.pm10_0_standard  =  100;
  HM330Xdata->rx_buffer.pm1_0_env        = 1010;
  HM330Xdata->rx_buffer.pm2_5_env        = 1025;
  HM330Xdata->rx_buffer.pm10_0_env       = 1100;
  HM330Xdata->rx_buffer.particles_0_3um  = 2003;
  HM330Xdata->rx_buffer.particles_0_5um  = 2005;
  HM330Xdata->rx_buffer.particles_1_0um  = 2010;
  HM330Xdata->rx_buffer.particles_2_5um  = 2025;
  HM330Xdata->rx_buffer.particles_5_0um  = 2050;
  HM330Xdata->rx_buffer.particles_10_0um = 2100;
#else
  uint32_t time_out_count = 0;
#if HM330X_HIDE_OUT_OF_DATE
  HM330Xdata->valid = false;
#endif
  Wire.requestFrom((uint8_t)HM330X_DEFAULT_ADDRESS, (uint8_t)HM330X_FRAME_SIZE);
  while (HM330X_FRAME_SIZE != Wire.available()) {
      time_out_count++;
      if (time_out_count > 10) {
          return HM330X_ERROR_TIMEOUT;
      }
      delay(1);
  }
  uint8_t checksum = 0;
  uint8_t *data = (uint8_t*)&(HM330Xdata->rx_buffer);
  int i = 0;
  for (; i < HM330X_FRAME_SIZE-1; i++) {
      checksum += data[i] = Wire.read();
  }
  data[i] = Wire.read();
  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, data, HM330X_FRAME_SIZE);
  if (checksum != data[i]) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("HM3: checksum error"));
    return HM330X_ERROR_CHECKSUM;
  }

  for (i = 0; i < (HM330X_FRAME_SIZE/2); i++) {
      uint8_t tmp = data[2*i];    // revert endianness
      data[2*i] = data[2*i+1],
      data[2*i+1] = tmp;
  }
#endif
    HM330Xdata->valid = true;
    return HM330X_NO_ERROR;
}

int HM330XSendI2CCommand(uint8_t cmd) {
  int ret = HM330X_NO_ERROR;
#ifdef HM330X_SIM
#else
  Wire.beginTransmission(HM330X_DEFAULT_ADDRESS);
  Wire.write(cmd);
  ret = Wire.endTransmission();
  if (ret != 0) {
      ret = HM330X_ERROR_NOT_FOUND;
  }
#endif
  return ret;
}

void HM330XInit(void) {
  if (!I2cSetDevice(HM330X_DEFAULT_ADDRESS)) { return; }

  int8_t set_pin = Pin(GPIO_HM330X_SET);
  if (set_pin >= 0) {
    pinMode(set_pin, OUTPUT);
    digitalWrite(set_pin, 1);
    delay(5);
  }

  if (HM330X_NO_ERROR != HM330XSendI2CCommand(HM330X_SELECT_COMM_CMD)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("HM3: no response from address 0x%02X"), HM330X_DEFAULT_ADDRESS);
    return;
  }

  HM330Xdata = (HM330XDATA*)calloc(1,sizeof(HM330XDATA));
  if (!HM330Xdata) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("HM3: out of memory"));
    return;
  }

  HM330Xdata->valid = false;
  HM330Xdata->set_pin = set_pin;
  HM330Xdata->state = HM330X_WARMUP;
  HM330Xdata->warmup_counter = HM330X_WARMUP_DELAY;

  I2cSetActiveFound(HM330X_DEFAULT_ADDRESS, "HM330X");
}

void HM330XEverySecond() {
  uint16_t time_until_tp = Settings->tele_period - TasmotaGlobal.tele_period;

  // if (HM330Xdata)
  //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("HM3: EverySec wu:%d, v:%d, s:%d, sp:%d"),
  //     HM330Xdata->warmup_counter,
  //     HM330Xdata->valid,
  //     HM330Xdata->state,
  //     HM330Xdata->set_pin
  //   );
  // else
  //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("HM3: nullptr"));

  if (HM330X_SLEEP == HM330Xdata->state) {
    if (time_until_tp < (HM330X_WARMUP_DELAY+HM330X_PRE_TELEPERIOD_ENABLE)) {
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HM3: Exit sleep mode"));
      if (HM330Xdata->set_pin >= 0) {
        digitalWrite(HM330Xdata->set_pin, 1);
      }
      HM330Xdata->state = HM330X_WARMUP;
      HM330Xdata->warmup_counter = HM330X_WARMUP_DELAY;
#if HM330X_HIDE_OUT_OF_DATE
      HM330Xdata->valid = false;
#endif
    }
  }
  if (HM330X_WARMUP == HM330Xdata->state) {
    HM330Xdata->warmup_counter--;
    if (0 == HM330Xdata->warmup_counter) {
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HM3: Warmup done"));
      if (HM330X_NO_ERROR != HM330XSendI2CCommand(HM330X_SELECT_COMM_CMD)) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("HM3: no response from address 0x%02X"), HM330X_DEFAULT_ADDRESS);
      }
      HM330Xdata->state = HM330X_ACTIVE;
    }
  }
  if (HM330X_ACTIVE == HM330Xdata->state) {
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HM3: Update"));
    HM330XUpdate();
  }
}

void HM330XEnterSleep()
{
  if (HM330Xdata->set_pin >= 0 && HM330X_ACTIVE == HM330Xdata->state && Settings->tele_period > (HM330X_WARMUP_DELAY+HM330X_PRE_TELEPERIOD_ENABLE)) {
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HM3: Enter sleep mode"));
    digitalWrite(HM330Xdata->set_pin, 0);
    HM330Xdata->state = HM330X_SLEEP;
#if HM330X_HIDE_OUT_OF_DATE
    HM330Xdata->valid = false;
#endif
  }
}


/*********************************************************************************************\
 * SNS Interface
\*********************************************************************************************/

void HM330XShow(bool json) {
  if (HM330Xdata->valid) {
    char types[10];
    strcpy_P(types, PSTR("HM330X"));

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"CF1\":%d,\"CF2.5\":%d,\"CF10\":%d,\"PM1\":%d,\"PM2.5\":%d,\"PM10\":%d,\"PB0.3\":%d,\"PB0.5\":%d,\"PB1\":%d,\"PB2.5\":%d,\"PB5\":%d,\"PB10\":%d}"),
        types,
        HM330Xdata->rx_buffer.pm1_0_standard, HM330Xdata->rx_buffer.pm2_5_standard, HM330Xdata->rx_buffer.pm10_0_standard,
        HM330Xdata->rx_buffer.pm1_0_env, HM330Xdata->rx_buffer.pm2_5_env, HM330Xdata->rx_buffer.pm10_0_env,
        HM330Xdata->rx_buffer.particles_0_3um, HM330Xdata->rx_buffer.particles_0_5um, HM330Xdata->rx_buffer.particles_1_0um,
        HM330Xdata->rx_buffer.particles_2_5um, HM330Xdata->rx_buffer.particles_5_0um, HM330Xdata->rx_buffer.particles_10_0um);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_COUNT, HM330Xdata->rx_buffer.pm1_0_env);     // PM1
        DomoticzSensor(DZ_VOLTAGE, HM330Xdata->rx_buffer.pm2_5_env);   // PM2.5
        DomoticzSensor(DZ_CURRENT, HM330Xdata->rx_buffer.pm10_0_env);  // PM10
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "1", HM330Xdata->rx_buffer.pm1_0_standard);
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "2.5", HM330Xdata->rx_buffer.pm2_5_standard);
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "10", HM330Xdata->rx_buffer.pm10_0_standard);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "1", HM330Xdata->rx_buffer.pm1_0_env);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "2.5", HM330Xdata->rx_buffer.pm2_5_env);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "10", HM330Xdata->rx_buffer.pm10_0_env);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "0.3", HM330Xdata->rx_buffer.particles_0_3um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "0.5", HM330Xdata->rx_buffer.particles_0_5um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "1", HM330Xdata->rx_buffer.particles_1_0um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "2.5", HM330Xdata->rx_buffer.particles_2_5um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "5", HM330Xdata->rx_buffer.particles_5_0um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "10", HM330Xdata->rx_buffer.particles_10_0um);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns93(uint32_t function)
{
  if (!I2cEnabled(XI2C_63)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    HM330XInit();
  }
  else if (HM330Xdata) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        HM330XEverySecond();
        break;
      case FUNC_AFTER_TELEPERIOD:
        HM330XEnterSleep();
        break;
      case FUNC_JSON_APPEND:
        HM330XShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        HM330XShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_HM330X
#endif  // USE_I2C
