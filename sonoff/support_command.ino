/*
  support_command.ino - command support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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

enum TasmotaCommands {
  CMND_BACKLOG, CMND_DELAY, CMND_POWER, CMND_STATUS, CMND_STATE, CMND_POWERONSTATE, CMND_PULSETIME,
  CMND_BLINKTIME, CMND_BLINKCOUNT, CMND_SENSOR, CMND_SAVEDATA, CMND_SETOPTION, CMND_TEMPERATURE_RESOLUTION, CMND_HUMIDITY_RESOLUTION,
  CMND_PRESSURE_RESOLUTION, CMND_POWER_RESOLUTION, CMND_VOLTAGE_RESOLUTION, CMND_FREQUENCY_RESOLUTION, CMND_CURRENT_RESOLUTION, CMND_ENERGY_RESOLUTION, CMND_WEIGHT_RESOLUTION,
  CMND_MODULE, CMND_MODULES, CMND_ADC, CMND_ADCS, CMND_GPIO, CMND_GPIOS, CMND_PWM, CMND_PWMFREQUENCY, CMND_PWMRANGE, CMND_COUNTER, CMND_COUNTERTYPE,
  CMND_COUNTERDEBOUNCE, CMND_BUTTONDEBOUNCE, CMND_SWITCHDEBOUNCE, CMND_SLEEP, CMND_UPGRADE, CMND_UPLOAD, CMND_OTAURL, CMND_SERIALLOG, CMND_SYSLOG,
  CMND_LOGHOST, CMND_LOGPORT, CMND_IPADDRESS, CMND_NTPSERVER, CMND_AP, CMND_SSID, CMND_PASSWORD, CMND_HOSTNAME,
  CMND_WIFICONFIG, CMND_FRIENDLYNAME, CMND_SWITCHMODE, CMND_INTERLOCK, CMND_TEMPLATE,
  CMND_TELEPERIOD, CMND_RESTART, CMND_RESET, CMND_TIME, CMND_TIMEZONE, CMND_TIMESTD, CMND_TIMEDST, CMND_ALTITUDE, CMND_LEDPOWER, CMND_LEDSTATE, CMND_LEDMASK,
  CMND_I2CSCAN, CMND_SERIALSEND, CMND_BAUDRATE, CMND_SERIALDELIMITER, CMND_DRIVER };
const char kTasmotaCommands[] PROGMEM =
  D_CMND_BACKLOG "|" D_CMND_DELAY "|" D_CMND_POWER "|" D_CMND_STATUS "|" D_CMND_STATE "|"  D_CMND_POWERONSTATE "|" D_CMND_PULSETIME "|"
  D_CMND_BLINKTIME "|" D_CMND_BLINKCOUNT "|" D_CMND_SENSOR "|" D_CMND_SAVEDATA "|" D_CMND_SETOPTION "|" D_CMND_TEMPERATURE_RESOLUTION "|" D_CMND_HUMIDITY_RESOLUTION "|"
  D_CMND_PRESSURE_RESOLUTION "|" D_CMND_POWER_RESOLUTION "|" D_CMND_VOLTAGE_RESOLUTION "|" D_CMND_FREQUENCY_RESOLUTION "|" D_CMND_CURRENT_RESOLUTION "|" D_CMND_ENERGY_RESOLUTION "|" D_CMND_WEIGHT_RESOLUTION "|"
  D_CMND_MODULE "|" D_CMND_MODULES "|" D_CMND_ADC "|" D_CMND_ADCS "|" D_CMND_GPIO "|" D_CMND_GPIOS "|" D_CMND_PWM "|" D_CMND_PWMFREQUENCY "|" D_CMND_PWMRANGE "|" D_CMND_COUNTER "|" D_CMND_COUNTERTYPE "|"
  D_CMND_COUNTERDEBOUNCE "|" D_CMND_BUTTONDEBOUNCE "|" D_CMND_SWITCHDEBOUNCE "|" D_CMND_SLEEP "|" D_CMND_UPGRADE "|" D_CMND_UPLOAD "|" D_CMND_OTAURL "|" D_CMND_SERIALLOG "|" D_CMND_SYSLOG "|"
  D_CMND_LOGHOST "|" D_CMND_LOGPORT "|" D_CMND_IPADDRESS "|" D_CMND_NTPSERVER "|" D_CMND_AP "|" D_CMND_SSID "|" D_CMND_PASSWORD "|" D_CMND_HOSTNAME "|"
  D_CMND_WIFICONFIG "|" D_CMND_FRIENDLYNAME "|" D_CMND_SWITCHMODE "|" D_CMND_INTERLOCK "|" D_CMND_TEMPLATE "|"
  D_CMND_TELEPERIOD "|" D_CMND_RESTART "|" D_CMND_RESET "|" D_CMND_TIME "|" D_CMND_TIMEZONE "|" D_CMND_TIMESTD "|" D_CMND_TIMEDST "|" D_CMND_ALTITUDE "|" D_CMND_LEDPOWER "|" D_CMND_LEDSTATE "|" D_CMND_LEDMASK "|"
  D_CMND_I2CSCAN "|" D_CMND_SERIALSEND "|" D_CMND_BAUDRATE "|" D_CMND_SERIALDELIMITER "|" D_CMND_DRIVER;

/********************************************************************************************/

void ExecuteCommand(char *cmnd, int source)
{
  char *start;
  char *token;

#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("ExecuteCommand"));
#endif
  ShowSource(source);

  token = strtok(cmnd, " ");
  if (token != nullptr) {
    start = strrchr(token, '/');   // Skip possible cmnd/sonoff/ preamble
    if (start) { token = start +1; }
  }
  uint16_t size = (token != nullptr) ? strlen(token) : 0;
  char stopic[size +2];  // / + \0
  snprintf_P(stopic, sizeof(stopic), PSTR("/%s"), (token == nullptr) ? "" : token);

  token = strtok(nullptr, "");
  size = (token != nullptr) ? strlen(token) : 0;
  char svalue[size +1];
  strlcpy(svalue, (token == nullptr) ? "" : token, sizeof(svalue));       // Fixed 5.8.0b
  CommandHandler(stopic, (uint8_t*)svalue, strlen(svalue));
}

/********************************************************************************************/

// topic:                    /power1  data: toggle  = Console command
// topic:         cmnd/sonoff/power1  data: toggle  = Mqtt command using topic
// topic:        cmnd/sonoffs/power1  data: toggle  = Mqtt command using a group topic
// topic: cmnd/DVES_83BB10_fb/power1  data: toggle  = Mqtt command using fallback topic

void CommandHandler(char* topic, uint8_t* data, unsigned int data_len)
{
  if (data_len > MQTT_MAX_PACKET_SIZE) { return; }  // Do not allow more data than would be feasable within stack space

  char *str;

  if (!strcmp(Settings.mqtt_prefix[0],Settings.mqtt_prefix[1])) {
    str = strstr(topic,Settings.mqtt_prefix[0]);
    if ((str == topic) && mqtt_cmnd_publish) {
      if (mqtt_cmnd_publish > 3) {
        mqtt_cmnd_publish -= 3;
      } else {
        mqtt_cmnd_publish = 0;
      }
      return;
    }
  }

  char topicBuf[TOPSZ];
  char dataBuf[data_len+1];
  char command [CMDSZ];
  char stemp1[TOPSZ];
  char *p;
  char *type = nullptr;
  uint8_t lines = 1;
  bool jsflg = false;
  bool grpflg = false;
  bool user_index = false;
  uint32_t i = 0;
  uint32_t index;
  uint32_t address;

#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("CommandHandler"));
#endif

  strlcpy(topicBuf, topic, sizeof(topicBuf));
  for (i = 0; i < data_len; i++) {
    if (!isspace(data[i])) { break; }
  }
  data_len -= i;
  memcpy(dataBuf, data +i, sizeof(dataBuf));
  dataBuf[sizeof(dataBuf)-1] = 0;

  if (topicBuf[0] != '/') { ShowSource(SRC_MQTT); }

  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_RESULT D_RECEIVED_TOPIC " %s, " D_DATA_SIZE " %d, " D_DATA " %s"), topicBuf, data_len, dataBuf);
