/*
  xdrv_03_energy.ino - Energy sensor support for Tasmota

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
/*********************************************************************************************\
 * Energy
\*********************************************************************************************/

#define XDRV_03                3
#define XSNS_03                3

//#define USE_ENERGY_MARGIN_DETECTION
//  #define USE_ENERGY_POWER_LIMIT

#define ENERGY_NONE            0
#define ENERGY_WATCHDOG        4        // Allow up to 4 seconds before deciding no valid data present
#define ENERGY_MAX_PHASES      3

#include <Ticker.h>

#define D_CMND_POWERCAL "PowerCal"
#define D_CMND_VOLTAGECAL "VoltageCal"
#define D_CMND_CURRENTCAL "CurrentCal"
#define D_CMND_FREQUENCYCAL "FrequencyCal"
#define D_CMND_TARIFF "Tariff"
#define D_CMND_MODULEADDRESS "ModuleAddress"

enum EnergyCommands {
  CMND_POWERCAL, CMND_VOLTAGECAL, CMND_CURRENTCAL, CMND_FREQUENCYCAL,
  CMND_POWERSET, CMND_VOLTAGESET, CMND_CURRENTSET, CMND_FREQUENCYSET, CMND_MODULEADDRESS, CMND_ENERGYCONFIG };

const char kEnergyCommands[] PROGMEM = "|"  // No prefix
  D_CMND_POWERCAL "|" D_CMND_VOLTAGECAL "|" D_CMND_CURRENTCAL "|" D_CMND_FREQUENCYCAL "|"
  D_CMND_POWERSET "|" D_CMND_VOLTAGESET "|" D_CMND_CURRENTSET "|" D_CMND_FREQUENCYSET "|" D_CMND_MODULEADDRESS "|" D_CMND_ENERGYCONFIG "|"
#ifdef USE_ENERGY_MARGIN_DETECTION
  D_CMND_POWERDELTA "|" D_CMND_POWERLOW "|" D_CMND_POWERHIGH "|" D_CMND_VOLTAGELOW "|" D_CMND_VOLTAGEHIGH "|" D_CMND_CURRENTLOW "|" D_CMND_CURRENTHIGH "|"
#ifdef USE_ENERGY_POWER_LIMIT
  D_CMND_MAXENERGY "|" D_CMND_MAXENERGYSTART "|"
  D_CMND_MAXPOWER "|" D_CMND_MAXPOWERHOLD "|" D_CMND_MAXPOWERWINDOW "|"
  D_CMND_SAFEPOWER "|" D_CMND_SAFEPOWERHOLD "|"  D_CMND_SAFEPOWERWINDOW "|"
#endif  // USE_ENERGY_POWER_LIMIT
#endif  // USE_ENERGY_MARGIN_DETECTION
  D_CMND_ENERGYTODAY "|" D_CMND_ENERGYYESTERDAY "|" D_CMND_ENERGYTOTAL "|" D_CMND_ENERGYUSAGE "|" D_CMND_ENERGYEXPORT "|" D_CMND_TARIFF;

void (* const EnergyCommand[])(void) PROGMEM = {
  &CmndPowerCal, &CmndVoltageCal, &CmndCurrentCal, &CmndFrequencyCal,
  &CmndPowerSet, &CmndVoltageSet, &CmndCurrentSet, &CmndFrequencySet, &CmndModuleAddress, &CmndEnergyConfig,
#ifdef USE_ENERGY_MARGIN_DETECTION
  &CmndPowerDelta, &CmndPowerLow, &CmndPowerHigh, &CmndVoltageLow, &CmndVoltageHigh, &CmndCurrentLow, &CmndCurrentHigh,
#ifdef USE_ENERGY_POWER_LIMIT
  &CmndMaxEnergy, &CmndMaxEnergyStart,
  &CmndMaxPower, &CmndMaxPowerHold, &CmndMaxPowerWindow,
  &CmndSafePower, &CmndSafePowerHold, &CmndSafePowerWindow,
#endif  // USE_ENERGY_POWER_LIMIT
#endif  // USE_ENERGY_MARGIN_DETECTION
  &CmndEnergyToday, &CmndEnergyYesterday, &CmndEnergyTotal, &CmndEnergyUsage, &CmndEnergyExport, &CmndTariff};

const char kEnergyPhases[] PROGMEM = "|%*_f / %*_f|%*_f / %*_f / %*_f||[%*_f,%*_f]|[%*_f,%*_f,%*_f]";

struct ENERGY {
  float voltage[ENERGY_MAX_PHASES];             // 123.1 V
  float current[ENERGY_MAX_PHASES];             // 123.123 A
  float active_power[ENERGY_MAX_PHASES];        // 123.1 W
  float apparent_power[ENERGY_MAX_PHASES];      // 123.1 VA
  float reactive_power[ENERGY_MAX_PHASES];      // 123.1 VAr
  float power_factor[ENERGY_MAX_PHASES];        // 0.12
  float frequency[ENERGY_MAX_PHASES];           // 123.1 Hz
  float import_active[ENERGY_MAX_PHASES];       // 123.123 kWh
  float export_active[ENERGY_MAX_PHASES];       // 123.123 kWh
  float start_energy[ENERGY_MAX_PHASES];        // 12345.12345 kWh total previous
  float daily[ENERGY_MAX_PHASES];               // 123.123 kWh
  float total[ENERGY_MAX_PHASES];               // 12345.12345 kWh total energy
  float daily_sum;                              // 123.123 kWh
  float total_sum;                              // 12345.12345 kWh total energy
  float yesterday_sum;                          // 123.123 kWh

  int32_t kWhtoday_delta[ENERGY_MAX_PHASES];    // 1212312345 Wh 10^-5 (deca micro Watt hours) - Overflows to Energy.kWhtoday (HLW and CSE only)
  int32_t kWhtoday_offset[ENERGY_MAX_PHASES];   // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily
  int32_t kWhtoday[ENERGY_MAX_PHASES];          // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily
  int32_t period[ENERGY_MAX_PHASES];            // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily

  uint8_t fifth_second;
  uint8_t command_code;
  uint8_t data_valid[ENERGY_MAX_PHASES];

  uint8_t phase_count;                          // Number of phases active
  bool voltage_common;                          // Use common voltage
  bool frequency_common;                        // Use common frequency
  bool use_overtemp;                            // Use global temperature as overtemp trigger on internal energy monitor hardware
  bool kWhtoday_offset_init;

  bool voltage_available;                       // Enable if voltage is measured
  bool current_available;                       // Enable if current is measured

  bool type_dc;
  bool power_on;

#ifdef USE_ENERGY_MARGIN_DETECTION
  uint16_t power_history[ENERGY_MAX_PHASES][3];
  uint8_t power_steady_counter;                 // Allow for power on stabilization
  bool min_power_flag;
  bool max_power_flag;
  bool min_voltage_flag;
  bool max_voltage_flag;
  bool min_current_flag;
  bool max_current_flag;

#ifdef USE_ENERGY_POWER_LIMIT
  uint16_t mplh_counter;
  uint16_t mplw_counter;
  uint8_t mplr_counter;
  uint8_t max_energy_state;
#endif  // USE_ENERGY_POWER_LIMIT
#endif  // USE_ENERGY_MARGIN_DETECTION
} Energy;

Ticker ticker_energy;

/********************************************************************************************/

