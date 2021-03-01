/*
  xdrv_23_zigbee.ino - zigbee support for Tasmota

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

// Status code used for ZigbeeStatus MQTT message
// Ex: {"ZbStatus":{"Status": 3,"Message":"Configured, starting coordinator"}}
const uint8_t  ZIGBEE_STATUS_OK = 0;                    // Zigbee started and working
const uint8_t  ZIGBEE_STATUS_BOOT = 1;                  // CC2530 booting
const uint8_t  ZIGBEE_STATUS_RESET_CONF = 2;            // Resetting CC2530 configuration
const uint8_t  ZIGBEE_STATUS_STARTING = 3;              // Starting CC2530 as coordinator
const uint8_t  ZIGBEE_STATUS_PERMITJOIN_CLOSE = 20;     // Disable PermitJoin
const uint8_t  ZIGBEE_STATUS_PERMITJOIN_OPEN_60 = 21;   // Enable PermitJoin for 60 seconds
const uint8_t  ZIGBEE_STATUS_PERMITJOIN_OPEN_XX = 22;   // Enable PermitJoin until next boot
const uint8_t  ZIGBEE_STATUS_PERMITJOIN_ERROR = 23;     // Enable PermitJoin until next boot
const uint8_t  ZIGBEE_STATUS_DEVICE_ANNOUNCE = 30;      // Device announces its address
const uint8_t  ZIGBEE_STATUS_NODE_DESC = 31;            // Node descriptor
const uint8_t  ZIGBEE_STATUS_ACTIVE_EP = 32;            // Endpoints descriptor
const uint8_t  ZIGBEE_STATUS_SIMPLE_DESC = 33;          // Simple Descriptor (clusters)
const uint8_t  ZIGBEE_STATUS_DEVICE_INDICATION = 34;    // Device announces its address
const uint8_t  ZIGBEE_STATUS_SCANNING = 40;             // State change
const uint8_t  ZIGBEE_STATUS_CC_VERSION = 50;           // Status: CC2530 ZNP Version
const uint8_t  ZIGBEE_STATUS_CC_INFO = 51;              // Status: CC2530 Device Configuration
const uint8_t  ZIGBEE_STATUS_EZ_VERSION = 55;           // Status: EFR32 EZ Version
const uint8_t  ZIGBEE_STATUS_EZ_INFO = 56;              // Status: EFR32 EZ Device Configuration
const uint8_t  ZIGBEE_STATUS_UNSUPPORTED_VERSION = 98;  // Unsupported ZNP version
const uint8_t  ZIGBEE_STATUS_ABORT = 99;                // Fatal error, Zigbee not working

typedef union Zigbee_Instruction {
  struct {
    uint8_t  i;      // instruction
    uint8_t  d8;     // 8 bits data
    uint16_t d16;    // 16 bits data
  } i;
  const void *p;              // pointer
} Zigbee_Instruction;

typedef struct Zigbee_Instruction_Type {
  uint8_t instr;
  uint8_t data;
} Zigbee_Instruction_Type;

enum Zigbee_StateMachine_Instruction_Set {
  // 4 bytes instructions
  ZGB_INSTR_4_BYTES = 0,
  ZGB_INSTR_NOOP = 0,                   // do nothing
  ZGB_INSTR_LABEL,                      // define a label
  ZGB_INSTR_GOTO,                       // goto label
  ZGB_INSTR_ON_ERROR_GOTO,              // goto label if error
  ZGB_INSTR_ON_TIMEOUT_GOTO,            // goto label if timeout
  ZGB_INSTR_WAIT,                       // wait for x ms (in chunks of 100ms)
  ZGB_INSTR_WAIT_FOREVER,               // wait forever but state machine still active
  ZGB_INSTR_STOP,                       // stop state machine with optional error code

  // 8 bytes instructions
  ZGB_INSTR_8_BYTES = 0x80,
  ZGB_INSTR_CALL = 0x80,                // call a function
  ZGB_INSTR_LOG,                        // log a message, if more detailed logging required, call a function
  ZGB_INSTR_MQTT_STATE,                 // send MQTT status string with code
  ZGB_INSTR_SEND,                       // send a ZNP message
  ZGB_INSTR_WAIT_UNTIL,                 // wait until the specified message is received, ignore all others
  ZGB_INSTR_WAIT_RECV,                  // wait for a message according to the filter
  ZGB_ON_RECV_UNEXPECTED,               // function to handle unexpected messages, or nullptr

  // 12 bytes instructions
  ZGB_INSTR_12_BYTES = 0xF0,
  ZGB_INSTR_WAIT_UNTIL_CALL,            // wait until the specified message is received and call function upon receive, ignore all others
  ZGB_INSTR_WAIT_RECV_CALL,             // wait for a filtered message and call function upon receive
};

#define ZI_NOOP()           { .i = { ZGB_INSTR_NOOP,   0x00, 0x0000} },
#define ZI_LABEL(x)         { .i = { ZGB_INSTR_LABEL,  (x),  0x0000} },
#define ZI_GOTO(x)          { .i = { ZGB_INSTR_GOTO,   (x),  0x0000} },
#define ZI_ON_ERROR_GOTO(x) { .i = { ZGB_INSTR_ON_ERROR_GOTO, (x), 0x0000} },
#define ZI_ON_TIMEOUT_GOTO(x) { .i = { ZGB_INSTR_ON_TIMEOUT_GOTO, (x), 0x0000} },
#define ZI_WAIT(x)          { .i = { ZGB_INSTR_WAIT,   0x00, (x)} },
#define ZI_WAIT_FOREVER()   { .i = { ZGB_INSTR_WAIT_FOREVER, 0x00, 0x0000} },
#define ZI_STOP(x)          { .i = { ZGB_INSTR_STOP,   (x), 0x0000} },

#define ZI_CALL(f, x)       { .i = { ZGB_INSTR_CALL, (x), 0x0000} }, { .p = (const void*)(f) },
// #define ZI_CALL2(f, x, y)   { .i = { ZGB_INSTR_CALL, (x), (y)} }, { .p = (const void*)(f) },
#define ZI_LOG(x, m)        { .i = { ZGB_INSTR_LOG,    (x), 0x0000 } }, { .p = ((const void*)(m)) },
#define ZI_MQTT_STATE(x, m) { .i = { ZGB_INSTR_MQTT_STATE,    (x), 0x0000 } }, { .p = ((const void*)(m)) },
#define ZI_ON_RECV_UNEXPECTED(f) { .i = { ZGB_ON_RECV_UNEXPECTED, 0x00, 0x0000} }, { .p = (const void*)(f) },
#define ZI_SEND(m)          { .i = { ZGB_INSTR_SEND, sizeof(m), 0x0000} }, { .p = (const void*)(m) },
#define ZI_WAIT_RECV(x, m)  { .i = { ZGB_INSTR_WAIT_RECV, sizeof(m), (x)} }, { .p = (const void*)(m) },
#define ZI_WAIT_UNTIL(x, m) { .i = { ZGB_INSTR_WAIT_UNTIL, sizeof(m), (x)} }, { .p = (const void*)(m) },
#define ZI_WAIT_UNTIL_FUNC(x, m, f) { .i = { ZGB_INSTR_WAIT_UNTIL_CALL, sizeof(m), (x)} }, { .p = (const void*)(m) }, { .p = (const void*)(f) },
#define ZI_WAIT_RECV_FUNC(x, m, f)  { .i = { ZGB_INSTR_WAIT_RECV_CALL, sizeof(m), (x)} },  { .p = (const void*)(m) }, { .p = (const void*)(f) },

/*********************************************************************************************\
 * State Machine
\*********************************************************************************************/

#define Z_B0(a)            (uint8_t)( ((a)      ) & 0xFF )
#define Z_B1(a)            (uint8_t)( ((a) >>  8) & 0xFF )
#define Z_B2(a)            (uint8_t)( ((a) >> 16) & 0xFF )
#define Z_B3(a)            (uint8_t)( ((a) >> 24) & 0xFF )
#define Z_B4(a)            (uint8_t)( ((a) >> 32) & 0xFF )
#define Z_B5(a)            (uint8_t)( ((a) >> 40) & 0xFF )
#define Z_B6(a)            (uint8_t)( ((a) >> 48) & 0xFF )
#define Z_B7(a)            (uint8_t)( ((a) >> 56) & 0xFF )
// Macro to define message to send and receive
#define ZBM(n, x...) const uint8_t n[] PROGMEM = { x };
// For commands that need to be changed with configuration, ZBR stores in RAM, and ZBW write new values
#define ZBR(n, x...)       uint8_t n[]         = { x };   // same but in RAM to be modified
#define ZBW(n, x...) { const uint8_t n##t[] = { x }; memcpy(n, n##t, sizeof(n)); }   // re-write content in RAM

#define USE_ZIGBEE_CHANNEL_MASK (1 << (USE_ZIGBEE_CHANNEL))

const char kCheckingDeviceConfiguration[] PROGMEM = D_LOG_ZIGBEE "checking device configuration";
const char kConfiguredCoord[] PROGMEM = "Configured, starting coordinator";
const char kConfiguredRouter[] PROGMEM = "Configured, starting router";
const char kConfiguredDevice[] PROGMEM = "Configured, starting device";
const char kStarted[] PROGMEM = "Started";
const char kZigbeeStarted[] PROGMEM = D_LOG_ZIGBEE "Zigbee started";
const char kResetting[] PROGMEM = "Resetting configuration";
const char kResettingDevice[] PROGMEM = D_LOG_ZIGBEE "Resetting EZSP device";
const char kReconfiguringDevice[] PROGMEM = D_LOG_ZIGBEE "Factory reset EZSP device";
const char kZNP123[] PROGMEM = "Only ZNP 1.2 and 3.x are currently supported";
const char kEZ8[] PROGMEM = "Only EZSP protocol v8 is currently supported";
const char kAbort[] PROGMEM = "Abort";
const char kZigbeeAbort[] PROGMEM = D_LOG_ZIGBEE "Abort";
const char kZigbeeGroup0[] PROGMEM = D_LOG_ZIGBEE "Subscribe to group 0 'ZbListen0 0'";

#ifdef USE_ZIGBEE_ZNP

// ZBS_* Zigbee Send
// ZBR_* Zigbee Recv
ZBM(ZBS_RESET, Z_AREQ | Z_SYS, SYS_RESET, 0x00 )        	  // 410001 SYS_RESET_REQ Hardware reset
ZBM(ZBR_RESET, Z_AREQ | Z_SYS, SYS_RESET_IND )              // 4180 SYS_RESET_REQ Hardware reset response

ZBM(ZBS_VERSION, Z_SREQ | Z_SYS, SYS_VERSION )              // 2102 Z_SYS:version
ZBM(ZBR_VERSION, Z_SRSP | Z_SYS, SYS_VERSION )              // 6102 Z_SYS:version

