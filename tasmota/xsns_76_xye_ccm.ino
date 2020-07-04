/*
  xsns_76_xye_ccm.ino - XYE CCM support for Tasmota

  Copyright (C) 2020  Andreas Schultz

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

#ifdef USE_XYE_CCM

#define XYE_LOG_MORE
#define XYE_DEBUG

#define XSNS_76 76

#define XYE_QUERY   0xC0
#define XYE_SET     0xC3
#define XYE_LOCK    0xCC
#define XYE_UNLOCK  0xCD

#define XYE_TIMEOUT 100 // 200 ms ACK timeout

#define XYE_BUFFER_SIZE 64

#define XYE_INVALID ((uint8_t)~0)

#include <TasmotaSerial.h>

TasmotaSerial *XYESerial = nullptr;

typedef struct
{
    uint8_t oper_mode;
    uint8_t fan;
    uint8_t set_temp;
    uint8_t mode_flags;
    uint8_t tmr_start;
    uint8_t tmr_stop;
    uint8_t field_0c;
} REQUEST;

typedef struct UNIT
{
    uint8_t flags;
    uint8_t cap1;
    uint8_t cap2;
    uint8_t oper_mode;
    uint8_t fan;
    uint8_t set_temp;
    uint8_t t1_temp;
    uint8_t t2a_temp;
    uint8_t t2b_temp;
    uint8_t t3_temp;
    uint8_t current;
    uint8_t frequency;
    uint8_t tmr_start;
    uint8_t tmr_stop;
    uint8_t run;
    uint8_t mode_flags;
    uint8_t oper_flags;
    uint16_t error;
    uint16_t protect;
    uint8_t com_error;
    uint8_t field_1b;
    uint8_t field_1c;
} UNIT;

struct XYE
{
  uint8_t *buffer = nullptr; // Serial receive buffer
  int byte_counter = 0;      // Index in serial receive buffer
  int cmd_status = 0;
  unsigned long state_130msecond = 0;         // State 130msecond timer
  uint8_t query_id = 0;
  uint8_t set_id = 0;
  uint8_t last_cmd = XYE_INVALID;
  uint8_t last_dev = XYE_INVALID;

  uint64_t updated = 0;
  uint64_t present = 0;
  uint64_t pending = 0;
  uint64_t error = 0;

  uint8_t cmnd[16];
  uint8_t mask[16];

  UNIT units[64];
} XYE;

#define D_PRFX_XYE "CCM"
#define D_CMND_XYE_BUS_ID "Bus-Id"
#define D_CMND_XYE_SET "Set"
#define D_CMND_XYE_LOCK "Lock"
#define D_CMND_XYE_UNLOCK "Unlock"
#define D_CMND_XYE_LOCK_ALL "LockAll"
#define D_CMND_XYE_UNLOCK_ALL "UnlockAll"

const char kXYECommands[] PROGMEM = D_PRFX_XYE "|"
  D_CMND_XYE_BUS_ID "|" D_CMND_XYE_SET "|"
  D_CMND_XYE_LOCK "|" D_CMND_XYE_UNLOCK "|"
  D_CMND_XYE_LOCK_ALL "|" D_CMND_XYE_UNLOCK_ALL
  ;

void (* const XYECommand[])(void) PROGMEM = {
  &CmndXYEBusId,
  &CmndXYESet,
  &CmndXYELock,
  &CmndXYEUnlock,
  &CmndXYELockAll,
  &CmndXYEUnlockAll,
  };

#define C_IDX_INVAL 0x80000000

const char kXYEoperMode[] PROGMEM =
  "off" "|" "auto" "|" "cool" "|" "dry" "|" "heat" "|" "fan" "|";

#define C_OP_OFF  0
#define C_OP_AUTO 1
#define C_OP_COOL 2
#define C_OP_DRY  3
#define C_OP_HEAT 4
#define C_OP_FAN  5

const char kXYEfanMode[] PROGMEM =
  "auto" "|" "high" "|" "medium" "|" "low";

#define C_FAN_AUTO    0
#define C_FAN_HIGH    1
#define C_FAN_MEDIUM  2
#define C_FAN_LOW     3

const char kXYEecoMode[] PROGMEM =
  "normal" "|" "eco" "|" "turbo";

#define C_ECO_NORMAL  0
#define C_ECO_ECO     1
#define C_ECO_TURBO   2

/*
 * Internal Functions
 */

