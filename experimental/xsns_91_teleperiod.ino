/*

  EXPERIMENTAL DRIVER - PROVIDED FOR TINKERERS BUT NO SUPPORT PROVIDED

  How to use: Simply copy file into sonoff folder and compile - The driver will then be included automatically
              Please note that if you use more than one experimental driver you will need to rename the 91
              value to a different value e.g. 92-99
  
  xdrv_91_teleperiod.ino - Append current teleperiod to SENSOR JSON as follows:

  19:43:55 MQT: tele/sonoff/SENSOR = {"Time":"2019-01-17T19:43:55","ANALOG":{"A0":0},"TelePeriod":{"teleperiod":10,"telecurrent":10}}

  Copyright (C) 2019  Andre Thomas and Theo Arends

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


#define XSNS_91 91

uint16_t tele_counter = 0;

void Xsns91_TeleCounterJSON(void)
{
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"TelePeriod\":{\"teleperiod\":%u,\"telecurrent\":%u}"),mqtt_data, Settings.tele_period, tele_counter);
  tele_counter = 0;
}

boolean Xsns91(byte function)
{
  boolean result = false;
  switch (function) {
    case FUNC_EVERY_SECOND:
      tele_counter++;
      break;
    case FUNC_JSON_APPEND:
      Xsns91_TeleCounterJSON();
      break;
    }
  return result;
}
