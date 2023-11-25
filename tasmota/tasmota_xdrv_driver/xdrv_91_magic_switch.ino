/*
  xdrv_91_magic_switch.ino - Driver for MagicSwitch as implemented in Sonoff BasicR4

  Copyright (C) 2021 Barbudor

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

#ifdef USE_MAGIC_SWITCH

#define XDRV_91           91

/********************************************************************************************************
 * Check defines
 */

#ifndef MAGICSWITCH_MIN_PULSE
#define MAGICSWITCH_MIN_PULSE  12000
#endif

#define MAGICSWITCH_MAX_REASONABLE_PULSE 500000

/********************************************************************************************************
 * Global private data
 */

// MagicSwitch Index => Mode
//
//
//
//  Index Mode  Pull-Up
//    1     0   Pull-Up
//    2     1   No Pull-up
//    3     2   Pull-Up
//    4     3   No Pull-up
//    5     4   Pull-Up
//    6     5   No Pull-up
//    7     6   Pull-Up
//    8     7   No Pull-up

struct MAGICSWITCH_DATA {
  uint32_t    start_time;           // timestamp at rising edge
  uint32_t    pulse_len;            // measured pulse length
  uint32_t    pulse_min;            // minimum pulse length
  uint32_t    switch_state;         // switch state
  uint8_t     pin;                  // the GPIO of the input
  uint8_t     mode;                 // mode
  int8_t      key_offset;           // index of the MagicSwitch in the list of Switches
} *MagicSwitch = nullptr;

#define MAGICSWITCH_MODE_NO_PULLUP       0x01

/********************************************************************************************************
 * Interrupt level operations
 */

extern "C" void IRAM_ATTR MagicSwitch_intr(void *arg) {
  struct MAGICSWITCH_DATA* ms = (struct MAGICSWITCH_DATA*)arg;
  uint32_t now = micros();
  bool state = digitalRead(ms->pin);
  if (state) {
    ms->start_time = now | 1; // "| 1" => avoid 1 in 4M chance to get 0
  } else if (ms->start_time) {
    uint32_t diff = now - ms->start_time;
    if (diff >= ms->pulse_min && diff <= MAGICSWITCH_MAX_REASONABLE_PULSE)
      ms->pulse_len = diff;
  }
}

/********************************************************************************************************
 * Driver initialisation
 */

void MagicSwitchInit(void) {
  if (PinUsed(GPIO_MAGIC_SWITCH, GPIO_ANY)) {
    // allocate data structure
    MagicSwitch = (struct MAGICSWITCH_DATA*)calloc(1, sizeof(struct MAGICSWITCH_DATA));
    if (MagicSwitch) {
      // get pins
      MagicSwitch->pin  = Pin(GPIO_MAGIC_SWITCH, GPIO_ANY);   // input
      MagicSwitch->mode = GetPin(MagicSwitch->pin) - AGPIO(GPIO_MAGIC_SWITCH); // Index 1 => mode 0, etc...
      MagicSwitch->key_offset = -1; // means not yet configured
      MagicSwitch->pulse_min = MAGICSWITCH_MIN_PULSE;

      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MSW: pin:%d, mode:%d"), MagicSwitch->pin, MagicSwitch->mode);

      pinMode(MagicSwitch->pin, (MagicSwitch->mode & MAGICSWITCH_MODE_NO_PULLUP) ? INPUT : INPUT_PULLUP);
      attachInterruptArg(MagicSwitch->pin, MagicSwitch_intr, MagicSwitch, CHANGE);      
    }
  }
}

bool MagicSwitchAddSwitch(void) {
  if (MagicSwitch->key_offset < 0) { 
    MagicSwitch->key_offset = XdrvMailbox.index;
    Settings->switchmode[MagicSwitch->key_offset] = 4;
    AddLog(LOG_LEVEL_INFO, PSTR("MSW: MagicSwitch is Switch %d"), MagicSwitch->key_offset + 1);
    return true;
  }
  return false;
}

/********************************************************************************************************
 * Driver operations
 */

void MagicSwitchLoop()
{
  uint32_t pulse_len = MagicSwitch->pulse_len;
  MagicSwitch->pulse_len = 0;
  if (pulse_len) {
    MagicSwitch->switch_state = 1;
    SwitchSetVirtualPinState(MagicSwitch->key_offset, MagicSwitch->switch_state);
    AddLog(LOG_LEVEL_DEBUG, PSTR("MSW: pulse length:%d"), pulse_len);
  } else if (MagicSwitch->switch_state) {
    MagicSwitch->switch_state = 0;
    SwitchSetVirtualPinState(MagicSwitch->key_offset, MagicSwitch->switch_state);
  }
}

/********************************************************************************************************
 * Commands
 */

const char kMagicSwitchCommands[] PROGMEM = "MagicSwitch|" 
  "Pulse"
  ;

void (* const MagicSwitchCommand[])(void) PROGMEM = {
  &CmndMagicSwitchPulse
  };

void CmndMagicSwitchPulse(void)
{
  if ((XdrvMailbox.payload >= 1000) && (XdrvMailbox.payload < MAGICSWITCH_MAX_REASONABLE_PULSE)) {
    MagicSwitch->pulse_min = XdrvMailbox.payload;
  }
  ResponseCmndNumber(MagicSwitch->pulse_min);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv91(uint32_t function) {
  bool result = false;

  if (FUNC_SETUP_RING2 == function) {
    MagicSwitchInit();
  } else if (MagicSwitch) {
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
      //case FUNC_EVERY_250_MSECOND:
        MagicSwitchLoop();
        break;
      case FUNC_ADD_SWITCH:
        result = MagicSwitchAddSwitch();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kMagicSwitchCommands, MagicSwitchCommand);
        break;
    }
  }
  return result;
}

#endif // USE_MAGIC_SWITCH
