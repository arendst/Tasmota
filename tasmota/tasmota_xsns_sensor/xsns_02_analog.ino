/*
  xsns_02_analog.ino - ADC support for Tasmota

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

#ifndef FIRMWARE_MINIMAL

#ifdef USE_ADC
/*********************************************************************************************\
 * ADC support for ESP8266 GPIO17 (=PIN_A0) and ESP32 up to 8 channels on GPIO32 to GPIO39
 * 
 * Command AdcParam<x> allows for configuration of multiple sequential ADC GPIOs.
 * Due to it's sequential nature it loses configurations when in-between ADC GPIOs are redefined.
 * ---------------------------------    --------------------------------------------------------------------------------------------------
 * AdcParam<channel> <parameters>
 * AdcParam1 1                       <- Set first ADC GPIO found, counting from GPIO0 up, to default parameters based on configured GPIO_ADC_xxxx
 * AdcParam1 1,32000,40000,3350      <- Temperature parameters for first ADC GPIO found, counting from GPIO0 up, and configured as GPIO_ADC_TEMP
 * AdcParam2 1,511                   <- Button parameter for second ADC GPIO found, counting from GPIO0 up, and configured as GPIO_ADC_BUTTON1
 * AdcParam3 1,511                   <- Button parameter for third ADC GPIO found, counting from GPIO0 up, and configured as GPIO_ADC_BUTTON2
 * 
 * Version v14.1.0.4 supersedes previous command with command AdcGpio for easier configuration of multiple ADC GPIOs.
 * As it is GPIO based it will not loose configurations when in-between GPIOs are redefined.
 * ---------------------------------    --------------------------------------------------------------------------------------------------
 * AdcGpio<gpio> <parameters>
 * AdcGpio 1                         <- ESP8266 Set ADC on GPIO17 to default parameters based on configured GPIO_ADC_xxxx
 * AdcGpio 32000,40000,3350          <- ESP8266 Temperature parameters for ADC on GPIO17 configured as GPIO_ADC_TEMP (no index needed)
 * AdcGpio33 1                       <- ESP32 Set ADC on GPIO33 to default parameters based on configured GPIO_ADC_xxxx
 * AdcGpio33 32000,40000,3350        <- ESP32 Temperature parameters for ADC on GPIO33 configured as GPIO_ADC_TEMP
 * AdcGpio37 511                     <- ESP32 Button parameter for ADC on GPIO37 configured as GPIO_ADC_BUTTON1
 * 
 * 
 * For shelly RGBW PM:
 * Template {"NAME":"Shelly Plus RGBW PM Pot.meter","GPIO":[1,0,0,0,419,0,0,0,0,0,544,0,0,0,0,1,0,0,1,0,0,416,417,418,0,0,0,0,0,4736,11264,11296,4704,0,4705,0],"FLAG":0,"BASE":1}
 * AdcGpio33 32000,40000,3350        <- Temperature parameters
 * AdcGpio34 1161,3472,10,30         <- Voltage parameters
 * AdcGpio35 960,1017,0.01,0.706     <- Current parameters
 * AdcGpio36 1552,176,3,1            <- I1 Potentiometer RGBW or RGB if SO37 128
 * AdcGpio36 1552,176,3,3            <- I1 Potentiometer RGBW or RGB if SO37 128 without fading
 * AdcGpio38 1552,176,3,1            <- I3 Potentiometer W if SO37 128
 * Fade 1
 * Speed 6
 * VoltRes 2
 * WattRes 2
 * 
 * Template {"NAME":"Shelly Plus RGBW PM Button","GPIO":[1,0,0,0,419,0,0,0,0,0,544,0,0,0,0,1,0,0,1,0,0,416,417,418,0,0,0,0,0,4736,11264,11296,4800,4801,4802,4803],"FLAG":0,"BASE":1}
 * AdcGpio33 32000,40000,3350        <- Temperature parameters
 * AdcGpio34 1161,3472,10,30         <- Voltage parameters
 * AdcGpio35 960,1017,0.01,0.706     <- Current parameters
 * AdcGpio36 511                     <- I1 ADC Button
 * AdcGpio37 511                     <- I2 ADC Button
 * AdcGpio38 511                     <- I3 ADC Button
 * AdcGpio39 511                     <- I4 ADC Button
 * Fade 1
 * Speed 6
 * VoltRes 2
 * WattRes 2
\*********************************************************************************************/

#define XSNS_02                       2
#if defined(ESP32) && defined(USE_ENERGY_SENSOR)
// Only ESP32 and up support more than one ADC channel enabling energy driver
#define XNRG_33                       33
#endif  // ESP32 and USE_ENERGY_SENSOR

#ifdef ESP32
#include "esp32-hal-adc.h"
#endif

#ifdef ESP8266
#define ANALOG_RESOLUTION             10               // 12 = 4095, 11 = 2047, 10 = 1023
#define ANALOG_RANGE                  1023             // 4095 = 12, 2047 = 11, 1023 = 10
#endif  // ESP8266

#ifdef ESP32
#undef ANALOG_RESOLUTION
#define ANALOG_RESOLUTION             12               // 12 = 4095, 11 = 2047, 10 = 1023
#undef ANALOG_RANGE
#define ANALOG_RANGE                  4095             // 4095 = 12, 2047 = 11, 1023 = 10
#endif  // ESP32

#define ANALOG_MARGIN                 5                // backward compatible div10 range

#define TO_CELSIUS(x) ((x) - 273.15f)
#define TO_KELVIN(x) ((x) + 273.15f)

// Parameters for equation
#define ANALOG_V33                    3.3f              // ESP8266 / ESP32 Analog voltage
#define ANALOG_T0                     TO_KELVIN(25.0f)  // 25 degrees Celsius in Kelvin (= 298.15)

// Mode 0 : Shelly 2.5 NTC Thermistor
// 3V3 --- ANALOG_NTC_BRIDGE_RESISTANCE ---v--- NTC --- Gnd
//                                         |
//                                        ADC0
// Mode 1 : NTC towards 3V3 (Sinilink Thermostat Relay Board (XY-WFT1)
// 3V3 --- NTC ---v--- ANALOG_NTC_BRIDGE_RESISTANCE --- Gnd
//                |
//               ADC0
#define ANALOG_NTC_BRIDGE_RESISTANCE  32000            // NTC Voltage bridge resistor
#define ANALOG_NTC_RESISTANCE         10000            // NTC Resistance
#define ANALOG_NTC_B_COEFFICIENT      3350             // NTC Beta Coefficient

// LDR parameters (example as used on Ulanzi)
// 3V3 --- LDR ---v--- ANALOG_LDR_BRIDGE_RESISTANCE --- Gnd
//                |
//               ADC0
#define ANALOG_LDR_BRIDGE_RESISTANCE  10000            // LDR Voltage bridge resistor
#define ANALOG_LDR_LUX_CALC_SCALAR    12518931         // Experimental
#define ANALOG_LDR_LUX_CALC_EXPONENT  -1.4050f         // Experimental

