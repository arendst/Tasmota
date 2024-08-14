/*
  support_command.ino - command support for Tasmota

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

const char kTasmotaCommands[] PROGMEM = "|"  // No prefix
  // SetOptions synonyms
  D_SO_WIFINOSLEEP "|"
  // Other commands
  D_CMND_UPGRADE "|" D_CMND_UPLOAD "|" D_CMND_OTAURL "|" D_CMND_SERIALLOG "|" D_CMND_RESTART "|"
#ifndef FIRMWARE_MINIMAL_ONLY
  D_CMND_BACKLOG "|" D_CMND_DELAY "|" D_CMND_POWER "|" D_CMND_POWERLOCK "|" D_CMND_TIMEDPOWER "|" D_CMND_STATUS "|" D_CMND_STATE "|" D_CMND_SLEEP "|"
  D_CMND_POWERONSTATE "|" D_CMND_PULSETIME "|" D_CMND_BLINKTIME "|" D_CMND_BLINKCOUNT "|" D_CMND_STATETEXT "|" D_CMND_SAVEDATA "|"
  D_CMND_SO "|" D_CMND_SETOPTION "|" D_CMND_TEMPERATURE_RESOLUTION "|" D_CMND_HUMIDITY_RESOLUTION "|" D_CMND_PRESSURE_RESOLUTION "|" D_CMND_POWER_RESOLUTION "|"
  D_CMND_VOLTAGE_RESOLUTION "|" D_CMND_FREQUENCY_RESOLUTION "|" D_CMND_CURRENT_RESOLUTION "|" D_CMND_ENERGY_RESOLUTION "|" D_CMND_WEIGHT_RESOLUTION "|"
  D_CMND_MODULE "|" D_CMND_MODULES "|" D_CMND_GPIO "|" D_CMND_GPIOREAD "|" D_CMND_GPIOS "|" D_CMND_TEMPLATE "|" D_CMND_PWM "|" D_CMND_PWMFREQUENCY "|" D_CMND_PWMRANGE "|"
  D_CMND_BUTTONDEBOUNCE "|" D_CMND_SWITCHDEBOUNCE "|" D_CMND_SYSLOG "|" D_CMND_LOGHOST "|" D_CMND_LOGPORT "|"
  D_CMND_SERIALBUFFER "|" D_CMND_SERIALSEND "|" D_CMND_BAUDRATE "|" D_CMND_SERIALCONFIG "|" D_CMND_SERIALDELIMITER "|"
  D_CMND_IPADDRESS "|" D_CMND_NTPSERVER "|" D_CMND_AP "|" D_CMND_SSID "|" D_CMND_PASSWORD "|" D_CMND_HOSTNAME "|" D_CMND_WIFICONFIG "|" D_CMND_WIFI "|" D_CMND_DNSTIMEOUT "|"
  D_CMND_DEVICENAME "|" D_CMND_FN "|" D_CMND_FRIENDLYNAME "|" D_CMND_SWITCHMODE "|" D_CMND_INTERLOCK "|" D_CMND_TELEPERIOD "|" D_CMND_RESET "|" D_CMND_TIME "|" D_CMND_TIMEZONE "|" D_CMND_TIMESTD "|"
  D_CMND_TIMEDST "|" D_CMND_ALTITUDE "|" D_CMND_LEDPOWER "|" D_CMND_LEDSTATE "|" D_CMND_LEDMASK "|" D_CMND_LEDPWM_ON "|" D_CMND_LEDPWM_OFF "|" D_CMND_LEDPWM_MODE "|"
  D_CMND_WIFIPOWER "|" D_CMND_TEMPOFFSET "|" D_CMND_HUMOFFSET "|" D_CMND_SPEEDUNIT "|" D_CMND_GLOBAL_TEMP "|" D_CMND_GLOBAL_HUM"|" D_CMND_GLOBAL_PRESS "|" D_CMND_SWITCHTEXT "|" D_CMND_WIFISCAN "|" D_CMND_WIFITEST "|"
  D_CMND_ZIGBEE_BATTPERCENT "|"
#ifdef USE_I2C
  D_CMND_I2CSCAN "|" D_CMND_I2CDRIVER "|"
#endif
#ifdef USE_DEVICE_GROUPS
  D_CMND_DEVGROUP_NAME "|"
#ifdef USE_DEVICE_GROUPS_SEND
  D_CMND_DEVGROUP_SEND "|"
#endif  // USE_DEVICE_GROUPS_SEND
  D_CMND_DEVGROUP_SHARE "|" D_CMND_DEVGROUPSTATUS "|" D_CMND_DEVGROUP_TIE "|"
#endif  // USE_DEVICE_GROUPS
  D_CMND_SETSENSOR "|" D_CMND_SENSOR "|" D_CMND_DRIVER "|" D_CMND_JSON
#ifdef ESP32
   "|Info|"
#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
  D_CMND_TOUCH_CAL "|" D_CMND_TOUCH_THRES "|"
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2
  D_CMND_CPU_FREQUENCY
#endif  // ESP32
#endif   //FIRMWARE_MINIMAL_ONLY
  ;

SO_SYNONYMS(kTasmotaSynonyms,
  127,
);

void (* const TasmotaCommand[])(void) PROGMEM = {
  &CmndUpgrade, &CmndUpgrade, &CmndOtaUrl, &CmndSeriallog, &CmndRestart,
#ifndef FIRMWARE_MINIMAL_ONLY
  &CmndBacklog, &CmndDelay, &CmndPower, &CmndPowerLock, &CmndTimedPower, &CmndStatus, &CmndState, &CmndSleep,
  &CmndPowerOnState, &CmndPulsetime, &CmndBlinktime, &CmndBlinkcount, &CmndStateText, &CmndSavedata,
  &CmndSetoption, &CmndSetoption, &CmndTemperatureResolution, &CmndHumidityResolution, &CmndPressureResolution, &CmndPowerResolution,
  &CmndVoltageResolution, &CmndFrequencyResolution, &CmndCurrentResolution, &CmndEnergyResolution, &CmndWeightResolution,
  &CmndModule, &CmndModules, &CmndGpio, &CmndGpioRead, &CmndGpios, &CmndTemplate, &CmndPwm, &CmndPwmfrequency, &CmndPwmrange,
  &CmndButtonDebounce, &CmndSwitchDebounce, &CmndSyslog, &CmndLoghost, &CmndLogport,
  &CmndSerialBuffer, &CmndSerialSend, &CmndBaudrate, &CmndSerialConfig, &CmndSerialDelimiter,
  &CmndIpAddress, &CmndNtpServer, &CmndAp, &CmndSsid, &CmndPassword, &CmndHostname, &CmndWifiConfig, &CmndWifi, &CmndDnsTimeout,
  &CmndDevicename, &CmndFriendlyname, &CmndFriendlyname, &CmndSwitchMode, &CmndInterlock, &CmndTeleperiod, &CmndReset, &CmndTime, &CmndTimezone, &CmndTimeStd,
  &CmndTimeDst, &CmndAltitude, &CmndLedPower, &CmndLedState, &CmndLedMask, &CmndLedPwmOn, &CmndLedPwmOff, &CmndLedPwmMode,
  &CmndWifiPower, &CmndTempOffset, &CmndHumOffset, &CmndSpeedUnit, &CmndGlobalTemp, &CmndGlobalHum, &CmndGlobalPress, &CmndSwitchText, &CmndWifiScan, &CmndWifiTest,
  &CmndBatteryPercent,
#ifdef USE_I2C
  &CmndI2cScan, &CmndI2cDriver,
#endif
#ifdef USE_DEVICE_GROUPS
  &CmndDevGroupName,
#ifdef USE_DEVICE_GROUPS_SEND
  &CmndDevGroupSend,
#endif  // USE_DEVICE_GROUPS_SEND
  &CmndDevGroupShare, &CmndDevGroupStatus, &CmndDevGroupTie,
#endif  // USE_DEVICE_GROUPS
  &CmndSetSensor, &CmndSensor, &CmndDriver, &CmndJson
#ifdef ESP32
  , &CmndInfo,
#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
  &CmndTouchCal, &CmndTouchThres,
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2
  &CmndCpuFrequency
#endif  // ESP32
#endif   //FIRMWARE_MINIMAL_ONLY
  };

const char kWifiConfig[] PROGMEM =
  D_WCFG_0_RESTART "||" D_WCFG_2_WIFIMANAGER "||" D_WCFG_4_RETRY "|" D_WCFG_5_WAIT "|" D_WCFG_6_SERIAL "|" D_WCFG_7_WIFIMANAGER_RESET_ONLY;

/********************************************************************************************/

#ifndef FIRMWARE_MINIMAL_ONLY
void CmndWifiScan(void)
{
  if (XdrvMailbox.data_len > 0) {
    if ( !Wifi.scan_state || Wifi.scan_state > 7 ) {
      ResponseCmndChar(D_JSON_SCANNING);
      Wifi.scan_state = 6;
    } else {
      ResponseCmndChar(D_JSON_BUSY);
    }
  } else {
    if ( !Wifi.scan_state ) {
      ResponseCmndChar(D_JSON_NOT_STARTED);
    } else if ( Wifi.scan_state >= 1 && Wifi.scan_state <= 5 ) {
      ResponseCmndChar(D_JSON_BUSY);
    } else if ( Wifi.scan_state >= 6 && Wifi.scan_state <= 7 ) {
      ResponseCmndChar(D_JSON_SCANNING);
    } else {  //show scan result
      Response_P(PSTR("{\"" D_CMND_WIFISCAN "\":"));

      if (WiFi.scanComplete() > 0) {
        // Sort networks by RSSI
        uint32_t indexes[WiFi.scanComplete()];
        for (uint32_t i = 0; i < WiFi.scanComplete(); i++) {
          indexes[i] = i;
        }
        for (uint32_t i = 0; i < WiFi.scanComplete(); i++) {
          for (uint32_t j = i + 1; j < WiFi.scanComplete(); j++) {
            if (WiFi.RSSI(indexes[j]) > WiFi.RSSI(indexes[i])) {
              std::swap(indexes[i], indexes[j]);
            }
          }
        }
        delay(0);

        ResponseAppend_P(PSTR("{"));
        for (uint32_t i = 0; i < WiFi.scanComplete(); i++) {
          ResponseAppend_P(PSTR("\"" D_STATUS5_NETWORK "%d\":{\"" D_SSID "\":\"%s\",\"" D_BSSID "\":\"%s\",\"" D_CHANNEL
                          "\":\"%d\",\"" D_JSON_SIGNAL "\":\"%d\",\"" D_RSSI "\":\"%d\",\"" D_JSON_ENCRYPTION "\":\"%s\"}"),
                          i+1,
                          WiFi.SSID(indexes[i]).c_str(),
                          WiFi.BSSIDstr(indexes[i]).c_str(),
                          WiFi.channel(indexes[i]),
                          WiFi.RSSI(indexes[i]),
                          WifiGetRssiAsQuality(WiFi.RSSI(indexes[i])),
                          WifiEncryptionType(indexes[i]).c_str());
          if ( ResponseSize() < ResponseLength() + 300 ) { break; }
          if ( i < WiFi.scanComplete() -1 ) { ResponseAppend_P(PSTR(",")); }
          //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "MAX SIZE: %d, SIZE: %d"),ResponseSize(),ResponseLength());
        }
        ResponseJsonEnd();
      } else {
        ResponseAppend_P(PSTR("\"" D_NO_NETWORKS_FOUND "\""));
      }
      ResponseJsonEnd();
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_CMND_WIFISCAN));
    }
  }
}

void CmndWifiTest(void)
{
  // Test WIFI Connection to Router if Tasmota is in AP mode since in AP mode, a STA connection can be established
  // at the same time for testing the connection.

#ifdef USE_WEBSERVER
  if (!WifiIsInManagerMode()) { ResponseCmndError(); return; }

  if ( (XdrvMailbox.data_len > 0) ) {

    if (Wifi.wifiTest != WIFI_TESTING) { // Start Test
      char* pos = strchr(XdrvMailbox.data, '+');
      if (pos != nullptr) {
        char ssid_test[XdrvMailbox.data_len];
        char pswd_test[XdrvMailbox.data_len];
        subStr(ssid_test, XdrvMailbox.data, "+", 1);
        subStr(pswd_test, XdrvMailbox.data, "+", 2);
        ResponseCmndIdxChar(D_JSON_TESTING);
        //Response_P(PSTR("{\"%s%d\":{\"Network\":\"%s,\"PASS\":\"%s\"}}"), XdrvMailbox.command, XdrvMailbox.index, ssid_test, pswd_test);

        if (WIFI_NOT_TESTING == Wifi.wifiTest) {
          if (MAX_WIFI_OPTION == Wifi.old_wificonfig) { Wifi.old_wificonfig = Settings->sta_config; }
          TasmotaGlobal.wifi_state_flag = Settings->sta_config = WIFI_MANAGER;
          Wifi.save_data_counter = TasmotaGlobal.save_data_counter;
        }

        Wifi.wifi_test_counter = 9;   // seconds to test user's proposed AP
        Wifi.wifiTest = WIFI_TESTING;
        TasmotaGlobal.save_data_counter = 0;               // Stop auto saving data - Updating Settings
        Settings->save_data = 0;
        TasmotaGlobal.sleep = 0;                           // Disable sleep
        TasmotaGlobal.restart_flag = 0;                    // No restart
        TasmotaGlobal.ota_state_flag = 0;                  // No OTA

        Wifi.wifi_Test_Restart = false;
        Wifi.wifi_Test_Save_SSID2 = false;
        if (0 == XdrvMailbox.index) { Wifi.wifi_Test_Restart = true; }      // If WifiTest is successful, save data on SSID1 and restart
        if (2 == XdrvMailbox.index) { Wifi.wifi_Test_Save_SSID2 = true; }   // If WifiTest is successful, save data on SSID2

        if (3 != XdrvMailbox.index) {                                       // WifiTest3 never ever makes anything persistent, thus works without webserver
          SettingsUpdateText(Wifi.wifi_Test_Save_SSID2 ? SET_STASSID2 : SET_STASSID1, ssid_test);
          SettingsUpdateText(Wifi.wifi_Test_Save_SSID2 ? SET_STAPWD2 : SET_STAPWD1, pswd_test);
        }

        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECTING_TO_AP " %s " D_AS " %s ..."),
          ssid_test, TasmotaGlobal.hostname);

        WiFiHelper::begin(ssid_test,pswd_test);
      }
    } else {
      ResponseCmndChar(D_JSON_BUSY);
    }

  } else {
    switch (Wifi.wifiTest) {
      case WIFI_TESTING:
        ResponseCmndChar(D_JSON_TESTING);
        break;
      case WIFI_NOT_TESTING:
        ResponseCmndChar(D_JSON_NOT_STARTED);
        break;
      case WIFI_TEST_FINISHED:
        ResponseCmndChar(Wifi.wifi_test_AP_TIMEOUT ? D_CONNECT_FAILED_AP_TIMEOUT : D_JSON_SUCCESSFUL);
        break;
      case WIFI_TEST_FINISHED_BAD:

          switch (WiFi.status()) {
            case WL_CONNECTED:
              ResponseCmndChar(D_CONNECT_FAILED_NO_IP_ADDRESS);
              break;
            case WL_NO_SSID_AVAIL:
              ResponseCmndChar(D_CONNECT_FAILED_AP_NOT_REACHED);
              break;
            case WL_CONNECT_FAILED:
              ResponseCmndChar(D_CONNECT_FAILED_WRONG_PASSWORD);
              break;
            default:  // WL_IDLE_STATUS and WL_DISCONNECTED - SSId in range but no answer from the router
              ResponseCmndChar(D_CONNECT_FAILED_AP_TIMEOUT);
          }

        break;
    }
  }
#else
  ResponseCmndError();
#endif //USE_WEBSERVER
}

#endif  // not defined FIRMWARE_MINIMAL_ONLY

void ResponseCmnd(void) {
  Response_P(PSTR("{\"%s\":"), XdrvMailbox.command);
}

void ResponseCmndNumber(int value) {
  Response_P(S_JSON_COMMAND_NVALUE, XdrvMailbox.command, value);
}

