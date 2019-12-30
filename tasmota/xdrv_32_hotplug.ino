/*
  xdrv_32_hotplug.ino - HotPlug support for sensors

  Copyright (C) 2019  Leonid Myravjev

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
 *
 * See wiki https://github.com/arendst/Tasmota/wiki/HotPlug FIXME
\*********************************************************************************************/

#define XDRV_32                32

#define D_PRFX_HOTPLUG "HotPlug"

const uint32_t HOTPLUG_MAX = 254; // 0 and 0xFF is OFF

const char kHotPlugCommands[] PROGMEM = "|" D_PRFX_HOTPLUG;
void (* const HotPlugCommand[])(void) PROGMEM = { &CmndHotPlugTime };

uint32_t hotplug_sleeptime = 0;
bool     hotplug_enabled = false;
uint8_t  hotplug_timeout = 0;

void HotPlugInit(void)
{
  // If empty eeprom is 0xFF by default
  if (Settings.hotplug_scan == 0xFF) Settings.hotplug_scan = 0;
  if (Settings.hotplug_scan != 0) {
    hotplug_enabled = true;
    hotplug_timeout = 1; // first scan in a second
  } else
    hotplug_enabled = false;
}

void HotPlugEverySecond(void)
{
  if (hotplug_enabled) {
    if (hotplug_timeout == 0) {
      XsnsCall(FUNC_HOTPLUG_SCAN);
      hotplug_timeout = Settings.hotplug_scan;
    }
    hotplug_timeout--;
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
  Response_P(S_JSON_COMMAND_NVALUE, XdrvMailbox.command, Settings.hotplug_scan);
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
