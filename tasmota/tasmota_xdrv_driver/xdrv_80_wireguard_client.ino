/*
  xdrv_80_wireguard_clientc.ino - creates a VPN connection to a Wireguard site

  Copyright (C) 2024  Stephan Hadinger

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

#ifdef USE_WIREGUARD

#define XDRV_80           80

#include "esp_wireguard.h"
#include "IniFile.h"
#include "LList.h"

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

static const char WIREGUARD_CONF_FNAME[] = "/wireguard.conf";
static const char WIREGUARD_NETMASK[] = "0.0.0.0";

#ifndef WG_ENDPOINT_RECHECK_SEC
#define WG_ENDPOINT_RECHECK_SEC   60    // while peer is down, re-resolve Endpoint every N seconds
#endif
static_assert(WG_ENDPOINT_RECHECK_SEC > 0, "WG_ENDPOINT_RECHECK_SEC must be greater than zero");

const char kWireGuardCommands[] PROGMEM = "WG|"   // Prefix
  "Connect|Stop";

void (* const WireGuardCommand[])(void) PROGMEM = {
  &CmndWGConnect, &CmndWGStop };

typedef struct {
  ip_addr_t   addr;
  ip_addr_t   mask;
} allowed_ips_t;

struct Wireguard_t {
  bool configured = false;
  bool auto_connect = false;
  bool started = false;
  bool peer_up = false;
  bool endpoint_is_ip = false;  // Endpoint is a literal IP, no need to re-resolve
  bool reconnect_pending = false;  // Retry a failed DDNS-triggered reconnect
  bool network_available = false;  // Track edges; FUNC_NETWORK_UP repeats every second
  int8_t peer_status = -1;    // known state: -1 unknown, 0 DOWN, 1 UP
  uint32_t recheck_counter = 0; // seconds since last Endpoint check / connect retry
  uint32_t connected_since_utc = 0;
  String endpoint;
  LList<allowed_ips_t> allowed_ips;
  // used by lib
  wireguard_config_t config = {};
  wireguard_ctx_t ctx = {0};
} Wireguard;

/*********************************************************************************************\
 * WireGuard internal lower level functions
\*********************************************************************************************/

// WireguardLoadConfig
//
// Load configuration from INI file
// returns 'true' if succesful
bool WireguardLoadConfig(const char *filename) {
  if (filename == NULL) { return false; }
  if (!ffsp) {
    AddLog(LOG_LEVEL_INFO, PSTR("WG : WireGuard initialization failed, no file system"));
    return false;
  }
  File file = ffsp->open(filename, "r");
  if (!file) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("WG : file '%s' not present, skipping"), filename);
    return false;
  }

  IniFile ini(file);
  bool valconf = true;
  wireguard_config_t& config = Wireguard.config;
  valconf = true;
  valconf = valconf && ini.getValueBase64("Interface", "PrivateKey", config.private_key2, sizeof(config.private_key2));
  valconf = valconf && ini.getCIDR("Interface", "Address", &config.address2, &config.subnet);
  valconf = valconf && ini.getValueBase64("Peer", "PublicKey", config.public_key2, sizeof(config.public_key2));
  valconf = valconf && ini.getValueBase64("Peer", "PresharedKey", config.preshared_key2, sizeof(config.preshared_key2));
  valconf = valconf && ini.getDomainPort("Peer", "Endpoint", Wireguard.endpoint, Wireguard.config.port, 51820 /*default port*/);
  // read optional NetMask
  ipaddr_aton(WIREGUARD_NETMASK, &config.netmask2);
  ini.getIPAddress("Tasmota", "Netmask", &Wireguard.config.netmask2);
  // read optional PersistentKeepalive
  ini.getValueUInt16("Peer", "PersistentKeepalive", config.persistent_keepalive);
  // read optional AutoConnect
  ini.getValueBool("Tasmota", "AutoConnect", Wireguard.auto_connect);
  // add allowedIPs
  String allowed_ips_str;
  ini.getValueString("Peer", "AllowedIPs", allowed_ips_str);

  if (valconf) {
    // read optional AllowedIPs
    allowed_ips_t allowip;

    while (allowed_ips_str.length() > 0) {
      int32_t comma = allowed_ips_str.indexOf(",");
      String cidr = (comma > 0) ? allowed_ips_str.substring(0, comma) : allowed_ips_str;
      cidr.trim();
      // AddLog(LOG_LEVEL_DEBUG, ">>>: allowed_ips_str '%s' comma %i cidr '%s'", allowed_ips_str.c_str(), comma, cidr.c_str());
      if (IniFile::parseCIDR(cidr, &allowip.addr, &allowip.mask)) {
        Wireguard.allowed_ips.addHead(allowip);
      } else {
        AddLog(LOG_LEVEL_INFO, PSTR("WG : Failed to parse allowed_ips '%s', skipping"), cidr.c_str());
      }
      if (comma > 0) {
        allowed_ips_str = allowed_ips_str.substring(comma + 1);
      } else {
        break;
      }
    }
  }

  file.close();
  if (!valconf) {
    AddLog(LOG_LEVEL_INFO, PSTR("WG : WireGuard initialization failed, invalid configuration"));
    return false;
  }
  // now parse values
  Wireguard.config.endpoint = Wireguard.endpoint.c_str();
  IPAddress endpoint_literal;
  Wireguard.endpoint_is_ip = endpoint_literal.fromString(Wireguard.endpoint);

  AddLog(LOG_LEVEL_DEBUG, PSTR("WG : reading '%s' address:%s/%s netmask:%s endpoint:%s:%i allowed_ips_str:'%s' PersistentKeepalive:%i"),
                          filename,
                          IPAddress(&config.address2).toString().c_str(), IPAddress(&config.subnet).toString().c_str(),
                          IPAddress(&config.netmask2).toString().c_str(),
                          Wireguard.config.endpoint, Wireguard.config.port, allowed_ips_str.c_str(),
                          config.persistent_keepalive);
  return true;
}