// CT Based Apparrent Power Measurement Parameters
// 3V3 --- R1 ----v--- R1 --- Gnd
//                |
//         CT+   CT-
//          |
//        ADC0
// Default settings for a 20A/1V Current Transformer.
// Analog peak to peak range is measured and converted to RMS current using ANALOG_CT_MULTIPLIER
#define ANALOG_CT_FLAGS               0                // (uint32_t) reserved for possible future use
#define ANALOG_CT_MULTIPLIER          2146             // (uint32_t) Multiplier*100000 to convert raw ADC peak to peak range 0..ANALOG_RANGE to RMS current in Amps. Value of 100000 corresponds to 1
#define ANALOG_CT_VOLTAGE             2300             // (int) Convert current in Amps to apparrent power in Watts using voltage in Volts*10. Value of 2200 corresponds to 220V

#define CT_FLAG_ENERGY_RESET          (1 << 0)         // Reset energy total

// Buttons
//        ---- Inverted
// 3V3 ---|  |----|
//                |
// 3V3 --- R1 ----|--- R1 --- Gnd
//                |
//                |---|  |--- Gnd
//                |   ----
//               ADC
#define ANALOG_BUTTON_THRESHOLD       ANALOG_RANGE / 8  // Add resistor tolerance

// Odroid joysticks
//        ---- Up
// 3V3 ---|  |------------
//                        |
//        ---- Dn         |--- R10k --- Gnd
// 3V3 ---|  |--- R10k ---|
//                        |
//                       ADC
// Press "Up" will raise ADC to ANALOG_RANGE, Press "Dn" will raise ADC to ANALOG_RANGE/2
#define ANALOG_JOYSTICK_THRESHOLD     (ANALOG_RANGE / 3) +100  // Add resistor tolerance

// pH scale minimum and maximum values
#define ANALOG_PH_MAX                             14.0f
#define ANALOG_PH_MIN                             0.0f

// Default values for calibration solution with lower PH
#define ANALOG_PH_CALSOLUTION_LOW_PH              4.0f
#define ANALOG_PH_CALSOLUTION_LOW_ANALOG_VALUE    282
// Default values for calibration solution with higher PH
#define ANALOG_PH_CALSOLUTION_HIGH_PH             9.18f
#define ANALOG_PH_CALSOLUTION_HIGH_ANALOG_VALUE   435

// Multiplier used to store pH with 2 decimal places in a non decimal datatype
#define ANALOG_PH_DECIMAL_MULTIPLIER              100.0f

// MQ-X sensor (MQ-02, MQ-03, MQ-04, MQ-05, MQ-06, MQ-07, MQ-08, MQ-09, MQ-131, MQ-135)
//
// A0  -------------------
//                        |
// GND -----------        |
//                |       |
// VCC ---        |       |
//        |       |       |
//       3V3     GND     ADC  <- (A0 for nodemcu, wemos; GPIO34,35,36,39 and other analog IN/OUT pin for esp32)
//means mq type (ex for mq-02 use 2, mq-131 use 131)
#define ANALOG_MQ_TYPE                            2
//exponential regression a params
#define ANALOG_MQ_A                               574.25f
//exponential regression b params
#define ANALOG_MQ_B                               -2.222f
/*
    Exponential regression:
  Gas    | a      | b
  LPG    | 44771  | -3.245
  CH4    | 2*10^31| 19.01
  CO     | 521853 | -3.821
  Alcohol| 0.3934 | -1.504
  Benzene| 4.8387 | -2.68
  Hexane | 7585.3 | -2.849
  NOx    | -462.43 | -2.204
  CL2    | 47.209 | -1.186
  O3     | 23.943 | -1.11
*/
//ratio for alarm, NOT USED yet (RS / R0 = 15 ppm)
#define ANALOG_MQ_RatioMQCleanAir                 15.0f
// Multiplier used to store pH with 2 decimal places in a non decimal datatype
#define ANALOG_MQ_DECIMAL_MULTIPLIER              100.0f
// lenght of filter
#define ANALOG_MQ_SAMPLES                         60

/*********************************************************************************************/

struct {
  uint8_t present;
} Adcs;

struct {
  float *mq_samples;
  float temperature;
  float current;
  float energy;
  int param[4];
  int indexOfPointer;
  uint32_t previous_millis;
  uint16_t last_value;
  uint16_t type;
  uint8_t index;
  uint8_t pin;
} Adc[MAX_ADCS];

/*********************************************************************************************\
 * External use
\*********************************************************************************************/

uint32_t AdcRange(void) {
  return ANALOG_RANGE;
}

bool AdcPin(uint32_t pin) {
  for (uint32_t channel = 0; channel < Adcs.present; channel++) {
    if (pin == Adc[channel].pin) {
      return true;
    }
  }
  return false;
}

/*********************************************************************************************/

#ifdef ESP32
void AdcFreeUnusedSettings(void) {
  // Go over all SET_ADC_PARAMx looking for pin numbers currently used on channels
  // Clear non used freeing global text space
  char parameters[40];
  for (uint32_t param_idx = 0; param_idx <= MAX_ADCS; param_idx++) {
    if (strchr(SettingsText(SET_ADC_PARAM1 + param_idx), ',') != nullptr) {
      uint32_t pin = atoi(subStr(parameters, SettingsText(SET_ADC_PARAM1 + param_idx), ",", 6));
      uint32_t channel;
      for (channel = 0; channel < Adcs.present; channel++) {
        if (pin == Adc[channel].pin) { break; }
      }
      if (channel == Adcs.present) {
        SettingsUpdateText(SET_ADC_PARAM1 + param_idx, "");
      }
    }
  }
}
#endif  // ESP32

int AdcFindSlot(uint32_t channel) {
  char parameters[40];
  for (uint32_t param_idx = 0; param_idx < MAX_ADCS; param_idx++) {
    if (strchr(SettingsText(SET_ADC_PARAM1 + param_idx), ',') != nullptr) {
//      if (Adc[channel].pin == atoi(subStr(parameters, SettingsText(SET_ADC_PARAM1 + param_idx), ",", 6))) {  // Assuming pin 0 is never an ADC channel
      subStr(parameters, SettingsText(SET_ADC_PARAM1 + param_idx), ",", 6);
      if (strlen(parameters) && (atoi(parameters) == Adc[channel].pin)) {
        return param_idx;  // Found
      }
    }
  }
  return -1;               // Not found
}

void AdcSaveSettings(uint32_t channel) {
  char parameters[40];
  snprintf_P(parameters, sizeof(parameters), PSTR("%d,%d,%d,%d,%d,%d"),
    Adc[channel].type,
    Adc[channel].param[0], Adc[channel].param[1], Adc[channel].param[2], Adc[channel].param[3],
    Adc[channel].pin);

#ifdef ESP8266
  SettingsUpdateText(SET_ADC_PARAM1, parameters);                    // Save in only slot
#else  // ESP32
  // Find used slot based on channel pin. If not find a free slot.
  int param_idx = AdcFindSlot(channel);
  if (-1 == param_idx) {
    for (param_idx = 0; param_idx < MAX_ADCS; param_idx++) {         // Find a free slot
      if (strchr(SettingsText(SET_ADC_PARAM1 + param_idx), ',') == nullptr) {
        break;
      }
    }
  }
  SettingsUpdateText(SET_ADC_PARAM1 + param_idx, parameters);       // Save in current slot
#endif  // ESP32
}

