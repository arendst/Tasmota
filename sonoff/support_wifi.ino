/*
  support_wifi.ino - wifi support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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
#define WIFI_RSSI_THRESHOLD    10   // Difference in dB between current network and scanned network
#endif
#ifndef WIFI_RESCAN_MINUTES
#define WIFI_RESCAN_MINUTES    44   // Number of minutes between wifi network rescan
#endif

#define WIFI_CONFIG_SEC        180  // seconds before restart
#define WIFI_CHECK_SEC         20   // seconds
#define WIFI_RETRY_OFFSET_SEC  20   // seconds

#include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager

uint8_t wifi_counter;
uint8_t wifi_retry_init;
uint8_t wifi_retry;
uint8_t wifi_status;
uint8_t wps_result;
uint8_t wifi_config_type = 0;
uint8_t wifi_config_counter = 0;

uint8_t wifi_scan_state;
uint8_t wifi_bssid[6];

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

boolean WifiConfigCounter(void)
{
  if (wifi_config_counter) {
    wifi_config_counter = WIFI_CONFIG_SEC;
  }
  return (wifi_config_counter);
}

extern "C" {
#include "user_interface.h"
}

void WifiWpsStatusCallback(wps_cb_status status);

void WifiWpsStatusCallback(wps_cb_status status)
{
/* from user_interface.h:
  enum wps_cb_status {
    WPS_CB_ST_SUCCESS = 0,
    WPS_CB_ST_FAILED,
    WPS_CB_ST_TIMEOUT,
    WPS_CB_ST_WEP,      // WPS failed because that WEP is not supported
    WPS_CB_ST_SCAN_ERR, // can not find the target WPS AP
  };
*/
  wps_result = status;
  if (WPS_CB_ST_SUCCESS == wps_result) {
    wifi_wps_disable();
  } else {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_WPS_FAILED_WITH_STATUS " %d"), wps_result);
    AddLog(LOG_LEVEL_DEBUG);
    wifi_config_counter = 2;
  }
}

boolean WifiWpsConfigDone(void)
{
  return (!wps_result);
}

boolean WifiWpsConfigBegin(void)
{
  wps_result = 99;
  if (!wifi_wps_disable()) { return false; }
  if (!wifi_wps_enable(WPS_TYPE_PBC)) { return false; }  // so far only WPS_TYPE_PBC is supported (SDK 2.0.0)
  if (!wifi_set_wps_cb((wps_st_cb_t) &WifiWpsStatusCallback)) { return false; }
  if (!wifi_wps_start()) { return false; }
  return true;
}

void WifiConfig(uint8_t type)
{
  if (!wifi_config_type) {
    if ((WIFI_RETRY == type) || (WIFI_WAIT == type)) { return; }
#if defined(USE_WEBSERVER) && defined(USE_EMULATION)
    UdpDisconnect();
#endif  // USE_EMULATION
    WiFi.disconnect();                       // Solve possible Wifi hangs
    wifi_config_type = type;

#ifndef USE_WPS
    if (WIFI_WPSCONFIG == wifi_config_type) { wifi_config_type = WIFI_MANAGER; }
#endif  // USE_WPS
#ifndef USE_WEBSERVER
    if (WIFI_MANAGER == wifi_config_type) { wifi_config_type = WIFI_SMARTCONFIG; }
#endif  // USE_WEBSERVER
#ifndef USE_SMARTCONFIG
    if (WIFI_SMARTCONFIG == wifi_config_type) { wifi_config_type = WIFI_SERIAL; }
#endif  // USE_SMARTCONFIG

    wifi_config_counter = WIFI_CONFIG_SEC;   // Allow up to WIFI_CONFIG_SECS seconds for phone to provide ssid/pswd
    wifi_counter = wifi_config_counter +5;
    blinks = 1999;
    if (WIFI_RESTART == wifi_config_type) {
      restart_flag = 2;
    }
    else if (WIFI_SERIAL == wifi_config_type) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_6_SERIAL " " D_ACTIVE_FOR_3_MINUTES));
    }
