/*
  xsns_17_senseair.ino - SenseAir CO2 sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

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
 * SenseAir K30, K70, S8 and S88 - CO2 sensor
 *
 * Adapted from EspEasy plugin P052 by Mikael Trieb (mikael__AT__triebconsulting.se)
 *
 * Hardware Serial will be selected if GPIO1 = [SAir Rx] and GPIO3 = [SAir Tx]
\*********************************************************************************************/

/*********************************************************************************************\
 *      aSENSE      tSENSE      K30         S8          S88
 * IR1  MeterStatus MeterStatus MeterStatus MeterStatus MeterStatus
 * IR4  Space CO2   Space CO2   Space CO2   Space CO2   Space CO2
 * IR5  Space Temp  Space Temp  -           -           Space Temp
 * IR6  -           RH          -           -           -
 * IR25 -           -           -           -           ETC Low
 * IR27 -           -           -           Type Id Low Type Id Low
 *
\*********************************************************************************************/

#define XSNS_17                      17

#define SENSEAIR_MODBUS_SPEED        9600
#define SENSEAIR_DEVICE_ADDRESS      0xFE    // Any address

#define COMMAND_READ_HOLDING_REGISTER 0x03
#define COMMAND_READ_INPUT_REGISTER   0x04

#define IR_METER_STATUS              1
#define IR_SPACE_CO2                 4
#define IR_SPACE_TEMP                5
#define IR_SPACE_HUMIDITY            6
#define IR_TEMP_ADJUSTMENT           11
#define IR_SENSOR_ETC_LOW            25
#define IR_SENSOR_ETC_HIGH           26
#define IR_SENSOR_TYPE_ID_LOW        27
#define IR_SENSOR_TYPE_ID_HIGH       28
#define IR_RELAY_STATE               29

#define SENSOR_TYPE_INIT             0
#define SENSOR_TYPE_UNKNOWN          1
#define SENSOR_TYPE_NONE             2
#define SENSOR_TYPE_KX0              3
#define SENSOR_TYPE_S8               4
#define SENSOR_TYPE_S88              5

#ifndef CO2_LOW
#define CO2_LOW                      800     // Below this CO2 value show green light
#endif
#ifndef CO2_HIGH
#define CO2_HIGH                     1200    // Above this CO2 value show red light
#endif

#include <TasmotaModbus.h>
TasmotaModbus *SenseairModbus;

const char kSenseairTypes[] PROGMEM = "Kx0|S8|S88";

uint8_t senseair_type = SENSOR_TYPE_INIT;
char senseair_types[7];

uint16_t senseair_co2 = 0;
float senseair_temperature = 0;
float senseair_humidity = 0;

const uint8_t input_registers[] {
    IR_SENSOR_ETC_LOW,
    IR_SENSOR_TYPE_ID_LOW,
    IR_METER_STATUS,
    IR_SPACE_CO2,
    IR_SPACE_TEMP,
    IR_SPACE_HUMIDITY,
//     IR_RELAY_STATE,
//     IR_TEMP_ADJUSTMENT
};

#define INPUT_REGISTERS_LOOP_START 2
#define INPUT_REGISTERS_LOOP_END_S8 4
#define INPUT_REGISTERS_LOOP_END_S88 5
#define INPUT_REGISTERS_LOOP_END_KX0 6

uint8_t senseair_read_state = 0;
uint8_t senseair_send_retry = 0;

void Senseair250ms(void)              // Every 250 mSec
{
  if (senseair_type == SENSOR_TYPE_INIT || senseair_type == SENSOR_TYPE_NONE) {
    return;
  }

  bool data_ready = SenseairModbus->ReceiveReady();

  if (data_ready) {
    uint16_t value = 0;

    // Read register.
    uint8_t error = SenseairModbus->Receive16BitRegister(&value);

    if (senseair_type == SENSOR_TYPE_UNKNOWN) {
      // Try to determine sensor model.
      switch (input_registers[senseair_read_state]) {
        case IR_SENSOR_ETC_LOW:
            if (error == 0) {
              senseair_type = SENSOR_TYPE_S88;
            }
            break;
        case IR_SENSOR_TYPE_ID_LOW:
            switch (error) {
            case 0:
              senseair_type = SENSOR_TYPE_S8;
              break;
            case 2: // Illegal Data Address
              senseair_type = SENSOR_TYPE_KX0;
              break;
          }
          break;
      }
      if (senseair_type != SENSOR_TYPE_UNKNOWN) {
        GetTextIndexed(senseair_types, sizeof(senseair_types), senseair_type - SENSOR_TYPE_KX0, kSenseairTypes);
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_SENSEAIR "%s detected"), senseair_types);
      }
    }

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_SENSEAIR "Read IR%d error %d"), input_registers[senseair_read_state], error);
    } else {
      // Process register contents.
      switch(input_registers[senseair_read_state]) {
//         case IR_SENSOR_TYPE_ID_LOW:                // 0x1A (26) IR_SENSOR_TYPE_ID_LOW - S8: fe 04 02 01 77 ec 92
//           break;
        case IR_METER_STATUS:                // 0x00 (0) IR_METER_STATUS - fe 04 02 00 00 ad 24
          switch (senseair_type) {
          case SENSOR_TYPE_S8: value &= 0x7f; break;
          case SENSOR_TYPE_S88: value &= 0xff; break;
          }
          if (value) {
            // Out of range or Warm Up is expected after power on
            AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_SENSEAIR "Meter status 0x%04X"), value);
          }
          break;
        case IR_SPACE_CO2:                // 0x03 (3) IR_SPACE_CO2 - fe 04 02 06 2c af 59
          senseair_co2 = value;