void ResponseCmndFloat(float value, uint32_t decimals) {
  Response_P(PSTR("{\"%s\":%*_f}"), XdrvMailbox.command, decimals, &value);  // Return float value without quotes
}

void ResponseCmndIdxFloat(float value, uint32_t decimals) {
  Response_P(PSTR("{\"%s%d\":%*_f}"), XdrvMailbox.command, XdrvMailbox.index, decimals, &value);  // Return float value without quotes
}

void ResponseCmndIdxNumber(int value) {
  Response_P(S_JSON_COMMAND_INDEX_NVALUE, XdrvMailbox.command, XdrvMailbox.index, value);
}

void ResponseCmndChar_P(const char* value) {
  Response_P(S_JSON_COMMAND_SVALUE, XdrvMailbox.command, value);
}

void ResponseCmndChar(const char* value) {
  Response_P(S_JSON_COMMAND_SVALUE, XdrvMailbox.command, EscapeJSONString(value).c_str());
}

void ResponseCmndStateText(uint32_t value) {
  ResponseCmndChar(GetStateText(value));
}

void ResponseCmndDone(void) {
  ResponseCmndChar_P(PSTR(D_JSON_DONE));
}

void ResponseCmndError(void) {
  ResponseCmndChar_P(PSTR(D_JSON_ERROR));
}

void ResponseCmndFailed(void) {
  ResponseCmndChar_P(PSTR(D_JSON_FAILED));
}

void ResponseCmndIdxChar(const char* value) {
  Response_P(S_JSON_COMMAND_INDEX_SVALUE, XdrvMailbox.command, XdrvMailbox.index, EscapeJSONString(value).c_str());
}

void ResponseCmndIdxError(void) {
  ResponseCmndIdxChar(PSTR(D_JSON_ERROR));
}

void ResponseCmndAll(uint32_t text_index, uint32_t count) {
  uint32_t real_index = text_index;
  ResponseClear();
  for (uint32_t i = 0; i < count; i++) {
    if ((SET_MQTT_GRP_TOPIC == text_index) && (1 == i)) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
    if ((SET_BUTTON1 == text_index) && (16 == i)) { real_index = SET_BUTTON17 -16; }
    ResponseAppend_P(PSTR("%c\"%s%d\":\"%s\""), (i)?',':'{', XdrvMailbox.command, i +1, EscapeJSONString(SettingsText(real_index +i)).c_str());
  }
  ResponseJsonEnd();
}

/********************************************************************************************/

void ExecuteCommand(const char *cmnd, uint32_t source)
{
  // cmnd: "status 0"  = stopic "status" and svalue " 0"
  // cmnd: "var1 =1"   = stopic "var1" and svalue " =1"
  // cmnd: "var1=1"    = stopic "var1" and svalue "=1"
  SHOW_FREE_MEM(PSTR("ExecuteCommand"));
  ShowSource(source);

  const char *pos = cmnd;
  while (*pos && isspace(*pos)) {
    pos++;                         // Skip all spaces
  }

  const char *start = pos;
  // Get a command. Commands can only use letters, digits and underscores
  while (*pos && (isalpha(*pos) || isdigit(*pos) || '_' == *pos || '/' == *pos)) {
    if ('/' == *pos) {            // Skip possible cmnd/tasmota/ preamble
      start = pos + 1;
    }
    pos++;
  }
  if ('\0' == *start || pos <= start) {
    return;                      // Did not find any command to execute
  }

  uint32_t size = pos - start;
  char stopic[size + 2];         // with leader '/' and end '\0'
  stopic[0] = '/';
  memcpy(stopic+1, start, size);
  stopic[size+1] = '\0';

  char svalue[strlen(pos) +1];   // pos point to the start of parameters
  strlcpy(svalue, pos, sizeof(svalue));
  CommandHandler(stopic, svalue, strlen(svalue));
}

/********************************************************************************************/

// topicBuf:                    /power1  dataBuf: toggle  = Console command
// topicBuf:        cmnd/tasmota/power1  dataBuf: toggle  = Mqtt command using topic
// topicBuf:       cmnd/tasmotas/power1  dataBuf: toggle  = Mqtt command using a group topic
// topicBuf: cmnd/DVES_83BB10_fb/power1  dataBuf: toggle  = Mqtt command using fallback topic

void CommandHandler(char* topicBuf, char* dataBuf, uint32_t data_len) {
  SHOW_FREE_MEM(PSTR("CommandHandler"));

  bool grpflg = false;
  uint32_t real_index = SET_MQTT_GRP_TOPIC;
  for (uint32_t i = 0; i < MAX_GROUP_TOPICS; i++) {
    if (1 == i) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
    char *group_topic = SettingsText(real_index +i);
    if (*group_topic && strstr(topicBuf, group_topic) != nullptr) {
      grpflg = true;
      break;
    }
  }

  char stemp1[TOPSZ];
  GetFallbackTopic_P(stemp1, "");        // Full Fallback topic = cmnd/DVES_xxxxxxxx_fb/
  TasmotaGlobal.fallback_topic_flag = (!strncmp(topicBuf, stemp1, strlen(stemp1)));

  char *type = strrchr(topicBuf, '/');   // Last part of received topic is always the command (type)

  uint32_t index = 1;
  bool user_index = false;
  if (type != nullptr) {
    type++;
    uint32_t i;
    int nLen;                            // strlen(type)
    char *s = type;
    for (nLen = 0; *s; s++, nLen++) {
      *s=toupper(*s);
    }
    i = nLen;
    if (i > 0) {                         // may be 0
      while (isdigit(type[i-1])) {
        i--;
      }
    }
    if (i < nLen) {
      index = atoi(type + i);
      user_index = true;
    }
    type[i] = '\0';
    if ((i > 1) && ('_' == type[0])) {
      type++;                            // Skip leading _ in command
      TasmotaGlobal.no_mqtt_response = true;
    }

    bool binary_data = (index > 299);    // Suppose binary data on topic index > 299
    if (!binary_data) {
      bool keep_spaces = ((strstr_P(type, PSTR("SERIALSEND")) != nullptr) && (index > 9));  // Do not skip leading spaces on (s)serialsend10 and up
      if (!keep_spaces) {
        while (*dataBuf && isspace(*dataBuf)) {
          dataBuf++;                     // Skip leading spaces in data
          data_len--;
        }
      }
    }

    int32_t payload = -99;
    if (!binary_data) {
      if (!strcmp(dataBuf,"?")) { data_len = 0; }

      char *p;
      payload = strtol(dataBuf, &p, 0);  // decimal, octal (0) or hex (0x)
      if (p == dataBuf) { payload = -99; }
      int temp_payload = GetStateNumber(dataBuf);
      if (temp_payload > -1) { payload = temp_payload; }
    }

    AddLog(LOG_LEVEL_DEBUG, PSTR("CMD: Grp %d, Cmd '%s', Idx %d, Len %d, Pld %d, Data '%s'"),
      grpflg, type, index, data_len, payload, (binary_data) ? HexToString((uint8_t*)dataBuf, data_len).c_str() : dataBuf);

    Response_P(PSTR("{\"" D_JSON_COMMAND "\":\"" D_JSON_ERROR "\"}"));

    if (Settings->ledstate &0x02) { TasmotaGlobal.blinks++; }

//    TasmotaGlobal.backlog_timer = millis() + (100 * MIN_BACKLOG_DELAY);
    TasmotaGlobal.backlog_timer = millis() + Settings->param[P_BACKLOG_DELAY];  // SetOption34

    char command[CMDSZ] = { 0 };
    XdrvMailbox.command = command;
    XdrvMailbox.index = index;
    XdrvMailbox.data_len = data_len;
    XdrvMailbox.payload = payload;
    XdrvMailbox.grpflg = grpflg;
    XdrvMailbox.usridx = user_index;
    XdrvMailbox.topic = type;
    XdrvMailbox.data = dataBuf;

#ifdef USE_SCRIPT_SUB_COMMAND
  // allow overwrite tasmota cmds
    if (!Script_SubCmd()) {
      if (!DecodeCommand(kTasmotaCommands, TasmotaCommand, kTasmotaSynonyms)) {
        if (!XdrvCall(FUNC_COMMAND)) {
          if (!XsnsCall(FUNC_COMMAND)) {
            type = nullptr;  // Unknown command
          }
        }
      }
    }
#else  // USE_SCRIPT_SUB_COMMAND
    if (!DecodeCommand(kTasmotaCommands, TasmotaCommand, kTasmotaSynonyms)) {
      if (!XdrvCall(FUNC_COMMAND)) {
        if (!XsnsCall(FUNC_COMMAND)) {
          type = nullptr;  // Unknown command
        }
      }
    }
#endif  // USE_SCRIPT_SUB_COMMAND

  }

  if (type == nullptr) {
    TasmotaGlobal.blinks = 201;
    snprintf_P(stemp1, sizeof(stemp1), PSTR(D_JSON_COMMAND));
    Response_P(PSTR("{\"" D_JSON_COMMAND "\":\"" D_JSON_UNKNOWN "\"}"));
    type = (char*)stemp1;
  }

  if (ResponseLength()) {
    if (TasmotaGlobal.no_mqtt_response){  // If it is activated, Tasmota will not publish MQTT messages, but it will proccess event trigger rules
      XdrvRulesProcess(0);
    } else {
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, type);
    }
  }
  TasmotaGlobal.fallback_topic_flag = false;
  TasmotaGlobal.no_mqtt_response = false;
}

void CmndBacklog(void) {
  // Backlog command1;command2;..   Execute commands in sequence with a delay in between set with SetOption34
  // Backlog0 command1;command2;..  Execute commands in sequence with no delay
  // Backlog2 command1;command2;..  Execute commands in sequence with no delay and no response but rule processing only
  // Backlog3 command1;command2;..  Execute commands in sequence with a delay but no response but rule processing only

  if (XdrvMailbox.data_len) {
    TasmotaGlobal.backlog_nodelay = (0 == (XdrvMailbox.index & 0x01));           // Backlog0, Backlog2
    TasmotaGlobal.backlog_no_mqtt_response = (2 == (XdrvMailbox.index & 0x02));  // Backlog2, Backlog3

    char *blcommand = strtok(XdrvMailbox.data, ";");
    while (blcommand != nullptr) {
      // Ignore semicolon (; = end of single command) between brackets {}
      char *next = strchr(blcommand, '\0') +1;  // Prepare for next ;
      while ((next != nullptr) && (ChrCount(blcommand, "{") != ChrCount(blcommand, "}"))) {  // Check for valid {} pair
        next--;                                 // Select end of line
        *next = ';';                            // Restore ; removed by strtok()
        next = strtok(nullptr, ";");            // Point to begin of next string up to next ; or nullptr
      }
      // Skip unnecessary command Backlog at start of blcommand
      while(true) {
        blcommand = Trim(blcommand);
        if (0 == strncasecmp_P(blcommand, PSTR(D_CMND_BACKLOG), strlen(D_CMND_BACKLOG))) {
          blcommand += strlen(D_CMND_BACKLOG);
        } else {
          break;
        }
      }
      // Do not allow command Reset in backlog
      if ((*blcommand != '\0') && (strncasecmp_P(blcommand, PSTR(D_CMND_RESET), strlen(D_CMND_RESET)) != 0))  {
        char* temp = (char*)malloc(strlen(blcommand)+1);
        if (temp != nullptr) {
          strcpy(temp, blcommand);
          char* &elem = backlog.addToLast();
          elem = temp;
        }
      }
      blcommand = strtok(nullptr, ";");
    }
//    ResponseCmndChar(D_JSON_APPENDED);
    ResponseClear();
    TasmotaGlobal.backlog_timer = millis();
  } else {
    bool blflag = BACKLOG_EMPTY;
    for (auto &elem : backlog) {
      free(elem);
      backlog.remove(&elem);
    }
    ResponseCmndChar(blflag ? PSTR(D_JSON_EMPTY) : PSTR(D_JSON_ABORTED));
  }
}

void CmndJson(void) {
  // Json {"template":{"NAME":"Dummy","GPIO":[320,0,321],"FLAG":0,"BASE":18},"power":2,"HSBColor":"51,97,100","Channel":[100,85,3]}
  //
  // Escape lower level tokens and add quotes around it
  // Input:
  // {"template":{"NAME":"Dummy","GPIO":[320,0,321],"FLAG":0,"BASE":18},"power":2,"HSBColor":"51,97,100","Channel":[100,85,3]}
  // Output (escaped subtokens):
  // {"template":"{\"NAME\":\"Dummy\",\"GPIO\":[320,0,321],\"FLAG\":0,\"BASE\":18}","power":2,"HSBColor":"51,97,100","Channel":[100,85,3]}
  uint32_t bracket = 0;
  String data_buf("");
  data_buf.reserve(XdrvMailbox.data_len);       // We need at least the same amount of characters
  for (uint32_t index = 0; index < XdrvMailbox.data_len; index++) {
    char c = (char)XdrvMailbox.data[index];
    if (c == '{') {
      bracket++;
      if (2 == bracket) { data_buf += '"'; }    // Add start quote
    }
    if (bracket > 1) {
      if (c == '\"') { data_buf += '\\'; }      // Escape any quote within second level token
    }
    data_buf += c;
    if (c == '}') {
      bracket--;
      if (1 == bracket) { data_buf += '"'; }    // Add end quote
    }
  }

  JsonParser parser((char*)data_buf.c_str());
  JsonParserObject root = parser.getRootObject();
  if (root) {
    // Convert to backlog commands
    // Input (escaped subtokens):
    // {"template":"{\"NAME\":\"Dummy\",\"GPIO\":[320,0,321],\"FLAG\":0,\"BASE\":18}","power":2,"HSBColor":"51,97,100","Channel":[100,85,3]}
    // Output:
    // template {"NAME":"Dummy","GPIO":[320,0,321],"FLAG":0,"BASE":18};power 2;HSBColor 51,97,100;Channel1 100;Channel2 85;Channel3 3
    String backlog;                             // We might need a larger string than XdrvMailbox.data_len accomodating decoded arrays
    for (auto command_key : root) {
      const char *command = command_key.getStr();
      JsonParserToken parameters = command_key.getValue();
      if (parameters.isArray()) {
        JsonParserArray parameter_arr = parameters.getArray();
        uint32_t index = 1;
        for (auto value : parameter_arr) {
          if (backlog.length()) { backlog += ";"; }
          backlog += command;
          backlog += index++;
          backlog += " ";
          backlog += value.getStr();            // Channel1 100;Channel2 85;Channel3 3
        }
      } else if (parameters.isObject()) {       // Should have been escaped
//        AddLog(LOG_LEVEL_DEBUG, PSTR("JSN: Object"));
      } else {
        String cmnd_param = command;
        cmnd_param += " ";
        cmnd_param += parameters.getStr();
        if (cmnd_param.indexOf(";") == -1) {    // Rule1 ON Clock#Timer=1 DO Backlog Color #FF000000D0; Wakeup 100 ENDON
          if (backlog.length()) { backlog += ";"; }
          backlog += cmnd_param;                // HSBColor 51,97,100
        } else {
          ExecuteCommand((char*)cmnd_param.c_str(), SRC_FILE);
        }
      }
    }
    if (backlog.length()) {
      XdrvMailbox.data = (char*)backlog.c_str();  // Backlog commands
      XdrvMailbox.data_len = 1;                 // Any data
      XdrvMailbox.index = 0;                    // Backlog0 - no delay
      CmndBacklog();
    }
  } else {
    ResponseCmndChar(PSTR(D_JSON_EMPTY));
  }
}

void CmndDelay(void) {
  // Delay -1  - Wait until next second
  // Delay 1   - Wait default time (200ms)
  // Delay 2   - Wait 2 x 100ms
  // Delay 10  - Wait 10 x 100ms
  if (XdrvMailbox.payload == -1) {
    TasmotaGlobal.backlog_timer = millis() + (1000 - RtcMillis());  // Next second (#18984)
  }
  else if ((XdrvMailbox.payload >= (MIN_BACKLOG_DELAY / 100)) && (XdrvMailbox.payload <= 3600)) {
    TasmotaGlobal.backlog_timer = millis() + (100 * XdrvMailbox.payload);
  }
  uint32_t bl_delay = 0;
  long bl_delta = TimePassedSince(TasmotaGlobal.backlog_timer);
  if (bl_delta < 0) { bl_delay = (bl_delta *-1) / 100; }
  ResponseCmndNumber(bl_delay);
}