// Check if ZNP_HAS_CONFIGURED is set
ZBR(ZBS_ZNPHC, Z_SREQ | Z_SYS, SYS_OSAL_NV_READ, ZNP_HAS_CONFIGURED & 0xFF, ZNP_HAS_CONFIGURED >> 8, 0x00 /* offset */ )  // 2108000F00 - 6108000155
ZBM(ZBR_ZNPHC, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS, 0x01 /* len */, 0x55)   // 6108000155
// If not set, the response is 61-08-02-00 = Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_INVALIDPARAMETER, 0x00 /* len */

ZBM(ZBS_PAN, Z_SREQ | Z_SYS, SYS_OSAL_NV_READ, CONF_PANID,0x00, 0x00 )				// 2108830000
ZBR(ZBR_PAN, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS, 0x02 /* len */,
              0x00, 0x00 /* pan_id */ )				// 61080002xxxx

ZBM(ZBS_EXTPAN, Z_SREQ | Z_SYS, SYS_OSAL_NV_READ, CONF_EXTENDED_PAN_ID,0x00, 0x00 )				// 21082D0000
ZBR(ZBR_EXTPAN, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS,
                0x08 /* len */,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ext_pan_id */
                )				// 61080008xxxxxxxxxxxxxxxx

ZBM(ZBS_CHANN, Z_SREQ | Z_SYS, SYS_OSAL_NV_READ, CONF_CHANLIST,0x00, 0x00 )				// 2108840000
ZBR(ZBR_CHANN, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS,
               0x04 /* len */,
               Z_B0(USE_ZIGBEE_CHANNEL_MASK), Z_B1(USE_ZIGBEE_CHANNEL_MASK), Z_B2(USE_ZIGBEE_CHANNEL_MASK), Z_B3(USE_ZIGBEE_CHANNEL_MASK),
               )				// 61080004xxxxxxxx

ZBM(ZBS_PFGK, Z_SREQ | Z_SAPI, SAPI_READ_CONFIGURATION, CONF_PRECFGKEY )				// 260462
ZBR(ZBR_PFGK, Z_SRSP | Z_SAPI, SAPI_READ_CONFIGURATION, Z_SUCCESS, CONF_PRECFGKEY,
              0x10 /* len */,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* key_l */
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* key_h */
              /*0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F,
              0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0D*/ )				// 660400621001030507090B0D0F00020406080A0C0D

ZBM(ZBS_PFGK3, Z_SREQ | Z_SYS, SYS_OSAL_NV_READ, CONF_PRECFGKEY,0x00, 0x00 )				// 2108620000
ZBR(ZBR_PFGK3, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS,
              0x10 /* len */,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* key_l */
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* key_h */
              /*0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F,
              0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0D*/ )				// 6108001001030507090B0D0F00020406080A0C0D

ZBM(ZBS_PFGKEN, Z_SREQ | Z_SYS, SYS_OSAL_NV_READ, CONF_PRECFGKEYS_ENABLE,0x00, 0x00 )				// 2108630000
ZBM(ZBR_PFGKEN, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS,
                0x01 /* len */, 0x00 )				// 6108000100

ZBM(ZBS_LOGTYPE, Z_SREQ | Z_SYS, SYS_OSAL_NV_READ, CONF_LOGICAL_TYPE,0x00, 0x00 )				// 2108870000
ZBM(ZBS_LOGTYPE_COORD,  Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS,
                0x01 /* len */, 0x00 )				// 6108000100 - coordinator
ZBM(ZBS_LOGTYPE_ROUTER, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS,
                0x01 /* len */, 0x01 )				// 6108000101 - router
ZBM(ZBS_LOGTYPE_DEVICE, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS,
                0x01 /* len */, 0x02 )				// 6108000102 - device


// commands to "format" the device
// Write configuration - write success
ZBM(ZBR_WNV_OK, Z_SRSP | Z_SYS, SYS_OSAL_NV_WRITE, Z_SUCCESS )				// 610900 - NV Write
ZBM(ZBR_WNV_OKE, Z_SRSP | Z_SYS, SYS_OSAL_NV_WRITE )				          // 6109xx - NV Write, OK or error

// Factory reset
ZBM(ZBS_FACTRES, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_STARTUP_OPTION,0x00, 0x00, 0x01 /* len */, 0x03 )				// 21090300000103
// Write PAN ID
ZBR(ZBS_W_PAN, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_PANID,0x00, 0x00, 0x02 /* len */, 0x00, 0x00 /* pan_id */  )				// 210983000002xxxx
// Write Universal PAN ID
ZBR(ZBS_W_ALL_PAN, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_PANID,0x00, 0x00, 0x02 /* len */, Z_B0(0xFFFF), Z_B1(0xFFFF)  )				// 210983000002FFFF
// Write EXT PAN ID
ZBR(ZBS_W_EXTPAN, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_EXTENDED_PAN_ID,0x00, 0x00, 0x08 /* len */,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ext_pan_id */
                  ) // 21092D0000086263151D004B1200
// Write Channel ID
ZBR(ZBS_W_CHANN, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_CHANLIST,0x00, 0x00, 0x04 /* len */,
                Z_B0(USE_ZIGBEE_CHANNEL_MASK), Z_B1(USE_ZIGBEE_CHANNEL_MASK), Z_B2(USE_ZIGBEE_CHANNEL_MASK), Z_B3(USE_ZIGBEE_CHANNEL_MASK),
                /*0x00, 0x08, 0x00, 0x00*/ )				// 210984000004xxxxxxxx
// Write All Channels
const uint32_t ZB_ALL_CHANNELS = 0x07FFF800;
ZBR(ZBS_W_ALL_CHANN, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_CHANLIST,0x00, 0x00, 0x04 /* len */,
                Z_B0(ZB_ALL_CHANNELS), Z_B1(ZB_ALL_CHANNELS), Z_B2(ZB_ALL_CHANNELS), Z_B3(ZB_ALL_CHANNELS),
                /*0x00, 0x08, 0x00, 0x00*/ )				// 21098400000400F8FF7F
// Write Logical Type = 00 = coordinator
ZBM(ZBS_W_LOGTYP_COORD,  Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_LOGICAL_TYPE,0x00, 0x00, 0x01 /* len */, 0x00 )				// 21098700000100
// Write Logical Type = 01 = router
ZBM(ZBS_W_LOGTYP_ROUTER, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_LOGICAL_TYPE,0x00, 0x00, 0x01 /* len */, 0x01 )				// 21098700000101
// Write Logical Type = 02 = device
ZBM(ZBS_W_LOGTYP_DEVICE, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_LOGICAL_TYPE,0x00, 0x00, 0x01 /* len */, 0x02 )				// 21098700000102
// Write precfgkey
ZBR(ZBS_W_PFGK, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_PRECFGKEY,0x00, 0x00,
                0x10 /* len */,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* key_l */
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* key_h */                /*0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F,
                0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0D*/ )				// 21096200001001030507090B0D0F00020406080A0C0D
// Write precfgkey enable
ZBM(ZBS_W_PFGKEN, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_PRECFGKEYS_ENABLE,0x00, 0x00, 0x01 /* len */, 0x00 )				// 21096300000100
// Write Security Mode
ZBM(ZBS_WNV_SECMODE, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, Z_B0(CONF_TCLK_TABLE_START), Z_B1(CONF_TCLK_TABLE_START),
                      0x00 /* offset */, 0x20 /* len */,
                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                      0x5a, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6c,
                      0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0x30, 0x39,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)				// 2109010100200FFFFFFFFFFFFFFFF5A6967426565416C6C69616E636530390000000000000000
// Write Z_ZDO Direct CB
ZBM(ZBS_W_ZDODCB, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_ZDO_DIRECT_CB,0x00, 0x00, 0x01 /* len */, 0x01 )				// 21098F00000101
// NV Init ZNP Has Configured
ZBR(ZBS_WNV_INITZNPHC, Z_SREQ | Z_SYS, SYS_OSAL_NV_ITEM_INIT, ZNP_HAS_CONFIGURED & 0xFF, ZNP_HAS_CONFIGURED >> 8,
                       0x01, 0x00 /* InitLen 16 bits */, 0x01 /* len */, 0x00 )  // 2107000F01000100 - 610709
// Init succeeded
//ZBM(ZBR_WNV_INIT_OK, Z_SRSP | Z_SYS, SYS_OSAL_NV_ITEM_INIT, Z_CREATED )				// 610709 - NV Write
ZBM(ZBR_WNV_INIT_OK, Z_SRSP | Z_SYS, SYS_OSAL_NV_ITEM_INIT )				  // 6107xx, Success if 610700 or 610709 - NV Write

// Write ZNP Has Configured
ZBR(ZBS_WNV_ZNPHC, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, Z_B0(ZNP_HAS_CONFIGURED), Z_B1(ZNP_HAS_CONFIGURED),
                   0x00 /* offset */, 0x01 /* len */, 0x55 )				// 2109000F000155 - 610900
// Z_ZDO:startupFromApp
ZBM(ZBS_STARTUPFROMAPP, Z_SREQ | Z_ZDO, ZDO_STARTUP_FROM_APP, 100, 0 /* delay */)   // 25406400
ZBM(ZBR_STARTUPFROMAPP, Z_SRSP | Z_ZDO, ZDO_STARTUP_FROM_APP )   // 6540 + 01 for new network, 00 for exisitng network, 02 for error
ZBM(AREQ_STARTUPFROMAPP, Z_AREQ | Z_ZDO, ZDO_STATE_CHANGE_IND )    // 45C00xx - state change
ZBM(AREQ_STARTUPFROMAPP_COORD, Z_AREQ | Z_ZDO, ZDO_STATE_CHANGE_IND, ZDO_DEV_ZB_COORD )    // 45C009 + 08 = starting, 09 = started
ZBM(AREQ_STARTUPFROMAPP_ROUTER, Z_AREQ | Z_ZDO, ZDO_STATE_CHANGE_IND, ZDO_DEV_ROUTER )    // 45C009 + 02 = looking PanID, 07 = started
ZBM(AREQ_STARTUPFROMAPP_DEVICE, Z_AREQ | Z_ZDO, ZDO_STATE_CHANGE_IND, ZDO_DEV_END_DEVICE )    // 45C009 + 02 = looking PanID, 06 = started
// GetDeviceInfo
ZBM(ZBS_GETDEVICEINFO, Z_SREQ | Z_UTIL, Z_UTIL_GET_DEVICE_INFO )     // 2700
ZBM(ZBR_GETDEVICEINFO, Z_SRSP | Z_UTIL, Z_UTIL_GET_DEVICE_INFO, Z_SUCCESS )   // Ex= 6700.00.6263151D004B1200.0000.07.09.00
    // IEEE Adr (8 bytes) = 6263151D004B1200
    // Short Addr (2 bytes) = 0000
    // Device Type (1 byte) = 07 (coord?)
    // Device State (1 byte) = 09 (coordinator started)
    // NumAssocDevices (1 byte) = 00

// Read Pan ID
//ZBM(ZBS_READ_NV_PANID, Z_SREQ | Z_SYS, SYS_OSAL_NV_READ, PANID & 0xFF, PANID >> 8, 0x00 /* offset */ )  // 2108830000

