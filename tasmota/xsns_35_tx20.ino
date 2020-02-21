/*
  xsns_35_Tx20.ino - La Crosse Tx20/Tx23 wind sensor support for Tasmota

  Copyright (C) 2020  Thomas Eckerstorfer, Norbert Richter and Theo Arends

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

#if defined(USE_TX20_WIND_SENSOR) || defined(USE_TX23_WIND_SENSOR)
#if defined(USE_TX20_WIND_SENSOR) && defined(USE_TX23_WIND_SENSOR)
#undef USE_TX20_WIND_SENSOR
#warning **** use USE_TX20_WIND_SENSOR or USE_TX23_WIND_SENSOR but not both together, TX20 disabled ****
#endif
/*********************************************************************************************\
 * La Crosse TX20/TX23 Anemometer
 *
 * based on https://github.com/bunnyhu/ESP8266_TX20_wind_sensor/
 * http://blog.bubux.de/windsensor-tx20-mit-esp8266/
 * https://www.john.geek.nz/2011/07/la-crosse-tx20-anemometer-communication-protocol/
 * http://www.rd-1000.com/chpm78/lacrosse/Lacrosse_TX23_protocol.html
 * https://www.john.geek.nz/2012/08/la-crosse-tx23u-anemometer-communication-protocol/
 * 
 * TX23 RJ11 connection:
 *  1 yellow      - GND
 *  2 green       - NC
 *  3 red         - Vcc 3.3V
 *  4 black/brown - TxD Signal (GPIOxx)
\*********************************************************************************************/

#define XSNS_35                  35

#define TX2X_BIT_TIME          1220  // microseconds
#define TX2X_RESET_VALUES        60  // seconds

// The Arduino standard GPIO routines are not enough,
// must use some from the Espressif SDK as well
extern "C" {
#include "gpio.h"
}

#ifdef USE_TX20_WIND_SENSOR
  #define D_TX2x_NAME "TX20"
#else 
  #define D_TX2x_NAME "TX23"
#endif

#ifdef USE_WEBSERVER
const char HTTP_SNS_TX2X[] PROGMEM =
   "{s}" D_TX2x_NAME " " D_TX20_WIND_SPEED "{m}%s " D_UNIT_KILOMETER_PER_HOUR "{e}"
   "{s}" D_TX2x_NAME " " D_TX20_WIND_SPEED_AVG "{m}%s " D_UNIT_KILOMETER_PER_HOUR "{e}"
   "{s}" D_TX2x_NAME " " D_TX20_WIND_SPEED_MAX "{m}%s " D_UNIT_KILOMETER_PER_HOUR "{e}"
   "{s}" D_TX2x_NAME " " D_TX20_WIND_DIRECTION "{m}%s (%s&deg;){e}";
#endif  // USE_WEBSERVER

const char kTx2xDirections[] PROGMEM = D_TX20_NORTH "|"
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

uint8_t tx2x_sa = 0;
uint8_t tx2x_sb = 0;
uint8_t tx2x_sd = 0;
uint8_t tx2x_se = 0;
uint16_t tx2x_sc = 0;
uint16_t tx2x_sf = 0;

float tx2x_wind_speed_kmh = 0;
float tx2x_wind_speed_max = 0;
float tx2x_wind_speed_avg = 0;
float tx2x_wind_sum = 0;
int tx2x_count = 0;
uint8_t tx2x_wind_direction = 0;

bool tx2x_available = false;

#ifdef USE_TX23_WIND_SENSOR
uint8_t tx23_stage = 0;
#endif

#ifndef ARDUINO_ESP8266_RELEASE_2_3_0      // Fix core 2.5.x ISR not in IRAM Exception
void TX2xStartRead(void) ICACHE_RAM_ATTR;  // As iram is tight and it works this way too
#endif  // ARDUINO_ESP8266_RELEASE_2_3_0