void CmndPower(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.devices_present)) {
    if ((XdrvMailbox.payload < POWER_OFF) || (XdrvMailbox.payload > POWER_BLINK_STOP)) {
      XdrvMailbox.payload = POWER_SHOW_STATE;
    }
//      Settings->flag.device_index_enable = XdrvMailbox.usridx;  // SetOption26 - Switch between POWER or POWER1
    ExecuteCommandPower(XdrvMailbox.index, XdrvMailbox.payload, SRC_IGNORE);
    ResponseClear();
  }
  else if (0 == XdrvMailbox.index) {
    if ((XdrvMailbox.payload < POWER_OFF) || (XdrvMailbox.payload > POWER_TOGGLE)) {
      XdrvMailbox.payload = POWER_SHOW_STATE;
    }
    SetAllPower(XdrvMailbox.payload, SRC_IGNORE);
    if (Settings->flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
      MqttPublishTeleState();
    }
    ResponseClear();
  }
}

void CmndPowerLock(void) {
  // PowerLock    - Show current state
  // PowerLock0 0 - Reset all power locks
  // PowerLock0 1 - Set all power locks
  // PowerLock1 1 - Set Power1 lock
  if (XdrvMailbox.index <= TasmotaGlobal.devices_present) {
    if (XdrvMailbox.payload >= 0) {
      XdrvMailbox.payload &= 1;
      if (0 == XdrvMailbox.index) {  // Control all bits
        Settings->power_lock = (XdrvMailbox.payload) ? (1 << TasmotaGlobal.devices_present) -1 : 0;
      } else {                       // Control individual bits
        bitWrite(Settings->power_lock, XdrvMailbox.index -1, XdrvMailbox.payload & 1);
      }
    }
    char stemp1[33];
    ext_snprintf_P(stemp1, sizeof(stemp1), PSTR("%*_b"), TasmotaGlobal.devices_present, Settings->power_lock);
    ResponseCmndChar(stemp1);
  }
}

/********************************************************************************************/

typedef struct {
  uint32_t time;
  char*    command;
} tTimedCmnd;
LList<tTimedCmnd> timed_cmnd;          // Timed command buffer

bool SetTimedCmnd(uint32_t time, const char *command) {
  // Remove command if present
  for (auto &elem : timed_cmnd) {
    if (strcmp(command, elem.command) == 0) {  // Equal
      free(elem.command);
      timed_cmnd.remove(&elem);
      break;
    }
  }
  // Add command
  char* cmnd = (char*)malloc(strlen(command) +1);
  if (cmnd) {
    strcpy(cmnd, command);
    tTimedCmnd &elem = timed_cmnd.addToLast();
    elem.time = millis() + time;
    elem.command = cmnd;
    return true;
  }
  return false;
}

void ResetTimedCmnd(const char *command) {
  for (auto &elem : timed_cmnd) {
    if (strncmp(command, elem.command, strlen(command)) == 0) {  // StartsWith
      free(elem.command);
      timed_cmnd.remove(&elem);
    }
  }
}

void ShowTimedCmnd(const char *command) {
  bool found = false;
  uint32_t now = millis();
  ResponseCmnd();     // {"TimedPower":
  for (auto &elem : timed_cmnd) {
    if (strncmp(command, elem.command, strlen(command)) == 0) {  // StartsWith
      ResponseAppend_P(PSTR("%s{\"" D_JSON_REMAINING "\":%d,\"" D_JSON_COMMAND "\":\"%s\"}"),
        (found) ? "," : "[", elem.time - now, elem.command);
      found = true;
    }
  }
  ResponseAppend_P((found) ? PSTR("]}") : PSTR("\"" D_JSON_EMPTY "\"}"));
}

void LoopTimedCmnd(void) {
  for (auto &elem : timed_cmnd) {
    if (TimeReached(elem.time)) {
      char* command = elem.command;
      timed_cmnd.remove(&elem);
      ExecuteCommand(command, SRC_TIMER);
      free(command);
    }
  }
}

/*------------------------------------------------------------------------------------------*/

void CmndTimedPower(void) {
  /*
  Allow timed power changes on a 50ms granularity
  TimedPower<index> <milliseconds>[,0|1|2|3]
  TimedPower                 - Show remaining timers
  TimedPower 2000            - Turn power1 on and then off after 2 seconds
  TimedPower1                - Clear active Power1 timers
  TimedPower1 0              - Stop timer and perform timed action
  TimedPower0 3000           - Turn all power on and then off after 3 seconds
  TimedPower1 2000           - Turn power1 on and then off after 2 seconds
  TimedPower2 2000,0|off     - Turn power2 off and then on after 2 seconds
  TimedPower1 2200,1|on      - Turn power1 on and then off after 2.2 seconds
  TimedPower2 2000,2|toggle  - Toggle power2 and then toggle again after 2 seconds
  TimedPower2 2500,3|blink   - Blink power2 and then stop blink after 2.5 seconds
  */
  if ((XdrvMailbox.index >= 0) && (XdrvMailbox.index <= TasmotaGlobal.devices_present)) {
    if (XdrvMailbox.data_len > 0) {
      uint32_t time = (XdrvMailbox.payload < 50) ? 50 : XdrvMailbox.payload;
      int start_state = POWER_ON;               // Default on
      if (ArgC() > 1) {
        char state_text[XdrvMailbox.data_len];
        ArgV(state_text, 2);
        start_state = GetStateNumber(Trim(state_text));
        if (start_state < 0) {
          start_state = atoi(state_text);
        }
        start_state &= 0x03;                    // POWER_OFF, POWER_ON, POWER_TOGGLE, POWER_BLINK
      }
      const uint8_t end_state[] = { POWER_ON, POWER_OFF, POWER_TOGGLE, POWER_BLINK_STOP };
      char cmnd[CMDSZ];
      snprintf_P(cmnd, sizeof(cmnd), PSTR(D_CMND_POWER "%d %d"), XdrvMailbox.index, end_state[start_state]);
      if (SetTimedCmnd(time, cmnd)) {           // Skip if no more room for timers
        XdrvMailbox.payload = start_state;
        CmndPower();
      }
    } else {
      if (!XdrvMailbox.usridx) {
//        ResetTimedCmnd(D_CMND_POWER);           // Remove all POWER timed command
        ShowTimedCmnd(D_CMND_POWER);            // Show remaining timers
        return;
      } else {
        char cmnd[CMDSZ];
        snprintf_P(cmnd, sizeof(cmnd), PSTR(D_CMND_POWER "%d"), XdrvMailbox.index);
        ResetTimedCmnd(cmnd);                   // Remove POWER<index> timed command 
      }
      ResponseCmndDone();
    }
  }
}

/********************************************************************************************/

void CmndStatusResponse(uint32_t index) {
  static String all_status = (const char*) nullptr;

  if (0 == XdrvMailbox.index) {  // Command status0
    if (99 == index) {
      all_status.replace("}{", ",");
      char cmnd_status[10];  // STATUS11
      snprintf_P(cmnd_status, sizeof(cmnd_status), PSTR(D_CMND_STATUS "0"));
      MqttPublishPayloadPrefixTopicRulesProcess_P(STAT, cmnd_status, all_status.c_str(), Settings->flag5.mqtt_status_retain);
      all_status = (const char*) nullptr;
    } else {
      if (0 == index) { all_status = ""; }
      all_status += ResponseData();
    }
  }
  else if (index < 99) {
    char cmnd_status[10];  // STATUS11
    char number[4] = { 0 };
    snprintf_P(cmnd_status, sizeof(cmnd_status), PSTR(D_CMND_STATUS "%s"), (index) ? itoa(index, number, 10) : "");
    MqttPublishPrefixTopicRulesProcess_P(STAT, cmnd_status, Settings->flag5.mqtt_status_retain);
  }
}

void CmndStatus(void)
{
  int32_t payload = XdrvMailbox.payload;

  if (0 == XdrvMailbox.index) { payload = 0; }  // All status messages in one MQTT message (status0)

  if (payload > MAX_STATUS) { return; }  // {"Command":"Error"}
  if (!Settings->flag.mqtt_enabled && (6 == payload)) { return; }  // SetOption3 - Enable MQTT
  if (!TasmotaGlobal.energy_driver && (9 == payload)) { return; }
  #ifndef FIRMWARE_MINIMAL
  if (!CrashFlag() && (12 == payload)) { return; }
  #endif // FIRMWARE_MINIMAL
  if (!Settings->flag3.shutter_mode && (13 == payload)) { return; }

  char stemp[200];
  char stemp2[TOPSZ];

  if ((0 == payload) || (-99 == payload)) {
    uint32_t maxfn = (TasmotaGlobal.devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : (!TasmotaGlobal.devices_present) ? 1 : TasmotaGlobal.devices_present;
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan()) { maxfn = 1; }
#endif  // USE_SONOFF_IFAN
    stemp[0] = '\0';
    for (uint32_t i = 0; i < maxfn; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%s%s\"%s\"" ), stemp, (i > 0 ? "," : ""), EscapeJSONString(SettingsText(SET_FRIENDLYNAME1 +i)).c_str());
    }
    stemp2[0] = '\0';
    for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
      snprintf_P(stemp2, sizeof(stemp2), PSTR("%s%s%d" ), stemp2, (i > 0 ? "," : ""), Settings->switchmode[i]);
    }
    Response_P(PSTR("{\"" D_CMND_STATUS "\":{\"" D_CMND_MODULE "\":%d,\"" D_CMND_DEVICENAME "\":\"%s\",\"" D_CMND_FRIENDLYNAME "\":[%s],\""
                          D_CMND_TOPIC "\":\"%s\",\"" D_CMND_BUTTONTOPIC "\":\"%s\",\""
                          D_CMND_POWER "\":\"%*_b\",\"" D_CMND_POWERLOCK "\":\"%*_b\",\""
                          D_CMND_POWERONSTATE "\":%d,\"" D_CMND_LEDSTATE "\":%d,\""
                          D_CMND_LEDMASK "\":\"%04X\",\"" D_CMND_SAVEDATA "\":%d,\"" D_JSON_SAVESTATE "\":%d,\"" D_CMND_SWITCHTOPIC "\":\"%s\",\""
                          D_CMND_SWITCHMODE "\":[%s],\"" D_CMND_BUTTONRETAIN "\":%d,\"" D_CMND_SWITCHRETAIN "\":%d,\"" D_CMND_SENSORRETAIN "\":%d,\"" D_CMND_POWERRETAIN "\":%d,\""
                          D_CMND_INFORETAIN "\":%d,\"" D_CMND_STATERETAIN "\":%d,\"" D_CMND_STATUSRETAIN "\":%d}}"),
                          ModuleNr(), EscapeJSONString(SettingsText(SET_DEVICENAME)).c_str(), stemp,
                          TasmotaGlobal.mqtt_topic, SettingsText(SET_MQTT_BUTTON_TOPIC),
                          TasmotaGlobal.devices_present, TasmotaGlobal.power,
                          TasmotaGlobal.devices_present, Settings->power_lock,
                          Settings->poweronstate, Settings->ledstate,
                          Settings->ledmask, Settings->save_data,
                          Settings->flag.save_state,           // SetOption0 - Save power state and use after restart
                          SettingsText(SET_MQTT_SWITCH_TOPIC),
                          stemp2,
                          Settings->flag.mqtt_button_retain,   // CMND_BUTTONRETAIN
                          Settings->flag.mqtt_switch_retain,   // CMND_SWITCHRETAIN
                          Settings->flag.mqtt_sensor_retain,   // CMND_SENSORRETAIN
                          Settings->flag.mqtt_power_retain,    // CMND_POWERRETAIN
                          Settings->flag5.mqtt_info_retain,    // CMND_INFORETAIN
                          Settings->flag5.mqtt_state_retain,   // CMND_STATERETAIN
                          Settings->flag5.mqtt_status_retain   // CMND_STATUSRETAIN
                          );
    CmndStatusResponse(0);
  }

  if ((0 == payload) || (1 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS1_PARAMETER "\":{\"" D_JSON_BAUDRATE "\":%d,\"" D_CMND_SERIALCONFIG "\":\"%s\",\"" D_CMND_GROUPTOPIC "\":\"%s\",\"" D_CMND_OTAURL "\":\"%s\",\""
                          D_JSON_RESTARTREASON "\":\"%s\",\"" D_JSON_UPTIME "\":\"%s\",\"" D_JSON_STARTUPUTC "\":\"%s\",\"" D_CMND_SLEEP "\":%d,\""
                          D_JSON_CONFIG_HOLDER "\":%d,\"" D_JSON_BOOTCOUNT "\":%d,\"BCResetTime\":\"%s\",\"" D_JSON_SAVECOUNT "\":%d"
#ifdef ESP8266
                          ",\"" D_JSON_SAVEADDRESS "\":\"%X\""
#endif
                          "}}"),
                          TasmotaGlobal.baudrate, GetSerialConfig().c_str(), SettingsText(SET_MQTT_GRP_TOPIC), SettingsText(SET_OTAURL),
                          GetResetReason().c_str(), GetUptime().c_str(), GetDateAndTime(DT_RESTART).c_str(), Settings->sleep,
                          Settings->cfg_holder, Settings->bootcount, GetDateAndTime(DT_BOOTCOUNT).c_str(), Settings->save_flag
#ifdef ESP8266
                          , GetSettingsAddress()
#endif
                          );
    CmndStatusResponse(1);
  }

  if ((0 == payload) || (2 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS2_FIRMWARE "\":{\"" D_JSON_VERSION "\":\"%s%s%s\",\"" D_JSON_BUILDDATETIME "\":\"%s\""
#ifdef ESP8266
                          ",\"" D_JSON_BOOTVERSION "\":%d"
#endif
                          ",\"" D_JSON_COREVERSION "\":\"" ARDUINO_CORE_RELEASE "\",\"" D_JSON_SDKVERSION "\":\"%s\","
                          "\"CpuFrequency\":%d,\"Hardware\":\"%s\""
                          "%s}}"),
                          TasmotaGlobal.version, TasmotaGlobal.image_name, GetCodeCores().c_str(), GetBuildDateAndTime().c_str()
#ifdef ESP8266
                          , ESP.getBootVersion()
#endif
                          , ESP.getSdkVersion(),
                          ESP.getCpuFreqMHz(), GetDeviceHardwareRevision().c_str(),
                          GetStatistics().c_str());
    CmndStatusResponse(2);
  }

  if ((0 == payload) || (3 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS3_LOGGING "\":{\"" D_CMND_SERIALLOG "\":%d,\"" D_CMND_WEBLOG "\":%d,\"" D_CMND_MQTTLOG "\":%d,\"" D_CMND_SYSLOG "\":%d,\""
                          D_CMND_LOGHOST "\":\"%s\",\"" D_CMND_LOGPORT "\":%d,\"" D_CMND_SSID "\":[\"%s\",\"%s\"],\"" D_CMND_TELEPERIOD "\":%d,\""
                          D_JSON_RESOLUTION "\":\"%08X\",\"" D_CMND_SETOPTION "\":[\"%08X\",\"%s\",\"%08X\",\"%08X\",\"%08X\",\"%08X\"]}}"),
                          Settings->seriallog_level, Settings->weblog_level, Settings->mqttlog_level, Settings->syslog_level,
                          SettingsText(SET_SYSLOG_HOST), Settings->syslog_port, EscapeJSONString(SettingsText(SET_STASSID1)).c_str(), EscapeJSONString(SettingsText(SET_STASSID2)).c_str(), Settings->tele_period,
                          Settings->flag2.data, Settings->flag.data, ToHex_P((unsigned char*)Settings->param, PARAM8_SIZE, stemp2, sizeof(stemp2)),
                          Settings->flag3.data, Settings->flag4.data, Settings->flag5.data, Settings->flag6.data);
    CmndStatusResponse(3);
  }

  if ((0 == payload) || (4 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS4_MEMORY "\":{\"" D_JSON_PROGRAMSIZE "\":%d,\"" D_JSON_FREEMEMORY "\":%d,\"" D_JSON_HEAPSIZE "\":%d,\""
#ifdef ESP32
                          D_JSON_STACKLOWMARK "\":%d,\"" D_JSON_PSRMAXMEMORY "\":%d,\"" D_JSON_PSRFREEMEMORY "\":%d,\""
#endif  // ESP32
                          D_JSON_PROGRAMFLASHSIZE "\":%d,\"" D_JSON_FLASHSIZE "\":%d"
                          ",\"" D_JSON_FLASHCHIPID "\":\"%06X\""
                          ",\"FlashFrequency\":%d,\"" D_JSON_FLASHMODE "\":\"" D_TASMOTA_FLASHMODE "\""),
                          ESP_getSketchSize()/1024, ESP_getFreeSketchSpace()/1024, ESP_getFreeHeap1024(),
#ifdef ESP32
                          uxTaskGetStackHighWaterMark(nullptr) / 1024, ESP.getPsramSize()/1024, ESP.getFreePsram()/1024,
                          ESP_getFlashChipMagicSize()/1024, ESP.getFlashChipSize()/1024
#endif  // ESP32
#ifdef ESP8266
                          ESP_getFlashChipSize()/1024, ESP.getFlashChipRealSize()/1024
#endif // ESP8266
                          , ESP_getFlashChipId()
                          , ESP.getFlashChipSpeed()/1000000);
    ResponseAppendFeatures();
    XsnsDriverState();
    ResponseAppend_P(PSTR(",\"Sensors\":"));
    XsnsSensorState(0);
