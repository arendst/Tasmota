/*
  xnrg_01_hlw8012.ino - HLW8012 (Sonoff Pow) energy sensor support for Tasmota

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

struct HLW {
#ifdef HLW_DEBUG
  uint32_t debug[HLW_SAMPLE_COUNT];
#endif
  volatile uint32_t cf_pulse_length = 0;
  volatile uint32_t cf_pulse_last_time = 0;
  volatile uint32_t cf_summed_pulse_length = 0;
  volatile uint32_t cf_pulse_counter = 0;
  uint32_t cf_power_pulse_length  = 0;

  volatile uint32_t cf1_pulse_length = 0;
  volatile uint32_t cf1_pulse_last_time = 0;
  volatile uint32_t cf1_summed_pulse_length = 0;
  volatile uint32_t cf1_pulse_counter = 0;

  uint32_t cf1_voltage_pulse_length  = 0;
  uint32_t cf1_current_pulse_length = 0;

  volatile uint32_t energy_period_counter = 0;

  uint32_t power_ratio = 0;
  uint32_t voltage_ratio = 0;
  uint32_t current_ratio = 0;

  uint8_t model_type = 0;
  volatile uint8_t cf1_timer = 0;
  uint8_t power_retry = 0;
  bool select_ui_flag = false;
  bool ui_flag = true;
  volatile bool load_off = true;
} Hlw;

// Fix core 2.5.x ISR not in IRAM Exception
#ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
void HlwCfInterrupt(void) IRAM_ATTR;
void HlwCf1Interrupt(void) IRAM_ATTR;
#endif  // USE_WS2812_DMA

void HlwCfInterrupt(void) {  // Service Power
  uint32_t us = micros();

  if (Hlw.load_off) {  // Restart plen measurement
    Hlw.cf_pulse_last_time = us;
    Hlw.load_off = false;
  } else {
    Hlw.cf_pulse_length = us - Hlw.cf_pulse_last_time;
    Hlw.cf_pulse_last_time = us;
    Hlw.cf_summed_pulse_length += Hlw.cf_pulse_length;
    Hlw.cf_pulse_counter++;
    Hlw.energy_period_counter++;
  }
  Energy->data_valid[0] = 0;
}

void HlwCf1Interrupt(void) {  // Service Voltage and Current
  uint32_t us = micros();

  Hlw.cf1_pulse_length = us - Hlw.cf1_pulse_last_time;
  Hlw.cf1_pulse_last_time = us;
  if ((Hlw.cf1_timer > 2) && (Hlw.cf1_timer < 8)) {  // Allow for 300 mSec set-up time and measure for up to 1 second
    Hlw.cf1_summed_pulse_length += Hlw.cf1_pulse_length;
#ifdef HLW_DEBUG
    Hlw.debug[Hlw.cf1_pulse_counter] = Hlw.cf1_pulse_length;
#endif
    Hlw.cf1_pulse_counter++;
    if (HLW_SAMPLE_COUNT == Hlw.cf1_pulse_counter) {
      Hlw.cf1_timer = 8;  // We need up to HLW_SAMPLE_COUNT samples within 1 second (low current could take up to 0.3 second)
    }
  }
  Energy->data_valid[0] = 0;
}

/********************************************************************************************/

