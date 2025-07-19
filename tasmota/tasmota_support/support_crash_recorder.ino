/*
  support_crash_recorder.ino - record the call stack in RTC in case of crash

  Copyright (C) 2021  Stephan Hadinger, Theo Arends,

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

#ifndef FIRMWARE_MINIMAL

// Generate a crash to test the crash recorder
void CmndCrash(void)
{
  volatile uint32_t dummy;
  dummy = *((uint32_t*) 0x00000000);
  (void)dummy;
}

// Do an infinite loop to trigger WDT watchdog
void CmndWDT(void)
{
  volatile uint32_t dummy = 0;
  while (1) {
    dummy = dummy + 1;
  }
}

// This will trigger the os watch after OSWATCH_RESET_TIME (=120) seconds
// or normal WDT on ESP32
void CmndBlockedLoop(void)
{
  while (1) {
#ifdef ESP32
    delay(10000);   // 10s on ESP32 so that the normal WDT fires after 5s. There is no OSWATCH_RESET_TIME on ESP32
#else
    delay(1000);
#endif
  }
}

#ifdef ESP8266

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

// Clear the RTC dump counter when we do a normal reboot, this avoids garbage data to stay in RTC
void CrashDumpClear(void)
{
  uint32_t value = 0;
  ESP.rtcUserMemoryWrite(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
}

/*********************************************************************************************\
 * CmndCrashDump - dump the crash history - called by `Status 12`
\*********************************************************************************************/

bool CrashFlag(void)
{
  return ((ResetReason() == REASON_EXCEPTION_RST) || (ResetReason() == REASON_SOFT_WDT_RST) || oswatch_blocked_loop);
}

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

#endif  // ESP8266

#ifdef ESP32

const uint32_t crash_magic = 0x53415400;   // Stack trace magic number (TASx)
const uint32_t crash_dump_max_len = 48;    // Dump only 48 call addresses

// RTC_NOINIT_ATTR: store in RTC slow memory that survices a boot/crash
// needs to be volatile and have no intializer
RTC_NOINIT_ATTR volatile struct {
  uint32_t magic;
  uint32_t stack[crash_dump_max_len];
  uint32_t pc;
  uint32_t exccause;
  uint32_t excvaddr;
} crash_recorder;

bool CrashFlag(void)
{
  return crash_recorder.magic == crash_magic;
}

// Clear the RTC dump counter when we do a normal reboot, this avoids garbage data to stay in RTC
void CrashDumpClear(void)
{
  crash_recorder.magic = 0;
  for (uint32_t i=0; i<crash_dump_max_len; i++) {
    crash_recorder.stack[i] = 0;
  }
}

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
/**
 * Save crash information in RTC memory
 * This function is called automatically if ESP8266 suffers an exception
 * It should be kept quick / consise to be able to execute before hardware wdt may kick in
 * 
 * https://github.com/espressif/esp-idf/blob/dd491ee8513689191421943b737a31c159c91d17/components/esp_system/panic.c#L217
 * https://github.com/espressif/esp-idf/blob/8131d6f46d690e9cb60cc5cd457863cc5479351f/components/esp_system/port/panic_handler.c
 * https://github.com/espressif/esp-idf/issues/5163#issuecomment-617666293
 * https://github.com/memfault/memfault-firmware-sdk/blob/70f2d4ed6884d907d4b39da99434b762209ba568/ports/esp_idf/memfault/CMakeLists.txt#L79
 * 
 */
// extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end )
// esp_err_t IRAM_ATTR esp_backtrace_print(int depth)

#if defined __has_include && __has_include("xtensa_api.h")
#include "xtensa_api.h"
#else
#include "freertos/xtensa_api.h"
#endif
#include "esp_debug_helpers.h"
#include "esp_cpu_utils.h"
extern "C" {
  // esp-idf 3.x
  void __real_panicHandler(XtExcFrame *frame);
  void __real_xt_unhandled_exception(XtExcFrame *frame);
}

