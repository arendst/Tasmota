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

#ifdef USE_ADC
/*********************************************************************************************\
 * ADC support for ESP8266 GPIO17 (=PIN_A0) and ESP32 up to 8 channels on GPIO32 to GPIO39
\*********************************************************************************************/

#define XSNS_02                       2

#ifdef ESP32
  #if ESP_IDF_VERSION_MAJOR >= 5
    #include "esp32-hal-adc.h"
  #endif
#endif

#ifdef ESP8266
#define ANALOG_RESOLUTION             10               // 12 = 4095, 11 = 2047, 10 = 1023
#define ANALOG_RANGE                  1023             // 4095 = 12, 2047 = 11, 1023 = 10
#define ANALOG_PERCENT                10               // backward compatible div10 range
#endif  // ESP8266
#ifdef ESP32
#undef ANALOG_RESOLUTION
#define ANALOG_RESOLUTION             12               // 12 = 4095, 11 = 2047, 10 = 1023
#undef ANALOG_RANGE
#define ANALOG_RANGE                  4095             // 4095 = 12, 2047 = 11, 1023 = 10
#undef ANALOG_PERCENT
#define ANALOG_PERCENT                ((ANALOG_RANGE + 50) / 100)  // approximation to 1% ADC range
#endif  // ESP32

#define TO_CELSIUS(x) ((x) - 273.15)
#define TO_KELVIN(x) ((x) + 273.15)

// Parameters for equation
#define ANALOG_V33                    3.3              // ESP8266 / ESP32 Analog voltage
#define ANALOG_T0                     TO_KELVIN(25.0)  // 25 degrees Celsius in Kelvin (= 298.15)

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

// LDR parameters
// 3V3 --- LDR ---v--- ANALOG_LDR_BRIDGE_RESISTANCE --- Gnd
//                |
//               ADC0
#define ANALOG_LDR_BRIDGE_RESISTANCE  10000            // LDR Voltage bridge resistor
#define ANALOG_LDR_LUX_CALC_SCALAR    12518931         // Experimental
#define ANALOG_LDR_LUX_CALC_EXPONENT  -1.4050          // Experimental

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
#define ANALOG_BUTTON                 128              // Add resistor tolerance

// Odroid joysticks
//        ---- Up
// 3V3 ---|  |------------
//                        |
//        ---- Dn         |--- R10k --- Gnd
// 3V3 ---|  |--- R10k ---|
//                        |
//                       ADC
// Press "Up" will raise ADC to ANALOG_RANGE, Press "Dn" will raise ADC to ANALOG_RANGE/2
#define ANALOG_JOYSTICK              (ANALOG_RANGE / 3) +100  // Add resistor tolerance

// pH scale minimum and maximum values
#define ANALOG_PH_MAX                             14.0
#define ANALOG_PH_MIN                             0.0

// Default values for calibration solution with lower PH
#define ANALOG_PH_CALSOLUTION_LOW_PH              4.0
#define ANALOG_PH_CALSOLUTION_LOW_ANALOG_VALUE    282
// Default values for calibration solution with higher PH
#define ANALOG_PH_CALSOLUTION_HIGH_PH             9.18
#define ANALOG_PH_CALSOLUTION_HIGH_ANALOG_VALUE   435

// Multiplier used to store pH with 2 decimal places in a non decimal datatype
#define ANALOG_PH_DECIMAL_MULTIPLIER              100.0

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
#define ANALOG_MQ_A                               574.25
//exponential regression b params
#define ANALOG_MQ_B                               -2.222
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
#define ANALOG_MQ_RatioMQCleanAir                 15.0
// Multiplier used to store pH with 2 decimal places in a non decimal datatype
#define ANALOG_MQ_DECIMAL_MULTIPLIER              100.0
// lenght of filter
#define ANALOG_MQ_SAMPLES                         60

struct {
  uint8_t present = 0;
  uint8_t type = 0;
} Adcs;

struct {
  float temperature = 0;
  float current = 0;
  float energy = 0;
  uint32_t param1 = 0;
  uint32_t param2 = 0;
  int param3 = 0;
  int param4 = 0;
  uint32_t previous_millis = 0;
  uint16_t last_value = 0;
  uint8_t type = 0;
  uint8_t pin = 0;
  float mq_samples[ANALOG_MQ_SAMPLES];
  int indexOfPointer = -1;
} Adc[MAX_ADCS];