// Z_ZDO:nodeDescReq
ZBM(ZBS_ZDO_NODEDESCREQ, Z_SREQ | Z_ZDO, ZDO_NODE_DESC_REQ, 0x00, 0x00 /* dst addr */, 0x00, 0x00 /* NWKAddrOfInterest */)    // 250200000000
ZBM(ZBR_ZDO_NODEDESCREQ, Z_SRSP | Z_ZDO, ZDO_NODE_DESC_REQ, Z_SUCCESS )   // 650200
// Async resp ex: 4582.0000.00.0000.00.40.8F.0000.50.A000.0100.A000.00
ZBM(AREQ_ZDO_NODEDESCRSP, Z_AREQ | Z_ZDO, ZDO_NODE_DESC_RSP)    // 4582
// SrcAddr (2 bytes) 0000
// Status (1 byte) 00 Success
// NwkAddr (2 bytes) 0000
// LogicalType (1 byte) - 00 Coordinator
// APSFlags (1 byte) - 40 0=APSFlags 4=NodeFreqBands
// MACCapabilityFlags (1 byte) - 8F ALL
// ManufacturerCode (2 bytes) - 0000
// MaxBufferSize (1 byte) - 50 NPDU
// MaxTransferSize (2 bytes) - A000 = 160
// ServerMask (2 bytes) - 0100 - Primary Trust Center
// MaxOutTransferSize (2 bytes) - A000 = 160
// DescriptorCapabilities (1 byte) - 00

// Z_ZDO:activeEpReq
ZBM(ZBS_ZDO_ACTIVEEPREQ, Z_SREQ | Z_ZDO, ZDO_ACTIVE_EP_REQ, 0x00, 0x00, 0x00, 0x00)  // 250500000000
ZBM(ZBR_ZDO_ACTIVEEPREQ, Z_SRSP | Z_ZDO, ZDO_ACTIVE_EP_REQ, Z_SUCCESS)  // 65050000
ZBM(ZBR_ZDO_ACTIVEEPRSP_NONE, Z_AREQ | Z_ZDO, ZDO_ACTIVE_EP_RSP, 0x00, 0x00 /* srcAddr */, Z_SUCCESS,
    0x00, 0x00 /* nwkaddr */, 0x00 /* activeepcount */)  // 45050000 - no Ep running
ZBM(ZBR_ZDO_ACTIVEEPRSP_OK, Z_AREQ | Z_ZDO, ZDO_ACTIVE_EP_RSP, 0x00, 0x00 /* srcAddr */, Z_SUCCESS,
    0x00, 0x00 /* nwkaddr */, 0x02 /* activeepcount */, 0x0B, 0x01 /* the actual endpoints */)  // 25050000 - no Ep running

// Z_AF:register profile:104, ep:01
ZBM(ZBS_AF_REGISTER01, Z_SREQ | Z_AF, AF_REGISTER, 0x01 /* endpoint */, Z_B0(Z_PROF_HA), Z_B1(Z_PROF_HA),    // 24000401050000000000
                        0x05, 0x00 /* AppDeviceId */, 0x00 /* AppDevVer */, 0x00 /* LatencyReq */,
                        0x00 /* AppNumInClusters */, 0x00 /* AppNumInClusters */)
ZBM(ZBR_AF_REGISTER,   Z_SRSP | Z_AF, AF_REGISTER, Z_SUCCESS)   // 640000
ZBM(ZBS_AF_REGISTER0B, Z_SREQ | Z_AF, AF_REGISTER, 0x0B /* endpoint */, Z_B0(Z_PROF_HA), Z_B1(Z_PROF_HA),    // 2400040B050000000000
                        0x05, 0x00 /* AppDeviceId */, 0x00 /* AppDevVer */, 0x00 /* LatencyReq */,
                        0x00 /* AppNumInClusters */, 0x00 /* AppNumInClusters */)
// Z_AF:register profile:104, ep:01 - main clusters for router or device
ZBM(ZBS_AF_REGISTER_ALL, Z_SREQ | Z_AF, AF_REGISTER, 0x01 /* endpoint */, Z_B0(Z_PROF_HA), Z_B1(Z_PROF_HA),    // 24000401050000000000
                        0x05, 0x00 /* AppDeviceId */, 0x00 /* AppDevVer */, 0x00 /* LatencyReq */,
                        0x0E /* AppNumInClusters */,                        // actually all clusters will be received
                        0x00,0x00,  0x04,0x00,  0x05,0x00,  0x06,0x00,      // 0x0000, 0x0004, 0x0005, 0x0006
                        0x07,0x00,  0x08,0x00,  0x0A,0x00,  0x02,0x01,      // 0x0007, 0x0008, 0x000A, 0X0102
                        0x00,0x03,  0x00,0x04,  0x02,0x04,  0x03,0x04,      // 0x0300, 0x0400, 0x0402, 0x0403
                        0x05,0x04,  0x06,0x04,                              // 0x0405, 0x0406
                        0x00 /* AppNumInClusters */)

// Z_ZDO:mgmtPermitJoinReq
ZBM(ZBS_PERMITJOINREQ_CLOSE, Z_SREQ | Z_ZDO, ZDO_MGMT_PERMIT_JOIN_REQ, 0x02 /* AddrMode */,   // 25360200000000
                              0x00, 0x00 /* DstAddr */, 0x00 /* Duration */, 0x00 /* TCSignificance */)
ZBM(ZBR_PERMITJOINREQ, Z_SRSP | Z_ZDO, ZDO_MGMT_PERMIT_JOIN_REQ, Z_SUCCESS)    // 653600
ZBM(ZBR_PERMITJOIN_AREQ_RSP,  Z_AREQ | Z_ZDO, ZDO_MGMT_PERMIT_JOIN_RSP, 0x00, 0x00 /* srcAddr*/, Z_SUCCESS )   // 45B6000000

// Update the relevant commands with Settings
void ZNP_UpdateConfig(uint8_t zb_channel, uint16_t zb_pan_id, uint64_t zb_ext_panid, uint64_t zb_precfgkey_l, uint64_t zb_precfgkey_h) {
  uint32_t zb_channel_mask = (1 << zb_channel);

  ZBW(ZBR_PAN, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS, 0x02 /* len */,
                Z_B0(zb_pan_id), Z_B1(zb_pan_id) )				// 61080002xxxx

  ZBW(ZBR_EXTPAN, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS,
                  0x08 /* len */,
                  Z_B0(zb_ext_panid), Z_B1(zb_ext_panid), Z_B2(zb_ext_panid), Z_B3(zb_ext_panid),
                  Z_B4(zb_ext_panid), Z_B5(zb_ext_panid), Z_B6(zb_ext_panid), Z_B7(zb_ext_panid),
                  )				// 61080008xxxxxxxxxxxxxxxx

  ZBW(ZBR_CHANN, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS,
                0x04 /* len */,
                Z_B0(zb_channel_mask), Z_B1(zb_channel_mask), Z_B2(zb_channel_mask), Z_B3(zb_channel_mask),
                )				// 61080004xxxxxxxx
  // Zstack 1.2
  ZBW(ZBR_PFGK, Z_SRSP | Z_SAPI, SAPI_READ_CONFIGURATION, Z_SUCCESS, CONF_PRECFGKEY,
                0x10 /* len */,
                Z_B0(zb_precfgkey_l), Z_B1(zb_precfgkey_l), Z_B2(zb_precfgkey_l), Z_B3(zb_precfgkey_l),
                Z_B4(zb_precfgkey_l), Z_B5(zb_precfgkey_l), Z_B6(zb_precfgkey_l), Z_B7(zb_precfgkey_l),
                Z_B0(zb_precfgkey_h), Z_B1(zb_precfgkey_h), Z_B2(zb_precfgkey_h), Z_B3(zb_precfgkey_h),
                Z_B4(zb_precfgkey_h), Z_B5(zb_precfgkey_h), Z_B6(zb_precfgkey_h), Z_B7(zb_precfgkey_h),
                /*0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F,
                0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0D*/ )				// 660400621001030507090B0D0F00020406080A0C0D
  // ZStack 3
  ZBW(ZBR_PFGK3, Z_SRSP | Z_SYS, SYS_OSAL_NV_READ, Z_SUCCESS,
                0x10 /* len */,
                Z_B0(zb_precfgkey_l), Z_B1(zb_precfgkey_l), Z_B2(zb_precfgkey_l), Z_B3(zb_precfgkey_l),
                Z_B4(zb_precfgkey_l), Z_B5(zb_precfgkey_l), Z_B6(zb_precfgkey_l), Z_B7(zb_precfgkey_l),
                Z_B0(zb_precfgkey_h), Z_B1(zb_precfgkey_h), Z_B2(zb_precfgkey_h), Z_B3(zb_precfgkey_h),
                Z_B4(zb_precfgkey_h), Z_B5(zb_precfgkey_h), Z_B6(zb_precfgkey_h), Z_B7(zb_precfgkey_h),
                /*0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F,
                0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0D*/ )				// 6108001001030507090B0D0F00020406080A0C0D

  ZBW(ZBS_W_PAN, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_PANID,0x00, 0x00, 0x02 /* len */, Z_B0(zb_pan_id), Z_B1(zb_pan_id)  )				// 210983000002xxxx
  // Write EXT PAN ID
  ZBW(ZBS_W_EXTPAN, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_EXTENDED_PAN_ID,0x00, 0x00, 0x08 /* len */,
                    Z_B0(zb_ext_panid), Z_B1(zb_ext_panid), Z_B2(zb_ext_panid), Z_B3(zb_ext_panid),
                    Z_B4(zb_ext_panid), Z_B5(zb_ext_panid), Z_B6(zb_ext_panid), Z_B7(zb_ext_panid)
                    ) // 21092D0000086263151D004B1200
  // Write Channel ID
  ZBW(ZBS_W_CHANN, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_CHANLIST,0x00, 0x00, 0x04 /* len */,
                  Z_B0(zb_channel_mask), Z_B1(zb_channel_mask), Z_B2(zb_channel_mask), Z_B3(zb_channel_mask),
                  /*0x00, 0x08, 0x00, 0x00*/ )				// 210984000004xxxxxxxx
  // Write precfgkey
  ZBW(ZBS_W_PFGK, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, CONF_PRECFGKEY,0x00, 0x00,
                  0x10 /* len */,
                  Z_B0(zb_precfgkey_l), Z_B1(zb_precfgkey_l), Z_B2(zb_precfgkey_l), Z_B3(zb_precfgkey_l),
                  Z_B4(zb_precfgkey_l), Z_B5(zb_precfgkey_l), Z_B6(zb_precfgkey_l), Z_B7(zb_precfgkey_l),
                  Z_B0(zb_precfgkey_h), Z_B1(zb_precfgkey_h), Z_B2(zb_precfgkey_h), Z_B3(zb_precfgkey_h),
                  Z_B4(zb_precfgkey_h), Z_B5(zb_precfgkey_h), Z_B6(zb_precfgkey_h), Z_B7(zb_precfgkey_h),
                  )				// 21096200001001030507090B0D0F00020406080A0C0D
}