char* EnergyFormatIndex(char* result, float* input, uint32_t resolution, bool json, uint32_t index, bool single = false) {
  char layout[20];
  GetTextIndexed(layout, sizeof(layout), (index -1) + (ENERGY_MAX_PHASES * json), kEnergyPhases);
  switch (index) {
    case 2:
      ext_snprintf_P(result, FLOATSZ * ENERGY_MAX_PHASES, layout, resolution, &input[0], resolution, &input[1]);
      break;
    case 3:
      ext_snprintf_P(result, FLOATSZ * ENERGY_MAX_PHASES, layout, resolution, &input[0], resolution, &input[1], resolution, &input[2]);
      break;
    default:
      ext_snprintf_P(result, FLOATSZ * ENERGY_MAX_PHASES, PSTR("%*_f"), resolution, &input[0]);
  }
  return result;
}

char* EnergyFormat(char* result, float* input, uint32_t resolution, bool json, bool single = false) {
  uint8_t index = (single) ? 1 : Energy.phase_count;  // 1,2,3
  return EnergyFormatIndex(result, input, resolution, json, index, single);
}

char* EnergyFormatSum(char* result, float* input, uint32_t resolution, bool json, bool single = false) {
  uint8_t index = (single) ? 1 : Energy.phase_count;  // 1,2,3
  float input_sum = 0.0;
  if (!Settings->flag5.energy_phase) {
    for (uint32_t i = 0; i < index; i++) {
      input_sum += input[i];
    }
    input = &input_sum;
    index = 1;
  }
  return EnergyFormatIndex(result, input, resolution, json, index, single);
}

/********************************************************************************************/

bool EnergyTariff1Active()  // Off-Peak hours
{
  uint8_t dst = 0;
  if (IsDst() && (Settings->tariff[0][1] != Settings->tariff[1][1])) {
    dst = 1;
  }
  if (Settings->tariff[0][dst] != Settings->tariff[1][dst]) {
    if (Settings->flag3.energy_weekend && ((RtcTime.day_of_week == 1) ||   // CMND_TARIFF
                                          (RtcTime.day_of_week == 7))) {
      return true;
    }
    uint32_t minutes = MinutesPastMidnight();
    if (Settings->tariff[0][dst] > Settings->tariff[1][dst]) {
      // {"Tariff":{"Off-Peak":{"STD":"22:00","DST":"23:00"},"Standard":{"STD":"06:00","DST":"07:00"},"Weekend":"OFF"}}
      return ((minutes >= Settings->tariff[0][dst]) || (minutes < Settings->tariff[1][dst]));
    } else {
      // {"Tariff":{"Off-Peak":{"STD":"00:29","DST":"01:29"},"Standard":{"STD":"07:29","DST":"08:29"},"Weekend":"OFF"}}
      return ((minutes >= Settings->tariff[0][dst]) && (minutes < Settings->tariff[1][dst]));
    }
  } else {
    return false;
  }
}

void EnergyUpdateToday(void) {
  Energy.total_sum = 0.0;
  Energy.yesterday_sum = 0.0;
  Energy.daily_sum = 0.0;

  for (uint32_t i = 0; i < Energy.phase_count; i++) {
    if (abs(Energy.kWhtoday_delta[i]) > 1000) {
      int32_t delta = Energy.kWhtoday_delta[i] / 1000;
      Energy.kWhtoday_delta[i] -= (delta * 1000);
      Energy.kWhtoday[i] += delta;
    }

    RtcSettings.energy_kWhtoday_ph[i] = Energy.kWhtoday_offset[i] + Energy.kWhtoday[i];
    Energy.daily[i] = (float)(RtcSettings.energy_kWhtoday_ph[i]) / 100000;
    Energy.total[i] = (float)(RtcSettings.energy_kWhtotal_ph[i] + RtcSettings.energy_kWhtoday_ph[i]) / 100000;

    Energy.total_sum += Energy.total[i];
    Energy.yesterday_sum += (float)Settings->energy_kWhyesterday_ph[i] / 100000;
    Energy.daily_sum += Energy.daily[i];
  }

  if (RtcTime.valid){ // We calc the difference only if we have a valid RTC time.

    uint32_t energy_diff = (uint32_t)(Energy.total_sum * 100000) - RtcSettings.energy_usage.last_usage_kWhtotal;
    RtcSettings.energy_usage.last_usage_kWhtotal = (uint32_t)(Energy.total_sum * 100000);

    uint32_t return_diff = 0;
    if (!isnan(Energy.export_active[0])) {
//      return_diff = (uint32_t)(Energy.export_active * 100000) - RtcSettings.energy_usage.last_return_kWhtotal;
//      RtcSettings.energy_usage.last_return_kWhtotal = (uint32_t)(Energy.export_active * 100000);

      float export_active = 0.0;
      for (uint32_t i = 0; i < Energy.phase_count; i++) {
        if (!isnan(Energy.export_active[i])) {
          export_active += Energy.export_active[i];
        }
      }
      return_diff = (uint32_t)(export_active * 100000) - RtcSettings.energy_usage.last_return_kWhtotal;
      RtcSettings.energy_usage.last_return_kWhtotal = (uint32_t)(export_active * 100000);
    }

    if (EnergyTariff1Active()) {  // Tarrif1 = Off-Peak
      RtcSettings.energy_usage.usage1_kWhtotal += energy_diff;
      RtcSettings.energy_usage.return1_kWhtotal += return_diff;
    } else {
      RtcSettings.energy_usage.usage2_kWhtotal += energy_diff;
      RtcSettings.energy_usage.return2_kWhtotal += return_diff;
    }
  }
}

void EnergyUpdateTotal(void) {
  // Provide total import active energy as float Energy.import_active[phase] in kWh: 98Wh = 0.098kWh

  for (uint32_t i = 0; i < Energy.phase_count; i++) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NRG: EnergyTotal[%d] %4_f kWh"), i, &Energy.import_active[i]);

    if (0 == Energy.start_energy[i] || (Energy.import_active[i] < Energy.start_energy[i])) {
      Energy.start_energy[i] = Energy.import_active[i];  // Init after restart and handle roll-over if any
    }
    else if (Energy.import_active[i] != Energy.start_energy[i]) {
      Energy.kWhtoday[i] = (int32_t)((Energy.import_active[i] - Energy.start_energy[i]) * 100000);
    }

    if ((Energy.total[i] < (Energy.import_active[i] - 0.01)) &&   // We subtract a little offset to avoid continuous updates
        Settings->flag3.hardware_energy_total) {    // SetOption72 - Enable hardware energy total counter as reference (#6561)
      RtcSettings.energy_kWhtotal_ph[i] = (int32_t)((Energy.import_active[i] * 100000) - Energy.kWhtoday_offset[i] - Energy.kWhtoday[i]);
      Settings->energy_kWhtotal_ph[i] = RtcSettings.energy_kWhtotal_ph[i];
      Energy.total[i] = (float)(RtcSettings.energy_kWhtotal_ph[i] + Energy.kWhtoday_offset[i] + Energy.kWhtoday[i]) / 100000;
      Settings->energy_kWhtotal_time = (!Energy.kWhtoday_offset[i]) ? LocalTime() : Midnight();
  //    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Energy Total updated with hardware value"));
    }
  }

  EnergyUpdateToday();
}

/*********************************************************************************************/

