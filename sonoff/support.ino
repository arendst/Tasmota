/*
  support.ino - support for Sonoff-Tasmota

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

IPAddress syslog_host_addr;      // Syslog host IP address
uint32_t syslog_host_hash = 0;   // Syslog host name hash

/*********************************************************************************************\
 * Watchdog extension (https://github.com/esp8266/Arduino/issues/1532)
\*********************************************************************************************/

#include <Ticker.h>

Ticker tickerOSWatch;

#define OSWATCH_RESET_TIME 120

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
  unsigned long t = millis();
  unsigned long last_run = abs(t - oswatch_last_loop_time);

#ifdef DEBUG_THEO
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_OSWATCH " FreeRam %d, rssi %d, last_run %d"), ESP.getFreeHeap(), WifiGetRssiAsQuality(WiFi.RSSI()), last_run);
#endif  // DEBUG_THEO
  if (last_run >= (OSWATCH_RESET_TIME * 1000)) {
//    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_OSWATCH " " D_BLOCKED_LOOP ". " D_RESTARTING));  // Save iram space
    RtcSettings.oswatch_blocked_loop = 1;
    RtcSettingsSave();
//    ESP.restart();  // normal reboot
    ESP.reset();  // hard reset
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

String GetResetReason(void)
{
  char buff[32];
  if (oswatch_blocked_loop) {
    strncpy_P(buff, PSTR(D_JSON_BLOCKED_LOOP), sizeof(buff));
    return String(buff);
  } else {
    return ESP.getResetReason();
  }
}

bool OsWatchBlockedLoop(void)
{
  return oswatch_blocked_loop;
}
/*********************************************************************************************\
 * Miscellaneous
\*********************************************************************************************/

#ifdef ARDUINO_ESP8266_RELEASE_2_3_0
// Functions not available in 2.3.0

// http://clc-wiki.net/wiki/C_standard_library:string.h:memchr
void* memchr(const void* ptr, int value, size_t num)
{
  unsigned char *p = (unsigned char*)ptr;
  while (num--) {
    if (*p != (unsigned char)value) {
      p++;
    } else {
      return p;
    }
  }
  return 0;
}

// http://clc-wiki.net/wiki/C_standard_library:string.h:strspn
// Get span until any character in string
size_t strcspn(const char *str1, const char *str2)
{
  size_t ret = 0;
  while (*str1) {
    if (strchr(str2, *str1)) {  // Slow
      return ret;
    } else {
      str1++;
      ret++;
    }
  }
  return ret;
}
#endif  // ARDUINO_ESP8266_RELEASE_2_3_0

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
  char *sub = NULL;
  char *ptr;
  int i;

  // Since strtok consumes the first arg, make a copy
  strncpy(dest, str, strlen(str)+1);
  for (i = 1, act = dest; i <= index; i++, act = NULL) {
    sub = strtok_r(act, delim, &ptr);
    if (sub == NULL) break;
  }
  sub = Trim(sub);
  return sub;
}

double CharToDouble(const char *str)
{
  // simple ascii to double, because atof or strtod are too large
  char strbuf[24];

  strlcpy(strbuf, str, sizeof(strbuf));
  char *pt = strbuf;
  while ((*pt != '\0') && isblank(*pt)) { pt++; }  // Trim leading spaces

  signed char sign = 1;
  if (*pt == '-') { sign = -1; }
  if (*pt == '-' || *pt=='+') { pt++; }            // Skip any sign

  double left = 0;
  if (*pt != '.') {
    left = atoi(pt);                               // Get left part
    while (isdigit(*pt)) { pt++; }                 // Skip number
  }

  double right = 0;
  if (*pt == '.') {
    pt++;
    right = atoi(pt);                              // Decimal part
    while (isdigit(*pt)) {
      pt++;
      right /= 10.0;
    }
  }

  double result = left + right;
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
    strcpy(s, "null");
    return s;
  } else {
    return dtostrf(number, 1, prec, s);
  }
}

