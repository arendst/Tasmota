/*
  support.ino - support for Tasmota

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

IPAddress syslog_host_addr;      // Syslog host IP address
uint32_t syslog_host_hash = 0;   // Syslog host name hash

extern "C" {
extern struct rst_info resetInfo;
}

/*********************************************************************************************\
 * Watchdog extension (https://github.com/esp8266/Arduino/issues/1532)
\*********************************************************************************************/

#include <Ticker.h>

Ticker tickerOSWatch;

const uint32_t OSWATCH_RESET_TIME = 120;

static unsigned long oswatch_last_loop_time;
uint8_t oswatch_blocked_loop = 0;

#ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
//void OsWatchTicker() ICACHE_RAM_ATTR;
#endif  // USE_WS2812_DMA

#ifdef USE_KNX
bool knx_started = false;
#endif  // USE_KNX

void OsWatchTicker(void)
{
  uint32_t t = millis();
  uint32_t last_run = abs(t - oswatch_last_loop_time);

#ifdef DEBUG_THEO
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_OSWATCH " FreeRam %d, rssi %d %% (%d dBm), last_run %d"), ESP.getFreeHeap(), WifiGetRssiAsQuality(WiFi.RSSI()), WiFi.RSSI(), last_run);
#endif  // DEBUG_THEO
  if (last_run >= (OSWATCH_RESET_TIME * 1000)) {
//    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_OSWATCH " " D_BLOCKED_LOOP ". " D_RESTARTING));  // Save iram space
    RtcSettings.oswatch_blocked_loop = 1;
    RtcSettingsSave();

//    ESP.restart();  // normal reboot
//    ESP.reset();  // hard reset

    // Force an exception to get a stackdump
    volatile uint32_t dummy;
    dummy = *((uint32_t*) 0x00000000);
  }
}

void OsWatchInit(void)
{
  oswatch_blocked_loop = RtcSettings.oswatch_blocked_loop;
  RtcSettings.oswatch_blocked_loop = 0;
  oswatch_last_loop_time = millis();
  tickerOSWatch.attach_ms(((OSWATCH_RESET_TIME / 3) * 1000), OsWatchTicker);
}

void OsWatchLoop(void)
{
  oswatch_last_loop_time = millis();
//  while(1) delay(1000);  // this will trigger the os watch
}

bool OsWatchBlockedLoop(void)
{
  return oswatch_blocked_loop;
}

uint32_t ResetReason(void)
{
  /*
    user_interface.h
    REASON_DEFAULT_RST      = 0,  // "Power on"                normal startup by power on
    REASON_WDT_RST          = 1,  // "Hardware Watchdog"       hardware watch dog reset
    REASON_EXCEPTION_RST    = 2,  // "Exception"               exception reset, GPIO status won’t change
    REASON_SOFT_WDT_RST     = 3,  // "Software Watchdog"       software watch dog reset, GPIO status won’t change
    REASON_SOFT_RESTART     = 4,  // "Software/System restart" software restart ,system_restart , GPIO status won’t change
    REASON_DEEP_SLEEP_AWAKE = 5,  // "Deep-Sleep Wake"         wake up from deep-sleep
    REASON_EXT_SYS_RST      = 6   // "External System"         external system reset
  */
  return resetInfo.reason;
}

String GetResetReason(void)
{
  if (oswatch_blocked_loop) {
    char buff[32];
    strncpy_P(buff, PSTR(D_JSON_BLOCKED_LOOP), sizeof(buff));
    return String(buff);
  } else {
    return ESP.getResetReason();
  }
}

/*********************************************************************************************\
 * Miscellaneous
\*********************************************************************************************/

// Get span until single character in string
size_t strchrspn(const char *str1, int character)
{
  size_t ret = 0;
  char *start = (char*)str1;
  char *end = strchr(str1, character);
  if (end) ret = end - start;
  return ret;
}

// Function to return a substring defined by a delimiter at an index
char* subStr(char* dest, char* str, const char *delim, int index)
{
  char *act;
  char *sub = nullptr;
  char *ptr;
  int i;

  // Since strtok consumes the first arg, make a copy
  strncpy(dest, str, strlen(str)+1);
  for (i = 1, act = dest; i <= index; i++, act = nullptr) {
    sub = strtok_r(act, delim, &ptr);
    if (sub == nullptr) break;
  }
  sub = Trim(sub);
  return sub;
}

float CharToFloat(const char *str)
{
  // simple ascii to double, because atof or strtod are too large
  char strbuf[24];

  strlcpy(strbuf, str, sizeof(strbuf));
  char *pt = strbuf;
  while ((*pt != '\0') && isblank(*pt)) { pt++; }  // Trim leading spaces

  signed char sign = 1;
  if (*pt == '-') { sign = -1; }
  if (*pt == '-' || *pt=='+') { pt++; }            // Skip any sign

  float left = 0;
  if (*pt != '.') {
    left = atoi(pt);                               // Get left part
    while (isdigit(*pt)) { pt++; }                 // Skip number
  }

  float right = 0;
  if (*pt == '.') {
    pt++;
    right = atoi(pt);                              // Decimal part
    while (isdigit(*pt)) {
      pt++;
      right /= 10.0f;
    }
  }

  float result = left + right;
  if (sign < 0) {
    return -result;                                // Add negative sign
  }
  return result;
}

int TextToInt(char *str)
{
  char *p;
  uint8_t radix = 10;
  if ('#' == str[0]) {
    radix = 16;
    str++;
  }
  return strtol(str, &p, radix);
}

char* ulltoa(unsigned long long value, char *str, int radix)
{
  char digits[64];
  char *dst = str;
  int i = 0;

//  if (radix < 2 || radix > 36) { radix = 10; }

  do {
    int n = value % radix;
    digits[i++] = (n < 10) ? (char)n+'0' : (char)n-10+'A';
    value /= radix;
  } while (value != 0);

  while (i > 0) { *dst++ = digits[--i]; }

  *dst = 0;
  return str;
}

// see https://stackoverflow.com/questions/6357031/how-do-you-convert-a-byte-array-to-a-hexadecimal-string-in-c
// char* ToHex_P(unsigned char * in, size_t insz, char * out, size_t outsz, char inbetween = '\0'); in tasmota_post.h
char* ToHex_P(const unsigned char * in, size_t insz, char * out, size_t outsz, char inbetween)
{
  // ToHex_P(in, insz, out, outz)      -> "12345667"
  // ToHex_P(in, insz, out, outz, ' ') -> "12 34 56 67"
  // ToHex_P(in, insz, out, outz, ':') -> "12:34:56:67"
  static const char * hex = "0123456789ABCDEF";
  int between = (inbetween) ? 3 : 2;
  const unsigned char * pin = in;
  char * pout = out;
  for (; pin < in+insz; pout += between, pin++) {
    pout[0] = hex[(pgm_read_byte(pin)>>4) & 0xF];
    pout[1] = hex[ pgm_read_byte(pin)     & 0xF];
    if (inbetween) { pout[2] = inbetween; }
    if (pout + 3 - out > outsz) { break; }  // Better to truncate output string than overflow buffer
  }
  pout[(inbetween && insz) ? -1 : 0] = 0;   // Discard last inbetween if any input
  return out;
}

