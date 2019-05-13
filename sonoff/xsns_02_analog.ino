/*
  xsns_02_analog.ino - ESP8266 ADC support for Sonoff-Tasmota

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

#define XSNS_02      2

#define TO_CELSIUS(x) ((x) - 273.15)
#define TO_KELVIN(x) ((x) + 273.15)

// Parameters for Steinhart-Hart equation
#define ANALOG_V33   3.3               // ESP8266 Analog voltage
#define ANALOG_T0    TO_KELVIN(25.0)   // 25 degrees Celcius in Kelvin (= 298.15)
// Shelly 2.5 thermistor
#define ANALOG_R21   32000.0           // Voltage bridge resistor
#define ANALOG_R0    10000.0           // Thermistor resistance
#define ANALOG_B     3350.0            // Thermistor Beta Coefficient

uint16_t adc_last_value = 0;
float adc_temp = 0;

uint16_t AdcRead(uint8_t factor)
{
  // factor 1 = 2 samples
  // factor 2 = 4 samples
  // factor 3 = 8 samples
  // factor 4 = 16 samples
  // factor 5 = 32 samples
  uint8_t samples = 1 << factor;
  uint16_t analog = 0;
  for (uint8_t i = 0; i < samples; i++) {
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

void AdcEverySecond(void)
{
  if (ADC0_TEMP == my_adc0) {
    int adc = AdcRead(2);
    // Steinhart-Hart equation for thermistor as temperature sensor
    double Rt = (adc * ANALOG_R21) / (1024.0 * ANALOG_V33 - (double)adc);
    double T = ANALOG_B / (ANALOG_B/ANALOG_T0 + log(Rt/ANALOG_R0));
    adc_temp = ConvertTemp(TO_CELSIUS(T));
  }
}

float AdcTemperature(void)
{
  return adc_temp;
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
  if (ADC0_TEMP == my_adc0) {
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
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns02(uint8_t function)
{
  bool result = false;

  if ((ADC0_INPUT == my_adc0) || (ADC0_TEMP == my_adc0)) {
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
  return result;
}

#endif  // USE_ADC_VCC
