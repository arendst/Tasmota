/*
  xdrv_99_debug.ino - debug support for Tasmota

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

//#define USE_DEBUG_DRIVER

#ifdef DEBUG_THEO
#ifndef USE_DEBUG_DRIVER
#define USE_DEBUG_DRIVER
#endif  // USE_DEBUG_DRIVER
#endif  // DEBUG_THEO

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
#define D_CMND_CFGPEEK   "CfgPeek"
#define D_CMND_CFGPOKE   "CfgPoke"
#define D_CMND_CFGXOR    "CfgXor"
#define D_CMND_CPUCHECK  "CpuChk"
#define D_CMND_EXCEPTION "Exception"
#define D_CMND_FLASHDUMP "FlashDump"
#define D_CMND_FLASHMODE "FlashMode"
#define D_CMND_FREEMEM   "FreeMem"
#define D_CMND_HELP      "Help"
#define D_CMND_RTCDUMP   "RtcDump"
#define D_CMND_SETSENSOR "SetSensor"
#define D_CMND_I2CWRITE  "I2CWrite"
#define D_CMND_I2CREAD   "I2CRead"
#define D_CMND_I2CSTRETCH "I2CStretch"
#define D_CMND_I2CCLOCK  "I2CClock"
#define D_CMND_SERBUFF   "SerBufSize"

const char kDebugCommands[] PROGMEM = "|"  // No prefix
  D_CMND_CFGDUMP "|" D_CMND_CFGPEEK "|" D_CMND_CFGPOKE "|"
#ifdef USE_WEBSERVER
  D_CMND_CFGXOR "|"
#endif
  D_CMND_CPUCHECK "|" D_CMND_SERBUFF "|"
#ifdef DEBUG_THEO
  D_CMND_EXCEPTION "|"
#endif
  D_CMND_FLASHDUMP "|" D_CMND_FLASHMODE "|" D_CMND_FREEMEM"|" D_CMND_HELP "|" D_CMND_RTCDUMP "|" D_CMND_SETSENSOR "|"
#ifdef USE_I2C
  D_CMND_I2CWRITE "|" D_CMND_I2CREAD "|" D_CMND_I2CSTRETCH "|" D_CMND_I2CCLOCK
#endif
  ;

void (* const DebugCommand[])(void) PROGMEM = {
  &CmndCfgDump, &CmndCfgPeek, &CmndCfgPoke,
#ifdef USE_WEBSERVER
  &CmndCfgXor,
#endif
  &CmndCpuCheck, &CmndSerBufSize,
#ifdef DEBUG_THEO
  &CmndException,
#endif
  &CmndFlashDump, &CmndFlashMode, &CmndFreemem, &CmndHelp, &CmndRtcDump, &CmndSetSensor,
#ifdef USE_I2C
  &CmndI2cWrite, &CmndI2cRead, &CmndI2cStretch, &CmndI2cClock
#endif
  };

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

00:00:08 MQTT: tele/tasmota/INFO3 = {"Started":"Fatal exception:28 flag:2 (EXCEPTION) epc1:0x4000bf64 epc2:0x00000000 epc3:0x00000000 excvaddr:0x00000007 depc:0x00000000"}
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
      int CPU_load = 100 - ( (CPU_loops*(1 + 30*TasmotaGlobal.sleep)) / (CPU_load_check *800) );
      CPU_loops = CPU_loops / CPU_load_check;
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, CPU %d%%(160MHz), Loops/sec %d"), ESP.getFreeHeap(), CPU_load, CPU_loops);
#else
      int CPU_load = 100 - ( (CPU_loops*(1 + 30*TasmotaGlobal.sleep)) / (CPU_load_check *400) );
      CPU_loops = CPU_loops / CPU_load_check;
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, CPU %d%%(80MHz), Loops/sec %d"), ESP.getFreeHeap(), CPU_load, CPU_loops);
#endif
      CPU_last_millis = CPU_last_loop_time;
      CPU_loops = 0;
    }
  }
}

/*******************************************************************************************/

#ifdef ESP8266
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

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d (%s)"), ESP.getFreeHeap(), 4 * (sp - g_pcont->stack), XdrvMailbox.data);
}

#endif  // ESP8266
#ifdef ESP32

void DebugFreeMem(void)
{
  register uint8_t *sp asm("a1");

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d (%s)"), ESP.getFreeHeap(), sp - pxTaskGetStackStart(NULL), XdrvMailbox.data);
}

#endif  // ESP8266 - ESP32

/*******************************************************************************************/

