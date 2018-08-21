/*
  settings.h - setting variables for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define PARAM8_SIZE  18                    // Number of param bytes (SetOption)

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so usefull...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint32_t save_state : 1;               // bit 0
    uint32_t button_restrict : 1;          // bit 1
    uint32_t value_units : 1;              // bit 2
    uint32_t mqtt_enabled : 1;             // bit 3
    uint32_t mqtt_response : 1;            // bit 4
    uint32_t mqtt_power_retain : 1;        // CMND_POWERRETAIN
    uint32_t mqtt_button_retain : 1;       // CMND_BUTTONRETAIN
    uint32_t mqtt_switch_retain : 1;       // CMND_SWITCHRETAIN
    uint32_t temperature_conversion : 1;   // bit 8
    uint32_t mqtt_sensor_retain : 1;       // CMND_SENSORRETAIN
    uint32_t mqtt_offline : 1;             // bit 10
    uint32_t button_swap : 1;              // bit 11 (v5.1.6)
    uint32_t stop_flash_rotate : 1;        // bit 12 (v5.2.0)
    uint32_t button_single : 1;            // bit 13 (v5.4.0)
    uint32_t interlock : 1;                // bit 14 (v5.6.0)
    uint32_t pwm_control : 1;              // bit 15 (v5.8.1)
    uint32_t ws_clock_reverse : 1;         // bit 16 (v5.8.1)
    uint32_t decimal_text : 1;             // bit 17 (v5.8.1)
    uint32_t light_signal : 1;             // bit 18 (v5.10.0c)
    uint32_t hass_discovery : 1;           // bit 19 (v5.11.1a)
    uint32_t not_power_linked : 1;         // bit 20 (v5.11.1f)
    uint32_t no_power_on_check : 1;        // bit 21 (v5.11.1i)
    uint32_t mqtt_serial : 1;              // bit 22 (v5.12.0f)
    uint32_t mqtt_serial_raw : 1;          // bit 23 (v6.1.1c)
    uint32_t rules_once : 1;               // bit 24 (v5.12.0k) - free since v5.14.0b
    uint32_t knx_enabled : 1;              // bit 25 (v5.12.0l) KNX
    uint32_t device_index_enable : 1;      // bit 26 (v5.13.1a)
    uint32_t knx_enable_enhancement : 1;   // bit 27 (v5.14.0a) KNX
    uint32_t rf_receive_decimal : 1;       // bit 28 (v6.0.0a)
    uint32_t ir_receive_decimal : 1;       // bit 29 (v6.0.0a)
    uint32_t hass_light : 1;               // bit 30 (v6.0.0b)
    uint32_t global_state : 1;             // bit 31 (v6.1.0)
  };
} SysBitfield;

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so usefull...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption50 .. SetOption81
    uint32_t timers_enable : 1;            // bit 0 (v6.1.1b)
    uint32_t spare01 : 1;
    uint32_t spare02 : 1;
    uint32_t spare03 : 1;
    uint32_t spare04 : 1;
    uint32_t spare05 : 1;
    uint32_t spare06 : 1;
    uint32_t spare07 : 1;
    uint32_t spare08 : 1;
    uint32_t spare09 : 1;
    uint32_t spare10 : 1;
    uint32_t spare11 : 1;
    uint32_t spare12 : 1;
    uint32_t spare13 : 1;
    uint32_t spare14 : 1;
    uint32_t spare15 : 1;
    uint32_t spare16 : 1;
    uint32_t spare17 : 1;
    uint32_t spare18 : 1;
    uint32_t spare19 : 1;
    uint32_t spare20 : 1;
    uint32_t spare21 : 1;
    uint32_t spare22 : 1;
    uint32_t spare23 : 1;
    uint32_t spare24 : 1;
    uint32_t spare25 : 1;
    uint32_t spare26 : 1;
    uint32_t spare27 : 1;
    uint32_t spare28 : 1;
    uint32_t spare29 : 1;
    uint32_t spare30 : 1;
    uint32_t spare31 : 1;
  };
} SysBitfield3;

typedef union {
  uint32_t data;                           // Allow bit manipulation
  struct {
    uint32_t spare00 : 1;
    uint32_t spare01 : 1;
    uint32_t spare02 : 1;
    uint32_t spare03 : 1;
    uint32_t spare04 : 1;
    uint32_t spare05 : 1;
    uint32_t spare06 : 1;
    uint32_t spare07 : 1;
    uint32_t spare08 : 1;
    uint32_t spare09 : 1;
    uint32_t spare10 : 1;
    uint32_t spare11 : 1;
    uint32_t spare12 : 1;
    uint32_t axis_resolution : 2;
    uint32_t current_resolution : 2;
    uint32_t voltage_resolution : 2;
    uint32_t wattage_resolution : 2;
    uint32_t emulation : 2;
    uint32_t energy_resolution : 3;
    uint32_t pressure_resolution : 2;
    uint32_t humidity_resolution : 2;
    uint32_t temperature_resolution : 2;
  };
} SysBitfield2;

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

typedef union {
  uint16_t data;
  struct {
    uint16_t pinmode : 3;                   // Pin mode (1 through 6)
    uint16_t pullup : 1;                    // Enable internal weak pull-up resistor
    uint16_t saved_state : 1;               // Save output state, if used.
    uint16_t int_report_mode : 2;           // Interrupt reporting mode 0 = immediate telemetry & event, 1 = immediate event only, 2 = immediate telemetry only
    uint16_t int_report_defer : 4;          // Number of interrupts to ignore until reporting (default 0, max 15)
    uint16_t int_count_sec : 1;             // 0 = Not, 1 = Report number of interrupts since last FUNC_EVERY_SECOND callback if more than 0 occured (overrides int_report_mode if enabled)
    uint16_t int_count_min : 1;             // 0 = Not, 1 = Report number of interrupts since last FUNC_EVERY_SECOND*60 callback if more than 0 occured (overrides int_report_mode if enabled)
    uint16_t spare13 : 1;
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} Mcp230xxCfg;

/*
struct SYSCFG {
  unsigned long cfg_holder;                // 000 Pre v6 header
  unsigned long save_flag;                 // 004
  unsigned long version;                   // 008
  unsigned long bootcount;                 // 00C
*/
struct SYSCFG {
  uint16_t cfg_holder;                     // 000 v6 header
  uint16_t cfg_size;                       // 002
  unsigned long save_flag;                 // 004
  unsigned long version;                   // 008
  uint16_t bootcount;                      // 00C
  uint16_t cfg_crc;                        // 00E
  SysBitfield   flag;                      // 010
  int16_t       save_data;                 // 014
  int8_t        timezone;                  // 016
  char          ota_url[101];              // 017
  char          mqtt_prefix[3][11];        // 07C
  uint8_t       baudrate;                  // 09D
  byte          seriallog_level;           // 09E
  uint8_t       sta_config;                // 09F
  byte          sta_active;                // 0A0
  char          sta_ssid[2][33];           // 0A1
  char          sta_pwd[2][65];            // 0E3
  char          hostname[33];              // 165
  char          syslog_host[33];           // 186