// Update configuration for ZStack 3
void ZNP_UpdateZStack3(void) {
  ZBW(ZBS_ZNPHC, Z_SREQ | Z_SYS, SYS_OSAL_NV_READ, ZNP_HAS_CONFIGURED3 & 0xFF, ZNP_HAS_CONFIGURED3 >> 8, 0x00 /* offset */ )  // 2108000F00 - 6108000155

  ZBW(ZBS_WNV_INITZNPHC, Z_SREQ | Z_SYS, SYS_OSAL_NV_ITEM_INIT, ZNP_HAS_CONFIGURED3 & 0xFF, ZNP_HAS_CONFIGURED3 >> 8,
                        0x01, 0x00 /* InitLen 16 bits */, 0x01 /* len */, 0x00 )  // 2107000F01000100 - 610709

  ZBW(ZBS_WNV_ZNPHC, Z_SREQ | Z_SYS, SYS_OSAL_NV_WRITE, Z_B0(ZNP_HAS_CONFIGURED3), Z_B1(ZNP_HAS_CONFIGURED3),
                    0x00 /* offset */, 0x01 /* len */, 0x55 )				// 2109000F000155 - 610900
}

static const Zigbee_Instruction zb_prog[] PROGMEM = {
  ZI_LABEL(0)
    ZI_NOOP()
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_ABORT)
    ZI_ON_TIMEOUT_GOTO(ZIGBEE_LABEL_ABORT)
    ZI_ON_RECV_UNEXPECTED(&ZNP_Recv_Default)
    ZI_WAIT(15500)                             // wait for 15 seconds for Tasmota to stabilize

    //ZI_MQTT_STATE(ZIGBEE_STATUS_BOOT, "Booting")
    ZI_LOG(LOG_LEVEL_INFO, D_LOG_ZIGBEE "rebooting CC2530 device")

    ZI_CALL(&ZNP_Reset_Device, 0)         // LOW = reset
    ZI_WAIT(100)                          // wait for .1 second
    ZI_CALL(&ZNP_Reset_Device, 1)         // HIGH = release reset
    ZI_WAIT_RECV_FUNC(5000, ZBR_RESET, &ZNP_Reboot)             // timeout 5s
    ZI_WAIT(100)
    ZI_LOG(LOG_LEVEL_DEBUG, kCheckingDeviceConfiguration)     // Log Debug: checking device configuration
    ZI_SEND(ZBS_VERSION)                      // check ZNP software version
    ZI_WAIT_RECV_FUNC(2000, ZBR_VERSION, &ZNP_ReceiveCheckVersion)  // Check if version is valid

    // Dispatching whether coordinator, router or end-device
    ZI_CALL(&Z_SwitchDeviceType, 0)           // goto ZIGBEE_LABEL_INIT_ROUTER, ZIGBEE_LABEL_INIT_DEVICE or continue if coordinator

    // ======================================================================
    // Start as Zigbee Coordinator
    // ======================================================================
    // Check the configuration as Coordinator
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_FACT_RESET_COORD)
    ZI_SEND(ZBS_ZNPHC)                        // check value of ZNP Has Configured
    ZI_WAIT_RECV(2000, ZBR_ZNPHC)

    ZI_SEND(ZBS_LOGTYPE)                      // check the logical type
    ZI_WAIT_RECV(1000, ZBS_LOGTYPE_COORD)     // it should be coordinator
    ZI_SEND(ZBS_PAN)                          // check PAN ID
    ZI_WAIT_RECV(1000, ZBR_PAN)
    ZI_SEND(ZBS_EXTPAN)                       // check EXT PAN ID
    ZI_WAIT_RECV(1000, ZBR_EXTPAN)
    ZI_SEND(ZBS_CHANN)                        // check CHANNEL
    ZI_WAIT_RECV(1000, ZBR_CHANN)
    ZI_SEND(ZBS_PFGKEN)                       // check PFGKEN
    ZI_WAIT_RECV(1000, ZBR_PFGKEN)

    ZI_CALL(&Z_GotoZB3, ZIGBEE_LABEL_ZB3_INIT)
    ZI_SEND(ZBS_PFGK)                         // check PFGK on ZB1.2
    ZI_WAIT_RECV(1000, ZBR_PFGK)
    ZI_GOTO(ZIGBEE_LABEL_START_COORD)

  ZI_LABEL(ZIGBEE_LABEL_ZB3_INIT)
    ZI_SEND(ZBS_PFGK3)                        // check PFGK on ZB3
    ZI_WAIT_RECV(1000, ZBR_PFGK3)
    //ZI_LOG(LOG_LEVEL_INFO, D_LOG_ZIGBEE "zigbee configuration ok")
    // all is good, we can start

  ZI_LABEL(ZIGBEE_LABEL_START_COORD)                // START ZNP App
    ZI_MQTT_STATE(ZIGBEE_STATUS_STARTING, kConfiguredCoord)
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_ABORT)
    // Z_ZDO:startupFromApp
    //ZI_LOG(LOG_LEVEL_INFO, D_LOG_ZIGBEE "starting zigbee coordinator")
    ZI_SEND(ZBS_STARTUPFROMAPP)                       // start coordinator
    ZI_WAIT_RECV(5000, ZBR_STARTUPFROMAPP)        // wait for sync ack of command
    ZI_WAIT_UNTIL_FUNC(20000, AREQ_STARTUPFROMAPP, &ZNP_ReceiveStateChange)      // wait for async message that coordinator started, max 20s
    ZI_SEND(ZBS_GETDEVICEINFO)                    // GetDeviceInfo
    ZI_WAIT_RECV_FUNC(2000, ZBR_GETDEVICEINFO, &ZNP_ReceiveDeviceInfo)
    //ZI_WAIT_RECV(2000, ZBR_GETDEVICEINFO)         // memorize info
    ZI_SEND(ZBS_ZDO_NODEDESCREQ)                  // Z_ZDO:nodeDescReq
    ZI_WAIT_RECV(1000, ZBR_ZDO_NODEDESCREQ)
    ZI_WAIT_UNTIL(5000, AREQ_ZDO_NODEDESCRSP)
    ZI_SEND(ZBS_ZDO_ACTIVEEPREQ)                  // Z_ZDO:activeEpReq
    ZI_WAIT_RECV(1000, ZBR_ZDO_ACTIVEEPREQ)
    ZI_WAIT_UNTIL(1000, ZBR_ZDO_ACTIVEEPRSP_NONE)
    ZI_SEND(ZBS_AF_REGISTER01)                    // Z_AF register for endpoint 01, profile 0x0104 Home Automation
    ZI_WAIT_RECV(1000, ZBR_AF_REGISTER)
    ZI_SEND(ZBS_AF_REGISTER0B)                    // Z_AF register for endpoint 0B, profile 0x0104 Home Automation
    ZI_WAIT_RECV(1000, ZBR_AF_REGISTER)
    // redo Z_ZDO:activeEpReq to check that Ep are available
    ZI_SEND(ZBS_ZDO_ACTIVEEPREQ)                  // Z_ZDO:activeEpReq
    ZI_WAIT_RECV(1000, ZBR_ZDO_ACTIVEEPREQ)
    ZI_WAIT_UNTIL(1000, ZBR_ZDO_ACTIVEEPRSP_OK)
    ZI_SEND(ZBS_PERMITJOINREQ_CLOSE)              // Closing the Permit Join
    ZI_WAIT_RECV(1000, ZBR_PERMITJOINREQ)
    ZI_WAIT_UNTIL(1000, ZBR_PERMITJOIN_AREQ_RSP)

    // ======================================================================
    // Correctly configured and running, enable all Tasmota features
    // ======================================================================
  ZI_LABEL(ZIGBEE_LABEL_READY)
    ZI_MQTT_STATE(ZIGBEE_STATUS_OK, kStarted)
    ZI_LOG(LOG_LEVEL_INFO, kZigbeeStarted)
    ZI_CALL(&Z_State_Ready, 1)                    // Now accept incoming messages
    ZI_CALL(&Z_Load_Devices, 0)
    ZI_CALL(&Z_Query_Bulbs, 0)
  ZI_LABEL(ZIGBEE_LABEL_MAIN_LOOP)
    ZI_WAIT_FOREVER()
    ZI_GOTO(ZIGBEE_LABEL_READY)

  ZI_LABEL(ZIGBEE_LABEL_FACT_RESET_COORD)                                    // reformat device
    ZI_MQTT_STATE(ZIGBEE_STATUS_RESET_CONF, kResetting)
    //ZI_LOG(LOG_LEVEL_INFO, D_LOG_ZIGBEE "zigbee bad configuration of device, doing a factory reset")
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_ABORT)
    ZI_SEND(ZBS_FACTRES)                          // factory reset
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    ZI_SEND(ZBS_RESET)                            // reset device
    ZI_WAIT_RECV(5000, ZBR_RESET)
    ZI_SEND(ZBS_W_PAN)                            // write PAN ID
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    ZI_SEND(ZBS_W_EXTPAN)                         // write EXT PAN ID
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    ZI_SEND(ZBS_W_CHANN)                          // write CHANNEL
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    ZI_SEND(ZBS_W_LOGTYP_COORD)                   // write Logical Type = coordinator
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    ZI_SEND(ZBS_W_PFGK)                           // write PRECFGKEY
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    ZI_SEND(ZBS_W_PFGKEN)                         // write PRECFGKEY Enable
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    ZI_SEND(ZBS_WNV_SECMODE)                      // write Security Mode
    ZI_WAIT_RECV(1000, ZBR_WNV_OKE)               // Tolerate error for ZNP 3.x
    ZI_SEND(ZBS_W_ZDODCB)                         // write Z_ZDO Direct CB
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    // Now mark the device as ready, writing 0x55 in memory slot 0x0F00
    ZI_SEND(ZBS_WNV_INITZNPHC)                    // Init NV ZNP Has Configured
    ZI_WAIT_RECV_FUNC(1000, ZBR_WNV_INIT_OK, &ZNP_CheckNVWrite)
    ZI_SEND(ZBS_WNV_ZNPHC)                        // Write NV ZNP Has Configured
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)

    //ZI_LOG(LOG_LEVEL_INFO, D_LOG_ZIGBEE "zigbee device reconfigured")
    ZI_GOTO(ZIGBEE_LABEL_START_COORD)

    // ======================================================================
    // Start as Zigbee Router
    // ======================================================================
  ZI_LABEL(ZIGBEE_LABEL_INIT_ROUTER)               // Init as a router
    // Check the configuration as Router
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_FACT_RESET_ROUTER)
    ZI_SEND(ZBS_ZNPHC)                            // check value of ZNP Has Configured
    ZI_WAIT_RECV(2000, ZBR_ZNPHC)
    ZI_SEND(ZBS_LOGTYPE)                          // check the logical type
    ZI_WAIT_RECV(1000, ZBS_LOGTYPE_ROUTER)        // it should be coordinator

  // ZI_LABEL(ZIGBEE_LABEL_START_ROUTER)              // Init as a router
    ZI_MQTT_STATE(ZIGBEE_STATUS_STARTING, kConfiguredRouter)
  ZI_LABEL(ZIGBEE_LABEL_START_ROUTER_DEVICE)
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_ABORT)
    ZI_SEND(ZBS_AF_REGISTER_ALL)                  // Z_AF register for endpoint 01, profile 0x0104 Home Automation
    ZI_WAIT_RECV(1000, ZBR_AF_REGISTER)
    ZI_SEND(ZBS_STARTUPFROMAPP)                   // start router
    ZI_WAIT_RECV(2000, ZBR_STARTUPFROMAPP)        // wait for sync ack of command
    ZI_WAIT_UNTIL_FUNC(0xFFFF, AREQ_STARTUPFROMAPP, &ZNP_ReceiveStateChange)      // wait for async message that coordinator started
    ZI_SEND(ZBS_GETDEVICEINFO)                    // GetDeviceInfo
    ZI_WAIT_RECV_FUNC(2000, ZBR_GETDEVICEINFO, &ZNP_ReceiveDeviceInfo)
    ZI_GOTO(ZIGBEE_LABEL_READY)

  ZI_LABEL(ZIGBEE_LABEL_FACT_RESET_ROUTER)        // Factory reset for router
    ZI_MQTT_STATE(ZIGBEE_STATUS_RESET_CONF, kResetting)
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_ABORT)
    ZI_SEND(ZBS_FACTRES)                          // factory reset
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    ZI_SEND(ZBS_RESET)                            // reset device
    ZI_WAIT_RECV(5000, ZBR_RESET)
    ZI_SEND(ZBS_W_LOGTYP_ROUTER)                  // write Logical Type = router
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
  ZI_LABEL(ZIGBEE_LABEL_FACT_RESET_ROUTER_DEVICE_POST)
    ZI_SEND(ZBS_W_ALL_PAN)                        // write universal PAN ID = 0xFFFF
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    ZI_SEND(ZBS_W_ALL_CHANN)                      // write Allows all CHANNELS = 0x07FFF800, 11-26
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)

    // Now mark the device as ready, writing 0x55 in memory slot 0x0F00
    ZI_SEND(ZBS_WNV_INITZNPHC)                    // Init NV ZNP Has Configured
    ZI_WAIT_RECV_FUNC(1000, ZBR_WNV_INIT_OK, &ZNP_CheckNVWrite)
    ZI_SEND(ZBS_WNV_ZNPHC)                        // Write NV ZNP Has Configured
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)

    ZI_GOTO(ZIGBEE_LABEL_START_ROUTER_DEVICE)

    // ======================================================================
    // Start as Zigbee Device
    // ======================================================================
  ZI_LABEL(ZIGBEE_LABEL_INIT_DEVICE)              // Init as a router
    // Check the configuration as Router
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_FACT_RESET_DEVICE)
    ZI_SEND(ZBS_ZNPHC)                            // check value of ZNP Has Configured
    ZI_WAIT_RECV(2000, ZBR_ZNPHC)
    ZI_SEND(ZBS_LOGTYPE)                          // check the logical type
    ZI_WAIT_RECV(1000, ZBS_LOGTYPE_DEVICE)        // it should be coordinator

    ZI_MQTT_STATE(ZIGBEE_STATUS_STARTING, kConfiguredDevice)
    ZI_GOTO(ZIGBEE_LABEL_START_ROUTER_DEVICE)

  ZI_LABEL(ZIGBEE_LABEL_FACT_RESET_DEVICE)        // Factory reset for router
    ZI_MQTT_STATE(ZIGBEE_STATUS_RESET_CONF, kResetting)
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_ABORT)
    ZI_SEND(ZBS_FACTRES)                          // factory reset
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    ZI_SEND(ZBS_RESET)                            // reset device
    ZI_WAIT_RECV(5000, ZBR_RESET)
    ZI_SEND(ZBS_W_LOGTYP_DEVICE)                  // write Logical Type = router
    ZI_WAIT_RECV(1000, ZBR_WNV_OK)
    ZI_GOTO(ZIGBEE_LABEL_FACT_RESET_ROUTER_DEVICE_POST)

  // Error: version of Z-Stack is not supported
  ZI_LABEL(ZIGBEE_LABEL_UNSUPPORTED_VERSION)
    ZI_MQTT_STATE(ZIGBEE_STATUS_UNSUPPORTED_VERSION, kZNP123)
    ZI_GOTO(ZIGBEE_LABEL_ABORT)

  // Abort state machine, general error
  ZI_LABEL(ZIGBEE_LABEL_ABORT)                    // Label 99: abort
    ZI_MQTT_STATE(ZIGBEE_STATUS_ABORT, kAbort)
    ZI_LOG(LOG_LEVEL_ERROR, kZigbeeAbort)
    ZI_STOP(ZIGBEE_LABEL_ABORT)
};

