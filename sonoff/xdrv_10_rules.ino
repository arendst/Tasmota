/*
  xdrv_10_rules.ino - rule support for Sonoff-Tasmota

  Copyright (C) 2019  ESP Easy Group and Theo Arends

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
#define D_CMND_SUBSCRIBE "Subscribe"
#define D_CMND_UNSUBSCRIBE "Unsubscribe"

#define D_JSON_INITIATED "Initiated"

#define COMPARE_OPERATOR_NONE            -1
#define COMPARE_OPERATOR_EQUAL            0
#define COMPARE_OPERATOR_BIGGER           1
#define COMPARE_OPERATOR_SMALLER          2
#define COMPARE_OPERATOR_EXACT_DIVISION   3
#define COMPARE_OPERATOR_NUMBER_EQUAL     4
#define COMPARE_OPERATOR_NOT_EQUAL        5
#define COMPARE_OPERATOR_BIGGER_EQUAL     6
#define COMPARE_OPERATOR_SMALLER_EQUAL    7
#define MAXIMUM_COMPARE_OPERATOR          COMPARE_OPERATOR_SMALLER_EQUAL
const char kCompareOperators[] PROGMEM = "=\0>\0<\0|\0==!=>=<=";

#ifdef USE_EXPRESSION
  #include <LinkedList.h>                 // Import LinkedList library

  const char kExpressionOperators[] PROGMEM = "+-*/%^";
  #define EXPRESSION_OPERATOR_ADD         0
  #define EXPRESSION_OPERATOR_SUBTRACT    1
  #define EXPRESSION_OPERATOR_MULTIPLY    2
  #define EXPRESSION_OPERATOR_DIVIDEDBY   3
  #define EXPRESSION_OPERATOR_MODULO      4
  #define EXPRESSION_OPERATOR_POWER       5

  const uint8_t kExpressionOperatorsPriorities[] PROGMEM = {1, 1, 2, 2, 3, 4};
  #define MAX_EXPRESSION_OPERATOR_PRIORITY    4
#endif  // USE_EXPRESSION

enum RulesCommands { CMND_RULE, CMND_RULETIMER, CMND_EVENT, CMND_VAR, CMND_MEM, CMND_ADD, CMND_SUB, CMND_MULT, CMND_SCALE, CMND_CALC_RESOLUTION, CMND_SUBSCRIBE, CMND_UNSUBSCRIBE };
const char kRulesCommands[] PROGMEM = D_CMND_RULE "|" D_CMND_RULETIMER "|" D_CMND_EVENT "|" D_CMND_VAR "|" D_CMND_MEM "|" D_CMND_ADD "|" D_CMND_SUB "|" D_CMND_MULT "|" D_CMND_SCALE "|" D_CMND_CALC_RESOLUTION "|" D_CMND_SUBSCRIBE "|" D_CMND_UNSUBSCRIBE ;

#ifdef SUPPORT_MQTT_EVENT
  #include <LinkedList.h>                 // Import LinkedList library
  typedef struct {
    String Event;
    String Topic;
    String Key;
  } MQTT_Subscription;
  LinkedList<MQTT_Subscription> subscriptions;
#endif    //SUPPORT_MQTT_EVENT

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
#if (MAX_RULE_VARS>16)
#error MAX_RULE_VARS is bigger than 16
#endif
#if (MAX_RULE_MEMS>5)
#error MAX_RULE_MEMS is bigger than 5
#endif
uint16_t vars_event = 0;
uint8_t mems_event = 0;

/*******************************************************************************************/

