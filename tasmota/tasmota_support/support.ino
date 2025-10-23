/*
  support.ino - support for Tasmota

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

extern "C" {
extern struct rst_info resetInfo;
}

/*********************************************************************************************\
 * ESP32 Watchdog
\*********************************************************************************************/
#ifdef ESP32
// Watchdog - yield() resets the watchdog

extern "C" void __yield(void);              // original function from Arduino Core
extern "C"
void yield(void) {
  __yield();
  feedLoopWDT();
}

// patching delay(uint32_t ms)
extern "C" void __real_delay(uint32_t ms);  // original function from Arduino Core

extern "C" void __wrap_delay(uint32_t ms) {
#ifdef USE_ESP32_WDT
  if (ms) { feedLoopWDT(); }
  __real_delay(ms);
  feedLoopWDT();
#else
  __real_delay(ms);
#endif
}

#endif // ESP32

/*********************************************************************************************\
 * Watchdog extension (https://github.com/esp8266/Arduino/issues/1532)
\*********************************************************************************************/

#ifdef ESP8266
#include <Ticker.h>

Ticker tickerOSWatch;

const uint32_t OSWATCH_RESET_TIME = 120;

static unsigned long oswatch_last_loop_time;
uint8_t oswatch_blocked_loop = 0;

#ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
//void OsWatchTicker() IRAM_ATTR;
#endif  // USE_WS2812_DMA

void OsWatchTicker(void) {
  uint32_t t = millis();
  uint32_t last_run = t - oswatch_last_loop_time;

#ifdef DEBUG_THEO
  int32_t rssi = WiFi.RSSI();
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_OSWATCH " FreeRam %d, rssi %d %% (%d dBm), last_run %d"), ESP_getFreeHeap(), WifiGetRssiAsQuality(rssi), rssi, last_run);
#endif  // DEBUG_THEO
  if (last_run >= (OSWATCH_RESET_TIME * 1000)) {
//    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_OSWATCH " " D_BLOCKED_LOOP ". " D_RESTARTING));  // Save iram space
    RtcSettings.oswatch_blocked_loop = 1;
    RtcSettingsSave();

//    ESP.restart();  // normal reboot
//    ESP.reset();  // hard reset
    // Force an exception to get a stackdump
    // ESP32: Guru Meditation Error: Core  0 panic'ed (LoadProhibited). Exception was unhandled.
    volatile uint32_t dummy;
    dummy = *((uint32_t*) 0x00000000);
    (void)dummy;    // avoid compiler warning
  }
}

void OsWatchInit(void) {
  oswatch_blocked_loop = RtcSettings.oswatch_blocked_loop;
  RtcSettings.oswatch_blocked_loop = 0;
  oswatch_last_loop_time = millis();
  tickerOSWatch.attach_ms(((OSWATCH_RESET_TIME / 3) * 1000), OsWatchTicker);
}

void OsWatchLoop(void) {
  oswatch_last_loop_time = millis();
//  while(1) delay(1000);  // this will trigger the os watch
}

bool OsWatchBlockedLoop(void) {
  return oswatch_blocked_loop;
}

#else  // Anything except ESP8266

void OsWatchInit(void) {}
void OsWatchLoop(void) {}
bool OsWatchBlockedLoop(void) {
  return false;
}

#endif  // ESP8266

uint32_t ResetReason(void) {
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
  return ESP_ResetInfoReason();
}

bool ResetReasonPowerOn(void) {
  uint32_t reset_reason = ESP_ResetInfoReason();
  return ((reset_reason == REASON_DEFAULT_RST) || (reset_reason == REASON_EXT_SYS_RST));
}

String GetResetReason(void) {
  if (OsWatchBlockedLoop()) {
    char buff[32];
    strncpy_P(buff, PSTR(D_JSON_BLOCKED_LOOP), sizeof(buff));
    return String(buff);
  } else {
    return ESP_getResetReason();
  }
}

#ifdef ESP32
/*********************************************************************************************\
 * ESP32 AutoMutex
\*********************************************************************************************/

//////////////////////////////////////////
// automutex.
// create a mute in your driver with:
// void *mutex = nullptr;
//
// then protect any function with
// TasAutoMutex m(&mutex, "somename");
// - mutex is automatically initialised if not already intialised.
// - it will be automagically released when the function is over.
// - the same thread can take multiple times (recursive).
// - advanced options m.give() and m.take() allow you fine control within a function.
// - if take=false at creat, it will not be initially taken.
// - name is used in serial log of mutex deadlock.
// - maxWait in ticks is how long it will wait before failing in a deadlock scenario (and then emitting on serial)
class TasAutoMutex {
  SemaphoreHandle_t mutex;
  bool taken;
  int maxWait;
  const char *name;
  public:
    TasAutoMutex(SemaphoreHandle_t* mutex, const char *name = "", int maxWait = 40, bool take=true);
    ~TasAutoMutex();
    void give();
    void take();
    static void init(SemaphoreHandle_t* ptr);
};
//////////////////////////////////////////

TasAutoMutex::TasAutoMutex(SemaphoreHandle_t*mutex, const char *name, int maxWait, bool take) {
  if (mutex) {
    if (!(*mutex)){
      TasAutoMutex::init(mutex);
    }
    this->mutex = *mutex;
    this->maxWait = maxWait;
    this->name = name;
    if (take) {
      this->taken = xSemaphoreTakeRecursive(this->mutex, this->maxWait);
//      if (!this->taken){
//        Serial.printf("\r\nMutexfail %s\r\n", this->name);
//      }
    }
  } else {
    this->mutex = (SemaphoreHandle_t)nullptr;
  }
}

TasAutoMutex::~TasAutoMutex() {
  if (this->mutex) {
    if (this->taken) {
      xSemaphoreGiveRecursive(this->mutex);
      this->taken = false;
    }
  }
}

void TasAutoMutex::init(SemaphoreHandle_t* ptr) {
  SemaphoreHandle_t mutex = xSemaphoreCreateRecursiveMutex();
  (*ptr) = mutex;
  // needed, else for ESP8266 as we will initialis more than once in logging
//  (*ptr) = (void *) 1;
}

void TasAutoMutex::give() {
  if (this->mutex) {
    if (this->taken) {
      xSemaphoreGiveRecursive(this->mutex);
      this->taken= false;
    }
  }
}

void TasAutoMutex::take() {
  if (this->mutex) {
    if (!this->taken) {
      this->taken = xSemaphoreTakeRecursive(this->mutex, this->maxWait);
//      if (!this->taken){
//        Serial.printf("\r\nMutexfail %s\r\n", this->name);
//      }
    }
  }
}

#endif  // ESP32


/*********************************************************************************************\
 * Miscellaneous
\*********************************************************************************************/
/*
String GetBinary(const void* ptr, size_t count) {
  uint32_t value = *(uint32_t*)ptr;
  value <<= (32 - count);
  String result;
  result.reserve(count + 1);
  for (uint32_t i = 0; i < count; i++) {
    result += (value &0x80000000) ? '1' : '0';
    value <<= 1;
  }
  return result;
}
*/
String GetBinary8(uint8_t value, size_t count) {
  if (count > 8) { count = 8; }
  value <<= (8 - count);
  String result;
  result.reserve(count + 1);
  for (uint32_t i = 0; i < count; i++) {
    result += (value &0x80) ? '1' : '0';
    value <<= 1;
  }
  return result;
}

// Get span until single character in string
size_t strchrspn(const char *str1, int character)
{
  size_t ret = 0;
  char *start = (char*)str1;
  char *end = strchr(str1, character);
  if (end) ret = end - start;
  return ret;
}

uint32_t ChrCount(const char *str, const char *delim) {
  uint32_t count = 0;
  char* read = (char*)str;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    if (ch == *delim) { count++; }
  }
  return count;
}

uint32_t ArgC(void) {
  return (XdrvMailbox.data_len > 0) ? ChrCount(XdrvMailbox.data, ",") +1 : 0;
}

// Function to return a substring defined by a delimiter at an index
char* subStr(char* dest, char* str, const char *delim, int index) {
  char* write = dest;
  char* read = str;
  char ch = '.';

  while (index && (ch != '\0')) {
    ch = *read++;
    if (strchr(delim, ch)) {
      index--;
      if (index) { write = dest; }
    } else {
      *write++ = ch;
    }
  }
  *write = '\0';
  dest = Trim(dest);
  return dest;
}

char* ArgV(char* dest, int index) {
  return subStr(dest, XdrvMailbox.data, ",", index);
}

uint32_t ArgVul(uint32_t *args, uint32_t count) {
  uint32_t argc = ArgC();
  if (argc > count) { argc = count; }
  count = argc;
  if (argc) {
    char argument[XdrvMailbox.data_len];
    for (uint32_t i = 0; i < argc; i++) {
      if (strlen(ArgV(argument, i +1))) {
        args[i] = strtoul(argument, nullptr, 0);
      } else {
        count--;
      }
    }
  }
  return count;
}

uint32_t ParseParameters(uint32_t count, uint32_t *params) {
  // Destroys XdrvMailbox.data
  char *p;
  uint32_t i = 0;
  for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < count; str = strtok_r(nullptr, ", ", &p), i++) {
    params[i] = strtoul(str, nullptr, 0);
  }
  return i;
}

float CharToFloat(const char *str)
{
  // simple ascii to double, because atof or strtod are too large
  char strbuf[24];

  strlcpy(strbuf, str, sizeof(strbuf));
  char *pt = strbuf;
  if (*pt == '\0') { return 0.0f; }

  while ((*pt != '\0') && isspace(*pt)) { pt++; }  // Trim leading spaces

  signed char sign = 1;
  if (*pt == '-') { sign = -1; }
  if (*pt == '-' || *pt == '+') { pt++; }          // Skip any sign

  float left = 0;
  if (*pt != '.') {
    left = atoi(pt);                               // Get left part
    while (isdigit(*pt)) { pt++; }                 // Skip number
  }

  float right = 0;
  if (*pt == '.') {
    pt++;
    uint32_t max_decimals = 0;
    while ((max_decimals < 8) && isdigit(pt[max_decimals])) { max_decimals++; }
    pt[max_decimals] = '\0';                       // Limit decimals to float max of 8
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

char* dtostrfd(double number, unsigned char prec, char *s)
{
  if ((isnan(number)) || (isinf(number))) {  // Fix for JSON output (https://stackoverflow.com/questions/1423081/json-left-out-infinity-and-nan-json-status-in-ecmascript)
    strcpy_P(s, PSTR("null"));
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

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: UnescapeIn %*_H"), *size, (uint8_t*)buffer);

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
//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: UnescapeOut %*_H"), *size, (uint8_t*)buffer);

  return buffer;
}

char* RemoveSpace(char* p) {
  // Remove white-space character (' ','\t','\n','\v','\f','\r')
  char* write = p;
  char* read = p;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    if (!isspace(ch)) {
      *write++ = ch;
    }
  }
  return p;
}

// remove spaces at the beginning and end of the string (but not in the middle)
char* TrimSpace(char *p) {
  // Remove white-space character (' ','\t','\n','\v','\f','\r')
  char* write = p;
  char* read = p;
  char ch = '.';

  // skip all leading spaces
  while (isspace(*read)) {
    read++;
  }
  // copy the rest
  do {
    ch = *read++;
    *write++ = ch;
  } while (ch != '\0');
  // move to end
  read = p + strlen(p);
  // move backwards
  while (p != read) {
    read--;
    if (isspace(*read)) {
      *read = '\0';
    } else {
      break;
    }
  }
  return p;
}

char* RemoveControlCharacter(char* p) {
  // Remove control character (0x00 .. 0x1F and 0x7F)
  char* write = p;
  char* read = p;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    if (!iscntrl(ch)) {
      *write++ = ch;
    }
  }
  *write++ = '\0';
  return p;
}

char* ReplaceChar(char* p, char find, char replace) {
  char* write = (char*)p;
  char* read = (char*)p;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    if (ch == find) {
      ch = replace;
    }
    *write++ = ch;
  }
  return p;
}

