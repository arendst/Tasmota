/*
  support_network.ino - Network support for Tasmota

  Copyright (C) 2020  Theo Arends

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
  if (Settings.flag3.mdns_enabled) {  // SetOption55 - Control mDNS service
    if (!Mdns.begun) {
//            if (mdns_delayed_start) {
//              AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_ATTEMPTING_CONNECTION));
//              mdns_delayed_start--;
//            } else {
//              mdns_delayed_start = Settings.param[P_MDNS_DELAYED_START];
        Mdns.begun = (uint8_t)MDNS.begin(my_hostname);
        AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS "%s"), (Mdns.begun) ? D_INITIALIZED : D_FAILED);
//            }
    }
  }
}

#ifdef MQTT_HOST_DISCOVERY
void MqttDiscoverServer(void)
{
  if (!Mdns.begun) { return; }

  int n = MDNS.queryService("mqtt", "tcp");  // Search for mqtt service

  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_QUERY_DONE " %d"), n);

  if (n > 0) {
    uint32_t i = 0;            // If the hostname isn't set, use the first record found.
#ifdef MDNS_HOSTNAME
    for (i = n; i > 0; i--) {  // Search from last to first and use first if not found
      if (!strcmp(MDNS.hostname(i).c_str(), MDNS_HOSTNAME)) {
        break;                 // Stop at matching record
      }
    }
#endif  // MDNS_HOSTNAME
    SettingsUpdateText(SET_MQTT_HOST, MDNS.IP(i).toString().c_str());
    Settings.mqtt_port = MDNS.port(i);

    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_MQTT_SERVICE_FOUND " %s, " D_IP_ADDRESS " %s, " D_PORT " %d"), MDNS.hostname(i).c_str(), SettingsText(SET_MQTT_HOST), Settings.mqtt_port);
  }
}
#endif  // MQTT_HOST_DISCOVERY

#ifdef WEBSERVER_ADVERTISE
void MdnsAddServiceHttp(void) {
  if (1 == Mdns.begun) {
    Mdns.begun = 2;
    MDNS.addService("http", "tcp", WEB_PORT);
  }
}

void MdnsUpdate(void) {
  if (2 == Mdns.begun) {
    MDNS.update();
    AddLog_P(LOG_LEVEL_DEBUG_MORE, D_LOG_MDNS, "MDNS.update");
  }
}
#endif  // WEBSERVER_ADVERTISE
#endif  // USE_DISCOVERY

/*********************************************************************************************\
 * Global network parameters
\*********************************************************************************************/

char* NetworkHostname(void) {
  if (global_state.eth_down) {
    return my_hostname;
  }
#ifdef ESP32
#ifdef USE_ETHERNET
  else {
    return EthernetHostname();
  }
#endif
#endif
}

IPAddress NetworkAddress(void) {
  if (global_state.eth_down) {
    return WiFi.localIP();
  }
#ifdef ESP32
#ifdef USE_ETHERNET
  else {
    return EthernetLocalIP();
  }
#endif
#endif
}

String NetworkMacAddress(void) {
  if (global_state.eth_down) {
    return WiFi.macAddress();
  }
#ifdef ESP32
#ifdef USE_ETHERNET
  else {
    return EthernetMacAddress();
  }
#endif
#endif
}
