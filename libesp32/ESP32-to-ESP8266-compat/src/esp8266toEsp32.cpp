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
#include <rom/rtc.h>
#include <ESP8266WiFi.h>
#include "esp8266toEsp32.h"
// ESP Stuff
struct rst_info resetInfo;

String ESP_getResetReason(void)
{
	// CPU 0
	return String(rtc_get_reset_reason(0));
}

String ESP_getResetInfo(void)
{
	return String(PSTR("0"));
}

uint32_t ESP_getBootVersion(void)
{
	return 1;
}

bool ESP_rtcUserMemoryWrite(uint32_t offset, uint32_t *data, size_t size)
{
	return false;
}

bool ESP_rtcUserMemoryRead(uint32_t offset, uint32_t *data, size_t size)
{
	return false;
}

void ESP_reset()
{
	ESP.restart();
}

uint32_t ESP_getFlashChipId()
{
	return 0;
}

uint32_t ESP_getChipId()
{
  uint32_t id = 0;
  for (uint32_t i = 0; i < 17; i = i +8) {
    id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return id;
}

String String_ESP_getChipId()
{
	uint64_t mac = ESP.getEfuseMac();
	return String(uint32_t(mac >> 32)) + String(uint32_t(mac));
}

uint32_t ESP_getFlashChipRealSize()
{
	return ESP.getFlashChipSize();
}

uint32_t ESP_getSketchSize(void)
{
  static uint32_t sketchsize = 0;

  if (!sketchsize) {
    sketchsize = ESP.getSketchSize();  // This takes almost 2 seconds on an ESP32
  }
  return sketchsize;
}