#ifdef USE_SMARTCONFIG
    else if (WIFI_SMARTCONFIG == wifi_config_type) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_1_SMARTCONFIG " " D_ACTIVE_FOR_3_MINUTES));
      WiFi.beginSmartConfig();
    }
#endif  // USE_SMARTCONFIG
#ifdef USE_WPS
    else if (WIFI_WPSCONFIG == wifi_config_type) {
      if (WifiWpsConfigBegin()) {
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_3_WPSCONFIG " " D_ACTIVE_FOR_3_MINUTES));
      } else {
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_3_WPSCONFIG " " D_FAILED_TO_START));
        wifi_config_counter = 3;
      }
    }
#endif  // USE_WPS
#ifdef USE_WEBSERVER
    else if (WIFI_MANAGER == wifi_config_type) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_2_WIFIMANAGER " " D_ACTIVE_FOR_3_MINUTES));
      WifiManagerBegin();
    }
#endif  // USE_WEBSERVER
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
 * See https://github.com/arendst/Sonoff-Tasmota/issues/2559
 */

// Sleep explanation: https://github.com/esp8266/Arduino/blob/3f0c601cfe81439ce17e9bd5d28994a7ed144482/libraries/ESP8266WiFi/src/ESP8266WiFiGeneric.cpp#L255
#if defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
#else  // Enabled in 2.3.0, 2.4.0 and stage
  if (sleep && Settings.flag3.sleep_normal) {
    WiFi.setSleepMode(WIFI_LIGHT_SLEEP);  // Allow light sleep during idle times
  } else {
    WiFi.setSleepMode(WIFI_MODEM_SLEEP);  // Disable sleep (Esp8288/Arduino core and sdk default)
  }
#endif
}

void WifiBegin(uint8_t flag, uint8_t channel)
{
  const char kWifiPhyMode[] = " BGN";

#if defined(USE_WEBSERVER) && defined(USE_EMULATION)
  UdpDisconnect();
#endif  // USE_EMULATION

#ifdef ARDUINO_ESP8266_RELEASE_2_3_0  // (!strncmp_P(ESP.getSdkVersion(),PSTR("1.5.3"),5))
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, PSTR(D_PATCH_ISSUE_2186));
  WiFi.mode(WIFI_OFF);      // See https://github.com/esp8266/Arduino/issues/2186
#endif

  WiFi.persistent(false);   // Solve possible wifi init errors (re-add at 6.2.1.16 #4044, #4083)
  WiFi.disconnect(true);    // Delete SDK wifi config
  delay(200);
  WiFi.mode(WIFI_STA);      // Disable AP mode
  WiFiSetSleepMode();
//  if (WiFi.getPhyMode() != WIFI_PHY_MODE_11N) { WiFi.setPhyMode(WIFI_PHY_MODE_11N); }
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
  if ('\0' == Settings.sta_ssid[Settings.sta_active][0]) { Settings.sta_active ^= 1; }  // Skip empty SSID
  if (Settings.ip_address[0]) {
    WiFi.config(Settings.ip_address[0], Settings.ip_address[1], Settings.ip_address[2], Settings.ip_address[3]);  // Set static IP
  }
  WiFi.hostname(my_hostname);
  if (channel) {
    WiFi.begin(Settings.sta_ssid[Settings.sta_active], Settings.sta_pwd[Settings.sta_active], channel, wifi_bssid);
  } else {
    WiFi.begin(Settings.sta_ssid[Settings.sta_active], Settings.sta_pwd[Settings.sta_active]);
  }
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_CONNECTING_TO_AP "%d %s " D_IN_MODE " 11%c " D_AS " %s..."),
    Settings.sta_active +1, Settings.sta_ssid[Settings.sta_active], kWifiPhyMode[WiFi.getPhyMode() & 0x3], my_hostname);
  AddLog(LOG_LEVEL_INFO);
}