void Energy200ms(void)
{
  Energy.power_on = (TasmotaGlobal.power != 0) | Settings->flag.no_power_on_check;  // SetOption21 - Show voltage even if powered off

  Energy.fifth_second++;
  if (5 == Energy.fifth_second) {
    Energy.fifth_second = 0;

    XnrgCall(FUNC_ENERGY_EVERY_SECOND);

    if (RtcTime.valid) {

      if (!Energy.kWhtoday_offset_init && (RtcTime.day_of_year == Settings->energy_kWhdoy)) {
        for (uint32_t i = 0; i < 3; i++) {
          Energy.kWhtoday_offset[i] = Settings->energy_kWhtoday_ph[i];
        }
        Energy.kWhtoday_offset_init = true;
      }

      if (LocalTime() == Midnight()) {
        for (uint32_t i = 0; i < 3; i++) {
          Settings->energy_kWhyesterday_ph[i] = RtcSettings.energy_kWhtoday_ph[i];

          RtcSettings.energy_kWhtotal_ph[i] += RtcSettings.energy_kWhtoday_ph[i];
          Settings->energy_kWhtotal_ph[i] = RtcSettings.energy_kWhtotal_ph[i];

          Energy.period[i] -= RtcSettings.energy_kWhtoday_ph[i];     // this becomes a large unsigned, effectively a negative for EnergyShow calculation
          Energy.kWhtoday[i] = 0;
          Energy.kWhtoday_offset[i] = 0;
          RtcSettings.energy_kWhtoday_ph[i] = 0;
          Energy.start_energy[i] = 0;
//        Energy.kWhtoday_delta = 0;                                 // dont zero this, we need to carry the remainder over to tomorrow
        }
        EnergyUpdateToday();
#if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
        Energy.max_energy_state  = 3;
#endif  // USE_ENERGY_POWER_LIMIT
      }
#if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
      if ((RtcTime.hour == Settings->energy_max_energy_start) && (3 == Energy.max_energy_state )) {
        Energy.max_energy_state  = 0;
      }
#endif  // USE_ENERGY_POWER_LIMIT

    }
  }

  XnrgCall(FUNC_EVERY_200_MSECOND);
}

void EnergySaveState(void)
{
  Settings->energy_kWhdoy = (RtcTime.valid) ? RtcTime.day_of_year : 0;

  for (uint32_t i = 0; i < 3; i++) {
    Settings->energy_kWhtoday_ph[i] = RtcSettings.energy_kWhtoday_ph[i];
    Settings->energy_kWhtotal_ph[i] = RtcSettings.energy_kWhtotal_ph[i];
  }

  Settings->energy_usage = RtcSettings.energy_usage;
}

#ifdef USE_ENERGY_MARGIN_DETECTION
bool EnergyMargin(bool type, uint16_t margin, uint16_t value, bool &flag, bool &save_flag)
{
  bool change;

  if (!margin) return false;
  change = save_flag;
  if (type) {
    flag = (value > margin);
  } else {
    flag = (value < margin);
  }
  save_flag = flag;
  return (change != save_flag);
}

