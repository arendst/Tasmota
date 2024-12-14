/*
  xdrv_10_rules.ino - rule support for Tasmota

  Copyright (C) 2021  ESP Easy Group and Theo Arends

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
 *   on loadavg<50 do power 2 endon
 *   on Time#Initialized do Backlog var1 0;event checktime=%time% endon on event#checktime>%timer1% do var1 1 endon on event#checktime>=%timer2%  do var1 0 endon * on event#checktime do Power1 %var1% endon
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

#define XDRV_10                 10

#ifndef RULE_MAX_EVENTSZ
#define RULE_MAX_EVENTSZ        100
#endif

#ifndef RULE_MAX_MQTT_EVENTSZ
#define RULE_MAX_MQTT_EVENTSZ   256
#endif

//#define DEBUG_RULES

#include <unishox.h>

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

#define COMPARE_OPERATOR_NONE                 -1
#define COMPARE_OPERATOR_EQUAL                 0
#define COMPARE_OPERATOR_BIGGER                1
#define COMPARE_OPERATOR_SMALLER               2
#define COMPARE_OPERATOR_EXACT_DIVISION        3
#define COMPARE_OPERATOR_NUMBER_EQUAL          4
#define COMPARE_OPERATOR_NOT_EQUAL             5
#define COMPARE_OPERATOR_BIGGER_EQUAL          6
#define COMPARE_OPERATOR_SMALLER_EQUAL         7
#define COMPARE_OPERATOR_STRING_ENDS_WITH      8
#define COMPARE_OPERATOR_STRING_STARTS_WITH    9
#define COMPARE_OPERATOR_STRING_CONTAINS      10
#define COMPARE_OPERATOR_STRING_NOT_EQUAL     11
#define COMPARE_OPERATOR_STRING_NOT_CONTAINS  12
#define MAXIMUM_COMPARE_OPERATOR              COMPARE_OPERATOR_STRING_NOT_CONTAINS
const char kCompareOperators[] PROGMEM = "=\0>\0<\0|\0==!=>=<=$>$<$|$!$^";

#ifdef USE_EXPRESSION
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

// Define to indicate that rules are always enabled
#ifdef USE_BERRY
  #define BERRY_RULES     1
#else
  #define BERRY_RULES     0
#endif

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

struct RULES {
  String event_value;
  unsigned long timer[MAX_RULE_TIMERS] = { 0 };
  uint32_t triggers[MAX_RULE_SETS] = { 0 };
  uint8_t trigger_count[MAX_RULE_SETS] = { 0 };

  long new_power = -1;
  long old_power = -1;
  long old_dimm = -1;

  uint16_t last_minute = 60;
  uint16_t vars_event = 0;   // Bitmask supporting MAX_RULE_VARS bits
  uint16_t mems_event = 0;   // Bitmask supporting MAX_RULE_MEMS bits
  bool teleperiod = false;
  bool busy = false;
  bool no_execute = false;   // Don't actually execute rule commands

  char event_data[RULE_MAX_EVENTSZ];
} Rules;

char rules_vars[MAX_RULE_VARS][33] = {{ 0 }};

#if (MAX_RULE_VARS>16)
#error MAX_RULE_VARS is bigger than 16
#endif
#if (MAX_RULE_MEMS>16)
#error MAX_RULE_MEMS is bigger than 16
#endif


/*******************************************************************************************/
/*
 * Add Unishox compression to Rules
 *
 * New compression for Rules, depends on SetOption93
 *
 * To avoid memory corruption when downgrading, the format is as follows:
 * - If `SetOption93 0`
 *   Rule[x][] = 511 char max NULL terminated string (512 with trailing NULL)
 *   Rule[x][0] = 0 if the Rule<x> is empty
 *   New: in case the string is empty we also enforce:
 *   Rule[x][1] = 0   (i.e. we have two conseutive NULLs)
 *
 * - If `SetOption93 1`
 *   If the rule is smaller than 511, it is stored uncompressed. Rule[x][0] is not null.
 *   If the rule is empty, Rule[x][0] = 0 and Rule[x][1] = 0;
 *   If the rule is bigger than 511, it is stored compressed
 *      The first byte of each Rule is always NULL.
 *      Rule[x][0] = 0,  if firmware is downgraded, the rule will be considered as empty
 *
 *      The second byte contains the size of uncompressed rule in 8-bytes blocks (i.e. (len+7)/8 )
 *      Maximum rule size is 2KB (2048 bytes per rule), although there is little chances compression ratio will go down to 75%
 *      Rule[x][1] = size uncompressed in dwords. If zero, the rule is empty.
 *
 *      The remaining bytes contain the compressed rule, NULL terminated
 */
/*******************************************************************************************/

#ifdef USE_UNISHOX_COMPRESSION
// Statically allocate one String per rule
String k_rules[MAX_RULE_SETS] = { String(), String(), String() };   // Strings are created empty
// Unishox compressor;   // singleton
#endif  // USE_UNISHOX_COMPRESSION

// Returns whether the rule is uncompressed, which means the first byte is not NULL
inline bool IsRuleUncompressed(uint32_t idx) {
#ifdef USE_UNISHOX_COMPRESSION
  return Settings->rules[idx][0] ? true : false;      // first byte not NULL, the rule is not empty and not compressed
#else
  return true;
#endif  // USE_UNISHOX_COMPRESSION
}

// Returns whether the rule is empty, which requires two consecutive NULL
inline bool IsRuleEmpty(uint32_t idx) {
#ifdef USE_UNISHOX_COMPRESSION
  return (Settings->rules[idx][0] == 0) && (Settings->rules[idx][1] == 0) ? true : false;
#else
  return (Settings->rules[idx][0] == 0) ? true : false;
#endif  // USE_UNISHOX_COMPRESSION
}

// Returns the approximate (+3-0) length of the rule, not counting the trailing NULL
size_t GetRuleLen(uint32_t idx) {
  // no need to use #ifdef USE_UNISHOX_COMPRESSION, the compiler will optimize since first test is always true
  if (IsRuleUncompressed(idx)) {
    return strlen(Settings->rules[idx]);
  } else {                        // either empty or compressed
    return Settings->rules[idx][1] * 8;   // cheap calculation, but not byte accurate (may overshoot by 7)
  }
}

// Returns the actual Flash storage for the Rule, including trailing NULL
size_t GetRuleLenStorage(uint32_t idx) {
#ifdef USE_UNISHOX_COMPRESSION
  if (Settings->rules[idx][0] || !Settings->rules[idx][1]) {    // if first byte is non-NULL it is uncompressed, if second byte is NULL, then it's either uncompressed or empty
    return 1 + strlen(Settings->rules[idx]);   // uncompressed or empty
  } else {
    return 2 + strlen(&Settings->rules[idx][1]); // skip first byte and get len of the compressed rule
  }
#else   // No USE_UNISHOX_COMPRESSION
  return 1 + strlen(Settings->rules[idx]);
#endif  // USE_UNISHOX_COMPRESSION
}

#ifdef USE_UNISHOX_COMPRESSION
// internal function, do the actual decompression
void GetRule_decompress(String &rule, const char *rule_head) {
  size_t buf_len = 1 + *rule_head * 8;       // the first byte contains size of buffer for uncompressed rule / 8, buf_len may overshoot by 7
  rule_head++;                               // advance to the actual compressed buffer

  rule = Decompress(rule_head, buf_len);
}
#endif  // USE_UNISHOX_COMPRESSION