#ifdef ESP8266
bool adcAttachPin(uint8_t pin) {
  return (ADC0_PIN == pin);
}
#endif
#if defined(ESP32) && (ESP_IDF_VERSION_MAJOR >= 5)
  bool adcAttachPin(uint8_t pin) {
    return true;                        // TODO - no more needed?
  }
#endif

void AdcSaveSettings(uint32_t idx) {
  char parameters[32];
  snprintf_P(parameters, sizeof(parameters), PSTR("%d,%d,%d,%d,%d"),
    Adc[idx].type, Adc[idx].param1, Adc[idx].param2, Adc[idx].param3, Adc[idx].param4);
  SettingsUpdateText(SET_ADC_PARAM1 + idx, parameters);
}

void AdcGetSettings(uint32_t idx) {
  char parameters[32];
  Adcs.type = 0;
  Adc[idx].param1 = 0;
  Adc[idx].param2 = 0;
  Adc[idx].param3 = 0;
  Adc[idx].param4 = 0;
  if (strchr(SettingsText(SET_ADC_PARAM1 + idx), ',') != nullptr) {
    Adcs.type = atoi(subStr(parameters, SettingsText(SET_ADC_PARAM1 + idx), ",", 1));
    Adc[idx].param1 = atoi(subStr(parameters, SettingsText(SET_ADC_PARAM1 + idx), ",", 2));
    Adc[idx].param2 = atoi(subStr(parameters, SettingsText(SET_ADC_PARAM1 + idx), ",", 3));
    Adc[idx].param3 = atoi(subStr(parameters, SettingsText(SET_ADC_PARAM1 + idx), ",", 4));
    Adc[idx].param4 = atoi(subStr(parameters, SettingsText(SET_ADC_PARAM1 + idx), ",", 5));
  }
}

void AdcInitParams(uint8_t idx) {
  if ((Adcs.type != Adc[idx].type) || (Adc[idx].param1 > 1000000)) {
    if (ADC_TEMP == Adc[idx].type) {
      // Default Shelly 2.5 and 1PM parameters
      Adc[idx].param1 = ANALOG_NTC_BRIDGE_RESISTANCE;
      Adc[idx].param2 = ANALOG_NTC_RESISTANCE;
      Adc[idx].param3 = ANALOG_NTC_B_COEFFICIENT * 10000;
      Adc[idx].param4 = 0; // Default to Shelly mode with NTC towards GND
    }
    else if (ADC_LIGHT == Adc[idx].type) {
      Adc[idx].param1 = ANALOG_LDR_BRIDGE_RESISTANCE;
      Adc[idx].param2 = ANALOG_LDR_LUX_CALC_SCALAR;
      Adc[idx].param3 = ANALOG_LDR_LUX_CALC_EXPONENT * 10000;
    }
    else if (ADC_RANGE == Adc[idx].type) {
      Adc[idx].param1 = 0;
      Adc[idx].param2 = ANALOG_RANGE;
      Adc[idx].param3 = 0;
      Adc[idx].param4 = 100;
    }
    else if (ADC_CT_POWER == Adc[idx].type) {
      Adc[idx].param1 = ANALOG_CT_FLAGS;         // (uint32_t) 0
      Adc[idx].param2 = ANALOG_CT_MULTIPLIER;    // (uint32_t) 100000
      Adc[idx].param3 = ANALOG_CT_VOLTAGE;       // (int)      10
    }
    else if (ADC_PH == Adc[idx].type) {
      Adc[idx].param1 = ANALOG_PH_CALSOLUTION_LOW_PH * ANALOG_PH_DECIMAL_MULTIPLIER;  // PH of the calibration solution 1, which is the one with the lower PH
      Adc[idx].param2 = ANALOG_PH_CALSOLUTION_LOW_ANALOG_VALUE;                       // Reading of AnalogInput while probe is in solution 1
      Adc[idx].param3 = ANALOG_PH_CALSOLUTION_HIGH_PH * ANALOG_PH_DECIMAL_MULTIPLIER; // PH of the calibration solution 2, which is the one with the higher PH
      Adc[idx].param4 = ANALOG_PH_CALSOLUTION_HIGH_ANALOG_VALUE;                      // Reading of AnalogInput while probe is in solution 2
    }
    else if (ADC_MQ == Adc[idx].type) {
      Adc[idx].param1 = ANALOG_MQ_TYPE;  // Could be MQ-002, MQ-004, MQ-131 ....
      Adc[idx].param2 = (int)(ANALOG_MQ_A * ANALOG_MQ_DECIMAL_MULTIPLIER);                       // Exponential regression
      Adc[idx].param3 = (int)(ANALOG_MQ_B * ANALOG_MQ_DECIMAL_MULTIPLIER);                      // Exponential regression
      Adc[idx].param4 = (int)(ANALOG_MQ_RatioMQCleanAir * ANALOG_MQ_DECIMAL_MULTIPLIER);                      // Exponential regression
    }
  }
  if ((Adcs.type != Adc[idx].type) || (0 == Adc[idx].param1) || (Adc[idx].param1 > ANALOG_RANGE)) {
    if ((ADC_BUTTON == Adc[idx].type) || (ADC_BUTTON_INV == Adc[idx].type)) {
      Adc[idx].param1 = ANALOG_BUTTON;
    }
    else if (ADC_JOY == Adc[idx].type) {
      Adc[idx].param1 = ANALOG_JOYSTICK;
    }
  }
}