char* ReplaceCommaWithDot(char* p) {
  return ReplaceChar(p, ',', '.');
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

char* SetStr(const char* str) {
  if (nullptr == str) { str = PSTR(""); }       // nullptr is considered empty string
  size_t str_len = strlen(str);
  if (0 == str_len) { return EmptyStr; }        // return empty string

  char* new_str = (char*) malloc(str_len + 1);
  if (nullptr == new_str) { return EmptyStr; }  // return empty string
  strlcpy(new_str, str, str_len + 1);
  return new_str;
}

char* StrCaseStr_P(const char* source, const char* search) {
  char case_source[strlen_P(source) +1];
  UpperCase_P(case_source, source);
  char case_search[strlen_P(search) +1];
  UpperCase_P(case_search, search);
  char *cp = strstr(case_source, case_search);
  if (cp) {
    uint32_t offset = cp - case_source;
    cp = (char*)source + offset;
  }
  return cp;
}

bool IsNumeric(const char* value) {
  // Test for characters '-.0123456789'
  char *digit = (char*)value;
  while (isdigit(*digit) || *digit == '.' || *digit == '-') { digit++; }
  return (*digit == '\0');
}

char* Trim(char* p) {
  // Remove leading and trailing tab, \n, \v, \f, \r and space
  if (p == nullptr) { return p; }
  if (*p != '\0') {
    while ((*p != '\0') && isspace(*p)) { p++; }  // Trim leading spaces
    char* q = p + strlen(p) -1;
    while ((q >= p) && isspace(*q)) { q--; }   // Trim trailing spaces
    q++;
    *q = '\0';
  }
  return p;
}

String HexToString(uint8_t* data, uint32_t length) {
  if (!data || !length) { return ""; }

  uint32_t len = (length < 16) ? length : 16;
  char hex_data[32];
  ToHex_P((const unsigned char*)data, len, hex_data, sizeof(hex_data));
  String result = hex_data;
  result += F(" [");
  for (uint32_t i = 0; i < len; i++) {
    result += (isprint(data[i])) ? (char)data[i] : ' ';
  }
  result += F("]");
  if (length > len) {
    result += F(" ...");
  }
  return result;
}

// Converts a Hex string (case insensitive) into an array of bytes
// Returns the number of bytes in the array, or -1 if an error occured
// The `out` buffer must be at least half the size of hex string
int32_t HexToBytes(const char* hex, uint8_t* out, size_t out_len) {
  size_t len = strlen_P(hex);
  if (len % 2 != 0) {
    return -1;
  }

  size_t bytes_out = len / 2;
  if (bytes_out > out_len) {
    bytes_out = out_len;
  }
  
  for (size_t i = 0; i < bytes_out; i++) {
    char byte[3];
    byte[0] = hex[i*2];
    byte[1] = hex[i*2 + 1];
    byte[2] = '\0';
    
    char* endPtr;
    out[i] = strtoul(byte, &endPtr, 16);
    
    if (*endPtr != '\0') {
      return -1;
    }
  }
  return bytes_out;
}

String UrlEncode(const String& text) {
  const char hex[] = "0123456789ABCDEF";

	String encoded = "";
	int len = text.length();
	int i = 0;
	while (i < len)	{
		char decodedChar = text.charAt(i++);
/*
    if (('a' <= decodedChar && decodedChar <= 'z') ||
        ('A' <= decodedChar && decodedChar <= 'Z') ||
        ('0' <= decodedChar && decodedChar <= '9') ||
        ('=' == decodedChar)) {
      encoded += decodedChar;
		} else {
      encoded += '%';
			encoded += hex[decodedChar >> 4];
			encoded += hex[decodedChar & 0xF];
    }
*/
    if ((' ' == decodedChar) || ('+' == decodedChar)) {
      encoded += '%';
			encoded += hex[decodedChar >> 4];
			encoded += hex[decodedChar & 0xF];
    } else {
      encoded += decodedChar;
    }

	}
	return encoded;
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

  return separators[Settings->flag3.use_underscore];
*/
  if (Settings->flag3.use_underscore) {  // SetOption64 - Enable "_" instead of "-" as sensor index separator
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
  IPAddress ip_address;
  return ip_address.fromString(str);
}

bool ParseIPv4(uint32_t* addr, const char* str_p)
{
  uint8_t *part = (uint8_t*)addr;
  uint8_t i;
  char str_r[strlen_P(str_p)+1];
  char * str = &str_r[0];
  strcpy_P(str, str_p);

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

bool NewerVersion(char* version_str) {
  // Function to parse & check if version_str is newer than our currently installed version.
  uint32_t version = 0;
  uint32_t i = 0;
  char *str_ptr;

  char version_dup[strlen(version_str) +1];
  strncpy(version_dup, version_str, sizeof(version_dup));  // Duplicate the version_str as strtok_r will modify it.
  // Loop through the version string, splitting on '.' seperators.
  for (char *str = strtok_r(version_dup, ".", &str_ptr); str && i < sizeof(TASMOTA_VERSION); str = strtok_r(nullptr, ".", &str_ptr), i++) {
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
  if ((i < 2) || (i > sizeof(TASMOTA_VERSION))) {
    return false;
  }
  // Keep shifting the parsed version until we hit the maximum number of tokens.
  // VERSION stores the major number of the version in the most significant byte of the uint32_t.
  while (i < sizeof(TASMOTA_VERSION)) {
    version <<= 8;
    i++;
  }
  // Now we should have a fully constructed version number in uint32_t form.
  return (version > TASMOTA_VERSION);
}

int32_t UpdateDevicesPresent(int32_t change) {
  int32_t difference = 0;
  int32_t devices_present = TasmotaGlobal.devices_present;  // Between 0 and 32
  devices_present += change;
  if (devices_present < 0) {                          // Support down to 0
    difference = devices_present;
    devices_present = 0;
  }
  else if (devices_present >= POWER_SIZE) {           // Support up to uint32_t as bitmask
    difference = devices_present - POWER_SIZE;
    devices_present = POWER_SIZE;
//    AddLog(LOG_LEVEL_DEBUG, PSTR("APP: Max 32 devices supported"));
  }
  TasmotaGlobal.devices_present = devices_present;

//  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DVC: DevicesPresent %d, Change %d"), TasmotaGlobal.devices_present, change);

  return difference;
}

void DevicesPresentNonDisplayOrLight(uint32_t &devices_claimed) {
  uint32_t display_and_lights = 0;
#ifdef USE_LIGHT
  display_and_lights += LightDevices();               // Skip light(s)
#endif  // USE_LIGHT
#ifdef USE_DISPLAY
  display_and_lights += DisplayDevices();             // Skip display
#endif  // USE_DISPLAY
  uint32_t devices_present = TasmotaGlobal.devices_present - display_and_lights;
  if (devices_claimed > devices_present) {
    devices_claimed = devices_present;                // Reduce amount of claimed devices
  }
}

char* GetPowerDevice(char* dest, uint32_t idx, size_t size, uint32_t option)
{
  strncpy_P(dest, S_RSLT_POWER, size);                // POWER
  if ((TasmotaGlobal.devices_present + option) > 1) {
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

float ConvertTempToFahrenheit(float tc) {
  if (isnan(tc)) { return NAN; }

  float result = tc;
  if (Settings->flag.temperature_conversion) {                 // SetOption8 - Switch between Celsius or Fahrenheit
    result = tc * 1.8f + 32;                                    // Fahrenheit
  }
  result = result + (0.1f * Settings->temp_comp);
  return result;
}

float ConvertTempToCelsius(float tf) {
  if (isnan(tf)) { return NAN; }

  float result = tf;
  if (Settings->flag.temperature_conversion) {                 // SetOption8 - Switch between Celsius or Fahrenheit
    result = (tf - 32) / 1.8f;                                  // Celsius
  }
  return result;
}

void UpdateGlobalTemperature(float t) {
  if (!Settings->global_sensor_index[0] && !TasmotaGlobal.user_globals[0]) {
    TasmotaGlobal.global_update = TasmotaGlobal.uptime;
    TasmotaGlobal.temperature_celsius = t;
  }
}

float ConvertTemp(float t) {
  UpdateGlobalTemperature(t);

  return ConvertTempToFahrenheit(t);
}

char TempUnit(void) {
  // SetOption8  - Switch between Celsius or Fahrenheit
  return (Settings->flag.temperature_conversion) ? D_UNIT_FAHRENHEIT[0] : D_UNIT_CELSIUS[0];
}

float ConvertHumidity(float h) {
  float result = h;

  if (!Settings->global_sensor_index[1] && !TasmotaGlobal.user_globals[1]) {
    TasmotaGlobal.global_update = TasmotaGlobal.uptime;
    TasmotaGlobal.humidity = h;
  }

  result = result + (0.1f * Settings->hum_comp);

  return result;
}

float CalcTempHumToDew(float t, float h) {
  if (isnan(h) || isnan(t)) { return NAN; }

  if (Settings->flag.temperature_conversion) {                 // SetOption8 - Switch between Celsius or Fahrenheit
    t = (t - 32) / 1.8f;                                       // Celsius
  }

  float gamma = TaylorLog(h / 100) + 17.62f * t / (243.5f + t);
  float result = (243.5f * gamma / (17.62f - gamma));

  if (Settings->flag.temperature_conversion) {                 // SetOption8 - Switch between Celsius or Fahrenheit
    result = result * 1.8f + 32;                               // Fahrenheit
  }
  return result;
}

#ifdef USE_HEAT_INDEX
float CalcTemHumToHeatIndex(float t, float h) {
  if (isnan(h) || isnan(t)) { return NAN; }

  if (!Settings->flag.temperature_conversion) {                // SetOption8 - Switch between Celsius or Fahrenheit
    t = t * 1.8f + 32;                                         // Fahrenheit
  }
  float hi = 0.5 * (t + 61.0 + ((t - 68.0) * 1.2) + (h * 0.094));
  if (hi > 79) {
    float pt = t * t;  // pow(t, 2)
    float ph = h * h;  // pow(h, 2)
    hi = -42.379 + 2.04901523 * t + 10.14333127 * h +
         -0.22475541 * t * h +
         -0.00683783 * pt +
         -0.05481717 * ph +
         0.00122874 * pt * h +
         0.00085282 * t * ph +
         -0.00000199 * pt * ph;
    if ((h < 13) && (t >= 80.0) && (t <= 112.0)) {
      hi -= ((13.0 - h) * 0.25) * sqrtf((17.0 - abs(t - 95.0)) * 0.05882);
    }
    else if ((h > 85.0) && (t >= 80.0) && (t <= 87.0)) {
      hi += ((h - 85.0) * 0.1) * ((87.0 - t) * 0.2);
    }
  }
  if (!Settings->flag.temperature_conversion) {                // SetOption8 - Switch between Celsius or Fahrenheit
    hi = (hi - 32) / 1.8f;                                     // Celsius
  }
  return hi;
}
#endif  // USE_HEAT_INDEX

float CalcTempHumToAbsHum(float t, float h) {
  if (isnan(t) || isnan(h)) { return NAN; }
  // taken from https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/
  // precision is about 0.1°C in range -30 to 35°C
  // August-Roche-Magnus 	6.1094 exp(17.625 x T)/(T + 243.04)
  // Buck (1981) 		6.1121 exp(17.502 x T)/(T + 240.97)
  // reference https://www.eas.ualberta.ca/jdwilson/EAS372_13/Vomel_CIRES_satvpformulae.html

  if (Settings->flag.temperature_conversion) {                 // SetOption8 - Switch between Celsius or Fahrenheit
    t = (t - 32) / 1.8f;                                       // Celsius
  }

  float temp = FastPrecisePow(2.718281828f, (17.67f * t) / (t + 243.5f));

  const float mw = 18.01534f;                                  // Molar mass of water g/mol
  const float r = 8.31447215f;                                 // Universal gas constant J/mol/K
//  return (6.112 * temp * h * 2.1674) / (273.15 + t);           // Simplified version
  return (6.112f * temp * h * mw) / ((273.15f + t) * r);       // Long version
}

float ConvertHgToHpa(float p) {
  // Convert mmHg (or inHg) to hPa
  float result = p;
  if (!isnan(p) && Settings->flag.pressure_conversion) {       // SetOption24 - Switch between hPa or mmHg pressure unit
    if (Settings->flag5.mm_vs_inch) {                          // SetOption139 - Switch between mmHg or inHg pressure unit
      result = p * 33.86389f;                                  // inHg (double to float saves 16 bytes!)
    } else {
      result = p * 1.3332239f;                                 // mmHg (double to float saves 16 bytes!)
    }
  }
  return result;
}

float ConvertPressure(float p) {
  // Convert hPa to mmHg (or inHg)
  float result = p;

  if (!Settings->global_sensor_index[2] && !TasmotaGlobal.user_globals[2]) {
    TasmotaGlobal.global_update = TasmotaGlobal.uptime;
    TasmotaGlobal.pressure_hpa = p;
  }

  if (!isnan(p) && Settings->flag.pressure_conversion) {       // SetOption24 - Switch between hPa or mmHg pressure unit
    if (Settings->flag5.mm_vs_inch) {                          // SetOption139 - Switch between mmHg or inHg pressure unit
//      result = p * 0.02952998016471;                           // inHg
      result = p * 0.0295299f;                                 // inHg (double to float saves 16 bytes!)
    } else {
//      result = p * 0.75006375541921;                           // mmHg
      result = p * 0.7500637f;                                 // mmHg (double to float saves 16 bytes!)
    }
  }
  return result;
}

float ConvertPressureForSeaLevel(float pressure) {
  if (pressure == 0.0f) {
    return pressure;
  }
  return ConvertPressure((pressure / FastPrecisePowf(1.0f - ((float)Settings->altitude / 44330.0f), 5.255f)) - 21.6f);
}

const char kPressureUnit[] PROGMEM = D_UNIT_PRESSURE "|" D_UNIT_MILLIMETER_MERCURY "|" D_UNIT_INCH_MERCURY;

String PressureUnit(void) {
  uint32_t index = (Settings->flag.pressure_conversion) ? Settings->flag5.mm_vs_inch +1 : 0;
  char text[8];
  return String(GetTextIndexed(text, sizeof(text), index, kPressureUnit));
}

float ConvertSpeed(float s)
{
  // Entry in m/s
  return s * kSpeedConversionFactor[Settings->flag2.speed_conversion];
}

String SpeedUnit(void) {
  char text[8];
  return String(GetTextIndexed(text, sizeof(text), Settings->flag2.speed_conversion, kSpeedUnit));
}

void ResetGlobalValues(void)
{
  if ((TasmotaGlobal.uptime - TasmotaGlobal.global_update) > GLOBAL_VALUES_VALID) {  // Reset after 5 minutes
    TasmotaGlobal.global_update = 0;
    TasmotaGlobal.temperature_celsius = NAN;
    TasmotaGlobal.humidity = 0.0f;
    TasmotaGlobal.pressure_hpa = 0.0f;
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

bool DecodeCommand(const char* haystack, void (* const MyCommand[])(void), const uint8_t *synonyms = nullptr);
bool DecodeCommand(const char* haystack, void (* const MyCommand[])(void), const uint8_t *synonyms) {
  SHOW_FREE_MEM(PSTR("DecodeCommand"));

  GetTextIndexed(XdrvMailbox.command, CMDSZ, 0, haystack);  // Get prefix if available
  int prefix_length = strlen(XdrvMailbox.command);
  if (prefix_length) {
    char prefix[prefix_length +1];
    snprintf_P(prefix, sizeof(prefix), XdrvMailbox.topic);  // Copy prefix part only
    if (strcasecmp(prefix, XdrvMailbox.command)) {
      return false;                                         // Prefix not in command
    }
  }
  size_t syn_count = synonyms ? pgm_read_byte(synonyms) : 0;
  int command_code = GetCommandCode(XdrvMailbox.command + prefix_length, CMDSZ, XdrvMailbox.topic + prefix_length, haystack);
  if (command_code > 0) {                                   // Skip prefix
    if (command_code > syn_count) {
      // We passed the synonyms zone, it's a regular command
      XdrvMailbox.command_code = command_code - 1 - syn_count;
      MyCommand[XdrvMailbox.command_code]();
    } else {
      // We have a SetOption synonym
      XdrvMailbox.index = pgm_read_byte(synonyms + command_code);
      CmndSetoptionBase(0);
    }
    return true;
  }
  return false;
}

const char kOptions[] PROGMEM = "OFF|" D_OFF "|FALSE|" D_FALSE "|STOP|" D_STOP "|" D_CELSIUS "|DOWN|" D_CLOSE "|"   // 0
                                "ON|" D_ON "|TRUE|" D_TRUE "|START|" D_START "|" D_FAHRENHEIT "|" D_USER "|"  // 1
                                "TOGGLE|" D_TOGGLE "|" D_ADMIN "|"                                            // 2
                                "BLINK|" D_BLINK "|"                                                          // 3
                                "BLINKOFF|" D_BLINKOFF "|"                                                    // 4
                                "UP|" D_OPEN  "|"                                                             // 100
                                "ALL" ;                                                                       // 255

const uint8_t sNumbers[] PROGMEM = { 0,0,0,0,0,0,0,0,0,
                                     1,1,1,1,1,1,1,1,
                                     2,2,2,
                                     3,3,
                                     4,4,
                                     100,100,
                                     255 };

int GetStateNumber(const char *state_text)
{
  char command[CMDSZ];
  int state_number = GetCommandCode(command, sizeof(command), state_text, kOptions);
  if (state_number >= 0) {
    state_number = pgm_read_byte(sNumbers + state_number);
  }
  return state_number;
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
    AddLog(LOG_LEVEL_DEBUG, PSTR("SRC: %s"), GetTextIndexed(stemp1, sizeof(stemp1), source, kCommandSource));
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
  uint32_t j = sizeof(Settings->web_color) / 3;          // First area contains j = 18 colors
/*
  if (i < j) {
    Settings->web_color[i][0] = (color >> 16) & 0xFF;  // Red
    Settings->web_color[i][1] = (color >> 8) & 0xFF;   // Green
    Settings->web_color[i][2] = color & 0xFF;          // Blue
  } else {
    Settings->web_color2[i-j][0] = (color >> 16) & 0xFF;  // Red
    Settings->web_color2[i-j][1] = (color >> 8) & 0xFF;   // Green
    Settings->web_color2[i-j][2] = color & 0xFF;          // Blue
  }
*/
  if (i >= j) {
    // Calculate i to index in Settings->web_color2 - Dirty(!) but saves 128 bytes code
    i += ((((uint8_t*)&Settings->web_color2 - (uint8_t*)&Settings->web_color) / 3) - j);
  }
  Settings->web_color[i][0] = (color >> 16) & 0xFF;  // Red
  Settings->web_color[i][1] = (color >> 8) & 0xFF;   // Green
  Settings->web_color[i][2] = color & 0xFF;          // Blue
}

uint32_t WebColor(uint32_t i)
{
  uint32_t j = sizeof(Settings->web_color) / 3;          // First area contains j = 18 colors
/*
  uint32_t tcolor = (i<j)? (Settings->web_color[i][0] << 16) | (Settings->web_color[i][1] << 8) | Settings->web_color[i][2] :
                           (Settings->web_color2[i-j][0] << 16) | (Settings->web_color2[i-j][1] << 8) | Settings->web_color2[i-j][2];
*/
  if (i >= j) {
    // Calculate i to index in Settings->web_color2 - Dirty(!) but saves 128 bytes code
    i += ((((uint8_t*)&Settings->web_color2 - (uint8_t*)&Settings->web_color) / 3) - j);
  }
  uint32_t tcolor = (Settings->web_color[i][0] << 16) | (Settings->web_color[i][1] << 8) | Settings->web_color[i][2];

  return tcolor;
}

void AllowInterrupts(bool state) {
  if (!state) {  // Stop interrupts
    XdrvXsnsCall(FUNC_INTERRUPT_STOP);

#ifdef USE_EMULATION
    UdpDisconnect();
#endif  // USE_EMULATION
  } else {       // Start interrupts
#ifdef USE_EMULATION
    UdpConnect();
#endif  // USE_EMULATION

    XdrvXsnsCall(FUNC_INTERRUPT_START);
  }
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
  case 3:
    snprintf_P(time_str, TIMESZ, PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL_MILLIS).c_str());
    break;
  default:
    snprintf_P(time_str, TIMESZ, PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
  }
  return time_str;
}

char* ResponseData(void) {
  return (char*)TasmotaGlobal.mqtt_data.c_str();
}

uint32_t ResponseSize(void) {
  return MAX_LOGSZ;                            // Arbitratry max length satisfying full log entry
}

uint32_t ResponseLength(void) {
  return TasmotaGlobal.mqtt_data.length();
}

void ResponseClear(void) {
  // Reset string length to zero
  TasmotaGlobal.mqtt_data = "";
//  TasmotaGlobal.mqtt_data = (const char*) nullptr;  // Doesn't work on ESP32 as strlen() (in MqttPublishPayload) will fail (for obvious reasons)
}

void ResponseJsonStart(void) {
  // Insert a JSON start bracket {
  TasmotaGlobal.mqtt_data.setCharAt(0,'{');
}

int Response_P(const char* format, ...)        // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  va_list arg;
  va_start(arg, format);
  char* mqtt_data = ext_vsnprintf_malloc_P(format, arg);
  va_end(arg);
  if (mqtt_data != nullptr) {
    TasmotaGlobal.mqtt_data = mqtt_data;
    free(mqtt_data);
  } else {
    TasmotaGlobal.mqtt_data = "";
  }
  return TasmotaGlobal.mqtt_data.length();
}

int ResponseTime_P(const char* format, ...)    // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  char timestr[100];
  TasmotaGlobal.mqtt_data = ResponseGetTime(Settings->flag2.time_format, timestr);

  va_list arg;
  va_start(arg, format);
  char* mqtt_data = ext_vsnprintf_malloc_P(format, arg);
  va_end(arg);
  if (mqtt_data != nullptr) {
    TasmotaGlobal.mqtt_data += mqtt_data;
    free(mqtt_data);
  }
  return TasmotaGlobal.mqtt_data.length();
}

int ResponseAppend_P(const char* format, ...)  // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  va_list arg;
  va_start(arg, format);
  char* mqtt_data = ext_vsnprintf_malloc_P(format, arg);
  va_end(arg);
  if (mqtt_data != nullptr) {
    TasmotaGlobal.mqtt_data += mqtt_data;
    free(mqtt_data);
  }
  return TasmotaGlobal.mqtt_data.length();
}

int ResponseAppendTimeFormat(uint32_t format)
{
  char time_str[TIMESZ];
  return ResponseAppend_P(ResponseGetTime(format, time_str));
}

int ResponseAppendTime(void)
{
  return ResponseAppendTimeFormat(Settings->flag2.time_format);
}

int ResponseAppendTHD(float f_temperature, float f_humidity) {
  float dewpoint = CalcTempHumToDew(f_temperature, f_humidity);
  int len = ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%*_f,\"" D_JSON_HUMIDITY "\":%*_f,\"" D_JSON_DEWPOINT "\":%*_f"),
                             Settings->flag2.temperature_resolution, &f_temperature,
                             Settings->flag2.humidity_resolution, &f_humidity,
                             Settings->flag2.temperature_resolution, &dewpoint);
#ifdef USE_HEAT_INDEX
  float heatindex = CalcTemHumToHeatIndex(f_temperature, f_humidity);
  int len2 = ResponseAppend_P(PSTR(",\"" D_JSON_HEATINDEX "\":%*_f"),
                              Settings->flag2.temperature_resolution, &heatindex);
  return len + len2;                              
#endif  // USE_HEAT_INDEX
  return len;
}

int ResponseJsonEnd(void)
{
  return ResponseAppend_P(PSTR("}"));
}

int ResponseJsonEndEnd(void)
{
  return ResponseAppend_P(PSTR("}}"));
}

bool ResponseContains_P(const char* needle) {
/*
  return (strstr_P(TasmotaGlobal.mqtt_data.c_str(), needle) != nullptr);
*/
  return (strstr_P(ResponseData(), needle) != nullptr);
}

bool GetNextSensor(void) {
  static uint32_t start_time = 0;
  static uint8_t sensor_set = 0;

  ResponseClear();
  int tele_period_save = TasmotaGlobal.tele_period;
  TasmotaGlobal.tele_period = 2;                     // Do not allow HA updates during next function call
  while (!ResponseLength()) {
    if (0 == sensor_set) {
      if (TimeReached(start_time)) {
        SetNextTimeInterval(start_time, 1000);
        sensor_set++;                                // Minimal loop time is 1 second
      }
      break;
    }
    else if (1 == sensor_set) {
      if (!XsnsCallNextJsonAppend()) {               // ,"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}
        sensor_set++;                                // Looped
        break;
      }
    }
    else {
      if (!XdrvCallNextJsonAppend()) {               // ,"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}
        sensor_set = 0;                              // Looped
        break;
      }
    }
  }

//  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DBG: GetNextSensor %d, %d"), sensor_set, ResponseLength());

  TasmotaGlobal.tele_period = tele_period_save;
  if (ResponseLength()) {
    ResponseJsonStart();                             // {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}}
    ResponseJsonEnd();

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DBG: GetNextSensor %d, '%s'"), sensor_set, ResponseData());

    return true;
  }
  return false;
}

/*********************************************************************************************\
 * GPIO Module and Template management
\*********************************************************************************************/

#ifdef ESP8266
uint16_t GpioConvert(uint8_t gpio) {
  if (gpio >= nitems(kGpioConvert)) {
    return AGPIO(GPIO_USER);
  }
  return pgm_read_word(kGpioConvert + gpio);
}

uint16_t Adc0Convert(uint8_t adc0) {
  if (adc0 > 7) {
    return AGPIO(GPIO_USER);
  }
  else if (0 == adc0) {
    return GPIO_NONE;
  }
  return AGPIO(GPIO_ADC_INPUT + adc0 -1);
}

void TemplateConvert(uint8_t template8[], uint16_t template16[]) {
  for (uint32_t i = 0; i < (sizeof(mytmplt) / 2) -2; i++) {
    template16[i] = GpioConvert(template8[i]);
  }
  template16[(sizeof(mytmplt) / 2) -2] = Adc0Convert(template8[sizeof(mytmplt8285) -1]);
}

void ConvertGpios(void) {
  if (Settings->gpio16_converted != 0xF5A0) {
    // Convert 8-bit user template
    TemplateConvert((uint8_t*)&Settings->ex_user_template8, (uint16_t*)&Settings->user_template);

    for (uint32_t i = 0; i < sizeof(Settings->ex_my_gp8.io); i++) {
      Settings->my_gp.io[i] = GpioConvert(Settings->ex_my_gp8.io[i]);
    }
    Settings->my_gp.io[(sizeof(myio) / 2) -1] = Adc0Convert(Settings->ex_my_adc0);
    Settings->gpio16_converted = 0xF5A0;
  }
}
#endif  // ESP8266

int IRAM_ATTR Pin(uint32_t gpio, uint32_t index = 0) {
  uint16_t real_gpio = gpio << 5;
  uint16_t mask = 0xFFE0;
  if (index < GPIO_ANY) {
    real_gpio += index;
    mask = 0xFFFF;
  }
  for (uint32_t i = 0; i < nitems(TasmotaGlobal.gpio_pin); i++) {
    if ((TasmotaGlobal.gpio_pin[i] & mask) == real_gpio) {
      return i;              // Pin number configured for gpio
    }
  }
  return -1;                 // No pin used for gpio
}

bool PinUsed(uint32_t gpio, uint32_t index = 0);
bool PinUsed(uint32_t gpio, uint32_t index) {
  return (Pin(gpio, index) >= 0);
}

uint32_t GetPin(uint32_t lpin) {
  if (lpin < nitems(TasmotaGlobal.gpio_pin)) {
    return TasmotaGlobal.gpio_pin[lpin];
  } else {
    return GPIO_NONE;
  }
}

void SetPin(uint32_t lpin, uint32_t gpio) {
  TasmotaGlobal.gpio_pin[lpin] = gpio;
}

void DigitalWrite(uint32_t gpio_pin, uint32_t index, uint32_t state) {
  static uint32_t pinmode_init[2] = { 0 };       // Pins 0 to 63 !!!

  if (PinUsed(gpio_pin, index)) {
    uint32_t pin = Pin(gpio_pin, index) & 0x3F;  // Fix possible overflow over 63 gpios
    if (!bitRead(pinmode_init[pin / 32], pin % 32)) {
      bitSet(pinmode_init[pin / 32], pin % 32);
      pinMode(pin, OUTPUT);
    }
    digitalWrite(pin, state &1);
  }
}

uint8_t ModuleNr(void)
{
  // 0    = User module (255)
  // 1 up = Template module 0 up
  return (USER_MODULE == Settings->module) ? 0 : Settings->module +1;
}

uint32_t ModuleTemplate(uint32_t module) {
  uint32_t i = 0;
  for (i = 0; i < sizeof(kModuleNiceList); i++) {
    if (module == pgm_read_byte(kModuleNiceList + i)) {
      break;
    }
  }
  if (i == sizeof(kModuleNiceList)) { i = 0; }
  return i;
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

bool ValidTemplate(const char *search) {
  return (StrCaseStr_P(SettingsText(SET_TEMPLATE_NAME), search) != nullptr);
}

String AnyModuleName(uint32_t index)
{
  if (USER_MODULE == index) {
    return String(SettingsText(SET_TEMPLATE_NAME));
  } else {
#ifdef ESP32
    index = ModuleTemplate(index);
#endif
    char name[TOPSZ];
    return String(GetTextIndexed(name, sizeof(name), index, kModuleNames));
  }
}

String ModuleName(void)
{
  return AnyModuleName(Settings->module);
}

#ifdef ESP8266
void GetInternalTemplate(void* ptr, uint32_t module, uint32_t option) {
  uint8_t module_template = pgm_read_byte(kModuleTemplateList + module);

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Template %d, Option %d"), module_template, option);

  // template8 = GPIO 0,1,2,3,4,5,9,10,12,13,14,15,16,Adc
  uint8_t template8[sizeof(mytmplt8285)] = { GPIO_NONE };
  if (module_template < TMP_WEMOS) {
    memcpy_P(&template8, &kModules8266[module_template], 6);
    memcpy_P(&template8[8], &kModules8266[module_template].gp.io[6], 6);
  } else {
    memcpy_P(&template8, &kModules8285[module_template - TMP_WEMOS], sizeof(template8));
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: GetInternalTemplate %*_H"), sizeof(mytmplt8285), (uint8_t *)&template8);

  // template16  = GPIO 0,1,2,3,4,5,9,10,12,13,14,15,16,Adc,Flg
  uint16_t template16[(sizeof(mytmplt) / 2)] = { GPIO_NONE };
  TemplateConvert(template8, template16);

  uint32_t index = 0;
  uint32_t size = sizeof(mycfgio);      // template16[module_template].gp
  switch (option) {
    case 2: {
      index = (sizeof(mytmplt) / 2) -1; // template16[module_template].flag
      size = 2;
      break;
    }
    case 3: {
      size = sizeof(mytmplt);           // template16[module_template]
      break;
    }
  }
  memcpy(ptr, &template16[index], size);

//  AddLog(LOG_LEVEL_DEBUG, PSTR("FNC: GetInternalTemplate option %d, %*_V"), option, size / 2, (uint8_t *)ptr);
}
#endif  // ESP8266

#ifdef CONFIG_IDF_TARGET_ESP32
// Conversion table from gpio template to physical gpio
const uint8_t Esp32TemplateToPhy[MAX_USER_PINS] = { ESP32_TEMPLATE_TO_PHY };
#endif // CONFIG_IDF_TARGET_ESP32

void TemplateGpios(myio *gp)
{
  uint16_t *dest = (uint16_t *)gp;
  uint16_t src[nitems(Settings->user_template.gp.io)];

  memset(dest, GPIO_NONE, sizeof(myio));
  if (USER_MODULE == Settings->module) {
    memcpy(&src, &Settings->user_template.gp, sizeof(mycfgio));
  } else {
#ifdef ESP8266
    GetInternalTemplate(&src, Settings->module, 1);
#endif  // ESP8266
#ifdef ESP32
    memcpy_P(&src, &kModules[ModuleTemplate(Settings->module)].gp, sizeof(mycfgio));
#endif  // ESP32
  }
  // 11 85 00 85 85 00 00 00 15 38 85 00 00 81

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: TemplateGpiosIn %*_H"), sizeof(mycfgio), (uint8_t *)&src);

  // Expand template to physical GPIO array, j=phy_GPIO, i=template_GPIO
  uint32_t j = 0;
  for (uint32_t i = 0; i < nitems(Settings->user_template.gp.io); i++) {
/*
#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32C3
    dest[i] = src[i];
#elif defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
    if (22 == i) { j = 33; }    // skip 22-32
    dest[j] = src[i];
    j++;
#elif defined(CONFIG_IDF_TARGET_ESP32)
    dest[Esp32TemplateToPhy[i]] = src[i];
#else // ESP8266
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    dest[j] = src[i];
    j++;
#endif
*/
#ifdef ESP8266
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    dest[j] = src[i];
    j++;
#endif  // ESP8266
#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32P4
    dest[i] = src[i];
#elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
    if (22 == i) { j = 33; }    // skip 22-32
    dest[j] = src[i];
    j++;
#else  // ESP32
    dest[Esp32TemplateToPhy[i]] = src[i];
#endif  // ESP32C2/C3/C6 and S2/S3
#endif  // ESP32
  }
  // 11 85 00 85 85 00 00 00 00 00 00 00 15 38 85 00 00 81

//  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: TemplateGpiosOut %*_H"), sizeof(myio), (uint8_t *)gp);
}

gpio_flag ModuleFlag(void)
{
  gpio_flag flag;

  if (USER_MODULE == Settings->module) {
    flag = Settings->user_template.flag;
  } else {
#ifdef ESP8266
    GetInternalTemplate(&flag, Settings->module, 2);
#endif  // ESP8266
#ifdef ESP32
    memcpy_P(&flag, &kModules[ModuleTemplate(Settings->module)].flag, sizeof(gpio_flag));
#endif  // ESP32
  }

  return flag;
}

void ModuleDefault(uint32_t module)
{
  if (USER_MODULE == module) { module = WEMOS; }  // Generic
  Settings->user_template_base = module;

#ifdef ESP32
  module = ModuleTemplate(module);
#endif

  char name[TOPSZ];
  SettingsUpdateText(SET_TEMPLATE_NAME, GetTextIndexed(name, sizeof(name), module, kModuleNames));
#ifdef ESP8266
  GetInternalTemplate(&Settings->user_template, module, 3);
#endif  // ESP8266
#ifdef ESP32
  memcpy_P(&Settings->user_template, &kModules[module], sizeof(mytmplt));
#endif  // ESP32
}

void SetModuleType(void)
{
  TasmotaGlobal.module_type = (USER_MODULE == Settings->module) ? Settings->user_template_base : Settings->module;
#ifdef ESP32
  if (TasmotaGlobal.emulated_module_type) {
    TasmotaGlobal.module_type = TasmotaGlobal.emulated_module_type;
  }
#endif
}

bool FlashPin(uint32_t pin) {
#ifdef ESP8266
  return (((pin > 5) && (pin < 9)) || (11 == pin));
#endif  // ESP8266
#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32C2
  return (((pin > 10) && (pin < 12)) || ((pin > 13) && (pin < 18)));  // ESP32C3 has GPIOs 11-17 reserved for Flash, with some boards GPIOs 12 13 are useable
#elif CONFIG_IDF_TARGET_ESP32C3
  return ((pin > 13) && (pin < 18));   // ESP32C3 has GPIOs 11-17 reserved for Flash, with some boards GPIOs 11 12 13 are useable
#elif CONFIG_IDF_TARGET_ESP32C5
  return ((pin > 15) && (pin < 23));   // ESP32C5 has GPIOs 16-22 reserved for Flash
#elif CONFIG_IDF_TARGET_ESP32C6
  return ((pin == 24) || (pin == 25) || (pin == 27) || (pin == 29) || (pin == 30));  // ESP32C6 has GPIOs 24-30 reserved for Flash, with some boards GPIOs 26 28 are useable
#elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
  return (pin > 21) && (pin < 33);     // ESP32S2 skip 22-32
#elif CONFIG_IDF_TARGET_ESP32P4
  return false;                        // ESP32P4 has no flash pins, but GPIOs 34-38 are strapping pins
#else
  return (pin >= 28) && (pin <= 31);   // ESP32 skip 28-31
#endif  // ESP32C2/C3/C5/C6 and S2/S3
#endif  // ESP32
}

bool RedPin(uint32_t pin) {            // Pin may be dangerous to change, display in RED in template console
#ifdef ESP8266
  return (9 == pin) || (10 == pin);
#endif  // ESP8266
#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32C2
  return (12 == pin) || (13 == pin);   // ESP32C2: GPIOs 12 13 are usually used for Flash (mode QIO/QOUT)
#elif CONFIG_IDF_TARGET_ESP32C3
  return (11 == pin) || (12 == pin) || (13 == pin);  // ESP32C3: GPIOs 11 12 13 are usually used for Flash (mode QIO/QOUT)
#elif CONFIG_IDF_TARGET_ESP32C5
  return (2 == pin) || (7 == pin) || (25 == pin) || (27 == pin) || (28 == pin);  // ESP32C5: GPIO2,7,25,27,28 are strapping pins
#elif CONFIG_IDF_TARGET_ESP32C6
  return (26 == pin) || (28 == pin);   // ESP32C6: GPIOs 26 28 are usually used for Flash (mode QIO/QOUT)
#elif CONFIG_IDF_TARGET_ESP32S2
  return false;                        // No red pin on ESP32S3
#elif  CONFIG_IDF_TARGET_ESP32P4
  return (34 >= pin) && (38 <= pin);   // strapping pins on ESP32P4
#elif CONFIG_IDF_TARGET_ESP32S3
  return (33 <= pin) && (37 >= pin);   // ESP32S3: GPIOs 33..37 are usually used for PSRAM
#else   // ESP32 red pins are 6-11 for original ESP32, other models like PICO are not impacted if flash pins are condfigured
  // PICO can also have 16/17/18/23 not available
  return ((6 <= pin) && (11 >= pin)) || (16 == pin) || (17 == pin);  // TODO adapt depending on the exact type of ESP32
#endif  // ESP32C2/C3/C6 and S2/S3
#endif  // ESP32
}

uint32_t ValidPin(uint32_t pin, uint32_t gpio, uint8_t isTuya = false) {
  if (FlashPin(pin)) {
    return GPIO_NONE;    // Disable flash pins GPIO6, GPIO7, GPIO8 and GPIO11
  }

#ifdef ESP8266
  if (((WEMOS == Settings->module) || isTuya) && !Settings->flag3.user_esp8285_enable) {  // SetOption51 - Enable ESP8285 user GPIO's
    if ((9 == pin) || (10 == pin)) {
      return GPIO_NONE;  // Disable possible flash GPIO9 and GPIO10
    }
  }
#endif

  return gpio;
}

bool ValidGPIO(uint32_t pin, uint32_t gpio) {
#ifdef ESP8266
#ifdef USE_ADC_VCC
  if (ADC0_PIN == pin) { return false; }  // ADC0 = GPIO17
#endif
#endif
  return (GPIO_USER == ValidPin(pin, BGPIO(gpio)));  // Only allow GPIO_USER pins
}


bool ValidSpiPinUsed(uint32_t gpio) {
  // ESP8266: If SPI pin selected chk if it's not one of the three Hardware SPI pins (12..14)
  bool result = false;
  if (PinUsed(gpio)) {
    int pin = Pin(gpio);
    result = ((pin < 12) || (pin > 14));
  }
  return result;
}

bool JsonTemplate(char* dataBuf)
{
  // Old: {"NAME":"Shelly 2.5","GPIO":[56,0,17,0,21,83,0,0,6,82,5,22,156],"FLAG":2,"BASE":18}
  // New: {"NAME":"Shelly 2.5","GPIO":[320,0,32,0,224,193,0,0,640,192,608,225,3456,4736],"FLAG":0,"BASE":18}

//  AddLog(LOG_LEVEL_DEBUG, PSTR("TPL: |%s|"), dataBuf);

  if (strlen(dataBuf) < 9) { return false; }  // Workaround exception if empty JSON like {} - Needs checks

  JsonParser parser((char*) dataBuf);
  JsonParserObject root = parser.getRootObject();
  if (!root) { return false; }

  // All parameters are optional allowing for partial changes
  JsonParserToken val = root[PSTR(D_JSON_NAME)];
  if (val) {
    SettingsUpdateText(SET_TEMPLATE_NAME, val.getStr());
  }
  JsonParserArray arr = root[PSTR(D_JSON_GPIO)];
  if (arr) {
#ifdef ESP8266
    bool old_template = false;
    uint8_t template8[sizeof(mytmplt8285)] = { GPIO_NONE };
    if (13 == arr.size()) {  // Possible old template
      uint32_t gpio = 0;
      for (uint32_t i = 0; i < nitems(template8) -1; i++) {
        gpio = arr[i].getUInt();
        if (gpio > 255) {    // New templates might have values above 255
          break;
        }
        template8[i] = gpio;
      }
      old_template = (gpio < 256);
    }
    if (old_template) {

      AddLog(LOG_LEVEL_DEBUG, PSTR("TPL: Converting template ..."));

      val = root[PSTR(D_JSON_FLAG)];
      if (val) {
        template8[nitems(template8) -1] = val.getUInt() & 0x0F;
      }
      TemplateConvert(template8, Settings->user_template.gp.io);
      Settings->user_template.flag.data = 0;
    } else {
#endif
      for (uint32_t i = 0; i < nitems(Settings->user_template.gp.io); i++) {
        JsonParserToken val = arr[i];
        if (!val) { break; }
        uint16_t gpio = val.getUInt();
        if (gpio == (AGPIO(GPIO_NONE) +1)) {
          gpio = AGPIO(GPIO_USER);
        }
        Settings->user_template.gp.io[i] = gpio;
      }
      val = root[PSTR(D_JSON_FLAG)];
      if (val) {
        Settings->user_template.flag.data = val.getUInt();
      }
    }
#ifdef ESP8266
  }
#endif
  val = root[PSTR(D_JSON_BASE)];
  if (val) {
    uint32_t base = val.getUInt();
    if ((0 == base) || !ValidTemplateModule(base -1)) { base = 18; }
    Settings->user_template_base = base -1;  // Default WEMOS
  }

  val = root[PSTR(D_JSON_CMND)];
  if (val) {
    if ((USER_MODULE == Settings->module) || StrCaseStr_P(val.getStr(), PSTR(D_CMND_MODULE " 0"))) {  // Only execute if current module = USER_MODULE = this template
      char* backup_data = XdrvMailbox.data;
      XdrvMailbox.data = (char*)val.getStr();   // Backlog commands
      ReplaceChar(XdrvMailbox.data, '|', ';');  // Support '|' as command separator for JSON backwards compatibility
      uint32_t backup_data_len = XdrvMailbox.data_len;
      XdrvMailbox.data_len = 1;                 // Any data
      uint32_t backup_index = XdrvMailbox.index;
      XdrvMailbox.index = 0;                    // Backlog0 - no delay
      CmndBacklog();
      XdrvMailbox.index = backup_index;
      XdrvMailbox.data_len = backup_data_len;
      XdrvMailbox.data = backup_data;
    }
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("TPL: Converted %*_V"), sizeof(Settings->user_template) / 2, (uint8_t*)&Settings->user_template);

  return true;
}

void TemplateJson(void)
{
//  AddLog(LOG_LEVEL_DEBUG, PSTR("TPL: Show %*_V"), sizeof(Settings->user_template) / 2, (uint8_t*)&Settings->user_template);

  Response_P(PSTR("{\"" D_JSON_NAME "\":\"%s\",\"" D_JSON_GPIO "\":["), SettingsText(SET_TEMPLATE_NAME));
  for (uint32_t i = 0; i < nitems(Settings->user_template.gp.io); i++) {
    uint16_t gpio = Settings->user_template.gp.io[i];
    if (gpio == AGPIO(GPIO_USER)) {
      gpio = AGPIO(GPIO_NONE) +1;
    }
    ResponseAppend_P(PSTR("%s%d"), (i>0)?",":"", gpio);
  }
  ResponseAppend_P(PSTR("],\"" D_JSON_FLAG "\":%d,\"" D_JSON_BASE "\":%d}"), Settings->user_template.flag, Settings->user_template_base +1);
}

#if ( defined(USE_SCRIPT) && defined(SUPPORT_MQTT_EVENT) ) || defined (USE_DT_VARS)

/*********************************************************************************************\
 * Parse json paylod with path
\*********************************************************************************************/
// parser object, source keys, delimiter, float result or NULL, string result or NULL, string size
// return 1 if numeric 2 if string, else 0 = not found
uint32_t JsonParsePath(JsonParserObject *jobj, const char *spath, char delim, float *nres, char *sres, uint32_t slen) {
  uint32_t res = 0;
  const char *cp = spath;
#ifdef DEBUG_JSON_PARSE_PATH
  AddLog(LOG_LEVEL_INFO, PSTR("JSON: parsing json key: %s from json: %s"), cp, jpath);
#endif
  JsonParserObject obj = *jobj;
  JsonParserObject lastobj = obj;
  char selem[64];
  uint8_t aindex = 0;
  String value = "";
  while (1) {
    // read next element
    for (uint32_t sp=0; sp<sizeof(selem)-1; sp++) {
      if (!*cp || *cp==delim) {
        selem[sp] = 0;
        cp++;
        break;
      }
      selem[sp] = *cp++;
    }
#ifdef DEBUG_JSON_PARSE_PATH
    AddLog(LOG_LEVEL_INFO, PSTR("JSON: cmp current key: %s"), selem);
#endif
    // check for array
    char *sp = strchr(selem,'[');
    if (sp) {
      *sp = 0;
      aindex = atoi(sp+1);
    }

    // now check element
    obj = obj[selem];
    if (!obj.isValid()) {
#ifdef DEBUG_JSON_PARSE_PATH
      AddLog(LOG_LEVEL_INFO, PSTR("JSON: obj invalid: %s"), selem);
#endif
      JsonParserToken tok = lastobj[selem];
      if (tok.isValid()) {
        if (tok.isArray()) {
          JsonParserArray array = JsonParserArray(tok);
          value = array[aindex].getStr();
          if (array.isNum()) {
            if (nres) *nres=tok.getFloat();
            res = 1;
          } else {
            res = 2;
          }
        } else {
          value = tok.getStr();
          if (tok.isNum()) {
            if (nres) *nres=tok.getFloat();
            res = 1;
          } else {
            res = 2;
          }
        }

      }
#ifdef DEBUG_JSON_PARSE_PATH
      AddLog(LOG_LEVEL_INFO, PSTR("JSON: token invalid: %s"), selem);
#endif
      break;
    }
    if (obj.isObject()) {
      lastobj = obj;
      continue;
    }
    if (!*cp) break;
  }
  if (sres) {
    strlcpy(sres,value.c_str(), slen);
  }
  return res;

}

#endif // USE_SCRIPT

/*********************************************************************************************\
 * Serial
\*********************************************************************************************/

String GetSerialConfig(uint8_t serial_config) {
  // Settings->serial_config layout
  // b000000xx - 5, 6, 7 or 8 data bits
  // b00000x00 - 1 or 2 stop bits
  // b000xx000 - None, Even or Odd parity

  const static char kParity[] PROGMEM = "NEOI";

  char config[4];
  config[0] = '5' + (serial_config & 0x3);
  config[1] = pgm_read_byte(&kParity[(serial_config >> 3) & 0x3]);
  config[2] = '1' + ((serial_config >> 2) & 0x1);
  config[3] = '\0';
  return String(config);
}

String GetSerialConfig(void) {
  return GetSerialConfig(Settings->serial_config);
}

int8_t ParseSerialConfig(const char *pstr)
{
  if (strlen(pstr) < 3)
    return -1;

  int8_t serial_config = (uint8_t)atoi(pstr);
  if (serial_config < 5 || serial_config > 8)
    return -1;
  serial_config -= 5;

  char parity = (pstr[1] & 0xdf);
  if ('E' == parity) {
    serial_config += 0x08;                         // Even parity
  }
  else if ('O' == parity) {
    serial_config += 0x10;                         // Odd parity
  }
  else if ('N' != parity) {
    return -1;
  }

  if ('2' == pstr[2]) {
    serial_config += 0x04;                         // Stop bits 2
  }
  else if ('1' != pstr[2]) {
    return -1;
  }

  return serial_config;
}

uint32_t ConvertSerialConfig(uint8_t serial_config) {
#ifdef ESP8266
  return (uint32_t)pgm_read_byte(kTasmotaSerialConfig + serial_config);
#elif defined(ESP32)
  return (uint32_t)pgm_read_dword(kTasmotaSerialConfig + serial_config);
#else
  #error "platform not supported"
#endif
}

// workaround disabled 05.11.2021 solved with https://github.com/espressif/arduino-esp32/pull/5549
//#if defined(ESP32) && CONFIG_IDF_TARGET_ESP32C3
// temporary workaround, see https://github.com/espressif/arduino-esp32/issues/5287
//#include <driver/uart.h>
//uint32_t GetSerialBaudrate(void) {
//  uint32_t br;
//  uart_get_baudrate(0, &br);
//  return (br / 300) * 300;  // Fix ESP32 strange results like 115201
//}
//#else
uint32_t GetSerialBaudrate(void) {
//  return (Serial.baudRate() / 300) * 300;  // Fix ESP32 strange results like 115201
// Since core 3.0.4 the returned baudrate could even be 115942 instead of 115200 !!!
  uint32_t margin = 300;
  uint32_t baudrate = Serial.baudRate();
  if (baudrate > 10000) {
    margin = 2400;
  }
  return (baudrate / margin) * margin;  // Fix ESP32 strange results like 115201
}
//#endif

#ifdef ESP8266
void SetSerialSwap(void) {
  if ((15 == Pin(GPIO_TXD)) && (13 == Pin(GPIO_RXD))) {
    Serial.flush();
    Serial.swap();
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_SERIAL "Serial pins swapped to alternate"));
  }
}
#endif

void SetSerialBegin(void) {
  TasmotaGlobal.baudrate = Settings->baudrate * 300;
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_SERIAL "Set to %s %d bit/s"), GetSerialConfig().c_str(), TasmotaGlobal.baudrate);
  Serial.flush();
#ifdef ESP8266
  Serial.begin(TasmotaGlobal.baudrate, (SerialConfig)ConvertSerialConfig(Settings->serial_config));
  SetSerialSwap();
#endif  // ESP8266
#ifdef ESP32
#if ARDUINO_USB_MODE
//  Serial.end();
//  Serial.begin();
  // Above sequence ends in "Exception":5,"Reason":"Load access fault"
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_SERIAL "HWCDC supports 115200 bit/s only"));
#else
  delay(10);  // Allow time to cleanup queues - if not used hangs ESP32
  Serial.end();
  delay(10);  // Allow time to cleanup queues - if not used hangs ESP32
  Serial.begin(TasmotaGlobal.baudrate, ConvertSerialConfig(Settings->serial_config));