//
// Read rule in memory, uncompress if needed
//
// Returns: String() object containing a copy of the rule (rule processing is destructive and will change the String)
String GetRule(uint32_t idx) {
  if (IsRuleUncompressed(idx)) {
    return String(Settings->rules[idx]);
  } else {
#ifdef USE_UNISHOX_COMPRESSION    // we still do #ifdef to make sure we don't link unnecessary code
    String rule("");
    if (Settings->rules[idx][1] == 0) { return rule; }     // the rule is empty

    // If the cache is empty, we need to decompress from Settings
    if (0 == k_rules[idx].length() ) {
      GetRule_decompress(rule, &Settings->rules[idx][1]);
      if (!Settings->flag4.compress_rules_cpu) {
        k_rules[idx] = rule;        // keep a copy for next time
      }
    } else {
      // we have a valid copy
      rule = k_rules[idx];
    }
    return rule;
#endif  // USE_UNISHOX_COMPRESSION
  }
  return "";  // Fix GCC10 warning
}

#ifdef USE_UNISHOX_COMPRESSION
// internal function, comrpess rule and store a cached version uncompressed (except if SetOption94 1)
// If out == nullptr, we are in dry-run mode, so don't keep rule in cache
int32_t SetRule_compress(uint32_t idx, const char *in, size_t in_len, char *out, size_t out_len) {
  int32_t len_compressed;
  len_compressed = compressor.unishox_compress(in, in_len, out, out_len);

  if (len_compressed >= 0) {                // negative means compression failed because of buffer too small, we leave the rule untouched
    // check if we need to store in cache
    k_rules[idx] = (const char*) nullptr;   // Assign the String to nullptr, clears previous string and disallocate internal buffers of String object
    if ((!Settings->flag4.compress_rules_cpu) && out) {      // if out == nullptr, don't store cache
      // keep copy in cache
      k_rules[idx] = in;
    }
  }
  return len_compressed;
}
#endif  // USE_UNISHOX_COMPRESSION

// Returns:
//   >= 0 : the actual stored size
//   <0 : not enough space
int32_t SetRule(uint32_t idx, const char *content, bool append = false) {
  if (nullptr == content) { content = ""; }   // if nullptr, use empty string
  size_t len_in = strlen(content);
  bool needsCompress = false;
  size_t offset = 0;

  if (len_in >= MAX_RULE_SIZE) {              // if input is more than 512, it will not fit uncompressed
    needsCompress = true;
  }
  if (append) {
    if (IsRuleUncompressed(idx) || IsRuleEmpty(idx)) {  // if already uncompressed (so below 512) and append mode, check if it still fits uncompressed
      offset = strlen(Settings->rules[idx]);
      if (len_in + offset >= MAX_RULE_SIZE) {
        needsCompress = true;
      }
    } else {
      needsCompress = true;                 // we append to a non-empty compressed rule, so it won't fit uncompressed
    }
  }

  if (!needsCompress) {                       // the rule fits uncompressed, so just copy it
//    strlcpy(Settings->rules[idx] + offset, content, sizeof(Settings->rules[idx]));
    strlcpy(Settings->rules[idx] + offset, content, sizeof(Settings->rules[idx]) - offset);
    if (0 == Settings->rules[idx][0]) {
      Settings->rules[idx][1] = 0;
    }

#ifdef USE_UNISHOX_COMPRESSION
    if (0 != len_in + offset) {
      // do a dry-run compression to display how much it would be compressed
      int32_t len_compressed, len_uncompressed;

      len_uncompressed = strlen(Settings->rules[idx]);
      len_compressed = compressor.unishox_compress(Settings->rules[idx], len_uncompressed, nullptr /* dry-run */, MAX_RULE_SIZE + 8);
      AddLog(LOG_LEVEL_INFO, PSTR("RUL: Stored uncompressed, would compress from %d to %d (-%d%%)"), len_uncompressed, len_compressed, 100 - changeUIntScale(len_compressed, 0, len_uncompressed, 0, 100));
    }
#endif  // USE_UNISHOX_COMPRESSION

    return len_in + offset;
  } else {
#ifdef USE_UNISHOX_COMPRESSION
    int32_t len_compressed;
    // allocate temp buffer so we don't nuke the rule if it's too big to fit
    char *buf_out = (char*) malloc(MAX_RULE_SIZE + 8);    // take some margin
    if (!buf_out) { return -1; }      // fail if couldn't allocate

    // compress
    if (append) {
      String content_append = GetRule(idx);   // get original Rule and decompress it if needed
      content_append += content;             // concat new content
      len_in = content_append.length();       // adjust length
      len_compressed = SetRule_compress(idx, content_append.c_str(), len_in, buf_out, MAX_RULE_SIZE + 8);
    } else {
      len_compressed = SetRule_compress(idx, content, len_in, buf_out, MAX_RULE_SIZE + 8);
    }

    if ((len_compressed >= 0) && (len_compressed < MAX_RULE_SIZE - 2)) {
      // size is ok, copy to Settings
      Settings->rules[idx][0] = 0;     // clear first byte to mark as compressed
      Settings->rules[idx][1] = (len_in + 7) / 8;    // store original length in first bytes (4 bytes chuks)
      memcpy(&Settings->rules[idx][2], buf_out, len_compressed);
      Settings->rules[idx][len_compressed + 2] = 0;  // add NULL termination
      AddLog(LOG_LEVEL_INFO, PSTR("RUL: Compressed from %d to %d (-%d%%)"), len_in, len_compressed, 100 - changeUIntScale(len_compressed, 0, len_in, 0, 100));
      // AddLog(LOG_LEVEL_INFO, PSTR("RUL: First bytes: %02X%02X%02X%02X"), Settings->rules[idx][0], Settings->rules[idx][1], Settings->rules[idx][2], Settings->rules[idx][3]);
      // AddLog(LOG_LEVEL_INFO, PSTR("RUL: GetRuleLenStorage = %d"), GetRuleLenStorage(idx));
    } else {
      len_compressed = -1;    // failed
      // clear rule cache, so it will be reloaded from Settings
      k_rules[idx] = (const char *) nullptr;
    }
    free(buf_out);
    return len_compressed;
#else   // No USE_UNISHOX_COMPRESSION
    return -1;                                // the rule does not fit and we can't compress
#endif  // USE_UNISHOX_COMPRESSION
  }

}

/*******************************************************************************************/

bool RulesRuleMatch(uint8_t rule_set, String &event, String &rule, bool stop_all_rules)
{
  // event = {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}}
  // event = {"System":{"Boot":1}}
  // rule = "INA219#CURRENT>0.100"

  bool match = false;
  char stemp[10];

  // Step1: Analyse rule
  String rule_expr = rule;                             // "TELE-INA219#CURRENT>0.100"
  if (Rules.teleperiod) {
    int ppos = rule_expr.indexOf(F("TELE-"));          // "TELE-INA219#CURRENT>0.100" or "INA219#CURRENT>0.100"
    if (ppos == -1) { return false; }                  // No pre-amble in rule
    rule_expr = rule.substring(5);                     // "INA219#CURRENT>0.100" or "SYSTEM#BOOT"
  }

  String rule_name, rule_param;
  int8_t compareOperator = parseCompareExpression(rule_expr, rule_name, rule_param);  // Parse the compare expression.Return operator and the left, right part of expression

  // rule_name  = "INA219#CURRENT"
  // rule_param = "0.100" or "%VAR1%"

#ifdef DEBUG_RULES
  AddLog(LOG_LEVEL_DEBUG, PSTR("RUL-RM1: Teleperiod %d, Expr %s, Name %s, Param %s"), Rules.teleperiod, rule_expr.c_str(), rule_name.c_str(), rule_param.c_str());
#endif

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
    if (rule_param.startsWith(F("%TIME%"))) {
      rule_param = String(MinutesPastMidnight());
    }
    if (rule_param.startsWith(F("%UPTIME%"))) {
      rule_param = String(MinutesUptime());
    }
    if (rule_param.startsWith(F("%TIMESTAMP%"))) {
      rule_param = GetDateAndTime(DT_LOCAL).c_str();
    }
#if defined(USE_TIMERS)
    if (rule_param.startsWith(F("%TIMER"))) {
      uint32_t index = rule_param.substring(6).toInt();
      if ((index > 0) && (index <= MAX_TIMERS)) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%%TIMER%d%%"), index);
        if (rule_param.startsWith(stemp)) {
          rule_param = String(TimerGetTimeOfDay(index -1));
        }
      }
    }
