/*
  support_wifi.ino - wifi support for Tasmota

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
 * Wifi
\*********************************************************************************************/

// Enable one of three below options for wifi re-connection debugging
//#define WIFI_FORCE_RF_CAL_ERASE            // Erase rf calibration sector on restart only
//#define WIFI_RF_MODE_RF_CAL                // Set RF_MODE to RF_CAL for restart and deepsleep during user_rf_pre_init
//#define WIFI_RF_PRE_INIT                   // Set RF_MODE to RF_CAL for restart, deepsleep and power on during user_rf_pre_init

#ifndef WIFI_RSSI_THRESHOLD
#define WIFI_RSSI_THRESHOLD     10         // Difference in dB between current network and scanned network
#endif
#ifndef WIFI_RESCAN_MINUTES
#define WIFI_RESCAN_MINUTES     44         // Number of minutes between wifi network rescan
#endif
#ifndef WIFI_RETRY_SECONDS
#define WIFI_RETRY_SECONDS      20         // Number of seconds connection to wifi network will retry
#endif

const uint8_t WIFI_CONFIG_SEC = 180;       // seconds before restart
const uint8_t WIFI_CHECK_SEC = 20;         // seconds
const uint8_t WIFI_RETRY_OFFSET_SEC = WIFI_RETRY_SECONDS;  // seconds

#include <ESP8266WiFi.h>                   // Wifi, MQTT, Ota, WifiManager
#include "lwip/dns.h"
#ifdef ESP32
  #include "esp_netif.h"
#endif  // ESP32

/**
 * Converts WiFi RSSI (signal strength) to a quality percentage
 * 
 * @param rssi The RSSI value in dBm (typically negative, e.g. -70)
 * @return Quality as a percentage (0-100)
 * 
 * The function maps RSSI values to a percentage scale:
 * - RSSI <= -100 dBm: 0% quality (very poor/no signal)
 * - RSSI >= -50 dBm: 100% quality (excellent signal)
 * - Values in between are linearly mapped (each 2.5 dBm = 5%)
 */
int WifiGetRssiAsQuality(int rssi) {
  if (rssi <= -100) { return 0; }
  if (rssi >= -50)  { return 100; }
  return 2 * (rssi + 100);
}

//                                           0    1   2       3        4
const char kWifiEncryptionTypes[] PROGMEM = "OPEN|WEP|WPA/PSK|WPA2/PSK|WPA/WPA2/PSK"
#ifdef ESP32
//                                            5               6        7             8
                                            "|WPA2-Enterprise|WPA3/PSK|WPA2/WPA3/PSK|WAPI/PSK"
#endif  // ESP32
;

/**
 * Returns a string representation of the WiFi encryption type
 * 
 * @param i Index of the network in the WiFi scan results
 * @return String containing the encryption type (e.g., "WPA2/PSK")
 * 
 * The function maps the encryption type values from WiFi.encryptionType() to
 * human-readable strings defined in kWifiEncryptionTypes.
 * 
 * ESP8266 and ESP32 use different encryption type enumerations, so this function
 * normalizes them to a consistent set of values.
 */
String WifiEncryptionType(uint32_t i) {
#ifdef ESP8266
  // Reference. WiFi.encryptionType =
  // 2 : ENC_TYPE_TKIP - WPA / PSK
  // 4 : ENC_TYPE_CCMP - WPA2 / PSK
  // 5 : ENC_TYPE_WEP  - WEP
  // 7 : ENC_TYPE_NONE - open network
  // 8 : ENC_TYPE_AUTO - WPA / WPA2 / PSK
  uint8_t typea[] = { 0,2,0,3,1,0,0,4 };
  int type = typea[WiFi.encryptionType(i) -1 &7];
#else
  int type = WiFi.encryptionType(i);
#endif
  if ((type < 0) || (type > 8)) { type = 0; }
  char stemp1[20];
  GetTextIndexed(stemp1, sizeof(stemp1), type, kWifiEncryptionTypes);
  return stemp1;
}

/**
 * Manages the WiFi configuration timeout counter
 * 
 * @return Current state of the WiFi configuration counter (true if active, false if not)
 * 
 * If the WiFi configuration counter is active, this function resets it to the maximum
 * value (WIFI_CONFIG_SEC). This extends the time available for configuration before
 * the device automatically restarts.
 * 
 * The function is typically called during user interaction with WiFi configuration
 * to prevent timeout while the user is actively configuring.
 */
bool WifiConfigCounter(void)
{
  if (Wifi.config_counter) {
    Wifi.config_counter = WIFI_CONFIG_SEC;
  }
  return (Wifi.config_counter);
}

/**
 * Initiates a WiFi configuration mode
 * 
 * @param type The configuration mode to activate (from enum WifiConfigModes)
 * 
 * This function handles the transition to different WiFi configuration modes:
 * - WIFI_RESTART: Triggers a device restart
 * - WIFI_SERIAL: Enables configuration via serial for 3 minutes
 * - WIFI_MANAGER/WIFI_MANAGER_RESET_ONLY: Activates the WiFi manager web interface
 * 
 * The function sets up a timeout counter (Wifi.config_counter) that will trigger
 * appropriate actions when it expires. It also disconnects from any current WiFi
 * connection before changing modes.
 * 
 * Error handling:
 * - Ignores requests for WIFI_RETRY or WIFI_WAIT if already in configuration mode
 * - Falls back to WIFI_SERIAL if WIFI_MANAGER is requested but webserver is disabled
 */
void WifiConfig(uint8_t type)
{
  if (!Wifi.config_type) {
    if ((WIFI_RETRY == type) || (WIFI_WAIT == type)) { return; }
#ifdef USE_EMULATION
    UdpDisconnect();
#endif  // USE_EMULATION
    WiFi.disconnect();                       // Solve possible Wifi hangs
    delay(100);
    Wifi.config_type = type;

#ifndef USE_WEBSERVER
    if (WIFI_MANAGER == Wifi.config_type) {
      Wifi.config_type = WIFI_SERIAL;
    }
#endif  // USE_WEBSERVER

    Wifi.config_counter = WIFI_CONFIG_SEC;   // Allow up to WIFI_CONFIG_SECS seconds for phone to provide ssid/pswd
    Wifi.counter = Wifi.config_counter +5;
    TasmotaGlobal.blinks = 255;
    if (WIFI_RESTART == Wifi.config_type) {
      TasmotaGlobal.restart_flag = 2;
    }
    else if (WIFI_SERIAL == Wifi.config_type) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_WCFG_6_SERIAL " " D_ACTIVE_FOR_3_MINUTES));
    }
#ifdef USE_WEBSERVER
    else if (WIFI_MANAGER == Wifi.config_type || WIFI_MANAGER_RESET_ONLY == Wifi.config_type) {
      WifiManagerBegin(WIFI_MANAGER_RESET_ONLY == Wifi.config_type);
    }
#endif  // USE_WEBSERVER
  }
}

#ifdef CONFIG_IDF_TARGET_ESP32C3
  // https://github.com/espressif/arduino-esp32/issues/6264#issuecomment-1040147331
  // There's an include for this but it doesn't define the function if it doesn't think it needs it, so manually declare the function
extern "C" void phy_bbpll_en_usb(bool en);
#endif  // CONFIG_IDF_TARGET_ESP32C3

/**
 * Sets the WiFi operating mode with proper handling for different ESP platforms
 * 
 * @param wifi_mode The WiFi mode to set (WIFI_OFF, WIFI_STA, WIFI_AP, WIFI_AP_STA)
 * 
 * This function handles platform-specific requirements when changing WiFi modes:
 * - For ESP32-C3: Enables USB serial-jtag after WiFi startup
 * - Ensures the hostname is set before mode changes
 * - Handles proper sleep/wake transitions for power management
 * 
 * The function includes retry logic if setting the mode fails on the first attempt.
 * For WIFI_OFF mode, it properly puts the WiFi into deep sleep to save power.
 * 
 * Error handling:
 * - Retries mode setting up to 2 times if it fails
 * - Adds delay between attempts to allow hardware to stabilize
 */
void WifiSetMode(WiFiMode_t wifi_mode) {
#ifdef CONFIG_IDF_TARGET_ESP32C3
  // https://github.com/espressif/arduino-esp32/issues/6264#issuecomment-1094376906
  // This brings the USB serial-jtag back to life. Suggest doing this immediately after wifi startup.
  phy_bbpll_en_usb(true);
#endif  // CONFIG_IDF_TARGET_ESP32C3
  if (WiFi.getMode() == wifi_mode) { return; }

  if (wifi_mode != WIFI_OFF) {
    WiFi.hostname(TasmotaGlobal.hostname);  // ESP32 needs this here (before WiFi.mode) for core 2.0.0

    // See: https://github.com/esp8266/Arduino/issues/6172#issuecomment-500457407
    WiFiHelper::forceSleepWake(); // Make sure WiFi is really active.
  }

  uint32_t retry = 2;
  while (!WiFi.mode(wifi_mode) && retry--) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "Retry set Mode..."));
    delay(100);
  }

  if (wifi_mode == WIFI_OFF) {
    delay(1000);
    WiFiHelper::forceSleepBegin();
  }
  delay(100);  // Must allow for some time to init.
}

/**
 * Configures the WiFi sleep mode based on system settings
 * 
 * This function sets the appropriate WiFi sleep mode to balance power consumption
 * and network responsiveness according to user settings:
 * 
 * - WIFI_NONE_SLEEP: No sleep (highest power consumption, fastest response)
 * - WIFI_LIGHT_SLEEP: Light sleep during idle times (medium power saving)
 * - WIFI_MODEM_SLEEP: Default sleep mode (moderate power saving)
 * 
 * The sleep mode is determined by:
 * - TasmotaGlobal.sleep: Global sleep setting
 * - Settings->flag5.wifi_no_sleep: Option to disable sleep
 * - Settings->flag3.sleep_normal: SetOption60 - Use normal sleep instead of dynamic sleep
 * - TasmotaGlobal.wifi_stay_asleep: Flag to maintain sleep state
 * 
 * Note: Sleep modes affect power consumption and network responsiveness.
 * Some ESP32 variants may have specific sleep behavior requirements.
 */
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
/*
  if (TasmotaGlobal.sleep && Settings->flag3.sleep_normal) {  // SetOption60 - Enable normal sleep instead of dynamic sleep
    WiFiHelper::setSleepMode(WIFI_LIGHT_SLEEP);        // Allow light sleep during idle times
  } else {
    WiFiHelper::setSleepMode(WIFI_MODEM_SLEEP);        // Disable sleep (Esp8288/Arduino core and sdk default)
  }
*/
  bool wifi_no_sleep = Settings->flag5.wifi_no_sleep;