void EnergyMarginCheck(void)
{
  if (Energy.power_steady_counter) {
    Energy.power_steady_counter--;
    return;
  }

  bool jsonflg = false;
  Response_P(PSTR("{\"" D_RSLT_MARGINS "\":{"));

  int16_t power_diff[ENERGY_MAX_PHASES] = { 0 };
  for (uint32_t phase = 0; phase < Energy.phase_count; phase++) {
    uint16_t active_power = (uint16_t)(Energy.active_power[phase]);

//    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: APower %d, HPower0 %d, HPower1 %d, HPower2 %d"), active_power, Energy.power_history[phase][0], Energy.power_history[phase][1], Energy.power_history[phase][2]);

    if (Settings->energy_power_delta[phase]) {
      power_diff[phase] = active_power - Energy.power_history[phase][0];
      uint16_t delta = abs(power_diff[phase]);
      bool threshold_met = false;
      if (delta > 0) {
        if (Settings->energy_power_delta[phase] < 101) {  // 1..100 = Percentage
          uint16_t min_power = (Energy.power_history[phase][0] > active_power) ? active_power : Energy.power_history[phase][0];
          if (0 == min_power) { min_power++; }    // Fix divide by 0 exception (#6741)
          delta = (delta * 100) / min_power;
          if (delta >= Settings->energy_power_delta[phase]) {
            threshold_met = true;
          }
        } else {                                  // 101..32000 = Absolute
          if (delta >= (Settings->energy_power_delta[phase] -100)) {
            threshold_met = true;
          }
        }
      }
      if (threshold_met) {
        Energy.power_history[phase][1] = active_power;  // We only want one report so reset history
        Energy.power_history[phase][2] = active_power;
        jsonflg = true;
      } else {
        power_diff[phase] = 0;
      }
    }
    Energy.power_history[phase][0] = Energy.power_history[phase][1];  // Shift in history every second allowing power changes to settle for up to three seconds
    Energy.power_history[phase][1] = Energy.power_history[phase][2];
    Energy.power_history[phase][2] = active_power;
  }
  if (jsonflg) {
/*
    char power_diff_chr[Energy.phase_count][FLOATSZ];
    for (uint32_t phase = 0; phase < Energy.phase_count; phase++) {
      dtostrfd(power_diff[phase], 0, power_diff_chr[phase]);
    }
    char value_chr[FLOATSZ * ENERGY_MAX_PHASES];
    ResponseAppend_P(PSTR("\"" D_CMND_POWERDELTA "\":%s"), EnergyFormat(value_chr, power_diff_chr[0], 1));
*/
    float power_diff_f[Energy.phase_count];
    for (uint32_t phase = 0; phase < Energy.phase_count; phase++) {
      power_diff_f[phase] = power_diff[phase];
    }
    char value_chr[FLOATSZ * ENERGY_MAX_PHASES];
    ResponseAppend_P(PSTR("\"" D_CMND_POWERDELTA "\":%s"), EnergyFormat(value_chr, power_diff_f, 0, 1));
  }

  uint16_t energy_power_u = (uint16_t)(Energy.active_power[0]);

  if (Energy.power_on && (Settings->energy_min_power || Settings->energy_max_power || Settings->energy_min_voltage || Settings->energy_max_voltage || Settings->energy_min_current || Settings->energy_max_current)) {
    uint16_t energy_voltage_u = (uint16_t)(Energy.voltage[0]);
    uint16_t energy_current_u = (uint16_t)(Energy.current[0] * 1000);

    DEBUG_DRIVER_LOG(PSTR("NRG: W %d, U %d, I %d"), energy_power_u, energy_voltage_u, energy_current_u);

    bool flag;
    if (EnergyMargin(false, Settings->energy_min_power, energy_power_u, flag, Energy.min_power_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_POWERLOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(true, Settings->energy_max_power, energy_power_u, flag, Energy.max_power_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_POWERHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(false, Settings->energy_min_voltage, energy_voltage_u, flag, Energy.min_voltage_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_VOLTAGELOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(true, Settings->energy_max_voltage, energy_voltage_u, flag, Energy.max_voltage_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_VOLTAGEHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(false, Settings->energy_min_current, energy_current_u, flag, Energy.min_current_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_CURRENTLOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(true, Settings->energy_max_current, energy_current_u, flag, Energy.max_current_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_CURRENTHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
  }
  if (jsonflg) {
    ResponseJsonEndEnd();
    MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_MARGINS), MQTT_TELE_RETAIN);
    EnergyMqttShow();
  }

#ifdef USE_ENERGY_POWER_LIMIT
  // Max Power
  if (Settings->energy_max_power_limit) {
    if (Energy.active_power[0] > Settings->energy_max_power_limit) {
      if (!Energy.mplh_counter) {
        Energy.mplh_counter = Settings->energy_max_power_limit_hold;
      } else {
        Energy.mplh_counter--;
        if (!Energy.mplh_counter) {
          ResponseTime_P(PSTR(",\"" D_JSON_MAXPOWERREACHED "\":%d}"), energy_power_u);
          MqttPublishPrefixTopicRulesProcess_P(STAT, S_RSLT_WARNING);
          EnergyMqttShow();
          SetAllPower(POWER_ALL_OFF, SRC_MAXPOWER);
          if (!Energy.mplr_counter) {
            Energy.mplr_counter = Settings->param[P_MAX_POWER_RETRY] +1;  // SetOption33 - Max Power Retry count
          }
          Energy.mplw_counter = Settings->energy_max_power_limit_window;
        }
      }
    }
    else if (TasmotaGlobal.power && (energy_power_u <= Settings->energy_max_power_limit)) {
      Energy.mplh_counter = 0;
      Energy.mplr_counter = 0;
      Energy.mplw_counter = 0;
    }
    if (!TasmotaGlobal.power) {
      if (Energy.mplw_counter) {
        Energy.mplw_counter--;
      } else {
        if (Energy.mplr_counter) {
          Energy.mplr_counter--;
          if (Energy.mplr_counter) {
            ResponseTime_P(PSTR(",\"" D_JSON_POWERMONITOR "\":\"%s\"}"), GetStateText(1));
            MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_JSON_POWERMONITOR));
            RestorePower(true, SRC_MAXPOWER);
          } else {
            ResponseTime_P(PSTR(",\"" D_JSON_MAXPOWERREACHEDRETRY "\":\"%s\"}"), GetStateText(0));
            MqttPublishPrefixTopicRulesProcess_P(STAT, S_RSLT_WARNING);
            EnergyMqttShow();
            SetAllPower(POWER_ALL_OFF, SRC_MAXPOWER);
          }
        }
      }
    }
  }

  // Max Energy
  if (Settings->energy_max_energy) {
    uint16_t energy_daily_u = (uint16_t)(Energy.daily_sum * 1000);
    if (!Energy.max_energy_state  && (RtcTime.hour == Settings->energy_max_energy_start)) {
      Energy.max_energy_state  = 1;
      ResponseTime_P(PSTR(",\"" D_JSON_ENERGYMONITOR "\":\"%s\"}"), GetStateText(1));
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_JSON_ENERGYMONITOR));
      RestorePower(true, SRC_MAXENERGY);
    }
    else if ((1 == Energy.max_energy_state ) && (energy_daily_u >= Settings->energy_max_energy)) {
      Energy.max_energy_state  = 2;
      ResponseTime_P(PSTR(",\"" D_JSON_MAXENERGYREACHED "\":%3_f}"), &Energy.daily_sum);
      MqttPublishPrefixTopicRulesProcess_P(STAT, S_RSLT_WARNING);
      EnergyMqttShow();
      SetAllPower(POWER_ALL_OFF, SRC_MAXENERGY);
    }
  }
#endif  // USE_ENERGY_POWER_LIMIT
}

void EnergyMqttShow(void)
{
// {"Time":"2017-12-16T11:48:55","ENERGY":{"Total":0.212,"Yesterday":0.000,"Today":0.014,"Period":2.0,"Power":22.0,"Factor":1.00,"Voltage":213.6,"Current":0.100}}
  int tele_period_save = TasmotaGlobal.tele_period;
  TasmotaGlobal.tele_period = 2;
  ResponseClear();
  ResponseAppendTime();
  EnergyShow(true);
  TasmotaGlobal.tele_period = tele_period_save;
  ResponseJsonEnd();
  MqttPublishTeleSensor();
}
#endif  // USE_ENERGY_MARGIN_DETECTION

void EnergyEverySecond(void)
{
  // Overtemp check
  if (Energy.use_overtemp && TasmotaGlobal.global_update) {
    if (TasmotaGlobal.power && !isnan(TasmotaGlobal.temperature_celsius) && (TasmotaGlobal.temperature_celsius > (float)Settings->param[P_OVER_TEMP])) {  // SetOption42 Device overtemp, turn off relays

      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Temperature %1_f"), &TasmotaGlobal.temperature_celsius);

      SetAllPower(POWER_ALL_OFF, SRC_OVERTEMP);
    }
  }

  // Invalid data reset
  if (TasmotaGlobal.uptime > ENERGY_WATCHDOG) {
    uint32_t data_valid = Energy.phase_count;
    for (uint32_t i = 0; i < Energy.phase_count; i++) {
      if (Energy.data_valid[i] <= ENERGY_WATCHDOG) {
        Energy.data_valid[i]++;
        if (Energy.data_valid[i] > ENERGY_WATCHDOG) {
          // Reset energy registers
          Energy.voltage[i] = 0;
          Energy.current[i] = 0;
          Energy.active_power[i] = 0;
          if (!isnan(Energy.apparent_power[i])) { Energy.apparent_power[i] = 0; }
          if (!isnan(Energy.reactive_power[i])) { Energy.reactive_power[i] = 0; }
          if (!isnan(Energy.frequency[i])) { Energy.frequency[i] = 0; }
          if (!isnan(Energy.power_factor[i])) { Energy.power_factor[i] = 0; }
          if (!isnan(Energy.export_active[i])) { Energy.export_active[i] = 0; }

          data_valid--;
        }
      }
    }
    if (!data_valid) {
      //Energy.start_energy = 0;
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Energy reset by invalid data"));

      XnrgCall(FUNC_ENERGY_RESET);
    }
  }

#ifdef USE_ENERGY_MARGIN_DETECTION
  EnergyMarginCheck();
#endif  // USE_ENERGY_MARGIN_DETECTION
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void EnergyCommandCalResponse(uint32_t nvalue) {
  snprintf_P(XdrvMailbox.command, CMDSZ, PSTR("%sCal"), XdrvMailbox.command);
  ResponseCmndNumber(nvalue);
}

void ResponseCmndEnergyTotalYesterdayToday(void) {
  char value_chr[FLOATSZ * ENERGY_MAX_PHASES];   // Used by EnergyFormatIndex
  char value2_chr[FLOATSZ * ENERGY_MAX_PHASES];
  char value3_chr[FLOATSZ * ENERGY_MAX_PHASES];

  float energy_yesterday_ph[3];
  for (uint32_t i = 0; i < Energy.phase_count; i++) {
    energy_yesterday_ph[i] = (float)Settings->energy_kWhyesterday_ph[i] / 100000;
    Energy.total[i] = (float)(RtcSettings.energy_kWhtotal_ph[i] + Energy.kWhtoday_offset[i] + Energy.kWhtoday[i]) / 100000;
  }

  Response_P(PSTR("{\"%s\":{\"" D_JSON_TOTAL "\":%s,\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s}}"),
    XdrvMailbox.command,
    EnergyFormat(value_chr, Energy.total, Settings->flag2.energy_resolution, true),
    EnergyFormat(value2_chr, energy_yesterday_ph, Settings->flag2.energy_resolution, true),
    EnergyFormat(value3_chr, Energy.daily, Settings->flag2.energy_resolution, true));
}

void CmndEnergyTotal(void) {
  uint32_t values[2] = { 0 };
  uint32_t params = ParseParameters(2, values);

  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Energy.phase_count) && (params > 0)) {
    uint32_t phase = XdrvMailbox.index -1;
    // Reset Energy Total
    RtcSettings.energy_kWhtotal_ph[phase] = values[0] * 100;
    Settings->energy_kWhtotal_ph[phase] = RtcSettings.energy_kWhtotal_ph[phase];
    if (params > 1) {
      Settings->energy_kWhtotal_time = values[1];
    } else {
      Settings->energy_kWhtotal_time = (!Energy.kWhtoday_offset[phase]) ? LocalTime() : Midnight();
    }
    RtcSettings.energy_usage.last_usage_kWhtotal = (uint32_t)(Energy.total[phase] * 1000);
  }
  ResponseCmndEnergyTotalYesterdayToday();
}

