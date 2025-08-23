/*
  tasmota_types.h - setting variables for Tasmota

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

#ifndef _TASMOTA_TYPES_H_
#define _TASMOTA_TYPES_H_

// Bitfield to be used for any SetOption0 .. SetOption31 persistent single bit
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint32_t save_state : 1;               // bit 0              - SetOption0   - SAVE_STATE - (Settings) Save power state (1) and use after restart
    uint32_t button_restrict : 1;          // bit 1              - SetOption1   - KEY_DISABLE_MULTIPRESS - (Button) Control button single press (1) or multipress (0)
    uint32_t mqtt_add_global_info : 1;     // bit 2              - SetOption2   - (MQTT) Add global temperature/humidity/pressure info to JSON sensor message
    uint32_t mqtt_enabled : 1;             // bit 3              - SetOption3   - MQTT_USE - (MQTT) Enable (1)
    uint32_t mqtt_response : 1;            // bit 4              - SetOption4   - MQTT_RESULT_COMMAND - (MQTT) Switch between RESULT (0) or COMMAND (1)
    uint32_t mqtt_power_retain : 1;        // bit 5              - CMND_POWERRETAIN - MQTT_POWER_RETAIN
    uint32_t mqtt_button_retain : 1;       // bit 6              - CMND_BUTTONRETAIN - MQTT_BUTTON_RETAIN
    uint32_t mqtt_switch_retain : 1;       // bit 7              - CMND_SWITCHRETAIN - MQTT_SWITCH_RETAIN
    uint32_t temperature_conversion : 1;   // bit 8              - SetOption8   - TEMP_CONVERSION - (Temperature) Switch between Celsius (0) or Fahrenheit (1)
    uint32_t mqtt_sensor_retain : 1;       // bit 9              - CMND_SENSORRETAIN
    uint32_t mqtt_offline : 1;             // bit 10             - SetOption10  - MQTT_LWT_MESSAGE - (MQTT) Control LWT message format, Offline (1) or none (0)
    uint32_t button_swap : 1;              // bit 11 (v5.1.6)    - SetOption11  - KEY_SWAP_DOUBLE_PRESS - (Button) Swap (1) button single and double press functionality
    uint32_t stop_flash_rotate : 1;        // bit 12 (v5.2.0)    - SetOption12  - APP_FLASH_CYCLE - (Settings) Switch between dynamic (0) or fixed (1) slot flash save location
    uint32_t button_single : 1;            // bit 13 (v5.4.0)    - SetOption13  - KEY_ONLY_SINGLE_PRESS - (Button) Support only single press (1) to speed up button press recognition
    uint32_t interlock : 1;                // bit 14 (v5.6.0)    - CMND_INTERLOCK - APP_INTERLOCK_MODE - Enable (1) /disable (0) interlock
    uint32_t pwm_control : 1;              // bit 15 (v5.8.1)    - SetOption15  - LIGHT_MODE - (Light) Switch between commands PWM (0) or COLOR/DIMMER/CT/CHANNEL (1)
    uint32_t ws_clock_reverse : 1;         // bit 16 (v5.8.1)    - SetOption16  - LIGHT_CLOCK_DIRECTION - (WS2812) Switch between clockwise (0) or counter-clockwise (1)
    uint32_t decimal_text : 1;             // bit 17 (v5.8.1)    - SetOption17  - LIGHT_COLOR_RADIX - (Light) Switch between decimal (1) or hexadecimal (0) output
    uint32_t light_signal : 1;             // bit 18 (v5.10.0c)  - SetOption18  - LIGHT_PAIRS_CO2 - (Light) Pair light signal (1) with CO2 sensor
    uint32_t hass_discovery : 1;           // bit 19 (v5.11.1a)  - SetOption19  - HOME_ASSISTANT_DISCOVERY_ENABLE - (HAss) Control automatic discovery (1) (See SetOption59)
    uint32_t not_power_linked : 1;         // bit 20 (v5.11.1f)  - SetOption20  - LIGHT_POWER_CONTROL - (Light) Control power in relation to Dimmer/Color/Ct changes (1)
    uint32_t no_power_on_check : 1;        // bit 21 (v5.11.1i)  - SetOption21  - ENERGY_VOLTAGE_ALWAYS - (Energy) Show voltage even if powered off (1)
    uint32_t mqtt_serial : 1;              // bit 22 (v5.12.0f)  - CMND_SERIALSEND and CMND_SERIALLOG
    uint32_t mqtt_serial_raw : 1;          // bit 23 (v6.1.1c)   - CMND_SERIALSEND3
    uint32_t pressure_conversion : 1;      // bit 24 (v6.3.0.2)  - SetOption24  - PRESSURE_CONVERSION - (Pressure) switch between hPa (0)or mmHg (1) unit
    uint32_t knx_enabled : 1;              // bit 25 (v5.12.0l)  - CMND_KNX_ENABLED - KNX_ENABLED
    uint32_t device_index_enable : 1;      // bit 26 (v5.13.1a)  - SetOption26  - MQTT_POWER_FORMAT - (MQTT, HAss) Switch between POWER (0) or POWER1 (1)
    uint32_t knx_enable_enhancement : 1;   // bit 27 (v5.14.0a)  - CMND_KNX_ENHANCED - KNX_ENHANCED
    uint32_t rf_receive_decimal : 1;       // bit 28 (v6.0.0a)   - SetOption28  - (RF) Receive data format hexadecimal (0) or decimal (1)
    uint32_t ir_receive_decimal : 1;       // bit 29 (v6.0.0a)   - SetOption29  - IR_DATA_RADIX - (IR) Receive data format hexadecimal (0) or decimal (1)
    uint32_t hass_light : 1;               // bit 30 (v6.0.0b)   - SetOption30  - HASS_AS_LIGHT - (HAss) enforce autodiscovery as light (1)
    uint32_t global_state : 1;             // bit 31 (v6.1.0)    - SetOption31  - APP_ENABLE_LEDLINK - (Wifi, MQTT) Control link led blinking (1)
  };
} SOBitfield;

// Bitfield to be used for any SetOption50 .. SetOption81 persistent single bit
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption50 .. SetOption81
    uint32_t timers_enable : 1;            // bit 0 (v6.1.1b)    - CMND_TIMERS - TIMERS_ENABLED
    uint32_t user_esp8285_enable : 1;      // bit 1 (v6.1.1.14)  - SetOption51  - (GPIO) Enable ESP8285 user GPIO's (1)
    uint32_t time_append_timezone : 1;     // bit 2 (v6.2.1.2)   - SetOption52  - MQTT_APPEND_TIMEZONE - (Time) Append timezone to JSON time (1)
    uint32_t gui_hostname_ip : 1;          // bit 3 (v6.2.1.20)  - SetOption53  - GUI_SHOW_HOSTNAME - (GUI) Show hostname and IP address in GUI main menu (1)
    uint32_t tuya_apply_o20 : 1;           // bit 4 (v6.3.0.4)   - SetOption54  - TUYA_SETOPTION_20 - (Tuya) Apply SetOption20 settings to Tuya device (1)
    uint32_t mdns_enabled : 1;             // bit 5 (v6.4.1.4)   - SetOption55  - MDNS_ENABLED - (mDNS) Service on (1) or off (0)
    uint32_t use_wifi_scan : 1;            // bit 6 (v6.3.0.10)  - SetOption56  - WIFI_SCAN_AT_RESTART - (Wifi) Scan network at restart for configured AP's (1) or used stored AP (0)
    uint32_t use_wifi_rescan : 1;          // bit 7 (v6.3.0.10)  - SetOption57  - WIFI_SCAN_REGULARLY - (Wifi) Scan network every 44 minutes for configured AP's (1)
    uint32_t receive_raw : 1;              // bit 8 (v6.3.0.11)  - SetOption58  - IR_ADD_RAW_DATA - (IR) Add IR Raw data to JSON message (1)
    uint32_t hass_tele_on_power : 1;       // bit 9 (v6.3.0.13)  - SetOption59  - TELE_ON_POWER - (MQTT) Send tele/%topic%/STATE in addition to stat/%topic%/RESULT (1)
    uint32_t sleep_normal : 1;             // bit 10 (v6.3.0.15) - SetOption60  - APP_NORMAL_SLEEP - (Sleep) Enable normal sleep (1) instead of dynamic sleep (0)
    uint32_t button_switch_force_local : 1;// bit 11 (v6.3.0.16) - SetOption61  - MQTT_BUTTON_SWITCH_FORCE_LOCAL - (Button, Switch) Force local operation (1) when button/switch topic is set
    uint32_t no_hold_retain : 1;           // bit 12 (v6.4.1.19) - SetOption62  - MQTT_NO_HOLD_RETAIN - (MQTT) Don't use retain flag on HOLD messages (1)
    uint32_t no_power_feedback : 1;        // bit 13 (v6.5.0.9)  - SetOption63  - APP_NO_RELAY_SCAN - (Power) Don't scan relay power state at restart (1)
    uint32_t use_underscore : 1;           // bit 14 (v6.5.0.12) - SetOption64  - MQTT_INDEX_SEPARATOR - (JSON) Enable "_" (1) instead of "-" (0) as sensor index separator
    uint32_t fast_power_cycle_disable : 1; // bit 15 (v6.6.0.20) - SetOption65  - APP_DISABLE_POWERCYCLE - (QPC) Disable (1) fast power cycle detection for device reset
    uint32_t tuya_serial_mqtt_publish : 1; // bit 16 (v6.6.0.21) - SetOption66  - MQTT_TUYA_RECEIVED - (Tuya) Enable (1) TuyaMcuReceived messages over Mqtt
    uint32_t buzzer_enable : 1;            // bit 17 (v6.6.0.1)  - SetOption67  - BUZZER_ENABLE - (Buzzer) Enable (1) buzzer when available
    uint32_t pwm_multi_channels : 1;       // bit 18 (v6.6.0.3)  - SetOption68  - LIGHT_CHANNEL_MODE - (Light) Enable multi-channels PWM (1) instead of Color PWM (0)
//    uint32_t ex_tuya_dimmer_min_limit : 1; // bit 19 (v6.6.0.5)  - SetOption69  - (not used) Limits Tuya dimmers to minimum of 10% (25) when enabled
    uint32_t sb_receive_invert : 1;        // bit 19 (v14.2.0.3) - SetOption69  - (Serial) Invert Serial receive on SerialBridge (1)
    uint32_t energy_weekend : 1;           // bit 20 (v6.6.0.8)  - CMND_TARIFF
    uint32_t dds2382_model : 1;            // bit 21 (v6.6.0.14) - SetOption71  - ENERGY_DDS2382_MODE - (DDS2382) Select different Modbus registers (1) for Active Energy (#6531)
    uint32_t hardware_energy_total : 1;    // bit 22 (v6.6.0.15) - SetOption72  - ENERGY_HARDWARE_TOTALS - (Energy) Enable (1) hardware energy total counter as reference (#6561)
    uint32_t mqtt_buttons : 1;             // bit 23 (v8.2.0.3)  - SetOption73  - MQTT_BUTTONS - (Button) Detach buttons from relays (1) and enable MQTT action state for multipress
    uint32_t ds18x20_internal_pullup : 1;  // bit 24 (v7.0.0.1)  - SetOption74  - DS18X20_PULL_UP - (DS18x20) Enable internal pullup (1) for single DS18x20 sensor
    uint32_t grouptopic_mode : 1;          // bit 25 (v7.0.0.1)  - SetOption75  - MQTT_GROUPTOPIC_FORMAT - (MQTT) GroupTopic replaces %topic% (0) or fixed topic cmnd/grouptopic (1)
    uint32_t bootcount_update : 1;         // bit 26 (v7.0.0.4)  - SetOption76  - DEEPSLEEP_BOOTCOUNT - (Deepsleep) Enable incrementing bootcount (1) when deepsleep is enabled
    uint32_t slider_dimmer_stay_on : 1;    // bit 27 (v7.0.0.6)  - SetOption77  - LIGHT_SLIDER_POWER - (Light) Do not power off (1) if slider moved to far left
    uint32_t ex_compatibility_check : 1;   // bit 28 (v7.1.2.6)  - SetOption78  - (not used) Disable OTA compatibility check
    uint32_t counter_reset_on_tele : 1;    // bit 29 (v8.1.0.1)  - SetOption79  - COUNTER_RESET - (Counter) Enable resetting of counters (1) after telemetry was sent
    uint32_t shutter_mode : 1;             // bit 30 (v6.6.0.14) - SetOption80  - SHUTTER_SUPPORT - (Shutter) Enable shutter support (1)
    uint32_t pcf8574_ports_inverted : 1;   // bit 31 (v6.6.0.14) - SetOption81  - PCF8574_INVERT_PORTS - (PCF8574) Invert all ports on PCF8574 devices (1)
  };
} SOBitfield3;

// Bitfield to be used for any SetOption82 .. SetOption113 persistent single bit
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption82 .. SetOption113
    uint32_t alexa_ct_range : 1;           // bit 0 (v8.1.0.2)   - SetOption82  - LIGHT_ALEXA_CT_RANGE - (Alexa) Reduced CT range for Alexa (1)
    uint32_t zigbee_use_names : 1;         // bit 1 (v8.1.0.4)   - SetOption83  - ZIGBEE_FRIENDLY_NAMES - (Zigbee) Use FriendlyNames (1) instead of ShortAddresses (0) when possible
    uint32_t awsiot_shadow : 1;            // bit 2 (v8.1.0.5)   - SetOption84  - (AWS IoT) publish MQTT state to a device shadow (1)
    uint32_t device_groups_enabled : 1;    // bit 3 (v8.1.0.9)   - SetOption85  - (DevGroups) Enable Device Groups (1)
    uint32_t led_timeout : 1;              // bit 4 (v8.1.0.9)   - SetOption86  - (PWM Dimmer) Turn brightness LED's off (1) 5 seconds after last change
    uint32_t powered_off_led : 1;          // bit 5 (v8.1.0.9)   - SetOption87  - (PWM Dimmer) Turn red LED on (1) when powered off
    uint32_t multiple_device_groups : 1;   // bit 6 (v8.1.0.9)   - SetOption88  - (DevGroups) Enable relays in separate device groups/PWM Dimmer Buttons control remote devices (1)
    uint32_t zigbee_distinct_topics : 1;   // bit 7 (v8.1.0.10)  - SetOption89  - ZIGBEE_DISTINCT_TOPICS - (MQTT, Zigbee) Distinct MQTT topics per device for Zigbee (1) (#7835)
    uint32_t only_json_message : 1;        // bit 8 (v8.2.0.3)   - SetOption90  - MQTT_ONLY_JSON_OUTPUT - (MQTT) Disable non-json MQTT response (1)
    uint32_t fade_at_startup : 1;          // bit 9 (v8.2.0.3)   - SetOption91  - (Light) Enable light fading at start/power on (1)
    uint32_t pwm_ct_mode : 1;              // bit 10 (v8.2.0.4)  - SetOption92  - LIGHT_PWM_CT_MODE - (Light) Set PWM Mode from regular PWM (0) to ColorTemp (1) control (Xiaomi Philips ...)
    uint32_t compress_rules_cpu : 1;       // bit 11 (v8.2.0.6)  - SetOption93  - (Compress) Keep uncompressed rules in memory to avoid CPU load of uncompressing at each tick (1)
    uint32_t max6675 : 1;                  // bit 12 (v8.3.1.2)  - SetOption94  - (MAXxxx) Implement simpler MAX6675 protocol (1) instead of MAX31855 (0)
    uint32_t network_wifi : 1;             // bit 13 (v8.3.1.3)  - CMND_WIFI - 1
    uint32_t network_ethernet : 1;         // bit 14 (v8.3.1.3)  - CMND_ETHERNET - 1
    uint32_t tuyamcu_baudrate : 1;         // bit 15 (v8.3.1.6)  - SetOption97  - (Tuya) Set Baud rate for TuyaMCU serial communication to 9600 (0) or 115200 (1)
    uint32_t rotary_uses_rules : 1;        // bit 16 (v8.3.1.6)  - SetOption98  - (Rotary) Use rules (1) instead of light control (0)
    uint32_t zerocross_dimmer : 1;         // bit 17 (v8.3.1.4)  - SetOption99  - (PWM Dimmer) Enable zerocross dimmer (1)
    uint32_t remove_zbreceived : 1;        // bit 18 (v8.3.1.7)  - SetOption100 - ZIGBEE_RMV_ZBRECEIVED - (Zigbee) Remove ZbReceived form JSON message (1)
    uint32_t zb_index_ep : 1;              // bit 19 (v8.3.1.7)  - SetOption101 - ZIGBEE_INDEX_EP - (Zigbee) Add the source endpoint as suffix to attributes, ex `Power3` (1) instead of `Power` (0) if sent from endpoint 3
    uint32_t obsolete1 : 1;                // bit 20 (v9.3.1.3)  - SetOption102 - teleinfo_baudrate Obsolete Teleinfo config has now a dedicated bit field
    uint32_t mqtt_tls : 1;                 // bit 21 (v8.4.0.1)  - SetOption103 - MQTT_TLS_ENABLED - (MQTT TLS) Enable TLS mode (1) (requires TLS version)
    uint32_t mqtt_no_retain : 1;           // bit 22 (v8.4.0.1)  - SetOption104 - MQTT_NO_RETAIN - (MQTT) No Retain (1) - disable all MQTT retained messages, some brokers don't support it: AWS IoT, Losant
    uint32_t white_blend_mode : 1;         // bit 23 (v8.4.0.1)  - SetOption105 - LIGHT_WHITE_BLEND_MODE - (Light) White Blend Mode (1) - used to be `RGBWWTable` last value `0`, now deprecated in favor of this option
    uint32_t virtual_ct : 1;               // bit 24 (v8.4.0.1)  - SetOption106 - LIGHT_VIRTUAL_CT - (Light) Virtual CT (1) - Creates a virtual White ColorTemp for RGBW lights
    uint32_t virtual_ct_cw : 1;            // bit 25 (v8.4.0.1)  - SetOption107 - LIGHT_VIRTUAL_CT_CW - (Light) Virtual CT Channel (1) - signals whether the hardware white is cold CW (true) or warm WW (false)
    uint32_t obsolete2 : 1;                // bit 26 (v9.3.1.3)  - SetOption108 - teleinfo_rawdata Obsolete Teleinfo config has now a dedicated bit field
    uint32_t alexa_gen_1 : 1;              // bit 27 (v8.4.0.3)  - SetOption109 - EMULATION_HUE_1ST_GEN - (Alexa) Gen1 mode (1) - if you only have Echo Dot 2nd gen devices
    uint32_t zb_disable_autobind : 1;      // bit 28 (v8.5.0.1)  - SetOption110 - (Zigbee) Disable auto-config (1) when pairing new devices
    uint32_t buzzer_freq_mode : 1;         // bit 29 (v8.5.0.1)  - SetOption111 - (Buzzer) Use frequency output (1) for buzzer pin instead of on/off signal (0)
    uint32_t zb_topic_fname : 1;           // bit 30 (v8.5.0.1)  - SetOption112 - ZIGBEE_TOPIC_FNAME - (Zigbee) Use friendly name in zigbee topic (1) (use with SetOption89)
    uint32_t rotary_poweron_dimlow : 1;    // bit 31 (v9.0.0.2)  - SetOption113 - (Rotary) Set dimmer low on rotary dial after power off (1)
  };
} SOBitfield4;

// Bitfield to be used for any SetOption114 .. SetOption145 persistent single bit
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption114 .. SetOption145
    uint32_t mqtt_switches : 1;            // bit 0 (v9.0.0.3)   - SetOption114 - MQTT_SWITCHES - (Switch) Detach Switches from relays and enable MQTT action state for all the SwitchModes (1)
    uint32_t mi32_enable : 1;              // bit 1 (v9.1.0.1)   - SetOption115 - BLE_ESP32_ENABLE - (ESP32 BLE) Enable ESP32 MI32 BLE (1)
    uint32_t zb_disable_autoquery : 1;     // bit 2 (v9.1.0.1)   - SetOption116 - (Zigbee) Disable auto-query of zigbee lights and devices (1)
    uint32_t fade_fixed_duration : 1;      // bit 3 (v9.1.0.2)   - SetOption117 - (Light) run fading at fixed duration instead of fixed slew rate
    uint32_t zb_received_as_subtopic : 1;  // bit 4 (v9.2.0.3)   - SetOption118 - (Zigbee) Move ZbReceived from JSON message and into the subtopic replacing "SENSOR" default
    uint32_t zb_omit_json_addr : 1;        // bit 5 (v9.2.0.3)   - SetOption119 - (Zigbee) Remove the device addr from json payload, can be used with zb_topic_fname where the addr is already known from the topic
    uint32_t zb_topic_endpoint : 1;        // bit 6 (v9.2.0.4)   - SetOption120 - (Zigbee) Append endpoint number to topic if device dependent (use with SetOption89)
    uint32_t mqtt_state_retain : 1;        // bit 7 (v9.3.0.1)   - CMND_STATERETAIN - MQTT_STATE_RETAIN
    uint32_t mqtt_info_retain  : 1;        // bit 8 (v9.3.0.1)   - CMND_INFORETAIN - MQTT_INFO_RETAIN
    uint32_t wiegand_hex_output : 1;       // bit 9 (v9.3.1.1)   - SetOption123 - (Wiegand) switch tag number output to hex format (1)
    uint32_t wiegand_keypad_to_tag : 1;    // bit 10 (v9.3.1.1)  - SetOption124 - (Wiegand) send key pad stroke as single char (0) or one tag (ending char #) (1)
    uint32_t zigbee_hide_bridge_topic : 1; // bit 11 (v9.3.1.1)  - SetOption125 - (Zigbee) Hide bridge topic from zigbee topic (use with SetOption89) (1)
    uint32_t ds18x20_mean : 1;             // bit 12 (v9.3.1.2)  - SetOption126 - (DS18x20) Enable arithmetic mean over teleperiod for JSON temperature (1)
    uint32_t wifi_no_sleep : 1;            // bit 13 (v9.5.0.2)  - SetOption127 - WIFI_NO_SLEEP - (Wifi) Keep wifi in no-sleep mode, prevents some occasional unresponsiveness
    uint32_t disable_referer_chk : 1;      // bit 14 (v9.5.0.5)  - SetOption128 - DISABLE_REFERER_CHK - (Web) Allow access without referer check
    uint32_t energy_phase : 1;             // bit 15 (v9.5.0.9)  - SetOption129 - (Energy) Show phase information
    uint32_t show_heap_with_timestamp : 1; // bit 16 (v9.5.0.9)  - SetOption130 - (Debug) Show heap with logging timestamp
    uint32_t tuya_allow_dimmer_0 : 1;      // bit 17 (v10.0.0.3) - SetOption131 - TUYA_ALLOW_DIMMER_0 - (Tuya) Allow save dimmer = 0 receved by MCU
    uint32_t tls_use_fingerprint : 1;      // bit 18 (v10.0.0.4) - SetOption132 - MQTT_TLS_FINGERPRINT - (TLS) Use fingerprint validation instead of CA based
    uint32_t shift595_invert_outputs : 1;  // bit 19 (v10.0.0.4) - SetOption133 - SHIFT595_INVERT_OUTPUTS - (Shift595) Invert outputs of 74x595 shift registers
    uint32_t pwm_force_same_phase : 1;     // bit 20 (v10.1.0.6) - SetOption134 - (PWM) force PWM lights to start at same phase, default is to spread phases to minimze overlap (also needed for H-bridge)
    uint32_t display_no_splash : 1;        // bit 21 (v11.0.0.2) - SetOption135 - (Display & LVGL) force disabling default splash screen
    uint32_t tuyasns_no_immediate : 1;     // bit 22 (v11.0.0.4) - SetOption136 - (TuyaSNS) When ON disable publish single SNS value on Tuya Receive (keep Teleperiod)
    uint32_t tuya_exclude_from_mqtt : 1;   // bit 23 (v11.0.0.5) - SetOption137 - TUYA_SETOPTION_137 - (Tuya) When Set, avoid the (MQTT-) publish of defined Tuya CMDs (see xdrv_16_tuyamcu.ino) if SetOption66 is active
    uint32_t gui_table_align : 1;          // bit 24 (v11.0.0.7) - SetOption138 - (GUI) Align (energy) table values left (0) or right (1)
    uint32_t mm_vs_inch : 1;               // bit 25 (v11.1.0.1) - SetOption139 - (Pressure) Switch between mmHg (0) or inHg (1) when SO24 1
    uint32_t mqtt_persistent : 1;          // bit 26 (v11.1.0.1) - SetOption140 - MQTT_CLEAN_SESSION - (MQTT) MQTT clean session (0 = default) or persistent session (1)
    uint32_t gui_module_name : 1;          // bit 27 (v11.1.0.3) - SetOption141 - GUI_NOSHOW_MODULE - (GUI) Disable display of GUI module name (1)
    uint32_t wait_for_wifi_result : 1;     // bit 28 (v11.1.0.4) - SetOption142 - (Wifi) Wait 1 second for wifi connection solving some FRITZ!Box modem issues (1)
    uint32_t zigbee_no_batt_autoprobe : 1; // bit 29 (v12.0.2.4) - SetOption143 - (Zigbee) Disable Battery auto-probe and using auto-binding
    uint32_t zigbee_include_time : 1;      // bit 30 (v12.0.2.4) - SetOption144 - (Zigbee) Include time in `ZbReceived` messages like other sensors
    uint32_t mqtt_status_retain : 1;       // bit 31 (v12.1.0.1) - CMND_STATUSRETAIN - MQTT_STATUS_RETAIN
  };
} SOBitfield5;

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption146 .. SetOption177
    uint32_t use_esp32_temperature : 1;    // bit 0  (v12.1.1.1) - SetOption146 - (ESP32) Show ESP32 internal temperature sensor
    uint32_t mqtt_disable_publish : 1;     // bit 1  (v12.1.1.2) - SetOption147 - MQTT_DISABLE_SSERIALRECEIVED - (MQTT) Disable publish SSerialReceived/IRReceived MQTT messages, you must use event trigger rules instead.
    uint32_t artnet_autorun : 1;           // bit 2  (v12.2.0.4) - SetOption148 - (Light) start DMX ArtNet at boot, listen to UDP port as soon as network is up
    uint32_t dns_ipv6_priority : 1;        // bit 3  (v12.2.0.6) - SetOption149 - (Wifi) prefer IPv6 DNS resolution to IPv4 address when available. Requires `#define USE_IPV6`
    uint32_t no_voltage_common : 1;        // bit 4  (v12.3.1.5) - SetOption150 - (Energy) Force no voltage/frequency common
    uint32_t matter_enabled : 1;           // bit 5  (v12.3.1.5) - SetOption151 - MATTER_ENABLED - (Matter) Enable Matter protocol over Wifi
    uint32_t bistable_single_pin : 1;      // bit 6  (v12.5.0.1) - SetOption152 - (Power) Switch between two (0) or one (1) pin bistable relay control
    uint32_t berry_no_autoexec : 1;        // bit 7  (v12.5.0.3) - SetOption153 - (Berry) Disable autoexec.be on restart (1)
    uint32_t berry_light_scheme : 1;       // bit 8  (v12.5.0.3) - SetOption154 - (Berry) Handle berry led using RMT0 as additional WS2812 scheme
    uint32_t zcfallingedge : 1;            // bit 9  (v13.0.0.1) - SetOption155 - (ZCDimmer) Enable rare falling Edge dimmer instead of leading edge
    uint32_t sen5x_passive_mode : 1;       // bit 10 (v13.1.0.1) - SetOption156 - (Sen5x) Run in passive mode when there is another I2C master (e.g. Ikea Vindstyrka), i.e. do not set up Sen5x sensor, higher polling interval
    uint32_t neopool_outputsensitive : 1;  // bit 11 (v13.2.0.1) - SetOption157 - (NeoPool) Output sensitive data (1)
    uint32_t mqtt_disable_modbus : 1;      // bit 12 (v13.3.0.5) - SetOption158 - MQTT_DISABLE_MODBUSRECEIVED - (MQTT) Disable publish ModbusReceived MQTT messages (1), you must use event trigger rules instead
    uint32_t counter_both_edges : 1;       // bit 13 (v13.3.0.5) - SetOption159 - (Counter) Enable counting on both rising and falling edge (1)
    uint32_t ld2410_use_pin : 1;           // bit 14 (v14.3.0.2) - SetOption160 - (LD2410) Disable generate moving event by sensor report - use LD2410 out pin for events (1)
    uint32_t gui_no_state_text : 1;        // bit 15 (v14.3.0.7) - SetOption161 - GUI_NOSHOW_STATETEXT - (GUI) Disable display of state text (1)
    uint32_t no_export_energy_today : 1;   // bit 16 (v14.3.0.7) - SetOption162 - (Energy) Do not add export energy to energy today (1)
    uint32_t gui_device_name : 1;          // bit 17 (v14.4.1.1) - SetOption163 - GUI_NOSHOW_DEVICENAME - (GUI) Disable display of GUI device name (1)
    uint32_t wizmote_enabled : 1;          // bit 18 (v14.4.1.4) - SetOption164 - (WizMote) Enable WiZ Smart Remote support (1)
    uint32_t spare19 : 1;                  // bit 19
    uint32_t spare20 : 1;                  // bit 20
    uint32_t spare21 : 1;                  // bit 21
    uint32_t spare22 : 1;                  // bit 22
    uint32_t spare23 : 1;                  // bit 23
    uint32_t spare24 : 1;                  // bit 24
    uint32_t spare25 : 1;                  // bit 25
    uint32_t spare26 : 1;                  // bit 26
    uint32_t spare27 : 1;                  // bit 27
    uint32_t spare28 : 1;                  // bit 28
    uint32_t spare29 : 1;                  // bit 29
    uint32_t spare30 : 1;                  // bit 30
    uint32_t spare31 : 1;                  // bit 31
  };
} SOBitfield6;

// Bitfield to be used for persistent multi bit
typedef union {
  uint32_t data;                           // Allow bit manipulation
  struct {
    uint32_t spare00 : 1;
    uint32_t speed_conversion : 3;         // (v8.1.0.10) - Tx2x sensor
    uint32_t time_format : 2;              // (v6.6.0.9)  - CMND_TIME
    uint32_t calc_resolution : 3;          // CALC_RESOLUTION
    uint32_t weight_resolution : 2;
    uint32_t frequency_resolution : 2;
    uint32_t axis_resolution : 2;
    uint32_t current_resolution : 2;       // 2
    uint32_t voltage_resolution : 2;
    uint32_t wattage_resolution : 2;
    uint32_t emulation : 2;                // EMULATION
    uint32_t energy_resolution : 3;        // ENERGY_RESOLUTION
    uint32_t pressure_resolution : 2;      // PRESSURE_RESOLUTION
    uint32_t humidity_resolution : 2;      // HUMIDITY_RESOLUTION
    uint32_t temperature_resolution : 2;   // TEMP_RESOLUTION
  };
} SysMBitfield1;

// Bitfield to be used for persistent multi bit
typedef union {
  uint32_t data;                           // Allow bit manipulation
  struct {
    uint32_t log_file_idx : 4;             // bit 0.3   (v14.4.1.2) - FileLog log rotate index
    uint32_t light_pixels_order : 3;       // bit 4.6   (v14.4.1.3) - LED light order <Compile>/GRB/RGB/RBG/BRG/BGR/GBR, high bit indicates W before (for RGBW)
    uint32_t light_pixels_rgbw : 1;        // bit 7     (v14.4.1.3) - LED true is 4 channels RGBW, false is 3 channels RGB
    uint32_t light_pixels_w_first : 1;     // bit 8     (v14.4.1.3) - LED true if W channel comes first, default is <RGB>W
    uint32_t spare09 : 1;                  // bit 9
    uint32_t spare10 : 1;                  // bit 10
    uint32_t spare11 : 1;                  // bit 11
    uint32_t spare12 : 1;                  // bit 12
    uint32_t spare13 : 1;                  // bit 13
    uint32_t spare14 : 1;                  // bit 14
    uint32_t spare15 : 1;                  // bit 15
    uint32_t json_pretty_print : 3;        // bit 16.18 (v14.6.0.1) - JSON pretty print log data no or indent
    uint32_t dali_group_sliders : 5;       // bit 19.23 (v14.3.0.3) - (DALI) Number of group sliders 0 to 16
    uint32_t FTP_Mode : 2;                 // bit 24/25
    uint32_t tariff_forced : 2;            // bit 26/27 (v12.4.0.2) - Energy forced tariff : 0=tariff change on time, 1|2=tariff forced
    uint32_t sunrise_dawn_angle : 2;       // bit 28/29 (v12.1.1.4) -
    uint32_t temperature_set_res : 2;      // bit 30/31 (v9.3.1.4) - TUYA_TEMP_SET_RES - (Tuya)
  };
} SysMBitfield2;

// Bitfield to be used for non-SetOption persistent single bit
typedef union {
  uint32_t data;                           // Allow bit manipulation
  struct {
    uint32_t telegram_send_enable : 1;     // bit 0  (v9.4.0.3) - CMND_TMSTATE 0/1 - Enable Telegram send
    uint32_t telegram_recv_enable : 1;     // bit 1  (v9.4.0.3) - CMND_TMSTATE 2/3 - Enable Telegram receive
    uint32_t telegram_echo_enable : 1;     // bit 2  (v9.4.0.3) - CMND_TMSTATE 4/5 - Enable Telegram echo
    uint32_t range_extender : 1;           // bit 3  (v9.5.0.5) - CMND_RGXSTATE - Enable range extender
    uint32_t range_extender_napt : 1;      // bit 4  (v9.5.0.5) - CMND_RGXNAPT - Enable range extender NAPT
    uint32_t sonoff_l1_music_sync : 1;     // bit 5  (v9.5.0.5) - CMND_L1MUSICSYNC - Enable sync to music
    uint32_t influxdb_default : 1;         // bit 6  (v9.5.0.5) - Set influxdb initial defaults if 0
    uint32_t influxdb_state : 1;           // bit 7  (v9.5.0.5) - CMND_IFX - Enable influxdb support
    uint32_t sspm_display : 1;             // bit 8  (v10.0.0.4) - CMND_SSPMDISPLAY - Enable gui display of powered on relays only
    uint32_t local_ntp_server : 1;         // bit 9  (v11.0.0.4) - CMND_RTCNTPSERVER - Enable local NTP server
    uint32_t influxdb_sensor : 1;          // bit 10 (v11.0.0.5) - CMND_IFXSENSOR - Enable sensor support in addition to teleperiod support
    uint32_t ex_serbridge_console : 1;     // bit 11 (v11.1.0.4) - (v14.1.0.2) Replaced by CMND_SSERIALMODE
    uint32_t telegram_disable_af : 1;      // bit 12 (v14.0.0.2) - CMND_TMSTATE 6/7 - Disable Telegram auto-fingerprint fix
    uint32_t dali_light : 1;               // bit 13 (v14.2.0.6) - CMND_DALILIGHT - Enable Tasmota light controls for DALI
    uint32_t spare14 : 1;                  // bit 14
    uint32_t spare15 : 1;                  // bit 15
    uint32_t spare16 : 1;                  // bit 16
    uint32_t spare17 : 1;                  // bit 17
    uint32_t spare18 : 1;                  // bit 18
    uint32_t spare19 : 1;                  // bit 19
    uint32_t spare20 : 1;                  // bit 20
    uint32_t spare21 : 1;                  // bit 21
    uint32_t spare22 : 1;                  // bit 22
    uint32_t spare23 : 1;                  // bit 23
    uint32_t spare24 : 1;                  // bit 24
    uint32_t spare25 : 1;                  // bit 25
    uint32_t spare26 : 1;                  // bit 26
    uint32_t spare27 : 1;                  // bit 27
    uint32_t spare28 : 1;                  // bit 28
    uint32_t spare29 : 1;                  // bit 29
    uint32_t spare30 : 1;                  // bit 30
    uint32_t spare31 : 1;                  // bit 31
  };
} SBitfield1;

typedef union {
  uint16_t data;
  struct {
    uint16_t hemis : 1;                    // bit 0        = 0=Northern, 1=Southern Hemisphere (=Opposite DST/STD)
    uint16_t week : 3;                     // bits 1 - 3   = 0=Last week of the month, 1=First, 2=Second, 3=Third, 4=Fourth
    uint16_t month : 4;                    // bits 4 - 7   = 1=Jan, 2=Feb, ... 12=Dec
    uint16_t dow : 3;                      // bits 8 - 10  = day of week, 1=Sun, 2=Mon, ... 7=Sat
    uint16_t hour : 5;                     // bits 11 - 15 = 0-23
  };
} TimeRule;

typedef union {
  uint32_t data;
  struct {
    uint32_t time : 11;                    // bits 0 - 10 = minutes in a day
    uint32_t window : 4;                   // bits 11 - 14 = minutes random window
    uint32_t repeat : 1;                   // bit 15
    uint32_t days : 7;                     // bits 16 - 22 = week day mask
    uint32_t device : 4;                   // bits 23 - 26 = 16 devices
    uint32_t power : 2;                    // bits 27 - 28 = 4 power states - Off, On, Toggle, Blink or Rule
    uint32_t mode : 2;                     // bits 29 - 30 = timer modes - 0 = Scheduler, 1 = Sunrise, 2 = Sunset
    uint32_t arm : 1;                      // bit 31
  };
} Timer;

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;
  struct {
    uint32_t stream : 1;
    uint32_t mirror : 1;
    uint32_t flip : 1;
    uint32_t rtsp : 1;
    uint32_t awb : 1;
    uint32_t awb_gain : 1;
    uint32_t aec : 1;
    uint32_t aec2 : 1;
    uint32_t agc : 1;
    uint32_t raw_gma : 1;
    uint32_t lenc : 1;
    uint32_t colorbar : 1;
    uint32_t wpc : 1;
    uint32_t dcw : 1;
    uint32_t bpc : 1;
    uint32_t spare15 : 1;
    uint32_t spare16 : 1;
    uint32_t feature : 2;
    uint32_t contrast : 3;
    uint32_t brightness : 3;
    uint32_t saturation : 3;
    uint32_t resolution : 4;
  };
} WebCamCfg;

typedef union {
  uint32_t data;
  struct {
    uint32_t wb_mode : 3;
    uint32_t ae_level : 3;
    uint32_t aec_value : 11;
    uint32_t gainceiling : 3;
    uint32_t agc_gain: 5;
    uint32_t special_effect : 3;
    uint32_t auth : 1;
    uint32_t spare29 : 1;
    uint32_t spare30 : 1;
    uint32_t upgraded : 1;
  };
} WebCamCfg2;

typedef union {
  uint16_t data;
  struct {
    uint16_t pinmode : 3;                   // Pin mode (1 through 6)
    uint16_t pullup : 1;                    // Enable internal weak pull-up resistor
    uint16_t saved_state : 1;               // Save output state, if used.
    uint16_t int_report_mode : 2;           // Interrupt reporting mode 0 = immediate telemetry & event, 1 = immediate event only, 2 = immediate telemetry only
    uint16_t int_report_defer : 4;          // Number of interrupts to ignore until reporting (default 0, max 15)
    uint16_t int_count_en : 1;              // Enable interrupt counter for this pin
    uint16_t int_retain_flag : 1;           // Report if interrupt occured for pin in next teleperiod
    uint16_t keep_output : 1;               // For output modes, preserve the value currently in the MCP230xx
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} Mcp230xxCfg;

typedef union {
  uint8_t data;
  struct {
    uint8_t spare0 : 1;
    uint8_t flowratemeter_unit : 1;        // Sensor96 9,x - unit l/min (0) or m³/h (1)
    uint8_t bh1750_2_resolution : 2;
    uint8_t bh1750_1_resolution : 2;       // Sensor10 1,2,3
    uint8_t hx711_json_weight_change : 1;  // Sensor34 8,x - Enable JSON message on weight change
    uint8_t mhz19b_abc_disable : 1;        // Disable ABC (Automatic Baseline Correction for MHZ19(B) (0 = Enabled (default), 1 = Disabled with Sensor15 command)
  };
} SensorCfg1;

typedef union {
  uint8_t data;
  struct {
  uint8_t pwm_count : 3;              // Number of PWMs to use for light
  uint8_t spare3 : 1;
  uint8_t spare4 : 1;
  uint8_t spare5 : 1;
  uint8_t spare6 : 1;
  uint8_t spare7 : 1;
  };
} PWMDimmerCfg;

typedef union {
  uint8_t data;
  struct {
  uint8_t nf_autotune : 1;            // Autotune the NF Noise Level
  uint8_t dist_autotune : 1;          // Autotune Disturber on/off
  uint8_t nf_autotune_both : 1;       // Autotune over both Areas: INDOORS/OUDOORS
  uint8_t mqtt_only_Light_Event : 1;  // mqtt only if lightning Irq
  uint8_t suppress_irq_no_Event : 1;  // suppress mqtt "IRQ with no Event". (Chip Bug)
  uint8_t spare5 : 1;
  uint8_t spare6 : 1;
  uint8_t spare7 : 1;
  };
} As3935IntCfg;

typedef union {
  uint16_t data;
  struct {
  uint16_t nf_autotune_time : 4;            // NF Noise Autotune Time
  uint16_t dist_autotune_time : 4;          // Disturber Autotune Time
  uint16_t nf_autotune_min : 4;             // Min Stages
  uint16_t spare3 : 4;
  };
} As3935Param;

typedef union {
  uint32_t data;
  struct {
  uint32_t raw_skip : 8;               // raw frame to skip when sending raw data (set to 2 means send 1 frame, then skip 2, ...)
  uint32_t raw_report_changed : 1;     // Report only changed values in raw frames (only valid if raw_skip=0)
  uint32_t raw_send : 1;               // Enable sending also real time raw data over MQTT
  uint32_t raw_limit : 1;              // Limit raw data to minimal relevant fields (the ones moving quickly)
  uint32_t mode_standard : 1;          // Set Linky Standard Mode (9600 bps stream) else legacy (1200 bps)
  uint32_t show_stats : 1;             // Display frames stats informations on WEB interface
  uint32_t spare1_1 : 1;               // Keep some spares for future uses
  uint32_t spare1_2 : 1;               // Keep some spares for future uses
  uint32_t spare1_3 : 1;               // Keep some spares for future uses
  uint32_t spare8_1 : 8;               // Keep some spares for future uses
  uint32_t spare8_2 : 8;               // Keep some spares for future uses
  };
} TeleinfoCfg;

typedef struct {
  uint32_t usage1_kWhtotal;
  uint32_t usage2_kWhtotal;
  uint32_t return1_kWhtotal;
  uint32_t return2_kWhtotal;
  uint32_t last_return_kWhtotal;
  uint32_t last_usage_kWhtotal;
} EnergyUsage;

typedef struct {
  uint8_t fnid = 0;
  uint8_t dpid = 0;
} TuyaFnidDpidMap;

typedef union {
  uint8_t data;
  struct {
  uint8_t type : 3;
  uint8_t invert : 1;
  uint8_t spare4 : 1;
  uint8_t spare5 : 1;
  uint8_t spare6 : 1;
  uint8_t spare7 : 1;
  };
} DisplayOptions;

const uint32_t settings_text_size = 699;   // Settings->text_pool[size] = Settings->display_model (2D2) - Settings->text_pool (017)
const uint8_t MAX_TUYA_FUNCTIONS = 16;
const uint8_t PARAM8_SIZE = 18;            // Number of param bytes (SetOption)

typedef struct {
  uint16_t      cfg_holder;                // 000  v6 header
  uint16_t      cfg_size;                  // 002
  uint32_t      save_flag;                 // 004
  uint32_t      version;                   // 008
  uint16_t      bootcount;                 // 00C
  uint16_t      cfg_crc;                   // 00E
  SOBitfield    flag;                      // 010
  int16_t       save_data;                 // 014
  int8_t        timezone;                  // 016

  // Start of char array storing all parameter strings ********

  char          text_pool[138];            // 017  Size is settings_text_size
  char          ex_sta_ssid[2][33];        // 0A1
  char          ex_sta_pwd[2][65];         // 0E3
  uint8_t       text_pool_165[132];        // 165
  char          ex_mqtt_host[33];          // 1E9
  uint8_t       text_pool_20A[35];         // 20A
  char          ex_mqtt_user[33];          // 22D
  char          ex_mqtt_pwd[33];           // 24E
  char          ex_mqtt_topic[33];         // 26F
  uint8_t       text_pool_290[66];         // 290

  // End of single char array of 698 chars max ****************

  uint8_t       display_model;             // 2D2
  uint8_t       display_mode;              // 2D3
  uint8_t       display_refresh;           // 2D4
  uint8_t       display_rows;              // 2D5
  uint8_t       display_cols[2];           // 2D6
  uint8_t       display_address[8];        // 2D8
  int8_t        display_dimmer_protected;  // 2E0 - if positive range 0..15, if negative range 0..100 (neg) - don't use directly
  uint8_t       display_size;              // 2E1
  TimeRule      tflag[2];                  // 2E2
  uint16_t      pwm_frequency;             // 2E6
  power_t       power;                     // 2E8
  uint16_t      pwm_value[MAX_PWMS_LEGACY];// 2EC
  int16_t       altitude;                  // 2F6
  uint16_t      tele_period;               // 2F8
  uint8_t       display_rotate;            // 2FA
  uint8_t       ledstate;                  // 2FB
  uint8_t       param[PARAM8_SIZE];        // 2FC  SetOption32 .. SetOption49
  int16_t       toffset[2];                // 30E
  uint8_t       display_font;              // 312
  DisplayOptions  display_options;         // 313
  int32_t       energy_kWhtoday_ph[3];     // 314
  int32_t       energy_kWhyesterday_ph[3]; // 320
  int32_t       energy_kWhtotal_ph[3];     // 32C
  int32_t       weight_user_tare;          // 338
  uint8_t       web_time_start;            // 33C
  uint8_t       web_time_end;              // 33D
  uint8_t       sserial_config;            // 33E
  uint8_t       tuyamcu_topic;             // 33F  Manage tuyaSend topic. ex_energy_power_delta on 6.6.0.20, replaced on 8.5.0.1
  uint16_t      domoticz_update_timer;     // 340
  uint16_t      pwm_range;                 // 342
#ifndef CONFIG_IDF_TARGET_ESP32P4
  uint32_t      domoticz_relay_idx[MAX_DOMOTICZ_IDX];  // 344
  uint32_t      domoticz_key_idx[MAX_DOMOTICZ_IDX];    // 354
#endif  // CONFIG_IDF_TARGET_ESP32P4
  uint32_t      energy_power_calibration;    // 364 (P4 344)
  uint32_t      energy_voltage_calibration;  // 368 (P4 348)
  uint32_t      energy_current_calibration;  // 36C (P4 34C)
  uint32_t      energy_power_calibration2;   // 370 (P4 350) - ex_energy_kWhtoday
  uint32_t      energy_voltage_calibration2; // 374 (P4 354) - ex_energy_kWhyesterday
  uint32_t      energy_current_calibration2; // 378 (P4 358) - ex_energy_kWhdoy, ex_energy_min_power
  uint16_t      energy_max_power;          // 37C (P4 35C)
  uint16_t      energy_min_voltage;        // 37E (P4 35E)
  uint16_t      energy_max_voltage;        // 380 (P4 360)
  uint16_t      energy_min_current;        // 382 (P4 362)
  uint16_t      energy_max_current;        // 384 (P4 364)
  uint16_t      energy_max_power_limit;                 // 386 (P4 366)  MaxPowerLimit
  uint16_t      energy_max_power_limit_hold;            // 388 (P4 368)  MaxPowerLimitHold
  uint16_t      energy_max_power_limit_window;          // 38A (P4 36A)  MaxPowerLimitWindow
  uint16_t      ex_energy_max_power_safe_limit;         // 38C (P4 36C)  MaxSafePowerLimit - Free since 14.1.0.3
  uint16_t      ex_energy_max_power_safe_limit_hold;    // 38E (P4 36E)  MaxSafePowerLimitHold - Free since 14.1.0.3
  uint16_t      ex_energy_max_power_safe_limit_window;  // 390 (P4 370)  MaxSafePowerLimitWindow - Free since 14.1.0.3
  uint16_t      energy_max_energy;         // 392 (P4 372)  MaxEnergy
  uint16_t      energy_max_energy_start;   // 394 (P4 374)  MaxEnergyStart
  uint16_t      mqtt_retry;                // 396 (P4 376)
  uint8_t       poweronstate;              // 398 (P4 378)
  uint8_t       last_module;               // 399 (P4 379)
  uint16_t      blinktime;                 // 39A (P4 37A)
  uint16_t      blinkcount;                // 39C (P4 37C)
  uint16_t      light_rotation;            // 39E (P4 37E)
  SOBitfield3   flag3;                     // 3A0 (P4 380)
  uint16_t      energy_kWhdoy;             // 3A4 (P4 384)
  uint16_t      energy_min_power;          // 3A6 (P4 386)
  uint32_t      pn532_password;            // 3A8 (P4 388) - ex_switchmode4-7, Free since 9.2.0.6

#ifdef ESP8266
  // --------------------------------------------------------------
  // Mapping 0x3AC to 0x496 for ESP8266
  // --------------------------------------------------------------
  myio          my_gp;                     // 3AC  2x18 bytes (ESP8266)
  uint16_t      gpio16_converted;          // 3D0

  uint8_t       free_esp8266_3D2[42];      // 3D2

  mytmplt       user_template;             // 3FC  2x15 bytes (ESP8266)

  uint8_t       free_esp8266_41A[55];      // 41A

  char          serial_delimiter;          // 451
  uint8_t       seriallog_level;           // 452
  uint8_t       sleep;                     // 453
  uint16_t      domoticz_switch_idx[MAX_DOMOTICZ_IDX];      // 454
  uint16_t      domoticz_sensor_idx[MAX_DOMOTICZ_SNS_IDX];  // 45C
  uint8_t       module;                    // 474
  uint8_t       ws_color[4][3];            // 475
  uint8_t       ws_width[3];               // 481
  myio8         ex_my_gp8;                 // 484 17 bytes (ESP8266) - Free since 9.0.0.1
  uint8_t       ex_my_adc0;                // 495  Free since 9.0.0.1 - Do not use anymore because of ESP32S3
                                           // 496
#endif  // ESP8266
#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32P4
  // --------------------------------------------------------------
  // Mapping 0x38C to 0x496 for ESP32P4 (Domoticz in filesystem)
  // --------------------------------------------------------------
  myio          my_gp;                     // 38C  2x55 bytes (ESP32-P4)
  uint8_t       eth_type;                  // 3FA
  uint8_t       eth_clk_mode;              // 3FB
  mytmplt       user_template;             // 3FC  2x56 bytes (ESP32-P4)
  WebCamCfg     webcam_config;             // 46C
  int8_t        eth_address;               // 470
  uint8_t       module;                    // 471
  uint8_t       ws_width[3];               // 472
  char          serial_delimiter;          // 475
  uint8_t       seriallog_level;           // 476
  uint8_t       sleep;                     // 477
  uint8_t       ws_color[4][3];            // 478  (+xC)

  uint8_t       free_esp32_484[18];        // 484
                                           // 496
#elif CONFIG_IDF_TARGET_ESP32S3
  // --------------------------------------------------------------
  // Mapping 0x3AC to 0x496 for ESP32S3
  // --------------------------------------------------------------
  myio          my_gp;                     // 3AC  2x49 bytes (ESP32-S3)
  uint8_t       eth_type;                  // 40E
  uint8_t       eth_clk_mode;              // 40F
  mytmplt       user_template;             // 410  2x39 bytes (ESP32-S3)
  int8_t        eth_address;               // 45E
  uint8_t       module;                    // 45F
  WebCamCfg     webcam_config;             // 460
  uint8_t       ws_width[3];               // 464
  char          serial_delimiter;          // 467
  uint8_t       seriallog_level;           // 468
  uint8_t       sleep;                     // 469
  uint16_t      domoticz_switch_idx[MAX_DOMOTICZ_IDX];      // 46A (+8)
  uint16_t      domoticz_sensor_idx[MAX_DOMOTICZ_SNS_IDX];  // 472 (+x18)
  uint8_t       ws_color[4][3];            // 48A  (+xC)
                                           // 496
#elif CONFIG_IDF_TARGET_ESP32S2
  // --------------------------------------------------------------
  // Mapping 0x3AC to 0x496 for ESP32S2
  // --------------------------------------------------------------
  myio          my_gp;                     // 3AC  2x47 bytes (ESP32-S2)
  mytmplt       user_template;             // 40A  2x37 bytes (ESP32-S2)
  uint8_t       eth_type;                  // 454
  uint8_t       eth_clk_mode;              // 455

  uint8_t       free_esp32s2_456[6];       // 456

  WebCamCfg     webcam_config;             // 45C
  int8_t        eth_address;               // 460
  char          serial_delimiter;          // 461
  uint8_t       seriallog_level;           // 462
  uint8_t       sleep;                     // 463
  uint16_t      domoticz_switch_idx[MAX_DOMOTICZ_IDX];      // 464
  uint16_t      domoticz_sensor_idx[MAX_DOMOTICZ_SNS_IDX];  // 46C
  uint8_t       module;                    // 484
  uint8_t       ws_color[4][3];            // 485
  uint8_t       ws_width[3];               // 491

  uint8_t       free_esp32s2_494[2];       // 494
                                           // 496
#else
  // -----------------------------------------------------------------------
  // Mapping 0x3AC to 0x496 for ESP32, ESP32C2, ESP32C3, ESP32C5 and ESP32C6
  // -----------------------------------------------------------------------
  myio          my_gp;                     // 3AC  2x40 bytes (ESP32) / 2x21 bytes (ESP32-C2) / 2x22 bytes (ESP32-C3) / 2x29 bytes (ESP32-C5) / 2x31 bytes (ESP32-C6)

  #if CONFIG_IDF_TARGET_ESP32C2
  uint8_t       free_esp32c2_3D6[38];      // 3D6  - Due to smaller myio
  #elif CONFIG_IDF_TARGET_ESP32C3
  uint8_t       free_esp32c3_3D8[36];      // 3D8  - Due to smaller myio
  #elif CONFIG_IDF_TARGET_ESP32C5
  uint8_t       free_esp32c5_3E4[22];      // 3E4  - Due to smaller myio
  #elif CONFIG_IDF_TARGET_ESP32C6
  uint8_t       free_esp32c6_3EA[18];      // 3EA  - Due to smaller myio
  #endif  // CONFIG_IDF_TARGET_ESP32C2/3/5/6

  mytmplt       user_template;             // 3FC  2x37 bytes (ESP32) / 2x22 bytes (ESP32-C2) / 2x23 bytes (ESP32-C3) / 2x30 bytes (ESP32-C5) / 2x32 bytes (ESP32-C6)

  #if CONFIG_IDF_TARGET_ESP32C2
  uint8_t       free_esp32c2_428[30];      // 428  - Due to smaller mytmplt
  #elif CONFIG_IDF_TARGET_ESP32C3
  uint8_t       free_esp32c3_42A[28];      // 42A  - Due to smaller mytmplt
  #elif CONFIG_IDF_TARGET_ESP32C5
  uint8_t       free_esp32c5_438[14];      // 438  - Due to smaller mytmplt
  #elif CONFIG_IDF_TARGET_ESP32C6 
  uint8_t       free_esp32c6_43C[10];      // 43C  - Due to smaller mytmplt
  #endif  // CONFIG_IDF_TARGET_ESP32C2/3/5/6

  uint8_t       eth_type;                  // 446
  uint8_t       eth_clk_mode;              // 447

  uint8_t       free_esp32_448[4];         // 448

  WebCamCfg     webcam_config;             // 44C
  int8_t        eth_address;               // 450
  char          serial_delimiter;          // 451
  uint8_t       seriallog_level;           // 452
  uint8_t       sleep;                     // 453
  uint16_t      domoticz_switch_idx[MAX_DOMOTICZ_IDX];      // 454
  uint16_t      domoticz_sensor_idx[MAX_DOMOTICZ_SNS_IDX];  // 45C
  uint8_t       module;                    // 474
  uint8_t       ws_color[4][3];            // 475
  uint8_t       ws_width[3];               // 481

  uint8_t       free_esp32_484[18];        // 484
                                           // 496
#endif  // ESP32 Mapping
#endif  // ESP32
  // --------------------------------------------------------------
  // End of CPU specific Mapping
  // --------------------------------------------------------------

  uint16_t      light_pixels : 15;         // 496
  uint16_t      light_pixels_reverse : 1;  // 496
  uint8_t       light_color[LST_MAX];      // 498  LST_MAX = 5
  uint8_t       light_correction;          // 49D
  uint8_t       light_dimmer;              // 49E
  uint8_t       rule_enabled;              // 49F
  uint8_t       rule_once;                 // 4A0  Bit 6+7 used by xdrv_10_scripter
  uint8_t       light_fade;                // 4A1
  uint8_t       light_speed;               // 4A2
  uint8_t       light_scheme;              // 4A3
  uint8_t       light_width;               // 4A4
  uint8_t       knx_GA_registered;         // 4A5  Number of Group Address to read
  uint16_t      light_wakeup;              // 4A6
  uint8_t       switchmode[MAX_SWITCHES_SET];  // 4A8 4A9 - Moved dn by 1 byte in v14.3.0.7
  uint16_t      dns_timeout;               // 4C8
  uint8_t       ds3502_state[MAX_DS3502];  // 4CA
  uint16_t      influxdb_port;             // 4CE
  power_t       interlock[MAX_INTERLOCKS_SET];  // 4D0 MAX_INTERLOCKS = MAX_RELAYS / 2
  int8_t        shutter_tilt_config[5][MAX_SHUTTERS];  // 510 508 - - Moved up by 8 bytes in v14.3.0.7
  int32_t       weight_absconv_a;          // 524
  int32_t       weight_absconv_b;          // 528
  uint16_t      mqtt_keepalive;            // 52C
  uint16_t      mqtt_socket_timeout;       // 52E
  uint8_t       mqtt_wifi_timeout;         // 530
  uint8_t       ina219_mode;               // 531
  uint8_t       weight_precision;          // 532  ex_pulse_timer free since 11.0.0.3
  uint8_t       knx_CB_registered;         // 533 4A8  Number of Group Address to write
  int8_t        shutter_tilt_pos[MAX_SHUTTERS];  // 534 51C
  uint16_t      influxdb_period;           // 538 520
  uint16_t      rf_duplicate_time;         // 53A 522
  uint8_t       global_sensor_index[3];    // 53C 4C5
  uint8_t       filelog_level;             // 53F
  uint16_t      tcp_baudrate;              // 540 
  uint16_t      button_debounce;           // 542
  uint32_t      ipv4_address[5];           // 544
  uint32_t      ipv4_rgx_address;          // 558
  uint32_t      ipv4_rgx_subnetmask;       // 55C
  uint16_t      pwm_value_ext[16-5];       // 560  Extension to pwm_value to store up to 16 PWM for ESP32. This array stores values 5..15
  uint16_t      pn532_pack;                // 576
  int32_t       weight_offset;             // 578
  uint16_t      pulse_timer[MAX_PULSETIMERS];  // 57C
  SysMBitfield1 flag2;                     // 5BC
  uint32_t      pulse_counter[MAX_COUNTERS];  // 5C0
  uint16_t      pulse_counter_type;        // 5D0
  uint16_t      pulse_counter_debounce;    // 5D2
  uint8_t       rf_code[17][9];            // 5D4
  uint8_t       timezone_minutes;          // 66D
  uint16_t      switch_debounce;           // 66E
  Timer         timer[MAX_TIMERS];         // 670
  int           latitude;                  // 6B0
  int           longitude;                 // 6B4
  uint16_t      knx_physsical_addr;        // 6B8  (address_t is a uint16_t)
  uint16_t      knx_GA_addr[MAX_KNX_GA];   // 6BA  (address_t is a uint16_t) x KNX_max_GA
  uint16_t      knx_CB_addr[MAX_KNX_CB];   // 6CE  (address_t is a uint16_t) x KNX_max_CB
  uint8_t       knx_GA_param[MAX_KNX_GA];  // 6E2  Type of Input (relay changed, button pressed, sensor read <-teleperiod)
  uint8_t       knx_CB_param[MAX_KNX_CB];  // 6EC  Type of Output (set relay, toggle relay, reply sensor value)
  Mcp230xxCfg   mcp230xx_config[16];       // 6F6
  uint8_t       mcp230xx_int_prio;         // 716
  SensorCfg1    SensorBits1;               // 717  On/Off settings used by Sensor Commands
  uint16_t      mcp230xx_int_timer;        // 718
  uint8_t       rgbwwTable[5];             // 71A
  uint8_t       user_template_base;        // 71F
  char          user_template_name[15];    // 720  15 bytes - Backward compatibility since v8.2.0.3

#ifdef ESP8266
  uint8_t       ex_user_template8[5];      // 72F  14 bytes (ESP8266) - Free since 9.0.0.1 - only 5 bytes referenced now
#endif  // ESP8266
#ifdef ESP32
  uint8_t       webcam_clk;                // 72F
  WebCamCfg2    webcam_config2;            // 730
#endif  // ESP32
  uint16_t      artnet_universe;           // 734
  uint16_t      modbus_sbaudrate;          // 736
  uint16_t      shutter_motorstop;         // 738
  uint8_t       battery_level_percent;     // 73A
  uint8_t       hdmi_addr[2];              // 73B  HDMI CEC physical address - warning this is a non-aligned uint16
  uint8_t       novasds_startingoffset;    // 73D
  uint8_t       web_color[18][3];          // 73E
  uint16_t      display_width;             // 774
  uint16_t      display_height;            // 776
  uint16_t      baudrate;                  // 778
  uint16_t      sbaudrate;                 // 77A
  EnergyUsage   energy_usage;              // 77C
  uint32_t      sensors[2][4];             // 794  Disable individual (0) sensor drivers / (1) GUI sensor output
  uint32_t      energy_kWhtotal_time;      // 7B4
  uint32_t      weight_item;               // 7B8  Weight of one item in gram * 10
  uint16_t      ledmask;                   // 7BC
  uint16_t      weight_max;                // 7BE  Total max weight in kilogram
  uint32_t      weight_reference;          // 7C0  Reference weight in gram
  uint32_t      weight_calibration;        // 7C4
  uint32_t      energy_frequency_calibration;  // 7C8  Also used by HX711 to save last weight
  uint16_t      web_refresh;               // 7CC
  char          script_pram[5][10];        // 7CE
  char          rules[MAX_RULE_SETS][MAX_RULE_SIZE];  // 800  Uses 512 bytes in v5.12.0m, 3 x 512 bytes in v5.14.0b
  TuyaFnidDpidMap tuya_fnid_map[MAX_TUYA_FUNCTIONS];  // E00  32 bytes
  uint16_t      ina226_r_shunt[4];         // E20
  uint16_t      ina226_i_fs[4];            // E28
  uint16_t      tariff[4][2];              // E30
  uint16_t      shutter_opentime[MAX_SHUTTERS];      // E40
  uint16_t      shutter_closetime[MAX_SHUTTERS];     // E48
  int16_t       shuttercoeff[5][MAX_SHUTTERS];       // E50
  uint8_t       shutter_options[MAX_SHUTTERS];       // E78
  uint8_t       shutter_set50percent[MAX_SHUTTERS];  // E7C
  uint8_t       shutter_position[MAX_SHUTTERS];      // E80
  uint8_t       shutter_startrelay[MAX_SHUTTERS];    // E84
  uint8_t       pcf8574_config[MAX_PCF8574];         // E88
  uint8_t       ot_hot_water_setpoint;     // E8C
  uint8_t       ot_boiler_setpoint;        // E8D
  uint8_t       ot_flags;                  // E8E
  uint8_t       ledpwm_mask;               // E8F
  uint16_t      dimmer_hw_min;             // E90
  uint16_t      dimmer_hw_max;             // E92
  uint32_t      deepsleep;                 // E94
  uint16_t      hass_new_discovery;        // E98  ex2_energy_power_delta on 8.4.0.3, replaced on 8.5.0.1
  uint8_t       shutter_motordelay[MAX_SHUTTERS];    // E9A
  int8_t        temp_comp;                 // E9E
  uint8_t       weight_change;             // E9F
  uint8_t       web_color2[2][3];          // EA0  Needs to be on integer / 3 distance from web_color
  uint16_t      zcdimmerset[5];            // EA6
  uint8_t       free_eb0[20];              // EB0  20 bytes
  uint16_t      light_pixels_height_1 : 15;// EC4  Pixels height minus 1, default 0 (0 means 1 line)
  uint16_t      light_pixels_alternate : 1;// EC4  Indicates alternate lines in Pixels Matrix
  uint8_t       shift595_device_count;     // EC6
  uint8_t       sta_config;                // EC7
  uint8_t       sta_active;                // EC8
  uint8_t       rule_stop;                 // EC9
  uint16_t      syslog_port;               // ECA
  uint8_t       syslog_level;              // ECC
  uint8_t       webserver;                 // ECD
  uint8_t       weblog_level;              // ECE
  uint8_t       mqtt_fingerprint[2][20];   // ECF
  uint8_t       influxdb_version;          // EF7
  SOBitfield4   flag4;                     // EF8
  uint16_t      mqtt_port;                 // EFC
  uint8_t       serial_config;             // EFE
  uint8_t       wifi_output_power;         // EFF
  uint8_t       shutter_accuracy;          // F00
  uint8_t       mqttlog_level;             // F01
  uint8_t       sps30_inuse_hours;         // F02
  uint8_t       hotplug_scan;              // F03
  uint8_t       bri_power_on;              // F04
  PWMDimmerCfg  pwm_dimmer_cfg;            // F05
  uint8_t       bri_preset_low;            // F06
  uint8_t       bri_preset_high;           // F07
  int8_t        hum_comp;                  // F08
  uint8_t       wifi_channel;              // F09
  uint8_t       wifi_bssid[6];             // F0A
  uint8_t       as3935_sensor_cfg[5];      // F10
  As3935IntCfg  as3935_functions;          // F15
  As3935Param   as3935_parameter;          // F16
  uint64_t      zb_ext_panid;              // F18
  uint64_t      zb_precfgkey_l;            // F20
  uint64_t      zb_precfgkey_h;            // F28
  uint16_t      zb_pan_id;                 // F30
  uint8_t       zb_channel;                // F32
  int8_t        zb_txradio_dbm;            // F33
  uint16_t      pms_wake_interval;         // F34
  uint8_t       config_version;            // F36  Setting mem layout identifier for different ESP types - don't move!
  uint8_t       windmeter_pulses_x_rot;    // F37
  uint16_t      windmeter_radius;          // F38
  uint16_t      windmeter_pulse_debounce;  // F3A
  int16_t       windmeter_speed_factor;    // F3C
  uint8_t       windmeter_tele_pchange;    // F3E
  uint8_t       ledpwm_on;                 // F3F
  uint8_t       ledpwm_off;                // F40
  uint8_t       sserial_mode;              // F41
  uint8_t       fallback_module;           // F42
  uint8_t       shutter_mode;              // F43
  uint16_t      energy_power_delta[3];     // F44
  uint16_t      shutter_pwmrange[2][MAX_SHUTTERS];  // F4A
  uint8_t       dimmer_step;               // F5A
  uint8_t       shd_leading_edge;          // F5B
  uint16_t      shd_warmup_brightness;     // F5C
  uint8_t       shd_warmup_time;           // F5E
  uint8_t       tcp_config;                // F5F
  uint8_t       light_step_pixels;				 // F60
  uint8_t       hdmi_cec_device_type;      // F61  - v13.1.0.1 (was ex_modbus_sbaudrate v12.2.0.5)
  uint8_t       modbus_sconfig;            // F62
  uint8_t       windmeter_measure_intvl;   // F63

  uint8_t       free_f64[8];               // F64 - Decrement if adding new Setting variables just above and below

  // Only 32 bit boundary variables below
  float         ms5837_pressure_offset;    // F6C
  uint32_t      touch_threshold;           // F70
  SOBitfield6   flag6;                     // F74
  uint16_t      flowratemeter_calibration[2];// F78
  int32_t       energy_kWhexport_ph[3];    // F7C
  uint32_t      eth_ipv4_address[5];       // F88
  uint32_t      power_lock;                // F9C
  SBitfield1    sbflag1;                   // FA0
  TeleinfoCfg   teleinfo;                  // FA4
  uint64_t      rf_protocol_mask;          // FA8
  uint8_t       device_group_tie[4];       // FB0
  SOBitfield5   flag5;                     // FB4
  uint16_t      pulse_counter_debounce_low;   // FB8
  uint16_t      pulse_counter_debounce_high;  // FBA
  uint32_t      keeloq_master_msb;         // FBC
  uint32_t      keeloq_master_lsb;         // FC0
  uint32_t      keeloq_serial;             // FC4
  uint32_t      keeloq_count;              // FC8
  uint32_t      device_group_share_in;     // FCC  Bitmask of device group items imported
  uint32_t      device_group_share_out;    // FD0  Bitmask of device group items exported
  uint32_t      bootcount_reset_time;      // FD4
  SysMBitfield2 mbflag2;                   // FD8
  uint32_t      shutter_button[MAX_SHUTTER_KEYS];  // FDC
  uint32_t      i2c_drivers[3];            // FEC  I2cDriver
  uint32_t      cfg_timestamp;             // FF8
  uint32_t      cfg_crc32;                 // FFC
} TSettings;

static_assert(sizeof(TSettings) == 4096, "TSettings Size is not correct");



typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint16_t data;                           // Allow bit manipulation
  struct {
    uint16_t system_init : 1;              // Changing layout here needs adjustments in xdrv_10_rules.ino too
    uint16_t system_boot : 1;
    uint16_t time_init : 1;
    uint16_t time_set : 1;
    uint16_t mqtt_connected : 1;
    uint16_t mqtt_disconnected : 1;
    uint16_t wifi_connected : 1;
    uint16_t wifi_disconnected : 1;
    uint16_t eth_connected : 1;
    uint16_t eth_disconnected : 1;
    uint16_t http_init : 1;
    uint16_t shutter_moved : 1;
    uint16_t shutter_moving : 1;
    uint16_t spare13 : 1;
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} RulesBitfield;

typedef union {
  uint8_t data;
  struct {
    uint8_t network_down : 1;
    uint8_t mqtt_down : 1;
    uint8_t wifi_down : 1;
    uint8_t eth_down : 1;
    uint8_t spare04 : 1;
    uint8_t spare05 : 1;
    uint8_t spare06 : 1;
    uint8_t spare07 : 1;
  };
} StateBitfield;

// See issue https://github.com/esp8266/Arduino/issues/2913
#ifdef ESP8266
#ifdef USE_ADC_VCC
  ADC_MODE(ADC_VCC);                       // Set ADC input for Power Supply Voltage usage
#endif
#endif

#endif  // _TASMOTA_TYPES_H_
