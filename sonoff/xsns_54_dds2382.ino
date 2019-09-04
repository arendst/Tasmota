/*
  xsns_47_dds2382.ino - Eastron dds2382-Modbus energy meter support for Sonoff-Tasmota

  Copyright (C) 2019  Matteo Campanella 
  
  Based on the work of Gennaro Tortone

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
#ifdef USE_DDS2382

/*********************************************************************************************\
 * Hiking dds238-2 Modbus energy meter
 *
 * Based on: https://github.com/reaper7/SDM_Energy_Meter
\*********************************************************************************************/

#define XSNS_54             54

// can be user defined in my_user_config.h
#ifndef DDS2382_SPEED
  #define DDS2382_SPEED      9600    // default dds2382 Modbus address
#endif
// can be user defined in my_user_config.h
#ifndef DDS2382_ADDR
  #define DDS2382_ADDR       1       // default dds2382 Modbus address
#endif


#include <TasmotaSerial.h>

enum DDS2382_Error {DDS2382_ERR_NO_ERROR=0, DDS2382_ERR_CRC_ERROR, DDS2382_ERR_WRONG_BYTES, DDS2382_ERR_INVALID_LENGTH};
enum DDS2382_State {DDS2382_STATE_INIT,DDS2382_STATE_READY,DDS2382_STATE_SEND_CMD,DDS2382_STATE_WAIT_RESPONSE,DDS2382_STATE_PROCESS_RESPONSE};

TasmotaSerial *DDS2382Serial;

uint32_t dds2382_state = DDS2382_STATE_READY;
uint8_t dds2382_tx_buffer[8];
uint8_t dds2382_rx_buffer[48];

struct {
  uint16_t voltage = 0;
  uint16_t current = 0;
  int16_t active_power = 0;
  uint16_t reactive_power = 0;
  uint16_t power_factor = 0;
  uint16_t frequency = 0;
  uint32_t energy_total = 0;
  uint16_t import_active = 0;
  uint16_t export_active = 0;
} dds2382_data;

bool DDS2382_ModbusReceiveReady(void)
{
  return (DDS2382Serial->available() > 1);
}

void DDS2382_ModbusSend(uint8_t function_code, uint16_t start_address, uint16_t register_count)
{
  dds2382_tx_buffer[0] = DDS2382_ADDR;
  dds2382_tx_buffer[1] = function_code;
  dds2382_tx_buffer[2] = (uint8_t)(start_address >> 8);
  dds2382_tx_buffer[3] = (uint8_t)(start_address);
  dds2382_tx_buffer[4] = (uint8_t)(register_count >> 8);
  dds2382_tx_buffer[5] = (uint8_t)(register_count);

  uint16_t crc = DDS2382_calculateCRC(dds2382_tx_buffer, 6);  // calculate out crc only from first 6 bytes
  dds2382_tx_buffer[6] = lowByte(crc);
  dds2382_tx_buffer[7] = highByte(crc);

  while (DDS2382Serial->available() > 0)  {  // read serial if any old data is available
    DDS2382Serial->read();
  }
  DDS2382Serial->flush();
  DDS2382Serial->write(dds2382_tx_buffer, sizeof(dds2382_tx_buffer));
}

uint8_t DDS2382_ModbusReceive()
{
  uint32_t len = DDS2382Serial->readBytes(dds2382_rx_buffer,41);
  if (len != 41) {
    return DDS2382_ERR_INVALID_LENGTH;
  }
  else {
    if (0x01 == dds2382_rx_buffer[0] && 0x03 == dds2382_rx_buffer[1] && 36 == dds2382_rx_buffer[2]) {   // check node number, op code and reply bytes count
      return DDS2382_ERR_NO_ERROR;
    } else {
      return DDS2382_ERR_WRONG_BYTES;
    }
  }
}

uint8_t DDS2382_checkDataCRC() {
  uint16_t crc = DDS2382_calculateCRC(dds2382_rx_buffer, 39);
  if (crc == ((dds2382_rx_buffer[40] << 8) | dds2382_rx_buffer[39])) {  //calculate crc from first 7 bytes and compare with received crc (bytes 39 & 40)
    return DDS2382_ERR_NO_ERROR;
  } else {
    return DDS2382_ERR_CRC_ERROR;
  }
}

