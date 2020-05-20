/*
  support_command.ino - command support for Tasmota

  Copyright (C) 2020  Theo Arends

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
  D_CMND_BACKLOG "|" D_CMND_DELAY "|" D_CMND_POWER "|" D_CMND_STATUS "|" D_CMND_STATE "|" D_CMND_SLEEP "|" D_CMND_UPGRADE "|" D_CMND_UPLOAD "|" D_CMND_OTAURL "|"
  D_CMND_SERIALLOG "|" D_CMND_RESTART "|" D_CMND_POWERONSTATE "|" D_CMND_PULSETIME "|" D_CMND_BLINKTIME "|" D_CMND_BLINKCOUNT "|" D_CMND_SAVEDATA "|"
  D_CMND_SO "|" D_CMND_SETOPTION "|" D_CMND_TEMPERATURE_RESOLUTION "|" D_CMND_HUMIDITY_RESOLUTION "|" D_CMND_PRESSURE_RESOLUTION "|" D_CMND_POWER_RESOLUTION "|"
  D_CMND_VOLTAGE_RESOLUTION "|" D_CMND_FREQUENCY_RESOLUTION "|" D_CMND_CURRENT_RESOLUTION "|" D_CMND_ENERGY_RESOLUTION "|" D_CMND_WEIGHT_RESOLUTION "|"
  D_CMND_MODULE "|" D_CMND_MODULES "|" D_CMND_GPIO "|" D_CMND_GPIOS "|" D_CMND_TEMPLATE "|" D_CMND_PWM "|" D_CMND_PWMFREQUENCY "|" D_CMND_PWMRANGE "|"
  D_CMND_BUTTONDEBOUNCE "|" D_CMND_SWITCHDEBOUNCE "|" D_CMND_SYSLOG "|" D_CMND_LOGHOST "|" D_CMND_LOGPORT "|" D_CMND_SERIALSEND "|" D_CMND_BAUDRATE "|" D_CMND_SERIALCONFIG "|"
  D_CMND_SERIALDELIMITER "|" D_CMND_IPADDRESS "|" D_CMND_NTPSERVER "|" D_CMND_AP "|" D_CMND_SSID "|" D_CMND_PASSWORD "|" D_CMND_HOSTNAME "|" D_CMND_WIFICONFIG "|"
  D_CMND_DEVICENAME "|" D_CMND_FRIENDLYNAME "|" D_CMND_SWITCHMODE "|" D_CMND_INTERLOCK "|" D_CMND_TELEPERIOD "|" D_CMND_RESET "|" D_CMND_TIME "|" D_CMND_TIMEZONE "|" D_CMND_TIMESTD "|"
  D_CMND_TIMEDST "|" D_CMND_ALTITUDE "|" D_CMND_LEDPOWER "|" D_CMND_LEDSTATE "|" D_CMND_LEDMASK "|" D_CMND_WIFIPOWER "|" D_CMND_TEMPOFFSET "|" D_CMND_HUMOFFSET "|"
  D_CMND_SPEEDUNIT "|" D_CMND_GLOBAL_TEMP "|" D_CMND_GLOBAL_HUM "|" D_CMND_SETLEDPWMON "|" D_CMND_SETLEDPWMOFF "|" D_CMND_SETLEDPWMMODE "|"
#ifdef USE_I2C
  D_CMND_I2CSCAN "|" D_CMND_I2CDRIVER "|"
#endif
#ifdef USE_DEVICE_GROUPS
  D_CMND_DEVGROUP_NAME "|"
#ifdef USE_DEVICE_GROUPS_SEND
  D_CMND_DEVGROUP_SEND "|"
#endif  // USE_DEVICE_GROUPS_SEND
  D_CMND_DEVGROUP_SHARE "|" D_CMND_DEVGROUPSTATUS "|"
#endif  // USE_DEVICE_GROUPS
  D_CMND_SENSOR "|" D_CMND_DRIVER;

void (* const TasmotaCommand[])(void) PROGMEM = {
  &CmndBacklog, &CmndDelay, &CmndPower, &CmndStatus, &CmndState, &CmndSleep, &CmndUpgrade, &CmndUpgrade, &CmndOtaUrl,
  &CmndSeriallog, &CmndRestart, &CmndPowerOnState, &CmndPulsetime, &CmndBlinktime, &CmndBlinkcount, &CmndSavedata,
  &CmndSetoption, &CmndSetoption, &CmndTemperatureResolution, &CmndHumidityResolution, &CmndPressureResolution, &CmndPowerResolution,
  &CmndVoltageResolution, &CmndFrequencyResolution, &CmndCurrentResolution, &CmndEnergyResolution, &CmndWeightResolution,
  &CmndModule, &CmndModules, &CmndGpio, &CmndGpios, &CmndTemplate, &CmndPwm, &CmndPwmfrequency, &CmndPwmrange,
  &CmndButtonDebounce, &CmndSwitchDebounce, &CmndSyslog, &CmndLoghost, &CmndLogport, &CmndSerialSend, &CmndBaudrate, &CmndSerialConfig,
  &CmndSerialDelimiter, &CmndIpAddress, &CmndNtpServer, &CmndAp, &CmndSsid, &CmndPassword, &CmndHostname, &CmndWifiConfig,
  &CmndDevicename, &CmndFriendlyname, &CmndSwitchMode, &CmndInterlock, &CmndTeleperiod, &CmndReset, &CmndTime, &CmndTimezone, &CmndTimeStd,
  &CmndTimeDst, &CmndAltitude, &CmndLedPower, &CmndLedState, &CmndLedMask, &CmndWifiPower, &CmndTempOffset, &CmndHumOffset,
  &CmndSpeedUnit, &CmndGlobalTemp, &CmndGlobalHum, &CmndSetLedPwmOn, &CmndSetLedPwmOff, &CmndSetLedPwmMode,
#ifdef USE_I2C
  &CmndI2cScan, CmndI2cDriver,
#endif
#ifdef USE_DEVICE_GROUPS
  &CmndDevGroupName,
#ifdef USE_DEVICE_GROUPS_SEND
  &CmndDevGroupSend,
#endif  // USE_DEVICE_GROUPS_SEND
  &CmndDevGroupShare, &CmndDevGroupStatus,
#endif  // USE_DEVICE_GROUPS
  &CmndSensor, &CmndDriver };

const char kWifiConfig[] PROGMEM =
  D_WCFG_0_RESTART "||" D_WCFG_2_WIFIMANAGER "||" D_WCFG_4_RETRY "|" D_WCFG_5_WAIT "|" D_WCFG_6_SERIAL "|" D_WCFG_7_WIFIMANAGER_RESET_ONLY;

/********************************************************************************************/

void ResponseCmndNumber(int value)
{
  Response_P(S_JSON_COMMAND_NVALUE, XdrvMailbox.command, value);
}

void ResponseCmndFloat(float value, uint32_t decimals)
{
  char stemp1[TOPSZ];
  dtostrfd(value, decimals, stemp1);
  Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, stemp1);  // Return float value without quotes
}

void ResponseCmndIdxNumber(int value)
{
  Response_P(S_JSON_COMMAND_INDEX_NVALUE, XdrvMailbox.command, XdrvMailbox.index, value);
}

void ResponseCmndChar_P(const char* value)
{
  Response_P(S_JSON_COMMAND_SVALUE, XdrvMailbox.command, value);
}

void ResponseCmndChar(const char* value)
{
  Response_P(S_JSON_COMMAND_SVALUE, XdrvMailbox.command, value);
}

void ResponseCmndStateText(uint32_t value)
{
  ResponseCmndChar(GetStateText(value));
}

void ResponseCmndDone(void)
{
  ResponseCmndChar(D_JSON_DONE);
}

void ResponseCmndIdxChar(const char* value)
{
  Response_P(S_JSON_COMMAND_INDEX_SVALUE, XdrvMailbox.command, XdrvMailbox.index, value);
}

void ResponseCmndAll(uint32_t text_index, uint32_t count)
{
  uint32_t real_index = text_index;
  mqtt_data[0] = '\0';
  for (uint32_t i = 0; i < count; i++) {
    if ((SET_MQTT_GRP_TOPIC == text_index) && (1 == i)) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
    ResponseAppend_P(PSTR("%c\"%s%d\":\"%s\""), (i) ? ',' : '{', XdrvMailbox.command, i +1, SettingsText(real_index +i));
  }
  ResponseJsonEnd();
}