void DebugRtcDump(char* parms)
{
#ifdef ESP8266
  uint32_t CFG_COLS = 16;

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

//  AddLog(LOG_LEVEL_DEBUG, PSTR("Cnfg: Parms %s, Start row %d, rows %d"), parms, srow, mrow);

  if (0 == mrow) {  // Default only 8 lines
    mrow = 8;
  }
  if (srow > maxrow) {
    srow = maxrow - mrow;
  }
  if (mrow < (maxrow - srow)) {
    maxrow = srow + mrow;
  }

  char log_data[100];  // 020:  C7 2B 2E AB  70 E8 09 AE  C8 88 3D EA  7C FF 48 2F | +. p     = | H/|
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
    AddLogData(LOG_LEVEL_INFO, log_data);
  }
#endif  // ESP8266
}

/*******************************************************************************************/

void DebugDump(uint32_t start, uint32_t size) {
  uint32_t CFG_COLS = 32;

  uint32_t idx;
  uint32_t maxrow;
  uint32_t row;
  uint32_t col;
  char *p;

  uint8_t *buffer = (uint8_t *)&start;
  maxrow = ((start + size + CFG_COLS) / CFG_COLS);

  uint32_t srow = 0;
  uint32_t mrow = maxrow;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("Cnfg: Parms %s, Start row %d, rows %d"), parms, srow, mrow);

  if (0 == mrow) {  // Default only 8 lines
    mrow = 8;
  }
  if (srow > maxrow) {
    srow = maxrow - mrow;
  }
  if (mrow < (maxrow - srow)) {
    maxrow = srow + mrow;
  }

  char log_data[150];  // 020:  C7 2B 2E AB  70 E8 09 AE  C8 88 3D EA  7C FF 48 2F  0E A7 D7 BF  02 0E D7 7D  C9 6F B9 3A  1D 01 3F 28 | +. p     = | H/       } o :  ?(|
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
    AddLogData(LOG_LEVEL_INFO, log_data);
    delay(1);
  }
}

void DebugCfgDump(char* parms)
{
  uint32_t CFG_COLS = 16;

  uint16_t idx;
  uint16_t maxrow;
  uint16_t row;
  uint16_t col;
  char *p;

  uint8_t *buffer = (uint8_t *) &Settings;
  maxrow = ((sizeof(Settings)+CFG_COLS)/CFG_COLS);

  uint16_t srow = strtol(parms, &p, 16) / CFG_COLS;
  uint16_t mrow = strtol(p, &p, 10);

//  AddLog(LOG_LEVEL_DEBUG, PSTR("Cnfg: Parms %s, Start row %d, rows %d"), parms, srow, mrow);

  if (0 == mrow) {  // Default only 8 lines
    mrow = 8;
  }
  if (srow > maxrow) {
    srow = maxrow - mrow;
  }
  if (mrow < (maxrow - srow)) {
    maxrow = srow + mrow;
  }

  char log_data[100];  // 020:  6D 75 73 31  3A 38 30 2F  61 70 69 2F  61 72 64 75 |mus1:80/api/ardu|
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
    AddLogData(LOG_LEVEL_INFO, log_data);
    delay(1);
  }
}

void DebugCfgPeek(char* parms)
{
  char *p;

  uint16_t address = strtol(parms, &p, 16);
  if (address > sizeof(Settings)) address = sizeof(Settings) -4;
  address = (address >> 2) << 2;

  uint8_t *buffer = (uint8_t *) &Settings;
  uint8_t data8 = buffer[address];
  uint16_t data16 = (buffer[address +1] << 8) + buffer[address];
  uint32_t data32 = (buffer[address +3] << 24) + (buffer[address +2] << 16) + data16;

  char log_data[100];  // 000: 09 12 00 10 |    | 0x09 (9), 0x1209 (4617), 0x10001209 (268440073)
  snprintf_P(log_data, sizeof(log_data), PSTR("%03X:"), address);
  for (uint32_t i = 0; i < 4; i++) {
    snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, buffer[address +i]);
  }
  snprintf_P(log_data, sizeof(log_data), PSTR("%s |"), log_data);
  for (uint32_t i = 0; i < 4; i++) {
    snprintf_P(log_data, sizeof(log_data), PSTR("%s%c"), log_data, ((buffer[address +i] > 0x20) && (buffer[address +i] < 0x7F)) ? (char)buffer[address +i] : ' ');
  }
  snprintf_P(log_data, sizeof(log_data), PSTR("%s| 0x%02X (%d), 0x%04X (%d), 0x%0LX (%lu)"), log_data, data8, data8, data16, data16, data32, data32);
  AddLogData(LOG_LEVEL_INFO, log_data);
}