// WireguardConnect
//
// Connect to peer, optionally using an Endpoint IP already resolved by the caller
bool WireguardConnect(const ip_addr_t *endpoint_ip) {
  if (!Wireguard.configured || Wireguard.started) {
    return false;
  }

  esp_err_t err = endpoint_ip ? esp_wireguard_connect_with_ip(&Wireguard.ctx, endpoint_ip)
                              : esp_wireguard_connect(&Wireguard.ctx);
  if (err == ESP_OK) {
    Wireguard.started = true;
    Wireguard.reconnect_pending = false;
    Wireguard.peer_status = -1;
    Wireguard.connected_since_utc = 0;
    Wireguard.recheck_counter = 0;
    if (!Wireguard.allowed_ips.isEmpty()) {
      for (const allowed_ips_t & allowedip : Wireguard.allowed_ips) {
        err = esp_wireguard_add_allowed_ip(&Wireguard.ctx, allowedip.addr, allowedip.mask);
        if (err != ESP_OK) {
          AddLog(LOG_LEVEL_INFO, PSTR("WG : Failed to add allowed_ips, no space left"));
          break;
        }
        AddLog(LOG_LEVEL_DEBUG, PSTR("WG : Added allowed_ips %s/%s"), IPAddress(&allowedip.addr).toString().c_str(),
                                                                       IPAddress(&allowedip.mask).toString().c_str());
      }
    } else {
      // allowed_ips is empty, so we add 0.0.0.0/0.0.0.0
      ip_addr_t ip_zero = IPADDR4_INIT_BYTES(0, 0, 0, 0);
      err = esp_wireguard_add_allowed_ip(&Wireguard.ctx, ip_zero, ip_zero);
      AddLog(LOG_LEVEL_DEBUG, PSTR("WG : Added default allowed_ips 0.0.0.0/0.0.0.0"));
    }
    return true;
  }

  // A failed connect can leave a netif or peer partially initialized.
  if (Wireguard.ctx.netif) {
    esp_wireguard_disconnect(&Wireguard.ctx);
  }
  Wireguard.started = false;
  Wireguard.peer_status = -1;
  Wireguard.connected_since_utc = 0;
  return false;
}

bool WireguardConnect(void) {
  return WireguardConnect(nullptr);
}

// WireguardStop
//
// Stop the current Wireguard connection
// Do nothing if there is no connection
void WireguardStop(void) {
  if (!Wireguard.configured || !Wireguard.ctx.netif) {
    return;
  }
  // Stop a complete or partially initialized WireGuard connection.
  esp_wireguard_disconnect(&Wireguard.ctx);
  Wireguard.started = false;
  Wireguard.peer_status = -1;
  Wireguard.connected_since_utc = 0;
  Wireguard.recheck_counter = 0;
  AddLog(LOG_LEVEL_INFO, PSTR("WG : Wireguard peer DOWN"));
}

/*********************************************************************************************\
 * WireGuard commands
\*********************************************************************************************/

// Initialize Wireguard client
void WireguardInit(void) {
  if (WireguardLoadConfig(WIREGUARD_CONF_FNAME)) {
    esp_wireguard_init(&Wireguard.config, &Wireguard.ctx);
    Wireguard.configured = true;
  } else {
    Wireguard.configured = false;
  }
}

// WireGuard Connect
void CmndWGConnect(void) {
  if (!Wireguard.configured) {
    ResponseCmndChar(PSTR("Not configured"));
    return;
  }
  if (Wireguard.started) {
    ResponseCmndChar(PSTR("Already started"));
    return;
  }

  if (WireguardConnect()) {
    ResponseCmndChar(PSTR("Success"));
  } else {
    ResponseCmndChar(PSTR("Failed"));
  } 
}

// WireGuard Stop
void CmndWGStop(void) {
  if (!Wireguard.configured) {
    ResponseCmndChar(PSTR("Not configured"));
    return;
  }

  // Always cancel automatic retries, including after a failed AutoConnect
  // or a failed DDNS-triggered reconnect.
  Wireguard.auto_connect = false;
  Wireguard.reconnect_pending = false;
  Wireguard.recheck_counter = 0;

  if (!Wireguard.started && !Wireguard.ctx.netif) {
    ResponseCmndChar(PSTR("Not started"));
    return;
  }

  WireguardStop();
  ResponseCmndChar(PSTR("Success"));
}

