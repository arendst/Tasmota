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
 *   on time#set do color 001008 endon
 *   on clock#timer=3 do color 080800 endon
 *   on rules#timer=1 do color 080800 endon
 *   on mqtt#connected do color 000010 endon on mqtt#disconnected do color 001010 endon on time#initialized do color 001000 endon on time#set do backlog color 000810;ruletimer1 10 endon on rules#timer=1 do color 080800 endon
 *   on event#anyname do color 100000 endon
 *   on event#anyname do color %value% endon
 *   on power1#state=1 do color 001000 endon
 *   on button1#state do publish cmnd/ring2/power %value% endon on button2#state do publish cmnd/strip1/power %value% endon
 *   on switch1#state do power2 %value% endon
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

#define MAX_RULE_TIMERS        8

#define ULONG_MAX              0xffffffffUL

#define D_CMND_RULE "Rule"
#define D_CMND_RULETIMER "RuleTimer"
#define D_CMND_EVENT "Event"

#define D_JSON_INITIATED "Initiated"

enum RulesCommands { CMND_RULE, CMND_RULETIMER, CMND_EVENT };
const char kRulesCommands[] PROGMEM = D_CMND_RULE "|" D_CMND_RULETIMER "|" D_CMND_EVENT ;

String rules_event_value;
unsigned long rules_timer[MAX_RULE_TIMERS] = { 0 };
uint8_t rules_quota = 0;
long rules_power = -1;

uint32_t rules_triggers = 0;
uint8_t rules_trigger_count = 0;

/*******************************************************************************************/

long TimeDifference(unsigned long prev, unsigned long next)
{
  // Return the time difference as a signed value, taking into account the timers may overflow.
  // Returned timediff is between -24.9 days and +24.9 days.
  // Returned value is positive when "next" is after "prev"
  long signed_diff = 0;
  // To cast a value to a signed long, the difference may not exceed half the ULONG_MAX
  const unsigned long half_max_unsigned_long = 2147483647u;  // = 2^31 -1
  if (next >= prev) {
    const unsigned long diff = next - prev;
    if (diff <= half_max_unsigned_long) {                    // Normal situation, just return the difference.
      signed_diff = static_cast<long>(diff);                 // Difference is a positive value.
    } else {
      // prev has overflow, return a negative difference value
      signed_diff = static_cast<long>((ULONG_MAX - next) + prev + 1u);
      signed_diff = -1 * signed_diff;
    }
  } else {
    // next < prev
    const unsigned long diff = prev - next;
    if (diff <= half_max_unsigned_long) {                    // Normal situation, return a negative difference value
      signed_diff = static_cast<long>(diff);
      signed_diff = -1 * signed_diff;
    } else {
      // next has overflow, return a positive difference value
      signed_diff = static_cast<long>((ULONG_MAX - prev) + next + 1u);
    }
  }
  return signed_diff;
}

long TimePassedSince(unsigned long timestamp)
{
  // Compute the number of milliSeconds passed since timestamp given.
  // Note: value can be negative if the timestamp has not yet been reached.
  return TimeDifference(timestamp, millis());
}

bool TimeReached(unsigned long timer)
{
  // Check if a certain timeout has been reached.
  const long passed = TimePassedSince(timer);
  return (passed >= 0);
}

/*******************************************************************************************/

bool RulesRuleMatch(String &event, String &rule)
{
  // event = {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}}
  // event = {"System":{"Boot":1}}
  // rule = "INA219#CURRENT>0.100"

  bool match = false;

  // Step1: Analyse rule
  int pos = rule.indexOf('#');
  if (pos == -1) { return false; }                     // No # sign in rule

  String rule_task = rule.substring(0, pos);           // "INA219" or "SYSTEM"
  String rule_name = rule.substring(pos +1);           // "CURRENT>0.100" or "BOOT"

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
      }
    }
  }

  String tmp_value = "none";
  double rule_value = 0;
  if (pos > 0) {
    tmp_value = rule_name.substring(pos + 1);          // "0.100"
    rule_value = CharToDouble((char*)tmp_value.c_str());  // 0.1      - This saves 9k code over toFLoat()!
    rule_name = rule_name.substring(0, pos);           // "CURRENT"
  }

  // Step2: Search rule_task and rule_name
  StaticJsonBuffer<400> jsonBuf;
  JsonObject &root = jsonBuf.parseObject(event);
  if (!root.success()) { return false; }               // No valid JSON data

  double value = 0;
  const char* str_value = root[rule_task][rule_name];

