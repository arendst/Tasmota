/*
  xdrv_24_Buzzer.ino - buzzer support for Tasmota

  Copyright (C) 2020  Theo Arends

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
} Buzzer;

/*********************************************************************************************/

void BuzzerOff(void)
{
  DigitalWrite(GPIO_BUZZER, Buzzer.inverted);  // Buzzer Off
}

//void BuzzerBeep(uint32_t count = 1, uint32_t on = 1, uint32_t off = 1, uint32_t tune = 0);
void BuzzerBeep(uint32_t count, uint32_t on, uint32_t off, uint32_t tune)
{
  Buzzer.set[0] = off;         // off duration in 100 mSec steps
  Buzzer.set[1] = on;          // on duration in 100 mSec steps
  Buzzer.duration = 1;         // Start buzzer on first step
  Buzzer.tune = 0;
  if (tune) {
    uint32_t tune1 = tune;
    uint32_t tune2 = tune;
    for (uint32_t i = 0; i < 32; i++) {
      if (!(tune2 & 0x80000000)) {
        tune2 <<= 1;           // Skip leading silence
      } else {
        Buzzer.tune <<= 1;     // Add swapped tune
        Buzzer.tune |= tune1 & 1;
        tune1 >>= 1;
      }
    }
    Buzzer.count = 1;          // Allow tune only once
  } else {
    Buzzer.count = count * 2;  // Start buzzer
  }

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("BUZ: %d(%d),%d,%d,0x%08X(0x%08X)"), count, Buzzer.count, on, off, tune, Buzzer.tune);

  Buzzer.enable = (Buzzer.count > 0);
  if (!Buzzer.enable) {
    BuzzerOff();
  }
}

void BuzzerBeep(uint32_t count) {
  BuzzerBeep(count, 1, 1, 0);
}

void BuzzerEnabledBeep(uint32_t count, uint32_t duration)
{
  if (Settings.flag3.buzzer_enable) {  // SetOption67 - Enable buzzer when available
    BuzzerBeep(count, duration, 1, 0);
  }
}

/*********************************************************************************************/

bool BuzzerPinState(void)
{
  if (XdrvMailbox.index == GPIO_BUZZER_INV) {
    Buzzer.inverted = 1;
    XdrvMailbox.index -= (GPIO_BUZZER_INV - GPIO_BUZZER);
    return true;
  }
  return false;
}

void BuzzerInit(void)
{
  if (pin[GPIO_BUZZER] < 99) {
    pinMode(pin[GPIO_BUZZER], OUTPUT);
    BuzzerOff();
  } else {
    Buzzer.active = false;
  }
}

void BuzzerEvery100mSec(void)
{
  if (Buzzer.enable) {
    if (Buzzer.count) {
      if (Buzzer.duration) {
        Buzzer.duration--;
        if (!Buzzer.duration) {
          if (Buzzer.tune) {
            Buzzer.state = Buzzer.tune & 1;
            Buzzer.tune >>= 1;
          } else {
            Buzzer.count--;
            Buzzer.state = Buzzer.count & 1;
          }
          Buzzer.duration = Buzzer.set[Buzzer.state];
        }
      }
      DigitalWrite(GPIO_BUZZER, (Buzzer.inverted) ? !Buzzer.state : Buzzer.state);
    } else {
      Buzzer.enable = false;
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
  // Buzzer <number of beeps>,<duration of beep in 100mS steps>,<duration of silence in 100mS steps>,<tune>
  // All parameters are optional
  //
  // Buzzer             = Buzzer 1,1,1 = Beep once with both duration and pause set to 100mS
  // Buzzer 0           = Stop active beep cycle
  // Buzzer 2           = Beep twice with duration 200mS and pause 100mS
  // Buzzer 2,3         = Beep twice with duration 300mS and pause 100mS
  // Buzzer 2,3,4       = Beep twice with duration 300mS and pause 400mS
  // Buzzer 2,3,4,0xF54 = Beep a sequence once indicated by 0xF54 = 1111 0101 01 with duration 300mS and pause 400mS

  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.payload > 0) {
      char *p;
      uint32_t i = 0;
      uint32_t parm[4] = { 0 };
      for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 4; str = strtok_r(nullptr, ", ", &p)) {
        parm[i] = strtoul(str, nullptr, 0);
        i++;
      }
      for (uint32_t i = 0; i < 3; i++) {
        if (parm[i] < 1) { parm[i] = 1; }  // Default Count, On time, Off time
      }
      BuzzerBeep(parm[0], parm[1], parm[2], parm[3]);
    } else {
      BuzzerBeep(0);
    }
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

  if (Buzzer.active) {
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