uint16_t DDS2382_calculateCRC(uint8_t *frame, uint8_t num)
{
  uint16_t crc = 0xFFFF;
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

uint8_t dds2382_nodata_count = 0;

void DDS2382_reset() {
  dds2382_nodata_count = 255;
  dds2382_state = DDS2382_STATE_READY;
  
  //dds2382_data.voltage = 0;
  //dds2382_data.current = 0;
  //dds2382_data.active_power = 0;
  //dds2382_data.reactive_power = 0;
  dds2382_data.power_factor = 0;
  //dds2382_data.frequency = 0;
  //dds2382_data.energy_total = 0;
  //dds2382_data.import_active = 0;
  //dds2382_data.export_active = 0;
}

void DDS2382_250ms(void)
{
  uint8_t error;

  switch (dds2382_state) {
    case DDS2382_STATE_READY:
      dds2382_state = DDS2382_STATE_SEND_CMD;
      break;
    case DDS2382_STATE_SEND_CMD:
      DDS2382_ModbusSend(0x03, 0, 18);
      dds2382_nodata_count = 0;
      dds2382_state = DDS2382_STATE_WAIT_RESPONSE;
      break;
    case DDS2382_STATE_WAIT_RESPONSE:
      if (DDS2382_ModbusReceiveReady()) {
        error = DDS2382_ModbusReceive();
        if (error) {
          AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_DEBUG "dds2382 response error %d"), error);
          dds2382_state = DDS2382_STATE_SEND_CMD;
        }
        else dds2382_state = DDS2382_STATE_PROCESS_RESPONSE;
      }
      else {
        if (dds2382_nodata_count < 15) dds2382_nodata_count++;  // max. 4 sec without data
        else if (dds2382_nodata_count != 255) {
          // no data from modbus, reset
          DDS2382_reset();
        }
      }
      break;
    case DDS2382_STATE_PROCESS_RESPONSE:
      error = DDS2382_checkDataCRC();
      if (error) AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_DEBUG "dds2382 CRC error %d"), error);
      else {
        dds2382_data.voltage = dds2382_rx_buffer[27];
        dds2382_data.voltage <<= 8;
        dds2382_data.voltage += dds2382_rx_buffer[28];

        dds2382_data.current = dds2382_rx_buffer[29];
        dds2382_data.current <<= 8;
        dds2382_data.current += dds2382_rx_buffer[30];

        dds2382_data.active_power = dds2382_rx_buffer[31];
        dds2382_data.active_power <<= 8;
        dds2382_data.active_power += dds2382_rx_buffer[32];

        dds2382_data.reactive_power = dds2382_rx_buffer[33];
        dds2382_data.reactive_power <<= 8;
        dds2382_data.reactive_power += dds2382_rx_buffer[34];

        dds2382_data.power_factor = dds2382_rx_buffer[35];
        dds2382_data.power_factor <<= 8;
        dds2382_data.power_factor += dds2382_rx_buffer[36];

        dds2382_data.frequency = dds2382_rx_buffer[37];
        dds2382_data.frequency <<= 8;
        dds2382_data.frequency += dds2382_rx_buffer[38];

        dds2382_data.energy_total = dds2382_rx_buffer[3];
        dds2382_data.energy_total <<= 8;
        dds2382_data.energy_total += dds2382_rx_buffer[4];
        dds2382_data.energy_total <<= 8;
        dds2382_data.energy_total += dds2382_rx_buffer[5];
        dds2382_data.energy_total <<= 8;
        dds2382_data.energy_total += dds2382_rx_buffer[6];

        dds2382_data.export_active = dds2382_rx_buffer[11];
        dds2382_data.export_active <<= 8;
        dds2382_data.export_active += dds2382_rx_buffer[12];
        dds2382_data.export_active <<= 8;
        dds2382_data.export_active += dds2382_rx_buffer[13];
        dds2382_data.export_active <<= 8;
        dds2382_data.export_active += dds2382_rx_buffer[14];

        dds2382_data.import_active = dds2382_rx_buffer[15];
        dds2382_data.import_active <<= 8;
        dds2382_data.import_active += dds2382_rx_buffer[16];
        dds2382_data.import_active <<= 8;
        dds2382_data.import_active += dds2382_rx_buffer[17];
        dds2382_data.import_active <<= 8;
        dds2382_data.import_active += dds2382_rx_buffer[18];               
      }
      dds2382_state = DDS2382_STATE_SEND_CMD;
      break;
    default: //invalid state - reset
      DDS2382_reset();
  }
}

