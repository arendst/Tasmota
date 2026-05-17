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

/*********************************************************************************************\
 * Backlog - command queue with configurable inter-command timing
 *
 * Variants:
 *   Backlog  / Backlog1 - commands separated by SetOption34 delay, MQTT responses on
 *   Backlog0 / Backlog2 - no delay, MQTT responses on/off
 *   Backlog3            - SetOption34 delay, MQTT responses off
\*********************************************************************************************/

namespace Backlog {

/*********************************************************************************************\
 * Private state - inaccessible outside this translation unit
\*********************************************************************************************/
namespace {
  LList<char*> _queue;
  uint32_t     _timer                = 0;
  bool         _nodelay_staged       = false;   // set by CmndBacklog / ExecuteCommandBlock
  bool         _nodelay_current      = false;   // set per drain step from flavor byte
  bool         _mutex                = false;
  bool         _delay_guard          = false;   // set by ScheduleDelay() inside a timed drain; prevents Loop() from overwriting _timer
  bool         _no_mqtt_resp_staged  = false;   // set by CmndBacklog / ExecuteCommandBlock
  bool         _no_mqtt_resp_current = false;   // set per drain step from flavor byte
}

/*********************************************************************************************\
 * Public interface
\*********************************************************************************************/

void Init() { _timer = millis(); }

bool IsEmpty()   { return _queue.isEmpty(); }
bool IsNodelay() { return _nodelay_current; }

uint32_t GetRemainingDelay_ms() {
  if (TimeReached(_timer)) { return 0; }
  return (uint32_t)(-TimePassedSince(_timer));
}

void SetNodelay(bool val)        { _nodelay_staged      = val; }
void SetNoMqttResponse(bool val) { _no_mqtt_resp_staged = val; }

// Log a warning when a command that requires inter-command settling time is called
// inside a NoDelay drain step. Call from handlers with hardware or state-machine
// dependencies that make them unsafe at zero inter-command delay.
void WarnIfNoDelay(PGM_P cmd_name_P) {
  if (_nodelay_current)
    AddLog(LOG_LEVEL_ERROR, PSTR("BLG: '%s' unsafe in NoDelay context"), cmd_name_P);
}

// Flavor-byte accessors - bit0=nodelay, bit1=no_mqtt_resp
static bool _NoDelayOf(const char* head)       { return !!(*head & (1 << 0)); }
static bool _NoMqttOf(const char* head)        { return !!(*head & (1 << 1)); }
static void _SetNoDelayIn(char* val, bool flag) { *val |= (flag ? 1 : 0) << 0; }
static void _SetNoMqttIn(char* val, bool flag)  { *val |= (flag ? 1 : 0) << 1; }

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

// Append a single command string to the queue (called once per parsed token
// inside CmndBacklog's tokenisation loop).
// Each entry is prefixed with a flavor byte (bit0=nodelay, bit1=no_mqtt_resp)
// baked in from the staged flags at enqueue time.
void EnqueueCmd(const char* cmd, NoDelay noDelay, NoMqttResponse noMqttResponse) {
  if (NoDelay::NoChange != noDelay)
    _nodelay_staged = NoDelay::ON == noDelay;

  if (NoMqttResponse::NoChange != noMqttResponse)
    _no_mqtt_resp_staged = NoMqttResponse::ON == noMqttResponse;

  char* temp = (char*)malloc(strlen(cmd) + 2);
  if (temp != nullptr) {
    *temp = 0;
    _SetNoDelayIn(temp, _nodelay_staged);
    _SetNoMqttIn(temp, _no_mqtt_resp_staged);
    strcpy(temp + 1, cmd);
    char* &elem = _queue.addToLast();
    elem = temp;
  }
}

// Insert a command at a specific position (used by the rules IF/ENDIF engine
// to prepend a command block in-order at the head of the queue).
void InsertCmd(const char* cmd, uint32_t position, NoDelay noDelay, NoMqttResponse noMqttResponse) {
  if (NoDelay::NoChange != noDelay)
    _nodelay_staged = NoDelay::ON == noDelay;

  if (NoMqttResponse::NoChange != noMqttResponse)
    _no_mqtt_resp_staged = NoMqttResponse::ON == noMqttResponse;

  char* temp = (char*)malloc(strlen(cmd) + 2);
  if (temp != nullptr) {
    *temp = 0;
    _SetNoDelayIn(temp, _nodelay_staged);
    _SetNoMqttIn(temp, _no_mqtt_resp_staged);
    strcpy(temp + 1, cmd);
    char* &elem = _queue.insertAt(position);
    elem = temp;
  }
}

// Discard all queued commands (called when Backlog is invoked with no data).
void Clear() {
  for (auto &elem : _queue) {
    free(elem);
    _queue.remove(&elem);
  }
}

// Main drain loop - called every iteration from BacklogLoop().
void Loop() {
  if (TimeReached(_timer)) {
    if (!_queue.isEmpty() && !_mutex) {
      _mutex = true;
      char* head = *_queue.head();
      _queue.removeHead();
      _nodelay_current      = _NoDelayOf(head);
      _no_mqtt_resp_current = _NoMqttOf(head);
      char* cmd = head + 1;
      if (_no_mqtt_resp_current) { SuppressMqttResponse(); }
      ExecuteCommand(cmd, SRC_BACKLOG);
      free(head);
      // Loop() is the sole owner of _timer after each drain step.
      // Exception: when CmndDelay ran during the drain it sets _delay_guard via
      // ScheduleDelay(). In that case Loop() preserves _timer for that one step.
      if (_nodelay_current) {
        _timer = millis();
      } else if (_delay_guard) {
        _delay_guard = false;
      } else {
        _timer = millis() + SettingsParam(P_BACKLOG_DELAY);
      }
      _mutex = false;
    }
    if (_queue.isEmpty()) {
      _nodelay_current      = false;
      _no_mqtt_resp_current = false;
    }
  }
}

} // namespace Backlog
