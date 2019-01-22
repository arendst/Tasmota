/*
  xnrg_06_pzem_dc.ino - PZEM-003,017 Modbus DC energy sensor support for Sonoff-Tasmota

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
#ifdef USE_PZEM_DC
/*********************************************************************************************\
 * PZEM-003 - DC 300V 10A Energy
 * PZEM-017 - DC 300V 50A - 300A Energy
 *
 * Based on:
 *   PZEM-003,017 docs Https://pan.baidu.com/s/1V9bDWj3RK2u6_fbBJ3GtqQ password rq37
 *
 * Hardware Serial will be selected if GPIO1 = [99 PZEM017 Rx] and GPIO3 = [62 PZEM0XX Tx]
\*********************************************************************************************/

#define XNRG_06                    6

#define PZEM_DC_DEVICE_ADDRESS  0x01  // PZEM default address

#include <TasmotaModbus.h>
TasmotaModbus *PzemDcModbus;

void PzemDcEverySecond(void)
{
  static uint8_t send_retry = 0;

  bool data_ready = PzemDcModbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[22];

    uint8_t error = PzemDcModbus->ReceiveBuffer(buffer, 8);
    AddLogSerial(LOG_LEVEL_DEBUG_MORE, buffer, (buffer[2]) ? buffer[2] +5 : sizeof(buffer));

    if (error) {
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "PzemDc response error %d"), error);
      AddLog(LOG_LEVEL_DEBUG);
    } else {
      //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
      // 01 04 10 05 40 00 0A 00 0D 00 00 00 02 00 00 00 00 00 00 D6 29
      // Id Cc Sz Volt- Curre Power------ Energy----- HiAlm LoAlm Crc--
      energy_voltage = (float)((buffer[3] << 8) + buffer[4]) / 100.0;                                               // 655.00 V
      energy_current = (float)((buffer[5] << 8) + buffer[6]) / 100.0;                                               // 655.00 A
      energy_active_power = (float)((buffer[9] << 24) + (buffer[10] << 16) + (buffer[7] << 8) + buffer[8]) / 10.0;  // 429496729.0 W
      float energy = (float)((buffer[13] << 24) + (buffer[14] << 16) + (buffer[11] << 8) + buffer[12]);             // 4294967295 Wh

      if (!energy_start || (energy < energy_start)) { energy_start = energy; }  // Init after restart and handle roll-over if any
      if (energy != energy_start) {
        energy_kWhtoday += (unsigned long)((energy - energy_start) * 100);
        energy_start = energy;
      }
      EnergyUpdateToday();
    }
  }

  if (0 == send_retry || data_ready) {
    send_retry = 5;
    PzemDcModbus->Send(PZEM_DC_DEVICE_ADDRESS, 0x04, 0, 8);
  }
  else {
    send_retry--;
  }
}

void PzemDcSnsInit(void)
{
  PzemDcModbus = new TasmotaModbus(pin[GPIO_PZEM017_RX], pin[GPIO_PZEM0XX_TX]);
  uint8_t result = PzemDcModbus->Begin(9600, 2);  // Uses two stop bits!!
  if (result) {
    if (2 == result) { ClaimSerial(); }
    energy_type_dc = true;
  } else {
    energy_flg = ENERGY_NONE;
  }
}

void PzemDcDrvInit(void)
{
  if (!energy_flg) {
    if ((pin[GPIO_PZEM017_RX] < 99) && (pin[GPIO_PZEM0XX_TX] < 99)) {
      energy_flg = XNRG_06;
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

int Xnrg06(byte function)
{
  int result = 0;

  if (FUNC_PRE_INIT == function) {
    PzemDcDrvInit();
  }
  else if (XNRG_06 == energy_flg) {
    switch (function) {
      case FUNC_INIT:
        PzemDcSnsInit();
        break;
      case FUNC_EVERY_SECOND:
        PzemDcEverySecond();
        break;
    }
  }
  return result;
}

#endif  // USE_PZEM_DC
#endif  // USE_ENERGY_SENSOR