/********************************************************************************************/

void ExecuteCommand(const char *cmnd, uint32_t source)
{
  // cmnd: "status 0"  = stopic "status" and svalue " 0"
  // cmnd: "var1 =1"   = stopic "var1" and svalue " =1"
  // cmnd: "var1=1"    = stopic "var1" and svalue "=1"
#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("ExecuteCommand"));
#endif
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

void CommandHandler(char* topicBuf, char* dataBuf, uint32_t data_len)
{
#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("CommandHandler"));
#endif

  while (*dataBuf && isspace(*dataBuf)) {
    dataBuf++;                           // Skip leading spaces in data
    data_len--;
  }

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
  GetFallbackTopic_P(stemp1, "");  // Full Fallback topic = cmnd/DVES_xxxxxxxx_fb/
  fallback_topic_flag = (!strncmp(topicBuf, stemp1, strlen(stemp1)));

  char *type = strrchr(topicBuf, '/');   // Last part of received topic is always the command (type)

  uint32_t index = 1;
  bool user_index = false;
  if (type != nullptr) {
    type++;
    uint32_t i;
    int nLen; // strlen(type)
    char *s = type;
    for (nLen = 0; *s; s++, nLen++) {
      *s=toupper(*s);
    }
    i = nLen;
    if (i > 0) { // may be 0
      while (isdigit(type[i-1])) {
        i--;
      }
    }
    if (i < nLen) {
      index = atoi(type + i);
      user_index = true;
    }
    type[i] = '\0';
  }

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("CMD: " D_GROUP " %d, " D_INDEX " %d, " D_COMMAND " \"%s\", " D_DATA " \"%s\""), grpflg, index, type, dataBuf);

  if (type != nullptr) {
    Response_P(PSTR("{\"" D_JSON_COMMAND "\":\"" D_JSON_ERROR "\"}"));

    if (Settings.ledstate &0x02) { blinks++; }

    if (!strcmp(dataBuf,"?")) { data_len = 0; }

    char *p;
    int32_t payload = strtol(dataBuf, &p, 0);  // decimal, octal (0) or hex (0x)
    if (p == dataBuf) { payload = -99; }
    int temp_payload = GetStateNumber(dataBuf);
    if (temp_payload > -1) { payload = temp_payload; }

    DEBUG_CORE_LOG(PSTR("CMD: Payload %d"), payload);

//    backlog_delay = millis() + (100 * MIN_BACKLOG_DELAY);
    backlog_delay = millis() + Settings.param[P_BACKLOG_DELAY];

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
      if (!DecodeCommand(kTasmotaCommands, TasmotaCommand)) {
        if (!XdrvCall(FUNC_COMMAND)) {
          if (!XsnsCall(FUNC_COMMAND)) {
            type = nullptr;  // Unknown command
          }
        }
      }
    }
#else //USE_SCRIPT_SUB_COMMAND
    if (!DecodeCommand(kTasmotaCommands, TasmotaCommand)) {
      if (!XdrvCall(FUNC_COMMAND)) {
        if (!XsnsCall(FUNC_COMMAND)) {
          type = nullptr;  // Unknown command
        }
      }
    }
#endif //USE_SCRIPT_SUB_COMMAND

  }

  if (type == nullptr) {
    blinks = 201;
    snprintf_P(stemp1, sizeof(stemp1), PSTR(D_JSON_COMMAND));
    Response_P(PSTR("{\"" D_JSON_COMMAND "\":\"" D_JSON_UNKNOWN "\"}"));
    type = (char*)stemp1;
  }

  if (mqtt_data[0] != '\0') {
     MqttPublishPrefixTopic_P(RESULT_OR_STAT, type);
     XdrvRulesProcess();
  }
  fallback_topic_flag = false;
}

/********************************************************************************************/

void CmndBacklog(void)
{
  if (XdrvMailbox.data_len) {

#ifdef SUPPORT_IF_STATEMENT
    char *blcommand = strtok(XdrvMailbox.data, ";");
    while ((blcommand != nullptr) && (backlog.size() < MAX_BACKLOG))
#else
    uint32_t bl_pointer = (!backlog_pointer) ? MAX_BACKLOG -1 : backlog_pointer;
    bl_pointer--;
    char *blcommand = strtok(XdrvMailbox.data, ";");
    while ((blcommand != nullptr) && (backlog_index != bl_pointer))
#endif
    {
      while(true) {
        blcommand = Trim(blcommand);
        if (!strncasecmp_P(blcommand, PSTR(D_CMND_BACKLOG), strlen(D_CMND_BACKLOG))) {
          blcommand += strlen(D_CMND_BACKLOG);                                  // Skip unnecessary command Backlog
        } else {
          break;
        }
      }
      if (*blcommand != '\0') {
#ifdef SUPPORT_IF_STATEMENT
        if (backlog.size() < MAX_BACKLOG) {
          backlog.add(blcommand);
        }
#else
        backlog[backlog_index] = String(blcommand);
        backlog_index++;
        if (backlog_index >= MAX_BACKLOG) backlog_index = 0;
#endif
      }
      blcommand = strtok(nullptr, ";");
    }
//    ResponseCmndChar(D_JSON_APPENDED);
    mqtt_data[0] = '\0';
  } else {
    bool blflag = BACKLOG_EMPTY;
#ifdef SUPPORT_IF_STATEMENT
    backlog.clear();
#else
    backlog_pointer = backlog_index;
#endif
    ResponseCmndChar(blflag ? D_JSON_EMPTY : D_JSON_ABORTED);
  }
}

void CmndDelay(void)
{
  if ((XdrvMailbox.payload >= (MIN_BACKLOG_DELAY / 100)) && (XdrvMailbox.payload <= 3600)) {
    backlog_delay = millis() + (100 * XdrvMailbox.payload);
  }
  uint32_t bl_delay = 0;
  long bl_delta = TimePassedSince(backlog_delay);
  if (bl_delta < 0) { bl_delay = (bl_delta *-1) / 100; }
  ResponseCmndNumber(bl_delay);
}

void CmndPower(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= devices_present)) {
    if ((XdrvMailbox.payload < POWER_OFF) || (XdrvMailbox.payload > POWER_BLINK_STOP)) {
      XdrvMailbox.payload = POWER_SHOW_STATE;
    }
//      Settings.flag.device_index_enable = XdrvMailbox.usridx;  // SetOption26 - Switch between POWER or POWER1
    ExecuteCommandPower(XdrvMailbox.index, XdrvMailbox.payload, SRC_IGNORE);
    mqtt_data[0] = '\0';
  }
  else if (0 == XdrvMailbox.index) {
    if ((XdrvMailbox.payload < POWER_OFF) || (XdrvMailbox.payload > POWER_TOGGLE)) {
      XdrvMailbox.payload = POWER_SHOW_STATE;
    }
    SetAllPower(XdrvMailbox.payload, SRC_IGNORE);
    mqtt_data[0] = '\0';
  }
}

