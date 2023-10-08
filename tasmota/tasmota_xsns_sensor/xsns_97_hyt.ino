/*
  xsns_97_hyt.ino - HYT I2C temperature and humidity sensor support for Tasmota

  Copyright (C) 2020  Thomas Schnittcher
  Copyright (C) 2022  Adjan Kretz, fix compatibility issues with current HYT

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
#ifdef USE_HYT
/*********************************************************************************************\
 * HYT221, HYT271, HYT371 and HYT939 Temperature and Humidity
 * https://www.ist-ag.com/en/products-services/humidity-modules-sensors
 *
 * I2C Address: 0x28
\*********************************************************************************************/

#define XSNS_97              97
#define XI2C_68              68  // See I2CDEVICES.md

#define HYT_ADDR           0x28  // I2C address of HYT 221, 271, 371 etc.

struct HYT {
  float   humidity = NAN;
  float   temperature = NAN;
  uint8_t valid = 0;
  uint8_t count = 0;
  char    name[6] = "HYT";
} HYT;

bool HYT_Read(void) {
  if (HYT.valid) { HYT.valid--; }

  Wire.beginTransmission(HYT_ADDR);
  Wire.requestFrom(HYT_ADDR, 4);
  if (Wire.available() == 4) {
    uint8_t data1      = Wire.read();
    uint8_t data2      = Wire.read();
    uint8_t data3      = Wire.read();
    uint8_t data4      = Wire.read();
    Wire.endTransmission();

    // Convert the data to 14-bits
    float humidity = ((((data1 & 0x3F) * 256) + data2) * 100.0) / 16383.0;
    int temp = ((data3 * 256) + (data4 & 0xFC)) / 4;
    float temperature = (temp / 16384.0) * 165.0 - 40.0;

    HYT.humidity = ConvertHumidity(humidity);
    HYT.temperature  = ConvertTemp(temperature);
  }
  if (isnan(HYT.temperature) || isnan(HYT.humidity)) { return false; }

  HYT.valid = SENSOR_MAX_MISS;

//  HYT.valid = 1;
  return true;
}

/********************************************************************************************/

void HYT_Detect(void) {
  if (I2cSetDevice(HYT_ADDR)) {
    I2cSetActiveFound(HYT_ADDR, "HYT");
    HYT.count = 1;
  }
}

/********************************************************************************************/

void HYT_EverySecond(void) {
  if (!(TasmotaGlobal.uptime %4)) {  // Every 4 seconds
    if (!HYT_Read()) {
      AddLogMissed(HYT.name, HYT.valid);
    }
  }
}

void HYT_Show(bool json) {
  if (HYT.valid) {
    TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), HYT.name, HYT.temperature, HYT.humidity);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns97(uint32_t function) {
  if (!I2cEnabled(XI2C_68)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    HYT_Detect();
  }
  else if (HYT.count) {
    switch (function) {
    case FUNC_EVERY_SECOND:
      HYT_EverySecond();
      break;
    case FUNC_JSON_APPEND:
      HYT_Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      HYT_Show(0);
      break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_HYT
#endif  // USE_I2C