void CmndEnergyYesterday(void) {
  uint32_t values[2] = { 0 };
  uint32_t params = ParseParameters(2, values);

  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Energy.phase_count) && (params > 0)) {
    uint32_t phase = XdrvMailbox.index -1;
    // Reset Energy Yesterday
    Settings->energy_kWhyesterday_ph[phase] = values[0] * 100;
    if (params > 1) {
      Settings->energy_kWhtotal_time = values[1];
    }
  }
  ResponseCmndEnergyTotalYesterdayToday();
}
void CmndEnergyToday(void) {
  uint32_t values[2] = { 0 };
  uint32_t params = ParseParameters(2, values);

  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Energy.phase_count) && (params > 0)) {
    uint32_t phase = XdrvMailbox.index -1;
    // Reset Energy Today
    Energy.kWhtoday_offset[phase] = values[0] * 100;
    Energy.kWhtoday[phase] = 0;
    Energy.kWhtoday_delta[phase] = 0;
    Energy.start_energy[phase] = 0;
    Energy.period[phase] = Energy.kWhtoday_offset[phase];
    Settings->energy_kWhtoday_ph[phase] = Energy.kWhtoday_offset[phase];
    RtcSettings.energy_kWhtoday_ph[phase] = Energy.kWhtoday_offset[phase];
    Energy.daily[phase] = (float)Energy.kWhtoday_offset[phase] / 100000;
    if (params > 1) {
      Settings->energy_kWhtotal_time = values[1];
    }
    else if (!RtcSettings.energy_kWhtotal_ph[phase] && !Energy.kWhtoday_offset[phase]) {
      Settings->energy_kWhtotal_time = LocalTime();
    }
  }
  ResponseCmndEnergyTotalYesterdayToday();
}

void ResponseCmndEnergyUsageExport(void) {
  float usage1_kWhtotal = (float)Settings->energy_usage.usage1_kWhtotal / 100000;
  float usage2_kWhtotal = (float)Settings->energy_usage.usage2_kWhtotal / 100000;
  float return1_kWhtotal = (float)Settings->energy_usage.return1_kWhtotal / 100000;
  float return2_kWhtotal = (float)Settings->energy_usage.return2_kWhtotal / 100000;

  Response_P(PSTR("{\"%s\":{\"" D_JSON_USAGE "\":[%*_f,%*_f],\"" D_JSON_EXPORT "\":[%*_f,%*_f]}}"),
    XdrvMailbox.command,
    Settings->flag2.energy_resolution, &usage1_kWhtotal,
    Settings->flag2.energy_resolution, &usage2_kWhtotal,
    Settings->flag2.energy_resolution, &return1_kWhtotal,
    Settings->flag2.energy_resolution, &return2_kWhtotal);
}

void CmndEnergyUsage(void) {
  uint32_t values[2] = { 0 };
  uint32_t params = ParseParameters(2, values);
  if (params > 0) {
    // Reset energy_usage.usage totals
    RtcSettings.energy_usage.usage1_kWhtotal = values[0] * 100;
    if (params > 1) {
      RtcSettings.energy_usage.usage2_kWhtotal = values[1] * 100;
    }
    Settings->energy_usage.usage1_kWhtotal = RtcSettings.energy_usage.usage1_kWhtotal;
    Settings->energy_usage.usage2_kWhtotal = RtcSettings.energy_usage.usage2_kWhtotal;
  }
  ResponseCmndEnergyUsageExport();
}

void CmndEnergyExport(void) {
  uint32_t values[2] = { 0 };
  uint32_t params = ParseParameters(2, values);
  if (params > 0) {
    // Reset energy_usage.return totals
    RtcSettings.energy_usage.return1_kWhtotal = values[0] * 100;
    if (params > 1) {
      RtcSettings.energy_usage.return2_kWhtotal = values[1] * 100;
    }
    Settings->energy_usage.return1_kWhtotal = RtcSettings.energy_usage.return1_kWhtotal;
    Settings->energy_usage.return2_kWhtotal = RtcSettings.energy_usage.return2_kWhtotal;
  }
  ResponseCmndEnergyUsageExport();
}

void CmndTariff(void) {
  // Tariff1 22:00,23:00 - Tariff1 start hour for Standard Time and Daylight Savings Time
  // Tariff2 6:00,7:00   - Tariff2 start hour for Standard Time and Daylight Savings Time
  // Tariffx 1320, 1380  = minutes and also 22:00, 23:00
  // Tariffx 22, 23      = hours and also 22:00, 23:00
  // Tariff9 0/1

  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
    uint32_t tariff = XdrvMailbox.index -1;
    uint32_t time_type = 0;
    char *p;
    char *str = strtok_r(XdrvMailbox.data, ", ", &p);  // 23:15, 22:30
    while ((str != nullptr) && (time_type < 2)) {
      char *q;
      uint32_t value = strtol(str, &q, 10);            // 23 or 22
      Settings->tariff[tariff][time_type] = value;
      if (value < 24) {                                // Below 24 is hours
        Settings->tariff[tariff][time_type] *= 60;      // Multiply hours by 60 minutes
        char *minute = strtok_r(nullptr, ":", &q);
        if (minute) {
          value = strtol(minute, nullptr, 10);         // 15 or 30
          if (value > 59) {
            value = 59;
          }
          Settings->tariff[tariff][time_type] += value;
        }
      }
      if (Settings->tariff[tariff][time_type] > 1439) {
        Settings->tariff[tariff][time_type] = 1439;     // Max is 23:59
      }
      str = strtok_r(nullptr, ", ", &p);
      time_type++;
    }
  }
  else if (XdrvMailbox.index == 9) {
    Settings->flag3.energy_weekend = XdrvMailbox.payload & 1;  // CMND_TARIFF
  }
  Response_P(PSTR("{\"%s\":{\"Off-Peak\":{\"STD\":\"%s\",\"DST\":\"%s\"},\"Standard\":{\"STD\":\"%s\",\"DST\":\"%s\"},\"Weekend\":\"%s\"}}"),
    XdrvMailbox.command,
    GetMinuteTime(Settings->tariff[0][0]).c_str(),GetMinuteTime(Settings->tariff[0][1]).c_str(),
    GetMinuteTime(Settings->tariff[1][0]).c_str(),GetMinuteTime(Settings->tariff[1][1]).c_str(),
    GetStateText(Settings->flag3.energy_weekend));             // CMND_TARIFF
}

void CmndPowerCal(void) {
  Energy.command_code = CMND_POWERCAL;
  if (XnrgCall(FUNC_COMMAND)) {  // microseconds
    if (XdrvMailbox.payload > 999) {
      Settings->energy_power_calibration = XdrvMailbox.payload;
    }
    ResponseCmndNumber(Settings->energy_power_calibration);
  }
}

