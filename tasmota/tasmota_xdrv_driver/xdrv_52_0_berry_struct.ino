/*
  xdrv_52_0_berry_struct.ino - Berry scripting language, native fucnctions

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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


#ifdef USE_BERRY

#include <berry.h>
#include <LList.h>

#include "be_mapping.h"
#include "re1.5.h"

#ifdef USE_BERRY_INT64
  #include "be_int64.h"
#endif

/*********************************************************************************************\
 * Logging for Tasmota Berry console
 *
 * We need to declare the the log class first since it is used in structure
\*********************************************************************************************/
#define BERRY_CONSOLE_CMD_DELIMITER   "\x01"

class Log_line {
public:
  Log_line() : log_line(nullptr) {}
  ~Log_line() {
    if (log_line != nullptr) {
      berry_free(log_line);
    }
  }
  char * getBuffer() { return log_line; }
  char * allocate(size_t size) {
    if (log_line != nullptr) {
      berry_free(log_line);
    }
    log_line = (char*) berry_malloc(size);
    return log_line;
  }

  char * log_line;
};

// typedef LList_elt<Log_line> log_elt;   // store the string after the header to avoid double allocation if we had used char*


class BerryLog {
public:
  inline bool isEmpty(void) const { return log.isEmpty(); }
  LList_elt<Log_line> * addString(const char * s, const char * prefix = nullptr, const char * suffix = nullptr) {
    if (suffix == nullptr) { suffix = ""; }
    if (prefix == nullptr) { prefix = ""; }
    if (s == nullptr) { s = ""; }
    size_t s_len = strlen_P(s) + strlen_P(prefix) + strlen_P(suffix);
    if (0 == s_len) { return nullptr; }   // do nothing
    LList_elt<Log_line> * log_elt = new LList_elt<Log_line>();
    log_elt->val().allocate(s_len + 1);
    snprintf_P(log_elt->val().getBuffer(), s_len+1, PSTR("%s%s%s"), prefix, s, suffix);
    log.addToLast(log_elt);
    return log_elt;
  }
  void reset(void) {
    log.reset();
  }
  LList<Log_line> log;
};

/*********************************************************************************************\
 * Berry global structure
 *
\*********************************************************************************************/
class BerryLog;

class BerrySupport {
public:
  bvm *vm = nullptr;                    // berry vm
  int32_t timeout = 0;                  // Berry heartbeat timeout, preventing code to run for too long. `0` means not enabled
  bool rules_busy = false;              // are we already processing rules, avoid infinite loop
  bool web_add_handler_done = false;    // did we already sent `web_add_handler` event
  bool autoexec_done = false;           // do we still need to load 'autoexec.be'
  bool repl_active = false;             // is REPL running (activates log recording)
  // output log is stored as a LinkedList of buffers
  // and active only when a REPL command is running
  BerryLog log;
};
BerrySupport berry;

// multi-purpose serial logging
extern "C" void serial_debug(const char * berry_buf, ...);

#endif  // USE_BERRY
