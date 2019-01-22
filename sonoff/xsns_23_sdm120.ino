/*
  xsns_23_sdm120.ino - Eastron SDM120-Modbus energy meter support for Sonoff-Tasmota

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

#ifdef USE_SDM120

/*********************************************************************************************\
 * Eastron SDM120-Modbus energy meter
 *
 * Based on: https://github.com/reaper7/SDM_Energy_Meter
\*********************************************************************************************/

#define XSNS_23             23

#include <TasmotaSerial.h>

TasmotaSerial *SDM120Serial;

uint8_t sdm120_type = 1;
//uint8_t sdm120_state = 0;

float sdm120_voltage = 0;
float sdm120_current = 0;
float sdm120_active_power = 0;
float sdm120_apparent_power = 0;
float sdm120_reactive_power = 0;
float sdm120_power_factor = 0;
float sdm120_frequency = 0;
float sdm120_energy_total = 0;
float sdm120_phase_angle = 0;
float sdm120_import_active = 0;
float sdm120_export_active = 0;
float sdm120_import_reactive = 0;
float sdm120_export_reactive = 0;
float sdm120_total_reactive = 0;

bool SDM120_ModbusReceiveReady(void)
{
  return (SDM120Serial->available() > 1);
}

void SDM120_ModbusSend(uint8_t function_code, uint16_t start_address, uint16_t register_count)
{
  uint8_t frame[8];

  frame[0] = 0x01;          // default SDM120 Modbus address
  frame[1] = function_code;
  frame[2] = (uint8_t)(start_address >> 8);
  frame[3] = (uint8_t)(start_address);
  frame[4] = (uint8_t)(register_count >> 8);
  frame[5] = (uint8_t)(register_count);

  uint16_t crc = SDM120_calculateCRC(frame, 6);  // calculate out crc only from first 6 bytes
  frame[6] = lowByte(crc);
  frame[7] = highByte(crc);

  while (SDM120Serial->available() > 0)  {  // read serial if any old data is available
    SDM120Serial->read();
  }

  SDM120Serial->flush();
  SDM120Serial->write(frame, sizeof(frame));
}

uint8_t SDM120_ModbusReceive(float *value)
{
  uint8_t buffer[9];

  *value = NAN;
  uint8_t len = 0;
  while (SDM120Serial->available() > 0) {
    buffer[len++] = (uint8_t)SDM120Serial->read();
  }

  if (len < 9)
    return 3;   // SDM_ERR_NOT_ENOUGHT_BYTES

  if (len == 9) {

    if (buffer[0] == 0x01 && buffer[1] == 0x04 && buffer[2] == 4) {   // check node number, op code and reply bytes count

      if((SDM120_calculateCRC(buffer, 7)) == ((buffer[8] << 8) | buffer[7])) {  //calculate crc from first 7 bytes and compare with received crc (bytes 7 & 8)

        ((uint8_t*)value)[3] = buffer[3];
        ((uint8_t*)value)[2] = buffer[4];
        ((uint8_t*)value)[1] = buffer[5];
        ((uint8_t*)value)[0] = buffer[6];

      } else return 1; // SDM_ERR_CRC_ERROR

    } else return 2;  // SDM_ERR_WRONG_BYTES
  }

  return 0;   // SDM_ERR_NO_ERROR
}

uint16_t SDM120_calculateCRC(uint8_t *frame, uint8_t num)
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

const uint16_t sdm120_start_addresses[] {
  0x0000,   // SDM120C_VOLTAGE  [V]
  0x0006,   // SDM120C_CURRENT  [A]
  0x000C,   // SDM120C_POWER    [W]
  0x0012,   // SDM120C_APPARENT_POWER  [VA]
  0x0018,   // SDM120C_REACTIVE_POWER  [VAR]
  0x001E,   // SDM120C_POWER_FACTOR
  0x0046,   // SDM120C_FREQUENCY  [Hz]
#ifdef USE_SDM220
  0x0156,   // SDM120C_TOTAL_ACTIVE_ENERGY  [Wh]
    0X0024, // SDM220_PHASE_ANGLE [Degre]
    0X0048, // SDM220_IMPORT_ACTIVE [kWh]
    0X004A, // SDM220_EXPORT_ACTIVE [kWh]
    0X004C, // SDM220_IMPORT_REACTIVE [kVArh]
    0X004E, // SDM220_EXPORT_REACTIVE [kVArh]
    0X0158  // SDM220 TOTAL_REACTIVE   [kVArh]
#else  // USE_SDM220
  0x0156    // SDM120C_TOTAL_ACTIVE_ENERGY  [Wh]
#endif // USE_SDM220
};

