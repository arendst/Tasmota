/*
  xsns_58_dht12.ino - DHT12 I2C temperature and humidity sensor support for Tasmota

  Copyright (C) 2021  Stefan Oskamp and Theo Arends

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

#define DHT12_ADDR           0x5C

struct DHT12 {
  float   temperature = NAN;
  float   humidity = NAN;
  uint8_t valid = 0;
  uint8_t count = 0;
  char    name[6] = "DHT12";
} Dht12;

bool Dht12Read(void)
{
  if (Dht12.valid) { Dht12.valid--; }

  Wire.beginTransmission(DHT12_ADDR);
  Wire.write(0);
  if (Wire.endTransmission() != 0) { return false; }

  delay(50);

  Wire.requestFrom(DHT12_ADDR, 5);
  delay(5);
  uint8_t humidity      = Wire.read();
  uint8_t humidityTenth = Wire.read();
  uint8_t temp          = Wire.read();
  uint8_t tempTenth     = Wire.read();
  uint8_t checksum      = Wire.read();

  Dht12.humidity    = ConvertHumidity( humidity + ((float) humidityTenth) /10 );
  Dht12.temperature = ConvertTemp( (temp + (tempTenth & 0x7F) / 10.0f) * ((tempTenth & 0x80) ? -1 : 1) );

  if (isnan(Dht12.temperature) || isnan(Dht12.humidity)) { return false; }

  Dht12.valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void Dht12Detect(void)
{
  if (I2cActive(DHT12_ADDR)) { return; }

  if (Dht12Read()) {
    I2cSetActiveFound(DHT12_ADDR, Dht12.name);
    Dht12.count = 1;
  }
}

void Dht12EverySecond(void)
{
  if (TasmotaGlobal.uptime &1) {
    // DHT12: 55mS
    if (!Dht12Read()) {
      AddLogMissed(Dht12.name, Dht12.valid);
    }
  }
}

void Dht12Show(bool json)
{
  if (Dht12.valid) {
    TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), Dht12.name, Dht12.temperature, Dht12.humidity);
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
  else if (Dht12.count) {
    switch (function) {
    case FUNC_EVERY_SECOND:
      Dht12EverySecond();
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
