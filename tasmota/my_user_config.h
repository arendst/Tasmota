/*
  my_user_config.h - user specific configuration for Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifndef _MY_USER_CONFIG_H_
#define _MY_USER_CONFIG_H_

/*********************************************************************************************\
 * This file consists of TWO sections.
 *
 * SECTION 1:
 * The first section contains PARAMETERS overriding flash settings if define CFG_HOLDER is CHANGED.
 * All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
 *   Corresponding MQTT/Serial/Console commands are shown in [brackets]
 *
 * SECTION 2:
 * The second section contains Enabled and Disabled FEATURES allowing different program size.
 *   Changes in this section do NOT need a change of define CFG_HOLDER.
 *
 * ATTENTION:
 * Users are advised to use the user_config_override.h file for most changes.
\*********************************************************************************************/

/*********************************************************************************************\
 * SECTION 1
 * - After initial load any change here only take effect if CFG_HOLDER is changed too
\*********************************************************************************************/

// -- Master parameter control --------------------
#define CFG_HOLDER             4617              // [Reset 1] Change this value (max 32000) to load SECTION1 configuration parameters to flash
                                                 // If following define is disabled it increases configuration corruption detection BUT
                                                 //  it only allows firmware upgrades starting from version 6.6.0.11

// -- Project -------------------------------------
#define PROJECT                "tasmota"         // PROJECT is used as the default topic delimiter

// If not selected the default will be SONOFF_BASIC
//#define MODULE                 SONOFF_BASIC      // [Module] Select default module from tasmota_template.h
#ifdef ESP8266
#define FALLBACK_MODULE        SONOFF_BASIC      // [Module2] Select default module on fast reboot where USER_MODULE is user template
//#define USER_TEMPLATE "{\"NAME\":\"Generic\",\"GPIO\":[1,1,1,1,1,1,1,1,1,1,1,1,1,1],\"FLAG\":0,\"BASE\":18}"  // [Template] Set JSON template
#endif  // ESP8266
#ifdef ESP32
#define FALLBACK_MODULE        WEMOS             // [Module2] Select default module on fast reboot where USER_MODULE is user template
//#define USER_TEMPLATE "{\"NAME\":\"ESP32-DevKit\",\"GPIO\":[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1],\"FLAG\":0,\"BASE\":1}"  // [Template] Set JSON template
#endif  // ESP32

#define SAVE_DATA              1                 // [SaveData] Save changed parameters to Flash (0 = disable, 1 - 3600 seconds)
#define SAVE_STATE             true              // [SetOption0] Save changed power state to Flash (false = disable, true = enable)
#define BOOT_LOOP_OFFSET       1                 // [SetOption36] Number of boot loops before starting restoring defaults (0 = disable, 1..200 = boot loops offset)

// -- Wi-Fi ---------------------------------------
#define WIFI_IP_ADDRESS        "0.0.0.0"         // [IpAddress1] Set to 0.0.0.0 for using DHCP or enter a static IP address
#define WIFI_GATEWAY           "192.168.1.1"     // [IpAddress2] If not using DHCP set Gateway IP address
#define WIFI_SUBNETMASK        "255.255.255.0"   // [IpAddress3] If not using DHCP set Network mask
#define WIFI_DNS               "192.168.1.1"     // [IpAddress4] If not using DHCP set DNS1 IP address (might be equal to WIFI_GATEWAY)
#define WIFI_DNS2              "0.0.0.0"         // [IpAddress5] If not using DHCP set DNS2 IP address (might be equal to WIFI_GATEWAY)

#ifndef STA_SSID1
#define STA_SSID1              ""                // [Ssid1] Wi-Fi SSID
#endif
#ifndef STA_PASS1
#define STA_PASS1              ""                // [Password1] Wi-Fi password
#endif
#ifndef STA_SSID2
#define STA_SSID2              ""                // [Ssid2] Optional alternate AP Wi-Fi SSID
#endif
#ifndef STA_PASS2
#define STA_PASS2              ""                // [Password2] Optional alternate AP Wi-Fi password
#endif
#ifndef WIFI_AP_PASSPHRASE
#define WIFI_AP_PASSPHRASE     ""                // AccessPoint passphrase. For WPA2 min 8 char, for open use "" (max 63 char).
#endif
#define WIFI_CONFIG_TOOL       WIFI_RETRY        // [WifiConfig] Default tool if Wi-Fi fails to connect (default option: 4 - WIFI_RETRY)
                                                 // (WIFI_RESTART, WIFI_MANAGER, WIFI_RETRY, WIFI_WAIT, WIFI_SERIAL, WIFI_MANAGER_RESET_ONLY)
                                                 // The configuration can be changed after first setup using WifiConfig 0, 2, 4, 5, 6 and 7.
#define DNS_TIMEOUT            1000              // [DnsTimeout] Number of ms before DNS timeout
#define WIFI_ARP_INTERVAL      60                // [SetOption41] Send gratuitous ARP interval
#define WIFI_SCAN_AT_RESTART   false             // [SetOption56] Scan Wi-Fi network at restart for configured AP's
#define WIFI_SCAN_REGULARLY    true              // [SetOption57] Scan Wi-Fi network every 44 minutes for configured AP's
#define WIFI_NO_SLEEP          false             // [SetOption127] Sets Wifi in no-sleep mode which improves responsiveness on some routers
#define WIFI_DEFAULT_HOSTNAME  "%s-%04d"         // [Hostname] Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>

// -- Syslog --------------------------------------
#define SYS_LOG_HOST           ""                // [LogHost] (Linux) syslog host
#define SYS_LOG_PORT           514               // [LogPort] default syslog UDP port
#define SYS_LOG_LEVEL          LOG_LEVEL_NONE    // [SysLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)
#define SERIAL_LOG_LEVEL       LOG_LEVEL_INFO    // [SerialLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)
#define WEB_LOG_LEVEL          LOG_LEVEL_INFO    // [WebLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)
#define MQTT_LOG_LEVEL         LOG_LEVEL_NONE    // [MqttLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)

// -- MQTT ----------------------------------------
#define MQTT_USE               true              // [SetOption3] Select default MQTT use (false = Off, true = On)

#define MQTT_KEEPALIVE         30                // [MqttKeepAlive] Number of seconds between KeepAlive messages
#define MQTT_SOCKET_TIMEOUT    4                 // [MqttTimeout] Number of seconds before Mqtt connection timeout
#define MQTT_WIFI_CLIENT_TIMEOUT 200             // [MqttWifiTimeout] Number of milliseconds before Mqtt Wi-Fi timeout

#define MQTT_HOST              ""                // [MqttHost]

#define MQTT_FINGERPRINT1      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // [MqttFingerprint1] (auto-learn)
#define MQTT_FINGERPRINT2      0xDA,0x39,0xA3,0xEE,0x5E,0x6B,0x4B,0x0D,0x32,0x55,0xBF,0xEF,0x95,0x60,0x18,0x90,0xAF,0xD8,0x07,0x09  // [MqttFingerprint2] (invalid - value from sha1(""))
#define MQTT_PORT              1883              // [MqttPort] MQTT port (10123 on CloudMQTT)
#define MQTT_USER              "DVES_USER"       // [MqttUser] MQTT user
#define MQTT_PASS              "DVES_PASS"       // [MqttPassword] MQTT password

#define MQTT_BUTTON_RETAIN     false             // [ButtonRetain] Button may send retain flag (false = off, true = on)
#define MQTT_POWER_RETAIN      false             // [PowerRetain] Power status message may send retain flag (false = off, true = on)
#define MQTT_SWITCH_RETAIN     false             // [SwitchRetain] Switch may send retain flag (false = off, true = on)
#define MQTT_SENSOR_RETAIN     false             // [SensorRetain] Sensor may send retain flag (false = off, true = on)
#define MQTT_INFO_RETAIN       false             // [InfoRetain] Info may send retain flag (false = off, true = on)
#define MQTT_STATE_RETAIN      false             // [StateRetain] State may send retain flag (false = off, true = on)
#define MQTT_STATUS_RETAIN     false             // [StatusRetain] Status may send retain flag (false = off, true = on)
#define MQTT_NO_HOLD_RETAIN    false             // [SetOption62] Disable retain flag on HOLD messages
#define MQTT_NO_RETAIN         false             // [SetOption104] No Retain - disable all MQTT retained messages, some brokers don't support it: AWS IoT, Losant

#define MQTT_STATUS_OFF        "OFF"             // [StateText1] Command or Status result when turned off (needs to be a string like "0" or "Off")
#define MQTT_STATUS_ON         "ON"              // [StateText2] Command or Status result when turned on (needs to be a string like "1" or "On")
#define MQTT_CMND_TOGGLE       "TOGGLE"          // [StateText3] Command to send when toggling (needs to be a string like "2" or "Toggle")
#define MQTT_CMND_HOLD         "HOLD"            // [StateText4] Command to send when button is kept down for over KEY_HOLD_TIME * 0.1 seconds (needs to be a string like "HOLD")

// -- MQTT topics ---------------------------------
  // Example "tasmota/bedroom/%topic%/%prefix%/" up to 80 characers
#define MQTT_FULLTOPIC         "%prefix%/%topic%/" // [FullTopic] Subscribe and Publish full topic name - Legacy topic

// %prefix% token options
#define SUB_PREFIX             "cmnd"            // [Prefix1] Tasmota devices subscribe to %prefix%/%topic% being SUB_PREFIX/MQTT_TOPIC and SUB_PREFIX/MQTT_GRPTOPIC
#define PUB_PREFIX             "stat"            // [Prefix2] Tasmota devices publish to %prefix%/%topic% being PUB_PREFIX/MQTT_TOPIC
#define PUB_PREFIX2            "tele"            // [Prefix3] Tasmota devices publish telemetry data to %prefix%/%topic% being PUB_PREFIX2/MQTT_TOPIC/UPTIME, POWER and TIME
                                                 //   May be named the same as PUB_PREFIX
// %topic% token options (also ButtonTopic and SwitchTopic)
#define MQTT_TOPIC             PROJECT "_%06X"   // [Topic] unique MQTT device topic including (part of) device MAC address
#define MQTT_GRPTOPIC          "tasmotas"        // [GroupTopic] MQTT Group topic
#define MQTT_GROUPTOPIC_FORMAT false             // [SetOption75] GroupTopic replaces %topic% (false) or fixed topic cmnd/grouptopic (true)
#define MQTT_BUTTON_TOPIC      "0"               // [ButtonTopic] MQTT button topic, "0" = same as MQTT_TOPIC, set to 'PROJECT "_BTN_%06X"' for unique topic including device MAC address
#define MQTT_SWITCH_TOPIC      "0"               // [SwitchTopic] MQTT button topic, "0" = same as MQTT_TOPIC, set to 'PROJECT "_SW_%06X"' for unique topic including device MAC address
#define MQTT_CLIENT_ID         "DVES_%06X"       // [MqttClient] Also fall back topic using last 6 characters of MAC address or use "DVES_%12X" for complete MAC address

// -- MQTT - Telemetry ----------------------------
#define TELE_PERIOD            300               // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)
#define TELE_ON_POWER          false             // [SetOption59] send tele/STATE together with stat/RESULT (false = Disable, true = Enable)

// -- MQTT - Domoticz -----------------------------
#define DOMOTICZ_UPDATE_TIMER  0                 // [DomoticzUpdateTimer] Send relay status (0 = disable, 1 - 3600 seconds)

// -- MQTT - Home Assistant Discovery -------------
#define HOME_ASSISTANT_DISCOVERY_ENABLE   false  // [SetOption19] Home Assistant Discovery (false = Disable, true = Enable)
#define HASS_AS_LIGHT          false             // [SetOption30] Enforce HAss autodiscovery as light
//#define DEEPSLEEP_LWT_HA_DISCOVERY             // Enable LWT topic and its payloads for read-only sensors (Status sensor not included) and binary_sensors on HAss Discovery (Commented out: all read-only sensors and binary_sensors
                                                 // won't be shown as OFFLINE on Home Assistant when the device is DeepSleeping - NOTE: This is only for read-only sensors and binary_sensors, relays will be shown as OFFLINE)

// -- MQTT - Options ------------------------------
#define MQTT_RESULT_COMMAND    false             // [SetOption4]  Switch between MQTT RESULT or COMMAND
#define MQTT_LWT_MESSAGE       false             // [SetOption10] Switch between MQTT LWT OFFLINE or empty message
#define MQTT_POWER_FORMAT      false             // [SetOption26] Switch between POWER or POWER1 for single power devices
#define MQTT_APPEND_TIMEZONE   false             // [SetOption52] Append timezone to JSON time
#define MQTT_BUTTON_SWITCH_FORCE_LOCAL   false   // [SetOption61] Force local operation when button/switch topic is set (false = off, true = on)
#define MQTT_INDEX_SEPARATOR   false             // [SetOption64] Enable "_" instead of "-" as sensor index separator
#define MQTT_TUYA_RECEIVED     false             // [SetOption66] Enable TuyaMcuReceived messages over Mqtt
#define MQTT_ONLY_JSON_OUTPUT  false             // [SetOption90] Disable non-json messages
#define MQTT_TLS_ENABLED       false             // [SetOption103] Enable TLS mode (requires TLS version)
#define MQTT_TLS_FINGERPRINT   false             // [SetOption132] Force TLS fingerprint validation instead of CA (requires TLS version)

// -- HTTP ----------------------------------------
#define WEB_SERVER             2                 // [WebServer] Web server (0 = Off, 1 = Start as User, 2 = Start as Admin)
#define WEB_PASSWORD           ""                // [WebPassword] Web server Admin mode Password for WEB_USERNAME (empty string = Disable)
#define FRIENDLY_NAME          "Tasmota"         // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa
#define EMULATION              EMUL_NONE         // [Emulation] Select Belkin WeMo (single relay/light) or Hue Bridge emulation (multi relay/light) (EMUL_NONE, EMUL_WEMO or EMUL_HUE)
#define EMULATION_HUE_1ST_GEN  false             // [Emulation] Force SetOption109 1 - if you only have Echo Dot 2nd gen devices
//#define USE_CORS                                 // [Cors] Enable CORS - Be aware that this feature is unsecure ATM (https://github.com/arendst/Tasmota/issues/6767)
  #define CORS_DOMAIN            ""                // [Cors] CORS Domain for preflight requests

// -- HTTP Options --------------------------------
#define GUI_NOSHOW_MODULE      false             // [SetOption141] Do not show module name in GUI main menu
#define GUI_NOSHOW_DEVICENAME  false             // [SetOption163] Do not show device name in GUI main menu
#define GUI_SHOW_HOSTNAME      false             // [SetOption53] Show hostname and IP address in GUI main menu
#define GUI_NOSHOW_STATETEXT   false             // [SetOption161] Do not show power state text in GUI