uint32_t AdcGetType(uint32_t channel, uint32_t param_idx) {
  // Get params and adc_type
  char parameters[40];
  for (uint32_t i = 0; i < 4; i++) {
    Adc[channel].param[i] = atoi(subStr(parameters, SettingsText(SET_ADC_PARAM1 + param_idx), ",", i +2));
  }
  return atoi(subStr(parameters, SettingsText(SET_ADC_PARAM1 + param_idx), ",", 1));
}

bool AdcGetSettings(uint32_t channel) {
  uint32_t adc_type = 0;
  // Find corresponding pin (since v14.1.0.4)
  int param_idx = AdcFindSlot(channel);
  if (param_idx > -1) {
    // Param is 148,32000,10000,3350,0,17
    adc_type = AdcGetType(channel, param_idx);
  } else {
    // Legacy support (No pin in param)
    // Param is 1,32000,10000,33500000,0 or 148,32000,10000,33500000,0 or 148,32000,10000,3350,0
    if (strchr(SettingsText(SET_ADC_PARAM1 + channel), ',') != nullptr) {
      adc_type = AdcGetType(channel, channel);
      if ((adc_type > 0) && (adc_type < GPIO_ADC_INPUT)) {  // Former ADC_END
        adc_type = Adc[channel].type;           // Migrate adc_type from 1..12 to UserSelectablePins index
      }
      if (GPIO_ADC_INPUT == adc_type) {
        Adc[channel].param[2] = ANALOG_MARGIN;  // Margin / Tolerance
        Adc[channel].param[3] = 0;              // Default mode (0) or Direct mode (1) using Dimmer or Channel command
      }
      if ((GPIO_ADC_TEMP == adc_type) && (Adc[channel].param[2] > 1000000)) {
        Adc[channel].param[2] /= 10000;         // Fix legacy value from 33500000 to 3350
      }
      AdcSaveSettings(channel);                 // Add pin
    }
  }
  return (Adc[channel].type == adc_type);
}

void AdcInitParams(uint32_t channel) {
  Adc[channel].param[0] = 0;
  Adc[channel].param[1] = 0;
  Adc[channel].param[2] = 0;
  Adc[channel].param[3] = 0;
  uint32_t adc_type = Adc[channel].type;
  switch (adc_type) {
    case GPIO_ADC_INPUT:
//      Adc[channel].param[0] = 0;
      Adc[channel].param[1] = ANALOG_RANGE;
      Adc[channel].param[2] = ANALOG_MARGIN;             // Margin / Tolerance
//      Adc[channel].param[3] = 0;                       // Default mode (0) or Direct mode (1) using Dimmer or Channel command
      break;
    case GPIO_ADC_TEMP:
      // Default Shelly 2.5 and 1PM parameters
      Adc[channel].param[0] = ANALOG_NTC_BRIDGE_RESISTANCE;
      Adc[channel].param[1] = ANALOG_NTC_RESISTANCE;
      Adc[channel].param[2] = ANALOG_NTC_B_COEFFICIENT;
//      Adc[channel].param[3] = 0;                       // Default to Shelly mode with NTC towards GND
      break;
    case GPIO_ADC_LIGHT:
      Adc[channel].param[0] = ANALOG_LDR_BRIDGE_RESISTANCE;
      Adc[channel].param[1] = ANALOG_LDR_LUX_CALC_SCALAR;
      Adc[channel].param[2] = ANALOG_LDR_LUX_CALC_EXPONENT * 10000;
//      Adc[channel].param[3] = 0;
      break;
    case GPIO_ADC_BUTTON:
    case GPIO_ADC_BUTTON_INV:
      Adc[channel].param[0] = ANALOG_BUTTON_THRESHOLD;  // Between 0 or 1
//      Adc[channel].param[1] = 0;
//      Adc[channel].param[2] = 0;
//      Adc[channel].param[3] = 0;
      break;
    case GPIO_ADC_RANGE:
//      Adc[channel].param[0] = 0;
      Adc[channel].param[1] = ANALOG_RANGE;
//      Adc[channel].param[2] = 0;
      Adc[channel].param[3] = 100;
      break;
    case GPIO_ADC_CT_POWER:
      Adc[channel].param[0] = ANALOG_CT_FLAGS;         // (uint32_t) 0
      Adc[channel].param[1] = ANALOG_CT_MULTIPLIER;    // (uint32_t) 100000
      Adc[channel].param[2] = ANALOG_CT_VOLTAGE;       // (int)      10
//      Adc[channel].param[3] = 0;
      break;
    case GPIO_ADC_JOY:
      Adc[channel].param[0] = ANALOG_JOYSTICK_THRESHOLD;
//      Adc[channel].param[1] = 0;
//      Adc[channel].param[2] = 0;
//      Adc[channel].param[3] = 0;
      break;
    case GPIO_ADC_PH:
      Adc[channel].param[0] = ANALOG_PH_CALSOLUTION_LOW_PH * ANALOG_PH_DECIMAL_MULTIPLIER;  // PH of the calibration solution 1, which is the one with the lower PH
      Adc[channel].param[1] = ANALOG_PH_CALSOLUTION_LOW_ANALOG_VALUE;                       // Reading of AnalogInput while probe is in solution 1
      Adc[channel].param[2] = ANALOG_PH_CALSOLUTION_HIGH_PH * ANALOG_PH_DECIMAL_MULTIPLIER; // PH of the calibration solution 2, which is the one with the higher PH
      Adc[channel].param[3] = ANALOG_PH_CALSOLUTION_HIGH_ANALOG_VALUE;                      // Reading of AnalogInput while probe is in solution 2
      break;
    case GPIO_ADC_MQ:
      Adc[channel].param[0] = ANALOG_MQ_TYPE;  // Could be MQ-002, MQ-004, MQ-131 ....
      Adc[channel].param[1] = (int)(ANALOG_MQ_A * ANALOG_MQ_DECIMAL_MULTIPLIER);                // Exponential regression
      Adc[channel].param[2] = (int)(ANALOG_MQ_B * ANALOG_MQ_DECIMAL_MULTIPLIER);                // Exponential regression
      Adc[channel].param[3] = (int)(ANALOG_MQ_RatioMQCleanAir * ANALOG_MQ_DECIMAL_MULTIPLIER);  // Exponential regression
      break; 
    case GPIO_ADC_VOLTAGE:
    case GPIO_ADC_CURRENT:
//      Adc[channel].param[0] = 0;
      Adc[channel].param[1] = ANALOG_RANGE;
//      Adc[channel].param[2] = 0;
      Adc[channel].param[3] = ANALOG_V33 * 10000;
      break;
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("ADC: AdcParam%d %d,%d,%d,%d,%d"), channel+1, Adc[channel].pin, Adc[channel].param[0], Adc[channel].param[1], Adc[channel].param[2], Adc[channel].param[3]);
}

