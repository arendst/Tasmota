/*
  xsns_96_ws303u.ino - Sentec WS303U ultrasonic wind speed and direction 
  support for Tasmota

  Copyright (C) 2022  Marius Bezuidenhout

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

#ifdef USE_WS303U
/*********************************************************************************************\
 * Sentec WS303U with RS485 interface
 *
 * https://www.cdsentec.com/productinfo/639389.html
 *********************************************************************************************
 * Connections:
 * _____________________________________________________________
 *         | Line color   | Description
 * -------------------------------------------------------------
 * Power   | brown        | Positive power supply (10 ~ 30V DC)
 *         | black        | Power supply negative (GND)
 * -------------------------------------------------------------
 * Data    | yellow       | 485-A
 *         | blue         | 485-B
 * -------------------------------------------------------------
 * 
\*********************************************************************************************/

#define XSNS_96                      96

// can be user defined in my_user_config.h
#ifndef WS303U_SPEED
  #define WS303U_SPEED      4800    // default WS303U Modbus baud
#endif
// can be user defined in my_user_config.h
#ifndef WS303U_ADDR
  #define WS303U_ADDR       1       // default WS303U Modbus address
#endif

#include <TasmotaModbus.h>
TasmotaModbus *Ws303uModbus;

// 0x0000    ReadOnly     Real time value of wind speed (expanded by 100 times)
// 0x0001    ReadOnly     Real time value of wind direction (integer, 0 ° due north, 90 ° due east)
// 0x0002    ReadOnly     Maximum wind speed after power on of equipment (expanded by 100 times)
// 0x0003    ReadOnly     Wind level corresponding to the current wind speed (integer, 0-17)
// 0x07D0    ReadWrite    1 ~ 254 (factory default 1)
// 0x07D1    ReadWrite    0 = 2400, 1 = 4800(factory default), 2 = 9600

struct WS303U {
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
  uint16_t real_wind_speed = 0;
  uint16_t wind_direction = 0;
  uint16_t max_wind_speed = 0;
  uint16_t wind_power = 0;
} Ws303u;

/*********************************************************************************************/

void WS303UEvery250ms(void)
{
  bool data_ready = Ws303uModbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[9];

    uint8_t error = Ws303uModbus->ReceiveBuffer(buffer, 8);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, Ws303uModbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("WS303U: WS303U error %d"), error);
    } else {
      Ws303u.real_wind_speed  = buffer[0];
      Ws303u.real_wind_speed |= buffer[1] << 8;
      Ws303u.wind_direction   = buffer[2];
      Ws303u.wind_direction  |= buffer[3] << 8;
      Ws303u.max_wind_speed   = buffer[4];
      Ws303u.max_wind_speed  |= buffer[5] << 8;
      Ws303u.wind_power       = buffer[6];
      Ws303u.wind_power      |= buffer[7] << 8;
    }
  }
  if (0 == Ws303u.send_retry || data_ready) {
    Ws303u.send_retry = 5;
    Ws303uModbus->Send(WS303U_ADDR, 0x03, 0x0000, 4); // Get first 4 registers
  } else {
    Ws303u.send_retry--;
  }
}

void WS303UInit(void)
{
  if (PinUsed(GPIO_WS303U_RX) && PinUsed(GPIO_WS303U_TX)) {
    Ws303uModbus = new TasmotaModbus(Pin(GPIO_WS303U_RX), Pin(GPIO_WS303U_TX));
    uint8_t result = Ws303uModbus->Begin(WS303U_SPEED);
    if (result) {
      if (2 == result) { ClaimSerial(); }
    }
  }
}

void WS303UShow(bool json)
{

}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns96(uint8_t function)
{
  bool result = false;
  
  switch (function) {
    case FUNC_INIT:
      WS303UInit();
      break;
    case FUNC_EVERY_250_MSECOND:
      WS303UEvery250ms();
      break;
    case FUNC_JSON_APPEND:
      WS303UShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      WS303UShow(0);
      break;
#endif  // USE_WEBSERVER
  }

  return result;
}

#endif  // USE_WS303U