void DebugCfgPoke(char* parms)
{
  char *p;

  uint16_t address = strtol(parms, &p, 16);
  if (address > sizeof(Settings)) address = sizeof(Settings) -4;
  address = (address >> 2) << 2;

  uint32_t data = strtol(p, &p, 16);

  uint8_t *buffer = (uint8_t *) &Settings;
  uint32_t data32 = (buffer[address +3] << 24) + (buffer[address +2] << 16) + (buffer[address +1] << 8) + buffer[address];

  uint8_t *nbuffer = (uint8_t *) &data;
  for (uint32_t i = 0; i < 4; i++) { buffer[address +i] = nbuffer[+i]; }

  uint32_t ndata32 = (buffer[address +3] << 24) + (buffer[address +2] << 16) + (buffer[address +1] << 8) + buffer[address];

  AddLog(LOG_LEVEL_INFO, PSTR("%03X: 0x%0LX (%lu) poked to 0x%0LX (%lu)"), address, data32, data32, ndata32, ndata32);
}

void SetFlashMode(uint8_t mode)
{
#ifdef ESP8266
  uint8_t *_buffer;
  uint32_t address;

  address = 0;
  _buffer = new uint8_t[FLASH_SECTOR_SIZE];

  if (ESP.flashRead(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE)) {
    if (_buffer[2] != mode) {  // DOUT
      _buffer[2] = mode;
      if (ESP.flashEraseSector(address / FLASH_SECTOR_SIZE)) {
        ESP.flashWrite(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE);
      }
    }
  }
  delete[] _buffer;
#endif  // ESP8266
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndHelp(void)
{
  AddLog_P(LOG_LEVEL_INFO, PSTR("HLP: %s"), kDebugCommands);
  ResponseCmndDone();
}

void CmndRtcDump(void)
{
  DebugRtcDump(XdrvMailbox.data);
  ResponseCmndDone();
}

void CmndCfgDump(void)
{
  DebugCfgDump(XdrvMailbox.data);
  ResponseCmndDone();
}

void CmndCfgPeek(void)
{
  DebugCfgPeek(XdrvMailbox.data);
  ResponseCmndDone();
}

void CmndCfgPoke(void)
{
  DebugCfgPoke(XdrvMailbox.data);
  ResponseCmndDone();
}

#ifdef USE_WEBSERVER
void CmndCfgXor(void)
{
  if (XdrvMailbox.data_len > 0) {
    Web.config_xor_on_set = XdrvMailbox.payload;
  }
  char temp[10];
  snprintf_P(temp, sizeof(temp), PSTR("0x%02X"), Web.config_xor_on_set);
  ResponseCmndChar(temp);
}
#endif  // USE_WEBSERVER

#ifdef DEBUG_THEO
void CmndException(void)
{
  if (XdrvMailbox.data_len > 0) { ExceptionTest(XdrvMailbox.payload); }
  ResponseCmndDone();
}
#endif  // DEBUG_THEO

void CmndCpuCheck(void)
{
  if (XdrvMailbox.data_len > 0) {
    CPU_load_check = XdrvMailbox.payload;
    CPU_last_millis = CPU_last_loop_time;
  }
  ResponseCmndNumber(CPU_load_check);
}

void CmndSerBufSize(void)
{
  if (XdrvMailbox.data_len > 0) {
    Serial.setRxBufferSize(XdrvMailbox.payload);
  }
#ifdef ESP8266
  ResponseCmndNumber(Serial.getRxBufferSize());
#endif  // ESP8266
#ifdef ESP32
  ResponseCmndDone();
#endif  // ESP32
}

void CmndFreemem(void)
{
  if (XdrvMailbox.data_len > 0) {
    CPU_show_freemem = XdrvMailbox.payload;
  }
  ResponseCmndNumber(CPU_show_freemem);
}

void CmndSetSensor(void)
{
  if (XdrvMailbox.index < MAX_XSNS_DRIVERS) {
    if (XdrvMailbox.payload >= 0) {
      bitWrite(Settings.sensors[XdrvMailbox.index / 32], XdrvMailbox.index % 32, XdrvMailbox.payload &1);
      if (1 == XdrvMailbox.payload) {
        TasmotaGlobal.restart_flag = 2;  // To safely re-enable a sensor currently most sensor need to follow complete restart init cycle
      }
    }
    Response_P(PSTR("{\"" D_CMND_SETSENSOR "\":"));
    XsnsSensorState();
    ResponseJsonEnd();
  }
}

void CmndFlashMode(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
    SetFlashMode(XdrvMailbox.payload);
  }
  ResponseCmndNumber(ESP.getFlashChipMode());
}

uint32_t DebugSwap32(uint32_t x) {
	return	((x << 24) & 0xff000000 ) |
		((x <<  8) & 0x00ff0000 ) |
		((x >>  8) & 0x0000ff00 ) |
		((x >> 24) & 0x000000ff );
}