void WifiBeginAfterScan()
{
  static int8_t best_network_db;

  // Not active
  if (0 == wifi_scan_state) { return; }
  // Init scan when not connected
  if (1 == wifi_scan_state) {
    memset((void*) &wifi_bssid, 0, sizeof(wifi_bssid));
    best_network_db = -127;
    wifi_scan_state = 3;
  }
  // Init scan when connected
  if (2 == wifi_scan_state) {
    uint8_t* bssid = WiFi.BSSID();                  // Get current bssid
    memcpy((void*) &wifi_bssid, (void*) bssid, sizeof(wifi_bssid));
    best_network_db = WiFi.RSSI();                  // Get current rssi and add threshold
    if (best_network_db < -WIFI_RSSI_THRESHOLD) { best_network_db += WIFI_RSSI_THRESHOLD; }
    wifi_scan_state = 3;
  }
  // Init scan
  if (3 == wifi_scan_state) {
    if (WiFi.scanComplete() != WIFI_SCAN_RUNNING) {
      WiFi.scanNetworks(true);                      // Start wifi scan async
      wifi_scan_state++;
      AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, PSTR("Network (re)scan started..."));
      return;
    }
  }
  int8_t wifi_scan_result = WiFi.scanComplete();
  // Check scan done
  if (4 == wifi_scan_state) {
    if (wifi_scan_result != WIFI_SCAN_RUNNING) {
      wifi_scan_state++;
    }
  }
  // Scan done
  if (5 == wifi_scan_state) {
    int32_t channel = 0;                            // No scan result
    int8_t ap = 3;                                  // AP default if not found
    uint8_t last_bssid[6];                          // Save last bssid
    memcpy((void*) &last_bssid, (void*) &wifi_bssid, sizeof(last_bssid));

    if (wifi_scan_result > 0) {
      // Networks found
      for (int8_t i = 0; i < wifi_scan_result; ++i) {

        String ssid_scan;
        int32_t rssi_scan;
        uint8_t sec_scan;
        uint8_t* bssid_scan;
        int32_t chan_scan;
        bool hidden_scan;

        WiFi.getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, bssid_scan, chan_scan, hidden_scan);

        bool known = false;
        uint8_t j;
        for (j = 0; j < 2; j++) {
          if (ssid_scan == Settings.sta_ssid[j]) {  // SSID match
            known = true;
            if (rssi_scan > best_network_db) {      // Best network
              if (sec_scan == ENC_TYPE_NONE || Settings.sta_pwd[j]) {  // Check for passphrase if not open wlan
                best_network_db = (int8_t)rssi_scan;
                channel = chan_scan;
                ap = j;                             // AP1 or AP2
                memcpy((void*) &wifi_bssid, (void*) bssid_scan, sizeof(wifi_bssid));
              }
            }
            break;
          }
        }
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI "Network %d, AP%c, SSId %s, Channel %d, BSSId %02X:%02X:%02X:%02X:%02X:%02X, RSSI %d, Encryption %d"),
          i, (known) ? (j) ? '2' : '1' : '-', ssid_scan.c_str(), chan_scan, bssid_scan[0], bssid_scan[1], bssid_scan[2], bssid_scan[3], bssid_scan[4], bssid_scan[5], rssi_scan, (sec_scan == ENC_TYPE_NONE) ? 0 : 1);
        AddLog(LOG_LEVEL_DEBUG);
        delay(0);
      }
      WiFi.scanDelete();                            // Clean up Ram
      delay(0);
    }
    wifi_scan_state = 0;
    // If bssid changed then (re)connect wifi
    for (uint8_t i = 0; i < sizeof(wifi_bssid); i++) {
      if (last_bssid[i] != wifi_bssid[i]) {
        WifiBegin(ap, channel);                     // 0 (AP1), 1 (AP2) or 3 (default AP)
        break;
      }
    }
  }
}

void WifiSetState(uint8_t state)
{
  if (state == global_state.wifi_down) {
    if (state) {
      rules_flag.wifi_connected = 1;
    } else {
      rules_flag.wifi_disconnected = 1;
    }
  }
  global_state.wifi_down = state ^1;
}