//  snprintf_P(log_data, sizeof(log_data), PSTR("RUL: Task %s, Name %s, Value %s, TrigCnt %d, TrigSt %d, Source %s, Json %s"),
//    rule_task.c_str(), rule_name.c_str(), tmp_value.c_str(), rules_trigger_count, bitRead(rules_triggers, rules_trigger_count), event.c_str(), (str_value) ? str_value : "none");
//  AddLog(LOG_LEVEL_DEBUG);

  if (!root[rule_task][rule_name].success()) { return false; }
  // No value but rule_name is ok

  rules_event_value = str_value;                       // Prepare %value%

  // Step 3: Compare rule (value)
  if (str_value) {
    value = CharToDouble((char*)str_value);
    switch (compare) {
      case '>':
        if (value > rule_value) match = true;
        break;
      case '<':
        if (value < rule_value) match = true;
        break;
      case '=':
        if (value == rule_value) match = true;
        break;
      case ' ':
        match = true;                                  // Json value but not needed
        break;
    }
  } else match = true;

  if (Settings.flag.rules_once) {
    if (match) {                                       // Only allow match state changes
      if (!bitRead(rules_triggers, rules_trigger_count)) {
        bitSet(rules_triggers, rules_trigger_count);
      } else {
        match = false;
      }
    } else {
      bitClear(rules_triggers, rules_trigger_count);
    }
  }

  return match;
}

/*******************************************************************************************/

bool RulesProcess()
{
  bool serviced = false;

  if (!Settings.flag.rules_enabled) { return serviced; }  // Not enabled
  if (!strlen(Settings.rules)) { return serviced; }       // No rules

  String event_saved = mqtt_data;
  event_saved.toUpperCase();
  String rules = Settings.rules;

  rules_trigger_count = 0;
  int plen = 0;
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
    if (plen == -1) { return serviced; }                  // Bad syntax - No endon
    String commands = rules.substring(pevt +4, plen);     // "Backlog Dimmer 10;Color 100000"
    plen += 6;

//    snprintf_P(log_data, sizeof(log_data), PSTR("RUL: Trigger |%s|, Commands |%s|"), event_trigger.c_str(), commands.c_str());
//    AddLog(LOG_LEVEL_DEBUG);

    rules_event_value = "";
    String event = event_saved;
    if (RulesRuleMatch(event, event_trigger)) {
      commands.replace(F("%value%"), rules_event_value);
      char command[commands.length() +1];
      snprintf(command, sizeof(command), commands.c_str());

      snprintf_P(log_data, sizeof(log_data), PSTR("RUL: %s performs \"%s\""), event_trigger.c_str(), command);
      AddLog(LOG_LEVEL_INFO);

//      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, D_CMND_RULE, D_JSON_INITIATED);
//      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_RULE));

      ExecuteCommand(command);
      serviced = true;
    }
    rules_trigger_count++;
  }
  return serviced;
}

/*******************************************************************************************/

void RulesInit()
{
  if (Settings.rules[0] == '\0') {
    Settings.flag.rules_enabled = 0;
    Settings.flag.rules_once = 0;
  }
}

void RulesSetPower()
{
  if (Settings.flag.rules_enabled) {
    uint16_t new_power = XdrvMailbox.index;
    if (rules_power == -1) rules_power = new_power;
    uint16_t old_power = rules_power;
    rules_power = new_power;
    for (byte i = 0; i < devices_present; i++) {
      uint8_t new_state = new_power &1;
      uint8_t old_state = old_power &1;
      if (new_state != old_state) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"Power%d\":{\"State\":%d}}"), i +1, new_state);
        RulesProcess();
      }
      new_power >>= 1;
      old_power >>= 1;
    }
  }
}

