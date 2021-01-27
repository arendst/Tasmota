/*
  xsns_35_Tx20.ino - La Crosse Tx20/Tx23 wind sensor support for Tasmota

  Copyright (C) 2021  Thomas Eckerstorfer, Norbert Richter and Theo Arends

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
 *
 * Reads speed and direction
 *
 * Calculate statistics:
 *  speed avg/min/max
 *  direction avg/min/max/range
 *
 * avg values are updated continuously (using exponentially weighted average)
 * min/max/range values are reset after TelePeriod time or TX2X_WEIGHT_AVG_SAMPLE seconds
 * (if TelePeriod is disabled)
 *
 * Statistic calculation can be disabled by defining USE_TX2X_WIND_SENSOR_NOSTATISTICS
 * (saves 1k8)
\*********************************************************************************************/

#define XSNS_35                  35

#if defined(USE_TX20_WIND_SENSOR) && defined(USE_TX23_WIND_SENSOR)
#undef USE_TX20_WIND_SENSOR
#warning **** use USE_TX20_WIND_SENSOR or USE_TX23_WIND_SENSOR but not both together, TX20 disabled ****
#endif // USE_TX20_WIND_SENSOR && USE_TX23_WIND_SENSOR

// #define USE_TX2X_WIND_SENSOR_NOSTATISTICS        // suppress statistics (speed/dir avg/min/max/range)
#define TX2X_BIT_TIME          1220  // microseconds
#define TX2X_WEIGHT_AVG_SAMPLE  150  // seconds
#define TX2X_TIMEOUT             10  // seconds
#define TX23_READ_INTERVAL        4  // seconds (don't use less than 3)

// The Arduino standard GPIO routines are not enough,
// must use some from the Espressif SDK as well
extern "C" {
#include "gpio.h"
}

#ifdef USE_TX20_WIND_SENSOR
#undef D_TX2x_NAME
#define D_TX2x_NAME "TX20"
#else  //  USE_TX20_WIND_SENSOR
#undef D_TX2x_NAME
#define D_TX2x_NAME "TX23"
#endif  //  USE_TX20_WIND_SENSOR

#ifdef USE_WEBSERVER
#define D_TX20_WIND_AVG "&empty;"
#define D_TX20_WIND_ANGLE "&ang;"
#define D_TX20_WIND_DEGREE "&deg;"
const char HTTP_SNS_TX2X[] PROGMEM =
   "{s}" D_TX2x_NAME " " D_TX20_WIND_SPEED "{m}%1_f %s{e}"
#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
   "{s}" D_TX2x_NAME " " D_TX20_WIND_SPEED " " D_TX20_WIND_AVG "{m}%1_f %s{e}"
   "{s}" D_TX2x_NAME " " D_TX20_WIND_SPEED_MIN "{m}%1_f %s{e}"
   "{s}" D_TX2x_NAME " " D_TX20_WIND_SPEED_MAX "{m}%1_f %s{e}"
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
   "{s}" D_TX2x_NAME " " D_TX20_WIND_DIRECTION "{m}%s %1_f" D_TX20_WIND_DEGREE "{e}"
#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
   "{s}" D_TX2x_NAME " " D_TX20_WIND_DIRECTION " " D_TX20_WIND_AVG "{m}%s %1_f" D_TX20_WIND_DEGREE "{e}"
   "{s}" D_TX2x_NAME " " D_TX20_WIND_DIRECTION " " D_TX20_WIND_ANGLE "{m}%1_f" D_TX20_WIND_DEGREE " (%1_f,%1_f)" D_TX20_WIND_DEGREE;
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
   ;
#endif  // USE_WEBSERVER

// float saves 48 byte
float const tx2x_f_pi           = 3.1415926535897932384626433;  // Pi
float const tx2x_f_halfpi       = tx2x_f_pi / 2.0;
float const tx2x_f_pi180        = tx2x_f_pi / 180.0;

#define TX2X_DIRECTIONS_MAXSIZE   3
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

int32_t tx2x_wind_speed = 0;
int32_t tx2x_wind_direction = 0;

#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
int32_t tx2x_wind_speed_min = 0xfff;
int32_t tx2x_wind_speed_max = 0;
float tx2x_wind_speed_avg = 0;
float tx2x_wind_direction_avg_x = 0;
float tx2x_wind_direction_avg_y = 0;
float tx2x_wind_direction_avg = 0;
int32_t tx2x_wind_direction_min = 0;
int32_t tx2x_wind_direction_max = 0;

