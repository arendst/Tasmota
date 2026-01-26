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
#ifdef ESP8266
#include "Arduino.h"
#include "WiFiHelper.h"

wl_status_t WiFiHelper::begin(const char* ssid, const char *passphrase, int32_t channel, const uint8_t* bssid, bool connect) {
  return WiFi.begin(ssid, passphrase, channel, bssid, connect);
}

wl_status_t WiFiHelper::begin(char* ssid, char *passphrase, int32_t channel, const uint8_t* bssid, bool connect) {
  return WiFi.begin(ssid, passphrase, channel, bssid, connect);
}

wl_status_t WiFiHelper::begin() {
  return WiFi.begin();
}

void WiFiHelper::scrubDNS(void) {
}


void WiFiHelper::hostname(const char* aHostname) {
  WiFi.hostname(aHostname);
}

void WiFiHelper::setSleepMode(int iSleepMode) {
  WiFi.setSleepMode((WiFiSleepType_t)iSleepMode);
}

int WiFiHelper::getPhyMode() {
  return WiFi.getPhyMode();
}

bool WiFiHelper::setPhyMode(WiFiPhyMode_t mode) {
  return WiFi.setPhyMode(mode);
}

void WiFiHelper::setOutputPower(int n) {
  WiFi.setOutputPower(n);
}
void WiFiHelper::forceSleepBegin() {
  WiFi.forceSleepBegin();
}
void WiFiHelper::forceSleepWake() {
  WiFi.forceSleepWake();
}
bool WiFiHelper::getNetworkInfo(uint8_t i, String &ssid, uint8_t &encType, int32_t &rssi, uint8_t* &bssid, int32_t &channel, bool &hidden_scan) {
  return WiFi.getNetworkInfo(i, ssid, encType, rssi, bssid, channel, hidden_scan);
}

int WiFiHelper::hostByName(const char* aHostname, IPAddress& aResult, int32_t timer_ms) {
  return WiFi.hostByName(aHostname, aResult, timer_ms);
}
int WiFiHelper::hostByName(const char* aHostname, IPAddress& aResult) {
  return WiFi.hostByName(aHostname, aResult);
}

String WiFiHelper::macAddress(void) {
  return WiFi.macAddress();
}

void WiFiHelper::IPv6ZoneAutoFix(IPAddress &addr, const char* aHostname) {
}
#endif // ESP8266
