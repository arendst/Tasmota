/*
  xnrg_01_hlw8012.ino - HLW8012 (Sonoff Pow) energy sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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
#ifdef USE_HLW8012
/*********************************************************************************************\
 * HLW8012, BL0937 or HJL-01 - Energy (Sonoff Pow, HuaFan, KMC70011, BlitzWolf)
 *
 * Based on Source: Shenzhen Heli Technology Co., Ltd
\*********************************************************************************************/

#define XNRG_01                 1

// Energy model type 0 (GPIO_HLW_CF) - HLW8012 based (Sonoff Pow, KMC70011, HuaFan, AplicWDP303075)
#define HLW_PREF            10000    // 1000.0W
#define HLW_UREF             2200    // 220.0V
#define HLW_IREF             4545    // 4.545A

// Energy model type 1 (GPIO_HJL_CF) - HJL-01/BL0937 based (BlitzWolf, Homecube, Gosund, Teckin)
#define HJL_PREF             1362
#define HJL_UREF              822
#define HJL_IREF             3300

#define HLW_POWER_PROBE_TIME   10    // Number of seconds to probe for power before deciding none used (low power pulse can take up to 10 seconds)
#define HLW_SAMPLE_COUNT       10    // Max number of samples per cycle

//#define HLW_DEBUG
#ifdef HLW_DEBUG
unsigned long hlw_debug[HLW_SAMPLE_COUNT];
#endif

unsigned long hlw_cf_pulse_length = 0;
unsigned long hlw_cf_pulse_last_time = 0;
unsigned long hlw_cf_power_pulse_length = 0;

unsigned long hlw_cf1_pulse_length = 0;
unsigned long hlw_cf1_pulse_last_time = 0;
unsigned long hlw_cf1_summed_pulse_length = 0;
unsigned long hlw_cf1_pulse_counter = 0;
unsigned long hlw_cf1_voltage_pulse_length = 0;
unsigned long hlw_cf1_current_pulse_length = 0;

unsigned long hlw_energy_period_counter = 0;

unsigned long hlw_power_ratio = 0;
unsigned long hlw_voltage_ratio = 0;
unsigned long hlw_current_ratio = 0;

uint8_t hlw_select_ui_flag = 0;
uint8_t hlw_ui_flag = 1;
uint8_t hlw_model_type = 0;
uint8_t hlw_load_off = 1;
uint8_t hlw_cf1_timer = 0;

#ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
void HlwCfInterrupt(void) ICACHE_RAM_ATTR;
void HlwCf1Interrupt(void) ICACHE_RAM_ATTR;
#endif  // USE_WS2812_DMA

void HlwCfInterrupt(void)  // Service Power
{
  unsigned long us = micros();

  if (hlw_load_off) {  // Restart plen measurement
    hlw_cf_pulse_last_time = us;
    hlw_load_off = 0;
  } else {
    hlw_cf_pulse_length = us - hlw_cf_pulse_last_time;
    hlw_cf_pulse_last_time = us;
    hlw_energy_period_counter++;
  }
}

void HlwCf1Interrupt(void)  // Service Voltage and Current
{
  unsigned long us = micros();

  hlw_cf1_pulse_length = us - hlw_cf1_pulse_last_time;
  hlw_cf1_pulse_last_time = us;
  if ((hlw_cf1_timer > 2) && (hlw_cf1_timer < 8)) {  // Allow for 300 mSec set-up time and measure for up to 1 second
    hlw_cf1_summed_pulse_length += hlw_cf1_pulse_length;
#ifdef HLW_DEBUG
    hlw_debug[hlw_cf1_pulse_counter] = hlw_cf1_pulse_length;
#endif
    hlw_cf1_pulse_counter++;
    if (HLW_SAMPLE_COUNT == hlw_cf1_pulse_counter) {
      hlw_cf1_timer = 8;  // We need up to HLW_SAMPLE_COUNT samples within 1 second (low current could take up to 0.3 second)
    }
  }
}

/********************************************************************************************/

