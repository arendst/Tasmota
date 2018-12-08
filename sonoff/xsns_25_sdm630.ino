/*
  xsns_25_sdm630.ino - Eastron SDM630-Modbus energy meter support for Sonoff-Tasmota

  Copyright (C) 2018  Gennaro Tortone

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

#ifdef USE_SDM630

/*********************************************************************************************\
 * Eastron SDM630-Modbus energy meter
 *
 * Based on: https://github.com/reaper7/SDM_Energy_Meter
\*********************************************************************************************/

#define XSNS_25             25

#include <TasmotaSerial.h>

TasmotaSerial *SDM630Serial;

uint8_t sdm630_type = 1;
//uint8_t sdm630_state = 0;

float sdm630_voltage[] = {0,0,0};
float sdm630_current[] = {0,0,0};
float sdm630_active_power[] = {0,0,0};
float sdm630_reactive_power[] = {0,0,0};
float sdm630_power_factor[] = {0,0,0};
float sdm630_energy_total = 0;

bool SDM630_ModbusReceiveReady(void)
{
  return (SDM630Serial->available() > 1);
}

void SDM630_ModbusSend(uint8_t function_code, uint16_t start_address, uint16_t register_count)
{
  uint8_t frame[8];

  frame[0] = 0x01;          // default SDM630 Modbus address
  frame[1] = function_code;
  frame[2] = (uint8_t)(start_address >> 8);
  frame[3] = (uint8_t)(start_address);
  frame[4] = (uint8_t)(register_count >> 8);
  frame[5] = (uint8_t)(register_count);

  uint16_t crc = SDM630_calculateCRC(frame, 6);  // calculate out crc only from first 6 bytes
  frame[6] = lowByte(crc);
  frame[7] = highByte(crc);

  while (SDM630Serial->available() > 0)  {  // read serial if any old data is available
    SDM630Serial->read();
  }

  SDM630Serial->flush();
  SDM630Serial->write(frame, sizeof(frame));
}

uint8_t SDM630_ModbusReceive(float *value)
{
  uint8_t buffer[9];

  *value = NAN;
  uint8_t len = 0;
  while (SDM630Serial->available() > 0) {
    buffer[len++] = (uint8_t)SDM630Serial->read();
  }

  if (len < 9)
    return 3;   // SDM_ERR_NOT_ENOUGHT_BYTES

  if (len == 9) {

    if (buffer[0] == 0x01 && buffer[1] == 0x04 && buffer[2] == 4) {   // check node number, op code and reply bytes count

      if((SDM630_calculateCRC(buffer, 7)) == ((buffer[8] << 8) | buffer[7])) {  //calculate crc from first 7 bytes and compare with received crc (bytes 7 & 8)

        ((uint8_t*)value)[3] = buffer[3];
        ((uint8_t*)value)[2] = buffer[4];
        ((uint8_t*)value)[1] = buffer[5];
        ((uint8_t*)value)[0] = buffer[6];

      } else return 1; // SDM_ERR_CRC_ERROR

    } else return 2;  // SDM_ERR_WRONG_BYTES
  }

  return 0;   // SDM_ERR_NO_ERROR
}

uint16_t SDM630_calculateCRC(uint8_t *frame, uint8_t num)
{
  uint16_t crc, flag;
  crc = 0xFFFF;
  for (uint8_t i = 0; i < num; i++) {
    crc ^= frame[i];
    for (uint8_t j = 8; j; j--) {
      if ((crc & 0x0001) != 0) {        // If the LSB is set
        crc >>= 1;                      // Shift right and XOR 0xA001
        crc ^= 0xA001;
      } else {                          // Else LSB is not set
        crc >>= 1;                      // Just shift right
      }
    }
  }
  return crc;
}

/*********************************************************************************************/

