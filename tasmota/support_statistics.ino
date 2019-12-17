/*
  support_statistics.ino - gather statistics for Tasmota

  Copyright (C) 2019  Theo Arends

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

#define USE_STATS_CODE

#ifdef USE_STATS_CODE
/*********************************************************************************************\
 * Gather statistics
\*********************************************************************************************/

String GetStatistics(void)
{
  char data[40];

  if (Settings.version < 0x08000000) {
    uint32_t str_len = 0;
    for (uint32_t i = 0; i < 2; i++) {
      str_len += strlen(Settings.sta_ssid[i]);
      str_len += strlen(Settings.sta_pwd[i]);
    }
    for (uint32_t i = 0; i < 3; i++) {
      str_len += strlen(Settings.mqtt_prefix[i]);
      str_len += strlen(Settings.ntp_server[i]);
    }
    for (uint32_t i = 0; i < 4; i++) {
      str_len += strlen(Settings.state_text[i]);
      str_len += strlen(Settings.friendlyname[i]);
    }
    for (uint32_t i = 0; i < MAX_RULE_MEMS; i++) {
      str_len += strlen(Settings.mems[i]);
    }

    str_len += strlen(Settings.ota_url);
    str_len += strlen(Settings.hostname);
    str_len += strlen(Settings.syslog_host);
    str_len += strlen(Settings.mqtt_host);
    str_len += strlen(Settings.mqtt_client);
    str_len += strlen(Settings.mqtt_user);
    str_len += strlen(Settings.mqtt_pwd);
    str_len += strlen(Settings.mqtt_topic);
    str_len += strlen(Settings.button_topic);
    str_len += strlen(Settings.switch_topic);
    str_len += strlen(Settings.mqtt_grptopic);
    str_len += strlen(Settings.web_password);
    str_len += strlen(Settings.mqtt_fulltopic);
    str_len += strlen(Settings.cors_domain);
    snprintf_P(data, sizeof(data), PSTR(",\"CR\":\"%d/1151\""), str_len);  // Char Usage Ratio
  } else {
    snprintf_P(data, sizeof(data), PSTR(",\"CR\":\"%d/%d\""), GetSettingsTextLen(), settings_text_size);  // Char Usage Ratio
  }
  return String(data);
}

#else

String GetStatistics(void)
{
  return String("");
}

#endif  // USE_STATS_CODE