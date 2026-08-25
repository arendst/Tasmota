/*
  xdrv_89_esp32_shelly_emulation.ino - Shelly emulation for Tasmota

  Copyright (C) 2026  Rene Hexel

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

#if defined(ESP32) && defined(USE_WEBSERVER) && defined(USE_EMULATION) && defined(USE_EMULATION_SHELLY)
/*********************************************************************************************\
 * Shelly emulation
 *
 * Presents Tasmota as a generation 2 Shelly Pro 2PM (SPSW-202PE16EU) to controllers
 * speaking the Shelly Gen2 RPC protocol, such as the Sigenergy mySigen application.
 *
 * Advertises _shelly._tcp over mDNS and serves /shelly and /rpc over HTTP. Two switch,
 * two input and two metering channels are exposed, mapped onto Tasmota relays, switches
 * and the energy driver where present. Channels beyond the relays actually configured
 * keep their state in NVS so a controller still sees two working outputs.
 *
 * Select with command `Emulation 3` or on the Configure Other web page.
\*********************************************************************************************/

#define XDRV_89 89

#define SHELLY_SETTINGS_VERSION 0x0101
#define SHELLY_MODEL "SPSW-202PE16EU"
#define SHELLY_APP "Pro2PM"
#define SHELLY_VERSION "2.0.0"
#define SHELLY_FW_ID "20260710-101218/2.0.0-g87fbfa4"

#include <esp_mac.h>
#include <uri/UriGlob.h>
#ifdef USE_DISCOVERY
#include "mdns.h"
#include "lwip/udp.h"
#include "lwip/netif.h"
#endif

struct ShellyEmulationSettings_t {
  uint16_t version;
  uint8_t virtual_power;
  uint8_t reserved;
};

struct ShellyEmulationGlobal_t {
  char mac[13];
  char mac_lower[13];
  char id[32];
  char hostname[32];
  uint32_t mdns_signature;
  bool mdns_active;
  bool web_handlers_added;
};

ShellyEmulationSettings_t ShellyEmulationSettings;
ShellyEmulationGlobal_t ShellyEmulation;

/*********************************************************************************************\
 * Identity and persistent state
\*********************************************************************************************/

static String ShellyJsonBool(bool value) {
  return value ? F("true") : F("false");
}

static String ShellyFloat(float value, uint8_t decimals) {
  if (!std::isfinite(value)) { value = 0.0f; }
  return String(value, (unsigned int)decimals);
}

