/*
  support_wifi.ino - wifi support for Tasmota

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

/*********************************************************************************************\
 * Wifi
\*********************************************************************************************/

#ifndef WIFI_RSSI_THRESHOLD
#define WIFI_RSSI_THRESHOLD     10         // Difference in dB between current network and scanned network
#endif
#ifndef WIFI_RESCAN_MINUTES
#define WIFI_RESCAN_MINUTES     44         // Number of minutes between wifi network rescan
#endif

const uint8_t WIFI_CONFIG_SEC = 180;       // seconds before restart
const uint8_t WIFI_CHECK_SEC = 20;         // seconds
const uint8_t WIFI_RETRY_OFFSET_SEC = 20;  // seconds

#include <ESP8266WiFi.h>                   // Wifi, MQTT, Ota, WifiManager
#if LWIP_IPV6
#include <AddrList.h>                      // IPv6 DualStack
#endif  // LWIP_IPV6=1

struct WIFI {
  uint32_t last_event = 0;                 // Last wifi connection event
  uint32_t downtime = 0;                   // Wifi down duration
  uint16_t link_count = 0;                 // Number of wifi re-connect
  uint8_t counter;
  uint8_t retry_init;
  uint8_t retry;
  uint8_t status;
  uint8_t config_type = 0;
  uint8_t config_counter = 0;
  uint8_t mdns_begun = 0;                  // mDNS active
  uint8_t scan_state;
  uint8_t bssid[6];
} Wifi;

int WifiGetRssiAsQuality(int rssi)
{
  int quality = 0;

  if (rssi <= -100) {
    quality = 0;
  } else if (rssi >= -50) {
    quality = 100;
  } else {
    quality = 2 * (rssi + 100);
  }
  return quality;
}

bool WifiConfigCounter(void)
{
  if (Wifi.config_counter) {
    Wifi.config_counter = WIFI_CONFIG_SEC;
  }
  return (Wifi.config_counter);
}

void WifiConfig(uint8_t type)
{
  if (!Wifi.config_type) {
    if ((WIFI_RETRY == type) || (WIFI_WAIT == type)) { return; }
#ifdef USE_EMULATION
    UdpDisconnect();
#endif  // USE_EMULATION
    WiFi.disconnect();                       // Solve possible Wifi hangs
    Wifi.config_type = type;

#ifndef USE_WEBSERVER
    if (WIFI_MANAGER == Wifi.config_type) {
      Wifi.config_type = WIFI_SERIAL;
    }
#endif  // USE_WEBSERVER

    Wifi.config_counter = WIFI_CONFIG_SEC;   // Allow up to WIFI_CONFIG_SECS seconds for phone to provide ssid/pswd
    Wifi.counter = Wifi.config_counter +5;
    blinks = 1999;
    if (WIFI_RESTART == Wifi.config_type) {
      restart_flag = 2;
    }
    else if (WIFI_SERIAL == Wifi.config_type) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_6_SERIAL " " D_ACTIVE_FOR_3_MINUTES));
    }
#ifdef USE_WEBSERVER
    else if (WIFI_MANAGER == Wifi.config_type || WIFI_MANAGER_RESET_ONLY == Wifi.config_type) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_2_WIFIMANAGER " " D_ACTIVE_FOR_3_MINUTES));
      WifiManagerBegin(WIFI_MANAGER_RESET_ONLY == Wifi.config_type);
    }
#endif  // USE_WEBSERVER
  }
}

void WifiSetMode(WiFiMode_t wifi_mode)
{
  if (WiFi.getMode() == wifi_mode) { return; }

  if (wifi_mode != WIFI_OFF) {
    // See: https://github.com/esp8266/Arduino/issues/6172#issuecomment-500457407
    WiFi.forceSleepWake(); // Make sure WiFi is really active.
    delay(100);
  }

  uint32_t retry = 2;
  while (!WiFi.mode(wifi_mode) && retry--) {
    AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR("Retry set Mode..."));
    delay(100);
  }

  if (wifi_mode == WIFI_OFF) {
    delay(1000);
    WiFi.forceSleepBegin();
    delay(1);
  } else {
    delay(30); // Must allow for some time to init.
  }
}