uint8_t XYECrc8(const uint8_t *p, uint8_t len)
{
  uint8_t crc8 = 0;

  for (int i = 0; i < len; i++)
    crc8 += *(p + i);

  return 255 - crc8;
}

void XYEInit(void)
{
#ifdef XYE_DEBUG
  AddLog_P2(LOG_LEVEL_INFO, PSTR("XYE: Starting Tx %d Rx %d"),
	    Pin(GPIO_XYE_TX), Pin(GPIO_XYE_RX));
#endif

  if (!PinUsed(GPIO_XYE_RX) || !PinUsed(GPIO_XYE_TX))
    return;

  memset(XYE.units, 0, sizeof(XYE.units));

  XYE.buffer = (uint8_t *)malloc(XYE_BUFFER_SIZE);
  if (XYE.buffer != nullptr) {
    XYESerial = new TasmotaSerial(Pin(GPIO_XYE_RX), Pin(GPIO_XYE_TX), 2);
    AddLog_P2(LOG_LEVEL_INFO, PSTR("XYE: Serial %p"), XYESerial);
    if (XYESerial->begin(4800)) {
      AddLog_P2(LOG_LEVEL_INFO, PSTR("XYE: Baud Set %d"), XYESerial->hardwareSerial());
      if (XYESerial->hardwareSerial()) {
	AddLog_P2(LOG_LEVEL_INFO, PSTR("XYE: Claimed"));
	ClaimSerial();
      }
      XYESerial->flush();

      /* the CCM01 uses a 130ms send rythm, set ssleep to 10ms so that we can match that */
      ssleep = 10;
    }
  }
}

#ifdef XYE_DEBUG

void XYEDebugData(const char *tag, uint8 *buffer, uint8_t len, uint8_t crc)
{
  snprintf_P(log_data, sizeof(log_data), PSTR("XYE: %s: \""), tag);
  for (uint32_t i = 0; i < len; i++) {
    snprintf_P(log_data, sizeof(log_data), PSTR("%s%02x"), log_data, buffer[i]);
  }
  snprintf_P(log_data, sizeof(log_data), PSTR("%s\", CRC: 0x%02x"), log_data, crc);
  AddLog(LOG_LEVEL_DEBUG_MORE);
}

#else

#define XYEDebugData(tag, buffer, len, crc) do {} while (0)

#endif

void XYESerialInput(void)
{
  while (XYESerial->available())
  {
    uint8_t serial_in_byte = XYESerial->read();

#ifdef XYE_DEBUG
    //AddLog_P2(LOG_LEVEL_INFO, PSTR("XYE: Serial In Byte 0x%02x"), serial_in_byte);
#endif

    if (XYE.byte_counter == 0 && serial_in_byte == 0xAA) {
      XYE.cmd_status = 1;
    } else if (XYE.byte_counter >= XYE_BUFFER_SIZE) {
      XYEDebugData(PSTR("RX"), &XYE.buffer[0], XYE.byte_counter, 0);
      XYE.cmd_status = 0;
      XYE.byte_counter = 0;
      return;
    }

    if (XYE.cmd_status == 1) {
      XYE.buffer[XYE.byte_counter++] = serial_in_byte;

      if (XYE.byte_counter == 32 && serial_in_byte == 0x55) {
	uint8_t crc = XYECrc8(&XYE.buffer[0], XYE.byte_counter);

	XYEDebugData(PSTR("RX"), &XYE.buffer[0], XYE.byte_counter, crc);

	if (crc == 0) {
	  XYEPacketProcess(&XYE.buffer[0], XYE.byte_counter);
	}

	// all read
	XYE.cmd_status = 0;
	XYE.byte_counter = 0;
      }
    }
  }
}