void WifiCheckIp(void)
{
  if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    WifiSetState(1);
    wifi_counter = WIFI_CHECK_SEC;
    wifi_retry = wifi_retry_init;
    AddLog_P((wifi_status != WL_CONNECTED) ? LOG_LEVEL_INFO : LOG_LEVEL_DEBUG_MORE, S_LOG_WIFI, PSTR(D_CONNECTED));
    if (wifi_status != WL_CONNECTED) {
//      AddLog_P(LOG_LEVEL_INFO, PSTR("Wifi: Set IP addresses"));
      Settings.ip_address[1] = (uint32_t)WiFi.gatewayIP();
      Settings.ip_address[2] = (uint32_t)WiFi.subnetMask();
      Settings.ip_address[3] = (uint32_t)WiFi.dnsIP();
    }
    wifi_status = WL_CONNECTED;
  } else {
    WifiSetState(0);
    uint8_t wifi_config_tool = Settings.sta_config;
    wifi_status = WiFi.status();
    switch (wifi_status) {
      case WL_CONNECTED:
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_NO_IP_ADDRESS));
        wifi_status = 0;
        wifi_retry = wifi_retry_init;
        break;
      case WL_NO_SSID_AVAIL:
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_AP_NOT_REACHED));
        if (WIFI_WAIT == Settings.sta_config) {
          wifi_retry = wifi_retry_init;
        } else {
          if (wifi_retry > (wifi_retry_init / 2)) {
            wifi_retry = wifi_retry_init / 2;
          }
          else if (wifi_retry) {
            wifi_retry = 0;
          }
        }
        break;
      case WL_CONNECT_FAILED:
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_WRONG_PASSWORD));
        if (wifi_retry > (wifi_retry_init / 2)) {
          wifi_retry = wifi_retry_init / 2;
        }
        else if (wifi_retry) {
          wifi_retry = 0;
        }
        break;
      default:  // WL_IDLE_STATUS and WL_DISCONNECTED
        if (!wifi_retry || ((wifi_retry_init / 2) == wifi_retry)) {
          AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_AP_TIMEOUT));
        } else {
          if (('\0' == Settings.sta_ssid[0][0]) && ('\0' == Settings.sta_ssid[1][0])) {
            wifi_config_tool = WIFI_CONFIG_NO_SSID;    // Skip empty SSIDs and start Wifi config tool
            wifi_retry = 0;
          } else {
            AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, PSTR(D_ATTEMPTING_CONNECTION));
          }
        }
    }
    if (wifi_retry) {
      if (Settings.flag3.use_wifi_scan) {
        if (wifi_retry_init == wifi_retry) {
          wifi_scan_state = 1;    // Select scanned SSID
        }
      } else {
        if (wifi_retry_init == wifi_retry) {
          WifiBegin(3, 0);        // Select default SSID
        }
        if ((Settings.sta_config != WIFI_WAIT) && ((wifi_retry_init / 2) == wifi_retry)) {
          WifiBegin(2, 0);        // Select alternate SSID
        }
      }
      wifi_counter = 1;
      wifi_retry--;
    } else {
      WifiConfig(wifi_config_tool);
      wifi_counter = 1;
      wifi_retry = wifi_retry_init;
    }
  }
}

