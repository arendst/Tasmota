/*********************************************************************************************\
 * Config settings
\*********************************************************************************************/

#ifdef ALLOW_MIGRATE_TO_V3
struct SYSCFG2 {      // Version 2.x (old)
  unsigned long cfg_holder;
  unsigned long saveFlag;
  unsigned long version;
  byte          seriallog_level;
  byte          syslog_level;
  char          syslog_host[32];
  char          sta_ssid1[32];
  char          sta_pwd1[64];
  char          otaUrl[80];
  char          mqtt_host[32];
  char          mqtt_grptopic[32];
  char          mqtt_topic[32];
  char          mqtt_topic2[32];
  char          mqtt_subtopic[32];
  int8_t        timezone;
  uint8_t       power;
  uint8_t       ledstate;
  uint16_t      mqtt_port;
  char          mqtt_client[33];
  char          mqtt_user[33];
  char          mqtt_pwd[33];
  uint8_t       webserver;
  unsigned long bootcount;
  char          hostname[33];
  uint16_t      syslog_port;
  byte          weblog_level;
  uint16_t      tele_period;
  uint8_t       sta_config;
  int16_t       savedata;
  byte          model;
  byte          mqtt_retain;
  byte          savestate;
  unsigned long hlw_pcal;
  unsigned long hlw_ucal;
  unsigned long hlw_ical;
  unsigned long hlw_kWhyesterday;
  byte          value_units;
  uint16_t      hlw_pmin;
  uint16_t      hlw_pmax;
  uint16_t      hlw_umin;
  uint16_t      hlw_umax;
  uint16_t      hlw_imin;
  uint16_t      hlw_imax;
  uint16_t      hlw_mpl;    // MaxPowerLimit
  uint16_t      hlw_mplh;   // MaxPowerLimitHold
  uint16_t      hlw_mplw;   // MaxPowerLimitWindow
  uint16_t      hlw_mspl;   // MaxSafePowerLimit
  uint16_t      hlw_msplh;  // MaxSafePowerLimitHold
  uint16_t      hlw_msplw;  // MaxSafePowerLimitWindow
  uint16_t      hlw_mkwh;   // MaxEnergy
  uint16_t      hlw_mkwhs;  // MaxEnergyStart
  char          domoticz_in_topic[33];
  char          domoticz_out_topic[33];
  uint16_t      domoticz_update_timer;
  unsigned long domoticz_relay_idx[4];
  unsigned long domoticz_key_idx[4];
  byte          message_format;  // Not used since 3.2.6a
  unsigned long hlw_kWhtoday;
  uint16_t      hlw_kWhdoy;
  uint8_t       switchmode;
  char          mqtt_fingerprint[60];
  byte          sta_active;
  char          sta_ssid2[33];
  char          sta_pwd2[65];

} sysCfg2;
#endif  // ALLOW_MIGRATE_TO_V3

struct SYSCFG {
  unsigned long cfg_holder;
  unsigned long saveFlag;
  unsigned long version;
  unsigned long bootcount;
  byte          migflg;               // Not used since 3.9.1
  int16_t       savedata;
  byte          savestate;
  byte          model;                // Not used since 3.9.1
  int8_t        timezone;
  char          otaUrl[101];
  char          ex_friendlyname[33];  // Not used since 3.2.5 - see below

  byte          serial_enable;
  byte          seriallog_level;
  uint8_t       sta_config;
  byte          sta_active;
  char          sta_ssid[2][33];
  char          sta_pwd[2][65];
  char          hostname[33];
  char          syslog_host[33];
  uint16_t      syslog_port;
  byte          syslog_level;
  uint8_t       webserver;
  byte          weblog_level;

  char          mqtt_fingerprint[60];
  char          mqtt_host[33];
  uint16_t      mqtt_port;
  char          mqtt_client[33];
  char          mqtt_user[33];
  char          mqtt_pwd[33];
  char          mqtt_topic[33];
  char          button_topic[33];
  char          mqtt_grptopic[33];
  char          mqtt_subtopic[33];
  byte          mqtt_button_retain;
  byte          mqtt_power_retain;
  byte          value_units;
  byte          button_restrict;       // Was message_format until 3.2.6a
  uint16_t      tele_period;

  uint8_t       power;
  uint8_t       ledstate;
  uint8_t       ex_switchmode;         // Not used since 3.9.21

  char          domoticz_in_topic[33];
  char          domoticz_out_topic[33];
  uint16_t      domoticz_update_timer;
  unsigned long domoticz_relay_idx[4];
  unsigned long domoticz_key_idx[4];

  unsigned long hlw_pcal;
  unsigned long hlw_ucal;
  unsigned long hlw_ical;
  unsigned long hlw_kWhtoday;
  unsigned long hlw_kWhyesterday;
  uint16_t      hlw_kWhdoy;
  uint16_t      hlw_pmin;
  uint16_t      hlw_pmax;
  uint16_t      hlw_umin;
  uint16_t      hlw_umax;
  uint16_t      hlw_imin;
  uint16_t      hlw_imax;
  uint16_t      hlw_mpl;    // MaxPowerLimit
  uint16_t      hlw_mplh;   // MaxPowerLimitHold
  uint16_t      hlw_mplw;   // MaxPowerLimitWindow
  uint16_t      hlw_mspl;   // MaxSafePowerLimit
  uint16_t      hlw_msplh;  // MaxSafePowerLimitHold
  uint16_t      hlw_msplw;  // MaxSafePowerLimitWindow
  uint16_t      hlw_mkwh;   // MaxEnergy
  uint16_t      hlw_mkwhs;  // MaxEnergyStart

  uint16_t      ex_pulsetime;         // Not used since 4.0.4
  uint8_t       poweronstate;
  uint16_t      blinktime;
  uint16_t      blinkcount;

  uint16_t      ws_pixels;
  uint8_t       ws_red;
  uint8_t       ws_green;
  uint8_t       ws_blue;
  uint8_t       ws_ledtable;
  uint8_t       ws_dimmer;
  uint8_t       ws_fade;
  uint8_t       ws_speed;
  uint8_t       ws_scheme;
  uint8_t       ws_width;
  uint16_t      ws_wakeup;

  char          friendlyname[4][33];
  char          switch_topic[33];
  byte          mqtt_switch_retain;
  uint8_t       mqtt_enabled;
  uint8_t       sleep;

  uint16_t      domoticz_switch_idx[4];
  uint16_t      domoticz_sensor_idx[12];

  uint8_t       module;
  mytmplt       my_module;

  uint16_t      led_pixels;
  uint8_t       led_color[5];
  uint8_t       led_table;
  uint8_t       led_dimmer[3];
  uint8_t       led_fade;
  uint8_t       led_speed;
  uint8_t       led_scheme;
  uint8_t       led_width;
  uint16_t      led_wakeup;

  uint8_t       emulation;

  char          web_password[33];
  uint8_t       switchmode[4];

  char          ntp_server[3][33];
  uint16_t      pulsetime[MAX_PULSETIMERS];
  uint16_t      pwmvalue[5];

} sysCfg;

struct RTCMEM {
  uint16_t      valid;
  byte          osw_flag;
  byte          nu1;
  unsigned long hlw_kWhtoday;
} rtcMem;

// See issue https://github.com/esp8266/Arduino/issues/2913  
#ifdef USE_ADC_VCC
  ADC_MODE(ADC_VCC);                        // Set ADC input for Power Supply Voltage usage
#endif

