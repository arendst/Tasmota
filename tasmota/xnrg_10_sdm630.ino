/*
  xnrg_10_sdm630.ino - Eastron SDM630-Modbus energy meter support for Tasmota

  Copyright (C) 2021  Gennaro Tortone and Theo Arends

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
#ifdef USE_SDM630
/*********************************************************************************************\
 * Eastron SDM630-Modbus energy meter
 *
 * Based on: https://github.com/reaper7/SDM_Energy_Meter
\*********************************************************************************************/

#define XNRG_10             10

// can be user defined in my_user_config.h
#ifndef SDM630_SPEED
  #define SDM630_SPEED      9600    // default SDM630 Modbus address
#endif
// can be user defined in my_user_config.h
#ifndef SDM630_ADDR
  #define SDM630_ADDR       1       // default SDM630 Modbus address
#endif

#include <TasmotaModbus.h>
TasmotaModbus *Sdm630Modbus;

const uint16_t sdm630_start_addresses[] {
           // 3P4 3P3 1P2 Unit Description
  0x0000,  //  +   -   +   V    Phase 1 line to neutral volts
  0x0002,  //  +   -   -   V    Phase 2 line to neutral volts
  0x0004,  //  +   -   -   V    Phase 3 line to neutral volts
  0x0006,  //  +   +   +   A    Phase 1 current
  0x0008,  //  +   +   -   A    Phase 2 current
  0x000A,  //  +   +   -   A    Phase 3 current
  0x000C,  //  +   -   +   W    Phase 1 power
  0x000E,  //  +   -   +   W    Phase 2 power
  0x0010,  //  +   -   -   W    Phase 3 power
  0x0018,  //  +   -   +   VAr  Phase 1 volt amps reactive
  0x001A,  //  +   -   -   VAr  Phase 2 volt amps reactive
  0x001C,  //  +   -   -   VAr  Phase 3 volt amps reactive
  0x001E,  //  +   -   +        Phase 1 power factor
  0x0020,  //  +   -   -        Phase 2 power factor
  0x0022,  //  +   -   -        Phase 3 power factor
  0x0046,  //  +   +   +   Hz   Frequency of supply voltages
  0x0160,  //  +   +   +   kWh  Phase 1 export active energy
  0x0162,  //  +   +   +   kWh  Phase 2 export active energy
  0x0164,  //  +   +   +   kWh  Phase 3 export active energy
#ifdef SDM630_IMPORT
  0x015A,  //  +   +   +   kWh  Phase 1 import active energy
  0x015C,  //  +   +   +   kWh  Phase 2 import active energy
  0x015E,  //  +   +   +   kWh  Phase 3 import active energy
#endif  // SDM630_IMPORT
  0x0156   //  +   +   +   kWh  Total active energy
};

struct SDM630 {
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
} Sdm630;

/*********************************************************************************************/

void SDM630Every250ms(void)
{
  bool data_ready = Sdm630Modbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[14];  // At least 5 + (2 * 2) = 9

    uint32_t error = Sdm630Modbus->ReceiveBuffer(buffer, 2);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, Sdm630Modbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("SDM: SDM630 error %d"), error);
    } else {
      Energy.data_valid[0] = 0;
      Energy.data_valid[1] = 0;
      Energy.data_valid[2] = 0;

      //  0  1  2  3  4  5  6  7  8
      // SA FC BC Fh Fl Sh Sl Cl Ch
      // 01 04 04 43 66 33 34 1B 38 = 230.2 Volt
      float value;
      ((uint8_t*)&value)[3] = buffer[3];   // Get float values
      ((uint8_t*)&value)[2] = buffer[4];
      ((uint8_t*)&value)[1] = buffer[5];
      ((uint8_t*)&value)[0] = buffer[6];

      switch(Sdm630.read_state) {
        case 0:
          Energy.voltage[0] = value;
          break;

        case 1:
          Energy.voltage[1] = value;
          break;

        case 2:
          Energy.voltage[2] = value;
          break;

        case 3:
          Energy.current[0] = value;
          break;

        case 4:
          Energy.current[1] = value;
          break;

        case 5:
          Energy.current[2] = value;
          break;

        case 6:
          Energy.active_power[0] = value;
          break;

        case 7:
          Energy.active_power[1] = value;
          break;

        case 8:
          Energy.active_power[2] = value;
          break;

        case 9:
          Energy.reactive_power[0] = value;
          break;

        case 10:
          Energy.reactive_power[1] = value;
          break;

        case 11:
          Energy.reactive_power[2] = value;
          break;

        case 12:
          Energy.power_factor[0] = value;
          break;

        case 13:
          Energy.power_factor[1] = value;
          break;

        case 14:
          Energy.power_factor[2] = value;
          break;

        case 15:
          Energy.frequency[0] = value;
          break;

        case 16:
          Energy.export_active[0] = value;
          break;

        case 17:
          Energy.export_active[1] = value;
          break;

        case 18:
          Energy.export_active[2] = value;
          break;

        case 19:
#ifdef SDM630_IMPORT
          Energy.import_active[0] = value;
          break;

        case 20:
          Energy.import_active[1] = value;
          break;

        case 21:
          Energy.import_active[2] = value;
          break;

        case 22:
#endif  // SDM630_IMPORT
          EnergyUpdateTotal(value, true);
          break;
      }

      Sdm630.read_state++;
      if (sizeof(sdm630_start_addresses)/2 == Sdm630.read_state) {
        Sdm630.read_state = 0;
      }
    }
  } // end data ready

  if (0 == Sdm630.send_retry || data_ready) {
    Sdm630.send_retry = 5;
    Sdm630Modbus->Send(SDM630_ADDR, 0x04, sdm630_start_addresses[Sdm630.read_state], 2);
  } else {
    Sdm630.send_retry--;
  }
}

void Sdm630SnsInit(void)
{
  Sdm630Modbus = new TasmotaModbus(Pin(GPIO_SDM630_RX), Pin(GPIO_SDM630_TX));
  uint8_t result = Sdm630Modbus->Begin(SDM630_SPEED);
  if (result) {
    if (2 == result) { ClaimSerial(); }
    Energy.phase_count = 3;
    Energy.frequency_common = true;             // Use common frequency
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Sdm630DrvInit(void)
{
  if (PinUsed(GPIO_SDM630_RX) && PinUsed(GPIO_SDM630_TX)) {
    TasmotaGlobal.energy_driver = XNRG_10;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg10(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
      SDM630Every250ms();
      break;
    case FUNC_INIT:
      Sdm630SnsInit();
      break;
    case FUNC_PRE_INIT:
      Sdm630DrvInit();
      break;
  }
  return result;
}

#endif  // USE_SDM630
#endif  // USE_ENERGY_SENSOR
