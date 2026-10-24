/*
  xsns_119_jbd_bms_ble.ino - JBD/Xiaoxiang BMS BLE reader for Tasmota

  Copyright (C) 2026

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
*/

#ifdef ESP32
#ifdef USE_BLE_ESP32
#ifdef USE_JBD_BMS_BLE

#define XSNS_119 119

#include <NimBLEDevice.h>

namespace BLE_ESP32 {
  int getAddr(uint8_t *dest, char *src);
  int getSeenDeviceCount(void);
  bool getSeenDeviceInfo(int idx, uint8_t *mac6, uint8_t *addrtype, char *name, size_t namelen, int8_t *rssi);
}

#define JBD_BMS_BLE_SERVICE       "ff00"
#define JBD_BMS_BLE_NOTIFY_CHAR   "ff01"
#define JBD_BMS_BLE_WRITE_CHAR    "ff02"
#define JBD_BMS_MAX_CELLS         32
#define JBD_BMS_MAX_TEMPS         8
#define JBD_BMS_DEFAULT_PERIOD    60
// how many BMS units can be tracked/polled at once. Only one BLE connection
// is ever open at a time (single radio), so units are polled round-robin,
// each one's own `next_poll` naturally staggering them across `JbdBmsPeriod`.
#define JBD_BMS_MAX_UNITS         4
#define JBD_BMS_MAX_PICKER_ROWS   8

enum JbdBmsCommand {
  CMND_JBDBMS_ADDR,
  CMND_JBDBMS_ADD,
  CMND_JBDBMS_DEL,
  CMND_JBDBMS_POLL,
  CMND_JBDBMS_PERIOD,
  CMND_JBDBMS_STATUS
};

enum JbdBmsOpType {
  JBD_OP_BASIC = 3,
  JBD_OP_CELLS = 4,
  JBD_OP_VERSION = 5
};

struct JbdBmsBleData {
  char addr[22];
  uint8_t configured;
  uint8_t addr_type;
  uint8_t active_ops;
  uint8_t cells;
  uint8_t temps;
  uint8_t soc;
  uint8_t sw_version;
  uint16_t cycles;
  uint16_t protection;
  uint16_t production_date;
  uint32_t balance_bits;
  uint8_t fet_status;
  uint16_t cell_mv[JBD_BMS_MAX_CELLS];
  int16_t temp_dc[JBD_BMS_MAX_TEMPS];
  uint16_t cell_min_mv;
  uint16_t cell_max_mv;
  uint16_t cell_delta_mv;
  float voltage;
  float current;
  float capacity;
  float full_capacity;
  char version[24];
  uint32_t next_poll;
  uint32_t last_seen;
  uint32_t polls;
  uint32_t errors;
  int16_t last_state;
  uint8_t valid_basic;
  uint8_t valid_cells;
};

JbdBmsBleData JbdBmsList[JBD_BMS_MAX_UNITS];
uint16_t JbdBmsPeriod = JBD_BMS_DEFAULT_PERIOD;  // shared poll interval, applies per-unit
char JbdBmsWebhook[JBD_BMS_MAX_UNITS][80] = {{0}};  // per-unit, e.g. "[192.168.1.50:7580]/batteryInfo" - persisted via JbdBmsSaveConfig()
bool JbdBmsShowOnMain = true;  // whether JbdBmsShow()/JbdBmsShowPicker() render on the main web page; toggled from the /ble config page, persisted alongside the rest

int JbdBmsOpComplete(BLE_ESP32::generic_sensor_t *op);

uint16_t JbdBmsU16(const uint8_t *p) {
  return ((uint16_t)p[0] << 8) | p[1];
}

uint16_t JbdBmsChecksum(const uint8_t *p, uint32_t len) {
  uint32_t sum = 0;
  for (uint32_t i = 0; i < len; i++) {
    sum += p[i];
  }
  return (uint16_t)(0x10000 - sum);
}

void JbdBmsMakeReadCommand(uint8_t command, uint8_t *frame) {
  frame[0] = 0xDD;
  frame[1] = 0xA5;
  frame[2] = command;
  frame[3] = 0x00;
  uint16_t crc = JbdBmsChecksum(frame + 2, 2);
  frame[4] = crc >> 8;
  frame[5] = crc & 0xFF;
  frame[6] = 0x77;
}

