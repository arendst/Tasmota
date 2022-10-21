/*
  xsns_84_tof10120.ino - TOF10120 sensor support for Tasmota

  Copyright (C) 2021  Cyril Pawelko and Theo Arends

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
#ifdef USE_TOF10120
/*********************************************************************************************\
 * TOF10120 I2C Mode (Serial not supported)
 *
 * I2C Address: 0x52
 *
 * Connections:
 * 1 - Black   Gnd
 * 2 - Red     Vcc
 * 3 - Yellow  Rxd - Not supported
 * 4 - White   Txd - Not supported
 * 5 - Blue    I2C SDA
 * 6 - Green   I2C SCL
\*********************************************************************************************/

#define XSNS_84                     84
#define XI2C_57                     57  // See I2CDEVICES.md

#define TOF10120_ADDRESS            0x52
#define TOF10120_INCORRECT_DISTANCE 0xFFFF

struct {
  uint16_t distance = 0;
  bool ready = false;
} tof10120_sensor;

/********************************************************************************************/

uint16_t Tof10120Read() {
  uint16_t distance;

  Wire.beginTransmission(TOF10120_ADDRESS);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(TOF10120_ADDRESS, 2);
  if (Wire.available() == 2) {
    distance = Wire.read();
    distance = distance <<8;
    distance |= Wire.read();
  } else {  // Incorrect value read
    AddLog(LOG_LEVEL_DEBUG, "TOF: No value read");
    return TOF10120_INCORRECT_DISTANCE;
  }
  AddLog(LOG_LEVEL_DEBUG_MORE, "TOF: Distance %u", distance);
  return distance;
}

void Tof10120Detect(void) {
  if (!I2cSetDevice(TOF10120_ADDRESS)) { return; }
  if (Tof10120Read() == TOF10120_INCORRECT_DISTANCE) { return; }

  I2cSetActiveFound(TOF10120_ADDRESS, "TOF10120");
  tof10120_sensor.ready = true;
}

void Tof10120Every_250MSecond(void) {
  uint16_t dist = Tof10120Read();
//  if ( dist < 400000) {
//    dist = TOF10120_INCORRECT_DISTANCE;
//  }
  tof10120_sensor.distance = dist;
}

#ifdef USE_DOMOTICZ
void Tof10120Every_Second(void) {
  char distance[FLOATSZ];
  dtostrfd((float)tof10120_sensor.distance / 10, 1, distance);
  DomoticzSensor(DZ_ILLUMINANCE, distance);
}
#endif  // USE_DOMOTICZ

void Tof10120Show(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"TOF10120\":{\"" D_JSON_DISTANCE "\":%d}"), tof10120_sensor.distance);
#ifdef USE_DOMOTICZ
    if (0 == TasmotaGlobal.tele_period) {
      Tof10120Every_Second();
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_DISTANCE, PSTR("TOF10120"), tof10120_sensor.distance);
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns84(uint8_t function) {
  if (!I2cEnabled(XI2C_57)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Tof10120Detect();
  }
  else if (tof10120_sensor.ready) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        Tof10120Every_250MSecond();
        break;
#ifdef USE_DOMOTICZ
     case FUNC_EVERY_SECOND:
        Tof10120Every_Second();
        break;
#endif  // USE_DOMOTICZ
      case FUNC_JSON_APPEND:
        Tof10120Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Tof10120Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_TOF10120
#endif  // USE_I2C