//#ifdef CONFIG_IDF_TARGET_ESP32C3
//  wifi_no_sleep = true;                         // Temporary patch for IDF4.4, wifi sleeping may cause wifi drops
//#endif
  if (0 == TasmotaGlobal.sleep || wifi_no_sleep) {
    if (!TasmotaGlobal.wifi_stay_asleep) {
      WiFiHelper::setSleepMode(WIFI_NONE_SLEEP);       // Disable sleep
    }
  } else {
    if (Settings->flag3.sleep_normal) {         // SetOption60 - Enable normal sleep instead of dynamic sleep
      WiFiHelper::setSleepMode(WIFI_LIGHT_SLEEP);      // Allow light sleep during idle times
    } else {
      WiFiHelper::setSleepMode(WIFI_MODEM_SLEEP);      // Sleep (Esp8288/Arduino core and sdk default)
    }
  }
  delay(100);
}

/**
 * Initiates a WiFi connection with the specified parameters
 * 
 * @param flag WiFi AP selection: 0=AP1, 1=AP2, 2=Toggle between APs, 3=Current AP
 * @param channel Optional WiFi channel to connect on (0 for auto)
 * 
 * This function handles the WiFi connection process:
 * 1. Disconnects from any current connections
 * 2. Sets the WiFi mode to station mode
 * 3. Configures sleep mode and power settings
 * 4. Attempts to connect to the selected access point
 * 
 * The function supports multiple connection scenarios:
 * - Connecting to a specific AP (primary or backup)
 * - Toggling between configured APs
 * - Connecting to a specific channel and BSSID for multi-AP installations
 * - Using static IP configuration if specified in settings
 * 
 * Error handling:
 * - Skips empty SSIDs by toggling to the alternate AP
 * - Logs connection details for troubleshooting
 * - Optionally waits for connection result based on settings
 */
void WifiBegin(uint8_t flag, uint8_t channel) {
#ifdef USE_EMULATION
  UdpDisconnect();
#endif  // USE_EMULATION

  WiFi.persistent(false);   // Solve possible wifi init errors (re-add at 6.2.1.16 #4044, #4083)
#if defined(USE_IPV6) && defined(ESP32)
  WiFi.enableIPv6(true);
#endif

#ifdef USE_WIFI_RANGE_EXTENDER
  if (WiFi.getMode() != WIFI_AP_STA || !RgxApUp()) {  // Preserve range extender connections (#17103)
#endif  // USE_WIFI_RANGE_EXTENDER
  WiFi.disconnect(true);  // Delete SDK wifi config
  delay(200);
  WifiSetMode(WIFI_STA);  // Disable AP mode
#ifdef USE_WIFI_RANGE_EXTENDER
  }
#endif  // USE_WIFI_RANGE_EXTENDER

  WiFiSetSleepMode();
  WifiSetOutputPower();
//  if (WiFiHelper::getPhyMode() != WIFI_PHY_MODE_11N) { WiFiHelper::setPhyMode(WIFI_PHY_MODE_11N); }  // B/G/N
//  if (WiFiHelper::getPhyMode() != WIFI_PHY_MODE_11G) { WiFiHelper::setPhyMode(WIFI_PHY_MODE_11G); }  // B/G
#ifdef ESP32
  if (Wifi.phy_mode) {
    WiFiHelper::setPhyMode(WiFiPhyMode_t(Wifi.phy_mode));  // 1-B/2-BG/3-BGN/4-BGNAX
  }
#endif
  WiFi.setAutoReconnect(true);
  switch (flag) {
  case 0:  // AP1
  case 1:  // AP2
    Settings->sta_active = flag;
    break;
  case 2:  // Toggle
    Settings->sta_active ^= 1;
  }        // 3: Current AP
  if (!strlen(SettingsText(SET_STASSID1 + Settings->sta_active))) {
    Settings->sta_active ^= 1;  // Skip empty SSID
  }
  if (Settings->ipv4_address[0]) {
    // AddLog(LOG_LEVEL_INFO, ">>>1: Wifi Config DNS %_I %_I", Settings->ipv4_address[3], Settings->ipv4_address[4]);
    WiFi.config(Settings->ipv4_address[0], Settings->ipv4_address[1], Settings->ipv4_address[2], Settings->ipv4_address[3], Settings->ipv4_address[4]);  // Set static IP
  }
  WiFi.hostname(TasmotaGlobal.hostname);  // ESP8266 needs this here (after WiFi.mode)

  char stemp[40] = { 0 };
  if (channel) {
    WiFiHelper::begin(SettingsText(SET_STASSID1 + Settings->sta_active), SettingsText(SET_STAPWD1 + Settings->sta_active), channel, Wifi.bssid);
    // Add connected BSSID and channel for multi-AP installations
    char hex_char[18];
    snprintf_P(stemp, sizeof(stemp), PSTR(" Channel %d BSSId %s"), channel, ToHex_P((unsigned char*)Wifi.bssid, 6, hex_char, sizeof(hex_char), ':'));
  } else {
    WiFiHelper::begin(SettingsText(SET_STASSID1 + Settings->sta_active), SettingsText(SET_STAPWD1 + Settings->sta_active));
  }
  delay(500);
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECTING_TO_AP "%d %s%s " D_IN_MODE " %s " D_AS " %s..."),
    Settings->sta_active +1, SettingsText(SET_STASSID1 + Settings->sta_active), stemp, WifiGetPhyMode().c_str(), TasmotaGlobal.hostname);

  if (Settings->flag5.wait_for_wifi_result) {  // SetOption142 - (Wifi) Wait 1 second for wifi connection solving some FRITZ!Box modem issues (1)
    WiFi.waitForConnectResult(1000);  // https://github.com/arendst/Tasmota/issues/14985
  }

#ifdef CONFIG_ESP_WIFI_REMOTE_ENABLED
  HostedMCUStatus();
#endif  // CONFIG_ESP_WIFI_REMOTE_ENABLED
}

/**
 * Manages WiFi network scanning and connection based on scan results
 * 
 * This function implements a state machine for WiFi scanning operations:
 * - States 1-5: Network scanning for automatic connection
 * - States 6-69: Network scanning for the wifiscan command
 * 
 * For automatic connection (states 1-5):
 * 1. Initializes scan parameters
 * 2. Starts an asynchronous WiFi scan
 * 3. Processes scan results to find the best network
 * 4. Connects to the best available network
 * 
 * For wifiscan command (states 6-69):
 * 1. Performs a WiFi scan
 * 2. Formats and publishes scan results via MQTT
 * 3. Maintains scan results for 1 minute before cleanup
 * 
 * The function selects networks based on:
 * - Signal strength (RSSI)
 * - Match with configured SSIDs
 * - Security type (open networks require no password)
 * 
 * Error handling:
 * - Logs scan progress and results
 * - Handles scan failures gracefully
 * - Manages memory by cleaning up scan results
 */
void WifiBeginAfterScan(void)
{
  // Not active
  if (0 == Wifi.scan_state) { return; }
  // Init scan when not connected
  if (1 == Wifi.scan_state) {
    memset((void*) &Wifi.bssid, 0, sizeof(Wifi.bssid));
    Wifi.best_network_db = -127;
    Wifi.scan_state = 3;
  }
  // Init scan when connected
  if (2 == Wifi.scan_state) {
    uint8_t* bssid = WiFi.BSSID();                  // Get current bssid
    memcpy((void*) &Wifi.bssid, (void*) bssid, sizeof(Wifi.bssid));
    Wifi.best_network_db = WiFi.RSSI();             // Get current rssi and add threshold
    if (Wifi.best_network_db < -WIFI_RSSI_THRESHOLD) {
      Wifi.best_network_db += WIFI_RSSI_THRESHOLD;
    }
    Wifi.scan_state = 3;
  }
  // Init scan
  if (3 == Wifi.scan_state) {
    if (WiFi.scanComplete() != WIFI_SCAN_RUNNING) {
      WiFi.scanNetworks(true);                      // Start wifi scan async
      Wifi.scan_state++;
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "Network (re)scan started..."));
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

        WiFiHelper::getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, bssid_scan, chan_scan, hidden_scan);

        bool known = false;
        uint32_t j;
        for (j = 0; j < MAX_SSIDS; j++) {
          if (ssid_scan == SettingsText(SET_STASSID1 + j)) {  // SSID match
            known = true;
            if (rssi_scan > Wifi.best_network_db) {      // Best network
              if (sec_scan == ENC_TYPE_NONE || SettingsText(SET_STAPWD1 + j)) {  // Check for passphrase if not open wlan
                Wifi.best_network_db = (int8_t)rssi_scan;
                channel = chan_scan;
                ap = j;                             // AP1 or AP2
                memcpy((void*) &Wifi.bssid, (void*) bssid_scan, sizeof(Wifi.bssid));
              }
            }
            break;
          }
        }
        char hex_char[18];
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "Network %d, AP%c, SSId %s, Channel %d, BSSId %s, RSSI %d, Encryption %d"),
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

  // Init scan for wifiscan command
  if (6 == Wifi.scan_state) {
    if (wifi_scan_result != WIFI_SCAN_RUNNING) {
      WiFi.scanNetworks(true);                      // Start wifi scan async
      Wifi.scan_state++;
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "Network scan started..."));
      return;
    }
  }
  // Check scan done
  if (7 == Wifi.scan_state) {
    if (wifi_scan_result != WIFI_SCAN_RUNNING) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "Network scan finished..."));
      Wifi.scan_state++;
      return;
    }
  }
  // Scan done. Show SSId's scan result by MQTT and in console
  if (7 < Wifi.scan_state) {
    Wifi.scan_state++;

    ResponseClear();

    int32_t initial_item = (Wifi.scan_state - 9)*10;

    if ( wifi_scan_result > initial_item ) {
      // Sort networks by RSSI
      uint32_t indexes[wifi_scan_result];
      for (uint32_t i = 0; i < wifi_scan_result; i++) {
        indexes[i] = i;
      }
      for (uint32_t i = 0; i < wifi_scan_result; i++) {
        for (uint32_t j = i + 1; j < wifi_scan_result; j++) {
          if (WiFi.RSSI(indexes[j]) > WiFi.RSSI(indexes[i])) {
            std::swap(indexes[i], indexes[j]);
          }
        }
      }
      delay(0);

      // Publish the list
      uint32_t end_item = ( wifi_scan_result > initial_item + 10 ) ? initial_item + 10 : wifi_scan_result;
      for (uint32_t i = initial_item; i < end_item; i++) {
        Response_P(PSTR("{\"" D_CMND_WIFISCAN "\":{\"" D_STATUS5_NETWORK "%d\":{\"" D_SSID "\":\"%s\",\"" D_BSSID "\":\"%s\",\"" D_CHANNEL
                        "\":\"%d\",\"" D_JSON_SIGNAL "\":\"%d\",\"" D_RSSI "\":\"%d\",\"" D_JSON_ENCRYPTION "\":\"%s\"}}}"),
                        i+1,
                        WiFi.SSID(indexes[i]).c_str(),
                        WiFi.BSSIDstr(indexes[i]).c_str(),
                        WiFi.channel(indexes[i]),
                        WiFi.RSSI(indexes[i]),
                        WifiGetRssiAsQuality(WiFi.RSSI(indexes[i])),
                        WifiEncryptionType(indexes[i]).c_str());
        MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_CMND_WIFISCAN));
      }
    } else if (9 == Wifi.scan_state) {
      Response_P(PSTR("{\"" D_CMND_WIFISCAN "\":\"" D_NO_NETWORKS_FOUND "\"}"));
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_CMND_WIFISCAN));
    }
    delay(0);
  }
  // Wait 1 minute before cleaning the results so the user can ask for the them using wifiscan command (HTTP use-case)
  if (69 == Wifi.scan_state) {
    //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "Network scan results deleted..."));
    Wifi.scan_state = 0;
    WiFi.scanDelete();                            // Clean up Ram
  }

}