void XYEPacketProcess(uint8 *b, uint8_t len)
{
  UNIT unit;

  if (b[2] != 0x80 ||
      b[3] != Settings.xye_bus_id,
      b[5] != Settings.xye_bus_id) {
    /* not a answer or not for us */
#ifdef XYE_LOG_MORE
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("XYE: unexpected packet: 0x%02x, 0x%02x, 0x%02x"),
	      b[2], b[3], b[5]);
#endif
    return;
  }
  if (b[1] != XYE.last_cmd) {
    /* we didn't ask for that */
#ifdef XYE_LOG_MORE
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("XYE: unexpected reply, got 0x%02x, expected 0x%02x"),
	      b[1], XYE.last_cmd);
#endif
    return;
  }
  if (b[4] != XYE.last_dev) {
    /* we didn't that device */
#ifdef XYE_LOG_MORE
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("XYE: reply from unexpected device, got 0x%02x, expected 0x%02x"),
	      b[1], XYE.last_dev);
#endif
    return;
  }

  memcpy(&unit, &XYE.units[XYE.last_dev], sizeof(unit));

  unit.cap1 = b[6];
  unit.cap2 = b[7];
  unit.oper_mode = b[8];
  unit.fan = b[9];
  unit.set_temp = b[10];
  unit.t1_temp = b[11];
  unit.t2a_temp = b[12];
  unit.t2b_temp = b[13];
  unit.t3_temp = b[14];
  unit.current = b[15];
  unit.frequency = b[16];
  unit.tmr_start = b[17];
  unit.tmr_stop = b[18];
  unit.run = b[19];
  unit.mode_flags = b[20];
  unit.oper_flags = b[21];
  unit.error = b[22] | (uint16_t)b[23] >> 8;
  unit.protect = b[24] | (uint16_t)b[25] >> 8;
  unit.com_error = b[26];
  unit.field_1b = b[27];
  unit.field_1c = b[28];

  if (memcmp(&unit, &XYE.units[XYE.last_dev], sizeof(unit)) != 0) {
    memcpy(&XYE.units[XYE.last_dev], &unit, sizeof(unit));
    XYE.updated |= ((uint64_t)1 << XYE.last_dev);
  }

  XYE.present |= (uint64_t)1 << XYE.last_dev;

  /* process the answer */
  XYE.last_dev = XYE_INVALID;

}

void XYESendRequest(uint8_t cmd, uint8_t device)
{
  uint8_t q[16];

  XYE.last_cmd = cmd;
  XYE.last_dev = device;

  q[0x00] = 0xAA;
  q[0x01] = cmd;
  q[0x02] = device;
  q[0x03] = Settings.xye_bus_id;
  q[0x04] = 0x80;
  q[0x05] = Settings.xye_bus_id;
  q[0x06] = 0x00;
  q[0x07] = 0x00;
  q[0x08] = 0x00;
  q[0x09] = 0x00;
  q[0x0a] = 0x00;
  q[0x0b] = 0x00;
  q[0x0c] = 0x00;
  q[0x0d] = 255 - cmd;
  q[0x0e] = 0x00;
  q[0x0f] = 0x55;

  q[14] = XYECrc8(&q[0], 16);

  XYESerial->write(&q[0], sizeof(q));
  XYESerial->flush();

  XYE.query_id = (XYE.query_id + 1) % 64;
}

void XYESetUnit(uint8_t unit)
{
  XYE.cmnd[0x02] = unit;

  XYE.cmnd[0x06] |= (XYE.units[unit].oper_mode & ~XYE.mask[0x06]);
  XYE.cmnd[0x07] |= (XYE.units[unit].fan & ~XYE.mask[0x07]);
  XYE.cmnd[0x08] |= (XYE.units[unit].set_temp & ~XYE.mask[0x08]);
  XYE.cmnd[0x09] |= (XYE.units[unit].mode_flags & ~XYE.mask[0x09]);
  XYE.cmnd[0x0a] |= (XYE.units[unit].tmr_start & ~XYE.mask[0x0a]);
  XYE.cmnd[0x0b] |= (XYE.units[unit].tmr_stop & ~XYE.mask[0x0b]);

  XYE.cmnd[0x0e] = 0;
  XYE.cmnd[0x0e] = XYECrc8(&XYE.cmnd[0], 16);

  XYEDebugData(PSTR("TX"), &XYE.cmnd[0], sizeof(XYE.cmnd), XYE.cmnd[14]);

  XYESerial->write(&XYE.cmnd[0], sizeof(XYE.cmnd));
  XYESerial->flush();

  XYE.last_cmd = XYE.cmnd[1];
}