void CmndStatus(void)
{
  uint32_t payload = ((XdrvMailbox.payload < 0) || (XdrvMailbox.payload > MAX_STATUS)) ? 99 : XdrvMailbox.payload;

  uint32_t option = STAT;
  char stemp[200];
  char stemp2[TOPSZ];

  // Workaround MQTT - TCP/IP stack queueing when SUB_PREFIX = PUB_PREFIX
  // Commented on 20200118 as it seems to be no longer needed
//  if (!strcmp(SettingsText(SET_MQTTPREFIX1), SettingsText(SET_MQTTPREFIX2)) && (!payload)) { option++; }  // TELE

  if ((!Settings.flag.mqtt_enabled) && (6 == payload)) { payload = 99; }  // SetOption3 - Enable MQTT
  if (!energy_flg && (9 == payload)) { payload = 99; }
  if (!CrashFlag() && (12 == payload)) { payload = 99; }

  if ((0 == payload) || (99 == payload)) {
    uint32_t maxfn = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : (!devices_present) ? 1 : devices_present;
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan()) { maxfn = 1; }
#endif  // USE_SONOFF_IFAN
    stemp[0] = '\0';
    for (uint32_t i = 0; i < maxfn; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%s%s\"%s\"" ), stemp, (i > 0 ? "," : ""), SettingsText(SET_FRIENDLYNAME1 +i));
    }
    stemp2[0] = '\0';
    for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
      snprintf_P(stemp2, sizeof(stemp2), PSTR("%s%s%d" ), stemp2, (i > 0 ? "," : ""), Settings.switchmode[i]);
    }
    Response_P(PSTR("{\"" D_CMND_STATUS "\":{\"" D_CMND_MODULE "\":%d,\"" D_CMND_DEVICENAME "\":\"%s\",\"" D_CMND_FRIENDLYNAME "\":[%s],\"" D_CMND_TOPIC "\":\"%s\",\""
                          D_CMND_BUTTONTOPIC "\":\"%s\",\"" D_CMND_POWER "\":%d,\"" D_CMND_POWERONSTATE "\":%d,\"" D_CMND_LEDSTATE "\":%d,\""
                          D_CMND_LEDMASK "\":\"%04X\",\"" D_CMND_SAVEDATA "\":%d,\"" D_JSON_SAVESTATE "\":%d,\"" D_CMND_SWITCHTOPIC "\":\"%s\",\""
                          D_CMND_SWITCHMODE "\":[%s],\"" D_CMND_BUTTONRETAIN "\":%d,\"" D_CMND_SWITCHRETAIN "\":%d,\"" D_CMND_SENSORRETAIN "\":%d,\"" D_CMND_POWERRETAIN "\":%d}}"),
                          ModuleNr(), SettingsText(SET_DEVICENAME), stemp, mqtt_topic,
                          SettingsText(SET_MQTT_BUTTON_TOPIC), power, Settings.poweronstate, Settings.ledstate,
                          Settings.ledmask, Settings.save_data,
                          Settings.flag.save_state,           // SetOption0 - Save power state and use after restart
                          SettingsText(SET_MQTT_SWITCH_TOPIC),
                          stemp2,
                          Settings.flag.mqtt_button_retain,   // CMND_BUTTONRETAIN
                          Settings.flag.mqtt_switch_retain,   // CMND_SWITCHRETAIN
                          Settings.flag.mqtt_sensor_retain,   // CMND_SENSORRETAIN
                          Settings.flag.mqtt_power_retain);   // CMND_POWERRETAIN
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS));
  }

  if ((0 == payload) || (1 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS1_PARAMETER "\":{\"" D_JSON_BAUDRATE "\":%d,\"" D_CMND_SERIALCONFIG "\":\"%s\",\"" D_CMND_GROUPTOPIC "\":\"%s\",\"" D_CMND_OTAURL "\":\"%s\",\""
                          D_JSON_RESTARTREASON "\":\"%s\",\"" D_JSON_UPTIME "\":\"%s\",\"" D_JSON_STARTUPUTC "\":\"%s\",\"" D_CMND_SLEEP "\":%d,\""
                          D_JSON_CONFIG_HOLDER "\":%d,\"" D_JSON_BOOTCOUNT "\":%d,\"BCResetTime\":\"%s\",\"" D_JSON_SAVECOUNT "\":%d"
#ifdef ESP8266
                          ",\"" D_JSON_SAVEADDRESS "\":\"%X\""
#endif
                          "}}"),
                          Settings.baudrate * 300, GetSerialConfig().c_str(), SettingsText(SET_MQTT_GRP_TOPIC), SettingsText(SET_OTAURL),
                          GetResetReason().c_str(), GetUptime().c_str(), GetDateAndTime(DT_RESTART).c_str(), Settings.sleep,
                          Settings.cfg_holder, Settings.bootcount, GetDateAndTime(DT_BOOTCOUNT).c_str(), Settings.save_flag
#ifdef ESP8266
                          , GetSettingsAddress()
#endif
                          );
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "1"));
  }

  if ((0 == payload) || (2 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS2_FIRMWARE "\":{\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_BUILDDATETIME "\":\"%s\""
#ifdef ESP8266
                          ",\"" D_JSON_BOOTVERSION "\":%d"
#endif
                          ",\"" D_JSON_COREVERSION "\":\"" ARDUINO_CORE_RELEASE "\",\"" D_JSON_SDKVERSION "\":\"%s\","
                          "\"Hardware\":\"%s\""
                          "%s}}"),
                          my_version, my_image, GetBuildDateAndTime().c_str()
#ifdef ESP8266
                          , ESP.getBootVersion()
#endif
                          , ESP.getSdkVersion(),
                          GetDeviceHardware().c_str(),
                          GetStatistics().c_str());
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "2"));
  }

  if ((0 == payload) || (3 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS3_LOGGING "\":{\"" D_CMND_SERIALLOG "\":%d,\"" D_CMND_WEBLOG "\":%d,\"" D_CMND_MQTTLOG "\":%d,\"" D_CMND_SYSLOG "\":%d,\""
                          D_CMND_LOGHOST "\":\"%s\",\"" D_CMND_LOGPORT "\":%d,\"" D_CMND_SSID "\":[\"%s\",\"%s\"],\"" D_CMND_TELEPERIOD "\":%d,\""
                          D_JSON_RESOLUTION "\":\"%08X\",\"" D_CMND_SETOPTION "\":[\"%08X\",\"%s\",\"%08X\",\"%08X\"]}}"),
                          Settings.seriallog_level, Settings.weblog_level, Settings.mqttlog_level, Settings.syslog_level,
                          SettingsText(SET_SYSLOG_HOST), Settings.syslog_port, SettingsText(SET_STASSID1), SettingsText(SET_STASSID2), Settings.tele_period,
                          Settings.flag2.data, Settings.flag.data, ToHex_P((unsigned char*)Settings.param, PARAM8_SIZE, stemp2, sizeof(stemp2)),
                          Settings.flag3.data, Settings.flag4.data);
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "3"));
  }

  if ((0 == payload) || (4 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS4_MEMORY "\":{\"" D_JSON_PROGRAMSIZE "\":%d,\"" D_JSON_FREEMEMORY "\":%d,\"" D_JSON_HEAPSIZE "\":%d,\""
#ifdef ESP32
                          D_JSON_PSRMAXMEMORY "\":%d,\"" D_JSON_PSRFREEMEMORY "\":%d,"
#endif
                          D_JSON_PROGRAMFLASHSIZE "\":%d,\"" D_JSON_FLASHSIZE "\":%d"
#ifdef ESP8266
                          ",\"" D_JSON_FLASHCHIPID "\":\"%06X\""
#endif
                          ",\"" D_JSON_FLASHMODE "\":%d,\""
                          D_JSON_FEATURES "\":[\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\"]"),
                          ESP_getSketchSize()/1024, ESP.getFreeSketchSpace()/1024, ESP_getFreeHeap()/1024,
#ifdef ESP32
                          ESP.getPsramSize()/1024, ESP.getFreePsram()/1024,
#endif
                          ESP.getFlashChipSize()/1024, ESP.getFlashChipRealSize()/1024
#ifdef ESP8266
                          , ESP.getFlashChipId()
#endif
                          , ESP.getFlashChipMode(),
                          LANGUAGE_LCID, feature_drv1, feature_drv2, feature_sns1, feature_sns2, feature5, feature6);
    XsnsDriverState();
    ResponseAppend_P(PSTR(",\"Sensors\":"));
    XsnsSensorState();
    ResponseJsonEndEnd();
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "4"));
  }

  if ((0 == payload) || (5 == payload)) {
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS5_NETWORK "\":{\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%s\",\"" D_JSON_GATEWAY "\":\"%s\",\""
                          D_JSON_SUBNETMASK "\":\"%s\",\"" D_JSON_DNSSERVER "\":\"%s\",\"" D_JSON_MAC "\":\"%s\",\""
                          D_CMND_WEBSERVER "\":%d,\"" D_CMND_WIFICONFIG "\":%d,\"" D_CMND_WIFIPOWER "\":%s}}"),
                          my_hostname, WiFi.localIP().toString().c_str(), IPAddress(Settings.ip_address[1]).toString().c_str(),
                          IPAddress(Settings.ip_address[2]).toString().c_str(), IPAddress(Settings.ip_address[3]).toString().c_str(), WiFi.macAddress().c_str(),
                          Settings.webserver, Settings.sta_config, WifiGetOutputPower().c_str());
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "5"));
  }

  if (((0 == payload) || (6 == payload)) && Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS6_MQTT "\":{\"" D_CMND_MQTTHOST "\":\"%s\",\"" D_CMND_MQTTPORT "\":%d,\"" D_CMND_MQTTCLIENT D_JSON_MASK "\":\"%s\",\""
                          D_CMND_MQTTCLIENT "\":\"%s\",\"" D_CMND_MQTTUSER "\":\"%s\",\"" D_JSON_MQTT_COUNT "\":%d,\"MAX_PACKET_SIZE\":%d,\"KEEPALIVE\":%d}}"),
                          SettingsText(SET_MQTT_HOST), Settings.mqtt_port, SettingsText(SET_MQTT_CLIENT),
                          mqtt_client, SettingsText(SET_MQTT_USER), MqttConnectCount(), MQTT_MAX_PACKET_SIZE, MQTT_KEEPALIVE);
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "6"));
  }

  if ((0 == payload) || (7 == payload)) {
    if (99 == Settings.timezone) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%d" ), Settings.timezone);
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
    MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "7"));
  }

