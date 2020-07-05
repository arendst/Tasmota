/*
  xdrv_23_zigbee.ino - zigbee support for Tasmota

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

#ifdef USE_ZIGBEE_EZSP
/*********************************************************************************************\
 * Parsers for incoming EZSP messages
\*********************************************************************************************/

// EZSP: received ASH "RSTACK" frame, indicating that the MCU finished boot
int32_t EZ_RSTACK(uint8_t reset_code) {
  const char *reason_str;

  switch (reset_code) {
    case 0x01: reason_str = PSTR("External"); break;
    case 0x02: reason_str = PSTR("Power-on"); break;
    case 0x03: reason_str = PSTR("Watchdog"); break;
    case 0x06: reason_str = PSTR("Assert"); break;
    case 0x09: reason_str = PSTR("Bootloader"); break;
    case 0x0B: reason_str = PSTR("Software"); break;
    case 0x00:
    default: reason_str = PSTR("Unknown"); break;
  }
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"Message\":\"EFR32 booted\",\"RestartReason\":\"%s\""
                  ",\"Code\":%d}}"),
                  ZIGBEE_STATUS_BOOT, reason_str, reset_code);

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
  XdrvRulesProcess();
}

// EZSP: received ASH "ERROR" frame, indicating that the MCU finished boot
int32_t EZ_ERROR(uint8_t error_code) {
  const char *reason_str;

  switch (error_code) {
    case 0x51: reason_str = PSTR("ACK timeout"); break;
    default: reason_str = PSTR("Unknown"); break;
  }
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"Message\":\"Failed state\",\"Error\":\"%s\""
                  ",\"Code\":%d}}"),
                  ZIGBEE_STATUS_ABORT, reason_str, error_code);

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
  XdrvRulesProcess();
}

int32_t EZ_ReadAPSUnicastMessage(int32_t res, class SBuffer &buf) {
  // Called when receiving a response from getConfigurationValue
  // Value is in bytes 2+3
  uint16_t value = buf.get16(2);
  return res;
}

/*********************************************************************************************\
 * Parsers for incoming EZSP messages
\*********************************************************************************************/

//
// Handle a "getEui64" incoming message
//
int32_t EZ_GetEUI64(int32_t res, class SBuffer &buf) {
  localIEEEAddr = buf.get64(2);
  return res;
}

//
// Handle a "getEui64" incoming message
//
int32_t EZ_GetNodeId(int32_t res, class SBuffer &buf) {
  localShortAddr = buf.get8(2);
  return res;
}

//
// Handle a "getNetworkParameters" incoming message
//
int32_t EZ_NetworkParameters(int32_t res, class SBuffer &buf) {
  uint8_t  node_type = buf.get8(3);
  // ext panid: 4->11
  // panid: 12->13
  // radioTxPower: 14
  // radioChannel: 15

  // Local short and long addresses are supposed to be already retrieved
  // localIEEEAddr = long_adr;
  // localShortAddr = short_adr;

  char hex[20];
  Uint64toHex(localIEEEAddr, hex, 64);
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"0x%s\",\"ShortAddr\":\"0x%04X\""
                  ",\"DeviceType\":%d}}"),
                  ZIGBEE_STATUS_EZ_INFO, hex, localShortAddr, node_type);

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
  XdrvRulesProcess();

  return res;
}

#endif // USE_ZIGBEE_EZSP

/*********************************************************************************************\
 * Parsers for incoming EZSP messages
\*********************************************************************************************/

//
// Handle a "getEui64" incoming message
//
int32_t Z_EZSPGetEUI64(int32_t res, class SBuffer &buf) {
  localIEEEAddr = buf.get64(2);
  return res;
}

//
// Handle a "getEui64" incoming message
//
int32_t Z_EZSPGetNodeId(int32_t res, class SBuffer &buf) {
  localShortAddr = buf.get8(2);
  return res;
}

//
// Handle a "getNetworkParameters" incoming message
//
int32_t Z_EZSPNetworkParameters(int32_t res, class SBuffer &buf) {
  uint8_t  node_type = buf.get8(3);
  // ext panid: 4->11
  // panid: 12->13
  // radioTxPower: 14
  // radioChannel: 15

  // Local short and long addresses are supposed to be already retrieved
  // localIEEEAddr = long_adr;
  // localShortAddr = short_adr;

  char hex[20];
  Uint64toHex(localIEEEAddr, hex, 64);
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"0x%s\",\"ShortAddr\":\"0x%04X\""
                  ",\"DeviceType\":%d}}"),
                  ZIGBEE_STATUS_EZ_INFO, hex, localShortAddr, node_type);

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
  XdrvRulesProcess();

  return res;
}

/*********************************************************************************************\
 * Parsers for incoming ZNP messages
\*********************************************************************************************/

//
// Handle a "Receive Device Info" incoming message
//
int32_t ZNP_ReceiveDeviceInfo(int32_t res, class SBuffer &buf) {
  // Ex= 6700.00.6263151D004B1200.0000.07.09.02.83869991
  // IEEE Adr (8 bytes) = 0x00124B001D156362
  // Short Addr (2 bytes) = 0x0000
  // Device Type (1 byte) = 0x07 (coord?)
  // Device State (1 byte) = 0x09 (coordinator started)
  // NumAssocDevices (1 byte) = 0x02
  // List of devices: 0x8683, 0x9199
  Z_IEEEAddress  long_adr = buf.get64(3);
  Z_ShortAddress short_adr = buf.get16(11);
  uint8_t device_type = buf.get8(13);
  uint8_t device_state = buf.get8(14);
  uint8_t device_associated = buf.get8(15);

  // keep track of the local IEEE address
  localIEEEAddr = long_adr;
  localShortAddr = short_adr;

  char hex[20];
  Uint64toHex(long_adr, hex, 64);
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"0x%s\",\"ShortAddr\":\"0x%04X\""
                  ",\"DeviceType\":%d,\"DeviceState\":%d"
                  ",\"NumAssocDevices\":%d"),
                  ZIGBEE_STATUS_CC_INFO, hex, short_adr, device_type, device_state,
                  device_associated);

  if (device_associated > 0) {    // If there are devices registered in CC2530, print the list
    uint idx = 16;
    ResponseAppend_P(PSTR(",\"AssocDevicesList\":["));
    for (uint32_t i = 0; i < device_associated; i++) {
      if (i > 0) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"0x%04X\""), buf.get16(idx));
      idx += 2;
    }
    ResponseAppend_P(PSTR("]"));
  }

  ResponseJsonEnd();      // append '}'
  ResponseJsonEnd();      // append '}'
  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
  XdrvRulesProcess();

  return res;
}

int32_t ZNP_CheckNVWrite(int32_t res, class SBuffer &buf) {
  // Check the status after NV Init "ZNP Has Configured"
  // Good response should be 610700 or 610709 (Success or Created)
  // We only filter the response on 6107 and check the code in this function
  uint8_t status = buf.get8(2);
  if ((0x00 == status) || (0x09 == status)) {
    return 0;   // Ok, continue
  } else {
    return -2;  // Error
  }
}

