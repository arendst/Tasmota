/*
  xnrg_06_pzem_dc.ino - PZEM-003,017 Modbus DC energy sensor support for Tasmota

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

#ifdef USE_ENERGY_SENSOR
#ifdef USE_PZEM_DC
/*********************************************************************************************\
 * PZEM-003 - DC 300V 10A Energy
 * PZEM-017 - DC 300V 50A - 300A Energy
 *
 * Based on:
 *   PZEM-003,017 docs Https://pan.baidu.com/s/1V9bDWj3RK2u6_fbBJ3GtqQ password rq37
 *
 * Hardware Serial will be selected if GPIO1 = [62 PZEM0XX Tx] and GPIO3 = [99 PZEM017 Rx]
\*********************************************************************************************/

#define XNRG_06                    6

const uint8_t PZEM_DC_DEVICE_ADDRESS = 0x01;  // PZEM default address
const uint32_t PZEM_DC_STABILIZE = 30;        // Number of seconds to stabilize configuration

#include <TasmotaModbus.h>
TasmotaModbus *PzemDcModbus;

struct PZEMDC {
  float energy = 0;
  float last_energy = 0;
  uint8_t send_retry = 0;
  uint8_t channel = 0;
  uint8_t address = 0;
  uint8_t address_step = ADDR_IDLE;
} PzemDc;

void PzemDcEverySecond(void)
{
  bool data_ready = PzemDcModbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[26];  // At least 5 + (2 * 8) = 21

    uint8_t registers = 8;
    if (ADDR_RECEIVE == PzemDc.address_step) {
      registers = 2;     // Need 1 byte extra as response is F8 06 00 02 00 01 FD A3
      PzemDc.address_step--;
    }
    uint8_t error = PzemDcModbus->ReceiveBuffer(buffer, registers);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, PzemDcModbus->ReceiveCount());

    if (error) {
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PDC: PzemDc %d error %d"), PZEM_DC_DEVICE_ADDRESS + PzemDc.channel, error);
    } else {
      Energy.data_valid[PzemDc.channel] = 0;
      if (8 == registers) {

        //           0     1     2     3     4     5     6     7           = ModBus register
        //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  = Buffer index
        // 01 04 10 05 40 00 0A 00 0D 00 00 00 02 00 00 00 00 00 00 D6 29
        // Id Cc Sz Volt- Curre Power------ Energy----- HiAlm LoAlm Crc--
        Energy.voltage[PzemDc.channel] = (float)((buffer[3] << 8) + buffer[4]) / 100.0;                                               // 655.00 V
        Energy.current[PzemDc.channel] = (float)((buffer[5] << 8) + buffer[6]) / 100.0;                                               // 655.00 A
        Energy.active_power[PzemDc.channel] = (float)((buffer[9] << 24) + (buffer[10] << 16) + (buffer[7] << 8) + buffer[8]) / 10.0;  // 429496729.0 W

        PzemDc.energy += (float)((buffer[13] << 24) + (buffer[14] << 16) + (buffer[11] << 8) + buffer[12]);             // 4294967295 Wh
        if (PzemDc.channel == Energy.phase_count -1) {
          if (PzemDc.energy > PzemDc.last_energy) {  // Handle missed channel
            if (uptime > PZEM_DC_STABILIZE) {
              EnergyUpdateTotal(PzemDc.energy, false);
            }
            PzemDc.last_energy = PzemDc.energy;
          }
          PzemDc.energy = 0;
        }
      }
    }
  }

  if (0 == PzemDc.send_retry || data_ready) {
    if (0 == PzemDc.channel) {
      PzemDc.channel = Energy.phase_count -1;
    } else {
      PzemDc.channel--;
    }
    PzemDc.send_retry = ENERGY_WATCHDOG;
    if (ADDR_SEND == PzemDc.address_step) {
      PzemDcModbus->Send(0xF8, 0x06, 0x0002, (uint16_t)PzemDc.address);
      PzemDc.address_step--;
    } else {
      PzemDcModbus->Send(PZEM_DC_DEVICE_ADDRESS + PzemDc.channel, 0x04, 0, 8);
    }
  }
  else {
    PzemDc.send_retry--;
    if ((Energy.phase_count > 1) && (0 == PzemDc.send_retry) && (uptime < PZEM_DC_STABILIZE)) {
      Energy.phase_count--;  // Decrement channels if no response after retry within 30 seconds after restart
    }
  }
}

void PzemDcSnsInit(void)
{
  PzemDcModbus = new TasmotaModbus(Pin(GPIO_PZEM017_RX), Pin(GPIO_PZEM0XX_TX));
  uint8_t result = PzemDcModbus->Begin(9600, 2);  // Uses two stop bits!!
  if (result) {
    if (2 == result) { ClaimSerial(); }
    Energy.type_dc = true;
    Energy.phase_count = 3;  // Start off with three channels
    PzemDc.channel = 0;
  } else {
    energy_flg = ENERGY_NONE;
  }
}

void PzemDcDrvInit(void)
{
  if ((Pin(GPIO_PZEM017_RX) < 99) && (Pin(GPIO_PZEM0XX_TX) < 99)) {
    energy_flg = XNRG_06;
  }
}

bool PzemDcCommand(void)
{
  bool serviced = true;

  if (CMND_MODULEADDRESS == Energy.command_code) {
    PzemDc.address = XdrvMailbox.payload;  // Valid addresses are 1, 2 and 3
    PzemDc.address_step = ADDR_SEND;
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg06(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_ENERGY_EVERY_SECOND:
      if (uptime > 4) { PzemDcEverySecond(); }  // Fix start up issue #5875
      break;
    case FUNC_COMMAND:
      result = PzemDcCommand();
      break;
    case FUNC_INIT:
      PzemDcSnsInit();
      break;
    case FUNC_PRE_INIT:
      PzemDcDrvInit();
      break;
  }
  return result;
}

#endif  // USE_PZEM_DC
#endif  // USE_ENERGY_SENSOR
