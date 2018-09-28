/*
  xnrg_06_pzem2.ino - PZEM-003,017 and PZEM-014,016 Modbus energy sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#ifdef USE_ENERGY_SENSOR
#ifdef USE_PZEM2
/*********************************************************************************************\
 * PZEM-003 - DC 300V 10A Energy
 * PZEM-014 - AC 220V 10A Energy
 * PZEM-016 - AC 220V 100A Energy
 * PZEM-017 - DC 300V 50A - 300A Energy
 *
 * Based on:
 *   PZEM-003,017 docs Https://pan.baidu.com/s/1V9bDWj3RK2u6_fbBJ3GtqQ password rq37
 *   PZEM-014,016 docs https://pan.baidu.com/s/1B0MdMgURyjtO1oQa2lavKw password ytkv
 *
 * Hardware Serial will be selected if GPIO1 = [99 PZEM Rx] and GPIO3 = [98 PZEM Tx]
\*********************************************************************************************/

#define XNRG_05                  5

#define PZEM2_TYPES_003_017      8  // Result 16 bit register count
#define PZEM2_TYPES_014_016     10  // Result 16 bit register count

#define PZEM2_READ_RESULT     0x04

#include <TasmotaSerial.h>

TasmotaSerial *Pzem2Serial;

uint8_t pzem2_type = PZEM2_TYPES_014_016;

/*********************************************************************************************/