int32_t ZNP_Reboot(int32_t res, class SBuffer &buf) {
  // print information about the reboot of device
  // 4180.02.02.00.02.06.03
  //
  uint8_t reason = buf.get8(2);
  uint8_t transport_rev = buf.get8(3);
  uint8_t product_id = buf.get8(4);
  uint8_t major_rel = buf.get8(5);
  uint8_t minor_rel = buf.get8(6);
  uint8_t hw_rev = buf.get8(7);
  const char *reason_str;

  switch (reason) {
    case 0:  reason_str = PSTR("Power-up"); break;
    case 1:  reason_str = PSTR("External"); break;
    case 2:  reason_str = PSTR("Watchdog"); break;
    default: reason_str = PSTR("Unknown");  break;
  }

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"Message\":\"CC2530 booted\",\"RestartReason\":\"%s\""
                  ",\"MajorRel\":%d,\"MinorRel\":%d}}"),
                  ZIGBEE_STATUS_BOOT, reason_str,
                  major_rel, minor_rel);

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
  XdrvRulesProcess();

  if ((0x02 == major_rel) && (0x06 == minor_rel)) {
  	return 0;	  // version 2.6.x is ok
  } else {
    return ZIGBEE_LABEL_UNSUPPORTED_VERSION;  // abort
  }
}

#ifdef USE_ZIGBEE_ZNP
int32_t ZNP_ReceiveCheckVersion(int32_t res, class SBuffer &buf) {
  // check that the version is supported
  // typical version for ZNP 1.2
  // 61020200-02.06.03.D9143401.0200000000
  // TranportRev = 02
  // Product = 00
  // MajorRel = 2
  // MinorRel = 6
  // MaintRel = 3
  // Revision = 20190425 d (0x013414D9)
  uint8_t major_rel = buf.get8(4);
  uint8_t minor_rel = buf.get8(5);
  uint8_t maint_rel = buf.get8(6);
  uint32_t revision = buf.get32(7);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"MajorRel\":%d,\"MinorRel\":%d"
                  ",\"MaintRel\":%d,\"Revision\":%d}}"),
                  ZIGBEE_STATUS_CC_VERSION, major_rel, minor_rel,
                  maint_rel, revision);

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
  XdrvRulesProcess();

  if ((0x02 == major_rel) && (0x06 == minor_rel)) {
  	return 0;	  // version 2.6.x is ok
  } else {
    return ZIGBEE_LABEL_UNSUPPORTED_VERSION;  // abort
  }
}
#endif // USE_ZIGBEE_ZNP

#ifdef USE_ZIGBEE_EZSP
int32_t EZ_ReceiveCheckVersion(int32_t res, class SBuffer &buf) {
  uint8_t protocol_version = buf.get8(2);
  uint8_t stack_type = buf.get8(3);
  uint16_t stack_version = buf.get16(4);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"Version\":\"%d.%d.%d.%d\",\"Protocol\":%d"
                  ",\"Stack\":%d}}"),
                  ZIGBEE_STATUS_EZ_VERSION,
                  (stack_version & 0xF000) >> 12,
                  (stack_version & 0x0F00) >> 8,
                  (stack_version & 0x00F0) >> 4,
                  stack_version & 0x000F,
                  protocol_version,
                  stack_type
                  );

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
  XdrvRulesProcess();

  if (0x08 == protocol_version) {
  	return 0;	  // protocol v8 is ok
  } else {
    return ZIGBEE_LABEL_UNSUPPORTED_VERSION;  // abort
  }
}

static bool EZ_reset_config = false;

// Set or clear reset_config
int32_t EZ_Set_ResetConfig(uint8_t value) {
  EZ_reset_config = value ? true : false;
  return 0;
}
// checks if we need to reset the configuration of the device
// if reset_config == 0, continue
// if reset_config == 1, goto ZIGBEE_LABEL_CONFIGURE_EZSP
int32_t EZ_GotoIfResetConfig(uint8_t value) {
  if (EZ_reset_config) { return ZIGBEE_LABEL_CONFIGURE_EZSP; }
  else                 { return 0; }
}

#endif // USE_ZIGBEE_EZSP

// checks the device type (coordinator, router, end-device)
// If coordinator continue
// If router goto ZIGBEE_LABEL_START_ROUTER
// If device goto ZIGBEE_LABEL_START_DEVICE
int32_t Z_SwitchDeviceType(int32_t res, class SBuffer &buf) {
  switch (Settings.zb_pan_id) {
    case 0xFFFF:    return ZIGBEE_LABEL_INIT_ROUTER;
    case 0xFFFE:    return ZIGBEE_LABEL_INIT_DEVICE;
    default:        return 0;   // continue
  }
}

//
// Helper function, checks if the incoming buffer matches the 2-bytes prefix, i.e. message type in PMEM
//
bool Z_ReceiveMatchPrefix(const class SBuffer &buf, const uint8_t *match) {
  if ( (pgm_read_byte(&match[0]) == buf.get8(0)) &&
       (pgm_read_byte(&match[1]) == buf.get8(1)) ) {
    return true;
  } else {
    return false;
  }
}

//
// Handle Permit Join response
//
int32_t ZNP_ReceivePermitJoinStatus(int32_t res, const class SBuffer &buf) {
  // we received a PermitJoin status change
  uint8_t     duration = buf.get8(2);
  uint8_t     status_code;
  const char* message;

  if (0xFF == duration) {
    status_code = ZIGBEE_STATUS_PERMITJOIN_OPEN_XX;
    message = PSTR("Enable Pairing mode until next boot");
  } else if (duration > 0) {
    status_code = ZIGBEE_STATUS_PERMITJOIN_OPEN_60;
    message = PSTR("Enable Pairing mode for %d seconds");
  } else {
    status_code = ZIGBEE_STATUS_PERMITJOIN_CLOSE;
    message = PSTR("Disable Pairing mode");
  }
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"Message\":\""),
                  status_code);
  ResponseAppend_P(message, duration);
  ResponseAppend_P(PSTR("\"}}"));

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
  XdrvRulesProcess();
  return -1;
}

//
// ZNP only
//
int32_t ZNP_ReceiveNodeDesc(int32_t res, const class SBuffer &buf) {
  // Received ZDO_NODE_DESC_RSP
  Z_ShortAddress    srcAddr = buf.get16(2);
  uint8_t           status  = buf.get8(4);
  Z_ShortAddress    nwkAddr = buf.get16(5);
  uint8_t           logicalType = buf.get8(7);
  uint8_t           apsFlags = buf.get8(8);
  uint8_t           MACCapabilityFlags = buf.get8(9);
  uint16_t          manufacturerCapabilities = buf.get16(10);
  uint8_t           maxBufferSize = buf.get8(12);
  uint16_t          maxInTransferSize = buf.get16(13);
  uint16_t          serverMask = buf.get16(15);
  uint16_t          maxOutTransferSize = buf.get16(17);
  uint8_t           descriptorCapabilities = buf.get8(19);


  if (0 == status) {
    uint8_t           deviceType = logicalType & 0x7;   // 0=coordinator, 1=router, 2=end device
    const char *      deviceTypeStr;
    switch (deviceType) {
      case 0:  deviceTypeStr = PSTR("Coordinator"); break;
      case 1:  deviceTypeStr = PSTR("Router"); break;
      case 2:  deviceTypeStr = PSTR("Device"); break;
      default: deviceTypeStr = PSTR("Unknown");  break;
    }
    bool              complexDescriptorAvailable = (logicalType & 0x08) ? 1 : 0;

    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                    "\"Status\":%d,\"NodeType\":\"%s\",\"ComplexDesc\":%s}}"),
                    ZIGBEE_STATUS_NODE_DESC, deviceTypeStr,
                    complexDescriptorAvailable ? PSTR("true") : PSTR("false")
                    );

    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
    XdrvRulesProcess();
  }

  return -1;
}

