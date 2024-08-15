/*
  xnrg_06_pzem_dc.ino - PZEM-003,017 Modbus DC energy sensor support for Tasmota

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
const uint32_t PZEM_DC_STABILIZE = 10;        // Number of seconds to stabilize 1 pzem

#include <TasmotaModbus.h>

struct PZEMDC_Data {
  TasmotaModbus *modbus;
  float energy;
  float last_energy;
  uint8_t send_retry;
  uint8_t channel;
  uint8_t address;
  uint8_t range;
  uint8_t address_step;
} *PzemDc = nullptr; // Will be dynamically allocated in PzemDcDrvInit() if GPIO in use

const char PZEMDC_Commands[] PROGMEM  = "range";
enum PZEMDC_COMMANDS {            // commands for Console
  CMND_PZEMDC_RANGE=0             // Set current range register
};

void PzemDcEverySecond(void)
{
  bool data_ready = PzemDc->modbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[26];  // At least 5 + (2 * 8) = 21

    uint8_t registers = 8;
    if (ADDR_RECEIVE == PzemDc->address_step) {
      registers = 2;     // Need 1 byte extra as response is F8 06 00 02 00 01 FD A3
      PzemDc->address_step--;
    }
    uint8_t error = PzemDc->modbus->ReceiveBuffer(buffer, registers);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, PzemDc->modbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("PDC: PzemDc %d error %d"), PZEM_DC_DEVICE_ADDRESS + PzemDc->channel, error);
    } else {
      Energy->data_valid[PzemDc->channel] = 0;
      if (8 == registers) {

        //           0     1     2     3     4     5     6     7           = ModBus register
        //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  = Buffer index
        // 01 04 10 05 40 00 0A 00 0D 00 00 00 02 00 00 00 00 00 00 D6 29
        // Id Cc Sz Volt- Curre Power------ Energy----- HiAlm LoAlm Crc--
        Energy->voltage[PzemDc->channel] = (float)((buffer[3] << 8) + buffer[4]) / 100.0f;                                               // 655.00 V
        Energy->current[PzemDc->channel] = (float)((buffer[5] << 8) + buffer[6]) / 100.0f;                                               // 655.00 A
        Energy->active_power[PzemDc->channel] = (float)((buffer[9] << 24) + (buffer[10] << 16) + (buffer[7] << 8) + buffer[8]) / 10.0f;  // 429496729.0 W
        Energy->import_active[PzemDc->channel] = (float)((buffer[13] << 24) + (buffer[14] << 16) + (buffer[11] << 8) + buffer[12]) / 1000.0f;  // 4294967.295 kWh
      }
    }
    if (PzemDc->channel == Energy->phase_count -1) {
      if (TasmotaGlobal.uptime > (PZEM_DC_STABILIZE * ENERGY_MAX_PHASES)) {
        EnergyUpdateTotal();
      }
    }
  }

  if (0 == PzemDc->send_retry || data_ready) {
    if (0 == PzemDc->channel) {
      PzemDc->channel = Energy->phase_count -1;
    } else {
      PzemDc->channel--;
    }
    PzemDc->send_retry = ENERGY_WATCHDOG;
    if (PzemDc->address) {
      uint16_t addr = PzemDc->address;
      PzemDc->modbus->Send(0xF8, 0x06, 0x0002, 1, &addr);
      PzemDc->address = 0;
      PzemDc->address_step = ADDR_RECEIVE;
    } else if (PzemDc->range) {
      uint16_t range = PzemDc->range>>1;
      PzemDc->modbus->Send(0xF8, 0x06, 0x0003, 1, &range);
      PzemDc->range = 0;
      PzemDc->address_step = ADDR_RECEIVE;
    } else {
      PzemDc->modbus->Send(PZEM_DC_DEVICE_ADDRESS + PzemDc->channel, 0x04, 0, 8);
    }
  }
  else {
    PzemDc->send_retry--;
    if ((Energy->phase_count > 1) && (0 == PzemDc->send_retry) && (TasmotaGlobal.uptime < (PZEM_DC_STABILIZE * ENERGY_MAX_PHASES))) {
      Energy->phase_count--;  // Decrement channels if no response after retry within 30 seconds after restart
      if (TasmotaGlobal.discovery_counter) {
        TasmotaGlobal.discovery_counter += ENERGY_WATCHDOG + 1;  // Don't send Discovery yet, delay by 4s + 1s
      }
    }
  }
}

void PzemDcSnsInit(void)
{
  PzemDc->modbus = new TasmotaModbus(Pin(GPIO_PZEM017_RX), Pin(GPIO_PZEM0XX_TX), Pin(GPIO_NRG_MBS_TX_ENA));
  uint8_t result = PzemDc->modbus->Begin(9600, SERIAL_8N2);
  if (result) {
    if (2 == result) { ClaimSerial(); }
    Energy->type_dc = true;
    Energy->phase_count = ENERGY_MAX_PHASES;  // Start off with three channels
    PzemDc->channel = 0;
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void PzemDcDrvInit(void)
{
  if (PinUsed(GPIO_PZEM017_RX) && PinUsed(GPIO_PZEM0XX_TX)) {
    PzemDc = (struct PZEMDC_Data *)calloc(1, sizeof(struct PZEMDC_Data));
    if (!PzemDc) {
      AddLog(LOG_LEVEL_ERROR, PSTR("PDC: Memory allocation failed"));
      return;
    }
    TasmotaGlobal.energy_driver = XNRG_06;
  }
}

bool PzemDcCommand(void)
{
  bool serviced = true;

  if (CMND_MODULEADDRESS == Energy->command_code) {
    PzemDc->address = XdrvMailbox.payload;  // Valid addresses are 1, 2 and 3
  }
  else if (CMND_ENERGYCONFIG == Energy->command_code && XdrvMailbox.data_len > 0)
  {
    char *keyword, *savekeyword, *value, *savevalue, *str = XdrvMailbox.data;
    for(;;str=NULL) {
      keyword = strtok_r(str, " ,", &savekeyword);
      if (!keyword) break;
      value = strtok_r(keyword, "=", &savevalue);
      value = strtok_r(NULL, "=", &savevalue);
      if (!value) break;
      char command[CMDSZ];
      int command_code = GetCommandCode(command, sizeof(command), keyword, PZEMDC_Commands);
      switch (command_code) {
        case CMND_PZEMDC_RANGE:
          PzemDc->range = atoi(value)/50;
          break;
      }
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg06(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_ENERGY_EVERY_SECOND:
      if (TasmotaGlobal.uptime > 4) { PzemDcEverySecond(); }  // Fix start up issue #5875
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