void XYESetPending()
{
  if (XYE.cmnd[0x02] == 0xff) {
    XYESetUnit(0xff);
    XYE.cmnd[0x02] = 0;
    XYE.last_dev = XYE_INVALID;
    return;
  }

  for (uint8_t i = XYE.set_id; i < 64; i++) {
    if (!(XYE.pending & ((uint64_t)1 << i)))
      continue;

    XYESetUnit(i);
    XYE.set_id = i;
    XYE.last_dev = i;
    XYE.pending &= ~((uint64_t)1 << i);
    break;
  }
}

void XYEProcess(void)
{
  if ((XYE.last_dev != XYE_INVALID) &&
      (XYE.present & ((uint64_t)1 << XYE.last_dev))) {
    if (XYE.last_cmd == XYE_QUERY) {
      XYE.present &= ~((uint64_t)1 << XYE.last_dev);
      XYE.error &= ~((uint64_t)1 << XYE.last_dev);
    } else {
      XYE.error |= (uint64_t)1 << XYE.last_dev;
    }
    XYE.updated |= ((uint64_t)1 << XYE.last_dev);
  }

  if (XYE.pending != 0) {
    XYESetPending();
  } else {
    if (XYE.last_cmd == XYE_SET ||
	XYE.query_id == 0) {
      XYE.last_cmd == XYE_INVALID;
      XYEUpdateStatus();
    }
    XYESendRequest(XYE_QUERY, XYE.query_id);
  }
}

void XYEUpdateStatus()
{
  if (XYE.updated) {
    MqttPublishSensor();
    XYE.updated = 0;
  }
}

void XYEAppendTemp(bool comma, const char * tag, uint8_t t)
{
  char str_temperature[33];

  if (comma) {
    ResponseAppend_P(PSTR(","));
  }
  if (t < 40 || t > 238) {
    ResponseAppend_P(PSTR("\"%s\":null"), tag);
  } else {
    dtostrfd( (t - 40) / 2.0, 1, str_temperature);
    ResponseAppend_P(PSTR("\"%s\":%s"), tag, str_temperature);
  }
}

void XYEAppendOptInt(const char * tag, uint8_t t)
{
  if (t == 255) {
    ResponseAppend_P(PSTR("\"%s\":null,"), tag);
  } else {
    ResponseAppend_P(PSTR("\"%s\":%d,"), tag, t);
  }
}

void XYEAppendStrIndexed(const char * tag, uint32_t idx, const char* haystack)
{
  char s[10];

  if (!(idx & C_IDX_INVAL)) {
    GetTextIndexed(s, sizeof(s), idx, haystack);
  } else {
    snprintf_P(s, sizeof(s), PSTR("0x%02x"), idx & 0xff);
  }
  ResponseAppend_P(PSTR("\"%s\":\"%s\","), tag, s);
}