void AdcInit(void) {
  Adcs.present = 0;
  memset(&Adc, 0, sizeof(Adc));

  uint32_t pin = 0;                           // ESP32 full range of GPIOs possible for ADC channels
#ifdef ESP8266  
  pin = ADC0_PIN;                             // ESP8266 single ADC channel
#endif
  for (pin; pin < nitems(TasmotaGlobal.gpio_pin); pin++) {
    uint32_t adc_type = TasmotaGlobal.gpio_pin[pin] >> 5;
    switch(adc_type) {
      case GPIO_ADC_MQ:
        Adc[Adcs.present].mq_samples = (float*)calloc(sizeof(float), ANALOG_MQ_SAMPLES);  // Need calloc to reset registers to 0
        if (nullptr == Adc[Adcs.present].mq_samples) { continue; }
      case GPIO_ADC_INPUT:
      case GPIO_ADC_TEMP:
      case GPIO_ADC_LIGHT:
      case GPIO_ADC_BUTTON:
      case GPIO_ADC_BUTTON_INV:
      case GPIO_ADC_RANGE:
      case GPIO_ADC_CT_POWER:
      case GPIO_ADC_JOY:
      case GPIO_ADC_PH:
      case GPIO_ADC_VOLTAGE:
      case GPIO_ADC_CURRENT:
        Adc[Adcs.present].indexOfPointer = -1;  // Used to skip first update of GPIO_ADC_INPUT after restart
        Adc[Adcs.present].pin = pin;
        Adc[Adcs.present].type = adc_type;
        Adc[Adcs.present].index = TasmotaGlobal.gpio_pin[pin] & 0x001F;
        Adcs.present++;
        if (Adcs.present == MAX_ADCS) { break; }
    }
  }

  if (Adcs.present) {
#ifdef ESP32
    analogReadResolution(ANALOG_RESOLUTION);  // Default 12 bits (0 - 4095)
    analogSetAttenuation(ADC_11db);           // Default 11db
#endif
    for (uint32_t channel = 0; channel < Adcs.present; channel++) {
      if (!AdcGetSettings(channel)) {
        AdcInitParams(channel);
        AdcSaveSettings(channel);
      }
    }
  }
#ifdef ESP32
  AdcFreeUnusedSettings();
#endif  // ESP32
}

/*********************************************************************************************/

uint32_t AdcRead1(uint32_t pin) {
#ifdef ESP32 
  return analogReadMilliVolts(pin) / (ANALOG_V33 * 1000) * ANALOG_RANGE;  // Go back from mV to ADC
#else
  return analogRead(pin);
#endif
}

uint32_t AdcRead(uint32_t pin, uint32_t factor) {
  // factor 1 = 2 samples
  // factor 2 = 4 samples
  // factor 3 = 8 samples
  // factor 4 = 16 samples
  // factor 5 = 32 samples
  SystemBusyDelayExecute();

  uint32_t samples = 1 << factor;
  uint32_t analog = 0;
  for (uint32_t i = 0; i < samples; i++) {
#ifdef ESP32 
    analog += analogReadMilliVolts(pin);  // get the value corrected by calibrated values from the eFuses
#else
    analog += analogRead(pin);
#endif
    delay(1);
  }
  analog >>= factor;
#ifdef ESP32
  analog = analog / (ANALOG_V33 * 1000) * ANALOG_RANGE;  // Go back from mV to ADC
#endif
  return analog;
}

void AdcEvery250ms(void) {
  char adc_channel[3] = { 0 };
  uint32_t offset = 0;

  uint32_t dimmer_count = 0;
#ifdef USE_LIGHT
  if (!light_controller.isCTRGBLinked()) {                             // SetOption37 >= 128 (Light) RGB and White channel separation (default 0)
    for (uint32_t channel = 0; channel < Adcs.present; channel++) {
      if ((GPIO_ADC_INPUT == Adc[channel].type) && (Adc[channel].param[3] > 0)) {
        dimmer_count++;
      }
    }
  }
#endif  // USE_LIGHT
  for (uint32_t channel = 0; channel < Adcs.present; channel++) {
    uint32_t type_index = Adc[channel].index;
#ifdef ESP32
    snprintf_P(adc_channel, sizeof(adc_channel), PSTR("%d"), type_index +1);
    offset = 1;
#endif
    uint32_t adc_type = Adc[channel].type;
    int param0 = Adc[channel].param[0];
    int param1 = Adc[channel].param[1];
    int param2 = Adc[channel].param[2];
    int param3 = Adc[channel].param[3];
    if (GPIO_ADC_INPUT == adc_type) {
      int adc = AdcRead(Adc[channel].pin, 4);                          // 4 = 16 mS
      bool swap = (param1 < param0);
      uint32_t lo = (swap) ? param1 : param0;
      uint32_t hi = (swap) ? param0 : param1;
      int new_value = changeUIntScale(adc, lo, hi, 0, 100);
      if (swap) {
        new_value = 100 - new_value;
      }
      if ((new_value < Adc[channel].last_value -param2) ||
          (new_value > Adc[channel].last_value +param2) ||
          ((0 == new_value) && (Adc[channel].last_value != 0)) ||      // Lowest end
          ((100 == new_value) && (Adc[channel].last_value != 100))) {  // Highest end
        Adc[channel].last_value = new_value;
        if (-1 == Adc[channel].indexOfPointer) {
          Adc[channel].indexOfPointer = 0;
          continue;                                                    // Do not use potentiometer state on restart
        }
#ifdef USE_LIGHT
        if (0 == param3) {                                             // Default (0) or Direct mode (1)
#endif  // USE_LIGHT
          Response_P(PSTR("{\"ANALOG\":{\"A%ddiv10\":%d}}"), type_index + offset, new_value);
          XdrvRulesProcess(0);
#ifdef USE_LIGHT
        } else {
          char command[33];
          if (Settings->flag3.pwm_multi_channels) {                    // SetOption68 - Enable multi-channels PWM instead of Color PWM
            snprintf_P(command, sizeof(command), PSTR(D_CMND_CHANNEL "%d %d"), type_index +1, new_value);
          } else {
            uint32_t dimmer_option;
            if (dimmer_count > 1) {
              dimmer_option = (0 == type_index) ? 1 : 2;               // Change RGB (1) or W(W) (2) dimmer
            } else {
              dimmer_option = (3 == param3) ? 3 : 0;                   // Change both RGB and W(W) Dimmers (0) with no fading (3)
            }
            snprintf_P(command, sizeof(command), PSTR(D_CMND_DIMMER "%d %d"), dimmer_option, new_value);
          }
          ExecuteCommand(command, SRC_SWITCH);
        }
#endif  // USE_LIGHT
      }
    }
    else if (GPIO_ADC_JOY == adc_type) {
      uint16_t new_value = AdcRead(Adc[channel].pin, 1);
      if (new_value && (new_value != Adc[channel].last_value)) {
        Adc[channel].last_value = new_value;
        uint16_t value = new_value / param0;
        Response_P(PSTR("{\"ANALOG\":{\"Joy%s\":%d}}"), adc_channel, value);
        XdrvRulesProcess(0);
      } else {
        Adc[channel].last_value = 0;
      }
    }
  }
}

uint8_t AdcGetButton(uint32_t pin) {
  for (uint32_t channel = 0; channel < Adcs.present; channel++) {
    if (Adc[channel].pin == pin) {
      uint32_t adc_type = Adc[channel].type;
      uint32_t adc = AdcRead(Adc[channel].pin, 1);
      uint32_t param0 = Adc[channel].param[0];
      if (GPIO_ADC_BUTTON_INV == adc_type) {
        return (adc < param0);
      }
      else if (GPIO_ADC_BUTTON == adc_type) {
        return (adc > param0);
      }
    }
  }
  return 0;
}

