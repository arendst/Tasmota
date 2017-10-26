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

#ifndef MY_LANGUAGE
  #include "language/en-GB.h"
#else
  #define QUOTEME(x) QUOTEME_1(x)
  #define QUOTEME_1(x) #x
  #define INCLUDE_FILE(x) QUOTEME(language/x.h)
  #include INCLUDE_FILE(MY_LANGUAGE)
#endif

// Common
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
const char commands[MAX_BUTTON_COMMANDS][14] PROGMEM = {
  D_CMND_WIFICONFIG " 1",   // Press button three times
  D_CMND_WIFICONFIG " 2",   // Press button four times
  D_CMND_WIFICONFIG " 3",   // Press button five times
  D_CMND_RESTART " 1",      // Press button six times
  D_CMND_UPGRADE " 1" };    // Press button seven times
const char wificfg[MAX_WIFI_OPTION][WCFG_MAX_STRING_LENGTH] PROGMEM = {
  D_WCFG_0_RESTART,
  D_WCFG_1_SMARTCONFIG,
  D_WCFG_2_WIFIMANAGER,
  D_WCFG_3_WPSCONFIG,
  D_WCFG_4_RETRY,
  D_WCFG_5_WAIT };
const char PREFIXES[3][PRFX_MAX_STRING_LENGTH] PROGMEM = {
  D_CMND,
  D_STAT,
  D_TELE };

const char JSON_SNS_TEMPHUM[] PROGMEM =
  "%s, \"%s\":{\"" D_TEMPERATURE "\":%s, \"" D_HUMIDITY "\":%s}";

// support.ino
static const char monthNames[] = D_MONTH3LIST;

// webserver.ino
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