extern "C" IRAM_ATTR void custom_crash_recorder(XtExcFrame *exc_frame) {
  crash_recorder.magic = crash_magic;   // crash occured
  crash_recorder.pc = exc_frame->pc;
  crash_recorder.exccause = exc_frame->exccause;
  crash_recorder.excvaddr = exc_frame->excvaddr;
  for (uint32_t i=0; i<crash_dump_max_len; i++) {
    crash_recorder.stack[i] = 0;
  }

  uint32_t idx = 0;   // slot in stack trace

  // code copied from doBacktrace()
  //Initialize stk_frame with first frame of stack
  // esp_backtrace_frame_t stk_frame = {.pc = exc_frame->pc, .sp = exc_frame->a1, .next_pc = exc_frame->a0};
  esp_backtrace_frame_t stk_frame;
  stk_frame.pc = exc_frame->pc;
  stk_frame.sp = exc_frame->a1;
  stk_frame.next_pc = exc_frame->a0;
  crash_recorder.stack[idx++] = esp_cpu_process_stack_pc(stk_frame.pc);

  static const uint32_t depth = 100;
  //Check if first frame is valid
  bool corrupted = (esp_stack_ptr_is_sane(stk_frame.sp) &&
                    esp_ptr_executable((void*)esp_cpu_process_stack_pc(stk_frame.pc))) ?
                    false : true;
  uint32_t i = ((depth <= 0) ? INT32_MAX : depth) - 1;    //Account for stack frame that's already printed
  while (i-- > 0 && stk_frame.next_pc != 0 && !corrupted) {
      if (!esp_backtrace_get_next_frame(&stk_frame)) {    //Get next stack frame
          corrupted = true;
      }
      crash_recorder.stack[idx++] = esp_cpu_process_stack_pc(stk_frame.pc);
      if (idx >= crash_dump_max_len) break;   // no more slots
  }
}

extern "C" IRAM_ATTR void __wrap_panicHandler(XtExcFrame *frame) {
  custom_crash_recorder(frame);
  __real_panicHandler(frame);  // call the actual panic handler
}

extern "C" IRAM_ATTR void __wrap_xt_unhandled_exception(XtExcFrame *frame) {
  crash_recorder.magic = crash_magic;   // crash occured
  custom_crash_recorder(frame);
  __real_xt_unhandled_exception(frame);  // call the actual panic handler
}

// taken in panic.c, but orginal array is 'static' so can't be called
static const char *edesc[] = {
    "IllegalInstruction", "Syscall", "InstructionFetchError", "LoadStoreError",
    "Level1Interrupt", "Alloca", "IntegerDivideByZero", "PCValue",
    "Privileged", "LoadStoreAlignment", "res", "res",
    "InstrPDAddrError", "LoadStorePIFDataError", "InstrPIFAddrError", "LoadStorePIFAddrError",
    "InstTLBMiss", "InstTLBMultiHit", "InstFetchPrivilege", "res",
    "InstrFetchProhibited", "res", "res", "res",
    "LoadStoreTLBMiss", "LoadStoreTLBMultihit", "LoadStorePrivilege", "res",
    "LoadProhibited", "StoreProhibited", "res", "res",
    "Cp0Dis", "Cp1Dis", "Cp2Dis", "Cp3Dis",
    "Cp4Dis", "Cp5Dis", "Cp6Dis", "Cp7Dis"
};
#define NUM_EDESCS (sizeof(edesc) / sizeof(char *))

void CrashDump(void)
{
  if (crash_recorder.magic == crash_magic) {
    ResponseAppend_P("{\"Exception\":%d,\"Reason\":\"%s\",\"EPC\":\"%08x\",\"EXCVADDR\":\"%08x\"",
      crash_recorder.exccause,    // Exception Cause
      crash_recorder.exccause < NUM_EDESCS ? edesc[crash_recorder.exccause] : "Unknown",
      crash_recorder.pc,          // Exception Progam Counter
      crash_recorder.excvaddr     // Exception Virtual Address Register - Virtual address that caused last fetch, load, or store exception
    );
    // crash dump present
    ResponseAppend_P(PSTR(",\"CallChain\":["));
    for (uint32_t i = 0; i < crash_dump_max_len; i++) {
      uint32_t return_addr = crash_recorder.stack[i];
      if (!return_addr) { break; }
      if (i > 0) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"%08x\""), return_addr);
    }
    ResponseAppend_P(PSTR("]"));
  }
  ResponseJsonEnd();
}
#elif CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6

