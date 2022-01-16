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

// sorry, no <AddrList.h>
#undef LWIP_IPV6

#define ENC_TYPE_NONE 	WIFI_AUTH_OPEN
#define ENC_TYPE_WEP  	WIFI_AUTH_WEP
#define ENC_TYPE_CCMP  	WIFI_AUTH_WPA2_PSK
#define ENC_TYPE_TKIP  	WIFI_AUTH_WPA_WPA2_PSK
#define ENC_TYPE_AUTO   WIFI_AUTH_MAX + 1

#define WIFI_NONE_SLEEP     0
#define WIFI_LIGHT_SLEEP	1
#define WIFI_MODEM_SLEEP	2

class WiFiClass32 : public WiFiClass
{
public:
    static void hostname(const char* aHostname)
    {
        WiFi.setHostname(aHostname);
    }
    static void setSleepMode(int iSleepMode);
    static int getPhyMode();

    static void wps_disable();
    static void setOutputPower(int n);
    static void forceSleepBegin();
    static void forceSleepWake();
    static bool getNetworkInfo(uint8_t i, String &ssid, uint8_t &encType, int32_t &rssi, uint8_t* &bssid, int32_t &channel, bool &hidden_scan);
};

void wifi_station_disconnect();
void wifi_station_dhcpc_start();
extern WiFiClass32 WiFi32;
#define WiFi WiFi32
