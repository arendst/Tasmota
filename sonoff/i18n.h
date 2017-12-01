/*
  i18n.h - internationalization for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

#ifndef _I18N_H_
#define _I18N_H_

#ifndef MY_LANGUAGE
  #include "language/en-GB.h"
#else
  #define QUOTEME(x) QUOTEME_1(x)
  #define QUOTEME_1(x) #x
  #define INCLUDE_FILE(x) QUOTEME(language/x.h)
  #include INCLUDE_FILE(MY_LANGUAGE)
#endif

// Common
enum UnitNames {
  UNIT_AMPERE,
  UNIT_HOUR,
  UNIT_KILOWATTHOUR,
  UNIT_LUX,
  UNIT_MICROSECOND,
  UNIT_MILLIAMPERE,
  UNIT_MILLISECOND,
  UNIT_MINUTE,
  UNIT_PRESSURE,
  UNIT_SECOND,
  UNIT_SECTORS,
  UNIT_VOLT,
  UNIT_WATT,
  UNIT_WATTHOUR };
const char kUnitNames[] PROGMEM =
  D_UNIT_AMPERE "|"
  D_UNIT_HOUR "|"
  D_UNIT_KILOWATTHOUR "|"
  D_UNIT_LUX "|"
  D_UNIT_MICROSECOND "|"
  D_UNIT_MILLIAMPERE "|"
  D_UNIT_MILLISECOND "|"
  D_UNIT_MINUTE "|"
  D_UNIT_PRESSURE "|"
  D_UNIT_SECOND "|"
  D_UNIT_SECTORS "|"
  D_UNIT_VOLT "|"
  D_UNIT_WATT "|"
  D_UNIT_WATTHOUR ;

const char S_JSON_COMMAND_NVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%d %s\"}";
const char S_JSON_COMMAND_SVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%s %s\"}";
const char S_JSON_COMMAND_NVALUE_UNIT[] PROGMEM =             "{\"%s\":\"%d%s\"}";
const char S_JSON_COMMAND_NVALUE_UNIT_NVALUE_UNIT[] PROGMEM = "{\"%s\":\"%d%s (%d%s)\"}";

const char S_JSON_COMMAND_NVALUE_SVALUE[] PROGMEM =           "{\"%s\":\"%d (%s)\"}";
const char S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE[] PROGMEM =    "{\"%s\":\"%d (" D_ACTIVE " %d)\"}";

const char S_JSON_COMMAND_NVALUE[] PROGMEM =                  "{\"%s\":%d}";
const char S_JSON_COMMAND_SVALUE[] PROGMEM =                  "{\"%s\":\"%s\"}";
const char S_JSON_COMMAND_XVALUE[] PROGMEM =                  "{\"%s\":%s}";  // %s must provide quotes on non-number

const char S_JSON_COMMAND_INDEX_NVALUE[] PROGMEM =            "{\"%s%d\":%d}";
const char S_JSON_COMMAND_INDEX_SVALUE[] PROGMEM =            "{\"%s%d\":\"%s\"}";
const char S_JSON_COMMAND_INDEX_SVALUE_SVALUE[] PROGMEM =     "{\"%s%d\":\"%s%s\"}";

const char JSON_SNS_TEMPHUM[] PROGMEM = "%s,\"%s\":{\"" D_TEMPERATURE "\":%s,\"" D_HUMIDITY "\":%s}";

const char S_LOG_I2C_FOUND_AT[] PROGMEM = D_LOG_I2C "%s " D_FOUND_AT " 0x%x";

const char S_LOG_HTTP[] PROGMEM = D_LOG_HTTP;
const char S_LOG_WIFI[] PROGMEM = D_LOG_WIFI;
const char S_LOG_MQTT[] PROGMEM = D_LOG_MQTT;
const char S_RSLT_POWER[] PROGMEM = D_RSLT_POWER;
const char S_RSLT_RESULT[] PROGMEM = D_RSLT_RESULT;
const char S_RSLT_WARNING[] PROGMEM = D_RSLT_WARNING;
const char S_LWT[] PROGMEM = D_LWT;
const char S_OFFLINE[] PROGMEM = D_OFFLINE;

// sonoff.ino
#define MAX_BUTTON_COMMANDS  5  // Max number of button commands supported
const char kCommands[MAX_BUTTON_COMMANDS][14] PROGMEM = {
  D_CMND_WIFICONFIG " 1",   // Press button three times
  D_CMND_WIFICONFIG " 2",   // Press button four times
  D_CMND_WIFICONFIG " 3",   // Press button five times
  D_CMND_RESTART " 1",      // Press button six times
  D_CMND_UPGRADE " 1" };    // Press button seven times
const char kWifiConfig[MAX_WIFI_OPTION][WCFG_MAX_STRING_LENGTH] PROGMEM = {
  D_WCFG_0_RESTART,
  D_WCFG_1_SMARTCONFIG,
  D_WCFG_2_WIFIMANAGER,
  D_WCFG_3_WPSCONFIG,
  D_WCFG_4_RETRY,
  D_WCFG_5_WAIT };
const char kPrefixes[3][PRFX_MAX_STRING_LENGTH] PROGMEM = {
  D_CMND,
  D_STAT,
  D_TELE };

// support.ino
static const char kMonthNames[] = D_MONTH3LIST;

// webserver.ino
#ifdef USE_WEBSERVER
const char HTTP_SNS_TEMP[] PROGMEM = "%s{s}%s " D_TEMPERATURE "{m}%s&deg;%c{e}";                             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_HUM[] PROGMEM = "%s{s}%s " D_HUMIDITY "{m}%s%{e}";                                       // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_PRESSURE[] PROGMEM = "%s{s}%s " D_PRESSURE "{m}%s " D_UNIT_PRESSURE "{e}";               // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_SEAPRESSURE[] PROGMEM = "%s{s}%s " D_PRESSUREATSEALEVEL "{m}%s " D_UNIT_PRESSURE "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_ANALOG[] PROGMEM = "%s{s}%s " D_ANALOG_INPUT "%d{m}%d{e}";                               // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

const char S_MAIN_MENU[] PROGMEM = D_MAIN_MENU;
const char S_CONFIGURATION[] PROGMEM = D_CONFIGURATION;
const char S_CONFIGURE_MODULE[] PROGMEM = D_CONFIGURE_MODULE;
const char S_CONFIGURE_WIFI[] PROGMEM = D_CONFIGURE_WIFI;
const char S_NO_NETWORKS_FOUND[] PROGMEM = D_NO_NETWORKS_FOUND;
const char S_CONFIGURE_MQTT[] PROGMEM = D_CONFIGURE_MQTT;
const char S_CONFIGURE_LOGGING[] PROGMEM = D_CONFIGURE_LOGGING;
const char S_CONFIGURE_OTHER[] PROGMEM = D_CONFIGURE_OTHER;
const char S_SAVE_CONFIGURATION[] PROGMEM = D_SAVE_CONFIGURATION;
const char S_RESET_CONFIGURATION[] PROGMEM = D_RESET_CONFIGURATION;
const char S_RESTORE_CONFIGURATION[] PROGMEM = D_RESTORE_CONFIGURATION;
const char S_FIRMWARE_UPGRADE[] PROGMEM = D_FIRMWARE_UPGRADE;
const char S_CONSOLE[] PROGMEM = D_CONSOLE;
const char S_INFORMATION[] PROGMEM = D_INFORMATION;
const char S_RESTART[] PROGMEM = D_RESTART;
#endif  // USE_WEBSERVER

#endif  // _I18N_H_