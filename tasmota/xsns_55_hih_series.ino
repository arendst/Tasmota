/*
  xsns_55_hih_series.ino - Honeywell HIH series temperature and humidity sensor support for Tasmota

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

#ifdef USE_I2C
#ifdef USE_HIH6
/*********************************************************************************************\
 * HIH6130 - Temperature and Humidity
 *
 * https://sensing.honeywell.com/HIH6130-021-001-humidity-sensors
 * https://sensing.honeywell.com/i2c-comms-humidicon-tn-009061-2-en-final-07jun12.pdf
 * https://sensing.honeywell.com/command-mode-humidicon-tn-009062-3-en-final-07jun12.pdf
 * https://github.com/ControlEverythingCommunity/HIH6130/blob/master/Arduino/HIH6130.ino
 *
 * I2C Address: 0x27
\*********************************************************************************************/

#define XSNS_55             55
#define XI2C_36             36  // See I2CDEVICES.md

#define HIH6_ADDR           0x27

struct HIH6 {
  float temperature = 0;
  float humidity = 0;
  uint8_t valid = 0;
  uint8_t type = 0;
  char types[4] = "HIH";
} Hih6;

bool Hih6Read(void)
{
  Wire.beginTransmission(HIH6_ADDR);
  if (Wire.endTransmission() != 0) { return false; }  // In case of error

  delay(40);                                          // Wait for Valid data (Stale data doesn't work. See #6808)

  uint8_t data[4];
  Wire.requestFrom(HIH6_ADDR, 4);
  if (4 == Wire.available()) {
    data[0] = Wire.read();                            // Status and Humidity msb
    data[1] = Wire.read();                            // Humidity lsb
    data[2] = Wire.read();                            // Temp msb
    data[3] = Wire.read();                            // Temp lsb
  } else { return false; }

//  uint8_t status = data[0] >> 6;  // 0 = Valid data, 1 = Stale data, 2 = Command mode, 3 = Not used

  Hih6.humidity = ConvertHumidity(((float)(((data[0] & 0x3F) << 8) | data[1]) * 100.0) / 16383.0);
  // Convert the data to 14-bits
  int temp = ((data[2] << 8) | (data[3] & 0xFC)) / 4;
  Hih6.temperature = ConvertTemp(((float)temp / 16384.0) * 165.0 - 40.0);

  Hih6.valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void Hih6Detect(void)
{
  if (I2cActive(HIH6_ADDR)) { return; }

  if (uptime < 2) { delay(20); } // Skip entering power on comand mode
  Hih6.type = Hih6Read();
  if (Hih6.type) {
    I2cSetActiveFound(HIH6_ADDR, Hih6.types);
  }
}

void Hih6EverySecond(void)
{
  if (uptime &1) {
    // HIH6130: 30mS
    if (!Hih6Read()) {
      AddLogMissed(Hih6.types, Hih6.valid);
    }
  }
}

void Hih6Show(bool json)
{
  if (Hih6.valid) {
    char temperature[33];
    dtostrfd(Hih6.temperature, Settings.flag2.temperature_resolution, temperature);
    char humidity[33];
    dtostrfd(Hih6.humidity, Settings.flag2.humidity_resolution, humidity);

    if (json) {
      ResponseAppend_P(JSON_SNS_TEMPHUM, Hih6.types, temperature, humidity);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzTempHumSensor(temperature, humidity);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
      if (0 == tele_period) {
        KnxSensor(KNX_TEMPERATURE, Hih6.temperature);
        KnxSensor(KNX_HUMIDITY, Hih6.humidity);
      }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_TEMP, Hih6.types, temperature, TempUnit());
      WSContentSend_PD(HTTP_SNS_HUM, Hih6.types, humidity);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns55(uint8_t function)
{
  if (!I2cEnabled(XI2C_36)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Hih6Detect();
  }
  else if (Hih6.type) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Hih6EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Hih6Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Hih6Show(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_HIH6
#endif  // USE_I2C

