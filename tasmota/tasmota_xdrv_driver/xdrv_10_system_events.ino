/*
  xdrv_10_system_events.ino - system events producer for Tasmota

  Copyright (C) 2021  ESP Easy Group and Theo Arends

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
 * System events producer
 *
 * This file is the single source of truth for emitting Tasmota's "system" rules-events:
 *   - Power<i>#State / Power<i>#Boot, <switch>#Boot
 *   - Dimmer#State / Dimmer#Boot
 *   - Time#Minute, Time#Initialized, Time#Set
 *   - System#Init, System#Boot, System#Save
 *   - MQTT#Connected, MQTT#Disconnected
 *   - WIFI#Connected, WIFI#Disconnected
 *   - ETH#Connected, ETH#Disconnected (ESP32 + USE_ETHERNET only)
 *   - HTTP#Initialized
 *   - SHUTTER#Moved, SHUTTER#Moving (USE_SHUTTER only)
 *   - Event#<key> (user-driven `Event` command)
 *
 * The emission helpers (SystemEventsInit, SystemEventsEvery50ms, SystemEventsEverySecond,
 * SystemEventsSaveBeforeRestart, CmndEvent) are always compiled, regardless of USE_RULES,
 * USE_SCRIPT, or USE_BERRY. They are defined exactly once across the codebase, in this file.
 *
 * Slot-10 mutual exclusion
 * ------------------------
 * Exactly one of the three slot-10 files registers Xdrv10 / XDRV_10 per build:
 *
 *   USE_RULES defined                -> xdrv_10_rules.ino owns Xdrv10 (calls helpers from here)
 *   USE_SCRIPT defined (no USE_RULES) -> xdrv_10_scripter.ino owns Xdrv10 (helpers here unused)
 *   neither defined                   -> this file owns Xdrv10 (calls helpers locally)
 *
 * The conditional Xdrv10 / RulesProcessEvent / RulesProcess stub for the "neither" build is
 * gated by `#ifndef USE_RULES` / `#ifndef USE_SCRIPT` and added in a separate sub-task.
 *
 * Maintenance hazard: RulesBitfield layout
 * ----------------------------------------
 * The strict-priority drainage in SystemEventsEvery50ms and the inline `rules_flag.*` polls
 * in xdrv_10_scripter.ino both depend on the bit layout of RulesBitfield in
 * tasmota/include/tasmota_types.h. Any change to RulesBitfield (adding, removing, or
 * reordering bits) requires matching updates here AND in xdrv_10_rules.ino AND in
 * xdrv_10_scripter.ino. The comment next to the RulesBitfield definition in
 * tasmota_types.h is the canonical reminder for this contract.
\*********************************************************************************************/

#ifndef RULE_MAX_EVENTSZ
#define RULE_MAX_EVENTSZ        100
#endif

struct SYSTEM_EVENTS {
  int32_t new_power = -1;            // current TasmotaGlobal.power snapshot
  int32_t old_power = -1;            // last seen power mask; -1 == "never sampled" (boot path)
  int32_t old_dimm = -1;             // last seen Settings->light_dimmer; -1 == boot path
  uint16_t last_minute = 60;      // last RtcTime.minute that fired Time#Minute (60 = invalid sentinel)
  bool teleperiod = false;        // set by FUNC_TELEPERIOD_RULES_PROCESS in the active dispatcher
  bool busy = false;              // re-entry guard for the local RulesProcessEvent stub
  char event_data[RULE_MAX_EVENTSZ];  // single-slot queue for the `Event` command
} SystemEvents;

/*********************************************************************************************\
 * Producer initialization
\*********************************************************************************************/

void SystemEventsInit(void) {
  // Start with all rules-flag bits clear so the first 50ms drainage tick has a clean slate.
  // Settings->rule_enabled and Settings->rule_once are intentionally NOT touched here; they
  // are part of the Rules-engine domain and remain owned by xdrv_10_rules.ino.
  TasmotaGlobal.rules_flag.data = 0;
  SystemEvents.teleperiod = false;
}

/*********************************************************************************************\
 * Cross-translation-unit setters
 *
 * `xdrv_10_rules.ino` (in USE_RULES builds) needs to write a couple of fields in
 * `struct SYSTEM_EVENTS`, but the struct definition above is file-scoped and not visible
 * from `xdrv_10_rules.ino` (each .ino is a separate translation unit). Rather than promote
 * the struct definition to a shared header just to expose two fields, these tiny
 * inline-style setters are exposed as `extern` from the rules dispatcher. The cost is one
 * extra function call per access; the benefit is keeping `struct SYSTEM_EVENTS` private to
 * this producer.
 *
 * - `SystemEventsSetTeleperiod` mirrors `Rules.teleperiod` around
 *   `FUNC_TELEPERIOD_RULES_PROCESS` so `SystemEventsEvery50ms` sees the right value.
 * - `SystemEventsSetNewPower` is called from `RulesSetPower` (Rules' `FUNC_SET_POWER` hook)
 *   to update the snapshot of the current power mask so the next 50ms tick can detect the
 *   transition and emit `Power<i>#State`.
\*********************************************************************************************/