void AdcAttach(uint32_t pin, uint8_t type) {
  if (Adcs.present == MAX_ADCS) { return; }
  Adc[Adcs.present].pin = pin;
  if (adcAttachPin(Adc[Adcs.present].pin)) {
    Adc[Adcs.present].type = type;
//    analogSetPinAttenuation(Adc[Adcs.present].pin, ADC_11db);  // Default
    Adcs.present++;
  }
}

void AdcInit(void) {
  Adcs.present = 0;
  for (uint32_t i = 0; i < MAX_ADCS; i++) {
    if (PinUsed(GPIO_ADC_INPUT, i)) {
      AdcAttach(Pin(GPIO_ADC_INPUT, i), ADC_INPUT);
    }
    if (PinUsed(GPIO_ADC_TEMP, i)) {
      AdcAttach(Pin(GPIO_ADC_TEMP, i), ADC_TEMP);
    }
    if (PinUsed(GPIO_ADC_LIGHT, i)) {
      AdcAttach(Pin(GPIO_ADC_LIGHT, i), ADC_LIGHT);
    }
    if (PinUsed(GPIO_ADC_RANGE, i)) {
      AdcAttach(Pin(GPIO_ADC_RANGE, i), ADC_RANGE);
    }
    if (PinUsed(GPIO_ADC_CT_POWER, i)) {
      AdcAttach(Pin(GPIO_ADC_CT_POWER, i), ADC_CT_POWER);
    }
    if (PinUsed(GPIO_ADC_JOY, i)) {
      AdcAttach(Pin(GPIO_ADC_JOY, i), ADC_JOY);
    }
    if (PinUsed(GPIO_ADC_PH, i)) {
      AdcAttach(Pin(GPIO_ADC_PH, i), ADC_PH);
    }
    if (PinUsed(GPIO_ADC_MQ, i)) {
      AdcAttach(Pin(GPIO_ADC_MQ, i), ADC_MQ);
    }
  }
  for (uint32_t i = 0; i < MAX_KEYS; i++) {
    if (PinUsed(GPIO_ADC_BUTTON, i)) {
      AdcAttach(Pin(GPIO_ADC_BUTTON, i), ADC_BUTTON);
    }
    else if (PinUsed(GPIO_ADC_BUTTON_INV, i)) {
      AdcAttach(Pin(GPIO_ADC_BUTTON_INV, i), ADC_BUTTON_INV);
    }
  }

  if (Adcs.present) {
#ifdef ESP32
#if ESP_IDF_VERSION_MAJOR < 5
    analogSetClockDiv(1);               // Default 1
#endif
#if CONFIG_IDF_TARGET_ESP32
    analogSetWidth(ANALOG_RESOLUTION);  // Default 12 bits (0 - 4095)
#endif  // CONFIG_IDF_TARGET_ESP32
    analogSetAttenuation(ADC_11db);     // Default 11db
#endif
    for (uint32_t idx = 0; idx < Adcs.present; idx++) {
      AdcGetSettings(idx);
      AdcInitParams(idx);
      AdcSaveSettings(idx);
    }
  }
}

uint16_t AdcRead(uint32_t pin, uint32_t factor) {
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
  analog = analog/(ANALOG_V33*1000) * ANALOG_RANGE; // go back from mV to ADC
#endif
  return analog;
}

