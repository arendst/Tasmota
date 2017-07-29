/*
 *  xsns_ads1115.ino - ADS1115 16bit 4-channel A/D converter support for Sonoff-Tasmota
 *
 *  Copyright (C) 2017  Sebastian Muszynski
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef USE_I2C
#ifdef USE_ADS1115

/*********************************************************************************************\
 * ADS1115 - 16bit 4-channel A/D converter
 *
 * Required library: https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/ADS1115
 *
\*********************************************************************************************/

#include <ADS1115.h>

uint8_t ads1115addr;
uint8_t ads1115type = 0;
char ads1115stype[8];

uint8_t ads1115addresses[] = {
  ADS1115_ADDRESS_ADDR_GND, // address pin low (GND)
  ADS1115_ADDRESS_ADDR_VDD, // address pin high (VCC)
  ADS1115_ADDRESS_ADDR_SDA, // address pin tied to SDA pin
  ADS1115_ADDRESS_ADDR_SCL // address pin tied to SCL pin
};

ADS1115 adc0(ads1115addresses[0]);

int16_t ads1115_getConversion(byte channel)
{
  switch (channel) {
    case 0:
      adc0.setMultiplexer(ADS1115_MUX_P0_NG);
      break;
    case 1:
      adc0.setMultiplexer(ADS1115_MUX_P1_NG);
      break;
    case 2:
      adc0.setMultiplexer(ADS1115_MUX_P2_NG);
      break;
    case 3:
      adc0.setMultiplexer(ADS1115_MUX_P3_NG);
      break;
  }

  return adc0.getConversion(true);
}

boolean ads1115_detect()
{
  if (ads1115type) {
    return true;
  }

  char log[LOGSZ];
  uint8_t status;
  boolean success = false;

  for (byte i = 0; i < 4; i++) {
    ads1115addr = ads1115addresses[i];
    ADS1115 adc0(ads1115addr);

    snprintf_P(log, sizeof(log), PSTR("I2C: Probing addr 0x%x for ADS1115."), ads1115addr);
    addLog(LOG_LEVEL_DEBUG, log);

    if(adc0.testConnection()) {
      adc0.initialize();
      adc0.setGain(ADS1115_PGA_2P048); // Set the gain (PGA) +/-4.096V
      success = true;
      ads1115type = 1;
      strcpy(ads1115stype, "ADS1115");
      break;
    }
  }

  if (success) {
    snprintf_P(log, sizeof(log), PSTR("I2C: %s found at address 0x%x"), ads1115stype, ads1115addr);
    addLog(LOG_LEVEL_DEBUG, log);
  }

  return success;
}

/*********************************************************************************************\
 * Presentation
 *
\*********************************************************************************************/

void ads1115_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  if (!ads1115type) {
    return;
  }

  char stemp1[10];
  byte dsxflg = 0;

  for (byte i = 0; i < 4; i++) {
    int16_t val = ads1115_getConversion(i);
    if (!dsxflg) {
      snprintf_P(svalue, ssvalue, PSTR("%s, \"%s\":{"), svalue, ads1115stype);
      *djson = 1;
      stemp1[0] = '\0';
    }
    dsxflg++;
    snprintf_P(svalue, ssvalue, PSTR("%s%s\"AnalogInput%d\":%d"), svalue, stemp1, i, val);
    strcpy(stemp1, ", ");
  }
  *djson = 1;
  if (dsxflg) {
    snprintf_P(svalue, ssvalue, PSTR("%s}"), svalue);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_ADS[] PROGMEM =
  "<tr><th>ADS1115 AnalogInput%d</th><td>%d</td></tr>";

String ads1115_webPresent()
{
  String page = "";
  if (ads1115type) {
    char sensor[80];
    for (byte i = 0; i < 4; i++) {
      int16_t val = ads1115_getConversion(i);
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_ADS, i, val);
      page += sensor;
    }
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_ADS1115
#endif  // USE_I2C
