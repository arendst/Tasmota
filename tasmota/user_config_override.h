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

#undef  STA_SSID1
#define STA_SSID1         "Hello There"             // [Ssid1] Wifi SSID

#undef  STA_PASS1
#define STA_PASS1         "GeneralKenobi"     // [Password1] Wifi password

#undef MQTT_HOST
#define MQTT_HOST         "52.57.59.51"                // [MqttHost]

#undef MQTT_PORT
#define MQTT_PORT              1884              // [MqttPort] MQTT port (10123 on CloudMQTT)

#undef MQTT_USER
#define MQTT_USER              "defaultuser"       // [MqttUser] MQTT user

#undef MQTT_PASS
#define MQTT_PASS              "Picqyf-rigmu6-sozzug"       // [MqttPassword] MQTT password

#undef USER_TEMPLATE
#define USER_TEMPLATE "{\"NAME\":\"CE_Dongle_v1\",\"GPIO\":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,289,0,288,1,1,0,10752,10720,1,0,0,0,0,10624,10688,10592,10656,1,0,0,1],\"FLAG\":0,\"BASE\":1}"

#undef MODULE
#define MODULE                 USER_MODULE

#define USE_MODBUS_PT_BRIDGE

#endif  // _USER_CONFIG_OVERRIDE_H_
