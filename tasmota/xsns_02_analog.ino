/*
  xsns_02_analog.ino - ESP8266 ADC support for Tasmota

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

uint16_t adc_last_value = 0;
float adc_temp = 0;

void AdcInit(void)
{
  if ((Settings.adc_param_type != my_adc0) || (Settings.adc_param1 > 1000000) || (Settings.adc_param1 < 100)) {
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
    else if (ADC0_MOIST == my_adc0) {
      Settings.adc_param_type = ADC0_MOIST;
      Settings.adc_param1 = 0;
      Settings.adc_param2 = 1023;
      Settings.adc_param3 = 0;
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
    if ((new_value < adc_last_value -10) || (new_value > adc_last_value +10)) {
      adc_last_value = new_value;
      uint16_t value = adc_last_value / 10;
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

uint16_t AdcGetMoist(void)
// formula for calibration: value, fromLow, fromHigh, toHigh, toLow
// Example: 632, 0, 1023, 100, 0
// int( ( ( (<param2> - <analogue-value>) / ( <param2> - <param1> ) ) * ( <param3> - <param4> ) ) + <param4> )
// double amoist = ((Settings.adc_param2 - (double)adc) / (Settings.adc_param2 - Settings.adc_param1) * 100;
// int((((1023 - <analog-reading>) / ( 1023 - 0 )) * ( 100 - 0 )) + 0 )

{
   int adc = AdcRead(2);
   double amoist = ((double)Settings.adc_param2 - (double)adc) / ((double)Settings.adc_param2 - (double)Settings.adc_param1) * 100;
   //double amoist = ((1023 - (double)adc) / 1023) * 100;
   return (uint16_t)amoist;
}

void AdcEverySecond(void)
{
  if (ADC0_TEMP == my_adc0) {
    int adc = AdcRead(2);
    // Steinhart-Hart equation for thermistor as temperature sensor
    double Rt = (adc * Settings.adc_param1) / (1024.0 * ANALOG_V33 - (double)adc);
    double BC = (double)Settings.adc_param3 / 10000;
    double T = BC / (BC / ANALOG_T0 + TaylorLog(Rt / (double)Settings.adc_param2));
    adc_temp = ConvertTemp(TO_CELSIUS(T));
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
    dtostrfd(adc_temp, Settings.flag2.temperature_resolution, temperature);

    if (json) {
      ResponseAppend_P(JSON_SNS_TEMP, "ANALOG", temperature);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_TEMP, temperature);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
      if (0 == tele_period) {
        KnxSensor(KNX_TEMPERATURE, adc_temp);
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
  else if (ADC0_MOIST == my_adc0) {
    uint16_t adc_moist = AdcGetMoist();
    if (json) {
      ResponseAppend_P(JSON_SNS_MOISTURE, "ANALOG", adc_moist);
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_MOISTURE, "", adc_moist);
#endif  // USE_WEBSERVER
    }
   }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_CMND_ADCPARAM "AdcParam"
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
    if ((ADC0_TEMP == XdrvMailbox.payload) || (ADC0_LIGHT == XdrvMailbox.payload) || (ADC0_MOIST == XdrvMailbox.payload)) { 
//      if ((XdrvMailbox.payload == my_adc0) && ((ADC0_TEMP == my_adc0) || (ADC0_LIGHT == my_adc0))) {
      if (strstr(XdrvMailbox.data, ",") != nullptr) {  // Process parameter entry
        char sub_string[XdrvMailbox.data_len +1];
        // AdcParam 2, 32000, 10000, 3350
        // AdcParam 3, 10000, 12518931, -1.405
        Settings.adc_param_type = XdrvMailbox.payload;
//          Settings.adc_param_type = my_adc0;
        Settings.adc_param1 = strtol(subStr(sub_string, XdrvMailbox.data, ",", 2), nullptr, 10);
        Settings.adc_param2 = strtol(subStr(sub_string, XdrvMailbox.data, ",", 3), nullptr, 10);
        if (!ADC0_MOIST == XdrvMailbox.payload) {
          Settings.adc_param3 = (int)(CharToFloat(subStr(sub_string, XdrvMailbox.data, ",", 4)) * 10000);
        }
      } else {                                         // Set default values based on current adc type
        // AdcParam 2
        // AdcParam 3
        // AdcParam 6
        Settings.adc_param_type = 0;
        AdcInit();
      }
    }
  }

  // AdcParam
  int value = Settings.adc_param3;
  uint8_t precision;
  for (precision = 4; precision > 0; precision--) {
    if (value % 10) { break; }
    value /= 10;
  }
  char param3[33];
  dtostrfd(((double)Settings.adc_param3)/10000, precision, param3);
  if ((ADC0_TEMP == my_adc0) || (ADC0_LIGHT == my_adc0)) {
    Response_P(PSTR("{\"" D_CMND_ADCPARAM "\":[%d,%d,%d,%s]}"),
      Settings.adc_param_type, Settings.adc_param1, Settings.adc_param2, param3);
  } else if (ADC0_MOIST == my_adc0) {
    Response_P(PSTR("{\"" D_CMND_ADCPARAM "\":[%d,%d,%d]}"),
      Settings.adc_param_type, Settings.adc_param1, Settings.adc_param2);
  } 
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
      if ((ADC0_INPUT == my_adc0) || (ADC0_TEMP == my_adc0) || (ADC0_LIGHT == my_adc0) || (ADC0_MOIST == my_adc0)) {
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
