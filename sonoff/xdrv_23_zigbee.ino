/*
  xdrv_23_zigbee.ino - zigbee serial support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends and Stephan Hadinger

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

#define XDRV_23                    23

const uint32_t ZIGBEE_BUFFER_SIZE = 256;  // Max ZNP frame is SOF+LEN+CMD1+CMD2+250+FCS = 255
const uint8_t  ZIGBEE_SOF = 0xFE;

// State machine states
enum class ZnpStates {
	S_START = 0,
	S_READY
};

// ZNP Constants taken from https://github.com/Frans-Willem/AqaraHub/blob/master/src/znp/znp.h
enum class ZnpCommandType { POLL = 0, SREQ = 2, AREQ = 4, SRSP = 6 };

enum class ZnpSubsystem {
  RPC_Error = 0,
  SYS = 1,
  MAC = 2,
  NWK = 3,
  AF = 4,
  ZDO = 5,
  SAPI = 6,
  UTIL = 7,
  DEBUG = 8,
  APP = 9
};

enum class ZnpStatus : uint8_t {
  Success = 0x00,
  Failure = 0x01,
  InvalidParameter = 0x02,
  MemError = 0x03,
  BufferFull = 0x11
};

typedef uint64_t IEEEAddress;
typedef uint16_t ShortAddress;

enum class AddrMode : uint8_t {
  NotPresent = 0,
  Group = 1,
  ShortAddress = 2,
  IEEEAddress = 3,
  Broadcast = 0xFF
};

// Commands in the SYS subsystem
enum class SysCommand : uint8_t {
  RESET = 0x00,
  PING = 0x01,
  VERSION = 0x02,
  SET_EXTADDR = 0x03,
  GET_EXTADDR = 0x04,
  RAM_READ = 0x05,
  RAM_WRITE = 0x06,
  OSAL_NV_ITEM_INIT = 0x07,
  OSAL_NV_READ = 0x08,
  OSAL_NV_WRITE = 0x09,
  OSAL_START_TIMER = 0x0A,
  OSAL_STOP_TIMER = 0x0B,
  RANDOM = 0x0C,
  ADC_READ = 0x0D,
  GPIO = 0x0E,
  STACK_TUNE = 0x0F,
  SET_TIME = 0x10,
  GET_TIME = 0x11,
  OSAL_NV_DELETE = 0x12,
  OSAL_NV_LENGTH = 0x13,
  TEST_RF = 0x40,
  TEST_LOOPBACK = 0x41,
  RESET_IND = 0x80,
  OSAL_TIMER_EXPIRED = 0x81,
};

// Commands in the AF subsystem
enum class AfCommand : uint8_t {
  REGISTER = 0x00,
  DATA_REQUEST = 0x01,
  DATA_REQUEST_EXT = 0x02,
  DATA_REQUEST_SRC_RTG = 0x03,
  INTER_PAN_CTL = 0x10,
  DATA_STORE = 0x11,
  DATA_RETRIEVE = 0x12,
  APSF_CONFIG_SET = 0x13,
  DATA_CONFIRM = 0x80,
  REFLECT_ERROR = 0x83,
  INCOMING_MSG = 0x81,
  INCOMING_MSG_EXT = 0x82
};

// Commands in the ZDO subsystem
enum class ZdoCommand : uint8_t {
  NWK_ADDR_REQ = 0x00,
  IEEE_ADDR_REQ = 0x01,
  NODE_DESC_REQ = 0x02,
  POWER_DESC_REQ = 0x03,
  SIMPLE_DESC_REQ = 0x04,
  ACTIVE_EP_REQ = 0x05,
  MATCH_DESC_REQ = 0x06,
  COMPLEX_DESC_REQ = 0x07,
  USER_DESC_REQ = 0x08,
  DEVICE_ANNCE = 0x0A,
  USER_DESC_SET = 0x0B,
  SERVER_DISC_REQ = 0x0C,
  END_DEVICE_BIND_REQ = 0x20,
  BIND_REQ = 0x21,
  UNBIND_REQ = 0x22,
  SET_LINK_KEY = 0x23,
  REMOVE_LINK_KEY = 0x24,
  GET_LINK_KEY = 0x25,
  MGMT_NWK_DISC_REQ = 0x30,
  MGMT_LQI_REQ = 0x31,
  MGMT_RTQ_REQ = 0x32,
  MGMT_BIND_REQ = 0x33,
  MGMT_LEAVE_REQ = 0x34,
  MGMT_DIRECT_JOIN_REQ = 0x35,
  MGMT_PERMIT_JOIN_REQ = 0x36,
  MGMT_NWK_UPDATE_REQ = 0x37,
  MSG_CB_REGISTER = 0x3E,
  MGS_CB_REMOVE = 0x3F,
  STARTUP_FROM_APP = 0x40,
  AUTO_FIND_DESTINATION = 0x41,
  EXT_REMOVE_GROUP = 0x47,
  EXT_REMOVE_ALL_GROUP = 0x48,
  EXT_FIND_ALL_GROUPS_ENDPOINT = 0x49,
  EXT_FIND_GROUP = 0x4A,
  EXT_ADD_GROUP = 0x4B,
  EXT_COUNT_ALL_GROUPS = 0x4C,
  NWK_ADDR_RSP = 0x80,
  IEEE_ADDR_RSP = 0x81,
  NODE_DESC_RSP = 0x82,
  POWER_DESC_RSP = 0x83,
  SIMPLE_DESC_RSP = 0x84,
  ACTIVE_EP_RSP = 0x85,
  MATCH_DESC_RSP = 0x86,
  COMPLEX_DESC_RSP = 0x87,
  USER_DESC_RSP = 0x88,
  USER_DESC_CONF = 0x89,
  SERVER_DISC_RSP = 0x8A,
  END_DEVICE_BIND_RSP = 0xA0,
  BIND_RSP = 0xA1,
  UNBIND_RSP = 0xA2,
  MGMT_NWK_DISC_RSP = 0xB0,
  MGMT_LQI_RSP = 0xB1,
  MGMT_RTG_RSP = 0xB2,
  MGMT_BIND_RSP = 0xB3,
  MGMT_LEAVE_RSP = 0xB4,
  MGMT_DIRECT_JOIN_RSP = 0xB5,
  MGMT_PERMIT_JOIN_RSP = 0xB6,
  STATE_CHANGE_IND = 0xC0,
  END_DEVICE_ANNCE_IND = 0xC1,
  MATCH_DESC_RSP_SENT = 0xC2,
  STATUS_ERROR_RSP = 0xC3,
  SRC_RTG_IND = 0xC4,
  LEAVE_IND = 0xC9,
  TC_DEV_IND = 0xCA,
  PERMIT_JOIN_IND = 0xCB,
  MSG_CB_INCOMING = 0xFF
};

// Commands in the SAPI subsystem
enum class SapiCommand : uint8_t {
  START_REQUEST = 0x00,
  BIND_DEVICE = 0x01,
  ALLOW_BIND = 0x02,
  SEND_DATA_REQUEST = 0x03,
  READ_CONFIGURATION = 0x04,
  WRITE_CONFIGURATION = 0x05,
  GET_DEVICE_INFO = 0x06,
  FIND_DEVICE_REQUEST = 0x07,
  PERMIT_JOINING_REQUEST = 0x08,
  SYSTEM_RESET = 0x09,
  START_CONFIRM = 0x80,
  BIND_CONFIRM = 0x81,
  ALLOW_BIND_CONFIRM = 0x82,
  SEND_DATA_CONFIRM = 0x83,
  FIND_DEVICE_CONFIRM = 0x85,
  RECEIVE_DATA_INDICATION = 0x87,
};

// Commands in the UTIL subsystem
enum class UtilCommand : uint8_t {
  GET_DEVICE_INFO = 0x00,
  GET_NV_INFO = 0x01,
  SET_PANID = 0x02,
  SET_CHANNELS = 0x03,
  SET_SECLEVEL = 0x04,
  SET_PRECFGKEY = 0x05,
  CALLBACK_SUB_CMD = 0x06,
  KEY_EVENT = 0x07,
  TIME_ALIVE = 0x09,
  LED_CONTROL = 0x0A,
  TEST_LOOPBACK = 0x10,
  DATA_REQ = 0x11,
  SRC_MATCH_ENABLE = 0x20,
  SRC_MATCH_ADD_ENTRY = 0x21,
  SRC_MATCH_DEL_ENTRY = 0x22,
  SRC_MATCH_CHECK_SRC_ADDR = 0x23,
  SRC_MATCH_ACK_ALL_PENDING = 0x24,
  SRC_MATCH_CHECK_ALL_PENDING = 0x25,
  ADDRMGR_EXT_ADDR_LOOKUP = 0x40,
  ADDRMGR_NWK_ADDR_LOOKUP = 0x41,
  APSME_LINK_KEY_DATA_GET = 0x44,
  APSME_LINK_KEY_NV_ID_GET = 0x45,
  ASSOC_COUNT = 0x48,
  ASSOC_FIND_DEVICE = 0x49,
  ASSOC_GET_WITH_ADDRESS = 0x4A,
  APSME_REQUEST_KEY_CMD = 0x4B,
  ZCL_KEY_EST_INIT_EST = 0x80,
  ZCL_KEY_EST_SIGN = 0x81,
  UTIL_SYNC_REQ = 0xE0,
  ZCL_KEY_ESTABLISH_IND = 0xE1
};

enum class Capability : uint16_t {
  SYS = 0x0001,
  MAC = 0x0002,
  NWK = 0x0004,
  AF = 0x0008,
  ZDO = 0x0010,
  SAPI = 0x0020,
  UTIL = 0x0040,
  DEBUG = 0x0080,
  APP = 0x0100,
  ZOAD = 0x1000
};

enum class ConfigurationOption : uint8_t {
  STARTUP_OPTION = 0x03,
  POLL_RATE = 0x24,
  QUEUED_POLL_RATE = 0x25,
  RESPONSE_POLL_RATE = 0x26,
  POLL_FAILURE_RETRIES = 0x29,
  INDIRECT_MSG_TIMEOUT = 0x2B,
  ROUTE_EXPIRY_TIME = 0x2C,
  EXTENDED_PAN_ID = 0x2D,
  BCAST_RETRIES = 0x2E,
  PASSIVE_ACK_TIMEOUT = 0x2F,
  BCAST_DELIVERY_TIME = 0x30,
  APS_FRAME_RETRIES = 0x43,
  APS_ACK_WAIT_DURATION = 0x44,
  BINDING_TIME = 0x46,
  PRECFGKEY = 0x62,
  PRECFGKEYS_ENABLE = 0x63,
  SECURITY_MODE = 0x64,
  USERDESC = 0x81,
  PANID = 0x83,
  CHANLIST = 0x84,
  LOGICAL_TYPE = 0x87,
  ZDO_DIRECT_CB = 0x8F
};

const char kZigbeeCommands[] PROGMEM = D_CMND_ZIGBEEZNPSEND;

void (* const ZigbeeCommand[])(void) PROGMEM = { &CmndZigbeeZNPSend };

#include <TasmotaSerial.h>

TasmotaSerial *ZigbeeSerial = nullptr;

unsigned long zigbee_polling_window = 0;
uint8_t *zigbee_buffer = nullptr;
uint32_t zigbee_in_byte_counter = 0;
uint32_t zigbee_frame_len = 256;
bool zigbee_active = true;
bool zigbee_raw = false;

void ZigbeeInput(void)
{
  // Receive only valid ZNP frames:
  // 00 - SOF = 0xFE
  // 01 - Length of Data Field - 0..250
  // 02 - CMD1 - first byte of command
  // 03 - CMD2 - second byte of command
  // 04..FD - Data Field
  // FE (or last) - FCS Checksum

  while (ZigbeeSerial->available()) {
    yield();
    uint8_t zigbee_in_byte = ZigbeeSerial->read();

    if ((0 == zigbee_in_byte_counter) && (ZIGBEE_SOF != zigbee_in_byte)) {
      // waiting for SOF (Start Of Frame) byte, discard anything else
      continue;     // discard
    }

    if (zigbee_in_byte_counter < zigbee_frame_len) {
      zigbee_buffer[zigbee_in_byte_counter++] = zigbee_in_byte;
      zigbee_polling_window = millis();                               // Wait for more data
    } else {
      zigbee_polling_window = 0;                                      // Publish now
      break;
    }

    // recalculate frame length
    if (02 == zigbee_in_byte_counter) {
      // We just received the Lenght byte
      uint8_t len_byte = zigbee_buffer[1];
      if (len_byte > 250)  len_byte = 250;    // ZNP spec says len is 250 max

      zigbee_frame_len = len_byte + 5;        // SOF + LEN + CMD1 + CMD2 + FCS = 5 bytes overhead
    }
  }

  if (zigbee_in_byte_counter && (millis() > (zigbee_polling_window + ZIGBEE_POLLING))) {
    char hex_char[(zigbee_in_byte_counter * 2) + 2];
    Response_P(PSTR("{\"" D_JSON_ZIGBEEZNPRECEIVED "\":\"%s\"}"),
      ToHex((unsigned char*)zigbee_buffer, zigbee_in_byte_counter, hex_char, sizeof(hex_char)));
    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZNPRECEIVED));
    XdrvRulesProcess();
    zigbee_in_byte_counter = 0;
    zigbee_frame_len = 254;
  }
}

/********************************************************************************************/

