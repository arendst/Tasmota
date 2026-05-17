/*
  support_backlog.h - Public interface for the Backlog command queue

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

#ifndef _SUPPORT_BACKLOG_H_
#define _SUPPORT_BACKLOG_H_

namespace Backlog {

  enum class NoDelay : uint8_t {
    OFF,
    ON,
    NoChange
  };

  enum class NoMqttResponse : uint8_t {
    OFF,
    ON,
    NoChange
  };

  void Init();

  bool     IsEmpty();
  bool     IsNodelay();
  uint32_t GetRemainingDelay_ms();

  void SetNodelay(bool val);
  void SetNoMqttResponse(bool val);

  // Call from command handlers that are unsafe when the current drain step is NoDelay --
  // e.g. commands controlling hardware with settling-time requirements, interlock logic,
  // or state-machine dependencies. Logs an error if _nodelay_current is set.
  // Handlers without this call have not yet been evaluated for NoDelay safety.
  void WarnIfNoDelay(PGM_P cmd_name_P);

  void OnCommandExecuted();
  void ScheduleNow();
  void ScheduleDelay(uint32_t ms);

  void EnqueueCmd(const char* cmd, NoDelay noDelay = NoDelay::NoChange, NoMqttResponse noMqttResponse = NoMqttResponse::NoChange);
  void InsertCmd(const char* cmd, uint32_t position, NoDelay noDelay = NoDelay::NoChange, NoMqttResponse noMqttResponse = NoMqttResponse::NoChange);
  void Clear();

  void Loop();

  inline void EnqueueCmd(const char* cmd, NoMqttResponse noMqttResponse, NoDelay noDelay) { EnqueueCmd(cmd, noDelay, noMqttResponse); }
  inline void EnqueueCmd(const char* cmd, NoDelay noDelay)                                { EnqueueCmd(cmd, noDelay, NoMqttResponse::NoChange); }
  inline void EnqueueCmd(const char* cmd, NoMqttResponse noMqttResponse)                 { EnqueueCmd(cmd, NoDelay::NoChange, noMqttResponse); }
}

void BacklogLoop(void);

#endif  // _SUPPORT_BACKLOG_H_