/**
 * Returns the number of successful WiFi connections since boot
 * 
 * @return Number of successful WiFi connections
 * 
 * This function provides access to the internal counter that tracks
 * how many times the device has successfully connected to WiFi networks.
 * The counter is incremented each time a connection is established.
 */
uint16_t WifiLinkCount(void)
{
  return Wifi.link_count;
}

/**
 * Returns the total time the device has been disconnected from WiFi
 * 
 * @return String representation of the total disconnected time
 * 
 * This function calculates the cumulative time the device has spent
 * without a WiFi connection since boot. The time is formatted as a
 * human-readable duration string (e.g., "1h 23m 45s").
 * 
 * The downtime is tracked by recording timestamps when disconnections
 * occur and calculating the difference when connections are restored.
 */
String WifiDowntime(void)
{
  return GetDuration(Wifi.downtime);
}

/**
 * Updates the WiFi connection state and triggers related events
 * 
 * @param state The new WiFi state (1 = connected, 0 = disconnected)
 * 
 * This function manages the WiFi connection state tracking:
 * 1. When connected (state=1):
 *    - Sets the wifi_connected rules flag
 *    - Increments the connection counter
 *    - Updates the total downtime
 * 2. When disconnected (state=0):
 *    - Sets the wifi_disconnected rules flag
 *    - Records the disconnection timestamp
 * 
 * The function also updates the global state variables:
 * - TasmotaGlobal.global_state.wifi_down (inverted state)
 * - TasmotaGlobal.global_state.network_down (cleared when WiFi is up)
 * 
 * This state tracking enables proper event handling and metrics for
 * WiFi connection reliability.
 */
void WifiSetState(uint8_t state)
{
  if (state == TasmotaGlobal.global_state.wifi_down) {
    if (state) {
      TasmotaGlobal.rules_flag.wifi_connected = 1;
      Wifi.link_count++;
      Wifi.downtime += UpTime() - Wifi.last_event;
    } else {
      TasmotaGlobal.rules_flag.wifi_disconnected = 1;
      Wifi.last_event = UpTime();
    }
  }
  TasmotaGlobal.global_state.wifi_down = state ^1;
  if (!TasmotaGlobal.global_state.wifi_down) {
    TasmotaGlobal.global_state.network_down = 0;
  }
}

/*****************************************************************************************************\
 * IP detection revised for full IPv4 / IPv6 support
 *
 * In general, each interface (Wifi/Eth) can have 1x IPv4 and
 * 2x IPv6 (Global routable address and Link-Local starting witn fe80:...)
 *
 * We always use an IPv4 address if one is assigned, and revert to
 * IPv6 only on networks that are v6 only.
 * Ethernet calls can be safely used even if the USE_ETHERNET is not enabled
 *
 * New APIs:
 * - general form is:
 *   `bool XXXGetIPYYY(IPAddress*)` returns `true` if the address exists and copies the address
 *                                  if the pointer is non-null.
 *   `bool XXXHasIPYYY()`           same as above but only returns `true` or `false`
 *   `String XXXGetIPYYYStr()`      returns the IP as a `String` or empty `String` if none
 *
 *   `XXX` can be `Wifi` or `Eth`
 *   `YYY` can be `` for any address, `v6` for IPv6 global address or `v6LinkLocal` for Link-local
 *
 * - Legacy `Wifi.localIP()` and `ETH.localIP()` always return IPv4 and nothing on IPv6 only networks
 *
 * - v4/v6:
 *   `WifiGetIP`, `WifiGetIPStr`, `WifiHasIP`: get preferred v4/v6 address for Wifi
 *   `EthernetGetIP`, `EthernetGetIPStr`, `EthernetHasIP`: get preferred v4/v6 for Ethernet
 *
 * - Main IP to be used dual stack v4/v6
 *   `hasIP`, `IPGetListeningAddress`, `IPGetListeningAddressStr`: any IP to listen to for Web Server
 *             IPv4 is always preferred, and Eth is preferred over Wifi.
 *   `IPForUrl`: converts v4/v6 to use in URL, enclosing v6 in []
 *
 * - v6 only:
 *    `WifiGetIPv6`, `WifiGetIPv6Str`, `WifiHasIPv6`
 *    `WifiGetIPv6LinkLocal`, `WifiGetIPv6LinkLocalStr`
 *    `EthernetGetIPv6, `EthernetHasIPv6`, `EthernetGetIPv6Str`
 *    `EthernetGetIPv6LinkLocal`, `EthernetGetIPv6LinkLocalStr`
 *
 * - v4 only:
 *    `WifiGetIPv4`, `WifiGetIPv4Str`, `WifiHasIPv4`
 *    `EthernetGetIPv4`, `EthernetGetIPv4Str`, `EthernetHasIPv4`
 *
 * - DNS reporting actual values used (not the Settings):
 *    `DNSGetIP(n)`, `DNSGetIPStr(n)` with n=`0`/`1` (same dns for Wifi and Eth)
\*****************************************************************************************************/
bool WifiGetIP(IPAddress *ip, bool exclude_ap = false);
// IPv4 for Wifi
// Returns only IPv6 global address (no loopback and no link-local)
/**
 * Retrieves the IPv4 address of the WiFi interface
 * 
 * @param ip Pointer to store the IPv4 address (can be nullptr to just check existence)
 * @return true if a valid IPv4 address exists, false otherwise
 * 
 * This function gets the current IPv4 address of the WiFi interface if connected.
 * If the ip parameter is provided, the address is copied to it.
 * The function returns true only if a valid (non-zero) IPv4 address exists.
 */
bool WifiGetIPv4(IPAddress *ip)
{
  uint32_t wifi_uint = (WL_CONNECTED == WiFi.status()) ? (uint32_t)WiFi.localIP() : 0;  // See issue #23115
  if (ip != nullptr) { *ip = wifi_uint; }
  return wifi_uint != 0;
}

/**
 * Checks if the WiFi interface has a valid IPv4 address
 * 
 * @return true if a valid IPv4 address exists, false otherwise
 * 
 * This is a convenience wrapper around WifiGetIPv4() that only checks
 * for the existence of an IPv4 address without retrieving it.
 */
bool WifiHasIPv4(void)
{
  return WifiGetIPv4(nullptr);
}

/**
 * Returns the WiFi IPv4 address as a string
 * 
 * @return String containing the IPv4 address or empty string if none
 * 
 * This function returns the current IPv4 address of the WiFi interface
 * formatted as a string (e.g., "192.168.1.100"). If no valid IPv4 address
 * exists, an empty string is returned.
 */
String WifiGetIPv4Str(void)
{
  IPAddress ip;
  return WifiGetIPv4(&ip) ? ip.toString() : String();
}

/**
 * Retrieves the IPv4 address of the Ethernet interface
 * 
 * @param ip Pointer to store the IPv4 address (can be nullptr to just check existence)
 * @return true if a valid IPv4 address exists, false otherwise
 * 
 * This function gets the current IPv4 address of the Ethernet interface if connected.
 * If the ip parameter is provided, the address is copied to it.
 * The function returns true only if a valid (non-zero) IPv4 address exists.
 * 
 * On platforms without Ethernet support, this always returns false.
 */
bool EthernetGetIPv4(IPAddress *ip)
{
//#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32 && defined(USE_ETHERNET)
#if defined(ESP32) && defined(USE_ETHERNET)
  uint32_t wifi_uint = (uint32_t) EthernetLocalIP();
  if (ip != nullptr) { *ip = wifi_uint; }
  return wifi_uint != 0;
#else
  if (ip != nullptr) { *ip = (uint32_t)0; }
  return false;
#endif
}

/**
 * Checks if the Ethernet interface has a valid IPv4 address
 * 
 * @return true if a valid IPv4 address exists, false otherwise
 * 
 * This is a convenience wrapper around EthernetGetIPv4() that only checks
 * for the existence of an IPv4 address without retrieving it.
 */
bool EthernetHasIPv4(void)
{
  return EthernetGetIPv4(nullptr);
}

/**
 * Returns the Ethernet IPv4 address as a string
 * 
 * @return String containing the IPv4 address or empty string if none
 * 
 * This function returns the current IPv4 address of the Ethernet interface
 * formatted as a string (e.g., "192.168.1.100"). If no valid IPv4 address
 * exists, an empty string is returned.
 */
String EthernetGetIPv4Str(void)
{
  IPAddress ip;
  return EthernetGetIPv4(&ip) ? ip.toString() : String();
}

#ifdef USE_IPV6
bool IPv6isLocal(const IPAddress & ip) {
  return ip.addr_type() == ESP_IP6_ADDR_IS_LINK_LOCAL;    // TODO
}