// WireguardCheckEndpoint
//
// Re-resolve Endpoint name; if its IP changed (dynamic DNS), restart the tunnel.
// WireGuard resolves the name only once at connect time and never tracks changes.
void WireguardCheckEndpoint(void) {
  if (Wireguard.endpoint_is_ip) { return; }
  IPAddress new_ip;
  if (!WifiHostByName(Wireguard.endpoint.c_str(), new_ip)) { return; }   // DNS failed, retry later
  IPAddress old_ip(&Wireguard.config.endpoint_ip);
  if (new_ip == old_ip) { return; }                                       // unchanged, let WG keep retrying
  AddLog(LOG_LEVEL_INFO, PSTR("WG : Endpoint '%s' changed %s -> %s, reconnecting"),
         Wireguard.endpoint.c_str(), old_ip.toString().c_str(), new_ip.toString().c_str());

  ip_addr_t new_endpoint_ip;
#ifdef ESP32
  new_ip.to_ip_addr_t(&new_endpoint_ip);
#else
  new_endpoint_ip = new_ip;
#endif

  Wireguard.reconnect_pending = true;
  WireguardStop();
  if (!WireguardConnect(&new_endpoint_ip)) {
    AddLog(LOG_LEVEL_INFO, PSTR("WG : Failed to reconnect to Endpoint '%s' (%s)"),
           Wireguard.endpoint.c_str(), new_ip.toString().c_str());
  }
}

// Loop every second
void WireguardLoop(void) {
  if (!Wireguard.configured) { return; }
  if (Wireguard.started) {
    esp_err_t err = esp_wireguard_peer_is_up(&Wireguard.ctx);
    if (err == ESP_OK) {
      Wireguard.recheck_counter = 0;
      if (Wireguard.peer_status != 1) {
        Wireguard.peer_status = 1;
        if (Rtc.utc_time >= START_VALID_TIME) {           // record the connection time only if we have a valid time
          Wireguard.connected_since_utc = Rtc.utc_time;
        }
        AddLog(LOG_LEVEL_INFO, PSTR("WG : Wireguard peer UP"));
      }
      // second chance, if connection happened with no time, and now time is valid
      if (Wireguard.connected_since_utc == 0 && Rtc.utc_time >= START_VALID_TIME) {
        Wireguard.connected_since_utc = Rtc.utc_time;
      }
    } else {
      if (Wireguard.peer_status != 0) {
        Wireguard.peer_status = 0;
        AddLog(LOG_LEVEL_INFO, PSTR("WG : Wireguard peer DOWN"));
      }
      if (++Wireguard.recheck_counter >= WG_ENDPOINT_RECHECK_SEC) {
        Wireguard.recheck_counter = 0;
        WireguardCheckEndpoint();
      }
    }
  } else if ((Wireguard.auto_connect || Wireguard.reconnect_pending) &&
             !TasmotaGlobal.global_state.network_down) {
    // Retry failed AutoConnect and DDNS-triggered reconnects periodically.
    if (++Wireguard.recheck_counter >= WG_ENDPOINT_RECHECK_SEC) {
      Wireguard.recheck_counter = 0;
      WireguardConnect();
    }
  }
}

void WireguardNetworkUpDown(bool up) {
  // FUNC_NETWORK_UP/DOWN repeats every second. Only act on transitions so a
  // failed connect is retried by WireguardLoop at WG_ENDPOINT_RECHECK_SEC.
  if (Wireguard.network_available == up) { return; }
  Wireguard.network_available = up;
  Wireguard.recheck_counter = 0;

  if (up) {
    if (!Wireguard.started && (Wireguard.auto_connect || Wireguard.reconnect_pending)) {
      WireguardConnect();
    }
  } else {
    // Network is down
    if (Wireguard.started) {
      WireguardStop();
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv80(uint32_t function) {
  bool result = false;
  if (TasmotaGlobal.no_autoexec) { return result; }  // do nothing in case of bootloop

  switch (function) {
    case FUNC_INIT:
      WireguardInit();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kWireGuardCommands, WireGuardCommand);
      break;
    case FUNC_EVERY_SECOND:
      WireguardLoop();
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_STATUS_RIGHT:
      if (Wireguard.started && Wireguard.peer_status == 1) {
        // number of seconds since connection, or -1 if no valid time
        int32_t seconds = Wireguard.connected_since_utc ? Rtc.utc_time - Wireguard.connected_since_utc : -1;
        WSContentStatusSticker(PSTR("VPN"));
      }
      break;
#endif // USE_WEBSERVER
      case FUNC_NETWORK_UP:
        WireguardNetworkUpDown(true);
        break;
      case FUNC_NETWORK_DOWN:
        WireguardNetworkUpDown(false);
        break;
  }
  return result;
}

#endif  // USE_WIREGUARD