//
// Porcess Receive Active Endpoint
//
int32_t Z_ReceiveActiveEp(int32_t res, const class SBuffer &buf) {
  // Received ZDO_ACTIVE_EP_RSP
#ifdef USE_ZIGBEE_ZNP
  // Z_ShortAddress    srcAddr = buf.get16(2);
  uint8_t           status  = buf.get8(4);
  Z_ShortAddress    nwkAddr = buf.get16(5);
  uint8_t           activeEpCount = buf.get8(7);
  uint8_t*          activeEpList = (uint8_t*) buf.charptr(8);
#endif
#ifdef USE_ZIGBEE_EZSP
  uint8_t           status  = buf.get8(0);
  Z_ShortAddress    nwkAddr = buf.get16(1);
  uint8_t           activeEpCount = buf.get8(3);
  uint8_t*          activeEpList = (uint8_t*) buf.charptr(4);
#endif

  for (uint32_t i = 0; i < activeEpCount; i++) {
    zigbee_devices.addEndpoint(nwkAddr, activeEpList[i]);
  }

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"ActiveEndpoints\":["),
                  ZIGBEE_STATUS_ACTIVE_EP);
  for (uint32_t i = 0; i < activeEpCount; i++) {
    if (i > 0) { ResponseAppend_P(PSTR(",")); }
    ResponseAppend_P(PSTR("\"0x%02X\""), activeEpList[i]);
  }
  ResponseAppend_P(PSTR("]}}"));
  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  XdrvRulesProcess();

  Z_SendAFInfoRequest(nwkAddr);       // probe for ModelId and ManufId

  return -1;
}

//
// Handle IEEEAddr incoming message
//
// Same works for both ZNP and EZSP
int32_t Z_ReceiveIEEEAddr(int32_t res, const class SBuffer &buf) {
#ifdef USE_ZIGBEE_ZNP
  uint8_t           status = buf.get8(2);
  Z_IEEEAddress     ieeeAddr = buf.get64(3);
  Z_ShortAddress    nwkAddr = buf.get16(11);
  // uint8_t           startIndex = buf.get8(13);   // not used
  // uint8_t           numAssocDev = buf.get8(14);
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
  uint8_t           status = buf.get8(0);
  Z_IEEEAddress     ieeeAddr = buf.get64(1);
  Z_ShortAddress    nwkAddr = buf.get16(9);
  // uint8_t           numAssocDev = buf.get8(11);
  // uint8_t           startIndex = buf.get8(12);   // not used
#endif // USE_ZIGBEE_EZSP

  if (0 == status) {    // SUCCESS
    zigbee_devices.updateDevice(nwkAddr, ieeeAddr);
    char hex[20];
    Uint64toHex(ieeeAddr, hex, 64);
    // Ping response
    const char * friendlyName = zigbee_devices.getFriendlyName(nwkAddr);

    Response_P(PSTR("{\"" D_JSON_ZIGBEE_PING "\":{\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""
                    ",\"" D_JSON_ZIGBEE_IEEE "\":\"0x%s\""), nwkAddr, hex);
    if (friendlyName) {
      ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), friendlyName);
    }
    ResponseAppend_P(PSTR("\"}}"));

    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
    XdrvRulesProcess();
  }
  return -1;
}
//
// Report any AF_DATA_CONFIRM message
// Ex: {"ZbConfirm":{"Endpoint":1,"Status":0,"StatusMessage":"SUCCESS"}}
//
int32_t ZNP_DataConfirm(int32_t res, const class SBuffer &buf) {
  uint8_t           status = buf.get8(2);
  uint8_t           endpoint = buf.get8(3);
  //uint8_t           transId = buf.get8(4);    // unused

  if (status) {   // only report errors
    Response_P(PSTR("{\"" D_JSON_ZIGBEE_CONFIRM "\":{\"" D_CMND_ZIGBEE_ENDPOINT "\":%d"
                      ",\"" D_JSON_ZIGBEE_STATUS "\":%d"
                      ",\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\""
                      "}}"), endpoint, status, getZigbeeStatusMessage(status).c_str());
    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
    XdrvRulesProcess();
  }

  return -1;
}

//
// Handle State Change Indication incoming message
//
// Reference:
// 0x00: Initialized - not started automatically
// 0x01: Initialized - not connected to anything
// 0x02: Discovering PAN's to join
// 0x03: Joining a PAN
// 0x04: Rejoining a PAN, only for end devices
// 0x05: Joined but not yet authenticated by trust center
// 0x06: Started as device after authentication
// 0x07: Device joined, authenticated and is a router
// 0x08: Starting as ZigBee Coordinator
// 0x09: Started as ZigBee Coordinator
// 0x0A: Device has lost information about its parent
int32_t ZNP_ReceiveStateChange(int32_t res, const class SBuffer &buf) {
  uint8_t           state = buf.get8(2);
  const char *      msg = nullptr;

  switch (state) {
    case ZDO_DEV_NWK_DISC:                        // 0x02
      msg = PSTR("Scanning Zigbee network");
      break;
    case ZDO_DEV_NWK_JOINING:                     // 0x03
    case ZDO_DEV_NWK_REJOIN:                      // 0x04
      msg = PSTR("Joining a PAN");
      break;
    case ZDO_DEV_END_DEVICE_UNAUTH:               // 0x05
      msg = PSTR("Joined, not yet authenticated");
      break;
    case ZDO_DEV_END_DEVICE:                      // 0x06
      msg = PSTR("Started as device");
      break;
    case ZDO_DEV_ROUTER:                          // 0x07
      msg = PSTR("Started as router");
      break;
    case ZDO_DEV_ZB_COORD:                        // 0x09
      msg = PSTR("Started as coordinator");
      break;
    case ZDO_DEV_NWK_ORPHAN:                      // 0x0A
      msg = PSTR("Device has lost its parent");
      break;
  };

  if (msg) {
    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                    "\"Status\":%d,\"NewState\":%d,\"Message\":\"%s\"}}"),
                    ZIGBEE_STATUS_SCANNING, state, msg
                    );

    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
    XdrvRulesProcess();
  }

  if ((ZDO_DEV_END_DEVICE == state) || (ZDO_DEV_ROUTER == state) || (ZDO_DEV_ZB_COORD == state)) {
    return 0;         // device sucessfully started
  } else {
    return -1;        // ignore
  }
}

