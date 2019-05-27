// Copyright 2016 Hisham Khalifa
// Copyright 2017 David Conran

#include <algorithm>
#include "IRsend.h"

//   GGG  L       OOOO  BBBB     AA   L       CCCC    AA    CCCC  H    H EEEEEE
//  G   G L      O    O B   B   AAAA  L      C    C  AAAA  C    C H    H E
// G      L      O    O BBBBB  A    A L      C      A    A C      HHHHHH EEEE
// G   GG L      O    O B   BB AAAAAA L      C    C AAAAAA C    C H    H E
//  GGGGG LLLLLL  OOOO  BBBBB  A    A LLLLLL  CCCC  A    A  CCCC  H    H EEEEEE

// Global Cache IR format sender originally added by Hisham Khalifa
//   (http://www.hishamkhalifa.com)

// Constants
const uint16_t kGlobalCacheMaxRepeat = 50;
const uint32_t kGlobalCacheMinUsec = 80;
const uint8_t kGlobalCacheFreqIndex = 0;
const uint8_t kGlobalCacheRptIndex = kGlobalCacheFreqIndex + 1;
const uint8_t kGlobalCacheRptStartIndex = kGlobalCacheRptIndex + 1;
const uint8_t kGlobalCacheStartIndex = kGlobalCacheRptStartIndex + 1;

#if SEND_GLOBALCACHE
// Send a shortened GlobalCache (GC) IRdb/control tower formatted message.
//
// Args:
//   buf: An array of uint16_t containing the shortened GlobalCache data.
//   len: Nr. of entries in the buf[] array.
//
// Status: STABLE / Known working.
//
// Note:
//   Global Cache format without the emitter ID or request ID.
//   Starts at the frequency (Hertz), followed by nr. of times to emit (count),
//   then the offset for repeats (where a repeat will start from),
//   then the rest of entries are the actual IR message as units of periodic
//   time.
//   e.g. sendir,1:1,1,38000,1,1,9,70,9,30,9,... -> 38000,1,1,9,70,9,30,9,...
// Ref:
//   https://irdb.globalcache.com/Home/Database
void IRsend::sendGC(uint16_t buf[], uint16_t len) {
  uint16_t hz = buf[kGlobalCacheFreqIndex];  // GC frequency is in Hz.
  enableIROut(hz);
  uint32_t periodic_time = calcUSecPeriod(hz, false);
  uint8_t emits =
      std::min(buf[kGlobalCacheRptIndex], (uint16_t)kGlobalCacheMaxRepeat);
  // Repeat
  for (uint8_t repeat = 0; repeat < emits; repeat++) {
    // First time through, start at the beginning (kGlobalCacheStartIndex),
    // otherwise for repeats, we start a specified offset from that.
    uint16_t offset = kGlobalCacheStartIndex;
    if (repeat) offset += buf[kGlobalCacheRptStartIndex] - 1;
    // Data
    for (; offset < len; offset++) {
      // Convert periodic units to microseconds.
      // Minimum is kGlobalCacheMinUsec for actual GC units.
      uint32_t microseconds =
          std::max(buf[offset] * periodic_time, kGlobalCacheMinUsec);
      // These codes start at an odd index (not even as with sendRaw).
      if (offset & 1)  // Odd bit.
        mark(microseconds);
      else  // Even bit.
        space(microseconds);
    }
  }
  // It's possible that we've ended on a mark(), thus ensure the LED is off.
  ledOff();
}
#endif