char* Uint64toHex(uint64_t value, char *str, uint16_t bits)
{
  ulltoa(value, str, 16);  // Get 64bit value

  int fill = 8;
  if ((bits > 3) && (bits < 65)) {
    fill = bits / 4;  // Max 16
    if (bits % 4) { fill++; }
  }
  int len = strlen(str);
  fill -= len;
  if (fill > 0) {
    memmove(str + fill, str, len +1);
    memset(str, '0', fill);
  }
  return str;
}

char* dtostrfd(double number, unsigned char prec, char *s)
{
  if ((isnan(number)) || (isinf(number))) {  // Fix for JSON output (https://stackoverflow.com/questions/1423081/json-left-out-infinity-and-nan-json-status-in-ecmascript)
    strcpy(s, "null");
    return s;
  } else {
    return dtostrf(number, 1, prec, s);
  }
}

char* Unescape(char* buffer, uint32_t* size)
{
  uint8_t* read = (uint8_t*)buffer;
  uint8_t* write = (uint8_t*)buffer;
  int32_t start_size = *size;
  int32_t end_size = *size;
  uint8_t che = 0;

//  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t*)buffer, *size);

  while (start_size > 0) {
    uint8_t ch = *read++;
    start_size--;
    if (ch != '\\') {
      *write++ = ch;
    } else {
      if (start_size > 0) {
        uint8_t chi = *read++;
        start_size--;
        end_size--;
        switch (chi) {
          case '\\': che = '\\'; break;  // 5C Backslash
          case 'a': che = '\a'; break;   // 07 Bell (Alert)
          case 'b': che = '\b'; break;   // 08 Backspace
          case 'e': che = '\e'; break;   // 1B Escape
          case 'f': che = '\f'; break;   // 0C Formfeed
          case 'n': che = '\n'; break;   // 0A Linefeed (Newline)
          case 'r': che = '\r'; break;   // 0D Carriage return
          case 's': che = ' ';  break;   // 20 Space
          case 't': che = '\t'; break;   // 09 Horizontal tab
          case 'v': che = '\v'; break;   // 0B Vertical tab
          case 'x': {
            uint8_t* start = read;
            che = (uint8_t)strtol((const char*)read, (char**)&read, 16);
            start_size -= (uint16_t)(read - start);
            end_size -= (uint16_t)(read - start);
            break;
          }
          case '"': che = '\"'; break;   // 22 Quotation mark
//          case '?': che = '\?'; break;   // 3F Question mark
          default : {
            che = chi;
            *write++ = ch;
            end_size++;
          }
        }
        *write++ = che;
      }
    }
  }
  *size = end_size;
  *write++ = 0;   // add the end string pointer reference
//  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t*)buffer, *size);

  return buffer;
}

char* RemoveSpace(char* p)
{
  char* write = p;
  char* read = p;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    if (!isspace(ch)) {
      *write++ = ch;
    }
  }
//  *write = '\0';  // Removed 20190223 as it buffer overflows on no isspace found - no need either
  return p;
}

char* ReplaceCommaWithDot(char* p)
{
  char* write = (char*)p;
  char* read = (char*)p;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    if (ch == ',') {
      ch = '.';
    }
    *write++ = ch;
  }
  return p;
}

char* LowerCase(char* dest, const char* source)
{
  char* write = dest;
  const char* read = source;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    *write++ = tolower(ch);
  }
  return dest;
}

char* UpperCase(char* dest, const char* source)
{
  char* write = dest;
  const char* read = source;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    *write++ = toupper(ch);
  }
  return dest;
}

char* UpperCase_P(char* dest, const char* source)
{
  char* write = dest;
  const char* read = source;
  char ch = '.';

  while (ch != '\0') {
    ch = pgm_read_byte(read++);
    *write++ = toupper(ch);
  }
  return dest;
}

char* Trim(char* p)
{
  while ((*p != '\0') && isblank(*p)) { p++; }  // Trim leading spaces
  char* q = p + strlen(p) -1;
  while ((q >= p) && isblank(*q)) { q--; }   // Trim trailing spaces
  q++;
  *q = '\0';
  return p;
}

char* RemoveAllSpaces(char* p)
{
  // remove any white space from the base64
  char *cursor = p;
  uint32_t offset = 0;
  while (1) {
    *cursor = *(cursor + offset);
    if ((' ' == *cursor) || ('\t' == *cursor) || ('\n' == *cursor)) {   // if space found, remove this char until end of string
      offset++;
    } else {
      if (0 == *cursor) { break; }
      cursor++;
    }
  }
  return p;
}

char* NoAlNumToUnderscore(char* dest, const char* source)
{
  char* write = dest;
  const char* read = source;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    *write++ = (isalnum(ch) || ('\0' == ch)) ? ch : '_';
  }
  return dest;
}

char IndexSeparator(void)
{
/*
  // 20 bytes more costly !?!
  const char separators[] = { "-_" };

  return separators[Settings.flag3.use_underscore];
*/
  if (Settings.flag3.use_underscore) {  // SetOption64 - Enable "_" instead of "-" as sensor index separator
    return '_';
  } else {
    return '-';
  }
}

void SetShortcutDefault(void)
{
  if ('\0' != XdrvMailbox.data[0]) {     // There must be at least one character in the buffer
    XdrvMailbox.data[0] = '0' + SC_DEFAULT;  // SC_CLEAR, SC_DEFAULT, SC_USER
    XdrvMailbox.data[1] = '\0';
  }
}

uint8_t Shortcut(void)
{
  uint8_t result = 10;

  if ('\0' == XdrvMailbox.data[1]) {    // Only allow single character input for shortcut
    if (('"' == XdrvMailbox.data[0]) || ('0' == XdrvMailbox.data[0])) {
      result = SC_CLEAR;
    } else {
      result = atoi(XdrvMailbox.data);  // 1 = SC_DEFAULT, 2 = SC_USER
      if (0 == result) {
        result = 10;
      }
    }
  }
  return result;
}

bool ValidIpAddress(const char* str)
{
  const char* p = str;

  while (*p && ((*p == '.') || ((*p >= '0') && (*p <= '9')))) { p++; }
  return (*p == '\0');
}

