/*
  support_crash_recorder.ino - record the call stack in RTC in case of crash

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

const uint32_t crash_magic = 0x53415400;   // Stack trace magic number (TASx)
const uint32_t crash_rtc_offset = 32;      // Offset in RTC memory skipping OTA used block
const uint32_t crash_dump_max_len = 31;    // Dump only 31 call addresses to satisfy max JSON length of about 600 characters

/**
 * Save crash information in RTC memory
 * This function is called automatically if ESP8266 suffers an exception
 * It should be kept quick / consise to be able to execute before hardware wdt may kick in
 */
extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end )
{
  uint32_t addr_written = 0;      // how many addresses have we already written in RTC
  uint32_t value;                 // 4 bytes buffer to write to RTC

  for (uint32_t i = stack; i < stack_end; i += 4) {
    value = *((uint32_t*) i);     // load value from stack
    if ((value >= 0x40000000) && (value < 0x40300000)) {  // keep only addresses in code area
      ESP.rtcUserMemoryWrite(crash_rtc_offset + addr_written, (uint32_t*)&value, sizeof(value));
      addr_written++;
      if (addr_written >= crash_dump_max_len) { break; }  // we store only 31 addresses
    }
  }
  value = crash_magic + addr_written;
  ESP.rtcUserMemoryWrite(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
}

// Generate a crash to test the crash recorder
void CmndCrash(void)
{
  volatile uint32_t dummy;
  dummy = *((uint32_t*) 0x00000000);
}

// do an infinite loop to trigger WDT watchdog
void CmndWDT(void)
{
  volatile uint32_t dummy = 0;
  while (1) {
    dummy++;
  }
}

// Clear the RTC dump counter when we do a normal reboot, this avoids garbage data to stay in RTC
void CrashDumpClear(void)
{
  uint32_t value = 0;
  ESP.rtcUserMemoryWrite(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
}

/*********************************************************************************************\
 * CmndCrashDump - dump the crash history - called by `Status 12`
\*********************************************************************************************/

void CrashDump(void)
{
  ResponseAppend_P(PSTR("{\"Exception\":%d,\"Reason\":\"%s\",\"EPC\":[\"%08x\",\"%08x\",\"%08x\"],\"EXCVADDR\":\"%08x\",\"DEPC\":\"%08x\""),
    resetInfo.exccause,        // Exception Cause
    GetResetReason().c_str(),  // Reset Reason
    resetInfo.epc1,            // Exception Progam Counter
    resetInfo.epc2,            // Exception Progam Counter - High-Priority Interrupt 1
    resetInfo.epc3,            // Exception Progam Counter - High-Priority Interrupt 2
    resetInfo.excvaddr,        // Exception Virtual Address Register - Virtual address that caused last fetch, load, or store exception
    resetInfo.depc);           // Double Exception Program Counter

  uint32_t value;
  ESP.rtcUserMemoryRead(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
  if (crash_magic == (value & 0xFFFFFF00)) {
    ResponseAppend_P(PSTR(",\"CallChain\":["));
    uint32_t count = value & 0x3F;
    for (uint32_t i = 0; i < count; i++) {
      ESP.rtcUserMemoryRead(crash_rtc_offset +i, (uint32_t*)&value, sizeof(value));
      if (i > 0) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"%08x\""), value);
    }
    ResponseAppend_P(PSTR("]"));
  }

  ResponseJsonEnd();
}