#ifdef USE_LIGHT
          LightSetSignal(CO2_LOW, CO2_HIGH, senseair_co2);
#endif  // USE_LIGHT
          break;
        case IR_SPACE_TEMP:                // 0x04 (4) IR_SPACE_TEMP - S8: fe 84 02 f2 f1 - Illegal Data Address
          senseair_temperature = ConvertTemp((float)value / 100);
          break;
        case IR_SPACE_HUMIDITY:                // 0x05 (5) IR_SPACE_HUMIDITY - S8: fe 84 02 f2 f1 - Illegal Data Address
          senseair_humidity = ConvertHumidity((float)value / 100);
          break;
//         case 5:                // 0x1C (28) IR_RELAY_STATE - S8: fe 04 02 01 54 ad 4b - firmware version
//           AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_SENSEAIR "Relay state %d"), (value >> 8) & 1);
//           break;
//         case 6:                // 0x0A (10) READ_TEMP_ADJUSTMENT - S8: fe 84 02 f2 f1 - Illegal Data Address
//           AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_SENSEAIR "Temp adjustment %d"), value);
//           break;
      }
    }

    // Find the next register to be read.
    senseair_read_state++;
    uint8_t input_registers_loop_end;
    switch (senseair_type) {
      case SENSOR_TYPE_KX0:
        input_registers_loop_end = INPUT_REGISTERS_LOOP_END_KX0;
        break;
      case SENSOR_TYPE_S8:
        input_registers_loop_end = INPUT_REGISTERS_LOOP_END_S8;
        break;
      case SENSOR_TYPE_S88:
        input_registers_loop_end = INPUT_REGISTERS_LOOP_END_S88;
        break;
    }
    if (senseair_read_state == input_registers_loop_end) {
      if (senseair_type == SENSOR_TYPE_UNKNOWN) {
        senseair_read_state = 0;
      } else {
        senseair_read_state = INPUT_REGISTERS_LOOP_START;
      }
    }
  }

  if (0 == senseair_send_retry || data_ready) {
    // Send the command (again).
    senseair_send_retry = 5;
    const uint16_t start_address = input_registers[senseair_read_state] - 1;
    const uint16_t count = 1;
    SenseairModbus->Send(SENSEAIR_DEVICE_ADDRESS, COMMAND_READ_INPUT_REGISTER, start_address, count);
  } else {
    senseair_send_retry--;
  }
}

/*********************************************************************************************/

void SenseairInit(void)
{
  if (senseair_type != SENSOR_TYPE_INIT) {
    return;
  }

  senseair_type = SENSOR_TYPE_NONE;
  if (PinUsed(GPIO_SAIR_RX) && PinUsed(GPIO_SAIR_TX)) {
    SenseairModbus = new TasmotaModbus(Pin(GPIO_SAIR_RX), Pin(GPIO_SAIR_TX));
    uint8_t result = SenseairModbus->Begin(SENSEAIR_MODBUS_SPEED);
    if (result) {
      if (result == 2) {
        // We have hardware serial, so claim it
        ClaimSerial();
      }
      senseair_type = SENSOR_TYPE_UNKNOWN;
    }
  }
}

void SenseairShow(uint32_t function)
{
  if (senseair_type < SENSOR_TYPE_KX0) {
    return;
  }

  if (function == FUNC_JSON_APPEND) {
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_CO2 "\":%d"), senseair_types, senseair_co2);
    switch (senseair_type) {
      case SENSOR_TYPE_S88:
        ResponseAppend_P(PSTR(",\"" D_JSON_TEMPERATURE "\":%f,\""), senseair_temperature);
        break;
      case SENSOR_TYPE_KX0:
        ResponseAppend_P(PSTR(","));
        ResponseAppendTHD(senseair_temperature, senseair_humidity);
        break;
      }
    ResponseJsonEnd();
#ifdef USE_DOMOTICZ
    if (0 == TasmotaGlobal.tele_period) {
      DomoticzSensor(DZ_AIRQUALITY, senseair_co2);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_CO2, senseair_types, senseair_co2);
    switch (senseair_type) {
      case SENSOR_TYPE_S88:
        WSContentSend_Temp(senseair_types, senseair_temperature);
        break;
      case SENSOR_TYPE_KX0:
        WSContentSend_THD(senseair_types, senseair_temperature, senseair_humidity);
        break;
    }
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns17(uint32_t function)
{
  switch (function) {
    case FUNC_INIT:
      SenseairInit();
      break;
    case FUNC_EVERY_250_MSECOND:
      Senseair250ms();
      break;
    case FUNC_JSON_APPEND:
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
#endif  // USE_WEBSERVER
      SenseairShow(function);
      break;
  }
  return false;
}

#endif  // USE_SENSEAIR