uint16_t Pzem2ModbusCalculateCRC(uint8_t *frame, uint8_t num)
{
  uint16_t crc = 0xFFFF;
  uint16_t flag;

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

void Pzem2ModbusSend(uint8_t function_code, uint16_t start_address, uint16_t register_count)
{
  uint8_t frame[8];

  frame[0] = 0xFE;  // Any Address
  frame[1] = function_code;
  frame[2] = (uint8_t)(start_address >> 8);
  frame[3] = (uint8_t)(start_address);
  frame[4] = (uint8_t)(register_count >> 8);
  frame[5] = (uint8_t)(register_count);
  uint16_t crc = Pzem2ModbusCalculateCRC(frame, 6);
  frame[6] = (uint8_t)((crc >> 8) & 0xFF);
  frame[7] = (uint8_t)(crc & 0xFF);

  Pzem2Serial->flush();
  Pzem2Serial->write(frame, sizeof(frame));
}

bool Pzem2ModbusReceiveReady()
{
  return (Pzem2Serial->available() >= 5);  // 5 - Error frame, 21 or 25 - Ok frame
}

uint8_t Pzem2ModbusReceive(uint8_t *buffer, uint8_t register_count)
{
  //  0  1  2  3  4  5  6
  // FE 04 02 08 98 HH LL
  // Id Cc Sz Regis Crc--

  uint8_t len = 0;
  while ((Pzem2Serial->available() > 0) && (len < (register_count *2) + 5)) {
    buffer[len++] = (uint8_t)Pzem2Serial->read();
    if (3 == len) {
      if (buffer[1] & 0x80) {  // fe 84 02 f2 f1
        return buffer[2];      // 1 = Illegal Function, 2 = Illegal Address, 3 = Illegal Data, 4 = Slave Error
      }
    }
  }

  AddLogSerial(LOG_LEVEL_DEBUG_MORE, buffer, len);

  if (len < 7) { return 7; }               // 7 = Not enough data
  if (len != buffer[2] + 5) { return 8; }  // 8 = Unexpected result

  uint16_t crc = (buffer[len -2] << 8) | buffer[len -1];
  if (Pzem2ModbusCalculateCRC(buffer, len -3) != crc) { return 9; } // 9 = crc error

  return 0;                    // 0 = No error
}

/*********************************************************************************************/

uint8_t pzem2_sendRetry = 0;

void Pzem2Every200ms()
{
  bool data_ready = Pzem2ModbusReceiveReady();

  if (data_ready) {
    uint8_t buffer[26];
    uint8_t error = Pzem2ModbusReceive(buffer, pzem2_type);
    if (error) {
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "PZEM2 response error %d"), error);
      AddLog(LOG_LEVEL_DEBUG);
//      if (9 == error) {
        if (PZEM2_TYPES_014_016 == pzem2_type) {
          pzem2_type = PZEM2_TYPES_003_017;
        } else {
          pzem2_type = PZEM2_TYPES_014_016;
        }
//      }
    } else {
      float energy = 0;

      if (PZEM2_TYPES_003_017 == pzem2_type) {
        energy_type_dc = true;
        //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
        // FE 04 10 27 10 00 64 03 E8 00 00 00 00 00 00 00 00 00 00 HH LL             = PZEM-017
        // Id Cc Sz Volt- Curre Power------ Energy----- HiAlm LoAlm Crc--
        energy_voltage = (float)((buffer[3] << 8) + buffer[4]) / 100.0;                                                                 // 655.00 V
        energy_current = (float)((buffer[5] << 8) + buffer[6]) / 100.0;                                                                 // 655.00 A
        energy_active_power = (float)((uint32_t)buffer[9] << 24 + (uint32_t)buffer[10] << 16 + (uint32_t)buffer[7] << 8 + buffer[8]) / 10.0;   // 429496729.0 W
        energy = (float)((uint32_t)buffer[13] << 24 + (uint32_t)buffer[14] << 16 + (uint32_t)buffer[11] << 8 + buffer[12]);             // 4294967295 Wh
        if (!energy_start || (energy < energy_start)) { energy_start = energy; }  // Init after restart and hanlde roll-over if any
        energy_kWhtoday += (energy - energy_start) * 100;
        energy_start = energy;
        EnergyUpdateToday();
      }
      else if (PZEM2_TYPES_014_016 == pzem2_type) {   // PZEM-014,016
        energy_type_dc = false;
        //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
        // FE 04 14 08 98 03 E8 00 00 08 98 00 00 00 00 00 00 01 F4 00 64 00 00 HH LL = PZEM-014
        // Id Cc Sz Volt- Current---- Power------ Energy----- Frequ PFact Alarm Crc--
        energy_voltage = (float)((buffer[3] << 8) + buffer[4]) / 10.0;                                                                     // 6553.0 V
        energy_current = (float)((uint32_t)buffer[7] << 24 + (uint32_t)buffer[8] << 16 + (uint32_t)buffer[5] << 8 + buffer[6]) / 1000.0;   // 4294967.000 A
        energy_active_power = (float)((uint32_t)buffer[11] << 24 + (uint32_t)buffer[12] << 16 + (uint32_t)buffer[9] << 8 + buffer[10]) / 10.0;    // 429496729.0 W
        energy_frequency = (float)((buffer[17] << 8) + buffer[18]) / 10.0;                                                                 // 50.0 Hz
        energy_power_factor = (float)((buffer[19] << 8) + buffer[20]) / 100.0;                                                             // 1.00
        energy = (float)((uint32_t)buffer[15] << 24 + (uint32_t)buffer[16] << 16 + (uint32_t)buffer[13] << 8 + buffer[14]);                // 4294967295 Wh
        if (!energy_start || (energy < energy_start)) { energy_start = energy; }  // Init after restart and hanlde roll-over if any
        energy_kWhtoday += (energy - energy_start) * 100;
        energy_start = energy;
        EnergyUpdateToday();
      }
    }
  }

  if (0 == pzem2_sendRetry || data_ready) {
    pzem2_sendRetry = 5;
    Pzem2ModbusSend(PZEM2_READ_RESULT, 0, pzem2_type);
  }
  else {
    pzem2_sendRetry--;
  }
}

void Pzem2SnsInit()
{
  // Software serial init needs to be done here as earlier (serial) interrupts may lead to Exceptions
  Pzem2Serial = new TasmotaSerial(pin[GPIO_PZEM2_RX], pin[GPIO_PZEM2_TX], 1);
  if (Pzem2Serial->begin(9600)) {
    if (Pzem2Serial->hardwareSerial()) { ClaimSerial(); }
  } else {
    energy_flg = ENERGY_NONE;
  }
}

void Pzem2DrvInit()
{
  if (!energy_flg) {
    if ((pin[GPIO_PZEM2_RX] < 99) && (pin[GPIO_PZEM2_TX] < 99)) {  // Any device with a Pzem-003,014,016,017
      energy_flg = XNRG_05;
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

int Xnrg05(byte function)
{
  int result = 0;

  if (FUNC_PRE_INIT == function) {
    Pzem2DrvInit();
  }
  else if (XNRG_05 == energy_flg) {
    switch (function) {
      case FUNC_INIT:
        Pzem2SnsInit();
        break;
      case FUNC_EVERY_200_MSECOND:
        Pzem2Every200ms();
        break;
    }
  }
  return result;
}

#endif  // USE_PZEM2
#endif  // USE_ENERGY_SENSOR