uint32_t AdcGetLux(uint32_t channel) {
  int adc = AdcRead(Adc[channel].pin, 2);
  // Source: https://www.allaboutcircuits.com/projects/design-a-luxmeter-using-a-light-dependent-resistor/
  float resistorVoltage = ((float)adc / ANALOG_RANGE) * ANALOG_V33;
  float ldrVoltage = ANALOG_V33 - resistorVoltage;
  float ldrResistance = ldrVoltage / resistorVoltage * (float)Adc[channel].param[0];
  float ldrLux = (float)Adc[channel].param[1] * FastPrecisePowf(ldrResistance, (float)Adc[channel].param[2] / 10000);

  return (uint32_t)ldrLux;
}

void AddSampleMq(uint32_t channel){
//  AddLog(LOG_LEVEL_DEBUG, PSTR("ADC: Adding sample for mq-sensor"));

  int _adc = AdcRead(Adc[channel].pin, 2);
  // init af array at same value
  if (Adc[channel].indexOfPointer == -1) {

//    AddLog(LOG_LEVEL_DEBUG, PSTR("ADC: Init samples for mq-sensor"));

    for (int i = 0; i < ANALOG_MQ_SAMPLES; i ++) {
      Adc[channel].mq_samples[i] = _adc;
    }
  } else {
    Adc[channel].mq_samples[Adc[channel].indexOfPointer] = _adc;
  }
  Adc[channel].indexOfPointer++;
  if (Adc[channel].indexOfPointer==ANALOG_MQ_SAMPLES) {
    Adc[channel].indexOfPointer=0;
  }
}

float AdcGetMq(uint32_t channel) {
//  AddLog(LOG_LEVEL_DEBUG, PSTR("ADC: Getting value for mq-sensor"));

  float avg = 0.0;
  for (int i = 0; i < ANALOG_MQ_SAMPLES; i ++) {
    avg += Adc[channel].mq_samples[i];
  }
  float voltage = (avg / ANALOG_MQ_SAMPLES) * ANALOG_V33 / ANALOG_RANGE;

  float _RL = 10;                                        // Value in KiloOhms
  float _RS_Calc = ((ANALOG_V33 * _RL) / voltage) -_RL;  // Get value of RS in a gas
  if (_RS_Calc < 0) {
    _RS_Calc = 0;                                        // No negative values accepted.
  }

  float _R0 = 10;
  float _ratio = _RS_Calc / _R0;                         // Get ratio RS_gas/RS_air
  float ppm = Adc[channel].param[1] / ANALOG_MQ_DECIMAL_MULTIPLIER * FastPrecisePowf(_ratio, Adc[channel].param[2] / ANALOG_MQ_DECIMAL_MULTIPLIER);  // Source excel analisis https://github.com/miguel5612/MQSensorsLib_Docs/tree/master/Internal_design_documents
  if (ppm < 0) { ppm = 0; }                              // No negative values accepted or upper datasheet recomendation.
  if (ppm > 100000) { ppm = 100000; }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("ADC: Ppm read. ADC-RAW: %2_f, ppm: %2_f"), &voltage, &ppm);

  return ppm;
}

float AdcGetPh(uint32_t channel) {
  int adc = AdcRead(Adc[channel].pin, 2);

  float y1 = (float)Adc[channel].param[0] / ANALOG_PH_DECIMAL_MULTIPLIER;
  int32_t x1 = Adc[channel].param[1];
  float y2 = (float)Adc[channel].param[2] / ANALOG_PH_DECIMAL_MULTIPLIER;
  int32_t x2 = Adc[channel].param[3];

  float m = (y2 - y1) / (float)(x2 - x1);
  float ph = m * (float)(adc - x1) + y1;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("ADC: Analog pH read. ADC-RAW: %d, cal-low(pH=ADC): %2_f = %d, cal-high(pH=ADC): %2_f = %d"), adc, &y1, x1, &y2, x2);

  return ph;
}

float AdcGetRange(uint32_t channel) {
  // formula for calibration: value, fromLow, fromHigh, toLow, toHigh
  // Example: 514, 632, 236, 0, 100
  // int( ((<param2> - <analog-value>) / (<param2> - <param1>) ) * (<param3> - <param4>) ) + <param4> )
  int adc = AdcRead(Adc[channel].pin, 5);
  float adcrange = ( ((float)Adc[channel].param[1] - (float)adc) / ( ((float)Adc[channel].param[1] - (float)Adc[channel].param[0])) * ((float)Adc[channel].param[2] - (float)Adc[channel].param[3]) + (float)Adc[channel].param[3] );
  return adcrange;
}

void AdcGetCurrentPower(uint32_t channel, uint32_t factor) {
  // factor 1 = 2 samples
  // factor 2 = 4 samples
  // factor 3 = 8 samples
  // factor 4 = 16 samples
  // factor 5 = 32 samples
  uint32_t samples = 1 << factor;
  uint32_t analog = 0;
  uint32_t analog_min = ANALOG_RANGE;
  uint32_t analog_max = 0;

  if (0 == Adc[channel].param[0]) {
    uint32_t tstart = millis();
    while (millis()-tstart < 35) {
      analog = AdcRead1(Adc[channel].pin);
      if (analog < analog_min) {
        analog_min = analog;
      }
      if (analog > analog_max) {
        analog_max = analog;
      }
    }
    //AddLog(0, PSTR("min: %u, max:%u, dif:%u"), analog_min, analog_max, analog_max-analog_min);
    Adc[channel].current = (float)(analog_max-analog_min) * ((float)(Adc[channel].param[1]) / 100000);
    if (Adc[channel].current < (((float)Adc[channel].param[3]) / 10000.0)) {
      Adc[channel].current = 0.0;
    }
  } else {
    analog = AdcRead(Adc[channel].pin, 5);
    if (analog > Adc[channel].param[0]) {
      Adc[channel].current = ((float)(analog) - (float)Adc[channel].param[0]) * ((float)(Adc[channel].param[1]) / 100000);
    } else {
      Adc[channel].current = 0;
    }
  }

  float power = Adc[channel].current * (float)(Adc[channel].param[2]) / 10;
  uint32_t current_millis = millis();
  Adc[channel].energy = Adc[channel].energy + ((power * (current_millis - Adc[channel].previous_millis)) / 3600000000);
  Adc[channel].previous_millis = current_millis;
}

