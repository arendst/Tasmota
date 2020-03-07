/*
  xdrv_23_zigbee_constants.ino - zigbee support for Tasmota

  Copyright (C) 2020  Theo Arends and Stephan Hadinger

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

#define OCCUPANCY "Occupancy"             // global define for Aqara

typedef uint64_t Z_IEEEAddress;
typedef uint16_t Z_ShortAddress;

enum ZnpCommandType {
  Z_POLL = 0x00,
  Z_SREQ = 0x20,
  Z_AREQ = 0x40,
  Z_SRSP = 0x60
};

enum ZnpSubsystem {
  Z_RPC_Error = 0x00,
  Z_SYS = 0x01,
  Z_MAC = 0x02,
  Z_NWK = 0x03,
  Z_AF = 0x04,
  Z_ZDO = 0x05,
  Z_SAPI = 0x06,
  Z_UTIL = 0x07,
  Z_DEBUG = 0x08,
  Z_APP = 0x09
};

// Commands in the SYS subsystem
enum SysCommand {
  SYS_RESET = 0x00,
  SYS_PING = 0x01,
  SYS_VERSION = 0x02,
  SYS_SET_EXTADDR = 0x03,
  SYS_GET_EXTADDR = 0x04,
  SYS_RAM_READ = 0x05,
  SYS_RAM_WRITE = 0x06,
  SYS_OSAL_NV_ITEM_INIT = 0x07,
  SYS_OSAL_NV_READ = 0x08,
  SYS_OSAL_NV_WRITE = 0x09,
  SYS_OSAL_START_TIMER = 0x0A,
  SYS_OSAL_STOP_TIMER = 0x0B,
  SYS_RANDOM = 0x0C,
  SYS_ADC_READ = 0x0D,
  SYS_GPIO = 0x0E,
  SYS_STACK_TUNE = 0x0F,
  SYS_SET_TIME = 0x10,
  SYS_GET_TIME = 0x11,
  SYS_OSAL_NV_DELETE = 0x12,
  SYS_OSAL_NV_LENGTH = 0x13,
  SYS_TEST_RF = 0x40,
  SYS_TEST_LOOPBACK = 0x41,
  SYS_RESET_IND = 0x80,
  SYS_OSAL_TIMER_EXPIRED = 0x81,
};
// Commands in the SAPI subsystem
enum SapiCommand {
  SAPI_START_REQUEST = 0x00,
  SAPI_BIND_DEVICE = 0x01,
  SAPI_ALLOW_BIND = 0x02,
  SAPI_SEND_DATA_REQUEST = 0x03,
  SAPI_READ_CONFIGURATION = 0x04,
  SAPI_WRITE_CONFIGURATION = 0x05,
  SAPI_GET_DEVICE_INFO = 0x06,
  SAPI_FIND_DEVICE_REQUEST = 0x07,
  SAPI_PERMIT_JOINING_REQUEST = 0x08,
  SAPI_SYSTEM_RESET = 0x09,
  SAPI_START_CONFIRM = 0x80,
  SAPI_BIND_CONFIRM = 0x81,
  SAPI_ALLOW_BIND_CONFIRM = 0x82,
  SAPI_SEND_DATA_CONFIRM = 0x83,
  SAPI_FIND_DEVICE_CONFIRM = 0x85,
  SAPI_RECEIVE_DATA_INDICATION = 0x87,
};
enum Z_configuration {
  CONF_EXTADDR = 0x01,
  CONF_BOOTCOUNTER = 0x02,
  CONF_STARTUP_OPTION = 0x03,
  CONF_START_DELAY = 0x04,
  CONF_NIB = 0x21,
  CONF_DEVICE_LIST = 0x22,
  CONF_ADDRMGR = 0x23,
  CONF_POLL_RATE = 0x24,
  CONF_QUEUED_POLL_RATE = 0x25,
  CONF_RESPONSE_POLL_RATE = 0x26,
  CONF_REJOIN_POLL_RATE = 0x27,
  CONF_DATA_RETRIES = 0x28,
  CONF_POLL_FAILURE_RETRIES = 0x29,
  CONF_STACK_PROFILE = 0x2A,
  CONF_INDIRECT_MSG_TIMEOUT = 0x2B,
  CONF_ROUTE_EXPIRY_TIME = 0x2C,
  CONF_EXTENDED_PAN_ID = 0x2D,
  CONF_BCAST_RETRIES = 0x2E,
  CONF_PASSIVE_ACK_TIMEOUT = 0x2F,
  CONF_BCAST_DELIVERY_TIME = 0x30,
  CONF_NWK_MODE = 0x31,
  CONF_CONCENTRATOR_ENABLE = 0x32,
  CONF_CONCENTRATOR_DISCOVERY = 0x33,
  CONF_CONCENTRATOR_RADIUS = 0x34,
  CONF_CONCENTRATOR_RC = 0x36,
  CONF_NWK_MGR_MODE = 0x37,
  CONF_SRC_RTG_EXPIRY_TIME = 0x38,
  CONF_ROUTE_DISCOVERY_TIME = 0x39,
  CONF_NWK_ACTIVE_KEY_INFO = 0x3A,
  CONF_NWK_ALTERN_KEY_INFO = 0x3B,
  CONF_ROUTER_OFF_ASSOC_CLEANUP = 0x3C,
  CONF_NWK_LEAVE_REQ_ALLOWED = 0x3D,
  CONF_NWK_CHILD_AGE_ENABLE = 0x3E,
  CONF_DEVICE_LIST_KA_TIMEOUT = 0x3F,
  CONF_BINDING_TABLE = 0x41,
  CONF_GROUP_TABLE = 0x42,
  CONF_APS_FRAME_RETRIES = 0x43,
  CONF_APS_ACK_WAIT_DURATION = 0x44,
  CONF_APS_ACK_WAIT_MULTIPLIER = 0x45,
  CONF_BINDING_TIME = 0x46,
  CONF_APS_USE_EXT_PANID = 0x47,
  CONF_APS_USE_INSECURE_JOIN = 0x48,
  CONF_COMMISSIONED_NWK_ADDR = 0x49,
  CONF_APS_NONMEMBER_RADIUS = 0x4B,
  CONF_APS_LINK_KEY_TABLE = 0x4C,
  CONF_APS_DUPREJ_TIMEOUT_INC = 0x4D,
  CONF_APS_DUPREJ_TIMEOUT_COUNT = 0x4E,
  CONF_APS_DUPREJ_TABLE_SIZE = 0x4F,
  CONF_DIAGNOSTIC_STATS = 0x50,
  CONF_SECURITY_LEVEL = 0x61,
  CONF_PRECFGKEY = 0x62,
  CONF_PRECFGKEYS_ENABLE = 0x63,
  CONF_SECURITY_MODE = 0x64,
  CONF_SECURE_PERMIT_JOIN = 0x65,
  CONF_APS_LINK_KEY_TYPE = 0x66,
  CONF_APS_ALLOW_R19_SECURITY = 0x67,
  CONF_IMPLICIT_CERTIFICATE = 0x69,
  CONF_DEVICE_PRIVATE_KEY = 0x6A,
  CONF_CA_PUBLIC_KEY = 0x6B,
  CONF_KE_MAX_DEVICES = 0x6C,
  CONF_USE_DEFAULT_TCLK = 0x6D,
  CONF_RNG_COUNTER = 0x6F,
  CONF_RANDOM_SEED = 0x70,
  CONF_TRUSTCENTER_ADDR = 0x71,
  CONF_USERDESC = 0x81,
  CONF_NWKKEY = 0x82,
  CONF_PANID = 0x83,
  CONF_CHANLIST = 0x84,
  CONF_LEAVE_CTRL = 0x85,
  CONF_SCAN_DURATION = 0x86,
  CONF_LOGICAL_TYPE = 0x87,
  CONF_NWKMGR_MIN_TX = 0x88,
  CONF_NWKMGR_ADDR = 0x89,
  CONF_ZDO_DIRECT_CB = 0x8F,
  CONF_TCLK_TABLE_START = 0x0101,
  ZNP_HAS_CONFIGURED = 0xF00
};

//
enum Z_Status {
  Z_Success = 0x00,
  Z_Failure = 0x01,
  Z_InvalidParameter = 0x02,
  Z_MemError = 0x03,
  Z_Created = 0x09,
  Z_BufferFull = 0x11
};

enum Z_App_Profiles {
  Z_PROF_IPM = 0x0101,    // Industrial Plant Monitoring
  Z_PROF_HA = 0x0104,     // Home Automation -- the only supported right now
  Z_PROF_CBA = 0x0105,    // Commercial Building Automation
  Z_PROF_TA = 0x0107,     // Telecom Applications
  Z_PROF_PHHC = 0x0108,   // Personal Home & Hospital Care
  Z_PROF_AMI = 0x0109,    // Advanced Metering Initiative
};

enum Z_Device_Ids {
  Z_DEVID_CONF_TOOL = 0x0005,
  // from https://www.rfwireless-world.com/Terminology/Zigbee-Profile-ID-list.html
  // Generic	0x0000	ON/OFF Switch
  // 0x0001	Level Control Switch
  // 0x0002	ON/OFF Output
  // 0x0003	Level Controllable Output
  // 0x0004	Scene Selector
  // 0x0005	Configuration Tool
  // 0x0006	Remote control
  // 0x0007	Combined Interface
  // 0x0008	Range Extender
  // 0x0009	Mains Power Outlet
  // Lighting	0x0100	ON/OFF Light
  // 0x0101	Dimmable Light
  // 0x0102	Color Dimmable Light
  // 0x0103	ON/OFF Light Switch
  // 0x0104	Dimmer Switch
  // 0x0105	Color Dimmer Switch
  // 0x0106	Light Sensor
  // 0x0107	Occupancy Sensor
  // Closures	0x0200	Shade
  // 0x0201	Shade Controller
  // HVAC	0x0300	Heating/Cooling Unit
  // 0x0301	Thermostat
  // 0x0302	Temperature Sensor
  // 0x0303	Pump
  // 0x0304	Pump Controller
  // 0x0305	Pressure Sensor
  // 0x0306	Flow sensor
  // Intruder Alarm Systems	0x0400	IAS Control and Indicating Equipment
  // 0x0401	IAS Ancillary Control Equipment
  // 0x0402	IAS Zone
  // 0x0403	IAS Warning Device
};

 enum Z_AddrMode : uint8_t {
  Z_Addr_NotPresent = 0,
  Z_Addr_Group = 1,
  Z_Addr_ShortAddress = 2,
  Z_Addr_IEEEAddress = 3,
  Z_Addr_Broadcast = 0xFF
};

// Commands in the AF subsystem
enum AfCommand : uint8_t {
  AF_REGISTER = 0x00,
  AF_DATA_REQUEST = 0x01,
  AF_DATA_REQUEST_EXT = 0x02,
  AF_DATA_REQUEST_SRC_RTG = 0x03,
  AF_INTER_PAN_CTL = 0x10,
  AF_DATA_STORE = 0x11,
  AF_DATA_RETRIEVE = 0x12,
  AF_APSF_CONFIG_SET = 0x13,
  AF_DATA_CONFIRM = 0x80,
  AF_REFLECT_ERROR = 0x83,
  AF_INCOMING_MSG = 0x81,
  AF_INCOMING_MSG_EXT = 0x82
};

// Commands in the ZDO subsystem
enum : uint8_t {
  ZDO_NWK_ADDR_REQ = 0x00,
  ZDO_IEEE_ADDR_REQ = 0x01,
  ZDO_NODE_DESC_REQ = 0x02,
  ZDO_POWER_DESC_REQ = 0x03,
  ZDO_SIMPLE_DESC_REQ = 0x04,
  ZDO_ACTIVE_EP_REQ = 0x05,
  ZDO_MATCH_DESC_REQ = 0x06,
  ZDO_COMPLEX_DESC_REQ = 0x07,
  ZDO_USER_DESC_REQ = 0x08,
  ZDO_DEVICE_ANNCE = 0x0A,
  ZDO_USER_DESC_SET = 0x0B,
  ZDO_SERVER_DISC_REQ = 0x0C,
  ZDO_END_DEVICE_BIND_REQ = 0x20,
  ZDO_BIND_REQ = 0x21,
  ZDO_UNBIND_REQ = 0x22,
  ZDO_SET_LINK_KEY = 0x23,
  ZDO_REMOVE_LINK_KEY = 0x24,
  ZDO_GET_LINK_KEY = 0x25,
  ZDO_MGMT_NWK_DISC_REQ = 0x30,
  ZDO_MGMT_LQI_REQ = 0x31,
  ZDO_MGMT_RTQ_REQ = 0x32,
  ZDO_MGMT_BIND_REQ = 0x33,
  ZDO_MGMT_LEAVE_REQ = 0x34,
  ZDO_MGMT_DIRECT_JOIN_REQ = 0x35,
  ZDO_MGMT_PERMIT_JOIN_REQ = 0x36,
  ZDO_MGMT_NWK_UPDATE_REQ = 0x37,
  ZDO_MSG_CB_REGISTER = 0x3E,
  ZDO_MGS_CB_REMOVE = 0x3F,
  ZDO_STARTUP_FROM_APP = 0x40,
  ZDO_AUTO_FIND_DESTINATION = 0x41,
  ZDO_EXT_REMOVE_GROUP = 0x47,
  ZDO_EXT_REMOVE_ALL_GROUP = 0x48,
  ZDO_EXT_FIND_ALL_GROUPS_ENDPOINT = 0x49,
  ZDO_EXT_FIND_GROUP = 0x4A,
  ZDO_EXT_ADD_GROUP = 0x4B,
  ZDO_EXT_COUNT_ALL_GROUPS = 0x4C,
  ZDO_NWK_ADDR_RSP = 0x80,
  ZDO_IEEE_ADDR_RSP = 0x81,
  ZDO_NODE_DESC_RSP = 0x82,
  ZDO_POWER_DESC_RSP = 0x83,
  ZDO_SIMPLE_DESC_RSP = 0x84,
  ZDO_ACTIVE_EP_RSP = 0x85,
  ZDO_MATCH_DESC_RSP = 0x86,
  ZDO_COMPLEX_DESC_RSP = 0x87,
  ZDO_USER_DESC_RSP = 0x88,
  ZDO_USER_DESC_CONF = 0x89,
  ZDO_SERVER_DISC_RSP = 0x8A,
  ZDO_END_DEVICE_BIND_RSP = 0xA0,
  ZDO_BIND_RSP = 0xA1,
  ZDO_UNBIND_RSP = 0xA2,
  ZDO_MGMT_NWK_DISC_RSP = 0xB0,
  ZDO_MGMT_LQI_RSP = 0xB1,
  ZDO_MGMT_RTG_RSP = 0xB2,
  ZDO_MGMT_BIND_RSP = 0xB3,
  ZDO_MGMT_LEAVE_RSP = 0xB4,
  ZDO_MGMT_DIRECT_JOIN_RSP = 0xB5,
  ZDO_MGMT_PERMIT_JOIN_RSP = 0xB6,
  ZDO_STATE_CHANGE_IND = 0xC0,
  ZDO_END_DEVICE_ANNCE_IND = 0xC1,
  ZDO_MATCH_DESC_RSP_SENT = 0xC2,
  ZDO_STATUS_ERROR_RSP = 0xC3,
  ZDO_SRC_RTG_IND = 0xC4,
  ZDO_LEAVE_IND = 0xC9,
  ZDO_TC_DEV_IND = 0xCA,
  ZDO_PERMIT_JOIN_IND = 0xCB,
  ZDO_MSG_CB_INCOMING = 0xFF
};

//https://e2e.ti.com/support/wireless-connectivity/zigbee-and-thread/f/158/t/475920
enum ZdoStates {
  ZDO_DEV_HOLD = 0x00,              // Initialized - not started automatically
  ZDO_DEV_INIT = 0x01,              // Initialized - not connected to anything
  ZDO_DEV_NWK_DISC = 0x02,          // Discovering PANIDs to join
  ZDO_DEV_NWK_JOINING = 0x03,       // Joining a PAN
  ZDO_DEV_NWK_REJOIN = 0x04,        // ReJoining a PAN, only for end devices
  ZDO_DEV_END_DEVICE_UNAUTH = 0x05, // Joined but not yet authenticated by trust center
  ZDO_DEV_END_DEVICE = 0x06,        // Started as a device after authentication. Note: you'll see this for both Routers or End Devices.
  ZDO_DEV_ROUTER = 0x07,            // Started as a Zigbee Router
  ZDO_DEV_COORD_STARTING = 0x08,    // Starting as a Zigbee Coordinator
  ZDO_DEV_ZB_COORD = 0x09,          // Started as a a Zigbee Coordinator
  ZDO_DEV_NWK_ORPHAN = 0x0A,        // Device has lost information about its parent.
};

// Commands in the UTIL subsystem
enum Z_Util {
  Z_UTIL_GET_DEVICE_INFO = 0x00,
  Z_UTIL_GET_NV_INFO = 0x01,
  Z_UTIL_SET_PANID = 0x02,
  Z_UTIL_SET_CHANNELS = 0x03,
  Z_UTIL_SET_SECLEVEL = 0x04,
  Z_UTIL_SET_PRECFGKEY = 0x05,
  Z_UTIL_CALLBACK_SUB_CMD = 0x06,
  Z_UTIL_KEY_EVENT = 0x07,
  Z_UTIL_TIME_ALIVE = 0x09,
  Z_UTIL_LED_CONTROL = 0x0A,
  Z_UTIL_TEST_LOOPBACK = 0x10,
  Z_UTIL_DATA_REQ = 0x11,
  Z_UTIL_SRC_MATCH_ENABLE = 0x20,
  Z_UTIL_SRC_MATCH_ADD_ENTRY = 0x21,
  Z_UTIL_SRC_MATCH_DEL_ENTRY = 0x22,
  Z_UTIL_SRC_MATCH_CHECK_SRC_ADDR = 0x23,
  Z_UTIL_SRC_MATCH_ACK_ALL_PENDING = 0x24,
  Z_UTIL_SRC_MATCH_CHECK_ALL_PENDING = 0x25,
  Z_UTIL_ADDRMGR_EXT_ADDR_LOOKUP = 0x40,
  Z_UTIL_ADDRMGR_NWK_ADDR_LOOKUP = 0x41,
  Z_UTIL_APSME_LINK_KEY_DATA_GET = 0x44,
  Z_UTIL_APSME_LINK_KEY_NV_ID_GET = 0x45,
  Z_UTIL_ASSOC_COUNT = 0x48,
  Z_UTIL_ASSOC_FIND_DEVICE = 0x49,
  Z_UTIL_ASSOC_GET_WITH_ADDRESS = 0x4A,
  Z_UTIL_APSME_REQUEST_KEY_CMD = 0x4B,
  Z_UTIL_ZCL_KEY_EST_INIT_EST = 0x80,
  Z_UTIL_ZCL_KEY_EST_SIGN = 0x81,
  Z_UTIL_UTIL_SYNC_REQ = 0xE0,
  Z_UTIL_ZCL_KEY_ESTABLISH_IND = 0xE1
};

enum ZCL_Global_Commands {
  ZCL_READ_ATTRIBUTES = 0x00,
  ZCL_READ_ATTRIBUTES_RESPONSE = 0x01,
  ZCL_WRITE_ATTRIBUTES = 0x02,
  ZCL_WRITE_ATTRIBUTES_UNDIVIDED = 0x03,
  ZCL_WRITE_ATTRIBUTES_RESPONSE = 0x04,
  ZCL_WRITE_ATTRIBUTES_NORESPONSE = 0x05,
  ZCL_CONFIGURE_REPORTING = 0x06,
  ZCL_CONFIGURE_REPORTING_RESPONSE = 0x07,
  ZCL_READ_REPORTING_CONFIGURATION = 0x08,
  ZCL_READ_REPORTING_CONFIGURATION_RESPONSE = 0x09,
  ZCL_REPORT_ATTRIBUTES = 0x0a,
  ZCL_DEFAULT_RESPONSE = 0x0b,
  ZCL_DISCOVER_ATTRIBUTES = 0x0c,
  ZCL_DISCOVER_ATTRIBUTES_RESPONSE = 0x0d

};

const uint16_t Z_ProfileIds[]   PROGMEM = { 0x0104, 0x0109, 0xA10E, 0xC05E };
const char     Z_ProfileNames[] PROGMEM = "ZigBee Home Automation|ZigBee Smart Energy|ZigBee Green Power|ZigBee Light Link";

typedef struct Z_StatusLine {
  uint32_t     status;          // no need to use uint8_t since it uses 32 bits anyways
  const char * status_msg;
} Z_StatusLine;

const Z_StatusLine Z_Status[] PROGMEM = {
  0x00,   "SUCCESS",
  0x01,   "FAILURE",
  0x7E,   "NOT_AUTHORIZED",
  0x7F,   "RESERVED_FIELD_NOT_ZERO",
  0x80,   "MALFORMED_COMMAND",
  0x81,   "UNSUP_CLUSTER_COMMAND",
  0x82,   "UNSUP_GENERAL_COMMAND",
  0x83,   "UNSUP_MANUF_CLUSTER_COMMAND",
  0x84,   "UNSUP_MANUF_GENERAL_COMMAND",
  0x85,   "INVALID_FIELD",
  0x86,   "UNSUPPORTED_ATTRIBUTE",
  0x87,   "INVALID_VALUE",
  0x88,   "READ_ONLY",
  0x89,   "INSUFFICIENT_SPACE",
  0x8A,   "DUPLICATE_EXISTS",
  0x8B,   "NOT_FOUND",
  0x8C,   "UNREPORTABLE_ATTRIBUTE",
  0x8D,   "INVALID_DATA_TYPE",
  0x8E,   "INVALID_SELECTOR",
  0x8F,   "WRITE_ONLY",
  0x90,   "INCONSISTENT_STARTUP_STATE",
  0x91,   "DEFINED_OUT_OF_BAND",
  0x92,   "INCONSISTENT",
  0x93,   "ACTION_DENIED",
  0x94,   "TIMEOUT",
  0x95,   "ABORT",
  0x96,   "INVALID_IMAGE",
  0x97,   "WAIT_FOR_DATA",
  0x98,   "NO_IMAGE_AVAILABLE",
  0x99,   "REQUIRE_MORE_IMAGE",
  0x9A,   "NOTIFICATION_PENDING",
  0xC0,   "HARDWARE_FAILURE",
  0xC1,   "SOFTWARE_FAILURE",
  0xC2,   "CALIBRATION_ERROR",
  0xC3,   "UNSUPPORTED_CLUSTER",
};

const __FlashStringHelper* getZigbeeStatusMessage(uint8_t status) {
  for (uint32_t i = 0; i < sizeof(Z_Status) / sizeof(Z_Status[0]); i++) {
    const Z_StatusLine *statl = &Z_Status[i];
    if (statl->status == status) {
      return (const __FlashStringHelper*) statl->status_msg;
    }
  }
  return nullptr;
}

#endif // USE_ZIGBEE
