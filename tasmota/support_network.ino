/*
  support_network.ino - Network support for Tasmota

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

/*********************************************************************************************\
 * MDNS
\*********************************************************************************************/

struct {
  uint8_t begun = 0;                  // mDNS active
} Mdns;

#ifdef USE_DISCOVERY
void StartMdns(void) {
//  static uint8_t mdns_delayed_start = Settings->param[P_MDNS_DELAYED_START];

  if (Settings->flag3.mdns_enabled) {  // SetOption55 - Control mDNS service
    if (!Mdns.begun) {
//      if (mdns_delayed_start) {
//        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_ATTEMPTING_CONNECTION));
//        mdns_delayed_start--;
//      } else {
//        mdns_delayed_start = Settings->param[P_MDNS_DELAYED_START];
        MDNS.end(); // close existing or MDNS.begin will fail
        Mdns.begun = (uint8_t)MDNS.begin(TasmotaGlobal.hostname);
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS "%s"), (Mdns.begun) ? PSTR(D_INITIALIZED) : PSTR(D_FAILED));
//      }
    }
  }
}

#ifdef MQTT_HOST_DISCOVERY
void MqttDiscoverServer(void)
{
  if (!Mdns.begun) { return; }

  int n = MDNS.queryService("mqtt", "tcp");  // Search for mqtt service

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_QUERY_DONE " %d"), n);

  if (n > 0) {
    uint32_t i = 0;            // If the hostname isn't set, use the first record found.
#ifdef MDNS_HOSTNAME
    for (i = n; i > 0; i--) {  // Search from last to first and use first if not found
      if (!strcmp(MDNS.hostname(i).c_str(), MDNS_HOSTNAME)) {
        break;                 // Stop at matching record
      }
    }
#endif  // MDNS_HOSTNAME
    SettingsUpdateText(SET_MQTT_HOST, MDNS.hostname(i).c_str());
    Settings->mqtt_port = MDNS.port(i);

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_MQTT_SERVICE_FOUND " %s," D_PORT " %d"), SettingsText(SET_MQTT_HOST), Settings->mqtt_port);
  }
}
#endif  // MQTT_HOST_DISCOVERY

#ifdef WEBSERVER_ADVERTISE
void MdnsAddServiceHttp(void) {
  if (1 == Mdns.begun) {
    Mdns.begun = 2;
    MDNS.addService("http", "tcp", WEB_PORT);
    MDNS.addServiceTxt("http", "tcp", "devicetype", "tasmota");
  }
}

#ifdef ESP8266 //Not needed with esp32 mdns
void MdnsUpdate(void) {
  if (2 == Mdns.begun) {
    MDNS.update(); // this is basically passpacket like a webserver
   // being called in main loop so no logging
   // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_MDNS "MDNS.update"));
  }
}
#endif  // ESP8266
#endif  // WEBSERVER_ADVERTISE
#endif  // USE_DISCOVERY

/*********************************************************************************************\
 * Global network parameters
\*********************************************************************************************/

char* NetworkHostname(void) {
#ifdef ESP32
#ifdef USE_ETHERNET
  if (!TasmotaGlobal.global_state.eth_down) {
    return EthernetHostname();
  }
#endif
#endif
  return TasmotaGlobal.hostname;
}

IPAddress NetworkAddress(void) {
#ifdef ESP32
#ifdef USE_ETHERNET
  if (!TasmotaGlobal.global_state.eth_down) {
    return EthernetLocalIP();
  }
#endif
#endif
  return WiFi.localIP();
}

String NetworkMacAddress(void) {
#ifdef ESP32
#ifdef USE_ETHERNET
  if (!TasmotaGlobal.global_state.eth_down) {
    return EthernetMacAddress();
  }
#endif
#endif
  return WiFi.macAddress();
}

String NetworkUniqueId(void) {
  String unique_id = WiFi.macAddress();
  unique_id.replace(":", "");  // Full 12 chars MAC address as ID
  return unique_id;
}