void HlwEvery200ms(void) {
  uint32_t cf1_pulse_length = 0;
  uint32_t hlw_w = 0;
  uint32_t hlw_u = 0;
  uint32_t hlw_i = 0;

  if (micros() - Hlw.cf_pulse_last_time > (HLW_POWER_PROBE_TIME * 1000000)) {
    Hlw.cf_pulse_length = 0;    // No load for some time
    Hlw.load_off = true;
  }
  Hlw.cf_power_pulse_length = Hlw.cf_pulse_length;
  if (Hlw.cf_pulse_counter && !Hlw.load_off) {
    Hlw.cf_power_pulse_length = Hlw.cf_summed_pulse_length / Hlw.cf_pulse_counter;
  }
  Hlw.cf_summed_pulse_length = 0;
  Hlw.cf_pulse_counter = 0;

  if (Hlw.cf_power_pulse_length  && Energy->power_on && !Hlw.load_off) {
    hlw_w = (Hlw.power_ratio * EnergyGetCalibration(ENERGY_POWER_CALIBRATION)) / Hlw.cf_power_pulse_length ;  // W *10
    Energy->active_power[0] = (float)hlw_w / 10;
    Hlw.power_retry = 1;        // Workaround issue #5161
  } else {
    if (Hlw.power_retry) {
      Hlw.power_retry--;
    } else {
      Energy->active_power[0] = 0;
    }
  }

  if (PinUsed(GPIO_NRG_CF1)) {
    Hlw.cf1_timer++;
    if (Hlw.cf1_timer >= 8) {
      Hlw.cf1_timer = 0;
      Hlw.select_ui_flag = (Hlw.select_ui_flag) ? false : true;
      DigitalWrite(GPIO_NRG_SEL, 0, Hlw.select_ui_flag);

      if (Hlw.cf1_pulse_counter) {
        cf1_pulse_length = Hlw.cf1_summed_pulse_length / Hlw.cf1_pulse_counter;
      }

#ifdef HLW_DEBUG
      // Debugging for calculating mean and median
      char stemp[100];
      stemp[0] = '\0';
      for (uint32_t i = 0; i < Hlw.cf1_pulse_counter; i++) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%s %d"), stemp, Hlw.debug[i]);
      }
      for (uint32_t i = 0; i < Hlw.cf1_pulse_counter; i++) {
        for (uint32_t j = i + 1; j < Hlw.cf1_pulse_counter; j++) {
          if (Hlw.debug[i] > Hlw.debug[j]) {  // Sort ascending
            std::swap(Hlw.debug[i], Hlw.debug[j]);
          }
        }
      }
      uint32_t median = Hlw.debug[(Hlw.cf1_pulse_counter +1) / 2];
      AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: power %d, ui %d, cnt %d, smpl%s, sum %d, mean %d, median %d"),
        Hlw.cf_power_pulse_length , Hlw.select_ui_flag, Hlw.cf1_pulse_counter, stemp, Hlw.cf1_summed_pulse_length, cf1_pulse_length, median);
#endif

      if (Hlw.select_ui_flag == Hlw.ui_flag) {
        Hlw.cf1_voltage_pulse_length  = cf1_pulse_length;

        if (Hlw.cf1_voltage_pulse_length  && Energy->power_on) {     // If powered on always provide voltage
          hlw_u = (Hlw.voltage_ratio * EnergyGetCalibration(ENERGY_VOLTAGE_CALIBRATION)) / Hlw.cf1_voltage_pulse_length ;  // V *10
          Energy->voltage[0] = (float)hlw_u / 10;
        } else {
          Energy->voltage[0] = 0;
        }

      } else {
        Hlw.cf1_current_pulse_length = cf1_pulse_length;

        if (Hlw.cf1_current_pulse_length && Energy->active_power[0]) {   // No current if no power being consumed
          hlw_i = (Hlw.current_ratio * EnergyGetCalibration(ENERGY_CURRENT_CALIBRATION)) / Hlw.cf1_current_pulse_length;  // mA
          Energy->current[0] = (float)hlw_i / 1000;
        } else {
          Energy->current[0] = 0;
        }

      }
      Hlw.cf1_summed_pulse_length = 0;
      Hlw.cf1_pulse_counter = 0;
    }
  }
}

void HlwEverySecond(void) {
  if (Energy->data_valid[0] > ENERGY_WATCHDOG) {
    Hlw.cf1_voltage_pulse_length  = 0;
    Hlw.cf1_current_pulse_length = 0;
    Hlw.cf_power_pulse_length  = 0;
  } else {
    if (Hlw.energy_period_counter) {

      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HLW: EPC %u, CFlen %d usec"), Hlw.energy_period_counter, Hlw.cf_pulse_length);

      uint32_t hlw_len = 10000 * 100 / Hlw.energy_period_counter;  // Add *100 to fix rounding on loads at 3.6kW (#9160)
      Hlw.energy_period_counter = 0;
      if (hlw_len) {
        Energy->kWhtoday_delta[0] += (((Hlw.power_ratio * EnergyGetCalibration(ENERGY_POWER_CALIBRATION)) / 36) * 100) / hlw_len;
//        Energy->kWhtoday_delta[0] += Energy->active_power[0] * 1000 / 36;
        EnergyUpdateToday();
      }
    }
  }
}

