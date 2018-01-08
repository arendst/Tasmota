/*
  xdrv_07_home_assistant.ino - home assistant support for Sonoff-Tasmota

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

#ifdef USE_HOME_ASSISTANT

#ifdef USE_WEBSERVER
const char HOME_ASSISTANT_HTTP_FORM[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_HOME_ASSISTANT_PARAMETERS "&nbsp;</b></legend><form method='post' action='sv'><br/><table>"
  "<input id='w' name='w' value='7' hidden><input id='r' name='r' value='1' hidden>"
  "<b>" D_HOME_ASSISTANT_DISCOVERY_ENABLE "</b><input id='de' type='checkbox' name='de' style='float:right;width:auto;' {de ><br/><br/>"
  "<b>" D_HOME_ASSISTANT_DISCOVERY_PREFIX "</b><br/>"
  "<input id='dp' type='text' name='dp' placeholder='" HOME_ASSISTANT_DISCOVERY_PREFIX "' value='{dp'>";
#endif  // USE_WEBSERVER

void HomeAssistantStart() {
  AddLog_P(LOG_LEVEL_INFO, D_LOG_HOME_ASSISTANT, "HomeAssistantStart()");
  if(Settings.home_assistant_discovery_enabled){
    HomeAssistantSendDiscovery();
  }
}

// Send discovery message for all power devices
void HomeAssistantSendDiscovery() {
  if(!Settings.flag.mqtt_enabled) {
    return;
  }

  char stopic[TOPSZ];
  char savailable[100];
  char stemp[16];

  AddLog_P(LOG_LEVEL_INFO, D_LOG_HOME_ASSISTANT, "HomeAssistantSendDiscovery()");

  HomeAssistantClearDiscovery();

  if(devices_present == 1) {
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/switch/%s/config"), Settings.home_assistant_discovery_prefix, Settings.mqtt_topic);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"name\":\"%s\",\"command_topic\":\"%s/%s/%s\",\"state_topic\":\"%s/%s/%s\",\"optimistic\":\"false\",\"payload_off\":\"%s\",\"payload_on\":\"%s\",\"availability_topic\":\"%s/%s/" D_LWT "\",\"payload_available\":\"" D_ONLINE "\",\"payload_not_available\":\"" D_OFFLINE "\"}"),
      Settings.friendlyname[0],
      Settings.mqtt_prefix[0],
      Settings.mqtt_topic,
      GetPowerDevice(stemp, 1, sizeof(stemp)),
      Settings.mqtt_prefix[1],
      Settings.mqtt_topic,
      GetPowerDevice(stemp, 1, sizeof(stemp)),
      Settings.state_text[0],
      Settings.state_text[1],
      Settings.mqtt_prefix[2],
      Settings.mqtt_topic
    );

    MqttPublish(stopic, true);
  } else {
    for(int i = 1; i <= devices_present; i++) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/switch/%s_%d/config"), Settings.home_assistant_discovery_prefix, Settings.mqtt_topic, i);
      
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"name\":\"%s\",\"command_topic\":\"%s/%s/%s\",\"state_topic\":\"%s/%s/%s\",\"optimistic\":\"false\",\"payload_off\":\"%s\",\"payload_on\":\"%s\",\"availability_topic\":\"%s/%s/" D_LWT "\",\"payload_available\":\"" D_ONLINE "\",\"payload_not_available\":\"" D_OFFLINE "\"}"),
        Settings.friendlyname[i-1],
        Settings.mqtt_prefix[0],
        Settings.mqtt_topic,
        GetPowerDevice(stemp, i, sizeof(stemp)),
        Settings.mqtt_prefix[1],
        Settings.mqtt_topic,
        GetPowerDevice(stemp, i, sizeof(stemp)),
        Settings.state_text[0],
        Settings.state_text[1],
        Settings.mqtt_prefix[2],
        Settings.mqtt_topic
      );

      MqttPublish(stopic, true);
    }
  }
}

//remove retained discovery message for current topic(s)
void HomeAssistantClearDiscovery() {
  if(!Settings.flag.mqtt_enabled) {
    return;
  }

  char stopic[TOPSZ];

  AddLog_P(LOG_LEVEL_INFO, D_LOG_HOME_ASSISTANT, "HomeAssistantClearDiscovery");

  if(devices_present == 1) {
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/switch/%s/config"), Settings.home_assistant_discovery_prefix, Settings.mqtt_topic);
    snprintf_P(mqtt_data, sizeof(mqtt_data), "");

    MqttPublish(stopic, true);
   } else {
    for(int i = 1; i <= devices_present; i++) {
      snprintf_P(stopic, sizeof(stopic), PSTR("%s/switch/%s_%d/config"), Settings.home_assistant_discovery_prefix, Settings.mqtt_topic, i);
      snprintf_P(mqtt_data, sizeof(mqtt_data), "");

      MqttPublish(stopic, true);
    }
  }
}
/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char S_CONFIGURE_HOME_ASSISTANT[] PROGMEM = D_CONFIGURE_HOME_ASSISTANT;

void HandleHomeAssistantConfiguration() {
  if (HTTP_USER == webserver_state) {
    HandleRoot();
    return;
  }
  AddLog_P(LOG_LEVEL_INFO, D_LOG_HOME_ASSISTANT, "HandleHomeAssistantConfiguration()");

  char stemp[32];
  char *sensortype;

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_HOME_ASSISTANT));
  page += FPSTR(HOME_ASSISTANT_HTTP_FORM);
  page.replace(F("{de"), FPSTR(Settings.home_assistant_discovery_enabled ? "checked" : ""));
  page.replace(F("{dp"), FPSTR(Settings.home_assistant_discovery_prefix));
  page += F("</table>");
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void HomeAssistantSaveSettings() {
  //clear old discovery
  HomeAssistantClearDiscovery();

  //Save input
  Settings.home_assistant_discovery_enabled = (strcmp(WebServer->arg("de").c_str(), "on") == 0);
  strlcpy(Settings.home_assistant_discovery_prefix,  WebServer->arg("dp").c_str(), sizeof(Settings.home_assistant_discovery_prefix));

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_HOME_ASSISTANT "HomeAssistantSaveSettings():" D_HOME_ASSISTANT_DISCOVERY_ENABLE ": %s, " D_HOME_ASSISTANT_DISCOVERY_PREFIX ": '%s'"),
    (Settings.home_assistant_discovery_enabled ? "yes" : "no"), Settings.home_assistant_discovery_prefix);
  AddLog(LOG_LEVEL_INFO);
}

#endif  // USE_WEBSERVER
#endif  // USE_HOME_ASSISTANT