#endif  // Not ARDUINO_USB_MODE
#endif  // ESP32
}

void SetSerialInitBegin(void) {
  TasmotaGlobal.baudrate = Settings->baudrate * 300;
  if ((GetSerialBaudrate() != TasmotaGlobal.baudrate) || (TS_SERIAL_8N1 != Settings->serial_config)) {
    SetSerialBegin();
  }
}

void SetSerialConfig(uint32_t serial_config) {
  if (serial_config > TS_SERIAL_8O2) {
    serial_config = TS_SERIAL_8N1;
  }
  if (serial_config != Settings->serial_config) {
    Settings->serial_config = serial_config;
    SetSerialBegin();
  }
}

void SetSerialBaudrate(uint32_t baudrate) {
  TasmotaGlobal.baudrate = baudrate;
  Settings->baudrate = TasmotaGlobal.baudrate / 300;
  if (GetSerialBaudrate() != TasmotaGlobal.baudrate) {
    SetSerialBegin();
  }
}

void SetSerial(uint32_t baudrate, uint32_t serial_config) {
  Settings->flag.mqtt_serial = 0;  // CMND_SERIALSEND and CMND_SERIALLOG
  Settings->serial_config = serial_config;
  TasmotaGlobal.baudrate = baudrate;
  Settings->baudrate = TasmotaGlobal.baudrate / 300;
  SetSeriallog(LOG_LEVEL_NONE);
  SetSerialBegin();
}