#include "lwip/netif.h"
//
// Scan through all interfaces to find a global or local IPv6 address
// Arg:
//    is_local: is the address Link-Local (true) or Global (false)
//    if_type: possible values are "st" for Wifi STA, "en" for Ethernet, "lo" for localhost (not useful)
// Returns `true` if found
bool WifiFindIPv6(IPAddress *ip, bool is_local, const char * if_type = "st") {
  for (netif* intf = netif_list; intf != nullptr; intf = intf->next) {
    if (intf->name[0] == if_type[0] && intf->name[1] == if_type[1]) {
      for (uint32_t i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
        ip_addr_t *ipv6 = &intf->ip6_addr[i];
        if (IP_IS_V6_VAL(*ipv6) && !ip_addr_isloopback(ipv6) && !ip_addr_isany(ipv6) && ((bool)ip_addr_islinklocal(ipv6) == is_local)) {
          if (ip != nullptr) { ip->from_ip_addr_t(ipv6); }
          return true;
        }
      }
    }
  }
  return false;
}


// Returns only IPv6 global address (no loopback and no link-local)
bool WifiGetIPv6(IPAddress *ip)
{
  return WifiFindIPv6(ip, false, "st");
}
bool WifiHasIPv6(void)
{
  return WifiGetIPv6(nullptr);
}
String WifiGetIPv6Str(void)
{
  IPAddress ip;
  return WifiGetIPv6(&ip) ? ip.toString(true) : String();
}

bool WifiGetIPv6LinkLocal(IPAddress *ip)
{
  return WifiFindIPv6(ip, true, "st");
}
String WifiGetIPv6LinkLocalStr(void)
{
  IPAddress ip;
  return WifiGetIPv6LinkLocal(&ip) ? ip.toString(true) : String();
}


// Returns only IPv6 global address (no loopback and no link-local)
bool EthernetGetIPv6(IPAddress *ip)
{
  return WifiFindIPv6(ip, false, "en");
}
bool EthernetHasIPv6(void)
{
  return EthernetGetIPv6(nullptr);
}
String EthernetGetIPv6Str(void)
{
  IPAddress ip;
  return EthernetGetIPv6(&ip) ? ip.toString(true) : String();
}

bool EthernetGetIPv6LinkLocal(IPAddress *ip)
{
  return WifiFindIPv6(ip, true, "en");
}
bool EthernetHasIPv6LinkLocal(void)
{
  return EthernetGetIPv6LinkLocal(nullptr);
}
String EthernetGetIPv6LinkLocalStr(void)
{
  IPAddress ip;
  return EthernetGetIPv6LinkLocal(&ip) ? ip.toString(true) : String();
}

bool DNSGetIP(IPAddress *ip, uint32_t idx)
{
#ifdef ESP32
  WiFiHelper::scrubDNS();    // internal calls to reconnect can zero the DNS servers, restore the previous values
#endif
  const ip_addr_t *ip_dns = dns_getserver(idx);
  if (!ip_addr_isany(ip_dns)) {
    if (ip != nullptr) { ip->from_ip_addr_t((ip_addr_t*)ip_dns); }
    return true;
  }
  if (ip != nullptr) { ip->from_ip_addr_t((ip_addr_t*)IP4_ADDR_ANY); }
  return false;
}
String DNSGetIPStr(uint32_t idx)
{
  IPAddress ip;
  return DNSGetIP(&ip, idx) ? ip.toString(true) : String(F("0.0.0.0"));
}

//
#include "lwip/dns.h"
#ifdef ESP32
#include "esp_netif_net_stack.h"
#endif
void WifiDumpAddressesIPv6(void)
{
  for (netif* intf = netif_list; intf != nullptr; intf = intf->next) {
#ifdef ESP32
    esp_netif_t *esp_netif = esp_netif_get_handle_from_netif_impl(intf);
    int32_t route_prio = esp_netif ? esp_netif_get_route_prio(esp_netif) : -1;
    if (!ip_addr_isany_val(intf->ip_addr)) AddLog(LOG_LEVEL_DEBUG, "WIF: '%c%c%i' IPv4 %s (%i)", intf->name[0], intf->name[1], intf->num, IPAddress(&intf->ip_addr).toString(true).c_str(), route_prio);
    for (uint32_t i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
      if (!ip_addr_isany_val(intf->ip6_addr[i]))
        AddLog(LOG_LEVEL_DEBUG, "IP : '%c%c%i' IPv6 %s %s (%i)", intf->name[0], intf->name[1], intf->num,
                                IPAddress(&intf->ip6_addr[i]).toString(true).c_str(),
                                ip_addr_islinklocal(&intf->ip6_addr[i]) ? "local" : "", route_prio);
    }
#else
    if (!ip_addr_isany_val(intf->ip_addr)) AddLog(LOG_LEVEL_DEBUG, "WIF: '%c%c%i' IPv4 %s", intf->name[0], intf->name[1], intf->num, IPAddress(&intf->ip_addr).toString(true).c_str());
    for (uint32_t i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
      if (!ip_addr_isany_val(intf->ip6_addr[i]))
        AddLog(LOG_LEVEL_DEBUG, "IP : '%c%c%i' IPv6 %s %s", intf->name[0], intf->name[1], intf->num,
                                IPAddress(&intf->ip6_addr[i]).toString(true).c_str(),
                                ip_addr_islinklocal(&intf->ip6_addr[i]) ? "local" : "");
    }
#endif
  }

  AddLog(LOG_LEVEL_DEBUG, "IP : DNS: %s %s", IPAddress(dns_getserver(0)).toString().c_str(),  IPAddress(dns_getserver(1)).toString(true).c_str());
  AddLog(LOG_LEVEL_DEBUG, "WIF: v4IP: %_I v6IP: %s mainIP: %s", (uint32_t) WiFi.localIP(), WifiGetIPv6Str().c_str(), WifiGetIPStr().c_str());
//#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32 && defined(USE_ETHERNET)
#if defined(ESP32) && defined(USE_ETHERNET)
  AddLog(LOG_LEVEL_DEBUG, "ETH: v4IP %_I v6IP: %s mainIP: %s", (uint32_t) EthernetLocalIP(), EthernetGetIPv6Str().c_str(), EthernetGetIPStr().c_str());
#endif
  AddLog(LOG_LEVEL_DEBUG, "IP : ListeningIP %s", IPGetListeningAddressStr().c_str());
}
#endif  // USE_IPV6

// Returns the IP address on which we listen (used for Web UI mainly)
//
// If IPv4 is set, it is preferred.
// If only IPv6, return the routable global address
bool IPGetListeningAddress(IPAddress * ip)
{
  if (ip == nullptr) return HasIP();    // no value added for this method if no parameter

#ifdef USE_IPV6
  // collect both Wifi and Eth IPs and choose an IPv4 if any (Eth has priority)
  IPAddress ip_wifi;
  bool has_wifi = WifiGetIP(&ip_wifi);

//#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32 && defined(USE_ETHERNET)
#if defined(ESP32) && defined(USE_ETHERNET)
  IPAddress ip_eth;
  bool has_eth = EthernetGetIP(&ip_eth);
  if (has_wifi && has_eth) {
    if (ip_eth.type() == IPv4) { *ip = ip_eth; return true; }
    if (ip_wifi.type() == IPv4) { *ip = ip_wifi; return true; }
    // both addresses are v6, return ETH
    *ip = ip_eth;
    return true;
  }
  // from here only wifi or eth may be valid
  if (has_eth) { *ip = ip_eth; return true; }
#endif

  if (has_wifi) { *ip = ip_wifi; return true; }

  *ip = IPAddress();
  return false;
#else // USE_IPV6
//#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32 && defined(USE_ETHERNET)
#if defined(ESP32) && defined(USE_ETHERNET)
  if (EthernetGetIP(ip)) { return true; }
#endif
  if (WifiGetIP(ip)) { return true; }
  *ip = IPAddress();
  return false;
#endif // USE_IPV6
}

String IPGetListeningAddressStr(void)
{
  IPAddress ip;
#ifdef USE_IPV6
  return IPGetListeningAddress(&ip) ? ip.toString(true) : String();
#else
  return IPGetListeningAddress(&ip) ? ip.toString() : String();
#endif
}

// Because of IPv6, we can't test an IP address agains (uint32_t)0L anymore
// This test would work only for IPv4 assigned addresses.
// We must now use the following instead
inline bool IPIsValid(const IPAddress & ip)
{
#ifdef USE_IPV6
  return !ip_addr_isany_val((const ip_addr_t &)ip);
#else
  return static_cast<uint32_t>(ip) != 0;
#endif
}

// Because of IPv6, URL encoding of IP address needs to be adapted
// IPv4: address is "x.x.x.x"
// IPv6: address is enclosed in brackets "[x.x::x.x...]"
String IPForUrl(const IPAddress & ip)
{
#ifdef USE_IPV6
  if (ip.type() == IPv4) {
    return ip.toString().c_str();
  } else {
    String s('[');
    s += ip.toString(true).c_str();
    s += ']';
    return s;
  }
#else
  return ip.toString().c_str();
#endif
}

// Check to see if we have any routable IP address
// IPv4 has always priority
// Copy the value of the IP if pointer provided (optional)
// `exclude_ap` allows to exlude AP IP address and focus only on local STA
bool WifiGetIP(IPAddress *ip, bool exclude_ap) {
#ifdef ESP32
  wifi_mode_t mode = WiFi.getMode();
  if ((mode == WIFI_MODE_STA || mode == WIFI_MODE_APSTA) && (uint32_t)WiFi.localIP() != 0) {
    if (ip != nullptr) { *ip = WiFi.localIP(); }
    return true;
  }
  if (!exclude_ap && (mode == WIFI_MODE_AP || mode == WIFI_MODE_APSTA) && (uint32_t)WiFi.softAPIP() != 0) {
    if (ip != nullptr) { *ip = WiFi.softAPIP(); }
    return true;
  }
#else
  WiFiMode_t mode = WiFi.getMode();
  if ((mode == WIFI_STA || mode == WIFI_AP_STA) && (uint32_t)WiFi.localIP() != 0) {
    if (ip != nullptr) { *ip = WiFi.localIP(); }
    return true;
  }
  if (!exclude_ap && (mode == WIFI_AP || mode == WIFI_AP_STA) && (uint32_t)WiFi.softAPIP() != 0) {
    if (ip != nullptr) { *ip = WiFi.softAPIP(); }
    return true;
  }
#endif
#ifdef USE_IPV6
  IPAddress lip;
  if (WifiGetIPv6(&lip)) {
    if (ip != nullptr) { *ip = lip; }
    return true;
  }
  if (ip != nullptr) { *ip = IPAddress(); }
#endif // USE_IPV6
  return false;
}

bool WifiHasIP(void) {
  return WifiGetIP(nullptr);
}

String WifiGetIPStr(void)
{
  IPAddress ip;
#ifdef USE_IPV6
  return WifiGetIP(&ip) ? ip.toString(true) : String();
#else
  return WifiGetIP(&ip) ? ip.toString() : String();
#endif
}

