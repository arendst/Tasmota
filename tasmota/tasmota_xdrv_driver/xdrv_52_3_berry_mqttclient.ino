/*
  xdrv_52_3_berry_mqttclient.ino - Berry independent MQTT client

  Copyright (C) 2026 Allen Schober

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

// An independent MQTT client for use in Berry code and applications (ESP32 only).
// Allows Berry code to connect to a separate MQTT broker with its own host,
// port, credentials, TLS, LWT, and auto-reconnect support.
//
// Berry usage:
//   var client = mqttclient()
//   client.set_on_message(def (topic, idx, payload_s, payload_b) print(topic, payload_s) end)
//   client.set_on_connect(def () client.subscribe('my/topic') end)
//   client.connect('broker.local', 1883, 'tasmota-client')
//   tasmota.add_fast_loop(def () client.loop() end)

#ifdef USE_BERRY
#ifdef USE_BERRY_MQTTCLIENT
#ifdef ESP32

#include "berry.h"
#include <PubSubClient.h>
#include <WiFiClient.h>
#ifdef USE_TLS
#include "WiFiClientSecureLightBearSSL.h"
#endif

// Forward declaration for trust anchors
extern const br_x509_trust_anchor Tasmota_TA[];

// Log prefix — use "MQT:" domain like xdrv_02_9_mqtt.ino
#define D_LOG_BMQTT       D_LOG_MQTT

/*********************************************************************************************\
 * BerryMqttClient - wrapper struct holding all state for one independent MQTT connection
 *
\*********************************************************************************************/

struct BerryMqttClient {
  WiFiClient                        *wifi_tcp;       // plain TCP transport
#ifdef USE_TLS
  BearSSL::WiFiClientSecure_light   *wifi_tls;       // TLS transport
#endif
  Client                            *active_client;  // whichever transport is in use
  PubSubClient                      *mqtt;           // the MQTT client
  bool                               use_tls;

  // Stored connection parameters for reconnect (heap-allocated copies)
  char      *cfg_host;
  uint16_t   cfg_port;
  char      *cfg_client_id;
  char      *cfg_user;
  char      *cfg_pass;
  char      *cfg_lwt_topic;
  char      *cfg_lwt_msg;
  uint8_t    cfg_lwt_qos;
  bool       cfg_lwt_retain;

  // Reconnect state
  bool       auto_reconnect;           // enable/disable auto-reconnect
  bool       was_connected;            // track if we were previously connected
  uint8_t    retry_counter_multiplier; // increases each failure (exponential backoff)
  uint32_t   retry_counter;            // millis() timestamp of next retry attempt

  BerryMqttClient() : wifi_tcp(nullptr),
#ifdef USE_TLS
    wifi_tls(nullptr),
#endif
    active_client(nullptr), mqtt(nullptr), use_tls(false),
    cfg_host(nullptr), cfg_port(0), cfg_client_id(nullptr),
    cfg_user(nullptr), cfg_pass(nullptr),
    cfg_lwt_topic(nullptr), cfg_lwt_msg(nullptr), cfg_lwt_qos(0), cfg_lwt_retain(false),
    auto_reconnect(false), was_connected(false),
    retry_counter_multiplier(1), retry_counter(0) {}

  ~BerryMqttClient() {
    cleanup();
    freeCfg();
  }

  // Free heap-allocated config strings
  void freeCfg() {
    if (cfg_host)      { free(cfg_host);      cfg_host = nullptr; }
    if (cfg_client_id) { free(cfg_client_id); cfg_client_id = nullptr; }
    if (cfg_user)      { free(cfg_user);      cfg_user = nullptr; }
    if (cfg_pass)      { free(cfg_pass);      cfg_pass = nullptr; }
    if (cfg_lwt_topic) { free(cfg_lwt_topic); cfg_lwt_topic = nullptr; }
    if (cfg_lwt_msg)   { free(cfg_lwt_msg);   cfg_lwt_msg = nullptr; }
  }

