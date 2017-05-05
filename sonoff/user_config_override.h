/*****************************************************************************************************\
 * User specific configuration parameters to override user_config.h
 *
 * ATTENTION: - Changes to most PARAMETER defines will only override flash settings if you change
 *              define CFG_HOLDER.
 *            - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *            - You still need to update user_config.h for major defines MODULE and USE_MQTT_TLS.
 *            - Changing MODULE defines are not being tested for validity as they are in user_config.h.
 *            - Most parameters can be changed online using commands via MQTT, WebConsole or serial.
 *            - So I see no use in this but anyway, your on your own.
\*****************************************************************************************************/

// Examples
//#ifdef CFG_HOLDER
//#undef CFG_HOLDER
//#endif
//#define CFG_HOLDER             0x20161210

//#ifdef STA_SSID1
//#undef STA_SSID1
//#endif
//#define STA_SSID1              "yourssid1"
#ifdef CFG_HOLDER
#undef CFG_HOLDER
#endif
#define CFG_HOLDER             0x20161213        // [Reset 1] Change this value to load following default configuration parameters
// -- Wifi ----------------------------------------
#ifdef STA_SSID1
#undef STA_SSID1
#endif
#define STA_SSID1              "BodeNet_Keller"  // [Ssid1] Wifi SSID
#ifdef STA_PASS1
#undef STA_PASS1
#endif
#define STA_PASS1              "BettyUndStefa"          // [Password1] Wifi password
#ifdef WIFI_CONFIG_TOOL
#undef WIFI_CONFIG_TOOL
#endif
#define WIFI_CONFIG_TOOL       WIFI_MANAGER      // [WifiConfig] Default tool if wifi fails to connect

// -- Syslog --------------------------------------
#ifdef SYS_LOG_HOST
#undef SYS_LOG_HOST
#endif
#define SYS_LOG_HOST           "192.168.178.42"          // [LogHost] (Linux) syslog host


#define USE_MQTT_TLS                             // EXPERIMENTAL Use TLS for MQTT connection (+53k code, +20k mem) - Disable by //

#ifdef MQTT_HOST
#undef MQTT_HOST
#endif
#define MQTT_HOST            "elocm.homeserver.com"    // [MqttHost]

#ifdef MQTT_FINGERPRINT
#undef MQTT_FINGERPRINT
#endif
#define MQTT_FINGERPRINT     "EF:2A:0F:3E:87:95:6E:F0:4B:A6:F4:38:73:29:1A:14:7E:A9:A1:B7"  // [MqttFingerprint]

#ifdef MQTT_PORT
#undef MQTT_PORT
#endif
#define MQTT_PORT            8883                // [MqttPort] MQTT TLS port

#ifdef MQTT_USER
#undef MQTT_USER
#endif
#define MQTT_USER            "homematic"         // [MqttUser] Mandatory user

#ifdef MQTT_PASS
#undef MQTT_PASS
#endif
#define MQTT_PASS            "geheim"            // [MqttPassword] Mandatory password


#undef MQTT_BUTTON_RETAIN
#undef MQTT_POWER_RETAIN
#undef MQTT_SWITCH_RETAIN
#undef MQTT_TOPIC_RETAIN

#define MQTT_BUTTON_RETAIN     1                 // [ButtonRetain] Button may send retain flag (0 = off, 1 = on)
#define MQTT_POWER_RETAIN      1                 // [PowerRetain] Power status message may send retain flag (0 = off, 1 = on)
#define MQTT_SWITCH_RETAIN     1                 // [SwitchRetain] Switch may send retain flag (0 = off, 1 = on)
#define MQTT_TOPIC_RETAIN      1                 // [TopicRetain] Topics may send retain flag (0 = off, 1 = on)

#undef SUB_PREFIX
#undef PUB_PREFIX
#undef PUB_PREFIX2
#undef USE_ADC_VCC 

#define SUB_PREFIX             "hm/setting"           // Sonoff devices subscribe to:- SUB_PREFIX/MQTT_TOPIC and SUB_PREFIX/MQTT_GRPTOPIC
#define PUB_PREFIX             "hm/status"            // Sonoff devices publish to:- PUB_PREFIX/MQTT_TOPIC
#define PUB_PREFIX2            "hm/status"            // Sonoff devices publish telemetry data to:- PUB_PREFIX2/MQTT_TOPIC/UPTIME, POWER/LIGHT and TIME

#ifdef USE_DOMOTICZ
#undef USE_DOMOTICZ                             // Enable Domoticz (+7k code, +0.3k mem) - Disable by //
#endif

#ifdef USE_DISCOVERY
#undef USE_DISCOVERY                            // Enable mDNS for the following services (+8k code, +0.3k mem) - Disable by //
#endif

#ifdef WEBSERVER_ADVERTISE
#undef WEBSERVER_ADVERTISE                    // Provide access to webserver by name <Hostname>.local/
#endif

#define USE_DS18x20                              // Optional using OneWire library for multiple DS18B20 and/or DS18S20 (+2k code)
#define USE_I2C                                 // I2C using library wire (+10k code, 0.2k mem) - Disable by //
#define USE_PCF8574                             // Add I2C code for PCF8574 8-channel DIO I/O chip.

// -- Application ---------------------------------
#undef APP_TIMEZONE
#define APP_TIMEZONE           99                 // [Timezone] +1 hour (Amsterdam) (-12 .. 12 = hours from UTC, 99 = use TIME_DST/TIME_STD)
