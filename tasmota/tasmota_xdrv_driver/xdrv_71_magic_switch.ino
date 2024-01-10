/*
  xdrv_71_magic_switch.ino - Driver for MagicSwitch as implemented in Sonoff BasicR4

  Copyright (C) 2023 Barbudor

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

#define XDRV_71           71

/********************************************************************************************************
 * Check defines
 */

#ifndef MAGICSWITCH_MIN_PULSE
#define MAGICSWITCH_MIN_PULSE  4000
#endif

#ifndef MAGICSWITCH_MASKING_WINDOW_LEN
#define MAGICSWITCH_MASKING_WINDOW_LEN  5
#endif

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

#define MAGICSWITCH_MODE_NO_PULLUP    0x01


struct MAGICSWITCH_DATA {
  uint32_t    start_time;           // timestamp at rising edge
  uint32_t    pulse_len;            // measured pulse length
  uint32_t    min_pulse;            // minimum pulse length
  uint8_t     switch_state;         // switch state - count down for masking window
  uint8_t     pin;                  // the GPIO of the input
  uint8_t     mode;                 // mode
  int8_t      key_offset;           // index of the MagicSwitch in the list of Switches
} *MagicSwitch = nullptr;


/********************************************************************************************************
 * Interrupt level operations
 */

extern "C" void IRAM_ATTR MagicSwitch_intr(void *arg) {
  struct MAGICSWITCH_DATA* ms = (struct MAGICSWITCH_DATA*)arg;
  uint32_t now = micros();
  bool pin_state = digitalRead(ms->pin);
  if (ms->pulse_len) {            // previous pulse not aknowledged, ignoring the edge
    return;                   
  } else if (pin_state) {         // rising edge (hopefully if we didn't missed), record start time
    ms->start_time = now | 1;     // "| 1" => avoid 1 in 4M chance to get 0
  } else if (ms->start_time) {    // falling edge, compute pulse length
    uint32_t pulse_len = now - ms->start_time;
    ms->start_time = 0;           // in case of missing an rising edge, avoid triggering false detection
    if (pulse_len >= ms->min_pulse)
      ms->pulse_len = pulse_len;
  }
}

/********************************************************************************************************
 * Driver operations
 */

void MagicSwitchLoop()
{
  if (MagicSwitch->switch_state) {
    MagicSwitch->switch_state--;
    if (!MagicSwitch->switch_state) {
      SwitchSetVirtualPinState(MagicSwitch->key_offset, 0);
      MagicSwitch->pulse_len = 0;       // acknowledge the pulse, close the masking window
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MSW: end"));
    }
  } else if (MagicSwitch->pulse_len) {
    SwitchSetVirtualPinState(MagicSwitch->key_offset, 1);
    MagicSwitch->switch_state = MAGICSWITCH_MASKING_WINDOW_LEN;
    AddLog(LOG_LEVEL_DEBUG, PSTR("MSW: length:%d, window:%d"), MagicSwitch->pulse_len, MagicSwitch->switch_state);
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
      MagicSwitch->min_pulse = MAGICSWITCH_MIN_PULSE;

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
    AddLog(LOG_LEVEL_INFO, PSTR("MSW: Switch %d, Switchmode 4"), MagicSwitch->key_offset + 1);
    return true;
  }
  return false;
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
  if ((XdrvMailbox.payload >= 1000) && (XdrvMailbox.payload < 500000)) {
    MagicSwitch->min_pulse = XdrvMailbox.payload;
  }
  ResponseCmndNumber(MagicSwitch->min_pulse);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv71(uint32_t function) {
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
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif // USE_MAGIC_SWITCH
