/*
  xsns_01_counter.ino - Counter sensors (water meters, electricity meters etc.) sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Maarten Damen and Theo Arends

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

#define XSNS_01             1

unsigned long last_counter_timer[MAX_COUNTERS]; // Last counter time in micro seconds

#ifndef ARDUINO_ESP8266_RELEASE_2_3_0       // Fix core 2.5.x ISR not in IRAM Exception
void CounterUpdate(uint8_t index) ICACHE_RAM_ATTR;
void CounterUpdate1(void) ICACHE_RAM_ATTR;
void CounterUpdate2(void) ICACHE_RAM_ATTR;
void CounterUpdate3(void) ICACHE_RAM_ATTR;
void CounterUpdate4(void) ICACHE_RAM_ATTR;
#endif  // ARDUINO_ESP8266_RELEASE_2_3_0

void CounterUpdate(uint8_t index)
{
  unsigned long counter_debounce_time = micros() - last_counter_timer[index -1];
  if (counter_debounce_time > Settings.pulse_counter_debounce * 1000) {
    last_counter_timer[index -1] = micros();
    if (bitRead(Settings.pulse_counter_type, index -1)) {
      RtcSettings.pulse_counter[index -1] = counter_debounce_time;
    } else {
      RtcSettings.pulse_counter[index -1]++;
    }

//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("CNTR: Interrupt %d"), index);
  }
}

void CounterUpdate1(void)
{
  CounterUpdate(1);
}

void CounterUpdate2(void)
{
  CounterUpdate(2);
}

void CounterUpdate3(void)
{
  CounterUpdate(3);
}

void CounterUpdate4(void)
{
  CounterUpdate(4);
}

/********************************************************************************************/

void CounterSaveState(void)
{
  for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    if (pin[GPIO_CNTR1 +i] < 99) {
      Settings.pulse_counter[i] = RtcSettings.pulse_counter[i];
    }
  }
}

