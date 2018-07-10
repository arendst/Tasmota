// Copyright 2016 Hisham Khalifa
// Copyright 2017 David Conran

#include <algorithm>
#include "IRsend.h"
#include "IRtimer.h"

//   GGG  L       OOOO  BBBB     AA   L       CCCC    AA    CCCC  H    H EEEEEE
//  G   G L      O    O B   B   AAAA  L      C    C  AAAA  C    C H    H E
// G      L      O    O BBBBB  A    A L      C      A    A C      HHHHHH EEEE
// G   GG L      O    O B   BB AAAAAA L      C    C AAAAAA C    C H    H E
//  GGGGG LLLLLL  OOOO  BBBBB  A    A LLLLLL  CCCC  A    A  CCCC  H    H EEEEEE

// Global Cache IR format sender originally added by Hisham Khalifa
//   (http://www.hishamkhalifa.com)

// Constants
#define GLOBALCACHE_MAX_REPEAT                                   50U
#define GLOBALCACHE_MIN_USEC                                     80U
#define GLOBALCACHE_FREQ_INDEX                                    0U
#define GLOBALCACHE_RPT_INDEX            GLOBALCACHE_FREQ_INDEX + 1U
#define GLOBALCACHE_RPT_START_INDEX       GLOBALCACHE_RPT_INDEX + 1U
#define GLOBALCACHE_START_INDEX     GLOBALCACHE_RPT_START_INDEX + 1U

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
  uint16_t hz = buf[GLOBALCACHE_FREQ_INDEX];  // GC frequency is in Hz.
  enableIROut(hz);
  uint32_t periodic_time = calcUSecPeriod(hz, false);
  uint8_t emits = std::min(buf[GLOBALCACHE_RPT_INDEX],
                           (uint16_t) GLOBALCACHE_MAX_REPEAT);
  // Repeat
  for (uint8_t repeat = 0; repeat < emits; repeat++) {
    // First time through, start at the beginning (GLOBALCACHE_START_INDEX),
    // otherwise for repeats, we start a specified offset from that.
    uint16_t offset = GLOBALCACHE_START_INDEX;
    if (repeat)
      offset += buf[GLOBALCACHE_RPT_START_INDEX] - 1;
    // Data
    for (; offset < len; offset++) {
      // Convert periodic units to microseconds.
      // Minimum is GLOBALCACHE_MIN_USEC for actual GC units.
      uint32_t microseconds = std::max(buf[offset] * periodic_time,
                                       GLOBALCACHE_MIN_USEC);
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
