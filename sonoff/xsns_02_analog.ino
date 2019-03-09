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

#define XSNS_02             2

uint16_t adc_last_value = 0;

uint16_t AdcRead(void)
{
  uint16_t analog = 0;
  for (uint8_t i = 0; i < 32; i++) {
    analog += analogRead(A0);
    delay(1);
  }
  analog >>= 5;
  return analog;
}

#ifdef USE_RULES
void AdcEvery250ms(void)
{
  uint16_t new_value = AdcRead();
  if ((new_value < adc_last_value -10) || (new_value > adc_last_value +10)) {
    adc_last_value = new_value;
    uint16_t value = adc_last_value / 10;
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"ANALOG\":{\"A0div10\":%d}}"), (value > 99) ? 100 : value);
    XdrvRulesProcess();
  }
}
#endif  // USE_RULES

void AdcShow(bool json)
{
  uint16_t analog = AdcRead();

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"ANALOG\":{\"A0\":%d}"), mqtt_data, analog);
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ANALOG, mqtt_data, "", 0, analog);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns02(uint8_t function)
{
  bool result = false;

  if (my_module_flag.adc0) {
    switch (function) {
#ifdef USE_RULES
      case FUNC_EVERY_250_MSECOND:
        AdcEvery250ms();
        break;
#endif  // USE_RULES
      case FUNC_JSON_APPEND:
        AdcShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        AdcShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_ADC_VCC