// Has a routable IP, whether IPv4 or IPv6, Wifi or Ethernet
bool HasIP(void) {
  if (WifiHasIP()) return true;
//#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32 && defined(USE_ETHERNET)
#if defined(ESP32) && defined(USE_ETHERNET)
  if (EthernetHasIP()) return true;
#endif
  return false;
}

/**
 * Verifies WiFi connection status and manages reconnection
 * 
 * This function checks if the device has a valid WiFi connection with an IP address.
 * It handles connection state transitions and reconnection attempts:
 * 
 * 1. If connected with a valid IP:
 *    - Updates connection state
 *    - Resets retry counters
 *    - Stores network parameters for quick reconnection
 *    - Updates DNS server information
 * 
 * 2. If disconnected or connection issues:
 *    - Updates connection state
 *    - Manages retry attempts based on failure type
 *    - Triggers appropriate reconnection strategy
 *    - Handles fallback to WiFi configuration modes
 * 
 * The function implements an adaptive retry mechanism that adjusts based on
 * the type of connection failure (AP not found, wrong password, etc.).
 * 
 * Error handling:
 * - Logs specific connection failure reasons
 * - Implements exponential backoff for retries
 * - Triggers device restart after excessive failures (100 max retries)
 */
void WifiCheckIp(void) {
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_WIFI D_CHECKING_CONNECTION));
  Wifi.counter = WIFI_CHECK_SEC;

  if ((WL_CONNECTED == WiFi.status()) && WifiHasIP()) {
    WifiSetState(1);
    Wifi.counter = WIFI_CHECK_SEC;
    Wifi.retry = Wifi.retry_init;
    Wifi.max_retry = 0;
    if (Wifi.status != WL_CONNECTED) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECTED));
//      AddLog(LOG_LEVEL_INFO, PSTR("Wifi: Set IP addresses"));
      // AddLog(LOG_LEVEL_INFO, ">>>1: Before DNS %_I %_I", Settings->ipv4_address[3], Settings->ipv4_address[4]);
      Settings->ipv4_address[1] = (uint32_t)WiFi.gatewayIP();
      Settings->ipv4_address[2] = (uint32_t)WiFi.subnetMask();
      Settings->ipv4_address[3] = (uint32_t)WiFi.dnsIP();
      Settings->ipv4_address[4] = (uint32_t)WiFi.dnsIP(1);
      // AddLog(LOG_LEVEL_INFO, ">>>1: After DNS %_I %_I", Settings->ipv4_address[3], Settings->ipv4_address[4]);

      // Save current AP parameters for quick reconnect
      Settings->wifi_channel = WiFi.channel();
      uint8_t *bssid = WiFi.BSSID();
      memcpy((void*) &Settings->wifi_bssid, (void*) bssid, sizeof(Settings->wifi_bssid));
    }
    Wifi.status = WL_CONNECTED;
  } else {
    WifiSetState(0);
    uint8_t wifi_config_tool = Settings->sta_config;
    Wifi.status = (Wifi.retry &1) ? WiFi.status() : 0;  // Skip every second to reset result WiFi.status()
    switch (Wifi.status) {
      case WL_CONNECTED:
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_NO_IP_ADDRESS));
        Wifi.status = 0;
        Wifi.retry = Wifi.retry_init;
        break;
      case WL_NO_SSID_AVAIL:
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_AP_NOT_REACHED));
        Settings->wifi_channel = 0;  // Disable stored AP
        if (WIFI_WAIT == Settings->sta_config) {
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
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_WRONG_PASSWORD));
        Settings->wifi_channel = 0;  // Disable stored AP
        if (Wifi.retry > (Wifi.retry_init / 2)) {
          Wifi.retry = Wifi.retry_init / 2;
        }
        else if (Wifi.retry) {
          Wifi.retry = 0;
        }
        break;
      default:  // WL_IDLE_STATUS and WL_DISCONNECTED
        if (!Wifi.retry || ((Wifi.retry_init / 2) == Wifi.retry)) {
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_AP_TIMEOUT));
          Settings->wifi_channel = 0;  // Disable stored AP
          Wifi.max_retry++;
          if (100 == Wifi.max_retry) {  // Restart after 100 * (WIFI_RETRY_OFFSET_SEC + MAC) / 2 seconds
            TasmotaGlobal.restart_flag = 2;
          }
        } else {
          if (!strlen(SettingsText(SET_STASSID1)) && !strlen(SettingsText(SET_STASSID2))) {
            Settings->wifi_channel = 0;  // Disable stored AP
            wifi_config_tool = WIFI_MANAGER;  // Skip empty SSIDs and start Wifi config tool
            Wifi.retry = 0;
          } else {
            AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION));
          }
        }
    }
    if (Wifi.retry) {
      if (Settings->flag3.use_wifi_scan) {  // SetOption56 - Scan wifi network at restart for configured AP's
        if (Wifi.retry_init == Wifi.retry) {
          Wifi.scan_state = 1;    // Select scanned SSID
        }
      } else {
        if (Wifi.retry_init == Wifi.retry) {
          WifiBegin(3, Settings->wifi_channel);  // Select default SSID
        }
        if ((Settings->sta_config != WIFI_WAIT) && ((Wifi.retry_init / 2) == Wifi.retry)) {
          WifiBegin(2, 0);        // Select alternate SSID
        }
      }
      Wifi.retry--;
    } else {
      WifiConfig(wifi_config_tool);
      Wifi.retry = Wifi.retry_init;
    }
    Wifi.counter = 1;             // Re-check in 1 second
  }
}

/**
 * Main WiFi management function called periodically from the main loop
 * 
 * @param param Configuration mode parameter (WIFI_SERIAL, WIFI_MANAGER, etc.)
 * 
 * This function serves as the central WiFi management routine that:
 * 1. Decrements the WiFi check counter
 * 2. Handles WiFi configuration modes (WIFI_SERIAL, WIFI_MANAGER)
 * 3. Manages configuration timeout countdown
 * 4. Calls WifiCheckIp() to verify connection status
 * 5. Updates WiFi state based on connection status
 * 6. Triggers periodic network rescans if enabled
 * 
 * The function implements a state machine that manages:
 * - WiFi configuration timeouts
 * - Connection monitoring
 * - Periodic rescanning of networks
 * - WiFi scan state processing
 * 
 * It's designed to be called regularly from the main loop to maintain
 * WiFi connectivity and handle configuration changes.
 */
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
      if (!Wifi.config_counter) {
//        SettingsSdkErase();  //  Disabled v6.1.0b due to possible bad wifi connects
        TasmotaGlobal.restart_flag = 2;
      }
    } else {
      if (Wifi.counter <= 0) {
        WifiCheckIp();
      }
      if ((WL_CONNECTED == WiFi.status()) && WifiHasIP() && !Wifi.config_type) {
        WifiSetState(1);
        if (Settings->flag3.use_wifi_rescan) {  // SetOption57 - Scan wifi network every 44 minutes for configured AP's
          if (!(TasmotaGlobal.uptime % (60 * WIFI_RESCAN_MINUTES))) {
            if (!Wifi.scan_state) { Wifi.scan_state = 2; } // If wifi scan routine is free, use it. Otherwise, wait for next RESCAN TIME
          }
        }
      } else {
        WifiSetState(0);
        Mdns.begun = 0;
      }
    }
    if (Wifi.scan_state) { WifiBeginAfterScan(); }
  }
}

/**
 * Returns the current WiFi state or configuration mode
 * 
 * @return Current WiFi state:
 *         - WIFI_RESTART: WiFi is being restarted
 *         - WIFI_SERIAL: Serial configuration mode active
 *         - WIFI_MANAGER: WiFi manager configuration mode active
 *         - WIFI_MANAGER_RESET_ONLY: WiFi manager reset-only mode active
 *         - -1: WiFi is down (not connected)
 * 
 * This function provides the current WiFi state for status reporting and
 * decision making. It returns the active configuration mode if one is running,
 * WIFI_RESTART if WiFi is up and running normally, or -1 if WiFi is down.
 */
int WifiState(void)
{
  int state = -1;

  if (!TasmotaGlobal.global_state.wifi_down) { state = WIFI_RESTART; }
  if (Wifi.config_type) { state = Wifi.config_type; }
  return state;
}

/**
 * Gets the current WiFi transmit power
 * 
 * @return Current WiFi transmit power in dBm as a float
 * 
 * This function returns the current WiFi transmit power setting.
 * If a fixed power is set in Settings->wifi_output_power, that value is used.
 * The power is stored internally as an integer (tenths of dBm) and
 * returned as a float value in dBm.
 */
float WifiGetOutputPower(void) {
  if (Settings->wifi_output_power) {
    Wifi.last_tx_pwr = Settings->wifi_output_power;
  }
  return (float)(Wifi.last_tx_pwr) / 10;
}

/**
 * Sets the WiFi transmit power based on settings
 * 
 * This function configures the WiFi transmit power:
 * - If Settings->wifi_output_power is non-zero, it sets a fixed power level
 * - If Settings->wifi_output_power is zero, it enables dynamic power management
 * 
 * For fixed power, the value is converted from tenths of dBm to dBm
 * (e.g., 170 becomes 17.0 dBm).
 * 
 * The function adds a delay after setting the power to allow the hardware
 * to stabilize.
 */
void WifiSetOutputPower(void) {
  if (Settings->wifi_output_power) {
    WiFiHelper::setOutputPower((float)(Settings->wifi_output_power) / 10);
    delay(100);
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WIF: Dynamic Tx power enabled"));  // WifiPower 0
  }
}

/**
 * Dynamically adjusts WiFi transmit power based on signal strength
 * 
 * This function implements dynamic power management to optimize power consumption
 * while maintaining reliable WiFi connectivity. It works by:
 * 
 * 1. Measuring the current RSSI (signal strength)
 * 2. Calculating the minimum required transmit power based on:
 *    - Current RSSI
 *    - WiFi sensitivity threshold for the current PHY mode
 *    - Maximum allowed transmit power for the current PHY mode
 * 
 * The function adjusts power based on different WiFi standards:
 * - 802.11b: Different sensitivity and max power than other modes
 * - 802.11g: Optimized for 54Mbps operation
 * - 802.11n/ax: Higher sensitivity requirements
 * 
 * This helps reduce overall power consumption while maintaining connection quality.
 * The function is only active when Settings->wifi_output_power is 0 (dynamic mode).
 * 
 * Original concept by ESPEasy (@TD-er).
 */
