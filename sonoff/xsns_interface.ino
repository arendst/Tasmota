/*
  xsns_interface.ino - External sensor interface support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends inspired by ESPEasy

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

void XSnsInit()
{
  for (byte i = 0; i < XSNS_MAX; i++) {
    xsns_func_ptr[i] = NULL;
  }
  xsns_present = 0;

#ifdef XSNS_01
  xsns_func_ptr[xsns_present++] = &Xsns01;
#endif

#ifdef XSNS_02
  xsns_func_ptr[xsns_present++] = &Xsns02;
#endif

#ifdef XSNS_03
  xsns_func_ptr[xsns_present++] = &Xsns03;
#endif

#ifdef XSNS_04
  xsns_func_ptr[xsns_present++] = &Xsns04;
#endif

#ifdef XSNS_05
  xsns_func_ptr[xsns_present++] = &Xsns05;
#endif

#ifdef XSNS_06
  xsns_func_ptr[xsns_present++] = &Xsns06;
#endif

#ifdef XSNS_07
  xsns_func_ptr[xsns_present++] = &Xsns07;
#endif

#ifdef XSNS_08
  xsns_func_ptr[xsns_present++] = &Xsns08;
#endif

#ifdef XSNS_09
  xsns_func_ptr[xsns_present++] = &Xsns09;
#endif

#ifdef XSNS_10
  xsns_func_ptr[xsns_present++] = &Xsns10;
#endif

#ifdef XSNS_11
  xsns_func_ptr[xsns_present++] = &Xsns11;
#endif

#ifdef XSNS_12
  xsns_func_ptr[xsns_present++] = &Xsns12;
#endif

#ifdef XSNS_13
  xsns_func_ptr[xsns_present++] = &Xsns13;
#endif

#ifdef XSNS_14
  xsns_func_ptr[xsns_present++] = &Xsns14;
#endif

#ifdef XSNS_15
  xsns_func_ptr[xsns_present++] = &Xsns15;
#endif

#ifdef XSNS_16
  xsns_func_ptr[xsns_present++] = &Xsns16;
#endif

#ifdef XSNS_17
  xsns_func_ptr[xsns_present++] = &Xsns17;
#endif

#ifdef XSNS_18
  xsns_func_ptr[xsns_present++] = &Xsns18;
#endif

#ifdef XSNS_19
  xsns_func_ptr[xsns_present++] = &Xsns19;
#endif

#ifdef XSNS_20
  xsns_func_ptr[xsns_present++] = &Xsns20;
#endif

  XsnsCall(FUNC_XSNS_INIT);
}

/*********************************************************************************************\
 * Function call to all xsns
\*********************************************************************************************/

boolean XsnsCall(byte Function)
{
  boolean result = false;

  switch (Function) {
    case FUNC_XSNS_INIT:
    case FUNC_XSNS_PREP:
    case FUNC_XSNS_JSON_APPEND:
    case FUNC_XSNS_MQTT_SHOW:
    case FUNC_XSNS_WEB:
      for (byte x = 0; x < xsns_present; x++) {
        xsns_func_ptr[x](Function);
      }
      break;
  }

  return result;
}