void XYEDumpUnit(struct UNIT *u)
{
  const char * mode = PSTR("n/a");
  uint32_t idx;

  ResponseAppend_P(PSTR("{"));
  /*
    ResponseAppend_P(PSTR("\"cap1\":%d,"), u->cap1);
    ResponseAppend_P(PSTR("\"cap2\":%d,"), u->cap2);
   */
  ResponseAppend_P(PSTR("\"capabilities\":{\"hasSwing\":%s,\"temperatureRange\":\"%s\"},"),
		    (u->cap2 & 0x10) ? PSTR("true") : PSTR("false"),
		    (u->cap2 & 0x80) ? PSTR("extended") : PSTR("normal"));

  switch (u->oper_mode) {
  case 0x00: idx = C_OP_OFF; break;
  case 0x80: idx = C_OP_AUTO; break;
  case 0x88: idx = C_OP_COOL; break;
  case 0x82: idx = C_OP_DRY; break;
  case 0x84: idx = C_OP_HEAT; break;
  case 0x81: idx = C_OP_FAN; break;
  default:   idx = u->oper_mode | C_IDX_INVAL; break;
  };
  XYEAppendStrIndexed(PSTR("operMode"), idx, kXYEoperMode);

  switch (u->fan) {
  case 0x80: idx = C_FAN_AUTO; break;
  case 0x01: idx = C_FAN_HIGH; break;
  case 0x02: idx = C_FAN_MEDIUM; break;
  case 0x03: idx = C_FAN_LOW; break;
  default:   idx = u->fan | C_IDX_INVAL; break;
  };
  XYEAppendStrIndexed(PSTR("fan"), idx, kXYEfanMode);
  ResponseAppend_P(PSTR("\"setTemp\":%d,"), u->set_temp);
  ResponseAppend_P(PSTR("\"temperatures\":{"), u->set_temp);
  XYEAppendTemp(false, PSTR("t1Temp"), u->t1_temp);
  XYEAppendTemp(true, PSTR("t2aTemp"), u->t2a_temp);
  XYEAppendTemp(true, PSTR("t2bTemp"), u->t2b_temp);
  XYEAppendTemp(true, PSTR("t3Temp"), u->t3_temp);
  ResponseAppend_P(PSTR("},"));
  XYEAppendOptInt(PSTR("current"), u->current);
  XYEAppendOptInt(PSTR("frequency"), u->frequency);
  ResponseAppend_P(PSTR("\"timer\":{\"start\":\"%d\",\"stop\":\"%d\"},"),
		   u->tmr_start, u->tmr_start);

  switch (u->mode_flags & 0x03) {
  case 0: mode = PSTR("normal"); break;
  case 1: mode = PSTR("eco"); break;
  case 2: mode = PSTR("turbo"); break;
  };
  ResponseAppend_P(PSTR("\"status\":{\"run\":%s,\"mode\":\"%s\",\"swing\":\"%s\",\"vent\":\"%s\",\"pump\":\"%s\",\"locked\":%s},"),
		   (u->run & 0x01) ? PSTR("true") : PSTR("false"),
		   mode,
		   (u->mode_flags & 0x04) ? PSTR("on") : PSTR("off"),
		   ((u->mode_flags & 0x88) == 0x88) ? PSTR("on") : PSTR("off"),
		   (u->oper_flags & 0x04) ? PSTR("on") : PSTR("off"),
		   (u->oper_flags & 0x80) ? PSTR("true") : PSTR("false"));
  ResponseAppend_P(PSTR("\"error\":\"0x%04x\","), u->error);
  ResponseAppend_P(PSTR("\"protect\":\"0x%04x\","), u->protect);
  ResponseAppend_P(PSTR("\"com_error\":%d,"), u->com_error);
  ResponseAppend_P(PSTR("\"field_1b\":\"0x%02x\","), u->field_1b);
  ResponseAppend_P(PSTR("\"field_1c\":\"0x%02x\""), u->field_1c);
  ResponseJsonEnd();
}

void XYEDumpUnits()
{
  bool add_coma = false;
  for (int i = 0; i < 64; ++i)
    {
      if (!(XYE.present & ((uint64_t)1 << i)))
	continue;

      ResponseAppend_P(PSTR("%s\"%d\":"), add_coma ? "," : "", i);
      XYEDumpUnit(&XYE.units[i]);

      add_coma = true;
    }
}

void XYEStatus(bool json)
{
  if (json) {
    ResponseAppend_P(PSTR(",\"CCM\":{"));
    ResponseAppend_P(PSTR("\"online\":%d,"), __builtin_popcount(XYE.present));
    ResponseAppend_P(PSTR("\"error\":%d,"), __builtin_popcount(XYE.error));
    ResponseAppend_P(PSTR("\"units\":{"));
    XYEDumpUnits();
    ResponseJsonEndEnd();
  } else {
    WSContentSend_P(PSTR("{s}CCM Online{m}%d{e}"), __builtin_popcount(XYE.present));
    WSContentSend_P(PSTR("{s}CCM Errors{m}%d{e}"), __builtin_popcount(XYE.error));
  }
}

void CmndXYEBusId(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 64)) {
    Settings.xye_bus_id = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.xye_bus_id);
}