void SystemEventsSetTeleperiod(bool v) {
  SystemEvents.teleperiod = v;
}

void SystemEventsSetNewPower(int32_t v) {
  SystemEvents.new_power = v;
}

/*********************************************************************************************\
 * 50ms producer tick
 *
 * Verbatim port of the Power/Dimmer/Event/rules_flag branches of RulesEvery50ms() from
 * xdrv_10_rules.ino, MINUS the Vars/Mems-events branch (which stays in Rules) and MINUS the
 * `Settings->rule_enabled` outer gate (this producer always runs whenever its dispatcher
 * calls it; the active RulesProcessEvent owns its own re-entry guard).
 *
 * The producer is split into two halves so the Rules dispatcher can interleave its
 * Rules-engine-only Vars/Mems drain between them, restoring the original strict-priority
 * else-if ladder:
 *
 *   1. SystemEventsEvery50msPowerDimmerEvent() — Power, Dimmer, queued Event (returns true
 *      if any branch emitted, telling the caller to yield this tick).
 *   2. (Rules dispatcher) Vars/Mems drain.
 *   3. SystemEventsEvery50msRulesFlag() — rules_flag bitfield drain (one bit per tick).
 *
 * The combined SystemEventsEvery50ms() wrapper below preserves the legacy single-call API
 * and is used by the standalone dispatcher (neither USE_RULES nor USE_SCRIPT) where there
 * is no Vars/Mems drain to interleave.
 *
 * At-most-one-event-per-tick discipline: each branch that emits MUST `return` before
 * subsequent branches are evaluated. Branch 1 (boot fan-out) is the documented exception
 * during the very first tick, where multiple Power#Boot and switch Boot events are emitted
 * back-to-back.
 *
 * Strict-priority drain inside the rules_flag bitfield mirrors today's ordering exactly:
 *   system_init > system_boot > time_init > time_set
 *     > mqtt_connected > mqtt_disconnected
 *     > wifi_connected > wifi_disconnected
 *     > eth_connected > eth_disconnected   (ESP32 + USE_ETHERNET only)
 *     > http_init
 *     > shutter_moved > shutter_moving     (USE_SHUTTER only)
\*********************************************************************************************/

// Returns true if a Power/Dimmer/Event branch emitted (caller should yield this tick).
bool SystemEventsEvery50msPowerDimmerEvent(void) {
  char json_event[RULE_MAX_EVENTSZ + 16];  // Add 16 chars for {"Event": .. }

  if (-1 == SystemEvents.new_power) { SystemEvents.new_power = TasmotaGlobal.power; }

  // Branch 1: Power state / boot
  if (SystemEvents.new_power != SystemEvents.old_power) {
    if (SystemEvents.old_power != -1) {
      for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
        uint8_t new_state = (SystemEvents.new_power >> i) & 1;
        if (new_state != ((SystemEvents.old_power >> i) & 1)) {
          snprintf_P(json_event, sizeof(json_event), PSTR("{\"Power%d\":{\"State\":%d}}"), i + 1, new_state);
          RulesProcessEvent(json_event);
        }
      }
    } else {
      // Boot time POWER OUTPUTS (Relays) Status
      for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
        uint8_t new_state = (SystemEvents.new_power >> i) & 1;
        snprintf_P(json_event, sizeof(json_event), PSTR("{\"Power%d\":{\"Boot\":%d}}"), i + 1, new_state);
        RulesProcessEvent(json_event);
      }
      // Boot time SWITCHES Status
      for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
        if (SwitchUsed(i)) {
          snprintf_P(json_event, sizeof(json_event), PSTR("{\"%s\":{\"Boot\":%d}}"), GetSwitchText(i).c_str(), (SwitchState(i)));
          RulesProcessEvent(json_event);
        }
      }
    }
    SystemEvents.old_power = SystemEvents.new_power;
    return true;
  }

  // Branch 2: Dimmer state / boot
  if (SystemEvents.old_dimm != Settings->light_dimmer) {
    if (SystemEvents.old_dimm != -1) {
      snprintf_P(json_event, sizeof(json_event), PSTR("{\"Dimmer\":{\"State\":%d}}"), Settings->light_dimmer);
    } else {
      // Boot time DIMMER VALUE
      snprintf_P(json_event, sizeof(json_event), PSTR("{\"Dimmer\":{\"Boot\":%d}}"), Settings->light_dimmer);
    }
    RulesProcessEvent(json_event);
    SystemEvents.old_dimm = Settings->light_dimmer;
    return true;
  }

  // Branch 3: queued `Event` command (single-slot queue)
  if (SystemEvents.event_data[0]) {
    char *event;
    char *parameter;
    event = strtok_r(SystemEvents.event_data, "=", &parameter);  // SystemEvents.event_data = fanspeed=10
    if (event) {
      event = Trim(event);
      if (parameter) {
        parameter = Trim(parameter);
      } else {
        parameter = event + strlen(event);  // '\0'
      }
      bool quotes = (parameter[0] != '{');
      snprintf_P(json_event, sizeof(json_event), PSTR("{\"Event\":{\"%s\":%s%s%s}}"), event, (quotes) ? "\"" : "", parameter, (quotes) ? "\"" : "");
      SystemEvents.event_data[0] = '\0';
      RulesProcessEvent(json_event);
    } else {
      SystemEvents.event_data[0] = '\0';
    }
    return true;
  }

  return false;
}