// -- HTTP GUI Colors -----------------------------
// HTML hex color codes. Only 3 and 6 digit hex string values are supported!! See https://www.w3schools.com/colors/colors_hex.asp
// Light theme - pre v7
// WebColor {"WebColor":["#000","#fff","#f2f2f2","#000","#fff","#000","#fff","#f00","#008000","#fff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#fff","#999","#000","#08405e"]}
/*
#define COLOR_TEXT                  "#000"       // [WebColor1] Global text color - Black
#define COLOR_BACKGROUND            "#fff"       // [WebColor2] Global background color - White
#define COLOR_FORM                  "#f2f2f2"    // [WebColor3] Form background color - Greyish
#define COLOR_INPUT_TEXT            "#000"       // [WebColor4] Input text color - Black
#define COLOR_INPUT                 "#fff"       // [WebColor5] Input background color - White
#define COLOR_CONSOLE_TEXT          "#000"       // [WebColor6] Console text color - Black
#define COLOR_CONSOLE               "#fff"       // [WebColor7] Console background color - White
#define COLOR_TEXT_WARNING          "#f00"       // [WebColor8] Warning text color - Red
#define COLOR_TEXT_SUCCESS          "#008000"    // [WebColor9] Success text color - Dark lime green
#define COLOR_BUTTON_TEXT           "#fff"       // [WebColor10] Button text color - White
#define COLOR_BUTTON                "#1fa3ec"    // [WebColor11] Button color - Vivid blue
#define COLOR_BUTTON_HOVER          "#0e70a4"    // [WebColor12] Button color when hovered over - Dark blue
#define COLOR_BUTTON_RESET          "#d43535"    // [WebColor13] Restart/Reset/Delete button color - Strong red
#define COLOR_BUTTON_RESET_HOVER    "#931f1f"    // [WebColor14] Restart/Reset/Delete button color when hovered over - Dark red
#define COLOR_BUTTON_SAVE           "#47c266"    // [WebColor15] Save button color - Moderate lime green
#define COLOR_BUTTON_SAVE_HOVER     "#5aaf6f"    // [WebColor16] Save button color when hovered over - Dark moderate lime green
#define COLOR_TIMER_TAB_TEXT        "#fff"       // [WebColor17] Config timer tab text color - White
#define COLOR_TIMER_TAB_BACKGROUND  "#999"       // [WebColor18] Config timer tab background color - Dark gray
#define COLOR_TITLE_TEXT            "#000"       // [WebColor19] Title text color - Whiteish
#define COLOR_BUTTON_OFF            "#a1d9f7"    // [WebColor20] Button color when off - Light blue
*/
// Dark theme
// WebColor {"WebColor":["#eaeaea","#252525","#4f4f4f","#000","#ddd","#65c115","#1f1f1f","#ff5661","#008000","#faffff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#faffff","#999","#eaeaea","#08405e"]}
#define COLOR_TEXT                  "#eaeaea"    // [WebColor1] Global text color - Very light gray
#define COLOR_BACKGROUND            "#252525"    // [WebColor2] Global background color - Very dark gray (mostly black)
#define COLOR_FORM                  "#4f4f4f"    // [WebColor3] Form background color - Very dark gray
#define COLOR_INPUT_TEXT            "#000"       // [WebColor4] Input text color - Black
#define COLOR_INPUT                 "#ddd"       // [WebColor5] Input background color - Very light gray
#define COLOR_CONSOLE_TEXT          "#65c115"    // [WebColor6] Console text color - Strong Green
#define COLOR_CONSOLE               "#1f1f1f"    // [WebColor7] Console background color - Very dark gray (mostly black)
#define COLOR_TEXT_WARNING          "#ff5661"    // [WebColor8] Warning text color - Brick Red
#define COLOR_TEXT_SUCCESS          "#008000"    // [WebColor9] Success text color - Dark lime green
#define COLOR_BUTTON_TEXT           "#faffff"    // [WebColor10] Button text color - Very pale (mostly white) cyan
#define COLOR_BUTTON                "#1fa3ec"    // [WebColor11] Button color - Vivid blue
#define COLOR_BUTTON_HOVER          "#0e70a4"    // [WebColor12] Button color when hovered over - Dark blue
#define COLOR_BUTTON_RESET          "#d43535"    // [WebColor13] Restart/Reset/Delete button color - Strong red
#define COLOR_BUTTON_RESET_HOVER    "#931f1f"    // [WebColor14] Restart/Reset/Delete button color when hovered over - Dark red
#define COLOR_BUTTON_SAVE           "#47c266"    // [WebColor15] Save button color - Moderate lime green
#define COLOR_BUTTON_SAVE_HOVER     "#5aaf6f"    // [WebColor16] Save button color when hovered over - Dark moderate lime green
#define COLOR_TIMER_TAB_TEXT        "#faffff"    // [WebColor17] Config timer tab text color - Very pale (mostly white) cyan.
#define COLOR_TIMER_TAB_BACKGROUND  "#999"       // [WebColor18] Config timer tab background color - Dark gray
#define COLOR_TITLE_TEXT            "#eaeaea"    // [WebColor19] Title text color - Very light gray
#define COLOR_BUTTON_OFF            "#08405e"    // [WebColor20] Button color when off - Darkest blueish

// -- KNX -----------------------------------------
#define KNX_ENABLED            false             // [Knx_Enabled] Enable KNX protocol
#define KNX_ENHANCED           false             // [Knx_Enhanced] Enable KNX Enhanced Mode

// -- mDNS ----------------------------------------
#define MDNS_ENABLED           false             // [SetOption55] Use mDNS (false = Disable, true = Enable)

// -- Time - Up to three NTP servers in your region
#define NTP_SERVER1      "2.pool.ntp.org"        // [NtpServer1] Select first NTP server by name or IP address (135.125.104.101, 2001:418:3ff::53)
#define NTP_SERVER2      "2.europe.pool.ntp.org" // [NtpServer2] Select second NTP server by name or IP address (192.36.143.134, 2a00:2381:19c6::100)
#define NTP_SERVER3      "2.nl.pool.ntp.org"     // [NtpServer3] Select third NTP server by name or IP address (46.249.42.13, 2603:c022:c003:c900::4)
                                                 // To manually set:
                                                 //   BackLog NtpServer1 2.pool.ntp.org; NtpServer2 2.europe.pool.ntp.org; NtpServer3 2.nl.pool.ntp.org

// -- Time - Start Daylight Saving Time and timezone offset from UTC in minutes
#define TIME_DST_HEMISPHERE    North             // [TimeDst] Hemisphere (0 or North, 1 or South)
#define TIME_DST_WEEK          Last              // Week of month (0 or Last, 1 or First, 2 or Second, 3 or Third, 4 or Fourth)
#define TIME_DST_DAY           Sun               // Day of week (1 or Sun, 2 or Mon, 3 or Tue, 4 or Wed, 5 or Thu, 6 or Fri, 7 or Sat)
#define TIME_DST_MONTH         Mar               // Month (1 or Jan, 2 or Feb, 3 or Mar, 4 or Apr, 5 or May, 6 or Jun, 7 or Jul, 8 or Aug, 9 or Sep, 10 or Oct, 11 or Nov, 12 or Dec)
#define TIME_DST_HOUR          2                 // Hour (0 to 23)
#define TIME_DST_OFFSET        +120              // Offset from UTC in minutes (-780 to +780)

// -- Time - Start Standard Time and timezone offset from UTC in minutes
#define TIME_STD_HEMISPHERE    North             // [TimeStd] Hemisphere (0 or North, 1 or South)
#define TIME_STD_WEEK          Last              // Week of month (0 or Last, 1 or First, 2 or Second, 3 or Third, 4 or Fourth)
#define TIME_STD_DAY           Sun               // Day of week (1 or Sun, 2 or Mon, 3 or Tue, 4 or Wed, 5 or Thu, 6 or Fri, 7 or Sat)
#define TIME_STD_MONTH         Oct               // Month (1 or Jan, 2 or Feb, 3 or Mar, 4 or Apr, 5 or May, 6 or Jun, 7 or Jul, 8 or Aug, 9 or Sep, 10 or Oct, 11 or Nov, 12 or Dec)
#define TIME_STD_HOUR          3                 // Hour (0 to 23)
#define TIME_STD_OFFSET        +60               // Offset from UTC in minutes (-780 to +780)

// -- Location ------------------------------------
#define LATITUDE               48.858360         // [Latitude] Your location to be used with sunrise and sunset
#define LONGITUDE              2.294442          // [Longitude] Your location to be used with sunrise and sunset

// -- Application ---------------------------------
#define APP_TIMEZONE           1                 // [Timezone] +1 hour (Amsterdam) (-13 .. 14 = hours from UTC, 99 = use TIME_DST/TIME_STD)
#define APP_LEDSTATE           LED_POWER         // [LedState] Function of led
                                                 //   (LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT)
#define APP_LEDMASK            0xFFFF            // [LedMask] Assign Relay to Power led (0xFFFF is default)
#define APP_ENABLE_LEDLINK     false             // [SetOption31] Enable link led blinking

#define APP_PULSETIME          0                 // [PulseTime] Time in 0.1 Sec to turn off power for relay 1 (0 = disabled)
#define APP_POWERON_STATE      POWER_ALL_SAVED   // [PowerOnState] Power On Relay state
                                                 //   (POWER_ALL_OFF, POWER_ALL_ON, POWER_ALL_SAVED_TOGGLE, POWER_ALL_SAVED, POWER_ALL_ALWAYS_ON, POWER_ALL_OFF_PULSETIME_ON)
#define APP_BLINKTIME          10                // [BlinkTime] Time in 0.1 Sec to blink/toggle power for relay 1
#define APP_BLINKCOUNT         10                // [BlinkCount] Number of blinks (0 = 32000)
#define APP_BISTABLE_PULSE     40                // [SetOption45] Pulse time in ms for two coil bistable latching relays

#define APP_NORMAL_SLEEP       false             // [SetOption60] Enable normal sleep instead of dynamic sleep
#define TASMOTA_SLEEP          50                // [Sleep] Sleep time to lower energy consumption (0 = Off, value in milliseconds),
#define PWM_MAX_SLEEP          10                // Sleep will be lowered to this value when light is on, to avoid flickering, and when buzzer is on for better on/off period accuracy

#define KEY_DEBOUNCE_TIME      50                // [ButtonDebounce] Number of mSeconds button press debounce time
#define KEY_HOLD_TIME          40                // [SetOption32] Number of 0.1 seconds to hold Button or external Pushbutton before sending HOLD message
#define KEY_DISABLE_MULTIPRESS false             // [SetOption1]  Disable button multipress
#define KEY_SWAP_DOUBLE_PRESS  false             // [SetOption11] Swap button single and double press functionality
#define KEY_ONLY_SINGLE_PRESS  false             // [SetOption13] Enable only single press to speed up button press recognition

#define MQTT_BUTTONS           false             // [SetOption73] Detach buttons from relays and send multi-press and hold MQTT messages instead

#define SWITCH_DEBOUNCE_TIME   50                // [SwitchDebounce] Number of mSeconds switch press debounce time
#define SWITCH_MODE            TOGGLE            // [SwitchMode] TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV, PUSHBUTTON_TOGGLE, TOGGLEMULTI, FOLLOWMULTI, FOLLOWMULTI_INV (the wall switch state)
#define MQTT_SWITCHES          false             // [SetOption114] Detach switches from relays and send MQTT messages instead

#define TEMP_CONVERSION        false             // [SetOption8] Return temperature in (false = Celsius or true = Fahrenheit)
#define PRESSURE_CONVERSION    false             // [SetOption24] Return pressure in (false = hPa or true = mmHg)
#define TEMP_RESOLUTION        1                 // [TempRes] Maximum number of decimals (0 - 3) showing sensor Temperature
#define HUMIDITY_RESOLUTION    1                 // [HumRes] Maximum number of decimals (0 - 3) showing sensor Humidity
#define PRESSURE_RESOLUTION    1                 // [PressRes] Maximum number of decimals (0 - 3) showing sensor Pressure
#define ENERGY_RESOLUTION      3                 // [EnergyRes] Maximum number of decimals (0 - 5) showing energy usage in kWh
#define CALC_RESOLUTION        3                 // [CalcRes] Maximum number of decimals (0 - 7) used in commands ADD, SUB, MULT and SCALE

#define APP_FLASH_CYCLE        false             // [SetOption12] Switch between dynamic or fixed slot flash save location
#define APP_NO_RELAY_SCAN      false             // [SetOption63] Don't scan relay power state at restart
#define APP_DISABLE_POWERCYCLE false             // [SetOption65] Disable fast power cycle detection for device reset
#define DEEPSLEEP_BOOTCOUNT    false             // [SetOption76] Enable incrementing bootcount when deepsleep is enabled

#define APP_INTERLOCK_MODE     false             // [Interlock] Relay interlock mode
#define APP_INTERLOCK_GROUP_1  0xFF              // [Interlock] Relay bitmask for interlock group 1 (0xFF if undef)
//#define APP_INTERLOCK_GROUP_2  0x00              // [Interlock] Relay bitmask for interlock group 2 (0x00 if undef)
//#define APP_INTERLOCK_GROUP_3  0x00              // [Interlock] Relay bitmask for interlock group 3 (0x00 if undef)
//#define APP_INTERLOCK_GROUP_4  0x00              // [Interlock] Relay bitmask for interlock group 4 (0x00 if undef)

// -- Lights --------------------------------------
#define WS2812_LEDS            30                // [Pixels] Number of WS2812 LEDs to start with (max is 512)
#define LIGHT_MODE             true              // [SetOption15] Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
#define LIGHT_CLOCK_DIRECTION  false             // [SetOption16] Switch WS2812 clock between clockwise or counter-clockwise
#define LIGHT_COLOR_RADIX      false             // [SetOption17] Switch between decimal or hexadecimal color output (false = hexadecimal, true = decimal)
#define LIGHT_PAIRS_CO2        false             // [SetOption18] Enable Pair light signal with CO2 sensor
#define LIGHT_POWER_CONTROL    false             // [SetOption20] Enable power control in relation to Dimmer/Color/Ct changes
#define LIGHT_CHANNEL_MODE     false             // [SetOption68] Enable multi-channels PWM instead of Color PWM
#define LIGHT_SLIDER_POWER     false             // [SetOption77] Do not power off if slider moved to far left
#define LIGHT_ALEXA_CT_RANGE   false             // [SetOption82] Reduced CT range for Alexa
#define LIGHT_PWM_CT_MODE      false             // [SetOption92] Set PWM Mode from regular PWM to ColorTemp control (Xiaomi Philips ...) a.k.a. module 48 mode
#define LIGHT_WHITE_BLEND_MODE false             // [SetOption105] White Blend Mode - used to be `RGBWWTable` last value `0`, now deprecated in favor of this option
#define LIGHT_VIRTUAL_CT       false             // [SetOption106] Virtual CT - Creates a virtual White ColorTemp for RGBW lights
#define LIGHT_VIRTUAL_CT_CW    false             // [SetOption107] Virtual CT Channel - signals whether the hardware white is cold CW (true) or warm WW (false)
#define LIGHT_VIRTUAL_CT_POINTS 3                // Number of reference points for Virtual CT (min 2, default 3)
#define USE_AC_ZERO_CROSS_DIMMER                 // Requires USE_COUNTER and USE_LIGHT

// -- Energy --------------------------------------
#define ENERGY_VOLTAGE_ALWAYS  false             // [SetOption21] Enable show voltage even if powered off
#define ENERGY_DDS2382_MODE    false             // [SetOption71] Enable DDS2382 different Modbus registers for Active Energy (#6531)
#define ENERGY_HARDWARE_TOTALS false             // [SetOption72] Enable hardware energy total counter as reference (#6561)

// -- Other Options -------------------------------
#define TIMERS_ENABLED         false             // [Timers] Enable Timers
#define RF_DATA_RADIX          false             // [SetOption28] RF receive data format (false = hexadecimal, true = decimal)
#define IR_DATA_RADIX          false             // [SetOption29] IR receive data format (false = hexadecimal, true = decimal)
#define TUYA_SETOPTION_20      false             // [SetOption54] Apply SetOption20 settings to Tuya device
#define TUYA_ALLOW_DIMMER_0    false             // [SetOption131] Allow save dimmer = 0 receved by MCU
#define TUYA_TEMP_SET_RES      1                 // [TuyaTempSetRes] Maximum number of decimals (0 - 3) showing sensor TemperatureSet
#define TUYA_SETOPTION_137     false             // [SetOption137] Avoid mqtt-publish of Tuya MCU heartbeat responses
#define IR_ADD_RAW_DATA        false             // [SetOption58] Add IR Raw data to JSON message
#define BUZZER_ENABLE          false             // [SetOption67] Enable buzzer when available
#define DS18X20_PULL_UP        false             // [SetOption74] Enable internal pullup for single DS18x20 sensor
#define COUNTER_RESET          false             // [SetOption79] Enable resetting of counters after telemetry was sent
#define SHUTTER_SUPPORT        false             // [SetOption80] Enable shutter support
#define PCF8574_INVERT_PORTS   false             // [SetOption81] Invert all ports on PCF8574 devices
#define ZIGBEE_FRIENDLY_NAMES  false             // [SetOption83] Enable Zigbee FriendlyNames instead of ShortAddresses when possible
#define ZIGBEE_DISTINCT_TOPICS false             // [SetOption89] Enable unique device topic based on Zigbee device ShortAddr
#define ZIGBEE_RMV_ZBRECEIVED  false             // [SetOption100] Remove ZbReceived form JSON message
#define ZIGBEE_INDEX_EP        false             // [SetOption101] Add the source endpoint as suffix to attributes, ex `Power3` instead of `Power` if sent from endpoint 3
#define ZIGBEE_TOPIC_FNAME     false             // [SetOption112] Enable friendly name in Zigbee topic (use with ZIGBEE_DISTINCT_TOPICS)

/*********************************************************************************************\
 * END OF SECTION 1
 *
 * SECTION 2
 * - Enable a feature by removing both // in front of it
 * - Disable a feature by preceding it with //
\*********************************************************************************************/

// -- Localization --------------------------------
  // If non selected the default en-GB will be used
