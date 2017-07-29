/*
  xsns_tsl2561.ino - TSL2561 ambient light sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Sebastian Muszynski

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
#ifdef USE_TSL2561
/*********************************************************************************************\
 * TSL2561 - Ambient Light Intensity
\*********************************************************************************************/

#include <TSL2561.h>

uint8_t tsl2561addr;
uint8_t tsl2561type = 0;
char tsl2561stype[7];

uint8_t tsl2561addresses[] = {
  TSL2561_ADDR_FLOAT,
  TSL2561_ADDR_LOW,
  TSL2561_ADDR_HIGH
};

TSL2561 tsl(tsl2561addresses[0]);

uint16_t tsl2561_readLux(void)
{
  return tsl.getLuminosity(TSL2561_VISIBLE);
}

boolean tsl2561_detect()
{
  if (tsl2561type) {
    return true;
  }

  char log[LOGSZ];
  uint8_t status;
  boolean success = false;

  for (byte i = 0; i < 4; i++) {
    tsl2561addr = tsl2561addresses[i];
    TSL2561 tsl(tsl2561addr);

    snprintf_P(log, sizeof(log), PSTR("I2C: Probing addr 0x%x for TSL2561."), tsl2561addr);
    addLog(LOG_LEVEL_DEBUG, log);

    if(tsl.begin()) {
      tsl.setGain(TSL2561_GAIN_16X);
      tsl.setTiming(TSL2561_INTEGRATIONTIME_101MS);

      success = true;
      tsl2561type = 1;
      strcpy(tsl2561stype, "TSL2561");
      break;
    }
  }

  if (success) {
    snprintf_P(log, sizeof(log), PSTR("I2C: %s found at address 0x%x"), tsl2561stype, tsl2561addr);
    addLog(LOG_LEVEL_DEBUG, log);
  }

  return success;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void tsl2561_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  if (!tsl2561type) {
    return;
  }

  uint16_t l = tsl2561_readLux();
  snprintf_P(svalue, ssvalue, PSTR("%s, \"%s\":{\"Illuminance\":%d}"), svalue, tsl2561stype, l);
  *djson = 1;
#ifdef USE_DOMOTICZ
  domoticz_sensor5(l);
#endif  // USE_DOMOTICZ
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_TSL2561[] PROGMEM =
  "<tr><th>TSL2561 Illuminance</th><td>%d lx</td></tr>";

String tsl2561_webPresent()
{
  String page = "";
  if (tsl2561type) {
    char sensor[80];
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TSL2561, tsl2561_readLux());
    page += sensor;
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_TSL2561
#endif  // USE_I2C