  byte          free_1A7[1];               // 1A7

  uint16_t      syslog_port;               // 1A8
  byte          syslog_level;              // 1AA
  uint8_t       webserver;                 // 1AB
  byte          weblog_level;              // 1AC
  uint8_t       mqtt_fingerprint[2][20];   // 1AD

  byte          free_1D5[20];              // 1D5  Free since 5.12.0e

  char          mqtt_host[33];             // 1E9
  uint16_t      mqtt_port;                 // 20A
  char          mqtt_client[33];           // 20C
  char          mqtt_user[33];             // 22D
  char          mqtt_pwd[33];              // 24E
  char          mqtt_topic[33];            // 26F
  char          button_topic[33];          // 290
  char          mqtt_grptopic[33];         // 2B1
  uint8_t       display_model;             // 2D2
  uint8_t       display_mode;              // 2D3
  uint8_t       display_refresh;           // 2D4
  uint8_t       display_rows;              // 2D5
  uint8_t       display_cols[2];           // 2D6
  uint8_t       display_address[8];        // 2D8
  uint8_t       display_dimmer;            // 2E0
  uint8_t       display_size;              // 2E1
  TimeRule      tflag[2];                  // 2E2
  uint16_t      pwm_frequency;             // 2E6
  power_t       power;                     // 2E8
  uint16_t      pwm_value[MAX_PWMS];       // 2EC
  int16_t       altitude;                  // 2F6
  uint16_t      tele_period;               // 2F8
  uint8_t       display_rotate;            // 2FA
  uint8_t       ledstate;                  // 2FB
  uint8_t       param[PARAM8_SIZE];        // 2FC  SetOption32 .. SetOption49
  int16_t       toffset[2];                // 30E