#if defined(USE_ENERGY_SENSOR) && defined(USE_ENERGY_MARGIN_DETECTION)
  if (energy_flg) {
    if ((0 == payload) || (9 == payload)) {
      Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS9_MARGIN "\":{\"" D_CMND_POWERDELTA "\":%d,\"" D_CMND_POWERLOW "\":%d,\"" D_CMND_POWERHIGH "\":%d,\""
                            D_CMND_VOLTAGELOW "\":%d,\"" D_CMND_VOLTAGEHIGH "\":%d,\"" D_CMND_CURRENTLOW "\":%d,\"" D_CMND_CURRENTHIGH "\":%d}}"),
                            Settings.energy_power_delta, Settings.energy_min_power, Settings.energy_max_power,
                            Settings.energy_min_voltage, Settings.energy_max_voltage, Settings.energy_min_current, Settings.energy_max_current);
      MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "9"));
    }
  }
#endif  // USE_ENERGY_MARGIN_DETECTION

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

  if (CrashFlag()) {
    if ((0 == payload) || (12 == payload)) {
      Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS12_STATUS "\":"));
      CrashDump();
      ResponseJsonEnd();
      MqttPublishPrefixTopic_P(option, PSTR(D_CMND_STATUS "12"));
    }
  }

#ifdef USE_SCRIPT_STATUS
  if (bitRead(Settings.rule_enabled, 0)) Run_Scripter(">U",2,mqtt_data);
#endif

  if (payload) {
    XdrvRulesProcess();  // Allow rule processing on single Status command only
  }

  mqtt_data[0] = '\0';
}

void CmndState(void)
{
  mqtt_data[0] = '\0';
  MqttShowState();
  if (Settings.flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_STATE), MQTT_TELE_RETAIN);
  }
#ifdef USE_HOME_ASSISTANT
  if (Settings.flag.hass_discovery) {       // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
    HAssPublishStatus();
  }
#endif  // USE_HOME_ASSISTANT
}

void CmndTempOffset(void)
{
  if (XdrvMailbox.data_len > 0) {
    int value = (int)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value > -127) && (value < 127)) {
      Settings.temp_comp = value;
    }
  }
  ResponseCmndFloat((float)(Settings.temp_comp) / 10, 1);
}

void CmndHumOffset(void)
{
  if (XdrvMailbox.data_len > 0) {
    int value = (int)(CharToFloat(XdrvMailbox.data) * 10);
    if ((value > -101) && (value < 101)) {
      Settings.hum_comp = value;
    }
  }
  ResponseCmndFloat((float)(Settings.hum_comp) / 10, 1);
}

void CmndGlobalTemp(void)
{
  if (XdrvMailbox.data_len > 0) {
    float temperature = CharToFloat(XdrvMailbox.data);
    if (!isnan(temperature) && Settings.flag.temperature_conversion) {    // SetOption8 - Switch between Celsius or Fahrenheit
      temperature = (temperature - 32) / 1.8;                             // Celsius
    }
    if ((temperature >= -50.0) && (temperature <= 100.0)) {
      ConvertTemp(temperature);
      global_update = 1;  // Keep global values just entered valid
    }
  }
  ResponseCmndFloat(global_temperature, 1);
}

void CmndGlobalHum(void)
{
  if (XdrvMailbox.data_len > 0) {
    float humidity = CharToFloat(XdrvMailbox.data);
    if ((humidity >= 0.0) && (humidity <= 100.0)) {
      ConvertHumidity(humidity);
      global_update = 1;  // Keep global values just entered valid
    }
  }
  ResponseCmndFloat(global_humidity, 1);
}

void CmndSleep(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 251)) {
    Settings.sleep = XdrvMailbox.payload;
    ssleep = XdrvMailbox.payload;
    WiFiSetSleepMode();
  }
  Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, XdrvMailbox.command, Settings.sleep, ssleep);

}

void CmndUpgrade(void)
{
  // Check if the payload is numerically 1, and had no trailing chars.
  //   e.g. "1foo" or "1.2.3" could fool us.
  // Check if the version we have been asked to upgrade to is higher than our current version.
  //   We also need at least 3 chars to make a valid version number string.
  if (((1 == XdrvMailbox.data_len) && (1 == XdrvMailbox.payload)) || ((XdrvMailbox.data_len >= 3) && NewerVersion(XdrvMailbox.data))) {
    ota_state_flag = 3;
    char stemp1[TOPSZ];
    Response_P(PSTR("{\"%s\":\"" D_JSON_VERSION " %s " D_JSON_FROM " %s\"}"), XdrvMailbox.command, my_version, GetOtaUrl(stemp1, sizeof(stemp1)));
  } else {
    Response_P(PSTR("{\"%s\":\"" D_JSON_ONE_OR_GT "\"}"), XdrvMailbox.command, my_version);
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
    Settings.flag.mqtt_serial = 0;       // CMND_SERIALSEND and CMND_SERIALLOG
    SetSeriallog(XdrvMailbox.payload);
  }
  Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, XdrvMailbox.command, Settings.seriallog_level, seriallog_level);
}

void CmndRestart(void)
{
  switch (XdrvMailbox.payload) {
  case 1:
    restart_flag = 2;
    ResponseCmndChar(D_JSON_RESTARTING);
    break;
  case -1:
    CmndCrash();    // force a crash
    break;
  case -2:
    CmndWDT();
    break;
  case -3:
    CmndBlockedLoop();
    break;
  case 99:
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_RESTARTING));
    EspRestart();
    break;
  default:
    ResponseCmndChar_P(PSTR(D_JSON_ONE_TO_RESTART));
  }
}

void CmndPowerOnState(void)
{
#ifdef ESP8266
  if (my_module_type != MOTOR)
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
      Settings.poweronstate = XdrvMailbox.payload;
      if (POWER_ALL_ALWAYS_ON == Settings.poweronstate) {
        for (uint32_t i = 1; i <= devices_present; i++) {
          ExecuteCommandPower(i, POWER_ON, SRC_IGNORE);
        }
      }
    }
    ResponseCmndNumber(Settings.poweronstate);
  }
}

void CmndPulsetime(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_PULSETIMERS)) {
    uint32_t items = 1;
    if (!XdrvMailbox.usridx && !XdrvMailbox.data_len) {
      items = MAX_PULSETIMERS;
    } else {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 65536)) {
        Settings.pulse_timer[XdrvMailbox.index -1] = XdrvMailbox.payload;  // 0 - 65535
        SetPulseTimer(XdrvMailbox.index -1, XdrvMailbox.payload);
      }
    }
    mqtt_data[0] = '\0';
    for (uint32_t i = 0; i < items; i++) {
      uint32_t index = (1 == items) ? XdrvMailbox.index : i +1;
      ResponseAppend_P(PSTR("%c\"%s%d\":{\"" D_JSON_SET "\":%d,\"" D_JSON_REMAINING "\":%d}"),
        (i) ? ',' : '{',
        XdrvMailbox.command, index,
        Settings.pulse_timer[index -1], GetPulseTimer(index -1));
    }
    ResponseJsonEnd();
  }
}

void CmndBlinktime(void)
{
  if ((XdrvMailbox.payload > 1) && (XdrvMailbox.payload <= 3600)) {
    Settings.blinktime = XdrvMailbox.payload;
    if (blink_timer > 0) { blink_timer = millis() + (100 * XdrvMailbox.payload); }
  }
  ResponseCmndNumber(Settings.blinktime);
}

void CmndBlinkcount(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 65536)) {
    Settings.blinkcount = XdrvMailbox.payload;  // 0 - 65535
    if (blink_counter) { blink_counter = Settings.blinkcount *2; }
  }
  ResponseCmndNumber(Settings.blinkcount);
}

