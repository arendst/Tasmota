/*
  xnrg_16_iem3000.ino - Schneider Electric iEM3000 series Modbus energy meter support for Tasmota

  Copyright (C) 2021  Marius Bezuidenhout

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
#ifdef USE_IEM3000
/*********************************************************************************************\
 * Schneider Electric iEM3000 series Modbus energy meter
 * iEM3150 / iEM3155 / iEM3250 / iEM3255 / iEM3350 / iEM3355 / iEM3455 / iEM3555
 * Important! Set meter Commnication -> Parity to None
\*********************************************************************************************/

#define XNRG_16             16

// can be user defined in my_user_config.h
#ifndef IEM3000_SPEED
  #define IEM3000_SPEED      19200   // default IEM3000 Modbus address
#endif
// can be user defined in my_user_config.h
#ifndef IEM3000_ADDR
  #define IEM3000_ADDR       1       // default IEM3000 Modbus address
#endif

#include <TasmotaModbus.h>
TasmotaModbus *Iem3000Modbus;

const uint16_t Iem3000_start_addresses[] {
            // ID                         (reg count/datatype)    [unit] Description
  0x0bb7,   //  0 . IEM3000_I1_CURRENT    (2/Float32)             [A]    I1: phase 1 current
  0x0bb9,   //  1 . IEM3000_I2_CURRENT    (2/Float32)             [A]    I2: phase 2 current
  0x0bbb,   //  2 . IEM3000_I3_CURRENT    (2/Float32)             [A]    I3: phase 3 current
  0x0bd3,   //  3 . IEM3000_L1_VOLTAGE    (2/Float32)             [V]    Voltage L1–N
  0x0bd5,   //  4 . IEM3000_L2_VOLTAGE    (2/Float32)             [V]    Voltage L2–N
  0x0bd7,   //  5 . IEM3000_L3_VOLTAGE    (2/Float32)             [V]    Voltage L3–N
  0x0bed,   //  6 . IEM3000_P1_POWER      (2/Float32)             [KW]   Active Power Phase 1
  0x0bef,   //  7 . IEM3000_P2_POWER      (2/Float32)             [KW]   Active Power Phase 2
  0x0bf1,   //  8 . IEM3000_P3_POWER      (2/Float32)             [KW]   Active Power Phase 3
  0x0c25,   //  9 . IEM3000_FREQUENCY     (2/Float32)             [Hz]   Frequency
  0xb02b,   // 10 . IEM3000_TOTAL_ACTIVE  (4/Int64)               [Wh]   Total Active Energy Import
};

struct IEM3000 {
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
} Iem3000;

/*********************************************************************************************/

void IEM3000Every250ms(void)
{
  bool data_ready = Iem3000Modbus->ReceiveReady();
  uint8_t reg_count = 4;
  if (Iem3000.read_state < 10) {
    reg_count = 2;
  }

  if (data_ready) {
    uint8_t buffer[14];  // At least 5 + sizeof(int64_t) = 13

    uint32_t error = Iem3000Modbus->ReceiveBuffer(buffer, reg_count);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, Iem3000Modbus->ReceiveCount());

    if (error) {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("SDM: Iem3000 error %d"), error);
    } else {
      Energy.data_valid[0] = 0;

      //  0  1  2  3  4  5  6  7  8
      // SA FC BC Fh Fl Sh Sl Cl Ch
      // 01 04 04 43 66 33 34 1B 38 = 230.2 Volt
      float value;
      int64_t value64;
      if(Iem3000.read_state >= 0 && Iem3000.read_state < 10) {
        ((uint8_t*)&value)[3] = buffer[3];   // Get float values
        ((uint8_t*)&value)[2] = buffer[4];
        ((uint8_t*)&value)[1] = buffer[5];
        ((uint8_t*)&value)[0] = buffer[6];
      } else {
        ((uint8_t*)&value64)[7] = buffer[3];   // Get int values
        ((uint8_t*)&value64)[6] = buffer[4];
        ((uint8_t*)&value64)[5] = buffer[5];
        ((uint8_t*)&value64)[4] = buffer[6];
        ((uint8_t*)&value64)[3] = buffer[7];
        ((uint8_t*)&value64)[2] = buffer[8];
        ((uint8_t*)&value64)[1] = buffer[9];
        ((uint8_t*)&value64)[0] = buffer[10];
      }

      switch(Iem3000.read_state) {
        case 0:
          Energy.current[0] = value;
          break;

        case 1:
          Energy.current[1] = value;
          break;

        case 2:
          Energy.current[2] = value;
          break;

        case 3:
          Energy.voltage[0]  = value;
          break;

        case 4:
          Energy.voltage[1]  = value;
          break;

        case 5:
          Energy.voltage[2]  = value;
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
          Energy.frequency[0] = value;
          break;

        case 10:
          EnergyUpdateTotal(value64  * 0.001f, true); // 1125 => 1.125
          break;
      }

      Iem3000.read_state++;
      if (sizeof(Iem3000_start_addresses)/2 == Iem3000.read_state) {
        Iem3000.read_state = 0;
      }
    }
  } // end data ready

  if (0 == Iem3000.send_retry || data_ready) {
    Iem3000.send_retry = 5;
    Iem3000Modbus->Send(IEM3000_ADDR, 0x03, Iem3000_start_addresses[Iem3000.read_state], reg_count);
  } else {
    Iem3000.send_retry--;
  }
}

void Iem3000SnsInit(void)
{
  Iem3000Modbus = new TasmotaModbus(Pin(GPIO_IEM3000_RX), Pin(GPIO_IEM3000_TX));
  uint8_t result = Iem3000Modbus->Begin(IEM3000_SPEED);
  if (result) {
    if (2 == result) { ClaimSerial(); }
    Energy.phase_count = 3;
    Energy.frequency_common = true;             // Use common frequency
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Iem3000DrvInit(void)
{
  if (PinUsed(GPIO_IEM3000_RX) && PinUsed(GPIO_IEM3000_TX)) {
    TasmotaGlobal.energy_driver = XNRG_16;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg16(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
      IEM3000Every250ms();
      break;
    case FUNC_INIT:
      Iem3000SnsInit();
      break;
    case FUNC_PRE_INIT:
      Iem3000DrvInit();
      break;
  }
  return result;
}

#endif  // USE_IEM3000
#endif  // USE_ENERGY_SENSOR
