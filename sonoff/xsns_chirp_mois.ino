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
    snprintf_P(log_data, sizeof(log_data), PSTR("I2C: %s found at address 0x%x"), chirpstype, chirpaddr);
    addLog(LOG_LEVEL_DEBUG);
  } else {
    chirptype = 0;
  }
  return success;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void chirp_mqttPresent(uint8_t* djson)
{
  if (!chirptype) {
    return;
  }
  char stemp1[6];
  float t2;
  uint16_t l = chirp_readLux();
  uint16_t m = chirp_readMoist();
  float t = chirp_readTemp();
  dtostrf(t, 1, sysCfg.flag.temperature_resolution, stemp1);
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s, \"%s\":{\"Light\":%d, \"Moisture\":%d, \"Temperature\":%s}"), mqtt_data, chirpstype, l,m,stemp1);
  *djson = 1;
#ifdef USE_DOMOTICZ
  domoticz_sensor5(l);
#endif  // USE_DOMOTICZ
}

#ifdef USE_WEBSERVER
const char HTTP_CHIRP_ILLUMINANCE[] PROGMEM =
  "<tr><th>CHIRP Illuminance</th><td>%d lx</td></tr>";
const char HTTP_CHIRP_MOISTURE[] PROGMEM =
    "<tr><th>CHIRP Moisture</th><td>%d</td></tr>";

String chirp_webPresent()
{
  String page = "";
  if (chirptype) {
    char sensor[80];
    char stemp1[6];
    snprintf_P(sensor, sizeof(sensor), HTTP_CHIRP_ILLUMINANCE, chirp_readLux());
    page += sensor;
    snprintf_P(sensor, sizeof(sensor), HTTP_CHIRP_MOISTURE, chirp_readMoist());
    page += sensor;
    dtostrf(chirp_readTemp(), 1, sysCfg.flag.temperature_resolution, stemp1);
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, "CHIRP", stemp1, tempUnit());
    page += sensor;
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_CHIRP
#endif  // USE_I2C