void CmndXYESet(void)
{
  // Set { "Id": 2, "operMode": "off" }
  // Set { "Id": 2, "operMode": "cool" }
  // Set { "Id": 2, "operMode": "cool", "setTemp": 21 }
  // Set { "Id": 2, "operMode": "cool", "setTemp": 21, "fan": "auto", "eco": "normal", "swing": false }
  DynamicJsonBuffer jsonBuf;
  struct UNIT *u;
  uint8_t id;
  char s[10];
  int v;

  uint8_t cmnd[16] = {0xaa, 0x00, 0xff, 0x00, 0x80, 0x00, 0x00, 0x00,
		      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55};
  uint8_t mask[16] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
		      0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff};

  const JsonObject &json = jsonBuf.parseObject((const char*) XdrvMailbox.data);
  if (!json.success()) {
    ResponseCmndChar_P(PSTR("JSON parsing failed"));
    return;
  }

  if (!json.containsKey("Id")) {
    ResponseCmndChar_P(PSTR("Id missing"));
    return;
  }

  cmnd[0x01] = XYE_SET;
  cmnd[0x03] = Settings.xye_bus_id;
  cmnd[0x05] = Settings.xye_bus_id;
  cmnd[0x0d] = 255 - cmnd[0x01];

  if (json["Id"].is<char*>()) {
    if (strcmp(json["Id"].as<char*>(), "ALL") != 0) {
      ResponseCmndChar_P(PSTR("Id must be numeric or 'ALL'"));
      return;
    }
  } else if (json["Id"].is<int>()) {
    v =  json["Id"];
    if (v <= 0 || v > 63) {
      ResponseCmndChar_P(PSTR("Id out of range"));
      return;
    }
    if (!(XYE.present & ((uint64_t)1 << v))) {
      ResponseCmndChar_P(PSTR("Unit not online"));
      return;
    }
    cmnd[0x02] = v;
  } else {
    ResponseCmndChar_P(PSTR("Id has wrong type"));
    return;
  }

  if (json.containsKey("operMode")) {
    mask[0x06] = 0xff;
    switch (GetCommandCode(s, sizeof(s), json["operMode"], kXYEoperMode)) {
    case C_OP_OFF:
      cmnd[0x06] = 0x00;
      break;
    case C_OP_AUTO:
      cmnd[0x06] = 0x80;
      break;
    case C_OP_FAN:
      cmnd[0x06] = 0x81;
      mask[0x08] = 0xff;
      cmnd[0x08] = 0xff;
      break;
    case C_OP_DRY:
      cmnd[0x06] = 0x82;
      break;
    case C_OP_HEAT:
      cmnd[0x06] = 0x84;
      break;
    case C_OP_COOL:
      cmnd[0x06] = 0x88;
      break;
    default:
      ResponseCmndChar_P(PSTR("Invalid operMode"));
      return;
    }
  }

  if (json.containsKey("fan")) {
    mask[0x07] = 0xff;
    switch (GetCommandCode(s, sizeof(s), json["fan"], kXYEfanMode)) {
    case C_FAN_AUTO:   cmnd[0x07] = 0x80; break;
    case C_FAN_HIGH:   cmnd[0x07] = 0x01; break;
    case C_FAN_MEDIUM: cmnd[0x07] = 0x02; break;
    case C_FAN_LOW:    cmnd[0x07] = 0x03; break;
    default:
      ResponseCmndChar_P(PSTR("Invalid fan value"));
      return;
    }
  }

  if (json.containsKey("setTemp") && mask[0x08] != 0xff) {
    mask[0x08] = 0xff;
    cmnd[0x08] = json["setTemp"];
    if (cmnd[0x08] < 17 || cmnd[0x08] > 30) {
      ResponseCmndChar_P(PSTR("setTemp out of range"));
      return;
    }
  }

  if (json.containsKey("eco")) {
    mask[0x09] = 0x03;
    switch (GetCommandCode(s, sizeof(s), json["fan"], kXYEecoMode)) {
    case C_ECO_NORMAL: cmnd[0x09] = 0x00; break;
    case C_ECO_ECO:    cmnd[0x09] = 0x01; break;
    case C_ECO_TURBO:  cmnd[0x09] = 0x02; break;
    default:
      ResponseCmndChar_P(PSTR("Invalid eco value"));
      return;
    }
  }

  if (json.containsKey("swing")) {
    bool swing = json["swing"];

    if (swing) {
      mask[0x09] |= 0x04;
      cmnd[0x09] |= 0x04;
    }
  }

  if (json.containsKey("vent")) {
    bool vent = json["vent"];

    if (vent) {
      mask[0x09] |= 0x88;
      cmnd[0x09] |= 0x88;
    }
  }

  if (json.containsKey("timer")) {
    JsonObject &timer = json["timer"];

    if (timer.containsKey("start")) {
      mask[0x0a] = 0xff;
      cmnd[0x0a] = timer["start"];
    }
    if (timer.containsKey("stop")) {
      mask[0x0b] = 0xff;
      cmnd[0x0b] = timer["stop"];
    }
  }

  XYEDebugData(PSTR("Cmnd"), cmnd, sizeof(cmnd), 0);
  XYEDebugData(PSTR("Mask"), mask, sizeof(mask), 0);

  memcpy(&XYE.cmnd, &cmnd, sizeof(XYE.cmnd));
  memcpy(&XYE.mask, &mask, sizeof(XYE.cmnd));

  if (cmnd[0x02] != 0xff) {
    XYE.pending |= (uint64_t)1 << cmnd[0x02];
    XYE.set_id = cmnd[0x02];
  } else {
    XYE.pending = XYE.present;
    XYE.set_id = 0;
  }

  ResponseCmndDone();
}

