/*
  xnrg_08_sdm120.ino - Eastron SDM120-Modbus energy meter support for Tasmota

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
#ifdef USE_SDM120
/*********************************************************************************************\
 * Eastron SDM120 or SDM220 Modbus energy meter
 *
 * Based on: https://github.com/reaper7/SDM_Energy_Meter
\*********************************************************************************************/

#define XNRG_08             8

// can be user defined in my_user_config.h
#ifndef SDM120_SPEED
  #define SDM120_SPEED      2400    // default SDM120 Modbus address
#endif
// can be user defined in my_user_config.h
#ifndef SDM120_ADDR
  #define SDM120_ADDR       1       // default SDM120 Modbus address
#endif

#include <TasmotaModbus.h>
TasmotaModbus *Sdm120Modbus;

const uint8_t sdm120_table = 8;
const uint8_t sdm220_table = 13;

const uint16_t sdm120_start_addresses[] {
  0x0000,   // SDM120C_VOLTAGE             [V]
  0x0006,   // SDM120C_CURRENT             [A]
  0x000C,   // SDM120C_POWER               [W]
  0x0012,   // SDM120C_APPARENT_POWER      [VA]
  0x0018,   // SDM120C_REACTIVE_POWER      [VAR]
  0x001E,   // SDM120C_POWER_FACTOR
  0x0046,   // SDM120C_FREQUENCY           [Hz]
  0x0156,   // SDM120C_TOTAL_ACTIVE_ENERGY [kWh]

  0X0048,   // SDM220_IMPORT_ACTIVE        [kWh]
  0X004A,   // SDM220_EXPORT_ACTIVE        [kWh]
  0X004C,   // SDM220_IMPORT_REACTIVE      [kVArh]
  0X004E,   // SDM220_EXPORT_REACTIVE      [kVArh]
  0X0024    // SDM220_PHASE_ANGLE          [Degree]
};

struct SDM120 {
  float total_active = 0;
  float import_active = NAN;
  float import_reactive = 0;
  float export_reactive = 0;
  float phase_angle = 0;
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
  uint8_t start_address_count = sdm220_table;
} Sdm120;

/*********************************************************************************************/

void SDM120Every250ms(void)
{
  bool data_ready = Sdm120Modbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[14];  // At least 5 + (2 * 2) = 9

    uint32_t error = Sdm120Modbus->ReceiveBuffer(buffer, 2);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, Sdm120Modbus->ReceiveCount());

    if (error) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("SDM: SDM120 error %d"), error);
    } else {
      Energy.data_valid[0] = 0;

      //  0  1  2  3  4  5  6  7  8
      // SA FC BC Fh Fl Sh Sl Cl Ch
      // 01 04 04 43 66 33 34 1B 38 = 230.2 Volt
      float value;
      ((uint8_t*)&value)[3] = buffer[3];   // Get float values
      ((uint8_t*)&value)[2] = buffer[4];
      ((uint8_t*)&value)[1] = buffer[5];
      ((uint8_t*)&value)[0] = buffer[6];

      switch(Sdm120.read_state) {
        case 0:
          Energy.voltage[0] = value;          // 230.2 V
          break;

        case 1:
          Energy.current[0]  = value;         // 1.260 A
          break;

        case 2:
          Energy.active_power[0] = value;     // -196.3 W
          break;

        case 3:
          Energy.apparent_power[0] = value;   // 223.4 VA
          break;

        case 4:
          Energy.reactive_power[0] = value;   // 92.2
          break;

        case 5:
          Energy.power_factor[0] = value;     // -0.91
          break;

        case 6:
          Energy.frequency[0] = value;        // 50.0 Hz
          break;

        case 7:
          Sdm120.total_active = value;     // 484.708 kWh = import_active + export_active
          break;

        case 8:
          Sdm120.import_active = value;    // 478.492 kWh
          break;

        case 9:
          Energy.export_active[0] = value;    // 6.216 kWh
          break;

        case 10:
          Sdm120.import_reactive = value;  // 172.750 kVArh
          break;

        case 11:
          Sdm120.export_reactive = value;  // 2.844 kVArh
          break;

        case 12:
          Sdm120.phase_angle = value;      // 0.00 Deg
          break;
      }

      Sdm120.read_state++;
      if (Sdm120.read_state == Sdm120.start_address_count) {
        Sdm120.read_state = 0;

        if (Sdm120.start_address_count > sdm120_table) {
          if (!isnan(Sdm120.import_active)) {
            Sdm120.total_active = Sdm120.import_active;
          } else {
            Sdm120.start_address_count = sdm120_table;  // No extended registers available
          }
        }
        EnergyUpdateTotal(Sdm120.total_active, true);  // 484.708 kWh
      }
    }
  } // end data ready

  if (0 == Sdm120.send_retry || data_ready) {
    Sdm120.send_retry = 5;
    Sdm120Modbus->Send(SDM120_ADDR, 0x04, sdm120_start_addresses[Sdm120.read_state], 2);
  } else {
    Sdm120.send_retry--;
  }
}