  // Store connection parameters for reconnect
  void storeCfg(const char *host, uint16_t port, const char *client_id,
                const char *user, const char *pass, bool tls,
                const char *lwt_topic, const char *lwt_msg,
                uint8_t lwt_qos, bool lwt_retain) {
    freeCfg();
    cfg_host      = host      ? strdup(host)      : nullptr;
    cfg_port      = port;
    cfg_client_id = client_id ? strdup(client_id) : nullptr;
    cfg_user      = user      ? strdup(user)      : nullptr;
    cfg_pass      = pass      ? strdup(pass)      : nullptr;
    use_tls       = tls;
    cfg_lwt_topic = lwt_topic ? strdup(lwt_topic) : nullptr;
    cfg_lwt_msg   = lwt_msg   ? strdup(lwt_msg)   : nullptr;
    cfg_lwt_qos   = lwt_qos;
    cfg_lwt_retain = lwt_retain;
  }

  void cleanup() {
    if (mqtt) {
      if (mqtt->connected()) { mqtt->disconnect(); }
      delete mqtt;
      mqtt = nullptr;
    }
    if (wifi_tcp) { wifi_tcp->stop(); delete wifi_tcp; wifi_tcp = nullptr; }
#ifdef USE_TLS
    if (wifi_tls) { wifi_tls->stop(); delete wifi_tls; wifi_tls = nullptr; }
#endif
    active_client = nullptr;
  }

  // Reset backoff state after successful connection
  void resetBackoff() {
    retry_counter_multiplier = 1;
    retry_counter = 0;
  }

  // Advance backoff for next retry attempt, returns delay in seconds
  // Matches xdrv_02_9_mqtt.ino max of 120secs
  uint32_t advanceBackoff() {
    uint32_t delay_sec = Settings->mqtt_retry * retry_counter_multiplier;
    if (delay_sec < 120) { retry_counter_multiplier++; }
    retry_counter = millis() + (delay_sec * 1000);
    return delay_sec;
  }

  // Attempt reconnect using stored config.
  // Returns MQTT_CONNECTED (0) on success, or a negative state code on failure:
  //   -5 = MQTT_DNS_DISCONNECTED, -2 = MQTT_TLS_NOT_AVAILABLE, or PubSubClient state code
  int32_t reconnect(bvm *vm) {
    if (!cfg_host || !cfg_client_id) { return MQTT_CONNECT_FAILED; }

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BMQTT D_ATTEMPTING_CONNECTION));
    cleanup();

    // Resolve hostname before allocating transport
    IPAddress ip;
    if (!WifiHostByName(cfg_host, ip)) {
      return -5;  // MQTT_DNS_DISCONNECTED
    }

    const char *conn_user = cfg_user;
    const char *conn_pass = cfg_pass;

    // Rebuild transport
    if (use_tls) {
#ifdef USE_TLS
      #if MQTT_MAX_PACKET_SIZE > 2000
        wifi_tls = new BearSSL::WiFiClientSecure_light(4096,4096);
      #else
        wifi_tls = new BearSSL::WiFiClientSecure_light(2048,2048);
      #endif
      wifi_tls->setTrustAnchor(Tasmota_TA, nitems(Tasmota_TA));
      wifi_tls->setDomainName(cfg_host);

      // Auto-detect AWS IoT: look for ".iot." and ".amazonaws.com" in hostname (same as xdrv_02_9_mqtt.ino)
      size_t host_len = strlen(cfg_host);
      const char *suffix = ".amazonaws.com";
      size_t suffix_len = 14;  // strlen(".amazonaws.com")
      bool aws_iot_host = strstr(cfg_host, ".iot.") != nullptr &&
                          host_len >= suffix_len &&
                          strcmp(cfg_host + host_len - suffix_len, suffix) == 0;

      if (cfg_port == 443 && aws_iot_host) {
        static const char *alpn_mqtt = "mqtt";
        wifi_tls->setALPN(&alpn_mqtt, 1);
      }

#if defined(USE_MQTT_CLIENT_CERT)
      loadTlsDir();
      if ((nullptr != AWS_IoT_Private_Key) && (nullptr != AWS_IoT_Client_Certificate)) {
        wifi_tls->setClientECCert(AWS_IoT_Client_Certificate,
                                  AWS_IoT_Private_Key,
                                  0xFFFF, 0);
#ifdef USE_MQTT_AWS_IOT
        if (aws_iot_host) {
          conn_user = nullptr;
          conn_pass = nullptr;
        }
#endif  // USE_MQTT_AWS_IOT
      }
#endif  // USE_MQTT_CLIENT_CERT

      active_client = wifi_tls;
#else
      return MQTT_CONNECT_FAILED;  // TLS not available
#endif
    } else {
      wifi_tcp = new WiFiClient();
      active_client = wifi_tcp;
      if ((443 == cfg_port) || (8883 == cfg_port) || (8443 == cfg_port)) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BMQTT "Warning non-TLS connection on TLS port %d"), cfg_port);
      }
    }

    // Rebuild PubSubClient
    mqtt = new PubSubClient();
    mqtt->setClient(*active_client);
    mqtt->setBufferSize(MQTT_MAX_PACKET_SIZE);
    mqtt->setKeepAlive(Settings->mqtt_keepalive);
    mqtt->setSocketTimeout(Settings->mqtt_socket_timeout);

    bvm *capture_vm = vm;
    mqtt->setCallback(
      [capture_vm](char* topic, uint8_t* payload, unsigned int length) {
        BerryMqttClient::dispatch(capture_vm, topic, payload, length);
      }
    );

    mqtt->setServer(ip, cfg_port);

    // Connect — PubSubClient handles NULL user/pass/lwt gracefully
    bool result = mqtt->connect(cfg_client_id, conn_user, conn_pass,
                                cfg_lwt_topic, cfg_lwt_qos, cfg_lwt_retain,
                                cfg_lwt_msg, true);

    if (result) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BMQTT D_CONNECTED " to %s:%d"), cfg_host, cfg_port);
