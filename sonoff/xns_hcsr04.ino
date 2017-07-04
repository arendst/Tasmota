/*
  xsns_hcsr04.ino - Distance sensors (Measure distance by ultrasonic sound) sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Maarten Damen and Theo Arends

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

/*********************************************************************************************\
 * Distance sensors (Measure distance by ultrasonic sound)
\*********************************************************************************************/

void hcsr04_init()
{
    if (pin[GPIO_SEN_TRIG] < 99) {
      pinMode(pin[GPIO_SEN_TRIG], OUTPUT);
      pinMode(pin[GPIO_SEN_ECHO], INPUT);
    }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void hcsr04_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  char stemp[16];

  byte dsxflg = 0;

  if (pin[GPIO_SEN_TRIG] < 99) {
/*    if (bitRead(sysCfg.pCounterType, i)) {
      dtostrf((double)rtcMem.pCounter[i] / 1000, 1, 3, stemp);
    } else {
      dsxflg++;
      dtostrf(rtcMem.pCounter[i], 1, 0, stemp);
*/ }
    snprintf_P(svalue, ssvalue, PSTR("%s, \"Distance\":%s"), svalue,  stemp);
    *djson = 1;
#ifdef USE_DOMOTICZ
    if (1 == dsxflg) {
      // need iunvestigation
    //  domoticz_sensor6(rtcMem.pCounter[i]);
      dsxflg++;
    }
#endif  // USE_DOMOTICZ

}
