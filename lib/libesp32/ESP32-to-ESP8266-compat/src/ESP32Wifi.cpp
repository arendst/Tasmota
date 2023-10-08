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

extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

//
// Wifi
//
#ifdef WiFi
#undef WiFi
#endif

#include "tasmota_options.h"
#include "lwip/dns.h"

wl_status_t WiFiClass32::begin(const char* wpa2_ssid, wpa2_auth_method_t method, const char* wpa2_identity, const char* wpa2_username, const char *wpa2_password, const char* ca_pem, const char* client_crt, const char* client_key, int32_t channel, const uint8_t* bssid, bool connect) {
  scrubDNS();
  wl_status_t ret = WiFiClass::begin(wpa2_ssid, method, wpa2_identity, wpa2_username, wpa2_password, ca_pem, client_crt, client_key, channel, bssid, connect);
  scrubDNS();
  return ret;
}

wl_status_t WiFiClass32::begin(const char* ssid, const char *passphrase, int32_t channel, const uint8_t* bssid, bool connect) {
  scrubDNS();
  wl_status_t ret = WiFiClass::begin(ssid, passphrase, channel, bssid, connect);
  scrubDNS();
  return ret;
}

wl_status_t WiFiClass32::begin(char* ssid, char *passphrase, int32_t channel, const uint8_t* bssid, bool connect) {
  scrubDNS();
  wl_status_t ret = WiFiClass::begin(ssid, passphrase, channel, bssid, connect);
  scrubDNS();
  return ret;
}
wl_status_t WiFiClass32::begin() {
  scrubDNS();
  wl_status_t ret = WiFiClass::begin();
  scrubDNS();
  return ret;
}

// scrubDNS
//
// LWIP has a single DNS table for all interfaces and for v4/v6
// Unfortunately when trying to connect to Wifi, the dns server table is erased.
//
// We restore DNS previous values if they are empty
// We restore or erase DNS entries if they are unsupported (v4 vs v6)
extern bool WifiHasIPv4(void);
extern bool EthernetHasIPv4(void);
extern bool WifiHasIPv6(void);
extern bool EthernetHasIPv6(void);

void WiFiClass32::scrubDNS(void) {
  // String dns_entry0 = IPAddress(dns_getserver(0)).toString();
  // String dns_entry1 = IPAddress(dns_getserver(1)).toString();
  // scan DNS entries
  bool has_v4 = WifiHasIPv4() || EthernetHasIPv4();
  bool has_v6 = false;
#ifdef USE_IPV6
  has_v6 = WifiHasIPv6() || EthernetHasIPv6();
#endif

  // First pass, save values
  for (uint32_t i=0; i<DNS_MAX_SERVERS; i++) {
#ifdef USE_IPV6
    const IPAddress ip_dns = IPAddress(dns_getserver(i));
    // Step 1. save valid values from DNS
    if (!ip_addr_isany_val((const ip_addr_t &)ip_dns)) {
      if (ip_dns.isV4() && has_v4) {
        dns_save4[i] = (ip_addr_t) ip_dns;    // dns entry is populated, save it in v4 slot
      } else if (ip_dns.isV6() && has_v6) {
        dns_save6[i] = (ip_addr_t) ip_dns;    // dns entry is populated, save it in v6 slot
      }
    }

    // Step 2. scrub addresses not supported
    if (!has_v4) { dns_save4[i] = *IP4_ADDR_ANY; }
    if (!has_v6) { dns_save6[i] = *IP_ADDR_ANY; }

    // Step 3. restore saved value
    if (has_v4 && has_v6) {   // if both IPv4 and IPv6 are active, prefer IPv4
      if (!ip_addr_isany_val(dns_save4[i])) { dns_setserver(i, &dns_save4[i]); }
      else { dns_setserver(i, &dns_save6[i]); }
    } else if (has_v4) {
      dns_setserver(i, &dns_save4[i]);
    } else if (has_v6) {
      dns_setserver(i, &dns_save6[i]);
    } else {
      dns_setserver(i, IP4_ADDR_ANY);
    }
#endif // USE_IPV6
  }
  // AddLog(LOG_LEVEL_DEBUG, "IP>: DNS: from(%s %s) to (%s %s) has4/6:%i-%i", dns_entry0.c_str(), dns_entry1.c_str(), IPAddress(dns_getserver(0)).toString().c_str(),  IPAddress(dns_getserver(1)).toString().c_str(), has_v4, has_v6);
}

void WiFiClass32::setSleepMode(int iSleepMode) {
  // WIFI_LIGHT_SLEEP and WIFI_MODEM_SLEEP
  WiFi.setSleep(iSleepMode != WIFI_NONE_SLEEP);
}