//
// Handle Receive End Device Announce incoming message
// This message is also received when a previously paired device is powered up
// Send back Active Ep Req message
//
int32_t Z_ReceiveEndDeviceAnnonce(int32_t res, const class SBuffer &buf) {
#ifdef USE_ZIGBEE_ZNP
  // Z_ShortAddress    srcAddr = buf.get16(2);
  Z_ShortAddress    nwkAddr = buf.get16(4);
  Z_IEEEAddress     ieeeAddr = buf.get64(6);
  uint8_t           capabilities = buf.get8(14);
#endif
#ifdef USE_ZIGBEE_EZSP
  // uint8_t           seq = buf.get8(0);
  Z_ShortAddress    nwkAddr = buf.get16(0);
  Z_IEEEAddress     ieeeAddr = buf.get64(2);
  uint8_t           capabilities = buf.get8(10);
#endif

  zigbee_devices.updateDevice(nwkAddr, ieeeAddr);

  char hex[20];
  Uint64toHex(ieeeAddr, hex, 64);
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"0x%s\",\"ShortAddr\":\"0x%04X\""
                  ",\"PowerSource\":%s,\"ReceiveWhenIdle\":%s,\"Security\":%s}}"),
                  ZIGBEE_STATUS_DEVICE_ANNOUNCE, hex, nwkAddr,
                  (capabilities & 0x04) ? PSTR("true") : PSTR("false"),
                  (capabilities & 0x08) ? PSTR("true") : PSTR("false"),
                  (capabilities & 0x40) ? PSTR("true") : PSTR("false")
                  );
  // query the state of the bulb (for Alexa)
  uint32_t wait_ms = 2000;    // wait for 2s
  Z_Query_Bulb(nwkAddr, wait_ms);

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  XdrvRulesProcess();
  Z_SendActiveEpReq(nwkAddr);
  return -1;
}

//
// Handle Receive TC Dev Ind incoming message
// 45CA
//
int32_t ZNP_ReceiveTCDevInd(int32_t res, const class SBuffer &buf) {
  Z_ShortAddress    srcAddr = buf.get16(2);
  Z_IEEEAddress     ieeeAddr = buf.get64(4);
  Z_ShortAddress    parentNw = buf.get16(12);

  zigbee_devices.updateDevice(srcAddr, ieeeAddr);

  char hex[20];
  Uint64toHex(ieeeAddr, hex, 64);
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"0x%s\",\"ShortAddr\":\"0x%04X\""
                  ",\"ParentNetwork\":\"0x%04X\"}}"),
                  ZIGBEE_STATUS_DEVICE_INDICATION, hex, srcAddr, parentNw
                  );

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  XdrvRulesProcess();
  return -1;
}

//
// Handle Bind Rsp incoming message
//
int32_t Z_BindRsp(int32_t res, const class SBuffer &buf) {
#ifdef USE_ZIGBEE_ZNP
  Z_ShortAddress    nwkAddr = buf.get16(2);
  uint8_t           status = buf.get8(4);
  String            msg = getZigbeeStatusMessage(status);
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
  uint8_t           status = buf.get8(0);
  Z_ShortAddress    nwkAddr = buf.get16(buf.len()-2);   // last 2 bytes
  String            msg = getZDPStatusMessage(status);
#endif // USE_ZIGBEE_EZSP

  const char * friendlyName = zigbee_devices.getFriendlyName(nwkAddr);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_BIND "\":{\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""), nwkAddr);
  if (friendlyName) {
    ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), friendlyName);
  }
  ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_STATUS "\":%d"
                  ",\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\""
                  "}}"), status, msg.c_str());

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  XdrvRulesProcess();

  return -1;
}

//
// Handle Unbind Rsp incoming message
//
int32_t Z_UnbindRsp(int32_t res, const class SBuffer &buf) {
#ifdef USE_ZIGBEE_ZNP
  Z_ShortAddress    nwkAddr = buf.get16(2);
  uint8_t           status = buf.get8(4);
  String            msg = getZigbeeStatusMessage(status);
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
  uint8_t           status = buf.get8(0);
  Z_ShortAddress    nwkAddr = buf.get16(buf.len()-2);   // last 2 bytes
  String            msg = getZDPStatusMessage(status);
#endif // USE_ZIGBEE_EZSP

  const char * friendlyName = zigbee_devices.getFriendlyName(nwkAddr);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_UNBIND "\":{\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""), nwkAddr);
  if (friendlyName) {
    ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), friendlyName);
  }
  ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_STATUS "\":%d"
                  ",\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\""
                  "}}"), status, msg.c_str());

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  XdrvRulesProcess();

  return -1;
}
//
// Handle MgMt Bind Rsp incoming message
//
int32_t Z_MgmtBindRsp(int32_t res, const class SBuffer &buf) {
#ifdef USE_ZIGBEE_ZNP
  uint16_t    shortaddr   = buf.get16(2);
  uint8_t     status      = buf.get8(4);
  uint8_t     bind_total  = buf.get8(5);
  uint8_t     bind_start  = buf.get8(6);
  uint8_t     bind_len    = buf.get8(7);
  const size_t prefix_len = 8;
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
  uint16_t    shortaddr   = buf.get16(buf.len()-2);
  uint8_t     status      = buf.get8(0);
  uint8_t     bind_total  = buf.get8(1);
  uint8_t     bind_start  = buf.get8(2);
  uint8_t     bind_len    = buf.get8(3);
  const size_t prefix_len = 4;
#endif // USE_ZIGBEE_EZSP

  const char * friendlyName = zigbee_devices.getFriendlyName(shortaddr);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_BIND_STATE "\":{\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""), shortaddr);
  if (friendlyName) {
    ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), friendlyName);
  }
  ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_STATUS "\":%d"
                        ",\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\""
                        ",\"BindingsTotal\":%d"
                        ",\"Bindings\":["
                        ), status, getZigbeeStatusMessage(status).c_str(), bind_total);

  uint32_t idx = prefix_len;
  for (uint32_t i = 0; i < bind_len; i++) {
    if (idx + 14 > buf.len()) { break; }   // overflow, frame size is between 14 and 21

    //uint64_t    srcaddr   = buf.get16(idx);     // unused
    uint8_t     srcep     = buf.get8(idx + 8);
    uint8_t     cluster   = buf.get16(idx + 9);
    uint8_t     addrmode  = buf.get8(idx + 11);
    uint16_t    group     = 0x0000;
    uint64_t    dstaddr   = 0;
    uint8_t     dstep     = 0x00;
    if (Z_Addr_Group == addrmode) {               // Group address mode
      group = buf.get16(idx + 12);
      idx += 14;
    } else if (Z_Addr_IEEEAddress == addrmode) {  // IEEE address mode
      dstaddr = buf.get64(idx + 12);
      dstep = buf.get8(idx + 20);
      idx += 21;
    } else {
      //AddLog_P2(LOG_LEVEL_INFO, PSTR("ZNP_MgmtBindRsp unknwon address mode %d"), addrmode);
      break;                                      // abort for any other value since we don't know the length of the field
    }

    if (i > 0) {
      ResponseAppend_P(PSTR(","));
    }
    ResponseAppend_P(PSTR("{\"Cluster\":\"0x%04X\",\"Endpoint\":%d,"), cluster, srcep);
    if (Z_Addr_Group == addrmode) {               // Group address mode
      ResponseAppend_P(PSTR("\"ToGroup\":%d}"), group);
    } else if (Z_Addr_IEEEAddress == addrmode) {  // IEEE address mode
      char hex[20];
      Uint64toHex(dstaddr, hex, 64);
      ResponseAppend_P(PSTR("\"ToDevice\":\"0x%s\",\"ToEndpoint\":%d}"), hex, dstep);
    }
  }

  ResponseAppend_P(PSTR("]}}"));

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_BIND_STATE));
  XdrvRulesProcess();

  return -1;
}