void WiFiSetSleepMode(void)
{
/* Excerpt from the esp8266 non os sdk api reference (v2.2.1):
 * Sets sleep type for power saving. Set WIFI_NONE_SLEEP to disable power saving.
 * - Default mode: WIFI_MODEM_SLEEP.
 * - In order to lower the power comsumption, ESP8266 changes the TCP timer
 *   tick from 250ms to 3s in WIFI_LIGHT_SLEEP mode, which leads to increased timeout for
 *   TCP timer. Therefore, the WIFI_MODEM_SLEEP or deep-sleep mode should be used
 *   where there is a requirement for the accurancy of the TCP timer.
 *
 * Sleep is disabled in core 2.4.1 and 2.4.2 as there are bugs in their SDKs
 * See https://github.com/arendst/Tasmota/issues/2559
 */

// Sleep explanation: https://github.com/esp8266/Arduino/blob/3f0c601cfe81439ce17e9bd5d28994a7ed144482/libraries/ESP8266WiFi/src/ESP8266WiFiGeneric.cpp#L255
#if defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
#else  // Enabled in 2.3.0, 2.4.0 and stage
  if (sleep && Settings.flag3.sleep_normal) {  // SetOption60 - Enable normal sleep instead of dynamic sleep
    WiFi.setSleepMode(WIFI_LIGHT_SLEEP);       // Allow light sleep during idle times
  } else {
    WiFi.setSleepMode(WIFI_MODEM_SLEEP);       // Disable sleep (Esp8288/Arduino core and sdk default)
  }
#endif
  WifiSetOutputPower();
}

void WifiBegin(uint8_t flag, uint8_t channel)
{
  const char kWifiPhyMode[] = " BGN";

#ifdef USE_EMULATION
  UdpDisconnect();
#endif  // USE_EMULATION

#ifdef ARDUINO_ESP8266_RELEASE_2_3_0  // (!strncmp_P(ESP.getSdkVersion(),PSTR("1.5.3"),5))
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, PSTR(D_PATCH_ISSUE_2186));
//  WiFi.mode(WIFI_OFF);      // See https://github.com/esp8266/Arduino/issues/2186
  WifiSetMode(WIFI_OFF);
#endif

  WiFi.persistent(false);   // Solve possible wifi init errors (re-add at 6.2.1.16 #4044, #4083)
  WiFi.disconnect(true);    // Delete SDK wifi config
  delay(200);
//  WiFi.mode(WIFI_STA);      // Disable AP mode
  WifiSetMode(WIFI_STA);
  WiFiSetSleepMode();
//  if (WiFi.getPhyMode() != WIFI_PHY_MODE_11N) { WiFi.setPhyMode(WIFI_PHY_MODE_11N); }  // B/G/N
//  if (WiFi.getPhyMode() != WIFI_PHY_MODE_11G) { WiFi.setPhyMode(WIFI_PHY_MODE_11G); }  // B/G
  if (!WiFi.getAutoConnect()) { WiFi.setAutoConnect(true); }
//  WiFi.setAutoReconnect(true);
  switch (flag) {
  case 0:  // AP1
  case 1:  // AP2
    Settings.sta_active = flag;
    break;
  case 2:  // Toggle
    Settings.sta_active ^= 1;
  }        // 3: Current AP
  if (!strlen(SettingsText(SET_STASSID1 + Settings.sta_active))) {
    Settings.sta_active ^= 1;  // Skip empty SSID
  }
  if (Settings.ip_address[0]) {
    WiFi.config(Settings.ip_address[0], Settings.ip_address[1], Settings.ip_address[2], Settings.ip_address[3]);  // Set static IP
  }
  WiFi.hostname(my_hostname);
  if (channel) {
    WiFi.begin(SettingsText(SET_STASSID1 + Settings.sta_active), SettingsText(SET_STAPWD1 + Settings.sta_active), channel, Wifi.bssid);
  } else {
    WiFi.begin(SettingsText(SET_STASSID1 + Settings.sta_active), SettingsText(SET_STAPWD1 + Settings.sta_active));
  }
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECTING_TO_AP "%d %s " D_IN_MODE " 11%c " D_AS " %s..."),
    Settings.sta_active +1, SettingsText(SET_STASSID1 + Settings.sta_active), kWifiPhyMode[WiFi.getPhyMode() & 0x3], my_hostname);