void ClaimSerial(void) {
#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#ifdef USE_USB_CDC_CONSOLE
  if (!tasconsole_serial) {
    return;              // USB console does not use serial
  }
#endif  // USE_USB_CDC_CONSOLE
#endif  // ESP32C3/C6, S2 or S3
#endif  // ESP32
  TasmotaGlobal.serial_local = true;
  AddLog(LOG_LEVEL_INFO, PSTR("SNS: Hardware Serial"));
  SetSeriallog(LOG_LEVEL_NONE);
  TasmotaGlobal.baudrate = GetSerialBaudrate();
  Settings->baudrate = TasmotaGlobal.baudrate / 300;
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

// values is a comma-delimited string: e.g. "72,101,108,108,111,32,87,111,114,108,100,33,10"
void SerialSendDecimal(char *values)
{
  char *p;
  uint8_t code;
  for (char* str = strtok_r(values, ",", &p); str; str = strtok_r(nullptr, ",", &p)) {
    code = (uint8_t)atoi(str);
    Serial.write(code);
  }
}

/*********************************************************************************************/

uint8_t Bcd2Dec(uint8_t n) {
  return n - 6 * (n >> 4);
}

uint8_t Dec2Bcd(uint8_t n) {
  return n + 6 * (n / 10);
}

/*********************************************************************************************/

uint8_t TasShiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
  uint8_t value = 0;

  for (uint32_t i = 0; i < 8; ++i) {
    digitalWrite(clockPin, HIGH);
#ifdef ESP32
    delayMicroseconds(1);
#endif
    if(bitOrder == LSBFIRST) {
      value |= digitalRead(dataPin) << i;
    } else {
      value |= digitalRead(dataPin) << (7 - i);
    }
    digitalWrite(clockPin, LOW);
#ifdef ESP32
    delayMicroseconds(1);
#endif
  }
  return value;
}

void TasShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
  for (uint32_t i = 0; i < 8; i++) {
    if(bitOrder == LSBFIRST) {
      digitalWrite(dataPin, !!(val & (1 << i)));
    } else {
      digitalWrite(dataPin, !!(val & (1 << (7 - i))));
    }
    digitalWrite(clockPin, HIGH);
#ifdef ESP32
    delayMicroseconds(1);
#endif
    digitalWrite(clockPin, LOW);
#ifdef ESP32
    delayMicroseconds(1);
#endif
  }
}

/*********************************************************************************************\
 * Sleep aware time scheduler functions borrowed from ESPEasy
\*********************************************************************************************/
/*
// No need to use 64-bit
inline uint64_t GetMicros64() {
#ifdef ESP8266
  return micros64();
#endif
#ifdef ESP32
  return esp_timer_get_time();
#endif
}

inline int64_t TimeDifference64(uint64_t prev, uint64_t next) {
  return ((int64_t) (next - prev));
}

int64_t TimePassedSince64(const uint64_t& timestamp) {
  return TimeDifference64(timestamp, GetMicros64());
}

bool TimeReached64(const uint64_t& timer) {
  return TimePassedSince64(timer) >= 0;
}
*/

// Return the time difference as a signed value, taking into account the timers may overflow.
// Returned timediff is between -24.9 days and +24.9 days.
// Returned value is positive when "next" is after "prev"
inline int32_t TimeDifference(uint32_t prev, uint32_t next) {
  return ((int32_t) (next - prev));
}

