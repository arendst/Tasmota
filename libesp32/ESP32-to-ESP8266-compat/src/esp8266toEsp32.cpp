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

String ESP32GetResetReason(uint32_t cpu_no)
{
	// tools\sdk\include\esp32\rom\rtc.h
  switch (rtc_get_reset_reason( (RESET_REASON) cpu_no)) {
    case POWERON_RESET          : return F("Vbat power on reset");                              // 1
    case SW_RESET               : return F("Software reset digital core");                      // 3
    case OWDT_RESET             : return F("Legacy watch dog reset digital core");              // 4
    case DEEPSLEEP_RESET        : return F("Deep Sleep reset digital core");                    // 5
    case SDIO_RESET             : return F("Reset by SLC module, reset digital core");          // 6
    case TG0WDT_SYS_RESET       : return F("Timer Group0 Watch dog reset digital core");        // 7
    case TG1WDT_SYS_RESET       : return F("Timer Group1 Watch dog reset digital core");        // 8
    case RTCWDT_SYS_RESET       : return F("RTC Watch dog Reset digital core");                 // 9
    case INTRUSION_RESET        : return F("Instrusion tested to reset CPU");                   // 10
    case TGWDT_CPU_RESET        : return F("Time Group reset CPU");                             // 11
    case SW_CPU_RESET           : return F("Software reset CPU");                               // 12
    case RTCWDT_CPU_RESET       : return F("RTC Watch dog Reset CPU");                          // 13
    case EXT_CPU_RESET          : return F("or APP CPU, reseted by PRO CPU");                   // 14
    case RTCWDT_BROWN_OUT_RESET : return F("Reset when the vdd voltage is not stable");         // 15
    case RTCWDT_RTC_RESET       : return F("RTC Watch dog reset digital core and rtc module");  // 16
    default                     : return F("NO_MEAN");                                          // 0
  }
}

String ESP_getResetReason(void)
{
  return ESP32GetResetReason(0);  // CPU 0
}

uint32_t ESP_ResetInfoReason(void)
{
  RESET_REASON reason = rtc_get_reset_reason(0);
  if (POWERON_RESET == reason) { return REASON_DEFAULT_RST; }
  if (SW_CPU_RESET == reason) { return REASON_SOFT_RESTART; }
  if (DEEPSLEEP_RESET == reason)  { return REASON_DEEP_SLEEP_AWAKE; }
  if (SW_RESET == reason) { return REASON_EXT_SYS_RST; }
}

uint32_t ESP_getBootVersion(void)
{
	return 1;
}

// ESP32 RTC memory is kept ONLY on deep_sleep wake. Any other restart will erase RTC memory
// Current solution is using NVS hopig it survives many writes ;-)
bool ESP_rtcUserMemoryWrite(uint32_t offset, uint32_t *data, size_t size)
{
  char sName[16];
  snprintf_P(sName, sizeof(sName), PSTR("rtc%d"), offset);

  nvs_handle handle;
  noInterrupts();
  nvs_open("tasrtc", NVS_READWRITE, &handle);
  nvs_set_blob(handle, sName, data, size);
  nvs_commit(handle);
  nvs_close(handle);
  interrupts();

  return true;
}

// ESP32 RTC memory is kept ONLY on deep_sleep wake. Any other restart will erase RTC memory
// Current solution is using NVS hopig it survives many writes ;-)
bool ESP_rtcUserMemoryRead(uint32_t offset, uint32_t *data, size_t size)
{
  char sName[16];
  snprintf_P(sName, sizeof(sName), PSTR("rtc%d"), offset);

  nvs_handle handle;
  noInterrupts();
  nvs_open("tasrtc", NVS_READONLY, &handle);
  size_t tsize = size;
  nvs_get_blob(handle, sName, data, &tsize);
  nvs_close(handle);
  interrupts();

  return true;
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

uint32_t ESP_getSketchSize(void)
{
  static uint32_t sketchsize = 0;

  if (!sketchsize) {
    sketchsize = ESP.getSketchSize();  // This takes almost 2 seconds on an ESP32
  }
  return sketchsize;
}

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void DisableBrownout(void)
{
  // https://github.com/espressif/arduino-esp32/issues/863#issuecomment-347179737
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector
}
