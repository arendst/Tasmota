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

#define PZEM2_MODBUS_SPEED    9600
#define PZEM2_DEVICE_ADDRESS  0x01  // PZEM default address
#define PZEM2_READ_RESULT     0x04  // Command Read result

#define PZEM2_TYPES_003_017      8  // Result 8 x 16 bit register count
#define PZEM2_TYPES_014_016     10  // Result 10 x 16 bit register count

#include <TasmotaModbus.h>
TasmotaModbus *Pzem2Modbus;

uint8_t pzem2_type = PZEM2_TYPES_014_016;
uint8_t pzem2_sendRetry = 0;

void Pzem2EverySecond()
{
  bool data_ready = Pzem2Modbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[26];

    uint8_t error = Pzem2Modbus->ReceiveBuffer(buffer, pzem2_type);
    AddLogSerial(LOG_LEVEL_DEBUG_MORE, buffer, (buffer[2]) ? buffer[2] +5 : sizeof(buffer));

    if (error) {
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "PZEM2 response error %d"), error);
      AddLog(LOG_LEVEL_DEBUG);
//      if (9 == error) {
/*
        if (PZEM2_TYPES_014_016 == pzem2_type) {
          pzem2_type = PZEM2_TYPES_003_017;
        } else {
          pzem2_type = PZEM2_TYPES_014_016;
        }
*/
//      }
    } else {
      float energy = 0;

      if (PZEM2_TYPES_003_017 == pzem2_type) {
        energy_type_dc = true;
        //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
        // 01 04 10 27 10 00 64 03 E8 00 00 00 00 00 00 00 00 00 00 HH LL             = PZEM-017
        // Id Cc Sz Volt- Curre Power------ Energy----- HiAlm LoAlm Crc--
        energy_voltage = (float)((buffer[3] << 8) + buffer[4]) / 100.0;                                               // 655.00 V
        energy_current = (float)((buffer[5] << 8) + buffer[6]) / 100.0;                                               // 655.00 A
        energy_active_power = (float)((buffer[9] << 24) + (buffer[10] << 16) + (buffer[7] << 8) + buffer[8]) / 10.0;  // 429496729.0 W
        energy = (float)((buffer[13] << 24) + (buffer[14] << 16) + (buffer[11] << 8) + buffer[12]);                   // 4294967295 Wh
        if (!energy_start || (energy < energy_start)) { energy_start = energy; }  // Init after restart and hanlde roll-over if any
        energy_kWhtoday += (energy - energy_start) * 100;
        energy_start = energy;
        EnergyUpdateToday();
      }
      else if (PZEM2_TYPES_014_016 == pzem2_type) {   // PZEM-014,016
        energy_type_dc = false;
        //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
        // 01 04 14 08 D1 00 6C 00 00 00 F4 00 00 00 26 00 00 01 F4 00 64 00 00 51 34 = PZEM-014
        // Id Cc Sz Volt- Current---- Power------ Energy----- Frequ PFact Alarm Crc--
        energy_voltage = (float)((buffer[3] << 8) + buffer[4]) / 10.0;                                                  // 6553.0 V
        energy_current = (float)((buffer[7] << 24) + (buffer[8] << 16) + (buffer[5] << 8) + buffer[6]) / 1000.0;        // 4294967.000 A
        energy_active_power = (float)((buffer[11] << 24) + (buffer[12] << 16) + (buffer[9] << 8) + buffer[10]) / 10.0;  // 429496729.0 W
        energy_frequency = (float)((buffer[17] << 8) + buffer[18]) / 10.0;                                              // 50.0 Hz
        energy_power_factor = (float)((buffer[19] << 8) + buffer[20]) / 100.0;                                          // 1.00
        energy = (float)((buffer[15] << 24) + (buffer[16] << 16) + (buffer[13] << 8) + buffer[14]);                     // 4294967295 Wh
        if (!energy_start || (energy < energy_start)) { energy_start = energy; }  // Init after restart and hanlde roll-over if any
        energy_kWhtoday += (energy - energy_start) * 100;
        energy_start = energy;
        EnergyUpdateToday();
      }
    }
  }

  if (0 == pzem2_sendRetry || data_ready) {
    pzem2_sendRetry = 5;
    Pzem2Modbus->Send(PZEM2_DEVICE_ADDRESS, PZEM2_READ_RESULT, 0, pzem2_type);
  }
  else {
    pzem2_sendRetry--;
  }
}

void Pzem2SnsInit()
{
  Pzem2Modbus = new TasmotaModbus(pin[GPIO_PZEM2_RX], pin[GPIO_PZEM2_TX]);
  uint8_t result = Pzem2Modbus->Begin(PZEM2_MODBUS_SPEED);
  if (result) {
    if (2 == result) { ClaimSerial(); }
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
      case FUNC_EVERY_SECOND:
        Pzem2EverySecond();
        break;
    }
  }
  return result;
}

#endif  // USE_PZEM2
#endif  // USE_ENERGY_SENSOR
