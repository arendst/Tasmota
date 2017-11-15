/*
  settings.h - setting variables for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

#define PARAM8_SIZE  23                    // Number of param bytes

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so usefull...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {
    uint32_t save_state : 1;               // bit 0
    uint32_t button_restrict : 1;          // bit 1
    uint32_t value_units : 1;              // bit 2
    uint32_t mqtt_enabled : 1;             // bit 3
    uint32_t mqtt_response : 1;            // bit 4
    uint32_t mqtt_power_retain : 1;
    uint32_t mqtt_button_retain : 1;
    uint32_t mqtt_switch_retain : 1;
    uint32_t temperature_conversion : 1;   // bit 8
    uint32_t mqtt_sensor_retain : 1;
    uint32_t mqtt_offline : 1;             // bit 10
    uint32_t button_swap : 1;              // bit 11 (v5.1.6)
    uint32_t stop_flash_rotate : 1;        // bit 12 (v5.2.0)
    uint32_t button_single : 1;            // bit 13 (v5.4.0)
    uint32_t interlock : 1;                // bit 14 (v5.6.0)
    uint32_t pwm_control : 1;              // bit 15 (v5.8.1)
    uint32_t ws_clock_reverse : 1;         // bit 16 (v5.8.1)
    uint32_t decimal_text : 1;             // bit 17 (v5.8.1)
    uint32_t spare18 : 1;
    uint32_t spare19 : 1;
    uint32_t voltage_resolution : 1;
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
    /*
    uint32_t wattage_resolution : 1;
    uint32_t voltage_resolution : 1;
    uint32_t emulation : 2;
    uint32_t energy_resolution : 3;
    uint32_t pressure_resolution : 2;
    uint32_t humidity_resolution : 2;
    uint32_t temperature_resolution : 2;
*/
  };
} SysBitfield;

