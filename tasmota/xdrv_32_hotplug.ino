/*
  xdrv_32_hotplug.ino - HotPlug support for sensors

  Copyright (C) 2021  Leonid Myravjev

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

#ifdef USE_HOTPLUG
/*********************************************************************************************\
 * HotPlug Support
 *
 * - Rescan bus every N seconds. It send FUNC_HOTPLUG_SCAN event to every sensors.
 * - If HotPlug is 0 or 0xFF -- HotPlug is off
\*********************************************************************************************/

#define XDRV_32              32

const uint32_t HOTPLUG_MAX = 254;  // 0 and 0xFF is OFF

const char kHotPlugCommands[] PROGMEM = "|"  // No prefix
  D_CMND_HOTPLUG;

void (* const HotPlugCommand[])(void) PROGMEM = {
  &CmndHotPlugTime };

struct {
//  uint32_t sleeptime = 0;
  bool     enabled = false;
  uint8_t  timeout = 0;
} Hotplug;

void HotPlugInit(void)
{
  // If empty eeprom is 0xFF by default
  if (Settings.hotplug_scan == 0xFF) { Settings.hotplug_scan = 0; }
  if (Settings.hotplug_scan != 0) {
    Hotplug.enabled = true;
    Hotplug.timeout = 1;  // First scan in a second
  } else
    Hotplug.enabled = false;
}

void HotPlugEverySecond(void)
{
  if (Hotplug.enabled) {
    if (Hotplug.timeout == 0) {
      XsnsCall(FUNC_HOTPLUG_SCAN);
      Hotplug.timeout = Settings.hotplug_scan;
    }
    Hotplug.timeout--;
  }
}

/*********************************************************************************************\
* Commands
\*********************************************************************************************/

void CmndHotPlugTime(void)
{
  if (XdrvMailbox.payload <= HOTPLUG_MAX) {
    Settings.hotplug_scan = XdrvMailbox.payload;
    HotPlugInit();
  }
  ResponseCmndNumber(Settings.hotplug_scan);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv32(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_SECOND:
      HotPlugEverySecond();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kHotPlugCommands, HotPlugCommand);
      break;
    case FUNC_PRE_INIT:
      HotPlugInit();
      break;
  }
  return result;
}

#endif //USE_HOTPLUG
