/*
  xsns_58_GP2Y10.ino - ESP8266 GP2Y10 support for Sonoff-Tasmota

  Copyright (C) 2019  Arnout Smeenge

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

#ifdef USE_GP2Y10
/*********************************************************************************************\
 * GP2Y10 support
\*********************************************************************************************/

#define XSNS_58                       58

// Parameters for equation
bool gpy_active = true;
bool gpy_gpio_pin = false;

void GPYInit(void)
{
  if ((ADC0_GP2Y10 == my_adc0) && gpy_gpio_pin) {
    Settings.adc_param_type = ADC0_GP2Y10;
    pinMode(pin[GPIO_GP2Y10], OUTPUT);
    digitalWrite(pin[GPIO_GP2Y10], HIGH);
  } else {
    //deactivate GPY
    gpy_active = false;
  }
}

bool GPYPinState()
{
  if ((XdrvMailbox.index == GPIO_GP2Y10) && (ADC0_GP2Y10 == my_adc0)) {
    XdrvMailbox.index = GPIO_GP2Y10;
    gpy_gpio_pin = true;
    return true;
  }
  return false;
}

uint16_t GPYRead(uint8_t factor)
{
  // factor 1 = 2 samples
  // factor 2 = 4 samples
  // factor 3 = 8 samples
  // factor 4 = 16 samples
  // factor 5 = 32 samples
  uint8_t samples = 1 << factor;
  uint16_t analog = 0;

  noInterrupts();
  byte x;
  for (x = 0; x < samples; x++)
  {
    digitalWrite(pin[GPIO_GP2Y10], LOW);
    delayMicroseconds(280);
    analog += analogRead(A0);
    delayMicroseconds(40);
    digitalWrite(pin[GPIO_GP2Y10], HIGH);
    delayMicroseconds(9680);
  }
  interrupts();

  int qual = 100 - ((analog*100) >> (factor+10));

  return qual;
}

#ifdef USE_RULES
void GPYEvery250ms(void)
{

}
#endif  // USE_RULES

void GPYEverySecond(void)
{

}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void GPYShow(bool json)
{
  if (gpy_active) {
      uint16_t gpy_quality = GPYRead(4);
        if (json) {
          ResponseAppend_P(JSON_SNS_AIRQUALITY, D_SENSOR_GP2Y10, gpy_quality);
    #ifdef USE_WEBSERVER
        } else {
          WSContentSend_PD(HTTP_SNS_AIRQUALITY, D_SENSOR_GP2Y10, gpy_quality);
    #endif  // USE_WEBSERVER
        }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns58(uint8_t function)
{
  bool result = false;

  if (gpy_active) {
    switch (function) {
#ifdef USE_RULES
      case FUNC_EVERY_250_MSECOND:
        GPYEvery250ms();
        break;
#endif  // USE_RULES
      case FUNC_EVERY_SECOND:
        GPYEverySecond();
        break;
      case FUNC_INIT:
        GPYInit();
        break;
      case FUNC_PIN_STATE:  //before init
        result = GPYPinState();
        break;
      case FUNC_JSON_APPEND:
        GPYShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        GPYShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_GP2Y10