char* Unescape(char* buffer, uint16_t* size)
{
  uint8_t* read = (uint8_t*)buffer;
  uint8_t* write = (uint8_t*)buffer;
  int16_t start_size = *size;
  int16_t end_size = *size;
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

void SetShortcut(char* str, uint8_t action)
{
  if ('\0' != str[0]) {     // There must be at least one character in the buffer
    str[0] = '0' + action;  // SC_CLEAR, SC_DEFAULT, SC_USER
    str[1] = '\0';
  }
}

uint8_t Shortcut(const char* str)
{
  uint8_t result = 10;

  if ('\0' == str[1]) {    // Only allow single character input for shortcut
    if (('"' == str[0]) || ('0' == str[0])) {
      result = SC_CLEAR;
    } else {
      result = atoi(str);  // 1 = SC_DEFAULT, 2 = SC_USER
      if (0 == result) {
        result = 10;
      }
    }
  }
  return result;
}

bool ParseIp(uint32_t* addr, const char* str)
{
  uint8_t *part = (uint8_t*)addr;
  uint8_t i;

  *addr = 0;
  for (i = 0; i < 4; i++) {
    part[i] = strtoul(str, NULL, 10);        // Convert byte
    str = strchr(str, '.');
    if (str == NULL || *str == '\0') {
      break;  // No more separators, exit
    }
    str++;                                   // Point to next character after separator
  }
  return (3 == i);
}

void MakeValidMqtt(uint8_t option, char* str)
{
// option 0 = replace by underscore
// option 1 = delete character
  uint16_t i = 0;
  while (str[i] > 0) {
//        if ((str[i] == '/') || (str[i] == '+') || (str[i] == '#') || (str[i] == ' ')) {
    if ((str[i] == '+') || (str[i] == '#') || (str[i] == ' ')) {
      if (option) {
        uint16_t j = i;
        while (str[j] > 0) {
          str[j] = str[j +1];
          j++;
        }
        i--;
      } else {
        str[i] = '_';
      }
    }
    i++;
  }
}

// Function to parse & check if version_str is newer than our currently installed version.
bool NewerVersion(char* version_str)
{
  uint32_t version = 0;
  uint8_t i = 0;
  char *str_ptr;
  char* version_dup = strdup(version_str);  // Duplicate the version_str as strtok_r will modify it.

  if (!version_dup) {
    return false;  // Bail if we can't duplicate. Assume bad.
  }
  // Loop through the version string, splitting on '.' seperators.
  for (char *str = strtok_r(version_dup, ".", &str_ptr); str && i < sizeof(VERSION); str = strtok_r(NULL, ".", &str_ptr), i++) {
    int field = atoi(str);
    // The fields in a version string can only range from 0-255.
    if ((field < 0) || (field > 255)) {
      free(version_dup);
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
  free(version_dup);  // We no longer need this.
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

char* GetPowerDevice(char* dest, uint8_t idx, size_t size, uint8_t option)
{
  char sidx[8];

  strncpy_P(dest, S_RSLT_POWER, size);                // POWER
  if ((devices_present + option) > 1) {
    snprintf_P(sidx, sizeof(sidx), PSTR("%d"), idx);  // x
    strncat(dest, sidx, size - strlen(dest) -1);      // POWERx
  }
  return dest;
}

char* GetPowerDevice(char* dest, uint8_t idx, size_t size)
{
  return GetPowerDevice(dest, idx, size, 0);
}

float ConvertTemp(float c)
{
  float result = c;

  if (!isnan(c) && Settings.flag.temperature_conversion) {
    result = c * 1.8 + 32;  // Fahrenheit
  }
  return result;
}

char TempUnit(void)
{
  return (Settings.flag.temperature_conversion) ? 'F' : 'C';
}

float ConvertPressure(float p)
{
  float result = p;

  if (!isnan(p) && Settings.flag.pressure_conversion) {
    result = p * 0.75006375541921;  // mmHg
  }
  return result;
}

String PressureUnit(void)
{
  return (Settings.flag.pressure_conversion) ? String(D_UNIT_MILLIMETER_MERCURY) : String(D_UNIT_PRESSURE);
}

void SetGlobalValues(float temperature, float humidity)
{
  global_update = uptime;
  global_temperature = temperature;
  global_humidity = humidity;
}

void ResetGlobalValues(void)
{
  if ((uptime - global_update) > GLOBAL_VALUES_VALID) {  // Reset after 5 minutes
    global_update = 0;
    global_temperature = 0;
    global_humidity = 0;
  }
}

double FastPrecisePow(double a, double b)
{
  // https://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
  // calculate approximation with fraction of the exponent
  int e = (int)b;
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  // exponentiation by squaring with the exponent's integer part
  // double r = u.d makes everything much slower, not sure why
  double r = 1.0;
  while (e) {
    if (e & 1) {
      r *= a;
    }
    a *= a;
    e >>= 1;
  }
  return r * u.d;
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

char* GetTextIndexed(char* destination, size_t destination_size, uint16_t index, const char* haystack)
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

int GetStateNumber(char *state_text)
{
  char command[CMDSZ];
  int state_number = -1;

  if (GetCommandCode(command, sizeof(command), state_text, kOptionOff) >= 0) {
    state_number = 0;
  }
  else if (GetCommandCode(command, sizeof(command), state_text, kOptionOn) >= 0) {
    state_number = 1;
  }
  else if (GetCommandCode(command, sizeof(command), state_text, kOptionToggle) >= 0) {
    state_number = 2;
  }
  else if (GetCommandCode(command, sizeof(command), state_text, kOptionBlink) >= 0) {
    state_number = 3;
  }
  else if (GetCommandCode(command, sizeof(command), state_text, kOptionBlinkOff) >= 0) {
    state_number = 4;
  }
  return state_number;
}

void SetSerialBaudrate(int baudrate)
{
  Settings.baudrate = baudrate / 1200;
  if (Serial.baudRate() != baudrate) {
    if (seriallog_level) {
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SET_BAUDRATE_TO " %d"), baudrate);
    }
    delay(100);
    Serial.flush();
    Serial.begin(baudrate, serial_config);
    delay(10);
    Serial.println();
  }
}

void ClaimSerial(void)
{
  serial_local = true;
  AddLog_P(LOG_LEVEL_INFO, PSTR("SNS: Hardware Serial"));
  SetSeriallog(LOG_LEVEL_NONE);
  baudrate = Serial.baudRate();
  Settings.baudrate = baudrate / 1200;
}

void SerialSendRaw(char *codes)
{
  char *p;
  char stemp[3];
  uint8_t code;

  int size = strlen(codes);

  while (size > 0) {
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
  for (uint16_t i = 0; i <= size; i++) {
    hash += (uint8_t)*buffer++ * (i +1);
  }
  return hash;
}

void ShowSource(int source)
{
  if ((source > 0) && (source < SRC_MAX)) {
    char stemp1[20];
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SRC: %s"), GetTextIndexed(stemp1, sizeof(stemp1), source, kCommandSource));
  }
}

/*********************************************************************************************\
 * GPIO Module and Template management
\*********************************************************************************************/

uint8_t ModuleNr()
{
  // 0    = User module (255)
  // 1 up = Template module 0 up
  return (USER_MODULE == Settings.module) ? 0 : Settings.module +1;
}

String AnyModuleName(uint8_t index)
{
  if (USER_MODULE == index) {
    return String(Settings.user_template.name);
  } else {
    return FPSTR(kModules[index].name);
  }
}

String ModuleName()
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

  uint8_t j = 0;
  for (uint8_t i = 0; i < sizeof(mycfgio); i++) {
    if (6 == i) { j = 9; }
    if (8 == i) { j = 12; }
    dest[j] = src[i];
    j++;
  }
  // 11 85 00 85 85 00 00 00 00 00 00 00 15 38 85 00 00 81

//  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t *)gp, sizeof(myio));
}

gpio_flag ModuleFlag()
{
  gpio_flag flag;

  if (USER_MODULE == Settings.module) {
    flag = Settings.user_template.flag;
  } else {
    memcpy_P(&flag, &kModules[Settings.module].flag, sizeof(gpio_flag));
  }

  return flag;
}

void ModuleDefault(uint8_t module)
{
  if (USER_MODULE == module) { module = WEMOS; }  // Generic
  Settings.user_template_base = module;
  memcpy_P(&Settings.user_template, &kModules[module], sizeof(mytmplt));
}

void SetModuleType()
{
  my_module_type = (USER_MODULE == Settings.module) ? Settings.user_template_base : Settings.module;
}

uint8_t ValidPin(uint8_t pin, uint8_t gpio)
{
  uint8_t result = gpio;

  if (((pin > 5) && (pin < 9)) || (11 == pin)) {
    result = GPIO_NONE;  // Disable flash pins GPIO6, GPIO7, GPIO8 and GPIO11
  }
  if ((WEMOS == Settings.module) && (!Settings.flag3.user_esp8285_enable)) {
    if ((pin == 9) || (pin == 10)) { result = GPIO_NONE; }  // Disable possible flash GPIO9 and GPIO10
  }
  return result;
}

bool ValidGPIO(uint8_t pin, uint8_t gpio)
{
  return (GPIO_USER == ValidPin(pin, gpio));  // Only allow GPIO_USER pins
}

bool GetUsedInModule(uint8_t val, uint8_t *arr)
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

  for (uint8_t i = 0; i < MAX_GPIO_PIN; i++) {
    if (arr[i] == val) { return true; }
    if (arr[i] == val + offset) { return true; }
  }
  return false;
}

bool JsonTemplate(const char* dataBuf)
{
  StaticJsonBuffer<350> jb;  // 331 from https://arduinojson.org/v5/assistant/
  JsonObject& obj = jb.parseObject(dataBuf);
  if (!obj.success()) { return false; }

  // All parameters are optional allowing for partial changes
  const char* name = obj[D_JSON_NAME];
  if (name != nullptr) {
    strlcpy(Settings.user_template.name, name, sizeof(Settings.user_template.name));
  }
  if (obj[D_JSON_GPIO].success()) {
    for (uint8_t i = 0; i < sizeof(mycfgio); i++) {
      Settings.user_template.gp.io[i] = obj[D_JSON_GPIO][i] | 0;
    }
  }
  if (obj[D_JSON_FLAG].success()) {
    uint8_t flag = obj[D_JSON_FLAG] | 0;
    memcpy(&Settings.user_template.flag, &flag, sizeof(gpio_flag));
  }
  if (obj[D_JSON_BASE].success()) {
    uint8_t base = obj[D_JSON_BASE];
    if ((0 == base) || (base >= MAXMODULE)) { base = 17; } else { base--; }
    Settings.user_template_base = base;  // Default WEMOS
  }
  return true;
}

void TemplateJson()
{
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_NAME "\":\"%s\",\"" D_JSON_GPIO "\":["), Settings.user_template.name);
  for (uint8_t i = 0; i < sizeof(Settings.user_template.gp); i++) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s%d"), mqtt_data, (i>0)?",":"", Settings.user_template.gp.io[i]);
  }
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s],\"" D_JSON_FLAG "\":%d,\"" D_JSON_BASE "\":%d}"),
    mqtt_data, Settings.user_template.flag, Settings.user_template_base +1);
}