const uint16_t sdm630_start_addresses[] {
  0x0000, // L1 - SDM630_VOLTAGE [V]
  0x0002, // L2 - SDM630_VOLTAGE [V]
  0x0004, // L3 - SDM630_VOLTAGE [V]
  0x0006, // L1 - SDM630_CURRENT [A]
  0x0008, // L2 - SDM630_CURRENT [A]
  0x000A, // L3 - SDM630_CURRENT [A]
  0x000C, // L1 - SDM630_POWER [W]
  0x000E, // L2 - SDM630_POWER [W]
  0x0010, // L3 - SDM630_POWER [W]
  0x0018, // L1 - SDM630_REACTIVE_POWER [VAR]
  0x001A, // L2 - SDM630_REACTIVE_POWER [VAR]
  0x001C, // L3 - SDM630_REACTIVE_POWER [VAR]
  0x001E, // L1 - SDM630_POWER_FACTOR
  0x0020, // L2 - SDM630_POWER_FACTOR
  0x0022, // L3 - SDM630_POWER_FACTOR
  0x0156 // Total - SDM630_TOTAL_ACTIVE_ENERGY [Wh]
};

uint8_t sdm630_read_state = 0;
uint8_t sdm630_send_retry = 0;

void SDM630250ms(void)              // Every 250 mSec
{
//  sdm630_state++;
//  if (6 == sdm630_state) {     // Every 300 mSec
//    sdm630_state = 0;

    float value = 0;
    bool data_ready = SDM630_ModbusReceiveReady();

    if (data_ready) {
      uint8_t error = SDM630_ModbusReceive(&value);
      if (error) {
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SDM630 response error %d"), error);
        AddLog(LOG_LEVEL_DEBUG);
      } else {
        switch(sdm630_read_state) {
          case 0:
            sdm630_voltage[0] = value;
            break;

          case 1:
            sdm630_voltage[1] = value;
            break;

          case 2:
            sdm630_voltage[2] = value;
            break;

          case 3:
            sdm630_current[0] = value;
            break;

          case 4:
            sdm630_current[1] = value;
            break;

          case 5:
            sdm630_current[2] = value;
            break;

          case 6:
            sdm630_active_power[0] = value;
            break;

          case 7:
            sdm630_active_power[1] = value;
            break;

          case 8:
            sdm630_active_power[2] = value;
            break;

          case 9:
            sdm630_reactive_power[0] = value;
            break;

          case 10:
            sdm630_reactive_power[1] = value;
            break;

          case 11:
            sdm630_reactive_power[2] = value;
            break;

          case 12:
            sdm630_power_factor[0] = value;
            break;

          case 13:
            sdm630_power_factor[1] = value;
            break;

          case 14:
            sdm630_power_factor[2] = value;
            break;

          case 15:
            sdm630_energy_total = value;
            break;
        } // end switch

        sdm630_read_state++;

        if (sizeof(sdm630_start_addresses)/2 == sdm630_read_state) {
          sdm630_read_state = 0;
        }
      }
    } // end data ready

    if (0 == sdm630_send_retry || data_ready) {
      sdm630_send_retry = 5;
       SDM630_ModbusSend(0x04, sdm630_start_addresses[sdm630_read_state], 2);
    } else {
      sdm630_send_retry--;
    }
//  } // end 300 ms
}