int WiFiClass32::getPhyMode() {
  int phy_mode = 0;  // " BGNL"
  uint8_t protocol_bitmap;
  if (esp_wifi_get_protocol(WIFI_IF_STA, &protocol_bitmap) == ESP_OK) {
    if (protocol_bitmap & 1) { phy_mode = TAS_WIFI_PHY_MODE_11B; }  // 1 = 11b (WIFI_PHY_MODE_11B)
    if (protocol_bitmap & 2) { phy_mode = TAS_WIFI_PHY_MODE_11G; }  // 2 = 11bg (WIFI_PHY_MODE_11G)
    if (protocol_bitmap & 4) { phy_mode = TAS_WIFI_PHY_MODE_11N; }  // 3 = 11bgn (WIFI_PHY_MODE_11N)
    if (protocol_bitmap & 8) { phy_mode = 4; }  // Low rate (WIFI_PHY_MODE_LR)
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

//
// Manage dns callbacks from lwip DNS resolver.
// We need a trick here, because the callback may be called after we timed-out and
// launched a new request. We need to discard outdated responses.
//
// We use a static ip_addr_t (anyways we don't support multi-threading)
// and use a counter so the callback can check if it's responding to the current
// request or to an old one (hence discard)
//
// It's not an issue to have old requests in flight. LWIP has a default limit of 4
// DNS requests in flight.
// If the buffer for in-flight requests is full, LWIP removes the oldest from the list.
// (it does not block new DNS resolutions)
static ip_addr_t dns_ipaddr;
static volatile uint32_t ip_addr_counter = 0;   // counter for requests
extern int32_t WifiDNSGetTimeout(void);
extern bool WifiDNSGetIPv6Priority(void);

static void wifi32_dns_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
  // Serial.printf("DNS: cb name=%s ipaddr=%s arg=%i counter=%i\n", name ? name : "<null>", IPAddress(ipaddr).toString().c_str(), (int) callback_arg, ip_addr_counter);
  uint32_t cb_counter = (uint32_t) callback_arg;
  if (cb_counter != ip_addr_counter) { return; }    // the response is from a previous request, ignore

  if (ipaddr != nullptr) {
    dns_ipaddr = *ipaddr;
  } else {
    dns_ipaddr = *IP4_ADDR_ANY;   // set to IPv4 0.0.0.0
  }
  WiFiClass32::dnsDone();
  // AddLog(LOG_LEVEL_DEBUG, "WIF: dns_found=%s", ipaddr ? IPAddress(*ipaddr).toString().c_str() : "<null>");
}
// We need this helper method to access protected methods from WiFiGeneric
void WiFiClass32::dnsDone(void) {
  setStatusBits(WIFI_DNS_DONE_BIT);
}

/**
 * Resolve the given hostname to an IP address.
 * @param aHostname     Name to be resolved
 * @param aResult       IPAddress structure to store the returned IP address
 * @return 1 if aIPAddrString was successfully converted to an IP address,
 *          else error code
 */
int WiFiClass32::hostByName(const char* aHostname, IPAddress& aResult, int32_t timer_ms)
{
  ip_addr_t addr;
  aResult = (uint32_t) 0;     // by default set to IPv4 0.0.0.0
  dns_ipaddr = *IP4_ADDR_ANY;  // by default set to IPv4 0.0.0.0

  scrubDNS();    // internal calls to reconnect can zero the DNS servers, save DNS for future use
  ip_addr_counter++;      // increase counter, from now ignore previous responses
  clearStatusBits(WIFI_DNS_IDLE_BIT | WIFI_DNS_DONE_BIT);
  uint8_t v4v6priority = LWIP_DNS_ADDRTYPE_IPV4;
#ifdef USE_IPV6
  v4v6priority = WifiDNSGetIPv6Priority() ? LWIP_DNS_ADDRTYPE_IPV6_IPV4 : LWIP_DNS_ADDRTYPE_IPV4_IPV6;
#endif // USE_IPV6
  err_t err = dns_gethostbyname_addrtype(aHostname, &dns_ipaddr, &wifi32_dns_found_callback, (void*) ip_addr_counter, v4v6priority);
  // Serial.printf("DNS: dns_gethostbyname_addrtype errg=%i counter=%i\n", err, ip_addr_counter);
  if(err == ERR_OK && !ip_addr_isany_val(dns_ipaddr)) {
#ifdef USE_IPV6
    aResult = dns_ipaddr;
#else // USE_IPV6
    aResult = ip_addr_get_ip4_u32(&dns_ipaddr);
#endif // USE_IPV6
  } else if(err == ERR_INPROGRESS) {
    waitStatusBits(WIFI_DNS_DONE_BIT, timer_ms);  //real internal timeout in lwip library is 14[s]
    clearStatusBits(WIFI_DNS_DONE_BIT);
  }

  if (!ip_addr_isany_val(dns_ipaddr)) {
#ifdef USE_IPV6
    aResult = dns_ipaddr;
#else // USE_IPV6
    aResult = ip_addr_get_ip4_u32(&dns_ipaddr);
#endif // USE_IPV6
    return true;
  }
  return false;
}

int WiFiClass32::hostByName(const char* aHostname, IPAddress& aResult)
{
  return hostByName(aHostname, aResult, WifiDNSGetTimeout());
}

void wifi_station_disconnect() {
    // erase ap: empty ssid, ...
    WiFi.disconnect(true, true);
}

void wifi_station_dhcpc_start() {
}

WiFiClass32 WiFi32;
