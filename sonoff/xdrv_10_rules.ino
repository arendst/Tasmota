/*
  xdrv_10_rules.ino - rule support for Sonoff-Tasmota

  Copyright (C) 2018  ESP Easy Group and Theo Arends

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

#ifdef USE_RULES
/*********************************************************************************************\
 * Rules based heavily on ESP Easy implementation
 *
 * Inspiration: https://github.com/letscontrolit/ESPEasy
 *
 * Add rules using the following, case insensitive, format:
 *   on <trigger1> do <commands> endon on <trigger2> do <commands> endon ..
 *
 * Examples:
 *   on System#Boot do Color 001000 endon
 *   on INA219#Current>0.100 do Dimmer 10 endon
 *   on INA219#Current>0.100 do Backlog Dimmer 10;Color 10,0,0 endon
 *   on INA219#Current>0.100 do Backlog Dimmer 10;Color 100000 endon on System#Boot do color 001000 endon
 *   on ds18b20#temperature>23 do power off endon on ds18b20#temperature<22 do power on endon
 *   on mqtt#connected do color 000010 endon
 *   on mqtt#disconnected do color 00100C endon
 *   on time#initialized do color 001000 endon
 *   on time#initialized>120 do color 001000 endon
 *   on time#set do color 001008 endon
 *   on clock#timer=3 do color 080800 endon
 *   on rules#timer=1 do color 080800 endon
 *   on mqtt#connected do color 000010 endon on mqtt#disconnected do color 001010 endon on time#initialized do color 001000 endon on time#set do backlog color 000810;ruletimer1 10 endon on rules#timer=1 do color 080800 endon
 *   on event#anyname do color 100000 endon
 *   on event#anyname do color %value% endon
 *   on power1#state=1 do color 001000 endon
 *   on button1#state do publish cmnd/ring2/power %value% endon on button2#state do publish cmnd/strip1/power %value% endon
 *   on switch1#state do power2 %value% endon
 *   on analog#a0div10 do publish cmnd/ring2/dimmer %value% endon
 *
 * Notes:
 *   Spaces after <on>, around <do> and before <endon> are mandatory
 *   System#Boot is initiated after MQTT is connected due to command handling preparation
 *   Control rule triggering with command:
 *     Rule 0 = Rules disabled (Off)
 *     Rule 1 = Rules enabled (On)
 *     Rule 2 = Toggle rules state
 *     Rule 4 = Perform commands as long as trigger is met (Once OFF)
 *     Rule 5 = Perform commands once until trigger is not met (Once ON)
 *     Rule 6 = Toggle Once state
 *   Execute an event like:
 *     Event anyname=001000
 *   Set a RuleTimer to 100 seconds like:
 *     RuleTimer2 100
\*********************************************************************************************/

#define XDRV_10             10

#define D_CMND_RULE "Rule"
#define D_CMND_RULETIMER "RuleTimer"
#define D_CMND_EVENT "Event"
#define D_CMND_VAR "Var"
#define D_CMND_MEM "Mem"
#define D_CMND_ADD "Add"
#define D_CMND_SUB "Sub"
#define D_CMND_MULT "Mult"
#define D_CMND_SCALE "Scale"
#define D_CMND_CALC_RESOLUTION "CalcRes"

#define D_JSON_INITIATED "Initiated"

enum RulesCommands { CMND_RULE, CMND_RULETIMER, CMND_EVENT, CMND_VAR, CMND_MEM, CMND_ADD, CMND_SUB, CMND_MULT, CMND_SCALE, CMND_CALC_RESOLUTION };
const char kRulesCommands[] PROGMEM = D_CMND_RULE "|" D_CMND_RULETIMER "|" D_CMND_EVENT "|" D_CMND_VAR "|" D_CMND_MEM "|" D_CMND_ADD "|" D_CMND_SUB "|" D_CMND_MULT "|" D_CMND_SCALE "|" D_CMND_CALC_RESOLUTION ;

