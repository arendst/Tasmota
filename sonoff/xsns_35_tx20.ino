/*
  xsns_35_Tx20.ino - La Crosse Tx20 wind sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Thomas Eckerstorfer and Theo Arends

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

#ifdef USE_TX20_WIND_SENSOR
/*********************************************************************************************\
 * La Crosse Tx20 wind sensor
 *
 * based on https://github.com/bunnyhu/ESP8266_TX20_wind_sensor/
 * http://blog.bubux.de/windsensor-tx20-mit-esp8266/
 * https://www.john.geek.nz/2011/07/la-crosse-tx20-anemometer-communication-protocol/
\*********************************************************************************************/

#define XSNS_35                  35

#define TX20_BIT_TIME          1220  // microseconds
#define TX20_RESET_VALUES        60  // seconds

// The Arduino standard GPIO routines are not enough,
// must use some from the Espressif SDK as well
extern "C" {
#include "gpio.h"
}

#ifdef USE_WEBSERVER

const char HTTP_SNS_TX20[] PROGMEM = "%s"
   "{s}TX20 " D_TX20_WIND_SPEED "{m}%s " D_UNIT_KILOMETER_PER_HOUR "{e}"
   "{s}TX20 " D_TX20_WIND_SPEED_AVG "{m}%s " D_UNIT_KILOMETER_PER_HOUR "{e}"
   "{s}TX20 " D_TX20_WIND_SPEED_MAX "{m}%s " D_UNIT_KILOMETER_PER_HOUR "{e}"
   "{s}TX20 " D_TX20_WIND_DIRECTION "{m}%s{e}";

#endif  // USE_WEBSERVER

const char kTx20Directions[] PROGMEM = D_TX20_NORTH "|"
                                       D_TX20_NORTH D_TX20_NORTH D_TX20_EAST "|"
                                       D_TX20_NORTH D_TX20_EAST "|"
                                       D_TX20_EAST D_TX20_NORTH D_TX20_EAST "|"
                                       D_TX20_EAST "|"
                                       D_TX20_EAST D_TX20_SOUTH D_TX20_EAST "|"
                                       D_TX20_SOUTH D_TX20_EAST "|"
                                       D_TX20_SOUTH D_TX20_SOUTH D_TX20_EAST "|"
                                       D_TX20_SOUTH "|"
                                       D_TX20_SOUTH D_TX20_SOUTH D_TX20_WEST "|"
                                       D_TX20_SOUTH D_TX20_WEST "|"
                                       D_TX20_WEST D_TX20_SOUTH D_TX20_WEST "|"
                                       D_TX20_WEST "|"
                                       D_TX20_WEST D_TX20_NORTH D_TX20_WEST "|"
                                       D_TX20_NORTH D_TX20_WEST "|"
                                       D_TX20_NORTH D_TX20_NORTH D_TX20_WEST;

uint8_t tx20_sa = 0;
uint8_t tx20_sb = 0;
uint8_t tx20_sd = 0;
uint8_t tx20_se = 0;
uint16_t tx20_sc = 0;
uint16_t tx20_sf = 0;

float tx20_wind_speed_kmh = 0;
float tx20_wind_speed_max = 0;
float tx20_wind_speed_avg = 0;
float tx20_wind_sum = 0;
int tx20_count = 0;
uint8_t tx20_wind_direction = 0;

boolean tx20_available = false;