// rules_flag bitfield drainage — strict priority, one bit per tick.
void SystemEventsEvery50msRulesFlag(void) {
  if (TasmotaGlobal.rules_flag.data) {
    char json_event[RULE_MAX_EVENTSZ + 16];
    json_event[0] = '\0';
    if (TasmotaGlobal.rules_flag.system_init) {
      TasmotaGlobal.rules_flag.system_init = 0;
      strncpy_P(json_event, PSTR("{\"System\":{\"Init\":1}}"), sizeof(json_event));
    }
    else if (TasmotaGlobal.rules_flag.system_boot) {
      TasmotaGlobal.rules_flag.system_boot = 0;
      strncpy_P(json_event, PSTR("{\"System\":{\"Boot\":1}}"), sizeof(json_event));
    }
    else if (TasmotaGlobal.rules_flag.time_init) {
      TasmotaGlobal.rules_flag.time_init = 0;
      snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Initialized\":%d}}"), MinutesPastMidnight());
    }
    else if (TasmotaGlobal.rules_flag.time_set) {
      TasmotaGlobal.rules_flag.time_set = 0;
      snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Set\":%d}}"), MinutesPastMidnight());
    }
    else if (TasmotaGlobal.rules_flag.mqtt_connected) {
      TasmotaGlobal.rules_flag.mqtt_connected = 0;
      strncpy_P(json_event, PSTR("{\"MQTT\":{\"Connected\":1}}"), sizeof(json_event));
    }
    else if (TasmotaGlobal.rules_flag.mqtt_disconnected) {
      TasmotaGlobal.rules_flag.mqtt_disconnected = 0;
      strncpy_P(json_event, PSTR("{\"MQTT\":{\"Disconnected\":1}}"), sizeof(json_event));
    }
    else if (TasmotaGlobal.rules_flag.wifi_connected) {
      TasmotaGlobal.rules_flag.wifi_connected = 0;
      strncpy_P(json_event, PSTR("{\"WIFI\":{\"Connected\":1}}"), sizeof(json_event));
    }
    else if (TasmotaGlobal.rules_flag.wifi_disconnected) {
      TasmotaGlobal.rules_flag.wifi_disconnected = 0;
      strncpy_P(json_event, PSTR("{\"WIFI\":{\"Disconnected\":1}}"), sizeof(json_event));
    }
#if defined(ESP32) && defined(USE_ETHERNET)
    else if (TasmotaGlobal.rules_flag.eth_connected) {
      TasmotaGlobal.rules_flag.eth_connected = 0;
      strncpy_P(json_event, PSTR("{\"ETH\":{\"Connected\":1}}"), sizeof(json_event));
    }
    else if (TasmotaGlobal.rules_flag.eth_disconnected) {
      TasmotaGlobal.rules_flag.eth_disconnected = 0;
      strncpy_P(json_event, PSTR("{\"ETH\":{\"Disconnected\":1}}"), sizeof(json_event));
    }
#endif  // ESP32 && USE_ETHERNET
    else if (TasmotaGlobal.rules_flag.http_init) {
      TasmotaGlobal.rules_flag.http_init = 0;
      strncpy_P(json_event, PSTR("{\"HTTP\":{\"Initialized\":1}}"), sizeof(json_event));
    }