void WiFiSetTXpowerBasedOnRssi(void) {
  // Dynamic WiFi transmit power based on RSSI lowering overall DC power usage.
  // Original idea by ESPEasy (@TD-er)
  if (!Settings->flag4.network_wifi || Settings->wifi_output_power) { return; }
  const WiFiMode_t cur_mode = WiFi.getMode();
  if (cur_mode == WIFI_OFF) { return; }

  // Range ESP32  : 2dBm - 20dBm
  // Range ESP8266: 0dBm - 20.5dBm
  int max_tx_pwr = MAX_TX_PWR_DBM_11b;
  int threshold = WIFI_SENSITIVITY_n;
  int phy_mode = WiFiHelper::getPhyMode();
  switch (phy_mode) {
    case 1:                  // 11b (WIFI_PHY_MODE_11B)
      threshold = WIFI_SENSITIVITY_11b;
      if (max_tx_pwr > MAX_TX_PWR_DBM_11b) max_tx_pwr = MAX_TX_PWR_DBM_11b;
      break;
    case 2:                  // 11bg (WIFI_PHY_MODE_11G)
      threshold = WIFI_SENSITIVITY_54g;
      if (max_tx_pwr > MAX_TX_PWR_DBM_54g) max_tx_pwr = MAX_TX_PWR_DBM_54g;
      break;
    case 3:                  // 11bgn (WIFI_PHY_MODE_HT20 = WIFI_PHY_MODE_11N)
    case 4:                  // 11bgn (WIFI_PHY_MODE_HT40)
    case 5:                  // 11ax  (WIFI_PHY_MODE_HE20)
      threshold = WIFI_SENSITIVITY_n;
      if (max_tx_pwr > MAX_TX_PWR_DBM_n) max_tx_pwr = MAX_TX_PWR_DBM_n;
      break;
  }
  threshold += 30;           // Margin in dBm * 10 on top of threshold

  // Assume AP sends with max set by ETSI standard.
  // 2.4 GHz: 100 mWatt (20 dBm)
  // US and some other countries allow 1000 mW (30 dBm)
  int rssi = WiFi.RSSI() * 10;
  int newrssi = rssi - 200;  // We cannot send with over 20 dBm, thus it makes no sense to force higher TX power all the time.

  int min_tx_pwr = 0;
  if (newrssi < threshold) {
    min_tx_pwr = threshold - newrssi;
  }
  if (min_tx_pwr > max_tx_pwr) {
    min_tx_pwr = max_tx_pwr;
  }
  WiFiHelper::setOutputPower((float)min_tx_pwr / 10);
  delay(Wifi.last_tx_pwr < min_tx_pwr);  // If increase the TX power, give power supply of the unit some rest
/*
  if (Wifi.last_tx_pwr != min_tx_pwr) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WIF: TX power %d, Sensitivity %d, RSSI %d"), min_tx_pwr / 10, threshold / 10, rssi / 10);
  }
*/
  Wifi.last_tx_pwr = min_tx_pwr;
}

/*
  See Esp.h, core_esp8266_phy.cpp and test_overrides.ino
  RF_DEFAULT = 0,  // RF_CAL or not after deep-sleep wake up, depends on init data byte 108.
  RF_CAL = 1,      // RF_CAL after deep-sleep wake up, there will be large current.
  RF_NO_CAL = 2,   // no RF_CAL after deep-sleep wake up, there will only be small current.
  RF_DISABLED = 4  // disable RF after deep-sleep wake up, just like modem sleep, there will be the smallest current.
*/
#ifdef WIFI_RF_MODE_RF_CAL
#ifndef USE_DEEPSLEEP
RF_MODE(RF_CAL);
#endif  // USE_DEEPSLEEP
#endif  // WIFI_RF_MODE_RF_CAL

#ifdef WIFI_RF_PRE_INIT
bool rf_pre_init_flag = false;
RF_PRE_INIT()
{
#ifndef USE_DEEPSLEEP
  system_deep_sleep_set_option(1);   // The option is 1 by default.
  system_phy_set_rfoption(RF_CAL);
#endif  // USE_DEEPSLEEP
  system_phy_set_powerup_option(3);  // 3: RF initialization will do the whole RF calibration which will take about 200ms; this increases the current consumption.
  rf_pre_init_flag = true;
}
#endif  // WIFI_RF_PRE_INIT

/**
 * Enables WiFi by setting the check counter to trigger immediate processing
 * 
 * This function activates WiFi by setting the Wifi.counter to 1, which will
 * cause the WifiCheck function to process WiFi operations on the next cycle.
 * It's a simple way to trigger WiFi initialization or reconnection from
 * other parts of the code.
 */
void WifiEnable(void) {
  Wifi.counter = 1;
}

//#ifdef ESP8266
//#include <sntp.h>                       // sntp_servermode_dhcp()
//#endif  // ESP8266

#ifdef ESP32
void WifiEvents(arduino_event_t *event);
#endif

/**
 * Initializes WiFi connection parameters and starts the connection process
 * 
 * This function sets up the WiFi system for initial connection:
 * 1. Registers event handlers for ESP32
 * 2. Initializes WiFi state variables
 * 3. Sets up retry timers with a randomized offset based on chip ID
 * 4. Configures WiFi for non-persistent settings
 * 
 * The function is typically called during device startup or after a
 * WiFi reconfiguration. It prepares the WiFi subsystem but doesn't
 * actually establish the connection (that happens in subsequent
 * WifiCheck calls).
 * 
 * The retry timing includes a chip-specific offset to prevent multiple
 * devices from attempting to reconnect simultaneously, which helps
 * avoid network congestion in multi-device installations.
 * 
 * Note: This function will not do anything if network_wifi flag is disabled.
 */
void WifiConnect(void)
{
  if (!Settings->flag4.network_wifi) { return; }

#ifdef ESP32
  static bool wifi_event_registered = false;
  if (!wifi_event_registered) {
    WiFi.onEvent(WifiEvents);   // register event listener only once
    wifi_event_registered = true;
#ifdef CONFIG_ESP_WIFI_REMOTE_ENABLED
    // Hosted MCU SDIO pins must be set before WiFi is initialized
    if (WiFi.setPins(Pin(GPIO_HSDIO_CLK),
                     Pin(GPIO_HSDIO_CMD),
                     Pin(GPIO_HSDIO_D0),
                     Pin(GPIO_HSDIO_D1),
                     Pin(GPIO_HSDIO_D2),
                     Pin(GPIO_HSDIO_D3),
                     Pin(GPIO_HSDIO_RST))) {
//      AddLog(LOG_LEVEL_DEBUG, PSTR("HMC: Hosted MCU SDIO pins set"));
    }
#endif  // CONFIG_ESP_WIFI_REMOTE_ENABLED
  }
#endif // ESP32
  WifiSetState(0);
//  WifiSetOutputPower();

//#ifdef ESP8266
  // https://github.com/arendst/Tasmota/issues/16061#issuecomment-1216970170
//  sntp_servermode_dhcp(0);
//#endif  // ESP8266

  WiFi.persistent(false);     // Solve possible wifi init errors
  Wifi.status = 0;
  Wifi.retry_init = WIFI_RETRY_OFFSET_SEC + (ESP_getChipId() & 0xF);  // Add extra delay to stop overrun by simultanous re-connects
  Wifi.retry = Wifi.retry_init;
  Wifi.max_retry = 0;
  Wifi.counter = 1;

  memcpy((void*) &Wifi.bssid, (void*) Settings->wifi_bssid, sizeof(Wifi.bssid));

#ifdef WIFI_RF_PRE_INIT
  if (rf_pre_init_flag) {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "Pre-init done"));
  }
#endif  // WIFI_RF_PRE_INIT
}

/**
 * Performs a clean shutdown of WiFi connections and services
 * 
 * @param option If true, performs a more thorough cleanup including SDK WiFi calibration data
 * 
 * This function properly terminates WiFi connections and related services:
 * 1. Disconnects any active UDP emulation services
 * 2. Disconnects MQTT if enabled
 * 3. Disconnects from WiFi with appropriate cleanup based on the option parameter
 * 
 * When option=true (used with WIFI_FORCE_RF_CAL_ERASE enabled):
 * - Performs a simple disconnect
 * - Erases SDK WiFi configuration and calibration data
 * 
 * When option=false (default, used for normal shutdown and DeepSleep):
 * - Performs a more standard disconnect that preserves calibration data
 * 
 * The function includes delays to ensure network buffers are properly flushed
 * before disconnection.
 */
void WifiShutdown(bool option) {
  // option = false - Legacy disconnect also used by DeepSleep
  // option = true  - Disconnect with SDK wifi calibrate sector erase when WIFI_FORCE_RF_CAL_ERASE enabled
  delay(100);                 // Allow time for message xfer - disabled v6.1.0b

#ifdef USE_EMULATION
  UdpDisconnect();
  delay(100);                 // Flush anything in the network buffers.
#endif  // USE_EMULATION

  if (Settings->flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    MqttDisconnect();
    delay(100);               // Flush anything in the network buffers.
  }

#ifdef WIFI_FORCE_RF_CAL_ERASE
  if (option) {
    WiFi.disconnect(false);   // Disconnect wifi
    SettingsErase(4);         // Delete SDK wifi config and calibrate data
  } else
#endif  // WIFI_FORCE_RF_CAL_ERASE
  {
    // Enable from 6.0.0a until 6.1.0a - disabled due to possible cause of bad wifi connect on core 2.3.0
    // Re-enabled from 6.3.0.7 with ESP.restart replaced by ESP.reset
    // Courtesy of EspEasy
    // WiFi.persistent(true);    // use SDK storage of SSID/WPA parameters
    ETS_UART_INTR_DISABLE();
#ifdef ESP8266
    wifi_station_disconnect();  // this will store empty ssid/wpa into sdk storage
#else
    WiFi.disconnect(true, true);
#endif
    ETS_UART_INTR_ENABLE();
    // WiFi.persistent(false);   // Do not use SDK storage of SSID/WPA parameters
  }
  delay(100);                 // Flush anything in the network buffers.
}

/**
 * Completely disables WiFi functionality
 * 
 * This function performs a full shutdown of WiFi:
 * 1. Checks if WiFi is already disabled to avoid redundant operations
 * 2. Calls WifiShutdown() to properly terminate connections
 * 3. Sets WiFi mode to WIFI_OFF to disable the radio
 * 4. Updates the global state to indicate WiFi is down
 * 
 * After calling this function, WiFi will remain disabled until explicitly
 * re-enabled. This is useful for power saving or when WiFi is not needed.
 */
void WifiDisable(void) {
  if (!TasmotaGlobal.global_state.wifi_down) {
    WifiShutdown();
    WifiSetMode(WIFI_OFF);
  }
  TasmotaGlobal.global_state.wifi_down = 1;
}

