/*
  xnrg_21_sdm230.ino - Eastron SDM230-Modbus energy meter support for Tasmota

  Copyright (C) 2022  dad401 and Theo Arends

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
#ifdef USE_SDM230
/*********************************************************************************************\
 * Eastron SDM230 Modbus energy meter
 *
 * Based on: existing tasmota drivers for SDM120 and SDM630,
 * https://github.com/reaper7/SDM_Energy_Meter and https://github.com/nmakel/sdm_modbus

 * manuals can be found here:
 * German:  https://bg-etech.de/download/manual/SDM230-Modbus.pdf
 * English: https://www.eastroneurope.com/images/uploads/products/manuals/SDM230_Sereis_Manual.pdf
 * Protocol Spec: https://www.eastroneurope.com/images/uploads/products/protocol/Correct_SDM230_Protocol.pdf
\*********************************************************************************************/

#define XNRG_21             21

//#define SDM230_MORE_REGS

// can be user defined in my_user_config.h
#ifndef SDM230_SPEED
  #define SDM230_SPEED      9600    // default SDM230 Modbus baudrate
#endif
// can be user defined in my_user_config.h
#ifndef SDM230_ADDR
  #define SDM230_ADDR       1       // default SDM120 Modbus address
#endif

#include <TasmotaModbus.h>
TasmotaModbus *Sdm230Modbus;

// read all default registers for generic energy driver
const uint16_t sdm230_start_addresses[] {
  0x0000,   // SDM230_VOLTAGE                             [V]
  0x0006,   // SDM230_CURRENT                             [A]
  0x000C,   // SDM230_POWER (Active)                      [W]
  0x0012,   // SDM230_POWER (Apparent) G                  [VA]
  0x0018,   // SDM230_POWER (Reactive)                    [VAr]
  0x001E,   // SDM230_POWER_FACTOR
  0x0046,   // SDM230_FREQUENCY                           [Hz]
  0X0048,   // SDM230_IMPORT_ACTIVE                       [kWh]
  0X004A,   // SDM230_EXPORT_ACTIVE                       [kWh]
//  0X0156,   // SDM230_TOTAL_ENERGY_ACTIVE                 [kWh] // = SDM230_IMPORT_ACTIVE = Energy->total

// read more registers offered by SDM230 (may cause timing issues)
#ifdef SDM230_MORE_REGS
  0x0024,   // SDM230_PHASE ANGLE                         [Degree]
  0X0056,   // SDM230_MAXIMUM_TOTAL_DEMAND_POWER_ACTIVE   [W]
  0X0180,   // SDM230_RESETTABLE_TOTAL_ENERGY_ACTIVE      [kWh]
#endif // SDM230_MORE_REGS

// for documentation / further use or implementation
//  0X0158,   // SDM230_TOTAL_ENERGY_REACTIVE               [kVArh]
//  0X0182,   // SDM230_RESETTABLE_TOTAL_ENERGY_REACTIVE    [kVArh]
//  0X004C,   // SDM230_IMPORT_REACTIVE                     [kVArh]
//  0X004E,   // SDM230_EXPORT_REACTIVE                     [kVArh]
//  0X0054,   // SDM230_TOTAL_DEMAND_POWER_ACTIVE           [W]
//  0X0058,   // SDM230_IMPORT_DEMAND_POWER_ACTIVE          [W]
//  0X005A,   // SDM230_MAXIMUM_IMPORT_DEMAND_POWER_ACTIVE  [W]
//  0X005C,   // SDM230_EXPORT_DEMAND_POWER_ACTIVE          [W]
//  0X005E,   // SDM230_MAXIMUM_EXPORT_DEMAND_POWER_ACTIVE  [W]
//  0X0102,   // SDM230_TOTAL_DEMAND_CURRENT                [A]
//  0X0108,   // SDM230_MAXIMUM_TOTAL_DEMAND_CURRENT        [A]
};

struct SDM230 {
#ifdef SDM230_MORE_REGS
  float phase_angle = 0;
  float maximum_total_demand_power_active = 0;
  float resettable_total_energy = 0;
#endif // SDM230_MORE_REGS
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
} Sdm230;

/*********************************************************************************************/

