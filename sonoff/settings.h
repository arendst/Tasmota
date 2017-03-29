/*********************************************************************************************\
 * Config settings
\*********************************************************************************************/

struct SYSCFG {
  unsigned long cfg_holder;
  unsigned long saveFlag;
  unsigned long version;
  unsigned long bootcount;
  byte          migflg;               // Not used since 3.9.1
  int16_t       savedata;
  byte          savestate;
  byte          mqtt_response;        // was model until 3.9.1
  int8_t        timezone;
  char          otaUrl[101];

  char          mqtt_prefix[3][11];   // was ex_friendlyname[33] until 3.2.5

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
  char          state_text[3][11];     // was ex_mqtt_subtopic[33] until 4.1.1
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

  // 3.0.6
  uint16_t      ex_pulsetime;         // Not used since 4.0.4

  // 3.1.1
  uint8_t       poweronstate;

  // 3.1.6
  uint16_t      blinktime;
  uint16_t      blinkcount;

  // 3.2.4
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

  // 3.2.5
  char          friendlyname[4][33];

  // 3.2.8
  char          switch_topic[33];
  byte          mqtt_switch_retain;
  uint8_t       mqtt_enabled;

  // 3.2.12
  uint8_t       sleep;

  // 3.9.3
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

  // 3.9.7
  uint8_t       emulation;

  // 3.9.20
  char          web_password[33];

  // 3.9.21
  uint8_t       switchmode[4];

  // 4.0.4
  char          ntp_server[3][33];
  uint16_t      pulsetime[MAX_PULSETIMERS];

  // 4.0.7
  uint16_t      pwmvalue[5];

  // 4.0.9
  uint32_t      ip_address[4];

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