//  if (LOG_LEVEL_DEBUG_MORE <= seriallog_level) { Serial.println(dataBuf); }

  if (XdrvMqttData(topicBuf, sizeof(topicBuf), dataBuf, sizeof(dataBuf))) { return; }

  grpflg = (strstr(topicBuf, Settings.mqtt_grptopic) != nullptr);

  GetFallbackTopic_P(stemp1, CMND, "");  // Full Fallback topic = cmnd/DVES_xxxxxxxx_fb/
  fallback_topic_flag = (!strncmp(topicBuf, stemp1, strlen(stemp1)));

  type = strrchr(topicBuf, '/');  // Last part of received topic is always the command (type)

  index = 1;
  if (type != nullptr) {
    type++;
    for (i = 0; i < strlen(type); i++) {
      type[i] = toupper(type[i]);
    }
    while (isdigit(type[i-1])) {
      i--;
    }
    if (i < strlen(type)) {
      index = atoi(type +i);
      user_index = true;
    }
    type[i] = '\0';
  }

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_RESULT D_GROUP " %d, " D_INDEX " %d, " D_COMMAND " %s, " D_DATA " %s"), grpflg, index, type, dataBuf);

  if (type != nullptr) {
    Response_P(PSTR("{\"" D_JSON_COMMAND "\":\"" D_JSON_ERROR "\"}"));
    if (Settings.ledstate &0x02) { blinks++; }

    if (!strcmp(dataBuf,"?")) { data_len = 0; }
    int16_t payload = -99;               // No payload
    uint16_t payload16 = 0;
    long payload32 = strtol(dataBuf, &p, 0);  // decimal, octal (0) or hex (0x)
    if (p != dataBuf) {
      payload = (int16_t) payload32;     // -32766 - 32767
      payload16 = (uint16_t) payload32;  // 0 - 65535
    } else {
      payload32 = 0;
    }
    backlog_delay = millis() + (100 * MIN_BACKLOG_DELAY);

    int temp_payload = GetStateNumber(dataBuf);
    if (temp_payload > -1) { payload = temp_payload; }

//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_RESULT "Payload %d, Payload16 %d, payload32 %u"), payload, payload16, payload32);

    int command_code = GetCommandCode(command, sizeof(command), type, kTasmotaCommands);
    if (-1 == command_code) {
//      XdrvMailbox.valid = 1;
      XdrvMailbox.index = index;
      XdrvMailbox.data_len = data_len;
      XdrvMailbox.payload16 = payload16;
      XdrvMailbox.payload = payload;
      XdrvMailbox.grpflg = grpflg;
      XdrvMailbox.usridx = user_index;
      XdrvMailbox.topic = type;
      XdrvMailbox.data = dataBuf;
      if (!XdrvCall(FUNC_COMMAND)) {
        if (!XsnsCall(FUNC_COMMAND)) {
          type = nullptr;  // Unknown command
        }
      }
    }
    else if (CMND_BACKLOG == command_code) {
      if (data_len) {
        uint8_t bl_pointer = (!backlog_pointer) ? MAX_BACKLOG -1 : backlog_pointer;
        bl_pointer--;
        char *blcommand = strtok(dataBuf, ";");
        while ((blcommand != nullptr) && (backlog_index != bl_pointer)) {
          while(true) {
            blcommand = Trim(blcommand);
            if (!strncasecmp_P(blcommand, PSTR(D_CMND_BACKLOG), strlen(D_CMND_BACKLOG))) {
              blcommand += strlen(D_CMND_BACKLOG);                                  // Skip unnecessary command Backlog
            } else {
              break;
            }
          }
          if (*blcommand != '\0') {
            backlog[backlog_index] = String(blcommand);
            backlog_index++;
            if (backlog_index >= MAX_BACKLOG) backlog_index = 0;
          }
          blcommand = strtok(nullptr, ";");
        }
//        Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_APPENDED);
        mqtt_data[0] = '\0';
      } else {
        uint8_t blflag = (backlog_pointer == backlog_index);
        backlog_pointer = backlog_index;
        Response_P(S_JSON_COMMAND_SVALUE, command, blflag ? D_JSON_EMPTY : D_JSON_ABORTED);
      }
    }
    else if (CMND_DELAY == command_code) {
      if ((payload >= MIN_BACKLOG_DELAY) && (payload <= 3600)) {
        backlog_delay = millis() + (100 * payload);
      }
      uint16_t bl_delay = 0;
      long bl_delta = TimePassedSince(backlog_delay);
      if (bl_delta < 0) { bl_delay = (bl_delta *-1) / 100; }
      Response_P(S_JSON_COMMAND_NVALUE, command, bl_delay);
    }
    else if ((CMND_POWER == command_code) && (index > 0) && (index <= devices_present)) {
      if ((payload < 0) || (payload > 4)) { payload = 9; }
//      Settings.flag.device_index_enable = user_index;
      ExecuteCommandPower(index, payload, SRC_IGNORE);
      fallback_topic_flag = false;
      return;
    }
    else if (CMND_STATUS == command_code) {
      if ((payload < 0) || (payload > MAX_STATUS)) payload = 99;
      PublishStatus(payload);
      fallback_topic_flag = false;
      return;
    }
    else if (CMND_STATE == command_code) {
      mqtt_data[0] = '\0';
      MqttShowState();
      if (Settings.flag3.hass_tele_on_power) {
        MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_STATE), MQTT_TELE_RETAIN);
      }
#ifdef USE_HOME_ASSISTANT
      if (Settings.flag.hass_discovery) {
        HAssPublishStatus();
      }