#ifdef USE_I2C
    ResponseAppend_P(PSTR(",\"" D_CMND_I2CDRIVER "\":"));
    I2cDriverState();
#endif
    ResponseJsonEndEnd();
    CmndStatusResponse(4);
  }

  if ((0 == payload) || (5 == payload)) {
#ifdef USE_IPV6
    if (5 == payload) { WifiDumpAddressesIPv6(); }
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS5_NETWORK "\":{\"" D_CMND_HOSTNAME "\":\"%s\",\""
                          D_CMND_IPADDRESS "\":\"%_I\",\"" D_JSON_GATEWAY "\":\"%_I\",\"" D_JSON_SUBNETMASK "\":\"%_I\",\""
                          D_JSON_DNSSERVER "1\":\"%s\",\"" D_JSON_DNSSERVER "2\":\"%s\",\""
                          D_JSON_MAC "\":\"%s\""
                          ",\"" D_JSON_IP6_GLOBAL "\":\"%s\",\"" D_JSON_IP6_LOCAL "\":\"%s\""),
                          TasmotaGlobal.hostname,
                          (uint32_t)WiFi.localIP(), Settings->ipv4_address[1], Settings->ipv4_address[2],
                          DNSGetIPStr(0).c_str(), DNSGetIPStr(1).c_str(),
                          WiFiHelper::macAddress().c_str()
                          ,WifiGetIPv6Str().c_str(), WifiGetIPv6LinkLocalStr().c_str());
#else // USE_IPV6
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS5_NETWORK "\":{\"" D_CMND_HOSTNAME "\":\"%s\",\""
                          D_CMND_IPADDRESS "\":\"%_I\",\"" D_JSON_GATEWAY "\":\"%_I\",\"" D_JSON_SUBNETMASK "\":\"%_I\",\""
                          D_JSON_DNSSERVER "1\":\"%_I\",\"" D_JSON_DNSSERVER "2\":\"%_I\",\""
                          D_JSON_MAC "\":\"%s\""),
                          TasmotaGlobal.hostname,
                          (uint32_t)WiFi.localIP(), Settings->ipv4_address[1], Settings->ipv4_address[2],
                          Settings->ipv4_address[3], Settings->ipv4_address[4],
                          WiFiHelper::macAddress().c_str());
#endif // USE_IPV6
#ifdef USE_TASMESH
    ResponseAppend_P(PSTR(",\"SoftAPMac\":\"%s\""), WiFi.softAPmacAddress().c_str());
#endif  // USE_TASMESH
//#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32 && defined(USE_ETHERNET)
#if defined(ESP32) && defined(USE_ETHERNET)
#ifdef USE_IPV6
    ResponseAppend_P(PSTR(",\"Ethernet\":{\"" D_CMND_HOSTNAME "\":\"%s\",\""
                          D_CMND_IPADDRESS "\":\"%_I\",\"" D_JSON_GATEWAY "\":\"%_I\",\"" D_JSON_SUBNETMASK "\":\"%_I\",\""
                          D_JSON_DNSSERVER "1\":\"%s\",\"" D_JSON_DNSSERVER "2\":\"%s\",\""
                          D_JSON_MAC "\":\"%s\",\"" D_JSON_IP6_GLOBAL "\":\"%s\",\"" D_JSON_IP6_LOCAL "\":\"%s\"}"),
                          EthernetHostname(),
                          (uint32_t)EthernetLocalIP(), Settings->eth_ipv4_address[1], Settings->eth_ipv4_address[2],
                          DNSGetIPStr(0).c_str(), DNSGetIPStr(1).c_str(),
                          EthernetMacAddress().c_str(),
                          EthernetGetIPv6Str().c_str(), EthernetGetIPv6LinkLocalStr().c_str());
#else // USE_IPV6
    ResponseAppend_P(PSTR(",\"Ethernet\":{\"" D_CMND_HOSTNAME "\":\"%s\",\""
                          D_CMND_IPADDRESS "\":\"%_I\",\"" D_JSON_GATEWAY "\":\"%_I\",\"" D_JSON_SUBNETMASK "\":\"%_I\",\""
                          D_JSON_DNSSERVER "1\":\"%_I\",\"" D_JSON_DNSSERVER "2\":\"%_I\",\""
                          D_JSON_MAC "\":\"%s\"}"),
                          EthernetHostname(),
                          (uint32_t)EthernetLocalIP(), Settings->eth_ipv4_address[1], Settings->eth_ipv4_address[2],
                          Settings->eth_ipv4_address[3], Settings->eth_ipv4_address[4],
                          EthernetMacAddress().c_str());

#endif // USE_IPV6
#endif  // USE_ETHERNET
    float wifi_tx_power = WifiGetOutputPower();
    ResponseAppend_P(PSTR(",\"" D_CMND_WEBSERVER "\":%d,\"HTTP_API\":%d,\"" D_CMND_WIFICONFIG "\":%d,\"" D_CMND_WIFIPOWER "\":%1_f}}"),
                          Settings->webserver, Settings->flag5.disable_referer_chk, Settings->sta_config, &wifi_tx_power);
    CmndStatusResponse(5);
  }

  if (((0 == payload) || (6 == payload)) && Settings->flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS6_MQTT "\":{\"" D_CMND_MQTTHOST "\":\"%s\",\"" D_CMND_MQTTPORT "\":%d,\"" D_CMND_MQTTCLIENT D_JSON_MASK "\":\"%s\",\""
                          D_CMND_MQTTCLIENT "\":\"%s\",\"" D_CMND_MQTTUSER "\":\"%s\",\"" D_JSON_MQTT_COUNT "\":%d,\"MAX_PACKET_SIZE\":%d,\"KEEPALIVE\":%d,\"SOCKET_TIMEOUT\":%d}}"),
                          SettingsText(SET_MQTT_HOST), Settings->mqtt_port, EscapeJSONString(SettingsText(SET_MQTT_CLIENT)).c_str(),
                          TasmotaGlobal.mqtt_client, EscapeJSONString(SettingsText(SET_MQTT_USER)).c_str(), MqttConnectCount(), MQTT_MAX_PACKET_SIZE, Settings->mqtt_keepalive, Settings->mqtt_socket_timeout);
    CmndStatusResponse(6);
  }

  if ((0 == payload) || (7 == payload)) {
    if (99 == Settings->timezone) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%d" ), Settings->timezone);
    } else {
      snprintf_P(stemp, sizeof(stemp), PSTR("\"%s\"" ), GetTimeZone().c_str());
    }
#if defined(USE_TIMERS) && defined(USE_SUNRISE)
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS7_TIME "\":{\"" D_JSON_UTC_TIME "\":\"%s\",\"" D_JSON_LOCAL_TIME "\":\"%s\",\"" D_JSON_STARTDST "\":\"%s\",\""
                          D_JSON_ENDDST "\":\"%s\",\"" D_CMND_TIMEZONE "\":%s,\"" D_JSON_SUNRISE "\":\"%s\",\"" D_JSON_SUNSET "\":\"%s\"}}"),
                          GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_LOCALNOTZ).c_str(), GetDateAndTime(DT_DST).c_str(),
                          GetDateAndTime(DT_STD).c_str(), stemp, GetSun(0).c_str(), GetSun(1).c_str());
#else
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS7_TIME "\":{\"" D_JSON_UTC_TIME "\":\"%s\",\"" D_JSON_LOCAL_TIME "\":\"%s\",\"" D_JSON_STARTDST "\":\"%s\",\""
                          D_JSON_ENDDST "\":\"%s\",\"" D_CMND_TIMEZONE "\":%s}}"),
                          GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_LOCALNOTZ).c_str(), GetDateAndTime(DT_DST).c_str(),
                          GetDateAndTime(DT_STD).c_str(), stemp);
#endif  // USE_TIMERS and USE_SUNRISE
    CmndStatusResponse(7);
  }

#if defined(USE_ENERGY_SENSOR) && defined(USE_ENERGY_MARGIN_DETECTION)
  if (TasmotaGlobal.energy_driver) {
    if ((0 == payload) || (9 == payload)) {
      EnergyMarginStatus();
      CmndStatusResponse(9);
    }
  }
#endif  // USE_ENERGY_MARGIN_DETECTION

  if ((0 == payload) || (8 == payload) || (10 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS10_SENSOR "\":"));
    MqttShowSensor(true);
    ResponseJsonEnd();
    CmndStatusResponse((8 == payload) ? 8 : 10);
  }

  if ((0 == payload) || (11 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS11_STATUS "\":"));
    MqttShowState();
    ResponseJsonEnd();
    CmndStatusResponse(11);
  }

#ifndef FIRMWARE_MINIMAL
  if (CrashFlag()) {
    if ((0 == payload) || (12 == payload)) {
      Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS12_STATUS "\":"));
      CrashDump();
      ResponseJsonEnd();
      CmndStatusResponse(12);
    }
  }
#endif // FIRMWARE_MINIMAL

#ifdef USE_SHUTTER
  if ((0 == payload) || (13 == payload)) {
    if (ShutterStatus()) { CmndStatusResponse(13); }
  }
#endif

  CmndStatusResponse(99);

  ResponseClear();
}

void CmndState(void)
{
  ResponseClear();
  MqttShowState();
  if (Settings->flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_STATE), Settings->flag5.mqtt_state_retain);
  }
#ifdef USE_HOME_ASSISTANT
  if (Settings->flag.hass_discovery) {       // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
    HAssPublishStatus();
  }
#endif  // USE_HOME_ASSISTANT
}

void CmndTempOffset(void)
{
  if (XdrvMailbox.data_len > 0) {
    int value = (int)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value > -127) && (value < 127)) {
      Settings->temp_comp = value;
    }
  }
  ResponseCmndFloat((float)(Settings->temp_comp) / 10, 1);
}

void CmndHumOffset(void)
{
  if (XdrvMailbox.data_len > 0) {
    int value = (int)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value > -101) && (value < 101)) {
      Settings->hum_comp = value;
    }
  }
  ResponseCmndFloat((float)(Settings->hum_comp) / 10, 1);
}

void CmndGlobalTemp(void) {
  if (2 == XdrvMailbox.index) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 251)) {
      Settings->global_sensor_index[0] = XdrvMailbox.payload;
      TasmotaGlobal.user_globals[0] = 0;
    }
    ResponseCmndIdxNumber(Settings->global_sensor_index[0]);
  } else {
    if (XdrvMailbox.data_len > 0) {
      // Set temperature based on SO8 (Celsius or Fahrenheit)
      float temperature = ConvertTempToCelsius(CharToFloat(XdrvMailbox.data));
      // Temperature is now Celsius
      if ((temperature >= -50.0f) && (temperature <= 100.0f)) {
        TasmotaGlobal.temperature_celsius = temperature;
        TasmotaGlobal.global_update = TasmotaGlobal.uptime;
        TasmotaGlobal.user_globals[0] = 1;
      }
    }
    ResponseCmndFloat(TasmotaGlobal.temperature_celsius, 1);
  }
}

void CmndGlobalHum(void) {
  if (2 == XdrvMailbox.index) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 251)) {
      Settings->global_sensor_index[1] = XdrvMailbox.payload;
      TasmotaGlobal.user_globals[1] = 0;
    }
    ResponseCmndIdxNumber(Settings->global_sensor_index[1]);
  } else {
    if (XdrvMailbox.data_len > 0) {
      float humidity = CharToFloat(XdrvMailbox.data);
      if ((humidity >= 0.0f) && (humidity <= 100.0f)) {
        TasmotaGlobal.humidity = humidity;
        TasmotaGlobal.global_update = TasmotaGlobal.uptime;
        TasmotaGlobal.user_globals[1] = 1;
      }
    }
    ResponseCmndFloat(TasmotaGlobal.humidity, 1);
  }
}

void CmndGlobalPress(void) {
  if (2 == XdrvMailbox.index) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 251)) {
      Settings->global_sensor_index[2] = XdrvMailbox.payload;
      TasmotaGlobal.user_globals[2] = 0;
    }
    ResponseCmndIdxNumber(Settings->global_sensor_index[2]);
  } else {
    if (XdrvMailbox.data_len > 0) {
      // Set pressure based on SO24 (hPa or mmHg (or inHg based on SO139))
      float pressure = ConvertHgToHpa(CharToFloat(XdrvMailbox.data));
      // Pressure is now hPa
      if ((pressure >= 0.0f) && (pressure <= 1200.0f)) {
        TasmotaGlobal.pressure_hpa = pressure;
        TasmotaGlobal.global_update = TasmotaGlobal.uptime;
        TasmotaGlobal.user_globals[2] = 1;
      }
    }
    ResponseCmndFloat(TasmotaGlobal.pressure_hpa, 1);
  }
}

void CmndSleep(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 251)) {
    Settings->sleep = XdrvMailbox.payload;
    TasmotaGlobal.sleep = XdrvMailbox.payload;
    WiFiSetSleepMode();
  }
  Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, XdrvMailbox.command, Settings->sleep, TasmotaGlobal.sleep);

}

void CmndUpgrade(void) {
  // Check if the payload is numerically 1, and had no trailing chars.
  //   e.g. "1foo" or "1.2.3" could fool us.
  // Check if the version we have been asked to upgrade to is higher than our current version.
  //   We also need at least 3 chars to make a valid version number string.
  // Upload 1 - OTA upload binary
  // Upload 2 - (ESP32 only) OTA upload safeboot binary if partition is present
  if (((1 == XdrvMailbox.data_len) && (1 == XdrvMailbox.payload)) || ((XdrvMailbox.data_len >= 3) && NewerVersion(XdrvMailbox.data))) {
#ifdef ESP32
    TasmotaGlobal.ota_factory = false;  // Reset in case of failed safeboot upgrade
#endif  // ESP32 and WEBCLIENT_HTTPS
    TasmotaGlobal.ota_state_flag = 3;
    char stemp1[TOPSZ];
    Response_P(PSTR("{\"%s\":\"" D_JSON_VERSION " %s " D_JSON_FROM " %s\"}"), XdrvMailbox.command, TasmotaGlobal.version, GetOtaUrl(stemp1, sizeof(stemp1)));
  }
#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
  else if (EspSingleOtaPartition() && !EspRunningFactoryPartition() && (1 == XdrvMailbox.data_len) && (2 == XdrvMailbox.payload)) {
    TasmotaGlobal.ota_factory = true;
    TasmotaGlobal.ota_state_flag = 3;
    ResponseCmndChar(PSTR("Safeboot"));
  }
#endif  // ESP32 and WEBCLIENT_HTTPS
  else {
    Response_P(PSTR("{\"%s\":\"" D_JSON_ONE_OR_GT "\"}"), XdrvMailbox.command, TasmotaGlobal.version);
  }
}