#if LWIP_IPV6
  for (bool configured = false; !configured;) {
    uint16_t cfgcnt = 0;
    for (auto addr : addrList) {
      if ((configured = !addr.isLocal() && addr.isV6()) || cfgcnt==30) {
        AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "Got IPv6 global address %s"), addr.toString().c_str());
        break;  // IPv6 is mandatory but stop after 15 seconds
      }
      delay(500);  // Loop until real IPv6 address is aquired or too many tries failed
      cfgcnt++;
    }
  }
#endif  // LWIP_IPV6=1
}

void WifiBeginAfterScan(void)
{
  static int8_t best_network_db;

  // Not active
  if (0 == Wifi.scan_state) { return; }
  // Init scan when not connected
  if (1 == Wifi.scan_state) {
    memset((void*) &Wifi.bssid, 0, sizeof(Wifi.bssid));
    best_network_db = -127;
    Wifi.scan_state = 3;
  }
  // Init scan when connected
  if (2 == Wifi.scan_state) {
    uint8_t* bssid = WiFi.BSSID();                  // Get current bssid
    memcpy((void*) &Wifi.bssid, (void*) bssid, sizeof(Wifi.bssid));
    best_network_db = WiFi.RSSI();                  // Get current rssi and add threshold
    if (best_network_db < -WIFI_RSSI_THRESHOLD) { best_network_db += WIFI_RSSI_THRESHOLD; }
    Wifi.scan_state = 3;
  }
  // Init scan
  if (3 == Wifi.scan_state) {
    if (WiFi.scanComplete() != WIFI_SCAN_RUNNING) {
      WiFi.scanNetworks(true);                      // Start wifi scan async
      Wifi.scan_state++;
      AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, PSTR("Network (re)scan started..."));
      return;
    }
  }
  int8_t wifi_scan_result = WiFi.scanComplete();
  // Check scan done
  if (4 == Wifi.scan_state) {
    if (wifi_scan_result != WIFI_SCAN_RUNNING) {
      Wifi.scan_state++;
    }
  }
  // Scan done
  if (5 == Wifi.scan_state) {
    int32_t channel = 0;                            // No scan result
    int8_t ap = 3;                                  // AP default if not found
    uint8_t last_bssid[6];                          // Save last bssid
    memcpy((void*) &last_bssid, (void*) &Wifi.bssid, sizeof(last_bssid));

    if (wifi_scan_result > 0) {
      // Networks found
      for (uint32_t i = 0; i < wifi_scan_result; ++i) {

        String ssid_scan;
        int32_t rssi_scan;
        uint8_t sec_scan;
        uint8_t* bssid_scan;
        int32_t chan_scan;
        bool hidden_scan;

        WiFi.getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, bssid_scan, chan_scan, hidden_scan);

        bool known = false;
        uint32_t j;
        for (j = 0; j < 2; j++) {
          if (ssid_scan == SettingsText(SET_STASSID1 + j)) {  // SSID match
            known = true;
            if (rssi_scan > best_network_db) {      // Best network
              if (sec_scan == ENC_TYPE_NONE || SettingsText(SET_STAPWD1 + j)) {  // Check for passphrase if not open wlan
                best_network_db = (int8_t)rssi_scan;
                channel = chan_scan;
                ap = j;                             // AP1 or AP2
                memcpy((void*) &Wifi.bssid, (void*) bssid_scan, sizeof(Wifi.bssid));
              }
            }
            break;
          }
        }
        char hex_char[18];
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "Network %d, AP%c, SSId %s, Channel %d, BSSId %s, RSSI %d, Encryption %d"),
          i,
          (known) ? (j) ? '2' : '1' : '-',
          ssid_scan.c_str(),
          chan_scan,
          ToHex_P((unsigned char*)bssid_scan, 6, hex_char, sizeof(hex_char), ':'),
          rssi_scan,
          (sec_scan == ENC_TYPE_NONE) ? 0 : 1);
        delay(0);
      }
      WiFi.scanDelete();                            // Clean up Ram
      delay(0);
    }
    Wifi.scan_state = 0;
    // If bssid changed then (re)connect wifi
    for (uint32_t i = 0; i < sizeof(Wifi.bssid); i++) {
      if (last_bssid[i] != Wifi.bssid[i]) {
        WifiBegin(ap, channel);                     // 0 (AP1), 1 (AP2) or 3 (default AP)
        break;
      }
    }
  }
}