#endif  // USE_HOME_ASSISTANT
    }
    else if (CMND_SLEEP == command_code) {
      if ((payload >= 0) && (payload < 251)) {
        Settings.sleep = payload;
        sleep = payload;
        WiFiSetSleepMode();
      }
      Response_P(S_JSON_COMMAND_NVALUE_UNIT_NVALUE_UNIT, command, sleep, (Settings.flag.value_units) ? " " D_UNIT_MILLISECOND : "", Settings.sleep, (Settings.flag.value_units) ? " " D_UNIT_MILLISECOND : "");
    }
    else if ((CMND_UPGRADE == command_code) || (CMND_UPLOAD == command_code)) {
      // Check if the payload is numerically 1, and had no trailing chars.
      //   e.g. "1foo" or "1.2.3" could fool us.
      // Check if the version we have been asked to upgrade to is higher than our current version.
      //   We also need at least 3 chars to make a valid version number string.
      if (((1 == data_len) && (1 == payload)) || ((data_len >= 3) && NewerVersion(dataBuf))) {
        ota_state_flag = 3;
        Response_P(PSTR("{\"%s\":\"" D_JSON_VERSION " %s " D_JSON_FROM " %s\"}"), command, my_version, GetOtaUrl(stemp1, sizeof(stemp1)));
      } else {
        Response_P(PSTR("{\"%s\":\"" D_JSON_ONE_OR_GT "\"}"), command, my_version);
      }
    }
    else if (CMND_OTAURL == command_code) {
      if ((data_len > 0) && (data_len < sizeof(Settings.ota_url))) {
        strlcpy(Settings.ota_url, (SC_DEFAULT == Shortcut(dataBuf)) ? OTA_URL : dataBuf, sizeof(Settings.ota_url));
      }
      Response_P(S_JSON_COMMAND_SVALUE, command, Settings.ota_url);
    }
    else if (CMND_SERIALLOG == command_code) {
      if ((payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_ALL)) {
        Settings.flag.mqtt_serial = 0;
        SetSeriallog(payload);
      }
      Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, command, Settings.seriallog_level, seriallog_level);
    }
    else if (CMND_RESTART == command_code) {
      switch (payload) {
      case 1:
        restart_flag = 2;
        Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_RESTARTING);
        break;
      case 99:
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_RESTARTING));
        EspRestart();
        break;
      default:
        Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_ONE_TO_RESTART);
      }
    }
    else if ((CMND_POWERONSTATE == command_code) && (my_module_type != MOTOR)) {
      /* 0 = Keep relays off after power on
       * 1 = Turn relays on after power on, if PulseTime set wait for PulseTime seconds, and turn relays off
       * 2 = Toggle relays after power on
       * 3 = Set relays to last saved state after power on
       * 4 = Turn relays on and disable any relay control (used for Sonoff Pow to always measure power)
       * 5 = Keep relays off after power on, if PulseTime set wait for PulseTime seconds, and turn relays on
       */
      if ((payload >= POWER_ALL_OFF) && (payload <= POWER_ALL_OFF_PULSETIME_ON)) {
        Settings.poweronstate = payload;
        if (POWER_ALL_ALWAYS_ON == Settings.poweronstate) {
          for (uint32_t i = 1; i <= devices_present; i++) {
            ExecuteCommandPower(i, POWER_ON, SRC_IGNORE);
          }
        }
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.poweronstate);
    }
    else if ((CMND_PULSETIME == command_code) && (index > 0) && (index <= MAX_PULSETIMERS)) {
      if (data_len > 0) {
        Settings.pulse_timer[index -1] = payload16;  // 0 - 65535
        SetPulseTimer(index -1, payload16);
      }
      Response_P(S_JSON_COMMAND_INDEX_NVALUE_ACTIVE_NVALUE, command, index, Settings.pulse_timer[index -1], GetPulseTimer(index -1));
    }
    else if (CMND_BLINKTIME == command_code) {
      if ((payload > 1) && (payload <= 3600)) {
        Settings.blinktime = payload;
        if (blink_timer > 0) { blink_timer = millis() + (100 * payload); }
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.blinktime);
    }
    else if (CMND_BLINKCOUNT == command_code) {
      if (data_len > 0) {
        Settings.blinkcount = payload16;  // 0 - 65535
        if (blink_counter) blink_counter = Settings.blinkcount *2;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.blinkcount);
    }
    else if (CMND_SAVEDATA == command_code) {
      if ((payload >= 0) && (payload <= 3600)) {
        Settings.save_data = payload;
        save_data_counter = Settings.save_data;
      }
      SettingsSaveAll();
      if (Settings.save_data > 1) {
        snprintf_P(stemp1, sizeof(stemp1), PSTR(D_JSON_EVERY " %d " D_UNIT_SECOND), Settings.save_data);
      }
      Response_P(S_JSON_COMMAND_SVALUE, command, (Settings.save_data > 1) ? stemp1 : GetStateText(Settings.save_data));
    }
    else if ((CMND_SENSOR == command_code) || (CMND_DRIVER == command_code)) {
      XdrvMailbox.index = index;
      XdrvMailbox.data_len = data_len;
      XdrvMailbox.payload16 = payload16;
      XdrvMailbox.payload = payload;
      XdrvMailbox.grpflg = grpflg;
      XdrvMailbox.topic = command;
      XdrvMailbox.data = dataBuf;
      if (CMND_SENSOR == command_code) {
        XsnsCall(FUNC_COMMAND_SENSOR);
      } else {
        XdrvCall(FUNC_COMMAND_DRIVER);
      }
    }
    else if ((CMND_SETOPTION == command_code) && (index < 82)) {
      uint8_t ptype;
      uint8_t pindex;
      if (index <= 31) {         // SetOption0 .. 31 = Settings.flag
        ptype = 0;
        pindex = index;          // 0 .. 31
      }
      else if (index <= 49) {    // SetOption32 .. 49 = Settings.param
        ptype = 2;
        pindex = index -32;      // 0 .. 17 (= PARAM8_SIZE -1)
      }
      else {                     // SetOption50 .. 81 = Settings.flag3
        ptype = 1;
        pindex = index -50;      // 0 .. 31
      }
      if (payload >= 0) {
        if (0 == ptype) {        // SetOption0 .. 31
          if (payload <= 1) {
            switch (pindex) {
              case 5:            // mqtt_power_retain (CMND_POWERRETAIN)
              case 6:            // mqtt_button_retain (CMND_BUTTONRETAIN)
              case 7:            // mqtt_switch_retain (CMND_SWITCHRETAIN)
              case 9:            // mqtt_sensor_retain (CMND_SENSORRETAIN)
              case 14:           // interlock (CMND_INTERLOCK)
              case 22:           // mqtt_serial (SerialSend and SerialLog)
              case 23:           // mqtt_serial_raw (SerialSend)
              case 25:           // knx_enabled (Web config)
              case 27:           // knx_enable_enhancement (Web config)
                ptype = 99;      // Command Error
                break;           // Ignore command SetOption
              case 3:            // mqtt
              case 15:           // pwm_control
                restart_flag = 2;
              default:
                bitWrite(Settings.flag.data, pindex, payload);
            }
            if (12 == pindex) {  // stop_flash_rotate
              stop_flash_rotate = payload;
              SettingsSave(2);
            }
#ifdef USE_HOME_ASSISTANT
            if ((19 == pindex) || (30 == pindex)) {
              HAssDiscover();    // Delayed execution to provide enough resources during hass_discovery or hass_light
            }
#endif  // USE_HOME_ASSISTANT
          }
        }
        else if (1 == ptype) {   // SetOption50 .. 81
          if (payload <= 1) {
            bitWrite(Settings.flag3.data, pindex, payload);
            if (5 == pindex) {   // SetOption55
              if (0 == payload) {
                restart_flag = 2;  // Disable mDNS needs restart
              }
            }
            if (10 == pindex) {  // SetOption60 enable or disable traditional sleep
              WiFiSetSleepMode();  // Update WiFi sleep mode accordingly
            }
          }
        }
        else {                   // SetOption32 .. 49
          uint8_t param_low = 0;
          uint8_t param_high = 255;
          switch (pindex) {
            case P_HOLD_TIME:
            case P_MAX_POWER_RETRY:
              param_low = 1;
              param_high = 250;
              break;
            case P_TUYA_RELAYS:
              param_high = 8;
              break;
          }
          if ((payload >= param_low) && (payload <= param_high)) {
            Settings.param[pindex] = payload;
            switch (pindex) {
#ifdef USE_LIGHT
              case P_RGB_REMAP:
                LightUpdateColorMapping();
                break;
#endif
#if defined(USE_IR_REMOTE) && defined(USE_IR_RECEIVE)
              case P_IR_UNKNOW_THRESHOLD:
                IrReceiveUpdateThreshold();
                break;
#endif
#ifdef USE_TUYA_DIMMER
              case P_TUYA_RELAYS:
                restart_flag = 2;  // Need a restart to update GUI
                break;
#endif
            }
          }
        }
      }
      if (ptype < 99) {
        if (2 == ptype) snprintf_P(stemp1, sizeof(stemp1), PSTR("%d"), Settings.param[pindex]);
        Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, (2 == ptype) ? stemp1 : (1 == ptype) ? GetStateText(bitRead(Settings.flag3.data, pindex)) : GetStateText(bitRead(Settings.flag.data, pindex)));
      }
    }
    else if (CMND_TEMPERATURE_RESOLUTION == command_code) {
      if ((payload >= 0) && (payload <= 3)) {
        Settings.flag2.temperature_resolution = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag2.temperature_resolution);
    }
    else if (CMND_HUMIDITY_RESOLUTION == command_code) {
      if ((payload >= 0) && (payload <= 3)) {
        Settings.flag2.humidity_resolution = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag2.humidity_resolution);
    }
    else if (CMND_PRESSURE_RESOLUTION == command_code) {
      if ((payload >= 0) && (payload <= 3)) {
        Settings.flag2.pressure_resolution = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag2.pressure_resolution);
    }
    else if (CMND_POWER_RESOLUTION == command_code) {
      if ((payload >= 0) && (payload <= 3)) {
        Settings.flag2.wattage_resolution = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag2.wattage_resolution);
    }
    else if (CMND_VOLTAGE_RESOLUTION == command_code) {
      if ((payload >= 0) && (payload <= 3)) {
        Settings.flag2.voltage_resolution = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag2.voltage_resolution);
    }
    else if (CMND_FREQUENCY_RESOLUTION == command_code) {
      if ((payload >= 0) && (payload <= 3)) {
        Settings.flag2.frequency_resolution = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag2.frequency_resolution);
    }
    else if (CMND_CURRENT_RESOLUTION == command_code) {
      if ((payload >= 0) && (payload <= 3)) {
        Settings.flag2.current_resolution = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag2.current_resolution);
    }
    else if (CMND_ENERGY_RESOLUTION == command_code) {
      if ((payload >= 0) && (payload <= 5)) {
        Settings.flag2.energy_resolution = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag2.energy_resolution);
    }
    else if (CMND_WEIGHT_RESOLUTION == command_code) {
      if ((payload >= 0) && (payload <= 3)) {
        Settings.flag2.weight_resolution = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag2.weight_resolution);
    }
    else if (CMND_MODULE == command_code) {
      if ((payload >= 0) && (payload <= MAXMODULE)) {
        bool present = false;
        if (0 == payload) {
          payload = USER_MODULE;
          present = true;
        } else {
          payload--;
          present = ValidTemplateModule(payload);
        }
        if (present) {
          Settings.last_module = Settings.module;
          Settings.module = payload;
          SetModuleType();
          if (Settings.last_module != payload) {
            for (uint32_t i = 0; i < sizeof(Settings.my_gp); i++) {
              Settings.my_gp.io[i] = GPIO_NONE;
            }
          }
          restart_flag = 2;
        }
      }
      Response_P(S_JSON_COMMAND_NVALUE_SVALUE, command, ModuleNr(), ModuleName().c_str());
    }
    else if (CMND_MODULES == command_code) {
      uint8_t midx = USER_MODULE;
      for (uint32_t i = 0; i <= sizeof(kModuleNiceList); i++) {
        if (i > 0) { midx = pgm_read_byte(kModuleNiceList + i -1); }
        if (!jsflg) {
          Response_P(PSTR("{\"" D_CMND_MODULES "%d\":["), lines);
        } else {
          ResponseAppend_P(PSTR(","));
        }
        jsflg = true;
        uint8_t j = i ? midx +1 : 0;
        if ((ResponseAppend_P(PSTR("\"%d (%s)\""), j, AnyModuleName(midx).c_str()) > (LOGSZ - TOPSZ)) || (i == sizeof(kModuleNiceList))) {
          ResponseAppend_P(PSTR("]}"));
          MqttPublishPrefixTopic_P(RESULT_OR_STAT, type);
          jsflg = false;
          lines++;
        }
      }
      mqtt_data[0] = '\0';
    }