void CmndOtaUrl(void)
{
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_OTAURL, (SC_DEFAULT == Shortcut()) ? PSTR(OTA_URL) : XdrvMailbox.data);
  }
  ResponseCmndChar(SettingsText(SET_OTAURL));
}

void CmndSeriallog(void)
{
  if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {

#ifdef ESP32
    if (tasconsole_serial) {
#endif  // ESP32

    Settings->flag.mqtt_serial = 0;       // CMND_SERIALSEND and CMND_SERIALLOG

#ifdef ESP32
    }
#endif  // ESP32

    SetTasConlog(XdrvMailbox.payload);
  }
  Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, XdrvMailbox.command, Settings->seriallog_level, TasmotaGlobal.seriallog_level);
}

void CmndRestart(void)
{
  switch (XdrvMailbox.payload) {
  case 1:
    TasmotaGlobal.restart_flag = 2;
    ResponseCmndChar(PSTR(D_JSON_RESTARTING));
    break;
  case 2:
    TasmotaGlobal.restart_flag = 2;
    TasmotaGlobal.restart_halt = true;
    ResponseCmndChar(PSTR(D_JSON_HALTING));
    break;
#ifdef ESP32
  case 3:
    if (EspPrepSwitchPartition(2)) {  // Toggle partition between safeboot and production
      TasmotaGlobal.restart_flag = 2;
      ResponseCmndChar(PSTR("Switching"));
    }
    break;
#endif  // ESP32
  case 9:
    TasmotaGlobal.restart_flag = 2;
    TasmotaGlobal.restart_deepsleep = true;
    ResponseCmndChar(PSTR("Go to sleep"));
    break;
#ifndef FIRMWARE_MINIMAL
  case -1:
    CmndCrash();    // force a crash
    break;
  case -2:
    CmndWDT();
    break;
  case -3:
    CmndBlockedLoop();
    break;
#endif // FIRMWARE_MINIMAL
  case 99:
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_RESTARTING));
    EspRestart();
    break;
  default:
    ResponseCmndChar_P(PSTR(D_JSON_ONE_TO_RESTART));
  }
}

void CmndPowerOnState(void)
{
#ifdef ESP8266
  if (TasmotaGlobal.module_type != MOTOR)
#endif  // ESP8266
  {
    /* 0 = Keep relays off after power on
      * 1 = Turn relays on after power on, if PulseTime set wait for PulseTime seconds, and turn relays off
      * 2 = Toggle relays after power on
      * 3 = Set relays to last saved state after power on
      * 4 = Turn relays on and disable any relay control (used for Sonoff Pow to always measure power)
      * 5 = Keep relays off after power on, if PulseTime set wait for PulseTime seconds, and turn relays on
      */
    if ((XdrvMailbox.payload >= POWER_ALL_OFF) && (XdrvMailbox.payload <= POWER_ALL_OFF_PULSETIME_ON)) {
      Settings->poweronstate = XdrvMailbox.payload;
      if (POWER_ALL_ALWAYS_ON == Settings->poweronstate) {
        for (uint32_t i = 1; i <= TasmotaGlobal.devices_present; i++) {
          ExecuteCommandPower(i, POWER_ON, SRC_IGNORE);
        }
      }
    }
    ResponseCmndNumber(Settings->poweronstate);
  }
}

void CmndPulsetime(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_PULSETIMERS)) {
/*
    uint32_t items = 1;
    if (!XdrvMailbox.usridx && !XdrvMailbox.data_len) {
      items = MAX_PULSETIMERS;
    } else {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 65536)) {
        Settings->pulse_timer[XdrvMailbox.index -1] = XdrvMailbox.payload;  // 0 - 65535
        SetPulseTimer(XdrvMailbox.index -1, XdrvMailbox.payload);
      }
    }
    ResponseClear();
    for (uint32_t i = 0; i < items; i++) {
      uint32_t index = (1 == items) ? XdrvMailbox.index : i +1;
      ResponseAppend_P(PSTR("%c\"%s%d\":{\"" D_JSON_SET "\":%d,\"" D_JSON_REMAINING "\":%d}"),
        (i) ? ',' : '{',
        XdrvMailbox.command, index,
        Settings->pulse_timer[index -1], GetPulseTimer(index -1));
    }
    ResponseJsonEnd();
*/
    if (!XdrvMailbox.usridx && !XdrvMailbox.data_len) {
      Response_P(PSTR("{\"" D_CMND_PULSETIME "\":{\"" D_JSON_SET "\":["));
      for (uint32_t i = 0; i < MAX_PULSETIMERS; i++) {
        ResponseAppend_P(PSTR("%s%d"), (i)?",":"", Settings->pulse_timer[i]);
      }
      ResponseAppend_P(PSTR("],\"" D_JSON_REMAINING "\":["));
      for (uint32_t i = 0; i < MAX_PULSETIMERS; i++) {
        ResponseAppend_P(PSTR("%s%d"), (i)?",":"", GetPulseTimer(i));
      }
      ResponseAppend_P(PSTR("]}}"));
    } else {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 65536)) {
        Settings->pulse_timer[XdrvMailbox.index -1] = XdrvMailbox.payload;  // 0 - 65535
        SetPulseTimer(XdrvMailbox.index -1, XdrvMailbox.payload);
      }
      uint32_t index = XdrvMailbox.index;
      Response_P(PSTR("{\"%s%d\":{\"" D_JSON_SET "\":%d,\"" D_JSON_REMAINING "\":%d}}"),
        XdrvMailbox.command, index,
        Settings->pulse_timer[index -1], GetPulseTimer(index -1));
    }
  }
}

void CmndBlinktime(void)
{
  if ((XdrvMailbox.payload > 1) && (XdrvMailbox.payload <= 3600)) {
    Settings->blinktime = XdrvMailbox.payload;
    if (TasmotaGlobal.blink_timer > 0) {
      TasmotaGlobal.blink_timer = millis() + (100 * XdrvMailbox.payload);
    }
  }
  ResponseCmndNumber(Settings->blinktime);
}

void CmndBlinkcount(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 32000)) {
    Settings->blinkcount = XdrvMailbox.payload;  // 0 - 65535
    if (TasmotaGlobal.blink_counter) { TasmotaGlobal.blink_counter = Settings->blinkcount *2; }
  }
  ResponseCmndNumber(Settings->blinkcount);
}

void CmndStateText(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_STATE_TEXT)) {
    if (!XdrvMailbox.usridx) {
      ResponseCmndAll(SET_STATE_TXT1, MAX_STATE_TEXT);
    } else {
      if (XdrvMailbox.data_len > 0) {
        for (uint32_t i = 0; i <= XdrvMailbox.data_len; i++) {
          if (XdrvMailbox.data[i] == ' ') XdrvMailbox.data[i] = '_';
        }
        SettingsUpdateText(SET_STATE_TXT1 + XdrvMailbox.index -1, XdrvMailbox.data);
      }
      ResponseCmndIdxChar(GetStateText(XdrvMailbox.index -1));
    }
  }
}

void CmndSavedata(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3600)) {
    Settings->save_data = XdrvMailbox.payload;
    TasmotaGlobal.save_data_counter = Settings->save_data;
  }
  SettingsSaveAll();
  char stemp1[TOPSZ];
  if (Settings->save_data > 1) {
    snprintf_P(stemp1, sizeof(stemp1), PSTR(D_JSON_EVERY " %d " D_UNIT_SECOND), Settings->save_data);
  }
  ResponseCmndChar((Settings->save_data > 1) ? stemp1 : GetStateText(Settings->save_data));
}

void CmndSetoption(void) {
  snprintf_P(XdrvMailbox.command, CMDSZ, PSTR(D_CMND_SETOPTION));  // Rename result shortcut command SO to SetOption
  CmndSetoptionBase(1);
}

// Code called by SetOption and by Berry
bool SetoptionDecode(uint32_t index, uint32_t *ptype, uint32_t *pindex) {
  if (index < 178) {
    if (index <= 31) {         // SetOption0 .. 31 = Settings->flag
      *ptype = 2;
      *pindex = index;         // 0 .. 31
    }
    else if (index <= 49) {    // SetOption32 .. 49 = Settings->param
      *ptype = 1;
      *pindex = index -32;     // 0 .. 17 (= PARAM8_SIZE -1)
    }
    else if (index <= 81) {    // SetOption50 .. 81 = Settings->flag3
      *ptype = 3;
      *pindex = index -50;     // 0 .. 31
    }
    else if (index <= 113) {   // SetOption82 .. 113 = Settings->flag4
      *ptype = 4;
      *pindex = index -82;     // 0 .. 31
    }
    else if (index <= 145) {   // SetOption114 .. 145 = Settings->flag5
      *ptype = 5;
      *pindex = index -114;    // 0 .. 31
    }
    else {                     // SetOption146 .. 177 = Settings->flag6
      *ptype = 6;
      *pindex = index -146;    // 0 .. 31
    }
    return true;
  }
  return false;
}

uint32_t GetOption(uint32_t index) {
  uint32_t ptype;
  uint32_t pindex;
  if (SetoptionDecode(index, &ptype, &pindex)) {
    if (1 == ptype) {
      return Settings->param[pindex];
    } else {
      uint32_t flag = Settings->flag.data;
      if (3 == ptype) {
        flag = Settings->flag3.data;
      }
      else if (4 == ptype) {
        flag = Settings->flag4.data;
      }
      else if (5 == ptype) {
        flag = Settings->flag5.data;
      }
      else if (6 == ptype) {
        flag = Settings->flag6.data;
      }
      return bitRead(flag, pindex);
    }
  } else {
    return 0;   // fallback
  }
}

void CmndSetoptionBase(bool indexed) {
  // Allow a command to access a single SetOption by it's command name
  // indexed = 0 : No index will be returned attached to the command
  //               {"ClockDirection":"OFF"}
  // indexed = 1 : The SetOption index will be returned with the command
  //               {"SetOption16":"OFF"}
  uint32_t ptype;
  uint32_t pindex;
  if (SetoptionDecode(XdrvMailbox.index, &ptype, &pindex)) {

    if (XdrvMailbox.payload >= 0) {
      if (1 == ptype) {                    // SetOption32 .. 49
        uint32_t param_low = 0;
        uint32_t param_high = 255;
        switch (pindex) {
          case P_HOLD_TIME:
          case P_MAX_POWER_RETRY:
          case P_BISTABLE_PULSE:
            param_low = 1;
            param_high = 250;
            break;
        }
        if ((XdrvMailbox.payload >= param_low) && (XdrvMailbox.payload <= param_high)) {
          Settings->param[pindex] = XdrvMailbox.payload;
#ifdef USE_LIGHT
          if (P_RGB_REMAP == pindex) {
            LightUpdateColorMapping();
            TasmotaGlobal.restart_flag = 2;  // SetOption37 needs a reboot in most cases
          }
#endif
#if (defined(USE_IR_REMOTE) && defined(USE_IR_RECEIVE)) || defined(USE_IR_REMOTE_FULL)
          if (P_IR_UNKNOW_THRESHOLD == pindex) {
            IrReceiveUpdateThreshold();    // SetOption38
          }
          if (P_IR_TOLERANCE == pindex) {
            IrReceiveUpdateTolerance();    // SetOption44
          }
#endif
#ifdef ROTARY_V1
          if (P_ROTARY_MAX_STEP == pindex) {
            RotaryInitMaxSteps();          // SetOption43
          }
#endif
        } else {
          ptype = 99;                      // Command Error
        }
      } else {
        if (XdrvMailbox.payload <= 1) {
          if (2 == ptype) {                // SetOption0 .. 31
            switch (pindex) {
              case 5:                      // mqtt_power_retain (CMND_POWERRETAIN)
              case 6:                      // mqtt_button_retain (CMND_BUTTONRETAIN)
              case 7:                      // mqtt_switch_retain (CMND_SWITCHRETAIN)
              case 9:                      // mqtt_sensor_retain (CMND_SENSORRETAIN)
              case 14:                     // interlock (CMND_INTERLOCK)
              case 22:                     // mqtt_serial (SerialSend and SerialLog)
              case 23:                     // mqtt_serial_raw (SerialSend)
              case 25:                     // knx_enabled (Web config)
              case 27:                     // knx_enable_enhancement (Web config)
                ptype = 99;                // Command Error
                break;                     // Ignore command SetOption
              case 3:                      // mqtt
              case 15:                     // pwm_control
                TasmotaGlobal.restart_flag = 2;
              default:
                bitWrite(Settings->flag.data, pindex, XdrvMailbox.payload);
            }
            if (12 == pindex) {            // stop_flash_rotate
              TasmotaGlobal.stop_flash_rotate = XdrvMailbox.payload;
              SettingsSave(2);
            }
#ifdef USE_HOME_ASSISTANT
            if ((19 == pindex) || (30 == pindex) || (114 == pindex)) {
              HAssDiscover();              // Delayed execution to provide enough resources during hass_discovery or hass_light
            }
#endif  // USE_HOME_ASSISTANT
#ifdef USE_TASMOTA_DISCOVERY
            if (19 == pindex) {
              TasRediscover();
            }
#endif  // USE_TASMOTA_DISCOVERY
          }
          else if (3 == ptype) {           // SetOption50 .. 81
            bitWrite(Settings->flag3.data, pindex, XdrvMailbox.payload);
            switch (pindex) {
              case 5:                      // SetOption55
                if (0 == XdrvMailbox.payload) {
                  TasmotaGlobal.restart_flag = 2;  // Disable mDNS needs restart
                }
                break;
              case 10:                     // SetOption60 enable or disable traditional sleep
                WiFiSetSleepMode();        // Update WiFi sleep mode accordingly
                break;
              case 18:                     // SetOption68 for multi-channel PWM, requires a reboot
              case 25:                     // SetOption75 grouptopic change
                TasmotaGlobal.restart_flag = 2;
                break;
            }
          }
          else if (4 == ptype) {           // SetOption82 .. 113
            bitWrite(Settings->flag4.data, pindex, XdrvMailbox.payload);
            switch (pindex) {
#ifdef USE_LIGHT
              case 0:                      // SetOption 82 - (Alexa) Reduced CT range for Alexa (1)
                setAlexaCTRange();
                break;
#endif
              case 3:                      // SetOption85 - Enable Device Groups
              case 6:                      // SetOption88 - PWM Dimmer Buttons control remote devices
              case 15:                     // SetOption97 - Set Baud rate for TuyaMCU serial communication (0 = 9600 or 1 = 115200)
              case 20:                     // SetOption102 - Set Baud rate for Teleinfo serial communication (0 = 1200 or 1 = 9600)
              case 21:                     // SetOption103 - Enable TLS mode (requires TLS version)
              case 22:                     // SetOption104 - No Retain - disable all MQTT retained messages, some brokers don't support it: AWS IoT, Losant
              case 24:                     // SetOption106 - Virtual CT - Creates a virtual White ColorTemp for RGBW lights
              case 25:                     // SetOption107 - Virtual CT Channel - signals whether the hardware white is cold CW (true) or warm WW (false)
                TasmotaGlobal.restart_flag = 2;
                break;
#ifdef USE_PWM_DIMMER
              case 5:                      // SetOption87 - (PWM Dimmer) Turn red LED on (1) when powered off
                TasmotaGlobal.restore_powered_off_led_counter = 1;
                break;
#endif  // USE_PWM_DIMMER
            }
          }
          else if (5 == ptype) {           // SetOption114 .. 145
            bitWrite(Settings->flag5.data, pindex, XdrvMailbox.payload);
            switch (pindex) {
              case 1:                      // SetOption115 - Enable ESP32 MI32
                if (0 == XdrvMailbox.payload) {
                  TasmotaGlobal.restart_flag = 2;
                }
                break;
              case 18:                     // SetOption132 - TLS Fingerprint
                TasmotaGlobal.restart_flag = 2;
                break;
            }
          }
          else if (6 == ptype) {           // SetOption146 .. 177
            bitWrite(Settings->flag6.data, pindex, XdrvMailbox.payload);
            switch (pindex) {
              case 5:                     // SetOption151 - Matter enabled
              case 6:                     // SetOption152 - (Power) Use single pin bistable
              case 7:                     // SetOption153 - (Berry) Disable autoexec.be on restart (1)
                TasmotaGlobal.restart_flag = 2;
                break;
            }
          }
        } else {
          ptype = 99;                      // Command Error
        }
      }
    }

    if (ptype < 99) {
      if (1 == ptype) {
        if (indexed) {
          ResponseCmndIdxNumber(Settings->param[pindex]);
        } else {
          ResponseCmndNumber(Settings->param[pindex]);
        }
      } else {
        uint32_t flag = Settings->flag.data;
        if (3 == ptype) {
          flag = Settings->flag3.data;
        }
        else if (4 == ptype) {
          flag = Settings->flag4.data;
        }
        else if (5 == ptype) {
          flag = Settings->flag5.data;
        }
        else if (6 == ptype) {
          flag = Settings->flag6.data;
        }
        if (indexed) {
          ResponseCmndIdxChar(GetStateText(bitRead(flag, pindex)));
        } else {
          ResponseCmndChar(GetStateText(bitRead(flag, pindex)));
        }
      }
    }
  }
}

void CmndTemperatureResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings->flag2.temperature_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->flag2.temperature_resolution);
}

void CmndHumidityResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings->flag2.humidity_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->flag2.humidity_resolution);
}

void CmndPressureResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings->flag2.pressure_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->flag2.pressure_resolution);
}

void CmndPowerResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings->flag2.wattage_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->flag2.wattage_resolution);
}

void CmndVoltageResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings->flag2.voltage_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->flag2.voltage_resolution);
}

void CmndFrequencyResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings->flag2.frequency_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->flag2.frequency_resolution);
}

void CmndCurrentResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings->flag2.current_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->flag2.current_resolution);
}

void CmndEnergyResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 5)) {
    Settings->flag2.energy_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->flag2.energy_resolution);
}

void CmndWeightResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings->flag2.weight_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->flag2.weight_resolution);
}

void CmndSpeedUnit(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
    Settings->flag2.speed_conversion = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->flag2.speed_conversion);
}

void CmndModule(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= MAXMODULE)) {
    bool present = false;
    if (0 == XdrvMailbox.payload) {
      XdrvMailbox.payload = USER_MODULE;
      present = true;
    } else {
      XdrvMailbox.payload--;
      present = ValidTemplateModule(XdrvMailbox.payload);
    }
    if (present) {
      if (XdrvMailbox.index == 2) {
        Settings->fallback_module = XdrvMailbox.payload;
      } else {
        Settings->last_module = Settings->module;
        Settings->module = XdrvMailbox.payload;
        SetModuleType();
        if (Settings->last_module != XdrvMailbox.payload) {
          for (uint32_t i = 0; i < nitems(Settings->my_gp.io); i++) {
            Settings->my_gp.io[i] = GPIO_NONE;
          }
        }
        TasmotaGlobal.restart_flag = 2;
      }
    }
  }
  uint8_t module_real = Settings->module;
  uint8_t module_number = ModuleNr();
  if (XdrvMailbox.index == 2) {
    module_real = Settings->fallback_module;
    module_number = (USER_MODULE == Settings->fallback_module) ? 0 : Settings->fallback_module +1;
    strcat(XdrvMailbox.command, "2");
  }
  Response_P(S_JSON_COMMAND_NVALUE_SVALUE, XdrvMailbox.command, module_number, AnyModuleName(module_real).c_str());
}

void CmndModules(void)
{
  uint32_t midx = USER_MODULE;
  Response_P(PSTR("{\"" D_CMND_MODULES "\":{"));
  for (uint32_t i = 0; i <= sizeof(kModuleNiceList); i++) {
    if (i > 0) {
      midx = pgm_read_byte(kModuleNiceList + i -1);
      ResponseAppend_P(PSTR(","));
    }
    uint32_t j = i ? midx +1 : 0;
    ResponseAppend_P(PSTR("\"%d\":\"%s\""), j, AnyModuleName(midx).c_str());
  }
  ResponseJsonEndEnd();
}

void CmndGpio(void)
{
  if (XdrvMailbox.index < nitems(Settings->my_gp.io)) {
    myio template_gp;
    TemplateGpios(&template_gp);
    if (ValidGPIO(XdrvMailbox.index, template_gp.io[XdrvMailbox.index]) && (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < AGPIO(GPIO_SENSOR_END))) {
      bool present = false;
      for (uint32_t i = 0; i < nitems(kGpioNiceList); i++) {
        uint32_t midx = pgm_read_word(kGpioNiceList + i);
        uint32_t max_midx = ((midx & 0x001F) > 0) ? midx : midx +1;
        if ((XdrvMailbox.payload >= (midx & 0xFFE0)) && (XdrvMailbox.payload < max_midx)) {
          present = true;
          break;
        }
      }
      if (present) {
        for (uint32_t i = 0; i < nitems(Settings->my_gp.io); i++) {
          if (ValidGPIO(i, template_gp.io[i]) && (Settings->my_gp.io[i] == XdrvMailbox.payload)) {
            Settings->my_gp.io[i] = GPIO_NONE;
          }
        }
        Settings->my_gp.io[XdrvMailbox.index] = XdrvMailbox.payload;
        TasmotaGlobal.restart_flag = 2;
      }
    }
    bool jsflg = false;
    bool jsflg2 = false;
    for (uint32_t i = 0; i < nitems(Settings->my_gp.io); i++) {
      if (ValidGPIO(i, template_gp.io[i]) || ((255 == XdrvMailbox.payload) && !FlashPin(i))) {
        if (!jsflg) {
          Response_P(PSTR("{"));
        } else {
          ResponseAppend_P(PSTR(","));
        }
        jsflg = true;
        uint32_t sensor_type = Settings->my_gp.io[i];
        if (!ValidGPIO(i, template_gp.io[i])) {
          sensor_type = template_gp.io[i];
          if (AGPIO(GPIO_USER) == sensor_type) {  // A user GPIO equals a not connected (=GPIO_NONE) GPIO here
            sensor_type = GPIO_NONE;
          }
        }
        char sindex[4] = { 0 };
        uint32_t sensor_name_idx = BGPIO(sensor_type);
        uint32_t nice_list_search = sensor_type & 0xFFE0;
        for (uint32_t j = 0; j < nitems(kGpioNiceList); j++) {
          uint32_t nls_idx = pgm_read_word(kGpioNiceList + j);
          if (((nls_idx & 0xFFE0) == nice_list_search) && ((nls_idx & 0x001F) > 0)) {
            snprintf_P(sindex, sizeof(sindex), PSTR("%d"), (sensor_type & 0x001F) +1);
            break;
          }
        }
        const char *sensor_names = kSensorNames;
        if (sensor_name_idx > GPIO_FIX_START) {
          sensor_name_idx = sensor_name_idx - GPIO_FIX_START -1;
          sensor_names = kSensorNamesFixed;
        }
        char stemp1[TOPSZ];
        ResponseAppend_P(PSTR("\"" D_CMND_GPIO "%d\":{\"%d\":\"%s%s\"}"), i, sensor_type, GetTextIndexed(stemp1, sizeof(stemp1), sensor_name_idx, sensor_names), sindex);
        jsflg2 = true;
      }
    }
    if (jsflg) {
      ResponseJsonEnd();
    } else {
      if (!jsflg2) {
        ResponseCmndChar(PSTR(D_JSON_NOT_SUPPORTED));
      }
    }
  }
}

void CmndGpioRead(void) {
  // Perform a digitalRead on each configured GPIO
  myio template_gp;
  TemplateGpios(&template_gp);
  bool jsflg = false;
  Response_P(PSTR("{\"" D_CMND_GPIOREAD "\":{"));
  for (uint32_t i = 0; i < nitems(Settings->my_gp.io); i++) {
    uint32_t sensor_type = template_gp.io[i];    // Template GPIO
    if (((sensor_type != GPIO_NONE) && (AGPIO(GPIO_USER) != sensor_type)) ||
        (Settings->my_gp.io[i] != GPIO_NONE)) {  // Module GPIO
      if (jsflg) {
        ResponseAppend_P(PSTR(","));
      }
      jsflg = true;
      bool state = digitalRead(i);
      ResponseAppend_P(PSTR("\"" D_CMND_GPIO "%d\":%d"), i, state);
    }
  }
  ResponseAppend_P(PSTR("}}"));
}

void ShowGpios(const uint16_t *NiceList, uint32_t size, uint32_t offset, uint32_t &lines) {
  uint32_t ridx;
  uint32_t midx;
  bool jsflg = false;
  for (uint32_t i = offset; i < size; i++) {  // Skip ADC_NONE
    if (NiceList == nullptr) {
      ridx = AGPIO(i);
      midx = i;
    } else {
      ridx = pgm_read_word(NiceList + i) & 0xFFE0;
      midx = BGPIO(ridx);
    }
    if (!jsflg) {
      Response_P(PSTR("{\"" D_CMND_GPIOS "%d\":{"), lines);
    } else {
      ResponseAppend_P(PSTR(","));
    }
    jsflg = true;
    char stemp1[TOPSZ];
    if ((ResponseAppend_P(PSTR("\"%d\":\"%s\""), ridx, GetTextIndexed(stemp1, sizeof(stemp1), midx, kSensorNames)) > (MAX_LOGSZ - TOPSZ)) || (i == size -1)) {
      ResponseJsonEndEnd();
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, XdrvMailbox.command);
      jsflg = false;
      lines++;
    }
  }
}

void CmndGpios(void)
{
  uint32_t lines = 1;
  if (XdrvMailbox.payload == 255) {
//    DumpConvertTable();
    ShowGpios(nullptr, GPIO_SENSOR_END, 0, lines);
  } else {
    ShowGpios(kGpioNiceList, nitems(kGpioNiceList), 0, lines);
#ifdef ESP8266
#ifndef USE_ADC_VCC
    ShowGpios(kAdcNiceList, nitems(kAdcNiceList), 1, lines);
#endif  // USE_ADC_VCC
#endif  // ESP8266
  }
  ResponseClear();
}

void CmndTemplate(void)
{
  // {"NAME":"Shelly 2.5","GPIO":[320,0,32,0,224,193,0,0,640,192,608,225,3456,4736],"FLAG":0,"BASE":18}

  bool error = false;

  if (strchr(XdrvMailbox.data, '{') == nullptr) {  // If no JSON it must be parameter
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= MAXMODULE)) {
      XdrvMailbox.payload--;
      if (ValidTemplateModule(XdrvMailbox.payload)) {
        ModuleDefault(XdrvMailbox.payload);     // Copy template module
        if (USER_MODULE == Settings->module) { TasmotaGlobal.restart_flag = 2; }
      }
    }
    else if (0 == XdrvMailbox.payload) {        // Copy current template to user template
      if (Settings->module != USER_MODULE) {
        ModuleDefault(Settings->module);
      }
    }
    else if (255 == XdrvMailbox.payload) {      // Copy current module with user configured GPIO
      if (Settings->module != USER_MODULE) {
        ModuleDefault(Settings->module);
      }
      SettingsUpdateText(SET_TEMPLATE_NAME, PSTR("Merged"));
      uint32_t j = 0;
      for (uint32_t i = 0; i < nitems(Settings->user_template.gp.io); i++) {
#ifdef ESP8266
        if (6 == i) { j = 9; }
        if (8 == i) { j = 12; }
#endif  // ESP8266
#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6
        // No change
#elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
//        if (22 == i) { j = 33; }  // TODO 20230821 verify
#else  // ESP32
//        if (28 == i) { j = 32; }  // TODO 20230821 verify
#endif  // Non plain ESP32
#endif  // ESP32
        if (TasmotaGlobal.my_module.io[j] > GPIO_NONE) {
          Settings->user_template.gp.io[i] = TasmotaGlobal.my_module.io[j];
        }
        j++;
      }
    }
  }
  else {
#ifndef FIRMWARE_MINIMAL      // if tasmota-minimal, `Template` is read-only
    if (JsonTemplate(XdrvMailbox.data)) {
      if (USER_MODULE == Settings->module) { TasmotaGlobal.restart_flag = 2; }
    } else {
      ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON));
      error = true;
    }
#endif // FIRMWARE_MINIMAL
  }
  if (!error) { TemplateJson(); }
}

void CmndButtonDebounce(void)
{
  if ((XdrvMailbox.payload > 39) && (XdrvMailbox.payload < 1001)) {
    Settings->button_debounce = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->button_debounce);
}

void CmndSwitchDebounce(void)
{
  if ((XdrvMailbox.payload > 39) && (XdrvMailbox.payload < 1010)) {
    Settings->switch_debounce = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->switch_debounce);
}

void CmndBaudrate(void)
{
  if (XdrvMailbox.payload >= 300) {
    XdrvMailbox.payload /= 300;  // Make it a valid baudrate
    TasmotaGlobal.baudrate = (XdrvMailbox.payload & 0xFFFF) * 300;
    SetSerialBaudrate(TasmotaGlobal.baudrate);
  }
  ResponseCmndNumber(TasmotaGlobal.baudrate);
}

void CmndSerialConfig(void)
{
  // See TasmotaSerialConfig for possible options
  // SerialConfig 0..23 where 3 equals 8N1
  // SerialConfig 8N1

  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.data_len < 3) {                    // Use 0..23 as serial config option
      if ((XdrvMailbox.payload >= TS_SERIAL_5N1) && (XdrvMailbox.payload <= TS_SERIAL_8O2)) {
        SetSerialConfig(XdrvMailbox.payload);
      }
    }
    else if ((XdrvMailbox.payload >= 5) && (XdrvMailbox.payload <= 8)) {
      int8_t serial_config = ParseSerialConfig(XdrvMailbox.data);
      if (serial_config >= 0) {
        SetSerialConfig(serial_config);
      }
    }
  }
  ResponseCmndChar(GetSerialConfig().c_str());
}

void CmndSerialBuffer(void) {
  // Allow non-pesistent serial receive buffer size change
  //   between MIN_INPUT_BUFFER_SIZE and MAX_INPUT_BUFFER_SIZE characters
  size_t size = 0;
  if (XdrvMailbox.data_len > 0) {
    size = XdrvMailbox.payload;
    if (1 == XdrvMailbox.payload) {
      size = INPUT_BUFFER_SIZE;
    }
    else if (XdrvMailbox.payload < MIN_INPUT_BUFFER_SIZE) {
      size = MIN_INPUT_BUFFER_SIZE;
    }
    else if (XdrvMailbox.payload > MAX_INPUT_BUFFER_SIZE) {
      size = MAX_INPUT_BUFFER_SIZE;
    }
    Serial.setRxBufferSize(size);
  }
#ifdef ESP8266
  ResponseCmndNumber(Serial.getRxBufferSize());
#endif
#ifdef ESP32
  if (size) {
    ResponseCmndNumber(size);
  } else {
    ResponseCmndDone();
  }
#endif
}

void CmndSerialSend(void) {
  if (XdrvMailbox.index > 9) { XdrvMailbox.index -= 10; }
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 6)) {
    SetSeriallog(LOG_LEVEL_NONE);
    Settings->flag.mqtt_serial = 1;                                  // CMND_SERIALSEND and CMND_SERIALLOG
    Settings->flag.mqtt_serial_raw = (XdrvMailbox.index > 3) ? 1 : 0;  // CMND_SERIALSEND3
    if (XdrvMailbox.data_len > 0) {
      if (1 == XdrvMailbox.index) {
        Serial.printf("%s\n", XdrvMailbox.data);                    // "Hello Tiger\n"
      }
      else if (2 == XdrvMailbox.index || 4 == XdrvMailbox.index) {
        for (uint32_t i = 0; i < XdrvMailbox.data_len; i++) {
          Serial.write(XdrvMailbox.data[i]);                        // "Hello Tiger" or "A0"
        }
      }
      else if (3 == XdrvMailbox.index) {
        uint32_t dat_len = XdrvMailbox.data_len;
        Serial.printf("%s", Unescape(XdrvMailbox.data, &dat_len));  // "Hello\f"
      }
      else if (5 == XdrvMailbox.index) {
        SerialSendRaw(RemoveSpace(XdrvMailbox.data));               // "AA004566" as hex values
      }
      else if (6 == XdrvMailbox.index) {
        SerialSendDecimal(XdrvMailbox.data);
      }
      ResponseCmndDone();
    }
  }
}