String rules_event_value;
unsigned long rules_timer[MAX_RULE_TIMERS] = { 0 };
uint8_t rules_quota = 0;
long rules_new_power = -1;
long rules_old_power = -1;
long rules_old_dimm = -1;

uint32_t rules_triggers[MAX_RULE_SETS] = { 0 };
uint16_t rules_last_minute = 60;
uint8_t rules_trigger_count[MAX_RULE_SETS] = { 0 };
uint8_t rules_teleperiod = 0;

char event_data[100];
char vars[MAX_RULE_VARS][33] = { 0 };

/*******************************************************************************************/

bool RulesRuleMatch(byte rule_set, String &event, String &rule)
{
  // event = {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}}
  // event = {"System":{"Boot":1}}
  // rule = "INA219#CURRENT>0.100"

  bool match = false;
  char stemp[10];

  // Step1: Analyse rule
  int pos = rule.indexOf('#');
  if (pos == -1) { return false; }                     // No # sign in rule

  String rule_task = rule.substring(0, pos);           // "INA219" or "SYSTEM"
  if (rules_teleperiod) {
    int ppos = rule_task.indexOf("TELE-");             // "TELE-INA219" or "INA219"
    if (ppos == -1) { return false; }                  // No pre-amble in rule
    rule_task = rule.substring(5, pos);                // "INA219" or "SYSTEM"
  }

  String rule_name = rule.substring(pos +1);           // "CURRENT>0.100" or "BOOT" or "%var1%" or "MINUTE|5"

  char compare = ' ';
  pos = rule_name.indexOf(">");
  if (pos > 0) {
    compare = '>';
  } else {
    pos = rule_name.indexOf("<");
    if (pos > 0) {
      compare = '<';
    } else {
      pos = rule_name.indexOf("=");
      if (pos > 0) {
        compare = '=';
      } else {
        pos = rule_name.indexOf("|");                  // Modulo, cannot use % easily as it is used for variable detection
        if (pos > 0) {
          compare = '%';
        }
      }
    }
  }

  char rule_svalue[CMDSZ] = { 0 };
  double rule_value = 0;
  if (pos > 0) {
    String rule_param = rule_name.substring(pos + 1);
    for (byte i = 0; i < MAX_RULE_VARS; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%%VAR%d%%"), i +1);
      if (rule_param.startsWith(stemp)) {
        rule_param = vars[i];
        break;
      }
    }
    for (byte i = 0; i < MAX_RULE_MEMS; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%%MEM%d%%"), i +1);
      if (rule_param.startsWith(stemp)) {
        rule_param = Settings.mems[i];
        break;
      }
    }
    snprintf_P(stemp, sizeof(stemp), PSTR("%%TIME%%"));
    if (rule_param.startsWith(stemp)) {
      rule_param = String(GetMinutesPastMidnight());
    }
    snprintf_P(stemp, sizeof(stemp), PSTR("%%UPTIME%%"));
    if (rule_param.startsWith(stemp)) {
      rule_param = String(GetMinutesUptime());
    }
#if defined(USE_TIMERS) && defined(USE_SUNRISE)
    snprintf_P(stemp, sizeof(stemp), PSTR("%%SUNRISE%%"));
    if (rule_param.startsWith(stemp)) {
      rule_param = String(GetSunMinutes(0));
    }
    snprintf_P(stemp, sizeof(stemp), PSTR("%%SUNSET%%"));
    if (rule_param.startsWith(stemp)) {
      rule_param = String(GetSunMinutes(1));
    }
#endif  // USE_TIMERS and USE_SUNRISE
    rule_param.toUpperCase();
    snprintf(rule_svalue, sizeof(rule_svalue), rule_param.c_str());

    int temp_value = GetStateNumber(rule_svalue);
    if (temp_value > -1) {
      rule_value = temp_value;
    } else {
      rule_value = CharToDouble((char*)rule_svalue);   // 0.1      - This saves 9k code over toFLoat()!
    }
    rule_name = rule_name.substring(0, pos);           // "CURRENT"
  }

  // Step2: Search rule_task and rule_name
  StaticJsonBuffer<1024> jsonBuf;
  JsonObject &root = jsonBuf.parseObject(event);
  if (!root.success()) { return false; }               // No valid JSON data

  double value = 0;
  const char* str_value = root[rule_task][rule_name];

//snprintf_P(log_data, sizeof(log_data), PSTR("RUL: Task %s, Name %s, Value |%s|, TrigCnt %d, TrigSt %d, Source %s, Json %s"),
//  rule_task.c_str(), rule_name.c_str(), rule_svalue, rules_trigger_count[rule_set], bitRead(rules_triggers[rule_set], rules_trigger_count[rule_set]), event.c_str(), (str_value) ? str_value : "none");
//AddLog(LOG_LEVEL_DEBUG);

  if (!root[rule_task][rule_name].success()) { return false; }
  // No value but rule_name is ok

  rules_event_value = str_value;                       // Prepare %value%

  // Step 3: Compare rule (value)
  if (str_value) {
    value = CharToDouble((char*)str_value);
    int int_value = int(value);
    int int_rule_value = int(rule_value);
    switch (compare) {
      case '%':
        if ((int_value > 0) && (int_rule_value > 0)) {
          if ((int_value % int_rule_value) == 0) { match = true; }
        }
        break;
      case '>':
        if (value > rule_value) { match = true; }
        break;
      case '<':
        if (value < rule_value) { match = true; }
        break;
      case '=':
//        if (value == rule_value) { match = true; }     // Compare values - only decimals or partly hexadecimals
        if (!strcasecmp(str_value, rule_svalue)) { match = true; }  // Compare strings - this also works for hexadecimals
        break;
      case ' ':
        match = true;                                  // Json value but not needed
        break;
    }
  } else match = true;

  if (bitRead(Settings.rule_once, rule_set)) {
    if (match) {                                       // Only allow match state changes
      if (!bitRead(rules_triggers[rule_set], rules_trigger_count[rule_set])) {
        bitSet(rules_triggers[rule_set], rules_trigger_count[rule_set]);
      } else {
        match = false;
      }
    } else {
      bitClear(rules_triggers[rule_set], rules_trigger_count[rule_set]);
    }
  }

  return match;
}

