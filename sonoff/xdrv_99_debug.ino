/*
  xdrv_99_debug.ino - debug support for Sonoff-Tasmota

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

//#define USE_DEBUG_DRIVER

#ifdef DEBUG_THEO
#ifndef USE_DEBUG_DRIVER
#define USE_DEBUG_DRIVER
#endif  // USE_DEBUG_DRIVER
#endif  // DEBUG_THEO

//#define USE_DEBUG_SETTING_NAMES

#ifdef USE_DEBUG_DRIVER
/*********************************************************************************************\
 * Virtual debugging support - Part1
 *
 * Needs file zzzz_debug.ino due to DEFINE processing
\*********************************************************************************************/

#define XDRV_99             99

#ifndef CPU_LOAD_CHECK
#define CPU_LOAD_CHECK      1                 // Seconds between each CPU_LOAD log
#endif

/*********************************************************************************************\
 * Debug commands
\*********************************************************************************************/

#define D_CMND_CFGDUMP   "CfgDump"
#define D_CMND_CFGPOKE   "CfgPoke"
#define D_CMND_CFGPEEK   "CfgPeek"
#define D_CMND_CFGSHOW   "CfgShow"
#define D_CMND_CFGXOR    "CfgXor"
#define D_CMND_CPUCHECK  "CpuChk"
#define D_CMND_EXCEPTION "Exception"
#define D_CMND_FREEMEM   "FreeMem"
#define D_CMND_RTCDUMP   "RtcDump"
#define D_CMND_HELP      "Help"
#define D_CMND_SETSENSOR "SetSensor"
#define D_CMND_FLASHMODE "FlashMode"

enum DebugCommands {
  CMND_CFGDUMP, CMND_CFGPEEK, CMND_CFGPOKE, CMND_CFGSHOW, CMND_CFGXOR,
  CMND_CPUCHECK, CMND_EXCEPTION, CMND_FREEMEM, CMND_RTCDUMP, CMND_SETSENSOR, CMND_FLASHMODE, CMND_HELP };
const char kDebugCommands[] PROGMEM =
  D_CMND_CFGDUMP "|" D_CMND_CFGPEEK "|" D_CMND_CFGPOKE "|" D_CMND_CFGSHOW "|" D_CMND_CFGXOR "|"
  D_CMND_CPUCHECK "|" D_CMND_EXCEPTION "|" D_CMND_FREEMEM "|" D_CMND_RTCDUMP "|" D_CMND_SETSENSOR "|" D_CMND_FLASHMODE "|" D_CMND_HELP;

uint32_t CPU_loops = 0;
uint32_t CPU_last_millis = 0;
uint32_t CPU_last_loop_time = 0;
uint8_t CPU_load_check = 0;
uint8_t CPU_show_freemem = 0;

/*******************************************************************************************/

