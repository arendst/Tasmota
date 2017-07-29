/*
  xsns_sht3x.ino - SHT1x temperature and sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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
#ifdef USE_SHT3X
#include "Wire.h"
/*********************************************************************************************\
 * SHT3x - Temperature and Humidy
 *
 * Reading temperature and humidity takes about TBD milliseconds!
 * Source: Wemos https://github.com/wemos/WEMOS_SHT3x_Arduino_Library/blob/master/src/WEMOS_SHT3X.cpp
\*********************************************************************************************/

uint8_t sht3x_sda_pin;
uint8_t sht3x_scl_pin;
uint8_t sht3x_type = 0;


boolean sht3x_readTempHum(float &t, float &h)
{
   unsigned int data[6];

  float tempRaw;
  float humRaw;

  Wire.beginTransmission(0x45);
  // Send measurement command
  Wire.write(0x2C);
  Wire.write(0x06);
  // Stop I2C transmission
  if (Wire.endTransmission()!=0)
    return 0;

  delay(500);

  // Request 6 bytes of data
  Wire.requestFrom(0x45, 6);

  // Read 6 bytes of data
  // cTemp msb, cTemp lsb, cTemp crc, humidity msb, humidity lsb, humidity crc
  for (int i=0;i<6;i++) {
          data[i]=Wire.read();
  };

  delay(50);

  if (Wire.available()!=0)
    return 0;

  // Convert the data
  t = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
  //fTemp = (cTemp * 1.8) + 32;
  h = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);

  return (!isnan(t) && !isnan(h));
}

boolean sht3x_readCharTempHum(char* temp, char* hum)
{
  float t;
  float h;

  boolean success = sht3x_readTempHum(t, h);
  dtostrf(t, 1, sysCfg.flag.temperature_resolution, temp);
  dtostrf(h, 1, sysCfg.flag.humidity_resolution, hum);
  return success;
}

boolean sht3x_detect()
{
  if (sht3x_type) {
    return true;
  }

  float t;
  float h;

  sht3x_sda_pin = pin[GPIO_I2C_SDA];
  sht3x_scl_pin = pin[GPIO_I2C_SCL];
  if (sht3x_readTempHum(t, h)) {
    sht3x_type = 1;
    addLog_P(LOG_LEVEL_DEBUG, PSTR("I2C: SHT3X found"));
  } else {
    Wire.begin(sht3x_sda_pin, sht3x_scl_pin);
    sht3x_type = 0;
  }
  return sht3x_type;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void sht3x_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  if (!sht3x_type) {
    return;
  }

  char stemp[10];
  char shum[10];

  if (sht3x_readCharTempHum(stemp, shum)) {
    snprintf_P(svalue, ssvalue, JSON_SNS_TEMPHUM, svalue, "SHT3X", stemp, shum);
    *djson = 1;
#ifdef USE_DOMOTICZ
    domoticz_sensor2(stemp, shum);
#endif  // USE_DOMOTICZ
  }
}

#ifdef USE_WEBSERVER
String sht3x_webPresent()
{
  String page = "";
  if (sht3x_type) {
    char stemp[10];
    char shum[10];

    if (sht3x_readCharTempHum(stemp, shum)) {
      char sensor[80];
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, "SHT3X", stemp, tempUnit());
      page += sensor;
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_HUM, "SHT3X", shum);
      page += sensor;
    }
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_SHT3X
#endif  // USE_I2C