#ifdef USE_SHUTTER
    else if (TasmotaGlobal.rules_flag.shutter_moved) {
      TasmotaGlobal.rules_flag.shutter_moved = 0;
      strncpy_P(json_event, PSTR("{\"SHUTTER\":{\"Moved\":1}}"), sizeof(json_event));
    }
    else if (TasmotaGlobal.rules_flag.shutter_moving) {
      TasmotaGlobal.rules_flag.shutter_moving = 0;
      strncpy_P(json_event, PSTR("{\"SHUTTER\":{\"Moving\":1}}"), sizeof(json_event));
    }
#endif  // USE_SHUTTER
    if (json_event[0]) {
      RulesProcessEvent(json_event);  // Only service one event within 50ms
    }
  }
}

// Legacy single-call wrapper. Used by the standalone Xdrv10 dispatcher in the
// "neither USE_RULES nor USE_SCRIPT" build, where there is no Vars/Mems drain to interleave.
void SystemEventsEvery50ms(void) {
  if (SystemEventsEvery50msPowerDimmerEvent()) { return; }
  SystemEventsEvery50msRulesFlag();
}

/*********************************************************************************************\
 * Per-second producer tick
 *
 * Emits Time#Minute once per RTC minute, but only after the device has been up for at least
 * 60 seconds AND the RTC reports a valid time. The 60-second guard suppresses a spurious
 * Time#Minute emission during boot when last_minute is still the sentinel (60).
 *
 * Note: this function does NOT emit Rules#Timer. The RuleTimer[] machinery and its
 * {"Rules":{"Timer":<i>}} emission remain a Rules-engine concern in xdrv_10_rules.ino;
 * Rules timers are not part of the system-event surface that we expose to Scripter or to
 * the "neither" build mode.
\*********************************************************************************************/

void SystemEventsEverySecond(void) {
  if (RtcTime.valid && (TasmotaGlobal.uptime > 60) && (RtcTime.minute != SystemEvents.last_minute)) {
    // Execute from one minute after restart every minute only once
    SystemEvents.last_minute = RtcTime.minute;
    char json_event[120];
    snprintf_P(json_event, sizeof(json_event), PSTR("{\"Time\":{\"Minute\":%d}}"), MinutesPastMidnight());
    RulesProcessEvent(json_event);
  }
}

/*********************************************************************************************\
 * Save-before-restart producer hook
 *
 * Emits System#Save right before the device persists its config and restarts, giving rules
 * (or Berry handlers in the "neither" build) a final opportunity to react. Gating against
 * Settings->rule_enabled / Rules.busy is the responsibility of the active RulesProcessEvent
 * implementation, not of this producer.
\*********************************************************************************************/

void SystemEventsSaveBeforeRestart(void) {
  RulesProcessEvent("{\"System\":{\"Save\":1}}");
}

/*********************************************************************************************\
 * `Event` command handler
 *
 * Queues a user-supplied `key=value` payload into the single-slot `SystemEvents.event_data`
 * buffer. The next `SystemEventsEvery50ms` tick will tokenize and emit it as a
 * `{"Event":{"<key>":<rendered_value>}}` JSON event (Branch 3 of the 50ms producer tick).
 *
 * This is the SINGLE definition of `CmndEvent` for every build mode. Rules' `kRulesCommands`
 * table points at this externally-defined symbol via an `extern void CmndEvent(void);` in
 * `xdrv_10_rules.ino`. Scripter does not register the `Event` command and is unaffected by
 * this symbol.
 *
 * In `USE_SCRIPT`-only builds, this function is still linked (always-compiled) but no
 * command table references it; it is dead code in that build mode. Acceptable cost for
 * keeping Scripter untouched.
\*********************************************************************************************/

void CmndEvent(void) {
  if (XdrvMailbox.data_len > 0) {
    strlcpy(SystemEvents.event_data, XdrvMailbox.data, sizeof(SystemEvents.event_data));
#ifdef USE_DEVICE_GROUPS
    if (!XdrvMailbox.grpflg) SendDeviceGroupMessage(1, DGR_MSGTYP_UPDATE, DGR_ITEM_EVENT, XdrvMailbox.data);
#endif  // USE_DEVICE_GROUPS
  }
  if (XdrvMailbox.command) ResponseCmndDone();
}