bool ParseIp(uint32_t* addr, const char* str)
{
  uint8_t *part = (uint8_t*)addr;
  uint8_t i;

  *addr = 0;
  for (i = 0; i < 4; i++) {
    part[i] = strtoul(str, nullptr, 10);        // Convert byte
    str = strchr(str, '.');
    if (str == nullptr || *str == '\0') {
      break;  // No more separators, exit
    }
    str++;                                   // Point to next character after separator
  }
  return (3 == i);
}

// Function to parse & check if version_str is newer than our currently installed version.
bool NewerVersion(char* version_str)
{
  uint32_t version = 0;
  uint32_t i = 0;
  char *str_ptr;

  char version_dup[strlen(version_str) +1];
  strncpy(version_dup, version_str, sizeof(version_dup));  // Duplicate the version_str as strtok_r will modify it.
  // Loop through the version string, splitting on '.' seperators.
  for (char *str = strtok_r(version_dup, ".", &str_ptr); str && i < sizeof(VERSION); str = strtok_r(nullptr, ".", &str_ptr), i++) {
    int field = atoi(str);
    // The fields in a version string can only range from 0-255.
    if ((field < 0) || (field > 255)) {
      return false;
    }
    // Shuffle the accumulated bytes across, and add the new byte.
    version = (version << 8) + field;
    // Check alpha delimiter after 1.2.3 only
    if ((2 == i) && isalpha(str[strlen(str)-1])) {
      field = str[strlen(str)-1] & 0x1f;
      version = (version << 8) + field;
      i++;
    }
  }
  // A version string should have 2-4 fields. e.g. 1.2, 1.2.3, or 1.2.3a (= 1.2.3.1).
  // If not, then don't consider it a valid version string.
  if ((i < 2) || (i > sizeof(VERSION))) {
    return false;
  }
  // Keep shifting the parsed version until we hit the maximum number of tokens.
  // VERSION stores the major number of the version in the most significant byte of the uint32_t.
  while (i < sizeof(VERSION)) {
    version <<= 8;
    i++;
  }
  // Now we should have a fully constructed version number in uint32_t form.
  return (version > VERSION);
}

char* GetPowerDevice(char* dest, uint32_t idx, size_t size, uint32_t option)
{
  strncpy_P(dest, S_RSLT_POWER, size);                // POWER
  if ((devices_present + option) > 1) {
    char sidx[8];
    snprintf_P(sidx, sizeof(sidx), PSTR("%d"), idx);  // x
    strncat(dest, sidx, size - strlen(dest) -1);      // POWERx
  }
  return dest;
}

char* GetPowerDevice(char* dest, uint32_t idx, size_t size)
{
  return GetPowerDevice(dest, idx, size, 0);
}

void GetEspHardwareType(void)
{
  // esptool.py get_efuses
  uint32_t efuse1 = *(uint32_t*)(0x3FF00050);
  uint32_t efuse2 = *(uint32_t*)(0x3FF00054);
//  uint32_t efuse3 = *(uint32_t*)(0x3FF00058);
//  uint32_t efuse4 = *(uint32_t*)(0x3FF0005C);

  is_8285 = ( (efuse1 & (1 << 4)) || (efuse2 & (1 << 16)) );
  if (is_8285 && (ESP.getFlashChipRealSize() > 1048576)) {
    is_8285 = false;  // ESP8285 can only have 1M flash
  }
}

String GetDeviceHardware(void)
{
  char buff[10];
  if (is_8285) {
    strcpy_P(buff, PSTR("ESP8285"));
  } else {
    strcpy_P(buff, PSTR("ESP8266EX"));
  }
  return String(buff);
}

float ConvertTemp(float c)
{
  float result = c;

  global_update = uptime;
  global_temperature = c;

  if (!isnan(c) && Settings.flag.temperature_conversion) {    // SetOption8 - Switch between Celsius or Fahrenheit
    result = c * 1.8 + 32;                                    // Fahrenheit
  }
  result = result + (0.1 * Settings.temp_comp);
  return result;
}

float ConvertTempToCelsius(float c)
{
  float result = c;

  if (!isnan(c) && Settings.flag.temperature_conversion) {    // SetOption8 - Switch between Celsius or Fahrenheit
    result = (c - 32) / 1.8;                                  // Celsius
  }
  result = result + (0.1 * Settings.temp_comp);
  return result;
}

char TempUnit(void)
{
  return (Settings.flag.temperature_conversion) ? 'F' : 'C';  // SetOption8  - Switch between Celsius or Fahrenheit
}

float ConvertHumidity(float h)
{
  global_update = uptime;
  global_humidity = h;

  return h;
}

float ConvertPressure(float p)
{
  float result = p;

  global_update = uptime;
  global_pressure = p;

  if (!isnan(p) && Settings.flag.pressure_conversion) {  // SetOption24 - Switch between hPa or mmHg pressure unit
    result = p * 0.75006375541921;                       // mmHg
  }
  return result;
}

String PressureUnit(void)
{
  return (Settings.flag.pressure_conversion) ? String(D_UNIT_MILLIMETER_MERCURY) : String(D_UNIT_PRESSURE);
}

void ResetGlobalValues(void)
{
  if ((uptime - global_update) > GLOBAL_VALUES_VALID) {  // Reset after 5 minutes
    global_update = 0;
    global_temperature = 9999;
    global_humidity = 0;
    global_pressure = 0;
  }
}

uint32_t SqrtInt(uint32_t num)
{
  if (num <= 1) {
    return num;
  }

  uint32_t x = num / 2;
  uint32_t y;
  do {
    y = (x + num / x) / 2;
    if (y >= x) {
      return x;
    }
    x = y;
  } while (true);
}

uint32_t RoundSqrtInt(uint32_t num)
{
  uint32_t s = SqrtInt(4 * num);
  if (s & 1) {
    s++;
  }
  return s / 2;
}

char* GetTextIndexed(char* destination, size_t destination_size, uint32_t index, const char* haystack)
{
  // Returns empty string if not found
  // Returns text of found
  char* write = destination;
  const char* read = haystack;

  index++;
  while (index--) {
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    if (0 == ch) {
      if (index) {
        write = destination;
      }
      break;
    }
  }
  *write = '\0';
  return destination;
}

int GetCommandCode(char* destination, size_t destination_size, const char* needle, const char* haystack)
{
  // Returns -1 of not found
  // Returns index and command if found
  int result = -1;
  const char* read = haystack;
  char* write = destination;

  while (true) {
    result++;
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    *write = '\0';
    if (!strcasecmp(needle, destination)) {
      break;
    }
    if (0 == ch) {
      result = -1;
      break;
    }
  }
  return result;
}

