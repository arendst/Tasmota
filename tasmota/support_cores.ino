/*
  support_cores.ino - Arduino core support for Tasmota

  Copyright (C) 2021  Theo Arends

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

/*********************************************************************************************\
 * Core overrides
\*********************************************************************************************/

// Add below line to tasmota_globals.h
// extern "C" void resetPins();
void resetPins()
{
/*
  for (int i = 0; i <= 5; ++i) {
    pinMode(i, INPUT);
  }
  // pins 6-11 are used for the SPI flash interface
  for (int i = 12; i <= 16; ++i) {
    pinMode(i, INPUT);
  }
*/
}

/*********************************************************************************************\
 * Hardware related
\*********************************************************************************************/

#ifdef ESP8266

void HwWdtDisable(void) {
  *((volatile uint32_t*) 0x60000900) &= ~(1);  // Hardware WDT OFF
}

void HwWdtEnable(void) {
  *((volatile uint32_t*) 0x60000900) |= 1;     // Hardware WDT ON
}

void WdtDisable(void) {
  ESP.wdtDisable();
  HwWdtDisable();
}

void WdtEnable(void) {
  HwWdtEnable();
  ESP.wdtEnable(0);
}

#endif  // ESP8266
