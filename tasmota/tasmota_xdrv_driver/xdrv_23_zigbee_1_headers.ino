/*
  xdrv_23_zigbee_1_headers.ino - zigbee support for Tasmota

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE

#ifdef USE_ZIGBEE_EEPROM
#include "Eeprom24C512.h"
#endif // USE_ZIGBEE_EEPROM

// channels numbers for Zigbee radio energy scan
#define USE_ZIGBEE_CHANNEL_MIN    11
#define USE_ZIGBEE_CHANNEL_MAX    26
#define USE_ZIGBEE_CHANNEL_COUNT  (USE_ZIGBEE_CHANNEL_MAX - USE_ZIGBEE_CHANNEL_MIN + 1)

// contains some definitions for functions used before their declarations

typedef int32_t (*ZB_Func)(uint8_t value);
typedef int32_t (*ZB_RecvMsgFunc)(int32_t res, const SBuffer &buf);

// Labels used in the State Machine -- internal only
const uint8_t  ZIGBEE_LABEL_RESTART = 1;     // Restart the state_machine in a different mode
const uint8_t  ZIGBEE_LABEL_INIT_COORD = 10;     // Start ZNP as coordinator
const uint8_t  ZIGBEE_LABEL_START_COORD = 11;     // Start ZNP as coordinator
const uint8_t  ZIGBEE_LABEL_INIT_ROUTER = 12;    // Init ZNP as router
const uint8_t  ZIGBEE_LABEL_START_ROUTER = 13;    // Start ZNP as router
const uint8_t  ZIGBEE_LABEL_INIT_DEVICE = 14;    // Init ZNP as end-device
const uint8_t  ZIGBEE_LABEL_START_DEVICE = 15;    // Start ZNP as end-device
const uint8_t  ZIGBEE_LABEL_START_ROUTER_DEVICE = 16;    // Start common to router and device
const uint8_t  ZIGBEE_LABEL_ZB3_INIT = 17;        // check parameters for ZB3
const uint8_t  ZIGBEE_LABEL_COORD_STARTED = 18;     // Coordinator has started
const uint8_t  ZIGBEE_LABEL_FACT_RESET_ROUTER_DEVICE_POST = 19;   // common post configuration for router and device
const uint8_t  ZIGBEE_LABEL_READY = 20;   // goto label 20 for main loop
const uint8_t  ZIGBEE_LABEL_MAIN_LOOP = 21;   // main loop
const uint8_t  ZIGBEE_LABEL_NETWORK_CONFIGURED = 22;   // main loop
const uint8_t  ZIGBEE_LABEL_BAD_CONFIG = 23;          // EZSP configuration is not the right one
const uint8_t  ZIGBEE_LABEL_PERMIT_JOIN_CLOSE = 30;   // disable permit join
const uint8_t  ZIGBEE_LABEL_PERMIT_JOIN_OPEN_60 = 31;    // enable permit join for 60 seconds
const uint8_t  ZIGBEE_LABEL_PERMIT_JOIN_OPEN_XX = 32;    // enable permit join for 60 seconds
// factory reset or reconfiguration
const uint8_t  ZIGBEE_LABEL_FACT_RESET_COORD = 50;   // main loop
const uint8_t  ZIGBEE_LABEL_FACT_RESET_ROUTER = 51;   // main loop
const uint8_t  ZIGBEE_LABEL_FACT_RESET_DEVICE = 52;   // main loop
const uint8_t  ZIGBEE_LABEL_CONFIGURE_EZSP = 53;   // main loop
// errors
const uint8_t  ZIGBEE_LABEL_ABORT = 99;   // goto label 99 in case of fatal error
const uint8_t  ZIGBEE_LABEL_UNSUPPORTED_VERSION = 98;  // Unsupported ZNP version

class ZigbeeStatus {
public:
  ZigbeeStatus()
#ifdef USE_ZIGBEE_EEPROM
    : eeprom(USE_ZIGBEE_ZBBRIDGE_EEPROM)
#endif // USE_ZIGBEE_EEPROM
  {}

  bool active = false;                // is Zigbee active for this device, i.e. GPIOs configured
  bool state_machine = false;		      // the state machine is running
  bool state_waiting = false;         // the state machine is waiting for external event or timeout
  bool state_no_timeout = false;      // the current wait loop does not generate a timeout but only continues running
  bool init_phase = true;             // initialization phase, before accepting zigbee traffic
  bool recv_until = false;            // ignore all messages until the received frame fully matches
  bool eeprom_present = false;        // is the ZBBridge EEPROM present?
  bool eeprom_ready = false;          // is the ZBBridge EEPROM formatted and ready?
#ifdef USE_ZIGBEE_ZNP
  bool zb3 = false;                   // true if ZStack 3.x, false if ZStack 1.2
#else
  bool zb3 = true;                    // always ZB3 with EZSP
#endif // USE_ZIGBEE_ZNP
  // Zigbee mapping
  bool mapping_in_progress = false;   // is there a mapping in progress
  bool mapping_ready = false;         // do we have mapping information ready
  uint32_t mapping_end_time = 0;

  uint8_t on_error_goto = ZIGBEE_LABEL_ABORT;         // on error goto label, 99 default to abort
  uint8_t on_timeout_goto = ZIGBEE_LABEL_ABORT;       // on timeout goto label, 99 default to abort
  uint8_t *recv_filter = nullptr;        // receive filter message
  uint8_t recv_filter_len = 0;
  int16_t pc = 0;                     // program counter, -1 means abort
  uint32_t next_timeout = 0;          // millis for the next timeout

  ZB_RecvMsgFunc recv_func = nullptr;          // function to call when message is expected
  ZB_RecvMsgFunc recv_unexpected = nullptr;    // function called when unexpected message is received

  // version
  uint8_t major_rel = 0;
  uint8_t minor_rel = 0;
  uint8_t maint_rel = 0;
  uint32_t revision = 0;
  
  // Energy scan
  int8_t energy[USE_ZIGBEE_CHANNEL_COUNT];

  uint32_t permit_end_time = 0;       // timestamp when permit join ends, with ZNP 1.2, it takes only 0 (off) and -1 (on)
#ifdef USE_ZIGBEE_EZSP
  uint16_t ezsp_version = 0;
#endif

#ifdef USE_ZIGBEE_EEPROM
  Eeprom24C512 eeprom;     // takes only 1 bytes of RAM
#endif // USE_ZIGBEE_EEPROM
};
struct ZigbeeStatus zigbee;
SBuffer *zigbee_buffer = nullptr;

void zigbeeZCLSendCmd(ZCLFrame &msg);
void ZigbeeZCLSend_Raw(const ZCLFrame &zcl);
bool ZbAppendWriteBuf(SBuffer & buf, const Z_attribute & attr, bool prepend_status_ok = false);

// parse Hex formatted attribute names like '0301/0001"
uint32_t parseHex(const char **data, size_t max_len = 8) {
  uint32_t ret = 0;
  for (uint32_t i = 0; i < max_len; i++) {
    int8_t v = hexValue(**data);
    if (v < 0) { break; }     // non hex digit, we stop parsing
    ret = (ret << 4) | v;
    *data += 1;
  }
  return ret;
}

#endif // USE_ZIGBEE
