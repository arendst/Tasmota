/*
  xdrv_24_buzzer.ino - buzzer support for Sonoff-Tasmota

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

#ifdef USE_BUZZER
/*********************************************************************************************\
 * Buzzer support
\*********************************************************************************************/
#define XDRV_24                    24

struct BUZZER {
  uint32_t tune = 0;
  bool active = true;
  bool enable = false;
  uint8_t inverted = 0;            // Buzzer inverted flag (1 = (0 = On, 1 = Off))
  uint8_t count = 0;               // Number of buzzes
  uint8_t set[2];
  uint8_t duration;
  uint8_t state = 0;
} buzzer;

/*********************************************************************************************/

//void BuzzerBeep(uint32_t count = 1, uint32_t on = 1, uint32_t off = 1, uint32_t tune = 0);
void BuzzerBeep(uint32_t count, uint32_t on, uint32_t off, uint32_t tune)
{
  buzzer.set[0] = off;
  buzzer.set[1] = on;
  buzzer.duration = 1;         // Start buzzer on first step
  buzzer.tune = tune;
  if (buzzer.tune) {
    buzzer.count = 1;          // Allow tune only once
  } else {
    buzzer.count = count * 2;  // Start buzzer
  }

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("BUZ: %d,%d,%d,0x%08X"), count, on, off, tune);

  buzzer.enable = true;
}

void BuzzerBeep(uint32_t count) {
  BuzzerBeep(count, 1, 1, 0);
}

void BuzzerEnabledBeep(uint32_t count)
{
  if (Settings.flag3.buzzer_enable) {  // SetOption67
    BuzzerBeep(count);
  }
}

/*********************************************************************************************/

bool BuzzerPinState()
{
  if (XdrvMailbox.index == GPIO_BUZZER_INV) {
    buzzer.inverted = 1;
    XdrvMailbox.index -= (GPIO_BUZZER_INV - GPIO_BUZZER);
    return true;
  }
  return false;
}

void BuzzerInit(void)
{
  if (pin[GPIO_BUZZER] < 99) {
    pinMode(pin[GPIO_BUZZER], OUTPUT);
    digitalWrite(pin[GPIO_BUZZER], buzzer.inverted);  // Buzzer Off
  } else {
    buzzer.active = false;
  }
}

void BuzzerEvery100mSec(void)
{
  if (buzzer.enable) {
    if (buzzer.count) {
      if (buzzer.duration) {
        buzzer.duration--;
        if (!buzzer.duration) {
          if (buzzer.tune) {
            buzzer.state = buzzer.tune & 1;
            buzzer.tune >>= 1;
          } else {
            buzzer.count--;
            buzzer.state = buzzer.count & 1;
          }
          buzzer.duration = buzzer.set[buzzer.state];
        }
      }
      digitalWrite(pin[GPIO_BUZZER], (buzzer.inverted) ? !buzzer.state : buzzer.state);
    } else {
      buzzer.enable = false;
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kBuzzerCommands[] PROGMEM = "|"  // No prefix
  "Buzzer" ;

void (* const BuzzerCommand[])(void) PROGMEM = {
  &CmndBuzzer };

void CmndBuzzer(void)
{
  // Buzzer <number of beeps>,<duration of beep in 100mS steps>,<duration of silence in 100mS steps>
  // All parameters are optional
  //
  // Buzzer             = Buzzer 1,1,1 = Beep once with both duration and pause set to 100mS
  // Buzzer 2           = Beep twice with duration 200mS and pause 100mS
  // Buzzer 2,3         = Beep twice with duration 300mS and pause 100mS
  // Buzzer 2,3,4       = Beep twice with duration 300mS and pause 400mS
  // Buzzer 2,3,4,0x2AF = Beep a sequence once indicated by 0x2AF with duration 300mS and pause 400mS
  //                        resulting in beep ----_-_-_-

  if (XdrvMailbox.data_len > 0) {
    char *p;
    uint32_t i = 0;
    uint32_t parm[4] = { 0 };
    for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 4; str = strtok_r(nullptr, ", ", &p)) {
      parm[i] = strtol(str, nullptr, 0);
      i++;
    }
    for (uint32_t i = 0; i < 4; i++) {
      if (i < 3) {
        if (parm[i] < 1) { parm[i] = 1; }  // Default Count, On time, Off time
      } else {
        if (parm[3] < 0) { parm[3] = 0; }  // Default Tune bitmap
      }
    }
    BuzzerBeep(parm[0], parm[1], parm[2], parm[3]);
  } else {
    BuzzerBeep(1);
  }
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv24(uint8_t function)
{
  bool result = false;

  if (buzzer.active) {
    switch (function) {
      case FUNC_EVERY_100_MSECOND:
        BuzzerEvery100mSec();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kBuzzerCommands, BuzzerCommand);
        break;
      case FUNC_PRE_INIT:
        BuzzerInit();
        break;
      case FUNC_PIN_STATE:
        result = BuzzerPinState();
        break;
    }
  }
  return result;
}

#endif  // USE_BUZZER