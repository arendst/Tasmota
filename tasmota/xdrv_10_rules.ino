/*
  xdrv_10_rules.ino - rule support for Tasmota

  Copyright (C) 2020  ESP Easy Group and Theo Arends

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
#ifndef USE_SCRIPT
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
#define D_CMND_IF "If"

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

  const char kExpressionOperators[] PROGMEM = "+-*/%^\0";
  #define EXPRESSION_OPERATOR_ADD         0
  #define EXPRESSION_OPERATOR_SUBTRACT    1
  #define EXPRESSION_OPERATOR_MULTIPLY    2
  #define EXPRESSION_OPERATOR_DIVIDEDBY   3
  #define EXPRESSION_OPERATOR_MODULO      4
  #define EXPRESSION_OPERATOR_POWER       5

  const uint8_t kExpressionOperatorsPriorities[] PROGMEM = {1, 1, 2, 2, 3, 4};
  #define MAX_EXPRESSION_OPERATOR_PRIORITY    4


  #define LOGIC_OPERATOR_AND        1
  #define LOGIC_OPERATOR_OR         2

  #define IF_BLOCK_INVALID        -1
  #define IF_BLOCK_ANY            0
  #define IF_BLOCK_ELSEIF         1
  #define IF_BLOCK_ELSE           2
  #define IF_BLOCK_ENDIF          3
#endif  // USE_EXPRESSION

const char kRulesCommands[] PROGMEM = "|"  // No prefix
  D_CMND_RULE "|" D_CMND_RULETIMER "|" D_CMND_EVENT "|" D_CMND_VAR "|" D_CMND_MEM "|"
  D_CMND_ADD "|"  D_CMND_SUB "|" D_CMND_MULT "|" D_CMND_SCALE "|" D_CMND_CALC_RESOLUTION
#ifdef SUPPORT_MQTT_EVENT
  "|" D_CMND_SUBSCRIBE "|" D_CMND_UNSUBSCRIBE
#endif
#ifdef SUPPORT_IF_STATEMENT
  "|" D_CMND_IF
#endif
  ;

void (* const RulesCommand[])(void) PROGMEM = {
  &CmndRule, &CmndRuleTimer, &CmndEvent, &CmndVariable, &CmndMemory,
  &CmndAddition, &CmndSubtract, &CmndMultiply, &CmndScale, &CmndCalcResolution
#ifdef SUPPORT_MQTT_EVENT
  , &CmndSubscribe, &CmndUnsubscribe
#endif
#ifdef SUPPORT_IF_STATEMENT
  , &CmndIf
#endif
  };

#ifdef SUPPORT_MQTT_EVENT
  #include <LinkedList.h>                 // Import LinkedList library
  typedef struct {
    String Event;
    String Topic;
    String Key;
  } MQTT_Subscription;
  LinkedList<MQTT_Subscription> subscriptions;
#endif  // SUPPORT_MQTT_EVENT

struct RULES {
  String event_value;
  unsigned long timer[MAX_RULE_TIMERS] = { 0 };
  uint32_t triggers[MAX_RULE_SETS] = { 0 };
  uint8_t trigger_count[MAX_RULE_SETS] = { 0 };

  long new_power = -1;
  long old_power = -1;
  long old_dimm = -1;

  uint16_t last_minute = 60;
  uint16_t vars_event = 0;
  uint8_t mems_event = 0;
  bool teleperiod = false;

  char event_data[100];
} Rules;

char rules_vars[MAX_RULE_VARS][33] = {{ 0 }};

#if (MAX_RULE_VARS>16)
#error MAX_RULE_VARS is bigger than 16
#endif
#if (MAX_RULE_MEMS>16)
#error MAX_RULE_MEMS is bigger than 16
#endif

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
  if (Rules.teleperiod) {
    int ppos = rule_task.indexOf("TELE-");             // "TELE-INA219" or "INA219"
    if (ppos == -1) { return false; }                  // No pre-amble in rule
    rule_task = rule.substring(5, pos);                // "INA219" or "SYSTEM"
  }

  String rule_expr = rule.substring(pos +1);           // "CURRENT>0.100" or "BOOT" or "%var1%" or "MINUTE|5"
  String rule_name, rule_param;
  int8_t compareOperator = parseCompareExpression(rule_expr, rule_name, rule_param);    //Parse the compare expression.Return operator and the left, right part of expression

  char rule_svalue[80] = { 0 };
  float rule_value = 0;
  if (compareOperator != COMPARE_OPERATOR_NONE) {
    for (uint32_t i = 0; i < MAX_RULE_VARS; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%%VAR%d%%"), i +1);
      if (rule_param.startsWith(stemp)) {
        rule_param = rules_vars[i];
        break;
      }
    }
    for (uint32_t i = 0; i < MAX_RULE_MEMS; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("%%MEM%d%%"), i +1);
      if (rule_param.startsWith(stemp)) {
        rule_param = SettingsText(SET_MEM1 + i);
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
      rule_value = CharToFloat((char*)rule_svalue);   // 0.1      - This saves 9k code over toFLoat()!
    }
  }

  // Step2: Search rule_task and rule_name
  StaticJsonBuffer<1024> jsonBuf;
  JsonObject &root = jsonBuf.parseObject(event);
  if (!root.success()) { return false; }               // No valid JSON data

  const char* str_value;
  if ((pos = rule_name.indexOf("[")) > 0) {            // "CURRENT[1]"
    int rule_name_idx = rule_name.substring(pos +1).toInt();
    if ((rule_name_idx < 1) || (rule_name_idx > 6)) {  // Allow indexes 1 to 6
      rule_name_idx = 1;
    }
    rule_name = rule_name.substring(0, pos);           // "CURRENT"
    str_value = root[rule_task][rule_name][rule_name_idx -1];  // "ENERGY" and "CURRENT" and 0
  } else {
    str_value = root[rule_task][rule_name];            // "INA219" and "CURRENT"
  }

//AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Task %s, Name %s, Value |%s|, TrigCnt %d, TrigSt %d, Source %s, Json %s"),
//  rule_task.c_str(), rule_name.c_str(), rule_svalue, Rules.trigger_count[rule_set], bitRead(Rules.triggers[rule_set], Rules.trigger_count[rule_set]), event.c_str(), (str_value) ? str_value : "none");

  if (!root[rule_task][rule_name].success()) { return false; }
  // No value but rule_name is ok

  Rules.event_value = str_value;                       // Prepare %value%

  // Step 3: Compare rule (value)
  float value = 0;
  if (str_value) {
    value = CharToFloat((char*)str_value);
    int int_value = int(value);
    int int_rule_value = int(rule_value);
    switch (compareOperator) {
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
      if (!bitRead(Rules.triggers[rule_set], Rules.trigger_count[rule_set])) {
        bitSet(Rules.triggers[rule_set], Rules.trigger_count[rule_set]);
      } else {
        match = false;
      }
    } else {
      bitClear(Rules.triggers[rule_set], Rules.trigger_count[rule_set]);
    }
  }

  return match;
}

