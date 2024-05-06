/*
  xnrg_17_ornowe512.ino - Orno WE517-Modbus energy meter support for Tasmota

  Copyright (C) 2021  Maxime Vincent - based on the work of Gennaro Tortone and Theo Arends

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
#ifdef USE_WE517
/*********************************************************************************************\
 * Orno WE517-Modbus energy meter
 *
 * [SetOption72: Set reference used for total energy]
 * This driver supports SetOption72 = 1, which enables the use of Hardware Energy Totals,
 * (as apposed to software energy totals kept in Tasmota flash memory)
\*********************************************************************************************/

#define XNRG_17             17

// can be user defined in my_user_config.h
#ifndef WE517_SPEED
  #define WE517_SPEED      9600    // default WE517 baudrate
#endif
// can be user defined in my_user_config.h
#ifndef WE517_ADDR
  #define WE517_ADDR       1       // default WE517 Modbus address
#endif

#define FUNCTION_CODE_READ_HOLDING_REGISTERS (0x03)

#include <TasmotaModbus.h>
TasmotaModbus *We517Modbus;

const uint16_t we517_start_addresses[] {
  /*    */          // 3P4 3P3 1P2 Unit Description
  /*  0  */ 0x000E,  //  +   -   +   V    Phase 1 line to neutral volts
  /*  1  */ 0x0010,  //  +   -   -   V    Phase 2 line to neutral volts
  /*  2  */ 0x0012,  //  +   -   -   V    Phase 3 line to neutral volts
  /*  3  */ 0x0016,  //  +   +   +   A    Phase 1 current
  /*  4  */ 0x0018,  //  +   +   -   A    Phase 2 current
  /*  5  */ 0x001A,  //  +   +   -   A    Phase 3 current
  /*  6  */ 0x001E,  //  +   -   +   kW   Phase 1 power
  /*  7  */ 0x0020,  //  +   -   +   kW   Phase 2 power
  /*  8  */ 0x0022,  //  +   -   -   kW   Phase 3 power
  /*  9  */ 0x0026,  //  +   -   +   VAr  Phase 1 volt amps reactive
  /* 10  */ 0x0028,  //  +   -   -   VAr  Phase 2 volt amps reactive
  /* 11  */ 0x002A,  //  +   -   -   VAr  Phase 3 volt amps reactive
  /* 12  */ 0x0036,  //  +   -   +        Phase 1 power factor
  /* 13  */ 0x0038,  //  +   -   -        Phase 2 power factor
  /* 14  */ 0x003A,  //  +   -   -        Phase 3 power factor
  /* 15  */ 0x0014,  //  +   +   +   Hz   Frequency of supply voltages
  /* 16  */ 0x0100   //  +   +   +   kWh  Total active energy
};

struct WE517 {
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
} We517;

/*********************************************************************************************/

void WE517Every250ms(void)
{
  bool data_ready = We517Modbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[14];  // At least 5 + (2 * 2) = 9

    uint32_t error = We517Modbus->ReceiveBuffer(buffer, 2);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, We517Modbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("ORNO: WE517 error %d"), error);
    } else {
      Energy->data_valid[0] = 0;
      Energy->data_valid[1] = 0;
      Energy->data_valid[2] = 0;

      //  0  1  2  3  4  5  6  7  8
      // SA FC BC Fh Fl Sh Sl Cl Ch
      // 01 04 04 43 66 33 34 1B 38 = 230.2 Volt
      float value;
      ((uint8_t*)&value)[3] = buffer[3];   // Get float values
      ((uint8_t*)&value)[2] = buffer[4];
      ((uint8_t*)&value)[1] = buffer[5];
      ((uint8_t*)&value)[0] = buffer[6];

      switch(We517.read_state) {
        case 0:
          Energy->voltage[0] = value;
          break;

        case 1:
          Energy->voltage[1] = value;
          break;

        case 2:
          Energy->voltage[2] = value;
          break;

        case 3:
          Energy->current[0] = value;
          break;

        case 4:
          Energy->current[1] = value;
          break;

        case 5:
          Energy->current[2] = value;
          break;

        case 6:
          Energy->active_power[0] = value * 1000;
          break;

        case 7:
          Energy->active_power[1] = value * 1000;
          break;

        case 8:
          Energy->active_power[2] = value * 1000;
          break;

        case 9:
          Energy->reactive_power[0] = value;
          break;

        case 10:
          Energy->reactive_power[1] = value;
          break;

        case 11:
          Energy->reactive_power[2] = value;
          break;

        case 12:
          Energy->power_factor[0] = value;
          break;

        case 13:
          Energy->power_factor[1] = value;
          break;

        case 14:
          Energy->power_factor[2] = value;
          break;

        case 15:
          Energy->frequency[0] = value;
          break;

        case 16:
          Energy->import_active[0] = value;
          EnergyUpdateTotal();
          break;
      }

      We517.read_state++;
      if (sizeof(we517_start_addresses)/2 == We517.read_state) {
        We517.read_state = 0;
      }
    }
  } // end data ready

  if (0 == We517.send_retry || data_ready) {
    We517.send_retry = 5;
    We517Modbus->Send(WE517_ADDR, FUNCTION_CODE_READ_HOLDING_REGISTERS, we517_start_addresses[We517.read_state], 2);
  } else {
    We517.send_retry--;
  }
}

void We517SnsInit(void) {
  We517Modbus = new TasmotaModbus(Pin(GPIO_WE517_RX), Pin(GPIO_WE517_TX), Pin(GPIO_NRG_MBS_TX_ENA));
  uint8_t result = We517Modbus->Begin(WE517_SPEED, SERIAL_8E1);
  if (result) {
      if (2 == result) {
//          AddLog(LOG_LEVEL_DEBUG, PSTR("ORNO: WE517 HW serial init 8E1 at %d baud"), WE517_SPEED);
          Serial.begin(WE517_SPEED, SERIAL_8E1);
          ClaimSerial();
      }
      Energy->phase_count = 3;
      Energy->frequency_common = true; // Use common frequency
  } else {
      TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void We517DrvInit(void)
{
  if (PinUsed(GPIO_WE517_RX) && PinUsed(GPIO_WE517_TX)) {
    TasmotaGlobal.energy_driver = XNRG_17;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg17(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
      WE517Every250ms();
      break;
    case FUNC_INIT:
      We517SnsInit();
      break;
    case FUNC_PRE_INIT:
      We517DrvInit();
      break;
  }
  return result;
}

#endif  // USE_WE517
#endif  // USE_ENERGY_SENSOR