int32_t TimePassedSince(uint32_t timestamp) {
  // Compute the number of milliSeconds passed since timestamp given.
  // Note: value can be negative if the timestamp has not yet been reached.
  return TimeDifference(timestamp, millis());
}

bool TimeReached(uint32_t timer) {
  // Check if a certain timeout has been reached.
  // This is roll-over proof.
  return TimePassedSince(timer) >= 0;
}

void SetNextTimeInterval(uint32_t& timer, const uint32_t step) {
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

int32_t TimePassedSinceUsec(uint32_t timestamp) {
  return TimeDifference(timestamp, micros());
}

bool TimeReachedUsec(uint32_t timer) {
  // Check if a certain timeout has been reached.
  const long passed = TimePassedSinceUsec(timer);
  return (passed >= 0);
}

void SystemBusyDelay(uint32_t busy) {
/*
  TasmotaGlobal.busy_time = millis();
  SetNextTimeInterval(TasmotaGlobal.busy_time, busy +1);
  if (!TasmotaGlobal.busy_time) {
    TasmotaGlobal.busy_time++;
  }
*/
  TasmotaGlobal.busy_time = busy;
}

void SystemBusyDelayExecute(void) {
  if (TasmotaGlobal.busy_time) {
/*
    // Calls to millis() interrupt RMT and defeats our goal
    if (!TimeReached(TasmotaGlobal.busy_time)) {
      delay(1);
    }
*/
    delay(TasmotaGlobal.busy_time);
    TasmotaGlobal.busy_time = 0;
  }
}

/*********************************************************************************************\
 * Syslog
 *
 * Example:
 *   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LOG "Any value %d"), value);
 *
\*********************************************************************************************/

void SetMinimumSeriallog(void) {
  TasmotaGlobal.seriallog_level = (Settings->seriallog_level < LOG_LEVEL_INFO) ? (uint8_t)LOG_LEVEL_INFO : Settings->seriallog_level;
}

void SetTasConlog(uint32_t loglevel) {
  Settings->seriallog_level = loglevel;
  TasmotaGlobal.seriallog_level = loglevel;
  TasmotaGlobal.seriallog_timer = 0;
}

void SetSeriallog(uint32_t loglevel) {
#ifdef ESP32
  if (tasconsole_serial) {
#endif  // ESP32

  SetTasConlog(loglevel);

#ifdef ESP32
  }
#endif  // ESP32
}

void SetSyslog(uint32_t loglevel)
{
  Settings->syslog_level = loglevel;
  TasmotaGlobal.syslog_level = loglevel;
  TasmotaGlobal.syslog_timer = 0;
}

void SyslogAsync(bool refresh) {
  static IPAddress syslog_host_addr;      // Syslog host IP address
  static uint32_t syslog_host_hash = 0;   // Syslog host name hash
  static uint32_t index = 1;

  if (!TasmotaGlobal.syslog_level || TasmotaGlobal.global_state.network_down) { return; }
  if (refresh && !NeedLogRefresh(TasmotaGlobal.syslog_level, index)) { return; }

  char* line;
  size_t len;
  while (int loglevel = GetLog(TasmotaGlobal.syslog_level, &index, &line, &len)) {
    // <--- mxtime ---> TAG: <---------------------- MSG ---------------------------->
    // 00:00:02.096-029 HTP: Web server active on wemos5 with IP address 192.168.2.172
    //                  HTP: Web server active on wemos5 with IP address 192.168.2.172
    uint32_t mxtime = strchr(line, ' ') - line +1;  // Remove mxtime
    if (mxtime > 0) {
      uint32_t current_hash = GetHash(SettingsText(SET_SYSLOG_HOST), strlen(SettingsText(SET_SYSLOG_HOST)));
      if (syslog_host_hash != current_hash) {
        IPAddress temp_syslog_host_addr;
        if (!WifiHostByName(SettingsText(SET_SYSLOG_HOST), temp_syslog_host_addr)) {  // If sleep enabled this might result in exception so try to do it once using hash
          TasmotaGlobal.syslog_level = 0;
          TasmotaGlobal.syslog_timer = SYSLOG_TIMER;
          AddLog(LOG_LEVEL_INFO, PSTR("SLG: " D_RETRY_IN " %d " D_UNIT_SECOND), SYSLOG_TIMER);
          return;
        }
        syslog_host_hash = current_hash;
        syslog_host_addr = temp_syslog_host_addr;
      }
      if (!PortUdp.beginPacket(syslog_host_addr, Settings->syslog_port)) {
        TasmotaGlobal.syslog_level = 0;
        TasmotaGlobal.syslog_timer = SYSLOG_TIMER;
        AddLog(LOG_LEVEL_INFO, PSTR("SLG: " D_SYSLOG_HOST_NOT_FOUND ". " D_RETRY_IN " %d " D_UNIT_SECOND), SYSLOG_TIMER);
        return;
      }

      char header[128];
      /* Legacy format (until v13.3.0.1) - HOSTNAME TAG: MSG
         SYSLOG-MSG = wemos5 ESP-HTP: Web server active on wemos5 with IP address 192.168.2.172
         Result = 2023-12-20T13:41:11.825749+01:00 wemos5 ESP-HTP: Web server active on wemos5 with IP address 192.168.2.172
           and below message in syslog if hostname starts with a "z"
         2023-12-17T00:09:52.797782+01:00 domus8 rsyslogd: Uncompression of a message failed with return code -3 - enable debug logging if you need further information. Message ignored. [v8.2302.0]
         Notice in both cases the date and time is taken from the syslog server

         Example of rsyslog filter using rsyslog properties:
         :programname, startswith, "ESP-" /var/log/udp-logs/esp.log  # Log in esp.log
         :programname, startswith, "ESP-" stop                       # Do not log in syslog

      */
//      snprintf_P(header, sizeof(header), PSTR("%s ESP-"), NetworkHostname());

      /* Legacy format - <PRI>HOSTNAME TAG: MSG
         <PRI> = Facility 16 (= local use 0), Severity 6 (= informational) => 16 * 8 + 6 = 128 + 6 = <134>
         SYSLOG-MSG = <134>wemos5 ESP-HTP: Web server active on wemos5 with IP address 192.168.2.172
         Result = 2023-12-21T11:31:50.378816+01:00 wemos5 ESP-HTP: Web server active on wemos5 with IP address 192.168.2.172
         Notice in both cases the date and time is taken from the syslog server. Uncompression message is gone.

         Translate Tasmota loglevel to syslog severity level:
         LOG_LEVEL_ERROR      1 -> severity level 3 - Error
         LOG_LEVEL_INFO       2 -> severity level 6 - Informational
         LOG_LEVEL_DEBUG      3 -> severity level 7 - Debug
         LOG_LEVEL_DEBUG_MORE 4 -> severity level 7 - Debug

         Example of rsyslog filter using rsyslog properties:
         :programname, startswith, "ESP-" /var/log/udp-logs/esp.log  # Log in esp.log
         :programname, startswith, "ESP-" stop                       # Do not log in syslog
      */
//      snprintf_P(header, sizeof(header), PSTR("<%d>%s ESP-"), 128 + min(loglevel * 3, 7), NetworkHostname());

      /* RFC3164 - BSD syslog protocol - <PRI>TIMESTAMP HOSTNAME TAG: MSG
         <PRI> = Facility 16 (= local use 0), Severity 6 (= informational) => 16 * 8 + 6 = <134>
         TIMESTAMP = Mmm dd hh:mm:ss
         TAG: = ESP-HTP:
         SYSLOG-MSG = <134>Jan  1 00:00:02 wemos5 ESP-HTP: Web server active on wemos5 with IP address 192.168.2.172
         Result = 2023-01-01T00:00:02+01:00 wemos5 ESP-HTP: Web server active on wemos5 with IP address 192.168.2.172
         Notice Year is taken from syslog server. Month, day and time is provided by Tasmota device. No milliseconds

         Example of rsyslog filter using rsyslog properties:
         :programname, startswith, "ESP-" /var/log/udp-logs/esp.log  # Log in esp.log
         :programname, startswith, "ESP-" stop                       # Do not log in syslog
      */
//      snprintf_P(header, sizeof(header), PSTR("<134>%s %s ESP-"), GetSyslogDate(line).c_str(), NetworkHostname());

      char* msg_start = line +mxtime;
      uint32_t msg_len = len -mxtime -1;

      /* RFC5424 - Syslog protocol - <PRI>VERSION TIMESTAMP HOSTNAME APP_NAME PROCID MSGID STRUCTURED-DATA MSG
         <PRI>[5] = Facility 16 (= local use 0), Severity 6 (= informational) => 16 * 8 + 6 = <134>
         VERSION[2] = 1
         TIMESTAMP = yyyy-mm-ddThh:mm:ss.nnnnnn-hh:mm (= local with timezone)
         HOSTNAME[255] = wemos5
         APP_NAME[48] = tasmota
         PROCID[128] = -
         MSGID[32] = HTP:
         STRUCTURED-DATA = -
         SYSLOG-MSG = <134>1 1970-01-01T00:00:02.096000+01:00 wemos5 tasmota - HTP: - Web server active on wemos5 with IP address 192.168.2.172
         Result = 1970-01-01T00:00:02.096000+00:00 wemos5 tasmota HTP: Web server active on wemos5 with IP address 192.168.2.172
         Notice date and time is provided by Tasmota device.

         Example of rsyslog filter using rsyslog properties:
         :programname, isequal, "tasmota" /var/log/udp-logs/esp.log  # Log in esp.log
         :programname, isequal, "tasmota" stop                       # Do not log in syslog
      */
      char timestamp[mxtime];
      subStr(timestamp, line, " ", 1);                        // 00:00:02.096-026
      subStr(timestamp, timestamp, "-", 1);                   // 00:00:02.096

      snprintf_P(header, sizeof(header), PSTR("<%d>1 %s%s000%s %s tasmota - - -"),
        128 + min(loglevel * 3, 7),                           // Error (1) = 131, Info (2) = 134, Debug (3) = 135, DebugMore = (4) 135
        GetDate().c_str(), timestamp, GetTimeZone().c_str(),  // 1970-01-01T00:00:02.096000+01:00
        NetworkHostname());
/*
      // msgid is currently not well supported in rsyslog (https://github.com/rsyslog/rsyslog/issues/3592#issuecomment-480186237)
      char msgid[5];
      char* line_msgid = strchr(msg_start, ' ');
      if (line_msgid && (line_msgid - msg_start < sizeof(msgid))) {  // Only 3 character message ids supported
        subStr(msgid, msg_start, " ", 1);                     // HTP:
        uint32_t strlen_msgid = strlen(msgid) +1;
        msg_start += strlen_msgid;
        msg_len -= strlen_msgid;
      } else {
        strcpy(msgid, "-");                                   // -
      }
      snprintf_P(header, sizeof(header), PSTR("<%d>1 %s%s000%s %s tasmota - %s -"),
        128 + min(loglevel * 3, 7),                           // Error (1) = 131, Info (2) = 134, Debug (3) = 135, DebugMore = (4) 135
        GetDate().c_str(), timestamp, GetTimeZone().c_str(),  // 1970-01-01T00:00:02.096000+01:00
        NetworkHostname(), msgid);
*/

/*
      TasConsole.printf((char*)"Loglevel ");
      char number[10];
      TasConsole.printf(itoa(loglevel, number, 10));
      TasConsole.printf((char*)", Header '");
      TasConsole.printf(header);
      TasConsole.printf((char*)"', Msg '");
      TasConsole.write((uint8_t*)msg_start, msg_len);
      TasConsole.printf((char*)"'\r\n");
*/
#ifdef ESP8266
      // Packets over 1460 bytes are not send
      uint32_t package_len;
      int32_t log_len = msg_len;
      while (log_len > 0) {
        PortUdp.write(header);
        package_len = (log_len > 1460) ? 1460 : log_len;
        PortUdp.write((uint8_t*)msg_start, package_len);
        PortUdp.endPacket();
        log_len -= 1460;
        msg_start += 1460;
      }
#else
      PortUdp.write((const uint8_t*)header, strlen(header));
      PortUdp.write((uint8_t*)msg_start, msg_len);
      PortUdp.endPacket();
#endif

      delay(1);                          // Add time for UDP handling (#5512)
    }
  }
}

bool NeedLogRefresh(uint32_t req_loglevel, uint32_t index) {
  if (!TasmotaGlobal.log_buffer) { return false; }  // Leave now if there is no buffer available

#ifdef ESP32
  // this takes the mutex, and will be release when the class is destroyed -
  // i.e. when the functon leaves  You CAN call mutex.give() to leave early.
  TasAutoMutex mutex((SemaphoreHandle_t *)&TasmotaGlobal.log_buffer_mutex);
#endif  // ESP32

  // Skip initial buffer fill
  if (strlen(TasmotaGlobal.log_buffer) < LOG_BUFFER_SIZE / 2) { return false; }

  char* line;
  size_t len;
  if (!GetLog(req_loglevel, &index, &line, &len)) { return false; }
  return ((line - TasmotaGlobal.log_buffer) < LOG_BUFFER_SIZE / 4);
}

uint32_t GetLog(uint32_t req_loglevel, uint32_t* index_p, char** entry_pp, size_t* len_p) {
  if (!TasmotaGlobal.log_buffer) { return 0; }  // Leave now if there is no buffer available
  if (TasmotaGlobal.uptime < 3) { return 0; }   // Allow time to setup correct log level

  uint32_t index = *index_p;
  if (!req_loglevel || (index == TasmotaGlobal.log_buffer_pointer)) { return 0; }

#ifdef ESP32
  // this takes the mutex, and will be release when the class is destroyed -
  // i.e. when the functon leaves  You CAN call mutex.give() to leave early.
  TasAutoMutex mutex((SemaphoreHandle_t *)&TasmotaGlobal.log_buffer_mutex);
#endif  // ESP32

  if (!index) {                            // Dump all
    index = TasmotaGlobal.log_buffer[0];
  }

  do {
    size_t len = 0;
    uint32_t loglevel = 0;
    char* entry_p = TasmotaGlobal.log_buffer;
    do {
      uint32_t cur_idx = *entry_p;
      entry_p++;
      size_t tmp = strchrspn(entry_p, '\1');
      tmp++;                               // Skip terminating '\1'
      if (cur_idx == index) {              // Found the requested entry
        loglevel = *entry_p - '0';
        entry_p++;                         // Skip loglevel
        len = tmp -1;
        break;
      }
      entry_p += tmp;
    } while (entry_p < TasmotaGlobal.log_buffer + LOG_BUFFER_SIZE && *entry_p != '\0');
    index++;
    if (index > 255) { index = 1; }        // Skip 0 as it is not allowed
    *index_p = index;
    if ((len > 0) &&
        (loglevel <= req_loglevel) &&
        (TasmotaGlobal.masterlog_level <= req_loglevel)) {
      *entry_pp = entry_p;
      *len_p = len;
      return loglevel;
    }
    delay(0);
  } while (index != TasmotaGlobal.log_buffer_pointer);
  return 0;
}

bool LogDataJsonPrettyPrint(const char *log_line, uint32_t log_data_len, std::function<void(const char*, uint32_t)> println) {
  // log_line:
  // 14:49:36.123 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}
  // 14:30:16.749-172/38 MQT: tele/atomlite3/INFO3 = {"Info3":{"RestartReason":"Vbat power on reset","BootCount":74}}

  if (!Settings->mbflag2.json_pretty_print) { return false; }  // [JsonPP] Number of indents
  char *bch = (char*)memchr(log_line, '{', log_data_len);
  if (!bch) { return false; }                // No JSON data

  uint32_t pos_brace = bch - log_line;
  uint32_t cnt_brace = 0;                    // {}
  uint32_t len_mxtime = strchr(log_line, ' ') - log_line +2;
  uint32_t pos_value_pair = pos_brace;
  uint32_t cnt_bracket = 0;                  // []
  uint32_t cnt_Indent = 0;                   // indent
  bool quotes = false;                       // ""
  bool bracket_comma = false;
  bool pls_print = false;
  for (uint32_t i = pos_brace; i < log_data_len; i++) {
    char curchar = log_line[i];
    char nxtchar = log_line[i +1];
    cnt_Indent = cnt_brace + cnt_bracket;
    if (curchar == '{') {
      cnt_brace++;
      pls_print = true;
    }
    else if (cnt_brace) {
      if (nxtchar == '}') {
        pls_print = true;
      }
      if (curchar == '}') {
        cnt_brace--;
        if (cnt_brace) {
          if (nxtchar != ',') {
            pls_print = true;
            cnt_Indent = cnt_brace + cnt_bracket;
          }
        } else {
          pls_print = true;
          cnt_Indent = 0;
        }
      }
      else if (curchar == '[') {
        cnt_bracket++;
        if (nxtchar == '[') {
          pls_print = true;
        }
      }
      else if (curchar == ']') {
        cnt_bracket--;
        if (nxtchar == ',') {
          bracket_comma = true;
        }
        else {
          pls_print = true;
          if ((nxtchar == ']') || (nxtchar == '}')) {
            cnt_Indent = cnt_brace + cnt_bracket;
          }
        }
      }
      else if (curchar == '"') {
        quotes ^= 1;
      }
      else if (curchar == ',') {
        if (!quotes && (!cnt_bracket || bracket_comma)) {
          bracket_comma = false;
          pls_print = true;
        }
      }
    }

    if (pls_print) {
      pls_print = false;
      uint32_t len_id = (pos_brace == i) ? pos_brace +1 : len_mxtime;
      uint32_t len_indent = cnt_Indent * Settings->mbflag2.json_pretty_print;
      uint32_t len_value_pair = (i - pos_value_pair) +1;
      uint32_t len_full = len_id + len_indent + len_value_pair +1;

      char line[len_full];              // Known max value pair size is 152
      strlcpy(line, log_line, len_id);  // Repeat mxtime
      sprintf(line, "%s%*s", line, len_indent, "");  // Add space indent
      strncat(line, log_line + pos_value_pair, len_value_pair);
      println(line, strlen(line));      // Callback for output
      pos_value_pair = i +1;
    }
  }
  return true;
}

uint32_t HighestLogLevel(void) {
  uint32_t highest_loglevel = TasmotaGlobal.seriallog_level;
  if (Settings->seriallog_level > highest_loglevel) { highest_loglevel = Settings->seriallog_level; }
  if (Settings->mqttlog_level > highest_loglevel) { highest_loglevel = Settings->mqttlog_level; }
#ifdef USE_WEBSERVER
  if (Settings->weblog_level > highest_loglevel) { highest_loglevel = Settings->weblog_level; }
#endif  // USE_WEBSERVER
#ifdef USE_UFILESYS
  uint32_t filelog_level = Settings->filelog_level % 10;
  if (filelog_level > highest_loglevel) { highest_loglevel = filelog_level; }
#endif  // USE_UFILESYS
  if (TasmotaGlobal.syslog_level > highest_loglevel) { highest_loglevel = TasmotaGlobal.syslog_level; }
  if (TasmotaGlobal.templog_level > highest_loglevel) { highest_loglevel = TasmotaGlobal.templog_level; }
  if (TasmotaGlobal.uptime < 3) { highest_loglevel = LOG_LEVEL_DEBUG_MORE; }  // Log all before setup correct log level
  return highest_loglevel;
}

void AddLogData(uint32_t loglevel, const char* log_data, const char* log_data_payload = nullptr, const char* log_data_retained = nullptr) {
  // Ignore any logging when maxlog_level = 0 OR logging for levels equal or lower than maxlog_level
  if (!TasmotaGlobal.maxlog_level || (loglevel > TasmotaGlobal.maxlog_level)) { return; }
  // Store log_data in buffer
  // To lower heap usage log_data_payload may contain the payload data from MqttPublishPayload()
  //  and log_data_retained may contain optional retained message from MqttPublishPayload()
#ifdef ESP32
  // this takes the mutex, and will be release when the class is destroyed -
  // i.e. when the functon leaves  You CAN call mutex.give() to leave early.
  TasAutoMutex mutex((SemaphoreHandle_t *)&TasmotaGlobal.log_buffer_mutex);
#endif  // ESP32

  char mxtime[21];  // "13:45:21.999-123/12 "
  snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d.%03d"),
    RtcTime.hour, RtcTime.minute, RtcTime.second, RtcMillis());
  if (Settings->flag5.show_heap_with_timestamp) {
#ifdef ESP8266
    snprintf_P(mxtime, sizeof(mxtime), PSTR("%s-%03d"),
      mxtime, ESP_getFreeHeap1024());
#else
    snprintf_P(mxtime, sizeof(mxtime), PSTR("%s-%03d/%02d"),
      mxtime, ESP_getFreeHeap1024(), ESP_getHeapFragmentation());
#endif
  }
  strcat(mxtime, " ");

  char empty[2] = { 0 };
  if (!log_data_payload) { log_data_payload = empty; }
  if (!log_data_retained) { log_data_retained = empty; }

  if ((loglevel <= TasmotaGlobal.seriallog_level) &&
      (TasmotaGlobal.masterlog_level <= TasmotaGlobal.seriallog_level)) {
    if (!Settings->mbflag2.json_pretty_print || !strchr(log_data_payload, '{')) {
      TasConsole.printf("%s%s%s%s\r\n", mxtime, log_data, log_data_payload, log_data_retained);
    }
  }

  if (!TasmotaGlobal.log_buffer) { return; }  // Leave now if there is no buffer available

  uint32_t highest_loglevel = HighestLogLevel();
  if ((loglevel <= highest_loglevel) &&    // Log only when needed
      (TasmotaGlobal.masterlog_level <= highest_loglevel)) {
    // Delimited, zero-terminated buffer of log lines.
    // Each entry has this format: [index][loglevel][log data]['\1']

    // Truncate log messages longer than MAX_LOGSZ which is the log buffer size minus 64 spare
    char *too_long = nullptr;
    uint32_t log_data_len = strlen(log_data) + strlen(log_data_payload) + strlen(log_data_retained);
    if (log_data_len > MAX_LOGSZ) {
      too_long = (char*)malloc(TOPSZ);     // Use heap in favour of stack
      snprintf_P(too_long, TOPSZ - 20, PSTR("%s%s"), log_data, log_data_payload);   // 20 = strlen("... 123456 truncated")
      snprintf_P(too_long, TOPSZ, PSTR("%s... %d truncated"), too_long, log_data_len);
      log_data = too_long;
      log_data_payload = empty;
      log_data_retained = empty;
    }
    log_data_len = strlen(mxtime) + strlen(log_data) + strlen(log_data_payload) + strlen(log_data_retained);

    TasmotaGlobal.log_buffer_pointer &= 0xFF;
    if (!TasmotaGlobal.log_buffer_pointer) {
      TasmotaGlobal.log_buffer_pointer++;  // Index 0 is not allowed as it is the end of char string
    }
    while (TasmotaGlobal.log_buffer_pointer == TasmotaGlobal.log_buffer[0] ||  // If log already holds the next index, remove it
           strlen(TasmotaGlobal.log_buffer) + log_data_len +4 > LOG_BUFFER_SIZE) {  // 4 = log_buffer_pointer + '\1' + '\0'
      char* it = TasmotaGlobal.log_buffer;
      it++;                                // Skip log_buffer_pointer
      it += strchrspn(it, '\1');           // Skip log line
      it++;                                // Skip delimiting "\1"
      memmove(TasmotaGlobal.log_buffer, it, LOG_BUFFER_SIZE -(it-TasmotaGlobal.log_buffer));  // Move buffer forward to remove oldest log line
    }
    char *log_line = TasmotaGlobal.log_buffer + strlen(TasmotaGlobal.log_buffer);  // Ponter to next entry
    snprintf_P(log_line, log_data_len +4, PSTR("%c%c%s%s%s%s\1"),
      TasmotaGlobal.log_buffer_pointer++, '0'+loglevel, mxtime, log_data, log_data_payload, log_data_retained);
    if (too_long) { free(too_long); }
    TasmotaGlobal.log_buffer_pointer &= 0xFF;
    if (!TasmotaGlobal.log_buffer_pointer) {
      TasmotaGlobal.log_buffer_pointer++;  // Index 0 is not allowed as it is the end of char string
    }

    // These calls fail to show initial logging
    log_line += 2;                         // Skip log_buffer_pointer and loglevel
    // 14:49:36.123 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}
    // 14:30:16.749-172/38 MQT: tele/atomlite3/INFO3 = {"Info3":{"RestartReason":"Vbat power on reset","BootCount":74}}

    if ((loglevel <= TasmotaGlobal.seriallog_level) &&
        (TasmotaGlobal.masterlog_level <= TasmotaGlobal.seriallog_level)) {
      LogDataJsonPrettyPrint(log_line, log_data_len, TasConsoleLDJsonPPCb);
    }

#ifdef USE_SERIAL_BRIDGE
    if (loglevel <= TasmotaGlobal.seriallog_level) {
      SerialBridgeWrite(log_line, log_data_len);
    }
#endif  // USE_SERIAL_BRIDGE
#ifdef USE_TELNET
#ifdef ESP32
    if (loglevel <= TasmotaGlobal.seriallog_level) {
      TelnetWrite(log_line, log_data_len);  // This uses too much heap on ESP8266
    }
#endif  // ESP32
#endif  // USE_TELNET

  }
}