void SDM630Init(void)
{
  sdm630_type = 0;
  if ((pin[GPIO_SDM630_RX] < 99) && (pin[GPIO_SDM630_TX] < 99)) {
    SDM630Serial = new TasmotaSerial(pin[GPIO_SDM630_RX], pin[GPIO_SDM630_TX], 1);
#ifdef SDM630_SPEED
    if (SDM630Serial->begin(SDM630_SPEED)) {
#else
    if (SDM630Serial->begin(2400)) {
#endif
      if (SDM630Serial->hardwareSerial()) { ClaimSerial(); }
      sdm630_type = 1;
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_SDM630_DATA[] PROGMEM = "%s"
  "{s}SDM630 " D_VOLTAGE "{m}%s/%s/%s " D_UNIT_VOLT "{e}"
  "{s}SDM630 " D_CURRENT "{m}%s/%s/%s " D_UNIT_AMPERE "{e}"
  "{s}SDM630 " D_POWERUSAGE_ACTIVE "{m}%s/%s/%s " D_UNIT_WATT "{e}"
  "{s}SDM630 " D_POWERUSAGE_REACTIVE "{m}%s/%s/%s " D_UNIT_VAR "{e}"
  "{s}SDM630 " D_POWER_FACTOR "{m}%s/%s/%s{e}"
  "{s}SDM630 " D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";
#endif  // USE_WEBSERVER

void SDM630Show(boolean json)
{
  char voltage_l1[10];
  char voltage_l2[10];
  char voltage_l3[10];
  char current_l1[10];
  char current_l2[10];
  char current_l3[10];
  char active_power_l1[10];
  char active_power_l2[10];
  char active_power_l3[10];
  char reactive_power_l1[10];
  char reactive_power_l2[10];
  char reactive_power_l3[10];
  char power_factor_l1[10];
  char power_factor_l2[10];
  char power_factor_l3[10];
  char energy_total[10];

  dtostrfd(sdm630_voltage[0], Settings.flag2.voltage_resolution, voltage_l1);
  dtostrfd(sdm630_voltage[1], Settings.flag2.voltage_resolution, voltage_l2);
  dtostrfd(sdm630_voltage[2], Settings.flag2.voltage_resolution, voltage_l3);
  dtostrfd(sdm630_current[0], Settings.flag2.current_resolution, current_l1);
  dtostrfd(sdm630_current[1], Settings.flag2.current_resolution, current_l2);
  dtostrfd(sdm630_current[2], Settings.flag2.current_resolution, current_l3);
  dtostrfd(sdm630_active_power[0], Settings.flag2.wattage_resolution, active_power_l1);
  dtostrfd(sdm630_active_power[1], Settings.flag2.wattage_resolution, active_power_l2);
  dtostrfd(sdm630_active_power[2], Settings.flag2.wattage_resolution, active_power_l3);
  dtostrfd(sdm630_reactive_power[0], Settings.flag2.wattage_resolution, reactive_power_l1);
  dtostrfd(sdm630_reactive_power[1], Settings.flag2.wattage_resolution, reactive_power_l2);
  dtostrfd(sdm630_reactive_power[2], Settings.flag2.wattage_resolution, reactive_power_l3);
  dtostrfd(sdm630_power_factor[0], 2, power_factor_l1);
  dtostrfd(sdm630_power_factor[1], 2, power_factor_l2);
  dtostrfd(sdm630_power_factor[2], 2, power_factor_l3);
  dtostrfd(sdm630_energy_total, Settings.flag2.energy_resolution, energy_total);

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_RSLT_ENERGY "\":{\"" D_JSON_TOTAL "\":%s,\""
      D_JSON_ACTIVE_POWERUSAGE "\":[%s,%s,%s],\"" D_JSON_REACTIVE_POWERUSAGE "\":[%s,%s,%s],\""
      D_JSON_POWERFACTOR "\":[%s,%s,%s],\"" D_JSON_VOLTAGE "\":[%s,%s,%s],\"" D_JSON_CURRENT "\":[%s,%s,%s]}"),
      mqtt_data, energy_total, active_power_l1, active_power_l2, active_power_l3,
      reactive_power_l1, reactive_power_l2, reactive_power_l3,
      power_factor_l1, power_factor_l2, power_factor_l3,
      voltage_l1, voltage_l2, voltage_l3,
      current_l1, current_l2, current_l3);
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_SDM630_DATA, mqtt_data,
    voltage_l1, voltage_l2, voltage_l3, current_l1, current_l2, current_l3,
    active_power_l1, active_power_l2, active_power_l3,
    reactive_power_l1, reactive_power_l2, reactive_power_l3,
    power_factor_l1, power_factor_l2, power_factor_l3, energy_total);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns25(byte function)
{
  boolean result = false;

  if (sdm630_type) {
    switch (function) {
      case FUNC_INIT:
        SDM630Init();
        break;
      case FUNC_EVERY_250_MSECOND:
        SDM630250ms();
        break;
      case FUNC_JSON_APPEND:
        SDM630Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        SDM630Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif