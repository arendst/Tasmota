/*
  support_heap_map.ino - ESP32 heap map (debug) support for Tasmota

  Copyright (C) 2026  Stephan Hadinger

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

#if defined(ESP32) && defined(USE_HEAP_MAP)
/*********************************************************************************************\
 * Heap map (debug)
 *
 * USE_HEAP_MAP          log one table row per heap at boot stages (macro HEAP_MAP):
 *                       largest free block, minimum free since boot, used blocks with delta against the previous row,
 *                       used bytes with delta against the line above, free, and a used bar
 *                       After boot, a row "boot" at end of boot (uptime 10s), then a row every HEAP_MAP_PERIOD
 *                       seconds (default 30, 0 = off). The table header is repeated every HEAP_MAP_HEADER_ROWS rows.
 *                       Berry: tasmota.heapmap("label") logs a row with this label.
 *                       Each driver call that changes the internal heap by HEAP_MAP_CALL_MIN bytes is logged (see below).
 *                       With HEAP_MAP_EVENTS_BOOT_ONLY (default 1) stage rows and driver call lines stop after the "boot" row,
 *                       only the periodic rows (and Berry tasmota.heapmap()) remain.
 *                       Only these driver events are logged: PRE_INIT, INIT, MODULE_INIT, MQTT_INIT, WEB_ADD_HANDLER,
 *                       NETWORK_UP and the display DISPLAY_INIT_DRIVER, DISPLAY_INIT (see kHeapMapFuncCode).
 *
 * The internal row walks MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT heaps only (all caps must match):
 * - ESP32 IRAM-only heaps (EXEC | 32BIT, used by Berry with USE_BERRY_IRAM) have no 8BIT and are excluded
 * - PSRAM has no INTERNAL and is excluded (not reported at all)
 * - RTC fast memory is included where the IDF adds it as general heap (e.g. C3/S3, 8 KB)
 * On chips with unified D/IRAM (C3, S3, ...), 32-bit allocations come from the same memory
 * as everything else and are counted, as they do reduce general heap.
 *
 * heap_caps_walk() calls the walker with the heap lock held (critical section), so the
 * the walker only fills a local structure. Logging is done after the walk.
\*********************************************************************************************/

#define HEAP_MAP_BAR_WIDTH  12
#define HEAP_MAP_HEADER_ROWS  20             // repeat the table header after this many rows

typedef struct {
  uint32_t used_blocks;
  uint32_t largest_free;
} HeapMapSummary_t;

// Heap map state, zero at boot
struct {
  // Internal heap, for deltas
  uint32_t last_free;             // free bytes after the last HMP line (table row or driver call line), deltas are since the line above
  uint32_t last_blocks;           // used blocks at the last table row (driver call lines have no block count)
  bool     has_last;              // last_free and last_blocks are set
  uint32_t rows;                  // table rows since the last header, 0 = no header yet
  bool     network_up;            // network rows logged for the current network up (support_tasmota.ino)
  bool     boot_done;             // row "boot" logged (support_tasmota.ino), with HEAP_MAP_EVENTS_BOOT_ONLY stages and driver calls stop
#if HEAP_MAP_CALL_MIN > 0
  // Driver call tracking, internal heap only (see HeapMapCallEnd)
  int32_t  call_nested;           // heap reported by lines printed inside the current call
  uint32_t call_depth;            // nesting of driver calls, 0 = not in a driver call
  bool     call_logging;          // avoid logging from calls made by the logger
  int32_t  other;                 // heap used outside logged driver calls, not reported yet
  uint32_t mark;                  // free at the end of the last top level call or table row
  bool     mark_valid;            // mark is set
#endif  // HEAP_MAP_CALL_MIN > 0
} HeapMapGlobal;

// Bytes as KB with one decimal, e.g. "35.9K"
static void HeapMapKB(char *buf, size_t size, uint32_t bytes) {
  uint32_t k10 = (uint32_t)(((uint64_t)bytes * 10 + 512) / 1024);
  snprintf_P(buf, size, PSTR("%u.%uK"), (unsigned int)(k10 / 10), (unsigned int)(k10 % 10));
}

// Signed delta in KB with one decimal (e.g. "+42.5K", "+0.0K" up to 51 bytes), "0" only if exactly zero
static void HeapMapDeltaKB(char *buf, size_t size, int32_t delta) {
  if (0 == delta) {
    snprintf_P(buf, size, PSTR("0"));
    return;
  }
  uint32_t abs_delta = (delta < 0) ? (uint32_t)(-(int64_t)delta) : (uint32_t)delta;
  uint32_t k10 = (uint32_t)(((uint64_t)abs_delta * 10 + 512) / 1024);
  snprintf_P(buf, size, PSTR("%c%u.%uK"), (delta < 0) ? '-' : '+', (unsigned int)(k10 / 10), (unsigned int)(k10 % 10));
}

// Bar and percentage of the heap that is not free, e.g. "[####........]  32%"
// (not free = used blocks and block headers, the same for table rows and driver call lines)
static void HeapMapBar(char *buf, size_t size, uint32_t free_bytes, uint32_t total) {
  uint32_t not_free = (total > free_bytes) ? total - free_bytes : 0;
  uint32_t pct = total ? (uint32_t)(((uint64_t)not_free * 100 + total / 2) / total) : 0;
  uint32_t fill = total ? (uint32_t)(((uint64_t)not_free * HEAP_MAP_BAR_WIDTH + total / 2) / total) : 0;
  char bar[HEAP_MAP_BAR_WIDTH + 1];
  for (uint32_t i = 0; i < HEAP_MAP_BAR_WIDTH; i++) { bar[i] = (i < fill) ? '#' : '.'; }
  bar[HEAP_MAP_BAR_WIDTH] = 0;
  snprintf_P(buf, size, PSTR("[%s] %3u%%"), bar, (unsigned int)pct);
}

// Row layout: the less useful columns come first, so that the longer label of a driver call line
// (driver and function, up to 43 chars) covers them, and its used, delta and free align with the table:
//   HMP: stage           largest     min  blks delta    used   delta    free  heap used
//   HMP: network_connect  112.9K  187.7K   400  +243   88.6K  +40.2K  189.7K  [####........]  32%
//   HMP: xdrv_52  WEB_ADD_HANDLER                      89.9K   +1.2K  188.5K  [####........]  33%
//   HMP: network_started  112.9K  187.7K   411   +11   90.2K   +0.3K  188.1K  [####........]  33%
// used = heap size - free (used blocks and block headers), free is the heap free counter, for all lines
static void HeapMapHeader(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("HMP: %-15s %7s %7s %5s %5s %7s %7s %7s  heap used"),
    PSTR("stage"), PSTR("largest"), PSTR("min"), PSTR("blks"), PSTR("delta"), PSTR("used"), PSTR("delta"), PSTR("free"));
  HeapMapGlobal.rows = 0;
}

static void HeapMapSummary(const char *stage) {
  const uint32_t caps = MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT;
  HeapMapSummary_t s;
  memset(&s, 0, sizeof(s));
  heap_caps_walk(caps, [](walker_heap_into_t heap_info, walker_block_info_t block_info, void *user_data) -> bool {
    HeapMapSummary_t *sum = (HeapMapSummary_t*)user_data;
    if (block_info.used) {
      sum->used_blocks++;
    } else if (block_info.size > sum->largest_free) {
      sum->largest_free = block_info.size;
    }
    return true;
  }, &s);
  uint32_t free_bytes = heap_caps_get_free_size(caps);   // same counter as driver call lines
  uint32_t total = heap_caps_get_total_size(caps);
  uint32_t used_bytes = (total > free_bytes) ? total - free_bytes : 0;

  char used[12], free_str[12], largest[12], min_str[12];
  HeapMapKB(used, sizeof(used), used_bytes);
  HeapMapKB(free_str, sizeof(free_str), free_bytes);
  HeapMapKB(largest, sizeof(largest), s.largest_free);
  HeapMapKB(min_str, sizeof(min_str), heap_caps_get_minimum_free_size(caps));

  char d_used[14] = "";                     // empty on the first row
  char d_blocks[8] = "";
  if (HeapMapGlobal.has_last) {
    HeapMapDeltaKB(d_used, sizeof(d_used), (int32_t)(HeapMapGlobal.last_free - free_bytes));
    int32_t db = (int32_t)(s.used_blocks - HeapMapGlobal.last_blocks);
    snprintf_P(d_blocks, sizeof(d_blocks), db ? PSTR("%+d") : PSTR("0"), (int)db);
  }

  char bar[HEAP_MAP_BAR_WIDTH + 8];
  HeapMapBar(bar, sizeof(bar), free_bytes, total);

  AddLog(LOG_LEVEL_DEBUG, PSTR("HMP: %-15.15s %7s %7s %5u %5s %7s %7s %7s  %s"),
    stage, largest, min_str, (unsigned int)s.used_blocks, d_blocks, used, d_used, free_str, bar);

  uint32_t free_logged = heap_caps_get_free_size(caps);  // after the log line, so its cost is not in the next delta
#if HEAP_MAP_CALL_MIN > 0
  // this row reports everything since the line above, including "other"
  if (HeapMapGlobal.call_depth > 0) {          // row printed inside a driver call (e.g. Berry): don't report it twice
    if (HeapMapGlobal.has_last) {
      HeapMapGlobal.call_nested += (int32_t)(HeapMapGlobal.last_free - free_logged) - HeapMapGlobal.other;
    }
  } else {
    HeapMapGlobal.mark = free_logged;
    HeapMapGlobal.mark_valid = true;
  }
  HeapMapGlobal.other = 0;
#endif  // HEAP_MAP_CALL_MIN > 0
  HeapMapGlobal.last_free = free_logged;
  HeapMapGlobal.last_blocks = s.used_blocks;
  HeapMapGlobal.has_last = true;
}

// Stage row (macro HEAP_MAP), not logged after boot if HEAP_MAP_EVENTS_BOOT_ONLY
void HeapMapEvent(const char *stage) {
#if HEAP_MAP_EVENTS_BOOT_ONLY
  if (HeapMapGlobal.boot_done) { return; }
#endif  // HEAP_MAP_EVENTS_BOOT_ONLY
  HeapMap(stage, false);
}

// header: force the table header (Berry); otherwise it is printed every HEAP_MAP_HEADER_ROWS rows
void HeapMap(const char *stage, bool header) {
  if (header || (0 == HeapMapGlobal.rows) || (HeapMapGlobal.rows >= HEAP_MAP_HEADER_ROWS)) { HeapMapHeader(); }
  HeapMapGlobal.rows++;
  HeapMapSummary(stage);
}

#if HEAP_MAP_CALL_MIN > 0
/*********************************************************************************************\
 * Heap change per driver call (macro HEAP_MAP_CALL in the xdrv/xsns/xlgt/xnrg/xdsp dispatchers)
 *
 * Logs `HMP: xdrv_52  EVERY_50_MSECOND   50.1K   +1.2K   45.6K  [######......]  50%` (used, delta, free, bar) when a single driver call changes the
 * internal heap (MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT, PSRAM excluded) by at least HEAP_MAP_CALL_MIN bytes.
 * + means the driver now holds more memory, - means it released memory.
 *
 * Nested driver calls (e.g. a rule or Berry running a command) are logged on their own and
 * excluded from the caller, as is the cost of the log line itself.
 * Heap used outside logged calls (core code, other tasks, calls below the threshold) is summed and shown
 * as an `other` line before the next driver call line (if at least HEAP_MAP_CALL_MIN), or included in the
 * delta of the next table row. So the deltas add up from line to line.
 * The driver number is the one the driver reports: TasmotaGlobal.light_driver / energy_driver once
 * claimed, Settings->display_model for the display driver that answers FUNC_DISPLAY_MODEL. Drivers that did
 * not claim the device are shown by position in the driver list, e.g. `xlgt#3`.
 * The heap free counter is global: allocations by other tasks (WiFi, LwIP, ...) during the call
 * are attributed to the driver. A + and - pair of the same size on unrelated calls is usually such noise.
\*********************************************************************************************/

// Tracked events, all other driver calls (LOOP, EVERY_xx_MSECOND, JSON_APPEND, COMMAND, ...) are not logged.
// Codes and names are in the same order
const uint8_t kHeapMapFuncCode[] PROGMEM = {
  FUNC_PRE_INIT, FUNC_INIT, FUNC_MODULE_INIT, FUNC_MQTT_INIT, FUNC_WEB_ADD_HANDLER, FUNC_NETWORK_UP };
const char kHeapMapFuncName[] PROGMEM =
  "PRE_INIT|INIT|MODULE_INIT|MQTT_INIT|WEB_ADD_HANDLER|NETWORK_UP";
// Display events are the first of enum XdspFunctions (xdrv_13_display.ino): FUNC_DISPLAY_INIT_DRIVER, FUNC_DISPLAY_INIT
const char kHeapMapFuncDisplay[] PROGMEM =
  "DISPLAY_INIT_DRIVER|DISPLAY_INIT";
#define HEAP_MAP_DISPLAY_TRACKED  2

// Index of a tracked event in kHeapMapFuncName (or kHeapMapFuncDisplay for the display driver), -1 if not tracked
static int32_t HeapMapFuncIndex(bool display, uint32_t function) {
  if (display) { return (function < HEAP_MAP_DISPLAY_TRACKED) ? (int32_t)function : -1; }
  for (uint32_t i = 0; i < sizeof(kHeapMapFuncCode); i++) {
    if (pgm_read_byte(kHeapMapFuncCode + i) == function) { return i; }
  }
  return -1;
}

// Called by macro HEAP_MAP_CALL before each driver call: true if the call is tracked
bool HeapMapTracked(bool display, uint32_t function) {
#if HEAP_MAP_EVENTS_BOOT_ONLY
  if (HeapMapGlobal.boot_done) { return false; }
#endif  // HEAP_MAP_EVENTS_BOOT_ONLY
  return HeapMapFuncIndex(display, function) >= 0;
}

