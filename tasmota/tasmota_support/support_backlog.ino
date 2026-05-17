/*
  support_backlog.ino - Backlog queue bridge for Tasmota

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

/*********************************************************************************************\
 * Scheduler entry point - called from the main loop in tasmota.ino.
 * Implementation lives in support_backlog.cpp (separate translation unit).
\*********************************************************************************************/

void BacklogLoop(void) { Backlog::Loop(); }

// Bridge: exposes TasmotaGlobal.sleep to support_backlog.cpp (separate translation unit).
// Called from Backlog::Loop() when _fast_budget_ms == 0 to derive the burst budget at runtime.
uint8_t TasmotaGetSleep() { return TasmotaGlobal.sleep; }