/********************************************************************************************/
/*
 * Parse a comparison expression.
 * Get 3 parts - left expression, compare operator and right expression.
 * Input:
 *      expr        - A comparison expression like VAR1 >= MEM1 + 10
 *      leftExpr    - Used to accept returned left parts of expression
 *      rightExpr   - Used to accept returned right parts of expression
 * Output:
 *      leftExpr    - Left parts of expression
 *      rightExpr   - Right parts of expression
 * Return:
 *      compare operator
 *      COMPARE_OPERATOR_NONE   - failed
 */
int8_t parseCompareExpression(String &expr, String &leftExpr, String &rightExpr)
{
  char compare_operator[3];
  int8_t compare = COMPARE_OPERATOR_NONE;
  leftExpr = expr;
  int position;
  for (int8_t i = MAXIMUM_COMPARE_OPERATOR; i >= 0; i--) {
    snprintf_P(compare_operator, sizeof(compare_operator), kCompareOperators + (i *2));
    if ((position = expr.indexOf(compare_operator)) > 0) {
      compare = i;
      leftExpr = expr.substring(0, position);
      leftExpr.trim();
      rightExpr = expr.substring(position + strlen(compare_operator));
      rightExpr.trim();
      break;
    }
  }
  return compare;
}

void RulesVarReplace(String &commands, const String &sfind, const String &replace)
{
//  String ufind = sfind;
//  ufind.toUpperCase();
//  char *find = (char*)ufind.c_str();
  char *find = (char*)sfind.c_str();
  uint32_t flen = strlen(find);

  String ucommand = commands;
  ucommand.toUpperCase();
  char *read_from = (char*)ucommand.c_str();
  char *write_to = (char*)commands.c_str();
  char *found_at;
  while ((found_at = strstr(read_from, find)) != nullptr) {
    write_to += (found_at - read_from);
    memmove_P(write_to, find, flen);                      // Make variable Uppercase
    write_to += flen;
    read_from = found_at + flen;
  }

  commands.replace(find, replace);
}

/*******************************************************************************************/

bool RuleSetProcess(uint8_t rule_set, String &event_saved)
{
  bool serviced = false;
  char stemp[10];

  delay(0);                                               // Prohibit possible loop software watchdog

//AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Event = %s, Rule = %s"), event_saved.c_str(), Settings.rules[rule_set]);

  String rules = Settings.rules[rule_set];

  Rules.trigger_count[rule_set] = 0;
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

    String commands = rules.substring(pevt +4, plen);     // "Backlog Dimmer 10;Color 100000"
    Rules.event_value = "";
    String event = event_saved;

//AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Event |%s|, Rule |%s|, Command(s) |%s|"), event.c_str(), event_trigger.c_str(), commands.c_str());

    if (RulesRuleMatch(rule_set, event, event_trigger)) {
      if (plen == plen2) { stop_all_rules = true; }         // If BREAK was used on a triggered rule, Stop execution of this rule set
      commands.trim();
      String ucommand = commands;
      ucommand.toUpperCase();

//      if (!ucommand.startsWith("BACKLOG")) { commands = "backlog " + commands; }  // Always use Backlog to prevent power race exception
      // Use Backlog with event to prevent rule event loop exception unless IF is used which uses an implicit backlog
      if ((ucommand.indexOf("IF ") == -1) &&
          (ucommand.indexOf("EVENT ") != -1) &&
          (ucommand.indexOf("BACKLOG ") == -1)) {
        commands = "backlog " + commands;
      }

      RulesVarReplace(commands, F("%VALUE%"), Rules.event_value);
      for (uint32_t i = 0; i < MAX_RULE_VARS; i++) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%%VAR%d%%"), i +1);
        RulesVarReplace(commands, stemp, rules_vars[i]);
      }
      for (uint32_t i = 0; i < MAX_RULE_MEMS; i++) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%%MEM%d%%"), i +1);
        RulesVarReplace(commands, stemp, SettingsText(SET_MEM1 +i));
      }
      RulesVarReplace(commands, F("%TIME%"), String(MinutesPastMidnight()));
      RulesVarReplace(commands, F("%UPTIME%"), String(MinutesUptime()));
      RulesVarReplace(commands, F("%TIMESTAMP%"), GetDateAndTime(DT_LOCAL));
      RulesVarReplace(commands, F("%TOPIC%"), SettingsText(SET_MQTT_TOPIC));
#if defined(USE_TIMERS) && defined(USE_SUNRISE)
      RulesVarReplace(commands, F("%SUNRISE%"), String(SunMinutes(0)));
      RulesVarReplace(commands, F("%SUNSET%"), String(SunMinutes(1)));
#endif  // USE_TIMERS and USE_SUNRISE

      char command[commands.length() +1];
      strlcpy(command, commands.c_str(), sizeof(command));

      AddLog_P2(LOG_LEVEL_INFO, PSTR("RUL: %s performs \"%s\""), event_trigger.c_str(), command);

//      Response_P(S_JSON_COMMAND_SVALUE, D_CMND_RULE, D_JSON_INITIATED);
//      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_RULE));
#ifdef SUPPORT_IF_STATEMENT
      char *pCmd = command;
      RulesPreprocessCommand(pCmd);                       // Do pre-process for IF statement
#endif
      ExecuteCommand(command, SRC_RULE);
      serviced = true;
      if (stop_all_rules) { return serviced; }            // If BREAK was used, Stop execution of this rule set
    }
    plen += 6;
    Rules.trigger_count[rule_set]++;
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
  // json_event = {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}}
  // json_event = {"System":{"Boot":1}}
  // json_event = {"SerialReceived":"on"} - invalid but will be expanded to {"SerialReceived":{"Data":"on"}}
  char *p = strchr(json_event, ':');
  if ((p != NULL) && !(strchr(++p, ':'))) {  // Find second colon
    event_saved.replace(F(":"), F(":{\"Data\":"));
    event_saved += F("}");
    // event_saved = {"SerialReceived":{"Data":"on"}}
  }
  event_saved.toUpperCase();

//AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Event %s"), event_saved.c_str());

  for (uint32_t i = 0; i < MAX_RULE_SETS; i++) {
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
  for (uint32_t i = 0; i < MAX_RULE_SETS; i++) {
    if (Settings.rules[i][0] == '\0') {
      bitWrite(Settings.rule_enabled, i, 0);
      bitWrite(Settings.rule_once, i, 0);
    }
  }
  Rules.teleperiod = false;
}

void RulesEvery50ms(void)
{
  if (Settings.rule_enabled) {  // Any rule enabled
    char json_event[120];

    if (-1 == Rules.new_power) { Rules.new_power = power; }
    if (Rules.new_power != Rules.old_power) {
      if (Rules.old_power != -1) {
        for (uint32_t i = 0; i < devices_present; i++) {
          uint8_t new_state = (Rules.new_power >> i) &1;
          if (new_state != ((Rules.old_power >> i) &1)) {
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"Power%d\":{\"State\":%d}}"), i +1, new_state);
            RulesProcessEvent(json_event);
          }
        }
      } else {
        // Boot time POWER OUTPUTS (Relays) Status
        for (uint32_t i = 0; i < devices_present; i++) {
          uint8_t new_state = (Rules.new_power >> i) &1;
          snprintf_P(json_event, sizeof(json_event), PSTR("{\"Power%d\":{\"Boot\":%d}}"), i +1, new_state);
          RulesProcessEvent(json_event);
        }
        // Boot time SWITCHES Status
        for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
#ifdef USE_TM1638
          if ((pin[GPIO_SWT1 +i] < 99) || ((pin[GPIO_TM16CLK] < 99) && (pin[GPIO_TM16DIO] < 99) && (pin[GPIO_TM16STB] < 99))) {
#else
          if (pin[GPIO_SWT1 +i] < 99) {
#endif  // USE_TM1638
            bool swm = ((FOLLOW_INV == Settings.switchmode[i]) || (PUSHBUTTON_INV == Settings.switchmode[i]) || (PUSHBUTTONHOLD_INV == Settings.switchmode[i]) || (FOLLOWMULTI_INV == Settings.switchmode[i]));
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"" D_JSON_SWITCH "%d\":{\"Boot\":%d}}"), i +1, (swm ^ SwitchLastState(i)));
            RulesProcessEvent(json_event);
          }
        }
      }
      Rules.old_power = Rules.new_power;
    }
    else if (Rules.old_dimm != Settings.light_dimmer) {
      if (Rules.old_dimm != -1) {
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Dimmer\":{\"State\":%d}}"), Settings.light_dimmer);
      } else {
        // Boot time DIMMER VALUE
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Dimmer\":{\"Boot\":%d}}"), Settings.light_dimmer);
      }
      RulesProcessEvent(json_event);
      Rules.old_dimm = Settings.light_dimmer;
    }
    else if (Rules.event_data[0]) {
      char *event;
      char *parameter;
      event = strtok_r(Rules.event_data, "=", &parameter);     // Rules.event_data = fanspeed=10
      if (event) {
        event = Trim(event);
        if (parameter) {
          parameter = Trim(parameter);
        } else {
          parameter = event + strlen(event);  // '\0'
        }
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Event\":{\"%s\":\"%s\"}}"), event, parameter);
        Rules.event_data[0] ='\0';
        RulesProcessEvent(json_event);
      } else {
        Rules.event_data[0] ='\0';
      }
    }
    else if (Rules.vars_event || Rules.mems_event){
      if (Rules.vars_event) {
        for (uint32_t i = 0; i < MAX_RULE_VARS; i++) {
          if (bitRead(Rules.vars_event, i)) {
            bitClear(Rules.vars_event, i);
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"Var%d\":{\"State\":%s}}"), i+1, rules_vars[i]);
            RulesProcessEvent(json_event);
            break;
          }
        }
      }
      if (Rules.mems_event) {
        for (uint32_t i = 0; i < MAX_RULE_MEMS; i++) {
          if (bitRead(Rules.mems_event, i)) {
            bitClear(Rules.mems_event, i);
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"Mem%d\":{\"State\":%s}}"), i+1, SettingsText(SET_MEM1 +i));
            RulesProcessEvent(json_event);
            break;
          }
        }
      }
    }
    else if (rules_flag.data) {
      uint16_t mask = 1;
      for (uint32_t i = 0; i < MAX_RULES_FLAG; i++) {
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
            case 7: strncpy_P(json_event, PSTR("{\"HTTP\":{\"Initialized\":1}}"), sizeof(json_event)); break;
#ifdef USE_SHUTTER
            case 8: strncpy_P(json_event, PSTR("{\"SHUTTER\":{\"Moved\":1}}"), sizeof(json_event)); break;
            case 9: strncpy_P(json_event, PSTR("{\"SHUTTER\":{\"Moving\":1}}"), sizeof(json_event)); break;
#endif  // USE_SHUTTER
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
      ResponseJsonEnd();
      RulesProcess();
    }
  }
}

void RulesEverySecond(void)
{
  if (Settings.rule_enabled) {  // Any rule enabled
    char json_event[120];

    if (RtcTime.valid) {
      if ((uptime > 60) && (RtcTime.minute != Rules.last_minute)) {  // Execute from one minute after restart every minute only once
        Rules.last_minute = RtcTime.minute;
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Minute\":%d}}"), MinutesPastMidnight());
        RulesProcessEvent(json_event);
      }
    }
    for (uint32_t i = 0; i < MAX_RULE_TIMERS; i++) {
      if (Rules.timer[i] != 0L) {           // Timer active?
        if (TimeReached(Rules.timer[i])) {  // Timer finished?
          Rules.timer[i] = 0L;              // Turn off this timer
          snprintf_P(json_event, sizeof(json_event), PSTR("{\"Rules\":{\"Timer\":%d}}"), i +1);
          RulesProcessEvent(json_event);
        }
      }
    }
  }
}

void RulesSaveBeforeRestart(void)
{
  if (Settings.rule_enabled) {  // Any rule enabled
    char json_event[32];

    strncpy_P(json_event, PSTR("{\"System\":{\"Save\":1}}"), sizeof(json_event));
    RulesProcessEvent(json_event);
  }
}

void RulesSetPower(void)
{
  Rules.new_power = XdrvMailbox.index;
}