#if defined(USE_SUNRISE)
    if (rule_param.startsWith(F("%SUNRISE%"))) {
      rule_param = String(SunMinutes(0));
    }
    if (rule_param.startsWith(F("%SUNSET%"))) {
      rule_param = String(SunMinutes(1));
    }
#endif  // USE_SUNRISE
#endif  // USE_TIMERS
#if defined(USE_LIGHT)
    char scolor[LIGHT_COLOR_SIZE];
    if (rule_param.startsWith(F("%COLOR%"))) {
      rule_param = LightGetColor(scolor);
    }
#endif  // USE_LIGHT
// #ifdef USE_ZIGBEE
//     if (rule_param.startsWith(F("%ZBDEVICE%"))) {
//       snprintf_P(stemp, sizeof(stemp), PSTR("0x%04X"), Z_GetLastDevice());
//       rule_param = String(stemp);
//     }
//     if (rule_param.startsWith(F("%ZBGROUP%"))) {
//       rule_param = String(Z_GetLastGroup());
//     }
//     if (rule_param.startsWith(F("%ZBCLUSTER%"))) {
//       rule_param = String(Z_GetLastCluster());
//     }
//     if (rule_param.startsWith(F("%ZBENDPOINT%"))) {
//       rule_param = String(Z_GetLastEndpoint());
//     }
// #endif  // USE_ZIGBEE
    rule_param.toUpperCase();
    strlcpy(rule_svalue, rule_param.c_str(), sizeof(rule_svalue));

    int temp_value = GetStateNumber(rule_svalue);
    if (temp_value > -1) {
      rule_value = temp_value;
    } else {
      rule_value = CharToFloat((char*)rule_svalue);    // 0.1      - This saves 9k code over toFLoat()!
    }
  }

  // Step2: Search rule_name
  int pos;
  int rule_name_idx = 0;
  if ((pos = rule_name.indexOf(F("["))) > 0) {         // "SUBTYPE1#CURRENT[1]"
    rule_name_idx = rule_name.substring(pos +1).toInt();
    if ((rule_name_idx < 1) || (rule_name_idx > 6)) {  // Allow indexes 1 to 6
      rule_name_idx = 1;
    }
    rule_name = rule_name.substring(0, pos);           // "SUBTYPE1#CURRENT"
  }

  String buf = event;                                  // Copy the string into a new buffer that will be modified

// Do not do below replace as it will replace escaped quote too.
//  buf.replace("\\"," ");                               // "Disable" any escaped control character

//AddLog(LOG_LEVEL_DEBUG, PSTR("RUL-RM2: RulesRuleMatch '%s'"), buf.c_str());

  JsonParser parser((char*)buf.c_str());
  JsonParserObject obj = parser.getRootObject();
  if (!obj) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("RUL: Event too long (%d)"), event.length());
    AddLog(LOG_LEVEL_DEBUG, PSTR("RUL: No valid JSON (%s)"), buf.c_str());
    return false; // No valid JSON data
  }
  String subtype;
  uint32_t i = 0;
  while ((pos = rule_name.indexOf(F("#"))) > 0) {      // "SUBTYPE1#SUBTYPE2#CURRENT"
    subtype = rule_name.substring(0, pos);
    obj = obj[subtype.c_str()].getObject();
    if (!obj) { return false; }                        // not found

    rule_name = rule_name.substring(pos +1);
    if (i++ > 10) { return false; }                    // Abandon possible loop

    yield();
  }

  JsonParserToken val = obj[rule_name.c_str()];
  if (!val) { return false; }                          // last level not found
  const char* str_value;
  if (rule_name_idx) {
    if (val.isArray()) {
      str_value = (val.getArray())[rule_name_idx -1].getStr();
    } else {
      str_value = val.getStr();
    }
  } else {
    str_value = val.getStr();                          // "CURRENT"
  }

#ifdef DEBUG_RULES
  AddLog(LOG_LEVEL_DEBUG, PSTR("RUL-RM3: Name %s, Value '%s', TrigCnt %d, TrigSt %d, Source %s, Json '%s'"),
    rule_name.c_str(), rule_svalue, Rules.trigger_count[rule_set], bitRead(Rules.triggers[rule_set],
    Rules.trigger_count[rule_set]), event.c_str(), (str_value[0] != '\0') ? str_value : "none");
#endif

  Rules.event_value = str_value;                       // Prepare %value%

  // Step 3: Compare rule (value)
  float value = 0;
  if (str_value) {
    value = CharToFloat((char*)str_value);
    int int_value = int(value);
    int int_rule_value = int(rule_value);
    String str_str_value = String(str_value);
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
      case COMPARE_OPERATOR_STRING_ENDS_WITH:
        match = str_str_value.endsWith(rule_svalue);
        break;
      case COMPARE_OPERATOR_STRING_STARTS_WITH:
        match = str_str_value.startsWith(rule_svalue);
        break;
      case COMPARE_OPERATOR_STRING_CONTAINS:
        match = (str_str_value.indexOf(rule_svalue) >= 0);
        break;
      case  COMPARE_OPERATOR_STRING_NOT_EQUAL:
        match = (0!=strcasecmp(str_value, rule_svalue));  // Compare strings - this also works for hexadecimals
        break;
      case  COMPARE_OPERATOR_STRING_NOT_CONTAINS:
        match = (str_str_value.indexOf(rule_svalue) < 0);
        break;
      default:
        match = true;
    }
  } else match = true;

  if (stop_all_rules) { match = false; }

