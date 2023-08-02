/*
  xnrg_11_ddsu666.ino - Chint DDSU666-Modbus energy meter support for Tasmota

  Copyright (C) 2021  Pablo Zerón and Theo Arends

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
#ifdef USE_DDSU666
/*********************************************************************************************\
 * Chint DDSU666 Modbus energy meter
\*********************************************************************************************/

#define XNRG_11             11

// can be user defined in my_user_config.h
#ifndef DDSU666_SPEED
  #define DDSU666_SPEED      9600    // default DDSU66 Modbus address
#endif
// can be user defined in my_user_config.h
#ifndef DDSU666_ADDR
  #define DDSU666_ADDR       1       // default DDSU66 Modbus address
#endif

#include <TasmotaModbus.h>
TasmotaModbus *Ddsu666Modbus;

const uint16_t Ddsu666_start_addresses[] {
  0x2000,   // DDSU666_VOLTAGE             [V]
  0x2002,   // DDSU666_CURRENT             [A]
  0x2004,   // DDSU666_POWER               [KW]
  0x2006,   // DDSU666_REACTIVE_POWER      [KVAR]
  0x200A,   // DDSU666_POWER_FACTOR
  0x200E,   // DDSU666_FREQUENCY           [Hz]
  0X4000,   // DDSU666_IMPORT_ACTIVE       [kWh]
  0X400A,   // DDSU666_EXPORT_ACTIVE       [kWh]
};

struct DDSU666 {
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
} Ddsu666;

/*********************************************************************************************/

void DDSU666Every250ms(void)
{
  bool data_ready = Ddsu666Modbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[14];  // At least 5 + (2 * 2) = 9

    uint32_t error = Ddsu666Modbus->ReceiveBuffer(buffer, 2);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, Ddsu666Modbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("SDM: Ddsu666 error %d"), error);
    } else {
      Energy->data_valid[0] = 0;

      //  0  1  2  3  4  5  6  7  8
      // SA FC BC Fh Fl Sh Sl Cl Ch
      // 01 04 04 43 66 33 34 1B 38 = 230.2 Volt
      float value;
      ((uint8_t*)&value)[3] = buffer[3];   // Get float values
      ((uint8_t*)&value)[2] = buffer[4];
      ((uint8_t*)&value)[1] = buffer[5];
      ((uint8_t*)&value)[0] = buffer[6];

      switch(Ddsu666.read_state) {
        case 0:
          Energy->voltage[0] = value;          // 230.2 V
          break;

        case 1:
          Energy->current[0]  = value;         // 1.260 A
          break;

        case 2:
          Energy->active_power[0] = value * 1000;     // -196.3 W
          break;

        case 3:
          Energy->reactive_power[0] = value * 1000;   // 92.2
          break;

        case 4:
          Energy->power_factor[0] = value;     // 0.91
          break;

        case 5:
          Energy->frequency[0] = value;        // 50.0 Hz
          break;

        case 6:
          Energy->import_active[0] = value;    // 478.492 kWh
          break;

        case 7:
          Energy->export_active[0] = value;    // 6.216 kWh
          break;
      }

      Ddsu666.read_state++;

      if (Ddsu666.read_state == 8) {
        Ddsu666.read_state = 0;
        EnergyUpdateTotal();  // 484.708 kWh
      }
    }
  } // end data ready

  if (0 == Ddsu666.send_retry || data_ready) {
    Ddsu666.send_retry = 5;
    Ddsu666Modbus->Send(DDSU666_ADDR, 0x04, Ddsu666_start_addresses[Ddsu666.read_state], 2);
  } else {
    Ddsu666.send_retry--;
  }
}

void Ddsu666SnsInit(void)
{
  Ddsu666Modbus = new TasmotaModbus(Pin(GPIO_DDSU666_RX), Pin(GPIO_DDSU666_TX), Pin(GPIO_NRG_MBS_TX_ENA));
  uint8_t result = Ddsu666Modbus->Begin(DDSU666_SPEED);
  if (result) {
    if (2 == result) { ClaimSerial(); }
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Ddsu666DrvInit(void)
{
  if (PinUsed(GPIO_DDSU666_RX) && PinUsed(GPIO_DDSU666_TX)) {
    TasmotaGlobal.energy_driver = XNRG_11;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg11(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
      DDSU666Every250ms();
      break;
    case FUNC_INIT:
      Ddsu666SnsInit();
      break;
    case FUNC_PRE_INIT:
      Ddsu666DrvInit();
      break;
  }
  return result;
}

#endif  // USE_DDSU666
#endif  // USE_ENERGY_SENSOR