void CmndVoltageCal(void) {
  Energy.command_code = CMND_VOLTAGECAL;
  if (XnrgCall(FUNC_COMMAND)) {  // microseconds
    if (XdrvMailbox.payload > 999) {
      Settings->energy_voltage_calibration = XdrvMailbox.payload;
    }
    ResponseCmndNumber(Settings->energy_voltage_calibration);
  }
}

void CmndCurrentCal(void) {
  Energy.command_code = CMND_CURRENTCAL;
  if (XnrgCall(FUNC_COMMAND)) {  // microseconds
    if (XdrvMailbox.payload > 999) {
      Settings->energy_current_calibration = XdrvMailbox.payload;
    }
    ResponseCmndNumber(Settings->energy_current_calibration);
  }
}

void CmndFrequencyCal(void) {
  Energy.command_code = CMND_FREQUENCYCAL;
  if (XnrgCall(FUNC_COMMAND)) {  // microseconds
    if (XdrvMailbox.payload > 999) {
      Settings->energy_frequency_calibration = XdrvMailbox.payload;
    }
    ResponseCmndNumber(Settings->energy_frequency_calibration);
  }
}

void CmndPowerSet(void) {
  Energy.command_code = CMND_POWERSET;
  if (XnrgCall(FUNC_COMMAND)) {  // Watt
    EnergyCommandCalResponse(Settings->energy_power_calibration);
  }
}

void CmndVoltageSet(void) {
  Energy.command_code = CMND_VOLTAGESET;
  if (XnrgCall(FUNC_COMMAND)) {  // Volt
    EnergyCommandCalResponse(Settings->energy_voltage_calibration);
  }
}

void CmndCurrentSet(void) {
  Energy.command_code = CMND_CURRENTSET;
  if (XnrgCall(FUNC_COMMAND)) {  // milliAmpere
    EnergyCommandCalResponse(Settings->energy_current_calibration);
  }
}

void CmndFrequencySet(void) {
  Energy.command_code = CMND_FREQUENCYSET;
  if (XnrgCall(FUNC_COMMAND)) {  // Hz
    EnergyCommandCalResponse(Settings->energy_frequency_calibration);
  }
}

void CmndModuleAddress(void) {
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 4) && (1 == Energy.phase_count)) {
    Energy.command_code = CMND_MODULEADDRESS;
    if (XnrgCall(FUNC_COMMAND)) {  // Module address
      ResponseCmndDone();
    }
  }
}

void CmndEnergyConfig(void) {
  Energy.command_code = CMND_ENERGYCONFIG;
  if (XnrgCall(FUNC_COMMAND)) {
    ResponseCmndDone();
  }
}

#ifdef USE_ENERGY_MARGIN_DETECTION
void CmndPowerDelta(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= ENERGY_MAX_PHASES)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32000)) {
      Settings->energy_power_delta[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Settings->energy_power_delta[XdrvMailbox.index -1]);
  }
}

void CmndPowerLow(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_min_power = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_min_power);
}

void CmndPowerHigh(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power);
}

void CmndVoltageLow(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
    Settings->energy_min_voltage = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_min_voltage);
}

void CmndVoltageHigh(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
    Settings->energy_max_voltage = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_voltage);
}

void CmndCurrentLow(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
    Settings->energy_min_current = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_min_current);
}

void CmndCurrentHigh(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
    Settings->energy_max_current = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_current);
}

#ifdef USE_ENERGY_POWER_LIMIT
void CmndMaxPower(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power_limit = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_limit);
}

void CmndMaxPowerHold(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power_limit_hold = (1 == XdrvMailbox.payload) ? MAX_POWER_HOLD : XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_limit_hold);
}

void CmndMaxPowerWindow(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power_limit_window = (1 == XdrvMailbox.payload) ? MAX_POWER_WINDOW : XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_limit_window);
}

void CmndSafePower(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power_safe_limit = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_safe_limit);
}

void CmndSafePowerHold(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_power_safe_limit_hold = (1 == XdrvMailbox.payload) ? SAFE_POWER_HOLD : XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_safe_limit_hold);
}

void CmndSafePowerWindow(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 1440)) {
    Settings->energy_max_power_safe_limit_window = (1 == XdrvMailbox.payload) ? SAFE_POWER_WINDOW : XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_power_safe_limit_window);
}

void CmndMaxEnergy(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->energy_max_energy = XdrvMailbox.payload;
    Energy.max_energy_state  = 3;
  }
  ResponseCmndNumber(Settings->energy_max_energy);
}

void CmndMaxEnergyStart(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 24)) {
    Settings->energy_max_energy_start = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->energy_max_energy_start);
}
#endif  // USE_ENERGY_POWER_LIMIT
#endif  // USE_ENERGY_MARGIN_DETECTION

void EnergyDrvInit(void) {
  memset(&Energy, 0, sizeof(Energy));  // Reset all to 0 and false;
  for (uint32_t phase = 0; phase < ENERGY_MAX_PHASES; phase++) {
    Energy.apparent_power[phase] = NAN;
    Energy.reactive_power[phase] = NAN;
    Energy.power_factor[phase] = NAN;
    Energy.frequency[phase] = NAN;
    Energy.export_active[phase] = NAN;
  }
  Energy.phase_count = 1;              // Number of phases active
  Energy.voltage_available = true;     // Enable if voltage is measured
  Energy.current_available = true;     // Enable if current is measured
  Energy.power_on = true;
#ifdef USE_ENERGY_MARGIN_DETECTION
  Energy.power_steady_counter = 8;     // Allow for power on stabilization
#endif  // USE_ENERGY_MARGIN_DETECTION

  TasmotaGlobal.energy_driver = ENERGY_NONE;
  XnrgCall(FUNC_PRE_INIT);  // Find first energy driver
}