void AdcEverySecond(void) {
  for (uint32_t channel = 0; channel < Adcs.present; channel++) {
    uint32_t type_index = Adc[channel].index;
    uint32_t adc_type = Adc[channel].type;
    int param0 = Adc[channel].param[0];
    int param1 = Adc[channel].param[1];
    int param2 = Adc[channel].param[2];
    int param3 = Adc[channel].param[3];
    if (GPIO_ADC_TEMP == adc_type) {
      int adc = AdcRead(Adc[channel].pin, 2);
      // Steinhart-Hart equation for thermistor as temperature sensor:
      // float Rt = (adc * Adc[channel].param[0] * MAX_ADC_V) / (ANALOG_RANGE * ANALOG_V33 - (float)adc * MAX_ADC_V);
      // MAX_ADC_V in ESP8266 is 1
      // MAX_ADC_V in ESP32 is 3.3
      float Rt;
#ifdef ESP8266
      if (param3) { // Alternate mode
        Rt = (float)param0 * (ANALOG_RANGE * ANALOG_V33 - (float)adc) / (float)adc;
      } else {
        Rt = (float)param0 * (float)adc / (ANALOG_RANGE * ANALOG_V33 - (float)adc);
      }
#else
      if (param3) { // Alternate mode
        Rt = (float)param0 * (ANALOG_RANGE - (float)adc) / (float)adc;
      } else {
        Rt = (float)param0 * (float)adc / (ANALOG_RANGE - (float)adc);
      }
#endif
      float BC = (float)param2;                                              // Shelly param3 = 3350 (ANALOG_NTC_B_COEFFICIENT)
      float T = BC / (BC / ANALOG_T0 + TaylorLog(Rt / (float)param1));       // Shelly param2 = 10000 (ANALOG_NTC_RESISTANCE)
      Adc[channel].temperature = ConvertTemp(TO_CELSIUS(T));
    }
    else if (GPIO_ADC_CT_POWER == adc_type) {
      AdcGetCurrentPower(channel, 5);
    }
    else if (GPIO_ADC_MQ == adc_type) {
      AddSampleMq(channel);
      AdcGetMq(channel);
    }
  }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void AdcShowContinuation(bool *jsonflg) {
  if (*jsonflg) {
    ResponseAppend_P(PSTR(","));
  } else {
    ResponseAppend_P(PSTR(",\"ANALOG\":{"));
    *jsonflg = true;
  }
}

enum DomoFlagsAdc { ADC_TEMP, ADC_LIGHT, ADC_CT_POWER, ADC_END };

void AdcShow(bool json) {
  bool domo_flag[ADC_END] = { false };
  char adc_name[10] = { 0 };  // ANALOG8
  char adc_channel[3] = { 0 };
  uint32_t offset = 0;

  bool jsonflg = false;
  for (uint32_t channel = 0; channel < Adcs.present; channel++) {
    uint32_t type_index = Adc[channel].index;
#ifdef ESP32
    snprintf_P(adc_name, sizeof(adc_name), PSTR("ADC%d"), type_index +1);
    snprintf_P(adc_channel, sizeof(adc_channel), PSTR("%d"), type_index +1);
    offset = 1;
#endif
    uint32_t adc_type = Adc[channel].type;
    switch (adc_type) {
      case GPIO_ADC_INPUT: {
#ifdef USE_LIGHT
        if (0 == Adc[channel].param[3]) {                                // Default (0) or Direct mode (1)
#endif  // USE_LIGHT
          uint16_t analog = AdcRead(Adc[channel].pin, 5);
          if (json) {
            AdcShowContinuation(&jsonflg);
            ResponseAppend_P(PSTR("\"A%d\":%d"), type_index + offset, analog);
#ifdef USE_WEBSERVER
          } else {
            WSContentSend_PD(HTTP_SNS_ANALOG, "", type_index + offset, analog);
#endif  // USE_WEBSERVER
          }
#ifdef USE_LIGHT
        }
#endif  // USE_LIGHT
        break;
      }
      case GPIO_ADC_TEMP: {
        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "%s\":%*_f"), adc_channel, Settings->flag2.temperature_resolution, &Adc[channel].temperature);
          if ((0 == TasmotaGlobal.tele_period) && (!domo_flag[ADC_TEMP])) {
#ifdef USE_DOMOTICZ
            DomoticzFloatSensor(DZ_TEMP, Adc[channel].temperature);
            domo_flag[ADC_TEMP] = true;
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
            KnxSensor(KNX_TEMPERATURE, Adc[channel].temperature);
#endif  // USE_KNX
          }
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_Temp(adc_name, Adc[channel].temperature);
#endif  // USE_WEBSERVER
        }
        break;
      }
      case GPIO_ADC_LIGHT: {
        uint32_t adc_light = AdcGetLux(channel);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"" D_JSON_ILLUMINANCE "%s\":%d"), adc_channel, adc_light);
#ifdef USE_DOMOTICZ
          if ((0 == TasmotaGlobal.tele_period) && (!domo_flag[ADC_LIGHT])) {
            DomoticzSensor(DZ_ILLUMINANCE, adc_light);
            domo_flag[ADC_LIGHT] = true;
          }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_ILLUMINANCE, adc_name, adc_light);
#endif  // USE_WEBSERVER
        }
        break;
      }
      case GPIO_ADC_RANGE: {
        float adc_range = AdcGetRange(channel);
        char range_chr[FLOATSZ];
        dtostrfd(adc_range, Settings->flag2.frequency_resolution, range_chr);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"" D_JSON_RANGE "%s\":%s"), adc_channel, range_chr);
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_RANGE_CHR, adc_name, range_chr);
#endif  // USE_WEBSERVER
        }
        break;
      }
      case GPIO_ADC_CT_POWER: {
        AdcGetCurrentPower(channel, 5);

        float voltage = (float)(Adc[channel].param[2]) / 10;
        char voltage_chr[FLOATSZ];
        dtostrfd(voltage, Settings->flag2.voltage_resolution, voltage_chr);
        char current_chr[FLOATSZ];
        dtostrfd(Adc[channel].current, Settings->flag2.current_resolution, current_chr);
        char power_chr[FLOATSZ];
        dtostrfd(voltage * Adc[channel].current, Settings->flag2.wattage_resolution, power_chr);
        char energy_chr[FLOATSZ];
        dtostrfd(Adc[channel].energy, Settings->flag2.energy_resolution, energy_chr);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"CTEnergy%s\":{\"" D_JSON_ENERGY "\":%s,\"" D_JSON_POWERUSAGE "\":%s,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s}"),
            adc_channel, energy_chr, power_chr, voltage_chr, current_chr);
#ifdef USE_DOMOTICZ
          if ((0 == TasmotaGlobal.tele_period) && (!domo_flag[ADC_CT_POWER])) {
            DomoticzSensor(DZ_POWER_ENERGY, power_chr);
            DomoticzSensor(DZ_VOLTAGE, voltage_chr);
            DomoticzSensor(DZ_CURRENT, current_chr);
            domo_flag[ADC_CT_POWER] = true;
          }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_VOLTAGE, voltage_chr);
          WSContentSend_PD(HTTP_SNS_CURRENT, current_chr);
          WSContentSend_PD(HTTP_SNS_POWER, power_chr);
          WSContentSend_PD(HTTP_SNS_ENERGY_TOTAL, energy_chr);
#endif  // USE_WEBSERVER
        }
        break;
      }
      case GPIO_ADC_JOY: {
        uint16_t new_value = AdcRead(Adc[channel].pin, 1);
        uint16_t value = new_value / Adc[channel].param[0];
        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"Joy%s\":%d"), adc_channel, value);
        }
        break;
      }
      case GPIO_ADC_PH: {
        float ph = AdcGetPh(channel);
        char ph_chr[FLOATSZ];
        dtostrfd(ph, 2, ph_chr);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"pH%s\":%s"), adc_channel, ph_chr);
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_PH, "", ph_chr);
#endif // USE_WEBSERVER
        }
        break;
      }
      case GPIO_ADC_MQ: {
        float mq = AdcGetMq(channel);
        char mq_chr[FLOATSZ];
        dtostrfd(mq, 2, mq_chr);

        float mqnumber =Adc[channel].param[0];
        char mqnumber_chr[FLOATSZ];
        dtostrfd(mqnumber, 0, mqnumber_chr);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"MQ%d_%d\":%s"), Adc[channel].param[0], type_index + offset, mq_chr);
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_MQ, mqnumber_chr, mq_chr);
#endif // USE_WEBSERVER
        }
        break;
      }
      case GPIO_ADC_VOLTAGE: 