void CmndXYELock(void)
{
  CmndXYELockingCmnd(XYE_LOCK);
}

void CmndXYEUnlock(void)
{
  CmndXYELockingCmnd(XYE_UNLOCK);
}

void CmndXYELockAll(void)
{
  XdrvMailbox.payload = 0xff;
  CmndXYELockingCmnd(XYE_LOCK);
}

void CmndXYEUnlockAll(void)
{
  XdrvMailbox.payload = 0xff;
  CmndXYELockingCmnd(XYE_UNLOCK);
}

void CmndXYELockingCmnd(uint8_t code)
{
  uint8_t cmnd[16] = {0xaa, 0x00, 0xff, 0x00, 0x80, 0x00, 0x00, 0x00,
		      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55};
  uint8_t mask[16] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
		      0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff};

  if (XdrvMailbox.payload > 63 && XdrvMailbox.payload != 0xff) {
    ResponseCmndChar_P(PSTR("invalid Id"));
    return;
  }

  cmnd[0x01] = code;
  cmnd[0x02] = XdrvMailbox.payload;
  cmnd[0x03] = Settings.xye_bus_id;
  cmnd[0x05] = Settings.xye_bus_id;
  cmnd[0x0d] = 255 - cmnd[0x01];

  XYEDebugData(PSTR("Cmnd"), cmnd, sizeof(cmnd), 0);
  XYEDebugData(PSTR("Mask"), mask, sizeof(mask), 0);

  memcpy(&XYE.cmnd, &cmnd, sizeof(XYE.cmnd));
  memcpy(&XYE.mask, &mask, sizeof(XYE.cmnd));

  if (cmnd[0x02] != 0xff) {
    XYE.pending |= (uint64_t)1 << cmnd[0x02];
    XYE.set_id = cmnd[0x02];
  } else {
    XYE.pending = XYE.present;
    XYE.set_id = 0;
  }

  ResponseCmndDone();
}

/*
 * Interface
 */

bool Xsns76(uint8_t function)
{
  bool result = false;

  if (FUNC_INIT == function)
    XYEInit();

  if (!XYESerial) {
    return result;
  }

  switch (function) {
  case FUNC_LOOP:
    XYESerialInput();
    if (TimeReached(XYE.state_130msecond)) {
      SetNextTimeInterval(XYE.state_130msecond, 130);
      XYEProcess();
    }
    break;

  case FUNC_COMMAND:
    result = DecodeCommand(kXYECommands, XYECommand);
    break;

  case FUNC_JSON_APPEND:
    XYEStatus(1);
    break;

#ifdef USE_WEBSERVER
  case FUNC_WEB_SENSOR:
    XYEStatus(0);
    break;
#endif // USE_WEBSERVER
  }
  return result;
}

#endif  // USE_XYE_CCM
