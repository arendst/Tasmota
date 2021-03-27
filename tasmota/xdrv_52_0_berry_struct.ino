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

#define BERRY_CONSOLE_CMD_DELIMITER   "\x01"

typedef LList_elt<char[0]> log_elt;   // store the string after the header to avoid double allocation if we had used char*

class BerryLog {
public:
  // typedef LList_elt<char[0]> log_elt;   // store the string after the header to avoid double allocation if we had used char*
  inline static size_t size(size_t chars) { return sizeof(log_elt) + chars; }
  inline bool isEmpty(void) const { return log.isEmpty(); }
  log_elt * addString(const char * s, const char * prefix = nullptr, const char * suffix = nullptr) {
    if (suffix == nullptr) { suffix = ""; }
    if (prefix == nullptr) { prefix = ""; }
    if (s == nullptr) { s = ""; }
    size_t s_len = strlen_P(s) + strlen_P(prefix) + strlen_P(suffix);
    if (0 == s_len) { return nullptr; }   // do nothing
    log_elt * elt = (log_elt*) ::operator new(sizeof(log_elt) + s_len + 1); // use low-level new to specify the bytes size
    snprintf_P((char*) &elt->val(), s_len+1, PSTR("%s%s%s"), prefix, s, suffix);
    log.addToLast(elt);
    return elt;
  }
  void reset(void) {
    log.reset();
  }
  LList<char[0]> log;
};

class BerrySupport {
public:
  bvm *vm = nullptr;                    // berry vm
  bool rules_busy = false;              // are we already processing rules, avoid infinite loop
  bool autoexec_done = false;           // do we still need to load 'autoexec.be'
  bool repl_active = false;             // is REPL running (activates log recording)
  // output log is stored as a LinkedList of buffers
  // and active only when a REPL command is running
  BerryLog log;
};
BerrySupport berry;


#endif  // USE_BERRY
