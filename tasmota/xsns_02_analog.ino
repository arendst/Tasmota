/*
  xsns_02_analog.ino - ESP8266 ADC support for Tasmota

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

#ifndef USE_ADC_VCC
/*********************************************************************************************\
 * ADC support
\*********************************************************************************************/

#define XSNS_02                       2

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
#define ANALOG_CT_MULTIPLIER          2146             // (uint32_t) Multiplier*100000 to convert raw ADC peak to peak range 0..1023 to RMS current in Amps. Value of 100000 corresponds to 1
#define ANALOG_CT_VOLTAGE             2300             // (int) Convert current in Amps to apparrent power in Watts using voltage in Volts*10. Value of 2200 corresponds to 220V

#define CT_FLAG_ENERGY_RESET          (1 << 0)         // Reset energy total

struct {
  float temperature = 0;
  float current = 0;
  float energy = 0;
  uint32_t previous_millis = 0;
  uint16_t last_value = 0;
} Adc;

void AdcInit(void)
{
  if ((Settings.adc_param_type != my_adc0) || (Settings.adc_param1 > 1000000)) {
    if (ADC0_TEMP == my_adc0) {
      // Default Shelly 2.5 and 1PM parameters
      Settings.adc_param_type = ADC0_TEMP;
      Settings.adc_param1 = ANALOG_NTC_BRIDGE_RESISTANCE;
      Settings.adc_param2 = ANALOG_NTC_RESISTANCE;
      Settings.adc_param3 = ANALOG_NTC_B_COEFFICIENT * 10000;
    }
    else if (ADC0_LIGHT == my_adc0) {
      Settings.adc_param_type = ADC0_LIGHT;
      Settings.adc_param1 = ANALOG_LDR_BRIDGE_RESISTANCE;
      Settings.adc_param2 = ANALOG_LDR_LUX_CALC_SCALAR;
      Settings.adc_param3 = ANALOG_LDR_LUX_CALC_EXPONENT * 10000;
    }
    else if (ADC0_RANGE == my_adc0) {
      Settings.adc_param_type = ADC0_RANGE;
      Settings.adc_param1 = 0;
      Settings.adc_param2 = 1023;
      Settings.adc_param3 = 0;
      Settings.adc_param4 = 100;
    }
    else if (ADC0_CT_POWER == my_adc0) {
      Settings.adc_param_type = ADC0_CT_POWER;
      Settings.adc_param1 = ANALOG_CT_FLAGS;              //(uint32_t) 0
      Settings.adc_param2 = ANALOG_CT_MULTIPLIER;         //(uint32_t) 100000
      Settings.adc_param3 = ANALOG_CT_VOLTAGE;            //(int)      10
    }
  }
}

uint16_t AdcRead(uint8_t factor)
{
  // factor 1 = 2 samples
  // factor 2 = 4 samples
  // factor 3 = 8 samples
  // factor 4 = 16 samples
  // factor 5 = 32 samples
  uint8_t samples = 1 << factor;
  uint16_t analog = 0;
  for (uint32_t i = 0; i < samples; i++) {
    analog += analogRead(A0);
    delay(1);
  }
  analog >>= factor;
  return analog;
}

#ifdef USE_RULES
void AdcEvery250ms(void)
{
  if (ADC0_INPUT == my_adc0) {
    uint16_t new_value = AdcRead(5);
    if ((new_value < Adc.last_value -10) || (new_value > Adc.last_value +10)) {
      Adc.last_value = new_value;
      uint16_t value = Adc.last_value / 10;
      Response_P(PSTR("{\"ANALOG\":{\"A0div10\":%d}}"), (value > 99) ? 100 : value);
      XdrvRulesProcess();
    }
  }
}
#endif  // USE_RULES

uint16_t AdcGetLux(void)
{
  int adc = AdcRead(2);
  // Source: https://www.allaboutcircuits.com/projects/design-a-luxmeter-using-a-light-dependent-resistor/
  double resistorVoltage = ((double)adc / 1023) * ANALOG_V33;
  double ldrVoltage = ANALOG_V33 - resistorVoltage;
  double ldrResistance = ldrVoltage / resistorVoltage * (double)Settings.adc_param1;
  double ldrLux = (double)Settings.adc_param2 * FastPrecisePow(ldrResistance, (double)Settings.adc_param3 / 10000);

  return (uint16_t)ldrLux;
}

uint16_t AdcGetRange(void)
{
  // formula for calibration: value, fromLow, fromHigh, toLow, toHigh
  // Example: 514, 632, 236, 0, 100
  // int( ((<param2> - <analog-value>) / (<param2> - <param1>) ) * (<param3> - <param4>) ) + <param4> )
  int adc = AdcRead(2);
  double adcrange = ( ((double)Settings.adc_param2 - (double)adc) / ( ((double)Settings.adc_param2 - (double)Settings.adc_param1)) * ((double)Settings.adc_param3 - (double)Settings.adc_param4) + (double)Settings.adc_param4 );
  return (uint16_t)adcrange;
}

