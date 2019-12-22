/*
  xdrv_23_zigbee.ino - zigbee support for Tasmota

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

  char hex[20];
  Uint64toHex(long_adr, hex, 64);
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"%s\",\"ShortAddr\":\"0x%04X\""
                  ",\"DeviceType\":%d,\"DeviceState\":%d"
                  ",\"NumAssocDevices\":%d"),
                  ZIGBEE_STATUS_CC_INFO, hex, short_adr, device_type, device_state,
                  device_associated);

  if (device_associated > 0) {
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

const char     Z_RebootReason[] PROGMEM = "Power-up|External|Watchdog";

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
  char reason_str[12];

  if (reason > 3) { reason = 3; }
  GetTextIndexed(reason_str, sizeof(reason_str), reason, Z_RebootReason);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"Message\":\"%s\",\"RestartReason\":\"%s\""
                  ",\"MajorRel\":%d,\"MinorRel\":%d}}"),
                  ZIGBEE_STATUS_BOOT, "CC2530 booted", reason_str,
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

bool Z_ReceiveMatchPrefix(const class SBuffer &buf, const uint8_t *match) {
  if ( (pgm_read_byte(&match[0]) == buf.get8(0)) &&
       (pgm_read_byte(&match[1]) == buf.get8(1)) ) {
    return true;
  } else {
    return false;
  }
}

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

// Send ACTIVE_EP_REQ to collect active endpoints for this address
void Z_SendActiveEpReq(uint16_t shortaddr) {
  uint8_t ActiveEpReq[] = { Z_SREQ | Z_ZDO, ZDO_ACTIVE_EP_REQ,
              Z_B0(shortaddr), Z_B1(shortaddr), Z_B0(shortaddr), Z_B1(shortaddr) };

  uint8_t NodeDescReq[] = { Z_SREQ | Z_ZDO, ZDO_NODE_DESC_REQ,
              Z_B0(shortaddr), Z_B1(shortaddr), Z_B0(shortaddr), Z_B1(shortaddr) };

  ZigbeeZNPSend(ActiveEpReq, sizeof(ActiveEpReq));
  //ZigbeeZNPSend(NodeDescReq, sizeof(NodeDescReq));      Not sure this is useful
}

// Send ZDO_SIMPLE_DESC_REQ to get full list of supported Clusters for a specific endpoint
void Z_SendSimpleDescReq(uint16_t shortaddr, uint8_t endpoint) {
  uint8_t SimpleDescReq[] = { Z_SREQ | Z_ZDO, ZDO_SIMPLE_DESC_REQ,  // 2504
              Z_B0(shortaddr), Z_B1(shortaddr), Z_B0(shortaddr), Z_B1(shortaddr),
              endpoint };

  ZigbeeZNPSend(SimpleDescReq, sizeof(SimpleDescReq));
}

const char* Z_DeviceType[] = { "Coordinator", "Router", "End Device", "Unknown" };
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
    zigbee_devices.updateLastSeen(nwkAddr);

    uint8_t           deviceType = logicalType & 0x7;   // 0=coordinator, 1=router, 2=end device
    if (deviceType > 3) { deviceType = 3; }
    bool              complexDescriptorAvailable = (logicalType & 0x08) ? 1 : 0;

    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                    "\"Status\":%d,\"NodeType\":\"%s\",\"ComplexDesc\":%s}}"),
                    ZIGBEE_STATUS_NODE_DESC, Z_DeviceType[deviceType],
                    complexDescriptorAvailable ? "true" : "false"
                    );

    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
    XdrvRulesProcess();
  }

  return -1;
}

int32_t Z_ReceiveActiveEp(int32_t res, const class SBuffer &buf) {
  // Received ZDO_ACTIVE_EP_RSP
  Z_ShortAddress    srcAddr = buf.get16(2);
  uint8_t           status  = buf.get8(4);
  Z_ShortAddress    nwkAddr = buf.get16(5);
  uint8_t           activeEpCount = buf.get8(7);
  uint8_t*          activeEpList = (uint8_t*) buf.charptr(8);


  for (uint32_t i = 0; i < activeEpCount; i++) {
    zigbee_devices.addEndoint(nwkAddr, activeEpList[i]);
  }

  for (uint32_t i = 0; i < activeEpCount; i++) {
    Z_SendSimpleDescReq(nwkAddr, activeEpList[i]);
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
  return -1;
}

void Z_SendAFInfoRequest(uint16_t shortaddr, uint8_t endpoint, uint16_t clusterid, uint8_t transacid) {
  SBuffer buf(100);
  buf.add8(Z_SREQ | Z_AF);        // 24
  buf.add8(AF_DATA_REQUEST);      // 01
  buf.add16(shortaddr);
  buf.add8(endpoint);             // dest endpoint
  buf.add8(0x01);                 // source endpoint
  buf.add16(clusterid);
  buf.add8(transacid);
  buf.add8(0x30);                 // 30 options
  buf.add8(0x1E);                 // 1E radius

  buf.add8(3 + 2*sizeof(uint16_t)); // Len = 0x07
  buf.add8(0x00);                 // Frame Control Field
  buf.add8(transacid);            // Transaction Sequence Number
  buf.add8(ZCL_READ_ATTRIBUTES);  // 00 Command
  buf.add16(0x0004);              // 0400 ManufacturerName
  buf.add16(0x0005);              // 0500 ModelIdentifier

  ZigbeeZNPSend(buf.getBuffer(), buf.len());
}


int32_t Z_ReceiveSimpleDesc(int32_t res, const class SBuffer &buf) {
  // Received ZDO_SIMPLE_DESC_RSP
  Z_ShortAddress    srcAddr = buf.get16(2);
  uint8_t           status  = buf.get8(4);
  Z_ShortAddress    nwkAddr = buf.get16(5);
  uint8_t           lenDescriptor = buf.get8(7);
  uint8_t           endpoint = buf.get8(8);
  uint16_t          profileId = buf.get16(9);  // The profile Id for this endpoint.
  uint16_t          deviceId = buf.get16(11);   // The Device Description Id for this endpoint.
  uint8_t           deviceVersion = buf.get8(13); // 0 – Version 1.00
  uint8_t           numInCluster = buf.get8(14);
  uint8_t           numOutCluster = buf.get8(15 + numInCluster*2);

  if (0 == status) {
    zigbee_devices.addEndointProfile(nwkAddr, endpoint, profileId);
    for (uint32_t i = 0; i < numInCluster; i++) {
      zigbee_devices.addCluster(nwkAddr, endpoint, buf.get16(15 + i*2), false);
    }
    for (uint32_t i = 0; i < numOutCluster; i++) {
      zigbee_devices.addCluster(nwkAddr, endpoint, buf.get16(16 + numInCluster*2 + i*2), true);
    }

    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                    "\"Status\":%d,\"Endpoint\":\"0x%02X\""
                    ",\"ProfileId\":\"0x%04X\",\"DeviceId\":\"0x%04X\",\"DeviceVersion\":%d"
                    "\"InClusters\":["),
                    ZIGBEE_STATUS_SIMPLE_DESC, endpoint,
                    profileId, deviceId, deviceVersion);
    for (uint32_t i = 0; i < numInCluster; i++) {
      if (i > 0) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"0x%04X\""), buf.get16(15 + i*2));
    }
    ResponseAppend_P(PSTR("],\"OutClusters\":["));
    for (uint32_t i = 0; i < numOutCluster; i++) {
      if (i > 0) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"0x%04X\""), buf.get16(16 + numInCluster*2 + i*2));
    }
    ResponseAppend_P(PSTR("]}}"));
    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
    XdrvRulesProcess();

    uint8_t cluster = zigbee_devices.findClusterEndpointIn(nwkAddr, 0x0000);
    if (cluster) {
      Z_SendAFInfoRequest(nwkAddr, cluster, 0x0000, 0x01); // TODO, do we need tarnsacId counter?
    }
  }
  return -1;
}

int32_t Z_ReceiveEndDeviceAnnonce(int32_t res, const class SBuffer &buf) {
  Z_ShortAddress    srcAddr = buf.get16(2);
  Z_ShortAddress    nwkAddr = buf.get16(4);
  Z_IEEEAddress     ieeeAddr = buf.get64(6);
  uint8_t           capabilities = buf.get8(14);

  zigbee_devices.updateDevice(nwkAddr, ieeeAddr);

  char hex[20];
  Uint64toHex(ieeeAddr, hex, 64);
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"%s\",\"ShortAddr\":\"0x%04X\""
                  ",\"PowerSource\":%s,\"ReceiveWhenIdle\":%s,\"Security\":%s}}"),
                  ZIGBEE_STATUS_DEVICE_ANNOUNCE, hex, nwkAddr,
                  (capabilities & 0x04) ? "true" : "false",
                  (capabilities & 0x08) ? "true" : "false",
                  (capabilities & 0x40) ? "true" : "false"
                  );

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  XdrvRulesProcess();
  Z_SendActiveEpReq(nwkAddr);
  return -1;
}

// Aqara Occupancy behavior: the Aqara device only sends Occupancy: true events every 60 seconds.
// Here we add a timer so if we don't receive a Occupancy event for 90 seconds, we send Occupancy:false
const uint32_t OCCUPANCY_TIMEOUT = 90 * 1000;  // 90 s

void Z_AqaraOccupancy(uint16_t shortaddr, uint16_t cluster, uint16_t endpoint, const JsonObject *json) {
  // Read OCCUPANCY value if any
  const JsonVariant &val_endpoint = getCaseInsensitive(*json, PSTR(OCCUPANCY));
  if (nullptr != &val_endpoint) {
    uint32_t occupancy = strToUInt(val_endpoint);

    if (occupancy) {
      zigbee_devices.setTimer(shortaddr, OCCUPANCY_TIMEOUT, cluster, endpoint, 0, &Z_OccupancyCallback);
    }
  }
}


// Publish the received values once they have been coalesced
int32_t Z_PublishAttributes(uint16_t shortaddr, uint16_t cluster, uint16_t endpoint, uint32_t value) {
  const JsonObject *json = zigbee_devices.jsonGet(shortaddr);
  if (json == nullptr) { return 0; }                 // don't crash if not found

  // Post-provess for Aqara Presence Senson
  Z_AqaraOccupancy(shortaddr, cluster, endpoint, json);

  String msg = "";
  json->printTo(msg);
  zigbee_devices.jsonClear(shortaddr);
  Response_P(PSTR("{\"" D_CMND_ZIGBEE_RECEIVED "\":{\"0x%04X\":%s}}"), shortaddr, msg.c_str());
  MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR));
  XdrvRulesProcess();
}

int32_t Z_ReceiveAfIncomingMessage(int32_t res, const class SBuffer &buf) {
  uint16_t        groupid = buf.get16(2);
  uint16_t        clusterid = buf.get16(4);
  Z_ShortAddress  srcaddr = buf.get16(6);
  uint8_t         srcendpoint = buf.get8(8);
  uint8_t         dstendpoint = buf.get8(9);
  uint8_t         wasbroadcast = buf.get8(10);
  uint8_t         linkquality = buf.get8(11);
  uint8_t         securityuse = buf.get8(12);
  uint32_t        timestamp = buf.get32(13);
  uint8_t         seqnumber = buf.get8(17);

  bool            defer_attributes = false;     // do we defer attributes reporting to coalesce

  zigbee_devices.updateLastSeen(srcaddr);
  ZCLFrame zcl_received = ZCLFrame::parseRawFrame(buf, 19, buf.get8(18), clusterid, groupid,
                              srcaddr,
                              srcendpoint, dstendpoint, wasbroadcast,
                              linkquality, securityuse, seqnumber,
                              timestamp);
  zcl_received.log();
  char shortaddr[8];
  snprintf_P(shortaddr, sizeof(shortaddr), PSTR("0x%04X"), srcaddr);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json_root = jsonBuffer.createObject();
  JsonObject& json1 = json_root.createNestedObject(F(D_CMND_ZIGBEE_RECEIVED));
  JsonObject& json = json1.createNestedObject(shortaddr);

  if ( (!zcl_received.isClusterSpecificCommand()) && (ZCL_REPORT_ATTRIBUTES == zcl_received.getCmdId())) {
    zcl_received.parseRawAttributes(json);
    if (clusterid) { defer_attributes = true; }  // don't defer system Cluster=0 messages
  } else if ( (!zcl_received.isClusterSpecificCommand()) && (ZCL_READ_ATTRIBUTES_RESPONSE == zcl_received.getCmdId())) {
    zcl_received.parseReadAttributes(json);
  } else if (zcl_received.isClusterSpecificCommand()) {
    zcl_received.parseClusterSpecificCommand(json);
  }
  String msg("");
  msg.reserve(100);
  json_root.printTo(msg);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZigbeeZCLRawReceived: %s"), msg.c_str());

  zcl_received.postProcessAttributes(srcaddr, json);
  // Add linkquality
  json[F(D_CMND_ZIGBEE_LINKQUALITY)] = linkquality;

  if (defer_attributes) {
    // Prepare for publish
    zigbee_devices.jsonAppend(srcaddr, json);
    zigbee_devices.setTimer(srcaddr, USE_ZIGBEE_COALESCE_ATTR_TIMER, clusterid, srcendpoint, 0, &Z_PublishAttributes);
  } else {
    // Publish immediately
    msg = "";
    json_root.printTo(msg);
    Response_P(PSTR("%s"), msg.c_str());
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR));
    XdrvRulesProcess();
  }
  return -1;
}

typedef struct Z_Dispatcher {
  const uint8_t*  match;
  ZB_RecvMsgFunc  func;
} Z_Dispatcher;

// Filters for ZCL frames
ZBM(AREQ_AF_INCOMING_MESSAGE, Z_AREQ | Z_AF, AF_INCOMING_MSG)              // 4481
ZBM(AREQ_END_DEVICE_ANNCE_IND, Z_AREQ | Z_ZDO, ZDO_END_DEVICE_ANNCE_IND)   // 45C1
ZBM(AREQ_PERMITJOIN_OPEN_XX, Z_AREQ | Z_ZDO, ZDO_PERMIT_JOIN_IND )    // 45CB
ZBM(AREQ_ZDO_ACTIVEEPRSP, Z_AREQ | Z_ZDO, ZDO_ACTIVE_EP_RSP)    // 4585
ZBM(AREQ_ZDO_SIMPLEDESCRSP, Z_AREQ | Z_ZDO, ZDO_SIMPLE_DESC_RSP)    // 4584

const Z_Dispatcher Z_DispatchTable[] PROGMEM = {
  { AREQ_AF_INCOMING_MESSAGE,     &Z_ReceiveAfIncomingMessage },
  { AREQ_END_DEVICE_ANNCE_IND,    &Z_ReceiveEndDeviceAnnonce },
  { AREQ_PERMITJOIN_OPEN_XX,      &Z_ReceivePermitJoinStatus },
  { AREQ_ZDO_NODEDESCRSP,         &Z_ReceiveNodeDesc },
  { AREQ_ZDO_ACTIVEEPRSP,         &Z_ReceiveActiveEp },
  { AREQ_ZDO_SIMPLEDESCRSP,       &Z_ReceiveSimpleDesc },
};

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

int32_t Z_State_Ready(uint8_t value) {
  zigbee.init_phase = false;             // initialization phase complete
  return 0;                              // continue
}

#endif // USE_ZIGBEE