//#define MY_LANGUAGE            af_AF           // Afrikaans in South Africa
//#define MY_LANGUAGE            bg_BG           // Bulgarian in Bulgaria
//#define MY_LANGUAGE            ca_AD           // Catalan in All catalan speaking countries ( Andorra )
//#define MY_LANGUAGE            cs_CZ           // Czech in Czech
//#define MY_LANGUAGE            de_DE           // German in Germany
//#define MY_LANGUAGE            el_GR           // Greek in Greece
//#define MY_LANGUAGE            en_GB           // English in Great Britain. Enabled by Default
//#define MY_LANGUAGE            es_ES           // Spanish in Spain
//#define MY_LANGUAGE            fr_FR           // French in France
//#define MY_LANGUAGE            fy_NL           // Frysk in Nederland
//#define MY_LANGUAGE            he_HE           // Hebrew in Israel
//#define MY_LANGUAGE            hu_HU           // Hungarian in Hungary
//#define MY_LANGUAGE            it_IT           // Italian in Italy
//#define MY_LANGUAGE            ko_KO           // Korean in Korea
//#define MY_LANGUAGE            lt_LT           // Lithuanian in Lithuania
//#define MY_LANGUAGE            nl_NL           // Dutch in the Nederland
//#define MY_LANGUAGE            pl_PL           // Polish in Poland
//#define MY_LANGUAGE            pt_BR           // Portuguese in Brazil
//#define MY_LANGUAGE            pt_PT           // Portuguese in Portugal
//#define MY_LANGUAGE            ro_RO           // Romanian in Romania
//#define MY_LANGUAGE            ru_RU           // Russian in Russia
//#define MY_LANGUAGE            sk_SK           // Slovak in Slovakia
//#define MY_LANGUAGE            sv_SE           // Swedish in Sweden
//#define MY_LANGUAGE            tr_TR           // Turkish in Turkey
//#define MY_LANGUAGE            uk_UA           // Ukrainian in Ukraine
//#define MY_LANGUAGE            vi_VN           // Vietnamese in Vietnam
//#define MY_LANGUAGE            zh_CN           // Chinese (Simplified) in China
//#define MY_LANGUAGE            zh_TW           // Chinese (Traditional) in Taiwan

//#define USE_HEAT_INDEX                           // Show calculated Heat index from temperature and humidity (+1k4 code)

// -- Wifi Config tools ---------------------------
#define WIFI_SOFT_AP_CHANNEL   1                 // Soft Access Point Channel number between 1 and 13 as used by Wi-Fi Manager web GUI
#define USE_IMPROV                               // Add support for IMPROV serial protocol as used by esp-web-tools (+2k code)

// -- IPv6 support -------------------------------
// #define USE_IPV6                                 // Enable IPv6 support (if the underlying esp-idf is also configured to support it)
                                                 // Code size increase: ESP8266: +34.5kb
                                                 // Enabled by default on ESP32 and variants

// -- ESP-NOW -------------------------------------
//#define USE_TASMESH                              // Enable Tasmota Mesh using ESP-NOW (+11k code)
//  #define USE_TASMESH_HEARTBEAT                  // If enabled, the broker will detect when nodes come online and offline and send Birth and LWT messages over MQTT correspondingly
//  #define TASMESH_OFFLINE_DELAY  3               // Maximum number of seconds since the last heartbeat before the broker considers a node to be offline
//#define USE_WIZMOTE                              // [SetOption164 1] Add support for WiZ Smart Remote (uses ESP-NOW) (+4k2 code)
//  #define WIZMOTE_CHANNEL      1                 // WiZ Smart Remote ESP-NOW channel if WiFi is disabled

// -- OTA -----------------------------------------
//#define USE_ARDUINO_OTA                          // Add optional support for Arduino OTA with ESP8266 (+13k code)

// -- Influxdb ------------------------------------
//#define USE_INFLUXDB                             // Enable influxdb support (+5k code)
//  #define INFLUXDB_STATE     0                   // [Ifx] Influxdb initially Off (0) or On (1)
//  #define INFLUXDB_VERSION   1                   // Version of Influxdb 1 or 2
//  #define INFLUXDB_HOST      "influxdb"          // [IfxHost] Influxdb hostname or IP address
//  #define INFLUXDB_PORT      8086                // [IfxPort] Influxdb port number
//  #define INFLUXDB_ORG       ""                  // [IfxUser, IfxOrg] Influxdb v1 username or v2 organisation
//  #define INFLUXDB_TOKEN     ""                  // [IfxPassword, IfxToken] Influxdb v1 password or v2 token
//  #define INFLUXDB_BUCKET    "db"                // [IfxDatabase, IfxBucket] Influxdb v1 database or v2 bucket
//  #define INFLUXDB_RP        ""                  // [IfxRP] Influxdb retention policy

// -- MQTT ----------------------------------------
#define MQTT_LWT_OFFLINE       "Offline"         // MQTT LWT offline topic message
#define MQTT_LWT_ONLINE        "Online"          // MQTT LWT online topic message

#define MQTT_CLEAN_SESSION     1                 // [SetOption140] Mqtt clean session connection (0 = No clean session, 1 = Clean session (default))
#define MQTT_DISABLE_SSERIALRECEIVED 0           // [SetOption147] 1 = Disable SSerialReceived/IrReceived mqtt messages, 0 = Enable these mqtt messages (default)
#define MQTT_DISABLE_MODBUSRECEIVED  0           // [SetOption158] 1 = Disable ModbusReceived mqtt messages, 0 = Enable ModbusReceived mqtt messages (default)

// -- MQTT - Domoticz -----------------------------
#define USE_DOMOTICZ                             // Enable Domoticz (+6k code, +0.3k mem)
  #define DOMOTICZ_IN_TOPIC    "domoticz/in"     // Domoticz Input Topic
  #define DOMOTICZ_OUT_TOPIC   "domoticz/out"    // Domoticz Output Topic

// -- MQTT - Home Assistant Discovery -------------
//#define USE_HOME_ASSISTANT                                   // Enable Home Assistant Discovery Support (+12k code, +6 bytes mem)
//  #define HOME_ASSISTANT_DISCOVERY_PREFIX   "homeassistant"  // Home Assistant discovery prefix
//  #define HOME_ASSISTANT_LWT_TOPIC   "homeassistant/status"  // home Assistant Birth and Last Will Topic (default = homeassistant/status)
//  #define HOME_ASSISTANT_LWT_SUBSCRIBE    true               // Subscribe to Home Assistant Birth and Last Will Topic (default = true)

// -- MQTT - Tasmota Discovery ---------------------
#define USE_TASMOTA_DISCOVERY                      // Enable Tasmota Discovery support (+2k code)

// -- MQTT - TLS - AWS IoT ------------------------
// Using TLS starting with version v6.5.0.16 compilation will only work using Core 2.4.2 and 2.5.2. No longer supported: 2.3.0
//#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
//  #define USE_MQTT_TLS_CA_CERT                   // [DEPRECATED] Now TLS supports dual mode using SetOption132 - this flag is now ignored
//  #define USE_MQTT_AWS_IOT_LIGHT                 // Enable MQTT for AWS IoT in light mode, with user/password instead of private certificate
//  #define USE_MQTT_CLIENT_CERT                   // Enable MQTT with custom client certificate - requires a private key (+11.9k code, +0.4k mem)
//  #define USE_MQTT_AWS_IOT                       // [Deprecated] Enable MQTT for AWS IoT - it includes USE_MQTT_CLIENT_CERT but it forces no user account/password
                                                   //   Note: you need to generate a private key + certificate per device and update 'tasmota/tasmota_aws_iot.cpp'
                                                   //   Full documentation here: https://github.com/arendst/Tasmota/wiki/AWS-IoT
//  for USE_4K_RSA (support for 4096 bits certificates, instead of 2048), you need to uncommend `-DUSE_4K_RSA` in `build_flags` from `platform.ini` or `platform_override.ini`

// -- MQTT - TLS - Azure IoT & IoT Central ---------
// Starting with version v9.4.0.3 added support for both Azure IoT Hub and IoT Central
//#define USE_MQTT_TLS                             // REQUIRED Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
//  #define USE_MQTT_AZURE_IOT                     // REQUIRED Enable accesss to IoT Hub without DPS using a preshared key: https://tasmota.github.io/docs/Azure-IoT-Hub/  Enable MQTT for Azure IoT Hub (+1k code)
//  #define USE_MQTT_AZURE_DPS_SCOPEID             // OPTIONAL Enables Azure Device Provisioning Service (DPS) for provision at scale, REQUIRED for IoT Central.  Uses the REST over HTTPS protocol (+4k memory)
//  #define USE_MQTT_AZURE_DPS_PRESHAREDKEY        // OPTIONAL The Preshared Key of DPS https://github.com/tasmota/docs/blob/development/docs/Azure-IoT-Central.md
//  #define USE_MQTT_AZURE_DPS_SCOPE_ENDPOINT      // OPTIONAL Defaults to "https://global.azure-devices-provisioning.net/", can be changed for Azure China, Azure Germany or others.

// -- Wireguard VPN support ------------------------
// VPN support since v14.6.1
// #define USE_WIREGUARD                            // Enable Wireguard VPN support (ESP8266: +28k code +0.9k mem, ESP32: +22k code +0.9k mem)

// -- Telegram Protocol ---------------------------
//#define USE_TELEGRAM                             // Support for Telegram protocol (+49k code, +7.0k mem and +4.8k additional during connection handshake)
  #define USE_TELEGRAM_FINGERPRINT "\x4E\x7F\xF5\x6D\x1E\x29\x40\x58\xAB\x84\xDE\x63\x69\x7B\xCD\xDF\x44\x2E\xD2\xF6" // Telegram api.telegram.org TLS public key fingerpring

// -- KNX IP Protocol -----------------------------
//#define USE_KNX                                  // Enable KNX IP Protocol Support (+9.4k code, +3k7 mem)
  #define USE_KNX_WEB_MENU                       // Enable KNX WEB MENU (+8.3k code, +144 mem)

// -- Telnet --------------------------------------
//#define USE_TELNET                               // Add support for telnet (+2k code)
//  #define TELNET_BUF_SIZE        256             // [TelnetBuffer] Size of input buffer (default 256)
//  #define TELNET_START           1               // [Telnet] Start telnet on network connection (default 0 - No start)
//  #define TELNET_PORT            23              // [Telnet] Telnet port (default 23)
//  #define TELNET_COL_DISABLE     1               // [TelnetColor] Disable colors (default 0 - Enable colors)
//  #define TELNET_COL_PROMPT      33              // [TelnetColor] ANSI color escape code (default 33 - Yellow)
//  #define TELNET_COL_RESPONSE    32              // [TelnetColor] ANSI color escape code (default 32 - Green)
//  #define TELNET_COL_LOGGING     36              // [TelnetColor] ANSI color escape code (default 36 - Cyan)

//#define USE_XYZMODEM                             // Add support for XModem over serial and telnet (+5k code)

// -- HTTP ----------------------------------------
#define USE_WEBSERVER                            // Enable web server and Wi-Fi Manager (+66k code, +8k mem)
  #define WEB_PORT             80                // Web server Port for User and Admin mode
  #define WEB_USERNAME         "admin"           // Web server Admin mode user name
//  #define DISABLE_REFERER_CHK                     // [SetOption128] Disable HTTP API
  #define USE_ENHANCED_GUI_WIFI_SCAN             // Enable Wi-Fi scan output with BSSID (+0k5 code)
  #define USE_WEB_STATUS_LINE                      // Enable upper status line in web UI (+0k5 code)
//    #define USE_WEB_STATUS_LINE_WIFI               // Enable upper left wifi indicator in main page (+0k5 code)
//    #define USE_WEB_STATUS_LINE_HEAP               // Enable upper left display of free heap memory (+0k1 code)
//  #define USE_ALPINEJS                           // Enable AlpineJS v2.8.2 (+8k8 code)
//  #define USE_WEBSEND_RESPONSE                   // Enable command WebSend response message (+1k code)
//  #define USE_WEBGETCONFIG                       // Enable restoring config from external webserver (+0k6)
//  #define USE_WEBRUN                             // Enable executing a tasmota command file from external web server (+0.4 code)
//  #define USE_GPIO_VIEWER                        // Enable GPIO Viewer to see realtime GPIO states (+6k code)
//    #define GV_SAMPLING_INTERVAL  100            // [GvSampling] milliseconds - Use Tasmota Scheduler (100) or Ticker (20..99,101..1000)
  #define USE_EMULATION_HUE                      // Enable Hue Bridge emulation for Alexa (+14k code, +2k mem common)
  #define USE_EMULATION_WEMO                     // Enable Belkin WeMo emulation for Alexa (+6k code, +2k mem common)
  // #define USE_CCLOADER                           // Enable CCLoader FW upgrade tool (for CC25xx devices)

// -- mDNS ----------------------------------------
//#define USE_DISCOVERY                            // Enable mDNS for the following services (+8k code or +23.5k code with core 2_5_x, +0.3k mem)
  #define WEBSERVER_ADVERTISE                    // Provide access to webserver by name <Hostname>.local/
  // #define MQTT_HOST_DISCOVERY                    // Find MQTT host server (overrides MQTT_HOST if found) - disabled by default because it causes blocked repeated 3000ms pauses

// -- Time ----------------------------------------
#define USE_TIMERS                               // Add support for up to 16 timers (+2k2 code)
  #define USE_TIMERS_WEB                         // Add timer webpage support (+4k5 code)
  #define USE_SUNRISE                            // Add support for Sunrise and sunset tools (+16k)
    #define SUNRISE_DAWN_ANGLE DAWN_NORMAL       // Select desired Dawn Angle from (DAWN_NORMAL, DAWN_CIVIL, DAWN_NAUTIC, DAWN_ASTRONOMIC)

// -- Ping ----------------------------------------
//  #define USE_PING                                 // Enable Ping command (+2k code)

// -- Compression ---------------------------------
#define USE_UNISHOX_COMPRESSION                  // Add support for string compression in Rules or Scripts

// -- Rules or Script  ----------------------------
// Select none or only one of the below defines USE_RULES or USE_SCRIPT
#define USE_RULES                                // Add support for rules (+13k code, +768 bytes mem)
  #define SUPPORT_MQTT_EVENT                     // Support trigger event with MQTT subscriptions (+1k8 code)
  #define USE_EXPRESSION                         // Add support for expression evaluation in rules (+1k7 code)
    #define SUPPORT_IF_STATEMENT                 // Add support for IF statement in rules (+2k7)
//  #define USER_RULE1 "<Any rule1 data>"          // Add rule1 data saved at initial firmware load or when command reset is executed
//  #define USER_RULE2 "<Any rule2 data>"          // Add rule2 data saved at initial firmware load or when command reset is executed
//  #define USER_RULE3 "<Any rule3 data>"          // Add rule3 data saved at initial firmware load or when command reset is executed

//#define USE_SCRIPT                               // Add support for script (+36k code, +1k mem)
//  #define USE_SCRIPT_FATFS 4                     // Script: Add FAT FileSystem Support
//  #define SUPPORT_MQTT_EVENT                     // Support trigger event with MQTT subscriptions (+3k5 code)

//#define USER_BACKLOG "<Any command separated by a semicolon (;)>"  // Add commands executed at firmware load or when command reset is executed

// -- Optional modules ----------------------------
#define ROTARY_V1                                // Add support for Rotary Encoder as used in MI Desk Lamp (+0k8 code)
  #define ROTARY_MAX_STEPS     10                // Rotary step boundary
#define USE_SONOFF_RF                            // Add support for Sonoff Rf Bridge (+3k2 code)
//  #define FIX_JSON_HEXADECIMAL                   // Add "0x" as prefix to hexadecimal value (disabled for legacy)
  #define USE_RF_FLASH                           // Add support for flashing the EFM8BB1 chip on the Sonoff RF Bridge. C2CK must be connected to GPIO4, C2D to GPIO5 on the PCB (+2k7 code)
#define USE_SONOFF_SC                            // Add support for Sonoff Sc (+1k1 code)
#define USE_TUYA_MCU                             // Add support for Tuya Serial MCU
  #define TUYA_DIMMER_ID       0                 // Default dimmer Id
  #define USE_TUYA_TIME                          // Add support for Set Time in Tuya MCU
//#define USE_TUYAMCUBR                            // Add support for TuyaMCU Bridge
#define USE_ARMTRONIX_DIMMERS                    // Add support for Armtronix Dimmers (+1k4 code)
#define USE_PS_16_DZ                             // Add support for PS-16-DZ Dimmer (+2k code)
#define USE_SONOFF_IFAN                          // Add support for Sonoff iFan02 and iFan03 (+2k code)
#define USE_BUZZER                               // Add support for a buzzer (+0k6 code)
#define USE_ARILUX_RF                            // Add support for Arilux RF remote controller (+0k8 code, 252 iram (non 2.3.0))
#define USE_SHUTTER                              // Add Shutter support for up to 4 shutter with different motortypes (+11k code)
#define USE_DEEPSLEEP                            // Add support for deepsleep (+1k code)
#define USE_EXS_DIMMER                           // Add support for ES-Store Wi-Fi Dimmer (+1k5 code)
//  #define EXS_MCU_CMNDS                          // Add command to send MCU commands (+0k8 code)
//#define USE_HOTPLUG                              // Add support for sensor HotPlug
#define USE_DEVICE_GROUPS                        // Add support for device groups (+5k5 code)
  #define DEVICE_GROUPS_ADDRESS 239,255,250,250  // Device groups multicast address
  #define DEVICE_GROUPS_PORT 4447                // Device groups multicast port
  #define USE_DEVICE_GROUPS_SEND                 // Add support for the DevGroupSend command (+0k6 code)