void Tx20StartRead(void)
{
  /* La Crosse TX20 Anemometer datagram every 2 seconds
   * 0-0 11011 0011 111010101111 0101 1100 000101010000 0-0 - Received pin data at 1200 uSec per bit
   *     sa    sb   sc           sd   se   sf
   *     00100 1100 000101010000 1010 1100 000101010000     - sa to sd inverted user data, LSB first
   * sa - Start frame always 00100
   * sb - Wind direction 0 - 15
   * sc - Wind speed 0 - 511
   * sd - Checksum
   * se - Wind direction 0 - 15
   * sf - Wind speed 0 - 511
   */

  tx20_available = false;

  tx20_sa = 0;
  tx20_sb = 0;
  tx20_sd = 0;
  tx20_se = 0;
  tx20_sc = 0;
  tx20_sf = 0;

  delayMicroseconds(TX20_BIT_TIME / 2);

  for (int bitcount = 41; bitcount > 0; bitcount--) {
    uint8_t dpin = (digitalRead(pin[GPIO_TX20_TXD_BLACK]));
    if (bitcount > 41 - 5) {
      // start, inverted
      tx20_sa = (tx20_sa << 1) | (dpin ^ 1);
    } else if (bitcount > 41 - 5 - 4) {
      // wind dir, inverted
      tx20_sb = tx20_sb >> 1 | ((dpin ^ 1) << 3);
    } else if (bitcount > 41 - 5 - 4 - 12) {
      // windspeed, inverted
      tx20_sc = tx20_sc >> 1 | ((dpin ^ 1) << 11);
    } else if (bitcount > 41 - 5 - 4 - 12 - 4) {
      // checksum, inverted
      tx20_sd = tx20_sd >> 1 | ((dpin ^ 1) << 3);
    } else if (bitcount > 41 - 5 - 4 - 12 - 4 - 4) {
      // wind dir
      tx20_se = tx20_se >> 1 | (dpin << 3);
    } else {
      // windspeed
      tx20_sf = tx20_sf >> 1 | (dpin << 11);
    }

    delayMicroseconds(TX20_BIT_TIME);
  }

  uint8_t chk = (tx20_sb + (tx20_sc & 0xf) + ((tx20_sc >> 4) & 0xf) + ((tx20_sc >> 8) & 0xf));
  chk &= 0xf;

  if ((chk == tx20_sd) && (tx20_sc < 400)) {  // if checksum seems to be ok and wind speed below 40 m/s
    tx20_available = true;
  }
/*
  if ((tx20_sb == tx20_se) && (tx20_sc == tx20_sf) && (tx20_sc < 400)) {
    tx20_available = true;
  }
*/
  // Must clear this bit in the interrupt register,
  // it gets set even when interrupts are disabled
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, 1 << pin[GPIO_TX20_TXD_BLACK]);
}

void Tx20Read(void)
{
  if (!(uptime % TX20_RESET_VALUES)) {
    tx20_count = 0;
    tx20_wind_sum = 0;
    tx20_wind_speed_max = 0;
  }
  else if (tx20_available) {
    tx20_wind_speed_kmh = float(tx20_sc) * 0.36;
    if (tx20_wind_speed_kmh > tx20_wind_speed_max) {
      tx20_wind_speed_max = tx20_wind_speed_kmh;
    }
    tx20_count++;
    tx20_wind_sum += tx20_wind_speed_kmh;
    tx20_wind_speed_avg = tx20_wind_sum / tx20_count;
    tx20_wind_direction = tx20_sb;
  }
}

void Tx20Init(void) {
  pinMode(pin[GPIO_TX20_TXD_BLACK], INPUT);
  attachInterrupt(pin[GPIO_TX20_TXD_BLACK], Tx20StartRead, RISING);
}

void Tx20Show(boolean json)
{
  char wind_speed_string[33];
  dtostrfd(tx20_wind_speed_kmh, 2, wind_speed_string);
  char wind_speed_max_string[33];
  dtostrfd(tx20_wind_speed_max, 2, wind_speed_max_string);
  char wind_speed_avg_string[33];
  dtostrfd(tx20_wind_speed_avg, 2, wind_speed_avg_string);
  char wind_direction_string[4];
  GetTextIndexed(wind_direction_string, sizeof(wind_direction_string), tx20_wind_direction, kTx20Directions);

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"TX20\":{\"Speed\":%s,\"SpeedAvg\":%s,\"SpeedMax\":%s,\"Direction\":\"%s\"}"),
      mqtt_data, wind_speed_string, wind_speed_avg_string, wind_speed_max_string, wind_direction_string);
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TX20, mqtt_data, wind_speed_string, wind_speed_avg_string, wind_speed_max_string, wind_direction_string);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns35(byte function)
{
  boolean result = false;

  if (pin[GPIO_TX20_TXD_BLACK] < 99) {
    switch (function) {
      case FUNC_INIT:
        Tx20Init();
        break;
      case FUNC_EVERY_SECOND:
        Tx20Read();
        break;
      case FUNC_JSON_APPEND:
        Tx20Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Tx20Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_TX20_WIND_SENSOR