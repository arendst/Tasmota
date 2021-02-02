/*
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
//
#include "Arduino.h"
#include "lwip/apps/sntp.h"
#include <nvs.h>

#if CONFIG_IDF_TARGET_ESP32
#include <rom/rtc.h>
//#include "esp32/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/rom/rtc.h"
#endif

#include <ESP8266WiFi.h>
#include "esp8266toEsp32.h"

// ESP Stuff