#ifdef USE_RULES
void AdcEvery250ms(void) {
  char adc_idx[3] = { 0 };
  uint32_t offset = 0;
  for (uint32_t idx = 0; idx < Adcs.present; idx++) {
#ifdef ESP32
    snprintf_P(adc_idx, sizeof(adc_idx), PSTR("%d"), idx +1);
    offset = 1;
#endif
    if (ADC_INPUT == Adc[idx].type) {
      uint16_t new_value = AdcRead(Adc[idx].pin, 5);
      if ((new_value < Adc[idx].last_value -ANALOG_PERCENT) || (new_value > Adc[idx].last_value +ANALOG_PERCENT)) {
        Adc[idx].last_value = new_value;
        uint16_t value = Adc[idx].last_value / ANALOG_PERCENT;
        Response_P(PSTR("{\"ANALOG\":{\"A%ddiv10\":%d}}"), idx + offset, (value > 99) ? 100 : value);
        XdrvRulesProcess(0);
      }
    }
    else if (ADC_JOY == Adc[idx].type) {
      uint16_t new_value = AdcRead(Adc[idx].pin, 1);
      if (new_value && (new_value != Adc[idx].last_value)) {
        Adc[idx].last_value = new_value;
        uint16_t value = new_value / Adc[idx].param1;
        Response_P(PSTR("{\"ANALOG\":{\"Joy%s\":%d}}"), adc_idx, value);
        XdrvRulesProcess(0);
      } else {
        Adc[idx].last_value = 0;
      }
    }
  }
}
#endif  // USE_RULES

uint8_t AdcGetButton(uint32_t pin) {
  for (uint32_t idx = 0; idx < Adcs.present; idx++) {
    if (Adc[idx].pin == pin) {
      if (ADC_BUTTON_INV == Adc[idx].type) {
        return (AdcRead(Adc[idx].pin, 1) < Adc[idx].param1);
      }
      else if (ADC_BUTTON == Adc[idx].type) {
        return (AdcRead(Adc[idx].pin, 1) > Adc[idx].param1);
      }
    }
  }
  return 0;
}

uint16_t AdcGetLux(uint32_t idx) {
  int adc = AdcRead(Adc[idx].pin, 2);
  // Source: https://www.allaboutcircuits.com/projects/design-a-luxmeter-using-a-light-dependent-resistor/
  double resistorVoltage = ((double)adc / ANALOG_RANGE) * ANALOG_V33;
  double ldrVoltage = ANALOG_V33 - resistorVoltage;
  double ldrResistance = ldrVoltage / resistorVoltage * (double)Adc[idx].param1;
  double ldrLux = (double)Adc[idx].param2 * FastPrecisePow(ldrResistance, (double)Adc[idx].param3 / 10000);

  return (uint16_t)ldrLux;
}

void AddSampleMq(uint32_t idx){
//  AddLog(LOG_LEVEL_DEBUG, PSTR("ADC: Adding sample for mq-sensor"));

  int _adc = AdcRead(Adc[idx].pin, 2);
  // init af array at same value
  if (Adc[idx].indexOfPointer==-1) {

//    AddLog(LOG_LEVEL_DEBUG, PSTR("ADC: Init samples for mq-sensor"));

    for (int i = 0; i < ANALOG_MQ_SAMPLES; i ++) {
      Adc[idx].mq_samples[i] = _adc;
    }
  } else {
    Adc[idx].mq_samples[Adc[idx].indexOfPointer] = _adc;
  }
  Adc[idx].indexOfPointer++;
  if (Adc[idx].indexOfPointer==ANALOG_MQ_SAMPLES) {
    Adc[idx].indexOfPointer=0;
  }
}