/**
 * Performs a clean device restart with proper shutdown procedures
 * 
 * This function handles different types of restart operations:
 * 1. Normal restart: Performs cleanup and calls ESP.restart()
 * 2. Halt (TasmotaGlobal.restart_halt): Enters an infinite loop with watchdog feeding
 * 3. Deep sleep (TasmotaGlobal.restart_deepsleep): Enters deep sleep mode
 * 
 * Before restarting, the function:
 * 1. Resets PWM outputs
 * 2. Performs a clean WiFi shutdown
 * 3. Clears any crash dump data
 * 4. For ESP32-C3: Forces GPIO hold for relays to maintain state during reset
 * 
 * The halt mode is useful for debugging, as it keeps the device running
 * but in a known state with visual LED feedback.
 * 
 * Deep sleep mode puts the device into the lowest power state, with only
 * hardware-triggered wake up possible.
 */
void EspRestart(void) {
  ResetPwm();
  WifiShutdown(true);
#ifndef FIRMWARE_MINIMAL
  CrashDumpClear();           // Clear the stack dump in RTC
#endif // FIRMWARE_MINIMAL

#ifdef CONFIG_IDF_TARGET_ESP32C3
  GpioForceHoldRelay();       // Retain the state when the chip or system is reset, for example, when watchdog time-out or Deep-sleep
#endif  // CONFIG_IDF_TARGET_ESP32C3

  if (TasmotaGlobal.restart_halt) {  // Restart 2
    while (1) {
      OsWatchLoop();          // Feed OsWatch timer to prevent restart
      SetLedLink(1);          // Wifi led on
      delay(200);             // Satisfy SDK
      SetLedLink(0);          // Wifi led off
      delay(800);             // Satisfy SDK
    }
  }
  else if (TasmotaGlobal.restart_deepsleep) {  // Restart 9
  #ifdef USE_DEEPSLEEP
    DeepSleepStart();
    // should never come to this line....
  #endif
    ESP.deepSleep(0);         // Deep sleep mode with only hardware triggered wake up

  }
  else {
    ESP_Restart();
  }
}

#ifdef ESP8266
//
// Gratuitous ARP, backported from https://github.com/esp8266/Arduino/pull/6889
//
extern "C" {
#if LWIP_VERSION_MAJOR == 1
#include "netif/wlan_lwip_if.h" // eagle_lwip_getif()
#include "netif/etharp.h" // gratuitous arp
#else
#include "lwip/etharp.h" // gratuitous arp
#endif
}

/**
 * Sends a Gratuitous ARP packet to update network ARP tables
 * 
 * This function sends a Gratuitous ARP announcement to inform other devices
 * on the network about the device's MAC and IP address mapping. This helps
 * maintain connectivity by refreshing ARP cache entries on network devices,
 * particularly useful with routers that might otherwise expire ARP entries.
 * 
 * The function:
 * 1. Finds the active station interface
 * 2. Verifies it has a valid IP address
 * 3. Sends a gratuitous ARP packet
 * 
 * This implementation handles differences between LWIP v1 and v2.
 * Backported from https://github.com/esp8266/Arduino/pull/6889
 */
void stationKeepAliveNow(void) {
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_WIFI "Sending Gratuitous ARP"));
  for (netif* interface = netif_list; interface != nullptr; interface = interface->next)
    if (
          (interface->flags & NETIF_FLAG_LINK_UP)
      && (interface->flags & NETIF_FLAG_UP)
#if LWIP_VERSION_MAJOR == 1
      && interface == eagle_lwip_getif(STATION_IF) /* lwip1 does not set if->num properly */
      && (!ip_addr_isany(&interface->ip_addr))
#else
      && interface->num == STATION_IF
      && (!ip4_addr_isany_val(*netif_ip4_addr(interface)))
#endif
  )
  {
    etharp_gratuitous(interface);
    break;
  }
}

/**
 * Periodically sends Gratuitous ARP packets to maintain network presence
 * 
 * This function manages the timing for sending Gratuitous ARP packets
 * based on the configured interval in Settings->param[P_ARP_GRATUITOUS].
 * 
 * The timing can be configured as:
 * - Values 1-100: Seconds between ARP packets
 * - Values >100: Minutes between ARP packets (value - 100)
 *   e.g., 105 = 5 minutes, 110 = 10 minutes
 * - Value 0: Feature disabled
 * 
 * This helps maintain connectivity with network devices that might
 * otherwise expire ARP cache entries, particularly useful with some
 * router models that aggressively clear their ARP tables.
 */
void wifiKeepAlive(void) {
  static uint32_t wifi_timer = millis();                     // Wifi keepalive timer

  uint32_t wifiTimerSec = Settings->param[P_ARP_GRATUITOUS];  // 8-bits number of seconds, or minutes if > 100

  if ((WL_CONNECTED != Wifi.status) || (0 == wifiTimerSec)) { return; }   // quick exit if wifi not connected or feature disabled

  if (TimeReached(wifi_timer)) {
    stationKeepAliveNow();
    if (wifiTimerSec > 100) {
      wifiTimerSec = (wifiTimerSec - 100) * 60;              // convert >100 as minutes, ex: 105 = 5 minutes, 110 = 10 minutes
    }
    SetNextTimeInterval(wifi_timer, wifiTimerSec * 1000);
  }
}
#endif  // ESP8266

/**
 * Returns the configured DNS resolution timeout
 * 
 * @return DNS timeout value in milliseconds from settings
 * 
 * This function exposes the DNS timeout setting to be used by WiFi32
 * and other components that need to know how long to wait for DNS
 * resolution before timing out.
 */
int32_t WifiDNSGetTimeout(void) {
  return Settings->dns_timeout;
}
/**
 * Determines if IPv6 should be prioritized for DNS resolution
 * 
 * @return true if IPv6 should be prioritized, false otherwise
 * 
 * This function determines whether IPv6 addresses should be prioritized
 * over IPv4 for DNS resolution based on:
 * 
 * 1. User settings (Settings->flag6.dns_ipv6_priority)
 * 2. Availability of IPv4 and IPv6 addresses
 * 
 * The logic ensures that:
 * - If only IPv4 is available, IPv4 is prioritized regardless of settings
 * - If only IPv6 is available, IPv6 is prioritized regardless of settings
 * - If both are available, the user setting determines priority
 * 
 * When the priority changes, the DNS cache is cleared on ESP32 to ensure
 * proper resolution with the new priority.
 */
bool WifiDNSGetIPv6Priority(void) {
#ifdef USE_IPV6
  // we prioritize IPv6 only if a global IPv6 address is available, otherwise revert to IPv4 if we have one as well
  // Any change in logic needs to clear the DNS cache
  static bool had_v6prio = false;

  bool has_v4 = WifiHasIPv4() || EthernetHasIPv4();
  bool has_v6 = WifiHasIPv6() || EthernetHasIPv6();
  bool v6prio = Settings->flag6.dns_ipv6_priority;

  if (has_v4 && !has_v6) {
    v6prio = false;   // revert to IPv4 first
  } else if (has_v6 && !has_v4) {
    v6prio = true;    // only IPv6 is available
  }

  // any change of state requires a dns cache clear
  if (had_v6prio != v6prio) {
#ifdef ESP32
    dns_clear_cache();    // this function doesn't exist in LWIP used by ESP8266
#endif
    had_v6prio = v6prio;
  }

  return v6prio;
#endif // USE_IPV6
  return false;
}

/**
 * Resolves a hostname to an IP address with enhanced handling
 * 
 * @param aHostname The hostname to resolve
 * @param aResult Reference to store the resulting IP address
 * @return true if resolution was successful, false otherwise
 * 
 * This function extends the standard hostname resolution with:
 * 1. Direct IP address parsing (for ESP_IDF_VERSION_MAJOR >= 5 with IPv6)
 * 2. IPv6 zone auto-fixing for link-local addresses
 * 3. Timeout handling based on Settings->dns_timeout
 * 4. Detailed logging of resolution results and timing
 * 
 * The function is used throughout Tasmota for all DNS resolution needs,
 * providing consistent behavior and error handling.
 */
bool WifiHostByName(const char* aHostname, IPAddress& aResult) {
#ifdef USE_IPV6
#if ESP_IDF_VERSION_MAJOR >= 5
  // try converting directly to IP
  if (aResult.fromString(aHostname)) {
    WiFiHelper::IPv6ZoneAutoFix(aResult, aHostname);
    return true;   // we're done
  }
#endif
#endif // USE_IPV6

  uint32_t dns_start = millis();
  bool success = WiFiHelper::hostByName(aHostname, aResult, Settings->dns_timeout);
  uint32_t dns_end = millis();
  if (success) {
    // Host name resolved
    if (0xFFFFFFFF != (uint32_t)aResult) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_WIFI "DNS resolved '%s' (%s) in %i ms"), aHostname, aResult.toString().c_str(), dns_end - dns_start);
      return true;
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "DNS failed for %s after %i ms"), aHostname, dns_end - dns_start);
  return false;
}

/**
 * Checks if a hostname can be resolved via DNS
 * 
 * @param aHostname The hostname to check
 * @return true if the hostname can be resolved, false otherwise
 * 
 * This is a convenience wrapper around WifiHostByName that simply checks
 * if a hostname can be resolved without needing the resulting IP address.
 */
bool WifiDnsPresent(const char* aHostname) {
  IPAddress aResult;
  return WifiHostByName(aHostname, aResult);
}

/**
 * Periodically polls NTP servers to synchronize device time
 * 
 * This function manages the NTP time synchronization process:
 * 1. Determines when to attempt synchronization based on:
 *    - Initial sync attempt shortly after boot
 *    - Hourly sync attempts thereafter
 *    - Forced sync requests via TasmotaGlobal.ntp_force_sync
 * 
 * 2. Calls WifiGetNtp() to retrieve the current time from NTP servers
 * 
 * 3. Updates the RTC time if a valid time is received
 * 
 * The function implements a staggered sync schedule based on the device's
 * chip ID to prevent all devices from querying NTP servers simultaneously.
 * 
 * Time synchronization is skipped if:
 * - The network is down
 * - The user has manually set the time
 */