void CounterInit(void)
{
  typedef void (*function) () ;
  function counter_callbacks[] = { CounterUpdate1, CounterUpdate2, CounterUpdate3, CounterUpdate4 };

  for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    if (pin[GPIO_CNTR1 +i] < 99) {
      pinMode(pin[GPIO_CNTR1 +i], bitRead(counter_no_pullup, i) ? INPUT : INPUT_PULLUP);
      attachInterrupt(pin[GPIO_CNTR1 +i], counter_callbacks[i], FALLING);
// STB mode
      //avoid DIV 0 on unitiialized
      if (Settings.pulse_devider[i] == 0 || Settings.pulse_devider[i] == 65535 ) {
        Settings.pulse_devider[i] = COUNTERDEVIDER;
      }
// end
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_COUNTER[] PROGMEM =
  "{s}" D_COUNTER "%d{m}%s%s{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void CounterShow(bool json)
{
  char stemp[10];

  uint8_t dsxflg = 0;
  uint8_t header = 0;
  for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    if (pin[GPIO_CNTR1 +i] < 99) {
      char counter[33];
      if (bitRead(Settings.pulse_counter_type, i)) {
        dtostrfd((double)RtcSettings.pulse_counter[i] / 1000000, 6, counter);
      } else {
        dsxflg++;
	//STB mod
	dtostrfd(RtcSettings.pulse_counter[i]/Settings.pulse_devider[i], 0, counter);
	//end
      }

      if (json) {
        if (!header) {
          ResponseAppend_P(PSTR(",\"COUNTER\":{"));
          stemp[0] = '\0';
        }
        header++;
        ResponseAppend_P(PSTR("%s\"C%d\":%s"), stemp, i +1, counter);
        strlcpy(stemp, ",", sizeof(stemp));
#ifdef USE_DOMOTICZ
        if ((0 == tele_period) && (1 == dsxflg)) {

	  //STB mod
          DomoticzSensor(DZ_COUNT, RtcSettings.pulse_counter[i]/Settings.pulse_devider[i]);
	  //end
          dsxflg++;
        }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_COUNTER, i +1, counter, (bitRead(Settings.pulse_counter_type, i)) ? " " D_UNIT_SECOND : "");
#endif  // USE_WEBSERVER
      }
    }
    if (bitRead(Settings.pulse_counter_type, i)) {
      RtcSettings.pulse_counter[i] = 0xFFFFFFFF;  // Set Timer to max in case of no more interrupts due to stall of measured device
    }
  }
  if (json) {
    if (header) {
      ResponseJsonEnd();
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/
//stb mod
enum CounterCommands { CMND_COUNTER, CMND_COUNTERTYPE, CMND_COUNTERDEBOUNCE, CMND_COUNTERDEVIDER };
const char kCounterCommands[] PROGMEM = D_CMND_COUNTER "|" D_CMND_COUNTERTYPE "|" D_CMND_COUNTERDEBOUNCE "|" D_CMND_COUNTERDEVIDER ;
//end
bool CounterCommand(void)
{
  char command[CMDSZ];
  bool serviced = true;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kCounterCommands);
  if (CMND_COUNTER == command_code) {
    if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_COUNTERS)) {
      if ((XdrvMailbox.payload >= 0) && (pin[GPIO_CNTR1 + XdrvMailbox.index -1] < 99)) {
//STB mode
        Settings.pulse_devider[XdrvMailbox.index-1] = Settings.pulse_devider[XdrvMailbox.index-1] == 0 ? COUNTERDEVIDER : Settings.pulse_devider[XdrvMailbox.index-1];
        if ((XdrvMailbox.data[0] == '-') || (XdrvMailbox.data[0] == '+')) {
          RtcSettings.pulse_counter[XdrvMailbox.index-1] += XdrvMailbox.payload * Settings.pulse_devider[XdrvMailbox.index-1];
          Settings.pulse_counter[XdrvMailbox.index-1] += XdrvMailbox.payload * Settings.pulse_devider[XdrvMailbox.index-1];
        } else {
          RtcSettings.pulse_counter[XdrvMailbox.index-1] = XdrvMailbox.payload * Settings.pulse_devider[XdrvMailbox.index-1];
          Settings.pulse_counter[XdrvMailbox.index -1] = XdrvMailbox.payload * Settings.pulse_devider[XdrvMailbox.index-1];
        }
      }
      Response_P(S_JSON_COMMAND_INDEX_LVALUE, command, XdrvMailbox.index, RtcSettings.pulse_counter[XdrvMailbox.index -1]/Settings.pulse_devider[XdrvMailbox.index -1]);
//end
    }
  }
  else if (CMND_COUNTERTYPE == command_code) {
    if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_COUNTERS)) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1) && (pin[GPIO_CNTR1 + XdrvMailbox.index -1] < 99)) {
        bitWrite(Settings.pulse_counter_type, XdrvMailbox.index -1, XdrvMailbox.payload &1);
        RtcSettings.pulse_counter[XdrvMailbox.index -1] = 0;
        Settings.pulse_counter[XdrvMailbox.index -1] = 0;
      }
      Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, bitRead(Settings.pulse_counter_type, XdrvMailbox.index -1));
    }
  }
  else if (CMND_COUNTERDEBOUNCE == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32001)) {
      Settings.pulse_counter_debounce = XdrvMailbox.payload;
    }
    Response_P(S_JSON_COMMAND_NVALUE, command, Settings.pulse_counter_debounce);
  }
//stb mod
   else if ((CMND_COUNTERDEVIDER == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_COUNTERS)) {
      if (XdrvMailbox.payload >= 0) {
        unsigned long _counter;
        Settings.pulse_devider[XdrvMailbox.index -1] = Settings.pulse_devider[XdrvMailbox.index -1] == 0 ? COUNTERDEVIDER : Settings.pulse_devider[XdrvMailbox.index -1];
        _counter = RtcSettings.pulse_counter[XdrvMailbox.index -1]/Settings.pulse_devider[XdrvMailbox.index -1];
        Settings.pulse_devider[XdrvMailbox.index -1] = XdrvMailbox.payload;
        RtcSettings.pulse_counter[XdrvMailbox.index -1] = _counter * Settings.pulse_devider[XdrvMailbox.index -1];
      }
      Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, Settings.pulse_devider[XdrvMailbox.index -1]);
    }
 //end
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns01(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      CounterInit();
      break;
    case FUNC_JSON_APPEND:
      CounterShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      CounterShow(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_SAVE_BEFORE_RESTART:
    case FUNC_SAVE_AT_MIDNIGHT:
      CounterSaveState();
      break;
    case FUNC_COMMAND:
      result = CounterCommand();
      break;
  }
  return result;
}

#endif  // USE_COUNTER