extern "C" {
  // esp-idf 3.x
  void __real_panicHandler(void *frame);
  void __real_xt_unhandled_exception(void *frame);
}

// from panic_arch.c
const char *esp32c3_crash_reason[] = {
    "Instruction address misaligned",
    "Instruction access fault",
    "Illegal instruction",
    "Breakpoint",
    "Load address misaligned",
    "Load access fault",
    "Store address misaligned",
    "Store access fault",
    "Environment call from U-mode",
    "Environment call from S-mode",
    "",
    "Environment call from M-mode",
    "Instruction page fault",
    "Load page fault",
    "",
    "Store page fault",
};
#define NUM_C3_REASONS (sizeof(esp32c3_crash_reason) / sizeof(char *))

#include <riscv/rvruntime-frames.h>
extern "C" IRAM_ATTR void custom_crash_recorder(void *exc_frame) {
  RvExcFrame *regs = (RvExcFrame *)exc_frame;

  crash_recorder.magic = crash_magic;   // crash occured
  crash_recorder.pc = regs->mepc;
  crash_recorder.exccause = regs->mcause;
  crash_recorder.excvaddr = regs->mtval;
  for (uint32_t i=0; i<crash_dump_max_len; i++) {
    crash_recorder.stack[i] = 0;
  }

  uint32_t idx = 0;   // slot in stack trace
  crash_recorder.stack[idx++] = regs->ra;   // push return address as first value

  // // code copied from panic_print_basic_backtrace()
  uint32_t * sp = (uint32_t*) regs->sp;
  uint32_t i = 0;
  for (uint32_t i = 0; ((uint32_t) sp) < 0x3FCDFFF0 && i < 320 && idx < crash_dump_max_len; i++, sp++) {
    uint32_t value = *sp;
    if ((value >= 0x40000000) && (value < 0x42800000)) {  // keep only addresses in code area
      crash_recorder.stack[idx++] = value;
    }
  }
}

extern "C" IRAM_ATTR void __wrap_panicHandler(void *frame) {
  custom_crash_recorder(frame);
  __real_panicHandler(frame);  // call the actual panic handler
}

extern "C" IRAM_ATTR void __wrap_xt_unhandled_exception(void *frame) {
  crash_recorder.magic = crash_magic;   // crash occured
  custom_crash_recorder(frame);
  __real_xt_unhandled_exception(frame);  // call the actual panic handler
}

void CrashDump(void)
{
  if (crash_recorder.magic == crash_magic) {
    ResponseAppend_P("{\"Exception\":%d,\"Reason\":\"%s\",\"EPC\":\"%08x\",\"EXCVADDR\":\"%08x\"",
      crash_recorder.exccause,    // Exception Cause
      crash_recorder.exccause < NUM_C3_REASONS ? esp32c3_crash_reason[crash_recorder.exccause] : "Unknown",
      crash_recorder.pc,          // Exception Progam Counter
      crash_recorder.excvaddr     // Exception Virtual Address Register - Virtual address that caused last fetch, load, or store exception
    );
    // crash dump present
    ResponseAppend_P(PSTR(",\"CallChain\":["));
    for (uint32_t i = 0; i < crash_dump_max_len; i++) {
      uint32_t return_addr = crash_recorder.stack[i];
      if (!return_addr) { break; }
      if (i > 0) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"%08x\""), return_addr);
    }
    ResponseAppend_P(PSTR("]"));
  }
  ResponseJsonEnd();
}
#else // unknown

bool CrashFlag(void)
{
  return false;
}

// Clear the RTC dump counter when we do a normal reboot, this avoids garbage data to stay in RTC
void CrashDumpClear(void)
{
}

void CrashDump(void)
{
}

#endif
#endif

#endif  //  FIRMWARE_MINIMAL