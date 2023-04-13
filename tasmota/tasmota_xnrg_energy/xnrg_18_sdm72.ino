/*
  xnrg_18_sdm72.ino - Eastron SDM72D-M Modbus energy meter support for Tasmota

  Copyright (C) 2021  Gennaro Tortone, Theo Arends and Norbert Richter

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
#ifdef USE_SDM72
/*********************************************************************************************\
 * Eastron SDM72 Modbus energy meter
\*********************************************************************************************/

#define XNRG_18             18

//#define SDM72_IMPEXP

// can be user defined in my_user_config.h
#ifndef SDM72_SPEED
  #define SDM72_SPEED       9600    // default SDM72 Modbus address
#endif
// can be user defined in my_user_config.h
#ifndef SDM72_ADDR
  #define SDM72_ADDR        1       // default SDM72 Modbus address
#endif

#include <TasmotaModbus.h>
TasmotaModbus *Sdm72Modbus;

const uint16_t sdm72_register[] {
  0x0000,  // Phase 1 line to neutral volts [V]
  0x0002,  // Phase 2 line to neutral volts [V]
  0x0004,  // Phase 3 line to neutral volts [V]
  0x0006,  // Phase 1 current               [A]
  0x0008,  // Phase 2 current               [A]
  0x000A,  // Phase 3 current               [A]
  0x000C,  // Phase 1 power                 [W]
  0x000E,  // Phase 2 power                 [W]
  0x0010,  // Phase 3 power                 [W]
  0x0018,  // Phase 1 volt amps reactive    [VAr]
  0x001A,  // Phase 2 volt amps reactive    [VAr]
  0x001C,  // Phase 3 volt amps reactive    [VAr]
  0x001E,  // Phase 1 power factor
  0x0020,  // Phase 2 power factor
  0x0022,  // Phase 3 power factor
  0x0046,  // Frequency of supply voltages
  0x0156,  // Total Energy                  [kWh]
#ifdef SDM72_IMPEXP
  0x0500,   // Total import active power    [W]
  0x0502,   // Total export active power    [W]
  0x0048,   // Import active energy         [kWh]
  0x004A    // Export active energy         [kWh]
#endif  //  SDM72_IMPEXP
};

struct SDM72 {
  float total_active = NAN;
#ifdef SDM72_IMPEXP
  float import_power = 0;
  float export_power = 0;
  float import_active = 0;
#endif  //  SDM72_IMPEXP
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
} Sdm72;

/*********************************************************************************************/

void Sdm72Every250ms(void)
{
  bool data_ready = Sdm72Modbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[14];  // At least 5 + (2 * 2) = 9

    uint32_t error = Sdm72Modbus->ReceiveBuffer(buffer, 2);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, Sdm72Modbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("SDM: SDM72 error %d"), error);
    } else {
      Energy->data_valid[0] = 0;
      Energy->data_valid[1] = 0;
      Energy->data_valid[2] = 0;

      float value;
      ((uint8_t*)&value)[3] = buffer[3];   // Get float values
      ((uint8_t*)&value)[2] = buffer[4];
      ((uint8_t*)&value)[1] = buffer[5];
      ((uint8_t*)&value)[0] = buffer[6];

      switch(Sdm72.read_state) {
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
          Energy->active_power[0] = value;
          break;

        case 7:
          Energy->active_power[1] = value;
          break;

        case 8:
          Energy->active_power[2] = value;
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
          Sdm72.total_active = value;         // kWh
          break;

#ifdef SDM72_IMPEXP
        case 17:
          Sdm72.import_power = value;         // W
          break;

        case 18:
          Sdm72.export_power = value;         // W
          break;

        case 19:
          Energy->import_active[0] = value;    // kWh
          break;

        case 20:
          Energy->export_active[0] = value;    // kWh
          break;
#endif  //  SDM72_IMPEXP
      }

      ++Sdm72.read_state %= nitems(sdm72_register);
      if (0 == Sdm72.read_state) {
        if (!isnan(Sdm72.total_active)) {
          Energy->import_active[0] = Sdm72.total_active;
        }
        EnergyUpdateTotal();
      }
    }
  } // end data ready

  if (0 == Sdm72.send_retry || data_ready) {
    Sdm72.send_retry = 5;
    Sdm72Modbus->Send(SDM72_ADDR, 0x04, sdm72_register[Sdm72.read_state], 2);
  } else {
    Sdm72.send_retry--;
  }
}

void Sdm72SnsInit(void)
{
  Sdm72Modbus = new TasmotaModbus(Pin(GPIO_SDM72_RX), Pin(GPIO_SDM72_TX), Pin(GPIO_NRG_MBS_TX_ENA));
  uint8_t result = Sdm72Modbus->Begin(SDM72_SPEED);
  if (result) {
    if (2 == result) {
        ClaimSerial();
    }
    Energy->phase_count = 3;
    Energy->frequency_common = true;
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Sdm72DrvInit(void)
{
  if (PinUsed(GPIO_SDM72_RX) && PinUsed(GPIO_SDM72_TX)) {
    TasmotaGlobal.energy_driver = XNRG_18;
  }
}

#ifdef SDM72_IMPEXP
void Sdm72Show(bool json) {
  if (isnan(Sdm72.total_active)) { return; }

  if (json) {
    ResponseAppend_P(PSTR(",\"" D_JSON_EXPORT_POWER "\":%s"), EnergyFmt(&Sdm72.export_power, Settings->flag2.wattage_resolution));
    ResponseAppend_P(PSTR(",\"" D_JSON_IMPORT_POWER "\":%s"), EnergyFmt(&Sdm72.import_power, Settings->flag2.wattage_resolution));
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_EXPORT_POWER, WebEnergyFmt(&Sdm72.export_power, Settings->flag2.wattage_resolution));
    WSContentSend_PD(HTTP_SNS_IMPORT_POWER, WebEnergyFmt(&Sdm72.import_power, Settings->flag2.wattage_resolution));
#endif  // USE_WEBSERVER
  }
}
#endif  // SDM72_IMPEXP

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg18(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
      Sdm72Every250ms();
      break;
#ifdef SDM72_IMPEXP
    case FUNC_JSON_APPEND:
      Sdm72Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_COL_SENSOR:
      Sdm72Show(0);
      break;
#endif  // USE_WEBSERVER
#endif  // SDM72_IMPEXP
    case FUNC_INIT:
      Sdm72SnsInit();
      break;
    case FUNC_PRE_INIT:
      Sdm72DrvInit();
      break;
  }
  return result;
}

#endif  // USE_SDM72
#endif  // USE_ENERGY_SENSOR
