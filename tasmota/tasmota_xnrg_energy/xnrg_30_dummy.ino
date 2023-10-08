/*
  xnrg_30_dummy.ino - Dummy energy sensor support for Tasmota

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
 * User is supposed to enter valid data for Voltage, Current and Power using commands
 *   VoltageSet 240 (= 240V), CurrentSet 0.417 (= 417mA) and PowerSet 100 (= 100W) or
 *   VoltageCal 24000 (= 240V), CurrentCal 41666 (= 0.417A) and PowerCal 10000 (= 100W)
 * Each phase or channel can be set using commands overriding above commands
 *   EnergyConfig1, EnergyConfig2 and EnergyConfig3 for Current phases (0.417 = 417mA)
 *   EnergyConfig4, EnergyConfig5 and EnergyConfig6 for Active Power phases (100 = 100W)
 *   In addition on ESP32 supporting more channels:
 *     EnergyConfig11 to 18 for Current phases 1 to 8 (0.417 = 417mA)
 *     EnergyConfig111 to 118 for Active Power phases 1 to 8 (100 = 100W)
 *
 * Active Power is adjusted to calculated Apparent Power (=U*I) if the latter is smaller than the first
 *
 * Enable by selecting any GPIO as Option A2
\*********************************************************************************************/

#define XNRG_30             30

#define NRG_DUMMY_U_COMMON  true    // Phase voltage = false, Common voltage = true
#define NRG_DUMMY_F_COMMON  true    // Phase frequency = false, Common frequency = true
#define NRG_DUMMY_DC        false   // AC = false, DC = true;
#define NRG_DUMMY_OVERTEMP  true    // Use global temperature for overtemp detection

#define NRG_DUMMY_UREF      24000   // Voltage 240.00 V (= P / I)
#define NRG_DUMMY_IREF      41666   // Current 0.417 A (= P / U)
#define NRG_DUMMY_PREF      10000   // Power 100.00 W (= U * I)
#define NRG_DUMMY_FREF      5000    // Frequency 50.00 Hz

/********************************************************************************************/

struct {
  int32_t current[ENERGY_MAX_PHASES] = { 0 };
  int32_t power[ENERGY_MAX_PHASES] = { 0 };
} NrgDummy;

void NrgDummyEverySecond(void) {
  if (Energy->power_on) {  // Powered on
    for (uint32_t channel = 0; channel < Energy->phase_count; channel++) {

      float power_calibration = (float)EnergyGetCalibration(ENERGY_POWER_CALIBRATION, channel) / 100;
      float voltage_calibration = (float)EnergyGetCalibration(ENERGY_VOLTAGE_CALIBRATION, channel) / 100;
      float current_calibration = (float)EnergyGetCalibration(ENERGY_CURRENT_CALIBRATION, channel) / 100000;
      float frequency_calibration = (float)EnergyGetCalibration(ENERGY_FREQUENCY_CALIBRATION, channel) / 100;

      if (voltage_calibration > 100) {
        Energy->voltage[channel] = voltage_calibration;    // V
      }
      Energy->frequency[channel] = (frequency_calibration > 45) ? frequency_calibration : NAN;  // Hz
      if (bitRead(TasmotaGlobal.power, channel)) {        // Emulate power read only if device is powered on
        Energy->active_power[channel] = (NrgDummy.power[channel]) ? ((float)NrgDummy.power[channel] / 1000) : power_calibration;   // W
        if (0 == Energy->active_power[channel]) {
          Energy->current[channel] = 0;
        } else {
          Energy->current[channel] = (NrgDummy.current[channel]) ? ((float)NrgDummy.current[channel] / 1000) : current_calibration;  // A
          Energy->kWhtoday_delta[channel] += Energy->active_power[channel] * 1000 / 36;
        }
        Energy->data_valid[channel] = 0;
      }
    }
    EnergyUpdateToday();
  }
}