float AdcGetMq(uint32_t idx) {
//  AddLog(LOG_LEVEL_DEBUG, PSTR("ADC: Getting value for mq-sensor"));

  float avg = 0.0;
  for (int i = 0; i < ANALOG_MQ_SAMPLES; i ++) {
    avg += Adc[idx].mq_samples[i];
  }
  float voltage = (avg / ANALOG_MQ_SAMPLES) * ANALOG_V33 / ANALOG_RANGE;

  float _RL = 10;                                        // Value in KiloOhms
  float _RS_Calc = ((ANALOG_V33 * _RL) / voltage) -_RL;  // Get value of RS in a gas
  if (_RS_Calc < 0) {
    _RS_Calc = 0;                                        // No negative values accepted.
  }

  float _R0 = 10;
  float _ratio = _RS_Calc / _R0;                         // Get ratio RS_gas/RS_air
  float ppm = Adc[idx].param2 / ANALOG_MQ_DECIMAL_MULTIPLIER * FastPrecisePow(_ratio, Adc[idx].param3 / ANALOG_MQ_DECIMAL_MULTIPLIER);  // Source excel analisis https://github.com/miguel5612/MQSensorsLib_Docs/tree/master/Internal_design_documents
  if (ppm < 0) { ppm = 0; }                              // No negative values accepted or upper datasheet recomendation.
  if (ppm > 100000) { ppm = 100000; }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("ADC: Ppm read. ADC-RAW: %2_f, ppm: %2_f"), &voltage, &ppm);

  return ppm;
}

float AdcGetPh(uint32_t idx) {
  int adc = AdcRead(Adc[idx].pin, 2);

  float y1 = (float)Adc[idx].param1 / ANALOG_PH_DECIMAL_MULTIPLIER;
  int32_t x1 = Adc[idx].param2;
  float y2 = (float)Adc[idx].param3 / ANALOG_PH_DECIMAL_MULTIPLIER;
  int32_t x2 = Adc[idx].param4;

  float m = (y2 - y1) / (float)(x2 - x1);
  float ph = m * (float)(adc - x1) + y1;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("ADC: Analog pH read. ADC-RAW: %d, cal-low(pH=ADC): %2_f = %d, cal-high(pH=ADC): %2_f = %d"), adc, &y1, x1, &y2, x2);

  return ph;
}

float AdcGetRange(uint32_t idx) {
  // formula for calibration: value, fromLow, fromHigh, toLow, toHigh
  // Example: 514, 632, 236, 0, 100
  // int( ((<param2> - <analog-value>) / (<param2> - <param1>) ) * (<param3> - <param4>) ) + <param4> )
  int adc = AdcRead(Adc[idx].pin, 5);
  double adcrange = ( ((double)Adc[idx].param2 - (double)adc) / ( ((double)Adc[idx].param2 - (double)Adc[idx].param1)) * ((double)Adc[idx].param3 - (double)Adc[idx].param4) + (double)Adc[idx].param4 );
  return (float)adcrange;
}

void AdcGetCurrentPower(uint8_t idx, uint8_t factor) {
  // factor 1 = 2 samples
  // factor 2 = 4 samples
  // factor 3 = 8 samples
  // factor 4 = 16 samples
  // factor 5 = 32 samples
  uint8_t samples = 1 << factor;
  uint16_t analog = 0;
  uint16_t analog_min = ANALOG_RANGE;
  uint16_t analog_max = 0;

  if (0 == Adc[idx].param1) {
    unsigned long tstart=millis();
    while (millis()-tstart < 35) {
      analog = analogRead(Adc[idx].pin);
      if (analog < analog_min) {
        analog_min = analog;
      }
      if (analog > analog_max) {
        analog_max = analog;
      }
    }
    //AddLog(0, PSTR("min: %u, max:%u, dif:%u"), analog_min, analog_max, analog_max-analog_min);
    Adc[idx].current = (float)(analog_max-analog_min) * ((float)(Adc[idx].param2) / 100000);
    if (Adc[idx].current < (((float)Adc[idx].param4) / 10000.0))
        Adc[idx].current = 0.0;
  }
  else {
    analog = AdcRead(Adc[idx].pin, 5);
    if (analog > Adc[idx].param1) {
     Adc[idx].current = ((float)(analog) - (float)Adc[idx].param1) * ((float)(Adc[idx].param2) / 100000);
    }
    else {
      Adc[idx].current = 0;
    }
  }

  float power = Adc[idx].current * (float)(Adc[idx].param3) / 10;
  uint32_t current_millis = millis();
  Adc[idx].energy = Adc[idx].energy + ((power * (current_millis - Adc[idx].previous_millis)) / 3600000000);
  Adc[idx].previous_millis = current_millis;
}