void WifiCheck(uint8_t param)
{
  wifi_counter--;
  switch (param) {
  case WIFI_SERIAL:
  case WIFI_SMARTCONFIG:
  case WIFI_MANAGER:
  case WIFI_WPSCONFIG:
    WifiConfig(param);
    break;
  default:
    if (wifi_config_counter) {
      wifi_config_counter--;
      wifi_counter = wifi_config_counter +5;
      if (wifi_config_counter) {
#ifdef USE_SMARTCONFIG
        if ((WIFI_SMARTCONFIG == wifi_config_type) && WiFi.smartConfigDone()) {
          wifi_config_counter = 0;
        }
#endif  // USE_SMARTCONFIG
#ifdef USE_WPS
        if ((WIFI_WPSCONFIG == wifi_config_type) && WifiWpsConfigDone()) {
          wifi_config_counter = 0;
        }
#endif  // USE_WPS
        if (!wifi_config_counter) {
          if (strlen(WiFi.SSID().c_str())) {
            strlcpy(Settings.sta_ssid[0], WiFi.SSID().c_str(), sizeof(Settings.sta_ssid[0]));
          }
          if (strlen(WiFi.psk().c_str())) {
            strlcpy(Settings.sta_pwd[0], WiFi.psk().c_str(), sizeof(Settings.sta_pwd[0]));
          }
          Settings.sta_active = 0;
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_WCFG_1_SMARTCONFIG D_CMND_SSID "1 %s"), Settings.sta_ssid[0]);
          AddLog(LOG_LEVEL_INFO);
        }
      }
      if (!wifi_config_counter) {
#ifdef USE_SMARTCONFIG
        if (WIFI_SMARTCONFIG == wifi_config_type) { WiFi.stopSmartConfig(); }
#endif  // USE_SMARTCONFIG
//        SettingsSdkErase();  //  Disabled v6.1.0b due to possible bad wifi connects
        restart_flag = 2;
      }
    } else {
      if (wifi_scan_state) { WifiBeginAfterScan(); }

      if (wifi_counter <= 0) {
        AddLog_P(LOG_LEVEL_DEBUG_MORE, S_LOG_WIFI, PSTR(D_CHECKING_CONNECTION));
        wifi_counter = WIFI_CHECK_SEC;
        WifiCheckIp();
      }
      if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0) && !wifi_config_type) {
        WifiSetState(1);

        if (Settings.flag3.use_wifi_rescan) {
          if (!(uptime % (60 * WIFI_RESCAN_MINUTES))) {
            wifi_scan_state = 2;
          }
        }

#ifdef BE_MINIMAL
        if (1 == RtcSettings.ota_loader) {
          RtcSettings.ota_loader = 0;
          ota_state_flag = 3;
        }
#endif  // BE_MINIMAL

#ifdef USE_DISCOVERY
        if (!mdns_begun) {
          if (mdns_delayed_start) {
            AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_ATTEMPTING_CONNECTION));
            mdns_delayed_start--;
          } else {
            mdns_delayed_start = Settings.param[P_MDNS_DELAYED_START];
            mdns_begun = MDNS.begin(my_hostname);
            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MDNS "%s"), (mdns_begun) ? D_INITIALIZED : D_FAILED);
            AddLog(LOG_LEVEL_INFO);
          }
        }
#endif  // USE_DISCOVERY

#ifdef USE_WEBSERVER
        if (Settings.webserver) {
          StartWebserver(Settings.webserver, WiFi.localIP());
#ifdef USE_DISCOVERY
#ifdef WEBSERVER_ADVERTISE
          if (mdns_begun) {
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
        if (!knx_started && Settings.flag.knx_enabled) {
          KNXStart();
          knx_started = true;
        }
#endif  // USE_KNX

      } else {
        WifiSetState(0);
#if defined(USE_WEBSERVER) && defined(USE_EMULATION)
        UdpDisconnect();
#endif  // USE_EMULATION
        mdns_begun = false;
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
  if (wifi_config_type) { state = wifi_config_type; }
  return state;
}

void WifiConnect(void)
{
  WifiSetState(0);
  WiFi.persistent(false);     // Solve possible wifi init errors
  wifi_status = 0;
  wifi_retry_init = WIFI_RETRY_OFFSET_SEC + ((ESP.getChipId() & 0xF) * 2);
  wifi_retry = wifi_retry_init;
  wifi_counter = 1;
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

void EspRestart(void)
{
  delay(100);                 // Allow time for message xfer - disabled v6.1.0b
  if (Settings.flag.mqtt_enabled) MqttDisconnect();
  WifiDisconnect();
//  ESP.restart();            // This results in exception 3 on restarts on core 2.3.0
  ESP.reset();
}

/*
void EspRestart(void)
{
  ESP.restart();
}
*/

void WifiAddDelayWhenDisconnected(void)
{
  if (APP_BAUDRATE == baudrate) {  // When baudrate too low it will fail on Sonoff Pow R2 and S31 serial interface initialization
    if (global_state.wifi_down) {
      delay(DRIVER_BOOT_DELAY);
    }
  }
}