void TX2xStartRead(void)
{
  /**
   * La Crosse TX20 Anemometer datagram every 2 seconds
   * 0-0 11011 0011 111010101111 0101 1100 000101010000 0-0 - Received pin data at 1200 uSec per bit
   *     sa    sb   sc           sd   se   sf
   *     00100 1100 000101010000 1010 1100 000101010000     - sa to sd inverted user data, LSB first
   * sa - Start frame (invert) 00100
   * sb - Wind direction (invert) 0 - 15
   * sc - Wind speed 0 (invert) - 511
   * sd - Checksum (invert)
   * se - Wind direction 0 - 15
   * sf - Wind speed 0 - 511
   * 
   * La Crosse TX23 Anemometer datagram after setting TxD to low/high
   * 1-1 0 1 0-0 11011 0011 111010101111 0101 1100 000101010000 1-1 - Received pin data at 1200 uSec per bit
   *     t s c   sa    sb   sc           sd   se   sf
   *     1 0 1-1 00100 1100 000101010000 1010 1100 000101010000     - sa to sd inverted user data, LSB first
   * t  - host pulls TxD low - signals TX23 to sent measurement
   * s  - TxD released - TxD is pulled high due to pullup
   * c  - TX23U pulls TxD low - calculation in progress
   * sa - Start frame 11011
   * sb - Wind direction 0 - 15
   * sc - Wind speed 0 - 511
   * sd - Checksum
   * se - Wind direction (invert) 0 - 15
   * sf - Wind speed (invert) 0 - 511
   */
#ifdef USE_TX23_WIND_SENSOR
  if (0!=tx23_stage)
  {
    if ((2==tx23_stage) || (3==tx23_stage))
    {
#endif
      tx2x_available = false;

      tx2x_sa = 0;
      tx2x_sb = 0;
      tx2x_sd = 0;
      tx2x_se = 0;
      tx2x_sc = 0;
      tx2x_sf = 0;

      delayMicroseconds(TX2X_BIT_TIME / 2);

      for (int32_t bitcount = 41; bitcount > 0; bitcount--) {
        uint8_t dpin = (digitalRead(pin[GPIO_TX2X_TXD_BLACK]));
#ifdef USE_TX23_WIND_SENSOR
        if (bitcount > 41 - 5) {
          // start
          tx2x_sa = (tx2x_sa << 1) | (dpin);
        } else if (bitcount > 41 - 5 - 4) {
          // wind dir
          tx2x_sb = tx2x_sb >> 1 | ((dpin) << 3);
        } else if (bitcount > 41 - 5 - 4 - 12) {
          // windspeed
          tx2x_sc = tx2x_sc >> 1 | ((dpin) << 11);
        } else if (bitcount > 41 - 5 - 4 - 12 - 4) {
          // checksum
          tx2x_sd = tx2x_sd >> 1 | ((dpin) << 3);
        } else if (bitcount > 41 - 5 - 4 - 12 - 4 - 4) {
          // wind dir (invert)
          tx2x_se = tx2x_se >> 1 | ((dpin ^ 1) << 3);
        } else {
          // windspeed (invert)
          tx2x_sf = tx2x_sf >> 1 | ((dpin ^ 1) << 11);
        }
#else
        if (bitcount > 41 - 5) {
          // start frame (invert)
          tx2x_sa = (tx2x_sa << 1) | (dpin ^ 1);
        } else if (bitcount > 41 - 5 - 4) {
          // wind dir (invert)
          tx2x_sb = tx2x_sb >> 1 | ((dpin ^ 1) << 3);
        } else if (bitcount > 41 - 5 - 4 - 12) {
          // windspeed (invert)
          tx2x_sc = tx2x_sc >> 1 | ((dpin ^ 1) << 11);
        } else if (bitcount > 41 - 5 - 4 - 12 - 4) {
          // checksum (invert)
          tx2x_sd = tx2x_sd >> 1 | ((dpin ^ 1) << 3);
        } else if (bitcount > 41 - 5 - 4 - 12 - 4 - 4) {
          // wind dir
          tx2x_se = tx2x_se >> 1 | (dpin << 3);
        } else {
          // windspeed
          tx2x_sf = tx2x_sf >> 1 | (dpin << 11);
        }
#endif
        delayMicroseconds(TX2X_BIT_TIME);
      }

      uint8_t chk = (tx2x_sb + (tx2x_sc & 0xf) + ((tx2x_sc >> 4) & 0xf) + ((tx2x_sc >> 8) & 0xf));
      chk &= 0xf;

#ifdef USE_TX23_WIND_SENSOR
      // check checksum, non-inverted with inverted values and max. speed
      if ((chk == tx2x_sd) && (tx2x_sb==tx2x_se) && (tx2x_sc==tx2x_sf) && (tx2x_sc < 511)) {
        tx2x_available = true;
      }
#else
      if ((chk == tx2x_sd) && (tx2x_sc < 511)) {  // if checksum seems to be ok and wind speed below 51.1 m/s
        tx2x_available = true;
      }
#endif
#ifdef USE_TX23_WIND_SENSOR
    }
    tx23_stage++;
  }
#endif

  // Must clear this bit in the interrupt register,
  // it gets set even when interrupts are disabled
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, 1 << pin[GPIO_TX2X_TXD_BLACK]);
}

