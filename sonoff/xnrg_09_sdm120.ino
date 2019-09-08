/*
  xnrg_09_sdm120.ino - Eastron SDM120-Modbus energy meter support for Sonoff-Tasmota

  Copyright (C) 2019  Gennaro Tortone and Theo Arends

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
#ifdef USE_SDM120_2
/*********************************************************************************************\
 * Eastron SDM120 or SDM220 Modbus energy meter
 *
 * Based on: https://github.com/reaper7/SDM_Energy_Meter
\*********************************************************************************************/

#define XNRG_09             9

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

const uint16_t sdm120_start_addresses[] {
  0x0000,   // SDM120C_VOLTAGE  [V]
  0x0006,   // SDM120C_CURRENT  [A]
  0x000C,   // SDM120C_POWER    [W]
  0x0012,   // SDM120C_APPARENT_POWER  [VA]
  0x0018,   // SDM120C_REACTIVE_POWER  [VAR]
  0x001E,   // SDM120C_POWER_FACTOR
  0x0046,   // SDM120C_FREQUENCY  [Hz]
#ifdef USE_SDM220
  0x0156,   // SDM120C_TOTAL_ACTIVE_ENERGY  [Wh]
    0X0024, // SDM220_PHASE_ANGLE [Degre]
    0X0048, // SDM220_IMPORT_ACTIVE [kWh]
    0X004A, // SDM220_EXPORT_ACTIVE [kWh]
    0X004C, // SDM220_IMPORT_REACTIVE [kVArh]
    0X004E, // SDM220_EXPORT_REACTIVE [kVArh]
    0X0158  // SDM220 TOTAL_REACTIVE   [kVArh]
#else  // USE_SDM220
  0x0156    // SDM120C_TOTAL_ACTIVE_ENERGY  [Wh]
#endif // USE_SDM220
};

struct SDM120 {
  uint8_t read_state = 0;
  uint8_t send_retry = 0;
} Sdm120;

#ifdef USE_SDM220
struct SDM220 {
  float phase_angle = 0;
  float import_active = 0;
  float export_active = 0;
  float import_reactive = 0;
  float export_reactive = 0;
  float total_reactive = 0;
} Sdm220;
#endif  // USE_SDM220

/*********************************************************************************************/

void SDM120Every250ms(void)
{
  bool data_ready = Sdm120Modbus->ReceiveReady();

  if (data_ready) {
    uint8_t buffer[9];

    uint32_t error = Sdm120Modbus->ReceiveBuffer(buffer, 2);
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, sizeof(buffer));

    if (error) {
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "SDM120 response error %d"), error);
    } else {
      Energy.data_valid = 0;

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
          Energy.voltage = value;          // 230.2 V
          break;

        case 1:
          Energy.current  = value;         // 1.260 A
          break;

        case 2:
          Energy.active_power = value;     // -196.3 W
          break;

        case 3:
          Energy.apparent_power = value;   // 223.4 VA
          break;

        case 4:
          Energy.reactive_power = value;   // 92.2
          break;

        case 5:
          Energy.power_factor = value;     // -0.91
          break;

        case 6:
          Energy.frequency = value;        // 50.0 Hz
          break;

        case 7:
          EnergyUpdateTotal(value, true);  // 484.708 kWh
          break;

#ifdef USE_SDM220
        case 8:
          Sdm220.phase_angle = value;      // 0.00 Deg
          break;

        case 9:
          Sdm220.import_active = value;    // 478.492 kWh
          break;

        case 10:
          Sdm220.export_active = value;    // 6.216 kWh
          break;

        case 11:
          Sdm220.import_reactive = value;  // 172.750 kVArh
          break;

        case 12:
          Sdm220.export_reactive = value;  // 2.844 kVArh
          break;

        case 13:
          Sdm220.total_reactive = value;   // 175.594 kVArh
          break;
#endif  // USE_SDM220
      }

      Sdm120.read_state++;
      if (sizeof(sdm120_start_addresses)/2 == Sdm120.read_state) {
        Sdm120.read_state = 0;
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
  Sdm120Modbus = new TasmotaModbus(pin[GPIO_SDM120_RX], pin[GPIO_SDM120_TX]);
  uint8_t result = Sdm120Modbus->Begin(SDM120_SPEED);
  if (result) {
    if (2 == result) { ClaimSerial(); }
  } else {
    energy_flg = ENERGY_NONE;
  }
}