#endif // USE_ZIGBEE_ZNP

#ifdef USE_ZIGBEE_EZSP

// patterns for EZSP

// wait for RSTACK, meaning the device booted
ZBM(ZBR_RSTACK, Z_B0(EZSP_rstAck), Z_B1(EZSP_rstAck))        	  // FEFF - internal code for RSTACK

// call version() and ask for EZSP v8
ZBM(ZBS_VERSION, EZSP_version, 0x00, 0x08)        	  // 000008
ZBM(ZBR_VERSION, EZSP_version, 0x00, 0x08, 0x02)      // 00000802 - expect v8, proto v2

// general configuration
// inspired from bellows: https://github.com/zigpy/bellows/blob/dev/bellows/config/ezsp.py
ZBM(ZBS_SET_ADDR_TABLE,   EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_KEY_TABLE_SIZE, 0x02, 0x00)                  // 53001E0400
ZBM(ZBS_SET_MCAST_TABLE,  EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_MULTICAST_TABLE_SIZE, 0x10, 0x00)            // 5300061000
ZBM(ZBS_SET_STK_PROF,     EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_STACK_PROFILE, 0x02, 0x00)                   // 53000C0200
ZBM(ZBS_SET_SEC_LEVEL,    EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_SECURITY_LEVEL, 0x05, 0x00)                  // 53000D0500
ZBM(ZBS_SET_MAX_DEVICES,  EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_MAX_END_DEVICE_CHILDREN, 0x20, 0x00)         // 5300111800
ZBM(ZBS_SET_INDIRECT_TMO, EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_INDIRECT_TRANSMISSION_TIMEOUT, 0x00, 0x1E)   // 530012001E
ZBM(ZBS_SET_TC_CACHE,     EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_TRUST_CENTER_ADDRESS_CACHE_SIZE, 0x02, 0x00) // 5300190200
ZBM(ZBS_SET_ROUTE_TBL,    EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_SOURCE_ROUTE_TABLE_SIZE, 0x10, 0x00)         // 53001A1000
ZBM(ZBS_SET_KEY_TBL,      EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_KEY_TABLE_SIZE, 0x04, 0x00)                  // 53001E0400
ZBM(ZBS_SET_PANID_CNFLCT, EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_PAN_ID_CONFLICT_REPORT_THRESHOLD, 0x02, 0x00)// 5300220200
ZBM(ZBS_SET_ZDO_REQ,      EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_APPLICATION_ZDO_FLAGS, EMBER_APP_RECEIVES_SUPPORTED_ZDO_REQUESTS | EMBER_APP_HANDLES_UNSUPPORTED_ZDO_REQUESTS, 0x00)           // 53002A0300
ZBM(ZBS_SET_NETWORKS,     EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_SUPPORTED_NETWORKS, 0x01, 0x00)              // 53002D0100
ZBM(ZBS_SET_PACKET_BUF,   EZSP_setConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_PACKET_BUFFER_COUNT, 0xFF, 0x00)             // 530001FF00

ZBM(ZBR_SET_OK,  EZSP_setConfigurationValue, 0x00 /*high*/, 0x00 /*ok*/)   // 530000
// There is a bug in v6.7 where the response if 000000 instead of 530000
// If we detect the version to be v6.7, the first byte is changed to 00
ZBR(ZBR_SET_OK2, EZSP_setConfigurationValue, 0x00 /*high*/, 0x00 /*ok*/)

// Read some configuration values
// ZBM(ZBS_GET_APS_UNI,      EZSP_getConfigurationValue, 0x00 /*high*/, EZSP_CONFIG_APS_UNICAST_MESSAGE_COUNT)                   // 520003
// ZBM(ZBR_GET_OK,           EZSP_getConfigurationValue, 0x00 /*high*/, 0x00 /*ok*/)   // 5200 - followed by the value

// Add Endpoints
ZBM(ZBS_ADD_ENDPOINT1,    EZSP_addEndpoint, 0x00 /*high*/, 0x01 /*ep*/, Z_B0(Z_PROF_HA), Z_B1(Z_PROF_HA),
                          0x05, 0x00 /* AppDeviceId */, 0x00 /* AppDevVer */,
                          0x0F /* inputClusterCount */,                         // actually all clusters will be received
                          0X0F /* outputClusterCount */,                        // 02000104010500000F0F0000040005000600070008000A00020100030004020403040504060400050000040005000600070008000A0002010003000402040304050406040005
                          0x00,0x00,  0x04,0x00,  0x05,0x00,  0x06,0x00,      // 0x0000, 0x0004, 0x0005, 0x0006
                          0x07,0x00,  0x08,0x00,  0x0A,0x00,  0x02,0x01,      // 0x0007, 0x0008, 0x000A, 0X0102
                          0x00,0x03,  0x00,0x04,  0x02,0x04,  0x03,0x04,      // 0x0300, 0x0400, 0x0402, 0x0403
                          0x05,0x04,  0x06,0x04,  0x00,0x05,                  // 0x0405, 0x0406, 0x0500
                          0x00,0x00,  0x04,0x00,  0x05,0x00,  0x06,0x00,      // 0x0000, 0x0004, 0x0005, 0x0006
                          0x07,0x00,  0x08,0x00,  0x0A,0x00,  0x02,0x01,      // 0x0007, 0x0008, 0x000A, 0X0102
                          0x00,0x03,  0x00,0x04,  0x02,0x04,  0x03,0x04,      // 0x0300, 0x0400, 0x0402, 0x0403
                          0x05,0x04,  0x06,0x04,  0x00,0x05,                  // 0x0405, 0x0406, 0x0500
                          )
