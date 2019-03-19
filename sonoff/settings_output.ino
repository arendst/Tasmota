/////////////////////////////////
#define PASSWD_OBSCURE "****"

void publishLine(){
    MqttPublishPrefixTopic_P(RESULT_OR_STAT, mqtt_data);
    mqtt_data[0] = '\0';
}
void SettingsOutputAsCommands(){
    if (Settings.seriallog_level != 0) {
      // CMND_SERIALLOG 
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_SERIALLOG, Settings.seriallog_level);
    }
    if (Settings.poweronstate != APP_POWERON_STATE) {
      /* 0 = Keep relays off after power on
       * 1 = Turn relays on after power on, if PulseTime set wait for PulseTime seconds, and turn relays off
       * 2 = Toggle relays after power on
       * 3 = Set relays to last saved state after power on
       * 4 = Turn relays on and disable any relay control (used for Sonoff Pow to always measure power)
       * 5 = Keep relays off after power on, if PulseTime set wait for PulseTime seconds, and turn relays on
       */
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_POWERONSTATE, Settings.poweronstate);
      publishLine();
    }
    { // PULSETIME
      for (int index=1; index<MAX_PULSETIMERS; index++) {
        if (Settings.pulse_timer[index -1] != 0){  // 0 - 65535
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, D_CMND_PULSETIME,  index, Settings.pulse_timer[index -1]);
            publishLine();
        }
      }
    }
    if (Settings.blinktime != APP_BLINKTIME) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_BLINKTIME,  Settings.blinktime);
        publishLine();
    }
    if (Settings.blinkcount != APP_BLINKCOUNT ){
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_BLINKCOUNT, Settings.blinkcount);
      publishLine();
    }
    if (Settings.save_data != SAVE_DATA) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, D_CMND_SAVEDATA, Settings.save_data);
      publishLine();
    }
    // TODO: Test SetOption
    if (Settings.flag2.temperature_resolution != TEMP_RESOLUTION){
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_TEMPERATURE_RESOLUTION,
      Settings.flag2.temperature_resolution);
      publishLine();
    }
    if (Settings.flag2.humidity_resolution != HUMIDITY_RESOLUTION){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_HUMIDITY_RESOLUTION,
      Settings.flag2.humidity_resolution);
      publishLine();
    }
    if (Settings.flag2.pressure_resolution != PRESSURE_RESOLUTION){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_PRESSURE_RESOLUTION,
      Settings.flag2.pressure_resolution);
      publishLine();
    }
    if (Settings.flag2.wattage_resolution != 0){
          snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_POWER_RESOLUTION,
      Settings.flag2.wattage_resolution);
      publishLine();
    }
    if (Settings.flag2.voltage_resolution != 0){
          snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_VOLTAGE_RESOLUTION,
      Settings.flag2.voltage_resolution);
      publishLine();
    }
    
    // if (CMND_FREQUENCY_RESOLUTION == command_code) 
    if (Settings.flag2.current_resolution != 3){
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_CURRENT_RESOLUTION,
      Settings.flag2.current_resolution);
      publishLine();
     }
     if (Settings.flag2.energy_resolution != ENERGY_RESOLUTION){
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_ENERGY_RESOLUTION,
      Settings.flag2.energy_resolution);
      publishLine();
     }
     if (Settings.flag2.weight_resolution != 0){ //TODO Really?
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_WEIGHT_RESOLUTION,
      Settings.flag2.weight_resolution);
      publishLine();
     }
    if (Settings.module != MODULE){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_MODULE, Settings.module);
        publishLine();

      // TODO: check GPIO deviations from default
      // See support.ino line  700 ff.
      // or: just print all gpios which are not "NONE".
      for (int index=0; index<sizeof(Settings.my_gp); index++){
          if (Settings.my_gp.io[index] != GPIO_NONE){
              snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, D_CMND_GPIO, index, Settings.my_gp.io[index] );
                publishLine();
          }
      }
    }
    if (pwm_present && Settings.pwm_range != PWM_RANGE){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_PWMRANGE, index, Settings.pwm_range );
                publishLine();
    }
    if (pwm_present && Settings.pwm_frequency != PWM_FREQ){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_PWMFREQUENCY, index, Settings.pwm_frequency );
                publishLine();
    }
    for (int i=0; i<MAX_COUNTERS; i++){
        if (Settings.pulse_counter_type != 0){
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, 
                       D_CMND_COUNTERTYPE, index, bitRead(Settings.pulse_counter_type, i));
            publishLine();
        }
    }
    if (Settings.pulse_counter_debounce > 0 ){
                snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_COUNTERDEBOUNCE, Settings.pulse_counter_debounce );
                publishLine();

    }
    if (Settings.button_debounce != KEY_DEBOUNCE_TIME ){
                snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_BUTTONDEBOUNCE, Settings.button_debounce );
                publishLine();
    }
    if (Settings.switch_debounce != SWITCH_DEBOUNCE_TIME ){
                snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_SWITCHDEBOUNCE, Settings.switch_debounce );
                publishLine();
    }
    if (Settings.baudrate != APP_BAUDRATE / 1200){
                snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_BAUDRATE, Settings.baudrate * 1200 );
                publishLine();
    }
    if (Settings.serial_delimiter != 0xff){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_SERIALDELIMITER, Settings.serial_delimiter );
            publishLine();
    }
    if (Settings.syslog_level != SYS_LOG_LEVEL){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_SYSLOG, Settings.syslog_level );
            publishLine();
    }   
    if (strcasecmp(Settings.syslog_host, SYS_LOG_HOST) != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, D_CMND_LOGHOST, Settings.syslog_host );
            publishLine();
    }    
    if (Settings.syslog_port != SYS_LOG_PORT){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_LOGPORT, Settings.syslog_port );
            publishLine();
    }
    // IPADDRESS: normally set automatically by DHCP.
    if (strcasecmp(Settings.ntp_server[0], NTP_SERVER1) != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_NTPSERVER, 1, Settings.ntp_server[0] );
            publishLine();
    }
    if (strcasecmp(Settings.ntp_server[1], NTP_SERVER2) != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_NTPSERVER, 2, Settings.ntp_server[1] );
            publishLine();
    }
    if (strcasecmp(Settings.ntp_server[2], NTP_SERVER3) != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_NTPSERVER, 3, Settings.ntp_server[2] );
            publishLine();
    }
    if (strcasecmp(Settings.sta_ssid[0], STA_SSID1) != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_SSID, 1, Settings.sta_ssid[0] );
        publishLine();
    }
    if (strcasecmp(Settings.sta_pwd[0], STA_PASS1) != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_PASSWORD, 1, PASSWD_OBSCURE);
        publishLine();
    }    
    if (strcasecmp(Settings.sta_ssid[1], STA_SSID2) != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_SSID, 2, Settings.sta_ssid[1] );
        publishLine();
    }
    if (strcasecmp(Settings.sta_pwd[1], STA_PASS2) != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_PASSWORD, 2, PASSWD_OBSCURE);
        publishLine();
    }    
    if (strcasecmp(Settings.hostname, WIFI_HOSTNAME) != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, D_CMND_HOSTNAME, Settings.hostname);
        publishLine();
    }    
    if (Settings.sta_config != WIFI_CONFIG_TOOL){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_WIFICONFIG, Settings.sta_config);
        publishLine();
    }    
    if (strcasecmp(Settings.friendlyname[0], FRIENDLY_NAME) != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_FRIENDLYNAME, 1, Settings.friendlyname[0]);
        publishLine();
    }
    if (strcasecmp(Settings.friendlyname[1], FRIENDLY_NAME"2") != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_FRIENDLYNAME, 2,  Settings.friendlyname[1]);
        publishLine();
    }
    if (strcasecmp(Settings.friendlyname[2], FRIENDLY_NAME"3") != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_FRIENDLYNAME, 3,  Settings.friendlyname[2]);
        publishLine();
    }
    if (strcasecmp(Settings.friendlyname[3], FRIENDLY_NAME"4") != 0){
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, D_CMND_FRIENDLYNAME, 4, Settings.friendlyname[3]);
        publishLine();
    }
    for (int i=0; i<MAX_SWITCHES; i++){
        if (Settings.switchmode[i] != SWITCH_MODE){
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, D_CMND_SWITCHMODE, i, Settings.switchmode[i]);
            publishLine();
        }
    }
    if (Settings.flag.interlock != 0){
        if (Settings.interlock[0] != 0xFF){
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, D_CMND_INTERLOCK, "TODO");
            publishLine();
        }else{
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_INTERLOCK, Settings.flag.interlock);
            publishLine();
        }
    }
    if (Settings.tele_period != TELE_PERIOD){
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_TELEPERIOD, Settings.tele_period);
            publishLine();
    }
    if (Settings.timezone != APP_TIMEZONE){
        if (Settings.timezone_minutes != 0){
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_TIMEZONE, Settings.timezone*60 + Settings.timezone_minutes);
            publishLine();
        }else{
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_TIMEZONE, Settings.timezone);
            publishLine();
        }
    }
    // TODO: CMND_TIMESTD  CMND_TIMEDST
    if ((Settings.tflag[0].hemis != TIME_STD_HEMISPHERE)
        || (Settings.tflag[0].week != TIME_STD_WEEK)
        || (Settings.tflag[0].dow != TIME_STD_DAY)
        || (Settings.tflag[0].month != TIME_STD_MONTH)
        || (Settings.tflag[0].hour != TIME_STD_HOUR)
        || (Settings.toffset[0] != TIME_STD_OFFSET)){
            int ts=0;
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s %d, %d, %d, %d, %d, %d"),
            D_CMND_TIMESTD, Settings.tflag[ts].hemis, Settings.tflag[ts].week, Settings.tflag[ts].month, Settings.tflag[ts].dow, Settings.tflag[ts].hour, Settings.toffset[ts]);
            // TimeStd 0/1, 0/1/2/3/4, 1..12, 1..7, 0..23, +/-780
        publishLine();
    }
    if ((Settings.tflag[1].hemis != TIME_DST_HEMISPHERE)
        || (Settings.tflag[1].week != TIME_DST_WEEK)
        || (Settings.tflag[1].dow != TIME_DST_DAY)
        || (Settings.tflag[1].month != TIME_DST_MONTH)
        || (Settings.tflag[1].hour != TIME_DST_HOUR)
        || (Settings.toffset[1] != TIME_DST_OFFSET)){
            int ts=0;
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s %d, %d, %d, %d, %d, %d"),
            D_CMND_TIMESTD, Settings.tflag[ts].hemis, Settings.tflag[ts].week, Settings.tflag[ts].month, Settings.tflag[ts].dow, Settings.tflag[ts].hour, Settings.toffset[ts]);
            // TimeStd 0/1, 0/1/2/3/4, 1..12, 1..7, 0..23, +/-780
        publishLine();
    }
    if (Settings.altitude != 0){
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_ALTITUDE, Settings.altitude);
            publishLine();
    }
    if (Settings.ledstate != APP_LEDSTATE){
            snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, D_CMND_LEDSTATE, Settings.ledstate);
            publishLine();
    }  
}
