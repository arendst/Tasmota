/*
  WiFi compat with ESP32

  Copyright (C) 2021  Theo Arends / Jörg Schüler-Maroldt

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
#pragma once
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
} WiFiPhyMode_t;

/*
// ESP32 was never defined until IDF 4.4
typedef enum
{
    WIFI_PHY_MODE_LR,   // PHY mode for Low Rate
    WIFI_PHY_MODE_11B,  // PHY mode for 11b
    WIFI_PHY_MODE_11G,  // PHY mode for 11g
    WIFI_PHY_MODE_HT20, // PHY mode for 11n Bandwidth HT20
    WIFI_PHY_MODE_HT40, // PHY mode for 11n Bandwidth HT40
    WIFI_PHY_MODE_HE20, // PHY mode for 11n Bandwidth HE20
} wifi_phy_mode_t;
*/

class WiFiClass32 : public WiFiClass
{
public:
    wl_status_t begin(const char* wpa2_ssid, wpa2_auth_method_t method, const char* wpa2_identity=NULL, const char* wpa2_username=NULL, const char *wpa2_password=NULL, const char* ca_pem=NULL, const char* client_crt=NULL, const char* client_key=NULL, int32_t channel=0, const uint8_t* bssid=0, bool connect=true);
    wl_status_t begin(const char* ssid, const char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
    wl_status_t begin(char* ssid, char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
    wl_status_t begin();

    static void hostname(const char* aHostname)
    {
        WiFi.setHostname(aHostname);
    }
    static void setSleepMode(int iSleepMode);
    static int getPhyMode();
    static bool setPhyMode(WiFiPhyMode_t mode);

    static void wps_disable();
    static void setOutputPower(int n);
    static void forceSleepBegin();
    static void forceSleepWake();
    static bool getNetworkInfo(uint8_t i, String &ssid, uint8_t &encType, int32_t &rssi, uint8_t* &bssid, int32_t &channel, bool &hidden_scan);

    static void dnsDone(void);      // used by the callback to stop the dns timer
    int hostByName(const char* aHostname, IPAddress& aResult, int32_t timer_ms);
    int hostByName(const char* aHostname, IPAddress& aResult);

    void scrubDNS(void);
protected:
#ifdef USE_IPV6
    ip_addr_t dns_save4[DNS_MAX_SERVERS] = {};      // IPv4 DNS servers
    ip_addr_t dns_save6[DNS_MAX_SERVERS] = {};      // IPv6 DNS servers
#endif // USE_IPV6
};

void wifi_station_disconnect();
void wifi_station_dhcpc_start();
extern WiFiClass32 WiFi32;
#define WiFi WiFi32