void TasConsoleLDJsonPPCb(const char* line, uint32_t len) {
  TasConsole.println(line);
}

void AddLog(uint32_t loglevel, PGM_P formatP, ...) {
#ifdef ESP32
  if (xPortInIsrContext()) {
    // When called from an ISR, you should not send out logs.
    // Allocating memory from within an ISR is a big no-no.
    // Also long-time blocking like sending logs (especially to a syslog server) 
    // is also really not a good idea from an ISR call.
    return;
  }
#endif
  uint32_t highest_loglevel = HighestLogLevel();
  // If no logging is requested then do not access heap to fight fragmentation
  if ((loglevel <= highest_loglevel) && (TasmotaGlobal.masterlog_level <= highest_loglevel)) {
    va_list arg;
    va_start(arg, formatP);
    char* log_data = ext_vsnprintf_malloc_P(formatP, arg);
    va_end(arg);
    if (log_data == nullptr) { return; }

    AddLogData(loglevel, log_data);
    free(log_data);
  }
}

void AddLogBuffer(uint32_t loglevel, uint8_t *buffer, uint32_t count) {
  char hex_char[(count * 3) + 2];
  AddLog(loglevel, PSTR("DMP: %s"), ToHex_P(buffer, count, hex_char, sizeof(hex_char), ' '));
}

