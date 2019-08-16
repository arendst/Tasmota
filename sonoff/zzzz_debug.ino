/*
  zzzz_debug.ino - debug support for Sonoff-Tasmota

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

#ifdef USE_DEBUG_DRIVER
/*********************************************************************************************\
 * Virtual debugging support - Part 2
 *
 * Needs to be the last alphabetical file due to DEFINE compile order
\*********************************************************************************************/

bool XsnsPresent(uint8_t sns_index)
{
  uint8_t index = 0;
  for (uint32_t i = 0; i < sizeof(kXsnsList); i++) {
#ifdef XFUNC_PTR_IN_ROM
    index = pgm_read_byte(kXsnsList + i);
#else
    index = kXsnsList[i];
#endif
    if (index == sns_index) { return true; }
  }
  return false;
}

String XsnsGetSensors(void)
{
  char state[2] = { 0 };

  String data = F("[");
  for (uint32_t i = 0; i < MAX_XSNS_DRIVERS; i++) {
    if (i && (!(i % 16))) { data += F(","); }
    if (!(i % 16)) { data += F("\""); }
    state[0] = '-';
    if (XsnsPresent(i)) { state[0] = bitRead(Settings.sensors[i / 32], i % 32) ? '1' : '0'; }
    data += String(state);
    if (i && (!((i +1) % 16))) { data += F("\""); }
  }
  data += F("]");

  return data;
}

#endif  // USE_DEBUG_DRIVER