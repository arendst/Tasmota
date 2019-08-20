/*
  xdrv_25_dali.ino - DALI support for Sonoff-Tasmota

  Copyright (C) 2019  Stefan Agner

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

#ifdef USE_DALI
/*********************************************************************************************\
 * DALI (Digital Addressable Lighting Interface) support
\*********************************************************************************************/

#include <Dali.h>

#define XDRV_25                    25

enum ExecuteCommandDaliOptions { DALI_OFF, DALI_ON, DALI_BUS_TEST = 9 };

struct DALI {
  bool active = true;
} Dali;

/*********************************************************************************************/

/*********************************************************************************************/

void DaliInit(void)
{
  if (pin[GPIO_DALI] < 99) {
    dali.setupTransmit(pin[GPIO_DALI]); //setup digital pin to transmit commands
    AddLog_P2(LOG_LEVEL_INFO, PSTR("DALI: active on pin %d"), pin[GPIO_DALI]);
    dali.msgMode = true;
    //dali.busTest();
    devices_present++;
  } else {
    Dali.active = false;
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kDaliCommands[] PROGMEM = "|"  // No prefix
  "DALI" "|" "DALIDimmer" ;

void (* const DaliCommand[])(void) PROGMEM = {
  &CmndDali, &CmndDaliDimmer };

void CmndDali(void)
{

  // DALI commands
  //
  // DALI off or 0    = DALI broadcast all off
  // DALI on or 1     = DALI broadcast all on
  // DALI 9           = Bus Test
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= devices_present)) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("DALI: idx %d, payload %d"), XdrvMailbox.index, XdrvMailbox.payload);
    switch (XdrvMailbox.payload) {
    case DALI_OFF:
      dali.transmit(BROADCAST_C, OFF_C);
      break;
    case DALI_ON:
      dali.transmit(BROADCAST_C, ON_C);
      break;
    case DALI_BUS_TEST:
      dali.transmit(BROADCAST_C, ON_C);
      break;
    }
  }

  ResponseCmndDone();
}

void CmndDaliDimmer(void)
{

  // DALI commands
  //
  // DALI 0..254     = DALI brightness control
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= devices_present)) {
    if (XdrvMailbox.payload > 254) { XdrvMailbox.payload = 254; }
    AddLog_P2(LOG_LEVEL_INFO, PSTR("DALI: idx %d, brightness %d"), XdrvMailbox.index, XdrvMailbox.payload);
    dali.transmit(BROADCAST_DP, XdrvMailbox.payload);
  }

  ResponseCmndDone();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv25(uint8_t function)
{
  bool result = false;

  if (Dali.active) {
    switch (function) {
      case FUNC_COMMAND:
        result = DecodeCommand(kDaliCommands, DaliCommand);
        break;
      case FUNC_PRE_INIT:
        DaliInit();
        break;
    }
  }
  return result;
}

#endif  // USE_DALI