void AddLogSerial() {
  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t*)TasmotaGlobal.serial_in_buffer, TasmotaGlobal.serial_in_byte_counter);
}

void AddLogMissed(const char *sensor, uint32_t misses)
{
  AddLog(LOG_LEVEL_DEBUG, PSTR("SNS: %s missed %d"), sensor, SENSOR_MAX_MISS - misses);
}

/*********************************************************************************************\
 * HTML and URL encode
\*********************************************************************************************/

const char kUnescapeCode[] = "&><\"\'\\";
const char kEscapeCode[] PROGMEM = "&amp;|&gt;|&lt;|&quot;|&apos;|&#92;";

String HtmlEscape(const String unescaped) {
  char escaped[10];
  size_t ulen = unescaped.length();
  String result;
  result.reserve(ulen);          // pre-reserve the required space to avoid mutiple reallocations
  for (size_t i = 0; i < ulen; i++) {
    char c = unescaped[i];
    char *p = strchr(kUnescapeCode, c);
    if (p != nullptr) {
      result += GetTextIndexed(escaped, sizeof(escaped), p - kUnescapeCode, kEscapeCode);
    } else {
      result += c;
    }
  }
  return result;
}

String SettingsTextEscaped(uint32_t index) {
  return HtmlEscape(SettingsText(index));
}

String UrlEscape(const char *unescaped) {
  static const char *hex = "0123456789ABCDEF";
  String result;
  result.reserve(strlen(unescaped));

  while (*unescaped != '\0') {
    if (('a' <= *unescaped && *unescaped <= 'z') ||
        ('A' <= *unescaped && *unescaped <= 'Z') ||
        ('0' <= *unescaped && *unescaped <= '9') ||
        *unescaped == '-' || *unescaped == '_' || *unescaped == '.' || *unescaped == '~')
    {
      result += *unescaped;
    }
    else
    {
      result += '%';
      result += hex[*unescaped >> 4];
      result += hex[*unescaped & 0xf];
    }
    unescaped++;
  }
  return result;
}

/*********************************************************************************************\
 * Uncompress static PROGMEM strings
\*********************************************************************************************/

#ifdef USE_UNISHOX_COMPRESSION

#include <unishox.h>

Unishox compressor;

// New variant where you provide the String object yourself
int32_t DecompressNoAlloc(const char * compressed, size_t uncompressed_size, String & content) {
  uncompressed_size += 2;    // take a security margin

  // We use a nasty trick here. To avoid allocating twice the buffer,
  // we first extend the buffer of the String object to the target size (maybe overshooting by 7 bytes)
  // then we decompress in this buffer,
  // and finally assign the raw string to the String, which happens to work: String uses memmove(), so overlapping works
  content.reserve(uncompressed_size);
  char * buffer = content.begin();

  int32_t len = compressor.unishox_decompress(compressed, strlen_P(compressed), buffer, uncompressed_size);
  if (len > 0) {
    buffer[len] = 0;    // terminate string with NULL
    content = buffer;         // copy in place
  }
  return len;
}

String Decompress(const char * compressed, size_t uncompressed_size) {
  String content("");
  DecompressNoAlloc(compressed, uncompressed_size, content);
  return content;
}

#endif // USE_UNISHOX_COMPRESSION