void DDS2382Init(void)
{
  dds2382_state = DDS2382_STATE_INIT;
  if ((pin[GPIO_DDS2382_RX] < 99) && (pin[GPIO_DDS2382_TX] < 99)) {
    DDS2382Serial = new TasmotaSerial(pin[GPIO_DDS2382_RX], pin[GPIO_DDS2382_TX], 1);
    if (DDS2382Serial->begin(DDS2382_SPEED)) {
      if (DDS2382Serial->hardwareSerial()) { ClaimSerial(); }
      DDS2382Serial->setTimeout(50);
      dds2382_state = DDS2382_STATE_READY;
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_DDS2382_DATA[] PROGMEM =
  "{s}dds2382 " D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
  "{s}dds2382 " D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
  "{s}dds2382 " D_POWERUSAGE_ACTIVE "{m}%s " D_UNIT_WATT "{e}"
  "{s}dds2382 " D_POWERUSAGE_REACTIVE "{m}%s " D_UNIT_VAR "{e}"
  "{s}dds2382 " D_POWER_FACTOR "{m}%s{e}"
  "{s}dds2382 " D_FREQUENCY "{m}%s " D_UNIT_HERTZ "{e}"
  "{s}dds2382 " D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}dds2382 " D_IMPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}dds2382 " D_EXPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  ;
#endif  // USE_WEBSERVER

void DDS2382Show(bool json)
{
  char voltage[33];
  dtostrfd(((float)dds2382_data.voltage)/10, Settings.flag2.voltage_resolution, voltage);
  char current[33];
  dtostrfd(((float)dds2382_data.current)/100, Settings.flag2.current_resolution, current);
  char active_power[33];
  dtostrfd(((float)dds2382_data.active_power), Settings.flag2.wattage_resolution, active_power);
  char reactive_power[33];
  dtostrfd(((float)dds2382_data.reactive_power), Settings.flag2.wattage_resolution, reactive_power);
  char power_factor[33];
  dtostrfd(((float)dds2382_data.power_factor)/1000, 2, power_factor);
  char frequency[33];
  dtostrfd(((float)dds2382_data.frequency)/100, Settings.flag2.frequency_resolution, frequency);
  char energy_total[33];
  dtostrfd(((float)dds2382_data.energy_total)/100, Settings.flag2.energy_resolution, energy_total);
  char import_active[33];
  dtostrfd(((float)dds2382_data.import_active)/100, Settings.flag2.wattage_resolution, import_active);
  char export_active[33];
  dtostrfd(((float)dds2382_data.export_active)/100, Settings.flag2.wattage_resolution, export_active);
  if (json) {
    ResponseAppend_P(PSTR(",\"" D_RSLT_ENERGY "\":{\"" D_JSON_TOTAL "\":%s,\"" D_JSON_ACTIVE_POWERUSAGE "\":%s,\"" D_JSON_REACTIVE_POWERUSAGE "\":%s,\"" D_JSON_FREQUENCY "\":%s,\"" D_JSON_POWERFACTOR "\":%s,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT  "\":%s,\"" D_JSON_IMPORT_ACTIVE "\":%s,\"" D_JSON_EXPORT_ACTIVE "\":%s}"),
      energy_total, active_power, reactive_power, frequency, power_factor, voltage, current, import_active, export_active);
#ifdef USE_DOMOTICZ
    if (0 == tele_period) {
      char energy_total_chr[33];
      dtostrfd(dds2382_data.energy_total * 1000, 1, energy_total_chr);
      DomoticzSensor(DZ_VOLTAGE, voltage);
      DomoticzSensor(DZ_CURRENT, current);
      DomoticzSensorPowerEnergy(dds2382_data.active_power, energy_total_chr);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_DDS2382_DATA, voltage, current, active_power, reactive_power, power_factor, frequency, energy_total,import_active,export_active);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns54(uint8_t function)
{
  bool result = false;

  if (dds2382_state > DDS2382_STATE_INIT) {
    switch (function) {
      case FUNC_INIT:
        DDS2382Init();
        break;
      case FUNC_EVERY_250_MSECOND:
        DDS2382_250ms();
        break;
      case FUNC_JSON_APPEND:
        DDS2382Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        DDS2382Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif   // USE_DDS2382