typedef union {
  uint32_t data;                           // Allow bit manipulation using SetOption
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
    uint32_t spare13 : 1;
    uint32_t spare14 : 1;
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

struct SYSCFG {
  unsigned long cfg_holder;                // 000
  unsigned long save_flag;                 // 004
  unsigned long version;                   // 008
  unsigned long bootcount;                 // 00C
  SysBitfield   flag;                      // 010 Add flag since 5.0.2
  int16_t       save_data;                 // 014
  int8_t        timezone;                  // 016
  char          ota_url[101];              // 017
  char          mqtt_prefix[3][11];        // 07C

  byte          free_09D[1];               // 09D

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
  char          mqtt_fingerprint[60];      // 1AD To be freed by binary fingerprint
  char          mqtt_host[33];             // 1E9
  uint16_t      mqtt_port;                 // 20A
  char          mqtt_client[33];           // 20C
  char          mqtt_user[33];             // 22D
  char          mqtt_pwd[33];              // 24E
  char          mqtt_topic[33];            // 26F
  char          button_topic[33];          // 290
  char          mqtt_grptopic[33];         // 2B1
  uint8_t       mqtt_fingerprinth[20];     // 2D2 Reserved for binary fingerprint

  uint16_t      pwm_frequency;             // 2E6
  power_t       power;                     // 2E8
  uint16_t      pwm_value[MAX_PWMS];       // 2EC

  int16_t       altitude;                  // 2F6 Add since 5.8.0i
  uint16_t      tele_period;               // 2F8
  uint8_t       ex_power;                  // 2FA Not used since 5.8.0j
  uint8_t       ledstate;                  // 2FB
  uint8_t       param[PARAM8_SIZE];        // 2FC was domoticz_in_topic until 5.1.6
  char          state_text[4][11];         // 313

  byte          free_33F[1];               // 33F

  uint16_t      domoticz_update_timer;     // 340
  uint16_t      pwm_range;                 // 342

  unsigned long domoticz_relay_idx[MAX_DOMOTICZ_IDX]; // 344
  unsigned long domoticz_key_idx[MAX_DOMOTICZ_IDX];   // 354

  unsigned long hlw_power_calibration;     // 364
  unsigned long hlw_voltage_calibration;   // 368
  unsigned long hlw_current_calibration;   // 36C
  unsigned long hlw_kWhtoday;              // 370
  unsigned long hlw_kWhyesterday;          // 374
  uint16_t      hlw_kWhdoy;                // 378
  uint16_t      hlw_pmin;                  // 37A
  uint16_t      hlw_pmax;                  // 37C
  uint16_t      hlw_umin;                  // 37E
  uint16_t      hlw_umax;                  // 380
  uint16_t      hlw_imin;                  // 382
  uint16_t      hlw_imax;                  // 384
  uint16_t      hlw_mpl;                   // 386 MaxPowerLimit
  uint16_t      hlw_mplh;                  // 388 MaxPowerLimitHold
  uint16_t      hlw_mplw;                  // 38A MaxPowerLimitWindow
  uint16_t      hlw_mspl;                  // 38C MaxSafePowerLimit
  uint16_t      hlw_msplh;                 // 38E MaxSafePowerLimitHold
  uint16_t      hlw_msplw;                 // 390 MaxSafePowerLimitWindow
  uint16_t      hlw_mkwh;                  // 392 MaxEnergy
  uint16_t      hlw_mkwhs;                 // 394 MaxEnergyStart
  uint16_t      mqtt_retry;                // 396
  uint8_t       poweronstate;              // 398
  uint8_t       last_module;               // 399

  uint16_t      blinktime;                 // 39A
  uint16_t      blinkcount;                // 39C
  uint16_t      ws_pixels;                 // 39E Not used since 5.8.0
  uint8_t       ws_red;                    // 3A0 Not used since 5.8.0
  uint8_t       ws_green;                  // 3A1 Not used since 5.8.0
  uint8_t       ws_blue;                   // 3A2 Not used since 5.8.0
  uint8_t       ws_ledtable;               // 3A3 Not used since 5.8.0
  uint8_t       ws_dimmer;                 // 3A4 Not used since 5.8.0
  uint8_t       ws_fade;                   // 3A5 Not used since 5.8.0
  uint8_t       ws_speed;                  // 3A6 Not used since 5.8.0
  uint8_t       ws_scheme;                 // 3A7 Not used since 5.8.0
  uint8_t       ex_ws_width;               // 3A8 Not used since 5.8.0

  byte          free_3A9[1];               // 3A9

  uint16_t      ws_wakeup;                 // 3AA Not used since 5.8.0
  char          friendlyname[MAX_FRIENDLYNAMES][33]; // 3AC
  char          switch_topic[33];          // 430

  byte          free_451[2];               // 451

  uint8_t       sleep;                     // 453
  uint16_t      domoticz_switch_idx[MAX_DOMOTICZ_IDX]; // 454
  uint16_t      domoticz_sensor_idx[12];   // 45C
  uint8_t       module;                    // 474

  uint8_t       ws_color[4][3];            // 475
  uint8_t       ws_width[3];               // 481

  myio          my_gp;                     // 484
  uint16_t      light_pixels;              // 496
  uint8_t       light_color[5];            // 498
  uint8_t       light_correction;          // 49D
  uint8_t       light_dimmer;              // 49E

  byte          free_49F[2];               // 49F

  uint8_t       light_fade;                // 4A1
  uint8_t       light_speed;               // 4A2
  uint8_t       light_scheme;              // 4A3
  uint8_t       light_width;               // 4A4

  byte          free_4A5[1];               // 4A5

  uint16_t      light_wakeup;              // 4A6

  byte          free_4A8[1];               // 4A8

  char          web_password[33];          // 4A9
  uint8_t       switchmode[MAX_SWITCHES];  // 4CA
  char          ntp_server[3][33];         // 4CE

  byte          ina219_mode;               // 531

  uint16_t      pulse_timer[MAX_PULSETIMERS]; // 532

  byte          free_542[2];               // 542

  uint32_t      ip_address[4];             // 544
  unsigned long hlw_kWhtotal;              // 554
  char          mqtt_fulltopic[100];       // 558

  SysBitfield2  flag2;                     // 5BC Add flag2 since 5.9.2

  unsigned long pulse_counter[MAX_COUNTERS];  // 5C0
  uint16_t      pulse_counter_type;        // 5D0
  uint16_t      pulse_counter_debounce;    // 5D2
  uint8_t       rf_code[17][9];            // 5D4

} Settings;

struct RTCMEM {
  uint16_t      valid;                     // 000
  byte          oswatch_blocked_loop;      // 002
  uint8_t       unused;                    // 003
  unsigned long hlw_kWhtoday;              // 004
  unsigned long hlw_kWhtotal;              // 008
  unsigned long pulse_counter[MAX_COUNTERS];  // 00C
  power_t       power;                     // 01C
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
  unsigned long valid;
} RtcTime;

struct TimeChangeRule
{
  uint8_t       hemis;                     // 0-Northern, 1=Southern Hemisphere (=Opposite DST/STD)
  uint8_t       week;                      // 1=First, 2=Second, 3=Third, 4=Fourth, or 0=Last week of the month
  uint8_t       dow;                       // day of week, 1=Sun, 2=Mon, ... 7=Sat
  uint8_t       month;                     // 1=Jan, 2=Feb, ... 12=Dec
  uint8_t       hour;                      // 0-23
  int           offset;                    // offset from UTC in minutes
};

TimeChangeRule DaylightSavingTime = { TIME_DST }; // Daylight Saving Time
TimeChangeRule StandardTime = { TIME_STD }; // Standard Time

// See issue https://github.com/esp8266/Arduino/issues/2913
#ifdef USE_ADC_VCC
  ADC_MODE(ADC_VCC);                       // Set ADC input for Power Supply Voltage usage
#endif

#endif  // _SETTINGS_H_