#define USE_PWM_DIMMER                           // Add support for MJ-SD01/acenx/NTONPOWER PWM dimmers (+2k3 code, DGR=0k7)
  #define USE_PWM_DIMMER_REMOTE                  // Add support for remote switches to PWM Dimmer (requires USE_DEVICE_GROUPS) (+0k6 code)
//#define USE_KEELOQ                               // Add support for Jarolift rollers by Keeloq algorithm (+4k5 code)
#define USE_SONOFF_D1                            // Add support for Sonoff D1 Dimmer (+0k7 code)
#define USE_SHELLY_DIMMER                        // Add support for Shelly Dimmer (+3k code)
  #define SHELLY_CMDS                            // Add command to send co-processor commands (+0k3 code)
  #define SHELLY_FW_UPGRADE                      // Add firmware upgrade option for co-processor (+3k4 code)
//  #define SHELLY_VOLTAGE_MON                     // Add support for reading voltage and current measurment (-0k0 code)
//#define USE_MAGIC_SWITCH                         // Add Sonoff MagicSwitch support as implemented in Sonoff Basic R4 (+612B flash, +64B IRAM for intr)
//  #define MAGICSWITCH_MIN_PULSE  4000            // Overridable minimum pulse, also overridable by command MagicSwitchPulse (not saved to flash)
//  #define MAGICSWITCH_MASKING_WINDOW_LEN  5      // Overridable masking window (in number of 50ms loops)

// -- Optional light modules ----------------------
#define USE_LIGHT                                // Add support for light control
#define USE_MY92X1                               // Add support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#define USE_SM16716                              // Add support for SM16716 RGB LED controller (+0k7 code)
#define USE_SM2135                               // Add support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#define USE_SM2335                               // Add support for SM2335 RGBCW led control as used in SwitchBot Color Bulb (+0k7 code)
#define USE_BP1658CJ                             // Add support for BP1658CJ RGBCW led control as used in Orein OS0100411267 Bulb
#define USE_BP5758D                              // Add support for BP5758D RGBCW led control as used in some Tuya lightbulbs (+0k8 code)
#define USE_SONOFF_L1                            // Add support for Sonoff L1 led control
#define USE_ELECTRIQ_MOODL                       // Add support for ElectriQ iQ-wifiMOODL RGBW LED controller (+0k3 code)
#define USE_LIGHT_PALETTE                        // Add support for color palette (+0k7 code)
#define USE_LIGHT_VIRTUAL_CT                     // Add support for Virtual White Color Temperature (+1.1k code)
#define USE_DGR_LIGHT_SEQUENCE                   // Add support for device group light sequencing (requires USE_DEVICE_GROUPS) (+0k2 code)
//#define USE_LSC_MCSL                             // Add support for GPE Multi color smart light as sold by Action in the Netherlands (+1k1 code)

// -- Optional adressable leds ----------------------
#define USE_WS2812                               // WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
// -------- below is for ESP8266 only
//  #define USE_WS2812_DMA                         // ESP8266 only, DMA supports only GPIO03 (= Serial RXD) (+1k mem). When USE_WS2812_DMA is enabled expect Exceptions on Pow
  #define USE_WS2812_RMT  0                      // ESP32 only, hardware RMT support (default). Specify the RMT channel 0..7. This should be preferred to software bit bang.
//  #define USE_WS2812_I2S  0                      // ESP32 only, hardware I2S support. Specify the I2S channel 0..2. This is exclusive from RMT. By default, prefer RMT support
//  #define USE_WS2812_INVERTED                    // Use inverted data signal
  #define USE_WS2812_HARDWARE  NEO_HW_WS2812     // Hardware type (NEO_HW_WS2812, NEO_HW_WS2812X, NEO_HW_WS2813, NEO_HW_SK6812, NEO_HW_LC8812, NEO_HW_APA106, NEO_HW_P9813)
  #define USE_WS2812_CTYPE     NEO_GRB           // Color type (NEO_RGB, NEO_GRB, NEO_BRG, NEO_RBG, NEO_RGBW, NEO_GRBW)
// -------- below if for ESP32x only -- ESP32 uses a lightweight library instead of NeoPixelBus
  // #define USE_WS2812_FORCE_NEOPIXELBUS           // this option forces to use NeoPixelBus (like ESP866), which disables Berry support and limits features -- DO NOT USE unless you have a good reason

// #define USE_LIGHT_ARTNET                         // Add support for DMX/ArtNet via UDP on port 6454 (+3.5k code)
  #define USE_LIGHT_ARTNET_MCAST 239,255,25,54   // Multicast address used to listen: 239.255.25.54

  #define USE_BERRY_ANIMATE                      // Legacy tentative for LED animation framework, DEPRECATED
  // #define USE_BERRY_ANIMATION                    // New animation framework with dedicated language (ESP32x only, experimental, 94k not yet optimized)
    // #define USE_BERRY_ANIMATION_DSL                // DSL transpiler for new animation framework (not mandatory if DSL is transpiled externally, +98k not optimized yet)

// -- Counter input -------------------------------
#define USE_COUNTER                              // Enable inputs as counter (+0k8 code)

// -- Internal Analog input -----------------------
//#define USE_ADC_VCC                              // Display Vcc in Power status. Disable for use as Analog input on selected devices

// -- One wire sensors ----------------------------
#define USE_DS18x20                              // Add support for DS18x20 sensors with id sort, single scan and read retry (+2k6 code)
//  #define W1_PARASITE_POWER                      // Optimize for parasite powered sensors
//  #define DS18x20_USE_ID_AS_NAME                 // Use last 3 bytes for naming of sensors
//  #define DS18x20_USE_ID_ALIAS                   // Add support aliasing for DS18x20 sensors. See comments in xsns_05 files (+0k5 code)

// -- I2C sensors ---------------------------------
#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)
#define I2CDRIVERS_0_31        0xFFFFFFFF          // Enable I2CDriver0  to I2CDriver31
#define I2CDRIVERS_32_63       0xFFFFFFFF          // Enable I2CDriver32 to I2CDriver63
#define I2CDRIVERS_64_95       0xFFFFFFFF          // Enable I2CDriver64 to I2CDriver95

#ifdef USE_I2C
//  #define USE_I2C_BUS2                           // Add experimental support for second I2C bus on ESP8266 (+0k6k code)
//  #define USE_SHT                                // [I2cDriver8] Enable SHT1X sensor (+1k4 code)
//  #define USE_HTU                                // [I2cDriver9] Enable HTU21/SI7013/SI7020/SI7021 sensor (I2C address 0x40) (+1k5 code)
//  #define USE_BMP                                // [I2cDriver10] Enable BMP085/BMP180/BMP280/BME280 sensors (I2C addresses 0x76 and 0x77) (+4k4 code)
//    #define USE_BME68X                           // Enable support for BME680/BME688 sensor using Bosch BME68x library (+6k9 code)
//  #define USE_BH1750                             // [I2cDriver11] Enable BH1750 sensor (I2C address 0x23 or 0x5C) (+0k5 code)
//  #define USE_VEML6070                           // [I2cDriver12] Enable VEML6070 sensor (I2C addresses 0x38 and 0x39) (+1k5 code)
    #define USE_VEML6070_RSET    270000          // VEML6070, Rset in Ohm used on PCB board, default 270K = 270000ohm, range for this sensor: 220K ... 1Meg
    #define USE_VEML6070_SHOW_RAW                // VEML6070, shows the raw value of UV-A
//  #define USE_ADS1115                            // [I2cDriver13] Enable ADS1115 16 bit A/D converter (I2C address 0x48, 0x49, 0x4A or 0x4B) based on Adafruit ADS1x15 library (no library needed) (+0k7 code)
//  #define USE_INA219                             // [I2cDriver14] Enable INA219 (I2C address 0x40, 0x41 0x44 or 0x45) Low voltage and current sensor (+1k code)
  //  #define INA219_SHUNT_RESISTOR (0.100)        // 0.1 Ohm default shunt resistor, can be overriden in user_config_override or using Sensor13
//  #define USE_INA226                             // [I2cDriver35] Enable INA226 (I2C address 0x40, 0x41 0x44 or 0x45) Low voltage and current sensor (+2k3 code)
//  #define USE_SHT3X                              // [I2cDriver15] Enable SHT3x (I2C address 0x44 or 0x45) or SHTC3 (I2C address 0x70) sensor (+0k7 code)
//  #define USE_TSL2561                            // [I2cDriver16] Enable TSL2561 sensor (I2C address 0x29, 0x39 or 0x49) using library Joba_Tsl2561 (+2k3 code)
//  #define USE_TSL2591                            // [I2cDriver40] Enable TSL2591 sensor (I2C address 0x29) using library Adafruit_TSL2591 (+1k6 code)
//  #define USE_MGS                                // [I2cDriver17] Enable Xadow and Grove Mutichannel Gas sensor using library Multichannel_Gas_Sensor (+10k code)
    #define MGS_SENSOR_ADDR    0x04              // Default Mutichannel Gas sensor i2c address
//  #define USE_SGP30                              // [I2cDriver18] Enable SGP30 sensor (I2C address 0x58) (+1k1 code)
//  #define USE_SGP40                              // [I2cDriver69] Enable SGP40 sensor (I2C address 0x59) (+1k4 code)
//  #define USE_SGP4X                              // [I2cDriver82] Enable SGP41 sensor (I2C address 0x59) (+7k2 code)
//  #define USE_SEN5X                              // [I2cDriver76] Enable SEN5X sensor (I2C address 0x69) (+3k code)
//  #define USE_SI1145                             // [I2cDriver19] Enable SI1145/46/47 sensor (I2C address 0x60) (+1k code)
//  #define USE_LM75AD                             // [I2cDriver20] Enable LM75AD sensor (I2C addresses 0x48 - 0x4F) (+0k5 code)
//  #define USE_APDS9960                           // [I2cDriver21] Enable APDS9960 Proximity Sensor (I2C address 0x39). Disables SHT and VEML6070 (+4k7 code)
    #define USE_APDS9960_GESTURE                   // Enable APDS9960 Gesture feature (+2k code)
    #define USE_APDS9960_PROXIMITY                 // Enable APDS9960 Proximity feature (>50 code)
    #define USE_APDS9960_COLOR                     // Enable APDS9960 Color feature (+0.8k code)
    #define USE_APDS9960_STARTMODE  0              // Default to enable Gesture mode
//  #define USE_MCP230xx                           // [I2cDriver22] Enable MCP23008/MCP23017 - Must define I2C Address in #define USE_MCP230xx_ADDR below - range 0x20 - 0x27 (+5k1 code)
//    #define USE_MCP230xx_ADDR 0x20               // Enable MCP23008/MCP23017 I2C Address to use (Must be within range 0x20 through 0x26 - set according to your wired setup)
//    #define USE_MCP230xx_OUTPUT                  // Enable MCP23008/MCP23017 OUTPUT support through sensor29 commands (+2k2 code)
//    #define USE_MCP230xx_DISPLAYOUTPUT           // Enable MCP23008/MCP23017 to display state of OUTPUT pins on Web UI (+0k2 code)
//  #define USE_MCP23XXX_DRV                       // [I2cDriver77] Enable MCP23xxx support as virtual switch/button/relay (+3k(I2C)/+5k(SPI) code)
//  #define USE_PCA9685                            // [I2cDriver1] Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+1k4 code)
//  #define USE_PCA9685_V2                         // [I2cDriver1] Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+3k4 code)
//    #define USE_PCA9685_ADDR 0x40                // Enable PCA9685 I2C Address to use (Must be within range 0x40 through 0x47 - set according to your wired setup)
//    #define USE_PCA9685_FREQ 50                  // Define default PWM frequency in Hz to be used (must be within 24 to 1526) - If other value is used, it will rever to 50Hz
//  #define USE_PCA9632                            // [I2cDriver75] Enable PCA9632 I2C HW PWM Driver (+1k8 code)
//    #define USE_PCA9632_ADDR 0x60                // Define PCA9685 I2C Address to use (Must be within range 0x60 through 0x63 - set according to your wired setup)
//    #define USE_PCA9632_CM_0 0                   // Mapping for channel 0
//    #define USE_PCA9632_CM_1 1                   // Mapping for channel 1
//    #define USE_PCA9632_CM_2 2                   // Mapping for channel 2
//    #define USE_PCA9632_CM_3 3                   // Mapping for channel 3
//  #define USE_MPR121                             // [I2cDriver23] Enable MPR121 controller (I2C addresses 0x5A, 0x5B, 0x5C and 0x5D) in input mode for touch buttons (+1k3 code)
//  #define USE_CCS811                             // [I2cDriver24] Enable CCS811 sensor (I2C address 0x5A) (+2k2 code)
//  #define USE_CCS811_V2                          // [I2cDriver24] Enable CCS811 sensor (I2C addresses 0x5A and 0x5B) (+2k8 code)
//  #define USE_ENS16x                             // [I2cDriver85] Enable ENS160 and ENS161 sensor (I2C addresses 0x52 and 0x53) (+1.9kB of code and 12B of RAM)
//  #define USE_ENS210                             // [I2cDriver86] Enable ENS210 sensor (I2C addresses 0x43) (+1.7kB of code and 12B of RAM)
//  #define USE_MPU6050                            // [I2cDriver25] Enable MPU6050 sensor (I2C address 0x68 AD0 low or 0x69 AD0 high) (+3K3 of code and 188 Bytes of RAM)
//    #define USE_MPU6050_DMP                      // Enable in MPU6050 to use the DMP on the chip, should create better results (+8k6 of code)
//  #define USE_MGC3130                            // [I2cDriver27] Enable MGC3130 Electric Field Effect Sensor (I2C address 0x42) (+2k7 code, 0k3 mem)
//  #define USE_MAX44009                           // [I2cDriver28] Enable MAX44009 Ambient Light sensor (I2C addresses 0x4A and 0x4B) (+0k8 code)
//  #define USE_SCD30                              // [I2cDriver29] Enable Sensiron SCd30 CO2 sensor (I2C address 0x61) (+3k3 code)
//  #define USE_SCD40                              // [I2cDriver62] Enable Sensiron SCd40/Scd41 CO2 sensor (I2C address 0x62) (+3k5 code)
//  #define USE_SPS30                              // [I2cDriver30] Enable Sensiron SPS30 particle sensor (I2C address 0x69) (+1.7 code)
//  #define USE_ADE7880                            // [I2cDriver65] Enable ADE7880 Energy monitor as used on Shelly 3EM (I2C address 0x38) (+3k8)
  #define USE_ADE7953                            // [I2cDriver7] Enable ADE7953 Energy monitor as used on Shelly 2.5 (I2C address 0x38) (+1k5)
