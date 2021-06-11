/*
  xnrg_20_dummy.ino - Dummy energy sensor support for Tasmota

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

#ifdef USE_ENERGY_SENSOR
#ifdef USE_ENERGY_DUMMY
/*********************************************************************************************\
 * Provides dummy energy monitoring for up to three channels based on relay count
 *
 * User is supposed to enter valid data for Voltage, Current and Power
 * Active Power is adjusted to calculated Apparent Power (=U*I) if the latter is smaller than the first
 *
 * Enable by selecting any GPIO as Option A2
\*********************************************************************************************/

#define XNRG_20             20

#define NRG_DUMMY_U_COMMON  true    // Phase voltage = false, Common voltage = true
#define NRG_DUMMY_F_COMMON  true    // Phase frequency = false, Common frequency = true
#define NRG_DUMMY_DC        false   // AC = false, DC = true;
#define NRG_DUMMY_OVERTEMP  true    // Use global temperature for overtemp detection

#define NRG_DUMMY_UREF      24000   // Voltage 240.00 V (= P / I)
#define NRG_DUMMY_IREF      41666   // Current 0.417 A (= P / U)
#define NRG_DUMMY_PREF      10000   // Power 100.00 W (= U * I)
#define NRG_DUMMY_FREF      5000    // Frequency 50.00 Hz

/********************************************************************************************/

void NrgDummyEverySecond(void) {
  if (Energy.power_on) {  // Powered on
    float energy = 0;
    for (uint32_t channel = 0; channel < Energy.phase_count; channel++) {
      Energy.voltage[channel] = ((float)Settings->energy_voltage_calibration / 100);       // V
      Energy.frequency[channel] = ((float)Settings->energy_frequency_calibration / 100);   // Hz
      if (bitRead(TasmotaGlobal.power, channel)) {  // Emulate power read only if device is powered on
        Energy.active_power[channel] = ((float)Settings->energy_power_calibration / 100);    // W
        if (0 == Energy.active_power[channel]) {
          Energy.current[channel] = 0;
        } else {
          Energy.current[channel] = ((float)Settings->energy_current_calibration / 100000);  // A
          energy += Energy.active_power[channel];
        }
        Energy.data_valid[channel] = 0;
      }
    }

    if (energy > 0) {
      Energy.kWhtoday_delta += energy * 1000 / 36;
      EnergyUpdateToday();
    }
  }
}

bool NrgDummyCommand(void) {
  bool serviced = true;

  uint32_t value = (uint32_t)(CharToFloat(XdrvMailbox.data) * 100);  // 1.23 = 123

  if (CMND_POWERSET == Energy.command_code) {
    if (XdrvMailbox.data_len) {
      if ((value > 100) && (value < 200000)) {    // Between 1.00 and 2000.00 W
        Settings->energy_power_calibration = value;
      }
    }
  }
  else if (CMND_VOLTAGESET == Energy.command_code) {
    if (XdrvMailbox.data_len) {
      if ((value > 10000) && (value < 26000)) {   // Between 100.00 and 260.00 V
        Settings->energy_voltage_calibration = value;
      }
    }
  }
  else if (CMND_CURRENTSET == Energy.command_code) {
    if (XdrvMailbox.data_len) {
      if ((value > 1000) && (value < 1000000)) {  // Between 10.00 mA and 10.00000 A
        Settings->energy_current_calibration = value;
      }
    }
  }
  else if (CMND_FREQUENCYSET == Energy.command_code) {
    if (XdrvMailbox.data_len) {
      if ((value > 4500) && (value < 6500)) {     // Between 45.00 and 65.00 Hz
        Settings->energy_frequency_calibration = value;
      }
    }
  }
  else if (CMND_ENERGYCONFIG == Energy.command_code) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Config index %d, payload %d, data '%s'"),
      XdrvMailbox.index, XdrvMailbox.payload, XdrvMailbox.data ? XdrvMailbox.data : "null" );
  }
  else serviced = false;  // Unknown command

  return serviced;
}

void NrgDummyDrvInit(void) {
  if (TasmotaGlobal.gpio_optiona.dummy_energy && TasmotaGlobal.devices_present) {
    if (HLW_PREF_PULSE == Settings->energy_power_calibration) {
      Settings->energy_frequency_calibration = NRG_DUMMY_FREF;
      Settings->energy_voltage_calibration = NRG_DUMMY_UREF;
      Settings->energy_current_calibration = NRG_DUMMY_IREF;
      Settings->energy_power_calibration = NRG_DUMMY_PREF;
    }

    Energy.phase_count = (TasmotaGlobal.devices_present < ENERGY_MAX_PHASES) ? TasmotaGlobal.devices_present : ENERGY_MAX_PHASES;
    Energy.voltage_common = NRG_DUMMY_U_COMMON;    // Phase voltage = false, Common voltage = true
    Energy.frequency_common = NRG_DUMMY_F_COMMON;  // Phase frequency = false, Common frequency = true
    Energy.type_dc = NRG_DUMMY_DC;                 // AC = false, DC = true;
    Energy.use_overtemp = NRG_DUMMY_OVERTEMP;      // Use global temperature for overtemp detection

    TasmotaGlobal.energy_driver = XNRG_20;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg20(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_ENERGY_EVERY_SECOND:
      NrgDummyEverySecond();
      break;
    case FUNC_COMMAND:
      result = NrgDummyCommand();
      break;
    case FUNC_PRE_INIT:
      NrgDummyDrvInit();
      break;
  }
  return result;
}

#endif  // USE_ENERGY_DUMMY
#endif  // USE_ENERGY_SENSOR