uint16_t WifiLinkCount(void)
{
  return Wifi.link_count;
}

String WifiDowntime(void)
{
  return GetDuration(Wifi.downtime);
}

void WifiSetState(uint8_t state)
{
  if (state == global_state.wifi_down) {
    if (state) {
      rules_flag.wifi_connected = 1;
      Wifi.link_count++;
      Wifi.downtime += UpTime() - Wifi.last_event;
    } else {
      rules_flag.wifi_disconnected = 1;
      Wifi.last_event = UpTime();
    }
  }
  global_state.wifi_down = state ^1;
}

#if LWIP_IPV6
bool WifiCheckIPv6(void)
{
  bool ipv6_global=false;

  for (auto a : addrList) {
    if(!a.isLocal() && a.isV6()) ipv6_global=true;
  }
  return ipv6_global;
}

String WifiGetIPv6(void)
{
  for (auto a : addrList) {
    if(!a.isLocal() && a.isV6()) return a.toString();
  }
  return "";
}

bool WifiCheckIPAddrStatus(void)	// Return false for 169.254.x.x or fe80::/64
{
  bool ip_global=false;

  for (auto a : addrList) {
    if(!a.isLocal()) ip_global=true;
  }
  return ip_global;
}
#endif  // LWIP_IPV6=1

