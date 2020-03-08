/*
  xsns_63_AHT1x.ino - AHT10 I2C temperature and humidity sensor support for Tasmota

  Copyright (C) 2020  Martin Wagner

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
#ifdef USE_AHT1x
/*********************************************************************************************\
 * AHT10/15 - Temperature and Humidity
 *
 * I2C Address: 0x38
 *
 * Attention: this Sensor is incompatible with other I2C devices on I2C bus.
 * 
 * The Datasheet write:
 * "Only a single AHT10 can be connected to the I2C bus and no other I2C 
 *  devices can be connected".
 * 
 * after lot of search and tests, now is confirmed that works only reliable with one sensor
 * on I2C Bus
\*********************************************************************************************/

#define XSNS_63              63
#define XI2C_43              43  // See I2CDEVICES.md

#define AHT10_ADDR           0x38

uint8_t eSensorCalibrateCmd[3] = {0xE1, 0x08, 0x00};
uint8_t eSensorMeasureCmd[3]   = {0xAC, 0x33, 0x00};
uint8_t eSensorResetCmd        = 0xBA;

struct AHT10 {
  float   humidity = NAN;
  float   temperature = NAN;
  uint8_t valid = 0;
  uint8_t count = 0;
  char    name[6] = "AHT1x";
} AHT10;

bool AHT10Read(void)
{
  if (AHT10.valid) { AHT10.valid--; }

  uint8_t data[6];

  Wire.beginTransmission(AHT10_ADDR);
  Wire.write(eSensorMeasureCmd, 3);
  Wire.endTransmission();
  delay(80);

  Wire.requestFrom(AHT10_ADDR, 6);
  for (uint32_t i = 0; Wire.available() > 0; i++) {
    data[i] = Wire.read();
  }

  uint32_t result_h = ((data[1] << 16) | (data[2] << 8) | data[3]) >> 4;
  uint32_t result_t = ((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5];

  float humidity = result_h * 100 / 1048576;
  float temperature = ((200 * result_t) / 1048576) - 50;

  if (isnan(temperature) || isnan(humidity)) { return false; }

  AHT10.humidity = ConvertHumidity(humidity);
  AHT10.temperature  = ConvertTemp(temperature);

  AHT10.valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

bool AHT10Init(void)
{
  Wire.begin(AHT10_ADDR);
  Wire.beginTransmission(AHT10_ADDR);
  Wire.write(eSensorCalibrateCmd, 3); // init with internal temp coef.
  Wire.endTransmission();

  delay(40);  //  after tests, its ok

  return (0x08 == (AHT10ReadStatus() & 0x68)); 
}

uint8_t AHT10ReadStatus(void)
{
  Wire.requestFrom(AHT10_ADDR, 1);
  uint8_t result = Wire.read();
  return result;
}

void AHT10Reset(void)
{
  Wire.beginTransmission(AHT10_ADDR);
  Wire.write(eSensorResetCmd);
  Wire.endTransmission();
  delay(20);
}

/********************************************************************************************/

void AHT10Detect(void)
{
  if (I2cActive(AHT10_ADDR)) { return; }

  if (AHT10Init()) {
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

bool Xsns63(uint8_t function)
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