uint8_t sdm120_read_state = 0;
uint8_t sdm120_send_retry = 0;

void SDM120250ms(void)              // Every 250 mSec
{
//  sdm120_state++;
//  if (6 == sdm120_state) {     // Every 300 mSec
//    sdm120_state = 0;

    float value = 0;
    bool data_ready = SDM120_ModbusReceiveReady();

    if (data_ready) {
      uint8_t error = SDM120_ModbusReceive(&value);
      if (error) {
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SDM120 response error %d"), error);
        AddLog(LOG_LEVEL_DEBUG);
      } else {
        switch(sdm120_read_state) {
          case 0:
            sdm120_voltage = value;
            break;

          case 1:
            sdm120_current = value;
            break;

          case 2:
            sdm120_active_power = value;
            break;

          case 3:
            sdm120_apparent_power = value;
            break;

          case 4:
            sdm120_reactive_power = value;
            break;

          case 5:
            sdm120_power_factor = value;
            break;

          case 6:
            sdm120_frequency = value;
            break;

          case 7:
            sdm120_energy_total = value;
            break;
#ifdef USE_SDM220
          case 8:
            sdm120_phase_angle = value;
            break;

          case 9:
            sdm120_import_active = value;
            break;

          case 10:
            sdm120_export_active = value;
            break;

          case 11:
            sdm120_import_reactive = value;
            break;

          case 12:
            sdm120_export_reactive = value;
            break;

          case 13:
            sdm120_total_reactive = value;
            break;
#endif  // USE_SDM220
        } // end switch

        sdm120_read_state++;

        if (sizeof(sdm120_start_addresses)/2 == sdm120_read_state) {
          sdm120_read_state = 0;
        }
      }
    } // end data ready

    if (0 == sdm120_send_retry || data_ready) {
      sdm120_send_retry = 5;
       SDM120_ModbusSend(0x04, sdm120_start_addresses[sdm120_read_state], 2);
    } else {
      sdm120_send_retry--;
    }
//  } // end 300 ms
}