#ifdef USE_TLS
      if (use_tls) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BMQTT "TLS connected, stack low mark %d"),
          uxTaskGetStackHighWaterMark(nullptr));
        if (!wifi_tls->getMFLNStatus()) {
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BMQTT "MFLN not supported by TLS server"));
        }
      }
#endif
      return MQTT_CONNECTED;
    }
    // failure logged by caller with retry info
    return mqtt->state();
  }

  // Retrieve BerryMqttClient* from Berry instance's .p member
  static BerryMqttClient* fromBerry(bvm *vm) {
    be_getmember(vm, 1, ".p");
    void *p = be_tocomptr(vm, -1);
    be_pop(vm, 1);
    return (BerryMqttClient*) p;
  }

  // Callback dispatch: PubSubClient callback -> Berry closure
  //
  // This fires synchronously from within PubSubClient.loop(), which is called
  // from be_mqttc_loop(), which has `self` at stack position 1.
  // So we can safely access .on_message from self to dispatch to the Berry closure.
  static void dispatch(bvm *vm, char* topic, uint8_t* payload, unsigned int length) {
    if (vm == nullptr) return;

    // Get the callback closure from self.on_message (self is at stack position 1)
    be_getmember(vm, 1, ".on_message");
    if (be_isnil(vm, -1)) {
      be_pop(vm, 1);
      return;  // no callback registered
    }

    // Push arguments: topic(string), idx(int=0), payload_s(string), payload_b(bytes)
    be_pushstring(vm, topic);
    be_pushint(vm, 0);
    be_pushnstring(vm, (const char*)payload, length);
    be_pushbytes(vm, payload, length);

    // Call the closure with 4 arguments
    int32_t ret = be_pcall(vm, 4);
    if (ret != 0) {
      be_error_pop_all(vm);  // clear Berry stack on error
    } else {
      be_pop(vm, 5);  // pop retval (replaces closure) + 4 args
    }
  }

  // Fire the .on_connect Berry callback (no arguments)
  static void dispatchOnConnect(bvm *vm) {
    if (vm == nullptr) return;

    be_getmember(vm, 1, ".on_connect");
    if (be_isnil(vm, -1)) {
      be_pop(vm, 1);
      return;
    }

    int32_t ret = be_pcall(vm, 0);
    if (ret != 0) {
      be_error_pop_all(vm);
    } else {
      be_pop(vm, 1);  // pop retval (replaces closure)
    }
  }
};

/*********************************************************************************************\
 * Native Berry functions
\*********************************************************************************************/

