/*
  xsns_64_AHT10.ino - AHT10 I2C temperature and humidity sensor support for Tasmota

  Copyright (C) 2020  M. Wagner

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
#ifdef USE_AHT10
/*********************************************************************************************\
 * AHT10 - Temperature and Humidity
 *
 * I2C Address: 0x38
\*********************************************************************************************/

#define XSNS_64              64
#define XI2C_43              43  // See I2CDEVICES.md

#define AHT10_ADDR           0x38

unsigned char eSensorCalibrateCmd[3] = {0xE1, 0x08, 0x00};
unsigned char eSensorNormalCmd[3]    = {0xA8, 0x00, 0x00};
unsigned char eSensorMeasureCmd[3]   = {0xAC, 0x33, 0x00};
unsigned char eSensorResetCmd        = 0xBA;

struct AHT10 {
  float   humidity = NAN;
  float   temperature = NAN;
  uint8_t valid = 0;
  uint8_t count = 0;
  char    name[6] = "AHT10";
} AHT10;

bool begin()
{
    Wire.begin(AHT10_ADDR);
    Wire.beginTransmission(AHT10_ADDR);
    Wire.write(eSensorCalibrateCmd, 3);
    Wire.endTransmission();
    delay(500);

    if((readStatus() & 0x68) == 0x08)
        return true;
    else
    {
        return false;
    }
}

bool AHT10Read(void)
{
  unsigned long result_t, result_h, temp[6];

  if (AHT10.valid) { AHT10.valid--; }

    Wire.beginTransmission(AHT10_ADDR);
    Wire.write(eSensorMeasureCmd, 3);
    Wire.endTransmission();
    delay(100);

    Wire.requestFrom(AHT10_ADDR, 6);
    for(unsigned char i = 0; Wire.available() > 0; i++)
    {
        temp[i] = Wire.read();
    }
    result_h = ((temp[1] << 16) | (temp[2] << 8) | temp[3]) >> 4;
    result_t = ((temp[3] & 0x0F) << 16) | (temp[4] << 8) | temp[5];

  AHT10.humidity = result_h * 100 / 1048576;
  AHT10.temperature  =  ((200 * result_t) / 1048576) - 50;

  if (isnan(AHT10.temperature) || isnan(AHT10.humidity)) { return false; }

  AHT10.valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/
unsigned char readStatus(void)
{
    unsigned char result = 0;

    Wire.requestFrom(AHT10_ADDR, 1);
    result = Wire.read();
    return result;
}

void AHT10Detect(void)
{
  if (I2cActive(AHT10_ADDR))
  {
  return;
  }

  if (begin())
  {
    I2cSetActiveFound(AHT10_ADDR, AHT10.name);
    AHT10.count = 1;
  }
}

void AHT10EverySecond(void)
{
  if (uptime &1) {
    // AHT10: 55mS
    if (!AHT10Read()) {
      AddLogMissed(AHT10.name, AHT10.valid);
    }
  }
}

void AHT10Show(bool json)
{
  if (AHT10.valid) {
    char temperature[33];
    dtostrfd(AHT10.temperature, Settings.flag2.temperature_resolution, temperature);
    char humidity[33];
    dtostrfd(AHT10.humidity, Settings.flag2.humidity_resolution, humidity);

    if (json) {
      ResponseAppend_P(JSON_SNS_TEMPHUM, AHT10.name, temperature, humidity);
#ifdef USE_DOMOTICZ
      if ((0 == tele_period)) {
        DomoticzTempHumSensor(temperature, humidity);
      }
#endif // USE_DOMOTICZ
#ifdef USE_KNX
      if (0 == tele_period) {
        KnxSensor(KNX_TEMPERATURE, AHT10.temperature);
        KnxSensor(KNX_HUMIDITY, AHT10.humidity);
      }
#endif // USE_KNX
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_TEMP, AHT10.name, temperature, TempUnit());
      WSContentSend_PD(HTTP_SNS_HUM, AHT10.name, humidity);
#endif // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns64(uint8_t function)
{
  if (!I2cEnabled(XI2C_43)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    AHT10Detect();
  }
  else if (AHT10.count) {
    switch (function) {
    case FUNC_EVERY_SECOND:
      AHT10EverySecond();
      break;
    case FUNC_JSON_APPEND:
      AHT10Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      AHT10Show(0);
      break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_AHT10
#endif  // USE_I2C