void CmndSavedata(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3600)) {
    Settings.save_data = XdrvMailbox.payload;
    save_data_counter = Settings.save_data;
  }
  SettingsSaveAll();
  char stemp1[TOPSZ];
  if (Settings.save_data > 1) {
    snprintf_P(stemp1, sizeof(stemp1), PSTR(D_JSON_EVERY " %d " D_UNIT_SECOND), Settings.save_data);
  }
  ResponseCmndChar((Settings.save_data > 1) ? stemp1 : GetStateText(Settings.save_data));
}

void CmndSetoption(void)
{
  snprintf_P(XdrvMailbox.command, CMDSZ, PSTR(D_CMND_SETOPTION));  // Rename result shortcut command SO to SetOption

  if (XdrvMailbox.index < 114) {
    uint32_t ptype;
    uint32_t pindex;
    if (XdrvMailbox.index <= 31) {         // SetOption0 .. 31 = Settings.flag
      ptype = 2;
      pindex = XdrvMailbox.index;          // 0 .. 31
    }
    else if (XdrvMailbox.index <= 49) {    // SetOption32 .. 49 = Settings.param
      ptype = 1;
      pindex = XdrvMailbox.index -32;      // 0 .. 17 (= PARAM8_SIZE -1)
    }
    else if (XdrvMailbox.index <= 81) {    // SetOption50 .. 81 = Settings.flag3
      ptype = 3;
      pindex = XdrvMailbox.index -50;      // 0 .. 31
    }
    else {                                 // SetOption82 .. 113 = Settings.flag4
      ptype = 4;
      pindex = XdrvMailbox.index -82;      // 0 .. 31
    }

    if (XdrvMailbox.payload >= 0) {
      if (1 == ptype) {                    // SetOption32 .. 49
        uint32_t param_low = 0;
        uint32_t param_high = 255;
        switch (pindex) {
          case P_HOLD_TIME:
          case P_MAX_POWER_RETRY:
            param_low = 1;
            param_high = 250;
            break;
        }
        if ((XdrvMailbox.payload >= param_low) && (XdrvMailbox.payload <= param_high)) {
          Settings.param[pindex] = XdrvMailbox.payload;
#ifdef USE_LIGHT
          if (P_RGB_REMAP == pindex) {
            LightUpdateColorMapping();
            restart_flag = 2;              // SetOption37 needs a reboot in most cases
          }
#endif
#if (defined(USE_IR_REMOTE) && defined(USE_IR_RECEIVE)) || defined(USE_IR_REMOTE_FULL)
          if (P_IR_UNKNOW_THRESHOLD == pindex) {
            IrReceiveUpdateThreshold();
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
                restart_flag = 2;
              default:
                bitWrite(Settings.flag.data, pindex, XdrvMailbox.payload);
            }
            if (12 == pindex) {            // stop_flash_rotate
              stop_flash_rotate = XdrvMailbox.payload;
              SettingsSave(2);
            }
  #ifdef USE_HOME_ASSISTANT
            if ((19 == pindex) || (30 == pindex)) {
              HAssDiscover();              // Delayed execution to provide enough resources during hass_discovery or hass_light
            }
  #endif  // USE_HOME_ASSISTANT
          }
          else if (3 == ptype) {           // SetOption50 .. 81
            bitWrite(Settings.flag3.data, pindex, XdrvMailbox.payload);
            switch (pindex) {
              case 5:                      // SetOption55
                if (0 == XdrvMailbox.payload) {
                  restart_flag = 2;        // Disable mDNS needs restart
                }
                break;
              case 10:                     // SetOption60 enable or disable traditional sleep
                WiFiSetSleepMode();        // Update WiFi sleep mode accordingly
                break;
              case 18:                     // SetOption68 for multi-channel PWM, requires a reboot
              case 25:                     // SetOption75 grouptopic change
                restart_flag = 2;
                break;
            }
          }
          else if (4 == ptype) {           // SetOption82 .. 113
            bitWrite(Settings.flag4.data, pindex, XdrvMailbox.payload);
            switch (pindex) {
              case 3:                      // SetOption85 - Enable Device Groups
              case 6:                      // SetOption88 - PWM Dimmer Buttons control remote devices
                restart_flag = 2;
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
        ResponseCmndIdxNumber(Settings.param[pindex]);
      } else {
        uint32_t flag = Settings.flag.data;
        if (3 == ptype) {
          flag = Settings.flag3.data;
        }
        else if (4 == ptype) {
          flag = Settings.flag4.data;
        }
        ResponseCmndIdxChar(GetStateText(bitRead(flag, pindex)));
      }
    }
  }
}

void CmndTemperatureResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings.flag2.temperature_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.flag2.temperature_resolution);
}

void CmndHumidityResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings.flag2.humidity_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.flag2.humidity_resolution);
}

void CmndPressureResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings.flag2.pressure_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.flag2.pressure_resolution);
}

void CmndPowerResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings.flag2.wattage_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.flag2.wattage_resolution);
}

void CmndVoltageResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings.flag2.voltage_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.flag2.voltage_resolution);
}

void CmndFrequencyResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings.flag2.frequency_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.flag2.frequency_resolution);
}

void CmndCurrentResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings.flag2.current_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.flag2.current_resolution);
}

void CmndEnergyResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 5)) {
    Settings.flag2.energy_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.flag2.energy_resolution);
}

void CmndWeightResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    Settings.flag2.weight_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.flag2.weight_resolution);
}

void CmndSpeedUnit(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
    Settings.flag2.speed_conversion = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.flag2.speed_conversion);
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
      Settings.last_module = Settings.module;
      Settings.module = XdrvMailbox.payload;
      SetModuleType();
      if (Settings.last_module != XdrvMailbox.payload) {
        for (uint32_t i = 0; i < ARRAY_SIZE(Settings.my_gp.io); i++) {
          Settings.my_gp.io[i] = GPIO_NONE;
        }
      }
      restart_flag = 2;
    }
  }
  Response_P(S_JSON_COMMAND_NVALUE_SVALUE, XdrvMailbox.command, ModuleNr(), ModuleName().c_str());
}

void CmndModules(void)
{
  uint32_t midx = USER_MODULE;
  uint32_t lines = 1;
  bool jsflg = false;
  for (uint32_t i = 0; i <= sizeof(kModuleNiceList); i++) {
    if (i > 0) { midx = pgm_read_byte(kModuleNiceList + i -1); }
    if (!jsflg) {
      Response_P(PSTR("{\"" D_CMND_MODULES "%d\":{"), lines);
    } else {
      ResponseAppend_P(PSTR(","));
    }
    jsflg = true;
    uint32_t j = i ? midx +1 : 0;
    if ((ResponseAppend_P(PSTR("\"%d\":\"%s\""), j, AnyModuleName(midx).c_str()) > (LOGSZ - TOPSZ)) || (i == sizeof(kModuleNiceList))) {
      ResponseJsonEndEnd();
      MqttPublishPrefixTopic_P(RESULT_OR_STAT, XdrvMailbox.command);
      jsflg = false;
      lines++;
    }
  }
  mqtt_data[0] = '\0';
}