/*********************************************************************************************\
 * Send specific ZNP messages
\*********************************************************************************************/

//
// Send ZDO_IEEE_ADDR_REQ request to get IEEE long address
//
void Z_SendIEEEAddrReq(uint16_t shortaddr) {
#ifdef USE_ZIGBEE_ZNP
  uint8_t IEEEAddrReq[] = { Z_SREQ | Z_ZDO, ZDO_IEEE_ADDR_REQ, Z_B0(shortaddr), Z_B1(shortaddr), 0x00, 0x00 };

  ZigbeeZNPSend(IEEEAddrReq, sizeof(IEEEAddrReq));
#endif
#ifdef USE_ZIGBEE_EZSP
  uint8_t IEEEAddrReq[] = { Z_B0(shortaddr), Z_B1(shortaddr), 0x00, 0x00 };
  EZ_SendZDO(shortaddr, ZDO_IEEE_addr_req, IEEEAddrReq, sizeof(IEEEAddrReq));
#endif
}

//
// Send ACTIVE_EP_REQ to collect active endpoints for this address
//
void Z_SendActiveEpReq(uint16_t shortaddr) {
#ifdef USE_ZIGBEE_ZNP
  uint8_t ActiveEpReq[] = { Z_SREQ | Z_ZDO, ZDO_ACTIVE_EP_REQ, Z_B0(shortaddr), Z_B1(shortaddr), Z_B0(shortaddr), Z_B1(shortaddr) };
  ZigbeeZNPSend(ActiveEpReq, sizeof(ActiveEpReq));
#endif
#ifdef USE_ZIGBEE_EZSP
  uint8_t ActiveEpReq[] = { Z_B0(shortaddr), Z_B1(shortaddr) };
  EZ_SendZDO(shortaddr, ZDO_Active_EP_req, ActiveEpReq, sizeof(ActiveEpReq));
#endif
}

//
// Send AF Info Request
//
void Z_SendAFInfoRequest(uint16_t shortaddr) {
  uint8_t endpoint = zigbee_devices.findFirstEndpoint(shortaddr);
  if (0x00 == endpoint) { endpoint = 0x01; }    // if we don't know the endpoint, try 0x01
  uint8_t transacid = zigbee_devices.getNextSeqNumber(shortaddr);

  uint8_t InfoReq[] = { 0x04, 0x00, 0x05, 0x00 };

  ZigbeeZCLSend_Raw(shortaddr, 0x0000 /*group*/, 0x0000 /*cluster*/, endpoint, ZCL_READ_ATTRIBUTES,
                    false /*clusterSpecific*/, 0x0000 /*manuf*/,
                    InfoReq, sizeof(InfoReq), true /*needResponse*/, transacid);
}


//
// Handle trustCenterJoinHandler
// 2400
//
#ifdef USE_ZIGBEE_EZSP
int32_t EZ_ReceiveTCJoinHandler(int32_t res, const class SBuffer &buf) {
  uint16_t      srcAddr = buf.get16(2);
  uint64_t      ieeeAddr = buf.get64(4);
  uint8_t       status = buf.get8(12);
  uint8_t       decision = buf.get8(13);
  uint16_t      parentNw = buf.get16(14);

  if (EMBER_DEVICE_LEFT != status) {    // ignore message if the device is leaving
    zigbee_devices.updateDevice(srcAddr, ieeeAddr);

    char hex[20];
    Uint64toHex(ieeeAddr, hex, 64);
    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                    "\"Status\":%d,\"IEEEAddr\":\"0x%s\",\"ShortAddr\":\"0x%04X\""
                    ",\"ParentNetwork\":\"0x%04X\""
                    ",\"Status\":%d,\"Decision\":%d"
                    "}}"),
                    ZIGBEE_STATUS_DEVICE_INDICATION, hex, srcAddr, parentNw,
                    status, decision
                    );

    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
    XdrvRulesProcess();
  }
  return -1;
}
#endif // USE_ZIGBEE_EZSP

//
// Parse incoming ZCL message.
//
// This code is common to ZNP and EZSP
void Z_IncomingMessage(ZCLFrame &zcl_received) {
  uint16_t srcaddr = zcl_received.getSrcAddr();
  uint16_t groupid = zcl_received.getGroupAddr();
  uint16_t clusterid = zcl_received.getClusterId();
  uint8_t  linkquality = zcl_received.getLinkQuality();
  uint8_t  srcendpoint = zcl_received.getSrcEndpoint();

  bool            defer_attributes = false;     // do we defer attributes reporting to coalesce

  // log the packet details
  zcl_received.log();

  zigbee_devices.setLQI(srcaddr, linkquality != 0xFF ? linkquality : 0xFE);       // EFR32 has a different scale for LQI

  char shortaddr[8];
  snprintf_P(shortaddr, sizeof(shortaddr), PSTR("0x%04X"), srcaddr);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  if ( (!zcl_received.isClusterSpecificCommand()) && (ZCL_DEFAULT_RESPONSE == zcl_received.getCmdId())) {
      zcl_received.parseResponse();   // Zigbee general "Degault Response", publish ZbResponse message
  } else {
    // Build the ZbReceive json
    if ( (!zcl_received.isClusterSpecificCommand()) && (ZCL_REPORT_ATTRIBUTES == zcl_received.getCmdId())) {
      zcl_received.parseReportAttributes(json);    // Zigbee report attributes from sensors
      if (clusterid) { defer_attributes = true; }  // don't defer system Cluster=0 messages
    } else if ( (!zcl_received.isClusterSpecificCommand()) && (ZCL_READ_ATTRIBUTES_RESPONSE == zcl_received.getCmdId())) {
      zcl_received.parseReadAttributesResponse(json);
      if (clusterid) { defer_attributes = true; }  // don't defer system Cluster=0 messages
    } else if ( (!zcl_received.isClusterSpecificCommand()) && (ZCL_READ_ATTRIBUTES == zcl_received.getCmdId())) {
      zcl_received.parseReadAttributes(json);
      // never defer read_attributes, so the auto-responder can send response back on a per cluster basis
    } else if (zcl_received.isClusterSpecificCommand()) {
      zcl_received.parseClusterSpecificCommand(json);
    }

    {   // fence to force early de-allocation of msg
      String msg("");
      msg.reserve(100);
      json.printTo(msg);
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE D_JSON_ZIGBEEZCL_RAW_RECEIVED ": {\"0x%04X\":%s}"), srcaddr, msg.c_str());
    }

    zcl_received.postProcessAttributes(srcaddr, json);
    // Add Endpoint
    json[F(D_CMND_ZIGBEE_ENDPOINT)] = srcendpoint;
    // Add Group if non-zero
    if (groupid) {
      json[F(D_CMND_ZIGBEE_GROUP)] = groupid;
    }
    // Add linkquality
    json[F(D_CMND_ZIGBEE_LINKQUALITY)] = linkquality;

    // since we just receveived data from the device, it is reachable
    zigbee_devices.resetTimersForDevice(srcaddr, 0 /* groupaddr */, Z_CAT_REACHABILITY);    // remove any reachability timer already there
    zigbee_devices.setReachable(srcaddr, true);     // mark device as reachable

    // Post-provess for Aqara Presence Senson
    Z_AqaraOccupancy(srcaddr, clusterid, srcendpoint, json);

    if (defer_attributes) {
      // Prepare for publish
      if (zigbee_devices.jsonIsConflict(srcaddr, json)) {
        // there is conflicting values, force a publish of the previous message now and don't coalesce
        zigbee_devices.jsonPublishFlush(srcaddr);
      }
      zigbee_devices.jsonAppend(srcaddr, json);
      zigbee_devices.setTimer(srcaddr, 0 /* groupaddr */, USE_ZIGBEE_COALESCE_ATTR_TIMER, clusterid, srcendpoint, Z_CAT_READ_ATTR, 0, &Z_PublishAttributes);
    } else {
      // Publish immediately
      zigbee_devices.jsonPublishNow(srcaddr, json);

      // Add auto-responder here
      Z_AutoResponder(srcaddr, clusterid, srcendpoint, json[F("ReadNames")]);
    }
  }
}