void AdcGetCurrentPower(uint8_t factor)
{
  // factor 1 = 2 samples
  // factor 2 = 4 samples
  // factor 3 = 8 samples
  // factor 4 = 16 samples
  // factor 5 = 32 samples
  uint8_t samples = 1 << factor;
  uint16_t analog = 0;
  uint16_t analog_min = 1023;
  uint16_t analog_max = 0;

  if(Settings.adc_param1==0){
  for (uint32_t i = 0; i < samples; i++) {
    analog = analogRead(A0);
    if (analog < analog_min) {
      analog_min = analog;
    }
    if (analog > analog_max) {
      analog_max = analog;
    }
    delay(1);
  }
  Adc.current = (float)(analog_max-analog_min) * ((float)(Settings.adc_param2) / 100000);
  } 
  else{

    analog = AdcRead(5);
    if(analog>Settings.adc_param1){
     Adc.current = ((float)(analog) - (float)Settings.adc_param1) * ((float)(Settings.adc_param2) / 100000);
    }
    else{
      Adc.current = 0;
    }
  }

  float power = Adc.current * (float)(Settings.adc_param3) / 10;
  uint32_t current_millis = millis();
  Adc.energy = Adc.energy + ((power * (current_millis - Adc.previous_millis)) / 3600000000);
  Adc.previous_millis = current_millis;
}

void AdcEverySecond(void)
{
  if (ADC0_TEMP == my_adc0) {
    int adc = AdcRead(2);
    // Steinhart-Hart equation for thermistor as temperature sensor
    double Rt = (adc * Settings.adc_param1) / (1024.0 * ANALOG_V33 - (double)adc);
    double BC = (double)Settings.adc_param3 / 10000;
    double T = BC / (BC / ANALOG_T0 + TaylorLog(Rt / (double)Settings.adc_param2));
    Adc.temperature = ConvertTemp(TO_CELSIUS(T));
  }
  else if (ADC0_CT_POWER == my_adc0) {
    AdcGetCurrentPower(5);
  }
}

void AdcShow(bool json)
{
  if (ADC0_INPUT == my_adc0) {
    uint16_t analog = AdcRead(5);

    if (json) {
      ResponseAppend_P(PSTR(",\"ANALOG\":{\"A0\":%d}"), analog);
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_ANALOG, "", 0, analog);
#endif  // USE_WEBSERVER
    }
  }

  else if (ADC0_TEMP == my_adc0) {
    char temperature[33];
    dtostrfd(Adc.temperature, Settings.flag2.temperature_resolution, temperature);

    if (json) {
      ResponseAppend_P(JSON_SNS_TEMP, "ANALOG", temperature);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_TEMP, temperature);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
      if (0 == tele_period) {
        KnxSensor(KNX_TEMPERATURE, Adc.temperature);
      }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_TEMP, "", temperature, TempUnit());
#endif  // USE_WEBSERVER
    }
  }

  else if (ADC0_LIGHT == my_adc0) {
    uint16_t adc_light = AdcGetLux();

    if (json) {
      ResponseAppend_P(JSON_SNS_ILLUMINANCE, "ANALOG", adc_light);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_ILLUMINANCE, adc_light);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_ILLUMINANCE, "", adc_light);
#endif  // USE_WEBSERVER
    }
  }

  else if (ADC0_RANGE == my_adc0) {
    uint16_t adc_range = AdcGetRange();

    if (json) {
      ResponseAppend_P(JSON_SNS_RANGE, "ANALOG", adc_range);
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_RANGE, "", adc_range);
#endif  // USE_WEBSERVER
    }
  }

  else if (ADC0_CT_POWER == my_adc0) {
    AdcGetCurrentPower(5);

    float voltage = (float)(Settings.adc_param3) / 10;
    char voltage_chr[FLOATSZ];
    dtostrfd(voltage, Settings.flag2.voltage_resolution, voltage_chr);
    char current_chr[FLOATSZ];
    dtostrfd(Adc.current, Settings.flag2.current_resolution, current_chr);
    char power_chr[FLOATSZ];
    dtostrfd(voltage * Adc.current, Settings.flag2.wattage_resolution, power_chr);
    char energy_chr[FLOATSZ];
    dtostrfd(Adc.energy, Settings.flag2.energy_resolution, energy_chr);

    if (json) {
      ResponseAppend_P(PSTR(",\"ANALOG\":{\"" D_JSON_ENERGY "\":%s,\"" D_JSON_POWERUSAGE "\":%s,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s}"),
        energy_chr, power_chr, voltage_chr, current_chr);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_POWER_ENERGY, power_chr);
        DomoticzSensor(DZ_VOLTAGE, voltage_chr);
        DomoticzSensor(DZ_CURRENT, current_chr);
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
  }

}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kAdcCommands[] PROGMEM = "|"  // No prefix
  D_CMND_ADC "|" D_CMND_ADCS "|" D_CMND_ADCPARAM;