void HlwSnsInit(void) {
  if (!EnergyGetCalibration(ENERGY_POWER_CALIBRATION) || (4975 == EnergyGetCalibration(ENERGY_POWER_CALIBRATION))) {
    EnergySetCalibration(ENERGY_POWER_CALIBRATION, HLW_PREF_PULSE);
    EnergySetCalibration(ENERGY_VOLTAGE_CALIBRATION, HLW_UREF_PULSE);
    EnergySetCalibration(ENERGY_CURRENT_CALIBRATION, HLW_IREF_PULSE);
  }

  if (Hlw.model_type) {
    Hlw.power_ratio = HJL_PREF;
    Hlw.voltage_ratio = HJL_UREF;
    Hlw.current_ratio = HJL_IREF;
  } else {
    Hlw.power_ratio = HLW_PREF;
    Hlw.voltage_ratio = HLW_UREF;
    Hlw.current_ratio = HLW_IREF;
  }

  if (PinUsed(GPIO_NRG_SEL)) {
    pinMode(Pin(GPIO_NRG_SEL), OUTPUT);
    digitalWrite(Pin(GPIO_NRG_SEL), Hlw.select_ui_flag);
  }
  if (PinUsed(GPIO_NRG_CF1)) {
    pinMode(Pin(GPIO_NRG_CF1), INPUT_PULLUP);
    attachInterrupt(Pin(GPIO_NRG_CF1), HlwCf1Interrupt, FALLING);
  }
  pinMode(Pin(GPIO_HLW_CF), INPUT_PULLUP);
  attachInterrupt(Pin(GPIO_HLW_CF), HlwCfInterrupt, FALLING);
}

void HlwDrvInit(void) {
  Hlw.model_type = 0;                      // HLW8012
  if (PinUsed(GPIO_HJL_CF)) {
    SetPin(Pin(GPIO_HJL_CF), AGPIO(GPIO_HLW_CF));
    Hlw.model_type = 1;                    // HJL-01/BL0937
  }

  if (PinUsed(GPIO_HLW_CF)) {              // HLW8012 or HJL-01 based device Power monitor

    Hlw.ui_flag = true;                    // Voltage on high
    if (PinUsed(GPIO_NRG_SEL_INV)) {
      SetPin(Pin(GPIO_NRG_SEL_INV), AGPIO(GPIO_NRG_SEL));
      Hlw.ui_flag = false;                 // Voltage on low
    }

    if (PinUsed(GPIO_NRG_CF1)) {           // Voltage and/or Current monitor
      if (!PinUsed(GPIO_NRG_SEL)) {        // Voltage and/or Current selector
        Energy->current_available = false;  // Assume Voltage
      }
    } else {
      Energy->current_available = false;
      Energy->voltage_available = false;
    }
    Energy->use_overtemp = true;            // Use global temperature for overtemp detection

    TasmotaGlobal.energy_driver = XNRG_01;
  }
}

bool HlwCommand(void) {
  bool serviced = true;

  float value = CharToFloat(XdrvMailbox.data);

  if ((CMND_POWERCAL == Energy->command_code) ||
      (CMND_VOLTAGECAL == Energy->command_code) ||
      (CMND_CURRENTCAL == Energy->command_code)) {
    // Service in xdrv_03_energy.ino
  }
  else if (CMND_POWERSET == Energy->command_code) {    // xxx.x W
    if (XdrvMailbox.data_len && Hlw.cf_power_pulse_length ) {
      XdrvMailbox.payload = ((uint32_t)(value * 10) * Hlw.cf_power_pulse_length ) / Hlw.power_ratio;
    }
  }
  else if (CMND_VOLTAGESET == Energy->command_code) {  // xxx.x V
    if (XdrvMailbox.data_len && Hlw.cf1_voltage_pulse_length ) {
      XdrvMailbox.payload = ((uint32_t)(value * 10) * Hlw.cf1_voltage_pulse_length ) / Hlw.voltage_ratio;
    }
  }
  else if (CMND_CURRENTSET == Energy->command_code) {  // xxx mA
    if (XdrvMailbox.data_len && Hlw.cf1_current_pulse_length) {
      XdrvMailbox.payload = ((uint32_t)(value) * Hlw.cf1_current_pulse_length) / Hlw.current_ratio;
    }
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xnrg01(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_EVERY_200_MSECOND:
      HlwEvery200ms();
      break;
    case FUNC_ENERGY_EVERY_SECOND:
      HlwEverySecond();
      break;
    case FUNC_COMMAND:
      result = HlwCommand();
      break;
    case FUNC_INIT:
      HlwSnsInit();
      break;
    case FUNC_PRE_INIT:
      HlwDrvInit();
      break;
  }
  return result;
}

#endif  // USE_HLW8012
#endif  // USE_ENERGY_SENSOR