#ifdef USE_ZIGBEE_EZSP

/*********************************************************************************************\
 * Send ZDO Message
\*********************************************************************************************/

void EZ_SendZDO(uint16_t shortaddr, uint16_t cmd, const unsigned char *payload, size_t payload_len) {
  SBuffer buf(payload_len + 22);
  uint8_t seq = zigbee_devices.getNextSeqNumber(0x0000);

  buf.add16(EZSP_sendUnicast);

  buf.add8(EMBER_OUTGOING_DIRECT);    // 00
  buf.add16(shortaddr);               // dest addr
  // ApsFrame
  buf.add16(0x0000);                  // ZOD profile
  buf.add16(cmd);                     // ZDO cmd in cluster
  buf.add8(0);                        // srcEp
  buf.add8(0);                        // dstEp
  buf.add16(EMBER_APS_OPTION_ENABLE_ROUTE_DISCOVERY | EMBER_APS_OPTION_RETRY);      // APS frame
  buf.add16(0x0000);                  // groupId
  buf.add8(seq);
  // end of ApsFrame
  buf.add8(0x01);                     // tag TODO
  buf.add8(payload_len + 1);        // insert seq number
  buf.add8(seq);
  buf.addBuffer(payload, payload_len);

  ZigbeeEZSPSendCmd(buf.buf(), buf.len(), true);
}

/*********************************************************************************************\
 * Send specific EZSP messages
\*********************************************************************************************/

int32_t EZ_IncomingMessage(int32_t res, const class SBuffer &buf) {
  uint8_t         msgtype = buf.get8(2);      // see EZSP_EmberIncomingMessageType
  bool            wasbroadcast = (msgtype >= EMBER_INCOMING_MULTICAST) && (msgtype <= EMBER_INCOMING_BROADCAST_LOOPBACK);
  uint16_t        profileid = buf.get16(3);   // HA = 0x0104, ZDO = 0x0000
  uint16_t        clusterid = buf.get16(5);
  uint8_t         srcendpoint = buf.get8(7);
  uint8_t         dstendpoint = buf.get8(8);
  uint16_t        apsoptions = buf.get16(9); // see EZSP_EmberApsOption, usually EMBER_APS_OPTION_ENABLE_ADDRESS_DISCOVERY
  bool            securityuse = (apsoptions & EMBER_APS_OPTION_ENCRYPTION) ? true : false;
  uint16_t        groupid = buf.get16(11);
  uint8_t         seqnumber = buf.get8(13);
  uint8_t         linkquality = buf.get8(14);
  // uint8_t         linkrsssi = buf.get8(15);     // probably not used as there is no equivalent in Z-Stack
  uint16_t        srcaddr = buf.get16(16);
  // uint8_t         bindingindex = buf.get8(18);      // not sure we need this one as a coordinator
  // uint8_t         addressindex = buf.get8(19);      // not sure how to handle this one
  // offset 20 is len, and buffer starts at offset 21


  if ((0x0000 == profileid) && (0x00 == srcendpoint))  {
    // ZDO request
    // Since ZDO messages start with a sequence number, we skip it
    // but we add the source address in the last 2 bytes
    SBuffer zdo_buf(buf.get8(20) - 1 + 2);
    zdo_buf.addBuffer(buf.buf(22), buf.get8(20) - 1);
    zdo_buf.add16(srcaddr);
    switch (clusterid) {
      case ZDO_Device_annce:
        return Z_ReceiveEndDeviceAnnonce(res, zdo_buf);
      case ZDO_Active_EP_rsp:
        return Z_ReceiveActiveEp(res, zdo_buf);
      case ZDO_IEEE_addr_rsp:
        return Z_ReceiveIEEEAddr(res, zdo_buf);
      case ZDO_Bind_rsp:
        return Z_BindRsp(res, zdo_buf);
      case ZDO_Unbind_rsp:
        return Z_UnbindRsp(res, zdo_buf);
      case ZDO_Mgmt_Bind_rsp:
        return Z_MgmtBindRsp(res, zdo_buf);
    }
  } else {
    bool            defer_attributes = false;     // do we defer attributes reporting to coalesce
    ZCLFrame zcl_received = ZCLFrame::parseRawFrame(buf, 21, buf.get8(20), clusterid, groupid,
                                srcaddr,
                                srcendpoint, dstendpoint, wasbroadcast,
                                linkquality, securityuse, seqnumber);
    //
    Z_IncomingMessage(zcl_received);
  }
  return -1;
}

//
// Callback for loading Zigbee configuration from Flash, called by the state machine
//
// value = 0 : drive reset pin and halt MCU
// value = 1 : release the reset pin, restart
int32_t EZ_Reset_Device(uint8_t value) {
  // we use Led4i to drive the reset pin. Since it is reverted we need to pass 1 to start reset, and 0 to release reset
  if (PinUsed(GPIO_LED1, ZIGBEE_EZSP_RESET_LED - 1)) {
    SetLedPowerIdx(ZIGBEE_EZSP_RESET_LED - 1, value ? 0 : 1);
  } else {
    // no GPIO so we use software Reset instead
    if (value) {  // send reset only when we are supposed to release reset
      uint8_t ezsp_reset[1] = { 0xC0 };       // EZSP ASH Reset
      ZigbeeEZSPSendRaw(ezsp_reset, sizeof(ezsp_reset), true);
    }
  }
  return 0;                              // continue
}