void WifiPollNtp() {
  static uint8_t ntp_sync_minute = 0;
  static uint32_t ntp_run_time = 0;

  if (TasmotaGlobal.global_state.network_down || Rtc.user_time_entry) { return; }

  uint8_t uptime_minute = (TasmotaGlobal.uptime / 60) % 60;  // 0 .. 59
  if ((ntp_sync_minute > 59) && (uptime_minute > 2)) {
    ntp_sync_minute = 1;                 // If sync prepare for a new cycle
  }
  // First try ASAP to sync. If fails try once every 60 seconds based on chip id
  uint8_t offset = (TasmotaGlobal.uptime < 30) ? RtcTime.second + ntp_run_time : (((ESP_getChipId() & 0xF) * 3) + 3) ;

  if ( (((offset == RtcTime.second) && ( (RtcTime.year < 2016) ||                  // Never synced
                                         (ntp_sync_minute == uptime_minute))) ||   // Re-sync every hour
       TasmotaGlobal.ntp_force_sync ) ) {                                          // Forced sync

    TasmotaGlobal.ntp_force_sync = false;

    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("NTP: Sync time..."));
    ntp_run_time = millis();
    uint64_t ntp_nanos = WifiGetNtp();
    uint32_t ntp_time = ntp_nanos / 1000000000;
    ntp_run_time = (millis() - ntp_run_time) / 1000;
//    AddLog(LOG_LEVEL_DEBUG, PSTR("NTP: Runtime %d"), ntp_run_time);
    if (ntp_run_time < 5) { ntp_run_time = 0; }  // DNS timeout is around 10s

    if (ntp_time > START_VALID_TIME) {
      Rtc.utc_time = ntp_time;
      Rtc.nanos = ntp_nanos % 1000000000;
      ntp_sync_minute = 60;             // Sync so block further requests
      RtcSync("NTP");
    } else {
      ntp_sync_minute++;                // Try again in next minute
    }
  }
}

/**
 * Retrieves the current time from an NTP server
 * 
 * @return Current time in nanoseconds since Unix epoch, or 0 on failure
 * 
 * This function implements the NTP client protocol:
 * 1. Selects an NTP server from configured options or fallbacks
 * 2. Resolves the server hostname to an IP address
 * 3. Creates a UDP socket with a random local port
 * 4. Sends an NTP request packet
 * 5. Waits for and processes the response
 * 
 * The function handles various error conditions:
 * - DNS resolution failures
 * - Socket creation failures
 * - Packet send/receive errors
 * - Invalid or unsynchronized server responses
 * 
 * If a server fails, the function increments ntp_server_id to try
 * the next configured server on the next attempt.
 */
uint64_t WifiGetNtp(void) {
  static uint8_t ntp_server_id = 0;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("NTP: Start NTP Sync %d ..."), ntp_server_id);

  IPAddress time_server_ip;

  char fallback_ntp_server[2][32];
  ext_snprintf_P(fallback_ntp_server[0], sizeof(fallback_ntp_server[0]), PSTR("%_I"), Settings->ipv4_address[1]);  // #17984
  ext_snprintf_P(fallback_ntp_server[1], sizeof(fallback_ntp_server[1]), PSTR("%d.pool.ntp.org"), random(0,3));

  char* ntp_server;
  for (uint32_t i = 0; i < MAX_NTP_SERVERS +2; i++) {
    if (ntp_server_id >= MAX_NTP_SERVERS +2) { ntp_server_id = 0; }
    ntp_server = (ntp_server_id < MAX_NTP_SERVERS) ? SettingsText(SET_NTPSERVER1 + ntp_server_id) : fallback_ntp_server[ntp_server_id - MAX_NTP_SERVERS];
    if (strlen(ntp_server)) {
      break;
    }
    ntp_server_id++;
  }
  if (!WifiHostByName(ntp_server, time_server_ip)) {
    ntp_server_id++;
//    AddLog(LOG_LEVEL_DEBUG, PSTR("NTP: Unable to resolve '%s'"), ntp_server);
    return 0;
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("NTP: NtpServer '%s' IP %_I"), ntp_server, (uint32_t)time_server_ip);

  WiFiUDP udp;

  uint32_t attempts = 3;
  while (attempts > 0) {
    uint32_t port = random(1025, 65535);   // Create a random port for the UDP connection.
#ifdef USE_IPV6
    if (udp.begin(IPAddress(IPv6), port) != 0) {
#else
    if (udp.begin(port) != 0) {
#endif
      break;
    }
    attempts--;
  }
  if (0 == attempts) { return 0; }

  while (udp.parsePacket() > 0) {          // Discard any previously received packets
    yield();
  }

  const uint32_t NTP_PACKET_SIZE = 48;     // NTP time is in the first 48 bytes of message
  uint8_t packet_buffer[NTP_PACKET_SIZE];  // Buffer to hold incoming & outgoing packets
  memset(packet_buffer, 0, NTP_PACKET_SIZE);
  packet_buffer[0]  = 0b11100011;          // LI, Version, Mode
  packet_buffer[1]  = 0;                   // Stratum, or type of clock
  packet_buffer[2]  = 6;                   // Polling Interval
  packet_buffer[3]  = 0xEC;                // Peer Clock Precision
  packet_buffer[12] = 49;
  packet_buffer[13] = 0x4E;
  packet_buffer[14] = 49;
  packet_buffer[15] = 52;

  if (udp.beginPacket(time_server_ip, 123) == 0) {  // NTP requests are to port 123
    ntp_server_id++;                                // Next server next time
    udp.stop();
    return 0;
  }
  udp.write(packet_buffer, NTP_PACKET_SIZE);
  udp.endPacket();

  uint32_t begin_wait = millis();
  while (!TimeReached(begin_wait + 1000)) {         // Wait up to one second
    uint32_t size        = udp.parsePacket();
    uint32_t remote_port = udp.remotePort();

    if ((size >= NTP_PACKET_SIZE) && (remote_port == 123)) {
      udp.read(packet_buffer, NTP_PACKET_SIZE);     // Read packet into the buffer
      udp.stop();

      if ((packet_buffer[0] & 0b11000000) == 0b11000000) {
        // Leap-Indicator: unknown (clock unsynchronized)
        // See: https://github.com/letscontrolit/ESPEasy/issues/2886#issuecomment-586656384
        AddLog(LOG_LEVEL_DEBUG, PSTR("NTP: IP %_I unsynced"), (uint32_t)time_server_ip);
        ntp_server_id++;                            // Next server next time
        return 0;
      }

      // convert four bytes starting at location 40 to a long integer
      // TX time is used here.
      uint32_t secs_since_1900 = (uint32_t)packet_buffer[40] << 24;
      secs_since_1900 |= (uint32_t)packet_buffer[41] << 16;
      secs_since_1900 |= (uint32_t)packet_buffer[42] << 8;
      secs_since_1900 |= (uint32_t)packet_buffer[43];
      if (0 == secs_since_1900) {                   // No time stamp received
        ntp_server_id++;                            // Next server next time
        return 0;
      }
      uint32_t tmp_fraction = (uint32_t)packet_buffer[44] << 24;
      tmp_fraction |= (uint32_t)packet_buffer[45] << 16;
      tmp_fraction |= (uint32_t)packet_buffer[46] << 8;
      tmp_fraction |= (uint32_t)packet_buffer[47];
      uint32_t fraction = (((uint64_t)tmp_fraction) * 1000000000) >> 32;
      return (((uint64_t)secs_since_1900) - 2208988800UL) * 1000000000 + fraction;
    }
    delay(10);
  }
  // Timeout.
  AddLog(LOG_LEVEL_DEBUG, PSTR("NTP: No reply from %_I"), (uint32_t)time_server_ip);
  udp.stop();
  ntp_server_id++;                                  // Next server next time
  return 0;
}

// --------------------------------------------------------------------------------
// Respond to some Arduino/esp-idf events for better IPv6 support
// --------------------------------------------------------------------------------
#ifdef ESP32
extern esp_netif_t* get_esp_interface_netif(esp_interface_t interface);

// typedef void (*WiFiEventSysCb)(arduino_event_t *event);

/**
 * Event handler for ESP32 WiFi and network events
 * 
 * @param event Pointer to the arduino_event_t structure containing event details
 * 
 * This function processes WiFi and network events on ESP32 platforms:
 * 
 * 1. IPv6 address assignment:
 *    - Logs when global or local IPv6 addresses are assigned
 *    - Distinguishes between WiFi and Ethernet interfaces
 * 
 * 2. WiFi connection events:
 *    - Creates IPv6 link-local addresses when WiFi connects
 *    - Works around race conditions in the ESP-IDF LWIP implementation
 * 
 * 3. IPv4 address assignment:
 *    - Logs when IPv4 addresses are assigned
 *    - Includes subnet mask and gateway information
 * 
 * The function also ensures DNS servers are properly maintained by calling
 * WiFiHelper::scrubDNS() to restore DNS settings that might be zeroed by
 * internal reconnection processes.
 */
void WifiEvents(arduino_event_t *event) {
  switch (event->event_id) {

#ifdef USE_IPV6
    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
    {
// Serial.printf(">>> event ARDUINO_EVENT_WIFI_STA_GOT_IP6 \n");
      IPAddress addr(IPv6, (const uint8_t*)event->event_info.got_ip6.ip6_info.ip.addr, event->event_info.got_ip6.ip6_info.ip.zone);
      AddLog(LOG_LEVEL_DEBUG, PSTR("%s: IPv6 %s %s"),
             event->event_id == ARDUINO_EVENT_ETH_GOT_IP6 ? "ETH" : "WIF",
             IPv6isLocal(addr) ? PSTR("Local") : PSTR("Global"), addr.toString(true).c_str());
    }
    break;

    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      // workaround for the race condition in LWIP, see https://github.com/espressif/arduino-esp32/pull/9016#discussion_r1451774885
      {
        uint32_t i = 5;   // try 5 times only
        while (esp_netif_create_ip6_linklocal(get_esp_interface_netif(ESP_IF_WIFI_STA)) != ESP_OK) {
          delay(1);
          if (i-- == 0) {
            break;
          }
        }
      }
    break;
#endif // USE_IPV6
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
    {
// Serial.printf(">>> event ARDUINO_EVENT_WIFI_STA_GOT_IP \n");
      ip_addr_t ip_addr4;
      ip_addr_copy_from_ip4(ip_addr4, event->event_info.got_ip.ip_info.ip);
      AddLog(LOG_LEVEL_DEBUG, PSTR("WIF: IPv4 %_I, mask %_I, gateway %_I"),
              event->event_info.got_ip.ip_info.ip.addr,
              event->event_info.got_ip.ip_info.netmask.addr,
              event->event_info.got_ip.ip_info.gw.addr);
    }
    break;

    default:
      break;
  }
  WiFiHelper::scrubDNS();    // internal calls to reconnect can zero the DNS servers, restore the previous values
}
#endif // ESP32
