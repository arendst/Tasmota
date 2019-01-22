/*
  xsns_22_sr04.ino - SR04 ultrasonic sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Nuno Ferreira and Theo Arends

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

#ifdef USE_SR04

#include <NewPing.h>
/*********************************************************************************************\
 * HC-SR04, HC-SR04+, JSN-SR04T - Ultrasonic distance sensor
 *
 * Code for SR04 family of ultrasonic distance sensors
 * References:
 * - https://www.dfrobot.com/wiki/index.php/Weather-proof_Ultrasonic_Sensor_SKU_:_SEN0207
\*********************************************************************************************/

uint8_t sr04_echo_pin = 0;
uint8_t sr04_trig_pin = 0;
real64_t distance;

NewPing* sonar = NULL;

void Sr04Init(void)
{
  sr04_echo_pin = pin[GPIO_SR04_ECHO];
  sr04_trig_pin = pin[GPIO_SR04_TRIG];
  sonar = new NewPing(sr04_trig_pin, sr04_echo_pin, 300);
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_DISTANCE[] PROGMEM =
  "%s{s}SR04 " D_DISTANCE "{m}%s" D_UNIT_CENTIMETER "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Sr04Show(boolean json)
{
  distance = (real64_t)(sonar->ping_median(5))/ US_ROUNDTRIP_CM;

  if (distance != 0) {                // Check if read failed
    char distance_chr[33];
    dtostrfd(distance, 3, distance_chr);

    if(json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"SR04\":{\"" D_JSON_DISTANCE "\":%s}"), mqtt_data, distance_chr);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_COUNT, distance_chr);  // Send distance as Domoticz Counter value
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_DISTANCE, mqtt_data, distance_chr);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_22

boolean Xsns22(byte function)
{
  boolean result = false;

  if ((pin[GPIO_SR04_ECHO] < 99) && (pin[GPIO_SR04_TRIG] < 99)) {
    switch (function) {
      case FUNC_INIT:
        Sr04Init();
        break;
      case FUNC_JSON_APPEND:
        Sr04Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Sr04Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SR04
