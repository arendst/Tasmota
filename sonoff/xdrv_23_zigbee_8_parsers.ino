/*
  xdrv_23_zigbee.ino - zigbee support for Sonoff-Tasmota

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
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATUS "\":{"
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
  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATUS));
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

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATUS "\":{"
                  "\"Status\":%d,\"MajorRel\":%d,\"MinorRel\":%d"
                  ",\"MaintRel\":%d,\"Revision\":%d}}"),
                  ZIGBEE_STATUS_CC_VERSION, major_rel, minor_rel,
                  maint_rel, revision);

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATUS));
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
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATUS "\":{"
                  "\"Status\":%d,\"Message\":\""),
                  status_code);
  ResponseAppend_P(message, duration);
  ResponseAppend_P(PSTR("\"}}"));

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATUS));
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
    uint8_t           deviceType = logicalType & 0x7;   // 0=coordinator, 1=router, 2=end device
    if (deviceType > 3) { deviceType = 3; }
    bool              complexDescriptorAvailable = (logicalType & 0x08) ? 1 : 0;

    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATUS "\":{"
                    "\"Status\":%d,\"NodeType\":\"%s\",\"ComplexDesc\":%s}}"),
                    ZIGBEE_STATUS_NODE_DESC, Z_DeviceType[deviceType],
                    complexDescriptorAvailable ? "true" : "false"
                    );

    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCLRECEIVED));
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
    Z_AddDeviceEndpoint(nwkAddr, activeEpList[i]);
  }

  for (uint32_t i = 0; i < activeEpCount; i++) {
    Z_SendSimpleDescReq(nwkAddr, activeEpList[i]);
  }

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATUS "\":{"
                  "\"Status\":%d,\"ActiveEndpoints\":["),
                  ZIGBEE_STATUS_ACTIVE_EP);
  for (uint32_t i = 0; i < activeEpCount; i++) {
    if (i > 0) { ResponseAppend_P(PSTR(",")); }
    ResponseAppend_P(PSTR("\"0x%02X\""), activeEpList[i]);
  }
  ResponseAppend_P(PSTR("]}}"));
  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCLRECEIVED));
  XdrvRulesProcess();
  return -1;
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
    for (uint32_t i = 0; i < numInCluster; i++) {
      Z_AddDeviceCluster(nwkAddr, endpoint, buf.get16(15 + i*2), false);
    }
    for (uint32_t i = 0; i < numOutCluster; i++) {
      Z_AddDeviceCluster(nwkAddr, endpoint, buf.get16(16 + numInCluster*2 + i*2), true);
    }
    // String dump = Z_DumpDevices();
    // Serial.printf(">>> Devices dump = %s\n", dump.c_str());

    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATUS "\":{"
                    "\"Status\":%d,\"Endpoint\":\"0x%02X\""
                    ",\"ProfileId\":\"0x%04X\",\"DeviceId\":\"0x%04X\",\"DeviceVerion\":%d"
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
    MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCLRECEIVED));
    XdrvRulesProcess();
  }
  return -1;
}

int32_t Z_ReceiveEndDeviceAnnonce(int32_t res, const class SBuffer &buf) {
  Z_ShortAddress    srcAddr = buf.get16(2);
  Z_ShortAddress    nwkAddr = buf.get16(4);
  Z_IEEEAddress     ieeeAddr = buf.get64(6);
  uint8_t           capabilities = buf.get8(14);

  Z_AddDeviceLongAddr(nwkAddr, ieeeAddr);
//   String dump = Z_DumpDevices();
// Serial.printf(">>> Devices dump = %s\n", dump.c_str());

  char hex[20];
  Uint64toHex(ieeeAddr, hex, 64);
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATUS "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"%s\",\"ShortAddr\":\"0x%04X\""
                  ",\"PowerSource\":%s,\"ReceiveWhenIdle\":%s,\"Security\":%s}}"),
                  ZIGBEE_STATUS_DEVICE_ANNOUNCE, hex, nwkAddr,
                  (capabilities & 0x04) ? "true" : "false",
                  (capabilities & 0x08) ? "true" : "false",
                  (capabilities & 0x40) ? "true" : "false"
                  );

  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCLRECEIVED));
  XdrvRulesProcess();
  Z_SendActiveEpReq(nwkAddr);
  return -1;
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

  ZCLFrame zcl_received = ZCLFrame::parseRawFrame(buf, 19, buf.get8(18), clusterid, groupid);

  zcl_received.publishMQTTReceived(groupid, clusterid, srcaddr,
                                  srcendpoint, dstendpoint, wasbroadcast,
                                  linkquality, securityuse, seqnumber,
                                  timestamp);

  char shortaddr[8];
  snprintf_P(shortaddr, sizeof(shortaddr), PSTR("0x%04X"), srcaddr);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json_root = jsonBuffer.createObject();
  JsonObject& json = json_root.createNestedObject(shortaddr);
  if ( (!zcl_received.isClusterSpecificCommand()) && (ZCL_REPORT_ATTRIBUTES == zcl_received.getCmdId())) {
   zcl_received.parseRawAttributes(json);
  } else if (zcl_received.isClusterSpecificCommand()) {
   zcl_received.parseClusterSpecificCommand(json);
  }
  zcl_received.postProcessAttributes(json);

  String msg("");
  msg.reserve(100);
  json_root.printTo(msg);

  Response_P(PSTR("%s"), msg.c_str());
  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCLRECEIVED));
  XdrvRulesProcess();
  return -1;
}

int32_t Z_Recv_Default(int32_t res, const class SBuffer &buf) {
  // Default message handler for new messages
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZIG: Z_Recv_Default"));
  if (zigbee.init_phase) {
    // if still during initialization phase, ignore any unexpected message
  	return -1;	// ignore message
  } else {
    if (Z_ReceiveMatchPrefix(buf, ZBR_AF_INCOMING_MESSAGE)) {
      return Z_ReceiveAfIncomingMessage(res, buf);
    } else if (Z_ReceiveMatchPrefix(buf, ZBR_END_DEVICE_ANNCE_IND)) {
      return Z_ReceiveEndDeviceAnnonce(res, buf);
    } else if (Z_ReceiveMatchPrefix(buf, ZBR_PERMITJOIN_AREQ_OPEN_XX)) {
      return Z_ReceivePermitJoinStatus(res, buf);
    } else if (Z_ReceiveMatchPrefix(buf, AREQ_ZDO_NODEDESCRSP)) {
      return Z_ReceiveNodeDesc(res, buf);
    } else if (Z_ReceiveMatchPrefix(buf, AREQ_ZDO_ACTIVEEPRSP)) {
      return Z_ReceiveActiveEp(res, buf);
    } else if (Z_ReceiveMatchPrefix(buf, AREQ_ZDO_SIMPLEDESCRSP)) {
      return Z_ReceiveSimpleDesc(res, buf);
    }
    return -1;
  }
}

int32_t Z_State_Ready(uint8_t value) {
  zigbee.init_phase = false;             // initialization phase complete
  return 0;                              // continue
}

#endif // USE_ZIGBEE
