/*
  xnrg_29_berryenergy.ino - General wrapper to allow Energy drivers in Berry

  Copyright (C) 2021  Barbudor

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

#if 0
#ifdef USE_ENERGY_SENSOR
#ifdef USE_BERRY_ENERGY_DRIVER

#define XNRG_29                     29

/********************************************************************************************
*
* This driver requires a GPIO to be assigned Option_A6
* By default it will declare the number of phases to be equal to the number of relay (max 3)
*
*********************************************************************************************/

void BerryEnergyEverySecond(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("BNRG: Every Second, data_valid=%d"), Energy.data_valid[0]);
  if (Energy.data_valid[0] <= ENERGY_WATCHDOG) {
    // Calculate energy by using active power
    for (uint32_t channel = 0; channel < Energy.phase_count; channel++) {
      Energy.kWhtoday_delta[channel] += Energy.active_power[channel] * 1000 / 36;
    }
    EnergyUpdateToday();
  }
}

bool BerryEnergyCommand(void) {
  bool serviced = false;

  uint32_t channel = XdrvMailbox.index-1;
  if (channel > Energy.phase_count-1) channel = 0;
  uint32_t value = (uint32_t)(CharToFloat(XdrvMailbox.data) * 100);  // 1.23 = 123

  if (CMND_POWERSET == Energy.command_code) {
    // if (XdrvMailbox.data_len && ?????.power[channel]) {
    //   Settings->energy_power_calibration = (?????.power[channel] * 100) / value;
    //   serviced = true;
    // }
  }
  else if (CMND_VOLTAGESET == Energy.command_code) {
    // if (XdrvMailbox.data_len && ??????.voltage) {
    //   Settings->energy_voltage_calibration = (?????.voltage * 100) / value;
    //   serviced = true;
    // }
  }
  else if (CMND_CURRENTSET == Energy.command_code) {
    // if (XdrvMailbox.data_len && ?????.current[channel]) {
    //   Settings->energy_current_calibration = (????.current[channel] * 100) / value;
    //   serviced = true;
    // }
  }

  return serviced;
}

void BerryEnergyInit(void) {
}

void BerryEnergyPreinit(void) {
  /*if (TasmotaGlobal.gpio_optiona.berry_energy) {
    TasmotaGlobal.energy_driver = XNRG_29;
    AddLog(LOG_LEVEL_INFO, PSTR("BNRG: Enable Berry Energy driver"));
  }*/
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg29(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_EVERY_SECOND:
      BerryEnergyEverySecond();
      break;
    case FUNC_COMMAND:
      BerryEnergyCommand();
      break;
    case FUNC_INIT:
      BerryEnergyInit();
      break;
    case FUNC_PRE_INIT:
      BerryEnergyPreinit();
      break;
  }
  return result;
}

#endif  // USE_BERRY_ENERGY_DRIVER
#endif  // USE_ENERGY_SENSOR
#endif // #if 0