void RulesTeleperiod(void)
{
  Rules.teleperiod = true;
  RulesProcess();
  Rules.teleperiod = false;
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
  if (XdrvMailbox.data_len < 1 || XdrvMailbox.data_len > 256) {
    return false;
  }
  bool serviced = false;
  String sTopic = XdrvMailbox.topic;
  String sData = XdrvMailbox.data;
  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: MQTT Topic %s, Event %s"), XdrvMailbox.topic, XdrvMailbox.data);
  MQTT_Subscription event_item;
  //Looking for matched topic
  for (uint32_t index = 0; index < subscriptions.size(); index++) {
    event_item = subscriptions.get(index);

    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Match MQTT message Topic %s with subscription topic %s"), sTopic.c_str(), event_item.Topic.c_str());
    if (sTopic.startsWith(event_item.Topic)) {
      //This topic is subscribed by us, so serve it
      serviced = true;
      String value;
      if (event_item.Key.length() == 0) {   //If did not specify Key
        value = sData;
      } else {      //If specified Key, need to parse Key/Value from JSON data
        StaticJsonBuffer<500> jsonBuf;
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
      snprintf_P(Rules.event_data, sizeof(Rules.event_data), PSTR("%s=%s"), event_item.Event.c_str(), value.c_str());
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
 *      XdrvMailbox.data      - A char buffer with all the parameters
 *      XdrvMailbox.data_len  - Length of the parameters
 * Return:
 *      A string include subscribed event, topic and key.
 */
void CmndSubscribe(void)
{
  MQTT_Subscription subscription_item;
  String events;
  if (XdrvMailbox.data_len > 0) {
    char parameters[XdrvMailbox.data_len+1];
    memcpy(parameters, XdrvMailbox.data, XdrvMailbox.data_len);
    parameters[XdrvMailbox.data_len] = '\0';
    String event_name, topic, key;

    char * pos = strtok(parameters, ",");
    if (pos) {
      event_name = Trim(pos);
      pos = strtok(nullptr, ",");
      if (pos) {
        topic = Trim(pos);
        pos = strtok(nullptr, ",");
        if (pos) {
          key = Trim(pos);
        }
      }
    }
    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RUL: Subscribe command with parameters: %s, %s, %s."), event_name.c_str(), topic.c_str(), key.c_str());
    event_name.toUpperCase();
    if (event_name.length() > 0 && topic.length() > 0) {
      //Search all subscriptions
      for (uint32_t index=0; index < subscriptions.size(); index++) {
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
    for (uint32_t index=0; index < subscriptions.size(); index++) {
      subscription_item = subscriptions.get(index);
      events.concat(subscription_item.Event + "," + subscription_item.Topic
        + (subscription_item.Key.length()>0 ? "," : "")
        + subscription_item.Key + "; ");
    }
  }
  ResponseCmndChar(events.c_str());
}

/********************************************************************************************/
/*
 * Unsubscribe specified MQTT event. If no event specified, Unsubscribe all.
 * Command Unsubscribe format:
 *      UnSubscribe [<event_name>]
 * Input:
 *      XdrvMailbox.data      - Event name
 *      XdrvMailbox.data_len  - Length of the parameters
 * Return:
 *      list all the events unsubscribed.
 */
void CmndUnsubscribe(void)
{
  MQTT_Subscription subscription_item;
  String events;
  if (XdrvMailbox.data_len > 0) {
    for (uint32_t index = 0; index < subscriptions.size(); index++) {
      subscription_item = subscriptions.get(index);
      if (subscription_item.Event.equalsIgnoreCase(XdrvMailbox.data)) {
        String stopic = subscription_item.Topic + "/#";
        MqttUnsubscribe(stopic.c_str());
        events = subscription_item.Event;
        subscriptions.remove(index);
        break;
      }
    }
  } else {
    // If did not specify the event name, unsubscribe all event
    String stopic;
    while (subscriptions.size() > 0) {
      events.concat(subscriptions.get(0).Event + "; ");
      stopic = subscriptions.get(0).Topic + "/#";
      MqttUnsubscribe(stopic.c_str());
      subscriptions.remove(0);
    }
  }
  ResponseCmndChar(events.c_str());
}

#endif  // SUPPORT_MQTT_EVENT

#ifdef USE_EXPRESSION
/********************************************************************************************/
/*
 * Looking for matched bracket - ")"
 * Search buffer from current loction, skip all nested bracket pairs, find the matched close bracket.
 * Input:
 *      pStart    - Point to a char buffer start with "("
 * Output:
 *      N/A
 * Return:
 *      position of matched close bracket
 */
char * findClosureBracket(char * pStart)
{
  char * pointer = pStart + 1;
  //Look for the matched closure parenthesis.")"
  bool bFindClosures = false;
  uint8_t matchClosures = 1;
  while (*pointer)
  {
    if (*pointer == ')') {
      matchClosures--;
      if (matchClosures == 0) {
        bFindClosures = true;
        break;
      }
    } else if (*pointer == '(') {
      matchClosures++;
    }
    pointer++;
  }
  if (bFindClosures) {
    return pointer;
  } else {
    return nullptr;
  }
}

/********************************************************************************************/
/*
 * Parse a number value
 * Input:
 *      pNumber     - A char pointer point to a digit started string (guaranteed)
 *      value       - Reference a float variable used to accept the result
 * Output:
 *      pNumber     - Pointer forward to next character after the number
 *      value       - float type, the result value
 * Return:
 *      true    - succeed
 *      false   - failed
 */
bool findNextNumber(char * &pNumber, float &value)
{
  bool bSucceed = false;
  String sNumber = "";
  if (*pNumber == '-') {
    sNumber = "-";
    pNumber++;
  }
  while (*pNumber) {
    if (isdigit(*pNumber) || (*pNumber == '.')) {
      sNumber += *pNumber;
      pNumber++;
    } else {
      break;
    }
  }
  if (sNumber.length() > 0) {
    value = CharToFloat(sNumber.c_str());
    bSucceed = true;
  }
  return bSucceed;
}

/********************************************************************************************/
/*
 * Parse a variable (like VAR1, MEM3) and get its value (float type)
 * Input:
 *      pVarname    - A char pointer point to a variable name string
 *      value       - Reference a float variable used to accept the result
 * Output:
 *      pVarname    - Pointer forward to next character after the variable
 *      value       - float type, the result value
 * Return:
 *      true    - succeed
 *      false   - failed
 */
bool findNextVariableValue(char * &pVarname, float &value)
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
      value = CharToFloat(rules_vars[index -1]);
    }
  } else if (sVarName.startsWith(F("MEM"))) {
    int index = sVarName.substring(3).toInt();
    if (index > 0 && index <= MAX_RULE_MEMS) {
      value = CharToFloat(SettingsText(SET_MEM1 + index -1));
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
 *     - A float number start with a digit or minus, like 0.787, -3
 *     - A variable name, like VAR1, MEM3
 *     - An expression enclosed with a pair of round brackets, (.....)
 * Input:
 *      pointer     - A char pointer point to a place of the expression string
 *      value       - Reference a float variable used to accept the result
 * Output:
 *      pointer     - Pointer forward to next character after next object
 *      value       - float type, the result value
 * Return:
 *      true    - succeed
 *      false   - failed
 */
bool findNextObjectValue(char * &pointer, float &value)
{
  bool bSucceed = false;
  while (*pointer)
  {
    if (isspace(*pointer)) {      //Skip leading spaces
      pointer++;
      continue;
    }
    if (isdigit(*pointer) || (*pointer) == '-') {      //This object is a number
      bSucceed = findNextNumber(pointer, value);
      break;
    } else if (isalpha(*pointer)) {     //Should be a variable like VAR12, MEM1
      bSucceed = findNextVariableValue(pointer, value);
      break;
    } else if (*pointer == '(') {     //It is a sub expression bracketed with ()
      char * closureBracket = findClosureBracket(pointer);        //Get the position of closure bracket ")"
      if (closureBracket != nullptr) {
        value = evaluateExpression(pointer+1, closureBracket - pointer - 1);
        pointer = closureBracket + 1;
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
    op = EXPRESSION_OPERATOR_ADD;
    const char *pch = kExpressionOperators;
    char ch;
    while ((ch = pgm_read_byte(pch++)) != '\0') {
      if (ch == *pointer) {
        bSucceed = true;
        pointer++;
        break;
      }
      op++;
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
 *      value       - Reference a float variable used to accept the result
 * Output:
 *      pointer     - Pointer forward to next character after next object
 *      value       - float type, the result value
 * Return:
 *      true    - succeed
 *      false   - failed
 */
float calculateTwoValues(float v1, float v2, uint8_t op)
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
 *      float      - result.
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
float evaluateExpression(const char * expression, unsigned int len)
{
  char expbuf[len + 1];
  memcpy(expbuf, expression, len);
  expbuf[len] = '\0';
  char * scan_pointer = expbuf;

  LinkedList<float> object_values;
  LinkedList<int8_t> operators;
  int8_t op;
  float va;
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
  for (int32_t priority = MAX_EXPRESSION_OPERATOR_PRIORITY; priority>0; priority--) {
    int index = 0;
    while (index < operators.size()) {
      if (priority == pgm_read_byte(kExpressionOperatorsPriorities + operators.get(index))) {     //need to calculate the operator first
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
#endif  // USE_EXPRESSION

#ifdef  SUPPORT_IF_STATEMENT
void CmndIf(void)
{
  if (XdrvMailbox.data_len > 0) {
    char parameters[XdrvMailbox.data_len+1];
    memcpy(parameters, XdrvMailbox.data, XdrvMailbox.data_len);
    parameters[XdrvMailbox.data_len] = '\0';
    ProcessIfStatement(parameters);
  }
  ResponseCmndDone();
}

/********************************************************************************************/
/*
 * Evaluate a comparison expression.
 * Get the logic value of expression, true or false
 * Input:
 *      expression    - A comparison expression like VAR1 >= MEM1 + 10
 *      len           - Length of expression
 * Output:
 *      N/A
 * Return:
 *      logic value of comparison expression
 */
bool evaluateComparisonExpression(const char *expression, int len)
{
  bool bResult = true;
  char expbuf[len + 1];
  memcpy(expbuf, expression, len);
  expbuf[len] = '\0';
  String compare_expression = expbuf;
  String leftExpr, rightExpr;
  int8_t compareOp = parseCompareExpression(compare_expression, leftExpr, rightExpr);

  double leftValue = evaluateExpression(leftExpr.c_str(), leftExpr.length());
  double rightValue = evaluateExpression(rightExpr.c_str(), rightExpr.length());
  switch (compareOp) {
    case COMPARE_OPERATOR_EXACT_DIVISION:
      bResult = (rightValue != 0 && leftValue == int(leftValue)
          && rightValue == int(rightValue) && (int(leftValue) % int(rightValue)) == 0);
      break;
    case COMPARE_OPERATOR_EQUAL:
      bResult = leftExpr.equalsIgnoreCase(rightExpr);  // Compare strings - this also works for hexadecimals
      break;
    case COMPARE_OPERATOR_BIGGER:
      bResult = (leftValue > rightValue);
      break;
    case COMPARE_OPERATOR_SMALLER:
      bResult = (leftValue < rightValue);
      break;
    case COMPARE_OPERATOR_NUMBER_EQUAL:
      bResult = (leftValue == rightValue);
      break;
    case COMPARE_OPERATOR_NOT_EQUAL:
      bResult = (leftValue != rightValue);
      break;
    case COMPARE_OPERATOR_BIGGER_EQUAL:
      bResult = (leftValue >= rightValue);
      break;
    case COMPARE_OPERATOR_SMALLER_EQUAL:
      bResult = (leftValue <= rightValue);
      break;
  }
  return bResult;
}

/********************************************************************************************/
/*
 * Looking for a logical operator, either "AND" or "OR"
 * A logical operator is expected at this moment. If we find something else, this function will fail.
 * Input:
 *      pointer     - Point to a char buffer
 *      op          - Used to accpet the logical operator type
 * Output:
 *      Pointer     - pointer will forward to next character after the logical operator.
 *      op          - The logical operator type we found
 * Return:
 *      true    - succeed
 *      false   - failed
 */
bool findNextLogicOperator(char * &pointer, int8_t &op)
{
  bool bSucceed = false;
  while (*pointer && isspace(*pointer)) {
    //Skip spaces
    pointer++;
  }
  if (*pointer) {
    if (strncasecmp_P(pointer, PSTR("AND "), 4) == 0) {
      op = LOGIC_OPERATOR_AND;
      pointer += 4;
      bSucceed = true;
    } else if (strncasecmp_P(pointer, PSTR("OR "), 3) == 0) {
      op = LOGIC_OPERATOR_OR;
      pointer += 3;
      bSucceed = true;
    }
  }
  return bSucceed;
}

/********************************************************************************************/
/*
 * Find next logical object and get its value
 *      A logical object could be:
 *        - A comparison expression.
 *        - A logical expression bracketed with a pair of parenthesis.
 * Input:
 *      pointer     - A char pointer point to a start of logical object
 *      value       - Used to accept the result value
 * Output:
 *      pointer     - Pointer forward to next character after the object
 *      value       - boolean type, the value of the logical object.
 * Return:
 *      true    - succeed
 *      false   - failed
 */
bool findNextLogicObjectValue(char * &pointer, bool &value)
{
  bool bSucceed = false;
  while (*pointer && isspace(*pointer)) {
    //Skip leading spaces
    pointer++;
  }
  char * pExpr = pointer;
  while (*pointer) {
    if (isalpha(*pointer)
       && (strncasecmp_P(pointer, PSTR("AND "), 4) == 0
       || strncasecmp_P(pointer, PSTR("OR "), 3) == 0))
    {      //We have a logic operator, should stop
      value = evaluateComparisonExpression(pExpr, pointer - pExpr);
      bSucceed = true;
      break;
    } else if (*pointer == '(') {     //It is a sub expression bracketed with ()
      char * closureBracket = findClosureBracket(pointer);        //Get the position of closure bracket ")"
      if (closureBracket != nullptr) {
        value = evaluateLogicalExpression(pointer+1, closureBracket - pointer - 1);
        pointer = closureBracket + 1;
        bSucceed = true;
      }
      break;
    }
    pointer++;
  }
  if (!bSucceed && pointer > pExpr) {
    //The whole buffer is an comparison expression
    value = evaluateComparisonExpression(pExpr, pointer - pExpr);
    bSucceed = true;
  }
  return bSucceed;
}

/********************************************************************************************/
/*
 * Evaluate a logical expression
 *    Logic expression is constructed with multiple comparison expressions and logical
 *    operators between them. For example: Mem1==0 AND (time > sunrise + 60).
 *    Parenthesis are allowed to change the priority of logical operators.
 * Input:
 *      expression  - A logical expression
 *      len         - Length of the expression
 * Output:
 *      N/A
 * Return:
 *      boolean     - the value of logical expression
 */
bool evaluateLogicalExpression(const char * expression, int len)
{
  bool bResult = false;
  //Make a copy first
  char expbuff[len + 1];
  memcpy(expbuff, expression, len);
  expbuff[len] = '\0';

  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("EvalLogic: |%s|"), expbuff);
  char * pointer = expbuff;
  LinkedList<bool> values;
  LinkedList<int8_t> logicOperators;
  //Find first comparison expression
  bool bValue;
  if (findNextLogicObjectValue(pointer, bValue)) {
    values.add(bValue);
  } else {
    return false;
  }
  int8_t op;
  while (*pointer) {
    if (findNextLogicOperator(pointer, op)
      && (*pointer) && findNextLogicObjectValue(pointer, bValue))
    {
      logicOperators.add(op);
      values.add(bValue);
    } else {
      break;
    }
  }
  //Calculate all "AND" first
  int index = 0;
  while (index < logicOperators.size()) {
    if (logicOperators.get(index) == LOGIC_OPERATOR_AND) {
      values.set(index, values.get(index) && values.get(index+1));
      values.remove(index + 1);
      logicOperators.remove(index);
    } else {
      index++;
    }
  }
  //Then, calculate all "OR"
  index = 0;
  while (index < logicOperators.size()) {
    if (logicOperators.get(index) == LOGIC_OPERATOR_OR) {
      values.set(index, values.get(index) || values.get(index+1));
      values.remove(index + 1);
      logicOperators.remove(index);
    } else {
      index++;
    }
  }
  return values.get(0);
}

/********************************************************************************************/
/*
 * This function search in a buffer to find out an IF block start from current position
 *   Note: All the tokens found during the searching will be changed to NULL terminated string.
 *         Please make a copy before call this function if you still need it.
 * Input:
 *      pointer     - Point to a NULL end string buffer with the commands
 *      lenWord     - Accept the length of block end word
 *      block_type  - The block type you are looking for.
 * Output:
 *      pointer     - pointer point to the end of if block.
 *      lenWord     - The length of block end word ("ENDIF", "ELSEIF", "ELSE")
 * Return:
 *      The block type we find.
 *      IF_BLOCK_INVALID    - Failed.
 */
int8_t findIfBlock(char * &pointer, int &lenWord, int8_t block_type)
{
  int8_t foundBlock = IF_BLOCK_INVALID;
  //First break into words delimited by space or ";"
  const char * word;
  while (*pointer) {
    if (!isalpha(*pointer)) {
      pointer++;
      continue;
    }
    word = pointer;
    while (*pointer && isalpha(*pointer)) {
      pointer++;
    }
    lenWord = pointer - word;

    if (2 == lenWord && 0 == strncasecmp_P(word, PSTR("IF"), 2)) {
    //if we find a new "IF" that means this is nested if block
      //Try to finish this nested if block
      if (findIfBlock(pointer, lenWord, IF_BLOCK_ENDIF) != IF_BLOCK_ENDIF) {
        //If failed, we done.
        break;
      }
    } else if ( (IF_BLOCK_ENDIF == block_type || IF_BLOCK_ANY == block_type)
      && (5 == lenWord) && (0 == strncasecmp_P(word, PSTR("ENDIF"), 5)))
    {
      //Find an "ENDIF"
      foundBlock = IF_BLOCK_ENDIF;
      break;
    } else if ( (IF_BLOCK_ELSEIF == block_type || IF_BLOCK_ANY == block_type)
      && (6 == lenWord) && (0 == strncasecmp_P(word, PSTR("ELSEIF"), 6)))
    {
      //Find an "ELSEIF"
      foundBlock = IF_BLOCK_ELSEIF;
      break;
    } else if ( (IF_BLOCK_ELSE == block_type || IF_BLOCK_ANY == block_type)
      && (4 == lenWord) && (0 == strncasecmp_P(word, PSTR("ELSE"), 4)))
    {
      //Find an "ELSE"
      foundBlock = IF_BLOCK_ELSE;
      break;
    }
  }
  return foundBlock;
}

/********************************************************************************************/
/*
 * This function is used to execute a commands block in if statement when one of the condition is true.
 * Input:
 *      commands    - A char buffer include (but not limited) the commands block need to execute
 *      len         - Length of the commands block
 * Output:
        N/A
 * Return:
 *      void
 */
void ExecuteCommandBlock(const char * commands, int len)
{
  char cmdbuff[len + 1];    //apply enough space
  memcpy(cmdbuff, commands, len);
  cmdbuff[len] = '\0';

  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ExecCmd: |%s|"), cmdbuff);
  char oneCommand[len + 1];     //To put one command
  int insertPosition = 0;       //When insert into backlog, we should do it by 0, 1, 2 ...
  char * pos = cmdbuff;
  int lenEndBlock = 0;
  while (*pos) {
    if (isspace(*pos) || '\x1e' == *pos || ';' == *pos) {
      pos++;
      continue;
    }
    if (strncasecmp_P(pos, PSTR("BACKLOG "), 8) == 0) {
      //Skip "BACKLOG " and set not first command flag. So all followed command will be send to backlog
      pos += 8;
      continue;
    }
    if (strncasecmp_P(pos, PSTR("IF "), 3) == 0) {
      //Has a nested IF statement
      //Find the matched ENDIF
      char *pEndif = pos + 3;    //Skip "IF "
      if (IF_BLOCK_ENDIF != findIfBlock(pEndif, lenEndBlock, IF_BLOCK_ENDIF)) {
        //Cannot find matched endif, stop execution.
        break;
      }
      //We has the whole IF statement, copy to oneCommand
      memcpy(oneCommand, pos, pEndif - pos);
      oneCommand[pEndif - pos] = '\0';
      pos = pEndif;
    } else {    //Normal command
      //Looking for the command end single - '\x1e'
      char *pEndOfCommand = strpbrk(pos, "\x1e;");
      if (NULL == pEndOfCommand) {
        pEndOfCommand = pos + strlen(pos);
      }
      memcpy(oneCommand, pos, pEndOfCommand - pos);
      oneCommand[pEndOfCommand - pos] = '\0';
      pos = pEndOfCommand;
    }
    //Start to process current command we found
    //Going to insert the command into backlog
    String sCurrentCommand = oneCommand;
    sCurrentCommand.trim();
    if (sCurrentCommand.length() > 0
      && backlog.size() < MAX_BACKLOG && !backlog_mutex)
    {
      //Insert into backlog
      backlog_mutex = true;
      backlog.add(insertPosition, sCurrentCommand);
      backlog_mutex = false;
      insertPosition++;
    }
  }
  return;
}

/********************************************************************************************/
/*
 * Execute IF statement. This is the place to run a "IF ..." command.
 * Input:
 *      statements  - The IF statement we are going to process
 * Output:
        N/A
 * Return:
 *      void
 */
void ProcessIfStatement(const char* statements)
{
  String conditionExpression;
  int len = strlen(statements);
  char statbuff[len + 1];
  memcpy(statbuff, statements, len + 1);
  char *pos = statbuff;
  int lenEndBlock = 0;
  while (true) {             //Each loop process one IF (or ELSEIF) block
    //Find and test the condition expression followed the IF or ELSEIF
    //Search for the open bracket first
    while (*pos && *pos != '(') {
      pos++;
    }
    if (0 == *pos) { break; }
    char * posEnd = findClosureBracket(pos);

    if (true == evaluateLogicalExpression(pos + 1, posEnd - (pos + 1))) {
      //Looking for matched "ELSEIF", "ELSE" or "ENDIF", then Execute this block
      char * cmdBlockStart = posEnd + 1;
      char * cmdBlockEnd = cmdBlockStart;
      int8_t nextBlock = findIfBlock(cmdBlockEnd, lenEndBlock, IF_BLOCK_ANY);
      if (IF_BLOCK_INVALID == nextBlock) {
        //Failed
        break;
      }
      ExecuteCommandBlock(cmdBlockStart, cmdBlockEnd - cmdBlockStart - lenEndBlock);
      pos = cmdBlockEnd;
      break;
    } else {      //Does not match the IF condition, going to check elseif and else
      pos = posEnd + 1;
      int8_t nextBlock = findIfBlock(pos, lenEndBlock, IF_BLOCK_ANY);
      if (IF_BLOCK_ELSEIF == nextBlock) {
        //Continue process next ELSEIF block like IF
        continue;
      } else if (IF_BLOCK_ELSE == nextBlock) {
        //Looking for matched "ENDIF" then execute this block
        char * cmdBlockEnd = pos;
        int8_t nextBlock = findIfBlock(cmdBlockEnd, lenEndBlock, IF_BLOCK_ENDIF);
        if (IF_BLOCK_ENDIF != nextBlock) {
          //Failed
          break;
        }
        ExecuteCommandBlock(pos, cmdBlockEnd - pos - lenEndBlock);
        break;
      } else {    // IF_BLOCK_ENDIF == nextBlock
        //We done
        break;
      }
    }
  }
}

/********************************************************************************************/
/*
 * This function is called in Rules event handler to process any command between DO ... ENDON (BREAK)
 *    - Do escape (convert ";" into "\x1e") for all IF statements.
 * Input:
 *      commands    - The commands block need to execute
 * Output:
        N/A
 * Return:
 *      void
 */
void RulesPreprocessCommand(char *pCommands)
{
  char * cmd = pCommands;
  int lenEndBlock = 0;
  while (*cmd) {
    //Skip all ";" and space between two commands
    if (';' == *cmd || isspace(*cmd)) {
      cmd++;
    }
    else if (strncasecmp_P(cmd, PSTR("IF "), 3) == 0) {      //found IF block
                                                             //We are going to look for matched "ENDIF"
      char * pIfStart = cmd;
      char * pIfEnd = pIfStart + 3;   //Skip "IF "
                                      //int pIfStart = cmd - command;      //"IF" statement block start at position (relative to command start)
      if (IF_BLOCK_ENDIF == findIfBlock(pIfEnd, lenEndBlock, IF_BLOCK_ENDIF)) {
        //Found the ENDIF
        cmd = pIfEnd;   //Will continue process from here
        //Escapte from ";" to "\x1e".
        //By remove all ";" in IF statement block, we can prevent backlog command cut the whole block as multiple commands
        while (pIfStart < pIfEnd) {
          if (';' == *pIfStart)
            *pIfStart = '\x1e';
          pIfStart++;
        }
      }
      else {    //Did not find the matched ENDIF, stop processing
        break;
      }
    }
    else {      //Other commands, skip it
      while (*cmd && ';' != *cmd) {
        cmd++;
      }
    }
  }
  return;
}
#endif          //SUPPORT_IF_STATEMENT

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndRule(void)
{
  uint8_t index = XdrvMailbox.index;
  if ((index > 0) && (index <= MAX_RULE_SETS)) {
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
      Rules.triggers[index -1] = 0;  // Reset once flag
    }
    snprintf_P (mqtt_data, sizeof(mqtt_data), PSTR("{\"%s%d\":\"%s\",\"Once\":\"%s\",\"StopOnError\":\"%s\",\"Free\":%d,\"Rules\":\"%s\"}"),
      XdrvMailbox.command, index, GetStateText(bitRead(Settings.rule_enabled, index -1)), GetStateText(bitRead(Settings.rule_once, index -1)),
      GetStateText(bitRead(Settings.rule_stop, index -1)), sizeof(Settings.rules[index -1]) - strlen(Settings.rules[index -1]) -1, Settings.rules[index -1]);
  }
}

void CmndRuleTimer(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_RULE_TIMERS)) {
    if (XdrvMailbox.data_len > 0) {
#ifdef USE_EXPRESSION
      float timer_set = evaluateExpression(XdrvMailbox.data, XdrvMailbox.data_len);
      Rules.timer[XdrvMailbox.index -1] = (timer_set > 0) ? millis() + (1000 * timer_set) : 0;
#else
      Rules.timer[XdrvMailbox.index -1] = (XdrvMailbox.payload > 0) ? millis() + (1000 * XdrvMailbox.payload) : 0;
#endif  // USE_EXPRESSION
    }
    mqtt_data[0] = '\0';
    for (uint32_t i = 0; i < MAX_RULE_TIMERS; i++) {
      ResponseAppend_P(PSTR("%c\"T%d\":%d"), (i) ? ',' : '{', i +1, (Rules.timer[i]) ? (Rules.timer[i] - millis()) / 1000 : 0);
    }
    ResponseJsonEnd();
  }
}

void CmndEvent(void)
{
  if (XdrvMailbox.data_len > 0) {
    strlcpy(Rules.event_data, XdrvMailbox.data, sizeof(Rules.event_data));
  }
  ResponseCmndDone();
}

void CmndVariable(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_RULE_VARS)) {
    if (!XdrvMailbox.usridx) {
      mqtt_data[0] = '\0';
      for (uint32_t i = 0; i < MAX_RULE_VARS; i++) {
        ResponseAppend_P(PSTR("%c\"Var%d\":\"%s\""), (i) ? ',' : '{', i +1, rules_vars[i]);
      }
      ResponseJsonEnd();
    } else {
      if (XdrvMailbox.data_len > 0) {
#ifdef USE_EXPRESSION
        if (XdrvMailbox.data[0] == '=') {  // Spaces already been skipped in data
          dtostrfd(evaluateExpression(XdrvMailbox.data + 1, XdrvMailbox.data_len - 1), Settings.flag2.calc_resolution, rules_vars[XdrvMailbox.index -1]);
        } else {
          strlcpy(rules_vars[XdrvMailbox.index -1], ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data, sizeof(rules_vars[XdrvMailbox.index -1]));
        }
#else
        strlcpy(rules_vars[XdrvMailbox.index -1], ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data, sizeof(rules_vars[XdrvMailbox.index -1]));
#endif  // USE_EXPRESSION
        bitSet(Rules.vars_event, XdrvMailbox.index -1);
      }
      ResponseCmndIdxChar(rules_vars[XdrvMailbox.index -1]);
    }
  }
}

void CmndMemory(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_RULE_MEMS)) {
    if (!XdrvMailbox.usridx) {
      ResponseCmndAll(SET_MEM1, MAX_RULE_MEMS);
    } else {
      if (XdrvMailbox.data_len > 0) {
#ifdef USE_EXPRESSION
        if (XdrvMailbox.data[0] == '=') {  // Spaces already been skipped in data
          dtostrfd(evaluateExpression(XdrvMailbox.data + 1, XdrvMailbox.data_len - 1), Settings.flag2.calc_resolution, SettingsText(SET_MEM1 + XdrvMailbox.index -1));
        } else {
          SettingsUpdateText(SET_MEM1 + XdrvMailbox.index -1, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data);
        }
#else
        SettingsUpdateText(SET_MEM1 +  XdrvMailbox.index -1, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data);
#endif  // USE_EXPRESSION
        bitSet(Rules.mems_event, XdrvMailbox.index -1);
      }
      ResponseCmndIdxChar(SettingsText(SET_MEM1 + XdrvMailbox.index -1));
    }
  }
}

void CmndCalcResolution(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 7)) {
    Settings.flag2.calc_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.flag2.calc_resolution);
}

void CmndAddition(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      float tempvar = CharToFloat(rules_vars[XdrvMailbox.index -1]) + CharToFloat(XdrvMailbox.data);
      dtostrfd(tempvar, Settings.flag2.calc_resolution, rules_vars[XdrvMailbox.index -1]);
      bitSet(Rules.vars_event, XdrvMailbox.index -1);
    }
    ResponseCmndIdxChar(rules_vars[XdrvMailbox.index -1]);
  }
}

void CmndSubtract(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      float tempvar = CharToFloat(rules_vars[XdrvMailbox.index -1]) - CharToFloat(XdrvMailbox.data);
      dtostrfd(tempvar, Settings.flag2.calc_resolution, rules_vars[XdrvMailbox.index -1]);
      bitSet(Rules.vars_event, XdrvMailbox.index -1);
    }
    ResponseCmndIdxChar(rules_vars[XdrvMailbox.index -1]);
  }
}

void CmndMultiply(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      float tempvar = CharToFloat(rules_vars[XdrvMailbox.index -1]) * CharToFloat(XdrvMailbox.data);
      dtostrfd(tempvar, Settings.flag2.calc_resolution, rules_vars[XdrvMailbox.index -1]);
      bitSet(Rules.vars_event, XdrvMailbox.index -1);
    }
    ResponseCmndIdxChar(rules_vars[XdrvMailbox.index -1]);
  }
}