void ZigbeeInit(void)
{
  zigbee_active = false;
  if ((pin[GPIO_ZIGBEE_RX] < 99) && (pin[GPIO_ZIGBEE_TX] < 99)) {
    ZigbeeSerial = new TasmotaSerial(pin[GPIO_ZIGBEE_RX], pin[GPIO_ZIGBEE_TX]);
    if (ZigbeeSerial->begin(115200)) {    // ZNP is 115200, RTS/CTS (ignored), 8N1
      if (ZigbeeSerial->hardwareSerial()) {
        ClaimSerial();
        zigbee_buffer = (uint8_t*) serial_in_buffer;  // Use idle serial buffer to save RAM
      } else {
        zigbee_buffer = (uint8_t*) malloc(ZIGBEE_BUFFER_SIZE);
      }
      zigbee_active = true;
      ZigbeeSerial->flush();
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndZigbeeZNPSend(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t code;

    char *codes = RemoveSpace(XdrvMailbox.data);
    int32_t size = strlen(XdrvMailbox.data);

    while (size > 0) {
      char stemp[3];
      strlcpy(stemp, codes, sizeof(stemp));
      code = strtol(stemp, nullptr, 16);
      ZigbeeSerial->write(code);
      size -= 2;
      codes += 2;
    }
  }
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv23(uint8_t function)
{
  bool result = false;

  if (zigbee_active) {
    switch (function) {
      case FUNC_LOOP:
        if (ZigbeeSerial) { ZigbeeInput(); }
        break;
      case FUNC_PRE_INIT:
        ZigbeeInit();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kZigbeeCommands, ZigbeeCommand);
        break;
    }
  }
  return result;
}

#endif // USE_ZIGBEE