#ifndef USE_ADC_VCC
    else if (CMND_ADC == command_code) {
      if (ValidAdc() && (payload >= 0) && (payload < ADC0_END)) {
        Settings.my_adc0 = payload;
        restart_flag = 2;
      }
      Response_P(PSTR("{\"" D_CMND_ADC "0\":\"%d (%s)\"}"), Settings.my_adc0, GetTextIndexed(stemp1, sizeof(stemp1), Settings.my_adc0, kAdc0Names));
    }
    else if (CMND_ADCS == command_code) {
      Response_P(PSTR("{\"" D_CMND_ADCS "\":["));
      for (uint32_t i = 0; i < ADC0_END; i++) {
        if (jsflg) {
          ResponseAppend_P(PSTR(","));
        }
        jsflg = true;
        ResponseAppend_P(PSTR("\"%d (%s)\""), i, GetTextIndexed(stemp1, sizeof(stemp1), i, kAdc0Names));
      }
      ResponseAppend_P(PSTR("]}"));
    }
#endif  // USE_ADC_VCC
    else if ((CMND_GPIO == command_code) && (index < sizeof(Settings.my_gp))) {
      myio cmodule;
      ModuleGpios(&cmodule);
      if (ValidGPIO(index, cmodule.io[index]) && (payload >= 0) && (payload < GPIO_SENSOR_END)) {
        bool present = false;
        for (uint32_t i = 0; i < sizeof(kGpioNiceList); i++) {
          uint8_t midx = pgm_read_byte(kGpioNiceList + i);
          if (midx == payload) { present = true; }
        }
        if (present) {
          for (uint32_t i = 0; i < sizeof(Settings.my_gp); i++) {
            if (ValidGPIO(i, cmodule.io[i]) && (Settings.my_gp.io[i] == payload)) {
              Settings.my_gp.io[i] = GPIO_NONE;
            }
          }
          Settings.my_gp.io[index] = payload;
          restart_flag = 2;
        }
      }
      Response_P(PSTR("{"));
      for (uint32_t i = 0; i < sizeof(Settings.my_gp); i++) {
        if (ValidGPIO(i, cmodule.io[i])) {
          if (jsflg) { ResponseAppend_P(PSTR(",")); }
          jsflg = true;
          ResponseAppend_P(PSTR("\"" D_CMND_GPIO "%d\":\"%d (%s)\""), i, Settings.my_gp.io[i], GetTextIndexed(stemp1, sizeof(stemp1), Settings.my_gp.io[i], kSensorNames));
        }
      }
      if (jsflg) {
        ResponseJsonEnd();
      } else {
        Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_NOT_SUPPORTED);
      }
    }
    else if (CMND_GPIOS == command_code) {
      myio cmodule;
      ModuleGpios(&cmodule);
      uint8_t midx;
      for (uint32_t i = 0; i < sizeof(kGpioNiceList); i++) {
        midx = pgm_read_byte(kGpioNiceList + i);
        if (!GetUsedInModule(midx, cmodule.io)) {
          if (!jsflg) {
            Response_P(PSTR("{\"" D_CMND_GPIOS "%d\":["), lines);
          } else {
            ResponseAppend_P(PSTR(","));
          }
          jsflg = true;
          if ((ResponseAppend_P(PSTR("\"%d (%s)\""), midx, GetTextIndexed(stemp1, sizeof(stemp1), midx, kSensorNames)) > (LOGSZ - TOPSZ)) || (i == sizeof(kGpioNiceList) -1)) {
            ResponseAppend_P(PSTR("]}"));
            MqttPublishPrefixTopic_P(RESULT_OR_STAT, type);
            jsflg = false;
            lines++;
          }
        }
      }
      mqtt_data[0] = '\0';
    }
    else if (CMND_TEMPLATE == command_code) {
      // {"NAME":"Generic","GPIO":[17,254,29,254,7,254,254,254,138,254,139,254,254],"FLAG":1,"BASE":255}
      bool error = false;

      if (strstr(dataBuf, "{") == nullptr) {  // If no JSON it must be parameter
        if ((payload > 0) && (payload <= MAXMODULE)) {
          payload--;
          if (ValidTemplateModule(payload)) {
            ModuleDefault(payload);     // Copy template module
            if (USER_MODULE == Settings.module) { restart_flag = 2; }
          }
        }
        else if (0 == payload) {        // Copy current template to user template
          if (Settings.module != USER_MODULE) {
            ModuleDefault(Settings.module);
          }
        }
        else if (255 == payload) {      // Copy current module with user configured GPIO
          if (Settings.module != USER_MODULE) {
            ModuleDefault(Settings.module);
          }
          snprintf_P(Settings.user_template.name, sizeof(Settings.user_template.name), PSTR("Merged"));
          uint8_t j = 0;
          for (uint32_t i = 0; i < sizeof(mycfgio); i++) {
            if (6 == i) { j = 9; }
            if (8 == i) { j = 12; }
            if (my_module.io[j] > GPIO_NONE) {
              Settings.user_template.gp.io[i] = my_module.io[j];
            }
            j++;
          }
        }
      }
      else {
        if (JsonTemplate(dataBuf)) {    // Free 336 bytes StaticJsonBuffer stack space by moving code to function
          if (USER_MODULE == Settings.module) { restart_flag = 2; }
        } else {
          Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_INVALID_JSON);
          error = true;
        }
      }
      if (!error) { TemplateJson(); }
    }
    else if ((CMND_PWM == command_code) && pwm_present && (index > 0) && (index <= MAX_PWMS)) {
      if ((payload >= 0) && (payload <= Settings.pwm_range) && (pin[GPIO_PWM1 + index -1] < 99)) {
        Settings.pwm_value[index -1] = payload;
        analogWrite(pin[GPIO_PWM1 + index -1], bitRead(pwm_inverted, index -1) ? Settings.pwm_range - payload : payload);
      }
      Response_P(PSTR("{"));
      MqttShowPWMState();  // Render the PWM status to MQTT
      ResponseJsonEnd();
    }
    else if (CMND_PWMFREQUENCY == command_code) {
      if ((1 == payload) || ((payload >= PWM_MIN) && (payload <= PWM_MAX))) {
        Settings.pwm_frequency = (1 == payload) ? PWM_FREQ : payload;
        analogWriteFreq(Settings.pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.pwm_frequency);
    }
    else if (CMND_PWMRANGE == command_code) {
      if ((1 == payload) || ((payload > 254) && (payload < 1024))) {
        Settings.pwm_range = (1 == payload) ? PWM_RANGE : payload;
        for (uint32_t i = 0; i < MAX_PWMS; i++) {
          if (Settings.pwm_value[i] > Settings.pwm_range) {
            Settings.pwm_value[i] = Settings.pwm_range;
          }
        }
        analogWriteRange(Settings.pwm_range);      // Default is 1023 (Arduino.h)
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.pwm_range);
    }
    else if ((CMND_COUNTER == command_code) && (index > 0) && (index <= MAX_COUNTERS)) {
      if ((data_len > 0) && (pin[GPIO_CNTR1 + index -1] < 99)) {
        if ((dataBuf[0] == '-') || (dataBuf[0] == '+')) {
          RtcSettings.pulse_counter[index -1] += payload32;
          Settings.pulse_counter[index -1] += payload32;
        } else {
          RtcSettings.pulse_counter[index -1] = payload32;
          Settings.pulse_counter[index -1] = payload32;
        }
      }
      Response_P(S_JSON_COMMAND_INDEX_LVALUE, command, index, RtcSettings.pulse_counter[index -1]);
    }
    else if ((CMND_COUNTERTYPE == command_code) && (index > 0) && (index <= MAX_COUNTERS)) {
      if ((payload >= 0) && (payload <= 1) && (pin[GPIO_CNTR1 + index -1] < 99)) {
        bitWrite(Settings.pulse_counter_type, index -1, payload &1);
        RtcSettings.pulse_counter[index -1] = 0;
        Settings.pulse_counter[index -1] = 0;
      }
      Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, index, bitRead(Settings.pulse_counter_type, index -1));
    }
    else if (CMND_COUNTERDEBOUNCE == command_code) {
      if ((data_len > 0) && (payload16 < 32001)) {
        Settings.pulse_counter_debounce = payload16;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.pulse_counter_debounce);
    }
    else if (CMND_BUTTONDEBOUNCE == command_code) {
      if ((payload > 39) && (payload < 1001)) {
        Settings.button_debounce = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.button_debounce);
    }
    else if (CMND_SWITCHDEBOUNCE == command_code) {
      if ((payload > 39) && (payload < 1001)) {
        Settings.switch_debounce = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.switch_debounce);
    }
    else if (CMND_BAUDRATE == command_code) {
      if (payload32 > 1200) {
        payload32 /= 1200;  // Make it a valid baudrate
        baudrate = (1 == payload) ? APP_BAUDRATE : payload32 * 1200;
        SetSerialBaudrate(baudrate);
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.baudrate * 1200);
    }
    else if ((CMND_SERIALSEND == command_code) && (index > 0) && (index <= 5)) {
      SetSeriallog(LOG_LEVEL_NONE);
      Settings.flag.mqtt_serial = 1;
      Settings.flag.mqtt_serial_raw = (index > 3) ? 1 : 0;
      if (data_len > 0) {
        if (1 == index) {
          Serial.printf("%s\n", dataBuf);                    // "Hello Tiger\n"
        }
        else if (2 == index || 4 == index) {
          for (uint32_t i = 0; i < data_len; i++) {
            Serial.write(dataBuf[i]);                        // "Hello Tiger" or "A0"
          }
        }
        else if (3 == index) {
          uint16_t dat_len = data_len;
          Serial.printf("%s", Unescape(dataBuf, &dat_len));  // "Hello\f"
        }
        else if (5 == index) {
          SerialSendRaw(RemoveSpace(dataBuf));               // "AA004566" as hex values
        }
        Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
      }
    }
    else if (CMND_SERIALDELIMITER == command_code) {
      if ((data_len > 0) && (payload < 256)) {
        if (payload > 0) {
          Settings.serial_delimiter = payload;
        } else {
          uint16_t dat_len = data_len;
          Unescape(dataBuf, &dat_len);
          Settings.serial_delimiter = dataBuf[0];
        }
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.serial_delimiter);
    }
    else if (CMND_SYSLOG == command_code) {
      if ((payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_ALL)) {
        SetSyslog(payload);
      }
      Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, command, Settings.syslog_level, syslog_level);
    }
    else if (CMND_LOGHOST == command_code) {
      if ((data_len > 0) && (data_len < sizeof(Settings.syslog_host))) {
        strlcpy(Settings.syslog_host, (SC_DEFAULT == Shortcut(dataBuf)) ? SYS_LOG_HOST : dataBuf, sizeof(Settings.syslog_host));
      }
      Response_P(S_JSON_COMMAND_SVALUE, command, Settings.syslog_host);
    }
    else if (CMND_LOGPORT == command_code) {
      if (payload16 > 0) {
        Settings.syslog_port = (1 == payload16) ? SYS_LOG_PORT : payload16;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.syslog_port);
    }
    else if ((CMND_IPADDRESS == command_code) && (index > 0) && (index <= 4)) {
      if (ParseIp(&address, dataBuf)) {
        Settings.ip_address[index -1] = address;
//        restart_flag = 2;
      }
      snprintf_P(stemp1, sizeof(stemp1), PSTR(" (%s)"), WiFi.localIP().toString().c_str());
      Response_P(S_JSON_COMMAND_INDEX_SVALUE_SVALUE, command, index, IPAddress(Settings.ip_address[index -1]).toString().c_str(), (1 == index) ? stemp1:"");
    }
    else if ((CMND_NTPSERVER == command_code) && (index > 0) && (index <= 3)) {
      if ((data_len > 0) && (data_len < sizeof(Settings.ntp_server[0]))) {
        strlcpy(Settings.ntp_server[index -1], (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? (1==index)?NTP_SERVER1:(2==index)?NTP_SERVER2:NTP_SERVER3 : dataBuf, sizeof(Settings.ntp_server[0]));
        for (i = 0; i < strlen(Settings.ntp_server[index -1]); i++) {
          if (Settings.ntp_server[index -1][i] == ',') Settings.ntp_server[index -1][i] = '.';
        }
//        restart_flag = 2;  // Issue #3890
        ntp_force_sync = true;
      }
      Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.ntp_server[index -1]);
    }
    else if (CMND_AP == command_code) {
      if ((payload >= 0) && (payload <= 2)) {
        switch (payload) {
        case 0:  // Toggle
          Settings.sta_active ^= 1;
          break;
        case 1:  // AP1
        case 2:  // AP2
          Settings.sta_active = payload -1;
        }
        restart_flag = 2;
      }
      Response_P(S_JSON_COMMAND_NVALUE_SVALUE, command, Settings.sta_active +1, Settings.sta_ssid[Settings.sta_active]);
    }
    else if ((CMND_SSID == command_code) && (index > 0) && (index <= 2)) {
      if ((data_len > 0) && (data_len < sizeof(Settings.sta_ssid[0]))) {
        strlcpy(Settings.sta_ssid[index -1], (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? (1 == index) ? STA_SSID1 : STA_SSID2 : dataBuf, sizeof(Settings.sta_ssid[0]));
        Settings.sta_active = index -1;
        restart_flag = 2;
      }
      Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.sta_ssid[index -1]);
    }
    else if ((CMND_PASSWORD == command_code) && (index > 0) && (index <= 2)) {
      if ((data_len > 4 || SC_CLEAR == Shortcut(dataBuf) || SC_DEFAULT == Shortcut(dataBuf)) && (data_len < sizeof(Settings.sta_pwd[0]))) {
        strlcpy(Settings.sta_pwd[index -1], (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? (1 == index) ? STA_PASS1 : STA_PASS2 : dataBuf, sizeof(Settings.sta_pwd[0]));
        Settings.sta_active = index -1;
        restart_flag = 2;
        Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.sta_pwd[index -1]);
      } else {
        Response_P(S_JSON_COMMAND_INDEX_ASTERISK, command, index);
      }
    }
    else if (CMND_HOSTNAME == command_code) {
      if (!grpflg && (data_len > 0) && (data_len < sizeof(Settings.hostname))) {
        strlcpy(Settings.hostname, (SC_DEFAULT == Shortcut(dataBuf)) ? WIFI_HOSTNAME : dataBuf, sizeof(Settings.hostname));
        if (strstr(Settings.hostname, "%") != nullptr) {
          strlcpy(Settings.hostname, WIFI_HOSTNAME, sizeof(Settings.hostname));
        }
        restart_flag = 2;
      }
      Response_P(S_JSON_COMMAND_SVALUE, command, Settings.hostname);
    }
    else if (CMND_WIFICONFIG == command_code) {
      if ((payload >= WIFI_RESTART) && (payload < MAX_WIFI_OPTION)) {
        Settings.sta_config = payload;
        wifi_state_flag = Settings.sta_config;
        snprintf_P(stemp1, sizeof(stemp1), kWifiConfig[Settings.sta_config]);
        Response_P(PSTR("{\"" D_CMND_WIFICONFIG "\":\"%s " D_JSON_SELECTED "\"}"), stemp1);
        if (WifiState() > WIFI_RESTART) {
//          ResponseAppend_P(PSTR(" after restart"));
          restart_flag = 2;
        }
      } else {
        snprintf_P(stemp1, sizeof(stemp1), kWifiConfig[Settings.sta_config]);
        Response_P(S_JSON_COMMAND_NVALUE_SVALUE, command, Settings.sta_config, stemp1);
      }
    }
    else if ((CMND_FRIENDLYNAME == command_code) && (index > 0) && (index <= MAX_FRIENDLYNAMES)) {
      if ((data_len > 0) && (data_len < sizeof(Settings.friendlyname[0]))) {
        if (1 == index) {
          snprintf_P(stemp1, sizeof(stemp1), PSTR(FRIENDLY_NAME));
        } else {
          snprintf_P(stemp1, sizeof(stemp1), PSTR(FRIENDLY_NAME "%d"), index);
        }
        strlcpy(Settings.friendlyname[index -1], (SC_DEFAULT == Shortcut(dataBuf)) ? stemp1 : dataBuf, sizeof(Settings.friendlyname[index -1]));
      }
      Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.friendlyname[index -1]);
    }
    else if ((CMND_SWITCHMODE == command_code) && (index > 0) && (index <= MAX_SWITCHES)) {
      if ((payload >= 0) && (payload < MAX_SWITCH_OPTION)) {
        Settings.switchmode[index -1] = payload;
      }
      Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, index, Settings.switchmode[index-1]);
    }
    else if (CMND_INTERLOCK == command_code) {                      // Interlock 0 - Off, Interlock 1 - On, Interlock 1,2 3,4 5,6,7
      uint8_t max_relays = devices_present;
      if (light_type) { max_relays--; }
      if (max_relays > sizeof(Settings.interlock[0]) * 8) { max_relays = sizeof(Settings.interlock[0]) * 8; }
      if (max_relays > 1) {                                         // Only interlock with more than 1 relay
        if (data_len > 0) {
          if (strstr(dataBuf, ",") != nullptr) {                    // Interlock entry
            for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) { Settings.interlock[i] = 0; }  // Reset current interlocks
            char *group;
            char *q;
            uint8_t group_index = 0;
            power_t relay_mask = 0;
            for (group = strtok_r(dataBuf, " ", &q); group && group_index < MAX_INTERLOCKS; group = strtok_r(nullptr, " ", &q)) {
              char *str;
              for (str = strtok_r(group, ",", &p); str; str = strtok_r(nullptr, ",", &p)) {
                int pbit = atoi(str);
                if ((pbit > 0) && (pbit <= max_relays)) {           // Only valid relays
                  pbit--;
                  if (!bitRead(relay_mask, pbit)) {                 // Only relay once
                    bitSet(relay_mask, pbit);
                    bitSet(Settings.interlock[group_index], pbit);
                  }
                }
              }
              group_index++;
            }
            for (uint32_t i = 0; i < group_index; i++) {
              uint8_t minimal_bits = 0;
              for (uint32_t j = 0; j < max_relays; j++) {
                if (bitRead(Settings.interlock[i], j)) { minimal_bits++; }
              }
              if (minimal_bits < 2) { Settings.interlock[i] = 0; }  // Discard single relay as interlock
            }
          } else {
            Settings.flag.interlock = payload &1;                   // Enable/disable interlock
            if (Settings.flag.interlock) {
              SetDevicePower(power, SRC_IGNORE);                    // Remove multiple relays if set
            }
          }
        }
        Response_P(PSTR("{\"" D_CMND_INTERLOCK "\":\"%s\",\"" D_JSON_GROUPS "\":\""), GetStateText(Settings.flag.interlock));
        uint8_t anygroup = 0;
        for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
          if (Settings.interlock[i]) {
            anygroup++;
            ResponseAppend_P(PSTR("%s"), (anygroup > 1) ? " " : "");
            uint8_t anybit = 0;
            power_t mask = 1;
            for (uint32_t j = 0; j < max_relays; j++) {
              if (Settings.interlock[i] & mask) {
                anybit++;
                ResponseAppend_P(PSTR("%s%d"), (anybit > 1) ? "," : "", j +1);
              }
              mask <<= 1;
            }
          }
        }
        if (!anygroup) {
          for (uint32_t j = 1; j <= max_relays; j++) {
            ResponseAppend_P(PSTR("%s%d"), (j > 1) ? "," : "", j);
          }
        }
        ResponseAppend_P(PSTR("\"}"));
      } else {
        Settings.flag.interlock = 0;
        Response_P(S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag.interlock));
      }
    }
    else if (CMND_TELEPERIOD == command_code) {
      if ((payload >= 0) && (payload < 3601)) {
        Settings.tele_period = (1 == payload) ? TELE_PERIOD : payload;
        if ((Settings.tele_period > 0) && (Settings.tele_period < 10)) Settings.tele_period = 10;   // Do not allow periods < 10 seconds
        tele_period = Settings.tele_period;
      }
      Response_P(S_JSON_COMMAND_NVALUE_UNIT, command, Settings.tele_period, (Settings.flag.value_units) ? " " D_UNIT_SECOND : "");
    }
    else if (CMND_RESET == command_code) {
      switch (payload) {
      case 1:
        restart_flag = 211;
        Response_P(S_JSON_COMMAND_SVALUE, command , D_JSON_RESET_AND_RESTARTING);
        break;
      case 2 ... 6:
        restart_flag = 210 + payload;
        Response_P(PSTR("{\"" D_CMND_RESET "\":\"" D_JSON_ERASE ", " D_JSON_RESET_AND_RESTARTING "\"}"));
        break;
      default:
        Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_ONE_TO_RESET);
      }
    }
    else if (CMND_TIME == command_code) {
      if (data_len > 0) {
        RtcSetTime(payload32);
      }
      ResponseBeginTime();
      ResponseJsonEnd();
    }
    else if (CMND_TIMEZONE == command_code) {
      if ((data_len > 0) && (payload >= -13)) {
        Settings.timezone = payload;
        Settings.timezone_minutes = 0;
        if (payload < 15) {
          p = strtok (dataBuf, ":");
          if (p) {
            p = strtok (nullptr, ":");
            if (p) {
              Settings.timezone_minutes = strtol(p, nullptr, 10);
              if (Settings.timezone_minutes > 59) { Settings.timezone_minutes = 59; }
            }
          }
        } else {
          Settings.timezone = 99;
        }
        ntp_force_sync = true;
      }
      if (99 == Settings.timezone) {
        Response_P(S_JSON_COMMAND_NVALUE, command, Settings.timezone);
      } else {
        snprintf_P(stemp1, sizeof(stemp1), PSTR("%+03d:%02d"), Settings.timezone, Settings.timezone_minutes);
        Response_P(S_JSON_COMMAND_SVALUE, command, stemp1);
      }
    }
    else if ((CMND_TIMESTD == command_code) || (CMND_TIMEDST == command_code)) {
      // TimeStd 0/1, 0/1/2/3/4, 1..12, 1..7, 0..23, +/-780
      uint8_t ts = 0;
      if (CMND_TIMEDST == command_code) { ts = 1; }
      if (data_len > 0) {
        if (strstr(dataBuf, ",") != nullptr) {   // Process parameter entry
          uint8_t tpos = 0;                      // Parameter index
          int value = 0;
          p = dataBuf;                           // Parameters like "1, 2,3 , 4 ,5, -120" or ",,,,,+240"
          char *q = p;                           // Value entered flag
          while (p && (tpos < 7)) {
            if (p > q) {                         // Any value entered
              if (1 == tpos) { Settings.tflag[ts].hemis = value &1; }
              if (2 == tpos) { Settings.tflag[ts].week = (value < 0) ? 0 : (value > 4) ? 4 : value; }
              if (3 == tpos) { Settings.tflag[ts].month = (value < 1) ? 1 : (value > 12) ? 12 : value; }
              if (4 == tpos) { Settings.tflag[ts].dow = (value < 1) ? 1 : (value > 7) ? 7 : value; }
              if (5 == tpos) { Settings.tflag[ts].hour = (value < 0) ? 0 : (value > 23) ? 23 : value; }
              if (6 == tpos) { Settings.toffset[ts] = (value < -900) ? -900 : (value > 900) ? 900 : value; }
            }
            p = Trim(p);                        // Skip spaces
            if (tpos && (*p == ',')) { p++; }    // Skip separator
            p = Trim(p);                        // Skip spaces
            q = p;                               // Reset any value entered flag
            value = strtol(p, &p, 10);
            tpos++;                              // Next parameter
          }
          ntp_force_sync = true;
        } else {
          if (0 == payload) {
            if (0 == ts) {
              SettingsResetStd();
            } else {
              SettingsResetDst();
            }
          }
          ntp_force_sync = true;
        }
      }
      Response_P(PSTR("{\"%s\":{\"Hemisphere\":%d,\"Week\":%d,\"Month\":%d,\"Day\":%d,\"Hour\":%d,\"Offset\":%d}}"),
        command, Settings.tflag[ts].hemis, Settings.tflag[ts].week, Settings.tflag[ts].month, Settings.tflag[ts].dow, Settings.tflag[ts].hour, Settings.toffset[ts]);
    }
    else if (CMND_ALTITUDE == command_code) {
      if ((data_len > 0) && ((payload >= -30000) && (payload <= 30000))) {
        Settings.altitude = payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.altitude);
    }
    else if ((CMND_LEDPOWER == command_code) && (index > 0) && (index <= MAX_LEDS)) {
/*
      if ((payload >= 0) && (payload <= 2)) {
        Settings.ledstate &= 8;
        switch (payload) {
        case 0: // Off
        case 1: // On
          Settings.ledstate = payload << 3;
          break;
        case 2: // Toggle
          Settings.ledstate ^= 8;
          break;
        }
        blinks = 0;
        SetLedPowerIdx(index -1, Settings.ledstate &8);
      }
      Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, GetStateText(bitRead(Settings.ledstate, 3)));
*/
/*
      if (99 == pin[GPIO_LEDLNK]) {
        if ((payload >= 0) && (payload <= 2)) {
          Settings.ledstate &= 8;
          switch (payload) {
          case 0: // Off
          case 1: // On
            Settings.ledstate = payload << 3;
            break;
          case 2: // Toggle
            Settings.ledstate ^= 8;
            break;
          }
          blinks = 0;
          SetLedPower(Settings.ledstate &8);
        }
        Response_P(S_JSON_COMMAND_SVALUE, command, GetStateText(bitRead(Settings.ledstate, 3)));
      } else {
        if ((payload >= 0) && (payload <= 2)) {
          Settings.ledstate &= 8;                // Disable power control
          uint8_t mask = 1 << (index -1);        // Led to control
          switch (payload) {
          case 0: // Off
            led_power &= (0xFF ^ mask);
          case 1: // On
            led_power |= mask;
            break;
          case 2: // Toggle
            led_power ^= mask;
            break;
          }
          blinks = 0;
          SetLedPowerIdx(index -1, (led_power & mask));
        }
        Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, GetStateText(bitRead(led_power, index -1)));
      }
*/
      if (99 == pin[GPIO_LEDLNK]) { index = 1; }
      if ((payload >= 0) && (payload <= 2)) {
        Settings.ledstate &= 8;                // Disable power control
        uint8_t mask = 1 << (index -1);        // Led to control
        switch (payload) {
        case 0: // Off
          led_power &= (0xFF ^ mask);
          Settings.ledstate = 0;
          break;
        case 1: // On
          led_power |= mask;
          Settings.ledstate = 8;
          break;
        case 2: // Toggle
          led_power ^= mask;
          Settings.ledstate ^= 8;
          break;
        }
        blinks = 0;
        if (99 == pin[GPIO_LEDLNK]) {
          SetLedPower(Settings.ledstate &8);
        } else {
          SetLedPowerIdx(index -1, (led_power & mask));
        }
      }
      uint8_t state = bitRead(led_power, index -1);
      if (99 == pin[GPIO_LEDLNK]) {
        state = bitRead(Settings.ledstate, 3);
      }
      Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, GetStateText(state));
    }
    else if (CMND_LEDSTATE == command_code) {
      if ((payload >= 0) && (payload < MAX_LED_OPTION)) {
        Settings.ledstate = payload;
        if (!Settings.ledstate) {
          SetLedPowerAll(0);
          SetLedLink(0);
        }
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.ledstate);
    }
    else if (CMND_LEDMASK == command_code) {
      if (data_len > 0) {
        Settings.ledmask = payload16;
      }
      snprintf_P(stemp1, sizeof(stemp1), PSTR("%d (0x%04X)"), Settings.ledmask, Settings.ledmask);
      Response_P(S_JSON_COMMAND_SVALUE, command, stemp1);
    }