void EnergySnsInit(void)
{
  XnrgCall(FUNC_INIT);

  if (TasmotaGlobal.energy_driver) {

    // Update for split phase totals (v9.5.0.9)
    if ((Settings->energy_kWhtotal > 0) && (0 == Settings->energy_kWhtotal_ph[0])) {
      Settings->energy_kWhtotal_ph[0] = Settings->energy_kWhtotal;
      Settings->energy_kWhtoday_ph[0] = Settings->energy_kWhtoday;
      Settings->energy_kWhyesterday_ph[0] = Settings->energy_kWhyesterday;
      RtcSettings.energy_kWhtoday_ph[0] = RtcSettings.energy_kWhtoday;
      RtcSettings.energy_kWhtotal_ph[0] = RtcSettings.energy_kWhtotal;
      Settings->energy_kWhtotal = 0;
    }

//    Energy.kWhtoday_offset = 0;
    // Do not use at Power On as Rtc was invalid (but has been restored from Settings already)
    if ((ResetReason() != REASON_DEFAULT_RST) && RtcSettingsValid()) {
      for (uint32_t i = 0; i < 3; i++) {
        Energy.kWhtoday_offset[i] = RtcSettings.energy_kWhtoday_ph[i];
      }
      Energy.kWhtoday_offset_init = true;
    }
    for (uint32_t i = 0; i < 3; i++) {
//    Energy.kWhtoday_ph[i] = 0;
//    Energy.kWhtoday_delta[i] = 0;
      Energy.period[i] = Energy.kWhtoday_offset[i];
    }
    EnergyUpdateToday();
    ticker_energy.attach_ms(200, Energy200ms);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SNS1[] PROGMEM =
  "{s}" D_POWERUSAGE_APPARENT "{m}%s " D_UNIT_VA "{e}"
  "{s}" D_POWERUSAGE_REACTIVE "{m}%s " D_UNIT_VAR "{e}"
  "{s}" D_POWER_FACTOR "{m}%s{e}";

const char HTTP_ENERGY_SNS2[] PROGMEM =
  "{s}" D_ENERGY_TODAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_YESTERDAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

const char HTTP_ENERGY_SNS3[] PROGMEM =
  "{s}" D_EXPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}";
#endif  // USE_WEBSERVER

void EnergyShow(bool json) {
  if (Energy.voltage_common) {
    for (uint32_t i = 0; i < Energy.phase_count; i++) {
      Energy.voltage[i] = Energy.voltage[0];
    }
  }

  float apparent_power[Energy.phase_count];
  float reactive_power[Energy.phase_count];
  float power_factor[Energy.phase_count];
  if (!Energy.type_dc) {
    if (Energy.current_available && Energy.voltage_available) {
      for (uint32_t i = 0; i < Energy.phase_count; i++) {
        apparent_power[i] = Energy.apparent_power[i];
        if (isnan(apparent_power[i])) {
          apparent_power[i] = Energy.voltage[i] * Energy.current[i];
        }
        if (apparent_power[i] < Energy.active_power[i]) {  // Should be impossible
          Energy.active_power[i] = apparent_power[i];
        }

        power_factor[i] = Energy.power_factor[i];
        if (isnan(power_factor[i])) {
          power_factor[i] = (Energy.active_power[i] && apparent_power[i]) ? Energy.active_power[i] / apparent_power[i] : 0;
          if (power_factor[i] > 1) {
            power_factor[i] = 1;
          }
        }

        reactive_power[i] = Energy.reactive_power[i];
        if (isnan(reactive_power[i])) {
          reactive_power[i] = 0;
          uint32_t difference = ((uint32_t)(apparent_power[i] * 100) - (uint32_t)(Energy.active_power[i] * 100)) / 10;
          if ((Energy.current[i] > 0.005) && ((difference > 15) || (difference > (uint32_t)(apparent_power[i] * 100 / 1000)))) {
            // calculating reactive power only if current is greater than 0.005A and
            // difference between active and apparent power is greater than 1.5W or 1%
            //reactive_power[i] = (float)(RoundSqrtInt((uint64_t)(apparent_power[i] * apparent_power[i] * 100) - (uint64_t)(Energy.active_power[i] * Energy.active_power[i] * 100))) / 10;
            float power_diff = apparent_power[i] * apparent_power[i] - Energy.active_power[i] * Energy.active_power[i];
            if (power_diff < 10737418) // 2^30 / 100 (RoundSqrtInt is limited to 2^30-1)
              reactive_power[i] = (float)(RoundSqrtInt((uint32_t)(power_diff * 100.0))) / 10.0;
            else
              reactive_power[i] = (float)(SqrtInt((uint32_t)(power_diff)));
          }
        }

      }
    }
  }

  float active_power_sum = 0.0;
  float energy_yesterday_ph[Energy.phase_count];
  for (uint32_t i = 0; i < Energy.phase_count; i++) {
    energy_yesterday_ph[i] = (float)Settings->energy_kWhyesterday_ph[i] / 100000;

    active_power_sum += Energy.active_power[i];
  }

  bool energy_tariff = false;
  float energy_usage[2];
  float energy_return[2];
  if (Settings->tariff[0][0] != Settings->tariff[1][0]) {
    energy_usage[0] = (float)RtcSettings.energy_usage.usage1_kWhtotal / 100000;  // Tariff1
    energy_usage[1] = (float)RtcSettings.energy_usage.usage2_kWhtotal / 100000;  // Tariff2
    energy_return[0] = (float)RtcSettings.energy_usage.return1_kWhtotal / 100000;  // Tariff1
    energy_return[1] = (float)RtcSettings.energy_usage.return2_kWhtotal / 100000;  // Tariff2
    energy_tariff = true;
  }

  char value_chr[FLOATSZ * ENERGY_MAX_PHASES];   // Used by EnergyFormatIndex
  char value2_chr[FLOATSZ * ENERGY_MAX_PHASES];
  char value3_chr[FLOATSZ * ENERGY_MAX_PHASES];

  if (json) {
    bool show_energy_period = (0 == TasmotaGlobal.tele_period);

    ResponseAppend_P(PSTR(",\"" D_RSLT_ENERGY "\":{\"" D_JSON_TOTAL_START_TIME "\":\"%s\",\"" D_JSON_TOTAL "\":%s"),
      GetDateAndTime(DT_ENERGY).c_str(),
      EnergyFormatSum(value_chr, Energy.total, Settings->flag2.energy_resolution, json));

    if (energy_tariff) {
      ResponseAppend_P(PSTR(",\"" D_JSON_TOTAL D_CMND_TARIFF "\":%s"),
        EnergyFormatIndex(value_chr, energy_usage, Settings->flag2.energy_resolution, json, 2));
    }

    ResponseAppend_P(PSTR(",\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s"),
      EnergyFormatSum(value_chr, energy_yesterday_ph, Settings->flag2.energy_resolution, json),
      EnergyFormatSum(value2_chr, Energy.daily, Settings->flag2.energy_resolution, json));

/*
 #if defined(SDM630_IMPORT) || defined(SDM72_IMPEXP)
    if (!isnan(Energy.import_active[0])) {
      ResponseAppend_P(PSTR(",\"" D_JSON_IMPORT_ACTIVE "\":%s"),
        EnergyFormat(value_chr, Energy.import_active, Settings->flag2.energy_resolution, json));
      if (energy_tariff) {
        ResponseAppend_P(PSTR(",\"" D_JSON_IMPORT D_CMND_TARIFF "\":%s"),
          EnergyFormatIndex(value_chr, energy_return, Settings->flag2.energy_resolution, json, 2));
      }
    }
#endif  // SDM630_IMPORT || SDM72_IMPEXP
*/

    if (!isnan(Energy.export_active[0])) {
      ResponseAppend_P(PSTR(",\"" D_JSON_EXPORT_ACTIVE "\":%s"),
        EnergyFormat(value_chr, Energy.export_active, Settings->flag2.energy_resolution, json));
      if (energy_tariff) {
        ResponseAppend_P(PSTR(",\"" D_JSON_EXPORT D_CMND_TARIFF "\":%s"),
          EnergyFormatIndex(value_chr, energy_return, Settings->flag2.energy_resolution, json, 2));
      }
    }

    if (show_energy_period) {
      float energy_period[Energy.phase_count];
      for (uint32_t i = 0; i < Energy.phase_count; i++) {
        energy_period[i] = (float)(RtcSettings.energy_kWhtoday_ph[i] - Energy.period[i]) / 100;
        Energy.period[i] = RtcSettings.energy_kWhtoday_ph[i];
      }
      ResponseAppend_P(PSTR(",\"" D_JSON_PERIOD "\":%s"),
        EnergyFormat(value_chr, energy_period, Settings->flag2.wattage_resolution, json));
    }

    ResponseAppend_P(PSTR(",\"" D_JSON_POWERUSAGE "\":%s"),
        EnergyFormat(value_chr, Energy.active_power, Settings->flag2.wattage_resolution, json));
    if (!Energy.type_dc) {
      if (Energy.current_available && Energy.voltage_available) {
        ResponseAppend_P(PSTR(",\"" D_JSON_APPARENT_POWERUSAGE "\":%s,\"" D_JSON_REACTIVE_POWERUSAGE "\":%s,\"" D_JSON_POWERFACTOR "\":%s"),
          EnergyFormat(value_chr, apparent_power, Settings->flag2.wattage_resolution, json),
          EnergyFormat(value2_chr, reactive_power, Settings->flag2.wattage_resolution, json),
          EnergyFormat(value3_chr, power_factor, 2, json));
      }
      if (!isnan(Energy.frequency[0])) {
        ResponseAppend_P(PSTR(",\"" D_JSON_FREQUENCY "\":%s"),
          EnergyFormat(value_chr, Energy.frequency, Settings->flag2.frequency_resolution, json, Energy.frequency_common));
      }
    }
    if (Energy.voltage_available) {
      ResponseAppend_P(PSTR(",\"" D_JSON_VOLTAGE "\":%s"),
        EnergyFormat(value_chr, Energy.voltage, Settings->flag2.voltage_resolution, json, Energy.voltage_common));
    }
    if (Energy.current_available) {
      ResponseAppend_P(PSTR(",\"" D_JSON_CURRENT "\":%s"),
        EnergyFormat(value_chr, Energy.current, Settings->flag2.current_resolution, json));
    }
    XnrgCall(FUNC_JSON_APPEND);
    ResponseJsonEnd();

#ifdef USE_DOMOTICZ
    if (show_energy_period) {  // Only send if telemetry
      char temp_chr[FLOATSZ];
      if (Energy.voltage_available) {
        dtostrfd(Energy.voltage[0], Settings->flag2.voltage_resolution, temp_chr);
        DomoticzSensor(DZ_VOLTAGE, temp_chr);  // Voltage
      }
      if (Energy.current_available) {
        dtostrfd(Energy.current[0], Settings->flag2.current_resolution, temp_chr);
        DomoticzSensor(DZ_CURRENT, temp_chr);  // Current
      }
      dtostrfd(Energy.total_sum * 1000, 1, temp_chr);
      DomoticzSensorPowerEnergy((int)active_power_sum, temp_chr);  // PowerUsage, EnergyToday

      char energy_usage_chr[2][FLOATSZ];
      char energy_return_chr[2][FLOATSZ];
      dtostrfd((float)RtcSettings.energy_usage.usage1_kWhtotal / 100, 1, energy_usage_chr[0]);  // Tariff1
      dtostrfd((float)RtcSettings.energy_usage.usage2_kWhtotal / 100, 1, energy_usage_chr[1]);  // Tariff2
      dtostrfd((float)RtcSettings.energy_usage.return1_kWhtotal / 100, 1, energy_return_chr[0]);
      dtostrfd((float)RtcSettings.energy_usage.return2_kWhtotal / 100, 1, energy_return_chr[1]);
      DomoticzSensorP1SmartMeter(energy_usage_chr[0], energy_usage_chr[1], energy_return_chr[0], energy_return_chr[1], (int)active_power_sum);

    }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
    if (show_energy_period) {
      if (Energy.voltage_available) {
        KnxSensor(KNX_ENERGY_VOLTAGE, Energy.voltage[0]);
      }
      if (Energy.current_available) {
        KnxSensor(KNX_ENERGY_CURRENT, Energy.current[0]);
      }
      KnxSensor(KNX_ENERGY_POWER, active_power_sum);
      if (!Energy.type_dc) {
        KnxSensor(KNX_ENERGY_POWERFACTOR, power_factor[0]);
      }
      KnxSensor(KNX_ENERGY_DAILY, Energy.daily_sum);
      KnxSensor(KNX_ENERGY_TOTAL, Energy.total_sum);
      KnxSensor(KNX_ENERGY_YESTERDAY, Energy.yesterday_sum);
    }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
  } else {
    if (Energy.voltage_available) {
      WSContentSend_PD(HTTP_SNS_VOLTAGE, EnergyFormat(value_chr, Energy.voltage, Settings->flag2.voltage_resolution, json, Energy.voltage_common));
    }
    if (!Energy.type_dc) {
      if (!isnan(Energy.frequency[0])) {
        WSContentSend_PD(PSTR("{s}" D_FREQUENCY "{m}%s " D_UNIT_HERTZ "{e}"),
          EnergyFormat(value_chr, Energy.frequency, Settings->flag2.frequency_resolution, json, Energy.frequency_common));
      }
    }
    if (Energy.current_available) {
      WSContentSend_PD(HTTP_SNS_CURRENT, EnergyFormat(value_chr, Energy.current, Settings->flag2.current_resolution, json));
    }
    WSContentSend_PD(HTTP_SNS_POWER, EnergyFormat(value_chr, Energy.active_power, Settings->flag2.wattage_resolution, json));
    if (!Energy.type_dc) {
      if (Energy.current_available && Energy.voltage_available) {
        WSContentSend_PD(HTTP_ENERGY_SNS1, EnergyFormat(value_chr, apparent_power, Settings->flag2.wattage_resolution, json),
                                           EnergyFormat(value2_chr, reactive_power, Settings->flag2.wattage_resolution, json),
                                           EnergyFormat(value3_chr, power_factor, 2, json));
      }
    }
    WSContentSend_PD(HTTP_ENERGY_SNS2, EnergyFormatSum(value_chr, Energy.daily, Settings->flag2.energy_resolution, json),
                                       EnergyFormatSum(value2_chr, energy_yesterday_ph, Settings->flag2.energy_resolution, json),
                                       EnergyFormatSum(value3_chr, Energy.total, Settings->flag2.energy_resolution, json));
    if (!isnan(Energy.export_active[0])) {
      WSContentSend_PD(HTTP_ENERGY_SNS3, EnergyFormat(value_chr, Energy.export_active, Settings->flag2.energy_resolution, json));
    }
    XnrgCall(FUNC_WEB_SENSOR);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv03(uint8_t function)
{
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    EnergyDrvInit();
  }
  else if (TasmotaGlobal.energy_driver) {
    switch (function) {
      case FUNC_LOOP:
        XnrgCall(FUNC_LOOP);
        break;
      case FUNC_EVERY_250_MSECOND:
        if (TasmotaGlobal.uptime > 4) {
          XnrgCall(FUNC_EVERY_250_MSECOND);
        }
        break;
      case FUNC_EVERY_SECOND:
        XnrgCall(FUNC_EVERY_SECOND);
        break;
      case FUNC_SERIAL:
        result = XnrgCall(FUNC_SERIAL);
        break;
#ifdef USE_ENERGY_MARGIN_DETECTION
      case FUNC_SET_POWER:
        Energy.power_steady_counter = 2;
        break;
#endif  // USE_ENERGY_MARGIN_DETECTION
      case FUNC_COMMAND:
        result = DecodeCommand(kEnergyCommands, EnergyCommand);
        break;
    }
  }
  return result;
}

bool Xsns03(uint8_t function)
{
  bool result = false;

  if (TasmotaGlobal.energy_driver) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        EnergyEverySecond();
        break;
      case FUNC_JSON_APPEND:
        EnergyShow(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        EnergyShow(false);
        break;
#endif  // USE_WEBSERVER
      case FUNC_SAVE_BEFORE_RESTART:
        EnergySaveState();
        break;
      case FUNC_INIT:
        EnergySnsInit();
        break;
    }
  }
  return result;
}

#endif  // USE_ENERGY_SENSOR
