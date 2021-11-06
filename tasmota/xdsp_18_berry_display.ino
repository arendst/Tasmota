/*
  xdsp_18_berry_display.ino - Interface to virtual display driver with Berry, complements Universal Display with Berry scripting

  Copyright (C) 2021  Stephan Hadinger and Theo Arends

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

#ifdef USE_DISPLAY
#ifdef USE_BERRY

#define XDSP_18                18

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp18(uint8_t function) {
  bool result = false;

  switch (function) {

    case FUNC_DISPLAY_INIT_DRIVER:
      result = callBerryEventDispatcher(PSTR("display"), PSTR("init_driver"), 0, nullptr);
      break;
    case FUNC_DISPLAY_INIT:
      result = callBerryEventDispatcher(PSTR("display"), PSTR("init_driver"), 0, nullptr);
      break;
    case FUNC_DISPLAY_MODEL:
      result = callBerryEventDispatcher(PSTR("display"), PSTR("model"), 0, nullptr);
      break;
    case FUNC_DISPLAY_DIM:
      result = callBerryEventDispatcher(PSTR("display"), PSTR("dim"), GetDisplayDimmer(), nullptr);
      break;
    case FUNC_DISPLAY_POWER:
      result = callBerryEventDispatcher(PSTR("display"), PSTR("power"), disp_power ? GetDisplayDimmer() : 0, nullptr);
      break;
  }
  return result;
}

#endif  // USE_BERRY
#endif  // USE_DISPLAY