void CmndGpio(void)
{
  if (XdrvMailbox.index < ARRAY_SIZE(Settings.my_gp.io)) {
    myio cmodule;
    ModuleGpios(&cmodule);
    if (ValidGPIO(XdrvMailbox.index, cmodule.io[XdrvMailbox.index]) && (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < AGPIO(GPIO_SENSOR_END))) {
      bool present = false;
      for (uint32_t i = 0; i < ARRAY_SIZE(kGpioNiceList); i++) {
#ifdef ESP8266
        uint32_t midx = pgm_read_byte(kGpioNiceList + i);
        if (midx == XdrvMailbox.payload) {
          present = true;
          break;
        }
#else  // ESP32
        uint32_t midx = pgm_read_word(kGpioNiceList + i);
        if ((XdrvMailbox.payload >= (midx & 0xFFE0)) && (XdrvMailbox.payload < midx)) {
          present = true;
          break;
        }
#endif  // ESP8266 - ESP32
      }
      if (present) {
        for (uint32_t i = 0; i < ARRAY_SIZE(Settings.my_gp.io); i++) {
          if (ValidGPIO(i, cmodule.io[i]) && (Settings.my_gp.io[i] == XdrvMailbox.payload)) {
            Settings.my_gp.io[i] = GPIO_NONE;
          }
        }
        Settings.my_gp.io[XdrvMailbox.index] = XdrvMailbox.payload;
        restart_flag = 2;
      }
    }
    Response_P(PSTR("{"));
    bool jsflg = false;
    for (uint32_t i = 0; i < ARRAY_SIZE(Settings.my_gp.io); i++) {
      if (ValidGPIO(i, cmodule.io[i]) || ((AGPIO(GPIO_USER) == XdrvMailbox.payload) && !FlashPin(i))) {
        if (jsflg) { ResponseAppend_P(PSTR(",")); }
        jsflg = true;
        uint32_t sensor_type = Settings.my_gp.io[i];
        if (!ValidGPIO(i, cmodule.io[i])) {
          sensor_type = cmodule.io[i];
          if (AGPIO(GPIO_USER) == sensor_type) {  // A user GPIO equals a not connected (=GPIO_NONE) GPIO here
            sensor_type = GPIO_NONE;
          }
        }
        char sindex[4] = { 0 };
#ifdef ESP8266
        uint32_t sensor_name_idx = sensor_type;
#else  // ESP32
        uint32_t sensor_name_idx = sensor_type >> 5;
        uint32_t nice_list_search = sensor_type & 0xFFE0;
        for (uint32_t j = 0; j < ARRAY_SIZE(kGpioNiceList); j++) {
          uint32_t nls_idx = pgm_read_word(kGpioNiceList + j);
          if (((nls_idx & 0xFFE0) == nice_list_search) && ((nls_idx & 0x001F) > 0)) {
            snprintf_P(sindex, sizeof(sindex), PSTR("%d"), (sensor_type & 0x001F) +1);
            break;
          }
        }
#endif  // ESP8266 - ESP32
        const char *sensor_names = kSensorNames;
        if (sensor_name_idx > GPIO_FIX_START) {
          sensor_name_idx = sensor_name_idx - GPIO_FIX_START -1;
          sensor_names = kSensorNamesFixed;
        }
        char stemp1[TOPSZ];
        ResponseAppend_P(PSTR("\"" D_CMND_GPIO "%d\":{\"%d\":\"%s%s\"}"),
          i, sensor_type, GetTextIndexed(stemp1, sizeof(stemp1), sensor_name_idx, sensor_names), sindex);
      }
    }
    if (jsflg) {
      ResponseJsonEnd();
    } else {
      ResponseCmndChar(D_JSON_NOT_SUPPORTED);
    }
  }
}

void CmndGpios(void)
{
  myio cmodule;
  ModuleGpios(&cmodule);
  uint32_t lines = 1;
  bool jsflg = false;
  for (uint32_t i = 0; i < ARRAY_SIZE(kGpioNiceList); i++) {
#ifdef ESP8266
    uint32_t midx = pgm_read_byte(kGpioNiceList + i);
    uint32_t ridx = midx;
#else  // ESP32
    uint32_t ridx = pgm_read_word(kGpioNiceList + i) & 0xFFE0;
    uint32_t midx = ridx >> 5;
#endif  // ESP8266 - ESP32
    if ((XdrvMailbox.payload != 255) && GetUsedInModule(midx, cmodule.io)) { continue; }
    if (!jsflg) {
      Response_P(PSTR("{\"" D_CMND_GPIOS "%d\":{"), lines);
    } else {
      ResponseAppend_P(PSTR(","));
    }
    jsflg = true;
    char stemp1[TOPSZ];
    if ((ResponseAppend_P(PSTR("\"%d\":\"%s\""), ridx, GetTextIndexed(stemp1, sizeof(stemp1), midx, kSensorNames)) > (LOGSZ - TOPSZ)) || (i == ARRAY_SIZE(kGpioNiceList) -1)) {
      ResponseJsonEndEnd();
      MqttPublishPrefixTopic_P(RESULT_OR_STAT, XdrvMailbox.command);
      jsflg = false;
      lines++;
    }
  }
  mqtt_data[0] = '\0';
}

void CmndTemplate(void)
{
  // {"NAME":"Generic","GPIO":[17,254,29,254,7,254,254,254,138,254,139,254,254],"FLAG":1,"BASE":255}
  bool error = false;

  if (strstr(XdrvMailbox.data, "{") == nullptr) {  // If no JSON it must be parameter
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= MAXMODULE)) {
      XdrvMailbox.payload--;
      if (ValidTemplateModule(XdrvMailbox.payload)) {
        ModuleDefault(XdrvMailbox.payload);     // Copy template module
        if (USER_MODULE == Settings.module) { restart_flag = 2; }
      }
    }
    else if (0 == XdrvMailbox.payload) {        // Copy current template to user template
      if (Settings.module != USER_MODULE) {
        ModuleDefault(Settings.module);
      }
    }
    else if (255 == XdrvMailbox.payload) {      // Copy current module with user configured GPIO
      if (Settings.module != USER_MODULE) {
        ModuleDefault(Settings.module);
      }
      SettingsUpdateText(SET_TEMPLATE_NAME, "Merged");
      uint32_t j = 0;
      for (uint32_t i = 0; i < ARRAY_SIZE(Settings.user_template.gp.io); i++) {
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
    if (JsonTemplate(XdrvMailbox.data)) {    // Free 336 bytes StaticJsonBuffer stack space by moving code to function
      if (USER_MODULE == Settings.module) { restart_flag = 2; }
    } else {
      ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON));
      error = true;
    }
  }
  if (!error) { TemplateJson(); }
}

void CmndPwm(void)
{
  if (pwm_present && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_PWMS)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= Settings.pwm_range) && PinUsed(GPIO_PWM1, XdrvMailbox.index -1)) {
      Settings.pwm_value[XdrvMailbox.index -1] = XdrvMailbox.payload;
      analogWrite(Pin(GPIO_PWM1, XdrvMailbox.index -1), bitRead(pwm_inverted, XdrvMailbox.index -1) ? Settings.pwm_range - XdrvMailbox.payload : XdrvMailbox.payload);
    }
    Response_P(PSTR("{"));
    MqttShowPWMState();  // Render the PWM status to MQTT
    ResponseJsonEnd();
  }
}

void CmndPwmfrequency(void)
{
  if ((1 == XdrvMailbox.payload) || ((XdrvMailbox.payload >= PWM_MIN) && (XdrvMailbox.payload <= PWM_MAX))) {
    Settings.pwm_frequency = (1 == XdrvMailbox.payload) ? PWM_FREQ : XdrvMailbox.payload;
#ifdef ESP8266
    analogWriteFreq(Settings.pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)
#else
    analogWriteFreqRange(0,Settings.pwm_frequency,Settings.pwm_range);
#endif
  }
  ResponseCmndNumber(Settings.pwm_frequency);
}

void CmndPwmrange(void)
{
  if ((1 == XdrvMailbox.payload) || ((XdrvMailbox.payload > 254) && (XdrvMailbox.payload < 1024))) {
    Settings.pwm_range = (1 == XdrvMailbox.payload) ? PWM_RANGE : XdrvMailbox.payload;
    for (uint32_t i = 0; i < MAX_PWMS; i++) {
      if (Settings.pwm_value[i] > Settings.pwm_range) {
        Settings.pwm_value[i] = Settings.pwm_range;
      }
    }
#ifdef ESP8266
    analogWriteRange(Settings.pwm_range);      // Default is 1023 (Arduino.h)
#else
    analogWriteFreqRange(0,Settings.pwm_frequency,Settings.pwm_range);
#endif
  }
  ResponseCmndNumber(Settings.pwm_range);
}

void CmndButtonDebounce(void)
{
  if ((XdrvMailbox.payload > 39) && (XdrvMailbox.payload < 1001)) {
    Settings.button_debounce = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.button_debounce);
}

void CmndSwitchDebounce(void)
{
  if ((XdrvMailbox.payload > 39) && (XdrvMailbox.payload < 1001)) {
    Settings.switch_debounce = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.switch_debounce);
}