/*******************************************************************************************/

bool RuleSetProcess(byte rule_set, String &event_saved)
{
  bool serviced = false;
  char stemp[10];

  delay(0);                                               // Prohibit possible loop software watchdog

//snprintf_P(log_data, sizeof(log_data), PSTR("RUL: Event = %s, Rule = %s"), event_saved.c_str(), Settings.rules[rule_set]);
//AddLog(LOG_LEVEL_DEBUG);

  String rules = Settings.rules[rule_set];

  rules_trigger_count[rule_set] = 0;
  int plen = 0;
  int plen2 = 0;
  bool stop_all_rules = false;
  while (true) {
    rules = rules.substring(plen);                        // Select relative to last rule
    rules.trim();
    if (!rules.length()) { return serviced; }             // No more rules

    String rule = rules;
    rule.toUpperCase();                                   // "ON INA219#CURRENT>0.100 DO BACKLOG DIMMER 10;COLOR 100000 ENDON"
    if (!rule.startsWith("ON ")) { return serviced; }     // Bad syntax - Nothing to start on

    int pevt = rule.indexOf(" DO ");
    if (pevt == -1) { return serviced; }                  // Bad syntax - Nothing to do
    String event_trigger = rule.substring(3, pevt);       // "INA219#CURRENT>0.100"

    plen = rule.indexOf(" ENDON");
    plen2 = rule.indexOf(" BREAK");
    if ((plen == -1) && (plen2 == -1)) { return serviced; } // Bad syntax - No ENDON neither BREAK

    if (plen == -1) { plen = 9999; }
    if (plen2 == -1) { plen2 = 9999; }
    plen = tmin(plen, plen2);
    if (plen == plen2) { stop_all_rules = true; }     // If BREAK was used, Stop execution of this rule set

    String commands = rules.substring(pevt +4, plen);     // "Backlog Dimmer 10;Color 100000"
    plen += 6;
    rules_event_value = "";
    String event = event_saved;

//snprintf_P(log_data, sizeof(log_data), PSTR("RUL: Event |%s|, Rule |%s|, Command(s) |%s|"), event.c_str(), event_trigger.c_str(), commands.c_str());
//AddLog(LOG_LEVEL_DEBUG);

    if (RulesRuleMatch(rule_set, event, event_trigger)) {
      commands.trim();
      String ucommand = commands;
      ucommand.toUpperCase();
//      if (!ucommand.startsWith("BACKLOG")) { commands = "backlog " + commands; }  // Always use Backlog to prevent power race exception
      if (ucommand.indexOf("EVENT ") != -1) { commands = "backlog " + commands; }  // Always use Backlog with event to prevent rule event loop exception
      commands.replace(F("%value%"), rules_event_value);
      for (byte i = 0; i < MAX_RULE_VARS; i++) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%%var%d%%"), i +1);
        commands.replace(stemp, vars[i]);
      }
      for (byte i = 0; i < MAX_RULE_MEMS; i++) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%%mem%d%%"), i +1);
        commands.replace(stemp, Settings.mems[i]);
      }
      commands.replace(F("%time%"), String(GetMinutesPastMidnight()));
      commands.replace(F("%uptime%"), String(GetMinutesUptime()));
#if defined(USE_TIMERS) && defined(USE_SUNRISE)
      commands.replace(F("%sunrise%"), String(GetSunMinutes(0)));
      commands.replace(F("%sunset%"), String(GetSunMinutes(1)));
#endif  // USE_TIMERS and USE_SUNRISE

      char command[commands.length() +1];
      snprintf(command, sizeof(command), commands.c_str());

      snprintf_P(log_data, sizeof(log_data), PSTR("RUL: %s performs \"%s\""), event_trigger.c_str(), command);
      AddLog(LOG_LEVEL_INFO);

//      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, D_CMND_RULE, D_JSON_INITIATED);
//      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_RULE));

      ExecuteCommand(command, SRC_RULE);
      serviced = true;
      if (stop_all_rules) { return serviced; }     // If BREAK was used, Stop execution of this rule set
    }
    rules_trigger_count[rule_set]++;
  }
  return serviced;
}

