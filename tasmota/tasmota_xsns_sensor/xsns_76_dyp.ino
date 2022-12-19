/*
  xsns_76_dyp.ino - DYP ME007 serial interface

  Copyright (C) 2021  Janusz Kostorz

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

#ifdef USE_DYP
/*********************************************************************************************\
 * DYP ME007 ultrasonic distance sensor (300...4000mm), serial version
 *
 * Every one second code check last received value from sensor via serial port and return:
 *     -1 for checksum error
 *      0 for distance below 300mm
 *      300...4000 for measured distance
 *      4999 for distance above 4000mm
 *      5999 for not connected sensor
\*********************************************************************************************/

#define XSNS_76     76

#include <TasmotaSerial.h>
TasmotaSerial *DYPSerial = nullptr;

#define DYP_CRCERROR -1
#define DYP_BELOWMIN 0
#define DYP_MIN 300
#define DYP_MAX 4000
#define DYP_ABOVEMAX 4999
#define DYP_NOSENSOR 5999

uint16_t DYPDistance = 0;   // distance in millimeters

/*********************************************************************************************/

void DYPInit(void) {
  if (PinUsed(GPIO_DYP_RX)) {
    DYPSerial = new TasmotaSerial(Pin(GPIO_DYP_RX), -1, 1);
    if (DYPSerial->begin(9600)) {
      if (DYPSerial->hardwareSerial()) {
        ClaimSerial();
      }
    }
  }
}

void DYPEverySecond(void) {
  // check for serial data
  if (DYPSerial->available() < 6) {
    DYPDistance = DYP_NOSENSOR;
    return;
  }

  // trash old data (only 7 last bytes are needed for calculate distance)
  while (DYPSerial->available() > 7) {
    DYPSerial->read();
  }

  // read data from serial port - * 0xFF MSB LSB CRC *
  while (DYPSerial->available() > 3) {
    // check for start byte signature
    if (DYPSerial->read() != 0xFF) {
      continue;
    }

    // check for data bytes
    if (DYPSerial->available() > 2) {
      uint8_t msb = DYPSerial->read();
      uint8_t lsb = DYPSerial->read();
      if (((uint16_t)(0xFF + msb + lsb) & 0xFF) == DYPSerial->read()) {
        uint16_t data = (msb << 8) | lsb;
        if (data < DYP_MIN) {
          data = DYP_BELOWMIN;
        }
        if (data > DYP_MAX) {
          data = DYP_ABOVEMAX;
        }
        DYPDistance = data;  // mm
      } else {
        DYPDistance = DYP_CRCERROR;
      }
    }
  }
}

void DYPShow(bool json) {
  char types[4] = "DYP";
  float distance = (float)DYPDistance / 10;  // cm
  if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_DISTANCE "\":%1_f}"), types, &distance);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_F_DISTANCE_CM, types, &distance);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns76(uint32_t function) {
  if (FUNC_INIT == function) {
    DYPInit();
  }
  else if (DYPSerial) {
    switch (function) {
      case FUNC_EVERY_SECOND:
          DYPEverySecond();
          break;
      case FUNC_JSON_APPEND:
          DYPShow(1);
          break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
          DYPShow(0);
          break;
#endif  // USE_WEBSERVER
    }
  }
  return false;
}

#endif  // USE_DYP