void CmndFlashDump(void)
{
#ifdef ESP8266
  // FlashDump
  // FlashDump 0xFF000
  // FlashDump 0xFC000 10
  const uint32_t flash_start = 0x40200000;  // Start address flash
  const uint8_t bytes_per_cols = 0x20;
  const uint32_t max = (EEPROM_LOCATION + 5) * SPI_FLASH_SEC_SIZE;  // 0x100000 for 1M flash, 0x400000 for 4M flash

  uint32_t start = flash_start;
  uint32_t rows = 8;

  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= (max - bytes_per_cols))) {
    start += (XdrvMailbox.payload &0x7FFFFFFC);  // Fix exception as flash access is only allowed on 4 byte boundary

    char *p;
    uint32_t is_payload = strtol(XdrvMailbox.data, &p, 16);
    rows = strtol(p, &p, 10);
    if (0 == rows) { rows = 8; }
  }
  uint32_t end = start + (rows * bytes_per_cols);
  if ((end - flash_start) > max) {
    end = flash_start + max;
  }

  for (uint32_t pos = start; pos < end; pos += bytes_per_cols) {
    uint32_t* values = (uint32_t*)(pos);
    AddLog(LOG_LEVEL_INFO, PSTR("%06X:  %08X %08X %08X %08X  %08X %08X %08X %08X"), pos - flash_start,
      DebugSwap32(values[0]), DebugSwap32(values[1]), DebugSwap32(values[2]), DebugSwap32(values[3]),
      DebugSwap32(values[4]), DebugSwap32(values[5]), DebugSwap32(values[6]), DebugSwap32(values[7]));
  }
  ResponseCmndDone();
#endif  // ESP8266
}

#ifdef USE_I2C
void CmndI2cWrite(void)
{
  // I2cWrite <address>,<data>..
  if (TasmotaGlobal.i2c_enabled) {
    char* parms = XdrvMailbox.data;
    uint8_t buffer[100];
    uint32_t index = 0;

    char *p;
    char *data = strtok_r(parms, " ,", &p);
    while (data != NULL && index < sizeof(buffer)) {
      buffer[index++] = strtol(data, nullptr, 16);
      data = strtok_r(nullptr, " ,", &p);
    }

    if (index > 1) {
      AddLogBuffer(LOG_LEVEL_INFO, buffer, index);

      Wire.beginTransmission(buffer[0]);
      for (uint32_t i = 1; i < index; i++) {
        Wire.write(buffer[i]);
      }
      int result = Wire.endTransmission();
      AddLog(LOG_LEVEL_INFO, PSTR("I2C: Result %d"), result);
    }
  }
  ResponseCmndDone();
}

void CmndI2cRead(void)
{
  // I2cRead <address>,<size>
  if (TasmotaGlobal.i2c_enabled) {
    char* parms = XdrvMailbox.data;
    uint8_t buffer[100];
    uint32_t index = 0;

    char *p;
    char *data = strtok_r(parms, " ,", &p);
    while (data != NULL && index < sizeof(buffer)) {
      buffer[index++] = strtol(data, nullptr, 16);
      data = strtok_r(nullptr, " ,", &p);
    }

    if (index > 0) {
      uint8_t size = 1;
      if (index > 1) {
        size = buffer[1];
      }
      Wire.requestFrom(buffer[0], size);
      index = 0;
      while (Wire.available() && index < sizeof(buffer)) {
        buffer[index++] = Wire.read();
      }
      if (index > 0) {
        AddLogBuffer(LOG_LEVEL_INFO, buffer, index);
      }
    }
  }
  ResponseCmndDone();
}

void CmndI2cStretch(void)
{
#ifdef ESP8266
  if (TasmotaGlobal.i2c_enabled && (XdrvMailbox.payload > 0)) {
    Wire.setClockStretchLimit(XdrvMailbox.payload);
  }
  ResponseCmndDone();
#endif  // ESP8266
}

void CmndI2cClock(void)
{
  if (TasmotaGlobal.i2c_enabled && (XdrvMailbox.payload > 0)) {
    Wire.setClock(XdrvMailbox.payload);
  }
  ResponseCmndDone();
}
#endif // USE_I2C

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv99(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      CpuLoadLoop();
      break;
    case FUNC_FREE_MEM:
      if (CPU_show_freemem) { DebugFreeMem(); }
      break;
    case FUNC_PRE_INIT:
      CPU_last_millis = millis();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kDebugCommands, DebugCommand);
      break;
  }
  return result;
}

#endif  // USE_DEBUG_DRIVER