/*******************************************************************************************/

bool RulesProcessEvent(char *json_event)
{
  bool serviced = false;

  ShowFreeMem(PSTR("RulesProcessEvent"));

  String event_saved = json_event;
  event_saved.toUpperCase();

//snprintf_P(log_data, sizeof(log_data), PSTR("RUL: Event %s"), event_saved.c_str());
//AddLog(LOG_LEVEL_DEBUG);

  for (byte i = 0; i < MAX_RULE_SETS; i++) {
    if (strlen(Settings.rules[i]) && bitRead(Settings.rule_enabled, i)) {
      if (RuleSetProcess(i, event_saved)) { serviced = true; }
    }
  }
  return serviced;
}

bool RulesProcess(void)
{
  return RulesProcessEvent(mqtt_data);
}

void RulesInit(void)
{
  rules_flag.data = 0;
  for (byte i = 0; i < MAX_RULE_SETS; i++) {
    if (Settings.rules[i][0] == '\0') {
      bitWrite(Settings.rule_enabled, i, 0);
      bitWrite(Settings.rule_once, i, 0);
    }
  }
  rules_teleperiod = 0;
}

void RulesEvery50ms(void)
{
  if (Settings.rule_enabled) {  // Any rule enabled
    char json_event[120];

    if (-1 == rules_new_power) { rules_new_power = power; }
    if (rules_new_power != rules_old_power) {
      if (rules_old_power != -1) {
        for (byte i = 0; i < devices_present; i++) {
          uint8_t new_state = (rules_new_power >> i) &1;
          if (new_state != ((rules_old_power >> i) &1)) {
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"Power%d\":{\"State\":%d}}"), i +1, new_state);
            RulesProcessEvent(json_event);
          }
        }
      } else {
        // Boot time POWER OUTPUTS (Relays) Status
        for (byte i = 0; i < devices_present; i++) {
          uint8_t new_state = (rules_new_power >> i) &1;
          snprintf_P(json_event, sizeof(json_event), PSTR("{\"Power%d\":{\"Boot\":%d}}"), i +1, new_state);
          RulesProcessEvent(json_event);
        }
        // Boot time SWITCHES Status
        for (byte i = 0; i < MAX_SWITCHES; i++) {
#ifdef USE_TM1638
          if ((pin[GPIO_SWT1 +i] < 99) || ((pin[GPIO_TM16CLK] < 99) && (pin[GPIO_TM16DIO] < 99) && (pin[GPIO_TM16STB] < 99))) {
#else
          if (pin[GPIO_SWT1 +i] < 99) {
#endif // USE_TM1638
            boolean swm = ((FOLLOW_INV == Settings.switchmode[i]) || (PUSHBUTTON_INV == Settings.switchmode[i]) || (PUSHBUTTONHOLD_INV == Settings.switchmode[i]));
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"" D_JSON_SWITCH "%d\":{\"Boot\":%d}}"), i +1, (swm ^ lastwallswitch[i]));
            RulesProcessEvent(json_event);
          }
        }
      }
      rules_old_power = rules_new_power;
    }
    else if (rules_old_dimm != Settings.light_dimmer) {
      if (rules_old_dimm != -1) {
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Dimmer\":{\"State\":%d}}"), Settings.light_dimmer);
      } else {
        // Boot time DIMMER VALUE
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Dimmer\":{\"Boot\":%d}}"), Settings.light_dimmer);
      }
      RulesProcessEvent(json_event);
      rules_old_dimm = Settings.light_dimmer;
    }
    else if (event_data[0]) {
      char *event;
      char *parameter;
      event = strtok_r(event_data, "=", &parameter);     // event_data = fanspeed=10
      if (event) {
        event = Trim(event);
        if (parameter) {
          parameter = Trim(parameter);
        } else {
          parameter = event + strlen(event);  // '\0'
        }
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Event\":{\"%s\":\"%s\"}}"), event, parameter);
        event_data[0] ='\0';
        RulesProcessEvent(json_event);
      } else {
        event_data[0] ='\0';
      }
    }
    else if (rules_flag.data) {
      uint16_t mask = 1;
      for (byte i = 0; i < MAX_RULES_FLAG; i++) {
        if (rules_flag.data & mask) {
          rules_flag.data ^= mask;
          json_event[0] = '\0';
          switch (i) {
            case 0: strncpy_P(json_event, PSTR("{\"System\":{\"Boot\":1}}"), sizeof(json_event)); break;
            case 1: snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Initialized\":%d}}"), GetMinutesPastMidnight()); break;
            case 2: snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Set\":%d}}"), GetMinutesPastMidnight()); break;
            case 3: strncpy_P(json_event, PSTR("{\"MQTT\":{\"Connected\":1}}"), sizeof(json_event)); break;
            case 4: strncpy_P(json_event, PSTR("{\"MQTT\":{\"Disconnected\":1}}"), sizeof(json_event)); break;
            case 5: strncpy_P(json_event, PSTR("{\"WIFI\":{\"Connected\":1}}"), sizeof(json_event)); break;
            case 6: strncpy_P(json_event, PSTR("{\"WIFI\":{\"Disconnected\":1}}"), sizeof(json_event)); break;
          }
          if (json_event[0]) {
            RulesProcessEvent(json_event);
            break;                       // Only service one event within 50mS
          }
        }
        mask <<= 1;
      }
    }
  }
}