bool DecodeCommand(const char* haystack, void (* const MyCommand[])(void))
{
  GetTextIndexed(XdrvMailbox.command, CMDSZ, 0, haystack);  // Get prefix if available
  int prefix_length = strlen(XdrvMailbox.command);
  if (prefix_length) {
    char prefix[prefix_length +1];
    snprintf_P(prefix, sizeof(prefix), XdrvMailbox.topic);  // Copy prefix part only
    if (strcasecmp(prefix, XdrvMailbox.command)) {
      return false;                                         // Prefix not in command
    }
  }
  int command_code = GetCommandCode(XdrvMailbox.command + prefix_length, CMDSZ, XdrvMailbox.topic + prefix_length, haystack);
  if (command_code > 0) {                                   // Skip prefix
    XdrvMailbox.command_code = command_code -1;
    MyCommand[XdrvMailbox.command_code]();
    return true;
  }
  return false;
}

const char kOptions[] PROGMEM = "OFF|" D_OFF "|FALSE|" D_FALSE "|STOP|" D_STOP "|" D_CELSIUS "|"              // 0
                                "ON|" D_ON "|TRUE|" D_TRUE "|START|" D_START "|" D_FAHRENHEIT "|" D_USER "|"  // 1
                                "TOGGLE|" D_TOGGLE "|" D_ADMIN "|"                                            // 2
                                "BLINK|" D_BLINK "|"                                                          // 3
                                "BLINKOFF|" D_BLINKOFF "|"                                                    // 4
                                "ALL" ;                                                                       // 255

const uint8_t sNumbers[] PROGMEM = { 0,0,0,0,0,0,0,
                                     1,1,1,1,1,1,1,1,
                                     2,2,2,
                                     3,3,
                                     4,4,
                                     255 };

int GetStateNumber(char *state_text)
{
  char command[CMDSZ];
  int state_number = GetCommandCode(command, sizeof(command), state_text, kOptions);
  if (state_number >= 0) {
    state_number = pgm_read_byte(sNumbers + state_number);
  }
  return state_number;
}

String GetSerialConfig(void)
{
  // Settings.serial_config layout
  // b000000xx - 5, 6, 7 or 8 data bits
  // b00000x00 - 1 or 2 stop bits
  // b000xx000 - None, Even or Odd parity

  const char kParity[] = "NEOI";

  char config[4];
  config[0] = '5' + (Settings.serial_config & 0x3);
  config[1] = kParity[(Settings.serial_config >> 3) & 0x3];
  config[2] = '1' + ((Settings.serial_config >> 2) & 0x1);
  config[3] = '\0';
  return String(config);
}

void SetSerialBegin()
{
  uint32_t baudrate = Settings.baudrate * 300;
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_SERIAL "Set to %s %d bit/s"), GetSerialConfig().c_str(), baudrate);
  Serial.flush();
  Serial.begin(baudrate, (SerialConfig)pgm_read_byte(kTasmotaSerialConfig + Settings.serial_config));
}

void SetSerialConfig(uint32_t serial_config)
{
  if (serial_config > TS_SERIAL_8O2) {
    serial_config = TS_SERIAL_8N1;
  }
  if (serial_config != Settings.serial_config) {
    Settings.serial_config = serial_config;
    SetSerialBegin();
  }
}

void SetSerialBaudrate(uint32_t baudrate)
{
  Settings.baudrate = baudrate / 300;
  if (Serial.baudRate() != baudrate) {
    SetSerialBegin();
  }
}

void SetSerial(uint32_t baudrate, uint32_t serial_config)
{
  Settings.flag.mqtt_serial = 0;  // CMND_SERIALSEND and CMND_SERIALLOG
  Settings.serial_config = serial_config;
  Settings.baudrate = baudrate / 300;
  SetSeriallog(LOG_LEVEL_NONE);
  SetSerialBegin();
}

void ClaimSerial(void)
{
  serial_local = true;
  AddLog_P(LOG_LEVEL_INFO, PSTR("SNS: Hardware Serial"));
  SetSeriallog(LOG_LEVEL_NONE);
  Settings.baudrate = Serial.baudRate() / 300;
}

void SerialSendRaw(char *codes)
{
  char *p;
  char stemp[3];
  uint8_t code;

  int size = strlen(codes);

  while (size > 1) {
    strlcpy(stemp, codes, sizeof(stemp));
    code = strtol(stemp, &p, 16);
    Serial.write(code);
    size -= 2;
    codes += 2;
  }
}

uint32_t GetHash(const char *buffer, size_t size)
{
  uint32_t hash = 0;
  for (uint32_t i = 0; i <= size; i++) {
    hash += (uint8_t)*buffer++ * (i +1);
  }
  return hash;
}

void ShowSource(uint32_t source)
{
  if ((source > 0) && (source < SRC_MAX)) {
    char stemp1[20];
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SRC: %s"), GetTextIndexed(stemp1, sizeof(stemp1), source, kCommandSource));
  }
}

void WebHexCode(uint32_t i, const char* code)
{
  char scolor[10];

  strlcpy(scolor, code, sizeof(scolor));
  char* p = scolor;
  if ('#' == p[0]) { p++; }  // Skip

  if (3 == strlen(p)) {  // Convert 3 character to 6 character color code
    p[6] = p[3];  // \0
    p[5] = p[2];  // 3
    p[4] = p[2];  // 3
    p[3] = p[1];  // 2
    p[2] = p[1];  // 2
    p[1] = p[0];  // 1
  }

  uint32_t color = strtol(p, nullptr, 16);
/*
  if (3 == strlen(p)) {  // Convert 3 character to 6 character color code
    uint32_t w = ((color & 0xF00) << 8) | ((color & 0x0F0) << 4) | (color & 0x00F);  // 00010203
    color = w | (w << 4);                                                            // 00112233
  }
*/
  uint32_t j = sizeof(Settings.web_color) / 3;          // First area contains j = 18 colors
/*
  if (i < j) {
    Settings.web_color[i][0] = (color >> 16) & 0xFF;  // Red
    Settings.web_color[i][1] = (color >> 8) & 0xFF;   // Green
    Settings.web_color[i][2] = color & 0xFF;          // Blue
  } else {
    Settings.web_color2[i-j][0] = (color >> 16) & 0xFF;  // Red
    Settings.web_color2[i-j][1] = (color >> 8) & 0xFF;   // Green
    Settings.web_color2[i-j][2] = color & 0xFF;          // Blue
  }
*/
  if (i >= j) {
    // Calculate i to index in Settings.web_color2 - Dirty(!) but saves 128 bytes code
    i += ((((uint8_t*)&Settings.web_color2 - (uint8_t*)&Settings.web_color) / 3) - j);
  }
  Settings.web_color[i][0] = (color >> 16) & 0xFF;  // Red
  Settings.web_color[i][1] = (color >> 8) & 0xFF;   // Green
  Settings.web_color[i][2] = color & 0xFF;          // Blue
}

