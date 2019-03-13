/*
  xnrg_05_pzem_ac.ino - PZEM-014,016 Modbus AC energy sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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
#ifdef USE_PZEM_AC
/*********************************************************************************************\
 * PZEM-014 - AC 220V 10A Energy
 * PZEM-016 - AC 220V 100A Energy
 *
 * Based on:
 *   PZEM-014,016 docs https://pan.baidu.com/s/1B0MdMgURyjtO1oQa2lavKw password ytkv
 *
 * Hardware Serial will be selected if GPIO1 = [98 PZEM016 Rx] and GPIO3 = [62 PZEM0XX Tx]
\*********************************************************************************************/

#define XNRG_05                    5

#define PZEM_AC_DEVICE_ADDRESS  0x01  // PZEM default address

#include <TasmotaModbus.h>
TasmotaModbus *PzemAcModbus;

void PzemAcEverySecond(void)
{
  static uint8_t send_retry = 0;

  bool data_ready = PzemAcModbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[26];

    uint8_t error = PzemAcModbus->ReceiveBuffer(buffer, 10);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, (buffer[2]) ? buffer[2] +5 : sizeof(buffer));

    if (error) {
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "PzemAc response error %d"), error);
    } else {
      //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
      // 01 04 14 08 D1 00 6C 00 00 00 F4 00 00 00 26 00 00 01 F4 00 64 00 00 51 34
      // Id Cc Sz Volt- Current---- Power------ Energy----- Frequ PFact Alarm Crc--
      energy_voltage = (float)((buffer[3] << 8) + buffer[4]) / 10.0;                                                  // 6553.0 V
      energy_current = (float)((buffer[7] << 24) + (buffer[8] << 16) + (buffer[5] << 8) + buffer[6]) / 1000.0;        // 4294967.000 A
      energy_active_power = (float)((buffer[11] << 24) + (buffer[12] << 16) + (buffer[9] << 8) + buffer[10]) / 10.0;  // 429496729.0 W
      energy_frequency = (float)((buffer[17] << 8) + buffer[18]) / 10.0;                                              // 50.0 Hz
      energy_power_factor = (float)((buffer[19] << 8) + buffer[20]) / 100.0;                                          // 1.00
      float energy = (float)((buffer[15] << 24) + (buffer[16] << 16) + (buffer[13] << 8) + buffer[14]);               // 4294967295 Wh

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
    PzemAcModbus->Send(PZEM_AC_DEVICE_ADDRESS, 0x04, 0, 10);
  }
  else {
    send_retry--;
  }
}

void PzemAcSnsInit(void)
{
  PzemAcModbus = new TasmotaModbus(pin[GPIO_PZEM016_RX], pin[GPIO_PZEM0XX_TX]);
  uint8_t result = PzemAcModbus->Begin(9600);
  if (result) {
    if (2 == result) { ClaimSerial(); }
  } else {
    energy_flg = ENERGY_NONE;
  }
}

void PzemAcDrvInit(void)
{
  if (!energy_flg) {
    if ((pin[GPIO_PZEM016_RX] < 99) && (pin[GPIO_PZEM0XX_TX] < 99)) {
      energy_flg = XNRG_05;
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

int Xnrg05(uint8_t function)
{
  int result = 0;

  if (FUNC_PRE_INIT == function) {
    PzemAcDrvInit();
  }
  else if (XNRG_05 == energy_flg) {
    switch (function) {
      case FUNC_INIT:
        PzemAcSnsInit();
        break;
      case FUNC_EVERY_SECOND:
        PzemAcEverySecond();
        break;
    }
  }
  return result;
}

#endif  // USE_PZEM_AC
#endif  // USE_ENERGY_SENSOR