ZBM(ZBS_ADD_ENDPOINTB,    EZSP_addEndpoint, 0x00 /*high*/, 0x0B /*ep*/, Z_B0(Z_PROF_HA), Z_B1(Z_PROF_HA),
                          0x05, 0x00 /* AppDeviceId */, 0x00 /* AppDevVer */,
                          0x00 /* inputClusterCount */,                         // actually all clusters will be received
                          0X00 /* outputClusterCount */,                        // 02000B04010500000000
                          // 0x00,0x00,  0x04,0x00,  0x05,0x00,  0x06,0x00,      // 0x0000, 0x0004, 0x0005, 0x0006
                          // 0x07,0x00,  0x08,0x00,  0x0A,0x00,  0x02,0x01,      // 0x0007, 0x0008, 0x000A, 0X0102
                          // 0x00,0x03,  0x00,0x04,  0x02,0x04,  0x03,0x04,      // 0x0300, 0x0400, 0x0402, 0x0403
                          // 0x05,0x04,  0x06,0x04,                              // 0x0405, 0x0406, 0x0500
                          )
ZBM(ZBR_ADD_ENDPOINT,     EZSP_addEndpoint, 0x00 /*high*/, 0x00 /*ok*/)           // 020000

// set concentrator false
ZBM(ZBS_SET_CONCENTRATOR, EZSP_setConcentrator, 0x00 /*high*/, 0x00 /*false*/, 0xF9,0xFF /*HIGH_RAM_CONCENTRATOR*/,
                          0x58,0x02 /*minTime*/, 0x08,0x07 /*maxTime*/, 0x02 /*errThr*/, 0x05 /*failThr*/, 0x00 /*maxHops*/)  // 100000F9FF58020807020500
ZBM(ZBR_SET_CONCENTRATOR, EZSP_setConcentrator, 0x00 /*high*/, 0x00 /*ok*/)           // 100000

// setInitialSecurityState
#define EZ_SECURITY_MODE  EMBER_TRUST_CENTER_GLOBAL_LINK_KEY | EMBER_PRECONFIGURED_NETWORK_KEY_MODE | EMBER_HAVE_NETWORK_KEY | EMBER_HAVE_PRECONFIGURED_KEY
ZBR(ZBS_SET_SECURITY,     EZSP_setInitialSecurityState, 0x00 /*high*/,
                          Z_B0(EZ_SECURITY_MODE), Z_B1(EZ_SECURITY_MODE),
                          // preConfiguredKey
                          0x5A, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6C, 0x6C, 0x69, 0x61, 0x6E, 0x63, 0x65, 0x30, 0x39,   // well known key "ZigBeeAlliance09"
                          // networkKey
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* key_l */
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* key_h */                          0x00 /*sequence*/,
                          0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*trustcenter*/
                          )
ZBM(ZBR_SET_SECURITY,     EZSP_setInitialSecurityState, 0x00 /*high*/, 0x00 /*status*/)

// setIndividual policies
ZBM(ZBS_SET_POLICY_00,    EZSP_setPolicy, 0x00 /*high*/, EZSP_TRUST_CENTER_POLICY,
                          EZSP_DECISION_ALLOW_JOINS | EZSP_DECISION_ALLOW_UNSECURED_REJOINS)    // 55000003
ZBM(ZBS_SET_POLICY_02,    EZSP_setPolicy, 0x00 /*high*/, EZSP_UNICAST_REPLIES_POLICY,
                          EZSP_HOST_WILL_NOT_SUPPLY_REPLY)    // 55000220
ZBM(ZBS_SET_POLICY_03,    EZSP_setPolicy, 0x00 /*high*/, EZSP_POLL_HANDLER_POLICY,
                          EZSP_POLL_HANDLER_IGNORE)    // 55000330
ZBM(ZBS_SET_POLICY_04,    EZSP_setPolicy, 0x00 /*high*/, EZSP_MESSAGE_CONTENTS_IN_CALLBACK_POLICY,
                          EZSP_MESSAGE_TAG_ONLY_IN_CALLBACK)    // 55000440
ZBM(ZBS_SET_POLICY_05,    EZSP_setPolicy, 0x00 /*high*/, EZSP_TC_KEY_REQUEST_POLICY,
                          EZSP_ALLOW_TC_KEY_REQUESTS_AND_SEND_CURRENT_KEY)    // 55000551
ZBM(ZBS_SET_POLICY_06,    EZSP_setPolicy, 0x00 /*high*/, EZSP_APP_KEY_REQUEST_POLICY,
                          EZSP_DENY_APP_KEY_REQUESTS)    // 55000660
ZBM(ZBR_SET_POLICY_XX,    EZSP_setPolicy, 0x00 /*high*/, 0x00 /*status*/)

// networkInit - restart the network from previous settings
ZBM(ZBS_NETWORK_INIT,     EZSP_networkInit, 0x00 /*high*/, 0x00, 0x00)        // 17000000
ZBM(ZBR_NETWORK_INIT,     EZSP_networkInit, 0x00 /*high*/, 0x00 /*status*/)   // 170000

// formNetwork - i.e. start zigbee network as coordinator
ZBR(ZBS_FORM_NETWORK,     EZSP_formNetwork, 0x00 /*high*/,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ext_pan_id */
                          0x00, 0x00,   // pan_id
                          USE_ZIGBEE_TXRADIO_DBM /*radioTxPower*/,
                          USE_ZIGBEE_CHANNEL /*channel*/,
                          EMBER_USE_MAC_ASSOCIATION,
                          0xFF,0xFF, /*nwkManagerId, unused*/
                          0x00, /*nwkUpdateId, unused*/
                          0x00,0x00,0x00,0x00, /*NWK channel mask, unused*/
                          )  // 1E00...
ZBM(ZBR_FORM_NETWORK,     EZSP_formNetwork, 0x00 /*high*/, 0x00 /*status*/)   // 1E0000
ZBM(ZBR_NETWORK_UP,       EZSP_stackStatusHandler, 0x00 /*high*/, EMBER_NETWORK_UP)   // 190090

// leaveNetwork
ZBR(ZBS_LEAVE_NETWORK,    EZSP_leaveNetwork, 0x00 /*high*/)   // 2000
ZBM(ZBR_LEAVE_NETWORK,    EZSP_leaveNetwork, 0x00 /*high*/)   // 2000, we don't care whether it succeeeded or the network was not up

// read configuration details
ZBM(ZBS_GET_NETW_PARM,    EZSP_getNetworkParameters, 0x00 /*high*/)   // 2800
ZBM(ZBR_GET_NETW_PARM,    EZSP_getNetworkParameters, 0x00 /*high*/, 0x00 /*ok*/)   // 2800
ZBR(ZBR_CHECK_NETW_PARM,  EZSP_getNetworkParameters, 0x00 /*high*/,
                          0x00 /*status*/,
                          EMBER_COORDINATOR /*0x01*/,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ext_pan_id */
                          0x00, 0x00,     // pan_id
                          USE_ZIGBEE_TXRADIO_DBM /*radioTxPower*/,
                          USE_ZIGBEE_CHANNEL /*channel*/,
                          )   // 2800...

ZBM(ZBS_GET_EUI64,        EZSP_getEui64, 0x00 /*high*/)   // 2600
ZBM(ZBR_GET_EUI64,        EZSP_getEui64, 0x00 /*high*/)   // 2600
ZBM(ZBS_GET_NODEID,       EZSP_getNodeId, 0x00 /*high*/)   // 2700
ZBM(ZBR_GET_NODEID,       EZSP_getNodeId, 0x00 /*high*/)   // 2700

// auto subscribe to group 0 in slot 0
ZBM(ZBS_SET_MCAST_ENTRY,  EZSP_setMulticastTableEntry, 0x00 /*high*/,
                          0x00 /* slot */, 0x00,0x00 /* group */, 0x01 /* endpoint */, 0x00 /* network */)  // 64000000000100
ZBM(ZBR_SET_MCAST_ENTRY,  EZSP_setMulticastTableEntry, 0x00 /*high*/, 0x00 /* status */)

// check the network key
// getCurrentSecurityState
// TODO double check the security bitmask
ZBM(ZBS_GET_KEY_NWK,      EZSP_getKey, 0x00 /*high*/, EMBER_CURRENT_NETWORK_KEY)   // 6A0003
ZBM(ZBR_GET_KEY_NWK,      EZSP_getKey, 0x00 /*high*/, 0x00 /*status*/)   // 6A0000...

/*********************************************************************************************\
 * Update the relevant commands with Settings
\*********************************************************************************************/
//
uint64_t ezsp_key_low, ezsp_key_high;

void EZ_UpdateConfig(uint8_t zb_channel, uint16_t zb_pan_id, uint64_t zb_ext_panid, uint64_t zb_precfgkey_l, uint64_t zb_precfgkey_h, int8_t zb_txradio_dbm) {
  int8_t txradio = zb_txradio_dbm;
  // restrict txradio to acceptable range, and use default otherwise
  if (txradio < 0) { txradio = USE_ZIGBEE_TXRADIO_DBM; }
  if (txradio > 20) { txradio = USE_ZIGBEE_TXRADIO_DBM; }
  ezsp_key_low = zb_precfgkey_l;
  ezsp_key_high = zb_precfgkey_h;

  ZBW(ZBS_SET_SECURITY,     EZSP_setInitialSecurityState, 0x00 /*high*/,
                            Z_B0(EZ_SECURITY_MODE), Z_B1(EZ_SECURITY_MODE),
                            // preConfiguredKey
                            0x5A, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6C, 0x6C, 0x69, 0x61, 0x6E, 0x63, 0x65, 0x30, 0x39,   // well known key "ZigBeeAlliance09"
                            // networkKey
                            Z_B0(zb_precfgkey_l), Z_B1(zb_precfgkey_l), Z_B2(zb_precfgkey_l), Z_B3(zb_precfgkey_l),
                            Z_B4(zb_precfgkey_l), Z_B5(zb_precfgkey_l), Z_B6(zb_precfgkey_l), Z_B7(zb_precfgkey_l),
                            Z_B0(zb_precfgkey_h), Z_B1(zb_precfgkey_h), Z_B2(zb_precfgkey_h), Z_B3(zb_precfgkey_h),
                            Z_B4(zb_precfgkey_h), Z_B5(zb_precfgkey_h), Z_B6(zb_precfgkey_h), Z_B7(zb_precfgkey_h),
                            0x00 /*sequence*/,
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*trustcenter*/
                            )

  ZBW(ZBS_FORM_NETWORK,     EZSP_formNetwork, 0x00 /*high*/,
                            Z_B0(zb_ext_panid), Z_B1(zb_ext_panid), Z_B2(zb_ext_panid), Z_B3(zb_ext_panid),
                            Z_B4(zb_ext_panid), Z_B5(zb_ext_panid), Z_B6(zb_ext_panid), Z_B7(zb_ext_panid),
                            Z_B0(zb_pan_id), Z_B1(zb_pan_id),
                            (uint8_t)txradio /*radioTxPower*/,
                            zb_channel /*channel*/,
                            EMBER_USE_MAC_ASSOCIATION,
                            0xFF,0xFF, /*nwkManagerId, unused*/
                            0x00, /*nwkUpdateId, unused*/
                            0x00,0x00,0x00,0x00, /*NWK channel mask, unused*/
                            )  // 1E00...

  ZBW(ZBR_CHECK_NETW_PARM,  EZSP_getNetworkParameters, 0x00 /*high*/,
                            0x00 /*status*/,
                            EMBER_COORDINATOR /*0x01*/,
                            Z_B0(zb_ext_panid), Z_B1(zb_ext_panid), Z_B2(zb_ext_panid), Z_B3(zb_ext_panid),
                            Z_B4(zb_ext_panid), Z_B5(zb_ext_panid), Z_B6(zb_ext_panid), Z_B7(zb_ext_panid),
                            Z_B0(zb_pan_id), Z_B1(zb_pan_id),
                            (uint8_t)txradio /*radioTxPower*/,
                            zb_channel /*channel*/,
                            )   // 2800...
}