#ifdef DEBUG_THEO
void ExceptionTest(uint8_t type)
{
/*
Exception (28):
epc1=0x4000bf64 epc2=0x00000000 epc3=0x00000000 excvaddr=0x00000007 depc=0x00000000

ctx: cont
sp: 3fff1f30 end: 3fff2840 offset: 01a0

>>>stack>>>
3fff20d0:  202c3573 756f7247 2c302070 646e4920
3fff20e0:  40236a6e 7954202c 45206570 00454358
3fff20f0:  00000010 00000007 00000000 3fff2180
3fff2100:  3fff2190 40107bfc 3fff3e4c 3fff22c0
3fff2110:  40261934 000000f0 3fff22c0 401004d8
3fff2120:  40238fcf 00000050 3fff2100 4021fc10
3fff2130:  3fff32bc 4021680c 3ffeade1 4021ff7d
3fff2140:  3fff2190 3fff2180 0000000c 7fffffff
3fff2150:  00000019 00000000 00000000 3fff21c0
3fff2160:  3fff23f3 3ffe8e08 00000000 4021ffb4
3fff2170:  3fff2190 3fff2180 0000000c 40201118
3fff2180:  3fff21c0 0000003c 3ffef840 00000007
3fff2190:  00000000 00000000 00000000 40201128
3fff21a0:  3fff23f3 000000f1 3fff23ec 4020fafb
3fff21b0:  3fff23f3 3fff21c0 3fff21d0 3fff23f6
3fff21c0:  00000000 3fff23fb 4022321b 00000000

Exception 28: LoadProhibited: A load referenced a page mapped with an attribute that does not permit loads
Decoding 14 results
0x40236a6e: ets_vsnprintf at ?? line ?
0x40107bfc: vsnprintf at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/libc_replacements.c line 387
0x40261934: bignum_exptmod at ?? line ?
0x401004d8: malloc at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266\umm_malloc/umm_malloc.c line 1664
0x40238fcf: wifi_station_get_connect_status at ?? line ?
0x4021fc10: operator new[](unsigned int) at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/abi.cpp line 57
0x4021680c: ESP8266WiFiSTAClass::status() at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\libraries\ESP8266WiFi\src/ESP8266WiFiSTA.cpp line 569
0x4021ff7d: vsnprintf_P(char*, unsigned int, char const*, __va_list_tag) at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/pgmspace.cpp line 146
0x4021ffb4: snprintf_P(char*, unsigned int, char const*, ...) at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/pgmspace.cpp line 146
0x40201118: atol at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/core_esp8266_noniso.c line 45
0x40201128: atoi at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/core_esp8266_noniso.c line 45
0x4020fafb: MqttDataHandler(char*, unsigned char*, unsigned int) at R:\Arduino\Work-ESP8266\Theo\sonoff\sonoff-4\sonoff/sonoff.ino line 679 (discriminator 1)
0x4022321b: pp_attach at ?? line ?

00:00:08 MQTT: tele/sonoff/INFO3 = {"Started":"Fatal exception:28 flag:2 (EXCEPTION) epc1:0x4000bf64 epc2:0x00000000 epc3:0x00000000 excvaddr:0x00000007 depc:0x00000000"}
*/
  if (1 == type) {
    char svalue[10];
    snprintf_P(svalue, sizeof(svalue), PSTR("%s"), 7);  // Exception 28 as number in string (7 in excvaddr)
  }
/*
14:50:52 osWatch: FreeRam 25896, rssi 68, last_run 0
14:51:02 osWatch: FreeRam 25896, rssi 58, last_run 0
14:51:03 CMND: exception 2
14:51:12 osWatch: FreeRam 25360, rssi 60, last_run 8771
14:51:22 osWatch: FreeRam 25360, rssi 62, last_run 18771
14:51:32 osWatch: FreeRam 25360, rssi 62, last_run 28771
14:51:42 osWatch: FreeRam 25360, rssi 62, last_run 38771
14:51:42 osWatch: Warning, loop blocked. Restart now
*/
  if (2 == type) {
    while(1) delay(1000);  // this will trigger the os watch
  }
}

#endif  // DEBUG_THEO

/*******************************************************************************************/

void CpuLoadLoop(void)
{
  CPU_last_loop_time = millis();
  if (CPU_load_check && CPU_last_millis) {
    CPU_loops ++;
    if ((CPU_last_millis + (CPU_load_check *1000)) <= CPU_last_loop_time) {
#if defined(F_CPU) && (F_CPU == 160000000L)
      int CPU_load = 100 - ( (CPU_loops*(1 + 30*sleep)) / (CPU_load_check *800) );
      CPU_loops = CPU_loops / CPU_load_check;
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, CPU %d%%(160MHz), Loops/sec %d"), ESP.getFreeHeap(), CPU_load, CPU_loops);
#else
      int CPU_load = 100 - ( (CPU_loops*(1 + 30*sleep)) / (CPU_load_check *400) );
      CPU_loops = CPU_loops / CPU_load_check;
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, CPU %d%%(80MHz), Loops/sec %d"), ESP.getFreeHeap(), CPU_load, CPU_loops);
#endif
      CPU_last_millis = CPU_last_loop_time;
      CPU_loops = 0;
    }
  }
}