uint32_t WebColor(uint32_t i)
{
  uint32_t j = sizeof(Settings.web_color) / 3;          // First area contains j = 18 colors
/*
  uint32_t tcolor = (i<j)? (Settings.web_color[i][0] << 16) | (Settings.web_color[i][1] << 8) | Settings.web_color[i][2] :
                           (Settings.web_color2[i-j][0] << 16) | (Settings.web_color2[i-j][1] << 8) | Settings.web_color2[i-j][2];
*/
  if (i >= j) {
    // Calculate i to index in Settings.web_color2 - Dirty(!) but saves 128 bytes code
    i += ((((uint8_t*)&Settings.web_color2 - (uint8_t*)&Settings.web_color) / 3) - j);
  }
  uint32_t tcolor = (Settings.web_color[i][0] << 16) | (Settings.web_color[i][1] << 8) | Settings.web_color[i][2];

  return tcolor;
}

/*********************************************************************************************\
 * Response data handling
\*********************************************************************************************/

const uint16_t TIMESZ = 100;                   // Max number of characters in time string

char* ResponseGetTime(uint32_t format, char* time_str)
{
  switch (format) {
  case 1:
    snprintf_P(time_str, TIMESZ, PSTR("{\"" D_JSON_TIME "\":\"%s\",\"Epoch\":%u"), GetDateAndTime(DT_LOCAL).c_str(), UtcTime());
    break;
  case 2:
    snprintf_P(time_str, TIMESZ, PSTR("{\"" D_JSON_TIME "\":%u"), UtcTime());
    break;
  default:
    snprintf_P(time_str, TIMESZ, PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
  }
  return time_str;
}

int Response_P(const char* format, ...)        // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  va_list args;
  va_start(args, format);
  int len = vsnprintf_P(mqtt_data, sizeof(mqtt_data), format, args);
  va_end(args);
  return len;
}

int ResponseTime_P(const char* format, ...)    // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  va_list args;
  va_start(args, format);

  ResponseGetTime(Settings.flag2.time_format, mqtt_data);

  int mlen = strlen(mqtt_data);
  int len = vsnprintf_P(mqtt_data + mlen, sizeof(mqtt_data) - mlen, format, args);
  va_end(args);
  return len + mlen;
}

int ResponseAppend_P(const char* format, ...)  // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  va_list args;
  va_start(args, format);
  int mlen = strlen(mqtt_data);
  int len = vsnprintf_P(mqtt_data + mlen, sizeof(mqtt_data) - mlen, format, args);
  va_end(args);
  return len + mlen;
}

int ResponseAppendTimeFormat(uint32_t format)
{
  char time_str[TIMESZ];
  return ResponseAppend_P(ResponseGetTime(format, time_str));
}

int ResponseAppendTime(void)
{
  return ResponseAppendTimeFormat(Settings.flag2.time_format);
}

int ResponseJsonEnd(void)
{
  return ResponseAppend_P(PSTR("}"));
}

int ResponseJsonEndEnd(void)
{
  return ResponseAppend_P(PSTR("}}"));
}

/*********************************************************************************************\
 * GPIO Module and Template management
\*********************************************************************************************/

void DigitalWrite(uint32_t gpio_pin, uint32_t state)
{
  if (pin[gpio_pin] < 99) {
    digitalWrite(pin[gpio_pin], state &1);
  }
}

uint8_t ModuleNr(void)
{
  // 0    = User module (255)
  // 1 up = Template module 0 up
  return (USER_MODULE == Settings.module) ? 0 : Settings.module +1;
}

bool ValidTemplateModule(uint32_t index)
{
  for (uint32_t i = 0; i < sizeof(kModuleNiceList); i++) {
    if (index == pgm_read_byte(kModuleNiceList + i)) {
      return true;
    }
  }
  return false;
}

bool ValidModule(uint32_t index)
{
  if (index == USER_MODULE) { return true; }
  return ValidTemplateModule(index);
}

String AnyModuleName(uint32_t index)
{
  if (USER_MODULE == index) {
    return String(Settings.user_template.name);
  } else {
    return FPSTR(kModules[index].name);
  }
}

String ModuleName(void)
{
  return AnyModuleName(Settings.module);
}

void ModuleGpios(myio *gp)
{
  uint8_t *dest = (uint8_t *)gp;
  memset(dest, GPIO_NONE, sizeof(myio));

  uint8_t src[sizeof(mycfgio)];
  if (USER_MODULE == Settings.module) {
    memcpy(&src, &Settings.user_template.gp, sizeof(mycfgio));
  } else {
    memcpy_P(&src, &kModules[Settings.module].gp, sizeof(mycfgio));
  }
  // 11 85 00 85 85 00 00 00 15 38 85 00 00 81

//  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t *)&src, sizeof(mycfgio));

  uint32_t j = 0;
  for (uint32_t i = 0; i < sizeof(mycfgio); i++) {
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    dest[j] = src[i];
    j++;
  }
  // 11 85 00 85 85 00 00 00 00 00 00 00 15 38 85 00 00 81

//  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t *)gp, sizeof(myio));
}

gpio_flag ModuleFlag(void)
{
  gpio_flag flag;

  if (USER_MODULE == Settings.module) {
    flag = Settings.user_template.flag;
  } else {
    memcpy_P(&flag, &kModules[Settings.module].flag, sizeof(gpio_flag));
  }

  return flag;
}

void ModuleDefault(uint32_t module)
{
  if (USER_MODULE == module) { module = WEMOS; }  // Generic
  Settings.user_template_base = module;
  memcpy_P(&Settings.user_template, &kModules[module], sizeof(mytmplt));
}

void SetModuleType(void)
{
  my_module_type = (USER_MODULE == Settings.module) ? Settings.user_template_base : Settings.module;
}

bool FlashPin(uint32_t pin)
{
  return (((pin > 5) && (pin < 9)) || (11 == pin));
}

uint8_t ValidPin(uint32_t pin, uint32_t gpio)
{
  if (FlashPin(pin)) {
    return GPIO_NONE;    // Disable flash pins GPIO6, GPIO7, GPIO8 and GPIO11
  }

//  if (!is_8285 && !Settings.flag3.user_esp8285_enable) {  // SetOption51 - Enable ESP8285 user GPIO's
  if ((WEMOS == Settings.module) && !Settings.flag3.user_esp8285_enable) {  // SetOption51 - Enable ESP8285 user GPIO's
    if ((pin == 9) || (pin == 10)) {
      return GPIO_NONE;  // Disable possible flash GPIO9 and GPIO10
    }
  }

  return gpio;
}

bool ValidGPIO(uint32_t pin, uint32_t gpio)
{
  return (GPIO_USER == ValidPin(pin, gpio));  // Only allow GPIO_USER pins
}

