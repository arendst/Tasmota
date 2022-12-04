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
//
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <esp_wifi.h>

//
// Wifi
//
#ifdef WiFi
#undef WiFi
#endif

void WiFiClass32::setSleepMode(int iSleepMode) {
  // WIFI_LIGHT_SLEEP and WIFI_MODEM_SLEEP
  WiFi.setSleep(iSleepMode != WIFI_NONE_SLEEP);
}

int WiFiClass32::getPhyMode() {
  int phy_mode = 0;  // " BGNL"
  uint8_t protocol_bitmap;
  if (esp_wifi_get_protocol(WIFI_IF_STA, &protocol_bitmap) == ESP_OK) {
    if (protocol_bitmap & 1) { phy_mode = WIFI_PHY_MODE_11B; }  // 1 = 11b
    if (protocol_bitmap & 2) { phy_mode = WIFI_PHY_MODE_11G; }  // 2 = 11bg
    if (protocol_bitmap & 4) { phy_mode = WIFI_PHY_MODE_11N; }  // 3 = 11bgn
    if (protocol_bitmap & 8) { phy_mode = 4; }  // Low rate
  }
  return phy_mode;
}

bool WiFiClass32::setPhyMode(WiFiPhyMode_t mode) {
  uint8_t protocol_bitmap = WIFI_PROTOCOL_11B;     // 1
  switch (mode) {
    case 3: protocol_bitmap |= WIFI_PROTOCOL_11N;  // 4
    case 2: protocol_bitmap |= WIFI_PROTOCOL_11G;  // 2
  }
  return (ESP_OK == esp_wifi_set_protocol(WIFI_IF_STA, protocol_bitmap));
}

void WiFiClass32::wps_disable() {
}

void WiFiClass32::setOutputPower(int n) {
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

void WiFiClass32::forceSleepBegin() {
}

void WiFiClass32::forceSleepWake() {
}

bool WiFiClass32::getNetworkInfo(uint8_t i, String &ssid, uint8_t &encType, int32_t &rssi, uint8_t *&bssid, int32_t &channel, bool &hidden_scan) {
    hidden_scan = false;
    return WiFi.getNetworkInfo(i, ssid, encType, rssi, bssid, channel);
}

// from https://github.com/espressif/arduino-esp32/pull/7520
static const int WIFI_WANT_IP6_BIT_ALT = BIT15;
bool WiFiClass32::IPv6(bool state) {
  if (state)
    WiFiGenericClass::setStatusBits(WIFI_WANT_IP6_BIT_ALT);
  else
    WiFiGenericClass::clearStatusBits(WIFI_WANT_IP6_BIT_ALT);
  return true;
}

void wifi_station_disconnect() {
    // erase ap: empty ssid, ...
    WiFi.disconnect(true, true);
}

void wifi_station_dhcpc_start() {
}

WiFiClass32 WiFi32;