extern "C" {

  // mqttclient.init() -> nil
  int32_t be_mqttc_init(bvm *vm);
  int32_t be_mqttc_init(bvm *vm) {
    BerryMqttClient *ctx = new BerryMqttClient();
    be_pushcomptr(vm, (void*) ctx);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  // mqttclient.deinit() -> nil
  int32_t be_mqttc_deinit(bvm *vm);
  int32_t be_mqttc_deinit(bvm *vm) {
    BerryMqttClient *ctx = BerryMqttClient::fromBerry(vm);
    if (ctx) {
      delete ctx;
      be_pushcomptr(vm, (void*) nullptr);
      be_setmember(vm, 1, ".p");
    }
    be_return_nil(vm);
  }

  // mqttclient.connect(host, port, client_id [, user, pass, tls, lwt_topic, lwt_msg, lwt_qos, lwt_retain]) -> bool
  int32_t be_mqttc_connect(bvm *vm);
  int32_t be_mqttc_connect(bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc < 4 || !be_isstring(vm, 2) || !be_isint(vm, 3) || !be_isstring(vm, 4)) {
      be_raise(vm, "type_error", "connect(host:string, port:int, client_id:string [, user, pass, tls, lwt_topic, lwt_msg, lwt_qos, lwt_retain])");
    }

    BerryMqttClient *ctx = BerryMqttClient::fromBerry(vm);
    if (!ctx) { be_raise(vm, "internal_error", "mqttclient not initialized"); }

    const char *host      = be_tostring(vm, 2);
    int32_t port          = be_toint(vm, 3);
    const char *client_id = be_tostring(vm, 4);
    const char *user      = (argc >= 5 && be_isstring(vm, 5)) ? be_tostring(vm, 5) : nullptr;
    const char *pass      = (argc >= 6 && be_isstring(vm, 6)) ? be_tostring(vm, 6) : nullptr;
    bool use_tls          = (argc >= 7 && be_isbool(vm, 7))   ? be_tobool(vm, 7)   : false;
    const char *lwt_topic = (argc >= 8 && be_isstring(vm, 8)) ? be_tostring(vm, 8) : nullptr;
    const char *lwt_msg   = (argc >= 9 && be_isstring(vm, 9)) ? be_tostring(vm, 9) : nullptr;
    int32_t lwt_qos       = (argc >= 10 && be_isint(vm, 10))  ? be_toint(vm, 10)   : 0;
    bool lwt_retain       = (argc >= 11 && be_isbool(vm, 11)) ? be_tobool(vm, 11)  : false;

#ifndef USE_TLS
    if (use_tls) {
      be_raise(vm, "feature_error", "TLS not available in this build");
    }
#endif

    // Store config for reconnect — Berry string pointers only valid during this call
    ctx->storeCfg(host, port, client_id, user, pass, use_tls,
                  lwt_topic, lwt_msg, lwt_qos, lwt_retain);

    // Reset flags for new connection attempt
    ctx->was_connected = false;
    ctx->resetBackoff();

    // Delegate to reconnect() which uses stored cfg_* fields
    int32_t rc = ctx->reconnect(vm);

    // Enable auto-reconnect regardless of success/failure
    ctx->auto_reconnect = true;

    if (rc != MQTT_CONNECTED) {
      uint32_t retry_sec = ctx->advanceBackoff();
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BMQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND),
        ctx->cfg_host ? ctx->cfg_host : "?", ctx->cfg_port, rc, retry_sec);
    }
    // on success, was_connected stays false — loop() will detect the transition,
    // fire on_connect callback, and call resetBackoff()

    be_pushbool(vm, rc == MQTT_CONNECTED);
    be_return(vm);
  }

  // mqttclient.disconnect() -> nil
  int32_t be_mqttc_disconnect(bvm *vm);
  int32_t be_mqttc_disconnect(bvm *vm) {
    BerryMqttClient *ctx = BerryMqttClient::fromBerry(vm);
    if (ctx) {
      ctx->auto_reconnect = false;  // intentional disconnect stops retries
      ctx->was_connected = false;
      ctx->cleanup();
    }
    be_return_nil(vm);
  }

  // mqttclient.publish(topic:string, payload:string|bytes [, retain:bool]) -> bool
  int32_t be_mqttc_publish(bvm *vm);
  int32_t be_mqttc_publish(bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc < 3 || !be_isstring(vm, 2) || (!be_isstring(vm, 3) && !be_isbytes(vm, 3))) {
      be_raise(vm, "type_error", "publish(topic:string, payload:string|bytes [, retain:bool])");
    }

    BerryMqttClient *ctx = BerryMqttClient::fromBerry(vm);
    if (!ctx || !ctx->mqtt) { be_raise(vm, "internal_error", "mqttclient not connected"); }

    const char *topic = be_tostring(vm, 2);
    bool retain = (argc >= 4 && be_isbool(vm, 4)) ? be_tobool(vm, 4) : false;

    bool result;
    if (be_isstring(vm, 3)) {
      const char *payload = be_tostring(vm, 3);
      result = ctx->mqtt->publish(topic, (const uint8_t*)payload, strlen(payload), retain);
    } else {
      // bytes payload
      size_t payload_len = 0;
      const uint8_t *payload = (const uint8_t*) be_tobytes(vm, 3, &payload_len);
      result = ctx->mqtt->publish(topic, payload, payload_len, retain);
    }

    be_pushbool(vm, result);
    be_return(vm);
  }

  // mqttclient.subscribe(topic:string [, qos:int]) -> bool
  int32_t be_mqttc_subscribe(bvm *vm);
  int32_t be_mqttc_subscribe(bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc < 2 || !be_isstring(vm, 2)) {
      be_raise(vm, "type_error", "subscribe(topic:string [, qos:int])");
    }

    BerryMqttClient *ctx = BerryMqttClient::fromBerry(vm);
    if (!ctx || !ctx->mqtt) { be_raise(vm, "internal_error", "mqttclient not connected"); }

    const char *topic = be_tostring(vm, 2);
    uint8_t qos = (argc >= 3 && be_isint(vm, 3)) ? (uint8_t)be_toint(vm, 3) : 0;

    bool result = ctx->mqtt->subscribe(topic, qos);
    be_pushbool(vm, result);
    be_return(vm);
  }

  // mqttclient.unsubscribe(topic:string) -> bool
  int32_t be_mqttc_unsubscribe(bvm *vm);
  int32_t be_mqttc_unsubscribe(bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc < 2 || !be_isstring(vm, 2)) {
      be_raise(vm, "type_error", "unsubscribe(topic:string)");
    }

    BerryMqttClient *ctx = BerryMqttClient::fromBerry(vm);
    if (!ctx || !ctx->mqtt) { be_raise(vm, "internal_error", "mqttclient not connected"); }

    const char *topic = be_tostring(vm, 2);
    bool result = ctx->mqtt->unsubscribe(topic);
    be_pushbool(vm, result);
    be_return(vm);
  }

  // mqttclient.loop() -> nil
  // Must be called regularly (every ~50ms) via tasmota.add_fast_loop()
  // Handles message processing AND auto-reconnect with exponential backoff
  int32_t be_mqttc_loop(bvm *vm);
  int32_t be_mqttc_loop(bvm *vm) {
    BerryMqttClient *ctx = BerryMqttClient::fromBerry(vm);
    if (!ctx) { be_return_nil(vm); }

    if (ctx->mqtt) {
      ctx->mqtt->loop();  // processes incoming messages, fires callbacks synchronously

      bool is_connected = ctx->mqtt->connected();

      if (is_connected && !ctx->was_connected) {
        // Just connected (or reconnected) — fire on_connect callback
        ctx->was_connected = true;
        ctx->resetBackoff();
        BerryMqttClient::dispatchOnConnect(vm);
      } else if (!is_connected && ctx->was_connected) {
        // Just lost connection — release transport immediately (same as xdrv_02_9_mqtt.ino)
        ctx->was_connected = false;
        int32_t state = ctx->mqtt->state();
        ctx->cleanup();
        if (ctx->auto_reconnect) {
          uint32_t retry_sec = ctx->advanceBackoff();
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BMQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND),
            ctx->cfg_host ? ctx->cfg_host : "?", ctx->cfg_port, state, retry_sec);
        }
      }
    }

    // Auto-reconnect logic
    if (ctx->auto_reconnect && !ctx->was_connected) {
      uint32_t now = millis();
      if ((int32_t)(now - ctx->retry_counter) >= 0) {
        // Time to attempt reconnect
        int32_t rc = ctx->reconnect(vm);
        if (rc != MQTT_CONNECTED) {
          uint32_t retry_sec = ctx->advanceBackoff();
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_BMQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND),
            ctx->cfg_host ? ctx->cfg_host : "?", ctx->cfg_port, rc, retry_sec);
        }
        // on success, was_connected is still false — next loop iteration will
        // detect the connection and fire on_connect callback
      }
    }

    be_return_nil(vm);
  }

  // mqttclient.connected() -> bool
  int32_t be_mqttc_connected(bvm *vm);
  int32_t be_mqttc_connected(bvm *vm) {
    BerryMqttClient *ctx = BerryMqttClient::fromBerry(vm);
    bool result = (ctx && ctx->mqtt) ? ctx->mqtt->connected() : false;
    be_pushbool(vm, result);
    be_return(vm);
  }

  // mqttclient.state() -> int
  // Returns PubSubClient state codes
  // Possible values for state - PubSubClient.h
  //   Tasmota MQTT_DNS_DISCONNECTED        -5
  //   #define MQTT_CONNECTION_TIMEOUT      -4
  //   #define MQTT_CONNECTION_LOST         -3
  //   #define MQTT_CONNECT_FAILED          -2
  //   #define MQTT_DISCONNECTED            -1
  //   #define MQTT_CONNECTED                0
  //   #define MQTT_CONNECT_BAD_PROTOCOL     1
  //   #define MQTT_CONNECT_BAD_CLIENT_ID    2
  //   #define MQTT_CONNECT_UNAVAILABLE      3
  //   #define MQTT_CONNECT_BAD_CREDENTIALS  4
  //   #define MQTT_CONNECT_UNAUTHORIZED     5
  int32_t be_mqttc_state(bvm *vm);
  int32_t be_mqttc_state(bvm *vm) {
    BerryMqttClient *ctx = BerryMqttClient::fromBerry(vm);
    int32_t state = (ctx && ctx->mqtt) ? ctx->mqtt->state() : MQTT_DISCONNECTED;
    be_pushint(vm, state);
    be_return(vm);
  }

  // mqttclient.set_on_message(closure) -> nil
  // closure signature: def (topic:string, idx:int, payload_s:string, payload_b:bytes) -> nil
  int32_t be_mqttc_set_on_message(bvm *vm);
  int32_t be_mqttc_set_on_message(bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc < 2 || !be_isfunction(vm, 2)) {
      be_raise(vm, "type_error", "set_on_message(closure:function)");
    }
    be_pushvalue(vm, 2);
    be_setmember(vm, 1, ".on_message");
    be_return_nil(vm);
  }

  // mqttclient.set_on_connect(closure) -> nil
  // closure signature: def () -> nil
  // Called after every successful connect or reconnect
  int32_t be_mqttc_set_on_connect(bvm *vm);
  int32_t be_mqttc_set_on_connect(bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc < 2 || !be_isfunction(vm, 2)) {
      be_raise(vm, "type_error", "set_on_connect(closure:function)");
    }
    be_pushvalue(vm, 2);
    be_setmember(vm, 1, ".on_connect");
    be_return_nil(vm);
  }

  // mqttclient.set_auto_reconnect(enabled:bool) -> nil
  int32_t be_mqttc_set_auto_reconnect(bvm *vm);
  int32_t be_mqttc_set_auto_reconnect(bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc < 2 || !be_isbool(vm, 2)) {
      be_raise(vm, "type_error", "set_auto_reconnect(enabled:bool)");
    }

    BerryMqttClient *ctx = BerryMqttClient::fromBerry(vm);
    if (!ctx) { be_raise(vm, "internal_error", "mqttclient not initialized"); }

    ctx->auto_reconnect = be_tobool(vm, 2);
    if (ctx->auto_reconnect) {
      ctx->resetBackoff();  // reset backoff when re-enabling
    }
    be_return_nil(vm);
  }

}  // extern "C"

#endif  // ESP32
#endif  // USE_BERRY_MQTTCLIENT
#endif  // USE_BERRY
