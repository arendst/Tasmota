/*
  xdrv_20_lc_tech_relay.ino - LC Technology relay support for Sonoff-Tasmota

  Copyright (C) 2018 Oliver Welter

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

#ifdef USE_LC_TECH_RELAY
/*********************************************************************************************\
 * This code can be used for LC Technology relay boards.
 * They contain a socket for ESP01 boards and have 1, 2 or 4 channels.
 * The boards are controlled via serial commands and do not make use of GPIO switching
 * functionality. For some 1 channel boards, the 5V input line from the board must be wired
 * to 5V input of the ESP01 device. Dont know, why they missed that on some PCB's.
\*********************************************************************************************/

#define XDRV_20                20

const uint8_t LCTSeqBase[5]  = { 0xA0, 0x00, 0x00, 0xA0, 0x0A };

uint8_t LCTNumDevs = 0;
uint32_t LCTBaudrate = 0;
boolean LCTInitialized = false;

/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

boolean LCTSetRelay(void)
{
  power_t rpower = XdrvMailbox.index;

  return LCTSetStates(rpower);
}

boolean LCTSetStates(power_t rpower)
{
  uint8_t state;

  for (byte i = 0; i < LCTNumDevs; i++)
  {
    state = rpower &1;

    if (i < MAX_RELAYS)
    {
      LCTRelayBoardSwitch(i, state);
    }

    rpower >>= 1;
  }

  return true;
}

boolean LCTRelayBoardSwitch(uint8_t id, boolean nc)
{
  if (LCTInitialized == false)
  {
    return false;
  }

  SetSeriallog(LOG_LEVEL_NONE); // Important, if someone changes LOG_LEVEL to something nastier than NONE.
  SetSerialBaudrate(LCTBaudrate);

  uint8_t LCTByteBegin    = LCTSeqBase[0];
  uint8_t LCTByteDevID    = LCTSeqBase[1] + ((id & 0xff) + 1);
  uint8_t LCTByteDevState = LCTSeqBase[2] + (uint8_t)nc;
  uint8_t LCTByteEnd      = LCTSeqBase[3] + ((id & 0xff) + 3) - ((uint8_t)nc ? 1 : 2);
  uint8_t LCTByteReset    = LCTSeqBase[4];

  char buffer[5] = {
    LCTByteBegin,
    LCTByteDevID,
    LCTByteDevState,
    LCTByteEnd,
    LCTByteReset
  };

  SerialSendRaw(buffer);

  snprintf_P(log_data, sizeof(log_data), PSTR( "LCT: Sent new serial state to relay %d (state: %d): 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x"), id, nc ? 1 : 0, LCTByteBegin, LCTByteDevID, LCTByteDevState, LCTByteEnd, LCTByteReset);
  AddLog(LOG_LEVEL_DEBUG);

  return true;
}

/*********************************************************************************************\
 * API Functions
\*********************************************************************************************/

boolean LCTModuleSelected(void)
{
  devices_present = LCTNumDevs;

  return true;
}

void LCTInit(void)
{
  power_t rpower = Settings.power;

  if (!LCTInitialized)
  {
    suppress_serial_logging = true;

    ClaimSerial();

    LCTInitialized = true;
    LCTInitialized = LCTSetStates(rpower);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdrv20(byte function)
{
  boolean result = false;

  switch(Settings.module)
  {
      case LC_TECH_RELAY1:
        LCTNumDevs = 1;
        LCTBaudrate = 9600;
        break;
      case LC_TECH_RELAY2:
        LCTNumDevs = 2;
        LCTBaudrate = 115200;
        break;
      case LC_TECH_RELAY4:
        LCTNumDevs = 4;
        LCTBaudrate = 115200;
        break;
      default:
        LCTNumDevs = 0;
        break;
  }

  if (LCTNumDevs > 0)
  {
    switch (function)
    {
      case FUNC_MODULE_INIT:
        result = LCTModuleSelected();
        break;
      case FUNC_INIT:
        LCTInit();
        break;
      case FUNC_SET_DEVICE_POWER:
        result = LCTSetRelay();
        break;
    }
  }

  return result;
}

#endif  // USE_LC_TECH_RELAY