/*********************************************************************************************\
 * Default resolver
\*********************************************************************************************/

int32_t EZ_Recv_Default(int32_t res, const class SBuffer &buf) {
  // Default message handler for new messages
  if (zigbee.init_phase) {
    // if still during initialization phase, ignore any unexpected message
  	return -1;	// ignore message
  } else {
    uint16_t ezsp_command_index = buf.get16(0);

    switch (ezsp_command_index) {
      case EZSP_incomingMessageHandler:
        return EZ_IncomingMessage(res, buf);
        break;
      case EZSP_trustCenterJoinHandler:
        return EZ_ReceiveTCJoinHandler(res, buf);
        break;
    }
    return -1;
  }
}

#endif // USE_ZIGBEE_EZSP

/*********************************************************************************************\
 * Callbacks
\*********************************************************************************************/


// Aqara Occupancy behavior: the Aqara device only sends Occupancy: true events every 60 seconds.
// Here we add a timer so if we don't receive a Occupancy event for 90 seconds, we send Occupancy:false
void Z_AqaraOccupancy(uint16_t shortaddr, uint16_t cluster, uint8_t endpoint, const JsonObject &json) {
  static const uint32_t OCCUPANCY_TIMEOUT = 90 * 1000;  // 90 s
  // Read OCCUPANCY value if any
  const JsonVariant &val_endpoint = GetCaseInsensitive(json, PSTR(OCCUPANCY));
  if (nullptr != &val_endpoint) {
    uint32_t occupancy = strToUInt(val_endpoint);

    if (occupancy) {
      zigbee_devices.setTimer(shortaddr, 0 /* groupaddr */, OCCUPANCY_TIMEOUT, cluster, endpoint, Z_CAT_VIRTUAL_OCCUPANCY, 0, &Z_OccupancyCallback);
    } else {
      zigbee_devices.resetTimersForDevice(shortaddr, 0 /* groupaddr */, Z_CAT_VIRTUAL_OCCUPANCY);
    }
  }
}


// Publish the received values once they have been coalesced
int32_t Z_PublishAttributes(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  const JsonObject *json = zigbee_devices.jsonGet(shortaddr);
  if (json == nullptr) { return 0; }                 // don't crash if not found

  zigbee_devices.jsonPublishFlush(shortaddr);
  return 1;
}

/*********************************************************************************************\
 * Global dispatcher for incoming messages
\*********************************************************************************************/

#ifdef USE_ZIGBEE_ZNP

int32_t ZNP_ReceiveAfIncomingMessage(int32_t res, const class SBuffer &buf) {
  uint16_t        groupid = buf.get16(2);
  uint16_t        clusterid = buf.get16(4);
  uint16_t        srcaddr = buf.get16(6);
  uint8_t         srcendpoint = buf.get8(8);
  uint8_t         dstendpoint = buf.get8(9);
  uint8_t         wasbroadcast = buf.get8(10);
  uint8_t         linkquality = buf.get8(11);
  uint8_t         securityuse = buf.get8(12);
  // uint32_t        timestamp = buf.get32(13);
  uint8_t         seqnumber = buf.get8(17);

  bool            defer_attributes = false;     // do we defer attributes reporting to coalesce

  ZCLFrame zcl_received = ZCLFrame::parseRawFrame(buf, 19, buf.get8(18), clusterid, groupid,
                              srcaddr,
                              srcendpoint, dstendpoint, wasbroadcast,
                              linkquality, securityuse, seqnumber);
  //
  Z_IncomingMessage(zcl_received);

  return -1;
}

#endif // USE_ZIGBEE_ZNP


/*********************************************************************************************\
 * Global dispatcher for incoming messages
\*********************************************************************************************/

int32_t Z_ReceiveAfIncomingMessage(int32_t res, const class SBuffer &buf) {
  uint16_t        groupid = buf.get16(2);
  uint16_t        clusterid = buf.get16(4);
  uint16_t        srcaddr = buf.get16(6);
  uint8_t         srcendpoint = buf.get8(8);
  uint8_t         dstendpoint = buf.get8(9);
  uint8_t         wasbroadcast = buf.get8(10);
  uint8_t         linkquality = buf.get8(11);
  uint8_t         securityuse = buf.get8(12);
  // uint32_t        timestamp = buf.get32(13);
  uint8_t         seqnumber = buf.get8(17);

  bool            defer_attributes = false;     // do we defer attributes reporting to coalesce

  ZCLFrame zcl_received = ZCLFrame::parseRawFrame(buf, 19, buf.get8(18), clusterid, groupid,
                              srcaddr,
                              srcendpoint, dstendpoint, wasbroadcast,
                              linkquality, securityuse, seqnumber);
  //
  Z_IncomingMessage(zcl_received);

  return -1;
}

#ifdef USE_ZIGBEE_ZNP

// Structure for the Dispatcher callbacks table
typedef struct Z_Dispatcher {
  uint8_t match[2];
  ZB_RecvMsgFunc  func;
} Z_Dispatcher;

// Dispatcher callbacks table
const Z_Dispatcher Z_DispatchTable[] PROGMEM = {
  { { Z_AREQ | Z_AF, AF_DATA_CONFIRM },             &ZNP_DataConfirm },               // 4480
  { { Z_AREQ | Z_AF, AF_INCOMING_MSG },             &ZNP_ReceiveAfIncomingMessage },  // 4481
  // { { Z_AREQ | Z_ZDO, ZDO_STATE_CHANGE_IND },        &ZNP_ReceiveStateChange },    // 45C0
  { { Z_AREQ | Z_ZDO, ZDO_END_DEVICE_ANNCE_IND },   &Z_ReceiveEndDeviceAnnonce },     // 45C1
  { { Z_AREQ | Z_ZDO, ZDO_TC_DEV_IND },             &ZNP_ReceiveTCDevInd },           // 45CA
  { { Z_AREQ | Z_ZDO, ZDO_PERMIT_JOIN_IND },        &ZNP_ReceivePermitJoinStatus },   // 45CB
  { { Z_AREQ | Z_ZDO, ZDO_NODE_DESC_RSP },          &ZNP_ReceiveNodeDesc },           // 4582
  { { Z_AREQ | Z_ZDO, ZDO_ACTIVE_EP_RSP },          &Z_ReceiveActiveEp },             // 4585
  { { Z_AREQ | Z_ZDO, ZDO_IEEE_ADDR_RSP },          &Z_ReceiveIEEEAddr },             // 4581
  { { Z_AREQ | Z_ZDO, ZDO_BIND_RSP },               &Z_BindRsp },                   // 45A1
  { { Z_AREQ | Z_ZDO, ZDO_UNBIND_RSP },             &Z_UnbindRsp },                 // 45A2
  { { Z_AREQ | Z_ZDO, ZDO_MGMT_BIND_RSP },          &Z_MgmtBindRsp },               // 45B3
};

/*********************************************************************************************\
 * Default resolver
\*********************************************************************************************/