static const Zigbee_Instruction zb_prog[] PROGMEM = {
  ZI_LABEL(0)
    ZI_CALL(&EZ_Reset_Device, 0)         // immediately drive reset low
    ZI_LOG(LOG_LEVEL_INFO, kResettingDevice)     // Log Debug: resetting EZSP device
    // ZI_CALL(EZ_Set_ResetConfig, 0)           // for the firt pass, don't do a reset_config
  ZI_LABEL(ZIGBEE_LABEL_RESTART)
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_ABORT)
    ZI_ON_TIMEOUT_GOTO(ZIGBEE_LABEL_ABORT)
    ZI_ON_RECV_UNEXPECTED(&EZ_Recv_Default)
    ZI_WAIT(15500)                             // wait for 15 seconds for Tasmota to stabilize

    // Hardware reset
    ZI_LOG(LOG_LEVEL_INFO, kResettingDevice)     // Log Debug: resetting EZSP device
    ZI_CALL(&EZ_Reset_Device, 0)         // LOW = reset
    ZI_WAIT(100)                        // wait for .1 second
    ZI_CALL(&EZ_Reset_Device, 1)         // HIGH = release reset

    // wait for device to start
    ZI_WAIT_UNTIL(5000, ZBR_RSTACK)     // wait for RSTACK message

    // Init device and probe version
    ZI_SEND(ZBS_VERSION)                ZI_WAIT_RECV_FUNC(5000, ZBR_VERSION, &EZ_ReceiveCheckVersion)       // check EXT PAN ID

    // configure EFR32
    ZI_MQTT_STATE(ZIGBEE_STATUS_STARTING, kConfiguredCoord)
    ZI_SEND(ZBS_SET_ADDR_TABLE)         ZI_WAIT_RECV(2500, ZBR_SET_OK)      // Address table size
    ZI_SEND(ZBS_SET_MCAST_TABLE)        ZI_WAIT_RECV(2500, ZBR_SET_OK)
    ZI_SEND(ZBS_SET_STK_PROF)           ZI_WAIT_RECV(2500, ZBR_SET_OK)
    ZI_SEND(ZBS_SET_SEC_LEVEL)          ZI_WAIT_RECV(2500, ZBR_SET_OK)
    ZI_SEND(ZBS_SET_MAX_DEVICES)        ZI_WAIT_RECV(2500, ZBR_SET_OK)
    ZI_SEND(ZBS_SET_INDIRECT_TMO)       ZI_WAIT_RECV(2500, ZBR_SET_OK)
    ZI_SEND(ZBS_SET_TC_CACHE)           ZI_WAIT_RECV(2500, ZBR_SET_OK)
    ZI_SEND(ZBS_SET_ROUTE_TBL)          ZI_WAIT_RECV(2500, ZBR_SET_OK)
    ZI_SEND(ZBS_SET_KEY_TBL)            ZI_WAIT_RECV(2500, ZBR_SET_OK)
    ZI_SEND(ZBS_SET_PANID_CNFLCT)       ZI_WAIT_RECV(2500, ZBR_SET_OK)
    ZI_SEND(ZBS_SET_ZDO_REQ)            ZI_WAIT_RECV(2500, ZBR_SET_OK)
    ZI_SEND(ZBS_SET_NETWORKS)           ZI_WAIT_RECV(2500, ZBR_SET_OK)
    ZI_SEND(ZBS_SET_PACKET_BUF)         ZI_WAIT_RECV(2500, ZBR_SET_OK2)

    // read configuration
    // TODO - not sure it's useful
    //ZI_SEND(ZBS_GET_APS_UNI)            ZI_WAIT_RECV_FUNC(2500, ZBR_GET_OK, &EZ_ReadAPSUnicastMessage)

    // add endpoint 0x01 and 0x0B
    ZI_SEND(ZBS_ADD_ENDPOINT1)          ZI_WAIT_RECV(2500, ZBR_ADD_ENDPOINT)
    ZI_SEND(ZBS_ADD_ENDPOINTB)          ZI_WAIT_RECV(2500, ZBR_ADD_ENDPOINT)

    // set Concentrator
    ZI_SEND(ZBS_SET_CONCENTRATOR)       ZI_WAIT_RECV(2500, ZBR_SET_CONCENTRATOR)

    // setInitialSecurityState
    ZI_SEND(ZBS_SET_POLICY_00)          ZI_WAIT_RECV(2500, ZBR_SET_POLICY_XX)
    ZI_SEND(ZBS_SET_POLICY_02)          ZI_WAIT_RECV(2500, ZBR_SET_POLICY_XX)
    ZI_SEND(ZBS_SET_POLICY_03)          ZI_WAIT_RECV(2500, ZBR_SET_POLICY_XX)
    // ZI_SEND(ZBS_SET_POLICY_04)          ZI_WAIT_RECV(2500, ZBR_SET_POLICY_XX)
    ZI_SEND(ZBS_SET_POLICY_05)          ZI_WAIT_RECV(2500, ZBR_SET_POLICY_XX)
    ZI_SEND(ZBS_SET_POLICY_06)          ZI_WAIT_RECV(2500, ZBR_SET_POLICY_XX)

    // Decide whether we try 'networkInit()' to restore configuration, or create a new network
    ZI_CALL(&EZ_GotoIfResetConfig, ZIGBEE_LABEL_CONFIGURE_EZSP)    // goto ZIGBEE_LABEL_CONFIGURE_EZSP if reset_config is set

    // ZI_GOTO(ZIGBEE_LABEL_CONFIGURE_EZSP)

    // Try networkInit to restore settings, and check if network comes up
    ZI_ON_TIMEOUT_GOTO(ZIGBEE_LABEL_BAD_CONFIG)    //
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_BAD_CONFIG)
    ZI_SEND(ZBS_NETWORK_INIT)           ZI_WAIT_RECV(2500, ZBR_NETWORK_INIT)
    ZI_WAIT_RECV(1500, ZBR_NETWORK_UP)    // wait for network to start
    // check if configuration is ok
    ZI_SEND(ZBS_GET_KEY_NWK)            ZI_WAIT_RECV_FUNC(2500, ZBR_GET_KEY_NWK, &EZ_CheckKeyNWK)
    ZI_SEND(ZBS_GET_EUI64)              ZI_WAIT_RECV_FUNC(2500, ZBR_GET_EUI64, &EZ_GetEUI64)
    ZI_SEND(ZBS_GET_NETW_PARM)          ZI_WAIT_RECV_FUNC(2500, ZBR_CHECK_NETW_PARM, &EZ_NetworkParameters)

    // all ok, proceed to next step
    ZI_GOTO(ZIGBEE_LABEL_NETWORK_CONFIGURED)

  ZI_LABEL(ZIGBEE_LABEL_BAD_CONFIG)
    ZI_MQTT_STATE(ZIGBEE_STATUS_RESET_CONF, kResetting)
    ZI_CALL(EZ_Set_ResetConfig, 1)           // change mode to reset_config
    ZI_GOTO(ZIGBEE_LABEL_RESTART)       // restart state_machine

  ZI_LABEL(ZIGBEE_LABEL_CONFIGURE_EZSP)
    // Set back normal error handlers
    ZI_LOG(LOG_LEVEL_INFO, kReconfiguringDevice)     // Log Debug: reconfiguring EZSP device
    ZI_ON_TIMEOUT_GOTO(ZIGBEE_LABEL_ABORT)
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_ABORT)
    // set encryption keys
    ZI_SEND(ZBS_SET_SECURITY)           ZI_WAIT_RECV(2500, ZBR_SET_SECURITY)
    // formNetwork
    ZI_SEND(ZBS_FORM_NETWORK)           ZI_WAIT_RECV(2500, ZBR_FORM_NETWORK)
    ZI_WAIT_RECV(5000, ZBR_NETWORK_UP)    // wait for network to start

  ZI_LABEL(ZIGBEE_LABEL_NETWORK_CONFIGURED)
    // Set back normal error handlers
    ZI_ON_TIMEOUT_GOTO(ZIGBEE_LABEL_ABORT)
    ZI_ON_ERROR_GOTO(ZIGBEE_LABEL_ABORT)
    // Query device information
    ZI_SEND(ZBS_GET_EUI64)              ZI_WAIT_RECV_FUNC(2500, ZBR_GET_EUI64, &EZ_GetEUI64)
    ZI_SEND(ZBS_GET_NODEID)             ZI_WAIT_RECV_FUNC(2500, ZBR_GET_NODEID, &EZ_GetNodeId)
    // auto-register multicast group 0x0000
    ZI_LOG(LOG_LEVEL_INFO, kZigbeeGroup0)
    ZI_SEND(ZBS_SET_MCAST_ENTRY)        ZI_WAIT_RECV(2500, ZBR_SET_MCAST_ENTRY)

  // ZI_LABEL(ZIGBEE_LABEL_READY)
    ZI_MQTT_STATE(ZIGBEE_STATUS_OK, kStarted)
    ZI_LOG(LOG_LEVEL_INFO, kZigbeeStarted)
    ZI_CALL(&Z_State_Ready, 1)                    // Now accept incoming messages
    ZI_CALL(&Z_Prepare_EEPROM, 0)
    ZI_CALL(&Z_Load_Devices, 0)
    ZI_CALL(&Z_Load_Data_EEPROM, 0)
    ZI_CALL(&Z_Set_Save_Data_Timer_EEPROM, 0)
    ZI_CALL(&Z_Query_Bulbs, 0)

  ZI_LABEL(ZIGBEE_LABEL_MAIN_LOOP)
    ZI_WAIT_FOREVER()
    ZI_GOTO(ZIGBEE_LABEL_MAIN_LOOP)

  // Error: version of Z-Stack is not supported
  ZI_LABEL(ZIGBEE_LABEL_UNSUPPORTED_VERSION)
    ZI_MQTT_STATE(ZIGBEE_STATUS_UNSUPPORTED_VERSION, kEZ8)
    ZI_GOTO(ZIGBEE_LABEL_ABORT)

  // Abort state machine, general error
  ZI_LABEL(ZIGBEE_LABEL_ABORT)                    // Label 99: abort
    ZI_MQTT_STATE(ZIGBEE_STATUS_ABORT, kAbort)
    ZI_LOG(LOG_LEVEL_ERROR, kZigbeeAbort)
    ZI_STOP(ZIGBEE_LABEL_ABORT)
};