void SDM230Every250ms(void)
{
  bool data_ready = Sdm230Modbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[14];  // At least 5 + (2 * 2) = 9

    uint32_t error = Sdm230Modbus->ReceiveBuffer(buffer, 2);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, Sdm230Modbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("SDM: SDM230 error %d"), error);
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

      switch(Sdm230.read_state) {
        case 0:
          Energy->voltage[0] = value;          // 230.2 V
          break;

        case 1:
          Energy->current[0]  = value;         // 1.260 A
          break;

        case 2:
          Energy->active_power[0] = value;     // -196.3 W
          break;

        case 3:
          Energy->apparent_power[0] = value;   // 223.4 VA
          break;

        case 4:
          Energy->reactive_power[0] = value;   // 92.2
          break;

        case 5:
          Energy->power_factor[0] = value;     // -0.91
          break;

        case 6:
          Energy->frequency[0] = value;        // 50.0 Hz
          break;

        case 7:
          Energy->import_active[0] = value;     // 6.216 kWh => used in EnergyUpdateTotal()
          break;

        case 8:
          Energy->export_active[0] = value;    // 478.492 kWh
          break;

        #ifdef SDM230_MORE_REGS
        case 9:
          Sdm230.phase_angle = value;      // 0.00 Deg
          break;

        case 10:
          Sdm230.maximum_total_demand_power_active  = value;
          break;

        case 11:
          Sdm230.resettable_total_energy = value;
          break;
        #endif // SDM230_MORE_REGS
      }

      Sdm230.read_state++;
      if (sizeof(sdm230_start_addresses)/2 == Sdm230.read_state) {
        Sdm230.read_state = 0;
        EnergyUpdateTotal();                 // update every cycle after all registers have been read
      }

    }
  } // end data ready

  if (0 == Sdm230.send_retry || data_ready) {
    Sdm230.send_retry = 5;
    Sdm230Modbus->Send(SDM230_ADDR, 0x04, sdm230_start_addresses[Sdm230.read_state], 2);
  } else {
    Sdm230.send_retry--;
  }
}

void Sdm230SnsInit(void)
{
  Sdm230Modbus = new TasmotaModbus(Pin(GPIO_SDM230_RX), Pin(GPIO_SDM230_TX), Pin(GPIO_NRG_MBS_TX_ENA));
  uint8_t result = Sdm230Modbus->Begin(SDM230_SPEED);
  if (result) {
    if (2 == result) { ClaimSerial(); }
      Energy->phase_count = 1;
      Energy->frequency_common = true;             // Use common frequency
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Sdm230DrvInit(void)
{
  if (PinUsed(GPIO_SDM230_RX) && PinUsed(GPIO_SDM230_TX)) {
    TasmotaGlobal.energy_driver = XNRG_21;
  }
}

void Sdm230Reset(void)
{
  #ifdef SDM230_MORE_REGS
  Sdm230.phase_angle = 0;
  Sdm230.maximum_total_demand_power_active = 0;
  Sdm230.resettable_total_energy = 0;
  #endif // SDM230_MORE_REGS
}

#ifdef SDM230_MORE_REGS
void Sdm230Show(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"" D_JSON_PHASE_ANGLE "\":%s"), EnergyFmt(&Sdm230.phase_angle, 2));
    ResponseAppend_P(PSTR(",\"" D_JSON_POWERMAX "\":%s"), EnergyFmt(&Sdm230.maximum_total_demand_power_active, Settings->flag2.wattage_resolution));
    ResponseAppend_P(PSTR(",\"" D_JSON_RESETTABLE_TOTAL_ACTIVE "\":%s"), EnergyFmt(&Sdm230.resettable_total_energy, Settings->flag2.energy_resolution));
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_PHASE_ANGLE, WebEnergyFmt(&Sdm230.phase_angle, 2));
    WSContentSend_PD(HTTP_SNS_MAX_POWER, WebEnergyFmt(&Sdm230.maximum_total_demand_power_active, Settings->flag2.wattage_resolution));
    WSContentSend_PD(HTTP_SNS_RSTTBL_TOTAL_ACTIVE, WebEnergyFmt(&Sdm230.resettable_total_energy, Settings->flag2.energy_resolution));
#endif  // USE_WEBSERVER
  }
}
#endif  // SDM230_MORE_REGS

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg21(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
      SDM230Every250ms();
      break;
#ifdef SDM230_MORE_REGS
    case FUNC_JSON_APPEND:
      Sdm230Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_COL_SENSOR:
      Sdm230Show(0);
      break;
#endif  // USE_WEBSERVER
#endif  // SDM230_MORE_REGS
    case FUNC_ENERGY_RESET:
      Sdm230Reset();
      break;
    case FUNC_INIT:
      Sdm230SnsInit();
      break;
    case FUNC_PRE_INIT:
      Sdm230DrvInit();
      break;
  }
  return result;
}

#endif  // USE_SDM230
#endif  // USE_ENERGY_SENSOR
