/*
  xnrg_05_pzem_ac.ino - PZEM-014,016 Modbus AC energy sensor support for Tasmota

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

#ifdef USE_ENERGY_SENSOR
#ifdef USE_PZEM_AC
/*********************************************************************************************\
 * PZEM-004T V3 - AC 220V 10/100A Energy
 * PZEM-014     - AC 220V 10A Energy
 * PZEM-016     - AC 220V 100A Energy
 *
 * Based on:
 *   PZEM-014,016 docs https://pan.baidu.com/s/1B0MdMgURyjtO1oQa2lavKw password ytkv
 *
 * Hardware Serial will be selected if GPIO1 = [62 PZEM0XX Tx] and GPIO3 = [98 PZEM016 Rx]
\*********************************************************************************************/

#define XNRG_05                    5

const uint8_t PZEM_AC_DEVICE_ADDRESS = 0x01;  // PZEM default address
const uint32_t PZEM_AC_STABILIZE = 30;        // Number of seconds to stabilize configuration

#include <TasmotaModbus.h>
TasmotaModbus *PzemAcModbus;

struct PZEMAC {
  float energy = 0;
  float last_energy = 0;
  uint8_t send_retry = 0;
  uint8_t phase = 0;
  uint8_t address = 0;
  uint8_t address_step = ADDR_IDLE;
} PzemAc;

void PzemAcEverySecond(void)
{
  bool data_ready = PzemAcModbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[30];  // At least 5 + (2 * 10) = 25

    uint8_t registers = 10;
    if (ADDR_RECEIVE == PzemAc.address_step) {
      registers = 2;     // Need 1 byte extra as response is F8 06 00 02 00 01 FD A3
      PzemAc.address_step--;
    }
    uint8_t error = PzemAcModbus->ReceiveBuffer(buffer, registers);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, PzemAcModbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("PAC: PzemAc %d error %d"), PZEM_AC_DEVICE_ADDRESS + PzemAc.phase, error);
    } else {
      Energy.data_valid[PzemAc.phase] = 0;
      if (10 == registers) {

        //           0     1     2     3     4     5     6     7     8     9           = ModBus register
        //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  = Buffer index
        // 01 04 14 08 D1 00 6C 00 00 00 F4 00 00 00 26 00 00 01 F4 00 64 00 00 51 34
        // Id Cc Sz Volt- Current---- Power------ Energy----- Frequ PFact Alarm Crc--
        Energy.voltage[PzemAc.phase] = (float)((buffer[3] << 8) + buffer[4]) / 10.0;                                                  // 6553.0 V
        Energy.current[PzemAc.phase] = (float)((buffer[7] << 24) + (buffer[8] << 16) + (buffer[5] << 8) + buffer[6]) / 1000.0;        // 4294967.000 A
        Energy.active_power[PzemAc.phase] = (float)((buffer[11] << 24) + (buffer[12] << 16) + (buffer[9] << 8) + buffer[10]) / 10.0;  // 429496729.0 W
        Energy.frequency[PzemAc.phase] = (float)((buffer[17] << 8) + buffer[18]) / 10.0;                                              // 50.0 Hz
        Energy.power_factor[PzemAc.phase] = (float)((buffer[19] << 8) + buffer[20]) / 100.0;                                          // 1.00

        PzemAc.energy += (float)((buffer[15] << 24) + (buffer[16] << 16) + (buffer[13] << 8) + buffer[14]);                           // 4294967295 Wh
        if (PzemAc.phase == Energy.phase_count -1) {
          if (PzemAc.energy > PzemAc.last_energy) {  // Handle missed phase
            if (TasmotaGlobal.uptime > PZEM_AC_STABILIZE) {
              EnergyUpdateTotal(PzemAc.energy, false);
            }
            PzemAc.last_energy = PzemAc.energy;
          }
          PzemAc.energy = 0;
        }

      }
    }
  }

  if (0 == PzemAc.send_retry || data_ready) {
    if (0 == PzemAc.phase) {
      PzemAc.phase = Energy.phase_count -1;
    } else {
      PzemAc.phase--;
    }
    PzemAc.send_retry = ENERGY_WATCHDOG;
    if (ADDR_SEND == PzemAc.address_step) {
      PzemAcModbus->Send(0xF8, 0x06, 0x0002, (uint16_t)PzemAc.address);
      PzemAc.address_step--;
    } else {
      PzemAcModbus->Send(PZEM_AC_DEVICE_ADDRESS + PzemAc.phase, 0x04, 0, 10);
    }
  }
  else {
    PzemAc.send_retry--;
    if ((Energy.phase_count > 1) && (0 == PzemAc.send_retry) && (TasmotaGlobal.uptime < PZEM_AC_STABILIZE)) {
      Energy.phase_count--;  // Decrement phases if no response after retry within 30 seconds after restart
      if (TasmotaGlobal.discovery_counter) {
        TasmotaGlobal.discovery_counter += ENERGY_WATCHDOG + 1;  // Don't send Discovery yet, delay by 4s + 1s
      }
    }
  }
}

void PzemAcSnsInit(void)
{
  PzemAcModbus = new TasmotaModbus(Pin(GPIO_PZEM016_RX), Pin(GPIO_PZEM0XX_TX));
  uint8_t result = PzemAcModbus->Begin(9600);
  if (result) {
    if (2 == result) { ClaimSerial(); }
    Energy.phase_count = ENERGY_MAX_PHASES;  // Start off with three phases
    PzemAc.phase = 0;
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void PzemAcDrvInit(void)
{
  if (PinUsed(GPIO_PZEM016_RX) && PinUsed(GPIO_PZEM0XX_TX)) {
    TasmotaGlobal.energy_driver = XNRG_05;
  }
}

bool PzemAcCommand(void)
{
  bool serviced = true;

  if (CMND_MODULEADDRESS == Energy.command_code) {
    PzemAc.address = XdrvMailbox.payload;  // Valid addresses are 1, 2 and 3
    PzemAc.address_step = ADDR_SEND;
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg05(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_ENERGY_EVERY_SECOND:
      if (TasmotaGlobal.uptime > 4) { PzemAcEverySecond(); }  // Fix start up issue #5875
      break;
    case FUNC_COMMAND:
      result = PzemAcCommand();
      break;
    case FUNC_INIT:
      PzemAcSnsInit();
      break;
    case FUNC_PRE_INIT:
      PzemAcDrvInit();
      break;
  }
  return result;
}

#endif  // USE_PZEM_AC
#endif  // USE_ENERGY_SENSOR