void AdcEverySecond(void) {
  for (uint32_t idx = 0; idx < Adcs.present; idx++) {
    if (ADC_TEMP == Adc[idx].type) {
      int adc = AdcRead(Adc[idx].pin, 2);
      // Steinhart-Hart equation for thermistor as temperature sensor:
      // double Rt = (adc * Adc[idx].param1 * MAX_ADC_V) / (ANALOG_RANGE * ANALOG_V33 - (double)adc * MAX_ADC_V);
      // MAX_ADC_V in ESP8266 is 1
      // MAX_ADC_V in ESP32 is 3.3
      double Rt;
#ifdef ESP8266
      if (Adc[idx].param4) { // Alternate mode
        Rt = (double)Adc[idx].param1 * (ANALOG_RANGE * ANALOG_V33 - (double)adc) / (double)adc;
      } else {
        Rt = (double)Adc[idx].param1 * (double)adc / (ANALOG_RANGE * ANALOG_V33 - (double)adc);
      }
#else
      if (Adc[idx].param4) { // Alternate mode
        Rt = (double)Adc[idx].param1 * (ANALOG_RANGE - (double)adc) / (double)adc;
      } else {
        Rt = (double)Adc[idx].param1 * (double)adc / (ANALOG_RANGE - (double)adc);
      }
#endif
      double BC = (double)Adc[idx].param3 / 10000;                                      // Shelly param3 = 3350 (ANALOG_NTC_B_COEFFICIENT)
      double T = BC / (BC / ANALOG_T0 + TaylorLog(Rt / (double)Adc[idx].param2));       // Shelly param2 = 10000 (ANALOG_NTC_RESISTANCE)
      Adc[idx].temperature = ConvertTemp(TO_CELSIUS(T));
    }
    else if (ADC_CT_POWER == Adc[idx].type) {
      AdcGetCurrentPower(idx, 5);
    }
    else if (ADC_MQ == Adc[idx].type) {
      AddSampleMq(idx);
      AdcGetMq(idx);
    }
  }
}

void AdcShowContinuation(bool *jsonflg) {
  if (*jsonflg) {
    ResponseAppend_P(PSTR(","));
  } else {
    ResponseAppend_P(PSTR(",\"ANALOG\":{"));
    *jsonflg = true;
  }
}