void CmndScale(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      if (strstr(XdrvMailbox.data, ",") != nullptr) {  // Process parameter entry
        char sub_string[XdrvMailbox.data_len +1];

        float valueIN = CharToFloat(subStr(sub_string, XdrvMailbox.data, ",", 1));
        float fromLow = CharToFloat(subStr(sub_string, XdrvMailbox.data, ",", 2));
        float fromHigh = CharToFloat(subStr(sub_string, XdrvMailbox.data, ",", 3));
        float toLow = CharToFloat(subStr(sub_string, XdrvMailbox.data, ",", 4));
        float toHigh = CharToFloat(subStr(sub_string, XdrvMailbox.data, ",", 5));
        float value = map_double(valueIN, fromLow, fromHigh, toLow, toHigh);
        dtostrfd(value, Settings.flag2.calc_resolution, rules_vars[XdrvMailbox.index -1]);
        bitSet(Rules.vars_event, XdrvMailbox.index -1);
      }
    }
    ResponseCmndIdxChar(rules_vars[XdrvMailbox.index -1]);
  }
}

float map_double(float x, float in_min, float in_max, float out_min, float out_max)
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
      result = DecodeCommand(kRulesCommands, RulesCommand);
      break;
    case FUNC_RULES_PROCESS:
      result = RulesProcess();
      break;
    case FUNC_SAVE_BEFORE_RESTART:
      RulesSaveBeforeRestart();
      break;
#ifdef SUPPORT_MQTT_EVENT
    case FUNC_MQTT_DATA:
      result = RulesMqttData();
      break;
#endif  // SUPPORT_MQTT_EVENT
    case FUNC_PRE_INIT:
      RulesInit();
      break;
  }
  return result;
}

#endif  // Do not USE_SCRIPT
#endif  // USE_RULES
