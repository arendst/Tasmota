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
bool     TimeReached(uint32_t timer);
int32_t  TimePassedSince(uint32_t timestamp);
void     ExecuteCommand(const char *cmnd, uint32_t source);
void     SuppressMqttResponse();
uint8_t& SettingsParam(uint32_t index);

/*********************************************************************************************\
 * Backlog - command queue with configurable inter-command timing
 *
 * Variants:
 *   Backlog  / Backlog1 - commands separated by SetOption34 delay, MQTT responses on
 *   Backlog0 / Backlog2 - no delay (D_CMND_NODELAY inserted), MQTT responses on/off
 *   Backlog3            - SetOption34 delay, MQTT responses off
\*********************************************************************************************/

namespace Backlog {

/*********************************************************************************************\
 * Private state - inaccessible outside this translation unit
\*********************************************************************************************/
namespace {
  LList<char*> _queue;
  uint32_t     _timer        = 0;
  bool         _nodelay      = false;
  bool         _mutex        = false;
  bool         _delay_guard  = false;  // set by ScheduleDelay() inside a timed drain; prevents Loop() from overwriting _timer
  bool         _no_mqtt_resp = false;
}

/*********************************************************************************************\
 * Public interface
\*********************************************************************************************/

void Init() { _timer = millis(); }

bool IsEmpty()   { return _queue.isEmpty(); }
bool IsNodelay() { return _nodelay; }

uint32_t GetRemainingDelay_ms() {
  if (TimeReached(_timer)) { return 0; }
  return (uint32_t)(-TimePassedSince(_timer));
}

void SetNodelay(bool val)        { _nodelay      = val; }
void SetNoMqttResponse(bool val) { _no_mqtt_resp = val; }

// Called by CommandHandler for every dispatched command so that normal
// command processing schedules a new backlog drain window.
void OnCommandExecuted() {
  _timer = millis() + SettingsParam(P_BACKLOG_DELAY);
}

// Schedule the next drain to happen immediately (used after Backlog enqueue).
void ScheduleNow() {
  _timer = millis();
}

// Schedule the next drain after an explicit delay (used by CmndDelay).
// When called from within a timed drain (_mutex=true): sets _delay_guard so Loop() preserves
// the timer instead of overwriting it with the standard inter-command delay.
void ScheduleDelay(uint32_t ms) {
  _timer = millis() + ms;
  if (_mutex) { _delay_guard = true; }
}

// Append a single command string to the queue (called once per parsed token
// inside CmndBacklog's tokenisation loop).
void EnqueueCmd(const char* cmd) {
  char* temp = (char*)malloc(strlen(cmd) + 1);
  if (temp != nullptr) {
    strcpy(temp, cmd);
    char* &elem = _queue.addToLast();
    elem = temp;
  }
}

// Insert a command at a specific position (used by the rules IF/ENDIF engine
// to prepend a command block in-order at the head of the queue).
void InsertCmd(const char* cmd, uint32_t position) {
  char* temp = (char*)malloc(strlen(cmd) + 1);
  if (temp != nullptr) {
    strcpy(temp, cmd);
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
      bool nodelay = false;
      do {
        char* cmd = *_queue.head();
        _queue.removeHead();
        if (!strncasecmp_P(cmd, PSTR(D_CMND_NODELAY), strlen(D_CMND_NODELAY))) {
          free(cmd);
          nodelay = true;
        } else {
          if (_no_mqtt_resp) { SuppressMqttResponse(); }
          ExecuteCommand(cmd, SRC_BACKLOG);
          free(cmd);
          // Loop() owns the timer after each timed drain step.
          // Exception: when CmndDelay ran during the drain it sets _delay_guard via
          // ScheduleDelay(). In that case Loop() preserves _timer for that one step.
          if (nodelay || _nodelay) {
            _timer = millis();
          } else if (_delay_guard) {
            _delay_guard = false;
          } else {
            _timer = millis() + SettingsParam(P_BACKLOG_DELAY);
          }
          break;
        }
      } while (!_queue.isEmpty());
      _mutex = false;
    }
    if (_queue.isEmpty()) {
      _nodelay = false;
    }
  }
}

} // namespace Backlog
