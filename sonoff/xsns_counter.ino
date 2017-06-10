/*
  xsns_counter.ino - Counter sensors (water meters, electricity meters etc.) sensor support for Sonoff-Tasmota 

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
 * Counter sensors (water meters, electricity meters etc.)
\*********************************************************************************************/

unsigned long pTimeLast[MAX_COUNTERS]; // Last counter time in milli seconds

void counter_update(byte index)
{
//  char log[LOGSZ];

  unsigned long pTime = millis() - pTimeLast[index -1];
  if (pTime > sysCfg.pCounterDebounce) {
    pTimeLast[index -1] = millis();
    if (bitRead(sysCfg.pCounterType, index -1)) {
      rtcMem.pCounter[index -1] = pTime;
    } else {
      rtcMem.pCounter[index -1]++;
    }

//    snprintf_P(log, sizeof(log), PSTR("CNTR: Interrupt %d"), index);
//    addLog(LOG_LEVEL_DEBUG, log);
  }
}

void counter_update1()
{
  counter_update(1);
}

void counter_update2()
{
  counter_update(2);
}

void counter_update3()
{
  counter_update(3);
}

void counter_update4()
{
  counter_update(4);
}

void counter_savestate()
{
  for (byte i = 0; i < MAX_COUNTERS; i++) {
    if (pin[GPIO_CNTR1 +i] < 99) {
      sysCfg.pCounter[i] = rtcMem.pCounter[i];
    }
  }
}

void counter_init()
{
  typedef void (*function) () ;
  function counter_callbacks[] = { counter_update1, counter_update2, counter_update3, counter_update4 };
  
  for (byte i = 0; i < MAX_COUNTERS; i++) {
    if (pin[GPIO_CNTR1 +i] < 99) {
      pinMode(pin[GPIO_CNTR1 +i], INPUT_PULLUP);
      attachInterrupt(pin[GPIO_CNTR1 +i], counter_callbacks[i], FALLING);
    }
  }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void counter_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  char stemp[16];

  byte dsxflg = 0;
  for (byte i = 0; i < MAX_COUNTERS; i++) {
    if (pin[GPIO_CNTR1 +i] < 99) {
      if (bitRead(sysCfg.pCounterType, i)) {
        dtostrf((double)rtcMem.pCounter[i] / 1000, 1, 3, stemp);
      } else {
        dsxflg++;
        dtostrf(rtcMem.pCounter[i], 1, 0, stemp);
      }
      snprintf_P(svalue, ssvalue, PSTR("%s, \"Counter%d\":%s"), svalue, i +1, stemp);
      *djson = 1;
#ifdef USE_DOMOTICZ
      if (1 == dsxflg) {
        domoticz_sensor6(rtcMem.pCounter[i]);
        dsxflg++;
      }
#endif  // USE_DOMOTICZ
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_COUNTER[] PROGMEM =
  "<tr><th>Counter%d</th><td>%s%s</td></tr>";  

String counter_webPresent()
{
  String page = "";
  char stemp[16];
  char sensor[80];

  for (byte i = 0; i < MAX_COUNTERS; i++) {
    if (pin[GPIO_CNTR1 +i] < 99) {
      if (bitRead(sysCfg.pCounterType, i)) {
        dtostrf((double)rtcMem.pCounter[i] / 1000, 1, 3, stemp);
      } else {
        dtostrf(rtcMem.pCounter[i], 1, 0, stemp);
      }
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_COUNTER, i+1, stemp, (bitRead(sysCfg.pCounterType, i)) ? " Sec" : "");
      page += sensor;
    }
  }
  return page;
}
#endif  // USE_WEBSERVER