void Sdm120DrvInit(void)
{
  if (!energy_flg) {
    if ((pin[GPIO_SDM120_RX] < 99) && (pin[GPIO_SDM120_TX] < 99)) {
      energy_flg = XNRG_09;
    }
  }
}

#ifdef USE_SDM220

void Sdm220Reset(void)
{
  Sdm220.phase_angle = 0;
  Sdm220.import_active = 0;
  Sdm220.export_active = 0;
  Sdm220.import_reactive = 0;
  Sdm220.export_reactive = 0;
  Sdm220.total_reactive = 0;
}

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SDM220[] PROGMEM =
  "{s}" D_IMPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_EXPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_TOTAL_REACTIVE "{m}%s " D_UNIT_KWARH "{e}"
  "{s}" D_IMPORT_REACTIVE "{m}%s " D_UNIT_KWARH "{e}"
  "{s}" D_EXPORT_REACTIVE "{m}%s " D_UNIT_KWARH "{e}"
  "{s}" D_PHASE_ANGLE "{m}%s " D_UNIT_ANGLE "{e}";
#endif  // USE_WEBSERVER

void Sdm220Show(bool json)
{
  char import_active_chr[FLOATSZ];
  dtostrfd(Sdm220.import_active, Settings.flag2.energy_resolution, import_active_chr);
  char export_active_chr[FLOATSZ];
  dtostrfd(Sdm220.export_active, Settings.flag2.energy_resolution, export_active_chr);
  char total_reactive_chr[FLOATSZ];
  dtostrfd(Sdm220.total_reactive, Settings.flag2.energy_resolution, total_reactive_chr);
  char import_reactive_chr[FLOATSZ];
  dtostrfd(Sdm220.import_reactive, Settings.flag2.energy_resolution, import_reactive_chr);
  char export_reactive_chr[FLOATSZ];
  dtostrfd(Sdm220.export_reactive, Settings.flag2.energy_resolution, export_reactive_chr);
  char phase_angle_chr[FLOATSZ];
  dtostrfd(Sdm220.phase_angle, 2, phase_angle_chr);

  if (json) {
    ResponseAppend_P(PSTR(",\"" D_JSON_IMPORT_ACTIVE "\":%s,\"" D_JSON_EXPORT_ACTIVE "\":%s,\"" D_JSON_TOTAL_REACTIVE "\":%s,\"" D_JSON_IMPORT_REACTIVE "\":%s,\"" D_JSON_EXPORT_REACTIVE "\":%s,\"" D_JSON_PHASE_ANGLE "\":%s"),
      import_active_chr, export_active_chr, total_reactive_chr, import_reactive_chr, export_reactive_chr, phase_angle_chr);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_ENERGY_SDM220, import_active_chr, export_active_chr, total_reactive_chr, import_reactive_chr, export_reactive_chr, phase_angle_chr);
#endif  // USE_WEBSERVER
  }
}

#endif  // USE_SDM220

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

int Xnrg09(uint8_t function)
{
  int result = 0;

  if (FUNC_PRE_INIT == function) {
    Sdm120DrvInit();
  }
  else if (XNRG_09 == energy_flg) {
    switch (function) {
      case FUNC_INIT:
        Sdm120SnsInit();
        break;
      case FUNC_EVERY_250_MSECOND:
        if (uptime > 4) { SDM120Every250ms(); }
        break;

#ifdef USE_SDM220
      case FUNC_ENERGY_RESET:
        Sdm220Reset();
        break;
      case FUNC_JSON_APPEND:
        Sdm220Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Sdm220Show(0);
        break;
#endif  // USE_WEBSERVER
#endif  // USE_SDM220

    }
  }
  return result;
}

#endif  // USE_SDM120_2
#endif  // USE_ENERGY_SENSOR