bool NrgDummyCommand(void) {
  bool serviced = true;

  int32_t value = (int32_t)(CharToFloat(XdrvMailbox.data) * 1000);  // 1.234 = 1234, -1.234 = -1234
  uint32_t abs_value = abs(value) / 10;                             // 1.23 = 123,   -1.23 = 123

  if ((CMND_POWERCAL == Energy->command_code) || (CMND_VOLTAGECAL == Energy->command_code) || (CMND_CURRENTCAL == Energy->command_code)) {
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_POWERSET == Energy->command_code) {
    if (XdrvMailbox.data_len) {
      if ((abs_value >= 100) && (abs_value <= 16000000)) {   // Between 1.00 and 160000.00 W
        XdrvMailbox.payload = abs_value;
      }
    }
  }
  else if (CMND_VOLTAGESET == Energy->command_code) {
    if (XdrvMailbox.data_len) {
      if ((abs_value >= 10000) && (abs_value <= 40000)) {    // Between 100.00 and 400.00 V
        XdrvMailbox.payload = abs_value;
      }
    }
  }
  else if (CMND_CURRENTSET == Energy->command_code) {
    if (XdrvMailbox.data_len) {
      if ((abs_value >= 1000) && (abs_value <= 40000000)) {  // Between 10.00 mA and 400.00000 A
        XdrvMailbox.payload = abs_value;
      }
    }
  }
  else if (CMND_FREQUENCYSET == Energy->command_code) {
    if (XdrvMailbox.data_len) {
      if ((abs_value >= 4500) && (abs_value <= 6500)) {      // Between 45.00 and 65.00 Hz
        XdrvMailbox.payload = abs_value;
      }
    }
  }
  else if (CMND_ENERGYCONFIG == Energy->command_code) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Config index %d, payload %d, value %d, data '%s'"),
      XdrvMailbox.index, XdrvMailbox.payload, value, XdrvMailbox.data ? XdrvMailbox.data : "null" );

    // EnergyConfig1 to 3 = Set Energy->current[channel] in A like 0.417 for 417mA
    if ((XdrvMailbox.index > 0) && (XdrvMailbox.index < 4)) {
      NrgDummy.current[XdrvMailbox.index -1] = value;
    }
    // EnergyConfig4 to 6 = Set Energy->active_power[channel] in W like 100 for 100W
    if ((XdrvMailbox.index > 3) && (XdrvMailbox.index < 7)) {
      NrgDummy.power[XdrvMailbox.index -4] = value;
    }
#ifdef ESP32
    // EnergyConfig11 to 18 = Set Energy->current[channel] in A like 0.417 for 417mA
    if ((XdrvMailbox.index > 10) && (XdrvMailbox.index < ENERGY_MAX_PHASES + 10)) {
      NrgDummy.current[XdrvMailbox.index -1] = value;
    }
    // EnergyConfig111 to 118 = Set Energy->active_power[channel] in W like 100 for 100W
    if ((XdrvMailbox.index > 110) && (XdrvMailbox.index < ENERGY_MAX_PHASES +110)) {
      NrgDummy.power[XdrvMailbox.index -4] = value;
    }
#endif
  }
  else serviced = false;  // Unknown command

  return serviced;
}

void NrgDummyDrvInit(void) {
  uint32_t phase_count = (Settings->param[P_DUMMY_RELAYS] > 0) ? Settings->param[P_DUMMY_RELAYS] : TasmotaGlobal.devices_present;  // SetOption48 - (Energy) Support energy dummy relays
  if (TasmotaGlobal.gpio_optiona.dummy_energy && phase_count) {
    Energy->phase_count = (phase_count < ENERGY_MAX_PHASES) ? phase_count : ENERGY_MAX_PHASES;

    if (HLW_PREF_PULSE == EnergyGetCalibration(ENERGY_POWER_CALIBRATION)) {
      for (uint32_t i = 0; i < Energy->phase_count; i++) {
        EnergySetCalibration(ENERGY_POWER_CALIBRATION, NRG_DUMMY_PREF, i);
        EnergySetCalibration(ENERGY_VOLTAGE_CALIBRATION, NRG_DUMMY_UREF, i);
        EnergySetCalibration(ENERGY_CURRENT_CALIBRATION, NRG_DUMMY_IREF, i);
        EnergySetCalibration(ENERGY_FREQUENCY_CALIBRATION, NRG_DUMMY_FREF, i);
      }
    }

    Energy->voltage_common = NRG_DUMMY_U_COMMON;    // Phase voltage = false, Common voltage = true
    Energy->frequency_common = NRG_DUMMY_F_COMMON;  // Phase frequency = false, Common frequency = true
    Energy->type_dc = NRG_DUMMY_DC;                 // AC = false, DC = true;
    Energy->use_overtemp = NRG_DUMMY_OVERTEMP;      // Use global temperature for overtemp detection

    TasmotaGlobal.energy_driver = XNRG_30;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg30(uint32_t function) {
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