/*********************************************************************************************\
 * Sleep aware time scheduler functions borrowed from ESPEasy
\*********************************************************************************************/

long TimeDifference(unsigned long prev, unsigned long next)
{
  // Return the time difference as a signed value, taking into account the timers may overflow.
  // Returned timediff is between -24.9 days and +24.9 days.
  // Returned value is positive when "next" is after "prev"
  long signed_diff = 0;
  // To cast a value to a signed long, the difference may not exceed half 0xffffffffUL (= 4294967294)
  const unsigned long half_max_unsigned_long = 2147483647u;  // = 2^31 -1
  if (next >= prev) {
    const unsigned long diff = next - prev;
    if (diff <= half_max_unsigned_long) {                    // Normal situation, just return the difference.
      signed_diff = static_cast<long>(diff);                 // Difference is a positive value.
    } else {
      // prev has overflow, return a negative difference value
      signed_diff = static_cast<long>((0xffffffffUL - next) + prev + 1u);
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
      signed_diff = static_cast<long>((0xffffffffUL - prev) + next + 1u);
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
#define I2C_RETRY_COUNTER 3

uint32_t i2c_buffer = 0;

bool I2cValidRead(uint8_t addr, uint8_t reg, uint8_t size)
{
  uint8_t x = I2C_RETRY_COUNTER;

  i2c_buffer = 0;
  do {
    Wire.beginTransmission(addr);                       // start transmission to device
    Wire.write(reg);                                    // sends register address to read from
    if (0 == Wire.endTransmission(false)) {             // Try to become I2C Master, send data and collect bytes, keep master status for next request...
      Wire.requestFrom((int)addr, (int)size);           // send data n-bytes read
      if (Wire.available() == size) {
        for (uint8_t i = 0; i < size; i++) {
          i2c_buffer = i2c_buffer << 8 | Wire.read();   // receive DATA
        }
      }
    }
    x--;
  } while (Wire.endTransmission(true) != 0 && x != 0);  // end transmission
  return (x);
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

bool I2cDevice(uint8_t addr)
{
  for (uint8_t address = 1; address <= 127; address++) {
    Wire.beginTransmission(address);
    if (!Wire.endTransmission() && (address == addr)) {
      return true;
    }
  }
  return false;
}
#endif  // USE_I2C

/*********************************************************************************************\
 * Syslog
 *
 * Example:
 *   AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_LOG "Any value %d"), value);
 *
\*********************************************************************************************/

void SetSeriallog(uint8_t loglevel)
{
  Settings.seriallog_level = loglevel;
  seriallog_level = loglevel;
  seriallog_timer = 0;
}

#ifdef USE_WEBSERVER
void GetLog(uint8_t idx, char** entry_pp, size_t* len_p)
{
  char* entry_p = NULL;
  size_t len = 0;

  if (idx) {
    char* it = web_log;
    do {
      uint8_t cur_idx = *it;
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
  char syslog_preamble[64];  // Hostname + Id

  if (syslog_host_hash != GetHash(Settings.syslog_host, strlen(Settings.syslog_host))) {
    syslog_host_hash = GetHash(Settings.syslog_host, strlen(Settings.syslog_host));
    WiFi.hostByName(Settings.syslog_host, syslog_host_addr);  // If sleep enabled this might result in exception so try to do it once using hash
  }
  if (PortUdp.beginPacket(syslog_host_addr, Settings.syslog_port)) {
    snprintf_P(syslog_preamble, sizeof(syslog_preamble), PSTR("%s ESP-"), my_hostname);
    memmove(log_data + strlen(syslog_preamble), log_data, sizeof(log_data) - strlen(syslog_preamble));
    log_data[sizeof(log_data) -1] = '\0';
    memcpy(log_data, syslog_preamble, strlen(syslog_preamble));
    PortUdp.write(log_data);
    PortUdp.endPacket();
  } else {
    syslog_level = 0;
    syslog_timer = SYSLOG_TIMER;
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SYSLOG_HOST_NOT_FOUND ". " D_RETRY_IN " %d " D_UNIT_SECOND), SYSLOG_TIMER);
  }
}

void AddLog(uint8_t loglevel)
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
    if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
  }
#endif  // USE_WEBSERVER
  if (!global_state.wifi_down && (loglevel <= syslog_level)) { Syslog(); }
}

void AddLog_P(uint8_t loglevel, const char *formatP)
{
  snprintf_P(log_data, sizeof(log_data), formatP);
  AddLog(loglevel);
}

void AddLog_P(uint8_t loglevel, const char *formatP, const char *formatP2)
{
  char message[100];

  snprintf_P(log_data, sizeof(log_data), formatP);
  snprintf_P(message, sizeof(message), formatP2);
  strncat(log_data, message, sizeof(log_data) - strlen(log_data) -1);
  AddLog(loglevel);
}

void AddLog_P2(uint8_t loglevel, PGM_P formatP, ...)
{
  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
  va_end(arg);

  AddLog(loglevel);
}

void AddLogBuffer(uint8_t loglevel, uint8_t *buffer, int count)
{
  snprintf_P(log_data, sizeof(log_data), PSTR("DMP:"));
  for (int i = 0; i < count; i++) {
    snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, *(buffer++));
  }
  AddLog(loglevel);
}

void AddLogSerial(uint8_t loglevel)
{
  AddLogBuffer(loglevel, (uint8_t*)serial_in_buffer, serial_in_byte_counter);
}

void AddLogMissed(char *sensor, uint8_t misses)
{
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SNS: %s missed %d"), sensor, SENSOR_MAX_MISS - misses);
}