void RulesEvery50ms()
{
  if (Settings.flag.rules_enabled) {
    rules_quota++;
    if (rules_quota &1) {              // Every 100 ms
      mqtt_data[0] = '\0';
      uint16_t tele_period_save = tele_period;
      tele_period = 2;                 // Do not allow HA updates during next function call
      XsnsNextCall(FUNC_JSON_APPEND);  // ,"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}
      tele_period = tele_period_save;
      if (strlen(mqtt_data)) {
        mqtt_data[0] = '{';            // {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
        RulesProcess();
      }
    }
  }
}

void RulesEverySecond()
{
  if (Settings.flag.rules_enabled) {
    for (byte i = 0; i < MAX_RULE_TIMERS; i++) {
      if (rules_timer[i] != 0L) {           // Timer active?
        if (TimeReached(rules_timer[i])) {  // Timer finished?
          rules_timer[i] = 0L;              // Turn off this timer
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"Rules\":{\"Timer\":%d}}"), i +1);
          RulesProcess();
        }
      }
    }
  }
}

boolean RulesCommand()
{
  char command[CMDSZ];
  boolean serviced = true;
  uint8_t index = XdrvMailbox.index;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kRulesCommands);
  if (CMND_RULE == command_code) {
    if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.data_len < sizeof(Settings.rules))) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
        switch (XdrvMailbox.payload) {
        case 0: // Off
        case 1: // On
          Settings.flag.rules_enabled = XdrvMailbox.payload;
          break;
        case 2: // Toggle
          Settings.flag.rules_enabled ^= 1;
          break;
        case 4: // Off
        case 5: // On
          Settings.flag.rules_once = XdrvMailbox.payload &1;
          break;
        case 6: // Toggle
          Settings.flag.rules_once ^= 1;
          break;
        }
      } else {
/*
        String uc_data = XdrvMailbox.data;  // Do not allow Rule to be used within a rule
        uc_data.toUpperCase();
        String uc_command = command;
        uc_command += " ";    // Distuingish from RuleTimer
        uc_command.toUpperCase();
        if (!uc_data.indexOf(uc_command)) { strlcpy(Settings.rules, XdrvMailbox.data, sizeof(Settings.rules)); }
*/
        strlcpy(Settings.rules, XdrvMailbox.data, sizeof(Settings.rules));
      }
      rules_triggers = 0;  // Reset once flag
    }
    snprintf_P (mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":\"%s\",\"Once\":\"%s\",\"Rules\":\"%s\"}"), command, GetStateText(Settings.flag.rules_enabled), GetStateText(Settings.flag.rules_once), Settings.rules);
  }
  else if ((CMND_RULETIMER == command_code) && (index > 0) && (index <= MAX_RULE_TIMERS)) {
    if (XdrvMailbox.data_len > 0) {
      rules_timer[index -1] = (XdrvMailbox.payload > 0) ? millis() + (1000 * XdrvMailbox.payload) : 0;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_LVALUE, command, index, (rules_timer[index -1]) ? (rules_timer[index -1] - millis()) / 1000 : 0);
  }
  else if (CMND_EVENT == command_code) {
    if (XdrvMailbox.data_len > 0) {
      String event = XdrvMailbox.data;
      String parameter = "";
      int pos = event.indexOf('=');
      if (pos > 0) {
        parameter = event.substring(pos +1);
        parameter.trim();
        event = event.substring(0, pos);
      }
      event.trim();
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"Event\":{\"%s\":\"%s\"}}"), event.c_str(), parameter.c_str());
      RulesProcess();
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
  }
  else serviced = false;

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_10

boolean Xdrv10(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_INIT:
      RulesInit();
      break;
    case FUNC_SET_POWER:
      RulesSetPower();
      break;
    case FUNC_EVERY_50_MSECOND:
      RulesEvery50ms();
      break;
    case FUNC_EVERY_SECOND:
      RulesEverySecond();
      break;
    case FUNC_COMMAND:
      result = RulesCommand();
      break;
  }
  return result;
}

#endif  // USE_RULES