/*
  xsns_42_hmc5883l.ino - HMC5883L compass support for Sonoff-Tasmota

  Copyright (C) 2019  Kevin Baluha

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
#ifdef USE_HMC5883L
/*********************************************************************************************\
 * HMC5883L compass 
 *
 * Source: Kevin Baluha
 *
 * I2C Address: 0x1E 
\*********************************************************************************************/

#define XSNS_42                          42

#define D_SENSOR_HMC5883L                 "HMC5883L"

#define HMC5883L_ADDR                  0x1E

uint8_t HMC5883L_address;
uint8_t HMC5883L_addresses[] = { HMC5883L_ADDR };
uint8_t HMC5883L_found;

#include <HMC5883L.h>

HMC5883L hmc5883l;

int16_t mX = 0;
int16_t mY = 0;
int16_t mZ = 0;

struct {
    char * label;
    int16_t *value;
    } hmc5583l_values[] = {{ (char*)"mX", &mX}, { (char*)"mY", &mY}, {(char*)"mZ", &mZ} };

void HMC5883LGetValues(void)
{
  hmc5883l.read_data();
  mX = hmc5883l.meas.MX;
  mY = hmc5883l.meas.MY;
  mZ = hmc5883l.meas.MZ;
}

/* Work in progress - not yet fully functional */
void HMC5883LSetGain(int x, int y, int z)
{
  hmc5883l.setgain(x);
  hmc5883l.setgain(y);
  hmc5883l.setgain(z);
}

void HMC5883LDetect()
{
  if (HMC5883L_found)
  {
    return;
  }

  for (uint8_t i = 0; i < sizeof(HMC5883L_addresses); i++)
  {
    if(!I2cDevice(HMC5883L_addresses[i]))
      {
        continue;
      }

    HMC5883L_address = HMC5883L_addresses[i];
    hmc5883l.setAddr(HMC5883L_addresses[i]);
    if (hmc5883l.init()) 
      {
      HMC5883L_found = 1;
      }
  }

  if (HMC5883L_found)
  {
    snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, D_SENSOR_HMC5883L, HMC5883L_address);
    AddLog(LOG_LEVEL_DEBUG);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_AXIS[] PROGMEM = "%s{s}%s %s {m}%i{e}";                              // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif // USE_WEBSERVER

void HMC5883LtoString(uint8_t address)
{
  char label[30];
  snprintf_P(label, sizeof(label), "HMC5883 Compass (%02x)", address);

  for (uint8_t i = 0; i < sizeof(hmc5583l_values) / sizeof(hmc5583l_values[0]); i++) 
  {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_AXIS, mqtt_data, label, hmc5583l_values[i].label, *(hmc5583l_values[i].value));
  }
}

void HMC5883LtoJSON(char *comma_j) 
{

  char *comma = (char*)"";

  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s{"), mqtt_data,comma_j);
  for (uint8_t i = 0; i < sizeof(hmc5583l_values) / sizeof(hmc5583l_values[0]); i++) 
  {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"%s\":%i"), mqtt_data, comma, hmc5583l_values[i].label, *(hmc5583l_values[i].value));
    comma = (char*)",";
  }

  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
}

void HMC5883LShow(bool json)
{
  if (HMC5883L_found) 
  {
    HMC5883LGetValues();

    if (json) 
    {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"HMC5883L\":"), mqtt_data);
    }

    char *comma = (char*)"";

    if (json) 
    {
      HMC5883LtoJSON(comma);
    } 
#ifdef USE_WEBSERVER
    else 
    {
      HMC5883LtoString(HMC5883L_address);
    }
#endif // USE_WEBSERVER

  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns42(uint8_t function)
{
  bool result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_PREP_BEFORE_TELEPERIOD:
        HMC5883LDetect();
        break;
      case FUNC_EVERY_SECOND:
        if (tele_period == Settings.tele_period -3) {
          HMC5883LGetValues();
        }
        break;
      case FUNC_JSON_APPEND:
        HMC5883LShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        HMC5883LShow(0);
        HMC5883LGetValues();
        break;
#endif // USE_WEBSERVER
    }
  }
  return result;
}

#endif // USE_HMC5883L
#endif // USE_I2C