void WifiCheckIp(void)
{
#if LWIP_IPV6
  if(WifiCheckIPAddrStatus()) {
    Wifi.status = WL_CONNECTED;
#else
  if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
#endif  // LWIP_IPV6=1
    WifiSetState(1);
    Wifi.counter = WIFI_CHECK_SEC;
    Wifi.retry = Wifi.retry_init;
    AddLog_P((Wifi.status != WL_CONNECTED) ? LOG_LEVEL_INFO : LOG_LEVEL_DEBUG_MORE, S_LOG_WIFI, PSTR(D_CONNECTED));
    if (Wifi.status != WL_CONNECTED) {
//      AddLog_P(LOG_LEVEL_INFO, PSTR("Wifi: Set IP addresses"));
      Settings.ip_address[1] = (uint32_t)WiFi.gatewayIP();
      Settings.ip_address[2] = (uint32_t)WiFi.subnetMask();
      Settings.ip_address[3] = (uint32_t)WiFi.dnsIP();
    }
    Wifi.status = WL_CONNECTED;
#ifdef USE_DISCOVERY
#ifdef WEBSERVER_ADVERTISE
    if (2 == Wifi.mdns_begun) {
      MDNS.update();
      AddLog_P(LOG_LEVEL_DEBUG_MORE, D_LOG_MDNS, "MDNS.update");
    }
#endif  // USE_DISCOVERY
#endif  // WEBSERVER_ADVERTISE
  } else {
    WifiSetState(0);
    uint8_t wifi_config_tool = Settings.sta_config;
    Wifi.status = WiFi.status();
    switch (Wifi.status) {
      case WL_CONNECTED:
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_NO_IP_ADDRESS));
        Wifi.status = 0;
        Wifi.retry = Wifi.retry_init;
        break;
      case WL_NO_SSID_AVAIL:
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_AP_NOT_REACHED));
        if (WIFI_WAIT == Settings.sta_config) {
          Wifi.retry = Wifi.retry_init;
        } else {
          if (Wifi.retry > (Wifi.retry_init / 2)) {
            Wifi.retry = Wifi.retry_init / 2;
          }
          else if (Wifi.retry) {
            Wifi.retry = 0;
          }
        }
        break;
      case WL_CONNECT_FAILED:
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_WRONG_PASSWORD));
        if (Wifi.retry > (Wifi.retry_init / 2)) {
          Wifi.retry = Wifi.retry_init / 2;
        }
        else if (Wifi.retry) {
          Wifi.retry = 0;
        }
        break;
      default:  // WL_IDLE_STATUS and WL_DISCONNECTED
        if (!Wifi.retry || ((Wifi.retry_init / 2) == Wifi.retry)) {
          AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_AP_TIMEOUT));
        } else {
          if (!strlen(SettingsText(SET_STASSID1)) && !strlen(SettingsText(SET_STASSID2))) {
            wifi_config_tool = WIFI_MANAGER;  // Skip empty SSIDs and start Wifi config tool
            Wifi.retry = 0;
          } else {
            AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, PSTR(D_ATTEMPTING_CONNECTION));
          }
        }
    }
    if (Wifi.retry) {
      if (Settings.flag3.use_wifi_scan) {  // SetOption56 - Scan wifi network at restart for configured AP's
        if (Wifi.retry_init == Wifi.retry) {
          Wifi.scan_state = 1;    // Select scanned SSID
        }
      } else {
        if (Wifi.retry_init == Wifi.retry) {
          WifiBegin(3, 0);        // Select default SSID
        }
        if ((Settings.sta_config != WIFI_WAIT) && ((Wifi.retry_init / 2) == Wifi.retry)) {
          WifiBegin(2, 0);        // Select alternate SSID
        }
      }
      Wifi.counter = 1;
      Wifi.retry--;
    } else {
      WifiConfig(wifi_config_tool);
      Wifi.counter = 1;
      Wifi.retry = Wifi.retry_init;
    }
  }
}

void WifiCheck(uint8_t param)
{
  Wifi.counter--;
  switch (param) {
  case WIFI_SERIAL:
  case WIFI_MANAGER:
    WifiConfig(param);
    break;
  default:
    if (Wifi.config_counter) {
      Wifi.config_counter--;
      Wifi.counter = Wifi.config_counter +5;
      if (Wifi.config_counter) {
        if (!Wifi.config_counter) {
          if (strlen(WiFi.SSID().c_str())) {
            SettingsUpdateText(SET_STASSID1, WiFi.SSID().c_str());
          }
          if (strlen(WiFi.psk().c_str())) {
            SettingsUpdateText(SET_STAPWD1, WiFi.psk().c_str());
          }
          Settings.sta_active = 0;
          AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_WCFG_2_WIFIMANAGER D_CMND_SSID "1 %s"), SettingsText(SET_STASSID1));
        }
      }
      if (!Wifi.config_counter) {
//        SettingsSdkErase();  //  Disabled v6.1.0b due to possible bad wifi connects
        restart_flag = 2;
      }
    } else {
      if (Wifi.scan_state) { WifiBeginAfterScan(); }

      if (Wifi.counter <= 0) {
        AddLog_P(LOG_LEVEL_DEBUG_MORE, S_LOG_WIFI, PSTR(D_CHECKING_CONNECTION));
        Wifi.counter = WIFI_CHECK_SEC;
        WifiCheckIp();
      }
#if LWIP_IPV6
      if (WifiCheckIPAddrStatus()) {
#else
      if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0) && !Wifi.config_type) {
#endif  // LWIP_IPV6=1
        WifiSetState(1);

        if (Settings.flag3.use_wifi_rescan) {  // SetOption57 - Scan wifi network every 44 minutes for configured AP's
          if (!(uptime % (60 * WIFI_RESCAN_MINUTES))) {
            Wifi.scan_state = 2;
          }
        }

#ifdef FIRMWARE_MINIMAL
        if (1 == RtcSettings.ota_loader) {
          RtcSettings.ota_loader = 0;
          ota_state_flag = 3;
        }
#endif  // FIRMWARE_MINIMAL

#ifdef USE_DISCOVERY
        if (Settings.flag3.mdns_enabled) {  // SetOption55 - Control mDNS service
          if (!Wifi.mdns_begun) {
//            if (mdns_delayed_start) {
//              AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_ATTEMPTING_CONNECTION));
//              mdns_delayed_start--;
//            } else {
//              mdns_delayed_start = Settings.param[P_MDNS_DELAYED_START];
              Wifi.mdns_begun = (uint8_t)MDNS.begin(my_hostname);
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS "%s"), (Wifi.mdns_begun) ? D_INITIALIZED : D_FAILED);
//            }
          }
        }
