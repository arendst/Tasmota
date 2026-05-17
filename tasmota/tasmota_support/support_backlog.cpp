/*
  support_backlog.cpp - Backlog command queue implementation

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

#include <Arduino.h>
#include <LList.h>
#include "my_user_config.h"
#include "include/tasmota_compat.h"
#include "include/tasmota.h"
#include "include/i18n.h"
#include "include/support_backlog.h"

// Prototype duplicates for symbols defined in the unity build (no header available)
bool      TimeReached(uint32_t timer);
int32_t   TimePassedSince(uint32_t timestamp);
void      ExecuteCommand(const char *cmnd, uint32_t source);
void      SuppressMqttResponse();
uint8_t&  SettingsParam(uint32_t index);
uint32_t  GetOption(uint32_t index);
void      AddLog(uint32_t loglevel, PGM_P formatP, ...);
int       Response_P(PGM_P formatP, ...);
int       ResponseAppend_P(PGM_P formatP, ...);
uint8_t   TasmotaGetSleep();  // defined in support_backlog.ino: returns TasmotaGlobal.sleep

/*********************************************************************************************\
 * Backlog - command queue with configurable inter-command timing
 *
 * Variants:
 *   Backlog  / Backlog1 - commands separated by SetOption34 delay, MQTT responses on
 *   Backlog0 / Backlog2 - no delay, MQTT responses on/off
 *   Backlog3            - SetOption34 delay, MQTT responses off
 *
 * Queue entry layout:
 *   Without BACKLOG_TRACE_SOURCE: [flavor_byte][cmd\0]           - malloc(strlen+2)
 *   With    BACKLOG_TRACE_SOURCE: [flavor_byte][source][cmd\0]   - malloc(strlen+3)
 *   flavor_byte: bit0=nodelay, bit1=no_mqtt_resp
 *   source:      CommandSource enum value, 255 = not annotated
\*********************************************************************************************/

namespace Backlog {

/*********************************************************************************************\
 * Private state - inaccessible outside this translation unit
\*********************************************************************************************/
namespace {
  LList<char*> _timed_queue;   // timed-lane: Backlog1/3, or all commands when USE_BACKLOG_FASTLANE not defined
#ifdef USE_BACKLOG_FASTLANE
  LList<char*> _fast_queue;    // fast-lane: nodelay commands (Backlog0/2) when SO_BACKLOG_FASTLANE=1
  uint32_t     _fast_budget_ms     = BACKLOG_FASTLANE_BUDGET_MS;  // 0 = use TasmotaGlobal.sleep
  uint32_t     _fast_drain_count   = 0;
#endif
  uint32_t     _timer                = 0;
  bool         _nodelay_staged       = false;   // set by CmndBacklog / ExecuteCommandBlock
  bool         _nodelay_current      = false;   // set per drain step from flavor byte
  bool         _mutex                = false;
  bool         _delay_guard          = false;   // set by ScheduleDelay() inside a timed drain; prevents Loop() from overwriting _timer
  bool         _no_mqtt_resp_staged  = false;   // set by CmndBacklog / ExecuteCommandBlock
  bool         _no_mqtt_resp_current = false;   // set per drain step from flavor byte

  // Phase H: diagnostics and runtime config
  uint32_t     _drain_count           = 0;
  uint32_t     _enqueue_count         = 0;
  uint32_t     _insert_count          = 0;
  uint32_t     _mutex_skip            = 0;
  uint32_t     _depth                 = 0;  // running counter: +1 on enqueue/insert, -1 on drain, 0 on clear
  uint32_t     _max_depth             = 0;
  uint32_t     _max_bytes             = 0;
  uint32_t     _max_entry_len         = 0;
  uint32_t     _chunk_size            = 20;
  bool         _trace_drain           = false;

  // Phase S: queue byte limit
  uint32_t     _queue_bytes           = 0;  // current heap used by queue entries
  uint32_t     _discard_count         = 0;  // enqueue attempts rejected due to byte limit

  // Byte offset from start of a queue entry to the command string.
#ifdef BACKLOG_TRACE_SOURCE
  static constexpr uint8_t kCmdOffset = 2;
#else
  static constexpr uint8_t kCmdOffset = 1;
#endif

  static constexpr uint32_t kDefaultMaxBytes = BACKLOG_QUEUE_MAX_BYTES;
  static constexpr uint32_t kMinBytes        = BACKLOG_QUEUE_MIN_BYTES;
  static uint32_t           _bytes_limit     = kDefaultMaxBytes;

  // Estimated bookkeeping added by the heap allocator per malloc/new call on ESP targets.
  // Applied twice per queued command: once for the malloc'd entry, once for the LList node.
  static constexpr uint32_t kHeapAllocOverhead = 8;
  // Fixed heap cost per queued command, independent of command length:
  //   LList<char*>::element_size - node struct (pointer + value), from LList's public interface
  //   2 * kHeapAllocOverhead     - allocator bookkeeping for entry malloc + LList node new
  static constexpr uint32_t kEntryFixedOverhead =
      static_cast<uint32_t>(LList<char*>::element_size) + 2 * kHeapAllocOverhead;
  // Total fixed accounting cost added per queued command on top of the raw string length:
  //   kCmdOffset + 1 - header byte(s) + null terminator in the malloc'd entry (1+1 or 2+1)
  //   kEntryFixedOverhead - LList node struct (8 B) + 2 x allocator bookkeeping (2x8 B) = 24 B
  // Sum: 26 B (without BACKLOG_TRACE_SOURCE) or 27 B (with). Empirically confirmed on ESP8266.
  // See BACKLOG_QUEUE_MAX_BYTES / BACKLOG_QUEUE_MIN_BYTES in tasmota.h for capacity estimates.
  static constexpr uint32_t kPerCmdAcctOverhead = kCmdOffset + 1 + kEntryFixedOverhead;
}

// Accounting byte cost for cmd_count commands whose strings sum to str_len bytes.
// Single source of truth for the per-entry overhead formula - used by all check and
// drain sites so that formula changes require editing exactly one place.
static uint32_t _AcctBytes(uint32_t str_len, uint32_t cmd_count) {
  return str_len + cmd_count * kPerCmdAcctOverhead;
}

// Returns true if acct_bytes fit within the queue limit.
// On failure: increments _discard_count, returns false. Caller owns the log message.
static bool _CheckUsageCount(uint32_t acct_bytes) {
  if (_queue_bytes + acct_bytes > _bytes_limit) { _discard_count++; return false; }
  return true;
}

/*********************************************************************************************\
 * Public interface
\*********************************************************************************************/

void Init() { _timer = millis(); }

bool IsEmpty() {
#ifdef USE_BACKLOG_FASTLANE
  return _timed_queue.isEmpty() && _fast_queue.isEmpty();
#else
  return _timed_queue.isEmpty();
#endif
}

bool IsSleepBlocked() {
#ifdef USE_BACKLOG_FASTLANE
  if (GetOption(SO_BACKLOG_FASTLANE) && !_fast_queue.isEmpty()) return true;
#endif
  return !_timed_queue.isEmpty() && TimeReached(_timer);
}

uint32_t GetChunkSize() { return _chunk_size; }
bool     IsTraceDrain() { return _trace_drain; }

uint32_t GetRemainingDelay_ms() {
  if (TimeReached(_timer)) { return 0; }
  return (uint32_t)(-TimePassedSince(_timer));
}

void SetNodelay(bool val)        { _nodelay_staged      = val; }
void SetNoMqttResponse(bool val) { _no_mqtt_resp_staged = val; }
void SetChunkSize(uint32_t n)    { if (n > 0) _chunk_size = n; }
void SetTraceDrain(bool val)     { _trace_drain = val; }

uint32_t GetMaxBytes() { return _bytes_limit; }
void SetMaxBytes(uint32_t limit) {
  _bytes_limit = (limit == 0) ? kDefaultMaxBytes : max(limit, kMinBytes);
}

#ifdef USE_BACKLOG_FASTLANE
uint32_t GetFastBudget() { return _fast_budget_ms; }
void SetFastBudget(uint32_t ms) {
  if (ms == 0) {
    _fast_budget_ms = 0;  // 0 = derive from TasmotaGlobal.sleep at runtime
  } else {
    _fast_budget_ms = min(max(ms, (uint32_t)BACKLOG_FASTLANE_BUDGET_MIN_MS),
                              (uint32_t)BACKLOG_FASTLANE_BUDGET_MAX_MS);
  }
}
#endif

// Log a warning when a command that requires inter-command settling time is called
// inside a NoDelay drain step. Call from handlers with hardware or state-machine
// dependencies that make them unsafe at zero inter-command delay.
void WarnIfNoDelay(PGM_P cmd_name_P) {
  if (_nodelay_current)
    AddLog(LOG_LEVEL_ERROR, PSTR("BLG: '%s' unsafe in NoDelay context"), cmd_name_P);
}

// Flavor-byte accessors - bit0=nodelay, bit1=no_mqtt_resp
static bool _NoDelayOf(const char* head)        { return !!(*head & (1 << 0)); }
static bool _NoMqttOf(const char* head)         { return !!(*head & (1 << 1)); }
static void _SetNoDelayIn(char* val, bool flag)  { *val |= (flag ? 1 : 0) << 0; }
static void _SetNoMqttIn(char* val, bool flag)   { *val |= (flag ? 1 : 0) << 1; }

// Returns the configured post-external-command drain window in ms, or 0 if disabled.
// SO166=0 (default): window active, value from BACKLOG_EXT_DELAY compile constant.
// SO166=1:           window disabled; BacklogLoop is sole timer owner (SO34 still applies).
// Override at build time: #define BACKLOG_EXT_DELAY XXX in user_config_override.h.
static uint32_t _ExtDelayMs() {
  if (GetOption(SO_BACKLOG_EXT_DELAY_DISABLE)) { return 0; }
  return BACKLOG_EXT_DELAY;
}

// Called by CommandHandler for every dispatched command so that external commands
// (MQTT, Serial, Button, ...) can extend the Backlog drain window.
// BacklogLoop() overrides _timer after each drain step regardless - this only
// has lasting effect for commands that do NOT originate from the Backlog itself.
void OnCommandExecuted() {
  uint32_t ms = _ExtDelayMs();
  if (ms) { _timer = millis() + ms; }
}

// Schedule the next drain to happen immediately (used after Backlog enqueue).
void ScheduleNow() {
  _timer = millis();
}

// Schedule the next drain after an explicit delay (used by CmndDelay).
// No-op when _nodelay_current is set (Delay has no effect inside NoDelay sequences).
// When called from within a timed drain (_mutex=true): sets _delay_guard so Loop() preserves
// the timer instead of overwriting it with the standard inter-command delay.
void ScheduleDelay(uint32_t ms) {
  if (_nodelay_current) { return; }
  _timer = millis() + ms;
  if (_mutex) { _delay_guard = true; }
}

// Pre-check for CmndBacklog(): whole-sequence-or-nothing gate.
// Call before the tokenisation loop with the raw data string length and the number of
// commands (semicolons + 1). Counts the rejection and logs the byte context on failure.
// Contract: after a true return the caller MUST enqueue the sequence - no exceptions.
bool TryReserveSequence(uint32_t total_str_len, uint32_t cmd_count) {
  uint32_t estimated = _AcctBytes(total_str_len, cmd_count);
  if (!_CheckUsageCount(estimated)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("BLG: Sequence rejected, queue %u/%u B, need ~%u B"),
           _queue_bytes, _bytes_limit, estimated);
    return false;
  }
  return true;
}

// Append a single command string to the queue (called once per parsed token
// inside CmndBacklog's tokenisation loop).
// Each entry is prefixed with a flavor byte (bit0=nodelay, bit1=no_mqtt_resp)
// baked in from the staged flags at enqueue time.
void EnqueueCmd(const char* cmd, uint8_t source, NoDelay noDelay, NoMqttResponse noMqttResponse) {
  if (NoDelay::NoChange != noDelay)
    _nodelay_staged = NoDelay::ON == noDelay;

  if (NoMqttResponse::NoChange != noMqttResponse)
    _no_mqtt_resp_staged = NoMqttResponse::ON == noMqttResponse;

  uint32_t cmd_len     = strlen(cmd);
  uint32_t alloc_bytes = cmd_len + 1 + kCmdOffset;
  uint32_t acct_bytes  = _AcctBytes(cmd_len, 1);
  if (!_CheckUsageCount(acct_bytes)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("BLG: Queue full (%u/%u B), discarded: %s"),
           _queue_bytes, _bytes_limit, cmd);
    return;
  }
  char* temp = (char*)malloc(alloc_bytes);
  if (temp != nullptr) {
    *temp = 0;
    _SetNoDelayIn(temp, _nodelay_staged);
    _SetNoMqttIn(temp, _no_mqtt_resp_staged);
#ifdef BACKLOG_TRACE_SOURCE
    *(temp + 1) = (char)source;
#endif
    strcpy(temp + kCmdOffset, cmd);
#ifdef USE_BACKLOG_FASTLANE
    // Lane routing is decided at enqueue time from _nodelay_staged and SO167.
    // Switching between lanes within a sequence without restarting at position 0
    // is undefined behaviour - callers must not rely on cross-lane ordering.
    char* &elem = (GetOption(SO_BACKLOG_FASTLANE) && _nodelay_staged)
                  ? _fast_queue.addToLast()
                  : _timed_queue.addToLast();
#else
    char* &elem = _timed_queue.addToLast();
#endif
    elem = temp;
    _enqueue_count++;
    _depth++;
    _queue_bytes += acct_bytes;
    if (_queue_bytes > _max_bytes)  { _max_bytes  = _queue_bytes; }
    if (cmd_len > _max_entry_len)   { _max_entry_len = cmd_len; }
#ifdef USE_BACKLOG_FASTLANE
    uint32_t d = _fast_queue.length() + _timed_queue.length();
#else
    uint32_t d = _timed_queue.length();
#endif
    if (d > _max_depth) { _max_depth = d; }
  }
}

// Insert a command at a specific position (used by the rules IF/ENDIF engine
// to prepend a command block in-order at the head of the queue).
void InsertCmd(const char* cmd, uint32_t position, uint8_t source, NoDelay noDelay, NoMqttResponse noMqttResponse) {
  if (NoDelay::NoChange != noDelay)
    _nodelay_staged = NoDelay::ON == noDelay;

  if (NoMqttResponse::NoChange != noMqttResponse)
    _no_mqtt_resp_staged = NoMqttResponse::ON == noMqttResponse;

  uint32_t cmd_len     = strlen(cmd);
  uint32_t alloc_bytes = cmd_len + 1 + kCmdOffset;
  uint32_t acct_bytes  = _AcctBytes(cmd_len, 1);
  if (!_CheckUsageCount(acct_bytes)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("BLG: Queue full (%u/%u B), discarded: %s"),
           _queue_bytes, _bytes_limit, cmd);
    return;
  }
  char* temp = (char*)malloc(alloc_bytes);
  if (temp != nullptr) {
    *temp = 0;
    _SetNoDelayIn(temp, _nodelay_staged);
    _SetNoMqttIn(temp, _no_mqtt_resp_staged);
#ifdef BACKLOG_TRACE_SOURCE
    *(temp + 1) = (char)source;
#endif
    strcpy(temp + kCmdOffset, cmd);
#ifdef USE_BACKLOG_FASTLANE
    // Position counts within the target lane.
    char* &elem = (GetOption(SO_BACKLOG_FASTLANE) && _nodelay_staged)
                  ? _fast_queue.insertAt(position)
                  : _timed_queue.insertAt(position);
    uint32_t d = _fast_queue.length() + _timed_queue.length();
#else
    char* &elem = _timed_queue.insertAt(position);
    uint32_t d = _timed_queue.length();
#endif
    elem = temp;
    _insert_count++;
    _depth++;
    _queue_bytes += acct_bytes;
    if (_queue_bytes > _max_bytes)  { _max_bytes  = _queue_bytes; }
    if (cmd_len > _max_entry_len)   { _max_entry_len = cmd_len; }
    if (d > _max_depth) { _max_depth = d; }
  }
}

// Discard all queued commands (called when Backlog is invoked with no data).
void Clear() {
#ifdef USE_BACKLOG_FASTLANE
  for (auto &elem : _fast_queue)  { free(elem); _fast_queue.remove(&elem); }
#endif
  for (auto &elem : _timed_queue) { free(elem); _timed_queue.remove(&elem); }
  _depth       = 0;
  _queue_bytes = 0;
}

// Drain one entry from the given queue; update accounting and counters.
// Returns true if an entry was drained.
static bool _DrainOne(LList<char*>& queue, uint32_t& drain_counter, const char* tag) {
  if (queue.isEmpty() || _mutex) {
    if (!queue.isEmpty()) { _mutex_skip++; }
    return false;
  }
  _mutex = true;
  char* head = *queue.head();
  queue.removeHead();
  _nodelay_current      = _NoDelayOf(head);
  _no_mqtt_resp_current = _NoMqttOf(head);
  char*    cmd        = head + kCmdOffset;
  uint32_t acct_bytes = _AcctBytes(strlen(cmd), 1);  // must be read before free(head)
  if (_trace_drain) {
    // D= shows depth before this drain step (pre-decrement): last entry logs D=1, not D=0.
    // Semantics: "queue held D entries when this command was taken."
#ifdef BACKLOG_TRACE_SOURCE
    AddLog(LOG_LEVEL_INFO, PSTR("%s: D=%u T=%u Src=%u Cmd=\"%s\""), tag,
           _depth, (uint8_t)*head, (uint8_t)*(head + 1), cmd);
#else
    AddLog(LOG_LEVEL_INFO, PSTR("%s: D=%u T=%u Cmd=\"%s\""), tag,
           _depth, (uint8_t)*head, cmd);
#endif
  }
  if (_depth > 0) { _depth--; }
  else { AddLog(LOG_LEVEL_ERROR, PSTR("%s: depth counter underflow"), tag); }
  if (_no_mqtt_resp_current) { SuppressMqttResponse(); }
  ExecuteCommand(cmd, SRC_BACKLOG);
  free(head);
  drain_counter++;
  if (_queue_bytes >= acct_bytes) { _queue_bytes -= acct_bytes; }
  else { _queue_bytes = 0; }
  _mutex = false;
  return true;
}

// Main drain loop - called every iteration from BacklogLoop().
void Loop() {
#ifdef USE_BACKLOG_FASTLANE
  // Fast-Lane: drain nodelay entries for up to _fast_budget_ms per Loop() call, no timer gate.
  // At least one entry is always drained when the queue is non-empty (do-while guarantee).
  // _timer is owned exclusively by the Timed-Lane and is not touched here.
  if (GetOption(SO_BACKLOG_FASTLANE) && !_fast_queue.isEmpty()) {
    uint32_t budget      = _fast_budget_ms ? _fast_budget_ms : (uint32_t)TasmotaGetSleep();
    uint32_t burst_start = millis();
    do {
      if (!_DrainOne(_fast_queue, _fast_drain_count, PSTR("BLF"))) { break; }
    } while (!_fast_queue.isEmpty() && (millis() - burst_start) < budget);
  }
#endif

  // Timed-Lane: one entry per Loop() call, gated by _timer.
  // After each drain Loop() normally owns the timer (lookahead sets inter-command delay).
  // Exception: when CmndDelay ran during the drain it sets _delay_guard via ScheduleDelay().
  // In that case Loop() preserves _timer -- the delay value from CmndDelay takes precedence.
  // OnCommandExecuted() (external commands, SO166) can still shorten an active delay; this
  // matches the original Tasmota behaviour and is intentional (external override).
  if (TimeReached(_timer)) {
    if (_DrainOne(_timed_queue, _drain_count, PSTR("BLG"))) {
      if (_delay_guard) {
        _delay_guard = false;
      } else {
        bool next_nodelay = !_timed_queue.isEmpty() && _NoDelayOf(*_timed_queue.head());
        _timer = next_nodelay ? millis() : millis() + SettingsParam(P_BACKLOG_DELAY);
      }
    }
  }

  if (IsEmpty()) {
    _nodelay_current      = false;
    _no_mqtt_resp_current = false;
  }
}

// Backlog20 - Queue statistics snapshot
void DumpStats() {
#ifdef USE_BACKLOG_FASTLANE
  uint32_t depth = _fast_queue.length() + _timed_queue.length();
#else
  uint32_t depth = _timed_queue.length();
#endif
  int32_t  timer_delta = (int32_t)(_timer - millis());

  Response_P(PSTR("{\"BacklogStat\":{"));
  ResponseAppend_P(PSTR("\"Depth\":%u,\"DepthCounter\":%u,\"Bytes\":%u,"), depth, _depth, _queue_bytes);
  ResponseAppend_P(PSTR("\"TimerMs\":%d,\"Ready\":%u,\"Mutex\":%u,"),
                   timer_delta, TimeReached(_timer) ? 1 : 0, _mutex ? 1 : 0);
  ResponseAppend_P(PSTR("\"StagedNodelay\":%u,\"StagedNoMqtt\":%u,"),
                   _nodelay_staged ? 1 : 0, _no_mqtt_resp_staged ? 1 : 0);
  ResponseAppend_P(PSTR("\"CurrentNodelay\":%u,\"TraceDrain\":%u,"),
                   _nodelay_current ? 1 : 0, _trace_drain ? 1 : 0);
  ResponseAppend_P(PSTR("\"Drained\":%u,\"Enqueued\":%u,\"Inserted\":%u,"),
                   _drain_count, _enqueue_count, _insert_count);
  ResponseAppend_P(PSTR("\"Discarded\":%u,\"MutexSkipped\":%u,"), _discard_count, _mutex_skip);
  ResponseAppend_P(PSTR("\"MaxDepth\":%u,\"MaxBytes\":%u,\"MaxEntryLen\":%u,"),
                   _max_depth, _max_bytes, _max_entry_len);
  ResponseAppend_P(PSTR("\"SO34\":%u,\"SO166\":%u,\"ExtDelayMs\":%u,"),
                   SettingsParam(P_BACKLOG_DELAY),
                   GetOption(SO_BACKLOG_EXT_DELAY_DISABLE),
                   _ExtDelayMs());
#ifdef USE_BACKLOG_FASTLANE
  uint32_t fast_budget_eff = _fast_budget_ms ? _fast_budget_ms : (uint32_t)TasmotaGetSleep();
  ResponseAppend_P(PSTR("\"FastLane\":%u,\"FastDepth\":%u,\"FastDrained\":%u,\"FastBudget\":%u,\"FastBudgetMs\":%u,"),
                   GetOption(SO_BACKLOG_FASTLANE) ? 1 : 0,
                   _fast_queue.length(), _fast_drain_count, _fast_budget_ms, fast_budget_eff);
#endif
  ResponseAppend_P(PSTR("\"ChunkSize\":%u,\"BytesLimit\":%u,\"DefaultBytesLimit\":%u}}"),
                   _chunk_size, _bytes_limit, kDefaultMaxBytes);
}

// Emit one queue entry to both AddLog and ResponseAppend. Returns updated idx.
static uint32_t _DumpEntry(char* head, uint32_t idx, const char* lane, bool& first) {
  uint8_t flavor = (uint8_t)*head;
  char*   cmd    = head + kCmdOffset;
  if (!first) { ResponseAppend_P(PSTR(",")); }
  first = false;
#ifdef BACKLOG_TRACE_SOURCE
  uint8_t src = (uint8_t)*(head + 1);
  AddLog(LOG_LEVEL_INFO, PSTR("BLQ: {\"Idx\":%u,\"Lane\":\"%s\",\"T\":%u,\"Src\":%u,\"Cmd\":\"%s\"}"),
         idx, lane, flavor, src, cmd);
  ResponseAppend_P(PSTR("{\"Idx\":%u,\"Lane\":\"%s\",\"T\":%u,\"Src\":%u,\"Cmd\":\"%s\"}"),
                   idx, lane, flavor, src, cmd);
#else
  AddLog(LOG_LEVEL_INFO, PSTR("BLQ: {\"Idx\":%u,\"Lane\":\"%s\",\"T\":%u,\"Cmd\":\"%s\"}"),
         idx, lane, flavor, cmd);
  ResponseAppend_P(PSTR("{\"Idx\":%u,\"Lane\":\"%s\",\"T\":%u,\"Cmd\":\"%s\"}"),
                   idx, lane, flavor, cmd);
#endif
  return idx + 1;
}

// Backlog21..29 - Queue content, paged
// page = BacklogIndex - 21; entries [page*_chunk_size .. (page+1)*_chunk_size - 1]
// Fast-Lane entries (Lane:"F") appear first, Timed-Lane (Lane:"T") after. Idx is global.
// All output channels use identical JSON format - one entry object per AddLog line.
void DumpQueue(uint32_t page) {
#ifdef USE_BACKLOG_FASTLANE
  uint32_t depth = _fast_queue.length() + _timed_queue.length();
#else
  uint32_t depth = _timed_queue.length();
#endif
  uint32_t start_idx = page * _chunk_size;
  uint32_t end_idx   = start_idx + _chunk_size;

  AddLog(LOG_LEVEL_INFO, PSTR("BLQ: {\"BacklogQueue\":{\"Depth\":%u,\"StartIdx\":%u,\"ChunkSize\":%u,\"Entry\":["),
         depth, start_idx, _chunk_size);
  Response_P(PSTR("{\"BacklogQueue\":{\"Depth\":%u,\"StartIdx\":%u,\"ChunkSize\":%u,\"Entry\":["),
             depth, start_idx, _chunk_size);

  uint32_t idx  = 0;
  bool     first = true;

#ifdef USE_BACKLOG_FASTLANE
  for (auto &entry : _fast_queue) {
    if (idx >= end_idx)   { break; }
    if (idx >= start_idx) { idx = _DumpEntry(entry, idx, "F", first); }
    else                  { idx++; }
  }
#endif
  for (auto &entry : _timed_queue) {
    if (idx >= end_idx)   { break; }
    if (idx >= start_idx) { idx = _DumpEntry(entry, idx, "T", first); }
    else                  { idx++; }
  }

  AddLog(LOG_LEVEL_INFO, PSTR("BLQ: ]}}"));
  ResponseAppend_P(PSTR("]}}"));
}

} // namespace Backlog