#ifdef USE_I2C
    else if ((CMND_I2CSCAN == command_code) && i2c_flg) {
      I2cScan(mqtt_data, sizeof(mqtt_data));
    }
#endif  // USE_I2C
    else type = nullptr;  // Unknown command
  }
  if (type == nullptr) {
    blinks = 201;
    snprintf_P(topicBuf, sizeof(topicBuf), PSTR(D_JSON_COMMAND));
    Response_P(PSTR("{\"" D_JSON_COMMAND "\":\"" D_JSON_UNKNOWN "\"}"));
    type = (char*)topicBuf;
  }
  if (mqtt_data[0] != '\0') {
     MqttPublishPrefixTopic_P(RESULT_OR_STAT, type);
#ifdef USE_SCRIPT
     XdrvRulesProcess();
#endif
  }
  fallback_topic_flag = false;
}

/********************************************************************************************/

void PublishStatus(uint8_t payload)
{
  uint8_t option = STAT;
  char stemp[MAX_FRIENDLYNAMES * (sizeof(Settings.friendlyname[0]) +MAX_FRIENDLYNAMES)];
  char stemp2[100];

  // Workaround MQTT - TCP/IP stack queueing when SUB_PREFIX = PUB_PREFIX
  if (!strcmp(Settings.mqtt_prefix[0],Settings.mqtt_prefix[1]) && (!payload)) { option++; }  // TELE

  if ((!Settings.flag.mqtt_enabled) && (6 == payload)) { payload = 99; }
  if (!energy_flg && (9 == payload)) { payload = 99; }

  if ((0 == payload) || (99 == payload)) {
    uint8_t maxfn = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : (!devices_present) ? 1 : devices_present;
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan()) { maxfn = 1; }
#endif  // USE_SONOFF_IFAN
    stemp[0] = '\0';
    for (uint32_t i = 0; i < maxfn; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%s%s\"%s\"" ), stemp, (i > 0 ? "," : ""), Settings.friendlyname[i]);
    }
    stemp2[0] = '\0';
    for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
      snprintf_P(stemp2, sizeof(stemp2), PSTR("%s%s%d" ), stemp2, (i > 0 ? "," : ""), Settings.switchmode[i]);
    }
    Response_P(PSTR("{\"" D_CMND_STATUS "\":{\"" D_CMND_MODULE "\":%d,\"" D_CMND_FRIENDLYNAME "\":[%s],\"" D_CMND_TOPIC "\":\"%s\",\"" D_CMND_BUTTONTOPIC "\":\"%s\",\"" D_CMND_POWER "\":%d,\"" D_CMND_POWERONSTATE "\":%d,\"" D_CMND_LEDSTATE "\":%d,\"" D_CMND_LEDMASK "\":\"%04X\",\"" D_CMND_SAVEDATA "\":%d,\"" D_JSON_SAVESTATE "\":%d,\"" D_CMND_SWITCHTOPIC "\":\"%s\",\"" D_CMND_SWITCHMODE "\":[%s],\"" D_CMND_BUTTONRETAIN "\":%d,\"" D_CMND_SWITCHRETAIN "\":%d,\"" D_CMND_SENSORRETAIN "\":%d,\"" D_CMND_POWERRETAIN "\":%d}}"),
      ModuleNr(), stemp, mqtt_topic, Settings.button_topic, power, Settings.poweronstate, Settings.ledstate, Settings.ledmask, Settings.save_data, Settings.flag.save_state, Settings.switch_topic, stemp2, Settings.flag.mqtt_button_retain, Settings.flag.mqtt_switch_retain, Settings.flag.mqtt_sensor_retain, Settings.flag.mqtt_power_retain);
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS));
  }

  if ((0 == payload) || (1 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS1_PARAMETER "\":{\"" D_JSON_BAUDRATE "\":%d,\"" D_CMND_GROUPTOPIC "\":\"%s\",\"" D_CMND_OTAURL "\":\"%s\",\"" D_JSON_RESTARTREASON "\":\"%s\",\"" D_JSON_UPTIME "\":\"%s\",\"" D_JSON_STARTUPUTC "\":\"%s\",\"" D_CMND_SLEEP "\":%d,\"" D_JSON_CONFIG_HOLDER "\":%d,\"" D_JSON_BOOTCOUNT "\":%d,\"" D_JSON_SAVECOUNT "\":%d,\"" D_JSON_SAVEADDRESS "\":\"%X\"}}"),
      baudrate, Settings.mqtt_grptopic, Settings.ota_url, GetResetReason().c_str(), GetUptime().c_str(), GetDateAndTime(DT_RESTART).c_str(), Settings.sleep, Settings.cfg_holder, Settings.bootcount, Settings.save_flag, GetSettingsAddress());
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "1"));
  }

  if ((0 == payload) || (2 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS2_FIRMWARE "\":{\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_BUILDDATETIME "\":\"%s\",\"" D_JSON_BOOTVERSION "\":%d,\"" D_JSON_COREVERSION "\":\"" ARDUINO_ESP8266_RELEASE "\",\"" D_JSON_SDKVERSION "\":\"%s\"}}"),
      my_version, my_image, GetBuildDateAndTime().c_str(), ESP.getBootVersion(), ESP.getSdkVersion());
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "2"));
  }

  if ((0 == payload) || (3 == payload)) {
    stemp2[0] = '\0';
    for (uint32_t i = 0; i < PARAM8_SIZE; i++) {
      snprintf_P(stemp2, sizeof(stemp2), PSTR("%s%02X"), stemp2, Settings.param[i]);
    }
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS3_LOGGING "\":{\"" D_CMND_SERIALLOG "\":%d,\"" D_CMND_WEBLOG "\":%d,\"" D_CMND_SYSLOG "\":%d,\"" D_CMND_LOGHOST "\":\"%s\",\"" D_CMND_LOGPORT "\":%d,\"" D_CMND_SSID "\":[\"%s\",\"%s\"],\"" D_CMND_TELEPERIOD "\":%d,\"" D_JSON_RESOLUTION "\":\"%08X\",\"" D_CMND_SETOPTION "\":[\"%08X\",\"%s\",\"%08X\"]}}"),
      Settings.seriallog_level, Settings.weblog_level, Settings.syslog_level, Settings.syslog_host, Settings.syslog_port, Settings.sta_ssid[0], Settings.sta_ssid[1], Settings.tele_period, Settings.flag2.data, Settings.flag.data, stemp2, Settings.flag3.data);
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "3"));
  }

  if ((0 == payload) || (4 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS4_MEMORY "\":{\"" D_JSON_PROGRAMSIZE "\":%d,\"" D_JSON_FREEMEMORY "\":%d,\"" D_JSON_HEAPSIZE "\":%d,\"" D_JSON_PROGRAMFLASHSIZE "\":%d,\"" D_JSON_FLASHSIZE "\":%d,\"" D_JSON_FLASHCHIPID "\":\"%06X\",\"" D_JSON_FLASHMODE "\":%d,\"" D_JSON_FEATURES "\":[\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\"]}}"),
      ESP.getSketchSize()/1024, ESP.getFreeSketchSpace()/1024, ESP.getFreeHeap()/1024, ESP.getFlashChipSize()/1024, ESP.getFlashChipRealSize()/1024, ESP.getFlashChipId(), ESP.getFlashChipMode(), LANGUAGE_LCID, feature_drv1, feature_drv2, feature_sns1, feature_sns2);
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "4"));
  }

  if ((0 == payload) || (5 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS5_NETWORK "\":{\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%s\",\"" D_JSON_GATEWAY "\":\"%s\",\"" D_JSON_SUBNETMASK "\":\"%s\",\"" D_JSON_DNSSERVER "\":\"%s\",\"" D_JSON_MAC "\":\"%s\",\"" D_CMND_WEBSERVER "\":%d,\"" D_CMND_WIFICONFIG "\":%d}}"),
      my_hostname, WiFi.localIP().toString().c_str(), IPAddress(Settings.ip_address[1]).toString().c_str(), IPAddress(Settings.ip_address[2]).toString().c_str(), IPAddress(Settings.ip_address[3]).toString().c_str(),
      WiFi.macAddress().c_str(), Settings.webserver, Settings.sta_config);
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "5"));
  }

  if (((0 == payload) || (6 == payload)) && Settings.flag.mqtt_enabled) {
#ifdef USE_MQTT_AWS_IOT
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS6_MQTT "\":{\"" D_CMND_MQTTHOST "\":\"%s%s\",\"" D_CMND_MQTTPORT "\":%d,\"" D_CMND_MQTTCLIENT D_JSON_MASK "\":\"%s\",\"" D_CMND_MQTTCLIENT "\":\"%s\",\"" D_JSON_MQTT_COUNT "\":%d,\"MAX_PACKET_SIZE\":%d,\"KEEPALIVE\":%d}}"),
      Settings.mqtt_user, Settings.mqtt_host, Settings.mqtt_port, Settings.mqtt_client, mqtt_client, MqttConnectCount(), MQTT_MAX_PACKET_SIZE, MQTT_KEEPALIVE);
