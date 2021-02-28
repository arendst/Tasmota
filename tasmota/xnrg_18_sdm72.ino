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
  0x0034,   // 0 SDM72D_POWER               [W]
  0x0156,   // 3 SDM72D_TOTAL_ACTIVE        [kWh]
#ifdef SDM72_IMPEXP
  0x0500,   // 1 SDM72D_IMPORT_POWER        [W]
  0x0502,   // 2 SDM72D_EXPORT_POWER        [W]
  0x0048,   // 4 SDM72D_IMPORT_ACTIVE       [kWh]
  0x004A    // 5 SDM72D_EXPORT_ACTIVE       [kWh]
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
      Energy.data_valid[0] = 0;

      float value;
      ((uint8_t*)&value)[3] = buffer[3];   // Get float values
      ((uint8_t*)&value)[2] = buffer[4];
      ((uint8_t*)&value)[1] = buffer[5];
      ((uint8_t*)&value)[0] = buffer[6];

      switch(Sdm72.read_state) {
        case 0:
          Energy.active_power[0] = value;     // W
          break;

        case 1:
          Sdm72.total_active = value;         // kWh
          break;

#ifdef SDM72_IMPEXP
        case 2:
          Sdm72.import_power = value;         // W
          break;

        case 3:
          Sdm72.export_power = value;         // W
          break;

        case 4:
          Energy.import_active[0] = value;    // kWh
          break;

        case 5:
          Energy.export_active[0] = value;    // kWh
          break;
#endif  //  SDM72_IMPEXP
      }

      ++Sdm72.read_state %= nitems(sdm72_register);
      if (0 == Sdm72.read_state && !isnan(Sdm72.total_active)) {
        EnergyUpdateTotal(Sdm72.total_active, true);
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
  Sdm72Modbus = new TasmotaModbus(Pin(GPIO_SDM72_RX), Pin(GPIO_SDM72_TX));
  uint8_t result = Sdm72Modbus->Begin(SDM72_SPEED);
  if (result) {
    if (2 == result) {
        ClaimSerial();
    }
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Sdm72DrvInit(void)
{
  if (PinUsed(GPIO_SDM72_RX) && PinUsed(GPIO_SDM72_TX)) {
    Energy.voltage_available = false;
    Energy.current_available = false;
    TasmotaGlobal.energy_driver = XNRG_18;
  }
}

#ifdef USE_WEBSERVER
#ifdef SDM72_IMPEXP
const char HTTP_ENERGY_SDM72[] PROGMEM =
  "{s}" D_EXPORT_POWER "{m}%*_f " D_UNIT_WATT "{e}"
  "{s}" D_IMPORT_POWER "{m}%*_f " D_UNIT_WATT "{e}";
#endif  // SDM72_IMPEXP
#endif  // USE_WEBSERVER

void Sdm72Show(bool json)
{
  if (isnan(Sdm72.total_active)) { return; }

  if (json) {
#ifdef SDM72_IMPEXP
     ResponseAppend_P(PSTR(",\"" D_JSON_EXPORT_POWER "\":%*_f,\"" D_JSON_IMPORT_POWER "\":%*_f"),
      Settings.flag2.wattage_resolution, &Sdm72.export_power,
      Settings.flag2.wattage_resolution, &Sdm72.import_power);
#endif  // SDM72_IMPEXP
#ifdef USE_WEBSERVER
  } else {
#ifdef SDM72_IMPEXP
    WSContentSend_PD(HTTP_ENERGY_SDM72,
      Settings.flag2.wattage_resolution, &Sdm72.export_power,
      Settings.flag2.wattage_resolution, &Sdm72.import_power);
#endif  // SDM72_IMPEXP
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg18(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
      Sdm72Every250ms();
      break;
    case FUNC_JSON_APPEND:
      Sdm72Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Sdm72Show(0);
      break;
#endif  // USE_WEBSERVER
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