//AddLog(LOG_LEVEL_DEBUG, PSTR("RUL-RM4: Match 1 %d, Triggers %08X, TriggerCount %d"), match, Rules.triggers[rule_set], Rules.trigger_count[rule_set]);

  if (bitRead(Settings->rule_once, rule_set)) {
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

//AddLog(LOG_LEVEL_DEBUG, PSTR("RUL-RM5: Match 2 %d, Triggers %08X, TriggerCount %d"), match, Rules.triggers[rule_set], Rules.trigger_count[rule_set]);

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

#ifdef DEBUG_RULES
  AddLog(LOG_LEVEL_DEBUG, PSTR("RUL-RP1: Event '%s', Rule '%s'"), event_saved.c_str(), Settings->rules[rule_set]);
#endif

  String rules = GetRule(rule_set);

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
    if (!rule.startsWith(F("ON "))) { return serviced; }     // Bad syntax - Nothing to start on

    int pevt = rule.indexOf(F(" DO "));
    if (pevt == -1) { return serviced; }                  // Bad syntax - Nothing to do
    String event_trigger = rule.substring(3, pevt);       // "INA219#CURRENT>0.100"
    event_trigger.trim();

    plen = rule.indexOf(F(" ENDON"));
    plen2 = rule.indexOf(F(" BREAK"));
    if ((plen == -1) && (plen2 == -1)) { return serviced; } // Bad syntax - No ENDON neither BREAK

    if (plen == -1) { plen = 9999; }
    if (plen2 == -1) { plen2 = 9999; }
    plen = tmin(plen, plen2);

    String commands = rules.substring(pevt +4, plen);     // "Backlog Dimmer 10;Color 100000"
    Rules.event_value = "";
    String event = event_saved;

#ifdef DEBUG_RULES
    AddLog(LOG_LEVEL_DEBUG, PSTR("RUL-RP2: Event '%s', Rule '%s', Command(s) '%s'"), event.c_str(), event_trigger.c_str(), commands.c_str());
#endif

    if (!event_trigger.startsWith(F("FILE#")) && RulesRuleMatch(rule_set, event, event_trigger, stop_all_rules)) {
      if (Rules.no_execute) return true;
      if (plen == plen2) { stop_all_rules = true; }       // If BREAK was used on a triggered rule, Stop execution of this rule set
      commands.trim();
      String ucommand = commands;
      ucommand.toUpperCase();

//      if (!ucommand.startsWith("BACKLOG")) { commands = "backlog " + commands; }  // Always use Backlog to prevent power race exception
      // Use Backlog with event to prevent rule event loop exception unless IF is used which uses an implicit backlog
      if ((ucommand.indexOf(F("IF ")) == -1) &&
          (ucommand.indexOf(F("EVENT ")) != -1) &&
          (ucommand.indexOf(F("BACKLOG")) == -1)) {
        commands = String(F("backlog ")) + commands;
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
      for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%%POWER%d%%"), i +1);
        RulesVarReplace(commands, stemp, String(bitRead(TasmotaGlobal.power, i)));
      }
      for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
        if (SwitchUsed(i)) {
          snprintf_P(stemp, sizeof(stemp), PSTR("%%SWITCH%d%%"), i +1);
          RulesVarReplace(commands, stemp, String(SwitchState(i)));
        }
      }
      RulesVarReplace(commands, F("%TIME%"), String(MinutesPastMidnight()));
      RulesVarReplace(commands, F("%UTCTIME%"), String(UtcTime()));
      RulesVarReplace(commands, F("%UPTIME%"), String(MinutesUptime()));
      RulesVarReplace(commands, F("%TIMESTAMP%"), GetDateAndTime(DT_LOCAL));
      RulesVarReplace(commands, F("%TOPIC%"), TasmotaGlobal.mqtt_topic);
      snprintf_P(stemp, sizeof(stemp), PSTR("%06X"), ESP_getChipId());
      RulesVarReplace(commands, F("%DEVICEID%"), stemp);
      RulesVarReplace(commands, F("%MACADDR%"), NetworkUniqueId());
#if defined(USE_TIMERS)
      for (uint32_t i = 0; i < MAX_TIMERS; i++) {
        snprintf_P(stemp, sizeof(stemp), PSTR("%%TIMER%d%%"), i +1);
        RulesVarReplace(commands, stemp, String(TimerGetTimeOfDay(i)));
      }
#if defined(USE_SUNRISE)
      RulesVarReplace(commands, F("%SUNRISE%"), String(SunMinutes(0)));
      RulesVarReplace(commands, F("%SUNSET%"), String(SunMinutes(1)));
#endif  // USE_SUNRISE
#endif  // USE_TIMERS
#if defined(USE_LIGHT)
      char scolor[LIGHT_COLOR_SIZE];
      RulesVarReplace(commands, F("%COLOR%"), LightGetColor(scolor));
#endif  // USE_LIGHT
#ifdef USE_ZIGBEE
      snprintf_P(stemp, sizeof(stemp), PSTR("0x%04X"), Z_GetLastDevice());
      RulesVarReplace(commands, F("%ZBDEVICE%"), String(stemp));
      RulesVarReplace(commands, F("%ZBGROUP%"), String(Z_GetLastGroup()));
      RulesVarReplace(commands, F("%ZBCLUSTER%"), String(Z_GetLastCluster()));
      RulesVarReplace(commands, F("%ZBENDPOINT%"), String(Z_GetLastEndpoint()));
#endif  // USE_ZIGBEE

      char command[commands.length() +1];
      strlcpy(command, commands.c_str(), sizeof(command));

      AddLog(LOG_LEVEL_INFO, PSTR("RUL: %s performs '%s'"), event_trigger.c_str(), command);

//      Response_P(S_JSON_COMMAND_SVALUE, D_CMND_RULE, D_JSON_INITIATED);
//      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_RULE));
#ifdef SUPPORT_IF_STATEMENT
      char *pCmd = command;
      RulesPreprocessCommand(pCmd);                       // Do pre-process for IF statement
#endif  // SUPPORT_IF_STATEMENT
      ExecuteCommand(command, SRC_RULE);
      serviced = true;
    }
    plen += 6;
    Rules.trigger_count[rule_set]++;
  }
  return serviced;
}

/*******************************************************************************************/

String RuleLoadFile(const char* fname) {
  /* Read a string from rule space data between 'ON FILE#<fname> DO ' and ' ENDON' like:
       rule3 on file#calib.dat do {"rms":{"current_a":3166385,"voltage_a":-767262},"freq":0} endon
     NOTE: String may not contain word 'ENDON'!!
  */
  String filename = F("ON FILE#");
  filename += fname;
  filename += F(" DO ");
//  filename.toUpperCase();

  for (uint32_t i = 0; i < MAX_RULE_SETS; i++) {
    if (!GetRuleLen(i)) { continue; }

    String rules = GetRule(i);
    rules.toUpperCase();
    int start = rules.indexOf(filename);
    if (start == -1) { continue; }
    start += filename.length();
    int end = rules.indexOf(F(" ENDON"), start);
    if (end == -1) { continue; }

    rules = GetRule(i);
    return rules.substring(start, end);  // {"rms":{"current_a":3166385,"voltage_a":-767262},"freq":0}
  }
//  AddLog(LOG_LEVEL_DEBUG, PSTR("RUL: File '%s' not found or empty"), fname);
  return "";
}

/*******************************************************************************************/

bool RulesProcessEvent(const char *json_event)
{
#ifdef USE_BERRY
  // events are passed to Berry before Rules engine
  callBerryRule(json_event, Rules.teleperiod);
#endif  // USE_BERRY

  if (Rules.busy) { return false; }

  Rules.busy = true;
  bool serviced = false;

  SHOW_FREE_MEM(PSTR("RulesProcessEvent"));

#ifdef DEBUG_RULES
  AddLog(LOG_LEVEL_DEBUG, PSTR("RUL: RulesProcessEvent '%s'"), json_event);
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

#ifdef DEBUG_RULES
  AddLog(LOG_LEVEL_DEBUG, PSTR("RUL: Event '%s'"), event_saved.c_str());
#endif

  for (uint32_t i = 0; i < MAX_RULE_SETS; i++) {
    if (GetRuleLen(i) && bitRead(Settings->rule_enabled, i)) {
      if (RuleSetProcess(i, event_saved)) { serviced = true; }
    }
  }

  Rules.busy = false;

  return serviced;
}

bool RulesProcess(void) {

#ifdef DEBUG_RULES
  AddLog(LOG_LEVEL_DEBUG, PSTR("RUL: RulesProcess '%s'"), XdrvMailbox.data);
#endif

  if ((Settings->rule_enabled || BERRY_RULES) && !Rules.busy) {  // Any rule enabled
    return RulesProcessEvent(XdrvMailbox.data);
  }
  return false;
}

void RulesInit(void)
{
  // indicates scripter not enabled
  bitWrite(Settings->rule_once, 7, 0);
  // and indicates scripter do not use compress
  bitWrite(Settings->rule_once, 6, 0);

  TasmotaGlobal.rules_flag.data = 0;
  for (uint32_t i = 0; i < MAX_RULE_SETS; i++) {
    if (0 == GetRuleLen(i)) {
      bitWrite(Settings->rule_enabled, i, 0);
      bitWrite(Settings->rule_once, i, 0);
    }
  }
  Rules.teleperiod = false;
}