void Sdm120SnsInit(void)
{
  Sdm120Modbus = new TasmotaModbus(Pin(GPIO_SDM120_RX), Pin(GPIO_SDM120_TX));
  uint8_t result = Sdm120Modbus->Begin(SDM120_SPEED);
  if (result) {
    if (2 == result) { ClaimSerial(); }
  } else {
    TasmotaGlobal.energy_driver = ENERGY_NONE;
  }
}

void Sdm120DrvInit(void)
{
  if (PinUsed(GPIO_SDM120_RX) && PinUsed(GPIO_SDM120_TX)) {
    TasmotaGlobal.energy_driver = XNRG_08;
  }
}

void Sdm220Reset(void)
{
  if (isnan(Sdm120.import_active)) { return; }

  Sdm120.import_active = 0;
  Sdm120.import_reactive = 0;
  Sdm120.export_reactive = 0;
  Sdm120.phase_angle = 0;
}

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SDM220[] PROGMEM =
  "{s}" D_IMPORT_REACTIVE "{m}%s " D_UNIT_KWARH "{e}"
  "{s}" D_EXPORT_REACTIVE "{m}%s " D_UNIT_KWARH "{e}"
  "{s}" D_PHASE_ANGLE "{m}%s " D_UNIT_ANGLE "{e}";
#endif  // USE_WEBSERVER

void Sdm220Show(bool json)
{
  if (isnan(Sdm120.import_active)) { return; }

  char import_active_chr[FLOATSZ];
  dtostrfd(Sdm120.import_active, Settings->flag2.energy_resolution, import_active_chr);
  char import_reactive_chr[FLOATSZ];
  dtostrfd(Sdm120.import_reactive, Settings->flag2.energy_resolution, import_reactive_chr);
  char export_reactive_chr[FLOATSZ];
  dtostrfd(Sdm120.export_reactive, Settings->flag2.energy_resolution, export_reactive_chr);
  char phase_angle_chr[FLOATSZ];
  dtostrfd(Sdm120.phase_angle, 2, phase_angle_chr);

  if (json) {
    ResponseAppend_P(PSTR(",\"" D_JSON_IMPORT_ACTIVE "\":%s,\"" D_JSON_IMPORT_REACTIVE "\":%s,\"" D_JSON_EXPORT_REACTIVE "\":%s,\"" D_JSON_PHASE_ANGLE "\":%s"),
      import_active_chr, import_reactive_chr, export_reactive_chr, phase_angle_chr);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_ENERGY_SDM220, import_reactive_chr, export_reactive_chr, phase_angle_chr);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg08(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_250_MSECOND:
      SDM120Every250ms();
      break;
    case FUNC_JSON_APPEND:
      Sdm220Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Sdm220Show(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_ENERGY_RESET:
      Sdm220Reset();
      break;
    case FUNC_INIT:
      Sdm120SnsInit();
      break;
    case FUNC_PRE_INIT:
      Sdm120DrvInit();
      break;
  }
  return result;
}

#endif  // USE_SDM120
#endif  // USE_ENERGY_SENSOR
