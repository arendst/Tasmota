/*
  WiFiHelper.h - provide a wrapper for differences between ESP8266 and ESP32 WiFi

  Copyright (C) 2024  Theo Arends / Stephan Hadinger

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
#ifndef WIFIHELPER_H
#define WIFIHELPER_H


// ======================================================
// ESP8266 specific section
// ======================================================
#ifdef ESP8266
#include "ESP8266WiFi.h"
#endif


// ======================================================
// ESP32 specific section
// ======================================================
#ifdef ESP32
#include <WiFi.h>

#define ENC_TYPE_NONE 	WIFI_AUTH_OPEN
#define ENC_TYPE_WEP  	WIFI_AUTH_WEP
#define ENC_TYPE_CCMP  	WIFI_AUTH_WPA2_PSK
#define ENC_TYPE_TKIP  	WIFI_AUTH_WPA_WPA2_PSK
#define ENC_TYPE_AUTO   WIFI_AUTH_MAX + 1

#define WIFI_NONE_SLEEP     0
#define WIFI_LIGHT_SLEEP	1
#define WIFI_MODEM_SLEEP	2

// ESP8266
typedef enum WiFiPhyMode
{
    TAS_WIFI_PHY_MODE_LR = 0, TAS_WIFI_PHY_MODE_11B = 1, TAS_WIFI_PHY_MODE_11G = 2, TAS_WIFI_PHY_MODE_11N = 3
#if ESP_IDF_VERSION_MAJOR >= 5
    , TAS_WIFI_PHY_MODE_11AX = 4
#endif
} WiFiPhyMode_t;

#endif // ESP32

// This is an abstract class containing wrappers to call WiFi
class WiFiHelper {
public:
#ifdef ESP32
  static wl_status_t begin(const char* wpa2_ssid, wpa2_auth_method_t method, const char* wpa2_identity=NULL, const char* wpa2_username=NULL, const char *wpa2_password=NULL, const char* ca_pem=NULL, const char* client_crt=NULL, const char* client_key=NULL, int32_t channel=0, const uint8_t* bssid=0, bool connect=true);
#endif
  static wl_status_t begin(const char* ssid, const char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
  static wl_status_t begin(char* ssid, char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
  static wl_status_t begin();

  static void hostname(const char* aHostname);
  static void setSleepMode(int iSleepMode);
  static int getPhyMode();
  static bool setPhyMode(WiFiPhyMode_t mode);

  static void setOutputPower(int n);
  static void forceSleepBegin();
  static void forceSleepWake();
  static bool getNetworkInfo(uint8_t i, String &ssid, uint8_t &encType, int32_t &rssi, uint8_t* &bssid, int32_t &channel, bool &hidden_scan);

  static int hostByName(const char* aHostname, IPAddress& aResult, int32_t timer_ms);
  static int hostByName(const char* aHostname, IPAddress& aResult);

  static void scrubDNS(void);

  // With ESP32 Core3, the WiFi mac address is not valid until the wifi is actually started
  // this helper function always provide a valid mac address
  static String macAddress(void);

  // Auto-fix zone
  //
  // After a reconnect, the zone id may not be valid anymore
  // In such case we detect any "%st<n>" or "%en<n>" zone identifier
  // and replace with the current zone id
  static void IPv6ZoneAutoFix(IPAddress &addr, const char* aHostname);
};



#endif // WIFIHELPER_H