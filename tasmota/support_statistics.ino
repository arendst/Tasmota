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

struct STATS {
  uint32_t str_size = 0;  // Total number of characters reserved as char array in Settings
  uint32_t str_len = 0;   // Total number of characters used within char array
} Stats;

void StatisticsChar(const char* text, uint32_t size)
{
  uint32_t len = strlen(text);
  Stats.str_len += len;
  Stats.str_size += size;
}

String GetStatistics(void)
{
  Stats.str_len = 0;
  Stats.str_size = 0;

  for (uint32_t i = 0; i < 3; i++) {
    StatisticsChar(Settings.ntp_server[i], sizeof(Settings.ntp_server[i]));
  }
  for (uint32_t i = 0; i < 4; i++) {
    StatisticsChar(Settings.state_text[i], sizeof(Settings.state_text[i]));
    StatisticsChar(Settings.friendlyname[i], sizeof(Settings.friendlyname[i]));
  }
  for (uint32_t i = 0; i < MAX_RULE_MEMS; i++) {
    StatisticsChar(Settings.mems[i], sizeof(Settings.mems[i]));
  }
  StatisticsChar(Settings.mqtt_host, sizeof(Settings.mqtt_host));
  StatisticsChar(Settings.mqtt_client, sizeof(Settings.mqtt_client));
  StatisticsChar(Settings.mqtt_user, sizeof(Settings.mqtt_user));
  StatisticsChar(Settings.mqtt_pwd, sizeof(Settings.mqtt_pwd));
  StatisticsChar(Settings.mqtt_topic, sizeof(Settings.mqtt_topic));
  StatisticsChar(Settings.button_topic, sizeof(Settings.button_topic));
  StatisticsChar(Settings.switch_topic, sizeof(Settings.switch_topic));
  StatisticsChar(Settings.mqtt_grptopic, sizeof(Settings.mqtt_grptopic));
  StatisticsChar(Settings.web_password, sizeof(Settings.web_password));
  StatisticsChar(Settings.mqtt_fulltopic, sizeof(Settings.mqtt_fulltopic));
  StatisticsChar(Settings.cors_domain, sizeof(Settings.cors_domain));

  char data[40];
  snprintf_P(data, sizeof(data), PSTR(",\"CR\":\"%d/%d,%d/%d\""), Stats.str_len, Stats.str_size, GetSettingsTextLen(), settings_text_size);  // Char Usage Ratio
  return String(data);
}

#else

String GetStatistics(void)
{
  return String("");
}

#endif  // USE_STATS_CODE