#endif // USE_ZIGBEE_EZSP

uint8_t ZigbeeGetInstructionSize(uint8_t instr) {   // in Zigbee_Instruction lines (words)
  if (instr >= ZGB_INSTR_12_BYTES) {
    return 3;
  } else if (instr >= ZGB_INSTR_8_BYTES) {
    return 2;
  } else {
    return 1;
  }
}

void ZigbeeGotoLabel(uint8_t label) {
  // look for the label scanning entire code
  uint8_t  cur_instr = 0;
  uint8_t  cur_d8 = 0;
  uint8_t  cur_instr_len = 1;       // size of current instruction in words

  for (uint32_t i = 0; i < nitems(zb_prog); i += cur_instr_len) {
    const Zigbee_Instruction *cur_instr_line = &zb_prog[i];
    cur_instr = pgm_read_byte(&cur_instr_line->i.i);
    cur_d8    = pgm_read_byte(&cur_instr_line->i.d8);
    //AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("ZGB GOTO: pc %d instr %d"), i, cur_instr);

    if (ZGB_INSTR_LABEL == cur_instr) {
      //AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ZIGBEE "found label %d at pc %d"), cur_d8, i);
      if (label == cur_d8) {
        // label found, goto to this pc
        zigbee.pc = i;
        zigbee.state_machine = true;
        zigbee.state_waiting = false;
        return;
      }
    }
    // get instruction length
    cur_instr_len = ZigbeeGetInstructionSize(cur_instr);
  }

  // no label found, abort
  AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Goto label not found, label=%d pc=%d"), label, zigbee.pc);
  if (ZIGBEE_LABEL_ABORT != label) {
    // if not already looking for ZIGBEE_LABEL_ABORT, goto ZIGBEE_LABEL_ABORT
    ZigbeeGotoLabel(ZIGBEE_LABEL_ABORT);
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Label Abort (%d) not present, aborting Zigbee"), ZIGBEE_LABEL_ABORT);
    zigbee.state_machine = false;
    zigbee.active = false;
  }
}

void ZigbeeStateMachine_Run(void) {
  uint8_t cur_instr = 0;
  uint8_t cur_d8 = 0;
  uint16_t cur_d16 = 0;
  const void*   cur_ptr1 = nullptr;
  const void*   cur_ptr2 = nullptr;
  uint32_t now = millis();

  if (zigbee.state_waiting) {     // state machine is waiting for external event or timeout
    // checking if timeout expired
    if ((zigbee.next_timeout) && (now > zigbee.next_timeout)) {    // if next_timeout == 0 then wait forever
      if (!zigbee.state_no_timeout) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "timeout, goto label %d"), zigbee.on_timeout_goto);
        ZigbeeGotoLabel(zigbee.on_timeout_goto);
      } else {
        zigbee.state_waiting = false;     // simply stop waiting
      }
    }
  }

  while ((zigbee.state_machine) && (!zigbee.state_waiting)) {
    // reinit receive filters and functions (they only work for a single instruction)
    zigbee.recv_filter = nullptr;
    zigbee.recv_func   = nullptr;
    zigbee.recv_until  = false;
    zigbee.state_no_timeout = false;   // reset the no_timeout for next instruction

    if (zigbee.pc > nitems(zb_prog)) {
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Invalid pc: %d, aborting"), zigbee.pc);
      zigbee.pc = -1;
    }
    if (zigbee.pc < 0) {
      zigbee.state_machine = false;
      return;
    }

    // load current instruction details
    const Zigbee_Instruction *cur_instr_line = &zb_prog[zigbee.pc];
    cur_instr = pgm_read_byte(&cur_instr_line->i.i);
    cur_d8    = pgm_read_byte(&cur_instr_line->i.d8);
    cur_d16   = pgm_read_word(&cur_instr_line->i.d16);
    if (cur_instr >= ZGB_INSTR_8_BYTES) {
      cur_instr_line++;
      cur_ptr1 = cur_instr_line->p;
    }
    if (cur_instr >= ZGB_INSTR_12_BYTES) {
      cur_instr_line++;
      cur_ptr2 = cur_instr_line->p;
    }

    zigbee.pc += ZigbeeGetInstructionSize(cur_instr);               // move pc to next instruction, before any goto

    switch (cur_instr) {
      case ZGB_INSTR_NOOP:
      case ZGB_INSTR_LABEL:   // do nothing
        break;
      case ZGB_INSTR_GOTO:
        ZigbeeGotoLabel(cur_d8);
        break;
      case ZGB_INSTR_ON_ERROR_GOTO:
        zigbee.on_error_goto = cur_d8;
        break;
      case ZGB_INSTR_ON_TIMEOUT_GOTO:
        zigbee.on_timeout_goto = cur_d8;
        break;
      case ZGB_INSTR_WAIT:
        zigbee.next_timeout = now + cur_d16;
        zigbee.state_waiting = true;
        zigbee.state_no_timeout = true;    // do not generate a timeout error when waiting is done
        break;
      case ZGB_INSTR_WAIT_FOREVER:
        zigbee.next_timeout = 0;
        zigbee.state_waiting = true;
        break;
      case ZGB_INSTR_STOP:
        zigbee.state_machine = false;
        if (cur_d8) {
          AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Stopping (%d)"), cur_d8);
        }
        break;
      case ZGB_INSTR_CALL:
        if (cur_ptr1) {
          uint32_t res;
          res = (*((ZB_Func)cur_ptr1))(cur_d8);
          // res = (*((ZB_Func)cur_ptr1))(cur_d8, cur_d16);
          if (res > 0) {
            ZigbeeGotoLabel(res);
            continue;     // avoid incrementing PC after goto
          } else if (res == 0) {
            // do nothing
          } else if (res == -1) {
            // do nothing
          } else {
            ZigbeeGotoLabel(zigbee.on_error_goto);
            continue;
          }
        }
        break;
      case ZGB_INSTR_LOG:
        AddLog_P(cur_d8, (char*) cur_ptr1);
        break;
      case ZGB_INSTR_MQTT_STATE:
        {
        const char *f_msg = (const char*) cur_ptr1;
        Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{\"Status\":%d,\"Message\":\"%s\"}}"),
                          cur_d8, f_msg);
      	MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
        }
        break;
      case ZGB_INSTR_SEND:
#ifdef USE_ZIGBEE_ZNP
        ZigbeeZNPSend((uint8_t*) cur_ptr1, cur_d8 /* len */);
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
        ZigbeeEZSPSendCmd((uint8_t*) cur_ptr1, cur_d8 /* len */);  // send cancel byte
#endif // USE_ZIGBEE_EZSP
        break;
      case ZGB_INSTR_WAIT_UNTIL:
        zigbee.recv_until = true;   // and reuse ZGB_INSTR_WAIT_RECV
      case ZGB_INSTR_WAIT_RECV:
        zigbee.recv_filter = (uint8_t *) cur_ptr1;
        zigbee.recv_filter_len = cur_d8; // len
        if (0xFFFF == cur_d16) {
          zigbee.next_timeout = 0;    // forever
        } else {
          zigbee.next_timeout = now + cur_d16;
        }
        zigbee.state_waiting = true;
        break;
      case ZGB_ON_RECV_UNEXPECTED:
        zigbee.recv_unexpected = (ZB_RecvMsgFunc) cur_ptr1;
        break;
      case ZGB_INSTR_WAIT_UNTIL_CALL:
        zigbee.recv_until = true;   // and reuse ZGB_INSTR_WAIT_RECV
      case ZGB_INSTR_WAIT_RECV_CALL:
        zigbee.recv_filter = (uint8_t *) cur_ptr1;
        zigbee.recv_filter_len = cur_d8; // len
        zigbee.recv_func   = (ZB_RecvMsgFunc)  cur_ptr2;
        if (0xFFFF == cur_d16) {
          zigbee.next_timeout = 0;    // forever
        } else {
          zigbee.next_timeout = now + cur_d16;
        }
        zigbee.state_waiting = true;
        break;
    }
  }
}

//
// Process a bytes buffer and call any callback that matches the received message
//
int32_t ZigbeeProcessInput(SBuffer &buf) {
  if (!zigbee.state_machine) { return -1; }     // if state machine is stopped, send 'ignore' message

  // apply the receive filter, acts as 'startsWith()'
  bool recv_filter_match = true;
  bool recv_prefix_match = false;      // do the first 2 bytes match the response
  if ((zigbee.recv_filter) && (zigbee.recv_filter_len > 0)) {
    if (zigbee.recv_filter_len >= 2) {
      recv_prefix_match = false;
      if ( (pgm_read_byte(&zigbee.recv_filter[0]) == buf.get8(0)) &&
           (pgm_read_byte(&zigbee.recv_filter[1]) == buf.get8(1)) ) {
        recv_prefix_match = true;
      }
    }

    for (uint32_t i = 0; i < zigbee.recv_filter_len; i++) {
      if (pgm_read_byte(&zigbee.recv_filter[i]) != buf.get8(i)) {
        recv_filter_match = false;
        break;
      }
    }
  }

  // if there is a recv_callback, call it now
  int32_t res = -1;         // default to ok
                            // res  =  0   - proceed to next state
                            // res  >  0   - proceed to the specified state
                            // res  = -1  - silently ignore the message
                            // res <= -2 - move to error state
  // pre-compute the suggested value
  if ((zigbee.recv_filter) && (zigbee.recv_filter_len > 0)) {
    if (!recv_prefix_match) {
      res = -1;    // ignore
    } else {  // recv_prefix_match
      if (recv_filter_match) {
        res = 0;     // ok
      } else {
        if (zigbee.recv_until) {
          res = -1;  // ignore until full match
        } else {
          res = -2;  // error, because message is expected but wrong value
        }
      }
    }
  } else {    // we don't have any filter, ignore message by default
    res = -1;
  }

  if (recv_prefix_match) {
    if (zigbee.recv_func) {
      res = (*zigbee.recv_func)(res, buf);
    }
  }
  if (-1 == res) {
    // if frame was ignored up to now
    if (zigbee.recv_unexpected) {
      res = (*zigbee.recv_unexpected)(res, buf);
    }
  }

  // change state accordingly
  if (0 == res) {
    // if ok, continue execution
    zigbee.state_waiting = false;
  } else if (res > 0) {
    ZigbeeGotoLabel(res);     // if >0 then go to specified label
  } else if (-1 == res) {
    // -1 means ignore message
    // just do nothing
  } else {
    // any other negative value means error
    ZigbeeGotoLabel(zigbee.on_error_goto);
  }
  return 0;  // Fix GCC 10.1 warning
}

//
// Condiditional GOTO depending on ZB3 or not
// Take the branch if ZB3
int32_t Z_GotoZB3(uint8_t value) {
  if (zigbee.zb3) {
    return value; // take the branch
  } else {
    return 0;     // continue
  }
}

#endif // USE_ZIGBEE
