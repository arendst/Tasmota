/*
  support_crash_recorder.ino - record the call stack in RTC in cas of crash

  Copyright (C) 2019  Stephan Hadinger, Theo Arends, 

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

const uint32_t dump_max_len = 64;                   // dump only 64 call addresses

/**
 * Save crash information in RTC memory
 * This function is called automatically if ESP8266 suffers an exception
 * It should be kept quick / consise to be able to execute before hardware wdt may kick in
 */
extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end ) {
  uint32_t addr_written = 0;      // how many addresses have we already written in RTC
  uint32_t value;                 // 4 bytes buffer to write to RTC

  for (uint32_t i = stack; i < stack_end; i += 4) {
    value = *((uint32_t*) i);     // load value from stack
    if ((value >= 0x40000000) && (value < 0x40300000)) {  // keep only addresses in code area
      ESP.rtcUserMemoryWrite(addr_written, (uint32_t*)&value, sizeof(value));
      addr_written++;
      if (addr_written >= dump_max_len) { break; }        // we store only 64 addresses
    }
  }
  // fill the rest of RTC with zeros
  value = 0;
  while (addr_written < dump_max_len) {
    ESP.rtcUserMemoryWrite(addr_written++, (uint32_t*)&value, sizeof(value));
  }
}

// Generate a crash to test the crash recorder
void CmndCrash(void)
{
  volatile uint32_t dummy;
  dummy = *((uint32_t*) 0x00000000);
}

// Clear the RTC dump area when we do a normal reboot, this avoids garbage data to stay in RTC
void CrashDumpClear(void) {
  uint32_t value = 0;
  for (uint32_t i = 0; i < dump_max_len; i++) {
    ESP.rtcUserMemoryWrite(i, (uint32_t*)&value, sizeof(value));
  }
}

/*********************************************************************************************\
 * CmndCrashDump - dump the crash history - called by `Status 12`
\*********************************************************************************************/
void CrashDump(void)
{
  ResponseAppend_P(PSTR("{\"call_chain\":["));
  for (uint32_t i = 0; i < dump_max_len; i++) {
    uint32_t value;
    ESP.rtcUserMemoryRead(i, (uint32_t*)&value, sizeof(value));
    if ((value >= 0x40000000) && (value < 0x40300000)) {
      if (i > 0) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"%08x\""), value);
    }
  }
  ResponseAppend_P(PSTR("]}"));
}
