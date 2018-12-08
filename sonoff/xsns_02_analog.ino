/*
  xsns_02_analog.ino - ESP8266 ADC support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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
double scale = 1;
double offset = 0;
uint8_t resolution = 3;

const char ADC_SENSOR_RESPONSE[] PROGMEM = "{\"Sensor02_A%i\":{\"SCALE\":%s,\"OFFSET\":%s,\"DECIMALS\":%i}}";

double AdcRead(void)
{
  uint16_t analogRaw = 0;   // Raw analog value read from analog input
  double analogEU;          // Scaled analog value in engineering units
  for (byte i = 0; i < 32; i++) {
    analogRaw += analogRead(A0);
    delay(1);
  }
  analogRaw >>= 5;
  analogEU = scale * analogRaw + offset;
  return analogEU;
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

void AdcShow(boolean json)
{
  char analog[10];
  double analogEU = AdcRead();
  dtostrfd(analogEU, resolution, analog);

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"ANALOG\":{\"A0\":%s}"), mqtt_data, analog);
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ANALOG, mqtt_data, "", 0, analog);
#endif  // USE_WEBSERVER
  }
}

/***********************************************************************************\
 * Command Sensor02
 *
 * Analog scaling formula: 
 *    analogEU = SCALE * analogRaw + OFFSET
 * 
 * Command  | Payload       | Description
 * ---------|---------------|---------------------------------------------
 * Sensor02 |               | Show current parameter values
 * Sensor02 | SCALE,a       | Set scale to value a. Scale is double 
 * Sensor02 | OFFSET,b      | Set offset to value b. Offset is double
 * Sensor02 | DECIMALS,d    | Set resolution to d decimal digits.
 ***********************************************************************************/

bool AdcCommandSensor(void)
{
  boolean serviced = true;
  uint8_t paramcount = 0;
  char scale_str[10];
  char offset_str[10];

  if (XdrvMailbox.data_len > 0) {
    paramcount=1;
  }
  else {
    dtostrfd(scale, 4, scale_str);
    dtostrfd(offset, 4, offset_str);
    snprintf_P(mqtt_data, sizeof(mqtt_data), ADC_SENSOR_RESPONSE,0,scale_str,offset_str,resolution);
    return serviced;
  }

  char sub_string[XdrvMailbox.data_len];
  for (uint8_t ca=0;ca<XdrvMailbox.data_len;ca++) { // Replace <space> and <=> with <,> and count parameters
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
    if (',' == XdrvMailbox.data[ca]) { paramcount++; }
  }
  UpperCase(XdrvMailbox.data,XdrvMailbox.data); // All upper case

  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"SCALE")) {  // SCALE Command parameter
    if (paramcount > 1) {
      scale = CharToDouble(subStr(sub_string, XdrvMailbox.data, ",", 2));
      AdcShow(1);
      return serviced;
    }
    else {
      serviced = false;
      return serviced;
    }
  }

  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"OFFSET")) { // OFFSET Command parameter
    if (paramcount > 1) {
      offset = CharToDouble(subStr(sub_string, XdrvMailbox.data, ",", 2));
      AdcShow(1);
      return serviced;
    }
    else {
      serviced = false;
      return serviced;
    }
  }

  if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"DECIMALS")) { // DECIMALS Command parameter
    if (paramcount > 1) {
      resolution = CharToDouble(subStr(sub_string, XdrvMailbox.data, ",", 2));
      AdcShow(1);
      return serviced;
    }
    else {
      serviced = false;
      return serviced;
    }
  }

}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns02(byte function)
{
  boolean result = false;

  if (pin[GPIO_ADC0] < 99) {
    switch (function) {
      case FUNC_COMMAND:
        if (XSNS_02 == XdrvMailbox.index) {
          result = AdcCommandSensor();
      
 //         snprintf_P(log_data, sizeof(log_data), PSTR("ANALOG: Cmd Result %s"), result ? "true" : "false");
 //         AddLog(LOG_LEVEL_DEBUG);
      
        }
        break;
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