//  #define USE_VL53L0X                            // [I2cDriver31] Enable VL53L0x time of flight sensor (I2C address 0x29) (+4k code)
//    #define VL53L0X_XSHUT_ADDRESS 0x78           //   VL53L0X base address when used with XSHUT control
//  #define USE_VL53L1X                            // [I2cDriver54] Enable VL53L1X time of flight sensor (I2C address 0x29) using Pololu VL53L1X library (+2k9 code)
//    #define VL53L1X_XSHUT_ADDRESS 0x78           //   VL53L1X base address when used with XSHUT control
//    #define VL53L1X_DISTANCE_MODE Long           //   VL53L1X distance mode : Long | Medium | Short
//  #define USE_TOF10120                           // [I2cDriver57] Enable TOF10120 time of flight sensor (I2C address 0x52) (+0k6 code)
//  #define USE_MLX90614                           // [I2cDriver32] Enable MLX90614 ir temp sensor (I2C address 0x5a) (+0.6k code)
//  #define USE_CHIRP                              // [I2cDriver33] Enable CHIRP soil moisture sensor (variable I2C address, default 0x20)
//  #define USE_PAJ7620                            // [I2cDriver34] Enable PAJ7620 gesture sensor (I2C address 0x73) (+2.5k code)
//  #define USE_PCF8574                            // [I2cDriver2] Enable PCF8574 I/O Expander (I2C addresses 0x20 - 0x26 and 0x39 - 0x3F) (+2k1 code)
//    #define USE_PCF8574_MODE2                    // Enable Mode2 virtual relays/buttons/switches (+2k3 code)
//    #define USE_PCF8574_SENSOR                   // Enable Mode1 inputs and outputs in SENSOR message (+0k2 code)
//    #define USE_PCF8574_DISPLAYINPUT             // Enable Mode1 inputs display in Web page (+0k2 code)
//    #define USE_PCF8574_MQTTINPUT                // Enable Mode1 MQTT message & rule process on input change detection : stat/%topic%/PCF8574_INP = {"Time":"2021-03-07T16:19:23+01:00","PCF8574-1_INP":{"D1":1}} (+0k5 code)
//    #define PCF8574_ADDR1 0x20                   // First address to search for PCF8574
//    #define PCF8574_ADDR1_COUNT 7                // Number of addresses to search for PCF8574 - Default to 0x20 to 0x26
//    #define PCF8574_ADDR2 0x39                   // First address to search for PCF8574A
//    #define PCF8574_ADDR2_COUNT 6                // Number of addresses to search for PCF8574A - Default to 0x39 to 0x3E
//  #define USE_HIH6                               // [I2cDriver36] Enable Honeywell HIH Humidity and Temperature sensor (I2C address 0x27) (+0k6)
//  #define USE_DHT12                              // [I2cDriver41] Enable DHT12 humidity and temperature sensor (I2C address 0x5C) (+0k7 code)
//  #define USE_DS1624                             // [I2cDriver42] Enable DS1624, DS1621 temperature sensor (I2C addresses 0x48 - 0x4F) (+1k2 code)
//  #define USE_AHT1x                              // [I2cDriver43] Enable AHT10/15 humidity and temperature sensor (I2C address 0x38, 0x39) (+0k8 code)
//  #define USE_AHT2x                              // [I2cDriver43] Enable AHT20/AM2301B instead of AHT1x humidity and temperature sensor (I2C address 0x38) (+0k8 code)
//  #define USE_AHT3x                              // [I2cDriver43] Enable AHT30 humidity and temperature sensor (I2C address 0x38) (+0k8 code)
//  #define USE_WEMOS_MOTOR_V1                     // [I2cDriver44] Enable Wemos motor driver V1 (I2C addresses 0x2D - 0x30) (+0k7 code)
//    #define WEMOS_MOTOR_V1_ADDR  0x30            // Default I2C address 0x30
//    #define WEMOS_MOTOR_V1_FREQ  1000            // Default frequency
//  #define USE_HDC1080                            // [I2cDriver45] Enable HDC1080 temperature/humidity sensor (I2C address 0x40) (+1k5 code)
//  #define USE_IAQ                                // [I2cDriver46] Enable iAQ-core air quality sensor (I2C address 0x5a) (+0k6 code)
//  #define USE_AS3935                             // [I2cDriver48] Enable AS3935 Franklin Lightning Sensor (I2C address 0x03) (+5k4 code)
//  #define USE_VEML6075                           // [I2cDriver49] Enable VEML6075 UVA/UVB/UVINDEX Sensor (I2C address 0x10) (+2k1 code)
//  #define USE_VEML7700                           // [I2cDriver50] Enable VEML7700 Ambient Light sensor (I2C addresses 0x10) (+4k5 code)
//  #define USE_MCP9808                            // [I2cDriver51] Enable MCP9808 temperature sensor (I2C addresses 0x18 - 0x1F) (+0k9 code)
//  #define USE_HP303B                             // [I2cDriver52] Enable HP303B temperature and pressure sensor (I2C address 0x76 or 0x77) (+6k2 code)
//  #define USE_MLX90640                           // [I2cDriver53] Enable MLX90640 IR array temperature sensor (I2C address 0x33) (+20k code)
//  #define USE_EZOPH                              // [I2cDriver55] Enable support for EZO's pH sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_EZOORP                             // [I2cDriver55] Enable support for EZO's ORP sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_EZORTD                             // [I2cDriver55] Enable support for EZO's RTD sensor (+0k2 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_EZOHUM                             // [I2cDriver55] Enable support for EZO's HUM sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_EZOEC                              // [I2cDriver55] Enable support for EZO's EC sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_EZOCO2                             // [I2cDriver55] Enable support for EZO's CO2 sensor (+0k2 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_EZOO2                              // [I2cDriver55] Enable support for EZO's O2 sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_EZOPRS                             // [I2cDriver55] Enable support for EZO's PRS sensor (+0k7 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_EZOFLO                             // [I2cDriver55] Enable support for EZO's FLO sensor (+0k4 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_EZODO                              // [I2cDriver55] Enable support for EZO's DO sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_EZORGB                             // [I2cDriver55] Enable support for EZO's RGB sensor (+0k5 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_EZOPMP                             // [I2cDriver55] Enable support for EZO's PMP sensor (+0k3 code) - Shared EZO code required for any EZO device (+1k2 code)
//  #define USE_SEESAW_SOIL                        // [I2cDriver56] Enable Capacitice Soil Moisture & Temperature Sensor (I2C addresses 0x36 - 0x39) (+1k3 code)
//  #define USE_MPU_ACCEL                          // [I2cDriver58] Enable MPU6886/MPU9250 - found in M5Stack - support both I2C buses on ESP32 (I2C address 0x68) (+2k code)
//  #define USE_AM2320                             // [I2cDriver60] Enable AM2320 temperature and humidity Sensor (I2C address 0x5C) (+1k code)
//  #define USE_T67XX                              // [I2cDriver61] Enable Telaire T67XX CO2 sensor (I2C address 0x15) (+1k3 code)
//  #define USE_HM330X                             // [I2cDriver63] Enable support for SeedStudio Grove Particule sensor (I2C address 0x40) (+1k5 code)
//    #define HM330X_DEFAULT_ADDRESS    0x40       // Option: change default I2C address for HM330X used in SeedSTudio Particucle Sensor
//    #define HM330X_WARMUP_DELAY       30         // Option: change warmup delay during which data are not read from sensor after a power up
//    #define HM330X_HIDE_OUT_OF_DATE   false      // Option: change to true to hide data from web GUI and SENSOR while sensor is asleep
//  #define USE_HDC2010                            // [I2cDriver64] Enable HDC2010 temperature/humidity sensor (I2C address 0x40) (+1k5 code)
//  #define USE_DS3502                             // [I2CDriver67] Enable DS3502 digital potentiometer (I2C address 0x28 - 0x2B) (+0k4 code)
//  #define USE_HYT                                // [I2CDriver68] Enable HYTxxx temperature and humidity sensor (I2C address 0x28) (+0k5 code)
//  #define USE_LUXV30B                            // [I2CDriver70] Enable RFRobot SEN0390 LuxV30b ambient light sensor (I2C address 0x4A) (+0k5 code)
//  #define USE_QMC5883L                           // [I2CDriver71] Enable QMC5883L magnetic induction sensor (I2C address 0x0D) (+0k8 code)
//  #define USE_HMC5883L                           // [I2CDriver73] Enable HMC5883L magnetic induction sensor (I2C address 0x1E) (+1k3 code)
//    #define QMC5883L_TEMP_SHIFT       23         // sensor temperature are not calibrated (only relativ measurement) and need an absolute ground value in °C (see datasheet)
//    #define QMC5883L_OVERSAMPLE       0          // 0 .. 3 => 512(default), 256, 128, 64
//    #define QMC5883L_GAUSS            1          // 0,1(default) => 2GAUSS, 8GAUSS(default)
//    #define QMC5883L_FILTER           0          // 0 .. 3 => 10HZ(default), 50HZ, 100HZ, 200HZ
//  #define USE_INA3221                            // [I2CDriver72] Enable INA3221 3-channel DC voltage and current sensor (I2C address 0x40-0x44) (+3.2k code)
//    #define INA3221_ADDRESS1                     // allow to change the 1st address to search for INA3221 to 0x41..0x43
//    #define INA3221_MAX_COUNT                    // change the number of devices to search for (default 4).
//                                                 // Both settings together allow to limit searching for INA3221 to only a subset of addresses
//    #define  INA3221_CALC_CHARGE_AH              // calculate charge in Ah
//    #define  INA3221_CALC_ENERGY_WH              // calculate energy in Wh
//    #define  INA3221_SUPPLY_SIDE      0x7777     // the driver adds the measured Shunt Voltage to the Bus Voltage
                                                   // for the cannel with a negativ shunt (shunt <0) thus showing the values of the supply side (IN+)
                                                   // additionaly the bits set (bit 0,1,2) enable the scanning of the voltage in the according channel
//  #define USE_PMSA003I                           // [I2cDriver78] Enable PMSA003I Air Quality Sensor (I2C address 0x12) (+1k8 code)
//  #define USE_GDK101                             // [I2cDriver79] Enable GDK101 sensor (I2C addresses 0x18 - 0x1B) (+1k2 code)
//    #define GDK101_SHOW_FW_VERSION
//    #define GDK101_SHOW_STATUS
//    #define GDK101_SHOW_VIBRATION_STATUS
//    #define GDK101_SHOW_MEAS_TIME
//  #define USE_TC74                               // [I2cDriver80] Enable TC74 sensor (I2C addresses 0x48 - 0x4F) (+1k code)
//    #define TC74_MAX_SENSORS 8                   // Support non-default/multiple I2C addresses
//    #define TC74_I2C_PROBE_ADDRESSES { 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F }  // Addresses to probe/support
//    #define TC74_MAX_FAILCOUNT 8                 // Maximum failed polls before it's marked inactive until reprobing later
//  #define USE_PCA9557                            // [I2cDriver81] Enable PCA9557 8-bit I/O Expander (I2C addresses 0x18 - 0x1F) (+2k5 code)
//  #define USE_MAX17043                           // [I2cDriver83] Enable MAX17043 fuel-gauge systems Lipo batteries sensor (I2C address 0x36) (+0k9 code)
//  #define MAX17043_ALERT_THRESHOLD 32            // [I2cDriver83] Define the alert threshold for low battery level percentage 1-32
//  #define USE_AMSX915                            // [I2CDriver86] Enable AMS5915/AMS6915 pressure/temperature sensor (+1k2 code)
//  #define USE_SPL06_007                          // [I2cDriver87] Enable SPL06_007 pressure and temperature sensor (I2C addresses 0x76) (+2k5 code)
//  #define USE_QMP6988                            // [I2cDriver88] Enable QMP6988 pressure and temperature sensor (I2C address 0x56 or 0x70) (+2k9 code)
//  #define USE_MS5837                             // [I2cDriver91] Enable MS5837 sensor (I2C address 0x76) (+2k7 code)
//  #define USE_AP33772S                           // [I2cDriver93] Enable AP33772S USB PD Sink Controller (I2C addresses 0x52) (+3k1 code)

//  #define USE_RTC_CHIPS                          // Enable RTC chip support and NTP server - Select only one
//    #define USE_RV3028                           // [I2cDriver94] Enable RV3028 RTC chip support (I2C address 0x52)
//    #define USE_DS3231                           // [I2cDriver26] Enable DS3231 RTC - used by Ulanzi TC001 (I2C address 0x68) (+1k2 code)
//    #define DS3231_ENABLE_TEMP                   //   In DS3231 driver, enable the internal temperature sensor
//    #define USE_BM8563                           // [I2cDriver59] Enable BM8563 RTC - used by M5Stack - support both I2C buses on ESP32 (I2C address 0x51) (+2.5k code)
//    #define USE_PCF85363                         // [I2cDriver66] Enable PCF85363 RTC - used by Shelly 3EM (I2C address 0x51) (+0k7 code)
//    #define USE_RX8010                           // [I2cDriver90] Enable RX8010 RTC - used by IOTTIMER - support both I2C buses on ESP32 (I2C address 0x32) (+0k7 code)
//    #define USE_RX8030                           // [I2cDriver90] Enable RX8030 RTC - used by #23855 - support both I2C buses on ESP32 (I2C address 0x32) (+0k7 code)
//    #define USE_PCF85063                         // [I2cDriver92] Enable PCF85063 RTC support (I2C address 0x51)

//  #define USE_DISPLAY                            // Add I2C/TM1637/MAX7219 Display Support (+2k code)
    #define USE_DISPLAY_MODES1TO5                // Enable display mode 1 to 5 in addition to mode 0
    #define USE_DISPLAY_LCD                      // [DisplayModel 1] [I2cDriver3] Enable Lcd display (I2C addresses 0x27 and 0x3F) (+6k code)
    // REMOVED - #define USE_DISPLAY_SSD1306                  // [DisplayModel 2] [I2cDriver4] Enable SSD1306 Oled 128x64 display (I2C addresses 0x3C and 0x3D) (+16k code)
    #define USE_DISPLAY_MATRIX                   // [DisplayModel 3] [I2cDriver5] Enable 8x8 Matrix display (I2C adresseses see below) (+11k code)
      #define MTX_ADDRESS1     0x71              // [DisplayAddress1] I2C address of first 8x8 matrix module
      #define MTX_ADDRESS2     0x74              // [DisplayAddress2] I2C address of second 8x8 matrix module
      #define MTX_ADDRESS3     0x75              // [DisplayAddress3] I2C address of third 8x8 matrix module
      #define MTX_ADDRESS4     0x72              // [DisplayAddress4] I2C address of fourth 8x8 matrix module
      #define MTX_ADDRESS5     0x73              // [DisplayAddress5] I2C address of fifth 8x8 matrix module
      #define MTX_ADDRESS6     0x76              // [DisplayAddress6] I2C address of sixth 8x8 matrix module
      #define MTX_ADDRESS7     0x00              // [DisplayAddress7] I2C address of seventh 8x8 matrix module
      #define MTX_ADDRESS8     0x00              // [DisplayAddress8] I2C address of eigth 8x8 matrix module
    #define USE_DISPLAY_SEVENSEG                 // [DisplayModel 11] [I2cDriver47] Enable sevenseg display (I2C 0x70-0x77) (<+11k code)
//      #define USE_DISPLAY_SEVENSEG_COMMON_ANODE  // Enable support for common anode sevenseg displays
                                                 // Multiple sevenseg displays are logically arranged vertically with MTX_ADDRESS1 at y=0,
                                                 // MTX_ADDRESS2 at y=1, up to MTX_ADDRESS8 at y=7
                                                 // Command: DisplayText [yn]8888
                                                 // will display 8888 at sevenseg display at I2C address MTX_ADDRESS(n-1)
                                                 // Each segment may be address Command: DisplayText [xn]m
                                                 //  where n is 0..4 (4 digits and middle :) and m is decimal for bitmap of which segment to turn on.
                                                 // Reference: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-led-backpack.pdf
//      #define SEVENSEG_ADDRESS1     0x70         // No longer used.  Use MTX_ADDRESS1 - MTX_ADDRESS8 instead to specify I2C address of sevenseg displays
//    #define USE_DISPLAY_SH1106                   // [DisplayModel 7] [I2cDriver6] Enable SH1106 Oled 128x64 display (I2C addresses 0x3C and 0x3D)
//    #define USE_DISPLAY_TM1650                   // [DisplayModel 20] [I2cDriver74] Enable TM1650 display (I2C addresses 0x24 - 0x27 and 0x34 - 0x37)
//    #define USE_DT_VARS                          // Display variables that are exposed in JSON MQTT strings e.g. in TelePeriod messages.
//    #define MAX_DT_VARS     16                   // Defaults to 7
//    #define USE_GRAPH                            // Enable line charts with displays
//    #define NUM_GRAPHS     4                     // Max 16

#endif  // USE_I2C

//#define USE_DISPLAY                              // Add I2C/TM1637/MAX7219 Display Support (+2k code)
//  #define USE_DISPLAY_TM1637                     // [DisplayModel 15] Enable TM1637 Seven Segment Display Module (4-6 digits)
//  #define USE_DISPLAY_MAX7219                    // [DisplayModel 15] Enable MAX7219 Seven Segment Display Module (8 digits)
//  #define USE_DISPLAY_MAX7219_MATRIX             // [DisplayModel 19] Enable MAX7219 8x8 Matrix Display
//  #define USE_DISPLAY_TM1640                     // [DisplayModel 13] Enable TM1640 module  Seven Segment Display Module (stub)
//    #define USE_IOTTIMER                         // Enable TM1640 based IotTimer

// -- Universal Display Driver ---------------------------------
// #define USE_UNIVERSAL_DISPLAY                   // New universal display driver for both I2C and SPI
    #define MAX_TOUCH_BUTTONS 16                 // Virtual touch buttons