/*********************************************************************************************\
 * Conditional Xdrv10 / RulesProcessEvent / RulesProcess stub
 *
 * Compiled ONLY when neither USE_RULES nor USE_SCRIPT is defined ("neither" build mode).
 * In this mode, this file owns slot 10 and provides minimal local definitions of the
 * symbols that the rest of Tasmota expects: `RulesProcessEvent`, `RulesProcess`, and the
 * `Xdrv10` dispatcher. In USE_RULES builds, xdrv_10_rules.ino owns these symbols; in
 * USE_SCRIPT-only builds, xdrv_10_scripter.ino owns them.
 *
 * The local `RulesProcessEvent` stub provides:
 *   - Berry forwarding (under USE_BERRY) BEFORE the busy guard, matching Rules' ordering.
 *   - A re-entry guard via `SystemEvents.busy` (independent of `Rules.busy`).
 *   - The legacy `{"k":"v"}` -> `{"k":{"Data":"v"}}` rewrite on a local String copy when
 *     the event contains exactly one `:`. The rewritten string is not consumed by anyone
 *     in this build (no rule sets, no Scripter), but is computed for parity so the
 *     observable side-effects (String allocation, toUpperCase) match Rules' behavior.
\*********************************************************************************************/

#ifndef USE_RULES
#ifndef USE_SCRIPT

bool RulesProcessEvent(const char *json_event) {
#ifdef USE_BERRY
  // Events are passed to Berry before any local processing, mirroring Rules' ordering.
  // Berry has its own busy/re-entry handling internally.
  callBerryRule(json_event, SystemEvents.teleperiod);
#endif  // USE_BERRY

  if (SystemEvents.busy) { return false; }
  if (!strlen(json_event)) { return true; }

  SystemEvents.busy = true;

  // Data: rewrite — preserved bit-for-bit from xdrv_10_rules.ino's RulesProcessEvent.
  // json_event = {"INA219":{"Voltage":4.494,"Current":0.020,"Power":0.089}}  -> unchanged (two colons)
  // json_event = {"System":{"Boot":1}}                                       -> unchanged (two colons)
  // json_event = {"SerialReceived":"on"}                                     -> {"SerialReceived":{"Data":"on"}}
  String event_saved = json_event;
  char *p = strchr(json_event, ':');
  if ((p != NULL) && !(strchr(++p, ':'))) {  // Find second colon
    event_saved.replace(F(":"), F(":{\"Data\":"));
    event_saved += F("}");
  }
  event_saved.toUpperCase();
  // event_saved is intentionally not consumed further: there are no rule sets to evaluate
  // in this build mode and Scripter is absent. Computing it preserves observable behavior
  // (allocation/toUpperCase) and keeps a documented hook for future consumers.

  SystemEvents.busy = false;
  return true;
}

// Local `RulesProcess` stub — mirrors xdrv_10_rules.ino's `RulesProcess` shape so the
// `Xdrv10` dispatcher (added in task 2.3) can route `FUNC_RULES_PROCESS` and
// `FUNC_TELEPERIOD_RULES_PROCESS` here. The busy guard is checked against
// `SystemEvents.busy` (independent of `Rules.busy`, which does not exist in this build).
bool RulesProcess(void) {
  if (!SystemEvents.busy) {
    return RulesProcessEvent(XdrvMailbox.data);
  }
  return false;
}

/*********************************************************************************************\
 * Slot-10 ownership: Xdrv10 dispatcher and command table
 *
 * In this "neither" build mode (no USE_RULES, no USE_SCRIPT), this file owns slot 10. The
 * dispatcher routes the standard FUNC_* callbacks to the producer helpers defined above,
 * plus the local `RulesProcess` stub for FUNC_RULES_PROCESS / FUNC_TELEPERIOD_RULES_PROCESS.
 *
 * `D_CMND_EVENT` is defined locally because xdrv_10_rules.ino (which normally defines it)
 * is not compiled in this build mode.
\*********************************************************************************************/

#define XDRV_10                 10

#define D_CMND_EVENT "Event"

const char kSystemEventsCommands[] PROGMEM = "|" D_CMND_EVENT;

void (* const SystemEventsCommand[])(void) PROGMEM = { &CmndEvent };

bool Xdrv10(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      SystemEventsInit();
      break;
    case FUNC_EVERY_50_MSECOND:
      SystemEventsEvery50ms();
      break;
    case FUNC_EVERY_SECOND:
      SystemEventsEverySecond();
      break;
    case FUNC_SAVE_BEFORE_RESTART:
      SystemEventsSaveBeforeRestart();
      break;
    case FUNC_RULES_PROCESS:
      result = RulesProcess();
      break;
    case FUNC_TELEPERIOD_RULES_PROCESS:
      SystemEvents.teleperiod = true;
      result = RulesProcess();
      SystemEvents.teleperiod = false;
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kSystemEventsCommands, SystemEventsCommand);
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif  // !USE_SCRIPT
#endif  // !USE_RULES