bool ValidAdc(void)
{
  gpio_flag flag = ModuleFlag();
  uint32_t template_adc0 = flag.data &15;
  return (ADC0_USER == template_adc0);
}

bool GetUsedInModule(uint32_t val, uint8_t *arr)
{
  int offset = 0;

  if (!val) { return false; }  // None

  if ((val >= GPIO_KEY1) && (val < GPIO_KEY1 + MAX_KEYS)) {
    offset = (GPIO_KEY1_NP - GPIO_KEY1);
  }
  if ((val >= GPIO_KEY1_NP) && (val < GPIO_KEY1_NP + MAX_KEYS)) {
    offset = -(GPIO_KEY1_NP - GPIO_KEY1);
  }
  if ((val >= GPIO_KEY1_INV) && (val < GPIO_KEY1_INV + MAX_KEYS)) {
    offset = -(GPIO_KEY1_INV - GPIO_KEY1);
  }
  if ((val >= GPIO_KEY1_INV_NP) && (val < GPIO_KEY1_INV_NP + MAX_KEYS)) {
    offset = -(GPIO_KEY1_INV_NP - GPIO_KEY1);
  }

  if ((val >= GPIO_SWT1) && (val < GPIO_SWT1 + MAX_SWITCHES)) {
    offset = (GPIO_SWT1_NP - GPIO_SWT1);
  }
  if ((val >= GPIO_SWT1_NP) && (val < GPIO_SWT1_NP + MAX_SWITCHES)) {
    offset = -(GPIO_SWT1_NP - GPIO_SWT1);
  }

  if ((val >= GPIO_REL1) && (val < GPIO_REL1 + MAX_RELAYS)) {
    offset = (GPIO_REL1_INV - GPIO_REL1);
  }
  if ((val >= GPIO_REL1_INV) && (val < GPIO_REL1_INV + MAX_RELAYS)) {
    offset = -(GPIO_REL1_INV - GPIO_REL1);
  }

  if ((val >= GPIO_LED1) && (val < GPIO_LED1 + MAX_LEDS)) {
    offset = (GPIO_LED1_INV - GPIO_LED1);
  }
  if ((val >= GPIO_LED1_INV) && (val < GPIO_LED1_INV + MAX_LEDS)) {
    offset = -(GPIO_LED1_INV - GPIO_LED1);
  }

  if ((val >= GPIO_PWM1) && (val < GPIO_PWM1 + MAX_PWMS)) {
    offset = (GPIO_PWM1_INV - GPIO_PWM1);
  }
  if ((val >= GPIO_PWM1_INV) && (val < GPIO_PWM1_INV + MAX_PWMS)) {
    offset = -(GPIO_PWM1_INV - GPIO_PWM1);
  }

  if ((val >= GPIO_CNTR1) && (val < GPIO_CNTR1 + MAX_COUNTERS)) {
    offset = (GPIO_CNTR1_NP - GPIO_CNTR1);
  }
  if ((val >= GPIO_CNTR1_NP) && (val < GPIO_CNTR1_NP + MAX_COUNTERS)) {
    offset = -(GPIO_CNTR1_NP - GPIO_CNTR1);
  }

  for (uint32_t i = 0; i < MAX_GPIO_PIN; i++) {
    if (arr[i] == val) { return true; }
    if (arr[i] == val + offset) { return true; }
  }
  return false;
}

bool JsonTemplate(const char* dataBuf)
{
  // {"NAME":"Generic","GPIO":[17,254,29,254,7,254,254,254,138,254,139,254,254],"FLAG":1,"BASE":255}

  if (strlen(dataBuf) < 9) { return false; }  // Workaround exception if empty JSON like {} - Needs checks

  StaticJsonBuffer<350> jb;  // 331 from https://arduinojson.org/v5/assistant/
  JsonObject& obj = jb.parseObject(dataBuf);
  if (!obj.success()) { return false; }

  // All parameters are optional allowing for partial changes
  const char* name = obj[D_JSON_NAME];
  if (name != nullptr) {
    strlcpy(Settings.user_template.name, name, sizeof(Settings.user_template.name));
  }
  if (obj[D_JSON_GPIO].success()) {
    for (uint32_t i = 0; i < sizeof(mycfgio); i++) {
      Settings.user_template.gp.io[i] = obj[D_JSON_GPIO][i] | 0;
    }
  }
  if (obj[D_JSON_FLAG].success()) {
    uint8_t flag = obj[D_JSON_FLAG] | 0;
    memcpy(&Settings.user_template.flag, &flag, sizeof(gpio_flag));
  }
  if (obj[D_JSON_BASE].success()) {
    uint8_t base = obj[D_JSON_BASE];
    if ((0 == base) || !ValidTemplateModule(base -1)) { base = 18; }
    Settings.user_template_base = base -1;  // Default WEMOS
  }
  return true;
}

void TemplateJson(void)
{
  Response_P(PSTR("{\"" D_JSON_NAME "\":\"%s\",\"" D_JSON_GPIO "\":["), Settings.user_template.name);
  for (uint32_t i = 0; i < sizeof(Settings.user_template.gp); i++) {
    ResponseAppend_P(PSTR("%s%d"), (i>0)?",":"", Settings.user_template.gp.io[i]);
  }
  ResponseAppend_P(PSTR("],\"" D_JSON_FLAG "\":%d,\"" D_JSON_BASE "\":%d}"), Settings.user_template.flag, Settings.user_template_base +1);
}

/*********************************************************************************************\
 * Sleep aware time scheduler functions borrowed from ESPEasy
\*********************************************************************************************/

inline int32_t TimeDifference(uint32_t prev, uint32_t next) {
  return ((int32_t) (next - prev));
}

int32_t TimePassedSince(uint32_t timestamp)
{
  // Compute the number of milliSeconds passed since timestamp given.
  // Note: value can be negative if the timestamp has not yet been reached.
  return TimeDifference(timestamp, millis());
}

bool TimeReached(uint32_t timer)
{
  // Check if a certain timeout has been reached.
  const long passed = TimePassedSince(timer);
  return (passed >= 0);
}

void SetNextTimeInterval(unsigned long& timer, const unsigned long step)
{
  timer += step;
  const long passed = TimePassedSince(timer);
  if (passed < 0) { return; }   // Event has not yet happened, which is fine.
  if (static_cast<unsigned long>(passed) > step) {
    // No need to keep running behind, start again.
    timer = millis() + step;
    return;
  }
  // Try to get in sync again.
  timer = millis() + (step - passed);
}

/*********************************************************************************************\
 * Basic I2C routines
\*********************************************************************************************/

#ifdef USE_I2C
const uint8_t I2C_RETRY_COUNTER = 3;

uint32_t i2c_active[4] = { 0 };
uint32_t i2c_buffer = 0;