#if defined(ESP32) && defined(USE_ENERGY_SENSOR)
        if (TasmotaGlobal.energy_driver != XNRG_33)
#endif  // ESP32 and USE_ENERGY_SENSOR
        {
          float value = AdcGetRange(channel) / 10000;   // Volt
          if (value < 0.0f) { value = 0.0f; }           // Disregard negative values
          if (json) {
            AdcShowContinuation(&jsonflg);
            ResponseAppend_P(PSTR("\"" D_JSON_VOLTAGE "%s\":%*_f"), adc_channel, Settings->flag2.voltage_resolution, &value);
#ifdef USE_WEBSERVER
          } else {
//            WSContentSend_Voltage(adc_name, value);
            WSContentSend_PD(HTTP_SNS_F_VOLTAGE, adc_name, Settings->flag2.voltage_resolution, &value);
#endif // USE_WEBSERVER
          }
        }
        break;
      case GPIO_ADC_CURRENT: 
#if defined(ESP32) && defined(USE_ENERGY_SENSOR)
        if (TasmotaGlobal.energy_driver != XNRG_33)
#endif  // ESP32 and USE_ENERGY_SENSOR
        {
          float value = AdcGetRange(channel) / 10000;   // Ampere
          if (value < 0.0f) { value = 0.0f; }           // Disregard negative values
          if (json) {
            AdcShowContinuation(&jsonflg);
            ResponseAppend_P(PSTR("\"" D_JSON_CURRENT "%s\":%*_f"), adc_channel, Settings->flag2.current_resolution, &value);
#ifdef USE_WEBSERVER
          } else {
            WSContentSend_PD(HTTP_SNS_F_CURRENT, adc_name, Settings->flag2.current_resolution, &value);
#endif // USE_WEBSERVER
          }
        }
        break;
    }
  }
  if (jsonflg) {
    ResponseJsonEnd();
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kAdcCommands[] PROGMEM = "Adc|"  // No prefix
  D_CMND_ADCPARAM "|" D_CMND_ADCGPIO;

void (* const AdcCommand[])(void) PROGMEM = {
  &CmndAdcParam, &CmndAdcGpio };

uint32_t Decimals(int value) {
  uint32_t decimals;
  for (decimals = 4; decimals > 0; decimals--) {
    if (value % 10) { break; }
    value /= 10;
  }
  return decimals;
}

void CmndAdcGpio(void) {
  // AdcGpio33 1                           Set to default 
  // AdcGpio33 32000, 10000, 3350          ADC_TEMP Shelly mode
  for (uint32_t channel = 0; channel < Adcs.present; channel++) {
#ifdef ESP8266
    // AdcGpio 32000, 10000, 3350          ADC_TEMP Shelly mode
    XdrvMailbox.index = Adc[channel].pin;
#endif
    if (XdrvMailbox.index == Adc[channel].pin) {
      XdrvMailbox.index = channel +1;
      if (XdrvMailbox.data_len) {
        char data[64];
        snprintf_P(data, sizeof(data), PSTR("1,%s"), XdrvMailbox.data);
        XdrvMailbox.data = data;
        XdrvMailbox.data_len = strlen(data);
      }
      CmndAdcParam();
      break;
    }
  }
}

void CmndAdcParam(void) {
  // AdcParam 1, 0, ANALOG_RANGE, 0          ADC_INPUT rule | dimmer
  // AdcParam 1, 32000, 10000, 3350          ADC_TEMP Shelly mode
  // AdcParam 1, 32000, 10000, 3350, 1       ADC_TEMP Alternate mode
  // AdcParam 1, 10000, 12518931, -1.405
  // AdcParam 1, 128, 0, 0
  // AdcParam 1, 128, 0, 0
  // AdcParam 1, 0, ANALOG_RANGE, 0, 100     ADC_RANGE
  // AdcParam 1, 0, 2146, 0.23
  // AdcParam 1, 1000, 0, 0
  // AdcParam 1,                             ADC_PH
  // AdcParam 1,                             ADC_MQ
  // AdcParam 1, 0, ANALOG_RANGE, 0, 3.3     ADC_VOLTAGE
  // AdcParam 1, 0, ANALOG_RANGE, 0, 3.3     ADC_CURRENT
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_ADCS)) {
    uint8_t channel = XdrvMailbox.index -1;
    uint32_t adc_type = Adc[channel].type;
    if (XdrvMailbox.data_len) {
      AdcGetSettings(channel);
      if (ArgC() > 2) {  // Process parameter entry
        char argument[XdrvMailbox.data_len];
        Adc[channel].param[0] = strtol(ArgV(argument, 2), nullptr, 10);             // param1 = int
        Adc[channel].param[1] = strtol(ArgV(argument, 3), nullptr, 10);             // param2 = int
        if ((GPIO_ADC_INPUT == adc_type) ||
            (GPIO_ADC_TEMP == adc_type) ||
            (GPIO_ADC_RANGE == adc_type)) {
          Adc[channel].param[2] = abs(strtol(ArgV(argument, 4), nullptr, 10));      // param3 = abs(int)
          Adc[channel].param[3] = abs(strtol(ArgV(argument, 5), nullptr, 10));      // param4 = abs(int)
        } else {
          Adc[channel].param[2] = (int)(CharToFloat(ArgV(argument, 4)) * 10000);    // param3 = float
          if (ArgC() > 4) {
            Adc[channel].param[3] = (int)(CharToFloat(ArgV(argument, 5)) * 10000);  // param4 = float
          } else {
            Adc[channel].param[3] = 0;                                              // param4 = fixed 0
          }
        }
        if (GPIO_ADC_PH == adc_type) {
          float phLow = CharToFloat(ArgV(argument, 2));
          Adc[channel].param[0] = phLow * ANALOG_PH_DECIMAL_MULTIPLIER;             // param1 = float
//            Adc[channel].param[1] = strtol(ArgV(argument, 3), nullptr, 10);           // param2 = int
          float phHigh = CharToFloat(ArgV(argument, 4));
          Adc[channel].param[2] = phHigh * ANALOG_PH_DECIMAL_MULTIPLIER;            // param3 = float
          Adc[channel].param[3] = strtol(ArgV(argument, 5), nullptr, 10);           // param4 = int

//            AddLog(LOG_LEVEL_INFO, PSTR("ADC: Analog pH probe calibrated. cal-low(pH=ADC) %2_f = %d, cal-high(pH=ADC) %2_f = %d"), &phLow, Adc[channel].param[1], &phHigh, Adc[channel].param[3]);
        }
        if (GPIO_ADC_CT_POWER == adc_type) {
          if (((1 == Adc[channel].param[0]) & CT_FLAG_ENERGY_RESET) > 0) {          // param1 = int
            for (uint32_t i = 0; i < Adcs.present; i++) {
              Adc[i].energy = 0;
            }
            Adc[channel].param[0] ^= CT_FLAG_ENERGY_RESET;                          // Cancel energy reset flag
          }
        }
        if (GPIO_ADC_MQ == adc_type) {
          float a = CharToFloat(ArgV(argument, 3));                           // param2 = float
          float b = CharToFloat(ArgV(argument, 4));                           // param3 = float
          float ratioMQCleanAir = CharToFloat(ArgV(argument, 5));             // param4 = float
          if ((0 == a) && (0 == b) && (0 == ratioMQCleanAir)) {
            if (2 == Adc[channel].param[0]) {                                       // param1 = int
              a = 574.25;
              b = -2.222;
              ratioMQCleanAir = 9.83;
            }
            else if (4 == Adc[channel].param[0]) {
              a = 1012.7;
              b = -2.786;
              ratioMQCleanAir = 4.4;
            }
            else if (7 == Adc[channel].param[0]) {
              a = 99.042;
              b = -1.518;
              ratioMQCleanAir = 27.5;
            }
            if (131 == Adc[channel].param[0]) {
              a = 23.943;
              b = -1.11;
              ratioMQCleanAir = 15;
            }
          }
          Adc[channel].param[1] = (int)(a * ANALOG_MQ_DECIMAL_MULTIPLIER);                // Exponential regression
          Adc[channel].param[2] = (int)(b * ANALOG_MQ_DECIMAL_MULTIPLIER);                // Exponential regression
          Adc[channel].param[3] = (int)(ratioMQCleanAir * ANALOG_MQ_DECIMAL_MULTIPLIER);  // Exponential regression

//            AddLog(LOG_LEVEL_INFO, PSTR("ADC: MQ reset mq%d, a = %2_f, b = %2_f, ratioMQCleanAir = %2_f"), Adc[channel].param[0], &a, &b, &ratioMQCleanAir);
        }
      } else {                                         // Set default values based on current adc type
        // AdcParam 1
        AdcInitParams(channel);
      }
      AdcSaveSettings(channel);
    }

    // AdcParam / AdcGpio
    AdcGetSettings(channel);
    Response_P(PSTR("{\"%s"), XdrvMailbox.command);      // {"AdcParam or {"AdcGpio
    if (strstr_P(XdrvMailbox.command, PSTR(D_CMND_ADCGPIO))) {
#ifdef ESP8266
      ResponseAppend_P(PSTR("\":["));
#else
      ResponseAppend_P(PSTR("%d\":["), Adc[channel].pin);
#endif      
    } else {
      ResponseAppend_P(PSTR("%d\":[%d,"), channel +1, Adc[channel].pin);
    }
    ResponseAppend_P(PSTR("%d,%d"), Adc[channel].param[0], Adc[channel].param[1]);
    if ((GPIO_ADC_INPUT == adc_type) ||
        (GPIO_ADC_TEMP == adc_type) ||
        (GPIO_ADC_RANGE == adc_type) ||
        (GPIO_ADC_MQ == adc_type)) {
      ResponseAppend_P(PSTR(",%d,%d"), Adc[channel].param[2], Adc[channel].param[3]);    // param3 = int, param4 = int
    }
    else {
      float param = (float)Adc[channel].param[2] / 10000;
      ResponseAppend_P(PSTR(",%*_f"), Decimals(Adc[channel].param[2]), &param);    // param3 = float
      if ((GPIO_ADC_CT_POWER == adc_type) ||
          (GPIO_ADC_VOLTAGE == adc_type) ||
          (GPIO_ADC_CURRENT == adc_type)) {
        param = (float)Adc[channel].param[3] / 10000;
        ResponseAppend_P(PSTR(",%*_f"), Decimals(Adc[channel].param[3]), &param);  // param4 = float
      } else {
        ResponseAppend_P(PSTR(",%d"), Adc[channel].param[3]);                      // param4 = int
      }      
    }
    ResponseAppend_P(PSTR("]}"));
  }
}

