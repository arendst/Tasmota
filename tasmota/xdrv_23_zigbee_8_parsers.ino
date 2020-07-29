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

/*********************************************************************************************\
 * Parsers for incoming ZNP messages
\*********************************************************************************************/

//
// Handle a "Receive Device Info" incoming message
//
int32_t Z_ReceiveDeviceInfo(int32_t res, class SBuffer &buf) {
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

int32_t Z_CheckNVWrite(int32_t res, class SBuffer &buf) {
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

int32_t Z_Reboot(int32_t res, class SBuffer &buf) {
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

int32_t Z_ReceiveCheckVersion(int32_t res, class SBuffer &buf) {
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
int32_t Z_ReceivePermitJoinStatus(int32_t res, const class SBuffer &buf) {
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

int32_t Z_ReceiveNodeDesc(int32_t res, const class SBuffer &buf) {
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
  Z_ShortAddress    srcAddr = buf.get16(2);
  uint8_t           status  = buf.get8(4);
  Z_ShortAddress    nwkAddr = buf.get16(5);
  uint8_t           activeEpCount = buf.get8(7);
  uint8_t*          activeEpList = (uint8_t*) buf.charptr(8);

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
int32_t Z_ReceiveIEEEAddr(int32_t res, const class SBuffer &buf) {
  uint8_t           status = buf.get8(2);
  Z_IEEEAddress     ieeeAddr = buf.get64(3);
  Z_ShortAddress    nwkAddr = buf.get16(11);
  // uint8_t           startIndex = buf.get8(13);   // not used
  // uint8_t           numAssocDev = buf.get8(14);

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
int32_t Z_DataConfirm(int32_t res, const class SBuffer &buf) {
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
int32_t Z_ReceiveStateChange(int32_t res, const class SBuffer &buf) {
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
  Z_ShortAddress    srcAddr = buf.get16(2);
  Z_ShortAddress    nwkAddr = buf.get16(4);
  Z_IEEEAddress     ieeeAddr = buf.get64(6);
  uint8_t           capabilities = buf.get8(14);

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
int32_t Z_ReceiveTCDevInd(int32_t res, const class SBuffer &buf) {
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
  Z_ShortAddress    nwkAddr = buf.get16(2);
  uint8_t           status = buf.get8(4);

  const char * friendlyName = zigbee_devices.getFriendlyName(nwkAddr);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_BIND "\":{\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""), nwkAddr);
  if (friendlyName) {
    ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), friendlyName);
  }
  ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_STATUS "\":%d"
                  ",\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\""
                  "}}"), status, getZigbeeStatusMessage(status).c_str());

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  XdrvRulesProcess();

  return -1;
}

//
// Handle Unbind Rsp incoming message
//
int32_t Z_UnbindRsp(int32_t res, const class SBuffer &buf) {
  Z_ShortAddress    nwkAddr = buf.get16(2);
  uint8_t           status = buf.get8(4);

  const char * friendlyName = zigbee_devices.getFriendlyName(nwkAddr);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_UNBIND "\":{\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""), nwkAddr);
  if (friendlyName) {
    ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), friendlyName);
  }
  ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\""
                  "}}"), status, getZigbeeStatusMessage(status).c_str());

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  XdrvRulesProcess();

  return -1;
}
//
// Handle MgMt Bind Rsp incoming message
//
int32_t Z_MgmtBindRsp(int32_t res, const class SBuffer &buf) {
  uint16_t    shortaddr   = buf.get16(2);
  uint8_t     status      = buf.get8(4);
  uint8_t     bind_total  = buf.get8(5);
  uint8_t     bind_start  = buf.get8(6);
  uint8_t     bind_len    = buf.get8(7);

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

  uint32_t idx = 8;
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
      //AddLog_P2(LOG_LEVEL_INFO, PSTR("Z_MgmtBindRsp unknwon address mode %d"), addrmode);
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
  uint8_t IEEEAddrReq[] = { Z_SREQ | Z_ZDO, ZDO_IEEE_ADDR_REQ, Z_B0(shortaddr), Z_B1(shortaddr), 0x00, 0x00 };

  ZigbeeZNPSend(IEEEAddrReq, sizeof(IEEEAddrReq));
}

//
// Send ACTIVE_EP_REQ to collect active endpoints for this address
//
void Z_SendActiveEpReq(uint16_t shortaddr) {
  uint8_t ActiveEpReq[] = { Z_SREQ | Z_ZDO, ZDO_ACTIVE_EP_REQ, Z_B0(shortaddr), Z_B1(shortaddr), Z_B0(shortaddr), Z_B1(shortaddr) };

  ZigbeeZNPSend(ActiveEpReq, sizeof(ActiveEpReq));
}

//
// Send AF Info Request
//
void Z_SendAFInfoRequest(uint16_t shortaddr) {
  uint8_t endpoint = zigbee_devices.findFirstEndpoint(shortaddr);
  if (0x00 == endpoint) { endpoint = 0x01; }    // if we don't know the endpoint, try 0x01
  uint8_t transacid = zigbee_devices.getNextSeqNumber(shortaddr);

  uint8_t AFInfoReq[] = { Z_SREQ | Z_AF, AF_DATA_REQUEST, Z_B0(shortaddr), Z_B1(shortaddr), endpoint,
                            0x01, 0x00, 0x00, transacid, 0x30, 0x1E, 3 + 2*sizeof(uint16_t),
                            0x00, transacid, ZCL_READ_ATTRIBUTES, 0x04, 0x00, 0x05, 0x00
                          };
  ZigbeeZNPSend(AFInfoReq, sizeof(AFInfoReq));
}


/*********************************************************************************************\
 * Callbacks
\*********************************************************************************************/


// Aqara Occupancy behavior: the Aqara device only sends Occupancy: true events every 60 seconds.
// Here we add a timer so if we don't receive a Occupancy event for 90 seconds, we send Occupancy:false
void Z_AqaraOccupancy(uint16_t shortaddr, uint16_t cluster, uint8_t endpoint, const JsonObject &json) {
  static const uint32_t OCCUPANCY_TIMEOUT = 90 * 1000;  // 90 s
  // Read OCCUPANCY value if any
  const JsonVariant &val_endpoint = getCaseInsensitive(json, PSTR(OCCUPANCY));
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

int32_t Z_ReceiveAfIncomingMessage(int32_t res, const class SBuffer &buf) {
  uint16_t        groupid = buf.get16(2);
  uint16_t        clusterid = buf.get16(4);
  uint16_t        srcaddr = buf.get16(6);
  uint8_t         srcendpoint = buf.get8(8);
  uint8_t         dstendpoint = buf.get8(9);
  uint8_t         wasbroadcast = buf.get8(10);
  uint8_t         linkquality = buf.get8(11);
  uint8_t         securityuse = buf.get8(12);
  uint32_t        timestamp = buf.get32(13);
  uint8_t         seqnumber = buf.get8(17);

  bool            defer_attributes = false;     // do we defer attributes reporting to coalesce

  ZCLFrame zcl_received = ZCLFrame::parseRawFrame(buf, 19, buf.get8(18), clusterid, groupid,
                              srcaddr,
                              srcendpoint, dstendpoint, wasbroadcast,
                              linkquality, securityuse, seqnumber,
                              timestamp);
  zcl_received.log();
  char shortaddr[8];
  snprintf_P(shortaddr, sizeof(shortaddr), PSTR("0x%04X"), srcaddr);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  
  if ( (!zcl_received.isClusterSpecificCommand()) && (ZCL_DEFAULT_RESPONSE == zcl_received.getCmdId())) {
      zcl_received.parseResponse();
  } else {  
    // Build the ZbReceive json
    if ( (!zcl_received.isClusterSpecificCommand()) && (ZCL_REPORT_ATTRIBUTES == zcl_received.getCmdId())) {
      zcl_received.parseRawAttributes(json);
      if (clusterid) { defer_attributes = true; }  // don't defer system Cluster=0 messages
    } else if ( (!zcl_received.isClusterSpecificCommand()) && (ZCL_READ_ATTRIBUTES_RESPONSE == zcl_received.getCmdId())) {
      zcl_received.parseReadAttributes(json);
      if (clusterid) { defer_attributes = true; }  // don't defer system Cluster=0 messages
    } else if (zcl_received.isClusterSpecificCommand()) {
      zcl_received.parseClusterSpecificCommand(json);
    }
    String msg("");
    msg.reserve(100);
    json.printTo(msg);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE D_JSON_ZIGBEEZCL_RAW_RECEIVED ": {\"0x%04X\":%s}"), srcaddr, msg.c_str());

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
    }
  }
  return -1;
}