/*******************************************************************************************/

#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1)
// All version before core 2.4.2
// https://github.com/esp8266/Arduino/issues/2557

extern "C" {
#include <cont.h>
  extern cont_t g_cont;
}

void DebugFreeMem(void)
{
  register uint32_t *sp asm("a1");

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d, UnmodifiedStack %d (%s)"), ESP.getFreeHeap(), 4 * (sp - g_cont.stack), cont_get_free_stack(&g_cont), XdrvMailbox.data);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d (%s)"), ESP.getFreeHeap(), 4 * (sp - g_cont.stack), XdrvMailbox.data);
}

#else
// All version from core 2.4.2
// https://github.com/esp8266/Arduino/pull/5018
// https://github.com/esp8266/Arduino/pull/4553

extern "C" {
#include <cont.h>
  extern cont_t* g_pcont;
}

void DebugFreeMem(void)
{
  register uint32_t *sp asm("a1");

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d (%s)"), ESP.getFreeHeap(), 4 * (sp - g_pcont->stack), XdrvMailbox.data);
}

#endif  // ARDUINO_ESP8266_RELEASE_2_x_x

/*******************************************************************************************/

void DebugRtcDump(char* parms)
{
  #define CFG_COLS 16

  uint16_t idx;
  uint16_t maxrow;
  uint16_t row;
  uint16_t col;
  char *p;

  // |<--SDK data (256 bytes)-->|<--User data (512 bytes)-->|
  // 000 - 0FF: SDK
  //  000 - 01B: SDK rst_info
  // 100 - 2FF: User
  //  280 - 283: Tasmota RtcReboot   (Offset 100 (x 4bytes) - sizeof(RTCRBT) (x 4bytes))
  //  290 - 2EB: Tasmota RtcSettings (Offset 100 (x 4bytes))

  uint8_t buffer[768];
//  ESP.rtcUserMemoryRead(0, (uint32_t*)&buffer, sizeof(buffer));
  system_rtc_mem_read(0, (uint32_t*)&buffer, sizeof(buffer));

  maxrow = ((sizeof(buffer)+CFG_COLS)/CFG_COLS);

  uint16_t srow = strtol(parms, &p, 16) / CFG_COLS;
  uint16_t mrow = strtol(p, &p, 10);

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Cnfg: Parms %s, Start row %d, rows %d"), parms, srow, mrow);

  if (0 == mrow) {  // Default only 8 lines
    mrow = 8;
  }
  if (srow > maxrow) {
    srow = maxrow - mrow;
  }
  if (mrow < (maxrow - srow)) {
    maxrow = srow + mrow;
  }

  for (row = srow; row < maxrow; row++) {
    idx = row * CFG_COLS;
    snprintf_P(log_data, sizeof(log_data), PSTR("%03X:"), idx);
    for (col = 0; col < CFG_COLS; col++) {
      if (!(col%4)) {
        snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
      }
      snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, buffer[idx + col]);
    }
    snprintf_P(log_data, sizeof(log_data), PSTR("%s |"), log_data);
    for (col = 0; col < CFG_COLS; col++) {
//      if (!(col%4)) {
//        snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
//      }
      snprintf_P(log_data, sizeof(log_data), PSTR("%s%c"), log_data, ((buffer[idx + col] > 0x20) && (buffer[idx + col] < 0x7F)) ? (char)buffer[idx + col] : ' ');
    }
    snprintf_P(log_data, sizeof(log_data), PSTR("%s|"), log_data);
    AddLog(LOG_LEVEL_INFO);
  }
}

/*******************************************************************************************/