void SDM120Init(void)
{
  sdm120_type = 0;
  if ((pin[GPIO_SDM120_RX] < 99) && (pin[GPIO_SDM120_TX] < 99)) {
    SDM120Serial = new TasmotaSerial(pin[GPIO_SDM120_RX], pin[GPIO_SDM120_TX], 1);
#ifdef SDM120_SPEED
    if (SDM120Serial->begin(SDM120_SPEED)) {
#else
    if (SDM120Serial->begin(2400)) {
#endif
      if (SDM120Serial->hardwareSerial()) { ClaimSerial(); }
      sdm120_type = 1;
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_SDM120_DATA[] PROGMEM = "%s"
  "{s}SDM120 " D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
  "{s}SDM120 " D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
  "{s}SDM120 " D_POWERUSAGE_ACTIVE "{m}%s " D_UNIT_WATT "{e}"
  "{s}SDM120 " D_POWERUSAGE_APPARENT "{m}%s " D_UNIT_VA "{e}"
  "{s}SDM120 " D_POWERUSAGE_REACTIVE "{m}%s " D_UNIT_VAR "{e}"
  "{s}SDM120 " D_POWER_FACTOR "{m}%s{e}"
  "{s}SDM120 " D_FREQUENCY "{m}%s " D_UNIT_HERTZ "{e}"
  "{s}SDM120 " D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
#ifdef USE_SDM220
  "{s}SDM120 " D_PHASE_ANGLE "{m}%s " D_UNIT_ANGLE "{e}"
  "{s}SDM120 " D_IMPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}SDM120 " D_EXPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}SDM120 " D_IMPORT_REACTIVE "{m}%s " D_UNIT_KWARH "{e}"
  "{s}SDM120 " D_EXPORT_REACTIVE "{m}%s " D_UNIT_KWARH "{e}"
  "{s}SDM120 " D_TOTAL_REACTIVE "{m}%s " D_UNIT_KWARH "{e}"
#endif // USE_SDM220
  ;
#endif  // USE_WEBSERVER

void SDM120Show(boolean json)
{
  char voltage[33];
  dtostrfd(sdm120_voltage,        Settings.flag2.voltage_resolution, voltage);
  char current[33];
  dtostrfd(sdm120_current,        Settings.flag2.current_resolution, current);
  char active_power[33];
  dtostrfd(sdm120_active_power,   Settings.flag2.wattage_resolution, active_power);
  char apparent_power[33];
  dtostrfd(sdm120_apparent_power, Settings.flag2.wattage_resolution, apparent_power);
  char reactive_power[33];
  dtostrfd(sdm120_reactive_power, Settings.flag2.wattage_resolution, reactive_power);
  char power_factor[33];
  dtostrfd(sdm120_power_factor,   2, power_factor);
  char frequency[33];
  dtostrfd(sdm120_frequency,      Settings.flag2.frequency_resolution, frequency);
  char energy_total[33];
  dtostrfd(sdm120_energy_total,   Settings.flag2.energy_resolution, energy_total);
#ifdef USE_SDM220
  char phase_angle[33];
  dtostrfd(sdm120_phase_angle,    2, phase_angle);
  char import_active[33];
  dtostrfd(sdm120_import_active,  Settings.flag2.wattage_resolution, import_active);
  char export_active[33];
  dtostrfd(sdm120_export_active,  Settings.flag2.wattage_resolution, export_active);
  char import_reactive[33];
  dtostrfd(sdm120_import_reactive,Settings.flag2.wattage_resolution, import_reactive);
  char export_reactive[33];
  dtostrfd(sdm120_export_reactive,Settings.flag2.wattage_resolution, export_reactive);
  char total_reactive[33];
  dtostrfd(sdm120_total_reactive, Settings.flag2.wattage_resolution, total_reactive);
#endif // USE_SDM220
  if (json) {
#ifdef USE_SDM220
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_RSLT_ENERGY "\":{\"" D_JSON_TOTAL "\":%s,\"" D_JSON_ACTIVE_POWERUSAGE "\":%s,\"" D_JSON_APPARENT_POWERUSAGE "\":%s,\"" D_JSON_REACTIVE_POWERUSAGE "\":%s,\"" D_JSON_FREQUENCY "\":%s,\"" D_JSON_POWERFACTOR "\":%s,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT  "\":%s,\"" D_JSON_PHASE_ANGLE "\":%s,\"" D_JSON_IMPORT_ACTIVE "\":%s,\"" D_JSON_EXPORT_ACTIVE "\":%s,\"" D_JSON_IMPORT_REACTIVE "\":%s,\"" D_JSON_EXPORT_REACTIVE "\":%s,\"" D_JSON_TOTAL_REACTIVE "\":%s}"),
      mqtt_data, energy_total, active_power, apparent_power, reactive_power, frequency, power_factor, voltage, current, phase_angle, import_active, export_active, import_reactive, export_reactive, total_reactive);
#else
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_RSLT_ENERGY "\":{\"" D_JSON_TOTAL "\":%s,\"" D_JSON_ACTIVE_POWERUSAGE "\":%s,\"" D_JSON_APPARENT_POWERUSAGE "\":%s,\"" D_JSON_REACTIVE_POWERUSAGE "\":%s,\"" D_JSON_FREQUENCY "\":%s,\"" D_JSON_POWERFACTOR "\":%s,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s}"),
      mqtt_data, energy_total, active_power, apparent_power, reactive_power, frequency, power_factor, voltage, current);
#endif // USE_SDM220
#ifdef USE_DOMOTICZ
    if (0 == tele_period) {
      DomoticzSensor(DZ_VOLTAGE, voltage);
      DomoticzSensor(DZ_CURRENT, current);
      DomoticzSensorPowerEnergy((int)sdm120_active_power, energy_total);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
#ifdef USE_SDM220
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_SDM120_DATA, mqtt_data, voltage, current, active_power, apparent_power, reactive_power, power_factor, frequency, energy_total, phase_angle,import_active,export_active,import_reactive,export_reactive,total_reactive);
#else
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_SDM120_DATA, mqtt_data, voltage, current, active_power, apparent_power, reactive_power, power_factor, frequency, energy_total);
#endif  // USE_SDM220
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns23(byte function)
{
  boolean result = false;

  if (sdm120_type) {
    switch (function) {
      case FUNC_INIT:
        SDM120Init();
        break;
      case FUNC_EVERY_250_MSECOND:
        SDM120250ms();
        break;
      case FUNC_JSON_APPEND:
        SDM120Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        SDM120Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif   // USE_SDM120