// Structure for the Dispatcher callbacks table
typedef struct Z_Dispatcher {
  const uint8_t*  match;
  ZB_RecvMsgFunc  func;
} Z_Dispatcher;

// Ffilters based on ZNP frames
ZBM(AREQ_AF_DATA_CONFIRM, Z_AREQ | Z_AF, AF_DATA_CONFIRM)                   // 4480
ZBM(AREQ_AF_INCOMING_MESSAGE, Z_AREQ | Z_AF, AF_INCOMING_MSG)               // 4481
// ZBM(AREQ_STATE_CHANGE_IND, Z_AREQ | Z_ZDO, ZDO_STATE_CHANGE_IND)            // 45C0
ZBM(AREQ_END_DEVICE_ANNCE_IND, Z_AREQ | Z_ZDO, ZDO_END_DEVICE_ANNCE_IND)    // 45C1
ZBM(AREQ_END_DEVICE_TC_DEV_IND, Z_AREQ | Z_ZDO, ZDO_TC_DEV_IND)             // 45CA
ZBM(AREQ_PERMITJOIN_OPEN_XX, Z_AREQ | Z_ZDO, ZDO_PERMIT_JOIN_IND )          // 45CB
ZBM(AREQ_ZDO_ACTIVEEPRSP, Z_AREQ | Z_ZDO, ZDO_ACTIVE_EP_RSP)                // 4585
ZBM(AREQ_ZDO_SIMPLEDESCRSP, Z_AREQ | Z_ZDO, ZDO_SIMPLE_DESC_RSP)            // 4584
ZBM(AREQ_ZDO_IEEE_ADDR_RSP, Z_AREQ | Z_ZDO, ZDO_IEEE_ADDR_RSP)              // 4581
ZBM(AREQ_ZDO_BIND_RSP, Z_AREQ | Z_ZDO, ZDO_BIND_RSP)                        // 45A1
ZBM(AREQ_ZDO_UNBIND_RSP, Z_AREQ | Z_ZDO, ZDO_UNBIND_RSP)                    // 45A2
ZBM(AREQ_ZDO_MGMT_BIND_RSP, Z_AREQ | Z_ZDO, ZDO_MGMT_BIND_RSP)              // 45B3

