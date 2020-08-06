/*
  xsns_02_analog_esp32.ino - ESP32 ADC support for Tasmota

  Copyright (C) 2020  Theo Arends

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
#ifdef USE_ADC
/*********************************************************************************************\
 * ADC support for up to 8 channels on GPIO32 to GPIO39
\*********************************************************************************************/

#define XSNS_02                       2

#define ANALOG_RESOLUTION             12               // 12 = 4095, 11 = 2047, 10 = 1023
#define ANALOG_RANGE                  4095             // 4095 = 12, 2047 = 11, 1023 = 10

#define TO_CELSIUS(x) ((x) - 273.15)
#define TO_KELVIN(x) ((x) + 273.15)

// Parameters for equation
#define ANALOG_V33                    3.3              // ESP8266 Analog voltage
#define ANALOG_T0                     TO_KELVIN(25.0)  // 25 degrees Celcius in Kelvin (= 298.15)

// Shelly 2.5 NTC Thermistor
// 3V3 --- ANALOG_NTC_BRIDGE_RESISTANCE ---v--- NTC --- Gnd
//                                         |
//                                        ADC0
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
} Adc[MAX_ADCS];

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
  if (strstr(SettingsText(SET_ADC_PARAM1 + idx), ",") != nullptr) {
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
    else if (ADC_JOY == Adc[idx].type) {
      Adc[idx].param1 = ANALOG_JOYSTICK;
    }
  }
}