uint32_t tx2x_count = 0;
uint32_t tx2x_avg_samples;
uint32_t tx2x_last_uptime = 0;
bool tx2x_valuesread = false;
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS

#ifdef DEBUG_TASMOTA_SENSOR
uint32_t tx2x_sa = 0;
uint32_t tx2x_sb = 0;
uint32_t tx2x_sc = 0;
uint32_t tx2x_sd = 0;
uint32_t tx2x_se = 0;
uint32_t tx2x_sf = 0;
#endif  // DEBUG_TASMOTA_SENSOR
uint32_t tx2x_last_available = 0;

#ifdef USE_TX23_WIND_SENSOR
uint32_t tx23_stage = 0;
#endif  // USE_TX23_WIND_SENSOR

void ICACHE_RAM_ATTR TX2xStartRead(void)
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
#endif  // USE_TX23_WIND_SENSOR
#ifdef DEBUG_TASMOTA_SENSOR
      tx2x_sa = 0;
      tx2x_sb = 0;
      tx2x_sc = 0;
      tx2x_sd = 0;
      tx2x_se = 0;
      tx2x_sf = 0;
#else  // DEBUG_TASMOTA_SENSOR
      uint32_t tx2x_sa = 0;
      uint32_t tx2x_sb = 0;
      uint32_t tx2x_sc = 0;
      uint32_t tx2x_sd = 0;
      uint32_t tx2x_se = 0;
      uint32_t tx2x_sf = 0;
#endif  // DEBUG_TASMOTA_SENSOR

      delayMicroseconds(TX2X_BIT_TIME / 2);

      for (int32_t bitcount = 41; bitcount > 0; bitcount--) {
        uint32_t dpin = (digitalRead(Pin(GPIO_TX2X_TXD_BLACK)));
#ifdef USE_TX23_WIND_SENSOR
        dpin ^= 1;
#endif  // USE_TX23_WIND_SENSOR
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
        delayMicroseconds(TX2X_BIT_TIME);
      }

      uint32_t chk = (tx2x_sb + (tx2x_sc & 0xf) + ((tx2x_sc >> 4) & 0xf) + ((tx2x_sc >> 8) & 0xf));
      chk &= 0xf;

      // check checksum, start frame,non-inverted==inverted values and max. speed
      ;
#ifdef USE_TX23_WIND_SENSOR
      if ((chk == tx2x_sd) && (0x1b==tx2x_sa) && (tx2x_sb==tx2x_se) && (tx2x_sc==tx2x_sf) && (tx2x_sc < 511)) {
#else
      if ((chk == tx2x_sd) && (tx2x_sb==tx2x_se) && (tx2x_sc==tx2x_sf) && (tx2x_sc < 511)) {
#endif
        tx2x_last_available = TasmotaGlobal.uptime;
        // Wind speed spec: 0 to 180 km/h (0 to 50 m/s)
        tx2x_wind_speed = tx2x_sc;
        tx2x_wind_direction = tx2x_sb;
#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
        if (!tx2x_valuesread) {
          tx2x_wind_direction_min = tx2x_wind_direction;
          tx2x_wind_direction_max = tx2x_wind_direction;
          tx2x_valuesread = true;
        }
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
      }

#ifdef USE_TX23_WIND_SENSOR
    }
    tx23_stage++;
  }
#endif  // USE_TX23_WIND_SENSOR

  // Must clear this bit in the interrupt register,
  // it gets set even when interrupts are disabled
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, 1 << Pin(GPIO_TX2X_TXD_BLACK));
}

bool Tx2xAvailable(void)
{
  return ((TasmotaGlobal.uptime - tx2x_last_available) < TX2X_TIMEOUT);
}

#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
float atan2f(float a, float b)
{
    float atan2val;
    if (b > 0) {
        atan2val = atanf(a/b);
    } else if ((b < 0) && (a >= 0)) {
        atan2val = atanf(a/b) + tx2x_f_pi;
    } else if ((b < 0) && (a < 0)) {
        atan2val = atanf(a/b) - tx2x_f_pi;
    } else if ((b == 0) && (a > 0)) {
        atan2val = tx2x_f_halfpi;
    } else if ((b == 0) && (a < 0)) {
        atan2val = 0 - (tx2x_f_halfpi);
    } else if ((b == 0) && (a == 0)) {
        atan2val = 1000;               //represents undefined
    }
    return atan2val;
}