int32_t ZNP_Recv_Default(int32_t res, const class SBuffer &buf) {
  // Default message handler for new messages
  if (zigbee.init_phase) {
    // if still during initialization phase, ignore any unexpected message
  	return -1;	// ignore message
  } else {
    for (uint32_t i = 0; i < ARRAY_SIZE(Z_DispatchTable); i++) {
      if (Z_ReceiveMatchPrefix(buf, Z_DispatchTable[i].match)) {
        (*Z_DispatchTable[i].func)(res, buf);
      }
    }
    return -1;
  }
}

#endif // USE_ZIGBEE_ZNP

/*********************************************************************************************\
 * Functions called by State Machine
\*********************************************************************************************/

//
// Callback for loading Zigbee configuration from Flash, called by the state machine
//
int32_t Z_Load_Devices(uint8_t value) {
  // try to hidrate from known devices
  loadZigbeeDevices();
  return 0;                              // continue
}

//
// Query the state of a bulb (light) if its type allows it
//
void Z_Query_Bulb(uint16_t shortaddr, uint32_t &wait_ms) {
  const uint32_t inter_message_ms = 100;    // wait 100ms between messages

  if (0 <= zigbee_devices.getHueBulbtype(shortaddr)) {
    uint8_t endpoint = zigbee_devices.findFirstEndpoint(shortaddr);

    if (endpoint) {   // send only if we know the endpoint
      zigbee_devices.setTimer(shortaddr, 0 /* groupaddr */, wait_ms, 0x0006, endpoint, Z_CAT_NONE, 0 /* value */, &Z_ReadAttrCallback);
      wait_ms += inter_message_ms;
      zigbee_devices.setTimer(shortaddr, 0 /* groupaddr */, wait_ms, 0x0008, endpoint, Z_CAT_NONE, 0 /* value */, &Z_ReadAttrCallback);
      wait_ms += inter_message_ms;
      zigbee_devices.setTimer(shortaddr, 0 /* groupaddr */, wait_ms, 0x0300, endpoint, Z_CAT_NONE, 0 /* value */, &Z_ReadAttrCallback);
      wait_ms += inter_message_ms;
      zigbee_devices.setTimer(shortaddr, 0, wait_ms + Z_CAT_REACHABILITY_TIMEOUT, 0, endpoint, Z_CAT_REACHABILITY, 0 /* value */, &Z_Unreachable);
      wait_ms += 1000;              // wait 1 second between devices
    }
  }
}

//
// Send messages to query the state of each Hue emulated light
//
int32_t Z_Query_Bulbs(uint8_t value) {
  // Scan all devices and send deferred requests to know the state of bulbs
  uint32_t wait_ms = 1000;                  // start with 1.0 s delay
  for (uint32_t i = 0; i < zigbee_devices.devicesSize(); i++) {
    const Z_Device &device = zigbee_devices.devicesAt(i);
    Z_Query_Bulb(device.shortaddr, wait_ms);
  }
  return 0;                              // continue
}

//
// Zigbee initialization is complete, let the party begin
//
int32_t Z_State_Ready(uint8_t value) {
  zigbee.init_phase = false;             // initialization phase complete
  return 0;                              // continue
}

//
// Auto-responder for Read request from extenal devices.
//
// Mostly used for routers/end-devices
// json: holds the attributes in JSON format
void Z_AutoResponder(uint16_t srcaddr, uint16_t cluster, uint8_t endpoint, const JsonObject &json) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json_out = jsonBuffer.createObject();

  // responder
  switch (cluster) {
    case 0x0000:
      if (HasKeyCaseInsensitive(json, PSTR("ModelId")))           { json_out[F("ModelId")] = F(USE_ZIGBEE_MODELID); }
      if (HasKeyCaseInsensitive(json, PSTR("Manufacturer")))      { json_out[F("Manufacturer")] = F(USE_ZIGBEE_MANUFACTURER); }
      break;
#ifdef USE_LIGHT
    case 0x0006:
      if (HasKeyCaseInsensitive(json, PSTR("Power")))             { json_out[F("Power")] = Light.power ? 1 : 0; }
      break;
    case 0x0008:
      if (HasKeyCaseInsensitive(json, PSTR("Dimmer")))            { json_out[F("Dimmer")] = LightGetDimmer(0); }
      break;
    case 0x0300:
      {
      uint16_t hue;
      uint8_t  sat;
      float XY[2];
      LightGetHSB(&hue, &sat, nullptr);
      LightGetXY(&XY[0], &XY[1]);
      uint16_t uxy[2];
      for (uint32_t i = 0; i < ARRAY_SIZE(XY); i++) {
        uxy[i] = XY[i] * 65536.0f;
        uxy[i] = (uxy[i] > 0xFEFF) ? uxy[i] : 0xFEFF;
      }
      if (HasKeyCaseInsensitive(json, PSTR("Hue")))               { json_out[F("Hue")] = changeUIntScale(hue, 0, 360, 0, 254); }
      if (HasKeyCaseInsensitive(json, PSTR("Sat")))               { json_out[F("Sat")] = changeUIntScale(sat, 0, 255, 0, 254); }
      if (HasKeyCaseInsensitive(json, PSTR("CT")))                { json_out[F("CT")] = LightGetColorTemp(); }
      if (HasKeyCaseInsensitive(json, PSTR("X")))                 { json_out[F("X")] = uxy[0]; }
      if (HasKeyCaseInsensitive(json, PSTR("Y")))                 { json_out[F("Y")] = uxy[1]; }
      }
      break;
#endif
    case 0x000A:    // Time
      if (HasKeyCaseInsensitive(json, PSTR("Time")))              { json_out[F("Time")] = (Rtc.utc_time > (60 * 60 * 24 * 365 * 10)) ? Rtc.utc_time - 946684800 : Rtc.utc_time; }
      if (HasKeyCaseInsensitive(json, PSTR("TimeEpoch")))         { json_out[F("TimeEpoch")] = Rtc.utc_time; }
      if (HasKeyCaseInsensitive(json, PSTR("TimeStatus")))        { json_out[F("TimeStatus")] = (Rtc.utc_time > (60 * 60 * 24 * 365 * 10)) ? 0x02 : 0x00; }  // if time is beyond 2010 then we are synchronized
      if (HasKeyCaseInsensitive(json, PSTR("TimeZone")))          { json_out[F("TimeZone")] = Settings.toffset[0] * 60; }   // seconds
      break;
  }

  if (json_out.size() > 0) {
    // we have a non-empty output

    // log first
    String msg("");
    msg.reserve(100);
    json_out.printTo(msg);
    AddLog_P2(LOG_LEVEL_INFO, PSTR("ZIG: Auto-responder: ZbSend {\"Device\":\"0x%04X\""
                                          ",\"Cluster\":\"0x%04X\""
                                          ",\"Endpoint\":%d"
                                          ",\"Response\":%s}"
                                          ),
                                          srcaddr, cluster, endpoint,
                                          msg.c_str());

    // send
    const JsonVariant &json_out_v = json_out;
    ZbSendReportWrite(json_out_v, srcaddr, 0 /* group */,cluster, endpoint, 0 /* manuf */, ZCL_READ_ATTRIBUTES_RESPONSE);
  }
}

#endif // USE_ZIGBEE