void DebugCfgDump(char* parms)
{
  #define CFG_COLS 16

  uint16_t idx;
  uint16_t maxrow;
  uint16_t row;
  uint16_t col;
  char *p;

  uint8_t *buffer = (uint8_t *) &Settings;
  maxrow = ((sizeof(SYSCFG)+CFG_COLS)/CFG_COLS);

  uint16_t srow = strtol(parms, &p, 16) / CFG_COLS;
  uint16_t mrow = strtol(p, &p, 10);

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Cnfg: Parms %s, Start row %d, rows %d"), parms, srow, mrow);

  if (0 == mrow) {  // Default only 8 lines
    mrow = 8;
  }
  if (srow > maxrow) {
    srow = maxrow - mrow;
  }
  if (mrow < (maxrow - srow)) {
    maxrow = srow + mrow;
  }

  for (row = srow; row < maxrow; row++) {
    idx = row * CFG_COLS;
    snprintf_P(log_data, sizeof(log_data), PSTR("%03X:"), idx);
    for (col = 0; col < CFG_COLS; col++) {
      if (!(col%4)) {
        snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
      }
      snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, buffer[idx + col]);
    }
    snprintf_P(log_data, sizeof(log_data), PSTR("%s |"), log_data);
    for (col = 0; col < CFG_COLS; col++) {
//      if (!(col%4)) {
//        snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
//      }
      snprintf_P(log_data, sizeof(log_data), PSTR("%s%c"), log_data, ((buffer[idx + col] > 0x20) && (buffer[idx + col] < 0x7F)) ? (char)buffer[idx + col] : ' ');
    }
    snprintf_P(log_data, sizeof(log_data), PSTR("%s|"), log_data);
    AddLog(LOG_LEVEL_INFO);
    delay(1);
  }
}

void DebugCfgPeek(char* parms)
{
  char *p;

  uint16_t address = strtol(parms, &p, 16);
  if (address > sizeof(SYSCFG)) address = sizeof(SYSCFG) -4;
  address = (address >> 2) << 2;

  uint8_t *buffer = (uint8_t *) &Settings;
  uint8_t data8 = buffer[address];
  uint16_t data16 = (buffer[address +1] << 8) + buffer[address];
  uint32_t data32 = (buffer[address +3] << 24) + (buffer[address +2] << 16) + data16;

  snprintf_P(log_data, sizeof(log_data), PSTR("%03X:"), address);
  for (uint8_t i = 0; i < 4; i++) {
    snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, buffer[address +i]);
  }
  snprintf_P(log_data, sizeof(log_data), PSTR("%s |"), log_data);
  for (uint8_t i = 0; i < 4; i++) {
    snprintf_P(log_data, sizeof(log_data), PSTR("%s%c"), log_data, ((buffer[address +i] > 0x20) && (buffer[address +i] < 0x7F)) ? (char)buffer[address +i] : ' ');
  }
  snprintf_P(log_data, sizeof(log_data), PSTR("%s| 0x%02X (%d), 0x%04X (%d), 0x%0LX (%lu)"), log_data, data8, data8, data16, data16, data32, data32);
  AddLog(LOG_LEVEL_INFO);
}

void DebugCfgPoke(char* parms)
{
  char *p;

  uint16_t address = strtol(parms, &p, 16);
  if (address > sizeof(SYSCFG)) address = sizeof(SYSCFG) -4;
  address = (address >> 2) << 2;

  uint32_t data = strtol(p, &p, 16);

  uint8_t *buffer = (uint8_t *) &Settings;
  uint32_t data32 = (buffer[address +3] << 24) + (buffer[address +2] << 16) + (buffer[address +1] << 8) + buffer[address];

  uint8_t *nbuffer = (uint8_t *) &data;
  for (uint8_t i = 0; i < 4; i++) { buffer[address +i] = nbuffer[+i]; }

  uint32_t ndata32 = (buffer[address +3] << 24) + (buffer[address +2] << 16) + (buffer[address +1] << 8) + buffer[address];

  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: 0x%0LX (%lu) poked to 0x%0LX (%lu)"), address, data32, data32, ndata32, ndata32);
}