bool RulesRuleMatch(uint8_t rule_set, String &event, String &rule)
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

  char compare_operator[3];
  int8_t compare = COMPARE_OPERATOR_NONE;
  for (int8_t i = MAXIMUM_COMPARE_OPERATOR; i >= 0; i--) {
    snprintf_P(compare_operator, sizeof(compare_operator), kCompareOperators + (i *2));
    if ((pos = rule_name.indexOf(compare_operator)) > 0) {
      compare = i;
      break;
    }
  }

  char rule_svalue[CMDSZ] = { 0 };
  double rule_value = 0;
  if (compare != COMPARE_OPERATOR_NONE) {
    String rule_param = rule_name.substring(pos + strlen(compare_operator));
    for (uint8_t i = 0; i < MAX_RULE_VARS; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%%VAR%d%%"), i +1);
      if (rule_param.startsWith(stemp)) {
        rule_param = vars[i];
        break;
      }
    }
    for (uint8_t i = 0; i < MAX_RULE_MEMS; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%%MEM%d%%"), i +1);
      if (rule_param.startsWith(stemp)) {
        rule_param = Settings.mems[i];
        break;
      }
    }
    snprintf_P(stemp, sizeof(stemp), PSTR("%%TIME%%"));
    if (rule_param.startsWith(stemp)) {
      rule_param = String(MinutesPastMidnight());
    }
    snprintf_P(stemp, sizeof(stemp), PSTR("%%UPTIME%%"));
    if (rule_param.startsWith(stemp)) {
      rule_param = String(MinutesUptime());
    }
    snprintf_P(stemp, sizeof(stemp), PSTR("%%TIMESTAMP%%"));
    if (rule_param.startsWith(stemp)) {
      rule_param = GetDateAndTime(DT_LOCAL).c_str();
    }
#if defined(USE_TIMERS) && defined(USE_SUNRISE)
    snprintf_P(stemp, sizeof(stemp), PSTR("%%SUNRISE%%"));
    if (rule_param.startsWith(stemp)) {
      rule_param = String(SunMinutes(0));
    }
    snprintf_P(stemp, sizeof(stemp), PSTR("%%SUNSET%%"));
    if (rule_param.startsWith(stemp)) {
      rule_param = String(SunMinutes(1));
    }
#endif  // USE_TIMERS and USE_SUNRISE
    rule_param.toUpperCase();
    strlcpy(rule_svalue, rule_param.c_str(), sizeof(rule_svalue));

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

//AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Task %s, Name %s, Value |%s|, TrigCnt %d, TrigSt %d, Source %s, Json %s"),
//  rule_task.c_str(), rule_name.c_str(), rule_svalue, rules_trigger_count[rule_set], bitRead(rules_triggers[rule_set], rules_trigger_count[rule_set]), event.c_str(), (str_value) ? str_value : "none");

  if (!root[rule_task][rule_name].success()) { return false; }
  // No value but rule_name is ok

  rules_event_value = str_value;                       // Prepare %value%

  // Step 3: Compare rule (value)
  if (str_value) {
    value = CharToDouble((char*)str_value);
    int int_value = int(value);
    int int_rule_value = int(rule_value);
    switch (compare) {
      case COMPARE_OPERATOR_EXACT_DIVISION:
        match = (int_rule_value && (int_value % int_rule_value) == 0);
        break;
      case COMPARE_OPERATOR_EQUAL:
        match = (!strcasecmp(str_value, rule_svalue));  // Compare strings - this also works for hexadecimals
        break;
      case COMPARE_OPERATOR_BIGGER:
        match = (value > rule_value);
        break;
      case COMPARE_OPERATOR_SMALLER:
        match = (value < rule_value);
        break;
      case COMPARE_OPERATOR_NUMBER_EQUAL:
        match = (value == rule_value);
        break;
      case COMPARE_OPERATOR_NOT_EQUAL:
        match = (value != rule_value);
        break;
      case COMPARE_OPERATOR_BIGGER_EQUAL:
        match = (value >= rule_value);
        break;
      case COMPARE_OPERATOR_SMALLER_EQUAL:
        match = (value <= rule_value);
        break;
      default:
        match = true;
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

bool RuleSetProcess(uint8_t rule_set, String &event_saved)
{
  bool serviced = false;
  char stemp[10];

  delay(0);                                               // Prohibit possible loop software watchdog

//AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Event = %s, Rule = %s"), event_saved.c_str(), Settings.rules[rule_set]);

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

//AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Event |%s|, Rule |%s|, Command(s) |%s|"), event.c_str(), event_trigger.c_str(), commands.c_str());

    if (RulesRuleMatch(rule_set, event, event_trigger)) {
      commands.trim();
      String ucommand = commands;
      ucommand.toUpperCase();
//      if (!ucommand.startsWith("BACKLOG")) { commands = "backlog " + commands; }  // Always use Backlog to prevent power race exception
      if (ucommand.indexOf("EVENT ") != -1) { commands = "backlog " + commands; }  // Always use Backlog with event to prevent rule event loop exception
      commands.replace(F("%value%"), rules_event_value);
      for (uint8_t i = 0; i < MAX_RULE_VARS; i++) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%%var%d%%"), i +1);
        commands.replace(stemp, vars[i]);
      }
      for (uint8_t i = 0; i < MAX_RULE_MEMS; i++) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%%mem%d%%"), i +1);
        commands.replace(stemp, Settings.mems[i]);
      }
      commands.replace(F("%time%"), String(MinutesPastMidnight()));
      commands.replace(F("%uptime%"), String(MinutesUptime()));
      commands.replace(F("%timestamp%"), GetDateAndTime(DT_LOCAL).c_str());
#if defined(USE_TIMERS) && defined(USE_SUNRISE)
      commands.replace(F("%sunrise%"), String(SunMinutes(0)));
      commands.replace(F("%sunset%"), String(SunMinutes(1)));
#endif  // USE_TIMERS and USE_SUNRISE

      char command[commands.length() +1];
      strlcpy(command, commands.c_str(), sizeof(command));

      AddLog_P2(LOG_LEVEL_INFO, PSTR("RUL: %s performs \"%s\""), event_trigger.c_str(), command);

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

#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("RulesProcessEvent"));
#endif

  String event_saved = json_event;
  event_saved.toUpperCase();

//AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Event %s"), event_saved.c_str());

  for (uint8_t i = 0; i < MAX_RULE_SETS; i++) {
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
  for (uint8_t i = 0; i < MAX_RULE_SETS; i++) {
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
        for (uint8_t i = 0; i < devices_present; i++) {
          uint8_t new_state = (rules_new_power >> i) &1;
          if (new_state != ((rules_old_power >> i) &1)) {
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"Power%d\":{\"State\":%d}}"), i +1, new_state);
            RulesProcessEvent(json_event);
          }
        }
      } else {
        // Boot time POWER OUTPUTS (Relays) Status
        for (uint8_t i = 0; i < devices_present; i++) {
          uint8_t new_state = (rules_new_power >> i) &1;
          snprintf_P(json_event, sizeof(json_event), PSTR("{\"Power%d\":{\"Boot\":%d}}"), i +1, new_state);
          RulesProcessEvent(json_event);
        }
        // Boot time SWITCHES Status
        for (uint8_t i = 0; i < MAX_SWITCHES; i++) {
#ifdef USE_TM1638
          if ((pin[GPIO_SWT1 +i] < 99) || ((pin[GPIO_TM16CLK] < 99) && (pin[GPIO_TM16DIO] < 99) && (pin[GPIO_TM16STB] < 99))) {
#else
          if (pin[GPIO_SWT1 +i] < 99) {
#endif // USE_TM1638
            bool swm = ((FOLLOW_INV == Settings.switchmode[i]) || (PUSHBUTTON_INV == Settings.switchmode[i]) || (PUSHBUTTONHOLD_INV == Settings.switchmode[i]));
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"" D_JSON_SWITCH "%d\":{\"Boot\":%d}}"), i +1, (swm ^ SwitchLastState(i)));
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
    else if (vars_event) {
      for (uint8_t i = 0; i < MAX_RULE_VARS-1; i++) {
        if (bitRead(vars_event, i)) {
          bitClear(vars_event, i);
          snprintf_P(json_event, sizeof(json_event), PSTR("{\"Var%d\":{\"State\":%s}}"), i+1, vars[i]);
          RulesProcessEvent(json_event);
          break;
        }
      }
    }
    else if (mems_event) {
      for (uint8_t i = 0; i < MAX_RULE_MEMS-1; i++) {
        if (bitRead(mems_event, i)) {
          bitClear(mems_event, i);
          snprintf_P(json_event, sizeof(json_event), PSTR("{\"Mem%d\":{\"State\":%s}}"), i+1, Settings.mems[i]);
          RulesProcessEvent(json_event);
          break;
        }
      }
    }
    else if (rules_flag.data) {
      uint16_t mask = 1;
      for (uint8_t i = 0; i < MAX_RULES_FLAG; i++) {
        if (rules_flag.data & mask) {
          rules_flag.data ^= mask;
          json_event[0] = '\0';
          switch (i) {
            case 0: strncpy_P(json_event, PSTR("{\"System\":{\"Boot\":1}}"), sizeof(json_event)); break;
            case 1: snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Initialized\":%d}}"), MinutesPastMidnight()); break;
            case 2: snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Set\":%d}}"), MinutesPastMidnight()); break;
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
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Minute\":%d}}"), MinutesPastMidnight());
        RulesProcessEvent(json_event);
      }
    }
    for (uint8_t i = 0; i < MAX_RULE_TIMERS; i++) {
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

#ifdef SUPPORT_MQTT_EVENT
/********************************************************************************************/
/*
 * Rules: Process received MQTT message.
 *        If the message is in our subscription list, trigger an event with the value parsed from MQTT data
 * Input:
 *      void      - We are going to access XdrvMailbox data directly.
 * Return:
 *      true      - The message is consumed.
 *      false     - The message is not in our list.
 */
bool RulesMqttData(void)
{
  bool serviced = false;
  if (XdrvMailbox.data_len < 1 || XdrvMailbox.data_len > 128) {
    return false;
  }
  String sTopic = XdrvMailbox.topic;
  String sData = XdrvMailbox.data;
  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: MQTT Topic %s, Event %s"), XdrvMailbox.topic, XdrvMailbox.data);
  MQTT_Subscription event_item;
  //Looking for matched topic
  for (int index = 0; index < subscriptions.size(); index++) {
    event_item = subscriptions.get(index);

    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Match MQTT message Topic %s with subscription topic %s"), sTopic.c_str(), event_item.Topic.c_str());
    if (sTopic.startsWith(event_item.Topic)) {
      //This topic is subscribed by us, so serve it
      serviced = true;
      String value;
      if (event_item.Key.length() == 0) {   //If did not specify Key
        value = sData;
      } else {      //If specified Key, need to parse Key/Value from JSON data
        StaticJsonBuffer<400> jsonBuf;
        JsonObject& jsonData = jsonBuf.parseObject(sData);
        String key1 = event_item.Key;
        String key2;
        if (!jsonData.success()) break;       //Failed to parse JSON data, ignore this message.
        int dot;
        if ((dot = key1.indexOf('.')) > 0) {
          key2 = key1.substring(dot+1);
          key1 = key1.substring(0, dot);
          if (!jsonData[key1][key2].success()) break;   //Failed to get the key/value, ignore this message.
          value = (const char *)jsonData[key1][key2];
        } else {
          if (!jsonData[key1].success()) break;
          value = (const char *)jsonData[key1];
        }
      }
      value.trim();
      //Create an new event. Cannot directly call RulesProcessEvent().
      snprintf_P(event_data, sizeof(event_data), PSTR("%s=%s"), event_item.Event.c_str(), value.c_str());
    }
  }
  return serviced;
}

/********************************************************************************************/
/*
 * Subscribe a MQTT topic (with or without key) and assign an event name to it
 * Command Subscribe format:
 *      Subscribe <event_name>, <topic> [, <key>]
 *        This command will subscribe a <topic> and give it an event name <event_name>.
 *        The optional parameter <key> is for parse the specified key/value from MQTT message
 *            payload with JSON format.
 *      Subscribe
 *        Subscribe command without any parameter will list all topics currently subscribed.
 * Input:
 *      data      - A char buffer with all the parameters
 *      data_len  - Length of the parameters
 * Return:
 *      A string include subscribed event, topic and key.
 */
String RulesSubscribe(const char *data, int data_len)
{
  MQTT_Subscription subscription_item;
  String events;
  if (data_len > 0) {
    char parameters[data_len+1];
    memcpy(parameters, data, data_len);
    parameters[data_len] = '\0';
    String event_name, topic, key;

    char * pos = strtok(parameters, ",");
    if (pos) {
      event_name = Trim(pos);
      pos = strtok(NULL, ",");
      if (pos) {
        topic = Trim(pos);
        pos = strtok(NULL, ",");
        if (pos) {
          key = Trim(pos);
        }
      }
    }
    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Subscribe command with parameters: %s, %s, %s."), event_name.c_str(), topic.c_str(), key.c_str());
    event_name.toUpperCase();
    if (event_name.length() > 0 && topic.length() > 0) {
      //Search all subscriptions
      for (int index=0; index < subscriptions.size(); index++) {
        if (subscriptions.get(index).Event.equals(event_name)) {
          //If find exists one, remove it.
          String stopic = subscriptions.get(index).Topic + "/#";
          MqttUnsubscribe(stopic.c_str());
          subscriptions.remove(index);
          break;
        }
      }
      //Add "/#" to the topic
      if (!topic.endsWith("#")) {
        if (topic.endsWith("/")) {
          topic.concat("#");
        } else {
          topic.concat("/#");
        }
      }
      //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: New topic: %s."), topic.c_str());
      //MQTT Subscribe
      subscription_item.Event = event_name;
      subscription_item.Topic = topic.substring(0, topic.length() - 2);   //Remove "/#" so easy to match
      subscription_item.Key = key;
      subscriptions.add(subscription_item);

      MqttSubscribe(topic.c_str());
      events.concat(event_name + "," + topic
        + (key.length()>0 ? "," : "")
        + key);
    } else {
      events = D_JSON_WRONG_PARAMETERS;
    }
  } else {
    //If did not specify the event name, list all subscribed event
    for (int index=0; index < subscriptions.size(); index++) {
      subscription_item = subscriptions.get(index);
      events.concat(subscription_item.Event + "," + subscription_item.Topic
        + (subscription_item.Key.length()>0 ? "," : "")
        + subscription_item.Key + "; ");
    }
  }
  return events;
}

/********************************************************************************************/
/*
 * Unsubscribe specified MQTT event. If no event specified, Unsubscribe all.
 * Command Unsubscribe format:
 *      Unsubscribe [<event_name>]
 * Input:
 *      data      - Event name
 *      data_len  - Length of the parameters
 * Return:
 *      list all the events unsubscribed.
 */
String RulesUnsubscribe(const char * data, int data_len)
{
  MQTT_Subscription subscription_item;
  String events;
  if (data_len > 0) {
    for (int index = 0; index < subscriptions.size(); index++) {
      subscription_item = subscriptions.get(index);
      if (subscription_item.Event.equalsIgnoreCase(data)) {
        String stopic = subscription_item.Topic + "/#";
        MqttUnsubscribe(stopic.c_str());
        events = subscription_item.Event;
        subscriptions.remove(index);
        break;
      }
    }
  } else {
    //If did not specify the event name, unsubscribe all event
    String stopic;
    while (subscriptions.size() > 0) {
      events.concat(subscriptions.get(0).Event + "; ");
      stopic = subscriptions.get(0).Topic + "/#";
      MqttUnsubscribe(stopic.c_str());
      subscriptions.remove(0);
    }
  }
  return events;
}
#endif //     SUPPORT_MQTT_EVENT

#ifdef USE_EXPRESSION
/********************************************************************************************/
/*
 * Parse a number value
 * Input:
 *      pNumber     - A char pointer point to a digit started string (guaranteed)
 *      value       - Reference a double variable used to accept the result
 * Output:
 *      pNumber     - Pointer forward to next character after the number
 *      value       - double type, the result value
 * Return:
 *      true    - succeed
 *      false   - failed
 */
bool findNextNumber(char * &pNumber, double &value)
{
  bool bSucceed = false;
  String sNumber = "";
  while (*pNumber) {
    if (isdigit(*pNumber) || (*pNumber == '.')) {
      sNumber += *pNumber;
      pNumber++;
    } else {
      break;
    }
  }
  if (sNumber.length() > 0) {
    value = CharToDouble(sNumber.c_str());
    bSucceed = true;
  }
  return bSucceed;
}

/********************************************************************************************/
/*
 * Parse a variable (like VAR1, MEM3) and get its value (double type)
 * Input:
 *      pVarname    - A char pointer point to a variable name string
 *      value       - Reference a double variable used to accept the result
 * Output:
 *      pVarname    - Pointer forward to next character after the variable
 *      value       - double type, the result value
 * Return:
 *      true    - succeed
 *      false   - failed
 */
bool findNextVariableValue(char * &pVarname, double &value)
{
  bool succeed = true;
  value = 0;
  String sVarName = "";
  while (*pVarname) {
    if (isalpha(*pVarname) || isdigit(*pVarname)) {
      sVarName.concat(*pVarname);
      pVarname++;
    } else {
      break;
    }
  }
  sVarName.toUpperCase();
  if (sVarName.startsWith(F("VAR"))) {
    int index = sVarName.substring(3).toInt();
    if (index > 0 && index <= MAX_RULE_VARS) {
      value = CharToDouble(vars[index -1]);
    }
  } else if (sVarName.startsWith(F("MEM"))) {
    int index = sVarName.substring(3).toInt();
    if (index > 0 && index <= MAX_RULE_MEMS) {
      value = CharToDouble(Settings.mems[index -1]);
    }
  } else if (sVarName.equals(F("TIME"))) {
    value = MinutesPastMidnight();
  } else if (sVarName.equals(F("UPTIME"))) {
    value = MinutesUptime();
  } else if (sVarName.equals(F("UTCTIME"))) {
    value = UtcTime();
  } else if (sVarName.equals(F("LOCALTIME"))) {
    value = LocalTime();
#if defined(USE_TIMERS) && defined(USE_SUNRISE)
  } else if (sVarName.equals(F("SUNRISE"))) {
    value = SunMinutes(0);
  } else if (sVarName.equals(F("SUNSET"))) {
    value = SunMinutes(1);
#endif
  } else {
    succeed = false;
  }

  return succeed;
}

/********************************************************************************************/
/*
 * Find next object in expression and evaluate it
 *     An object could be:
 *     - A float number start with a digit, like 0.787
 *     - A variable name, like VAR1, MEM3
 *     - An expression enclosed with a pair of round brackets, (.....)
 * Input:
 *      pointer     - A char pointer point to a place of the expression string
 *      value       - Reference a double variable used to accept the result
 * Output:
 *      pointer     - Pointer forward to next character after next object
 *      value       - double type, the result value
 * Return:
 *      true    - succeed
 *      false   - failed
 */
bool findNextObjectValue(char * &pointer, double &value)
{
  bool bSucceed = false;
  while (*pointer)
  {
    if (isspace(*pointer)) {      //Skip leading spaces
      pointer++;
      continue;
    }
    if (isdigit(*pointer)) {      //This object is a number
      bSucceed = findNextNumber(pointer, value);
      break;
    } else if (isalpha(*pointer)) {     //Should be a variable like VAR12, MEM1
      bSucceed = findNextVariableValue(pointer, value);
      break;
    } else if (*pointer == '(') {     //It is a sub expression bracketed with ()
      pointer++;
      char * sub_exp_start = pointer; //Find out the sub expression between a pair of parenthesis. "()"
      unsigned int sub_exp_len = 0;
      //Look for the matched closure parenthesis.")"
      bool bFindClosures = false;
      uint8_t matchClosures = 1;
      while (*pointer)
      {
        if (*pointer == ')') {
          matchClosures--;
          if (matchClosures == 0) {
            sub_exp_len = pointer - sub_exp_start;
            bFindClosures = true;
            break;
          }
        } else if (*pointer == '(') {
          matchClosures++;
        }
        pointer++;
      }
      if (bFindClosures) {
        value = evaluateExpression(sub_exp_start, sub_exp_len);
        bSucceed = true;
      }
      break;
    } else {          //No number, no variable, no expression, then invalid object.
      break;
    }
  }
  return bSucceed;
}

/********************************************************************************************/
/*
 * Find next operator in expression
 *     An operator could be: +, - , * , / , %, ^
 * Input:
 *      pointer     - A char pointer point to a place of the expression string
 *      op          - Reference to a variable used to accept the result
 * Output:
 *      pointer     - Pointer forward to next character after next operator
 *      op          - The operator. 0, 1, 2, 3, 4, 5
 * Return:
 *      true    - succeed
 *      false   - failed
 */
bool findNextOperator(char * &pointer, int8_t &op)
{
  bool bSucceed = false;
  while (*pointer)
  {
    if (isspace(*pointer)) {      //Skip leading spaces
      pointer++;
      continue;
    }
    if (char *pch = strchr(kExpressionOperators, *pointer)) {      //If it is an operator
      op = (int8_t)(pch - kExpressionOperators);
      pointer++;
      bSucceed = true;
    }
    break;
  }
  return bSucceed;
}
/********************************************************************************************/
/*
 * Calculate a simple expression composed by 2 value and 1 operator, like 2 * 3
 * Input:
 *      pointer     - A char pointer point to a place of the expression string
 *      value       - Reference a double variable used to accept the result
 * Output:
 *      pointer     - Pointer forward to next character after next object
 *      value       - double type, the result value
 * Return:
 *      true    - succeed
 *      false   - failed
 */
double calculateTwoValues(double v1, double v2, uint8_t op)
{
  switch (op)
  {
    case EXPRESSION_OPERATOR_ADD:
      return v1 + v2;
    case EXPRESSION_OPERATOR_SUBTRACT:
      return v1 - v2;
    case EXPRESSION_OPERATOR_MULTIPLY:
      return v1 * v2;
    case EXPRESSION_OPERATOR_DIVIDEDBY:
      return (0 == v2) ? 0 : (v1 / v2);
    case EXPRESSION_OPERATOR_MODULO:
      return (0 == v2) ? 0 : (int(v1) % int(v2));
    case EXPRESSION_OPERATOR_POWER:
      return FastPrecisePow(v1, v2);
  }
  return 0;
}

/********************************************************************************************/
/*
 * Parse and evaluate an expression.
 * For example: "10 * ( MEM2 + 1) / 2"
 * Right now, only support operators listed here:  (order by priority)
 *      Priority 4: ^ (power)
 *      Priority 3: % (modulo, always get integer result)
 *      Priority 2: *, /
 *      Priority 1: +, -
 * Input:
 *      expression  - The expression to be evaluated
 *      len         - Length of the expression
 * Return:
 *      double      - result.
 *      0           - if the expression is invalid
 * An example:
 * MEM1 = 3, MEM2 = 6, VAR2 = 15, VAR10 = 80
 * At beginning, the expression might be complicated like: 3.14 * (MEM1 * (10 + VAR2 ^2) - 100) % 10 + VAR10 / (2 + MEM2)
 * We are going to scan the whole expression, evaluate each object.
 * Finally we will have a value list:.
 * Order          Object                              Value
 *  0             3.14                                3.14
 *  1             (MEM1 * (10 + VAR2 ^2) - 100)       605
 *  2             10                                  10
 *  3             VAR10                               80
 *  4             (2 + MEM2)                          8
 * And an operator list:
 * Order          Operator                            Priority
 *  0             *                                   2
 *  1             %                                   3
 *  2             +                                   1
 *  3             /                                   2
 */
double evaluateExpression(const char * expression, unsigned int len)
{
  char expbuf[len + 1];
  memcpy(expbuf, expression, len);
  expbuf[len] = '\0';
  char * scan_pointer = expbuf;

  LinkedList<double> object_values;
  LinkedList<int8_t> operators;
  int8_t op;
  double va;
  //Find and add the value of first object
  if (findNextObjectValue(scan_pointer, va)) {
    object_values.add(va);
  } else {
    return 0;
  }
  while (*scan_pointer)
  {
    if (findNextOperator(scan_pointer, op)
        && *scan_pointer
        && findNextObjectValue(scan_pointer, va))
    {
      operators.add(op);
      object_values.add(va);
    } else {
      //No operator followed or no more object after this operator, we done.
      break;
    }
  }

  //Going to evaluate the whole expression
  //Calculate by order of operator priorities. Looking for all operators with specified priority (from High to Low)
  for (int8_t priority = MAX_EXPRESSION_OPERATOR_PRIORITY; priority>0; priority--) {
    int index = 0;
    while (index < operators.size()) {
      if (priority == kExpressionOperatorsPriorities[(operators.get(index))]) {     //need to calculate the operator first
        //get current object value and remove the next object with current operator
        va = calculateTwoValues(object_values.get(index), object_values.remove(index + 1), operators.remove(index));
        //Replace the current value with the result
        object_values.set(index, va);
      } else {
        index++;
      }
    }
  }
  return object_values.get(0);
}
#endif          //USE_EXPRESSION

bool RulesCommand(void)
{
  char command[CMDSZ];
  bool serviced = true;
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
#ifdef USE_EXPRESSION
      double timer_set = evaluateExpression(XdrvMailbox.data, XdrvMailbox.data_len);
      rules_timer[index -1] = (timer_set > 0) ? millis() + (1000 * timer_set) : 0;
#else
      rules_timer[index -1] = (XdrvMailbox.payload > 0) ? millis() + (1000 * XdrvMailbox.payload) : 0;
#endif      //USE_EXPRESSION
    }
    mqtt_data[0] = '\0';
    for (uint8_t i = 0; i < MAX_RULE_TIMERS; i++) {
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
#ifdef USE_EXPRESSION
      dtostrfd(evaluateExpression(XdrvMailbox.data, XdrvMailbox.data_len), Settings.flag2.calc_resolution, vars[index -1]);
#else
      strlcpy(vars[index -1], ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data, sizeof(vars[index -1]));
#endif      //USE_EXPRESSION
      bitSet(vars_event, index -1);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, vars[index -1]);
  }
  else if ((CMND_MEM == command_code) && (index > 0) && (index <= MAX_RULE_MEMS)) {
    if (XdrvMailbox.data_len > 0) {
#ifdef USE_EXPRESSION
      dtostrfd(evaluateExpression(XdrvMailbox.data, XdrvMailbox.data_len), Settings.flag2.calc_resolution, Settings.mems[index -1]);
#else
      strlcpy(Settings.mems[index -1], ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data, sizeof(Settings.mems[index -1]));
#endif      //USE_EXPRESSION
      bitSet(mems_event, index -1);
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
      bitSet(vars_event, index -1);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, vars[index -1]);
  }
  else if ((CMND_SUB == command_code) && (index > 0) && (index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      double tempvar = CharToDouble(vars[index -1]) - CharToDouble(XdrvMailbox.data);
      dtostrfd(tempvar, Settings.flag2.calc_resolution, vars[index -1]);
      bitSet(vars_event, index -1);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, vars[index -1]);
  }
  else if ((CMND_MULT == command_code) && (index > 0) && (index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      double tempvar = CharToDouble(vars[index -1]) * CharToDouble(XdrvMailbox.data);
      dtostrfd(tempvar, Settings.flag2.calc_resolution, vars[index -1]);
      bitSet(vars_event, index -1);
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
        bitSet(vars_event, index -1);
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, vars[index -1]);
#ifdef SUPPORT_MQTT_EVENT
  } else if (CMND_SUBSCRIBE == command_code) {			//MQTT Subscribe command. Subscribe <Event>, <Topic> [, <Key>]
    String result = RulesSubscribe(XdrvMailbox.data, XdrvMailbox.data_len);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, result.c_str());
  } else if (CMND_UNSUBSCRIBE == command_code) {			//MQTT Un-subscribe command. UnSubscribe <Event>
    String result = RulesUnsubscribe(XdrvMailbox.data, XdrvMailbox.data_len);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, result.c_str());
#endif        //SUPPORT_MQTT_EVENT
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

bool Xdrv10(uint8_t function)
{
  bool result = false;

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
#ifdef SUPPORT_MQTT_EVENT
    case FUNC_MQTT_DATA:
      result = RulesMqttData();
      break;
#endif    //SUPPORT_MQTT_EVENT
  }
  return result;
}

#endif  // USE_RULES
