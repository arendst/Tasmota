/*
  xdrv_23_zigbee_7_8_default_plugin.ino - default plugin stored in Flash

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE

/********************************************************************
** Default plugin
** 
** Below is a the template loaded at boot
** We simulate a read-only file from the filesystem
********************************************************************/

const char Z_DEF_PLUGIN[] PROGMEM =
  "#Z2Tv1"                                      "\n"
  
  // bug in IKEA remotes battery, need to double the value
  ":TRADFRI*,"                                  "\n"
  ":SYMFONISK*,"                                "\n"
  "0001/0021=0001/0021,mul:2"                   "\n"    // BatteryPercentage

  // "Power" for lumi Door/Window is converted to "Contact"
  ":lumi.sensor_magnet*,"                       "\n"
  "0006/0000=0500/FFF2"                         "\n"    // 0xFFF0 + ZA_Contact
;

/********************************************************************
** Load from flash
********************************************************************/
void ZbAutoLoadFromFlash(void) {
  FlashFileImplPtr fp = FlashFileImplPtr(new FlashFileImpl(Z_DEF_PLUGIN));
  File f = File(fp);
  if (ZbLoad_inner(PSTR("<internal_plugin>"), f)) {
    AddLog(LOG_LEVEL_INFO, "ZIG: ZbLoad '%s' loaded successfully", PSTR("<internal_plugin>"));
  }
}

#endif // USE_ZIGBEE