void Tx2xCheckSampleCount(void)
{
  uint32_t tx2x_prev_avg_samples = tx2x_avg_samples;
  if (Settings.tele_period) {
    // number for avg samples = teleperiod value if set
    tx2x_avg_samples = Settings.tele_period;
  } else {
    // otherwise use default number of samples for this driver
    tx2x_avg_samples = TX2X_WEIGHT_AVG_SAMPLE;
  }
  if (tx2x_prev_avg_samples != tx2x_avg_samples) {
    tx2x_wind_speed_avg = tx2x_wind_speed;
    tx2x_count = 0;
  }
}

void Tx2xResetStat(void)
{
  DEBUG_SENSOR_LOG(PSTR(D_TX2x_NAME ": reset statistics"));
  tx2x_last_uptime = TasmotaGlobal.uptime;
  Tx2xResetStatData();
}

void Tx2xResetStatData(void)
{
  tx2x_wind_speed_min = tx2x_wind_speed;
  tx2x_wind_speed_max = tx2x_wind_speed;

  tx2x_wind_direction_min = tx2x_wind_direction;
  tx2x_wind_direction_max = tx2x_wind_direction;
}
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS

void Tx2xRead(void)
{
#ifdef USE_TX23_WIND_SENSOR
  // TX23 needs to trigger start transmission - TxD Line
  // ___________      _             ___   ___
  //            |____| |___________|   |_|   |__XXXXXXXXXX
  //           trigger  start conv  Startframe  Data
  //
  // note: TX23 speed calculation is unstable when conversion starts
  //       less than 2 seconds after last request
  if ((TasmotaGlobal.uptime % TX23_READ_INTERVAL)==0) {
    // TX23 start transmission by pulling down TxD line for at minimum 500ms
    // so we pull TxD signal to low every 3 seconds
    tx23_stage = 0;
    pinMode(Pin(GPIO_TX2X_TXD_BLACK), OUTPUT);
    digitalWrite(Pin(GPIO_TX2X_TXD_BLACK), LOW);
  } else if ((TasmotaGlobal.uptime % TX23_READ_INTERVAL)==1) {
    // after pulling down TxD: pull-up TxD every x+1 seconds
    // to trigger TX23 start transmission
    tx23_stage = 1; // first rising signal is invalid
    pinMode(Pin(GPIO_TX2X_TXD_BLACK), INPUT_PULLUP);
  }
#endif  // USE_TX23_WIND_SENSOR
  if (Tx2xAvailable()) {
#ifdef DEBUG_TASMOTA_SENSOR
    DEBUG_SENSOR_LOG(PSTR(D_TX2x_NAME ": sa=0x%02lx sb=%ld (0x%02lx), sc=%ld (0x%03lx), sd=0x%02lx, se=%ld, sf=%ld"), tx2x_sa,tx2x_sb,tx2x_sb,tx2x_sc,tx2x_sc,tx2x_sd,tx2x_se,tx2x_sf);
#endif  // DEBUG_TASMOTA_SENSOR
#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
    if (tx2x_wind_speed < tx2x_wind_speed_min) {
      tx2x_wind_speed_min = tx2x_wind_speed;
    }
    if (tx2x_wind_speed > tx2x_wind_speed_max) {
      tx2x_wind_speed_max = tx2x_wind_speed;
    }

    // exponentially weighted average is not quite as smooth as the arithmetic average
    // but close enough to the moving average and does not require the regular reset
    // of the divider with the associated jump in avg values after period is over
    if (tx2x_count <= tx2x_avg_samples) {
      tx2x_count++;
    }
    tx2x_wind_speed_avg -= tx2x_wind_speed_avg / tx2x_count;
    tx2x_wind_speed_avg += float(tx2x_wind_speed) / tx2x_count;

    tx2x_wind_direction_avg_x -= tx2x_wind_direction_avg_x / tx2x_count;
    tx2x_wind_direction_avg_x += cosf((tx2x_wind_direction*22.5) * tx2x_f_pi180) / tx2x_count;
    tx2x_wind_direction_avg_y -= tx2x_wind_direction_avg_y / tx2x_count;
    tx2x_wind_direction_avg_y += sinf((tx2x_wind_direction*22.5) * tx2x_f_pi180) / tx2x_count;
    tx2x_wind_direction_avg = atan2f(tx2x_wind_direction_avg_y, tx2x_wind_direction_avg_x) * 180.0f / tx2x_f_pi;
    if (tx2x_wind_direction_avg<0.0) {
      tx2x_wind_direction_avg += 360.0;
    }
    if (tx2x_wind_direction_avg>360.0) {
      tx2x_wind_direction_avg -= 360.0;
    }

    int32_t tx2x_wind_direction_avg_int = int((tx2x_wind_direction_avg/22.5)+0.5) % 16;

    // degrees min/max
    if (tx2x_wind_direction > tx2x_wind_direction_avg_int) {
      // clockwise or left-handed rotation
      if ((tx2x_wind_direction-tx2x_wind_direction_avg_int)>8) {
        // diff > 180°
        if ((tx2x_wind_direction - 16) < tx2x_wind_direction_min) {
          // new min (negative values < 0)
          tx2x_wind_direction_min = tx2x_wind_direction - 16;
        }
      } else {
        // diff <= 180°
        if (tx2x_wind_direction > tx2x_wind_direction_max) {
          // new max (origin max)
          tx2x_wind_direction_max = tx2x_wind_direction;
        }
      }
    } else {
      // also clockwise or left-handed rotation but needs other tests
      if ((tx2x_wind_direction_avg_int-tx2x_wind_direction)>8) {
        // diff > 180°
        if ((tx2x_wind_direction + 16) > tx2x_wind_direction_max) {
          // new max (overflow values > 15)
          tx2x_wind_direction_max = tx2x_wind_direction + 16;
        }
      } else {
        // diff <= 180°
        if (tx2x_wind_direction < tx2x_wind_direction_min) {
          // new min (origin min)
          tx2x_wind_direction_min = tx2x_wind_direction;
        }
      }
    }

#ifdef DEBUG_TASMOTA_SENSOR
    DEBUG_SENSOR_LOG(PSTR(D_TX2x_NAME ": dir stat - counter=%ld, actint=%ld, avgint=%ld, avg=%1_f (cosx=%1_f, siny=%1_f), min %d, max %d"),
      (TasmotaGlobal.uptime-tx2x_last_uptime),
      tx2x_wind_direction,
      tx2x_wind_direction_avg_int,
      &tx2x_wind_direction_avg,
      &tx2x_wind_direction_avg_x,
      &tx2x_wind_direction_avg_y,
      tx2x_wind_direction_min,
      tx2x_wind_direction_max
    );
#endif  // DEBUG_TASMOTA_SENSOR
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
  } else {
    DEBUG_SENSOR_LOG(PSTR(D_TX2x_NAME ": not available"));
    tx2x_wind_speed = 0;
    tx2x_wind_direction = 0;
#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
    tx2x_wind_speed_avg = 0;
    tx2x_wind_direction_avg = 0;
    Tx2xResetStatData();
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
  }

#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
  Tx2xCheckSampleCount();
  if (0==Settings.tele_period && (TasmotaGlobal.uptime-tx2x_last_uptime)>=tx2x_avg_samples) {
    Tx2xResetStat();
  }
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
}

