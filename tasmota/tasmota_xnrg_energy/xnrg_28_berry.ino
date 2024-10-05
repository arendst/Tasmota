/*
  xnrg_28_berry.ino - Berry empty energy sensor which implementation is delegated to Berry

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

#ifdef ESP32
#ifdef USE_ENERGY_SENSOR
#ifdef USE_BERRY

/*********************************************************************************************\
 * Provides the stub for a pure Berry energy monitoring driver
 *
 * Enable by selecting any GPIO as Option A9
\*********************************************************************************************/

#define XNRG_28             28

#define NRG_BERRY_U_COMMON  true    // Phase voltage = false, Common voltage = true
#define NRG_BERRY_F_COMMON  true    // Phase frequency = false, Common frequency = true
#define NRG_BERRY_DC        false   // AC = false, DC = true;
#define NRG_BERRY_OVERTEMP  true    // Use global temperature for overtemp detection

/********************************************************************************************/

void NrgBerryEverySecond(void) {
  if (TasmotaGlobal.gpio_optiona.berry_energy) {    // active only if OPTION_A 9
    // the only required action every second is to compute the power delta for the last second from `active_power`
    for (uint32_t channel = 0; channel < Energy->phase_count; channel++) {
      Energy->kWhtoday_delta[channel] += Energy->active_power[channel] * 1000 / 36;
      Energy->data_valid[channel] = 0;              // mark data as valid to reset the energy watchdog
    }
    EnergyUpdateToday();                            // centrak management of power counters
  }
}

void NrgBerryDrvInit(void) {
  if (TasmotaGlobal.gpio_optiona.berry_energy) {    // active only if OPTION_A 9
    // set some default parameters
    Energy->voltage_common = NRG_BERRY_U_COMMON;    // Phase voltage = false, Common voltage = true
    Energy->frequency_common = NRG_BERRY_F_COMMON;  // Phase frequency = false, Common frequency = true
    Energy->type_dc = NRG_BERRY_DC;                 // AC = false, DC = true;
    Energy->use_overtemp = NRG_BERRY_OVERTEMP;      // Use global temperature for overtemp detection

    TasmotaGlobal.energy_driver = XNRG_28;    // if OPTION_A 9 is set, mark this driver as the active driver, so it can be tested in Berry
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg28(uint32_t function) {
  switch (function) {
    case FUNC_ENERGY_EVERY_SECOND:
      NrgBerryEverySecond();
      break;
    case FUNC_PRE_INIT:
      NrgBerryDrvInit();
      break;
  }
  return false;
}

#endif  // USE_BERRY
#endif  // USE_ENERGY_SENSOR
#endif  // ESP32