// -- SPI sensors ---------------------------------
//#define USE_SPI                                  // Hardware SPI using GPIO12(MISO), GPIO13(MOSI) and GPIO14(CLK) in addition to two user selectable GPIOs(CS and DC)

#ifdef USE_SPI
//  #define USE_NRF24                              // Add SPI support for NRF24L01(+) (+2k6 code)
    #define USE_MIBLE                            // BLE-bridge for some Mijia-BLE-sensors (+4k7 code)
//  #define USE_DISPLAY                            // Add SPI Display support for 320x240 and 480x320 TFT
    // REMOVED -- #define USE_DISPLAY_ILI9341                  // [DisplayModel 4] Enable ILI9341 Tft 480x320 display (+19k code)
//    #define USE_DISPLAY_EPAPER_29                // [DisplayModel 5] Enable e-paper 2.9 inch display (+19k code)
//    #define USE_DISPLAY_EPAPER_42                // [DisplayModel 6] Enable e-paper 4.2 inch display
    // REMOVED -- #define USE_DISPLAY_SSD1351                  // [DisplayModel 9] Enable SSD1351 module
//    #define USE_DISPLAY_RA8876                   // [DisplayModel 10] [I2cDriver39] (Touch)
//    #define USE_DISPLAY_ST7789                   // [DisplayModel 12] Enable ST7789 module
    // REMOVED -- #define USE_DISPLAY_SSD1331                  // [DisplayModel 14] Enable SSD1331 module
//  #define USE_RC522                              // Add support for MFRC522 13.56Mhz Rfid reader (+6k code)
//    #define USE_RC522_DATA_FUNCTION              // Add support for reading data block content (+0k4 code)
//    #define USE_RC522_TYPE_INFORMATION           // Add support for showing card type (+0k4 code)
//  #define USE_MCP2515                            // Add support for can bus using MCP2515 (+7k code)
//  #define USE_CANSNIFFER                         // Add support for can bus sniffer using MCP2515 (+5k code)
//  #define USE_SPI_LORA                           // Add support for LoRaSend and LoRaCommand (+4k code)
//    #define USE_LORA_SX126X                      // Add driver support for LoRa on SX126x based devices like LiliGo T3S3 Lora32 (+16k code)
//    #define USE_LORA_SX127X                      // Add driver support for LoRa on SX127x based devices like M5Stack LoRa868, RFM95W (+5k code)
//    #define USE_LORAWAN_BRIDGE                   // Add support for LoRaWan bridge (+8k code)

#endif  // USE_SPI

// -- One wire sensors ----------------------------
// #define USE_HDMI_CEC                              // Add support for HDMI CEC bus (+7k code, 1456 bytes IRAM)

// -- Serial sensors ------------------------------
//#define USE_MHZ19                                // Add support for MH-Z19 CO2 sensor (+2k code)
//#define USE_SENSEAIR                             // Add support for SenseAir K30, K70 and S8 CO2 sensor (+2k3 code)
//#define USE_CM110x                               // Add support for CM110x CO2 sensors (+2k7code)
  #define CO2_LOW              800               // Below this CO2 value show green light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
  #define CO2_HIGH             1200              // Above this CO2 value show red light (needs PWM or WS2812 RG(B) led and enable with SetOption18 1)
//#define USE_PMS5003                              // Add support for PMS5003 and PMS7003 particle concentration sensor (+1k3 code)
//  #define PMS_MODEL_PMS3003                      // Enable support of PMS3003 instead of PMS5003/PMS7003 (needs the USE_PMS5003 above)
//  #define PMS_MODEL_PMS5003T                     // Enable support for PMSx003T models that report temperature and humidity (needs the USE_PMS5003 above)
//#define USE_NOVA_SDS                             // Add support for SDS011 and SDS021 particle concentration sensor (+1k5 code)
  #define STARTING_OFFSET      30                // Turn on NovaSDS XX-seconds before tele_period is reached
//#define USE_HPMA                                 // Add support for Honeywell HPMA115S0 particle concentration sensor (+1k4)
//#define USE_SR04                                 // Add support for HC-SR04 ultrasonic devices (+1k code)
//  #define SR04_MAX_SENSOR_DISTANCE  500          // Set sensor max detection distance
//#define USE_ME007                                // Add support for ME007 ultrasonic devices (+1k5 code)
//  #define ME007_MAX_SENSOR_DISTANCE  800         // Set sensor max detection distance
//  #define ME007_ENABLE_MEDIAN_FILTER             // Enables that distance measurements are filtered with an median filter of length 5
//#define USE_DYP                                  // Add support for DYP ME-007 ultrasonic distance sensor, serial port version (+0k5 code)
#define USE_SERIAL_BRIDGE                        // Add support for software Serial Bridge (+2k code)
//  #define SERIAL_BRIDGE_BUFFER_SIZE 256          // Serial Bridge receive buffer size (Default ESP8266 = 256, ESP32 = 800)
// #define USE_MODBUS_BRIDGE                        // Add support for software Modbus Bridge (+4.5k code)
// #define USE_MODBUS_BRIDGE_TCP                    // Add support for software Modbus TCP Bridge (also enable Modbus TCP Bridge) (+2k code)
// #define MODBUS_BRIDGE_TCP_DEFAULT_PORT 502       // Add support for software Modbus TCP Bridge (start the TCP bridge automatically at PORT 502)
//#define USE_TCP_BRIDGE                           //  Add support for Serial to TCP bridge (+1.3k code)
//#define USE_MP3_PLAYER                           // Use of the DFPlayer Mini MP3 Player RB-DFR-562 commands: play, pause, stop, track, volume and reset
  #define MP3_VOLUME           30                // Set the startup volume on init, the range can be 0..100(max)
//  #define USE_DY_SV17F                           // Use of DY-SV17F MP3 Player commands: play, stop, track and volume
//#define USE_AZ7798                               // Add support for AZ-Instrument 7798 CO2 datalogger (+1k6 code)
//#define USE_PN532_HSU                            // Add support for PN532 using HSU (Serial) interface (+1k7 code, 156 bytes mem)
//  #define USE_PN532_DATA_FUNCTION                // Add sensor40 command support for erase, setting data block content (+3k code, 32 bytes mem)
//#define USE_RDM6300                              // Add support for RDM6300 125kHz RFID Reader (+0k8)
//#define USE_IBEACON                              // Add support for bluetooth LE passive scan of ibeacon devices (uses HM17 module)
//#define USE_GPS                                  // Add support for GPS and NTP Server for becoming Stratus 1 Time Source (+3k1 code, +132 bytes RAM)
//  #define USE_FLOG                               // Add support for GPS logging in OTA's Flash (Experimental) (+2k9 code, +8 bytes RAM)
//#define USE_HM10                                 // (ESP8266 only) Add support for HM-10 as a BLE-bridge (+17k code)
//#define USE_HRXL                                 // Add support for MaxBotix HRXL-MaxSonar ultrasonic range finders (+0k7)
//#define USE_TASMOTA_CLIENT                       // Add support for Arduino Uno/Pro Mini via serial interface including flashing (+2k6 code, 64 mem)
  #define USE_TASMOTA_CLIENT_FLASH_SPEED 57600   // Usually 57600 for 3.3V variants and 115200 for 5V variants
  #define USE_TASMOTA_CLIENT_SERIAL_SPEED 57600  // Depends on the sketch that is running on the Uno/Pro Mini
//#define USE_OPENTHERM                            // Add support for OpenTherm (+15k code)
//#define USE_MIEL_HVAC                            // Add support for Mitsubishi Electric HVAC serial interface (+5k code)
//#define USE_TUYAMCUBR                            // Add support for TuyaMCU Bridge
//#define USE_PROJECTOR_CTRL                       // Add support for LCD/DLP Projector serial control interface (+2k code)
//  #define USE_PROJECTOR_CTRL_NEC                 // Use codes for NEC
//  #define USE_PROJECTOR_CTRL_OPTOMA              // Use codes for OPTOMA
//#define USE_AS608                                // Add support for AS608 optical and R503 capacitive fingerprint sensor (+3k code)
//  #define USE_AS608_MESSAGES                     // Add verbose error messages (+0k4 code)
//#define USE_TFMINIPLUS                           // Add support for TFmini Plus (TFmini, TFmini-S) LiDAR modules via UART interface (+0k8)
//#define USE_HRG15                                // Add support for Hydreon RG-15 Solid State Rain sensor (+1k5 code)
//#define USE_VINDRIKTNING                         // Add support for IKEA VINDRIKTNING particle concentration sensor (+0k6 code)
//  #define VINDRIKTNING_SHOW_PM1                  // Display undocumented/supposed PM1.0 values
//  #define VINDRIKTNING_SHOW_PM10                 // Display undocumented/supposed PM10 values
//#define USE_LD2402                               // Add support for HLK-LD2402 24GHz human presence sensor module (+10k9 code, 320 RAM)
//#define USE_LD2410                               // Add support for HLK-LD2410 24GHz smart wave motion sensor (+3k7 code, 88 RAM)
//#define USE_LD2410S                              // Add support for HLK-LD2410S Ultra Low-power 24GHz smart wave motion sensor (+4k7 code, 144 RAM)
//#define USE_LOX_O2                               // Add support for LuminOx LOX O2 Sensor (+0k8 code)
//#define USE_GM861                                // Add support for GM861 1D and 2D Bar Code Reader (+1k3 code)
//  #define GM861_DECODE_AIM                       // Decode AIM-id (+0k3 code)
//  #define GM861_HEARTBEAT                        // Enable heartbeat (+0k2 code)
//#define USE_WOOLIIS                              // Add support for Wooliis Hall Effect Coulometer or Battery capacity monitor (+1k6 code)
//#define USE_DALI                                 // Add support for DALI gateway (+5k code)

// -- Power monitoring sensors --------------------
#define USE_ENERGY_SENSOR                        // Add support for Energy Monitors (+14k code)
#define USE_ENERGY_MARGIN_DETECTION              // Add support for Energy Margin detection (+1k6 code)
  #define USE_ENERGY_POWER_LIMIT                 // Add additional support for Energy Power Limit detection (+1k2 code)
#define USE_ENERGY_DUMMY                         // Add support for dummy Energy monitor allowing user values (+0k7 code)
#define USE_HLW8012                              // Add support for HLW8012, BL0937 or HJL-01 Energy Monitor for Sonoff Pow and WolfBlitz
#define USE_CSE7766                              // Add support for CSE7766 Energy Monitor for Sonoff S31 and Pow R2
#define USE_PZEM004T                             // Add support for PZEM004T Energy monitor (+2k code)
#define USE_PZEM_AC                              // Add support for PZEM014,016 Energy monitor (+1k1 code)
#define USE_PZEM_DC                              // Add support for PZEM003,017 Energy monitor (+1k1 code)
#define USE_MCP39F501                            // Add support for MCP39F501 Energy monitor as used in Shelly 2 (+3k1 code)
//#define USE_SDM72                                // Add support for Eastron SDM72-Modbus energy monitor (+0k3 code)
  #define SDM72_SPEED          9600              // SDM72-Modbus RS485 serial speed (default: 9600 baud)
  // #define SDM72_IMPEXP                            // Show additonal import/export active energy and power in MQTT and Web (+0k5 code)
//#define USE_SDM120                               // Add support for Eastron SDM120-Modbus energy monitor (+1k1 code)
  #define SDM120_SPEED         2400              // SDM120-Modbus RS485 serial speed (default: 2400 baud)
//#define USE_SDM230                               // Add support for Eastron SDM230-Modbus energy monitor (+1k6 code)
  #define SDM230_SPEED         9600              // SDM230-Modbus RS485 serial speed (default: 9600 baud)
//  #define SDM230_MORE_REGS                        // read more registers (see xnrg_21_sdm230.ino, may cause timing issues (at your own risk, +0k4 code))
//#define USE_SDM630                               // Add support for Eastron SDM630-Modbus energy monitor (+0k6 code)
  #define SDM630_SPEED         9600              // SDM630-Modbus RS485 serial speed (default: 9600 baud)
//  #define SDM630_IMPORT                          // Show import active energy in MQTT and Web (+0k3 code)
//  #define SDM630_HIGH_UPDATE_RATE                //  SDM630-Modbus improved readout with higher update rate (+0k1 RAM)
//#define USE_DDS2382                              // Add support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
  #define DDS2382_SPEED        9600              // Hiking DDS2382 Modbus RS485 serial speed (default: 9600 baud)
//#define USE_DDSU666                              // Add support for Chint DDSU666 Modbus energy monitor (+0k6 code)
  #define DDSU666_SPEED        9600              // Chint DDSU666 Modbus RS485 serial speed (default: 9600 baud)
//#define USE_SOLAX_X1                             // Add support for Solax X1 series Modbus log info (+3k1 code)
  #define SOLAXX1_SPEED        9600              // Solax X1 Modbus RS485 serial speed (default: 9600 baud)
//  #define SOLAXX1_PV2                            // Solax X1 using second PV
//#define USE_LE01MR                               // Add support for F&F LE-01MR Modbus energy monitor (+1k code)
  #define LE01MR_SPEED         9600              // LE-01MR modbus baudrate (default: 9600)
  #define LE01MR_ADDR          1                 // LE-01MR modbus address (default: 0x01)
#define USE_BL09XX                               // Add support for various BL09XX Energy monitor as used in Blitzwolf SHP-10 or Sonoff Dual R3 v2 (+1k6 code)
//#define USE_TELEINFO                             // Add support for Teleinfo via serial RX interface (+5k2 code, +168 RAM + SmartMeter LinkedList Values RAM)
//#define USE_IEM3000                              // Add support for Schneider Electric iEM3000-Modbus series energy monitor (+0k8 code)
  #define IEM3000_SPEED          19200           // iEM3000-Modbus RS485 serial speed (default: 19200 baud)
  #define IEM3000_ADDR           1               // iEM3000-Modbus modbus address (default: 0x01)
//  #define IEM3000_IEM3155                        // Compatibility fix for Iem3155 (changes Power and Energy total readout)
//#define USE_WE517                                // Add support for Orno WE517-Modbus energy monitor (+1k code)
//#define USE_MODBUS_ENERGY                        // Add support for generic modbus energy monitor using a user file in rule space (+5k)
//#define USE_V9240                              // Add support for Vango Technologies V924x ultralow power, single-phase, power measurement (+12k)

// -- Low level interface devices -----------------
#define USE_DHT                                  // Add support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor (1k6 code)

//#define USE_MAX31855                             // Add support for MAX31855/MAX6675 K-Type thermocouple sensor using softSPI
//#define USE_MAX31865                             // Add support for MAX31865 RTD sensors using softSPI
  #define MAX31865_PTD_WIRES  2                  // PTDs come in several flavors, pick yours. Specific settings per sensor possible with MAX31865_PTD_WIRES1..MAX31865_PTD_WIRES6
  #define MAX31865_PTD_RES    100                // Nominal PTD resistance at 0°C (100Ω for a PT100, 1000Ω for a PT1000, YMMV!). Specific settings per sensor possible with MAX31865_PTD_RES1..MAX31865_PTD_RES6
  #define MAX31865_REF_RES    430                // Reference resistor (Usually 430Ω for a PT100, 4300Ω for a PT1000). Specific settings per sensor possible with MAX31865_REF_RES1..MAX31865_REF_RES6
  #define MAX31865_PTD_BIAS   0                  // To calibrate your not-so-good PTD. Specific settings per sensor possible with MAX31865_PTD_BIAS1..MAX31865_PTD_BIAS6
//#define USE_LMT01                                // Add support for TI LMT01 temperature sensor, count pulses on single GPIO (+0k5 code)
//#define USE_WIEGAND                              // Add support for 24/26/32/34 bit RFID Wiegand interface (D0/D1) (+1k7 code)

// -- IR Remote features - all protocols from IRremoteESP8266 --------------------------
// IR Full Protocols mode is now activated through USE_IR_REMOTE_FULL
//
// Code impact of IR full protocols is +90k code, 3k mem

