/*
  xsns_58_dht12.ino - DHT12 I2C temperature and humidity sensor support for Tasmota

  Copyright (C) 2019  Stefan Oskamp and Theo Arends

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
#ifdef USE_DHT12
/*********************************************************************************************\
 * DHT12 - Temperature and Humidity
 *
 * I2C Address: 0x5C
\*********************************************************************************************/

#define XSNS_58              58
#define XI2C_41              41  // See I2CDEVICES.md

#define DHT12_ADDR          (uint8_t) 0x5C

char    dht12_name[] = "DHT12-0x00";
uint8_t dht12_count = 0;
float   dht12_temperature = NAN;
float   dht12_humidity = NAN;

bool Dht12Read(void)
{
    Wire.beginTransmission(DHT12_ADDR);
    Wire.write(0);
    if (Wire.endTransmission() != 0) {
        return false;
    }
    delay(50);
    Wire.requestFrom(DHT12_ADDR, (uint8_t) 5);
    delay(5);
    uint8_t humidity      = Wire.read();
    uint8_t humidityTenth = Wire.read();
    uint8_t temp          = Wire.read();
    uint8_t tempTenth     = Wire.read();
    uint8_t checksum      = Wire.read();
    dht12_humidity    = (float) humidity + (float) humidityTenth/(float) 10.0;
    dht12_temperature = (float) temp     + (float) tempTenth/(float) 10.0;
    return (!isnan(dht12_temperature) && !isnan(dht12_humidity));
}

void Dht12Detect(void)
{
    if (Dht12Read()) {
        snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, dht12_name, DHT12_ADDR);
        AddLog(LOG_LEVEL_DEBUG);
        snprintf_P(dht12_name, sizeof(dht12_name), PSTR("%s%c0x%02X"), "DHT12", IndexSeparator(), DHT12_ADDR);
        dht12_count = 1;
    }
}

void Dht12Show(bool json)
{
  if (dht12_count > 0)
  {
    char temperature[33];
    dtostrfd(dht12_temperature, Settings.flag2.temperature_resolution, temperature);
    char humidity[33];
    dtostrfd(dht12_humidity, Settings.flag2.humidity_resolution, humidity);

    if (json)
    {
      ResponseAppend_P(JSON_SNS_TEMPHUM, dht12_name, temperature, humidity);
#ifdef USE_DOMOTICZ
      if ((0 == tele_period))
      {
        DomoticzTempHumSensor(temperature, humidity);
      }
#endif // USE_DOMOTICZ

#ifdef USE_KNX
      if (0 == tele_period)
      {
        KnxSensor(KNX_TEMPERATURE, dht12_temperature);
        KnxSensor(KNX_HUMIDITY, dht12_humidity);
      }
#endif // USE_KNX

#ifdef USE_WEBSERVER
    }
    else
    {
      WSContentSend_PD(HTTP_SNS_TEMP, dht12_name, temperature, TempUnit());
      WSContentSend_PD(HTTP_SNS_HUM, dht12_name, humidity);
#endif // USE_WEBSERVER
    }
  }
}



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns58(uint8_t function)
{
  if (!I2cEnabled(XI2C_41)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Dht12Detect();
  }
  else if (dht12_count > 0) {
    switch (function) {
    case FUNC_EVERY_SECOND:
      Dht12Read();
      break;
    case FUNC_JSON_APPEND:
      Dht12Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Dht12Show(0);
      break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_DHT12
#endif  // USE_I2C