void CmndBaudrate(void)
{
  if (XdrvMailbox.payload >= 300) {
    XdrvMailbox.payload /= 300;  // Make it a valid baudrate
    uint32_t baudrate = (XdrvMailbox.payload & 0xFFFF) * 300;
    SetSerialBaudrate(baudrate);
  }
  ResponseCmndNumber(Settings.baudrate * 300);
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
      uint8_t serial_config = XdrvMailbox.payload -5;  // Data bits 5, 6, 7 or 8, No parity and 1 stop bit

      bool valid = true;
      char parity = (XdrvMailbox.data[1] & 0xdf);
      if ('E' == parity) {
        serial_config += 0x08;                         // Even parity
      }
      else if ('O' == parity) {
        serial_config += 0x10;                         // Odd parity
      }
      else if ('N' != parity) {
        valid = false;
      }

      if ('2' == XdrvMailbox.data[2]) {
        serial_config += 0x04;                         // Stop bits 2
      }
      else if ('1' != XdrvMailbox.data[2]) {
        valid = false;
      }

      if (valid) {
        SetSerialConfig(serial_config);
      }
    }
  }
  ResponseCmndChar(GetSerialConfig().c_str());
}

void CmndSerialSend(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 5)) {
    SetSeriallog(LOG_LEVEL_NONE);
    Settings.flag.mqtt_serial = 1;                                  // CMND_SERIALSEND and CMND_SERIALLOG
    Settings.flag.mqtt_serial_raw = (XdrvMailbox.index > 3) ? 1 : 0;  // CMND_SERIALSEND3
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
      ResponseCmndDone();
    }
  }
}

void CmndSerialDelimiter(void)
{
  if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.payload < 256)) {
    if (XdrvMailbox.payload > 0) {
      Settings.serial_delimiter = XdrvMailbox.payload;
    } else {
      uint32_t dat_len = XdrvMailbox.data_len;
      Unescape(XdrvMailbox.data, &dat_len);
      Settings.serial_delimiter = XdrvMailbox.data[0];
    }
  }
  ResponseCmndNumber(Settings.serial_delimiter);
}

void CmndSyslog(void)
{
  if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {
    SetSyslog(XdrvMailbox.payload);
  }
  Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, XdrvMailbox.command, Settings.syslog_level, syslog_level);
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
    Settings.syslog_port = (1 == XdrvMailbox.payload) ? SYS_LOG_PORT : XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.syslog_port);
}

void CmndIpAddress(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 4)) {
    uint32_t address;
    if (ParseIp(&address, XdrvMailbox.data)) {
      Settings.ip_address[XdrvMailbox.index -1] = address;
//        restart_flag = 2;
    }
    char stemp1[TOPSZ];
    snprintf_P(stemp1, sizeof(stemp1), PSTR(" (%s)"), WiFi.localIP().toString().c_str());
    Response_P(S_JSON_COMMAND_INDEX_SVALUE_SVALUE, XdrvMailbox.command, XdrvMailbox.index, IPAddress(Settings.ip_address[XdrvMailbox.index -1]).toString().c_str(), (1 == XdrvMailbox.index) ? stemp1:"");
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
  //        restart_flag = 2;  // Issue #3890
        ntp_force_sync = true;
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
      Settings.sta_active ^= 1;
      break;
    case 1:  // AP1
    case 2:  // AP2
      Settings.sta_active = XdrvMailbox.payload -1;
    }
    restart_flag = 2;
  }
  Response_P(S_JSON_COMMAND_NVALUE_SVALUE, XdrvMailbox.command, Settings.sta_active +1, SettingsText(SET_STASSID1 + Settings.sta_active));
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
        Settings.sta_active = XdrvMailbox.index -1;
        restart_flag = 2;
      }
      ResponseCmndIdxChar(SettingsText(SET_STASSID1 + XdrvMailbox.index -1));
    }
  }
}

void CmndPassword(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
    if ((XdrvMailbox.data_len > 4) || (SC_CLEAR == Shortcut()) || (SC_DEFAULT == Shortcut())) {
      SettingsUpdateText(SET_STAPWD1 + XdrvMailbox.index -1,
              (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? (1 == XdrvMailbox.index) ? STA_PASS1 : STA_PASS2 : XdrvMailbox.data);
      Settings.sta_active = XdrvMailbox.index -1;
      restart_flag = 2;
      ResponseCmndIdxChar(SettingsText(SET_STAPWD1 + XdrvMailbox.index -1));
    } else {
      Response_P(S_JSON_COMMAND_INDEX_ASTERISK, XdrvMailbox.command, XdrvMailbox.index);
    }
  }
}

void CmndHostname(void)
{
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    SettingsUpdateText(SET_HOSTNAME, (SC_DEFAULT == Shortcut()) ? WIFI_HOSTNAME : XdrvMailbox.data);
    if (strstr(SettingsText(SET_HOSTNAME), "%") != nullptr) {
      SettingsUpdateText(SET_HOSTNAME, WIFI_HOSTNAME);
    }
    restart_flag = 2;
  }
  ResponseCmndChar(SettingsText(SET_HOSTNAME));
}

void CmndWifiConfig(void)
{
  if ((XdrvMailbox.payload >= WIFI_RESTART) && (XdrvMailbox.payload < MAX_WIFI_OPTION)) {
    if ((EX_WIFI_SMARTCONFIG == XdrvMailbox.payload) || (EX_WIFI_WPSCONFIG == XdrvMailbox.payload)) {
      XdrvMailbox.payload = WIFI_MANAGER;
    }
    Settings.sta_config = XdrvMailbox.payload;
    wifi_state_flag = Settings.sta_config;
    if (WifiState() > WIFI_RESTART) {
      restart_flag = 2;
    }
  }
  char stemp1[TOPSZ];
  Response_P(S_JSON_COMMAND_NVALUE_SVALUE, XdrvMailbox.command, Settings.sta_config, GetTextIndexed(stemp1, sizeof(stemp1), Settings.sta_config, kWifiConfig));
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

void CmndSwitchMode(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_SWITCHES)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < MAX_SWITCH_OPTION)) {
      Settings.switchmode[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Settings.switchmode[XdrvMailbox.index-1]);
  }
}

void CmndInterlock(void)
{
  // Interlock 0 - Off, Interlock 1 - On, Interlock 1,2 3,4 5,6,7
  uint32_t max_relays = devices_present;
  if (light_type) { max_relays--; }
  if (max_relays > sizeof(Settings.interlock[0]) * 8) { max_relays = sizeof(Settings.interlock[0]) * 8; }
  if (max_relays > 1) {                                         // Only interlock with more than 1 relay
    if (XdrvMailbox.data_len > 0) {
      if (strstr(XdrvMailbox.data, ",") != nullptr) {                    // Interlock entry
        for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) { Settings.interlock[i] = 0; }  // Reset current interlocks
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
                bitSet(Settings.interlock[group_index], pbit);
              }
            }
          }
          group_index++;
        }
        for (uint32_t i = 0; i < group_index; i++) {
          uint32_t minimal_bits = 0;
          for (uint32_t j = 0; j < max_relays; j++) {
            if (bitRead(Settings.interlock[i], j)) { minimal_bits++; }
          }
          if (minimal_bits < 2) { Settings.interlock[i] = 0; }  // Discard single relay as interlock
        }
      } else {
        Settings.flag.interlock = XdrvMailbox.payload &1;       // CMND_INTERLOCK - Enable/disable interlock
        if (Settings.flag.interlock) {
          SetDevicePower(power, SRC_IGNORE);                    // Remove multiple relays if set
        }
      }
#ifdef USE_SHUTTER
      if (Settings.flag3.shutter_mode) {  // SetOption80 - Enable shutter support
        ShutterInit(); // to update shutter mode
      }
#endif  // USE_SHUTTER
    }
    Response_P(PSTR("{\"" D_CMND_INTERLOCK "\":\"%s\",\"" D_JSON_GROUPS "\":\""), GetStateText(Settings.flag.interlock));
    uint32_t anygroup = 0;
    for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
      if (Settings.interlock[i]) {
        anygroup++;
        ResponseAppend_P(PSTR("%s"), (anygroup > 1) ? " " : "");
        uint32_t anybit = 0;
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
    Settings.flag.interlock = 0;                                // CMND_INTERLOCK - Enable/disable interlock
    ResponseCmndStateText(Settings.flag.interlock);
  }
}

void CmndTeleperiod(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings.tele_period = (1 == XdrvMailbox.payload) ? TELE_PERIOD : XdrvMailbox.payload;
    if ((Settings.tele_period > 0) && (Settings.tele_period < 10)) Settings.tele_period = 10;   // Do not allow periods < 10 seconds
//    tele_period = Settings.tele_period;
  }
  tele_period = Settings.tele_period;        // Show teleperiod data also on empty command
  ResponseCmndNumber(Settings.tele_period);
}

