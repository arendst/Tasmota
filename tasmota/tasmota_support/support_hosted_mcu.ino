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

String GetHostedMCUFwVersion()
{
  if(!esp_hosted_is_config_valid()) {
    return String("");
  }
  esp_hosted_coprocessor_fwver_t ver_info;
  esp_err_t err = esp_hosted_get_coprocessor_fwversion(&ver_info);
  if (err == ESP_OK) {
    char data[40];
    snprintf_P(data, sizeof(data), PSTR("%d.%d.%d"), ver_info.major1,ver_info.minor1,ver_info.patch1);
    // AddLog(LOG_LEVEL_DEBUG, PSTR("Fw: %d.%d.%d"), ver_info.major1, ver_info.minor1, ver_info.patch1);
    return String(data);
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("Err: %d, version 0.0..6 or older"), err);
  return String(PSTR("0.0.6")); // we can not know exactly, but API was added after 0.0.6
}

void OTAHostedMCU(const char* image_url) {
  AddLog(LOG_LEVEL_INFO, PSTR("OTA: co-processor OTA update started from %s"), image_url);
  esp_err_t ret = esp_hosted_slave_ota(image_url);
  // next lines are questionable, because ATM the system will reboot immediately - maybe we would see the failure
  if (ret == ESP_OK) {
    AddLog(LOG_LEVEL_INFO, PSTR("OTA: co-processor OTA update successful !!"));
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR("OTA: co-processor OTA update failed: %d"), ret);
  }
}


#endif //  CONFIG_ESP_WIFI_REMOTE_ENABLED