void Tx2xReset(void) 
{
    tx2x_count = 0;
    tx2x_wind_sum = 0;
    tx2x_wind_speed_max = 0;
}

void Tx2xRead(void)
{
#ifdef USE_TX23_WIND_SENSOR
  // note: TX23 speed calculation is unstable when conversion starts 
  //       less than 2 seconds after last request
  if ((uptime % 3)==0) {
    // TX23 start transmission by pulling down TxD line for at minimum 500ms
    // so we pull TxD signal to low every 3 seconds
    tx23_stage = 0;
    pinMode(pin[GPIO_TX2X_TXD_BLACK], OUTPUT);
    digitalWrite(pin[GPIO_TX2X_TXD_BLACK], LOW);
  } else if ((uptime % 3)==1) {
    // after pulling down TxD every 3 second we pull-up TxD every 3+1 seconds
    // to trigger start transmission
    tx23_stage = 1; // first rising signal is invalid
    pinMode(pin[GPIO_TX2X_TXD_BLACK], INPUT_PULLUP);
  }
#endif
  if (0==Settings.tele_period && !(uptime % TX2X_RESET_VALUES)) {
    Tx2xReset();
  }
  else if (tx2x_available) {
    // Wind speed spec: 0 to 180 km/h (0 to 50 m/s)
    tx2x_wind_speed_kmh = float(tx2x_sc) * 0.36;
    if (tx2x_wind_speed_kmh > tx2x_wind_speed_max) {
      tx2x_wind_speed_max = tx2x_wind_speed_kmh;
    }
    tx2x_count++;
    tx2x_wind_sum += tx2x_wind_speed_kmh;
    tx2x_wind_speed_avg = tx2x_wind_sum / tx2x_count;
    tx2x_wind_direction = tx2x_sb;
  }
}

void Tx2xInit(void) 
{
#ifdef USE_TX23_WIND_SENSOR
  tx23_stage = 0;
  pinMode(pin[GPIO_TX2X_TXD_BLACK], OUTPUT);
  digitalWrite(pin[GPIO_TX2X_TXD_BLACK], LOW);
#else
  pinMode(pin[GPIO_TX2X_TXD_BLACK], INPUT);
#endif
  attachInterrupt(pin[GPIO_TX2X_TXD_BLACK], TX2xStartRead, RISING);
}

void Tx2xShow(bool json)
{
  char wind_speed_string[33];
  dtostrfd(tx2x_wind_speed_kmh, 1, wind_speed_string);
  char wind_speed_max_string[33];
  dtostrfd(tx2x_wind_speed_max, 1, wind_speed_max_string);
  char wind_speed_avg_string[33];
  dtostrfd(tx2x_wind_speed_avg, 1, wind_speed_avg_string);
  char wind_direction_string[4];
  GetTextIndexed(wind_direction_string, sizeof(wind_direction_string), tx2x_wind_direction, kTx2xDirections);
  char wind_direction_degree[33];
  dtostrfd(tx2x_wind_direction*22.5, 1, wind_direction_degree);

  if (json) {
    ResponseAppend_P(PSTR(",\"" D_TX2x_NAME "\":{\"Speed\":%s,\"SpeedAvg\":%s,\"SpeedMax\":%s,\"Direction\":\"%s\",\"Degree\":%s}"),
      wind_speed_string, wind_speed_avg_string, wind_speed_max_string, wind_direction_string, wind_direction_degree);
    Tx2xReset();
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_TX2X, wind_speed_string, wind_speed_avg_string, wind_speed_max_string, wind_direction_string, wind_direction_degree);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns35(uint8_t function)
{
  bool result = false;

  if (pin[GPIO_TX2X_TXD_BLACK] < 99) {
    switch (function) {
      case FUNC_INIT:
        Tx2xInit();
        break;
      case FUNC_EVERY_SECOND:
        Tx2xRead();
        break;
      case FUNC_JSON_APPEND:
        Tx2xShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Tx2xShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_TX20_WIND_SENSOR || USE_TX23_WIND_SENSOR