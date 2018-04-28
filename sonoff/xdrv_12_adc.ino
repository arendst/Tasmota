/*
xsns_02_adc.ino - ADC analog input and output mapping for Sonoff-Tasmota

Copyright (C) 2018 Bryan Mayland

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

#ifdef USE_ADC_PWM

enum AdcCommands {
  CMND_ADCPWM
};

const char kAdcCommands[] PROGMEM =
  D_CMND_ADCPWM
;

static struct tagAdcState {
  uint8_t fast_reads_remaining;
  uint16_t last_val;
} adc_state;

static void AdcInit(void)
{
  adc_state.fast_reads_remaining = 1;
}

/*
  Functionaly equivalent to:
  constrain(map(x, in_lo, in_hi, 0, out_hi), 0, out_hi);
*/
static inline uint16_t constrain_map(long x, int16_t in_lo, int16_t in_hi, int16_t out_hi)
{
  // Prevent divide by zero
  int16_t in_range = in_hi - in_lo;
  if (in_range == 0)
    in_range = 1;

  long retVal;
  retVal = (x - in_lo) * out_hi / in_range;
  if (retVal > out_hi)
    return out_hi;
  if (retVal < 0)
    return 0;
  return retVal;
}

static void AdcRead(boolean force)
{
  if (Settings.adc_pwm_target == 0)
    return;

  if (force || adc_state.fast_reads_remaining > 0)
  {
    uint16_t adc = analogRead(A0);
    /* If the difference between the last stored value and the new one is more
       than 1% of range (10 ticks), then start a fast read session */
    if (adc_state.last_val - adc + 10U >= (10U * 10U))
      adc_state.fast_reads_remaining = 20 * 10; // 20 times a second for 10 seconds

    if (adc_state.fast_reads_remaining > 0)
    {
      uint16_t range = Settings.pwm_range;
      uint16_t newpwm = constrain_map(adc, Settings.adc_pwm_map_lo, Settings.adc_pwm_map_hi, range);
      uint8_t index = Settings.adc_pwm_target;
      analogWrite(pin[GPIO_PWM1 + index - 1], bitRead(pwm_inverted, index - 1) ? range - newpwm : newpwm);
      adc_state.last_val = adc;
      adc_state.fast_reads_remaining--;
    }
  }
}

static boolean AdcCommand()
{
  char command[CMDSZ];
  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kAdcCommands);
  if (CMND_ADCPWM == command_code)
  {
    // "AdcRange pwmtarget,lo,hi"
    char *buf = XdrvMailbox.data;
    if (strlen(buf) > 0)
    {
      Settings.adc_pwm_target = atoi(buf);
      uint8_t i = 0;
      while ((buf = strchr(buf, ',')))
      {
        ++buf;
        switch (++i)
        {
        case 1: Settings.adc_pwm_map_lo = atoi(buf); break;
        case 2: Settings.adc_pwm_map_hi = atoi(buf); break;
        }
      }
      // Force next read to update with new parameters
      adc_state.fast_reads_remaining = 1;
    }

    char newvals[16];
    snprintf_P(newvals, sizeof(newvals), PSTR("%u,%d,%d"),
      Settings.adc_pwm_target, Settings.adc_pwm_map_lo, Settings.adc_pwm_map_hi);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, newvals);

    return true;
  }
  else
    return false;
}

#endif // USE_ADC_PWM

void AdcShow(boolean json)
{
  uint16_t analog = 0;
  for (byte i = 0; i < 32; i++) {
    analog += analogRead(A0);
    delay(1);
  }
  analog >>= 5;

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_JSON_ANALOG_INPUT "0\":%d"), mqtt_data, analog);
#ifdef USE_WEBSERVER
  }
  else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ANALOG, mqtt_data, "", 0, analog);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
* Interface
\*********************************************************************************************/

#ifdef USE_ADC_PWM

#define XDRV_12

boolean Xdrv12(byte function)
{
  boolean result = false;

  switch (function) {
  case FUNC_INIT:
    AdcInit();
    break;
  case FUNC_EVERY_50_MSECOND:
    AdcRead(false);
    break;
  case FUNC_EVERY_SECOND:
    AdcRead(true);
    break;
  case FUNC_COMMAND:
    result = AdcCommand();
    break;
  }
  return result;
}

#endif 

#define XSNS_02

boolean Xsns02(byte function)
{
  boolean result = false;

  if (pin[GPIO_ADC0] < 99) {
    switch (function) {
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