void AdcAttach(uint8_t pin, uint8_t type) {
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
    if (PinUsed(GPIO_ADC_BUTTON, i)) {
      AdcAttach(Pin(GPIO_ADC_BUTTON, i), ADC_BUTTON);
    }
    if (PinUsed(GPIO_ADC_BUTTON_INV, i)) {
      AdcAttach(Pin(GPIO_ADC_BUTTON_INV, i), ADC_BUTTON_INV);
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
  }
  if (Adcs.present) {
    analogSetClockDiv(1);               // Default 1
    analogSetWidth(ANALOG_RESOLUTION);  // Default 12 bits (0 - 4095)
    analogSetAttenuation(ADC_11db);     // Default 11db
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
  uint32_t samples = 1 << factor;
  uint32_t analog = 0;
  for (uint32_t i = 0; i < samples; i++) {
    analog += analogRead(pin);
    delay(1);
  }
  analog >>= factor;
  return analog;
}

#ifdef USE_RULES
void AdcEvery250ms(void) {
  for (uint32_t idx = 0; idx < Adcs.present; idx++) {
    if (ADC_INPUT == Adc[idx].type) {
      uint16_t new_value = AdcRead(Adc[idx].pin, 5);
      if ((new_value < Adc[idx].last_value -10) || (new_value > Adc[idx].last_value +10)) {
        Adc[idx].last_value = new_value;
        uint16_t value = Adc[idx].last_value / 10;
        Response_P(PSTR("{\"ANALOG\":{\"A%ddiv10\":%d}}"), idx +1, (value > 99) ? 100 : value);
        XdrvRulesProcess();
      }
    }
    else if (ADC_JOY == Adc[idx].type) {
      uint16_t new_value = AdcRead(Adc[idx].pin, 1);
      if (new_value && (new_value != Adc[idx].last_value)) {
        Adc[idx].last_value = new_value;
        uint16_t value = new_value / Adc[idx].param1;
        Response_P(PSTR("{\"ANALOG\":{\"Joy%d\":%d}}"), idx +1, value);
        XdrvRulesProcess();
      } else {
        Adc[idx].last_value = 0;
      }
    }
  }
}
#endif  // USE_RULES

uint16_t AdcGetLux(uint32_t idx) {
  int adc = AdcRead(Adc[idx].pin, 2);
  // Source: https://www.allaboutcircuits.com/projects/design-a-luxmeter-using-a-light-dependent-resistor/
  double resistorVoltage = ((double)adc / ANALOG_RANGE) * ANALOG_V33;
  double ldrVoltage = ANALOG_V33 - resistorVoltage;
  double ldrResistance = ldrVoltage / resistorVoltage * (double)Adc[idx].param1;
  double ldrLux = (double)Adc[idx].param2 * FastPrecisePow(ldrResistance, (double)Adc[idx].param3 / 10000);

  return (uint16_t)ldrLux;
}

uint16_t AdcGetRange(uint32_t idx) {
  // formula for calibration: value, fromLow, fromHigh, toLow, toHigh
  // Example: 514, 632, 236, 0, 100
  // int( ((<param2> - <analog-value>) / (<param2> - <param1>) ) * (<param3> - <param4>) ) + <param4> )
  int adc = AdcRead(Adc[idx].pin, 2);
  double adcrange = ( ((double)Adc[idx].param2 - (double)adc) / ( ((double)Adc[idx].param2 - (double)Adc[idx].param1)) * ((double)Adc[idx].param3 - (double)Adc[idx].param4) + (double)Adc[idx].param4 );
  return (uint16_t)adcrange;
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
    for (uint32_t i = 0; i < samples; i++) {
      analog = analogRead(Adc[idx].pin);
      if (analog < analog_min) {
        analog_min = analog;
      }
      if (analog > analog_max) {
        analog_max = analog;
      }
      delay(1);
    }
    Adc[idx].current = (float)(analog_max-analog_min) * ((float)(Adc[idx].param2) / 100000);
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
      // Steinhart-Hart equation for thermistor as temperature sensor
      double Rt = (adc * Adc[idx].param1) / (1024.0 * ANALOG_V33 - (double)adc);
      double BC = (double)Adc[idx].param3 / 10000;
      double T = BC / (BC / ANALOG_T0 + TaylorLog(Rt / (double)Adc[idx].param2));
      Adc[idx].temperature = ConvertTemp(TO_CELSIUS(T));
    }
    else if (ADC_CT_POWER == Adc[idx].type) {
      AdcGetCurrentPower(idx, 5);
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
  char adc_name[10];  // ANALOG8

  bool jsonflg = false;
  for (uint32_t idx = 0; idx < Adcs.present; idx++) {
    snprintf_P(adc_name, sizeof(adc_name), PSTR("Analog%d"), idx +1);

    switch (Adc[idx].type) {
      case ADC_INPUT: {
        uint16_t analog = AdcRead(Adc[idx].pin, 5);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"A%d\":%d"), idx +1, analog);
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_ANALOG, "", idx +1, analog);
#endif  // USE_WEBSERVER
        }
        break;
      }
      case ADC_TEMP: {
        char temperature[33];
        dtostrfd(Adc[idx].temperature, Settings.flag2.temperature_resolution, temperature);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "%d\":%s"), idx +1, temperature);
          if ((0 == tele_period) && (!domo_flag[ADC_TEMP])) {
#ifdef USE_DOMOTICZ
            DomoticzSensor(DZ_TEMP, temperature);
            domo_flag[ADC_TEMP] = true;
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
            KnxSensor(KNX_TEMPERATURE, Adc[idx].temperature);
#endif  // USE_KNX
          }
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_TEMP, adc_name, temperature, TempUnit());
#endif  // USE_WEBSERVER
        }
        break;
      }
      case ADC_LIGHT: {
        uint16_t adc_light = AdcGetLux(idx);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"" D_JSON_ILLUMINANCE "%d\":%d"), idx +1, adc_light);
#ifdef USE_DOMOTICZ
          if ((0 == tele_period) && (!domo_flag[ADC_LIGHT])) {
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
        uint16_t adc_range = AdcGetRange(idx);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"" D_JSON_RANGE "%d\":%d"), idx +1, adc_range);
#ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_RANGE, adc_name, adc_range);
#endif  // USE_WEBSERVER
        }
        break;
      }
      case ADC_CT_POWER: {
        AdcGetCurrentPower(idx, 5);

        float voltage = (float)(Adc[idx].param3) / 10;
        char voltage_chr[FLOATSZ];
        dtostrfd(voltage, Settings.flag2.voltage_resolution, voltage_chr);
        char current_chr[FLOATSZ];
        dtostrfd(Adc[idx].current, Settings.flag2.current_resolution, current_chr);
        char power_chr[FLOATSZ];
        dtostrfd(voltage * Adc[idx].current, Settings.flag2.wattage_resolution, power_chr);
        char energy_chr[FLOATSZ];
        dtostrfd(Adc[idx].energy, Settings.flag2.energy_resolution, energy_chr);

        if (json) {
          AdcShowContinuation(&jsonflg);
          ResponseAppend_P(PSTR("\"CTEnergy%d\":{\"" D_JSON_ENERGY "\":%s,\"" D_JSON_POWERUSAGE "\":%s,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s}"),
            idx +1, energy_chr, power_chr, voltage_chr, current_chr);
#ifdef USE_DOMOTICZ
          if ((0 == tele_period) && (!domo_flag[ADC_CT_POWER])) {
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
          ResponseAppend_P(PSTR("\"Joy%d\":%d"), idx +1, value);
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
      if ((ADC_TEMP == XdrvMailbox.payload) ||
          (ADC_LIGHT == XdrvMailbox.payload) ||
          (ADC_RANGE == XdrvMailbox.payload) ||
          (ADC_CT_POWER == XdrvMailbox.payload) ||
          (ADC_JOY == XdrvMailbox.payload)) {
        AdcGetSettings(idx);
        if (ChrCount(XdrvMailbox.data, ",") > 2) {  // Process parameter entry
          char sub_string[XdrvMailbox.data_len +1];
          // AdcParam 2, 32000, 10000, 3350
          // AdcParam 3, 10000, 12518931, -1.405
          // AdcParam 6, 0, ANALOG_RANGE, 0, 100
          // AdcParam 7, 0, 2146, 0.23
          // AdcParam 8, 1000, 0, 0
          Adc[idx].type = XdrvMailbox.payload;
          Adc[idx].param1 = strtol(subStr(sub_string, XdrvMailbox.data, ",", 2), nullptr, 10);
          Adc[idx].param2 = strtol(subStr(sub_string, XdrvMailbox.data, ",", 3), nullptr, 10);
          if (ADC_RANGE == XdrvMailbox.payload) {
            Adc[idx].param3 = abs(strtol(subStr(sub_string, XdrvMailbox.data, ",", 4), nullptr, 10));
            Adc[idx].param4 = abs(strtol(subStr(sub_string, XdrvMailbox.data, ",", 5), nullptr, 10));
          } else {
            Adc[idx].param3 = (int)(CharToFloat(subStr(sub_string, XdrvMailbox.data, ",", 4)) * 10000);
          }
          if (ADC_CT_POWER == XdrvMailbox.payload) {
            if (((1 == Adc[idx].param1) & CT_FLAG_ENERGY_RESET) > 0) {
              for (uint32_t idx = 0; idx < MAX_ADCS; idx++) {
                Adc[idx].energy = 0;
              }
              Adc[idx].param1 ^= CT_FLAG_ENERGY_RESET;  // Cancel energy reset flag
            }
          }
        } else {                                         // Set default values based on current adc type
          // AdcParam 2
          // AdcParam 3
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
    if (ADC_RANGE == Adc[idx].type) {
      ResponseAppend_P(PSTR(",%d,%d"), Adc[idx].param3, Adc[idx].param4);
    } else {
      int value = Adc[idx].param3;
      uint8_t precision;
      for (precision = 4; precision > 0; precision--) {
        if (value % 10) { break; }
        value /= 10;
      }
      char param3[33];
      dtostrfd(((double)Adc[idx].param3)/10000, precision, param3);
      ResponseAppend_P(PSTR(",%s"), param3);
    }
    ResponseAppend_P(PSTR("]}"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns02(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kAdcCommands, AdcCommand);
      break;
    case FUNC_INIT:
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
#endif  // ESP32