// Line for a driver call or "other": label covers the first table columns, then used, delta, free and bar
static void HeapMapCallLine(const char *label, int32_t delta, uint32_t free_bytes) {
  uint32_t total = heap_caps_get_total_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  char used_str[12], delta_str[14], free_str[12];
  HeapMapKB(used_str, sizeof(used_str), (total > free_bytes) ? total - free_bytes : 0);
  HeapMapDeltaKB(delta_str, sizeof(delta_str), delta);
  HeapMapKB(free_str, sizeof(free_str), free_bytes);
  char bar[HEAP_MAP_BAR_WIDTH + 8];
  HeapMapBar(bar, sizeof(bar), free_bytes, total);
  AddLog(LOG_LEVEL_DEBUG, PSTR("HMP: %-43s %7s %7s %7s  %s"), label, used_str, delta_str, free_str, bar);
}

uint32_t HeapMapCallStart(int32_t *nested) {
  uint32_t free_start = heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if ((0 == HeapMapGlobal.call_depth) && HeapMapGlobal.mark_valid) {
    HeapMapGlobal.other += (int32_t)(HeapMapGlobal.mark - free_start);  // used since the last top level call or table row
  }
  HeapMapGlobal.call_depth++;
  *nested = HeapMapGlobal.call_nested;           // save the caller's sum
  HeapMapGlobal.call_nested = 0;
  return free_start;
}

void HeapMapCallEnd(const char *type, uint32_t id, uint32_t function, uint32_t free_start, int32_t nested) {
  uint32_t free_end = heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  int32_t used = (int32_t)(free_start - free_end);   // heap used by this call including nested calls
  int32_t self = used - HeapMapGlobal.call_nested;         // minus what lines inside this call already reported
  int32_t reported = HeapMapGlobal.call_nested;            // what the caller must exclude
  HeapMapGlobal.call_depth--;
  if (!HeapMapGlobal.call_logging && ((self >= HEAP_MAP_CALL_MIN) || (self <= -HEAP_MAP_CALL_MIN))) {
    HeapMapGlobal.call_logging = true;
    if ((HeapMapGlobal.other >= HEAP_MAP_CALL_MIN) || (HeapMapGlobal.other <= -HEAP_MAP_CALL_MIN)) {
      uint32_t free_other = HeapMapGlobal.has_last ? HeapMapGlobal.last_free - HeapMapGlobal.other : free_start;
      HeapMapCallLine(PSTR("other"), HeapMapGlobal.other, free_other);
    }
    HeapMapGlobal.other = 0;                     // printed, or too small and covered by the used column
    char fname[20];
    bool display = (id & HEAP_MAP_ID_DSP);
    GetTextIndexed(fname, sizeof(fname), HeapMapFuncIndex(display, function), display ? kHeapMapFuncDisplay : kHeapMapFuncName);
    uint32_t pos = id & 0xFFFF;             // position in the driver list if the number is unknown
#ifdef USE_DISPLAY
    if (display) {                          // ask the display driver if it is the active one
      id = XdspHeapMapId(pos);
      if (0 == id) { id = HEAP_MAP_ID_POS | pos; }
    }
#endif  // USE_DISPLAY
    char driver[12];                        // e.g. "xdrv_52", or "xlgt#3" by position, padded below so columns align
    if (id & (HEAP_MAP_ID_POS | HEAP_MAP_ID_DSP)) {
      snprintf_P(driver, sizeof(driver), PSTR("%s#%u"), type, (unsigned int)pos);
    } else {
      snprintf_P(driver, sizeof(driver), PSTR("%s_%02u"), type, (unsigned int)id);
    }
    char label[44];                         // driver and function, as wide as the first 5 table columns
    snprintf_P(label, sizeof(label), PSTR("%-8s %s"), driver, fname);
    HeapMapCallLine(label, self, free_end);
    uint32_t free_logged = heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    reported = used + (int32_t)(free_end - free_logged);  // the whole call and the log lines
    free_end = free_logged;
    HeapMapGlobal.last_free = free_logged;    // next delta starts from here
    HeapMapGlobal.has_last = true;
    HeapMapGlobal.call_logging = false;
  } else if (0 == HeapMapGlobal.call_depth) {
    HeapMapGlobal.other += self;                 // top level call not logged, report it later as "other"
  }
  HeapMapGlobal.call_nested = nested + reported; // restore the caller's sum, adding what was reported here
  if (0 == HeapMapGlobal.call_depth) {
    HeapMapGlobal.mark = free_end;
    HeapMapGlobal.mark_valid = true;
  }
}
#endif  // HEAP_MAP_CALL_MIN > 0
#endif  // ESP32 and USE_HEAP_MAP
