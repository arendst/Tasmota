/*
  support_hosted_mcu.ino - eeprom support for Tasmota

  Copyright (C) 2025  Theo Arends & Christian Baars

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

#ifdef CONFIG_ESP_WIFI_REMOTE_ENABLED

#include "esp_hosted.h"
#include "esp_hosted_api_types.h"
#include "esp_hosted_ota.h"

int GetFwVersionNumber(void) {
  // Function is not yet implemented in Arduino Core so emulate it here
  return 0x0200000E;   // v2.0.14
}

int GetHostedMCUFwVersionNumber(void) {
  static int version = -1;

  if (!esp_hosted_is_config_valid()) {
    return 0;
  }
  if (-1 == version) {
    version = 6;   // v0.0.6
    esp_hosted_coprocessor_fwver_t ver_info;
    esp_err_t err = esp_hosted_get_coprocessor_fwversion(&ver_info);  // This takes almost 4 seconds on <v0.0.6
    if (err == ESP_OK) {
      version = ver_info.major1 << 24 | ver_info.minor1 << 16 | ver_info.patch1;
    } else {
      // We can not know exactly, as API was added after 0.0.6
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HST: Error %d, hosted version 0.0.6 or older"), err);
    }  
  }
  return version;
}

String GetHostedMCUFwVersion(void) {
  int version = GetHostedMCUFwVersionNumber();

  if (0 == version) {
    return String("");
  }
  uint8_t major1 = version >> 24;
  uint8_t minor1 = version >> 16;
  uint16_t patch1 = version;
  char data[40];
  snprintf_P(data, sizeof(data), PSTR("%d.%d.%d"), major1, minor1, patch1);
  return String(data);
}

int OTAHostedMCU(const char* image_url) {
  return esp_hosted_slave_ota(image_url);
}

#endif  // CONFIG_ESP_WIFI_REMOTE_ENABLED