uint8_t rules_xsns_index = 0;

void RulesEvery100ms(void)
{
  if (Settings.rule_enabled && (uptime > 4)) {  // Any rule enabled and allow 4 seconds start-up time for sensors (#3811)
    mqtt_data[0] = '\0';
    int tele_period_save = tele_period;
    tele_period = 2;                                   // Do not allow HA updates during next function call
    XsnsNextCall(FUNC_JSON_APPEND, rules_xsns_index);  // ,"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}
    tele_period = tele_period_save;
    if (strlen(mqtt_data)) {
      mqtt_data[0] = '{';                              // {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
      RulesProcess();
    }
  }
}

void RulesEverySecond(void)
{
  if (Settings.rule_enabled) {  // Any rule enabled
    char json_event[120];

    if (RtcTime.valid) {
      if ((uptime > 60) && (RtcTime.minute != rules_last_minute)) {  // Execute from one minute after restart every minute only once
        rules_last_minute = RtcTime.minute;
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Minute\":%d}}"), GetMinutesPastMidnight());
        RulesProcessEvent(json_event);
      }
    }
    for (byte i = 0; i < MAX_RULE_TIMERS; i++) {
      if (rules_timer[i] != 0L) {           // Timer active?
        if (TimeReached(rules_timer[i])) {  // Timer finished?
          rules_timer[i] = 0L;              // Turn off this timer
          snprintf_P(json_event, sizeof(json_event), PSTR("{\"Rules\":{\"Timer\":%d}}"), i +1);
          RulesProcessEvent(json_event);
        }
      }
    }
  }
}