bool I2cValidRead(uint8_t addr, uint8_t reg, uint8_t size)
{
  uint8_t retry = I2C_RETRY_COUNTER;
  bool status = false;

  i2c_buffer = 0;
  while (!status && retry) {
    Wire.beginTransmission(addr);                       // start transmission to device
    Wire.write(reg);                                    // sends register address to read from
    if (0 == Wire.endTransmission(false)) {             // Try to become I2C Master, send data and collect bytes, keep master status for next request...
      Wire.requestFrom((int)addr, (int)size);           // send data n-bytes read
      if (Wire.available() == size) {
        for (uint32_t i = 0; i < size; i++) {
          i2c_buffer = i2c_buffer << 8 | Wire.read();   // receive DATA
        }
        status = true;
      }
    }
    retry--;
  }
  return status;
}

bool I2cValidRead8(uint8_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 1);
  *data = (uint8_t)i2c_buffer;
  return status;
}

bool I2cValidRead16(uint16_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 2);
  *data = (uint16_t)i2c_buffer;
  return status;
}

bool I2cValidReadS16(int16_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 2);
  *data = (int16_t)i2c_buffer;
  return status;
}

bool I2cValidRead16LE(uint16_t *data, uint8_t addr, uint8_t reg)
{
  uint16_t ldata;
  bool status = I2cValidRead16(&ldata, addr, reg);
  *data = (ldata >> 8) | (ldata << 8);
  return status;
}

bool I2cValidReadS16_LE(int16_t *data, uint8_t addr, uint8_t reg)
{
  uint16_t ldata;
  bool status = I2cValidRead16LE(&ldata, addr, reg);
  *data = (int16_t)ldata;
  return status;
}

bool I2cValidRead24(int32_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 3);
  *data = i2c_buffer;
  return status;
}

uint8_t I2cRead8(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 1);
  return (uint8_t)i2c_buffer;
}

uint16_t I2cRead16(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  return (uint16_t)i2c_buffer;
}

int16_t I2cReadS16(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  return (int16_t)i2c_buffer;
}

uint16_t I2cRead16LE(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  uint16_t temp = (uint16_t)i2c_buffer;
  return (temp >> 8) | (temp << 8);
}

int16_t I2cReadS16_LE(uint8_t addr, uint8_t reg)
{
  return (int16_t)I2cRead16LE(addr, reg);
}

int32_t I2cRead24(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 3);
  return i2c_buffer;
}

bool I2cWrite(uint8_t addr, uint8_t reg, uint32_t val, uint8_t size)
{
  uint8_t x = I2C_RETRY_COUNTER;

  do {
    Wire.beginTransmission((uint8_t)addr);              // start transmission to device
    Wire.write(reg);                                    // sends register address to write to
    uint8_t bytes = size;
    while (bytes--) {
      Wire.write((val >> (8 * bytes)) & 0xFF);          // write data
    }
    x--;
  } while (Wire.endTransmission(true) != 0 && x != 0);  // end transmission
  return (x);
}

bool I2cWrite8(uint8_t addr, uint8_t reg, uint16_t val)
{
   return I2cWrite(addr, reg, val, 1);
}

bool I2cWrite16(uint8_t addr, uint8_t reg, uint16_t val)
{
   return I2cWrite(addr, reg, val, 2);
}

int8_t I2cReadBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len)
{
  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  if (len != Wire.requestFrom((uint8_t)addr, (uint8_t)len)) {
    return 1;
  }
  while (len--) {
    *reg_data = (uint8_t)Wire.read();
    reg_data++;
  }
  return 0;
}

int8_t I2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len)
{
  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  while (len--) {
    Wire.write(*reg_data);
    reg_data++;
  }
  Wire.endTransmission();
  return 0;
}

void I2cScan(char *devs, unsigned int devs_len)
{
  // Return error codes defined in twi.h and core_esp8266_si2c.c
  // I2C_OK                      0
  // I2C_SCL_HELD_LOW            1 = SCL held low by another device, no procedure available to recover
  // I2C_SCL_HELD_LOW_AFTER_READ 2 = I2C bus error. SCL held low beyond slave clock stretch time
  // I2C_SDA_HELD_LOW            3 = I2C bus error. SDA line held low by slave/another_master after n bits
  // I2C_SDA_HELD_LOW_AFTER_INIT 4 = line busy. SDA again held low by another device. 2nd master?

  uint8_t error = 0;
  uint8_t address = 0;
  uint8_t any = 0;

  snprintf_P(devs, devs_len, PSTR("{\"" D_CMND_I2CSCAN "\":\"" D_JSON_I2CSCAN_DEVICES_FOUND_AT));
  for (address = 1; address <= 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (0 == error) {
      any = 1;
      snprintf_P(devs, devs_len, PSTR("%s 0x%02x"), devs, address);
    }
    else if (error != 2) {  // Seems to happen anyway using this scan
      any = 2;
      snprintf_P(devs, devs_len, PSTR("{\"" D_CMND_I2CSCAN "\":\"Error %d at 0x%02x"), error, address);
      break;
    }
  }
  if (any) {
    strncat(devs, "\"}", devs_len - strlen(devs) -1);
  }
  else {
    snprintf_P(devs, devs_len, PSTR("{\"" D_CMND_I2CSCAN "\":\"" D_JSON_I2CSCAN_NO_DEVICES_FOUND "\"}"));
  }
}

void I2cResetActive(uint32_t addr, uint32_t count = 1)
{
  addr &= 0x7F;         // Max I2C address is 127
  count &= 0x7F;        // Max 4 x 32 bits available
  while (count-- && (addr < 128)) {
    i2c_active[addr / 32] &= ~(1 << (addr % 32));
    addr++;
  }
//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("I2C: Active %08X,%08X,%08X,%08X"), i2c_active[0], i2c_active[1], i2c_active[2], i2c_active[3]);
}

void I2cSetActive(uint32_t addr, uint32_t count = 1)
{
  addr &= 0x7F;         // Max I2C address is 127
  count &= 0x7F;        // Max 4 x 32 bits available
  while (count-- && (addr < 128)) {
    i2c_active[addr / 32] |= (1 << (addr % 32));
    addr++;
  }
//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("I2C: Active %08X,%08X,%08X,%08X"), i2c_active[0], i2c_active[1], i2c_active[2], i2c_active[3]);
}

void I2cSetActiveFound(uint32_t addr, const char *types)
{
  I2cSetActive(addr);
  AddLog_P2(LOG_LEVEL_INFO, S_LOG_I2C_FOUND_AT, types, addr);
}

bool I2cActive(uint32_t addr)
{
  addr &= 0x7F;         // Max I2C address is 127
  if (i2c_active[addr / 32] & (1 << (addr % 32))) {
    return true;
  }
  return false;
}