/*********************************************************************************************\
 * Energy Interface
\*********************************************************************************************/

#if defined(ESP32) && defined(USE_ENERGY_SENSOR)
void AdcEnergyEverySecond(void) {
  uint32_t voltage_count = 0;
  uint32_t current_count = 0;
  for (uint32_t channel = 0; channel < Adcs.present; channel++) {
    uint32_t type_index = Adc[channel].index;
    uint32_t adc_type = Adc[channel].type;
    if (GPIO_ADC_VOLTAGE == adc_type) {
      Energy->voltage_available = true;
      float value = AdcGetRange(channel) / 10000;                   // Volt
      Energy->voltage[type_index] = (value < 0.0f) ? 0.0f : value;  // Disregard negative values
      voltage_count++;
    }
    else if (GPIO_ADC_CURRENT == adc_type) {
      Energy->current_available = true;
      float value = AdcGetRange(channel) / 10000;                   // Ampere
      Energy->current[type_index] = (value < 0.0f) ? 0.0f : value;  // Disregard negative values
      current_count++;
    }
  }
  for (uint32_t phase = 0; phase < Energy->phase_count; phase++) {
    uint32_t voltage_phase = (voltage_count == current_count) ? phase : 0;
    Energy->active_power[phase] = Energy->voltage[voltage_phase] * Energy->current[phase];  // Watt
    Energy->kWhtoday_delta[phase] += (uint32_t)(Energy->active_power[phase] * 1000) / 36;   // deca_microWh
    Energy->data_valid[phase] = 0;
  }

//  float delta = (float)Energy->kWhtoday_delta[0] / 100;
//  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("ADC: %3_fV, %3_fA, %3_fW, %2_fmWh"), &Energy->voltage[0], &Energy->current[0], &Energy->active_power[0], &delta);

  EnergyUpdateToday();
}

bool Xnrg33(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_ENERGY_EVERY_SECOND:
      AdcEnergyEverySecond();
      break;
    case FUNC_PRE_INIT: {
        uint32_t voltage_count = 0;
        uint32_t current_count = 0;
        for (uint32_t channel = 0; channel < Adcs.present; channel++) {
          uint32_t adc_type = Adc[channel].type;
          if (GPIO_ADC_VOLTAGE == adc_type) { voltage_count++; }
          if (GPIO_ADC_CURRENT == adc_type) { current_count++; }
        }
        if (voltage_count && current_count) {
          Energy->type_dc = true;
          Energy->voltage_common = (1 == voltage_count);
          Energy->phase_count = (voltage_count > current_count) ? voltage_count : current_count;
          Energy->voltage_available = false;
          Energy->current_available = false;
          Energy->use_overtemp = true;   // Use global temperature for overtemp detection
          TasmotaGlobal.energy_driver = XNRG_33;
        }
      }
      break;
  }
  return result;
}
#endif  // ESP32 and USE_ENERGY_SENSOR

/*********************************************************************************************\
 * Sensor Interface
\*********************************************************************************************/

bool Xsns02(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kAdcCommands, AdcCommand);
      break;
    case FUNC_SETUP_RING2:
      AdcInit();
      break;
    default:
      if (Adcs.present) {
        switch (function) {
          case FUNC_EVERY_250_MSECOND:
            AdcEvery250ms();
            break;
          case FUNC_EVERY_SECOND:
            AdcEverySecond();
            break;
          case FUNC_JSON_APPEND:
            AdcShow(1);
            break;
#ifdef USE_WEBSERVER
          case FUNC_WEB_SENSOR:
            AdcShow(0);
            break;
#endif  // USE_WEBSERVER
        }
      }
  }
  return result;
}

#endif  // USE_ADC
#endif  // FIRMWARE_MINIMAL