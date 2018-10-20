/*
  xsns_91_Tx20.ino - Tx20 wind sensor
  support for Sonoff-Tasmota

  Copyright (C) 2018  Thomas Eckerstorfer and Theo Arends
  based on https://github.com/bunnyhu/ESP8266_TX20_wind_sensor/
  http://blog.bubux.de/windsensor-tx20-mit-esp8266/

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

/*********************************************************************************************\
 * Tx20 wind sensor
 \*********************************************************************************************/
#ifdef USE_TX20_wind_sensor

#ifdef USE_WEBSERVER

const char HTTP_SNS_TX20[] PROGMEM = "%s"
   "{s} " D_TX20_WIND_SPEED_AVG "{m}%s " D_UNIT_KMH "{e}"
   "{s} " D_TX20_WIND_SPEED_MAX "{m}%s " D_UNIT_KMH "{e}"
   "{s} " D_TX20_WIND_DIRECTION "{m}%s {e}";

#endif  // USE_WEBSERVER

const char TX20_Directions[16][4] = {
    {D_TX20_NORTH},      {D_TX20_NORTH_NORTH_EAST},
    {D_TX20_NORTH_EAST}, {D_TX20_EAST_NORTH_EAST},
    {D_TX20_EAST},       {D_TX20_EAST_SOUTH_EAST},
    {D_TX20_SOUTH_EAST}, {D_TX20_SOUTH_SOUTH_EAST},
    {D_TX20_SOUTH},      {D_TX20_SOUTH_SOUTH_WEST},
    {D_TX20_SOUTH_WEST}, {D_TX20_WEST_SOUTH_WEST},
    {D_TX20_WEST},       {D_TX20_NORTH_NORTH_WEST},
    {D_TX20_NORTH_WEST}, {D_TX20_NORTH_NORTH_WEST}};

unsigned char chk;
unsigned char sa, sb, sd, se;
unsigned int sc, sf, dpin;
boolean validData = false;
boolean showData = false;
float wind_speed_max = 0;
float wind_sum = 0;
float count = 0;
float wind_speed_kmh = 0;

boolean readTX20() {
  int bitcount = 0;

  sa = sb = sd = se = 0;
  sc = 0;
  sf = 0;
  String tx20RawDataS = "";
  // delayMicroseconds(2000);

  for (bitcount = 41; bitcount > 0; bitcount--) {
    dpin = (digitalRead(pin[GPIO_TX20_TXD_BLACK]));
    if (!dpin) {
      tx20RawDataS += "1";
    } else {
      tx20RawDataS += "0";
    }
    if ((bitcount == 41 - 4) || (bitcount == 41 - 8) || (bitcount == 41 - 20) ||
        (bitcount == 41 - 24) || (bitcount == 41 - 28)) {
      tx20RawDataS += " ";
    }
    if (bitcount > 41 - 5) {
      // start, inverted
      sa = (sa << 1) | (dpin ^ 1);
    } else if (bitcount > 41 - 5 - 4) {
      // wind dir, inverted
      sb = sb >> 1 | ((dpin ^ 1) << 3);
    } else if (bitcount > 41 - 5 - 4 - 12) {
      // windspeed, inverted
      sc = sc >> 1 | ((dpin ^ 1) << 11);
    } else if (bitcount > 41 - 5 - 4 - 12 - 4) {
      // checksum, inverted
      sd = sd >> 1 | ((dpin ^ 1) << 3);
    } else if (bitcount > 41 - 5 - 4 - 12 - 4 - 4) {
      // wind dir
      se = se >> 1 | (dpin << 3);
    } else {
      // windspeed
      sf = sf >> 1 | (dpin << 11);
    }

    delayMicroseconds(1220);
  }
  chk = (sb + (sc & 0xf) + ((sc >> 4) & 0xf) + ((sc >> 8) & 0xf));
  chk &= 0xf;

  if ((chk == sd) &&
      (sc < 400)) {  // if checksum seems to be ok and wind speed below 40 m/s
    return true;
  } else {
    return false;
  }
}

void Tx20Init() {
  if (pin[GPIO_TX20_TXD_BLACK] < 99) {
    AddLog(LOG_LEVEL_DEBUG);
    pinMode(pin[GPIO_TX20_TXD_BLACK], INPUT);
  }
}

void Tx20Read() {
  if (digitalRead(pin[GPIO_TX20_TXD_BLACK]) == 1) {
    validData = readTX20();
    if (validData) {
      count++;
      float dir_now = float(sb) * 22.5;
      float kmph = 0.36;
      wind_speed_kmh = kmph * float(sc);
      if (wind_speed_kmh > wind_speed_max) {
        wind_speed_max = wind_speed_kmh;
      }
      wind_sum += wind_speed_kmh;
      showData = true;
    }
  }
}

void Tx20Show(boolean json) {
  char wind_speed_string[10];
  char wind_speed_max_string[10];
  char wind_speed_avg_string[10];
  const char *wind_direction;

  if (showData) {
    wind_direction = TX20_Directions[sb];
    dtostrf(wind_speed_kmh, 1, 2, wind_speed_string);
    dtostrf(wind_speed_max, 1, 2, wind_speed_max_string);

    snprintf_P(log_data, sizeof(log_data), PSTR("Wind direction: %s"),
               wind_direction);
    AddLog(LOG_LEVEL_DEBUG);
    snprintf_P(log_data, sizeof(log_data), PSTR("Wind max: %s km/h"),
               wind_speed_max_string);
    AddLog(LOG_LEVEL_DEBUG);
    snprintf_P(log_data, sizeof(log_data), PSTR("Wind speed: %s km/h"),
               wind_speed_string);
    AddLog(LOG_LEVEL_DEBUG);
    dtostrf(wind_sum / count, 1, 2, wind_speed_avg_string);
    snprintf_P(log_data, sizeof(log_data), PSTR("Wind avg: %s km/h"),
               wind_speed_avg_string);
    AddLog(LOG_LEVEL_DEBUG);

    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data),
                 PSTR("%s,\"TX20\":{\"SPEED_AVG:\":%s, \"SPEED_MAX\":%s, "
                      "\"DIRECTION\":\"%s\"}"),
                 mqtt_data, wind_speed_avg_string, wind_speed_max_string,
                 wind_direction);
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TX20,
                 mqtt_data, wind_speed_avg_string, wind_speed_max_string, wind_direction);

#endif  // USE_WEBSERVER
    }

    // showData = false;
    count = 0;
    wind_sum = 0;
    wind_speed_max = 0;
  }
}

  /*********************************************************************************************\
   * Interface
   \*********************************************************************************************/

#define XSNS_91

boolean Xsns91(byte function) {
  boolean result = false;
  switch (function) {
    case FUNC_INIT:
      Tx20Init();
      break;
    case FUNC_LOOP:
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
  return result;
}
#endif  // USE_TX20_wind_sensor