void RulesEvery50ms(void)
{
  if ((Settings->rule_enabled || BERRY_RULES) && !Rules.busy) {  // Any rule enabled
    char json_event[RULE_MAX_EVENTSZ +16];  // Add 16 chars for {"Event": .. }

    if (-1 == Rules.new_power) { Rules.new_power = TasmotaGlobal.power; }
    if (Rules.new_power != Rules.old_power) {
      if (Rules.old_power != -1) {
        for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
          uint8_t new_state = (Rules.new_power >> i) &1;
          if (new_state != ((Rules.old_power >> i) &1)) {
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"Power%d\":{\"State\":%d}}"), i +1, new_state);
            RulesProcessEvent(json_event);
          }
        }
      } else {
        // Boot time POWER OUTPUTS (Relays) Status
        for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
          uint8_t new_state = (Rules.new_power >> i) &1;
          snprintf_P(json_event, sizeof(json_event), PSTR("{\"Power%d\":{\"Boot\":%d}}"), i +1, new_state);
          RulesProcessEvent(json_event);
        }
        // Boot time SWITCHES Status
        for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
          if (SwitchUsed(i)) {
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"%s\":{\"Boot\":%d}}"), GetSwitchText(i).c_str(), (SwitchState(i)));
            RulesProcessEvent(json_event);
          }
        }
      }
      Rules.old_power = Rules.new_power;
    }
    else if (Rules.old_dimm != Settings->light_dimmer) {
      if (Rules.old_dimm != -1) {
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Dimmer\":{\"State\":%d}}"), Settings->light_dimmer);
      } else {
        // Boot time DIMMER VALUE
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Dimmer\":{\"Boot\":%d}}"), Settings->light_dimmer);
      }
      RulesProcessEvent(json_event);
      Rules.old_dimm = Settings->light_dimmer;
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
        bool quotes = (parameter[0] != '{');
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Event\":{\"%s\":%s%s%s}}"), event, (quotes)?"\"":"", parameter, (quotes)?"\"":"");
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
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"Var%d\":{\"State\":\"%s\"}}"), i+1, rules_vars[i]);
            RulesProcessEvent(json_event);
            break;
          }
        }
      }
      if (Rules.mems_event) {
        for (uint32_t i = 0; i < MAX_RULE_MEMS; i++) {
          if (bitRead(Rules.mems_event, i)) {
            bitClear(Rules.mems_event, i);
            snprintf_P(json_event, sizeof(json_event), PSTR("{\"Mem%d\":{\"State\":\"%s\"}}"), i+1, SettingsText(SET_MEM1 +i));
            RulesProcessEvent(json_event);
            break;
          }
        }
      }
    }
    else if (TasmotaGlobal.rules_flag.data) {
      json_event[0] = '\0';
      if (TasmotaGlobal.rules_flag.system_init) {
        TasmotaGlobal.rules_flag.system_init = 0;
        strncpy_P(json_event, PSTR("{\"System\":{\"Init\":1}}"), sizeof(json_event));
      }
      else if (TasmotaGlobal.rules_flag.system_boot) {
        TasmotaGlobal.rules_flag.system_boot = 0;
        strncpy_P(json_event, PSTR("{\"System\":{\"Boot\":1}}"), sizeof(json_event));
      }
      else if (TasmotaGlobal.rules_flag.time_init) {
        TasmotaGlobal.rules_flag.time_init = 0;
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Initialized\":%d}}"), MinutesPastMidnight());
      }
      else if (TasmotaGlobal.rules_flag.time_set) {
        TasmotaGlobal.rules_flag.time_set = 0;
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Set\":%d}}"), MinutesPastMidnight());
      }
      else if (TasmotaGlobal.rules_flag.mqtt_connected) {
        TasmotaGlobal.rules_flag.mqtt_connected = 0;
        strncpy_P(json_event, PSTR("{\"MQTT\":{\"Connected\":1}}"), sizeof(json_event));
      }
      else if (TasmotaGlobal.rules_flag.mqtt_disconnected) {
        TasmotaGlobal.rules_flag.mqtt_disconnected = 0;
        strncpy_P(json_event, PSTR("{\"MQTT\":{\"Disconnected\":1}}"), sizeof(json_event));
      }
      else if (TasmotaGlobal.rules_flag.wifi_connected) {
        TasmotaGlobal.rules_flag.wifi_connected = 0;
        strncpy_P(json_event, PSTR("{\"WIFI\":{\"Connected\":1}}"), sizeof(json_event));
      }
      else if (TasmotaGlobal.rules_flag.wifi_disconnected) {
        TasmotaGlobal.rules_flag.wifi_disconnected = 0;
        strncpy_P(json_event, PSTR("{\"WIFI\":{\"Disconnected\":1}}"), sizeof(json_event));
      }
//#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32 && defined(USE_ETHERNET)
#if defined(ESP32) && defined(USE_ETHERNET)
      else if (TasmotaGlobal.rules_flag.eth_connected) {
        TasmotaGlobal.rules_flag.eth_connected = 0;
        strncpy_P(json_event, PSTR("{\"ETH\":{\"Connected\":1}}"), sizeof(json_event));
      }
      else if (TasmotaGlobal.rules_flag.eth_disconnected) {
        TasmotaGlobal.rules_flag.eth_disconnected = 0;
        strncpy_P(json_event, PSTR("{\"ETH\":{\"Disconnected\":1}}"), sizeof(json_event));
      }
#endif  // USE_ETHERNET
      else if (TasmotaGlobal.rules_flag.http_init) {
        TasmotaGlobal.rules_flag.http_init = 0;
        strncpy_P(json_event, PSTR("{\"HTTP\":{\"Initialized\":1}}"), sizeof(json_event));
      }
#ifdef USE_SHUTTER
      else if (TasmotaGlobal.rules_flag.shutter_moved) {
        TasmotaGlobal.rules_flag.shutter_moved = 0;
        strncpy_P(json_event, PSTR("{\"SHUTTER\":{\"Moved\":1}}"), sizeof(json_event));
      }
      else if (TasmotaGlobal.rules_flag.shutter_moving) {
        TasmotaGlobal.rules_flag.shutter_moving = 0;
        strncpy_P(json_event, PSTR("{\"SHUTTER\":{\"Moving\":1}}"), sizeof(json_event));
      }
#endif  // USE_SHUTTER
      if (json_event[0]) {
        RulesProcessEvent(json_event);  // Only service one event within 50mS
      }
    }
  }
}

void RulesEvery100ms(void) {
  if ((Settings->rule_enabled || BERRY_RULES) && !Rules.busy && (TasmotaGlobal.uptime > 4)) {  // Any rule enabled and allow 4 seconds start-up time for sensors (#3811)
    if (GetNextSensor()) {
      RulesProcessEvent(ResponseData());
    }
  }
}

void RulesEverySecond(void)
{
  char json_event[120];
  if ((Settings->rule_enabled || BERRY_RULES) && !Rules.busy) {  // Any rule enabled
    if (RtcTime.valid) {
      if ((TasmotaGlobal.uptime > 60) && (RtcTime.minute != Rules.last_minute)) {  // Execute from one minute after restart every minute only once
        Rules.last_minute = RtcTime.minute;
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Minute\":%d}}"), MinutesPastMidnight());
        RulesProcessEvent(json_event);
      }
    }
  }
  for (uint32_t i = 0; i < MAX_RULE_TIMERS; i++) {
    if (Rules.timer[i] != 0L) {           // Timer active?
      if (TimeReached(Rules.timer[i])) {  // Timer finished?
        Rules.timer[i] = 0L;              // Turn off this timer
        if ((Settings->rule_enabled || BERRY_RULES) && !Rules.busy) {  // Any rule enabled
          snprintf_P(json_event, sizeof(json_event), PSTR("{\"Rules\":{\"Timer\":%d}}"), i +1);
          RulesProcessEvent(json_event);
        }
      }
    }
  }
}

void RulesSaveBeforeRestart(void)
{
  if ((Settings->rule_enabled || BERRY_RULES) && !Rules.busy) {  // Any rule enabled
    char json_event[32];

    strncpy_P(json_event, PSTR("{\"System\":{\"Save\":1}}"), sizeof(json_event));
    RulesProcessEvent(json_event);
  }
}

void RulesSetPower(void)
{
  Rules.new_power = XdrvMailbox.index;
}

#ifdef SUPPORT_MQTT_EVENT