// Dispatcher callbacks table
const Z_Dispatcher Z_DispatchTable[] PROGMEM = {
  { AREQ_AF_DATA_CONFIRM,         &Z_DataConfirm },
  { AREQ_AF_INCOMING_MESSAGE,     &Z_ReceiveAfIncomingMessage },
  // { AREQ_STATE_CHANGE_IND,        &Z_ReceiveStateChange },
  { AREQ_END_DEVICE_ANNCE_IND,    &Z_ReceiveEndDeviceAnnonce },
  { AREQ_END_DEVICE_TC_DEV_IND,   &Z_ReceiveTCDevInd },
  { AREQ_PERMITJOIN_OPEN_XX,      &Z_ReceivePermitJoinStatus },
  { AREQ_ZDO_NODEDESCRSP,         &Z_ReceiveNodeDesc },
  { AREQ_ZDO_ACTIVEEPRSP,         &Z_ReceiveActiveEp },
  { AREQ_ZDO_IEEE_ADDR_RSP,       &Z_ReceiveIEEEAddr },
  { AREQ_ZDO_BIND_RSP,            &Z_BindRsp },
  { AREQ_ZDO_UNBIND_RSP,          &Z_UnbindRsp },
  { AREQ_ZDO_MGMT_BIND_RSP,       &Z_MgmtBindRsp },
};

/*********************************************************************************************\
 * Default resolver
\*********************************************************************************************/

int32_t Z_Recv_Default(int32_t res, const class SBuffer &buf) {
  // Default message handler for new messages
  if (zigbee.init_phase) {
    // if still during initialization phase, ignore any unexpected message
  	return -1;	// ignore message
  } else {
    for (uint32_t i = 0; i < sizeof(Z_DispatchTable)/sizeof(Z_Dispatcher); i++) {
      if (Z_ReceiveMatchPrefix(buf, Z_DispatchTable[i].match)) {
        (*Z_DispatchTable[i].func)(res, buf);
      }
    }
    return -1;
  }
}

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

#endif // USE_ZIGBEE
