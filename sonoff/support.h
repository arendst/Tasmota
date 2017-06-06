/*
  support.h - parameters used by platformio for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include "user_interface.h"

/* Function prototypes. */
void WIFI_wps_status_cb(wps_cb_status status);

#endif // ifndef __SUPPORT_H__

#ifdef __cplusplus
}
#endif