void CmndReset(void)
{
  switch (XdrvMailbox.payload) {
  case 1:
    restart_flag = 211;
    ResponseCmndChar(PSTR(D_JSON_RESET_AND_RESTARTING));
    break;
  case 2 ... 6:
    restart_flag = 210 + XdrvMailbox.payload;
    Response_P(PSTR("{\"" D_CMND_RESET "\":\"" D_JSON_ERASE ", " D_JSON_RESET_AND_RESTARTING "\"}"));
    break;
  case 99:
    Settings.bootcount = 0;
    Settings.bootcount_reset_time = 0;
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
// payload 4 = reserved
// payload 1451602800 - disable NTP and set time to epoch

  uint32_t format = Settings.flag2.time_format;
  if (XdrvMailbox.data_len > 0) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 4)) {
      Settings.flag2.time_format = XdrvMailbox.payload -1;
      format = Settings.flag2.time_format;
    } else {
      format = 1;  // {"Time":"2019-09-04T14:31:29","Epoch":1567600289}
      RtcSetTime(XdrvMailbox.payload);
    }
  }
  mqtt_data[0] = '\0';
  ResponseAppendTimeFormat(format);
  ResponseJsonEnd();
}

void CmndTimezone(void)
{
  if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.payload >= -13)) {
    Settings.timezone = XdrvMailbox.payload;
    Settings.timezone_minutes = 0;
    if (XdrvMailbox.payload < 15) {
      char *p = strtok (XdrvMailbox.data, ":");
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
    ResponseCmndNumber(Settings.timezone);
  } else {
    char stemp1[TOPSZ];
    snprintf_P(stemp1, sizeof(stemp1), PSTR("%+03d:%02d"), Settings.timezone, Settings.timezone_minutes);
    ResponseCmndChar(stemp1);
  }
}

void CmndTimeStdDst(uint32_t ts)
{
  // TimeStd 0/1, 0/1/2/3/4, 1..12, 1..7, 0..23, +/-780
  if (XdrvMailbox.data_len > 0) {
    if (strstr(XdrvMailbox.data, ",") != nullptr) {   // Process parameter entry
      uint32_t tpos = 0;                      // Parameter index
      int value = 0;
      char *p = XdrvMailbox.data;                           // Parameters like "1, 2,3 , 4 ,5, -120" or ",,,,,+240"
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
      if (0 == XdrvMailbox.payload) {
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
    XdrvMailbox.command, Settings.tflag[ts].hemis, Settings.tflag[ts].week, Settings.tflag[ts].month, Settings.tflag[ts].dow, Settings.tflag[ts].hour, Settings.toffset[ts]);
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
    Settings.altitude = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.altitude);
}

void CmndLedPower(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_LEDS)) {
    if (!PinUsed(GPIO_LEDLNK)) { XdrvMailbox.index = 1; }
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
      Settings.ledstate &= 8;                // Disable power control
      uint32_t mask = 1 << (XdrvMailbox.index -1);        // Led to control
      switch (XdrvMailbox.payload) {
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
      if (!PinUsed(GPIO_LEDLNK)) {
        SetLedPower(Settings.ledstate &8);
      } else {
        SetLedPowerIdx(XdrvMailbox.index -1, (led_power & mask));
      }
    }
    bool state = bitRead(led_power, XdrvMailbox.index -1);
    if (!PinUsed(GPIO_LEDLNK)) {
      state = bitRead(Settings.ledstate, 3);
    }
    ResponseCmndIdxChar(GetStateText(state));
  }
}

void CmndLedState(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < MAX_LED_OPTION)) {
    Settings.ledstate = XdrvMailbox.payload;
    if (!Settings.ledstate) {
      SetLedPowerAll(0);
      SetLedLink(0);
    }
  }
  ResponseCmndNumber(Settings.ledstate);
}

void CmndLedMask(void)
{
  if (XdrvMailbox.data_len > 0) {
    Settings.ledmask = XdrvMailbox.payload;
  }
  char stemp1[TOPSZ];
  snprintf_P(stemp1, sizeof(stemp1), PSTR("%d (0x%04X)"), Settings.ledmask, Settings.ledmask);
  ResponseCmndChar(stemp1);
}

void CmndWifiPower(void)
{
  if (XdrvMailbox.data_len > 0) {
    Settings.wifi_output_power = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
    if (Settings.wifi_output_power > 205) {
      Settings.wifi_output_power = 205;
    }
    WifiSetOutputPower();
  }
  ResponseCmndChar(WifiGetOutputPower().c_str());
}

#ifdef USE_I2C
void CmndI2cScan(void)
{
  if (i2c_flg) {
    I2cScan(mqtt_data, sizeof(mqtt_data));
  }
}

void CmndI2cDriver(void)
{
  if (XdrvMailbox.index < MAX_I2C_DRIVERS) {
    if (XdrvMailbox.payload >= 0) {
      bitWrite(Settings.i2c_drivers[XdrvMailbox.index / 32], XdrvMailbox.index % 32, XdrvMailbox.payload &1);
      restart_flag = 2;
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
      restart_flag = 2;
    }
    ResponseCmndAll(SET_DEV_GROUP_NAME1, MAX_DEV_GROUP_NAMES);
  }
}

#ifdef USE_DEVICE_GROUPS_SEND
void CmndDevGroupSend(void)
{
  uint8_t device_group_index = (XdrvMailbox.usridx ? XdrvMailbox.index - 1 : 0);
  if (device_group_index < device_group_count) {
    if (!_SendDeviceGroupMessage(device_group_index, DGR_MSGTYPE_UPDATE_COMMAND)) {
      ResponseCmndChar(XdrvMailbox.data);
    }
  }
}
#endif  // USE_DEVICE_GROUPS_SEND

void CmndDevGroupShare(void)
{
  uint32_t parm[2] = { Settings.device_group_share_in, Settings.device_group_share_out };
  ParseParameters(2, parm);
  Settings.device_group_share_in = parm[0];
  Settings.device_group_share_out = parm[1];
  Response_P(PSTR("{\"" D_CMND_DEVGROUP_SHARE "\":{\"In\":\"%X\",\"Out\":\"%X\"}}"), Settings.device_group_share_in, Settings.device_group_share_out);
}

void CmndDevGroupStatus(void)
{
  DeviceGroupStatus((XdrvMailbox.usridx ? XdrvMailbox.index - 1 : 0));
}
#endif  // USE_DEVICE_GROUPS

void CmndSensor(void)
{
  XsnsCall(FUNC_COMMAND_SENSOR);
}

void CmndDriver(void)
{
  XdrvCall(FUNC_COMMAND_DRIVER);
}

void CmndSetLedPwmOff(void)
{
  if (XdrvMailbox.data_len > 0) {
	if (XdrvMailbox.payload < 0) {
	  Settings.ledpwm_off = 0;
	} else if (XdrvMailbox.payload > Settings.pwm_range) {
	  Settings.ledpwm_off = Settings.pwm_range;
    } else {
      Settings.ledpwm_off = XdrvMailbox.payload;
    }
	UpdateLedPowerAll();
  }
  ResponseCmndNumber(Settings.ledpwm_off);
}

void CmndSetLedPwmOn(void)
{
  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.payload < 0) {
	  Settings.ledpwm_on = 0;
    } else if (XdrvMailbox.payload > Settings.pwm_range) {
	  Settings.ledpwm_on = Settings.pwm_range;
    } else {
	  Settings.ledpwm_on = XdrvMailbox.payload;
    }
	UpdateLedPowerAll();
  }
  ResponseCmndNumber(Settings.ledpwm_on);
}

void CmndSetLedPwmMode(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_LEDS)) {
    if (!PinUsed(GPIO_LEDLNK)) { XdrvMailbox.index = 1; }
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
      uint32_t mask = 1 << (XdrvMailbox.index -1);        // Led to configure
      switch (XdrvMailbox.payload) {
      case 0: // digital
        Settings.ledpwm_mask &= (0xFF ^ mask);
        break;
      case 1: // pwm
        Settings.ledpwm_mask |= mask;
        break;
      case 2: // toggle
        Settings.ledpwm_mask ^= mask;
        break;
      }
      UpdateLedPowerAll();
    }
    bool state = bitRead(Settings.ledpwm_mask, XdrvMailbox.index -1);
    ResponseCmndIdxChar(GetStateText(state));
  }
}
