/*
  xsns_chirp.ino - CHIRP ambient light sensor support for Sonoff-Tasmota

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
#ifdef USE_CHIRP
/*********************************************************************************************\
 * CHIRP - Ambient Light Intensity
\*********************************************************************************************/
#define TWI_GET_CAPACITANCE     0x00
#define TWI_SET_ADDRESS         0x01
#define TWI_GET_ADDRESS         0x02
#define TWI_MEASURE_LIGHT       0x03
#define TWI_GET_LIGHT           0x04
#define TWI_GET_TEMPERATURE     0x05
#define TWI_RESET               0x06
#define TWI_GET_VERSION         0x07
#define TWI_SLEEP               0x08
#define TWI_GET_BUSY            0x09


#define CHIRP_ADDR1         0x20


#define CHIRP_CONTINUOUS_HIGH_RES_MODE 0x10 // Start measurement at 1lx resolution. Measurement time is approx 120ms.

uint8_t chirpaddr;
uint8_t chirptype = 0;
char chirpstype[7];

uint16_t chirp_readLux(void)
{
  Wire.beginTransmission(chirpaddr);
  Wire.write(TWI_MEASURE_LIGHT);
  Wire.write(TWI_GET_LIGHT);
  Wire.endTransmission();
  delay(20);
  Wire.requestFrom(chirpaddr, (uint8_t)TWI_GET_ADDRESS);
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}


uint16_t chirp_readMoist(void)
{
  Wire.beginTransmission(chirpaddr);
  Wire.write(TWI_GET_CAPACITANCE);
  Wire.endTransmission();
  delay(20);
  Wire.requestFrom(chirpaddr, (uint8_t)TWI_GET_ADDRESS);
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}

float chirp_readTemp(void)
{
  Wire.beginTransmission(chirpaddr);
  Wire.write(TWI_GET_TEMPERATURE);
  Wire.endTransmission();
  delay(20);
  Wire.requestFrom(chirpaddr, (uint8_t)TWI_GET_ADDRESS);
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read();
  return t/10.0;
}

boolean chirp_detect()
{
  if (chirptype) {
    return true;
  }
  uint8_t status;
  boolean success = false;

  chirpaddr = CHIRP_ADDR1;
  Wire.beginTransmission(chirpaddr);
  Wire.write(CHIRP_CONTINUOUS_HIGH_RES_MODE);
  status = Wire.endTransmission();
  if (!status) {
    success = true;
    chirptype = 1;
    strcpy(chirpstype, "CHIRP");
  }
  if (success) {
    snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, chirpstype, chirpaddr);
    AddLog(LOG_LEVEL_DEBUG);
  } else {
    chirptype = 0;
  }
  return success;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/
#ifdef USE_WEBSERVER
 #ifndef USE_BH1750  // avoid duplicate definition
  const char HTTP_SNS_ILLUMINANCE[] PROGMEM =  "%s{s}%s " D_ILLUMINANCE "{m}%s%{e}";
  #endif
  const char HTTP_SNS_MOISTURE[] PROGMEM = "%s{s}%s " D_MOISTURE "{m}%s%{e}";
#endif  // USE_WEBSERVER

const char JSON_SNS_LIGHTMOISTTEMP[] PROGMEM = "%s,\"%s\":{\"" D_LIGHT "\":%d,\"" D_MOISTURE "\":%s,\"" D_TEMPERATURE "\":%s}";

String chirp_Show(boolean json)
{
  if (chirptype) {
    char temperature[10];
    char moisture[10];
    uint16_t l = chirp_readLux();

    dtostrfd(chirp_readMoist(), Settings.flag2.humidity_resolution, moisture);
    dtostrfd(chirp_readTemp() , Settings.flag2.temperature_resolution, temperature);

     if (json) {
       snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SNS_LIGHTMOISTTEMP, mqtt_data, "CHIRP", l, moisture,temperature);

       #ifdef USE_DOMOTICZ
         DomoticzTempHumSensor(temperature, moisture);
         DomoticzSensor(DZ_ILLUMINANCE, l);
       #endif  // USE_DOMOTICZ

  #ifdef USE_WEBSERVER
     } else {
       snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ILLUMINANCE, mqtt_data, "CHIRP", l);
       snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_MOISTURE, mqtt_data, "CHIRP", moisture);
       snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, "CHIRP", temperature, TempUnit());

  #endif // USE_WEBSERVER
     }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_20

boolean Xsns20(byte function)
{
  boolean result = false;

  if (chirptype) {
    switch (function) {
//      case FUNC_XSNS_INIT:
//        break;
      case FUNC_XSNS_PREP:
        chirp_detect();
        break;
      case FUNC_XSNS_JSON_APPEND:
        chirp_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_XSNS_WEB:
        chirp_Show(0);
        break;
#endif // USE_WEBSERVER
    }
  }
  return result;
}

#endif // USE_BMP
#endif // USE_I2C