  byte          free_312[1];               // 312

  char          state_text[4][11];         // 313
  uint8_t       energy_power_delta;        // 33F
  uint16_t      domoticz_update_timer;     // 340
  uint16_t      pwm_range;                 // 342
  unsigned long domoticz_relay_idx[MAX_DOMOTICZ_IDX];  // 344
  unsigned long domoticz_key_idx[MAX_DOMOTICZ_IDX];    // 354
  unsigned long energy_power_calibration;  // 364
  unsigned long energy_voltage_calibration;  // 368
  unsigned long energy_current_calibration;  // 36C
  unsigned long energy_kWhtoday;           // 370
  unsigned long energy_kWhyesterday;       // 374
  uint16_t      energy_kWhdoy;             // 378
  uint16_t      energy_min_power;          // 37A
  uint16_t      energy_max_power;          // 37C
  uint16_t      energy_min_voltage;        // 37E
  uint16_t      energy_max_voltage;        // 380
  uint16_t      energy_min_current;        // 382
  uint16_t      energy_max_current;        // 384
  uint16_t      energy_max_power_limit;    // 386 MaxPowerLimit
  uint16_t      energy_max_power_limit_hold;         // 388 MaxPowerLimitHold
  uint16_t      energy_max_power_limit_window;       // 38A MaxPowerLimitWindow
  uint16_t      energy_max_power_safe_limit;         // 38C MaxSafePowerLimit
  uint16_t      energy_max_power_safe_limit_hold;    // 38E MaxSafePowerLimitHold
  uint16_t      energy_max_power_safe_limit_window;  // 390 MaxSafePowerLimitWindow
  uint16_t      energy_max_energy;         // 392 MaxEnergy
  uint16_t      energy_max_energy_start;   // 394 MaxEnergyStart
  uint16_t      mqtt_retry;                // 396
  uint8_t       poweronstate;              // 398
  uint8_t       last_module;               // 399
  uint16_t      blinktime;                 // 39A
  uint16_t      blinkcount;                // 39C
  uint16_t      light_rotation;            // 39E
  SysBitfield3  flag3;                     // 3A0
  uint8_t       switchmode[MAX_SWITCHES];  // 3A4  (6.0.0b - moved from 0x4CA)
  char          friendlyname[MAX_FRIENDLYNAMES][33]; // 3AC
  char          switch_topic[33];          // 430
  char          serial_delimiter;          // 451
  uint8_t       sbaudrate;                 // 452
  uint8_t       sleep;                     // 453
  uint16_t      domoticz_switch_idx[MAX_DOMOTICZ_IDX];      // 454
  uint16_t      domoticz_sensor_idx[MAX_DOMOTICZ_SNS_IDX];  // 45C
  uint8_t       module;                    // 474
  uint8_t       ws_color[4][3];            // 475
  uint8_t       ws_width[3];               // 481
  myio          my_gp;                     // 484
  uint16_t      light_pixels;              // 496
  uint8_t       light_color[5];            // 498
  uint8_t       light_correction;          // 49D
  uint8_t       light_dimmer;              // 49E
  uint8_t       rule_enabled;              // 49F
  uint8_t       rule_once;                 // 4A0
  uint8_t       light_fade;                // 4A1
  uint8_t       light_speed;               // 4A2
  uint8_t       light_scheme;              // 4A3
  uint8_t       light_width;               // 4A4
  byte          knx_GA_registered;         // 4A5  Number of Group Address to read
  uint16_t      light_wakeup;              // 4A6
  byte          knx_CB_registered;         // 4A8  Number of Group Address to write
  char          web_password[33];          // 4A9

  uint8_t       ex_switchmode[4];          // 4CA  Free since 6.0.0a

  char          ntp_server[3][33];         // 4CE
  byte          ina219_mode;               // 531
  uint16_t      pulse_timer[MAX_PULSETIMERS]; // 532

  byte          free_542[2];               // 542