void CmndSerialDelimiter(void)
{
  if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.payload < 256)) {
    if (XdrvMailbox.payload > 0) {
      Settings->serial_delimiter = XdrvMailbox.payload;
    } else {
      uint32_t dat_len = XdrvMailbox.data_len;
      Unescape(XdrvMailbox.data, &dat_len);
      Settings->serial_delimiter = XdrvMailbox.data[0];
    }
  }
  ResponseCmndNumber(Settings->serial_delimiter);
}

void CmndSyslog(void)
{
  if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {
    SetSyslog(XdrvMailbox.payload);
  }
  Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, XdrvMailbox.command, Settings->syslog_level, TasmotaGlobal.syslog_level);
}

void CmndLoghost(void)
{
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_SYSLOG_HOST, (SC_DEFAULT == Shortcut()) ? SYS_LOG_HOST : XdrvMailbox.data);
  }
  ResponseCmndChar(SettingsText(SET_SYSLOG_HOST));
}

void CmndLogport(void)
{
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 65536)) {
    Settings->syslog_port = (1 == XdrvMailbox.payload) ? SYS_LOG_PORT : XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->syslog_port);
}

void CmndIpAddress(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 5)) {
    char network_address[22];
    ext_snprintf_P(network_address, sizeof(network_address), PSTR(" (%_I)"), (uint32_t)WiFi.localIP());
    if (!XdrvMailbox.usridx) {
      ResponseClear();
      for (uint32_t i = 0; i < 5; i++) {
        ResponseAppend_P(PSTR("%c\"%s%d\":\"%_I%s\""), (i)?',':'{', XdrvMailbox.command, i +1, Settings->ipv4_address[i], (0 == i)?network_address:"");
      }
      ResponseJsonEnd();
    } else {
      uint32_t ipv4_address;
      if (ParseIPv4(&ipv4_address, XdrvMailbox.data)) {
        Settings->ipv4_address[XdrvMailbox.index -1] = ipv4_address;
      }
      Response_P(PSTR("{\"%s%d\":\"%_I%s\"}"), XdrvMailbox.command, XdrvMailbox.index, Settings->ipv4_address[XdrvMailbox.index -1], (1 == XdrvMailbox.index)?network_address:"");
    }
  }
}

void CmndNtpServer(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_NTP_SERVERS)) {
    if (!XdrvMailbox.usridx) {
      ResponseCmndAll(SET_NTPSERVER1, MAX_NTP_SERVERS);
    } else {
      uint32_t ntp_server = SET_NTPSERVER1 + XdrvMailbox.index -1;
      if (XdrvMailbox.data_len > 0) {
        SettingsUpdateText(ntp_server,
          (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? (1 == XdrvMailbox.index) ? PSTR(NTP_SERVER1) : (2 == XdrvMailbox.index) ? PSTR(NTP_SERVER2) : PSTR(NTP_SERVER3) : XdrvMailbox.data);
        SettingsUpdateText(ntp_server, ReplaceCommaWithDot(SettingsText(ntp_server)));
  //        TasmotaGlobal.restart_flag = 2;  // Issue #3890
        TasmotaGlobal.ntp_force_sync = true;
      }
      ResponseCmndIdxChar(SettingsText(ntp_server));
    }
  }
}

void CmndAp(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
    switch (XdrvMailbox.payload) {
    case 0:  // Toggle
      Settings->sta_active ^= 1;
      break;
    case 1:  // AP1
    case 2:  // AP2
      Settings->sta_active = XdrvMailbox.payload -1;
    }
    Settings->wifi_channel = 0;  // Disable stored AP
    TasmotaGlobal.restart_flag = 2;
  }
  Response_P(S_JSON_COMMAND_NVALUE_SVALUE, XdrvMailbox.command, Settings->sta_active +1, EscapeJSONString(SettingsText(SET_STASSID1 + Settings->sta_active)).c_str());
}

void CmndSsid(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_SSIDS)) {
    if (!XdrvMailbox.usridx) {
      ResponseCmndAll(SET_STASSID1, MAX_SSIDS);
    } else {
      if (XdrvMailbox.data_len > 0) {
        SettingsUpdateText(SET_STASSID1 + XdrvMailbox.index -1,
                (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? (1 == XdrvMailbox.index) ? STA_SSID1 : STA_SSID2 : XdrvMailbox.data);
        Settings->sta_active = XdrvMailbox.index -1;
        TasmotaGlobal.restart_flag = 2;
      }
      ResponseCmndIdxChar(SettingsText(SET_STASSID1 + XdrvMailbox.index -1));
    }
  }
}

void CmndPassword(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 4)) {
    bool show_asterisk = (XdrvMailbox.index > 2);
    if (show_asterisk) {
      XdrvMailbox.index -= 2;
    }
    if ((XdrvMailbox.data_len > 4) || (SC_CLEAR == Shortcut()) || (SC_DEFAULT == Shortcut())) {
      SettingsUpdateText(SET_STAPWD1 + XdrvMailbox.index -1,
              (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? (1 == XdrvMailbox.index) ? STA_PASS1 : STA_PASS2 : XdrvMailbox.data);
      Settings->sta_active = XdrvMailbox.index -1;
      TasmotaGlobal.restart_flag = 2;
      if (!show_asterisk) {
        ResponseCmndIdxChar(SettingsText(SET_STAPWD1 + XdrvMailbox.index -1));
      }
    } else {
      show_asterisk = true;
    }
    if (show_asterisk) {
      Response_P(S_JSON_COMMAND_INDEX_ASTERISK, XdrvMailbox.command, XdrvMailbox.index);
    }
  }
}

void CmndHostname(void)
{
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    SettingsUpdateText(SET_HOSTNAME, (SC_DEFAULT == Shortcut()) ? WIFI_HOSTNAME : XdrvMailbox.data);
    if (strchr(SettingsText(SET_HOSTNAME), '%') != nullptr) {
      SettingsUpdateText(SET_HOSTNAME, WIFI_HOSTNAME);
    }
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndChar(SettingsText(SET_HOSTNAME));
}

void CmndWifiConfig(void)
{
  if ((XdrvMailbox.payload >= WIFI_RESTART) && (XdrvMailbox.payload < MAX_WIFI_OPTION)) {
    if ((EX_WIFI_SMARTCONFIG == XdrvMailbox.payload) || (EX_WIFI_WPSCONFIG == XdrvMailbox.payload)) {
      XdrvMailbox.payload = WIFI_MANAGER;
    }
    Settings->sta_config = XdrvMailbox.payload;
    TasmotaGlobal.wifi_state_flag = Settings->sta_config;
    if (WifiState() > WIFI_RESTART) {
      TasmotaGlobal.restart_flag = 2;
    }
  }
  char stemp1[TOPSZ];
  Response_P(S_JSON_COMMAND_NVALUE_SVALUE, XdrvMailbox.command, Settings->sta_config, GetTextIndexed(stemp1, sizeof(stemp1), Settings->sta_config, kWifiConfig));
}

void CmndDevicename(void)
{
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    SettingsUpdateText(SET_DEVICENAME, ('"' == XdrvMailbox.data[0]) ? "" : (SC_DEFAULT == Shortcut()) ? SettingsText(SET_FRIENDLYNAME1) : XdrvMailbox.data);
  }
  ResponseCmndChar(SettingsText(SET_DEVICENAME));
}

void CmndFriendlyname(void)
{
  snprintf_P(XdrvMailbox.command, CMDSZ, PSTR(D_CMND_FRIENDLYNAME));  // Rename result shortcut command FN to FriendlyName
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_FRIENDLYNAMES)) {
    if (!XdrvMailbox.usridx && !XdrvMailbox.data_len) {
      ResponseCmndAll(SET_FRIENDLYNAME1, MAX_FRIENDLYNAMES);
    } else {
      if (XdrvMailbox.data_len > 0) {
        char stemp1[TOPSZ];
        if (1 == XdrvMailbox.index) {
          snprintf_P(stemp1, sizeof(stemp1), PSTR(FRIENDLY_NAME));
        } else {
          snprintf_P(stemp1, sizeof(stemp1), PSTR(FRIENDLY_NAME "%d"), XdrvMailbox.index);
        }
        SettingsUpdateText(SET_FRIENDLYNAME1 + XdrvMailbox.index -1, ('"' == XdrvMailbox.data[0]) ? "" : (SC_DEFAULT == Shortcut()) ? stemp1 : XdrvMailbox.data);
      }
      ResponseCmndIdxChar(SettingsText(SET_FRIENDLYNAME1 + XdrvMailbox.index -1));
    }
  }
}

void CmndSwitchText(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_SWITCHES_TXT)) {
    if (!XdrvMailbox.usridx && !XdrvMailbox.data_len) {
      ResponseCmndAll(SET_SWITCH_TXT1, MAX_SWITCHES_TXT);
    } else {
      if (XdrvMailbox.data_len > 0) {
        RemoveSpace(XdrvMailbox.data);
        SettingsUpdateText(SET_SWITCH_TXT1 + XdrvMailbox.index -1, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data);
      }
      ResponseCmndIdxChar(SettingsText(SET_SWITCH_TXT1 + XdrvMailbox.index -1));
    }
  }
}

void CmndSwitchMode(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_SWITCHES_SET)) {
    // SwitchMode1   - Show SwitchMode1
    // SwitchMode1 2 - Set SwitchMode tot 2
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < MAX_SWITCH_OPTION)) {
      Settings->switchmode[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Settings->switchmode[XdrvMailbox.index-1]);
  }
  else if (0 == XdrvMailbox.index) {
    // SwitchMode0   - Show all SwitchMode like {"SwitchMode":[2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]}
    // SwitchMode0 2 - Set all SwitchMode to 2
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < MAX_SWITCH_OPTION)) {
      for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
        Settings->switchmode[i] = XdrvMailbox.payload;
      }
    }
    Response_P(PSTR("{\"%s\":["), XdrvMailbox.command);
    for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
      ResponseAppend_P(PSTR("%s%d"), (i>0)?",":"", Settings->switchmode[i]);
    }
    ResponseAppend_P(PSTR("]}"));
  }
}

void CmndInterlock(void)
{
  // Interlock 0 - Off, Interlock 1 - On, Interlock 1,2 3,4 5,6,7
  uint32_t max_relays = TasmotaGlobal.devices_present;
  if (TasmotaGlobal.light_type) { max_relays--; }
  if (max_relays > sizeof(Settings->interlock[0]) * 8) { max_relays = sizeof(Settings->interlock[0]) * 8; }
  if (max_relays > 1) {                                         // Only interlock with more than 1 relay
    if (XdrvMailbox.data_len > 0) {
      if (strchr(XdrvMailbox.data, ',') != nullptr) {           // Interlock entry
        for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) { Settings->interlock[i] = 0; }  // Reset current interlocks
        char *group;
        char *q;
        uint32_t group_index = 0;
        power_t relay_mask = 0;
        for (group = strtok_r(XdrvMailbox.data, " ", &q); group && group_index < MAX_INTERLOCKS; group = strtok_r(nullptr, " ", &q)) {
          char *str;
          char *p;
          for (str = strtok_r(group, ",", &p); str; str = strtok_r(nullptr, ",", &p)) {
            int pbit = atoi(str);
            if ((pbit > 0) && (pbit <= max_relays)) {           // Only valid relays
              pbit--;
              if (!bitRead(relay_mask, pbit)) {                 // Only relay once
                bitSet(relay_mask, pbit);
                bitSet(Settings->interlock[group_index], pbit);
              }
            }
          }
          group_index++;
        }
        for (uint32_t i = 0; i < group_index; i++) {
          uint32_t minimal_bits = 0;
          for (uint32_t j = 0; j < max_relays; j++) {
            if (bitRead(Settings->interlock[i], j)) { minimal_bits++; }
          }
          if (minimal_bits < 2) { Settings->interlock[i] = 0; }  // Discard single relay as interlock
        }
      } else {
        Settings->flag.interlock = XdrvMailbox.payload &1;       // CMND_INTERLOCK - Enable/disable interlock
        if (Settings->flag.interlock) {
          SetDevicePower(TasmotaGlobal.power, SRC_IGNORE);                    // Remove multiple relays if set
        }
      }
#ifdef USE_SHUTTER
      if (Settings->flag3.shutter_mode) {  // SetOption80 - Enable shutter support
        ShutterInit(); // to update shutter mode
      }
#endif  // USE_SHUTTER
    }
    Response_P(PSTR("{\"" D_CMND_INTERLOCK "\":\"%s\",\"" D_JSON_GROUPS "\":\""), GetStateText(Settings->flag.interlock));
    uint32_t anygroup = 0;
    for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
      if (Settings->interlock[i]) {
        anygroup++;
        ResponseAppend_P(PSTR("%s"), (anygroup > 1) ? " " : "");
        uint32_t anybit = 0;
        power_t mask = 1;
        for (uint32_t j = 0; j < max_relays; j++) {
          if (Settings->interlock[i] & mask) {
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
    // never ever reset interlock mode inadvertently if we forced it upon compilation
    Settings->flag.interlock = APP_INTERLOCK_MODE;               // CMND_INTERLOCK - Enable/disable interlock
    ResponseCmndStateText(Settings->flag.interlock);
  }
}

void CmndTeleperiod(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->tele_period = (1 == XdrvMailbox.payload) ? TELE_PERIOD : XdrvMailbox.payload;
    if ((Settings->tele_period > 0) && (Settings->tele_period < 10)) {
      Settings->tele_period = 10;   // Do not allow periods < 10 seconds
    }
  }
  TasmotaGlobal.tele_period = (Settings->tele_period) ? Settings->tele_period : 3601;  // Show teleperiod data also on empty command
  ResponseCmndNumber(Settings->tele_period);
}

void CmndReset(void)
{
  switch (XdrvMailbox.payload) {
  case 1:
    TasmotaGlobal.restart_flag = 211;
    ResponseCmndChar(PSTR(D_JSON_RESET_AND_RESTARTING));
    break;
  case 2 ... 6:
    TasmotaGlobal.restart_flag = 210 + XdrvMailbox.payload;
    Response_P(PSTR("{\"" D_CMND_RESET "\":\"" D_JSON_ERASE ", " D_JSON_RESET_AND_RESTARTING "\"}"));
    break;
  case 99:
    Settings->bootcount = 0;
    Settings->bootcount_reset_time = 0;
    ResponseCmndDone();
    break;
  default:
    ResponseCmndChar(PSTR(D_JSON_ONE_TO_RESET));
  }
}

void CmndTime(void)
{
// payload 0 = (re-)enable NTP
// payload 1 = Time format {"Time":"2019-09-04T14:31:29"}
// payload 2 = Time format {"Time":"2019-09-04T14:31:29","Epoch":1567600289}
// payload 3 = Time format {"Time":1567600289}
// payload 4 = Time format {"Time":"2019-09-04T14:31:29.123"}
// payload 1451602800 - disable NTP and set time to epoch

  uint32_t format = Settings->flag2.time_format;
  if (XdrvMailbox.data_len > 0) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 5)) {
      Settings->flag2.time_format = XdrvMailbox.payload -1;
      format = Settings->flag2.time_format;
    } else {
      format = 1;  // {"Time":"2019-09-04T14:31:29","Epoch":1567600289}
      RtcSetTime(XdrvMailbox.payload);
    }
  }
  ResponseClear();
  ResponseAppendTimeFormat(format);
  ResponseJsonEnd();
}