void Tx2xInit(void)
{
  if (!Settings.flag2.speed_conversion) {
    Settings.flag2.speed_conversion = 2;  // 0 = none, 1 = m/s, 2 = km/h, 3 = kn, 4 = mph, 5 = ft/s, 6 = yd/s
  }
#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
  tx2x_valuesread = false;
  Tx2xResetStat();
  Tx2xCheckSampleCount();
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
#ifdef USE_TX23_WIND_SENSOR
  tx23_stage = 0;
  pinMode(Pin(GPIO_TX2X_TXD_BLACK), OUTPUT);
  digitalWrite(Pin(GPIO_TX2X_TXD_BLACK), LOW);
#else  // USE_TX23_WIND_SENSOR
  pinMode(Pin(GPIO_TX2X_TXD_BLACK), INPUT);
#endif // USE_TX23_WIND_SENSOR
  attachInterrupt(Pin(GPIO_TX2X_TXD_BLACK), TX2xStartRead, RISING);
}

int32_t Tx2xNormalize(int32_t value)
{
  while (value>15) {
    value -= 16;
  }
  while (value<0) {
    value += 16;
  }
  return value;
}

void Tx2xShow(bool json)
{
  if (!Tx2xAvailable()) { return; }

  float wind_speed_float = ConvertSpeed(tx2x_wind_speed) / 10;
  float wind_direction_float = tx2x_wind_direction * 22.5;
  char wind_direction_cardinal_string[TX2X_DIRECTIONS_MAXSIZE+1];
  GetTextIndexed(wind_direction_cardinal_string, sizeof(wind_direction_cardinal_string), tx2x_wind_direction, kTx2xDirections);
#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
  float wind_speed_min_float = ConvertSpeed(tx2x_wind_speed_min) / 10;
  float wind_speed_max_float = ConvertSpeed(tx2x_wind_speed_max) / 10;
  float wind_speed_avg_float = ConvertSpeed(tx2x_wind_speed_avg) / 10;
  float wind_direction_avg_float = tx2x_wind_direction_avg;
  char wind_direction_avg_cardinal_string[4];
  GetTextIndexed(wind_direction_avg_cardinal_string, sizeof(wind_direction_avg_cardinal_string), int((tx2x_wind_direction_avg/22.5f)+0.5f) % 16, kTx2xDirections);
  float wind_direction_range_float = (tx2x_wind_direction_max-tx2x_wind_direction_min) * 22.5;
  float wind_direction_min_float = Tx2xNormalize(tx2x_wind_direction_min) * 22.5;
  float wind_direction_max_float = tx2x_wind_direction_max * 22.5;
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS

  if (json) {
#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
#ifdef USE_TX2x_LEGACY_JSON
    ResponseAppend_P(PSTR(",\"" D_TX2x_NAME "\":{\"" D_JSON_SPEED "\":%1_f,\"SpeedAvg\":%1_f,\"SpeedMax\":%1_f,\"Direction\":\"%s\",\"Degree\":%1_f}"),
      &wind_speed_float,
      &wind_speed_avg_float,
      &wind_speed_max_float,
      wind_direction_cardinal_string,
      &wind_direction_float
    );
#else  // USE_TX2x_LEGACY_JSON
    ResponseAppend_P(PSTR(",\"" D_TX2x_NAME "\":{\"" D_JSON_SPEED "\":{\"Act\":%1_f,\"Avg\":%1_f,\"Min\":%1_f,\"Max\":%1_f},\"Dir\":{\"Card\":\"%s\",\"Deg\":%1_f,\"Avg\":%1_f,\"AvgCard\":\"%s\",\"Min\":%1_f,\"Max\":%1_f,\"Range\":%1_f}}"),
      &wind_speed_float,
      &wind_speed_avg_float,
      &wind_speed_min_float,
      &wind_speed_max_float,
      wind_direction_cardinal_string,
      &wind_direction_float,
      &wind_direction_avg_float,
      wind_direction_avg_cardinal_string,
      &wind_direction_min_float,
      &wind_direction_max_float,
      &wind_direction_range_float
    );
#endif  // USE_TX2x_LEGACY_JSON
#else  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
#ifdef USE_TX2x_LEGACY_JSON
    ResponseAppend_P(PSTR(",\"" D_TX2x_NAME "\":{\"" D_JSON_SPEED "\":%1_f,\"Direction\":\"%s\",\"Degree\":%1_f}"),
      &wind_speed_float, wind_direction_cardinal_string, &wind_direction_float);
#else  // USE_TX2x_LEGACY_JSON
    ResponseAppend_P(PSTR(",\"" D_TX2x_NAME "\":{\"" D_JSON_SPEED "\":{\"Act\":%1_f},\"Dir\":{\"Card\":\"%s\",\"Deg\":%1_f}}"),
      &wind_speed_float, wind_direction_cardinal_string, &wind_direction_float);
#endif  // USE_TX2x_LEGACY_JSON
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_TX2X,
      &wind_speed_float,
      SpeedUnit().c_str(),
#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
      &wind_speed_avg_float,
      SpeedUnit().c_str(),
      &wind_speed_min_float,
      SpeedUnit().c_str(),
      &wind_speed_max_float,
      SpeedUnit().c_str(),
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
      wind_direction_cardinal_string,
      &wind_direction_float
#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
      ,wind_direction_avg_cardinal_string,
      &wind_direction_avg_float,
      &wind_direction_range_float,
      &wind_direction_min_float,
      &wind_direction_max_float
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
    );
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns35(uint8_t function)
{
  bool result = false;

  if (PinUsed(GPIO_TX2X_TXD_BLACK)) {
    switch (function) {
      case FUNC_INIT:
        Tx2xInit();
        break;
      case FUNC_EVERY_SECOND:
        Tx2xRead();
        break;
#ifndef USE_TX2X_WIND_SENSOR_NOSTATISTICS
      case FUNC_AFTER_TELEPERIOD:
        Tx2xResetStat();
        break;
#endif  // USE_TX2X_WIND_SENSOR_NOSTATISTICS
      case FUNC_JSON_APPEND:
        Tx2xShow(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Tx2xShow(false);
        break;
#endif  // USE_WEBSERVER

    }
  }
  return result;
}

#endif  // USE_TX20_WIND_SENSOR || USE_TX23_WIND_SENSOR