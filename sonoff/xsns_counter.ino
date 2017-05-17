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

#ifdef USE_COUNTER
/*********************************************************************************************\
 * Counter sensors (water meters, electricity meters etc.)
\*********************************************************************************************/

void counter_update1()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("COUNTER: Received interrupt 1, increasing counter."));
  sysCfg.counters[0] += 1;
}

void counter_update2()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("COUNTER: Received interrupt 2, increasing counter."));
  sysCfg.counters[1] += 1;
}

void counter_update3()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("COUNTER: Received interrupt 3, increasing counter."));
  sysCfg.counters[2] += 1;
}

void counter_update4()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("COUNTER: Received interrupt 4, increasing counter."));
  sysCfg.counters[3] += 1;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void counter_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{ 
  for (byte i = 0; i < 4; i++) {
    if (pin[GPIO_COUNTER1 +i] < 99) {      
      snprintf_P(svalue, ssvalue, PSTR("%s, {\"Counter%d\":%d}"), svalue, i+1, sysCfg.counters[i]);
      *djson = 1;
    }    
  }  
}

#ifdef USE_WEBSERVER
String counter_webPresent()
{
  String page = "";
  char sensor[80];

  for (byte i = 0; i < 4; i++) {
    if (pin[GPIO_COUNTER1 +i] < 99) {      
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_COUNTER, i+1, sysCfg.counters[i]);
      page += sensor;
    }    
  }    

  return page;
}
#endif  // USE_WEBSERVER

#endif // USE_COUNTER