#ifdef USE_DEBUG_SETTING_NAMES
void DebugCfgShow(uint8_t more)
{
  uint8_t *SetAddr;
  SetAddr = (uint8_t *)&Settings;

  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: Hostname (%d)         [%s]"), (uint8_t *)&Settings.hostname - SetAddr, sizeof(Settings.hostname)-1, Settings.hostname);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: SSids (%d)            [%s], [%s]"), (uint8_t *)&Settings.sta_ssid - SetAddr, sizeof(Settings.sta_ssid[0])-1, Settings.sta_ssid[0], Settings.sta_ssid[1]);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: Friendlynames (%d)    [%s], [%s], [%s], [%s]"), (uint8_t *)&Settings.friendlyname - SetAddr, sizeof(Settings.friendlyname[0])-1, Settings.friendlyname[0], Settings.friendlyname[1], Settings.friendlyname[2], Settings.friendlyname[3]);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: OTA Url (%d)          [%s]"), (uint8_t *)&Settings.ota_url - SetAddr, sizeof(Settings.ota_url)-1, Settings.ota_url);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: StateText (%d)        [%s], [%s], [%s], [%s]"), (uint8_t *)&Settings.state_text - SetAddr, sizeof(Settings.state_text[0])-1, Settings.state_text[0], Settings.state_text[1], Settings.state_text[2], Settings.state_text[3]);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: Syslog Host (%d)      [%s]"), (uint8_t *)&Settings.syslog_host - SetAddr, sizeof(Settings.syslog_host)-1, Settings.syslog_host);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: NTP Servers (%d)      [%s], [%s], [%s]"), (uint8_t *)&Settings.ntp_server - SetAddr, sizeof(Settings.ntp_server[0])-1, Settings.ntp_server[0], Settings.ntp_server[1], Settings.ntp_server[2]);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: MQTT Host (%d)        [%s]"), (uint8_t *)&Settings.mqtt_host - SetAddr, sizeof(Settings.mqtt_host)-1, Settings.mqtt_host);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: MQTT Client (%d)      [%s]"), (uint8_t *)&Settings.mqtt_client - SetAddr, sizeof(Settings.mqtt_client)-1, Settings.mqtt_client);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: MQTT User (%d)        [%s]"), (uint8_t *)&Settings.mqtt_user - SetAddr, sizeof(Settings.mqtt_user)-1, Settings.mqtt_user);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: MQTT FullTopic (%d)   [%s]"), (uint8_t *)&Settings.mqtt_fulltopic - SetAddr, sizeof(Settings.mqtt_fulltopic)-1, Settings.mqtt_fulltopic);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: MQTT Topic (%d)       [%s]"), (uint8_t *)&Settings.mqtt_topic - SetAddr, sizeof(Settings.mqtt_topic)-1, Settings.mqtt_topic);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: MQTT GroupTopic (%d)  [%s]"), (uint8_t *)&Settings.mqtt_grptopic - SetAddr, sizeof(Settings.mqtt_grptopic)-1, Settings.mqtt_grptopic);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: MQTT ButtonTopic (%d) [%s]"), (uint8_t *)&Settings.button_topic - SetAddr, sizeof(Settings.button_topic)-1, Settings.button_topic);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: MQTT SwitchTopic (%d) [%s]"), (uint8_t *)&Settings.switch_topic - SetAddr, sizeof(Settings.switch_topic)-1, Settings.switch_topic);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: MQTT Prefixes (%d)    [%s], [%s], [%s]"), (uint8_t *)&Settings.mqtt_prefix - SetAddr, sizeof(Settings.mqtt_prefix[0])-1, Settings.mqtt_prefix[0], Settings.mqtt_prefix[1], Settings.mqtt_prefix[2]);
  if (17 == more) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: AP Passwords (%d)     [%s], [%s]"), (uint8_t *)&Settings.sta_pwd - SetAddr, sizeof(Settings.sta_pwd[0])-1, Settings.sta_pwd[0], Settings.sta_pwd[1]);
    AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: MQTT Password (%d)    [%s]"), (uint8_t *)&Settings.mqtt_pwd - SetAddr, sizeof(Settings.mqtt_pwd)-1, Settings.mqtt_pwd);
    AddLog_P2(LOG_LEVEL_INFO, PSTR("%03X: Web Password (%d)     [%s]"), (uint8_t *)&Settings.web_password - SetAddr, sizeof(Settings.web_password)-1, Settings.web_password);
  }
}
#endif  // USE_DEBUG_SETTING_NAMES