void HlwEvery200ms(void)
{
  unsigned long cf1_pulse_length = 0;
  unsigned long hlw_w = 0;
  unsigned long hlw_u = 0;
  unsigned long hlw_i = 0;

  if (micros() - hlw_cf_pulse_last_time > (HLW_POWER_PROBE_TIME * 1000000)) {
    hlw_cf_pulse_length = 0;    // No load for some time
    hlw_load_off = 1;
  }
  hlw_cf_power_pulse_length = hlw_cf_pulse_length;

  if (hlw_cf_power_pulse_length && energy_power_on && !hlw_load_off) {
    hlw_w = (hlw_power_ratio * Settings.energy_power_calibration) / hlw_cf_power_pulse_length;  // W *10
    energy_active_power = (float)hlw_w / 10;
  } else {
    energy_active_power = 0;
  }

  hlw_cf1_timer++;
  if (hlw_cf1_timer >= 8) {
    hlw_cf1_timer = 0;
    hlw_select_ui_flag = (hlw_select_ui_flag) ? 0 : 1;
    digitalWrite(pin[GPIO_NRG_SEL], hlw_select_ui_flag);

    if (hlw_cf1_pulse_counter) {
      cf1_pulse_length = hlw_cf1_summed_pulse_length / hlw_cf1_pulse_counter;
    }

#ifdef HLW_DEBUG
    // Debugging for calculating mean and median
    char stemp[100];
    stemp[0] = '\0';
    for (uint8_t i = 0; i < hlw_cf1_pulse_counter; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%s %d"), stemp, hlw_debug[i]);
    }
    for (uint8_t i = 0; i < hlw_cf1_pulse_counter; i++) {
      for (uint8_t j = i + 1; j < hlw_cf1_pulse_counter; j++) {
        if (hlw_debug[i] > hlw_debug[j]) {  // Sort ascending
          std::swap(hlw_debug[i], hlw_debug[j]);
        }
      }
    }
    unsigned long median = hlw_debug[(hlw_cf1_pulse_counter +1) / 2];
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("NRG: power %d, ui %d, cnt %d, smpl%s, sum %d, mean %d, median %d"),
      hlw_cf_power_pulse_length, hlw_select_ui_flag, hlw_cf1_pulse_counter, stemp, hlw_cf1_summed_pulse_length, cf1_pulse_length, median);
#endif

    if (hlw_select_ui_flag == hlw_ui_flag) {
      hlw_cf1_voltage_pulse_length = cf1_pulse_length;

      if (hlw_cf1_voltage_pulse_length && energy_power_on) {     // If powered on always provide voltage
        hlw_u = (hlw_voltage_ratio * Settings.energy_voltage_calibration) / hlw_cf1_voltage_pulse_length;  // V *10
        energy_voltage = (float)hlw_u / 10;
      } else {
        energy_voltage = 0;
      }

    } else {
      hlw_cf1_current_pulse_length = cf1_pulse_length;

      if (hlw_cf1_current_pulse_length && energy_active_power) {   // No current if no power being consumed
        hlw_i = (hlw_current_ratio * Settings.energy_current_calibration) / hlw_cf1_current_pulse_length;  // mA
        energy_current = (float)hlw_i / 1000;
      } else {
        energy_current = 0;
      }

    }
    hlw_cf1_summed_pulse_length = 0;
    hlw_cf1_pulse_counter = 0;
  }
}

void HlwEverySecond(void)
{
  unsigned long hlw_len;

  if (hlw_energy_period_counter) {
    hlw_len = 10000 / hlw_energy_period_counter;
    hlw_energy_period_counter = 0;
    if (hlw_len) {
      energy_kWhtoday_delta += ((hlw_power_ratio * Settings.energy_power_calibration) / hlw_len) / 36;
      EnergyUpdateToday();
    }
  }
}

