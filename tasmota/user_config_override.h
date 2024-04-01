/*
  user_config_override.h - user configuration overrides my_user_config.h for Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

#undef PROJECT
#define PROJECT           "dongle"         // PROJECT is used as the default topic delimiter

#undef FRIENDLY_NAME
#define FRIENDLY_NAME     "Dongle"         // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa

#undef MQTT_GRPTOPIC
#define MQTT_GRPTOPIC     "dongles"        // [GroupTopic] MQTT Group topic

// #undef  STA_SSID1
// #define STA_SSID1         "Hello There"             // [Ssid1] Wifi SSID

// #undef  STA_PASS1
// #define STA_PASS1         "GeneralKenobi"     // [Password1] Wifi password

#undef MQTT_HOST
#define MQTT_HOST         "52.57.59.51"                // [MqttHost]

#undef MQTT_PORT
#define MQTT_PORT              1884              // [MqttPort] MQTT port (10123 on CloudMQTT)

#undef MQTT_USER
#define MQTT_USER              "defaultuser"       // [MqttUser] MQTT user

#undef MQTT_PASS
#define MQTT_PASS              "Picqyf-rigmu6-sozzug"       // [MqttPassword] MQTT password

#undef USER_TEMPLATE
//dongle
// #define USER_TEMPLATE "{\"NAME\":\"CE_Dongle_v1\",\"GPIO\":[32,1,1,1,1,1,1,1,1,1,1,1,1,1,1,289,0,288,1,1,0,1,1,9952,0,0,0,0,9408,1,9440,1,1,0,0,1],\"FLAG\":0,\"BASE\":1}"
// #define USER_TEMPLATE "{\"NAME\":\"CE_Dongle_v2\",\"GPIO\":[32,1,1,1,1,1,1,1,1,1,1,1,1,1,1,289,0,288,1,1,0,1,1,9952,0,0,0,0,9408,1,9440,1,1,0,0,1],\"FLAG\":0,\"BASE\":1,\"CMND\":\"LedPower 1 | SetOption114 1 | SwitchMode0 1\"}"
//aqua
#define USER_TEMPLATE "{\"NAME\":\"CE_Aqua_v1\",\"GPIO\":[32,1,1,1,164,1,1,1,161,162,1,163,1312,166,290,289,0,544,167,168,0,225,224,9952,0,0,0,0,9408,226,9440,160,1,0,0,1],\"FLAG\":0,\"BASE\":1,\"CMND\":\"LedPower 1 | SetOption114 1 | SwitchMode0 1\"}"

#undef MODULE
#define MODULE                 USER_MODULE

#define USE_MODBUS_PT_BRIDGE


#endif  // _USER_CONFIG_OVERRIDE_H_