bool I2cSetDevice(uint32_t addr)
{
  addr &= 0x7F;         // Max I2C address is 127
  if (I2cActive(addr)) {
    return false;       // If already active report as not present;
  }
  Wire.beginTransmission((uint8_t)addr);
  return (0 == Wire.endTransmission());
}
#endif  // USE_I2C

/*********************************************************************************************\
 * Syslog
 *
 * Example:
 *   AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_LOG "Any value %d"), value);
 *
\*********************************************************************************************/

void SetSeriallog(uint32_t loglevel)
{
  Settings.seriallog_level = loglevel;
  seriallog_level = loglevel;
  seriallog_timer = 0;
}

void SetSyslog(uint32_t loglevel)
{
  Settings.syslog_level = loglevel;
  syslog_level = loglevel;
  syslog_timer = 0;
}

#ifdef USE_WEBSERVER
void GetLog(uint32_t idx, char** entry_pp, size_t* len_p)
{
  char* entry_p = nullptr;
  size_t len = 0;

  if (idx) {
    char* it = web_log;
    do {
      uint32_t cur_idx = *it;
      it++;
      size_t tmp = strchrspn(it, '\1');
      tmp++;                             // Skip terminating '\1'
      if (cur_idx == idx) {              // Found the requested entry
        len = tmp;
        entry_p = it;
        break;
      }
      it += tmp;
    } while (it < web_log + WEB_LOG_SIZE && *it != '\0');
  }
  *entry_pp = entry_p;
  *len_p = len;
}
#endif  // USE_WEBSERVER

void Syslog(void)
{
  // Destroys log_data

  uint32_t current_hash = GetHash(SettingsText(SET_SYSLOG_HOST), strlen(SettingsText(SET_SYSLOG_HOST)));
  if (syslog_host_hash != current_hash) {
    syslog_host_hash = current_hash;
    WiFi.hostByName(SettingsText(SET_SYSLOG_HOST), syslog_host_addr);  // If sleep enabled this might result in exception so try to do it once using hash
  }
  if (PortUdp.beginPacket(syslog_host_addr, Settings.syslog_port)) {
    char syslog_preamble[64];  // Hostname + Id
    snprintf_P(syslog_preamble, sizeof(syslog_preamble), PSTR("%s ESP-"), my_hostname);
    memmove(log_data + strlen(syslog_preamble), log_data, sizeof(log_data) - strlen(syslog_preamble));
    log_data[sizeof(log_data) -1] = '\0';
    memcpy(log_data, syslog_preamble, strlen(syslog_preamble));
    PortUdp.write(log_data, strlen(log_data));
    PortUdp.endPacket();
    delay(1);  // Add time for UDP handling (#5512)
  } else {
    syslog_level = 0;
    syslog_timer = SYSLOG_TIMER;
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SYSLOG_HOST_NOT_FOUND ". " D_RETRY_IN " %d " D_UNIT_SECOND), SYSLOG_TIMER);
  }
}

void AddLog(uint32_t loglevel)
{
  char mxtime[10];  // "13:45:21 "

  snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d "), RtcTime.hour, RtcTime.minute, RtcTime.second);

  if (loglevel <= seriallog_level) {
    Serial.printf("%s%s\r\n", mxtime, log_data);
  }
#ifdef USE_WEBSERVER
  if (Settings.webserver && (loglevel <= Settings.weblog_level)) {
    // Delimited, zero-terminated buffer of log lines.
    // Each entry has this format: [index][log data]['\1']
    web_log_index &= 0xFF;
    if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
    while (web_log_index == web_log[0] ||  // If log already holds the next index, remove it
           strlen(web_log) + strlen(log_data) + 13 > WEB_LOG_SIZE)  // 13 = web_log_index + mxtime + '\1' + '\0'
    {
      char* it = web_log;
      it++;                                // Skip web_log_index
      it += strchrspn(it, '\1');           // Skip log line
      it++;                                // Skip delimiting "\1"
      memmove(web_log, it, WEB_LOG_SIZE -(it-web_log));  // Move buffer forward to remove oldest log line
    }
    snprintf_P(web_log, sizeof(web_log), PSTR("%s%c%s%s\1"), web_log, web_log_index++, mxtime, log_data);
    web_log_index &= 0xFF;
    if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
  }
#endif  // USE_WEBSERVER
  if (Settings.flag.mqtt_enabled &&        // SetOption3 - Enable MQTT
      !global_state.mqtt_down &&
      (loglevel <= Settings.mqttlog_level)) { MqttPublishLogging(mxtime); }

  if (!global_state.wifi_down &&
      (loglevel <= syslog_level)) { Syslog(); }
}

void AddLog_P(uint32_t loglevel, const char *formatP)
{
  snprintf_P(log_data, sizeof(log_data), formatP);
  AddLog(loglevel);
}

void AddLog_P(uint32_t loglevel, const char *formatP, const char *formatP2)
{
  char message[sizeof(log_data)];

  snprintf_P(log_data, sizeof(log_data), formatP);
  snprintf_P(message, sizeof(message), formatP2);
  strncat(log_data, message, sizeof(log_data) - strlen(log_data) -1);
  AddLog(loglevel);
}

void PrepLog_P2(uint32_t loglevel, PGM_P formatP, ...)
{
  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
  va_end(arg);

  prepped_loglevel = loglevel;
}

void AddLog_P2(uint32_t loglevel, PGM_P formatP, ...)
{
  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
  va_end(arg);

  AddLog(loglevel);
}

void AddLog_Debug(PGM_P formatP, ...)
{
  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
  va_end(arg);

  AddLog(LOG_LEVEL_DEBUG);
}

void AddLogBuffer(uint32_t loglevel, uint8_t *buffer, uint32_t count)
{
/*
  snprintf_P(log_data, sizeof(log_data), PSTR("DMP:"));
  for (uint32_t i = 0; i < count; i++) {
    snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, *(buffer++));
  }
  AddLog(loglevel);
*/
/*
  strcpy_P(log_data, PSTR("DMP: "));
  ToHex_P(buffer, count, log_data + strlen(log_data), sizeof(log_data) - strlen(log_data), ' ');
  AddLog(loglevel);
*/
  char hex_char[(count * 3) + 2];
  AddLog_P2(loglevel, PSTR("DMP: %s"), ToHex_P(buffer, count, hex_char, sizeof(hex_char), ' '));
}

void AddLogSerial(uint32_t loglevel)
{
  AddLogBuffer(loglevel, (uint8_t*)serial_in_buffer, serial_in_byte_counter);
}

void AddLogMissed(char *sensor, uint32_t misses)
{
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SNS: %s missed %d"), sensor, SENSOR_MAX_MISS - misses);
}