void CmndTimezone(void)
{
  if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.payload >= -13)) {
    Settings->timezone = XdrvMailbox.payload;
    Settings->timezone_minutes = 0;
    if (XdrvMailbox.payload < 15) {
      char *p = strtok (XdrvMailbox.data, ":");
      if (p) {
        p = strtok (nullptr, ":");
        if (p) {
          Settings->timezone_minutes = strtol(p, nullptr, 10);
          if (Settings->timezone_minutes > 59) { Settings->timezone_minutes = 59; }
        }
      }
    } else {
      Settings->timezone = 99;
    }
    TasmotaGlobal.ntp_force_sync = true;
  }
  if (99 == Settings->timezone) {
    ResponseCmndNumber(Settings->timezone);
  } else {
    char stemp1[TOPSZ];
    snprintf_P(stemp1, sizeof(stemp1), PSTR("%+03d:%02d"), Settings->timezone, Settings->timezone_minutes);
    ResponseCmndChar(stemp1);
  }
}

void CmndTimeStdDst(uint32_t ts)
{
  // TimeStd 0/1, 0/1/2/3/4, 1..12, 1..7, 0..23, +/-780
  if (XdrvMailbox.data_len > 0) {
    if (strchr(XdrvMailbox.data, ',') != nullptr) {   // Process parameter entry
      uint32_t tpos = 0;                      // Parameter index
      int value = 0;
      char *p = XdrvMailbox.data;                           // Parameters like "1, 2,3 , 4 ,5, -120" or ",,,,,+240"
      char *q = p;                           // Value entered flag
      while (p && (tpos < 7)) {
        if (p > q) {                         // Any value entered
          if (1 == tpos) { Settings->tflag[ts].hemis = value &1; }
          if (2 == tpos) { Settings->tflag[ts].week = (value < 0) ? 0 : (value > 4) ? 4 : value; }
          if (3 == tpos) { Settings->tflag[ts].month = (value < 1) ? 1 : (value > 12) ? 12 : value; }
          if (4 == tpos) { Settings->tflag[ts].dow = (value < 1) ? 1 : (value > 7) ? 7 : value; }
          if (5 == tpos) { Settings->tflag[ts].hour = (value < 0) ? 0 : (value > 23) ? 23 : value; }
          if (6 == tpos) { Settings->toffset[ts] = (value < -900) ? -900 : (value > 900) ? 900 : value; }
        }
        p = Trim(p);                        // Skip spaces
        if (tpos && (*p == ',')) { p++; }    // Skip separator
        p = Trim(p);                        // Skip spaces
        q = p;                               // Reset any value entered flag
        value = strtol(p, &p, 10);
        tpos++;                              // Next parameter
      }
      TasmotaGlobal.ntp_force_sync = true;
    } else {
      if (0 == XdrvMailbox.payload) {
        if (0 == ts) {
          SettingsResetStd();
        } else {
          SettingsResetDst();
        }
      }
      TasmotaGlobal.ntp_force_sync = true;
    }
  }
  Response_P(PSTR("{\"%s\":{\"Hemisphere\":%d,\"Week\":%d,\"Month\":%d,\"Day\":%d,\"Hour\":%d,\"Offset\":%d}}"),
    XdrvMailbox.command, Settings->tflag[ts].hemis, Settings->tflag[ts].week, Settings->tflag[ts].month, Settings->tflag[ts].dow, Settings->tflag[ts].hour, Settings->toffset[ts]);
}

void CmndTimeStd(void)
{
  CmndTimeStdDst(0);
}

void CmndTimeDst(void)
{
  CmndTimeStdDst(1);
}

void CmndAltitude(void)
{
  if ((XdrvMailbox.data_len > 0) && ((XdrvMailbox.payload >= -30000) && (XdrvMailbox.payload <= 30000))) {
    Settings->altitude = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->altitude);
}

void CmndLedPower(void) {
  // If GPIO_LEDLINK (used for network status) then allow up to 4 GPIO_LEDx control using TasmotaGlobal.led_power
  // If no GPIO_LEDLINK then allow legacy single led GPIO_LED1 control using Settings->ledstate
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_LEDS)) {
    if (!PinUsed(GPIO_LEDLNK)) { XdrvMailbox.index = 1; }
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
      Settings->ledstate &= 8;                // Disable power control
      uint32_t mask = 1 << (XdrvMailbox.index -1);        // Led to control
      switch (XdrvMailbox.payload) {
      case 0: // Off
        TasmotaGlobal.led_power &= (0xFF ^ mask);
        Settings->ledstate = 0;
        break;
      case 1: // On
        TasmotaGlobal.led_power |= mask;
        Settings->ledstate = 8;
        break;
      case 2: // Toggle
        TasmotaGlobal.led_power ^= mask;
        Settings->ledstate ^= 8;
        break;
      }
      TasmotaGlobal.blinks = 0;
      if (!PinUsed(GPIO_LEDLNK)) {
        SetLedPower(Settings->ledstate &8);
      } else {
        SetLedPowerIdx(XdrvMailbox.index -1, (TasmotaGlobal.led_power & mask));
      }
    }
    bool state = bitRead(TasmotaGlobal.led_power, XdrvMailbox.index -1);
    if (!PinUsed(GPIO_LEDLNK)) {
      state = bitRead(Settings->ledstate, 3);
    }
    ResponseCmndIdxChar(GetStateText(state));
  }
}

void CmndLedState(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < MAX_LED_OPTION)) {
    Settings->ledstate = XdrvMailbox.payload;
    if (!Settings->ledstate) {
      SetLedPowerAll(0);
      SetLedLink(0);
    }
  }
  ResponseCmndNumber(Settings->ledstate);
}

void CmndLedMask(void) {
  if (XdrvMailbox.data_len > 0) {
#ifdef USE_PWM_DIMMER
    PWMDimmerSetBrightnessLeds(0);
#endif  // USE_PWM_DIMMER
    Settings->ledmask = XdrvMailbox.payload;
#ifdef USE_PWM_DIMMER
    PWMDimmerSetBrightnessLeds(-1);
#endif  // USE_PWM_DIMMER
  }
  char stemp1[TOPSZ];
  snprintf_P(stemp1, sizeof(stemp1), PSTR("%d (0x%04X)"), Settings->ledmask, Settings->ledmask);
  ResponseCmndChar(stemp1);
}

void CmndLedPwmOff(void) {
  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.payload < 0) {
      Settings->ledpwm_off = 0;
    }
    else if (XdrvMailbox.payload > 255) {
      Settings->ledpwm_off = 255;
    } else {
      Settings->ledpwm_off = XdrvMailbox.payload;
    }
    UpdateLedPowerAll();
  }
  ResponseCmndNumber(Settings->ledpwm_off);
}

void CmndLedPwmOn(void) {
  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.payload < 0) {
      Settings->ledpwm_on = 0;
    }
    else if (XdrvMailbox.payload > 255) {
      Settings->ledpwm_on = 255;
    } else {
      Settings->ledpwm_on = XdrvMailbox.payload;
    }
    UpdateLedPowerAll();
  }
  ResponseCmndNumber(Settings->ledpwm_on);
}

void CmndLedPwmMode(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_LEDS)) {
    if (!PinUsed(GPIO_LEDLNK)) { XdrvMailbox.index = 1; }
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
      uint32_t mask = 1 << (XdrvMailbox.index -1);        // Led to configure
      switch (XdrvMailbox.payload) {
      case 0: // digital
        Settings->ledpwm_mask &= (0xFF ^ mask);
        break;
      case 1: // pwm
        Settings->ledpwm_mask |= mask;
        break;
      case 2: // toggle
        Settings->ledpwm_mask ^= mask;
        break;
      }
      UpdateLedPowerAll();
    }
    bool state = bitRead(Settings->ledpwm_mask, XdrvMailbox.index -1);
    ResponseCmndIdxChar(GetStateText(state));
  }
}

void CmndWifiPower(void) {
  if (XdrvMailbox.data_len > 0) {
    Settings->wifi_output_power = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if (10 == Settings->wifi_output_power) {
      // WifiPower 1
      Settings->wifi_output_power = MAX_TX_PWR_DBM_54g;
    }
    else if (Settings->wifi_output_power > MAX_TX_PWR_DBM_11b) {
      Settings->wifi_output_power = MAX_TX_PWR_DBM_11b;
    }
    WifiSetOutputPower();
  }
  ResponseCmndFloat(WifiGetOutputPower(), 1);
}

void CmndWifi(void) {
  // Wifi    - Show current state
  // Wifi 0  - Off
  // Wifi 1  - On
  // Wifi 2  - B
  // Wifi 3  - BG
  // Wifi 4  - BGN
  // Wifi 6  - BGNAX
  uint32_t option = XdrvMailbox.payload -1;
  switch (XdrvMailbox.payload) {
    case 0:  // Wifi 0 = Off
    case 1:  // Wifi 1 = On
      {
        Settings->flag4.network_wifi = XdrvMailbox.payload;
        if (Settings->flag4.network_wifi) {
#ifdef ESP32
          WifiConnect();
#else   // ESP8266
          WifiEnable();
#endif  // ESP32
        }
        break;
      }
#ifdef ESP32
    case 6:  // Wifi 6 = BGNAX
      option = 4;
#endif  // ESP32
    case 4:  // Wifi 4 = BGN
    case 3:  // Wifi 3 = BG
    case 2:  // Wifi 2 = B
      {
#ifdef ESP32
        Wifi.phy_mode = option;
#endif  // ESP32
        WiFiHelper::setPhyMode(WiFiPhyMode_t(option));  // 1=B/2=BG/3=BGN/4=BGNAX
        break;
      }
  }
  Response_P(PSTR("{\"" D_JSON_WIFI "\":\"%s\",\"" D_JSON_WIFI_MODE "\":\"%s\"}"), 
    GetStateText(Settings->flag4.network_wifi), WifiGetPhyMode().c_str());
}

void CmndDnsTimeout(void) {
  // Set timeout between 100 and 20000 mSec
  if ((XdrvMailbox.payload >= 100) && (XdrvMailbox.payload <= 20000)) {
    Settings->dns_timeout = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->dns_timeout);
}

void UpdateBatteryPercent(int batt_percentage) {
  if (batt_percentage > 101) { batt_percentage = 100; }
  if (batt_percentage >= 0) {
    Settings->battery_level_percent = batt_percentage;
  }
}

void CmndBatteryPercent(void) {
  UpdateBatteryPercent(XdrvMailbox.payload);
  ResponseCmndNumber(Settings->battery_level_percent);
}

#ifdef USE_I2C
void CmndI2cScan(void) {
  // I2CScan0  - Scan bus1 and bus2
  // I2CScan   - Scan bus1
  // I2CScan2  - Scan bus2
  if (TasmotaGlobal.i2c_enabled) {
    if ((0 == XdrvMailbox.index) || (1 == XdrvMailbox.index)) {
      I2cScan();
    }
  }
#ifdef ESP32
  if (TasmotaGlobal.i2c_enabled_2) {
    if ((0 == XdrvMailbox.index) || (2 == XdrvMailbox.index)) {
      I2cScan(1);
    }
  }
#endif
}

void CmndI2cDriver(void)
{
  if (XdrvMailbox.index < MAX_I2C_DRIVERS) {
    if (XdrvMailbox.payload >= 0) {
      bitWrite(Settings->i2c_drivers[XdrvMailbox.index / 32], XdrvMailbox.index % 32, XdrvMailbox.payload &1);
      TasmotaGlobal.restart_flag = 2;
    }
  }
  Response_P(PSTR("{\"" D_CMND_I2CDRIVER "\":"));
  I2cDriverState();
  ResponseJsonEnd();
}
#endif  // USE_I2C

#ifdef USE_DEVICE_GROUPS
void CmndDevGroupName(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_DEV_GROUP_NAMES)) {
    if (XdrvMailbox.data_len > 0) {
      if (XdrvMailbox.data_len > TOPSZ)
        XdrvMailbox.data[TOPSZ - 1] = 0;
      else if (1 == XdrvMailbox.data_len && ('"' == XdrvMailbox.data[0] || '0' == XdrvMailbox.data[0]))
        XdrvMailbox.data[0] = 0;
      SettingsUpdateText(SET_DEV_GROUP_NAME1 + XdrvMailbox.index - 1, XdrvMailbox.data);
      TasmotaGlobal.restart_flag = 2;
    }
    ResponseCmndAll(SET_DEV_GROUP_NAME1, MAX_DEV_GROUP_NAMES);
  }
}

#ifdef USE_DEVICE_GROUPS_SEND
void CmndDevGroupSend(void)
{
  uint8_t device_group_index = (XdrvMailbox.usridx ? XdrvMailbox.index - 1 : 0);
  if (device_group_index < device_group_count) {
    if (!_SendDeviceGroupMessage(-device_group_index, (DevGroupMessageType)(DGR_MSGTYPE_UPDATE_COMMAND + DGR_MSGTYPFLAG_WITH_LOCAL))) {
      ResponseCmndChar(XdrvMailbox.data);
    }
  }
}
#endif  // USE_DEVICE_GROUPS_SEND

void CmndDevGroupShare(void)
{
  uint32_t parm[2] = { Settings->device_group_share_in, Settings->device_group_share_out };
  ParseParameters(2, parm);
  Settings->device_group_share_in = parm[0];
  Settings->device_group_share_out = parm[1];
  Response_P(PSTR("{\"" D_CMND_DEVGROUP_SHARE "\":{\"In\":\"%X\",\"Out\":\"%X\"}}"), Settings->device_group_share_in, Settings->device_group_share_out);
}

void CmndDevGroupStatus(void)
{
  DeviceGroupStatus((XdrvMailbox.usridx ? XdrvMailbox.index - 1 : 0));
}

void CmndDevGroupTie(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_DEV_GROUP_NAMES)) {
    if (XdrvMailbox.data_len > 0) {
      Settings->device_group_tie[XdrvMailbox.index - 1] = XdrvMailbox.payload;
    }
    Response_P(PSTR("{"));
    for (uint32_t i = 0; i < MAX_DEV_GROUP_NAMES; i++) {
      ResponseAppend_P(PSTR("%s\"%s%u\":%u"), (i)?",":"", D_CMND_DEVGROUP_TIE, i + 1, Settings->device_group_tie[i]);
    }
    ResponseJsonEnd();
  }
}
#endif  // USE_DEVICE_GROUPS

void CmndSetSensor(void)
{
  if (XdrvMailbox.index < MAX_XSNS_DRIVERS) {
    if (XdrvMailbox.payload >= 0) {
      bitWrite(Settings->sensors[0][XdrvMailbox.index / 32], XdrvMailbox.index % 32, XdrvMailbox.payload &1);
      if (1 == XdrvMailbox.payload) {
        TasmotaGlobal.restart_flag = 2;  // To safely re-enable a sensor currently most sensor need to follow complete restart init cycle
      }
    }
    Response_P(PSTR("{\"" D_CMND_SETSENSOR "\":"));
    XsnsSensorState(0);
    ResponseJsonEnd();
  }
}

void CmndSensor(void)
{
  XsnsCall(FUNC_COMMAND_SENSOR);
}

void CmndDriver(void)
{
  XdrvCall(FUNC_COMMAND_DRIVER);
}

#ifdef ESP32

void CmndInfo(void) {
  NvsInfo();
  ResponseCmndDone();
}

void CmndCpuFrequency(void) {
  if ((80 == XdrvMailbox.payload) || (160 == XdrvMailbox.payload) || (240 == XdrvMailbox.payload)) {
    setCpuFrequencyMhz(XdrvMailbox.payload);
  }
  ResponseCmndNumber(getCpuFrequencyMhz());
}

#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
void CmndTouchCal(void) {
  if (XdrvMailbox.payload >= 0) {
    if (XdrvMailbox.payload == 0) {
      TouchButton.calibration = 0;
    }
    else if (XdrvMailbox.payload < MAX_KEYS + 1) {
      TouchButton.calibration = bitSet(TouchButton.calibration, XdrvMailbox.payload);
    }
    else if (XdrvMailbox.payload == 255) {
      TouchButton.calibration = 0x0FFFFFFF;  // All MAX_KEYS pins
    }
  }
  ResponseCmndNumber(TouchButton.calibration);
  AddLog(LOG_LEVEL_INFO, PSTR("Button Touchvalue Hits,"));
}

void CmndTouchThres(void) {
  if (XdrvMailbox.data_len > 0) {
    Settings->touch_threshold = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->touch_threshold);
}
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2

#endif  // ESP32