#endif  // USE_DISCOVERY

#ifdef USE_WEBSERVER
        if (Settings.webserver) {
          StartWebserver(Settings.webserver, WiFi.localIP());
#ifdef USE_DISCOVERY
#ifdef WEBSERVER_ADVERTISE
          if (1 == Wifi.mdns_begun) {
            Wifi.mdns_begun = 2;
            MDNS.addService("http", "tcp", WEB_PORT);
          }
#endif  // WEBSERVER_ADVERTISE
#endif  // USE_DISCOVERY
        } else {
          StopWebserver();
        }
#ifdef USE_EMULATION
        if (Settings.flag2.emulation) { UdpConnect(); }
#endif  // USE_EMULATION
#endif  // USE_WEBSERVER

#ifdef USE_KNX
        if (!knx_started && Settings.flag.knx_enabled) {  // CMND_KNX_ENABLED
          KNXStart();
          knx_started = true;
        }
#endif  // USE_KNX

      } else {
        WifiSetState(0);
#ifdef USE_EMULATION
        UdpDisconnect();
#endif  // USE_EMULATION
        Wifi.mdns_begun = 0;
#ifdef USE_KNX
        knx_started = false;
#endif  // USE_KNX
      }
    }
  }
}

int WifiState(void)
{
  int state = -1;

  if (!global_state.wifi_down) { state = WIFI_RESTART; }
  if (Wifi.config_type) { state = Wifi.config_type; }
  return state;
}

void WifiSetOutputPower(void)
{
  WiFi.setOutputPower((float)(Settings.wifi_output_power) / 10);
}

void WifiConnect(void)
{
  WifiSetState(0);
  WifiSetOutputPower();
  WiFi.persistent(false);     // Solve possible wifi init errors
  Wifi.status = 0;
  Wifi.retry_init = WIFI_RETRY_OFFSET_SEC + ((ESP.getChipId() & 0xF) * 2);
  Wifi.retry = Wifi.retry_init;
  Wifi.counter = 1;
}

// Enable from 6.0.0a until 6.1.0a - disabled due to possible cause of bad wifi connect on core 2.3.0
// Re-enabled from 6.3.0.7 with ESP.restart replaced by ESP.reset
void WifiDisconnect(void)
{
  // Courtesy of EspEasy
  WiFi.persistent(true);      // use SDK storage of SSID/WPA parameters
  ETS_UART_INTR_DISABLE();
  wifi_station_disconnect();  // this will store empty ssid/wpa into sdk storage
  ETS_UART_INTR_ENABLE();
  WiFi.persistent(false);     // Do not use SDK storage of SSID/WPA parameters
}

void WifiShutdown(void)
{
  delay(100);                 // Allow time for message xfer - disabled v6.1.0b
  if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    MqttDisconnect();
  }
  WifiDisconnect();
}

void EspRestart(void)
{
  WifiShutdown();
  CrashDumpClear();           // Clear the stack dump in RTC
//  ESP.restart();            // This results in exception 3 on restarts on core 2.3.0
  ESP.reset();
}
