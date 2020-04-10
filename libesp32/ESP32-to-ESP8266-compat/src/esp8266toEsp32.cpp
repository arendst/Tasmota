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

String ESP_getBootVersion(void)
{
	return String(PSTR("Unknown"));
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

String String_ESP_getChipId()
{
	uint64_t mac = ESP.getEfuseMac();
	return String(uint32_t(mac >> 32)) + String(uint32_t(mac));
}

/*
uint64_t ESP_getChipId() 
{
	return ESP.getEfuseMac();
}
*/
