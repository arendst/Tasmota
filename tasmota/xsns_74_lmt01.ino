/*
  xns_74_lmt01.ino - Support for single wire LMT01 Temperature Sensor

  Copyright (C) 2020  Theo Arends, Justifiably

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

#ifdef USE_LMT01
/*********************************************************************************************\
 * LMT01 - 0.5°C Accurate 2-Pin Digital Output Temperature Sensor With Pulse Count Interface
 *
 * Uses fragments of public domain code LMT01_Example.ino released by Texas Instruments, July 10th 2017.
 * See https://training.ti.com/how-interface-lmt01-temperature-sensor-arduino
\*********************************************************************************************/

#define XSNS_74		      74

#define LMT01_TIMEOUT   200   // ms timeout for a reading cycle

bool lmt01_initialized = false;
float lmt01_temperature = NAN;

void LMT01_Init(void) {
  if (PinUsed(GPIO_LMT01)) {
    pinMode(Pin(GPIO_LMT01), INPUT);
    attachInterrupt(Pin(GPIO_LMT01), LMT01_countPulse, FALLING);
    lmt01_initialized = true;
  }
}

#ifndef ARDUINO_ESP8266_RELEASE_2_3_0  // Fix core 2.5.x ISR not in IRAM Exception
void LMT01_countPulse(void) ICACHE_RAM_ATTR;
#endif // ARDUINO_ESP8266_RELEASE_2_3_0

volatile int lmt01_pulseCount = 0;

void LMT01_countPulse(void) {
  lmt01_pulseCount++;
}

void LMT01_GetTemperature(void) {
  int pulses = 0;
  pulses = LMT01_getPulses();
  if (pulses >= 0) {
    // simple linear conversion, datasheet has a look-up table alternative
    // which is accurate over a wider temperature range
    lmt01_temperature = ConvertTemp(0.0625 * pulses - 50);
  } else {
    lmt01_temperature = NAN;  // Timeout
  }
}

int LMT01_getPulses(void) {
  int timeout = LMT01_TIMEOUT;
  int hold = -1;
  // complete current pulse cycle (50ms max)
  while(lmt01_pulseCount != hold && --timeout > 0) {
    hold = lmt01_pulseCount;
    delay(1);
  }
  lmt01_pulseCount = 0;
  // wait for start of next (54ms max)
  while(lmt01_pulseCount == 0 && --timeout > 0) {
    delay(1);
  }
  hold = -1;
  // take this count (up to 50ms)
  while(lmt01_pulseCount != hold && --timeout > 0) {
    hold = lmt01_pulseCount;
    delay(1);
  }
  // discard spurious low counts 
  if (timeout > 0 && hold >= 10) {
    return hold;
  }
  return -1;
}

void LMT01_Show(bool Json) {
  char temp[33];
  dtostrfd(lmt01_temperature, Settings.flag2.temperature_resolution, temp);

  if (Json) {
    ResponseAppend_P(JSON_SNS_TEMP, "LMT01", temp);
#ifdef USE_DOMOTICZ
    if (0 == tele_period) {
      DomoticzSensor(DZ_TEMP, temp);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
    if (0 == tele_period) {
      KnxSensor(KNX_TEMPERATURE, lmt01_temperature);
    }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_TEMP, "LMT01", temp, TempUnit());
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns74(uint8_t function)
{
  bool result = false;

  if (FUNC_INIT == function) {
    LMT01_Init();
  }
  else if (lmt01_initialized) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        LMT01_GetTemperature();
        break;
      case FUNC_JSON_APPEND:
        LMT01_Show(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        LMT01_Show(false);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_LMT01