void RulesSetPower(void)
{
  rules_new_power = XdrvMailbox.index;
}

void RulesTeleperiod(void)
{
  rules_teleperiod = 1;
  RulesProcess();
  rules_teleperiod = 0;
}

boolean RulesCommand(void)
{
  char command[CMDSZ];
  boolean serviced = true;
  uint8_t index = XdrvMailbox.index;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kRulesCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if ((CMND_RULE == command_code) && (index > 0) && (index <= MAX_RULE_SETS)) {
    if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.data_len < sizeof(Settings.rules[index -1]))) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 10)) {
        switch (XdrvMailbox.payload) {
        case 0: // Off
        case 1: // On
          bitWrite(Settings.rule_enabled, index -1, XdrvMailbox.payload);
          break;
        case 2: // Toggle
          bitWrite(Settings.rule_enabled, index -1, bitRead(Settings.rule_enabled, index -1) ^1);
          break;
        case 4: // Off
        case 5: // On
          bitWrite(Settings.rule_once, index -1, XdrvMailbox.payload &1);
          break;
        case 6: // Toggle
          bitWrite(Settings.rule_once, index -1, bitRead(Settings.rule_once, index -1) ^1);
          break;
        case 8: // Off
        case 9: // On
          bitWrite(Settings.rule_stop, index -1, XdrvMailbox.payload &1);
          break;
        case 10: // Toggle
          bitWrite(Settings.rule_stop, index -1, bitRead(Settings.rule_stop, index -1) ^1);
          break;
        }
      } else {
        int offset = 0;
        if ('+' == XdrvMailbox.data[0]) {
          offset = strlen(Settings.rules[index -1]);
          if (XdrvMailbox.data_len < (sizeof(Settings.rules[index -1]) - offset -1)) {  // Check free space
            XdrvMailbox.data[0] = ' ';  // Remove + and make sure at least one space is inserted
          } else {
            offset = -1;                // Not enough space so skip it
          }
        }
        if (offset != -1) {
          strlcpy(Settings.rules[index -1] + offset, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data, sizeof(Settings.rules[index -1]));
        }
      }
      rules_triggers[index -1] = 0;  // Reset once flag
    }
    snprintf_P (mqtt_data, sizeof(mqtt_data), PSTR("{\"%s%d\":\"%s\",\"Once\":\"%s\",\"StopOnError\":\"%s\",\"Free\":%d,\"Rules\":\"%s\"}"),
      command, index, GetStateText(bitRead(Settings.rule_enabled, index -1)), GetStateText(bitRead(Settings.rule_once, index -1)),
      GetStateText(bitRead(Settings.rule_stop, index -1)), sizeof(Settings.rules[index -1]) - strlen(Settings.rules[index -1]) -1, Settings.rules[index -1]);
  }
  else if ((CMND_RULETIMER == command_code) && (index > 0) && (index <= MAX_RULE_TIMERS)) {
    if (XdrvMailbox.data_len > 0) {
      rules_timer[index -1] = (XdrvMailbox.payload > 0) ? millis() + (1000 * XdrvMailbox.payload) : 0;
    }
    mqtt_data[0] = '\0';
    for (byte i = 0; i < MAX_RULE_TIMERS; i++) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%c\"T%d\":%d"), mqtt_data, (i) ? ',' : '{', i +1, (rules_timer[i]) ? (rules_timer[i] - millis()) / 1000 : 0);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
  }
  else if (CMND_EVENT == command_code) {
    if (XdrvMailbox.data_len > 0) {
      strlcpy(event_data, XdrvMailbox.data, sizeof(event_data));
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
  }
  else if ((CMND_VAR == command_code) && (index > 0) && (index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      strlcpy(vars[index -1], ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data, sizeof(vars[index -1]));
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, vars[index -1]);
  }
  else if ((CMND_MEM == command_code) && (index > 0) && (index <= MAX_RULE_MEMS)) {
    if (XdrvMailbox.data_len > 0) {
      strlcpy(Settings.mems[index -1], ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data, sizeof(Settings.mems[index -1]));
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.mems[index -1]);
  }
  else if (CMND_CALC_RESOLUTION == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 7)) {
      Settings.flag2.calc_resolution = XdrvMailbox.payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.flag2.calc_resolution);
  }
  else if ((CMND_ADD == command_code) && (index > 0) && (index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      double tempvar = CharToDouble(vars[index -1]) + CharToDouble(XdrvMailbox.data);
      dtostrfd(tempvar, Settings.flag2.calc_resolution, vars[index -1]);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, vars[index -1]);
  }
  else if ((CMND_SUB == command_code) && (index > 0) && (index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      double tempvar = CharToDouble(vars[index -1]) - CharToDouble(XdrvMailbox.data);
      dtostrfd(tempvar, Settings.flag2.calc_resolution, vars[index -1]);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, vars[index -1]);
  }
  else if ((CMND_MULT == command_code) && (index > 0) && (index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      double tempvar = CharToDouble(vars[index -1]) * CharToDouble(XdrvMailbox.data);
      dtostrfd(tempvar, Settings.flag2.calc_resolution, vars[index -1]);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, vars[index -1]);
  }
  else if ((CMND_SCALE == command_code) && (index > 0) && (index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      if (strstr(XdrvMailbox.data, ",")) {     // Process parameter entry
        char sub_string[XdrvMailbox.data_len +1];

        double valueIN = CharToDouble(subStr(sub_string, XdrvMailbox.data, ",", 1));
        double fromLow = CharToDouble(subStr(sub_string, XdrvMailbox.data, ",", 2));
        double fromHigh = CharToDouble(subStr(sub_string, XdrvMailbox.data, ",", 3));
        double toLow = CharToDouble(subStr(sub_string, XdrvMailbox.data, ",", 4));
        double toHigh = CharToDouble(subStr(sub_string, XdrvMailbox.data, ",", 5));
        double value = map_double(valueIN, fromLow, fromHigh, toLow, toHigh);
        dtostrfd(value, Settings.flag2.calc_resolution, vars[index -1]);
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, vars[index -1]);
  }
  else serviced = false;  // Unknown command

  return serviced;
}

double map_double(double x, double in_min, double in_max, double out_min, double out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdrv10(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      RulesInit();
      break;
    case FUNC_EVERY_50_MSECOND:
      RulesEvery50ms();
      break;
    case FUNC_EVERY_100_MSECOND:
      RulesEvery100ms();
      break;
    case FUNC_EVERY_SECOND:
      RulesEverySecond();
      break;
    case FUNC_SET_POWER:
      RulesSetPower();
      break;
    case FUNC_COMMAND:
      result = RulesCommand();
      break;
    case FUNC_RULES_PROCESS:
      result = RulesProcess();
      break;
  }
  return result;
}

#endif  // USE_RULES