// -- IR Remote features - subset of IR protocols --------------------------
#define USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 (+4k3 code, 0k3 mem, 48 iram)
  #define IR_SEND_INVERTED          false        // Invert the output. (default = false) e.g. LED is illuminated when GPIO is LOW rather than HIGH.
                                                 // Setting inverted to something other than the default could easily destroy your IR LED if you are overdriving it.
                                                 // Unless you REALLY know what you are doing, don't change this.
  #define IR_SEND_USE_MODULATION    true         // Do we do frequency modulation during transmission? i.e. If not, assume a 100% duty cycle.

  // ====> IR Full protocols are enabled with the line below
  // #define USE_IR_REMOTE_FULL                     // Support all IR protocols from IRremoteESP8266
  //
  // WARNING: if you change any IR configuration, you need to clear Platform.io cache
  //          currently the include change detection does not work for these parameters
  //          This is not an issue if you compile with gitpod or pre-compiled binaries
  //          since there is no cache of previously compiled libraries
  // ====>
  //
  // You can also disable specific protocols in user_config_override
  // Example: disable some HVAC protocols to reduce flash size
  // #define DECODE_WHIRLPOOL_AC  false
  // #define SEND_WHIRLPOOL_AC    false
  // #define DECODE_SAMSUNG_AC    false
  // #define SEND_SAMSUNG_AC      false
  // ...

  // When using 'USE_IR_REMOTE_FULL', parameters below
  // (USE_IR_REMOTE, USE_IR_RECEIVE, USE_IR_HVAC...) are IGNORED.
  #define USE_IR_SEND_NEC                        // Support IRsend NEC protocol
  #define USE_IR_SEND_RC5                        // Support IRsend Philips RC5 protocol
  #define USE_IR_SEND_RC6                        // Support IRsend Philips RC6 protocol

  // Enable IR devoder via GPIO `IR Recv` - always enabled if `USE_IR_REMOTE_FULL`
  #define USE_IR_RECEIVE                         // Support for IR receiver (+7k2 code, 264 iram)
    #define IR_RCV_BUFFER_SIZE      100          // Max number of packets allowed in capture buffer (default 100 (*2 bytes ram))
    #define IR_RCV_TIMEOUT          15           // Number of milli-Seconds of no-more-data before we consider a message ended (default 15)
    #define IR_RCV_MIN_UNKNOWN_SIZE 6            // Set the smallest sized "UNKNOWN" message packets we actually care about (default 6, max 255)
    #define IR_RCV_WHILE_SENDING    0            // Turns on receiver while sending messages, i.e. receive your own. This is unreliable and can cause IR timing issues
    #define IR_RCV_TOLERANCE        25           // Base tolerance percentage for matching incoming IR messages (default 25, max 100)

// -- SD Card support -----------------------------
// #define USE_SDCARD                               // mount SD Card, requires configured SPI pins and setting of `SDCard CS` gpio
  #define SDC_HIDE_INVISIBLES                    // hide hidden directories from the SD Card, which prevents crashes when dealing SD created on MacOS

// -- Zigbee interface ----------------------------
//#define USE_ZIGBEE                                // Enable serial communication with Zigbee CC2530/CC2652 flashed with ZNP or EFR32 flashed with EZSP (+49k code, +3k mem)
  #define USE_ZIGBEE_ZNP                         // Enable ZNP protocol, needed for CC2530/CC2652 based devices
//  #define USE_ZIGBEE_EZSP                        // Enable EZSP protocol, needed for EFR32 EmberZNet based devices, like Sonoff Zigbee bridge
                                                 // Note: USE_ZIGBEE_ZNP and USE_ZIGBEE_EZSP are mutually incompatible, you must select exactly one
  // #define USE_ZIGBEE_EEPROM                      // Use the EEPROM from the Sonoff ZBBridge to save Zigbee configuration and data
  #define USE_ZIGBEE_CHANNEL  11                 // Zigbee Channel (11-26)
  #define USE_ZIGBEE_TXRADIO_DBM  20             // Tx Radio power in dBm (only for EZSP, EFR32 can go up to 20 dBm)

  #define USE_ZIGBEE_COALESCE_ATTR_TIMER 350     // timer to coalesce attribute values (in ms)
  #define USE_ZIGBEE_DEBOUNCE_COMMANDS   200     // if commands are received from the same device/endpoint with same ZCL transaction number, discard packet in this time window (ms)
  #define USE_ZIGBEE_MODELID      "Tasmota Z2T"  // reported "ModelId"      (cluster 0000 / attribute 0005)
  #define USE_ZIGBEE_MANUFACTURER "Tasmota"      // reported "Manufacturer" (cluster 0000 / attribute 0004)
  #define USE_ZIGBEE_BATT_REPROBE (24*3600)      // Period in seconds during which we don't ask again for battery, default 1 day
  #define USE_ZIGBEE_BATT_REPROBE_PAUSE (3600)   // Min wait period when sending an autoprobe, default: wait at least 1 hour
  #define USE_ZBBRIDGE_TLS                       // TLS support for zbbridge
  #define USE_ZIGBEE_ZBBRIDGE_EEPROM 0x50        // I2C id for the ZBBridge EEPROM
  // #define USE_ZIGBEE_FORCE_NO_CHILDREN           // This feature forces `CONFIG_MAX_END_DEVICE_CHILDREN` to zero which means that the coordinator does not accept any direct child. End-devices must pair through a router.
                                                 // This may mitigate some battery drain issues with IKEA devices.
                                                 // **DO NOT USE UNLESS YOU KNOW EXACTLY WHAT YOU'RE DOING** See #10413


  // Auto-binding constants, see `Z_autoAttributeReporting`
  #define USE_ZIGBEE_AUTOBIND_MAX_ENDPOINTS 8       // max number of endpoint receiving auto-bind requests
  #define USE_ZIGBEE_AUTOBIND_MAX_CLUSTER   0x10    // max endpoint number (excluded receiving auto-bind requests)
  // Below are the threshold for attribute reporting
  #define USE_ZIGBEE_AUTOBIND_BATTVOLTAGE   0.2     // V
  #define USE_ZIGBEE_AUTOBIND_BATTPERCENT   5       // %
  #define USE_ZIGBEE_AUTOBIND_TEMPERATURE   0.5     // °C
  #define USE_ZIGBEE_AUTOBIND_HEATDEMAND    10      // %
  #define USE_ZIGBEE_AUTOBIND_PRESSURE      1       // hPA
  #define USE_ZIGBEE_AUTOBIND_ILLUMINANCE   5       // lux
  #define USE_ZIGBEE_AUTOBIND_HUMIDITY      1.0     // %
  #define USE_ZIGBEE_AUTOBIND_LIFT          1.0     // %
  // Below are the Max Thresholds for reporting time (in seconds)
  #define USE_ZIGBEE_MAXTIME_BATT           4*60*60   // 4h
  #define USE_ZIGBEE_MAXTIME_TRV            60*10     // 10m
  #define USE_ZIGBEE_MAXTIME_SENSOR         60*60     // 1h
  #define USE_ZIGBEE_MAXTIME_LIGHT          60*60     // 1h
  #define USE_ZIGBEE_MAXTIME_LIFT           4*60*60   // 4h

// -- Matter support (ESP32 and variants) ----------------------------
#define MATTER_ENABLED    false                    // [SetOption151] Is Matter enabled by default

// -- Other sensors/drivers -----------------------

//#define USE_SHIFT595                             // Add support for 74xx595 8-bit shift registers (+0k7 code)
  #define SHIFT595_INVERT_OUTPUTS false            // [SetOption133] Don't invert outputs of 74x595 shift register
  #define SHIFT595_DEVICE_COUNT  1                 // [Shift595DeviceCount] Set the number of connected 74x595 shift registers

//#define USE_TM1638                               // Add support for TM1638 switches copying Switch1 .. Switch8 (+1k code)
//  #define TM1638_USE_AS_BUTTON                   // Add support for buttons
//  #define TM1638_USE_AS_SWITCH                   // Add support for switches (default)
//  #define TM1638_MAX_DISPLAYS    8               // Add support for power control 8 displays
//  #define TM1638_MAX_KEYS        8               // Add support for 8 keys
//  #define TM1638_MAX_LEDS        8               // Add support for 8 leds
//#define USE_HX711                                // Add support for HX711 load cell (+1k5 code)
//  #define USE_HX711_M5SCALES                     // [I2cDriver89] Enable support for M5Unit (Mini)Scales (I2C address 0x26) (+0k4 code)
//  #define USE_HX711_GUI                          // Add optional web GUI to HX711 as scale (+1k8 code)
//  #define HX711_CAL_PRECISION     1              // When HX711 calibration is to course, raise this value

//#define USE_DINGTIAN_RELAY                       // Add support for the Dingian board using 74'595 et 74'165 shift registers
//  #define DINGTIAN_INPUTS_INVERTED               // Invert input states (Hi => OFF, Low => ON)
//  #define DINGTIAN_USE_AS_BUTTON                 // Inputs as Tasmota's virtual Buttons
//  #define DINGTIAN_USE_AS_SWITCH                 // Inputs as Tasmota's virtual Switches

// Select none or only one of the below defines
//#define USE_TX20_WIND_SENSOR                     // Add support for La Crosse TX20 anemometer (+2k6/0k8 code)
//#define USE_TX23_WIND_SENSOR                     // Add support for La Crosse TX23 anemometer (+2k7/1k code)

//#define USE_WINDMETER                            // Add support for analog anemometer (+2k2 code)

//#define USE_FTC532                               // Add support for FTC532 8-button touch controller (+0k6 code)

//#define USE_RC_SWITCH                            // Add support for RF transceiver using library RcSwitch (+2k7 code, 460 iram)

//#define USE_RF_SENSOR                            // Add support for RF sensor receiver (434MHz or 868MHz) (+0k8 code)
//  #define USE_THEO_V2                            // Add support for decoding Theo V2 sensors as documented on https://sidweb.nl using 434MHz RF sensor receiver (+1k4 code)
//  #define USE_ALECTO_V2                          // Add support for decoding Alecto V2 sensors like ACH2010, WS3000 and DKW2012 weather stations using 868MHz RF sensor receiver (+1k7 code)

//#define USE_HRE                                  // Add support for Badger HR-E Water Meter (+1k4 code)
//#define USE_A4988_STEPPER                        // Add support for A4988/DRV8825 stepper-motor-driver-circuit (+10k5 code)

//#define USE_PROMETHEUS                           // Add support for https://prometheus.io/ metrics exporting over HTTP /metrics endpoint

//#define USE_NEOPOOL                              // Add support for Sugar Valley NeoPool Controller - also known under brands Hidrolife, Aquascenic, Oxilife, Bionet, Hidroniser, UVScenic, Station, Brilix, Bayrol and Hay (+14k flash, +120 mem)
//  #define NEOPOOL_MODBUS_ADDRESS       1         // Any modbus address

//#define USE_FLOWRATEMETER                        // Add support for water flow meter YF-DN50 and similary (+1k7 code)

// -- Thermostat control ----------------------------
//#define USE_THERMOSTAT                           // Add support for Thermostat
  #define THERMOSTAT_CONTROLLER_OUTPUTS         1         // Number of outputs to be controlled independently
  #define THERMOSTAT_SENSOR_NAME                "DS18B20" // Name of the local sensor to be used
  #define THERMOSTAT_SENSOR_NUMBER              1         // Number of local sensors to be used
  #define THERMOSTAT_RELAY_NUMBER               1         // Default output relay number for the first controller (+i for following ones)
  #define THERMOSTAT_SWITCH_NUMBER              1         // Default input switch number for the first controller (+i for following ones)
  #define THERMOSTAT_TIME_ALLOW_RAMPUP          300       // Default time after last target update to allow ramp-up controller phase in minutes
  #define THERMOSTAT_TIME_RAMPUP_MAX            960       // Default time maximum ramp-up controller duration in minutes
  #define THERMOSTAT_TIME_RAMPUP_CYCLE          30        // Default time ramp-up cycle in minutes
  #define THERMOSTAT_TIME_SENS_LOST             30        // Maximum time w/o sensor update to set it as lost in minutes
  #define THERMOSTAT_TEMP_SENS_NUMBER           1         // Default temperature sensor number
  #define THERMOSTAT_TIME_MANUAL_TO_AUTO        60        // Default time without input switch active to change from manual to automatic in minutes
  #define THERMOSTAT_TIME_RESET                 12000     // Default reset time of the PI controller in seconds
  #define THERMOSTAT_TIME_PI_CYCLE              30        // Default cycle time for the thermostat controller in minutes
  #define THERMOSTAT_TIME_MAX_ACTION            20        // Default maximum thermostat time per cycle in minutes
  #define THERMOSTAT_TIME_MIN_ACTION            4         // Default minimum thermostat time per cycle in minutes
  #define THERMOSTAT_TIME_MIN_TURNOFF_ACTION    3         // Default minimum turnoff time in minutes, below it the thermostat will be held on
  #define THERMOSTAT_PROP_BAND                  4         // Default proportional band of the PI controller in degrees celsius
  #define THERMOSTAT_TEMP_RESET_ANTI_WINDUP     8         // Default range where reset antiwindup is disabled, in tenths of degrees celsius
  #define THERMOSTAT_TEMP_HYSTERESIS            1         // Default range hysteresis for temperature PI controller, in tenths of degrees celsius
  #define THERMOSTAT_TEMP_FROST_PROTECT         40        // Default minimum temperature for frost protection, in tenths of degrees celsius
  #define THERMOSTAT_TEMP_RAMPUP_DELTA_IN       4         // Default minimum delta temperature to target to get into rampup mode, in tenths of degrees celsius
  #define THERMOSTAT_TEMP_RAMPUP_DELTA_OUT      2         // Default minimum delta temperature to target to get out of the rampup mode, in tenths of degrees celsius
  #define THERMOSTAT_TEMP_PI_RAMPUP_ACC_E       200       // Default accumulated error when switching from ramp-up controller to PI in hundreths of degrees celsius
  #define THERMOSTAT_TIME_OUTPUT_DELAY          180       // Default output delay between state change and real actuation event (f.i. valve open/closed)
  #define THERMOSTAT_TEMP_INIT                  180       // Default init target temperature for the thermostat controller
  #define THERMOSTAT_TIME_MAX_OUTPUT_INCONSIST  3         // Default maximum time where the input and the outpus shall differ (for diagnostic) in minutes
  #define THERMOSTAT_TIME_MAX_AUTOTUNE          21600     // Maximum time for the PI autotune function to complete in seconds
  #define THERMOSTAT_DUTYCYCLE_AUTOTUNE         35        // Default duty cycle (in % over PI cycle time) for the step response of the autotune PI function
  #define THERMOSTAT_PEAKNUMBER_AUTOTUNE        8         // Default number of peak temperatures (max or min) to be used for the autotune PI function
  #define THERMOSTAT_TEMP_BAND_NO_PEAK_DET      1         // Default temperature band in thenths of degrees celsius within no peak will be detected
  #define THERMOSTAT_TIME_STD_DEV_PEAK_DET_OK   10        // Default standard deviation in minutes of the oscillation periods within the peak detection is successful

// -- PID and Timeprop ------------------------------ // Both together will add +12k1 code
// #define USE_TIMEPROP                            // Add support for the timeprop feature (+9k1 code)
                                                   // For details on the configuration please see the header of tasmota/xdrv_48_timeprop.ino
// #define USE_PID                                 // Add suport for the PID  feature (+11k2 code)
                                                   // For details on the configuration please see the header of tasmota/xdrv_49_pid.ino
// -- End of general directives ---------------------

/*********************************************************************************************\
 * ESP32 only features
\*********************************************************************************************/

#ifdef ESP32

//#define USE_ESP32_WDT                            // Enable Watchdog for ESP32, trigger a restart if loop has not responded for 5s, and if `yield();` was not called

#define SET_ESP32_STACK_SIZE  (8 * 1024)         // Set the stack size for Tasmota. The default value is 8192 for Arduino, some builds might need to increase it

#ifdef SOC_TOUCH_VERSION_1                       // ESP32
  #define ESP32_TOUCH_THRESHOLD   40             // [TouchThres] Below this level a touch is detected
#endif
#ifdef SOC_TOUCH_VERSION_2                       // ESP32-S2 and ESP32-S3
  #define ESP32_TOUCH_THRESHOLD   40000          // [TouchThres] Above this level a touch is detected
#endif

#define USE_ESP32_SENSORS                        // Add support for ESP32 temperature and optional hall effect sensor
#define USE_GPIO_VIEWER                          // Enable GPIO Viewer to see realtime GPIO states (+5k6 code)

//#define USE_SONOFF_SPM                           // Add support for ESP32 based Sonoff Smart Stackable Power Meter (+11k code)
//#define USE_DISPLAY_TM1621_SONOFF                // Add support for TM1621 dsiplay driver used by Sonoff POWR3xxD and THR3xxD