void JbdBmsMacToStr(const uint8_t *mac, char *out13) {
  snprintf_P(out13, 13, PSTR("%02X%02X%02X%02X%02X%02X"), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

bool JbdBmsNormalizeAddr(const char *src, char *dest, size_t dest_len, uint8_t *out_type) {
  if (!src || !src[0] || !dest || dest_len < 14) { return false; }

  char compact[16];
  uint32_t out = 0;
  uint8_t type = 0;

  for (uint32_t i = 0; src[i] && out < sizeof(compact) - 1; i++) {
    char c = src[i];
    if (':' == c || '-' == c || ' ' == c) { continue; }
    if ('/' == c) {
      if (src[i + 1] >= '0' && src[i + 1] <= '3') {
        type = src[i + 1] - '0';
      }
      break;
    }
    if (!isxdigit((unsigned char)c)) { return false; }
    compact[out++] = toupper((unsigned char)c);
  }
  compact[out] = '\0';
  if (12 != out) { return false; }

  snprintf(dest, dest_len, "%s/%u", compact, type);
  if (out_type) { *out_type = type; }
  return true;
}

#define JBD_BMS_CFG_FILE "/jbdbms.cfg"

// persists tracked units / period / webhooks / main-page visibility across
// reboot as a small text file on the filesystem partition (USE_UFILESYS) -
// NOT in Settings, to avoid touching the shared Settings struct/layout for
// a driver this custom. One line per item: "P:<period>", "U:<slot>:<addr>",
// "W:<slot>:<webhook>", "S:<0|1>" (JbdBmsShowOnMain).
void JbdBmsSaveConfig(void) {
#ifdef USE_UFILESYS
  char buf[700];
  int len = snprintf_P(buf, sizeof(buf), PSTR("P:%u\nS:%u\n"), JbdBmsPeriod, JbdBmsShowOnMain ? 1 : 0);
  for (int i = 0; i < JBD_BMS_MAX_UNITS && len < (int)sizeof(buf) - 4; i++) {
    if (JbdBmsList[i].configured) {
      len += snprintf_P(buf + len, sizeof(buf) - len, PSTR("U:%d:%s\n"), i, JbdBmsList[i].addr);
    }
    if (JbdBmsWebhook[i][0] && len < (int)sizeof(buf) - 4) {
      len += snprintf_P(buf + len, sizeof(buf) - len, PSTR("W:%d:%s\n"), i, JbdBmsWebhook[i]);
    }
  }
  if (len > 0 && len < (int)sizeof(buf)) {
    TfsSaveFile(JBD_BMS_CFG_FILE, (const uint8_t *)buf, len);
  }
#endif  // USE_UFILESYS
}

void JbdBmsLoadConfig(void) {
#ifdef USE_UFILESYS
  if (!TfsFileExists(JBD_BMS_CFG_FILE)) { return; }
  String s = TfsLoadString(JBD_BMS_CFG_FILE);
  char buf[700];
  strlcpy(buf, s.c_str(), sizeof(buf));

  char *saveptr = nullptr;
  char *line = strtok_r(buf, "\n", &saveptr);
  while (line) {
    if ('P' == line[0] && ':' == line[1]) {
      uint32_t period = atoi(line + 2);
      JbdBmsPeriod = (period > 3600) ? 3600 : period;
    } else if ('S' == line[0] && ':' == line[1]) {
      JbdBmsShowOnMain = (atoi(line + 2) != 0);
    } else if (('U' == line[0] || 'W' == line[0]) && ':' == line[1]) {
      int slot = atoi(line + 2);
      char *value = strchr(line + 2, ':');
      if (value && slot >= 0 && slot < JBD_BMS_MAX_UNITS) {
        value++;  // skip the ':'
        if ('U' == line[0]) {
          char normalized[sizeof(JbdBmsList[0].addr)];
          uint8_t type = 0;
          if (JbdBmsNormalizeAddr(value, normalized, sizeof(normalized), &type)) {
            strlcpy(JbdBmsList[slot].addr, normalized, sizeof(JbdBmsList[slot].addr));
            JbdBmsList[slot].addr_type = type;
            JbdBmsList[slot].configured = 1;
          }
        } else {
          strlcpy(JbdBmsWebhook[slot], value, sizeof(JbdBmsWebhook[slot]));
        }
      }
    }
    line = strtok_r(nullptr, "\n", &saveptr);
  }
#endif  // USE_UFILESYS
}

// find the slot (if any) tracking this MAC (compares just the 12 hex chars, ignores addr type)
int JbdBmsFindSlotByMac(const char *macstr12) {
  for (int i = 0; i < JBD_BMS_MAX_UNITS; i++) {
    if (JbdBmsList[i].configured && !strncmp(JbdBmsList[i].addr, macstr12, 12)) { return i; }
  }
  return -1;
}

int JbdBmsFindFreeSlot(void) {
  for (int i = 0; i < JBD_BMS_MAX_UNITS; i++) {
    if (!JbdBmsList[i].configured) { return i; }
  }
  return -1;
}

bool JbdBmsAnyActive(void) {
  for (int i = 0; i < JBD_BMS_MAX_UNITS; i++) {
    if (JbdBmsList[i].active_ops) { return true; }
  }
  return false;
}

bool JbdBmsGetAddr(int slot, uint8_t *addr) {
  if (slot < 0 || slot >= JBD_BMS_MAX_UNITS || !JbdBmsList[slot].configured) { return false; }

  char tmp[sizeof(JbdBmsList[slot].addr)];
  strlcpy(tmp, JbdBmsList[slot].addr, sizeof(tmp));
  return BLE_ESP32::getAddr(addr, tmp);
}

bool JbdBmsValidateFrame(const uint8_t *frame, uint32_t len, uint8_t expected_command, uint8_t *payload_len) {
  if (!frame || len < 7 || 0xDD != frame[0] || 0x77 != frame[len - 1]) { return false; }
  if (expected_command != frame[1]) { return false; }
  if (0x00 != frame[2]) { return false; }

  uint8_t data_len = frame[3];
  if ((uint32_t)data_len + 7 != len) { return false; }

  uint16_t got = ((uint16_t)frame[4 + data_len] << 8) | frame[5 + data_len];
  uint16_t crc_status = JbdBmsChecksum(frame + 2, data_len + 2);
  uint16_t crc_command = JbdBmsChecksum(frame + 1, data_len + 3);
  if (got != crc_status && got != crc_command) { return false; }

  if (payload_len) { *payload_len = data_len; }
  return true;
}

void JbdBmsParseBasic(int slot, const uint8_t *data, uint8_t len) {
  // NOTE: takes `slot` (not a JbdBmsBleData*) because Tasmota's build
  // auto-generates function prototypes near the top of the merged sketch,
  // before this struct type is visible there - a pointer-to-struct param
  // fails with "was not declared in this scope" at that early prototype.
  JbdBmsBleData *d = &JbdBmsList[slot];
  if (len < 23) { return; }

  d->voltage = JbdBmsU16(data) / 100.0f;
  d->current = (int16_t)JbdBmsU16(data + 2) / 100.0f;
  d->capacity = JbdBmsU16(data + 4) / 100.0f;
  d->full_capacity = JbdBmsU16(data + 6) / 100.0f;
  d->cycles = JbdBmsU16(data + 8);
  d->production_date = JbdBmsU16(data + 10);
  d->balance_bits = ((uint32_t)JbdBmsU16(data + 14) << 16) | JbdBmsU16(data + 12);
  d->protection = JbdBmsU16(data + 16);
  d->sw_version = data[18];
  d->soc = data[19];
  d->fet_status = data[20];
  d->cells = (data[21] <= JBD_BMS_MAX_CELLS) ? data[21] : d->cells;
  d->temps = (data[22] <= JBD_BMS_MAX_TEMPS) ? data[22] : JBD_BMS_MAX_TEMPS;

  for (uint8_t i = 0; i < d->temps; i++) {
    uint32_t offset = 23 + (i * 2);
    if (offset + 1 >= len) { break; }
    int32_t raw = JbdBmsU16(data + offset);
    d->temp_dc[i] = (int16_t)(raw - 2731);
  }

  d->valid_basic = 1;
}

void JbdBmsParseCells(int slot, const uint8_t *data, uint8_t len) {
  JbdBmsBleData *d = &JbdBmsList[slot];
  uint8_t cells = len / 2;
  if (cells > JBD_BMS_MAX_CELLS) { cells = JBD_BMS_MAX_CELLS; }
  if (!cells) { return; }

  uint16_t min_mv = 65535;
  uint16_t max_mv = 0;
  for (uint8_t i = 0; i < cells; i++) {
    uint16_t mv = JbdBmsU16(data + (i * 2));
    d->cell_mv[i] = mv;
    if (mv < min_mv) { min_mv = mv; }
    if (mv > max_mv) { max_mv = mv; }
  }

  d->cells = cells;
  d->cell_min_mv = min_mv;
  d->cell_max_mv = max_mv;
  d->cell_delta_mv = max_mv - min_mv;
  d->valid_cells = 1;
}

void JbdBmsParseVersion(int slot, const uint8_t *data, uint8_t len) {
  JbdBmsBleData *d = &JbdBmsList[slot];
  uint8_t out = 0;
  for (uint8_t i = 0; i < len && out < sizeof(d->version) - 1; i++) {
    char c = (char)data[i];
    if ('"' == c || '\\' == c) { c = '_'; }
    if (isprint((unsigned char)c)) {
      d->version[out++] = c;
    }
  }
  d->version[out] = '\0';
}

bool JbdBmsQueueOp(int slot, uint8_t command) {
  uint8_t addr[7];
  if (!JbdBmsGetAddr(slot, addr)) { return false; }

  BLE_ESP32::generic_sensor_t *op = nullptr;
  if (!BLE_ESP32::newOperation(&op)) { return false; }

  op->addr = NimBLEAddress(addr, addr[6]);
  op->serviceUUID = NimBLEUUID(JBD_BMS_BLE_SERVICE);
  op->characteristicUUID = NimBLEUUID(JBD_BMS_BLE_WRITE_CHAR);
  op->notificationCharacteristicUUID = NimBLEUUID(JBD_BMS_BLE_NOTIFY_CHAR);
  op->writelen = 7;
  JbdBmsMakeReadCommand(command, op->dataToWrite);
  op->notifyappend = 1;
  op->notifyendbyte = 0x77;
  op->writenoresponse = 1;
  op->notifyresponse = 1;
  op->subscribedelay = 500;
  op->writerepeat = 3;
  op->writerepeatinterval = 2000;
  op->completecallback = (void *)JbdBmsOpComplete;
  op->context = (void *)(uintptr_t)(((uint32_t)slot << 8) | command);

  if (!BLE_ESP32::extQueueOperation(&op)) {
    BLE_ESP32::freeOperation(&op);
    return false;
  }

  JbdBmsList[slot].active_ops++;
  return true;
}

bool JbdBmsPoll(int slot) {
  if (slot < 0 || slot >= JBD_BMS_MAX_UNITS) { return false; }
  if (!JbdBmsList[slot].configured || JbdBmsList[slot].active_ops) { return false; }

  bool queued = false;
  queued |= JbdBmsQueueOp(slot, JBD_OP_BASIC);
  queued |= JbdBmsQueueOp(slot, JBD_OP_CELLS);
  queued |= JbdBmsQueueOp(slot, JBD_OP_VERSION);

  if (queued) {
    JbdBmsList[slot].polls++;
  } else {
    JbdBmsList[slot].errors++;
  }
  return queued;
}

// Builds a WebSend command with every known field as a GET query parameter
// and fires it via ExecuteCommand. Rules can't do this on their own: the
// DO clause only ever substitutes %value% (the single field that matched
// the trigger), %VarN%/%MemN% and a fixed list of system placeholders -
// there is no "%OtherKey#Field%" cross-reference into sibling fields of the
// same JSON object (confirmed by reading RulesVarReplace() in
// xdrv_10_rules.ino). Each unit has its OWN webhook target (JbdBmsWebhook is
// per-slot, set via JBDBMSWebhook<N>) so different BMS units can be sent to
// different servers. JbdBmsWebhook[slot] is expected to already be a valid
// WebSend target, e.g. "[192.168.1.50:7580]/batteryInfo" - we only append
// "?field=value&...".
void JbdBmsFireWebhook(int slot) {
  if (!JbdBmsWebhook[slot][0]) { return; }
  JbdBmsBleData *d = &JbdBmsList[slot];
  if (!d->valid_basic) { return; }

  float power = d->voltage * d->current;
  char v_v[16], v_c[16], v_p[16], v_cap[16], v_fcap[16];
  ext_snprintf_P(v_v, sizeof(v_v), PSTR("%_f"), &d->voltage);
  ext_snprintf_P(v_c, sizeof(v_c), PSTR("%_f"), &d->current);
  ext_snprintf_P(v_p, sizeof(v_p), PSTR("%_f"), &power);
  ext_snprintf_P(v_cap, sizeof(v_cap), PSTR("%_f"), &d->capacity);
  ext_snprintf_P(v_fcap, sizeof(v_fcap), PSTR("%_f"), &d->full_capacity);

  char cmd[420];
  int len = snprintf_P(cmd, sizeof(cmd),
    PSTR("WebSend %s?addr=%s&voltageFull=%s&current=%s&power=%s&soc=%u&capacity=%s&fullCapacity=%s&cycles=%u&chargeMos=%u&dischargeMos=%u&balancing=%u"),
    JbdBmsWebhook[slot], d->addr, v_v, v_c, v_p, d->soc, v_cap, v_fcap, d->cycles,
    (d->fet_status & 0x01) ? 1 : 0, (d->fet_status & 0x02) ? 1 : 0, d->balance_bits ? 1 : 0);
  if (len < 0 || len >= (int)sizeof(cmd)) { return; }

  if (d->valid_cells) {
    for (uint8_t i = 0; i < d->cells && len < (int)sizeof(cmd) - 24; i++) {
      len += snprintf_P(cmd + len, sizeof(cmd) - len, PSTR("&cell%u=%u"), i + 1, d->cell_mv[i]);
    }
    len += snprintf_P(cmd + len, sizeof(cmd) - len, PSTR("&cellMin=%u&cellMax=%u&cellDelta=%u"),
                      d->cell_min_mv, d->cell_max_mv, d->cell_delta_mv);
  }
  if (d->temps && len < (int)sizeof(cmd) - 24) {
    char v_t[16];
    float t0 = d->temp_dc[0] / 10.0f;
    ext_snprintf_P(v_t, sizeof(v_t), PSTR("%*_f"), 1, &t0);
    len += snprintf_P(cmd + len, sizeof(cmd) - len, PSTR("&temp1=%s"), v_t);
  }
  if (len < (int)sizeof(cmd) - 24) {
    snprintf_P(cmd + len, sizeof(cmd) - len, PSTR("&protection=0x%04X&version=%s"), d->protection, d->version);
  }

  AddLog(LOG_LEVEL_INFO, PSTR("JBD: [%d] webhook -> %s"), slot, cmd);
  ExecuteCommand(cmd, SRC_SENSOR);
}

int JbdBmsOpComplete(BLE_ESP32::generic_sensor_t *op) {
  if (!op) { return 1; }

  uint32_t ctx = (uint32_t)(uintptr_t)op->context;
  uint8_t command = ctx & 0xFF;
  int slot = (ctx >> 8) & 0xFF;
  if (slot < 0 || slot >= JBD_BMS_MAX_UNITS) { return 1; }
  JbdBmsBleData *d = &JbdBmsList[slot];

  if (d->active_ops) {
    d->active_ops--;
  }

  d->last_state = op->state;

  if (op->state <= GEN_STATE_FAILED || op->notifytruncated || !op->notifylen) {
    d->errors++;
    AddLog(LOG_LEVEL_ERROR, PSTR("JBD: [%d] BLE op %u failed state %d notifylen %u truncated %u"), slot, command, op->state, op->notifylen, op->notifytruncated);
    return 1;
  }

  uint8_t payload_len = 0;
  if (!JbdBmsValidateFrame(op->dataNotify, op->notifylen, command, &payload_len)) {
    d->errors++;
    AddLog(LOG_LEVEL_ERROR, PSTR("JBD: [%d] Bad frame cmd %u len %u"), slot, command, op->notifylen);
    return 1;
  }

  const uint8_t *payload = op->dataNotify + 4;
  switch (command) {
    case JBD_OP_BASIC:
      JbdBmsParseBasic(slot, payload, payload_len);
      break;
    case JBD_OP_CELLS:
      JbdBmsParseCells(slot, payload, payload_len);
      break;
    case JBD_OP_VERSION:
      JbdBmsParseVersion(slot, payload, payload_len);
      break;
  }

  d->last_seen = millis() / 1000;
  AddLog(LOG_LEVEL_INFO, PSTR("JBD: [%d] BLE op %u ok"), slot, command);

  if (JBD_OP_VERSION == command) {
    // last of the 3 sub-ops in a poll cycle (see JbdBmsPoll) - by now Basic
    // and Cells have already been parsed too, so this is a fully fresh
    // snapshot. Fire Rules immediately with it instead of waiting for the
    // next scheduled tele/SENSOR publish (which may be much less frequent
    // than JbdBmsPeriod, and re-fires on every tele cycle regardless of
    // whether new data actually arrived). Mirrors the pattern used by other
    // Tasmota BLE sensor drivers (e.g. xsns_62_esp32_mi_ble.ino).
    Response_P(PSTR("{"));
    JbdBmsAppendJsonOne(slot, false);  // already closes the JSON object itself
    XdrvRulesProcess(0);

    // Tasmota Rules only expose %value% (the single field that matched the
    // trigger) - there is NO "%OtherKey#Field%" cross-reference into other
    // fields of the same JSON object (confirmed empirically: only %VALUE%,
    // %VarN%, %MemN% and a fixed list of system placeholders are ever
    // substituted, see RulesVarReplace() in xdrv_10_rules.ino). So a Rule
    // can't build a "send everything" webhook on its own. We build and fire
    // it here instead, in C++, where every field is a real typed value.
    JbdBmsFireWebhook(slot);
  }
  return 1;
}

void JbdBmsEverySecond(void) {
  if (!JbdBmsPeriod || JbdBmsAnyActive()) { return; }

  uint32_t now = millis() / 1000;
  for (int i = 0; i < JBD_BMS_MAX_UNITS; i++) {
    if (!JbdBmsList[i].configured) { continue; }
    if (now >= JbdBmsList[i].next_poll) {
      JbdBmsList[i].next_poll = now + JbdBmsPeriod;
      if (JbdBmsPoll(i)) { return; }  // one connection at a time; rest follow on later ticks
    }
  }
}

void JbdBmsAppendJsonOne(int slot, bool leading_comma) {
  JbdBmsBleData *d = &JbdBmsList[slot];
  if (!d->configured) { return; }

  uint32_t now = millis() / 1000;
  uint32_t age = (d->last_seen && now >= d->last_seen) ? now - d->last_seen : 0;

  ResponseAppend_P(PSTR("%s\"JBDBMS%d\":{\"Addr\":\"%s\",\"Configured\":%u,\"Active\":%u,\"Polls\":%u,\"Errors\":%u,\"LastState\":%d"),
                   leading_comma ? "," : "", slot + 1, d->addr,
                   d->configured, d->active_ops, d->polls, d->errors, d->last_state);

  if (d->last_seen) {
    ResponseAppend_P(PSTR(",\"Age\":%u"), age);
  }
  if (d->valid_basic) {
    // NOTE: this build links against picolibc's integer-only vfprintf (see
    // pio-tools/picolibc_flags.py), so plain "%f"/"%.2f" can't format floats
    // and prints the literal placeholder "*float*" instead. Tasmota's own
    // "%_f"/"%*_f" specifiers route through ext_snprintf_P (dtostrf-based)
    // and work correctly here; they take the float BY ADDRESS (varargs
    // otherwise promote float to double, which dtostrf can't read back).
    float power = d->voltage * d->current;
    ResponseAppend_P(PSTR(",\"Voltage\":%_f,\"Current\":%_f,\"Power\":%_f"),
                     &d->voltage, &d->current, &power);
    ResponseAppend_P(PSTR(",\"SOC\":%u,\"Capacity\":%_f,\"FullCapacity\":%_f,\"Cycles\":%u,\"Protection\":\"0x%04X\",\"SWVersion\":%u"),
                     d->soc, &d->capacity, &d->full_capacity,
                     d->cycles, d->protection, d->sw_version);
    ResponseAppend_P(PSTR(",\"ChargeMOS\":%u,\"DischargeMOS\":%u,\"Balancing\":%u,\"BalanceBits\":\"0x%08X\""),
                     (d->fet_status & 0x01) ? 1 : 0, (d->fet_status & 0x02) ? 1 : 0,
                     d->balance_bits ? 1 : 0, d->balance_bits);
    if (d->production_date) {
      ResponseAppend_P(PSTR(",\"ProductionDate\":\"%u-%02u-%02u\""),
                       2000 + (d->production_date >> 9), (d->production_date >> 5) & 0x0F, d->production_date & 0x1F);
    }
    for (uint8_t i = 0; i < d->temps; i++) {
      float temp_c = d->temp_dc[i] / 10.0f;
      ResponseAppend_P(PSTR(",\"Temp%u\":%*_f"), i + 1, 1, &temp_c);
    }
  }
  if (d->valid_cells) {
    ResponseAppend_P(PSTR(",\"Cells\":%u,\"CellMin\":%u,\"CellMax\":%u,\"CellDelta\":%u"),
                     d->cells, d->cell_min_mv, d->cell_max_mv, d->cell_delta_mv);
    for (uint8_t i = 0; i < d->cells; i++) {
      ResponseAppend_P(PSTR(",\"Cell%u\":%u"), i + 1, d->cell_mv[i]);
    }
  }
  if (d->version[0]) {
    ResponseAppend_P(PSTR(",\"Version\":\"%s\""), d->version);
  }
  ResponseJsonEnd();
}

void JbdBmsAppendJson(bool leading_comma) {
  for (int i = 0; i < JBD_BMS_MAX_UNITS; i++) {
    JbdBmsAppendJsonOne(i, leading_comma);
    if (JbdBmsList[i].configured) { leading_comma = true; }
  }
}

// two metrics per row instead of Tasmota's usual one-per-row {s}/{m}/{e}
// convention, to keep multi-unit pages compact: hand-written <tr><td> HTML
// (the {s}/{m}/{e} tokens only produce a fixed 2-column th/td pair, no way
// to fit two label/value pairs in one row with them).
void JbdBmsShowOne(int slot) {
  JbdBmsBleData *d = &JbdBmsList[slot];
  if (!d->configured || !d->valid_basic) { return; }

  uint32_t now = millis() / 1000;
  uint32_t age = (d->last_seen && now >= d->last_seen) ? now - d->last_seen : 0;
  float power = d->voltage * d->current;
  char a[24], b[24];

  WSContentSend_P(PSTR("<tr><td colspan=4 style='font-weight:bold;padding-top:8px'>JBD " D_BATTERY " #%d %s</td></tr>"),
                  slot + 1, d->version[0] ? d->version : PSTR(""));

  ext_snprintf_P(a, sizeof(a), PSTR("%_f " D_UNIT_VOLT), &d->voltage);
  ext_snprintf_P(b, sizeof(b), PSTR("%_f " D_UNIT_AMPERE), &d->current);
  WSContentSend_P(PSTR("<tr><td>" D_VOLTAGE "</td><td>%s</td><td>" D_CURRENT "</td><td>%s</td></tr>"), a, b);

  ext_snprintf_P(a, sizeof(a), PSTR("%_f " D_UNIT_WATT), &power);
  WSContentSend_P(PSTR("<tr><td>" D_POWERUSAGE "</td><td>%s</td><td>" D_BATTERY_CHARGE "</td><td>%u " D_UNIT_PERCENT "</td></tr>"), a, d->soc);

  ext_snprintf_P(a, sizeof(a), PSTR("%_f / %_f Ah"), &d->capacity, &d->full_capacity);
  WSContentSend_P(PSTR("<tr><td>" D_CAPACITY "</td><td>%s</td><td>Cycles</td><td>%u</td></tr>"), a, d->cycles);

  WSContentSend_P(PSTR("<tr><td>Chg / Dis MOS</td><td>%s / %s</td><td>Balancing</td><td>%s</td></tr>"),
                  (d->fet_status & 0x01) ? PSTR("ON") : PSTR("OFF"),
                  (d->fet_status & 0x02) ? PSTR("ON") : PSTR("OFF"),
                  d->balance_bits ? PSTR("Yes") : PSTR("No"));

  if (d->valid_cells && d->cells) {
    char cells_str[JBD_BMS_MAX_CELLS * 8];
    uint32_t out = 0;
    for (uint8_t i = 0; i < d->cells && out < sizeof(cells_str) - 6; i++) {
      out += snprintf_P(cells_str + out, sizeof(cells_str) - out, PSTR("%s%u"), i ? PSTR(" / ") : PSTR(""), d->cell_mv[i]);
    }
    WSContentSend_P(PSTR("<tr><td>Cells (mV)</td><td colspan=3>%s</td></tr>"), cells_str);
  }

  if (d->valid_cells && d->temps) {
    float t0 = d->temp_dc[0] / 10.0f;
    ext_snprintf_P(a, sizeof(a), PSTR("%*_f " D_UNIT_CELSIUS), 1, &t0);
    WSContentSend_P(PSTR("<tr><td>Cell Delta</td><td>%u " D_UNIT_MILLIVOLT "</td><td>" D_TEMPERATURE " 1</td><td>%s</td></tr>"), d->cell_delta_mv, a);
  } else if (d->valid_cells) {
    WSContentSend_P(PSTR("<tr><td>Cell Delta</td><td colspan=3>%u " D_UNIT_MILLIVOLT "</td></tr>"), d->cell_delta_mv);
  }

  for (uint8_t i = 1; i < d->temps; i += 2) {
    float ta = d->temp_dc[i] / 10.0f;
    ext_snprintf_P(a, sizeof(a), PSTR("%*_f " D_UNIT_CELSIUS), 1, &ta);
    if (i + 1 < d->temps) {
      float tb = d->temp_dc[i + 1] / 10.0f;
      ext_snprintf_P(b, sizeof(b), PSTR("%*_f " D_UNIT_CELSIUS), 1, &tb);
      WSContentSend_P(PSTR("<tr><td>" D_TEMPERATURE " %u</td><td>%s</td><td>" D_TEMPERATURE " %u</td><td>%s</td></tr>"), i + 1, a, i + 2, b);
    } else {
      WSContentSend_P(PSTR("<tr><td>" D_TEMPERATURE " %u</td><td colspan=3>%s</td></tr>"), i + 1, a);
    }
  }

  if (d->protection) {
    WSContentSend_P(PSTR("<tr><td>Protection</td><td>0x%04X</td><td>Last update</td><td>%us ago</td></tr>"), d->protection, age);
  } else {
    WSContentSend_P(PSTR("<tr><td>Last update</td><td colspan=3>%us ago</td></tr>"), age);
  }
}

void JbdBmsShow(void) {
  for (int i = 0; i < JBD_BMS_MAX_UNITS; i++) {
    JbdBmsShowOne(i);
  }
}

// lets you build the polled-BMS list straight from the main page, without the
// console: lists BLE devices the scanner has recently seen (name/RSSI where
// known), each with a one-click Add/Remove link. Several can be added - they
// are then polled round-robin (one BLE connection at a time, see
// JbdBmsEverySecond) and each gets its own block from JbdBmsShow above.
// Collapsed behind <details> so it doesn't clutter the page by default.
void JbdBmsShowPicker(void) {
  int total = BLE_ESP32::getSeenDeviceCount();
  bool covered[JBD_BMS_MAX_UNITS] = {false};
  int configured_count = 0;
  for (int i = 0; i < JBD_BMS_MAX_UNITS; i++) {
    if (JbdBmsList[i].configured) { configured_count++; }
  }
  if (!total && !configured_count) { return; }

  WSContentSend_P(PSTR("{s}JBD BMS devices{m}<details id='jbdpick'><summary>%d tracked, %d seen - tap to manage</summary>"),
                  configured_count, total);
  WSContentSend_P(PSTR("<table style='width:100%%;font-size:0.9em'>"));

  int shown = 0;
  for (int i = 0; i < total && shown < JBD_BMS_MAX_PICKER_ROWS; i++) {
    uint8_t mac[6];
    uint8_t addrtype = 0;
    char name[BLE_ESP32_MAXNAMELEN + 1];
    int8_t rssi = 0;
    name[0] = '\0';
    if (!BLE_ESP32::getSeenDeviceInfo(i, mac, &addrtype, name, sizeof(name), &rssi)) { continue; }

    char macstr[13];
    JbdBmsMacToStr(mac, macstr);
    int slot = JbdBmsFindSlotByMac(macstr);
    if (slot >= 0) { covered[slot] = true; }

    if (slot >= 0) {
      WSContentSend_P(PSTR("<tr><td>%s%s%s</td><td>%d dBm</td><td>#%d &nbsp; <a href='#' onclick='return jbdCmd(\"JBDBMSDel %s/%d\")'>Remove</a></td></tr>"),
                      macstr, name[0] ? " " : "", name, rssi, slot + 1, macstr, addrtype);
    } else if (JbdBmsFindFreeSlot() >= 0) {
      WSContentSend_P(PSTR("<tr><td>%s%s%s</td><td>%d dBm</td><td><a href='#' onclick='return jbdCmd(\"JBDBMSAdd %s/%d\")'>Add</a></td></tr>"),
                      macstr, name[0] ? " " : "", name, rssi, macstr, addrtype);
    } else {
      WSContentSend_P(PSTR("<tr><td>%s%s%s</td><td>%d dBm</td><td>(list full)</td></tr>"),
                      macstr, name[0] ? " " : "", name, rssi);
    }
    shown++;
  }

  // tracked units that are configured but not currently seen in the last scan pass - still offer Remove
  for (int i = 0; i < JBD_BMS_MAX_UNITS; i++) {
    if (JbdBmsList[i].configured && !covered[i]) {
      WSContentSend_P(PSTR("<tr><td>%.12s (not seen)</td><td>-</td><td>#%d &nbsp; <a href='#' onclick='return jbdCmd(\"JBDBMSDel %.12s/%d\")'>Remove</a></td></tr>"),
                      JbdBmsList[i].addr, i + 1, JbdBmsList[i].addr, JbdBmsList[i].addr_type);
    }
  }

  WSContentSend_P(PSTR("</table></details>{e}"));
}

void JbdBmsStatusResponse(void) {
  Response_P(PSTR("{"));
  JbdBmsAppendJson(false);
  ResponseJsonEnd();
}

void CmndJbdBmsAddr(void) {
  // legacy single-device command: clears every tracked unit, then tracks
  // exactly the given address in slot 0. Prefer JBDBMSAdd/JBDBMSDel (or the
  // main-page picker) to track more than one unit at a time.
  if (XdrvMailbox.data_len > 0) {
    char normalized[sizeof(JbdBmsList[0].addr)];
    uint8_t type = 0;
    if (JbdBmsNormalizeAddr(XdrvMailbox.data, normalized, sizeof(normalized), &type)) {
      memset(JbdBmsList, 0, sizeof(JbdBmsList));
      strlcpy(JbdBmsList[0].addr, normalized, sizeof(JbdBmsList[0].addr));
      JbdBmsList[0].addr_type = type;
      JbdBmsList[0].configured = 1;
      JbdBmsList[0].next_poll = 0;
      JbdBmsSaveConfig();
    } else {
      ResponseCmndChar(PSTR("InvalidAddr"));
      return;
    }
  }
  ResponseCmndChar(JbdBmsList[0].configured ? JbdBmsList[0].addr : PSTR(""));
}

void CmndJbdBmsAdd(void) {
  if (!XdrvMailbox.data_len) {
    ResponseCmndChar(PSTR("NeedAddr"));
    return;
  }
  char normalized[sizeof(JbdBmsList[0].addr)];
  uint8_t type = 0;
  if (!JbdBmsNormalizeAddr(XdrvMailbox.data, normalized, sizeof(normalized), &type)) {
    ResponseCmndChar(PSTR("InvalidAddr"));
    return;
  }
  int slot = JbdBmsFindSlotByMac(normalized);
  if (slot < 0) { slot = JbdBmsFindFreeSlot(); }
  if (slot < 0) {
    ResponseCmndChar(PSTR("ListFull"));
    return;
  }
  memset(&JbdBmsList[slot], 0, sizeof(JbdBmsList[slot]));
  strlcpy(JbdBmsList[slot].addr, normalized, sizeof(JbdBmsList[slot].addr));
  JbdBmsList[slot].addr_type = type;
  JbdBmsList[slot].configured = 1;
  JbdBmsList[slot].next_poll = 0;
  JbdBmsSaveConfig();
  ResponseCmndChar(JbdBmsList[slot].addr);
}

void CmndJbdBmsDel(void) {
  if (!XdrvMailbox.data_len) {
    ResponseCmndChar(PSTR("NeedAddr"));
    return;
  }
  char normalized[sizeof(JbdBmsList[0].addr)];
  uint8_t type = 0;
  if (!JbdBmsNormalizeAddr(XdrvMailbox.data, normalized, sizeof(normalized), &type)) {
    ResponseCmndChar(PSTR("InvalidAddr"));
    return;
  }
  int slot = JbdBmsFindSlotByMac(normalized);
  if (slot < 0) {
    ResponseCmndChar(PSTR("NotFound"));
    return;
  }
  memset(&JbdBmsList[slot], 0, sizeof(JbdBmsList[slot]));
  JbdBmsSaveConfig();
  ResponseCmndChar(PSTR("Removed"));
}

void CmndJbdBmsPoll(void) {
  bool any_configured = false;
  for (int i = 0; i < JBD_BMS_MAX_UNITS; i++) {
    if (JbdBmsList[i].configured) {
      any_configured = true;
      JbdBmsList[i].next_poll = 0;  // due immediately, actual start still serialized by JbdBmsEverySecond
    }
  }
  if (!any_configured) {
    ResponseCmndChar(PSTR("NoAddr"));
  } else if (JbdBmsAnyActive()) {
    ResponseCmndChar(PSTR("Busy"));
  } else {
    ResponseCmndChar(PSTR("Queued"));
  }
}

void CmndJbdBmsPeriod(void) {
  if (XdrvMailbox.data_len > 0) {
    uint32_t period = XdrvMailbox.payload;
    if (period > 3600) { period = 3600; }
    JbdBmsPeriod = period;
    JbdBmsSaveConfig();
  }
  ResponseCmndNumber(JbdBmsPeriod);
}

void CmndJbdBmsStatus(void) {
  JbdBmsStatusResponse();
}

void CmndJbdBmsWebhook(void) {
  // per-unit WebSend target, e.g. "JBDBMSWebhook1 [192.168.1.50:7580]/batteryInfo"
  // - the "1" matches the "JBDBMS1" JSON key (slot+1), so different BMS
  // units can be pointed at different servers. No index (bare "JBDBMSWebhook")
  // defaults to unit 1. Fired automatically (with every field appended as
  // ?query=params) each time that unit finishes a fresh poll - see
  // JbdBmsFireWebhook(). Empty/"0" = disabled.
  int idx = (XdrvMailbox.index < 1) ? 1 : XdrvMailbox.index;
  if (idx > JBD_BMS_MAX_UNITS) {
    ResponseCmndChar(PSTR("InvalidIndex"));
    return;
  }
  int slot = idx - 1;
  if (XdrvMailbox.data_len > 0) {
    strlcpy(JbdBmsWebhook[slot], (1 == XdrvMailbox.data_len && '0' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data, sizeof(JbdBmsWebhook[slot]));
    JbdBmsSaveConfig();
  }
  ResponseCmndIdxChar(JbdBmsWebhook[slot][0] ? JbdBmsWebhook[slot] : PSTR(""));
}

const char kJbdBmsCommands[] PROGMEM = "JBDBMS|Addr|Add|Del|Poll|Period|Status|Webhook";
void (* const JbdBmsCommands[])(void) PROGMEM = {
  &CmndJbdBmsAddr,
  &CmndJbdBmsAdd,
  &CmndJbdBmsDel,
  &CmndJbdBmsPoll,
  &CmndJbdBmsPeriod,
  &CmndJbdBmsStatus,
  &CmndJbdBmsWebhook
};

bool Xsns119(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      memset(JbdBmsList, 0, sizeof(JbdBmsList));
      memset(JbdBmsWebhook, 0, sizeof(JbdBmsWebhook));
      JbdBmsPeriod = JBD_BMS_DEFAULT_PERIOD;
      JbdBmsLoadConfig();  // restores tracked units / period / webhooks from flash, if any were saved
      break;
    case FUNC_EVERY_SECOND:
      JbdBmsEverySecond();
      break;
    case FUNC_JSON_APPEND:
      JbdBmsAppendJson(true);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      if (JbdBmsShowOnMain) {
        JbdBmsShow();
        JbdBmsShowPicker();
      }
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:
      // FUNC_WEB_SENSOR content lives inside #l1, which Tasmota's own JS
      // fully replaces (innerHTML) every ~2.3s to refresh live sensor data -
      // that wipes any DOM/attribute state, so our <details id='jbdpick'>
      // picker would snap shut right after being opened, AND any <script>
      // tag inside that content never runs at all (assigning .innerHTML
      // does not execute embedded <script> elements). This hook instead
      // renders once, in the static part of the page (outside #l1), so its
      // <script> actually executes - it (a) persists the picker's
      // open/closed state across those refreshes, and (b) defines
      // window.jbdCmd(), used by the Add/Remove links so a click fires the
      // command in the background instead of navigating the whole page to
      // the raw JSON response.
      WSContentSend_P(PSTR(
        "<script>(function(){"
        "var K='jbdbmsOpen';"
        "function ap(){try{"
        "var d=document.getElementById('jbdpick');"
        "if(!d)return;"
        "if(localStorage.getItem(K)==='1'){d.setAttribute('open','');}"
        "d.ontoggle=function(){try{localStorage.setItem(K,d.open?'1':'0');}catch(e){}};"
        "}catch(e){}}"
        "var el=document.getElementById('l1');"
        "if(el){new MutationObserver(ap).observe(el,{childList:true});}"
        "ap();"
        "window.jbdCmd=function(c){"
        "try{"
        "var x=new XMLHttpRequest();"
        "x.open('GET','cm?cmnd='+encodeURIComponent(c),true);"
        "x.onreadystatechange=function(){if(x.readyState===4&&typeof la==='function'){la();}};"
        "x.send();"
        "}catch(e){}"
        "return false;"
        "};"
        "})();</script>"));
      break;
#endif  // USE_WEBSERVER
    case FUNC_COMMAND:
      result = DecodeCommand(kJbdBmsCommands, JbdBmsCommands);
      break;
  }

  return result;
}

#endif  // USE_JBD_BMS_BLE
#endif  // USE_BLE_ESP32
#endif  // ESP32