#else
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS6_MQTT "\":{\"" D_CMND_MQTTHOST "\":\"%s\",\"" D_CMND_MQTTPORT "\":%d,\"" D_CMND_MQTTCLIENT D_JSON_MASK "\":\"%s\",\"" D_CMND_MQTTCLIENT "\":\"%s\",\"" D_CMND_MQTTUSER "\":\"%s\",\"" D_JSON_MQTT_COUNT "\":%d,\"MAX_PACKET_SIZE\":%d,\"KEEPALIVE\":%d}}"),
      Settings.mqtt_host, Settings.mqtt_port, Settings.mqtt_client, mqtt_client, Settings.mqtt_user, MqttConnectCount(), MQTT_MAX_PACKET_SIZE, MQTT_KEEPALIVE);
#endif
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "6"));
  }

  if ((0 == payload) || (7 == payload)) {
    if (99 == Settings.timezone) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%d" ), Settings.timezone);
    } else {
      snprintf_P(stemp, sizeof(stemp), PSTR("\"%s\"" ), GetTimeZone().c_str());
    }
#if defined(USE_TIMERS) && defined(USE_SUNRISE)
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS7_TIME "\":{\"" D_JSON_UTC_TIME "\":\"%s\",\"" D_JSON_LOCAL_TIME "\":\"%s\",\"" D_JSON_STARTDST "\":\"%s\",\"" D_JSON_ENDDST "\":\"%s\",\"" D_CMND_TIMEZONE "\":%s,\"" D_JSON_SUNRISE "\":\"%s\",\"" D_JSON_SUNSET "\":\"%s\"}}"),
      GetTime(0).c_str(), GetTime(1).c_str(), GetTime(2).c_str(), GetTime(3).c_str(), stemp, GetSun(0).c_str(), GetSun(1).c_str());