static void ShellyBuildIdentity(void) {
  uint8_t mac[6] = {};
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  snprintf(ShellyEmulation.mac, sizeof(ShellyEmulation.mac), "%02X%02X%02X%02X%02X%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  snprintf(ShellyEmulation.mac_lower, sizeof(ShellyEmulation.mac_lower), "%02x%02x%02x%02x%02x%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  snprintf(ShellyEmulation.id, sizeof(ShellyEmulation.id), "shellypro2pm-%s", ShellyEmulation.mac_lower);
  snprintf(ShellyEmulation.hostname, sizeof(ShellyEmulation.hostname), "ShellyPro2PM-%s", ShellyEmulation.mac);
}

static void ShellySettingsDefault(void) {
  memset(&ShellyEmulationSettings, 0, sizeof(ShellyEmulationSettings));
  ShellyEmulationSettings.version = SHELLY_SETTINGS_VERSION;
}

static void ShellySettingsSave(void) {
  NvmSave("shellyemu", "settings", &ShellyEmulationSettings, sizeof(ShellyEmulationSettings));
}

static void ShellySettingsLoad(bool erase) {
  if (erase) { NvmErase("shellyemu"); }
  ShellySettingsDefault();
  ShellyEmulationSettings_t loaded = {};
  if (!erase && NvmLoad("shellyemu", "settings", &loaded, sizeof(loaded)) &&
      loaded.version == SHELLY_SETTINGS_VERSION) {
    ShellyEmulationSettings = loaded;
  } else {
    ShellySettingsSave();
  }
}

/*********************************************************************************************\
 * Tasmota relay, switch and energy mapping
\*********************************************************************************************/

static bool ShellyPower(uint8_t id) {
  if (id < TasmotaGlobal.devices_present) {
    return bitRead(TasmotaGlobal.power, id);
  }
  return bitRead(ShellyEmulationSettings.virtual_power, id);
}

static void ShellySetPower(uint8_t id, bool state) {
  // A controller may repeat a set with the value the channel already holds.
  // Rewriting persistent state for such requests wears the flash needlessly.
  if (ShellyPower(id) == state) { return; }
  if (id < TasmotaGlobal.devices_present) {
    ExecuteCommandPower(id + 1, state ? POWER_ON : POWER_OFF, SRC_REMOTE);
  } else {
    bitWrite(ShellyEmulationSettings.virtual_power, id, state);
    ShellySettingsSave();
  }
}

static bool ShellyInput(uint8_t id) {
  return SwitchUsed(id) ? SwitchGetState(id) : false;
}

static float ShellyEnergyValue(const float *values, uint8_t id) {
#ifdef USE_ENERGY_SENSOR
  if (Energy && id < Energy->phase_count && std::isfinite(values[id])) { return values[id]; }
#endif  // USE_ENERGY_SENSOR
  return 0.0f;
}

static uint32_t ShellyMinuteTimestamp(void) {
  uint32_t now = UtcTime();
  return now - (now % 60);
}

/*********************************************************************************************\
 * Component payloads
\*********************************************************************************************/

static String ShellySwitchStatus(uint8_t id) {
  float power = 0.0f;
  float voltage = 0.0f;
  float frequency = 0.0f;
  float current = 0.0f;
  float pf = 0.0f;
  float imported = 0.0f;
  float returned = 0.0f;
#ifdef USE_ENERGY_SENSOR
  if (Energy) {
    power = ShellyEnergyValue(Energy->active_power, id);
    voltage = ShellyEnergyValue(Energy->voltage, id);
    frequency = ShellyEnergyValue(Energy->frequency, id);
    current = ShellyEnergyValue(Energy->current, id);
    pf = ShellyEnergyValue(Energy->power_factor, id);
    imported = ShellyEnergyValue(Energy->import_active, id) * 1000.0f;
    returned = ShellyEnergyValue(Energy->export_active, id) * 1000.0f;
  }
#endif  // USE_ENERGY_SENSOR
  if (!ShellyPower(id)) {
    power = 0.0f;
    current = 0.0f;
    pf = 0.0f;
  }

  String json;
  json.reserve(620);
  json += F("{\"id\":"); json += id;
  json += F(",\"source\":\"WS_in\",\"tag\":null,\"output\":"); json += ShellyJsonBool(ShellyPower(id));
  json += F(",\"apower\":"); json += ShellyFloat(power, 1);
  json += F(",\"voltage\":"); json += ShellyFloat(voltage, 1);
  json += F(",\"freq\":"); json += ShellyFloat(frequency, 1);
  json += F(",\"current\":"); json += ShellyFloat(current, 3);
  json += F(",\"pf\":"); json += ShellyFloat(pf, 2);
  json += F(",\"aenergy\":{\"total\":"); json += ShellyFloat(imported, 3);
  json += F(",\"by_minute\":[0.000,0.000,0.000],\"minute_ts\":"); json += ShellyMinuteTimestamp();
  json += F("},\"ret_aenergy\":{\"total\":"); json += ShellyFloat(returned, 3);
  json += F(",\"by_minute\":[0.000,0.000,0.000],\"minute_ts\":"); json += ShellyMinuteTimestamp();
  json += F("},\"counts\":{\"on_time\":0,\"on_time_rst_ts\":0,\"switch_on\":0,\"switch_on_rst_ts\":0,\"on_above_thr\":0,\"on_above_thr_rst_ts\":0}");
  json += F(",\"temperature\":{\"tC\":0.0,\"tF\":32.0}}");
  return json;
}

static String ShellySwitchConfig(uint8_t id) {
  String json;
  json.reserve(400);
  json += F("{\"id\":"); json += id;
  json += F(",\"name\":null,\"in_mode\":\"detached\",\"in_locked\":false,\"initial_state\":\"off\",\"auto_on\":false,\"auto_on_delay\":60.00,\"auto_off\":false,\"auto_off_delay\":60.00,\"power_limit\":4480,\"voltage_limit\":280,\"undervoltage_limit\":0,\"autorecover_voltage_errors\":false,\"current_limit\":16.000,\"reverse\":false,\"counts\":{\"enable\":true,\"power_thr\":100}}");
  return json;
}

static String ShellyInputStatus(uint8_t id) {
  return String(F("{\"id\":")) + id + F(",\"state\":") + ShellyJsonBool(ShellyInput(id)) + '}';
}

static String ShellyInputConfig(uint8_t id) {
  return String(F("{\"id\":")) + id + F(",\"name\":null,\"type\":\"switch\",\"enable\":true,\"invert\":false}");
}

static String ShellyBleStatus(void) {
  uint8_t address[6];
  WiFi.macAddress(address);
  address[5] += 2;  // Shelly Gen2 derives its public BLE address from the Wi-Fi MAC.
  char ble_address[20];
  snprintf(ble_address, sizeof(ble_address), "%02x:%02x:%02x:%02x:%02x:%02x,p",
           address[0], address[1], address[2], address[3], address[4], address[5]);
  return String(F("{\"addr\":\"")) + ble_address + F("\",\"flags\":[]}");
}

static String ShellyEthStatus(void) {
  // A real Pro 2PM reports its Ethernet address here; without a link (or on a
  // build lacking Ethernet support) the fields stay null just like on hardware.
  String json;
  json.reserve(96);
  json += F("{\"ip\":");
#ifdef USE_ETHERNET
  IPAddress eth_ip = EthernetLocalIP();
  if ((uint32_t)eth_ip) { json += '"'; json += eth_ip.toString(); json += '"'; } else { json += F("null"); }
  json += F(",\"ip6\":");
#ifdef USE_IPV6
  String eth_ip6 = EthernetGetIPv6Str();
  if (eth_ip6.length()) { json += '"'; json += eth_ip6; json += '"'; } else { json += F("null"); }
#else
  json += F("null");
#endif  // USE_IPV6
#else
  json += F("null,\"ip6\":null");
#endif  // USE_ETHERNET
  json += '}';
  return json;
}

static String ShellyWifiStatus(void) {
  String json;
  json.reserve(260);
  String ssid = WiFi.SSID();
  json += F("{\"sta_ip\":\""); json += WiFi.localIP().toString();
  json += F("\",\"status\":\""); json += WiFi.isConnected() ? F("got ip") : F("disconnected");
  json += F("\",\"ssid\":\""); json += EscapeJSONString(ssid.c_str());
  json += F("\",\"channel\":"); json += WiFi.channel();
  json += F(",\"rssi\":"); json += WiFi.RSSI();
  json += F(",\"bssid\":\""); String bssid = WiFi.BSSIDstr(); bssid.toLowerCase(); json += bssid;
  json += F("\",\"sta_ip6\":[]}");
  return json;
}

static String ShellyWifiConfig(void) {
  String json;
  json.reserve(480);
  json += F("{\"ap\":{\"ssid\":\""); json += ShellyEmulation.hostname;
  json += F("\",\"is_open\":false,\"enable\":false,\"range_extender\":{\"enable\":false}},\"sta\":{\"ssid\":\"");
  json += EscapeJSONString(SettingsText(SET_STASSID1));
  json += F("\",\"is_open\":false,\"enable\":true,\"ipv4mode\":\"dhcp\",\"ip\":null,\"netmask\":null,\"gw\":null,\"nameserver\":null},\"sta1\":{\"ssid\":\"");
  json += EscapeJSONString(SettingsText(SET_STASSID2));
  json += F("\",\"is_open\":false,\"enable\":true,\"ipv4mode\":\"dhcp\",\"ip\":null,\"netmask\":null,\"gw\":null,\"nameserver\":null},\"roam\":{\"rssi_thr\":-80,\"interval\":60}}");
  return json;
}

static String ShellySysStatus(void) {
  String time = GetDateAndTime(DT_LOCAL);
  if (time.length() >= 16) { time = time.substring(11, 16); }
  String json;
  json.reserve(420);
  json += F("{\"mac\":\""); json += ShellyEmulation.mac;
  json += F("\",\"restart_required\":false,\"time\":\""); json += time;
  json += F("\",\"unixtime\":"); json += UtcTime();
  json += F(",\"last_sync_ts\":"); json += UtcTime();
  json += F(",\"uptime\":"); json += millis() / 1000;
  json += F(",\"ram_size\":"); json += ESP.getHeapSize();
  json += F(",\"ram_free\":"); json += ESP.getFreeHeap();
  json += F(",\"ram_min_free\":"); json += ESP.getMinFreeHeap();
  json += F(",\"fs_size\":0,\"fs_free\":0,\"cfg_rev\":1,\"kvs_rev\":0,\"schedule_rev\":0,\"webhook_rev\":0,\"btrelay_rev\":0,\"bthc_rev\":0,\"available_updates\":{},\"reset_reason\":3,\"utc_offset\":0}");
  return json;
}

static String ShellySysConfig(void) {
  String json;
  json.reserve(520);
  json += F("{\"device\":{\"name\":\""); json += EscapeJSONString(SettingsText(SET_DEVICENAME));
  json += F("\",\"mac\":\""); json += ShellyEmulation.mac;
  json += F("\",\"fw_id\":\""); json += SHELLY_FW_ID;
  json += F("\",\"discoverable\":true,\"eco_mode\":false,\"profile\":\"switch\",\"addon_type\":null,\"sys_btn_toggle\":false,\"tls_check_cert_validity_time\":true,\"enhanced_security\":false},\"location\":{\"tz\":\"UTC\",\"lat\":0.0,\"lon\":0.0},\"debug\":{\"level\":2,\"file_level\":null,\"mqtt\":{\"enable\":false},\"websocket\":{\"enable\":false},\"file_log\":{\"enable\":false},\"udp\":{\"addr\":null}},\"ui_data\":{\"device_revision\":\"1-22\"},\"rpc_udp\":{\"dst_addr\":null,\"listen_port\":null},\"sntp\":{\"server\":null},\"cfg_rev\":1}");
  return json;
}

static String ShellyDeviceInfo(void) {
  String json;
  json.reserve(360);
  json += F("{\"name\":\""); json += EscapeJSONString(SettingsText(SET_DEVICENAME));
  json += F("\",\"id\":\""); json += ShellyEmulation.id;
  json += F("\",\"mac\":\""); json += ShellyEmulation.mac;
  json += F("\",\"slot\":0,\"model\":\""); json += SHELLY_MODEL;
  json += F("\",\"gen\":2,\"fw_id\":\""); json += SHELLY_FW_ID;
  json += F("\",\"ver\":\""); json += SHELLY_VERSION;
  json += F("\",\"app\":\""); json += SHELLY_APP;
  json += F("\",\"auth_en\":false,\"auth_domain\":null,\"profile\":\"switch\",\"provision\":\"complete\",\"enhanced_security\":false}");
  return json;
}

static String ShellyGetStatus(void) {
  String json;
  json.reserve(2400);
  json += F("{\"ble\":"); json += ShellyBleStatus();
  json += F(",\"bthome\":{},\"cloud\":{\"connected\":false},\"eth\":"); json += ShellyEthStatus();
  json += F(",\"input:0\":"); json += ShellyInputStatus(0);
  json += F(",\"input:1\":"); json += ShellyInputStatus(1);
  json += F(",\"knx\":{},\"mqtt\":{\"connected\":false}");
  json += F(",\"switch:0\":"); json += ShellySwitchStatus(0);
  json += F(",\"switch:1\":"); json += ShellySwitchStatus(1);
  json += F(",\"sys\":"); json += ShellySysStatus();
  json += F(",\"wifi\":"); json += ShellyWifiStatus();
  json += F(",\"ws\":{\"connected\":false}}");
  return json;
}

static String ShellyGetConfig(void) {
  String json;
  json.reserve(3100);
  json += F("{\"ble\":{\"rpc\":{\"enable\":true}},\"bthome\":{},\"cloud\":{\"enable\":false,\"server\":\"iot.shelly.cloud:6012/jrpc\"},\"eth\":{\"enable\":true,\"server_mode\":false,\"ipv4mode\":\"dhcp\",\"ip\":null,\"netmask\":null,\"gw\":null,\"nameserver\":null},\"input:0\":"); json += ShellyInputConfig(0);
  json += F(",\"input:1\":"); json += ShellyInputConfig(1);
  json += F(",\"knx\":{\"enable\":false,\"ia\":\"15.15.255\",\"routing\":{\"addr\":\"224.0.23.12:3671\"}},\"mqtt\":{\"enable\":false,\"server\":null,\"client_id\":null,\"user\":null,\"ssl_ca\":null,\"topic_prefix\":null,\"rpc_ntf\":true,\"status_ntf\":false,\"use_client_cert\":false,\"enable_rpc\":true,\"enable_control\":true}");
  json += F(",\"switch:0\":"); json += ShellySwitchConfig(0);
  json += F(",\"switch:1\":"); json += ShellySwitchConfig(1);
  json += F(",\"sys\":"); json += ShellySysConfig();
  json += F(",\"wifi\":"); json += ShellyWifiConfig();
  json += F(",\"ws\":{\"enable\":false,\"server\":null,\"ssl_ca\":\"ca.pem\"}}");
  return json;
}

/*********************************************************************************************\
 * RPC method dispatch
\*********************************************************************************************/

static int32_t ShellyIntegerParam(JsonParserObject params, const char *name, int32_t fallback) {
  JsonParserToken token = params[name];
  if (token && token.isNum()) { return token.getInt(fallback); }
  if (Webserver->hasArg(name)) { return Webserver->arg(name).toInt(); }
  return fallback;
}

static bool ShellyBooleanParam(JsonParserObject params, const char *name, bool fallback) {
  JsonParserToken token = params[name];
  if (token && (token.isBool() || token.isNum())) { return token.getBool(); }
  if (Webserver->hasArg(name)) {
    String value = Webserver->arg(name);
    if (value.equalsIgnoreCase("true") || value == "1") { return true; }
    if (value.equalsIgnoreCase("false") || value == "0") { return false; }
  }
  return fallback;
}

static String ShellyComponent(uint8_t index) {
  String json;
  switch (index) {
    case 0:
      json = F("{\"key\":\"ble\",\"status\":"); json += ShellyBleStatus();
      json += F(",\"config\":{\"rpc\":{\"enable\":true}}}"); break;
    case 1: json = F("{\"key\":\"bthome\",\"status\":{},\"config\":{}}"); break;
    case 2: json = F("{\"key\":\"cloud\",\"status\":{\"connected\":false},\"config\":{\"enable\":false,\"server\":\"iot.shelly.cloud:6012/jrpc\"}}"); break;
    case 3:
      json = F("{\"key\":\"eth\",\"status\":"); json += ShellyEthStatus();
      json += F(",\"config\":{\"enable\":true,\"server_mode\":false,\"ipv4mode\":\"dhcp\",\"ip\":null,\"netmask\":null,\"gw\":null,\"nameserver\":null}}"); break;
    case 4:
      json = F("{\"key\":\"input:0\",\"status\":"); json += ShellyInputStatus(0);
      json += F(",\"config\":"); json += ShellyInputConfig(0); json += '}'; break;
    case 5:
      json = F("{\"key\":\"input:1\",\"status\":"); json += ShellyInputStatus(1);
      json += F(",\"config\":"); json += ShellyInputConfig(1); json += '}'; break;
    case 6: json = F("{\"key\":\"knx\",\"status\":{},\"config\":{\"enable\":false,\"ia\":\"15.15.255\",\"routing\":{\"addr\":\"224.0.23.12:3671\"}}}"); break;
    case 7: json = F("{\"key\":\"mqtt\",\"status\":{\"connected\":false},\"config\":{\"enable\":false,\"server\":null,\"client_id\":null,\"user\":null,\"ssl_ca\":null,\"topic_prefix\":null,\"rpc_ntf\":true,\"status_ntf\":false,\"use_client_cert\":false,\"enable_rpc\":true,\"enable_control\":true}}"); break;
    case 8:
      json = F("{\"key\":\"switch:0\",\"status\":"); json += ShellySwitchStatus(0);
      json += F(",\"config\":"); json += ShellySwitchConfig(0);
      json += F(",\"attrs\":{\"inputs\":[0]}}"); break;
    case 9:
      json = F("{\"key\":\"switch:1\",\"status\":"); json += ShellySwitchStatus(1);
      json += F(",\"config\":"); json += ShellySwitchConfig(1);
      json += F(",\"attrs\":{\"inputs\":[1]}}"); break;
    case 10:
      json = F("{\"key\":\"sys\",\"status\":"); json += ShellySysStatus();
      json += F(",\"config\":"); json += ShellySysConfig(); json += '}'; break;
    case 11:
      json = F("{\"key\":\"wifi\",\"status\":"); json += ShellyWifiStatus();
      json += F(",\"config\":"); json += ShellyWifiConfig(); json += '}'; break;
    case 12: json = F("{\"key\":\"ws\",\"status\":{\"connected\":false},\"config\":{\"enable\":false,\"server\":null,\"ssl_ca\":\"ca.pem\"}}"); break;
  }
  return json;
}

static String ShellyGetComponents(JsonParserObject params) {
  int32_t requested_offset = ShellyIntegerParam(params, "offset", 0);
  uint8_t offset = (requested_offset < 0) ? 0 : (requested_offset > 13 ? 13 : requested_offset);
  bool dynamic_only = ShellyBooleanParam(params, "dynamic_only", false);
  String json;
  json.reserve(5000);
  json += F("{\"components\":[");
  if (!dynamic_only) {
    uint8_t end = min((uint8_t)13, (uint8_t)(offset + 10));
    for (uint8_t index = offset; index < end; index++) {
      if (index != offset) { json += ','; }
      json += ShellyComponent(index);
    }
  }
  json += F("],\"cfg_rev\":1,\"offset\":"); json += offset;
  json += F(",\"total\":"); json += dynamic_only ? 0 : 13;
  json += '}';
  return json;
}

static const char kShellyMethods[] PROGMEM =
  "[\"Shelly.GetDeviceInfo\",\"Shelly.GetStatus\",\"Shelly.GetConfig\",\"Shelly.GetComponents\",\"Shelly.ListMethods\",\"Shelly.ListProfiles\","
  "\"RPC.Ping\","
  "\"Wifi.GetStatus\",\"Wifi.GetConfig\",\"Switch.GetStatus\",\"Switch.GetConfig\",\"Switch.Set\",\"Switch.Toggle\","
  "\"Input.GetStatus\",\"Input.GetConfig\",\"BLE.GetStatus\",\"BLE.GetConfig\",\"Sys.GetStatus\",\"Sys.GetConfig\"]";

static bool ShellyGetId(JsonParserObject params, uint8_t &id) {
  int32_t parsed = -1;
  JsonParserToken token = params[PSTR("id")];
  if (token) { parsed = token.getInt(-1); }
  if (parsed < 0 && Webserver->hasArg("id")) {
    String value = Webserver->arg("id");
    if (value.length() != 1 || value[0] < '0' || value[0] > '9') { return false; }
    parsed = value[0] - '0';
  }
  if (parsed < 0 || parsed > 1) { return false; }
  id = parsed;
  return true;
}

static String ShellyRpcError(int code, const char *message) {
  return String(F("{\"code\":")) + code + F(",\"message\":\"") + message + F("\"}");
}

static bool ShellyDispatchRpc(const char *method, JsonParserObject params, String &result, int &error_code, const char *&error_message) {
  error_code = 0;
  error_message = nullptr;
  if (!strcasecmp(method, "Shelly.GetDeviceInfo")) { result = ShellyDeviceInfo(); }
  else if (!strcasecmp(method, "Shelly.GetStatus")) { result = ShellyGetStatus(); }
  else if (!strcasecmp(method, "Shelly.GetConfig")) { result = ShellyGetConfig(); }
  else if (!strcasecmp(method, "Shelly.GetComponents")) { result = ShellyGetComponents(params); }
  else if (!strcasecmp(method, "Shelly.ListMethods")) { result = String(F("{\"methods\":")) + FPSTR(kShellyMethods) + '}'; }
  else if (!strcasecmp(method, "Shelly.ListProfiles")) { result = F("{\"profiles\":{\"cover\":{\"components\":[{\"type\":\"input\",\"count\":2},{\"type\":\"cover\",\"count\":1}]},\"switch\":{\"components\":[{\"type\":\"input\",\"count\":2},{\"type\":\"switch\",\"count\":2}]}}}"); }
  else if (!strcasecmp(method, "RPC.Ping")) { result = F("{}"); }
  else if (!strcasecmp(method, "Wifi.GetStatus")) { result = ShellyWifiStatus(); }
  else if (!strcasecmp(method, "Wifi.GetConfig")) { result = ShellyWifiConfig(); }
  else if (!strcasecmp(method, "BLE.GetStatus")) { result = ShellyBleStatus(); }
  else if (!strcasecmp(method, "BLE.GetConfig")) { result = F("{\"rpc\":{\"enable\":true}}"); }
  else if (!strcasecmp(method, "Sys.GetStatus")) { result = ShellySysStatus(); }
  else if (!strcasecmp(method, "Sys.GetConfig")) { result = ShellySysConfig(); }
  else if (!strncasecmp(method, "Switch.", 7) || !strncasecmp(method, "Input.", 6)) {
    uint8_t id;
    if (!ShellyGetId(params, id)) {
      error_code = -104; error_message = "Invalid argument 'id'"; return false;
    }
    if (!strcasecmp(method, "Switch.GetStatus")) { result = ShellySwitchStatus(id); }
    else if (!strcasecmp(method, "Switch.GetConfig")) { result = ShellySwitchConfig(id); }
    else if (!strcasecmp(method, "Input.GetStatus")) { result = ShellyInputStatus(id); }
    else if (!strcasecmp(method, "Input.GetConfig")) { result = ShellyInputConfig(id); }
    else if (!strcasecmp(method, "Switch.Toggle")) {
      bool was_on = ShellyPower(id);
      ShellySetPower(id, !was_on);
      result = String(F("{\"was_on\":")) + ShellyJsonBool(was_on) + '}';
    } else if (!strcasecmp(method, "Switch.Set")) {
      bool have_on = false;
      bool on = false;
      JsonParserToken on_token = params[PSTR("on")];
      if (on_token && (on_token.isBool() || on_token.isNum())) { have_on = true; on = on_token.getBool(); }
      if (!have_on && Webserver->hasArg("on")) {
        String value = Webserver->arg("on");
        have_on = value.equalsIgnoreCase("true") || value.equalsIgnoreCase("false") || value == "1" || value == "0";
        on = value.equalsIgnoreCase("true") || value == "1";
      }
      if (!have_on) { error_code = -104; error_message = "Invalid argument 'on'"; return false; }
      bool was_on = ShellyPower(id);
      ShellySetPower(id, on);
      result = String(F("{\"was_on\":")) + ShellyJsonBool(was_on) + '}';
    } else {
      error_code = -103; error_message = "Method not found"; return false;
    }
  } else {
    error_code = -103; error_message = "Method not found"; return false;
  }
  return true;
}

/*********************************************************************************************\
 * Web server handlers
\*********************************************************************************************/

static void ShellySendJson(int status, const String &body) {
  Webserver->sendHeader(F("Server"), F("ShellyHTTP/1.0.0"));
  // ESP32 WebServer::send() appends its own Connection: close header.
  Webserver->send(status, F("application/json"), body);
  delay(1);
  Webserver->client().stop();
}

static void HandleShellyInfo(void) {
  Webserver->sendHeader(F("Pragma"), F("no-cache"));
  ShellySendJson(200, ShellyDeviceInfo());
}

static void HandleShellyRpc(void) {
  String uri = Webserver->uri();
  String body = Webserver->hasArg("plain") ? Webserver->arg("plain") : String();
  String method;
  int32_t request_id = 0;
  const char *request_source = "user_1";
  bool framed = uri.equalsIgnoreCase("/rpc");
  JsonParserObject params;

  if (framed) {
    if (!body.length()) { ShellySendJson(400, ShellyRpcError(-32700, "Invalid JSON")); return; }
    JsonParser parser((char*)body.c_str());
    JsonParserObject root = parser.getRootObject();
    if (!root) { ShellySendJson(400, ShellyRpcError(-32700, "Invalid JSON")); return; }
    method = root.getStr(PSTR("method"), "");
    request_id = root.getInt(PSTR("id"), 0);
    request_source = root.getStr(PSTR("src"), "user_1");
    params = root[PSTR("params")].getObject();
    String result;
    int error_code;
    const char *error_message;
    bool ok = method.length() && ShellyDispatchRpc(method.c_str(), params, result, error_code, error_message);
    String response = String(F("{\"id\":")) + request_id + F(",\"src\":\"") + ShellyEmulation.id + F("\",\"dst\":\"") + EscapeJSONString(request_source) + F("\",");
    if (ok) {
      response += F("\"result\":");
      response += result;
    } else {
      response += F("\"error\":");
      response += ShellyRpcError(error_code ? error_code : -103, error_message ? error_message : "Method not found");
    }
    response += '}';
    ShellySendJson(200, response);
    return;
  }

  method = uri.substring(5);
  if (method.startsWith("/")) { method.remove(0, 1); }
  if (!method.length()) { ShellySendJson(404, ShellyRpcError(-103, "Method not found")); return; }
  if (body.length()) {
    JsonParser parser((char*)body.c_str());
    params = parser.getRootObject();
    if (!params) { ShellySendJson(400, ShellyRpcError(-32700, "Invalid JSON")); return; }
    String result;
    int error_code;
    const char *error_message;
    bool ok = ShellyDispatchRpc(method.c_str(), params, result, error_code, error_message);
    ShellySendJson(200, ok ? result : ShellyRpcError(error_code, error_message));
    return;
  }
  String result;
  int error_code;
  const char *error_message;
  bool ok = ShellyDispatchRpc(method.c_str(), params, result, error_code, error_message);
  ShellySendJson(200, ok ? result : ShellyRpcError(error_code, error_message));
}

static void ShellyAddWebHandlers(void) {
  if (ShellyEmulation.web_handlers_added || !Webserver) { return; }
  WebServer_on(PSTR("/shelly"), HandleShellyInfo, HTTP_ANY);
  WebServer_on(PSTR("/rpc"), HandleShellyRpc, HTTP_ANY);
  Webserver->on(UriGlob("/rpc/*"), HTTP_ANY, HandleShellyRpc);
  ShellyEmulation.web_handlers_added = true;
}

/*********************************************************************************************\
 * Shelly service discovery
\*********************************************************************************************/

#ifdef USE_DISCOVERY
static void ShellyMdnsSetTtl(void) {
  // Espressif's mDNS component sets the multicast TTL to 255 but leaves QU and
  // legacy-unicast replies at lwIP's normal unicast TTL. RFC 6762 requires 255
  // for every mDNS packet and Shelly controllers discard replies carrying less.
  for (struct udp_pcb *pcb = udp_pcbs; pcb != nullptr; pcb = pcb->next) {
    if (5353 == pcb->local_port) { pcb->ttl = 255; }
  }
}

// Two interfaces (WiFi and Ethernet), each with one IPv4 plus up to three IPv6
// addresses (link-local, SLAAC global, DHCPv6 global) covers every observed case.
// A file-scope array keeps mdns types out of the function signatures, which the
// Arduino prototype generator would otherwise leak into non-ESP32 builds.
#define SHELLY_MDNS_MAX_ADDRESSES 8
static mdns_ip_addr_t ShellyMdnsAddresses[SHELLY_MDNS_MAX_ADDRESSES];

static uint32_t ShellyMdnsCollectAddresses(void) {
  // Chain every routable address of the WiFi and Ethernet interfaces under the
  // single Shelly hostname, so a controller can reach the device over IPv4 or
  // IPv6 on whichever interface it shares with the device.
  mdns_ip_addr_t *nodes = ShellyMdnsAddresses;
  uint32_t count = 0;
  for (netif *intf = netif_list; intf != nullptr && count < SHELLY_MDNS_MAX_ADDRESSES; intf = intf->next) {
    bool wifi = ('s' == intf->name[0] && 't' == intf->name[1]);
    bool ethernet = ('e' == intf->name[0] && 'n' == intf->name[1]);
    if (!wifi && !ethernet) { continue; }
    const ip_addr_t *ipv4 = &intf->ip_addr;
    if (IP_IS_V4(ipv4) && !ip_addr_isany(ipv4)) {
      memset(&nodes[count], 0, sizeof(nodes[count]));
      nodes[count].addr.type = ESP_IPADDR_TYPE_V4;
      nodes[count].addr.u_addr.ip4.addr = ip_2_ip4(ipv4)->addr;
      count++;
    }
#ifdef USE_IPV6
    for (uint32_t slot = 0; slot < LWIP_IPV6_NUM_ADDRESSES && count < SHELLY_MDNS_MAX_ADDRESSES; slot++) {
      const ip_addr_t *ipv6 = &intf->ip6_addr[slot];
      if (!IP_IS_V6_VAL(*ipv6) || ip_addr_isloopback(ipv6) || ip_addr_isany(ipv6)) { continue; }
      if (!ip6_addr_isvalid(netif_ip6_addr_state(intf, slot))) { continue; }
      memset(&nodes[count], 0, sizeof(nodes[count]));
      nodes[count].addr.type = ESP_IPADDR_TYPE_V6;
      memcpy(nodes[count].addr.u_addr.ip6.addr, ip_2_ip6(ipv6)->addr, sizeof(nodes[count].addr.u_addr.ip6.addr));
      count++;
    }
#endif  // USE_IPV6
  }
  for (uint32_t index = 0; index < count; index++) {
    nodes[index].next = (index + 1 < count) ? &nodes[index + 1] : nullptr;
  }
  return count;
}

static uint32_t ShellyMdnsSignature(uint32_t count) {
  uint32_t hash = 2166136261u;                   // FNV-1a over the address set
  for (uint32_t index = 0; index < count; index++) {
    const uint8_t *bytes = (const uint8_t *)&ShellyMdnsAddresses[index].addr;
    for (uint32_t offset = 0; offset < sizeof(ShellyMdnsAddresses[index].addr); offset++) {
      hash = (hash ^ bytes[offset]) * 16777619u;
    }
  }
  return hash ? hash : 1;                        // 0 is reserved for "not registered"
}

static void ShellyMdnsStop(void) {
  if (!ShellyEmulation.mdns_active) { return; }
  mdns_service_remove_for_host(ShellyEmulation.id, "_shelly", "_tcp", ShellyEmulation.hostname);
  mdns_service_remove_for_host(ShellyEmulation.id, "_http", "_tcp", ShellyEmulation.hostname);
  mdns_delegate_hostname_remove(ShellyEmulation.hostname);
  ShellyEmulation.mdns_active = false;
  ShellyEmulation.mdns_signature = 0;
}

static void ShellyMdnsStart(void) {
  // FUNC_NETWORK_UP repeats about once a second while any network is up, so this
  // also refreshes the advertisement when a DHCP lease moves or a global IPv6
  // address arrives after the initial registration.
  uint32_t count = ShellyMdnsCollectAddresses();
  if (!count) { return; }
  const uint32_t signature = ShellyMdnsSignature(count);
  if (ShellyEmulation.mdns_active && ShellyEmulation.mdns_signature == signature) { return; }
  // A selected emulation answers discovery just like WeMo and Hue answer SSDP.  Join
  // the running responder when SetOption55 has started it, otherwise start it here and
  // claim the http service slot so MdnsAddServiceHttp() does not later advertise the
  // Tasmota web server the user opted out of.
  if (!Mdns.begun) {
    if (!MDNS.begin(TasmotaGlobal.hostname)) {
      AddLog(LOG_LEVEL_ERROR, PSTR("SHL: Unable to start mDNS responder"));
      return;
    }
    Mdns.begun = 2;
    AddLog(LOG_LEVEL_DEBUG, PSTR("SHL: mDNS responder started for Shelly discovery"));
  }
  // Updating the address list in place keeps the registered services attached,
  // unlike remove-and-add, which would drop them with every address change.
  esp_err_t address_result = mdns_hostname_exists(ShellyEmulation.hostname)
    ? mdns_delegate_hostname_set_address(ShellyEmulation.hostname, ShellyMdnsAddresses)
    : mdns_delegate_hostname_add(ShellyEmulation.hostname, ShellyMdnsAddresses);
  if (ESP_OK != address_result) {
    AddLog(LOG_LEVEL_ERROR, PSTR("SHL: Unable to add mDNS hostname"));
    return;
  }
  if (!ShellyEmulation.mdns_active) {
    mdns_txt_item_t shelly_txt[] = {{"gen", "2"}, {"app", SHELLY_APP}, {"ver", SHELLY_VERSION}};
    mdns_txt_item_t http_txt[] = {{"gen", "2"}};
    esp_err_t shelly_result = mdns_service_add_for_host(ShellyEmulation.id, "_shelly", "_tcp", ShellyEmulation.hostname, 80, shelly_txt, 3);
    esp_err_t http_result = mdns_service_add_for_host(ShellyEmulation.id, "_http", "_tcp", ShellyEmulation.hostname, 80, http_txt, 1);
    if (ESP_OK != shelly_result || ESP_OK != http_result) {
      // ShellyMdnsStop normally avoids unnecessary remove calls. Mark this
      // partial registration active so it also removes the delegated hostname
      // and whichever service succeeded before the other one failed.
      ShellyEmulation.mdns_active = true;
      ShellyMdnsStop();
      AddLog(LOG_LEVEL_ERROR, PSTR("SHL: mDNS registration failed (%d, %d)"), shelly_result, http_result);
      return;
    }
    ShellyEmulation.mdns_active = true;
  }
  // The responder can create new port 5353 sockets when an interface or address
  // family becomes active, so the TTL fix must be re-applied after every change.
  ShellyMdnsSetTtl();
  ShellyEmulation.mdns_signature = signature;
  AddLog(LOG_LEVEL_INFO, PSTR("SHL: mDNS %s.local advertising %u address%s"),
         ShellyEmulation.hostname, count, (1 == count) ? PSTR("") : PSTR("es"));
}
#else
static void ShellyMdnsStop(void) {}
static void ShellyMdnsStart(void) {}
#endif  // USE_DISCOVERY

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv89(uint32_t function) {
  bool result = false;

  if (FUNC_RESET_SETTINGS == function) {
    ShellySettingsLoad(true);                    // Also clear when another emulation is selected
  }
  else if (EMUL_SHELLY == Settings->flag2.emulation) {
    switch (function) {
      case FUNC_PRE_INIT:
        ShellySettingsLoad(false);
        ShellyBuildIdentity();
        break;
      case FUNC_WEB_ADD_HANDLER:
        ShellyAddWebHandlers();
        break;
      case FUNC_NETWORK_UP:
        ShellyMdnsStart();
        break;
      case FUNC_NETWORK_DOWN:
        ShellyMdnsStop();
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // ESP32 && USE_WEBSERVER && USE_EMULATION && USE_EMULATION_SHELLY
