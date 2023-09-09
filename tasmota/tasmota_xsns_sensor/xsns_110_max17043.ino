/*
  xsns_110_max17043.ino - Support for MAX17043 fuel-gauge systems Lipo batteries for Tasmota

  Copyright (c) 2023  Vincent de Groot
  Copyright (c) 2023  Paul Blacknell

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
#ifdef USE_MAX17043
/*********************************************************************************************\
 * MAX17043 fuel-gauge for 3.7 Volt Lipo batteries
 *
 * Battery voltage in Volt and State Of Charge (SOC) in percent are published via MQTT
 * 
 * The alert flag and alert threshold are not required for MQTT, the alert pin is not used 
 * by this sensor driver.
 * 
 * Tested module(s):
 * 
 * https://www.dfrobot.com/product-1734.html
 * 
 * Not yet tested module(s):
 * 
 * https://www.aliexpress.us/item/2251832479401925.html 
 * 
\*********************************************************************************************/

#define XSNS_110           110
#define XI2C_83            83      // See I2CDEVICES.md

#define MAX17043_NAME      "MAX17043"

#define MAX17043_ADDRESS   0x36
#define MAX17043_VCELL     0x02
#define MAX17043_SOC       0x04
#define MAX17043_MODE      0x06
#define MAX17043_VERSION   0x08
#define MAX17043_CONFIG    0x0c
#define MAX17043_COMMAND   0xfe

#define MAX17043_MODE_COMMAND_POWERONRESET  0x5400
#define MAX17043_MODE_COMMAND_QUICKSTART    0x4000
#define MAX17043_CONFIG_POWER_UP_DEFAULT    0x971c
#define MAX17043_CONFIG_NO_COMPENSATION     0x9700

bool max17043 = false;
int battery_latest = 101;

/*********************************************************************************************/

void Max17043Init(void) {
  if (I2cSetDevice(MAX17043_ADDRESS)) { 
    if (REASON_DEEP_SLEEP_AWAKE == ESP_ResetInfoReason()) {
      // if waking from deep sleep we assume the hardware design maintained power to the MAX17043
      // retaining its model of the internal dynamics the battery
      // if the hardware design doesn't (to conserve battery) then we lose some of
      // the benefits of the device anyway as it'll be re-learning from scratch every DeepSleepTime seconds.
 
      // to confirm this is a MAX17043 - check for both the default (if the MAX17043 did lose it's power)
      // or our setting if power was maintained
      if (I2cRead16(MAX17043_ADDRESS, MAX17043_CONFIG) == MAX17043_CONFIG_NO_COMPENSATION
          || I2cRead16(MAX17043_ADDRESS, MAX17043_CONFIG) == MAX17043_CONFIG_POWER_UP_DEFAULT) {
        max17043 = true;
        I2cSetActiveFound(MAX17043_ADDRESS, MAX17043_NAME);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SNS: Waking from deep sleep - skipping " MAX17043_NAME " Power on Reset & Quick Start"));
      }
    } else {
      // otherwise perform a full Power on Reset (which is the same as disconnecting power)
      // and a Quick Start which essentially does the same but handles a noisy power up sequence

      I2cWrite16(MAX17043_ADDRESS, MAX17043_COMMAND, MAX17043_MODE_COMMAND_POWERONRESET);
      delay(10);
      if (I2cRead16(MAX17043_ADDRESS, MAX17043_CONFIG) == MAX17043_CONFIG_POWER_UP_DEFAULT) {  // Read the default to confirm this is a MAX17043
        I2cWrite16(MAX17043_ADDRESS, MAX17043_MODE, MAX17043_MODE_COMMAND_QUICKSTART);    
        I2cWrite16(MAX17043_ADDRESS, MAX17043_CONFIG, MAX17043_CONFIG_NO_COMPENSATION);
        delay(10);
        max17043 = true;
        I2cSetActiveFound(MAX17043_ADDRESS, MAX17043_NAME);
      }
    }
  }
}


void Max17043Show(bool json) {
  float voltage = (1.25f * (float)(I2cRead16(MAX17043_ADDRESS, MAX17043_VCELL) >> 4)) / 1000.0;  // Battery voltage in Volt
  uint16_t per = I2cRead16(MAX17043_ADDRESS, MAX17043_SOC);
  float percentage = (float)((per >> 8) + 0.003906f * (per & 0x00ff));  // Battery remaining charge in percent
  int battery_current;

  // During charging the percentage might be (slightly) above 100%. To avoid strange numbers
  // in the statistics the percentage provided by this driver will not go above 100%
  if (percentage > 100.0) { percentage = 100.0; }

  // only update the system percentage if it's changed
  battery_current = int(round(percentage));
  if (battery_latest != battery_current) {
    char cmnd[30];
    sprintf(cmnd, "%s %d", D_CMND_ZIGBEE_BATTPERCENT, battery_current);
    ExecuteCommand(cmnd, SRC_SENSOR);
    battery_latest = battery_current;
  }
  if (json) {
    ResponseAppend_P(PSTR(",\"" MAX17043_NAME "\":{\"" D_JSON_VOLTAGE "\":%3_f,\"" D_JSON_BATTPERCENT "\":%2_f}"), &voltage, &percentage );
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(PSTR("{s}" MAX17043_NAME " " D_VOLTAGE "{m}%1_f" D_UNIT_VOLT "{e}"), &voltage);
    WSContentSend_PD(PSTR("{s}" MAX17043_NAME " " D_BATTERY_CHARGE "{m}%1_f " D_UNIT_PERCENT " {e}"), &percentage);
#endif
  }
}

/*********************************************************************************************\
 *  Interface
\*********************************************************************************************/

bool Xsns110(uint32_t function) {
  if (!I2cEnabled(MAX17043_ADDRESS)) { return false; } 

  if (FUNC_INIT == function) {
    Max17043Init();
  }
  else if (max17043) {
    switch (function) {
      case FUNC_JSON_APPEND:
        Max17043Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Max17043Show(0);
        break;
#endif // USE_WEBSERVER
    }
  }
  return false;
}

#endif  // USE_MAX17043
#endif  // USE_I2C