typedef struct {
  char* event;
  char* topic;
  char* key;
} MQTT_Subscription;
LList<MQTT_Subscription> subscriptions;

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
bool RulesMqttData(void) {
/*
  XdrvMailbox.topic = topic;
  XdrvMailbox.index = strlen(topic);
  XdrvMailbox.data = (char*)data;
  XdrvMailbox.data_len = data_len;
*/
  if ((XdrvMailbox.data_len < 1) || (subscriptions.isEmpty())) {
    return false;                              // Process unchanged data
  }
  bool serviced = false;
  String buData = XdrvMailbox.data;            // Destroyed by JsonParser. Could be very long SENSOR message
  char ctopic[strlen(XdrvMailbox.topic)+1];
  strcpy(ctopic, XdrvMailbox.topic);           // Destroyed by result of following iteration

  for (auto &event_item : subscriptions) {     // Looking for all matched topics
    char etopic[strlen(event_item.topic)+2];
    strcpy(etopic, event_item.topic);          // tele/tasmota/SENSOR
    strcat(etopic, "/");                       // tele/tasmota/SENSOR/
    if ((strcmp(ctopic, event_item.topic) == 0) ||         // Equal tele/tasmota/SENSOR
        (strncmp(ctopic, etopic, strlen(etopic)) == 0)) {  // StartsWith tele/tasmota/SENSOR/

      serviced = true;                         // This topic is subscribed by us, so serve it
      String sData = buData;                   // sData will be destroyed by JsonParser
      char* value = nullptr;
      if (strlen(event_item.key) == 0) {       // If no key specified
        value = (char*)buData.c_str();         // {"DS18B20":{"Id":"0000048EC44C","Temperature":23.3}}
      } else {                                 // If key specified, need to parse Key/Value from JSON data
        JsonParser parser((char*)sData.c_str());
        JsonParserObject jsonData = parser.getRootObject();
        if (!jsonData) { break; }              // Failed to parse JSON data, ignore this message.

        char ckey1[strlen(event_item.key)+1];
        strcpy(ckey1, event_item.key);         // DS18B20.Temperature
        char* ckey2 = strchr(ckey1, '.');
        if (ckey2 != nullptr) {                // .Temperature
          *ckey2++ = '\0';                     // Temperature and ckey1 becomes DS18B20
          JsonParserToken val = jsonData[ckey1].getObject()[ckey2];
          if (val) { 
            value = (char*)val.getStr();       // 23.3
          }
        } else {                               // DS18B20
          JsonParserToken val = jsonData[ckey1];
          if (val) { 
            value = (char*)val.getStr();       // \0
          }
        }
      }
      if (value) {
        Trim(value);
        bool quotes = (value[0] != '{');
        Response_P(PSTR("{\"Event\":{\"%s\":%s%s%s}}"), event_item.event, (quotes)?"\"":"", value, (quotes)?"\"":"");
        RulesProcessEvent(ResponseData());
      }
    }
  }
  return serviced;
}

