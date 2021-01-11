/*
  xdsp_interface.ino - Display interface support for Tasmota

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

#if defined(USE_I2C) || defined(USE_SPI)
#ifdef USE_DISPLAY

#ifdef XFUNC_PTR_IN_ROM
bool (* const xdsp_func_ptr[])(uint8_t) PROGMEM = {   // Display Function Pointers
#else
bool (* const xdsp_func_ptr[])(uint8_t) = {   // Display Function Pointers
#endif

#ifdef XDSP_01
  &Xdsp01,
#endif

#ifdef XDSP_02
  &Xdsp02,
#endif

#ifdef XDSP_03
  &Xdsp03,
#endif

#ifdef XDSP_04
  &Xdsp04,
#endif

#ifdef XDSP_05
  &Xdsp05,
#endif

#ifdef XDSP_06
  &Xdsp06,
#endif

#ifdef XDSP_07
  &Xdsp07,
#endif

#ifdef XDSP_08
  &Xdsp08,
#endif

#ifdef XDSP_09
  &Xdsp09,
#endif

#ifdef XDSP_10
  &Xdsp10,
#endif

#ifdef XDSP_11
  &Xdsp11,
#endif

#ifdef XDSP_12
  &Xdsp12,
#endif

#ifdef XDSP_13
  &Xdsp13,
#endif

#ifdef XDSP_14
  &Xdsp14,
#endif

#ifdef XDSP_15
  &Xdsp15,
#endif

#ifdef XDSP_16
  &Xdsp16
#endif
};

const uint8_t xdsp_present = sizeof(xdsp_func_ptr) / sizeof(xdsp_func_ptr[0]);  // Number of drivers found

/*********************************************************************************************\
 * Function call to all xdsp
 *
 * FUNC_DISPLAY_INIT_DRIVER
 * FUNC_DISPLAY_INIT
 * FUNC_DISPLAY_EVERY_50_MSECOND
 * FUNC_DISPLAY_EVERY_SECOND
 * FUNC_DISPLAY_POWER
 * FUNC_DISPLAY_CLEAR
 * FUNC_DISPLAY_DRAW_FRAME
 * FUNC_DISPLAY_DRAW_HLINE
 * FUNC_DISPLAY_DRAW_VLINE
 * FUNC_DISPLAY_DRAW_CIRCLE
 * FUNC_DISPLAY_FILL_CIRCLE
 * FUNC_DISPLAY_DRAW_RECTANGLE
 * FUNC_DISPLAY_FILL_RECTANGLE
 * FUNC_DISPLAY_FONT_SIZE
 * FUNC_DISPLAY_ROTATION
 * FUNC_DISPLAY_DRAW_STRING
\*********************************************************************************************/

uint8_t XdspPresent(void)
{
  return xdsp_present;
}

bool XdspCall(uint8_t Function)
{
  bool result = false;

  DEBUG_TRACE_LOG(PSTR("DSP: %d"), Function);

  for (uint32_t x = 0; x < xdsp_present; x++) {
    result = xdsp_func_ptr[x](Function);

    if (result && (FUNC_DISPLAY_MODEL == Function)) {
      break;
    }
  }

  return result;
}

#endif  // USE_DISPLAY
#endif  // USE_I2C or USE_SPI
