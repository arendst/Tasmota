/*
  WiFi compat with ESP32

  Copyright (C) 2020  Theo Arends / Jörg Schüler-Maroldt

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
//
#include "Arduino.h"
#include <ESP8266WiFi.h>

//
// Wifi
//
#ifdef WiFi
#undef WiFi
#endif

void WiFiClass32::setSleepMode(int iSleepMode)
{
    // WIFI_MODEM_SLEEP
    WiFi.setSleep(iSleepMode != WIFI_MODEM_SLEEP);
}

int WiFiClass32::getPhyMode()
{
    return 0; // " BGN"
}

void WiFiClass32::wps_disable()
{
}

void WiFiClass32::setOutputPower(int n)
{
    wifi_power_t p = WIFI_POWER_2dBm;
    if (n > 19)
        p = WIFI_POWER_19_5dBm;
    else if (n > 18)
        p = WIFI_POWER_18_5dBm;
    else if (n >= 17)
        p = WIFI_POWER_17dBm;
    else if (n >= 15)
        p = WIFI_POWER_15dBm;
    else if (n >= 13)
        p = WIFI_POWER_13dBm;
    else if (n >= 11)
        p = WIFI_POWER_11dBm;
    else if (n >= 8)
        p = WIFI_POWER_8_5dBm;
    else if (n >= 7)
        p = WIFI_POWER_7dBm;
    else if (n >= 5)
        p = WIFI_POWER_5dBm;
    WiFi.setTxPower(p);
}

void WiFiClass32::forceSleepBegin()
{
}

void WiFiClass32::forceSleepWake()
{
}

bool WiFiClass32::getNetworkInfo(uint8_t i, String &ssid, uint8_t &encType, int32_t &rssi, uint8_t *&bssid, int32_t &channel, bool &hidden_scan)
{
    hidden_scan = false;
    return WiFi.getNetworkInfo(i, ssid, encType, rssi, bssid, channel);
}

void wifi_station_disconnect()
{
    // erase ap: empty ssid, ...
    WiFi.disconnect(true, true);
}

void wifi_station_dhcpc_start()
{
}

WiFiClass32 WiFi32;