void (* const AdcCommand[])(void) PROGMEM = {
  &CmndAdc, &CmndAdcs, &CmndAdcParam };

void CmndAdc(void)
{
  if (ValidAdc() && (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < ADC0_END)) {
    Settings.my_adc0 = XdrvMailbox.payload;
    restart_flag = 2;
  }
  char stemp1[TOPSZ];
  Response_P(PSTR("{\"" D_CMND_ADC "0\":{\"%d\":\"%s\"}}"), Settings.my_adc0, GetTextIndexed(stemp1, sizeof(stemp1), Settings.my_adc0, kAdc0Names));
}

void CmndAdcs(void)
{
  Response_P(PSTR("{\"" D_CMND_ADCS "\":{"));
  bool jsflg = false;
  char stemp1[TOPSZ];
  for (uint32_t i = 0; i < ADC0_END; i++) {
    if (jsflg) {
      ResponseAppend_P(PSTR(","));
    }
    jsflg = true;
    ResponseAppend_P(PSTR("\"%d\":\"%s\""), i, GetTextIndexed(stemp1, sizeof(stemp1), i, kAdc0Names));
  }
  ResponseJsonEndEnd();
}

void CmndAdcParam(void)
{
  if (XdrvMailbox.data_len) {
    if ((ADC0_TEMP == XdrvMailbox.payload) ||
        (ADC0_LIGHT == XdrvMailbox.payload) ||
        (ADC0_RANGE == XdrvMailbox.payload) ||
        (ADC0_CT_POWER == XdrvMailbox.payload)) {
      if (strstr(XdrvMailbox.data, ",") != nullptr) {  // Process parameter entry
        char sub_string[XdrvMailbox.data_len +1];
        // AdcParam 2, 32000, 10000, 3350
        // AdcParam 3, 10000, 12518931, -1.405
        // AdcParam 6, 0, 1023, 0, 100
        Settings.adc_param_type = XdrvMailbox.payload;
        Settings.adc_param1 = strtol(subStr(sub_string, XdrvMailbox.data, ",", 2), nullptr, 10);
        Settings.adc_param2 = strtol(subStr(sub_string, XdrvMailbox.data, ",", 3), nullptr, 10);
        if (ADC0_RANGE == XdrvMailbox.payload) {
          Settings.adc_param3 = abs(strtol(subStr(sub_string, XdrvMailbox.data, ",", 4), nullptr, 10));
          Settings.adc_param4 = abs(strtol(subStr(sub_string, XdrvMailbox.data, ",", 5), nullptr, 10));
        } else {
          Settings.adc_param3 = (int)(CharToFloat(subStr(sub_string, XdrvMailbox.data, ",", 4)) * 10000);
        }
        if (ADC0_CT_POWER == XdrvMailbox.payload) {
          if ((Settings.adc_param1 == 1 & CT_FLAG_ENERGY_RESET) > 0) {
            Adc.energy = 0;
            Settings.adc_param1 ^= CT_FLAG_ENERGY_RESET;  // Cancel energy reset flag
          }
        }
      } else {                                         // Set default values based on current adc type
        // AdcParam 2
        // AdcParam 3
        // AdcParam 6
        // AdcParam 7
        Settings.adc_param_type = 0;
        AdcInit();
      }
    }
  }

  // AdcParam
  Response_P(PSTR("{\"" D_CMND_ADCPARAM "\":[%d,%d,%d"), Settings.adc_param_type, Settings.adc_param1, Settings.adc_param2);
  if (ADC0_RANGE == my_adc0) {
    ResponseAppend_P(PSTR(",%d,%d"), Settings.adc_param3, Settings.adc_param4);
  } else {
    int value = Settings.adc_param3;
    uint8_t precision;
    for (precision = 4; precision > 0; precision--) {
      if (value % 10) { break; }
      value /= 10;
    }
    char param3[33];
    dtostrfd(((double)Settings.adc_param3)/10000, precision, param3);
    ResponseAppend_P(PSTR(",%s"), param3);
  }
  ResponseAppend_P(PSTR("]}"));
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns02(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kAdcCommands, AdcCommand);
      break;
    default:
      if ((ADC0_INPUT == my_adc0) ||
          (ADC0_TEMP == my_adc0) ||
          (ADC0_LIGHT == my_adc0) ||
          (ADC0_RANGE == my_adc0) ||
          (ADC0_CT_POWER == my_adc0)) {
        switch (function) {
#ifdef USE_RULES
          case FUNC_EVERY_250_MSECOND:
            AdcEvery250ms();
            break;
#endif  // USE_RULES
          case FUNC_EVERY_SECOND:
            AdcEverySecond();
            break;
          case FUNC_INIT:
            AdcInit();
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

#endif  // USE_ADC_VCC