//#define USE_ETHERNET                             // Add support for ethernet (+20k code)
//  #define USE_WT32_ETH01                         // Add support for Wireless-Tag WT32-ETH01
//  #define ETH_TYPE          0                    // [EthType] 0 = LAN8720, 1 = TLK110/IP101, 2 = RTL8201, 3 = DP83848, 4 = DM9051, 5 = KSZ8081, 6 = KSZ8041, 7 = JL1101, 8 = W5500, 9 = KSZ8851
//  #define ETH_ADDRESS       1                    // [EthAddress] 0 = PHY0 .. 31 = PHY31
//  #define ETH_CLKMODE       0                    // [EthClockMode] 0 = ETH_CLOCK_GPIO0_IN, 1 = ETH_CLOCK_GPIO0_OUT, 2 = ETH_CLOCK_GPIO16_OUT, 3 = ETH_CLOCK_GPIO17_OUT
  // Olimex ESP32-PoE
  #define ETH_TYPE          0                    // [EthType] 0 = LAN8720, 1 = TLK110/IP101, 2 = RTL8201, 3 = DP83848, 4 = DM9051, 5 = KSZ8081, 6 = KSZ8041, 7 = JL1101, 8 = W5500, 9 = KSZ8851
  #define ETH_ADDRESS       0                    // [EthAddress] 0 = PHY0 .. 31 = PHY31
  #define ETH_CLKMODE       3                    // [EthClockMode] 0 = ETH_CLOCK_GPIO0_IN, 1 = ETH_CLOCK_GPIO0_OUT, 2 = ETH_CLOCK_GPIO16_OUT, 3 = ETH_CLOCK_GPIO17_OUT
  // wESP32-PoE
//  #define ETH_TYPE          0                    // [EthType] 0 = LAN8720, 1 = TLK110/IP101, 2 = RTL8201, 3 = DP83848, 4 = DM9051, 5 = KSZ8081, 6 = KSZ8041, 7 = JL1101, 8 = W5500, 9 = KSZ8851
//  #define ETH_ADDRESS       0                    // [EthAddress] 0 = PHY0 .. 31 = PHY31
//  #define ETH_CLKMODE       0                    // [EthClockMode] 0 = ETH_CLOCK_GPIO0_IN, 1 = ETH_CLOCK_GPIO0_OUT, 2 = ETH_CLOCK_GPIO16_OUT, 3 = ETH_CLOCK_GPIO17_OUT

#define USE_ADC                                  // Add support for ADC on GPIO32 to GPIO39

#define USE_NETWORK_LIGHT_SCHEMES                // Add support for light schemes via DDP (via UDP on port 4048)

//#define USE_SPI                                  // Add support for hardware SPI
//#define USE_MI_ESP32                             // Add support for ESP32 as a BLE-bridge (+9k2 mem, +292k flash)
//#define USE_BLE_ESP32                            // Add support for ESP32 as a BLE-bridge (+9k2? mem, +292k? flash)
  //#define BLE_ESP32_ENABLE false                 // [SetOption115] Default value for SetOption115
  //#define USE_IBEACON                            // Add support for Bluetooth LE passive scan of iBeacon devices (uses HM17 module)
  //#define USE_IBEACON_ESP32                      // Add support for Bluetooth LE passive scan of iBeacon devices using the internal ESP32 Bluetooth module
//#define USE_WEBCAM                               // Add support for webcam

#define USE_AUTOCONF                             // Enable Esp32(x) autoconf feature, requires USE_BERRY and USE_WEBCLIENT_HTTPS (12KB Flash)
#define USE_EXTENSION_MANAGER                    // Enable Esp32(x) extensions manager, requires USE_BERRY and USE_WEBCLIENT_HTTPS (11KB Flash)
#define USE_BERRY                                // Enable Berry scripting language
  #define USE_BERRY_PYTHON_COMPAT                // Enable by default `import python_compat`
  #define USE_BERRY_TIMEOUT             4000     // Timeout in ms, will raise an exception if running time exceeds this timeout
  #define USE_BERRY_PSRAM                        // Allocate Berry memory in PSRAM if PSRAM is connected - this might be slightly slower but leaves main memory intact
  #define USE_BERRY_IRAM                         // Allocate some data structures in IRAM (which is usually unused) when possible and if no PSRAM is available
  #define USE_BERRY_FAST_LOOP_SLEEP_MS  5        // Minimum time in milliseconds to before calling again `tasmota.fast_loop()`, a smaller value will consume more CPU (min 1ms)
  // #define USE_BERRY_DEBUG                        // Compile Berry bytecode with line number information, makes exceptions easier to debug. Adds +8% of memory consumption for compiled code
  //   #define UBE_BERRY_DEBUG_GC                   // Print low-level GC metrics
  // #define USE_BERRY_INT64                        // Add 64 bits integer support (+1.7KB Flash)
  #define USE_WEBCLIENT_HTTPS                    // Enable HTTPS outgoing requests based on BearSSL (much ligher then mbedTLS, 42KB vs 150KB) in insecure mode (no verification of server's certificate)
                                                 // Note that only two ciphers are enabled: ECDHE_RSA_WITH_AES_128_GCM_SHA256, ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
    #define USE_BERRY_WEBCLIENT_USERAGENT  "TasmotaClient" // default user-agent used, can be changed with `wc.set_useragent()`
    #define USE_BERRY_WEBCLIENT_TIMEOUT  2000    // Default timeout in milliseconds
    // #define USE_BERRY_LEDS_PANEL                 // Add button to dynamically load the Leds Panel from a bec file online
    #define USE_BERRY_LEDS_PANEL_URL             "http://ota.tasmota.com/tapp/leds_panel.bec"
    // #define USE_BERRY_LVGL_PANEL                 // Add button to dynamically load the LVGL Panel from a bec file online
    #define USE_BERRY_LVGL_PANEL_URL             "http://ota.tasmota.com/tapp/lvgl_panel.bec"
    //#define USE_BERRY_PARTITION_WIZARD           // Add a button to dynamically load the Partion Wizard from a bec file online (+1.3KB Flash)
    #define USE_BERRY_PARTITION_WIZARD_URL      "http://ota.tasmota.com/tapp/partition_wizard.bec"
    //#define USE_BERRY_GPIOVIEWER                 // Add a button to dynamocally load the GPIO Viewer from a bec file online
    #define USE_BERRY_GPIOVIEWER_URL            "http://ota.tasmota.com/tapp/gpioviewer.bec"
  #define USE_BERRY_TCPSERVER                    // Enable TCP socket server (+0.6k)
  // #define USE_BERRY_ULP                          // Enable ULP (Ultra Low Power) support (+4.9k)
  // Berry crypto extensions below:
  #define USE_BERRY_CRYPTO_AES_GCM               // enable AES GCM 256 bits
  // #define USE_BERRY_CRYPTO_AES_CCM               // enable AES CCM 128 bits
  // #define USE_BERRY_CRYPTO_AES_CTR               // enable AES CTR 256 bits
  // #define USE_BERRY_CRYPTO_EC_P256               // enable EC P256r1
  // #define USE_BERRY_CRYPTO_EC_C25519             // enable Elliptic Curve C C25519
  #define USE_BERRY_CRYPTO_SHA256                // enable SHA256 hash function
  #define USE_BERRY_CRYPTO_HMAC_SHA256           // enable HMAC SHA256 hash function
  // #define USE_BERRY_CRYPTO_PBKDF2_HMAC_SHA256    // PBKDF2 with HMAC SHA256, used in Matter protocol
  // #define USE_BERRY_CRYPTO_HKDF_SHA256      // HKDF with HMAC SHA256, used in Matter protocol
  // #define USE_BERRY_CRYPTO_SPAKE2P_MATTER   // SPAKE2+ used in Matter 1.0, complete name is SPAKE2+-P256-SHA256-HKDF-SHA256-HMAC-SHA256
  // #define USE_BERRY_CRYPTO_RSA              // RSA primitives including JWT RS256 (3.9KB flash)
#define USE_CSE7761                              // Add support for CSE7761 Energy monitor as used in Sonoff Dual R3

// -- LVGL Graphics Library ---------------------------------
//#define USE_LVGL                                 // LVGL Engine, requires Berry (+382KB)
  #define USE_LVGL_PSRAM                         // Allocate LVGL memory in PSRAM if PSRAM is connected - this might be slightly slower but leaves main memory intact
  #define USE_LVGL_HASPMOTA                      // Enable OpenHASP compatiblity and Robotocondensed fonts (+90KB flash)
  #define USE_LVGL_MAX_SLEEP  10                 // max sleep in ms when LVGL is enabled, more than 10ms will make display less responsive
  #define USE_LVGL_PNG_DECODER                   // include a PNG image decoder from file system (+16KB)
  #define USE_LVGL_FREETYPE                      // Use the FreeType renderer to display fonts using native TTF files in file system (+77KB flash)
    #define USE_LVGL_FREETYPE_MAX_FACES 64       // max number of FreeType faces in cache
  #define USE_LVGL_BG_DEFAULT 0x000000           // Default color for the uninitialized background screen (black)
  // Disabling select widgets that will be rarely used in Tasmota (-13KB)
  // Main widgets as defined in LVGL8
    #define BE_LV_WIDGET_OBJ
    #define BE_LV_WIDGET_ARC
    #define BE_LV_WIDGET_BAR
    #define BE_LV_WIDGET_BTN        // LVGL 8
    #define BE_LV_WIDGET_BUTTON     // LVGL 9
    #define BE_LV_WIDGET_BTNMATRIX  // LVGL 8
    #define BE_LV_WIDGET_BUTTONMATRIX // LVGL 9
    #define BE_LV_WIDGET_CANVAS
    #define BE_LV_WIDGET_CHECKBOX
    #define BE_LV_WIDGET_DROPDOWN
    #define BE_LV_WIDGET_IMG        // LVGL 8
    #define BE_LV_WIDGET_IMAGE      // LVGL 9
    #define BE_LV_WIDGET_LABEL
    #define BE_LV_WIDGET_LINE
    #define BE_LV_WIDGET_ROLLER
    #define BE_LV_WIDGET_SLIDER
    #define BE_LV_WIDGET_SWITCH
    #define BE_LV_WIDGET_TABLE
    // #define BE_LV_WIDGET_TEXTAREA

    // adding ad-hoc colorwheel from LVGL8 to LVGL9
    #define BE_LV_WIDGET_COLORWHEEL

    #define BE_LV_WIDGET_ANIMIMG
    #define BE_LV_WIDGET_CHART
    #define BE_LV_WIDGET_IMGBTN       // LVGL 8
    #define BE_LV_WIDGET_IMAGEBUTTON  // LVGL 9
    // #define BE_LV_WIDGET_KEYBOARD
    #define BE_LV_WIDGET_LED
    #define BE_LV_WIDGET_LIST
    // #define BE_LV_WIDGET_MENU
    #ifdef BE_LV_WIDGET_MENU          // if menu is enabled, also enable sub-element classes
      #define BE_LV_WIDGET_MENU_CONT
      #define BE_LV_WIDGET_MENU_PAGE
      #define BE_LV_WIDGET_MENU_SECTION
      #define BE_LV_WIDGET_MENU_SEPARATOR
    #endif // BE_LV_WIDGET_MENU
    #define BE_LV_WIDGET_METER
    #define BE_LV_WIDGET_MSGBOX
    #define BE_LV_WIDGET_QRCODE
    #define BE_LV_WIDGET_SCALE
    #define BE_LV_WIDGET_SCALE_SECTION
    // #define BE_LV_WIDGET_SPINBOX
    #define BE_LV_WIDGET_SPINNER
    #define BE_LV_WIDGET_SPANGROUP
    #define BE_LV_WIDGET_SPAN
    #define BE_LV_WIDGET_TABVIEW
    // #define BE_LV_WIDGET_TILEVIEW

// -- Matter protocol ---------------------------------
  // #define USE_MATTER_DEVICE                      // Enable Matter device support (+380KB)
                                                    // Enabled by default in standard ESP32 binary

#endif  // ESP32

/*********************************************************************************************\
 * Debug features
\*********************************************************************************************/

//#define DEBUG_TASMOTA_CORE                       // Enable core debug messages
//#define DEBUG_TASMOTA_DRIVER                     // Enable driver debug messages
//#define DEBUG_TASMOTA_SENSOR                     // Enable sensor debug messages
//#define USE_DEBUG_DRIVER                         // Use xdrv_99_debug.ino providing commands CpuChk, CfgXor, CfgDump, CfgPeek and CfgPoke

/*********************************************************************************************\
 * Profiling features
\*********************************************************************************************/

//#define USE_PROFILING                            // Enable profiling
//#define PROFILE_THRESHOLD            70          // Minimum duration in milliseconds to start logging
//#define USE_PROFILE_DRIVER                       // Enable driver profiling
//#define USE_PROFILE_FUNCTION                     // Enable driver function profiling

/*********************************************************************************************\
 * Optional firmware configurations
 * Select none or just one for optional features and sensors as configured in tasmota_configurations.h
 * See RELEASENOTES.md for selected features
\*********************************************************************************************/

//#define FIRMWARE_LITE                            // Create tasmota-lite with no sensors
//#define FIRMWARE_SENSORS                         // Create tasmota-sensors with useful sensors enabled
//#define FIRMWARE_KNX_NO_EMULATION                // Create tasmota-knx with KNX but without Emulation
//#define FIRMWARE_DISPLAYS                        // Create tasmota-display with display drivers enabled
//#define FIRMWARE_IR                              // Create tasmota-ir with IR full protocols activated, and many sensors disabled
//#define FIRMWARE_MINIMAL                         // Create tasmota-minimal as intermediate firmware for OTA-MAGIC

/*********************************************************************************************\
 * Safe guard when needed defines are not done in Platformio                                                         *
\*********************************************************************************************/

#ifndef OTA_URL
  #define OTA_URL ""
#endif

/*********************************************************************************************\
 * User configurable items override                                                          *
\*********************************************************************************************/

#ifdef USE_CONFIG_OVERRIDE
  #include "user_config_override.h"              // Configuration overrides for my_user_config.h
#endif
#if defined(USE_MQTT_AWS_IOT) && !defined(USE_MQTT_CLIENT_CERT)
  #define USE_MQTT_CLIENT_CERT                   // USE_MQTT_AWS_IOT requires USE_MQTT_CLIENT_CERT
#endif

/*********************************************************************************************\
 * Post-process obsoletes
\*********************************************************************************************/

#ifdef USE_ESP32MAIL
#define USE_SENDMAIL                             // USE_ESP32MAIL is replaced by USE_SENDMAIL
#endif

/*********************************************************************************************\
 * Mutual exclude options
\*********************************************************************************************/

#if defined(ESP8266) && defined(USE_DISCOVERY) && (defined(USE_MQTT_CLIENT_CERT) || defined(USE_MQTT_AWS_IOT_LIGHT))
  #error "Select either USE_DISCOVERY or USE_MQTT_CLIENT_CERT/USE_MQTT_AWS_IOT, mDNS takes too much code space and is not needed for AWS IoT"
#endif

#if defined(USE_RULES) && defined(USE_SCRIPT)
  #error "Select either USE_RULES or USE_SCRIPT. They can't both be used at the same time"
#endif

/*********************************************************************************************\
 * Post-process compile options for Autoconf and others
\*********************************************************************************************/

#if defined(USE_AUTOCONF)
  #ifndef USE_BERRY
    #define USE_BERRY
  #endif
  #ifndef USE_WEBCLIENT_HTTPS
    #define USE_WEBCLIENT_HTTPS
  #endif
  #ifndef USE_MQTT_TLS
    #define USE_MQTT_TLS
  #endif
#endif // USE_AUTOCONF

#ifdef USE_SONOFF_SPM
  #define USE_ETHERNET
#endif

/*********************************************************************************************\
 * Post-process compile options for TLS
\*********************************************************************************************/

#ifdef ESP8266
#ifdef USE_SENDMAIL
  #define USE_TLS                                // flag indicates we need to include TLS code
#endif
#endif

#if defined(USE_MQTT_TLS) || defined(USE_TELEGRAM) || defined(USE_WEBCLIENT_HTTPS)
  #define USE_TLS                                // flag indicates we need to include TLS code
#endif

/*********************************************************************************************\
 * Post-process stack size adjustment
\*********************************************************************************************/

#if defined(USE_LVGL) && defined(USE_LVGL_FREETYPE)   // Freetype requires a stack of at least 24KB
  #if SET_ESP32_STACK_SIZE < (24 * 1024)
    #undef SET_ESP32_STACK_SIZE
    #define SET_ESP32_STACK_SIZE (24 * 1024)
  #endif
#endif // USE_LVGL && USE_LVGL_FREETYPE

/*********************************************************************************************\
 * Post-process I2S
\*********************************************************************************************/

#if defined(USE_I2S_ALL)
  #define USE_I2S
  #define USE_I2S_AUDIO
  #define USE_I2S_MIC
  #define USE_SHINE
  #define MP3_MIC_STREAM
  #define USE_I2S_AUDIO_BERRY
  #define USE_I2S_AAC
  #define USE_I2S_OPUS
#endif // USE_I2S_ALL

#endif  // _MY_USER_CONFIG_H_