bool RuleUnsubscribe(const char* event) {
  UpperCase((char*)event, event);
  bool do_all = (strcmp(event, "*") == 0);     // Wildcard
  //Search all subscriptions
  for (auto &index : subscriptions) {
    if (do_all ||                              // All
        (strcmp(event, index.event) == 0)) {   // Equal
      //If find exists one, remove it.
      char stopic[strlen(index.topic)+3];
      strcpy(stopic, index.topic);
      strcat(stopic, "/#");
      MqttUnsubscribe(stopic);
      free(index.key);
      free(index.topic);
      free(index.event);
      subscriptions.remove(&index);
      if (!do_all) {
        return true;
      }
    }
  }
  return do_all;
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
void CmndSubscribe(void) {
  if (XdrvMailbox.data_len > 0) {
    char* event = Trim(strtok(XdrvMailbox.data, ","));
    char* topic = Trim(strtok(nullptr, ","));
    char* key = Trim(strtok(nullptr, ","));

    if (event && topic) {
      RuleUnsubscribe(event);

      // Add "/#" to the topic
      uint32_t slen = strlen(topic);
      char stopic[slen +3];
      strcpy(stopic, topic);
      if (stopic[slen-1] != '#') {
        if (stopic[slen-1] == '/') {
          strcat(stopic, "#");
        } else {
          strcat(stopic, "/#");
        }
      }

      if (!key) { key = EmptyStr; }

      // MQTT Subscribe
      char* hevent = (char*)malloc(strlen(event) +1);
      char* htopic = (char*)malloc(strlen(stopic) -1);  // Remove "/#"
      char* hkey = (char*)malloc(strlen(key) +1);
      if (hevent && htopic && hkey) {
        strcpy(hevent, event);
        strlcpy(htopic, stopic, strlen(stopic)-1);      // Remove "/#" so easy to match
        strcpy(hkey, key);
        MQTT_Subscription &subscription_item = subscriptions.addToLast();
        subscription_item.event = hevent;
        subscription_item.topic = htopic;
        subscription_item.key = hkey;
        char* ftopic = (2 == XdrvMailbox.index)?htopic:stopic;  // Subscribe2
        MqttSubscribe(ftopic);
        ResponseCmnd();                        // {"Subscribe":
        ResponseAppend_P(PSTR("\"%s,%s%s%s\"}"), hevent, ftopic, (strlen(hkey))?",":"", EscapeJSONString(hkey).c_str());
      }        
    }
    return;                                    // {"Error"}
  }
  // If did not specify the event name, list all subscribed event
  bool found = false;
  ResponseCmnd();                              // {"Subscribe":
  for (auto &items : subscriptions) {
    ResponseAppend_P(PSTR("%s%s,%s%s%s"),
      (found) ? "; " : "\"", items.event, items.topic, (strlen(items.key))?",":"", EscapeJSONString(items.key).c_str());
    found = true;  
  }
  ResponseAppend_P((found) ? PSTR("\"}") : PSTR("\"" D_JSON_EMPTY "\"}"));
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
void CmndUnsubscribe(void) {
  if (XdrvMailbox.data_len > 0) {
    char* event = Trim(XdrvMailbox.data);
    if (RuleUnsubscribe(event)) {
      ResponseCmndChar(event);
    }
    return;                                    // {"Error"}
  }
  RuleUnsubscribe("*");
  ResponseCmndDone();
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
#if defined(USE_TIMERS)
  } else if (sVarName.startsWith(F("TIMER"))) {
    uint32_t index = sVarName.substring(5).toInt();
    if (index > 0 && index <= MAX_TIMERS) {
      value = TimerGetTimeOfDay(index -1);
    }
#if defined(USE_SUNRISE)
  } else if (sVarName.equals(F("SUNRISE"))) {
    value = SunMinutes(0);
  } else if (sVarName.equals(F("SUNSET"))) {
    value = SunMinutes(1);
#endif  // USE_SUNRISE
#endif  // USE_TIMERS
// #ifdef USE_ZIGBEE
//   // } else if (sVarName.equals(F("ZBDEVICE"))) {
//   //   value = Z_GetLastDevice();
//   } else if (sVarName.equals(F("ZBGROUP"))) {
//     value = Z_GetLastGroup();
//   } else if (sVarName.equals(F("ZBCLUSTER"))) {
//     value = Z_GetLastCluster();
//   } else if (sVarName.equals(F("ZBENDPOINT"))) {
//     value = Z_GetLastEndpoint();
// #endif  // USE_ZIGBEE
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
float calculateTwoValues(float v1, float v2, uint8_t op) {
  switch (op) {
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
      return FastPrecisePowf(v1, v2);
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
 *      float       - result
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
 * Results in:
 *      (10 + VAR2 ^2) = 235
 *  (MEM1 * 235 - 100) = 605
 *          (2 + MEM2) = 8
 *            605 % 10 = 5
 *            3.14 * 5 = 15.7
 *  VAR10 / 8 = 80 / 8 = 10
 *           15.7 + 10 = 25.7 <== end result
 */
float evaluateExpression(const char * expression, unsigned int len) {
  char expbuf[len + 1];
  memcpy(expbuf, expression, len);
  expbuf[len] = '\0';
  char * scan_pointer = expbuf;

  float object_values[21];
  int8_t operators[20];
  float va;
  // Find and add the value of first object
  if (findNextObjectValue(scan_pointer, va)) {
    object_values[0] = va;
  } else {
    return 0;
  }

  uint32_t operators_size = 0;
  int8_t op;
  while (*scan_pointer) {
    if (findNextOperator(scan_pointer, op)
        && *scan_pointer
        && findNextObjectValue(scan_pointer, va))
    {
      operators[operators_size++] = op;
      object_values[operators_size] = va;
    } else {
      // No operator followed or no more object after this operator, we done.
      break;
    }
    if (operators_size >= 20) {
      AddLog(LOG_LEVEL_ERROR, PSTR("RUL: Too many arguments"));
      return 0;
    }
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Expression '%s'"), expbuf);

  // Going to evaluate the whole expression
  // Calculate by order of operator priorities. Looking for all operators with specified priority (from High to Low)
  for (int32_t priority = MAX_EXPRESSION_OPERATOR_PRIORITY; priority > 0; priority--) {
    int index = 0;
    while (index < operators_size) {
      if (priority == pgm_read_byte(kExpressionOperatorsPriorities + operators[index])) {  // Need to calculate the operator first
        // Get current object value and remove the next object with current operator

//        AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: index %d, v1 '%4_f', v2 '%4_f', op %d"), index, &object_values[index], &object_values[index + 1], operators[index]);

        va = calculateTwoValues(object_values[index], object_values[index + 1], operators[index]);
        uint32_t i = index;
        while (i <= operators_size) {
//          operators[i++] = operators[i];           // operators.remove(index) - Fails on ESP32 (#22636)
          operators[i] = operators[i +1];           // operators.remove(index)
          i++;
          object_values[i] = object_values[i +1];  // object_values.remove(index + 1)
        }
        operators_size--;
        object_values[index] = va;                 // Replace the current value with the result

//        AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Intermediate '%4_f'"), &object_values[index]);

      } else {
        index++;
      }
    }
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Result '%4_f'"), &object_values[0]);

  return object_values[0];
}
#endif  // USE_EXPRESSION

#ifdef  SUPPORT_IF_STATEMENT
/********************************************************************************************/
/*
 * Process an if command
 * Example:
 * rule1 on event#test do backlog status 1; status 2; if (var1==10 AND var3==9 OR var4==8) status 3;status 4 endif; status 5; status 6 endon
 * 
 * Notice:
 * In case of "if" is true commands ``status 3`` and ``status 4`` will be inserted into the backlog between ``status 2`` and ``status 5``
 */
void CmndIf(void) {
  if (XdrvMailbox.data_len > 0) {
    char parameters[XdrvMailbox.data_len +1];
    strcpy(parameters, XdrvMailbox.data);
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
    case COMPARE_OPERATOR_STRING_ENDS_WITH:
      bResult = leftExpr.endsWith(rightExpr);
      break;
    case COMPARE_OPERATOR_STRING_STARTS_WITH:
      bResult = leftExpr.startsWith(rightExpr);
      break;
    case COMPARE_OPERATOR_STRING_CONTAINS:
      bResult = (leftExpr.indexOf(rightExpr) >= 0);
      break;
    case  COMPARE_OPERATOR_STRING_NOT_EQUAL:
      bResult = !leftExpr.equalsIgnoreCase(rightExpr);  // Compare strings - this also works for hexadecimals
      break;
    case  COMPARE_OPERATOR_STRING_NOT_CONTAINS:
      bResult = (leftExpr.indexOf(rightExpr) < 0);
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
bool evaluateLogicalExpression(const char * expression, int len) {
  //Make a copy first
  char expbuff[len + 1];
  memcpy(expbuff, expression, len);
  expbuff[len] = '\0';
  char * pointer = expbuff;

  bool values[21];
  int8_t logicOperators[20];

  //Find first comparison expression
  bool bValue;
  if (findNextLogicObjectValue(pointer, bValue)) {
    values[0] = bValue;
  } else {
    return false;
  }

  uint32_t logicOperators_size = 0;
  int8_t op;
  while (*pointer) {
    if (findNextLogicOperator(pointer, op)
      && (*pointer) && findNextLogicObjectValue(pointer, bValue))
    {
      logicOperators[logicOperators_size++] = op;
      values[logicOperators_size] = bValue;
    } else {
      break;
    }
    if (logicOperators_size >= 20) {
      AddLog(LOG_LEVEL_ERROR, PSTR("RUL: Too many arguments"));
      return false;
    }
  }

  // Calculate all "AND" first
  int index = 0;
  while (index < logicOperators_size) {
    if (logicOperators[index] == LOGIC_OPERATOR_AND) {
      values[index] &= values[index +1];
      uint32_t i = index;
      while (i <= logicOperators_size) {
        logicOperators[i++] = logicOperators[i];  // logicOperators.remove(index);
        values[i] = values[i +1];                 // values.remove(index + 1);
      }
      logicOperators_size--;
    } else {
      index++;
    }
  }
  // Then, calculate all "OR"
  index = 0;
  while (index < logicOperators_size) {
    if (logicOperators[index] == LOGIC_OPERATOR_OR) {
      values[index] |= values[index +1];
      uint32_t i = index;
      while (i <= logicOperators_size) {
        logicOperators[i++] = logicOperators[i];  // logicOperators.remove(index);
        values[i] = values[i +1];                 // values.remove(index + 1);
      }
      logicOperators_size--;
    } else {
      index++;
    }
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Expression '%s' = %d"), expbuff, values[0]);

  return values[0];
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
    char* blcommand = oneCommand;
    Trim(blcommand);

//    AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Position %d, Command '%s'"), insertPosition, blcommand);

    if (strlen(blcommand)) {
      //Insert into backlog
      char* temp = (char*)malloc(strlen(blcommand)+1);
      if (temp != nullptr) {
        strcpy(temp, blcommand);
        char* &elem = backlog.insertAt(insertPosition++);
        elem = temp;
      }
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
#endif  // SUPPORT_IF_STATEMENT

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndRule(void)
{
  if (0 == XdrvMailbox.index) {
    char data = '\0';
    if (XdrvMailbox.data_len > 0) {  // Allow show all if 0
      if (!((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 10))) {
        if ('"' == XdrvMailbox.data[0]) {
          data = '"';                // Save data as XdrvMailbox.data is destroyed
        } else {
          XdrvMailbox.data_len = 0;  // Discard any additional text
        }
      }
    }
    for (uint32_t i = 1; i <= MAX_RULE_SETS; i++) {
      XdrvMailbox.index = i;
      XdrvMailbox.data[0] = data;    // Only 0 or "
      CmndRule();
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, XdrvMailbox.command);
    }
    ResponseClear();                 // Disable further processing
    return;
  }
  uint8_t index = XdrvMailbox.index;
  if ((index > 0) && (index <= MAX_RULE_SETS)) {
    // if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.data_len < sizeof(Settings->rules[index -1]))) {    // TODO postpone size calculation
    if (XdrvMailbox.data_len > 0) {    // TODO postpone size calculation
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 10)) {
        switch (XdrvMailbox.payload) {
        case 0: // Off
        case 1: // On
          bitWrite(Settings->rule_enabled, index -1, XdrvMailbox.payload);
          break;
        case 2: // Toggle
          bitWrite(Settings->rule_enabled, index -1, bitRead(Settings->rule_enabled, index -1) ^1);
          break;
        case 4: // Off
        case 5: // On
          bitWrite(Settings->rule_once, index -1, XdrvMailbox.payload &1);
          break;
        case 6: // Toggle
          bitWrite(Settings->rule_once, index -1, bitRead(Settings->rule_once, index -1) ^1);
          break;
        case 8: // Off
        case 9: // On
          bitWrite(Settings->rule_stop, index -1, XdrvMailbox.payload &1);
          break;
        case 10: // Toggle
          bitWrite(Settings->rule_stop, index -1, bitRead(Settings->rule_stop, index -1) ^1);
          break;
        }
      } else {
        bool append = false;
        if ('+' == XdrvMailbox.data[0]) {
          XdrvMailbox.data[0] = ' ';  // Remove + and make sure at least one space is inserted
          append = true;
        }
        int32_t res = SetRule(index - 1, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data, append);
        if (res < 0) {
          AddLog(LOG_LEVEL_ERROR, PSTR("RUL: Not enough space"));
        }
      }
      Rules.triggers[index -1] = 0;  // Reset once flag
    }
    String rule = GetRule(index - 1);
    size_t rule_len = rule.length();
    if (rule_len > MAX_RULE_SIZE - 3) {

      size_t start_index = 0;                                       // start from 0
      while (start_index < rule_len) {                              // until we reached end of rule
        size_t last_index = start_index + MAX_RULE_SIZE - 3;        // set max length to what would fit uncompressed, i.e. MAX_RULE_SIZE - 3 (first NULL + length + last NULL)
        if (last_index < rule_len) {                                // if we didn't reach the end, try to shorten to last space character
          int32_t next_index = rule.lastIndexOf(" ", last_index);
          if (next_index > start_index) {                           // if space was found and is not before start_index (i.e. we are progressing)
            last_index = next_index;                                // shrink to the last space
          }                                                         // otherwise it means there are no spaces, we need to cut somewhere even if the result cannot be entered back
        } else {
          last_index = rule_len;                                    // until the end of the rule
        }
        AddLog(LOG_LEVEL_INFO, PSTR("RUL: Rule%d %s%s"),
                                        index, 0 == start_index ? PSTR("") : PSTR("+"),
                                        rule.substring(start_index, last_index).c_str());
        start_index = last_index + 1;
      }

      // we need to split the rule in chunks
//      rule = rule.substring(0, MAX_RULE_SIZE);
//      rule += F("...");
    }
    Response_P(PSTR("{\"%s%d\":{\"State\":\"%s\",\"Once\":\"%s\",\"StopOnError\":\"%s\",\"Length\":%d,\"Free\":%d,\"Rules\":\"%s\"}}"),
      XdrvMailbox.command, index, GetStateText(bitRead(Settings->rule_enabled, index -1)), GetStateText(bitRead(Settings->rule_once, index -1)),
      GetStateText(bitRead(Settings->rule_stop, index -1)),
      rule_len, MAX_RULE_SIZE - GetRuleLenStorage(index - 1),
      EscapeJSONString(rule.c_str()).c_str());
  }
}

void CmndRuleTimer(void)
{
  if (XdrvMailbox.index > MAX_RULE_TIMERS) { return; }

  uint32_t i = XdrvMailbox.index;
  uint32_t max_i = XdrvMailbox.index;
  if (0 == i) {
    i = 1;
    max_i = MAX_RULE_TIMERS;
  }
#ifdef USE_EXPRESSION
  float timer_set = evaluateExpression(XdrvMailbox.data, XdrvMailbox.data_len);
  timer_set = (timer_set > 0) ? millis() + (1000 * timer_set) : 0;
#else
  uint32_t timer_set = (XdrvMailbox.payload > 0) ? millis() + (1000 * XdrvMailbox.payload) : 0;
#endif  // USE_EXPRESSION
  if (XdrvMailbox.data_len > 0) {
    for ( ; i <= max_i ; ++i ) {
      Rules.timer[i -1] = timer_set;
    }
  }
  ResponseClear();
  for (i = 0; i < MAX_RULE_TIMERS; i++) {
    ResponseAppend_P(PSTR("%c\"T%d\":%d"), (i) ? ',' : '{', i +1, (Rules.timer[i]) ? (Rules.timer[i] - millis()) / 1000 : 0);
  }
  ResponseJsonEnd();
}

void CmndEvent(void)
{
  if (XdrvMailbox.data_len > 0) {
    strlcpy(Rules.event_data, XdrvMailbox.data, sizeof(Rules.event_data));
#ifdef USE_DEVICE_GROUPS
    if (!XdrvMailbox.grpflg) SendDeviceGroupMessage(1, DGR_MSGTYP_UPDATE, DGR_ITEM_EVENT, XdrvMailbox.data);
#endif  // USE_DEVICE_GROUPS
  }
  if (XdrvMailbox.command) ResponseCmndDone();
}

void CmndVariable(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_RULE_VARS)) {
    if (!XdrvMailbox.usridx) {
      ResponseClear();
      for (uint32_t i = 0; i < MAX_RULE_VARS; i++) {
        ResponseAppend_P(PSTR("%c\"Var%d\":\"%s\""), (i) ? ',' : '{', i +1, rules_vars[i]);
      }
      ResponseJsonEnd();
    } else {
      if (XdrvMailbox.data_len > 0) {
#ifdef USE_EXPRESSION
        if (XdrvMailbox.data[0] == '=') {  // Spaces already been skipped in data
          dtostrfd(evaluateExpression(XdrvMailbox.data + 1, XdrvMailbox.data_len - 1), Settings->flag2.calc_resolution, rules_vars[XdrvMailbox.index -1]);
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
          char rules_mem[FLOATSZ];
          dtostrfd(evaluateExpression(XdrvMailbox.data + 1, XdrvMailbox.data_len - 1), Settings->flag2.calc_resolution, rules_mem);
          SettingsUpdateText(SET_MEM1 + XdrvMailbox.index -1, rules_mem);
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
    Settings->flag2.calc_resolution = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->flag2.calc_resolution);
}

void CmndAddition(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      float tempvar = CharToFloat(rules_vars[XdrvMailbox.index -1]) + CharToFloat(XdrvMailbox.data);
      dtostrfd(tempvar, Settings->flag2.calc_resolution, rules_vars[XdrvMailbox.index -1]);
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
      dtostrfd(tempvar, Settings->flag2.calc_resolution, rules_vars[XdrvMailbox.index -1]);
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
      dtostrfd(tempvar, Settings->flag2.calc_resolution, rules_vars[XdrvMailbox.index -1]);
      bitSet(Rules.vars_event, XdrvMailbox.index -1);
    }
    ResponseCmndIdxChar(rules_vars[XdrvMailbox.index -1]);
  }
}

void CmndScale(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_RULE_VARS)) {
    if (XdrvMailbox.data_len > 0) {
      if (ArgC() == 5) {  // Process parameter entry
        char argument[XdrvMailbox.data_len];

        float valueIN = CharToFloat(ArgV(argument, 1));
        float fromLow = CharToFloat(ArgV(argument, 2));
        float fromHigh = CharToFloat(ArgV(argument, 3));
        float toLow = CharToFloat(ArgV(argument, 4));
        float toHigh = CharToFloat(ArgV(argument, 5));
        float value = map_float(valueIN, fromLow, fromHigh, toLow, toHigh);
        dtostrfd(value, Settings->flag2.calc_resolution, rules_vars[XdrvMailbox.index -1]);
        bitSet(Rules.vars_event, XdrvMailbox.index -1);
      } else {
        return;  // Command Error
      }
    }
    ResponseCmndIdxChar(rules_vars[XdrvMailbox.index -1]);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv10(uint32_t function)
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
    case FUNC_TELEPERIOD_RULES_PROCESS:
      Rules.teleperiod = true;
      result = RulesProcess();
      Rules.teleperiod = false;
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
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif  // Do not USE_SCRIPT
#endif  // USE_RULES