void SetFlashMode(uint8_t mode)
{
  uint8_t *_buffer;
  uint32_t address;

  address = 0;
  _buffer = new uint8_t[FLASH_SECTOR_SIZE];

  if (ESP.flashRead(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE)) {
    if (_buffer[2] != mode) {  // DOUT
      _buffer[2] = mode;
      if (ESP.flashEraseSector(address / FLASH_SECTOR_SIZE)) ESP.flashWrite(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE);
    }
  }
  delete[] _buffer;
}

/*******************************************************************************************/

bool DebugCommand(void)
{
  char command[CMDSZ];
  bool serviced = true;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kDebugCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if (CMND_HELP == command_code) {
    AddLog_P(LOG_LEVEL_INFO, kDebugCommands);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
  }
  else if (CMND_RTCDUMP == command_code) {
    DebugRtcDump(XdrvMailbox.data);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
  }
  else if (CMND_CFGDUMP == command_code) {
    DebugCfgDump(XdrvMailbox.data);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
  }
  else if (CMND_CFGPEEK == command_code) {
    DebugCfgPeek(XdrvMailbox.data);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
  }
  else if (CMND_CFGPOKE == command_code) {
    DebugCfgPoke(XdrvMailbox.data);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
  }
#ifdef USE_DEBUG_SETTING_NAMES
  else if (CMND_CFGSHOW == command_code) {
    DebugCfgShow(XdrvMailbox.payload);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
  }
#endif  // USE_DEBUG_SETTING_NAMES
#ifdef USE_WEBSERVER
  else if (CMND_CFGXOR == command_code) {
    if (XdrvMailbox.data_len > 0) {
      config_xor_on_set = XdrvMailbox.payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, config_xor_on_set);
  }
#endif  // USE_WEBSERVER
#ifdef DEBUG_THEO
  else if (CMND_EXCEPTION == command_code) {
    if (XdrvMailbox.data_len > 0) ExceptionTest(XdrvMailbox.payload);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
  }
#endif  // DEBUG_THEO
  else if (CMND_CPUCHECK == command_code) {
    if (XdrvMailbox.data_len > 0) {
      CPU_load_check = XdrvMailbox.payload;
      CPU_last_millis = CPU_last_loop_time;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, CPU_load_check);
  }
  else if (CMND_FREEMEM == command_code) {
    if (XdrvMailbox.data_len > 0) {
      CPU_show_freemem = XdrvMailbox.payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, CPU_show_freemem);
  }
  else if ((CMND_SETSENSOR == command_code) && (XdrvMailbox.index < MAX_XSNS_DRIVERS)) {
    if ((XdrvMailbox.payload >= 0) && XsnsPresent(XdrvMailbox.index)) {
      bitWrite(Settings.sensors[XdrvMailbox.index / 32], XdrvMailbox.index % 32, XdrvMailbox.payload &1);
      if (1 == XdrvMailbox.payload) { restart_flag = 2; }  // To safely re-enable a sensor currently most sensor need to follow complete restart init cycle
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_XVALUE, command, XsnsGetSensors().c_str());
  }
  else if (CMND_FLASHMODE == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
      SetFlashMode(XdrvMailbox.payload);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, ESP.getFlashChipMode());
  }
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv99(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      CPU_last_millis = millis();
      break;
    case FUNC_LOOP:
      CpuLoadLoop();
      break;
    case FUNC_COMMAND:
      result = DebugCommand();
      break;
    case FUNC_FREE_MEM:
      if (CPU_show_freemem) { DebugFreeMem(); }
      break;
  }
  return result;
}

#endif  // USE_DEBUG_DRIVER