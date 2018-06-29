/*
  xsns_14_sht3x.ino - SHT3X temperature and humidity sensor support for
  Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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
#ifdef USE_TMP112
/*********************************************************************************************\
 * TMP112 - Temperature
 *
 * I2C Address: 0x48
\*********************************************************************************************/

#define TMP112_ADDR 0x48     // address for tmp112 sensor
#define TMP112_NAME "TMP112" // name

bool Tmp112Show(boolean json) {
  unsigned int data[6];

  float t = NAN;
  char temperature[10];

  Wire.beginTransmission(TMP112_ADDR);

  Wire.write(0X00);       // Select Data Registers
  Wire.endTransmission(); // End Transmission
  delay(500);
  Wire.requestFrom(TMP112_ADDR, 2); // Request 2 bytes , Msb first

  if (Wire.available()) {
    Serial.flush(); // remove garbage

    // Read the two bytes
    int msb = Wire.read();
    int lsb = Wire.read();
    Wire.endTransmission();

    // Data conversion in raw values
    int temp = (msb * 256 + lsb) / 16;
		if(temp > 2047)
      temp -= 4096;
		
    t = ConvertTemp(temp * 0.0625);
    dtostrfd(t, Settings.flag2.temperature_resolution, temperature);

    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data),
                 PSTR("%s,\"%s\":{\"" D_JSON_TEMPERATURE "\":%s}"), mqtt_data,
                 TMP112_NAME, temperature);

          if (0 == tele_period) {  // We want the same first sensor to report to Domoticz in case a read is missed
#ifdef USE_DOMOTICZ
        DomoticzSensor(DZ_TEMP, temperature);

#endif // USE_DOMOTICZ

#ifdef USE_KNX
        KnxSensor(KNX_TEMPERATURE, t);
#endif // USE_KNX
          }

#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data,
                 TMP112_NAME, temperature, TempUnit());
#endif // USE_WEBSERVER
    }
  } else {
    snprintf_P(log_data, sizeof(log_data), PSTR("TMP112 not found at 0x48"));
    AddLog(LOG_LEVEL_DEBUG);
  }
  return (!isnan(t));
}

/********************************************************************************************/

void Tmp112Init() {
  Wire.beginTransmission(TMP112_ADDR);
  Wire.write(0x01);                  // Select ENABLE register
  Wire.write(0x60A0);                // Select normal operation
  if (Wire.endTransmission() != 0) { // End transmission and release I2C bus
    snprintf_P(log_data, sizeof(log_data), PSTR("TMP112 could not end transmission"));
    AddLog(LOG_LEVEL_DEBUG);
    return;
  }

  // log
  snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "TMP112", 0x48);
  AddLog(LOG_LEVEL_DEBUG);
}



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_25

boolean Xsns25(byte function) {
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
    case FUNC_INIT:
      Tmp112Init();
      break;
    case FUNC_JSON_APPEND:
      Tmp112Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_APPEND:
      Tmp112Show(0);
      break;
#endif // USE_WEBSERVER
    }
  }
  return result;
}

#endif // USE_TMP112
#endif // USE_I2C