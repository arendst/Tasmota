/*
  xsns_78_ezorgb.ino - EZO RGB I2C RGB sensor support for Tasmota

  Copyright (C) 2021  Christopher Tremblay

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
#ifdef USE_EZORGB

#define EZO_RGB_READ_LATENCY   400

struct EZORGB : public EZOStruct {
  EZORGB(uint32_t addr) : EZOStruct(addr), R(-1), G(-1), B(-6), Lux(0), hasLux(false) {}

  virtual void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];
    char *next;
    // R, G, B, Lux

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_RGB_READ_LATENCY);

    R = atoi(data);
    next = strchr(data, ',') + 1;
    G = atoi(next);
    next = strchr(next, ',') + 1;
    B = atoi(next);

    next    = strstr_P(next, PSTR(",Lux"));
    hasLux  = (next != nullptr);
    if (hasLux) {
      Lux = atoi(next + sizeof(",Lux") - 1);
    }
  }

  virtual void Show(bool json, const char *name)
  {
    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_RED "\":%d,\"" D_JSON_GREEN "\":%d,\"" D_JSON_BLUE "\":%d"), name, R, G, B);
      if (hasLux) {
        ResponseAppend_P(PSTR(",\"" D_JSON_ILLUMINANCE "\":%d"), Lux);
      }
      ResponseJsonEnd();

#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_COLOR_RED,   name, R);
      WSContentSend_PD(HTTP_SNS_COLOR_GREEN, name, G);
      WSContentSend_PD(HTTP_SNS_COLOR_BLUE,  name, B);

      if (hasLux) {
        WSContentSend_PD(HTTP_SNS_ILLUMINANCE, name, Lux);
      }
#endif
    }
  }

  static const char id[] PROGMEM;

private:
  uint16_t  R, G, B;
  uint16_t  Lux;
  bool      hasLux;
};

const char EZORGB::id[] PROGMEM = "RGB";

#endif  // USE_EZORGB
#endif  // USE_I2C