  uint32_t      ip_address[4];             // 544
  unsigned long energy_kWhtotal;           // 554
  char          mqtt_fulltopic[100];       // 558
  SysBitfield2  flag2;                     // 5BC
  unsigned long pulse_counter[MAX_COUNTERS];  // 5C0
  uint16_t      pulse_counter_type;        // 5D0
  uint16_t      pulse_counter_debounce;    // 5D2
  uint8_t       rf_code[17][9];            // 5D4

  byte          free_66d[3];               // 66D

  Timer         timer[MAX_TIMERS];         // 670
  int           latitude;                  // 6B0
  int           longitude;                 // 6B4
  uint16_t      knx_physsical_addr;        // 6B8  (address_t is a uint16_t)
  uint16_t      knx_GA_addr[MAX_KNX_GA];   // 6BA  (address_t is a uint16_t) x KNX_max_GA
  uint16_t      knx_CB_addr[MAX_KNX_CB];   // 6CE  (address_t is a uint16_t) x KNX_max_CB
  byte          knx_GA_param[MAX_KNX_GA];  // 6E2  Type of Input (relay changed, button pressed, sensor read <-teleperiod)
  byte          knx_CB_param[MAX_KNX_CB];  // 6EC  Type of Output (set relay, toggle relay, reply sensor value)
  Mcp230xxCfg   mcp230xx_config[16];       // 6F6
  uint8_t       mcp230xx_int_prio;         // 716

  byte          free_717[183];             // 717

  char          mems[RULES_MAX_MEMS][10];  // 7CE
                                           // 800 Full - no more free locations

  char          rules[MAX_RULE_SETS][MAX_RULE_SIZE]; // 800 uses 512 bytes in v5.12.0m, 3 x 512 bytes in v5.14.0b

                                           // E00 - FFF free locations
} Settings;

struct RTCMEM {
  uint16_t      valid;                     // 000
  byte          oswatch_blocked_loop;      // 002
  uint8_t       ota_loader;                // 003
  unsigned long energy_kWhtoday;              // 004
  unsigned long energy_kWhtotal;              // 008
  unsigned long pulse_counter[MAX_COUNTERS];  // 00C
  power_t       power;                     // 01C
                                           // 020 next free location
} RtcSettings;

struct TIME_T {
  uint8_t       second;
  uint8_t       minute;
  uint8_t       hour;
  uint8_t       day_of_week;               // sunday is day 1
  uint8_t       day_of_month;
  uint8_t       month;
  char          name_of_month[4];
  uint16_t      day_of_year;
  uint16_t      year;
  unsigned long days;
  unsigned long valid;
} RtcTime;

struct XDRVMAILBOX {
  uint16_t      valid;
  uint16_t      index;
  uint16_t      data_len;
  uint16_t      payload16;
  int16_t       payload;
  uint8_t       grpflg;
  uint8_t       notused;
  char         *topic;
  char         *data;
} XdrvMailbox;

#define MAX_RULES_FLAG  7                  // Number of bits used in RulesBitfield (tricky I know...)
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so usefull...
  uint16_t data;                           // Allow bit manipulation
  struct {
    uint16_t system_boot : 1;
    uint16_t time_init : 1;
    uint16_t time_set : 1;
    uint16_t mqtt_connected : 1;
    uint16_t mqtt_disconnected : 1;
    uint16_t wifi_connected : 1;
    uint16_t wifi_disconnected : 1;
    uint16_t spare07 : 1;
    uint16_t spare08 : 1;
    uint16_t spare09 : 1;
    uint16_t spare10 : 1;
    uint16_t spare11 : 1;
    uint16_t spare12 : 1;
    uint16_t spare13 : 1;
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} RulesBitfield;

typedef union {
  uint8_t data;
  struct {
    uint8_t wifi_down : 1;
    uint8_t mqtt_down : 1;
    uint8_t spare02 : 1;
    uint8_t spare03 : 1;
    uint8_t spare04 : 1;
    uint8_t spare05 : 1;
    uint8_t spare06 : 1;
    uint8_t spare07 : 1;
  };
} StateBitfield;

// See issue https://github.com/esp8266/Arduino/issues/2913
#ifdef USE_ADC_VCC
  ADC_MODE(ADC_VCC);                       // Set ADC input for Power Supply Voltage usage
#endif

#endif  // _SETTINGS_H_
