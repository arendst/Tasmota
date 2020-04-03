/*
  xsns_17_senseair.ino - SenseAir CO2 sensor support for Tasmota

  Copyright (C) 2020  Theo Arends

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

#ifdef USE_SENSEAIR
/*********************************************************************************************\
 * SenseAir K30, K70 and S8 - CO2 sensor
 *
 * Adapted from EspEasy plugin P052 by Mikael Trieb (mikael__AT__triebconsulting.se)
 *
 * Hardware Serial will be selected if GPIO1 = [SAir Rx] and GPIO3 = [SAir Tx]
\*********************************************************************************************/

#define XSNS_17                      17

#define SENSEAIR_MODBUS_SPEED        9600
#define SENSEAIR_DEVICE_ADDRESS      0xFE    // Any address
#define SENSEAIR_READ_REGISTER       0x04    // Command Read

#ifndef CO2_LOW
#define CO2_LOW                      800     // Below this CO2 value show green light
#endif
#ifndef CO2_HIGH
#define CO2_HIGH                     1200    // Above this CO2 value show red light
#endif

#include <TasmotaModbus.h>
TasmotaModbus *SenseairModbus;

const char kSenseairTypes[] PROGMEM = "Kx0|S8";

uint8_t senseair_type = 1;
char senseair_types[7];

uint16_t senseair_co2 = 0;
float senseair_temperature = 0;
float senseair_humidity = 0;

//uint8_t senseair_state = 0;

const uint8_t start_addresses[] { 0x1A, 0x00, 0x03, 0x04, 0x05, 0x1C, 0x0A };

uint8_t senseair_read_state = 0;
uint8_t senseair_send_retry = 0;

void Senseair250ms(void)              // Every 250 mSec
{
//  senseair_state++;
//  if (6 == senseair_state) {     // Every 300 mSec
//    senseair_state = 0;

    uint16_t value = 0;
    bool data_ready = SenseairModbus->ReceiveReady();

    if (data_ready) {
      uint8_t error = SenseairModbus->Receive16BitRegister(&value);
      if (error) {
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "SenseAir response error %d"), error);
      } else {
        switch(senseair_read_state) {
          case 0:                // 0x1A (26) READ_TYPE_LOW - S8: fe 04 02 01 77 ec 92
            senseair_type = 2;
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "SenseAir type id low %04X"), value);
            break;
          case 1:                // 0x00 (0) READ_ERRORLOG - fe 04 02 00 00 ad 24
            if (value) {
              AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "SenseAir error %04X"), value);
            }
            break;
          case 2:                // 0x03 (3) READ_CO2 - fe 04 02 06 2c af 59
            senseair_co2 = value;
#ifdef USE_LIGHT
            LightSetSignal(CO2_LOW, CO2_HIGH, senseair_co2);
#endif  // USE_LIGHT
            break;
          case 3:                // 0x04 (4) READ_TEMPERATURE - S8: fe 84 02 f2 f1 - Illegal Data Address
            senseair_temperature = ConvertTemp((float)value / 100);
            break;
          case 4:                // 0x05 (5) READ_HUMIDITY - S8: fe 84 02 f2 f1 - Illegal Data Address
            senseair_humidity = ConvertHumidity((float)value / 100);
            break;
          case 5:                // 0x1C (28) READ_RELAY_STATE - S8: fe 04 02 01 54 ad 4b - firmware version
          {
            bool relay_state = value >> 8 & 1;
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "SenseAir relay state %d"), relay_state);
            break;
          }
          case 6:                // 0x0A (10) READ_TEMP_ADJUSTMENT - S8: fe 84 02 f2 f1 - Illegal Data Address
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "SenseAir temp adjustment %d"), value);
            break;
        }
      }
      senseair_read_state++;
      if (2 == senseair_type) {  // S8
        if (3 == senseair_read_state) {
          senseair_read_state = 1;
        }
      } else {                   // K30, K70
        if (sizeof(start_addresses) == senseair_read_state) {
          senseair_read_state = 1;
        }
      }
    }

    if (0 == senseair_send_retry || data_ready) {
      senseair_send_retry = 5;
      SenseairModbus->Send(SENSEAIR_DEVICE_ADDRESS, SENSEAIR_READ_REGISTER, (uint16_t)start_addresses[senseair_read_state], 1);
    } else {
      senseair_send_retry--;
    }

//  }
}

/*********************************************************************************************/

void SenseairInit(void)
{
  senseair_type = 0;
  if ((pin[GPIO_SAIR_RX] < 99) && (pin[GPIO_SAIR_TX] < 99)) {
    SenseairModbus = new TasmotaModbus(pin[GPIO_SAIR_RX], pin[GPIO_SAIR_TX]);
    uint8_t result = SenseairModbus->Begin(SENSEAIR_MODBUS_SPEED);
    if (result) {
      if (2 == result) { ClaimSerial(); }
      senseair_type = 1;
    }
  }
}

void SenseairShow(bool json)
{
  GetTextIndexed(senseair_types, sizeof(senseair_types), senseair_type -1, kSenseairTypes);

  if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_CO2 "\":%d"), senseair_types, senseair_co2);
    if (senseair_type != 2) {
      ResponseAppend_P(PSTR(","));
      ResponseAppendTHD(senseair_temperature, senseair_humidity);
    }
    ResponseJsonEnd();
#ifdef USE_DOMOTICZ
    if (0 == tele_period) {
      DomoticzSensor(DZ_AIRQUALITY, senseair_co2);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_CO2, senseair_types, senseair_co2);
    if (senseair_type != 2) {
      WSContentSend_THD(senseair_types, senseair_temperature, senseair_humidity);
    }
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns17(uint8_t function)
{
  bool result = false;

  if (senseair_type) {
    switch (function) {
      case FUNC_INIT:
        SenseairInit();
        break;
      case FUNC_EVERY_250_MSECOND:
        Senseair250ms();
        break;
      case FUNC_JSON_APPEND:
        SenseairShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        SenseairShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SENSEAIR