void HlwSnsInit(void)
{
  if (!Settings.energy_power_calibration || (4975 == Settings.energy_power_calibration)) {
    Settings.energy_power_calibration = HLW_PREF_PULSE;
    Settings.energy_voltage_calibration = HLW_UREF_PULSE;
    Settings.energy_current_calibration = HLW_IREF_PULSE;
  }

  if (hlw_model_type) {
    hlw_power_ratio = HJL_PREF;
    hlw_voltage_ratio = HJL_UREF;
    hlw_current_ratio = HJL_IREF;
  } else {
    hlw_power_ratio = HLW_PREF;
    hlw_voltage_ratio = HLW_UREF;
    hlw_current_ratio = HLW_IREF;
  }

  pinMode(pin[GPIO_NRG_SEL], OUTPUT);
  digitalWrite(pin[GPIO_NRG_SEL], hlw_select_ui_flag);
  pinMode(pin[GPIO_NRG_CF1], INPUT_PULLUP);
  attachInterrupt(pin[GPIO_NRG_CF1], HlwCf1Interrupt, FALLING);
  pinMode(pin[GPIO_HLW_CF], INPUT_PULLUP);
  attachInterrupt(pin[GPIO_HLW_CF], HlwCfInterrupt, FALLING);
}

void HlwDrvInit(void)
{
  if (!energy_flg) {
    hlw_model_type = 0;    // HLW8012
    if (pin[GPIO_HJL_CF] < 99) {
      pin[GPIO_HLW_CF] = pin[GPIO_HJL_CF];
      pin[GPIO_HJL_CF] = 99;
      hlw_model_type = 1;  // HJL-01/BL0937
    }

    hlw_ui_flag = 1;       // Voltage on high
    if (pin[GPIO_NRG_SEL_INV] < 99) {
      pin[GPIO_NRG_SEL] = pin[GPIO_NRG_SEL_INV];
      pin[GPIO_NRG_SEL_INV] = 99;
      hlw_ui_flag = 0;     // Voltage on low
    }

    if ((pin[GPIO_NRG_SEL] < 99) && (pin[GPIO_NRG_CF1] < 99) && (pin[GPIO_HLW_CF] < 99)) {  // HLW8012 or HJL-01 based device
      energy_flg = XNRG_01;
    }
  }
}

bool HlwCommand(void)
{
  bool serviced = true;

  if ((CMND_POWERCAL == energy_command_code) || (CMND_VOLTAGECAL == energy_command_code) || (CMND_CURRENTCAL == energy_command_code)) {
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_POWERSET == energy_command_code) {
    if (XdrvMailbox.data_len && hlw_cf_power_pulse_length) {
      Settings.energy_power_calibration = ((unsigned long)(CharToDouble(XdrvMailbox.data) * 10) * hlw_cf_power_pulse_length) / hlw_power_ratio;
    }
  }
  else if (CMND_VOLTAGESET == energy_command_code) {
    if (XdrvMailbox.data_len && hlw_cf1_voltage_pulse_length) {
      Settings.energy_voltage_calibration = ((unsigned long)(CharToDouble(XdrvMailbox.data) * 10) * hlw_cf1_voltage_pulse_length) / hlw_voltage_ratio;
    }
  }
  else if (CMND_CURRENTSET == energy_command_code) {
    if (XdrvMailbox.data_len && hlw_cf1_current_pulse_length) {
      Settings.energy_current_calibration = ((unsigned long)(CharToDouble(XdrvMailbox.data)) * hlw_cf1_current_pulse_length) / hlw_current_ratio;
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

int Xnrg01(uint8_t function)
{
  int result = 0;

  if (FUNC_PRE_INIT == function) {
    HlwDrvInit();
  }
  else if (XNRG_01 == energy_flg) {
    switch (function) {
      case FUNC_INIT:
        HlwSnsInit();
        break;
      case FUNC_EVERY_SECOND:
        HlwEverySecond();
        break;
      case FUNC_EVERY_200_MSECOND:
        HlwEvery200ms();
        break;
      case FUNC_COMMAND:
        result = HlwCommand();
        break;
    }
  }
  return result;
}

#endif  // USE_HLW8012
#endif  // USE_ENERGY_SENSOR