void AdcShow(bool json) {
  bool domo_flag[ADC_END] = { false };
  char adc_name[10] = { 0 };  // ANALOG8
  char adc_idx[3] = { 0 };
  uint32_t offset = 0;

  bool jsonflg = false;
  for (uint32_t idx = 0; idx < Adcs.present; idx++) {
#ifdef ESP32
    snprintf_P(adc_name, sizeof(adc_name), PSTR("Analog%d"), idx +1);
    snprintf_P(adc_idx, sizeof(adc_idx), PSTR("%d"), idx +1);
    offset = 1;
#endif

    switch (Adc[idx].type) {
      case ADC_INPUT: {
        uint16_t analog = AdcRead(Adc[idx].pin, 5);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"A%d\":%d"), idx + offset, analog);
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_ANALOG, "", idx + offset, analog);
#endif  // USE_WEBSERVER
        }
        break;
      }
      case ADC_TEMP: {
        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "%s\":%*_f"), adc_idx, Settings->flag2.temperature_resolution, &Adc[idx].temperature);
          if ((0 == TasmotaGlobal.tele_period) && (!domo_flag[ADC_TEMP])) {
#ifdef USE_DOMOTICZ
            DomoticzFloatSensor(DZ_TEMP, Adc[idx].temperature);
            domo_flag[ADC_TEMP] = true;
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
            KnxSensor(KNX_TEMPERATURE, Adc[idx].temperature);
#endif  // USE_KNX
          }
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_Temp(adc_name, Adc[idx].temperature);
#endif  // USE_WEBSERVER
        }
        break;
      }
      case ADC_LIGHT: {
        uint16_t adc_light = AdcGetLux(idx);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"" D_JSON_ILLUMINANCE "%s\":%d"), adc_idx, adc_light);
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
      case ADC_RANGE: {
        float adc_range = AdcGetRange(idx);
        char range_chr[FLOATSZ];
        dtostrfd(adc_range, Settings->flag2.frequency_resolution, range_chr);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"" D_JSON_RANGE "%s\":%s"), adc_idx, range_chr);
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_RANGE_CHR, adc_name, range_chr);
#endif  // USE_WEBSERVER
        }
        break;
      }
      case ADC_CT_POWER: {
        AdcGetCurrentPower(idx, 5);

        float voltage = (float)(Adc[idx].param3) / 10;
        char voltage_chr[FLOATSZ];
        dtostrfd(voltage, Settings->flag2.voltage_resolution, voltage_chr);
        char current_chr[FLOATSZ];
        dtostrfd(Adc[idx].current, Settings->flag2.current_resolution, current_chr);
        char power_chr[FLOATSZ];
        dtostrfd(voltage * Adc[idx].current, Settings->flag2.wattage_resolution, power_chr);
        char energy_chr[FLOATSZ];
        dtostrfd(Adc[idx].energy, Settings->flag2.energy_resolution, energy_chr);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"CTEnergy%s\":{\"" D_JSON_ENERGY "\":%s,\"" D_JSON_POWERUSAGE "\":%s,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s}"),
            adc_idx, energy_chr, power_chr, voltage_chr, current_chr);
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
      case ADC_JOY: {
        uint16_t new_value = AdcRead(Adc[idx].pin, 1);
        uint16_t value = new_value / Adc[idx].param1;
        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"Joy%s\":%d"), adc_idx, value);
        }
        break;
      }
      case ADC_PH: {
        float ph = AdcGetPh(idx);
        char ph_chr[FLOATSZ];
        dtostrfd(ph, 2, ph_chr);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"pH%d\":%s"), idx + offset, ph_chr);
  #ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_PH, "", ph_chr);
  #endif // USE_WEBSERVER
        }
        break;
      }
      case ADC_MQ: {
        float mq = AdcGetMq(idx);
        char mq_chr[FLOATSZ];
        dtostrfd(mq, 2, mq_chr);

        float mqnumber =Adc[idx].param1;
        char mqnumber_chr[FLOATSZ];
        dtostrfd(mqnumber, 0, mqnumber_chr);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"MQ%d_%d\":%s"), Adc[idx].param1, idx + offset, mq_chr);
  #ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_MQ, mqnumber_chr, mq_chr);
  #endif // USE_WEBSERVER
        }
        break;
      }
    }
  }
  if (jsonflg) {
    ResponseJsonEnd();
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kAdcCommands[] PROGMEM = "|"  // No prefix
  D_CMND_ADCPARAM;

void (* const AdcCommand[])(void) PROGMEM = {
  &CmndAdcParam };

void CmndAdcParam(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_ADCS)) {
    uint8_t idx = XdrvMailbox.index -1;
    if (XdrvMailbox.data_len) {
      if (XdrvMailbox.payload > ADC_INPUT) {
        AdcGetSettings(idx);
        if (ArgC() > 3) {  // Process parameter entry
          char argument[XdrvMailbox.data_len];
          // AdcParam 2, 32000, 10000, 3350        ADC_TEMP Shelly mode
          // AdcParam 2, 32000, 10000, 3350, 1     ADC_TEMP Alternate mode
          // AdcParam 3, 10000, 12518931, -1.405
          // AdcParam 4, 128, 0, 0
          // AdcParam 5, 128, 0, 0
          // AdcParam 6, 0, ANALOG_RANGE, 0, 100
          // AdcParam 7, 0, 2146, 0.23
          // AdcParam 8, 1000, 0, 0
          Adc[idx].type = XdrvMailbox.payload;
          Adc[idx].param1 = strtol(ArgV(argument, 2), nullptr, 10);
          Adc[idx].param2 = strtol(ArgV(argument, 3), nullptr, 10);
          if (ADC_RANGE == XdrvMailbox.payload) {
            Adc[idx].param3 = abs(strtol(ArgV(argument, 4), nullptr, 10));
            Adc[idx].param4 = abs(strtol(ArgV(argument, 5), nullptr, 10));
          } else {
            Adc[idx].param3 = (int)(CharToFloat(ArgV(argument, 4)) * 10000);
            if (ArgC() > 4) {
              Adc[idx].param4 = (int)(CharToFloat(ArgV(argument, 5)) * 10000);
            }
            else{
              Adc[idx].param4 = 0;
            }
          }
          if (ADC_PH == XdrvMailbox.payload) {
            float phLow = CharToFloat(ArgV(argument, 2));
            float phHigh = CharToFloat(ArgV(argument, 4));
            Adc[idx].param1 = phLow * ANALOG_PH_DECIMAL_MULTIPLIER;
            Adc[idx].param2 = strtol(ArgV(argument, 3), nullptr, 10);
            Adc[idx].param3 = phHigh * ANALOG_PH_DECIMAL_MULTIPLIER;
            Adc[idx].param4 = strtol(ArgV(argument, 5), nullptr, 10);

//            AddLog(LOG_LEVEL_INFO, PSTR("ADC: Analog pH probe calibrated. cal-low(pH=ADC) %2_f = %d, cal-high(pH=ADC) %2_f = %d"), &phLow, Adc[idx].param2, &phHigh, Adc[idx].param4);
          }
          if (ADC_CT_POWER == XdrvMailbox.payload) {
            if (((1 == Adc[idx].param1) & CT_FLAG_ENERGY_RESET) > 0) {
              for (uint32_t idx = 0; idx < MAX_ADCS; idx++) {
                Adc[idx].energy = 0;
              }
              Adc[idx].param1 ^= CT_FLAG_ENERGY_RESET;  // Cancel energy reset flag
            }
          }
          if (ADC_MQ == XdrvMailbox.payload) {
            float a = CharToFloat(ArgV(argument, 3));
            float b = CharToFloat(ArgV(argument, 4));
            float ratioMQCleanAir = CharToFloat(ArgV(argument, 5));
            if (a==0 && b==0 && ratioMQCleanAir==0)
            {
              if (Adc[idx].param1==2)
              {
                a=574.25;
                b=-2.222;
                ratioMQCleanAir=9.83;
              }
              if (Adc[idx].param1==4)
              {
                a=1012.7;
                b=-2.786;
                ratioMQCleanAir=4.4;
              }
              if (Adc[idx].param1==7)
              {
                a=99.042;
                b=-1.518;
                ratioMQCleanAir=27.5;
              }
              if (Adc[idx].param1==131)
              {
                a=23.943;
                b=-1.11;
                ratioMQCleanAir=15;
              }
            }
            Adc[idx].param2 = (int)(a * ANALOG_MQ_DECIMAL_MULTIPLIER);                 // Exponential regression
            Adc[idx].param3 = (int)(b * ANALOG_MQ_DECIMAL_MULTIPLIER);                 // Exponential regression
            Adc[idx].param4 = (int)(ratioMQCleanAir * ANALOG_MQ_DECIMAL_MULTIPLIER);   // Exponential regression

//            AddLog(LOG_LEVEL_INFO, PSTR("ADC: MQ reset mq%d, a = %2_f, b = %2_f, ratioMQCleanAir = %2_f"), Adc[idx].param1, &a, &b, &ratioMQCleanAir);
          }
        } else {                                         // Set default values based on current adc type
          // AdcParam 2
          // AdcParam 3
          // AdcParam 4
          // AdcParam 5
          // AdcParam 6
          // AdcParam 7
          // AdcParam 8
          Adcs.type = 0;
          AdcInitParams(idx);
        }
        AdcSaveSettings(idx);
      }
    }

    // AdcParam
    AdcGetSettings(idx);
    Response_P(PSTR("{\"" D_CMND_ADCPARAM "%d\":[%d,%d,%d"), idx +1, Adcs.type, Adc[idx].param1, Adc[idx].param2);
    if ((ADC_RANGE == Adc[idx].type) || (ADC_MQ == Adc[idx].type)){
      ResponseAppend_P(PSTR(",%d,%d"), Adc[idx].param3, Adc[idx].param4);
    } else {
      int value = Adc[idx].param3;
      uint8_t precision;
      for (precision = 4; precision > 0; precision--) {
        if (value % 10) { break; }
        value /= 10;
      }
      char param3[FLOATSZ];
      dtostrfd(((double)Adc[idx].param3)/10000, precision, param3);
      if (ADC_CT_POWER == Adc[idx].type) {
        char param4[FLOATSZ];
        dtostrfd(((double)Adc[idx].param4)/10000, 3, param4);
        ResponseAppend_P(PSTR(",%s,%s"), param3, param4);
      } else {
        ResponseAppend_P(PSTR(",%s,%d"), param3, Adc[idx].param4);
      }
    }
    ResponseAppend_P(PSTR("]}"));
  }
}

/*********************************************************************************************\
 * Interface
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
#ifdef USE_RULES
          case FUNC_EVERY_250_MSECOND:
            AdcEvery250ms();
            break;
#endif  // USE_RULES
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