#else
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS7_TIME "\":{\"" D_JSON_UTC_TIME "\":\"%s\",\"" D_JSON_LOCAL_TIME "\":\"%s\",\"" D_JSON_STARTDST "\":\"%s\",\"" D_JSON_ENDDST "\":\"%s\",\"" D_CMND_TIMEZONE "\":%s}}"),
      GetTime(0).c_str(), GetTime(1).c_str(), GetTime(2).c_str(), GetTime(3).c_str(), stemp);
#endif  // USE_TIMERS and USE_SUNRISE
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "7"));
  }

  if (energy_flg) {
    if ((0 == payload) || (9 == payload)) {
      Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS9_MARGIN "\":{\"" D_CMND_POWERDELTA "\":%d,\"" D_CMND_POWERLOW "\":%d,\"" D_CMND_POWERHIGH "\":%d,\"" D_CMND_VOLTAGELOW "\":%d,\"" D_CMND_VOLTAGEHIGH "\":%d,\"" D_CMND_CURRENTLOW "\":%d,\"" D_CMND_CURRENTHIGH "\":%d}}"),
        Settings.energy_power_delta, Settings.energy_min_power, Settings.energy_max_power, Settings.energy_min_voltage, Settings.energy_max_voltage, Settings.energy_min_current, Settings.energy_max_current);
      MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "9"));
    }
  }

  if ((0 == payload) || (8 == payload) || (10 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS10_SENSOR "\":"));
    MqttShowSensor();
    ResponseJsonEnd();
    if (8 == payload) {
      MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "8"));
    } else {
      MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "10"));
    }
  }

  if ((0 == payload) || (11 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS11_STATUS "\":"));
    MqttShowState();
    ResponseJsonEnd();
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "11"));
  }

}
