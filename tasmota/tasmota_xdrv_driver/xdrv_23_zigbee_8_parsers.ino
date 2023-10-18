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

#ifdef USE_ZIGBEE_EZSP
void EZ_SendZDO(uint16_t shortaddr, uint16_t cmd, const unsigned char *payload, size_t payload_len, bool retry = true);

//
// Trying to get a uniform LQI measure, we are aligning with the definition of ZNP
// I.e. a linear projection from -87dBm to +10dB over 0..255
// for ZNP, lqi is linear from -87 to +10 dBm (https://sunmaysky.blogspot.com/2017/02/conversion-between-rssi-and-lqi-in-z.html)
uint8_t ZNP_RSSI2Lqi(int8_t rssi) {
  if (rssi < -87)  { rssi = -87; }
  if (rssi > 10)   { rssi = 10; }
  return changeUIntScale(rssi + 87, 0, 87+10, 0, 254);
}

/*********************************************************************************************\
 * Parsers for incoming EZSP messages
\*********************************************************************************************/

// EZSP: received ASH "RSTACK" frame, indicating that the MCU finished boot
void EZ_RSTACK(uint8_t reset_code) {
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
                  "\"Status\":%d,\"Message\":\"EFR32 EZSP booted\",\"RestartReason\":\"%s\""
                  ",\"Code\":%d}}"),
                  ZIGBEE_STATUS_BOOT, reason_str, reset_code);

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
}

// EZSP: received ASH "ERROR" frame, indicating that the MCU finished boot
void EZ_ERROR(uint8_t error_code) {
  const char *reason_str;

  switch (error_code) {
    case 0x51: reason_str = PSTR("ACK timeout"); break;
    default: reason_str = PSTR("Unknown"); break;
  }
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"Message\":\"Failed state\",\"Error\":\"%s\""
                  ",\"Code\":%d}}"),
                  ZIGBEE_STATUS_ABORT, reason_str, error_code);

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
}

int32_t EZ_ReadAPSUnicastMessage(int32_t res, SBuffer &buf) {
  // Called when receiving a response from getConfigurationValue
  // Value is in bytes 2+3
  // uint16_t value = buf.get16(2);
  return res;
}

/*********************************************************************************************\
 * Parsers for incoming EZSP messages
\*********************************************************************************************/

//
// Handle a "getEui64" incoming message
//
int32_t EZ_GetEUI64(int32_t res, SBuffer &buf) {
  localIEEEAddr = buf.get64(2);
  return res;
}

//
// Handle a "getEui64" incoming message
//
int32_t EZ_GetNodeId(int32_t res, SBuffer &buf) {
  localShortAddr = buf.get8(2);
  return res;
}

//
// Handle a "getNetworkParameters" incoming message
//
int32_t EZ_NetworkParameters(int32_t res, SBuffer &buf) {
  uint8_t  node_type = buf.get8(3);
  // ext panid: 4->11
  // panid: 12->13
  // radioTxPower: 14
  // radioChannel: 15

  // Local short and long addresses are supposed to be already retrieved
  // localIEEEAddr = long_adr;
  // localShortAddr = short_adr;

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"0x%_X\",\"ShortAddr\":\"0x%04X\""
                  ",\"DeviceType\":%d}}"),
                  ZIGBEE_STATUS_EZ_INFO, &localIEEEAddr, localShortAddr, node_type);

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));

  return res;
}

//
// Analyze response to "getKey" and check NWK key
//
int32_t EZ_CheckKeyNWK(int32_t res, SBuffer &buf) {
  // uint8_t  status = buf.get8(2);
  // uint16_t bitmask = buf.get16(3);
  uint8_t  key_type = buf.get8(5);
  uint64_t key_low  = buf.get64(6);
  uint64_t key_high = buf.get64(14);

  if ( (key_type == EMBER_CURRENT_NETWORK_KEY) &&
       (key_low  == ezsp_key_low) &&
       (key_high == ezsp_key_high) ) {
    return 0;     // proceed to next step
  } else {
    return -2;    // error state
  }
}

//
// Handle a "incomingRouteErrorHandler" incoming message
//
int32_t EZ_RouteError(int32_t res, const SBuffer &buf) {
  uint8_t  status = buf.get8(2);
  uint16_t shortaddr = buf.get16(3);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_ROUTE_ERROR "\":{"
                  "\"ShortAddr\":\"0x%04X\",\"" D_JSON_ZIGBEE_STATUS "\":%d,\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\"}}"),
                  shortaddr, status, getEmberStatus(status).c_str());

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));

  return -1;
}

//
// Handle EZSP Energy Scan result
//
int32_t EZSP_EnergyScanResult(int32_t res, const SBuffer &buf) {
  uint8_t  channel = buf.get8(2);
  int8_t   energy = buf.get8(3);

  if ((channel >= USE_ZIGBEE_CHANNEL_MIN) && (channel <= USE_ZIGBEE_CHANNEL_MAX)) {
    zigbee.energy[channel - USE_ZIGBEE_CHANNEL_MIN] = energy;
  }
  return -1;
}
//
// Handle EZSP Energy Scan complete
//
int32_t EZSP_EnergyScanComplete(int32_t res, const SBuffer &buf) {
  // uint8_t  channel = buf.get8(2);
  uint8_t   status = buf.get8(3);

  if (0 == status) {
    EnergyScanResults();
  }

  return -1;
}

//
// Dump energu scan results
//
void EnergyScanResults(void) {
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_SCAN "\":{"));
  for (uint32_t i = 0; i < USE_ZIGBEE_CHANNEL_COUNT; i++) {
    int8_t energy = zigbee.energy[i];

    if (i > 0) { ResponseAppend_P(PSTR(",")); }
    ResponseAppend_P(PSTR("\"%d\":%d"), i + USE_ZIGBEE_CHANNEL_MIN, energy);

    // display as log
    static const int32_t bar_min = -87;
    static const int32_t bar_max = 10;
    static const uint32_t bar_count = 60;
    char bar_str[bar_count + 1];
    uint32_t energy_unsigned = energy + 0x80;

    uint32_t bars = changeUIntScale(energy_unsigned, bar_min + 0x80, bar_max + 0x80, 0, bar_count);
    for  (uint32_t j = 0; j < bars; j++) { bar_str[j] = '#'; }
    bar_str[bars] = 0;

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Channel %2d: %s"), i + USE_ZIGBEE_CHANNEL_MIN, bar_str);
  }
  ResponseAppend_P(PSTR("}}"));
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
}

//
// Handle a "permitJoining" incoming message
//
int32_t EZ_PermitJoinRsp(int32_t res, const SBuffer &buf) {
  uint8_t  status = buf.get8(2);

  if (status) {     // only report if there is an error
    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{\"Status\":23,\"Message\":\"Pairing mode error 0x%02X\"}}"), status);
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
  }
  return -1;
}

//
// Received MessageSentHandler
//
// We normally ignore the message, but it's a way to sniff group ids for IKEA remote
// In case of a multicast message sent to 0xFFFD with non-null group id, we log the group id
int32_t EZ_MessageSent(int32_t res, const SBuffer &buf) {
  uint8_t  message_type = buf.get8(2);
  uint16_t dst_addr = buf.get16(3);
  uint16_t group_addr = buf.get16(13);

  if ((EMBER_OUTGOING_MULTICAST == message_type) && (0xFFFD == dst_addr)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "Sniffing group 0x%04X"), group_addr);
  }
  return -1;    // ignore
}

#endif // USE_ZIGBEE_EZSP

//
// Special case: EZSP does not send an event for PermitJoin end, so we generate a synthetic one
//
void Z_PermitJoinDisable(void) {
    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{\"Status\":20,\"Message\":\"Pairing mode disabled\"}}"));
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
}

/*********************************************************************************************\
 * Handle auto-mapping
\*********************************************************************************************/
// low-level sending of packet
void Z_Send_State_or_Map(uint16_t shortaddr, uint8_t index, uint16_t zdo_cmd) {
#ifdef USE_ZIGBEE_ZNP
  SBuffer buf(10);
  buf.add8(Z_SREQ | Z_ZDO);             // 25
  buf.add8(zdo_cmd);                    // 33
  buf.add16(shortaddr);                 // shortaddr
  buf.add8(index);                      // StartIndex = 0

  ZigbeeZNPSend(buf.getBuffer(), buf.len());
#endif // USE_ZIGBEE_ZNP


#ifdef USE_ZIGBEE_EZSP
  // ZDO message payload (see Zigbee spec 2.4.3.3.4)
  uint8_t buf[] = { index };           // index = 0

  EZ_SendZDO(shortaddr, zdo_cmd, buf, sizeof(buf), false);
#endif // USE_ZIGBEE_EZSP
}

// This callback is registered to send ZbMap(s) to each device one at a time
void Z_Map(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  if (BAD_SHORTADDR != shortaddr) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "sending `ZbMap 0x%04X`"), shortaddr);
#ifdef USE_ZIGBEE_ZNP
    Z_Send_State_or_Map(shortaddr, value, ZDO_MGMT_LQI_REQ);
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
    Z_Send_State_or_Map(shortaddr, value, ZDO_Mgmt_Lqi_req);
#endif // USE_ZIGBEE_EZSP
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "ZbMap done"));
    zigbee.mapping_in_progress = false;
    zigbee.mapping_ready = true;
  }
}
/*********************************************************************************************\
 * Parsers for incoming EZSP messages
\*********************************************************************************************/

//
// Handle a "getEui64" incoming message
//
int32_t Z_EZSPGetEUI64(int32_t res, SBuffer &buf) {
  localIEEEAddr = buf.get64(2);
  return res;
}

//
// Handle a "getEui64" incoming message
//
int32_t Z_EZSPGetNodeId(int32_t res, SBuffer &buf) {
  localShortAddr = buf.get8(2);
  return res;
}

//
// Handle a "getNetworkParameters" incoming message
//
int32_t Z_EZSPNetworkParameters(int32_t res, SBuffer &buf) {
  uint8_t  node_type = buf.get8(3);
  // ext panid: 4->11
  // panid: 12->13
  // radioTxPower: 14
  // radioChannel: 15

  // Local short and long addresses are supposed to be already retrieved
  // localIEEEAddr = long_adr;
  // localShortAddr = short_adr;

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"0x%_X\",\"ShortAddr\":\"0x%04X\""
                  ",\"DeviceType\":%d}}"),
                  ZIGBEE_STATUS_EZ_INFO, &localIEEEAddr, localShortAddr, node_type);

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));

  return res;
}

/*********************************************************************************************\
 * Parsers for incoming ZNP messages
\*********************************************************************************************/

//
// Handle a "Receive Device Info" incoming message
//
int32_t ZNP_ReceiveDeviceInfo(int32_t res, SBuffer &buf) {
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

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"0x%_X\",\"ShortAddr\":\"0x%04X\""
                  ",\"DeviceType\":%d,\"DeviceState\":%d"
                  ",\"NumAssocDevices\":%d"),
                  ZIGBEE_STATUS_CC_INFO, &long_adr, short_adr, device_type, device_state,
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

  ResponseJsonEndEnd();      // append '}}'
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));

  return res;
}

int32_t ZNP_CheckNVWrite(int32_t res, SBuffer &buf) {
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

int32_t ZNP_Reboot(int32_t res, SBuffer &buf) {
  // print information about the reboot of device
  // 4180.02.02.00.02.06.03
  //
  uint8_t reason = buf.get8(2);
  // uint8_t transport_rev = buf.get8(3);
  // uint8_t product_id = buf.get8(4);
  uint8_t major_rel = buf.get8(5);
  uint8_t minor_rel = buf.get8(6);
  // uint8_t hw_rev = buf.get8(7);
  const char *reason_str;

  switch (reason) {
    case 0:  reason_str = PSTR("Power-up"); break;
    case 1:  reason_str = PSTR("External"); break;
    case 2:  reason_str = PSTR("Watchdog"); break;
    default: reason_str = PSTR("Unknown");  break;
  }

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"Message\":\"CCxxxx ZNP booted\",\"RestartReason\":\"%s\""
                  ",\"MajorRel\":%d,\"MinorRel\":%d}}"),
                  ZIGBEE_STATUS_BOOT, reason_str,
                  major_rel, minor_rel);

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));

  if ((0x02 == major_rel) && ((0x06 == minor_rel) || (0x07 == minor_rel))) {
    if (0x07 == minor_rel) {
      zigbee.zb3 = true;        // we run Zigbee 3
      ZNP_UpdateZStack3();      // update configuration for ZStack 3
    }
  	return 0;	  // version 2.6.x and 2.7.x are ok
  } else {
    return ZIGBEE_LABEL_UNSUPPORTED_VERSION;  // abort
  }
}

#ifdef USE_ZIGBEE_ZNP
int32_t ZNP_ReceiveCheckVersion(int32_t res, SBuffer &buf) {
  // check that the version is supported
  // typical version for ZNP 1.2
  // 61020200-02.06.03.D9143401.0200000000
  // TranportRev = 02
  // Product = 00
  // MajorRel = 2
  // MinorRel = 6
  // MaintRel = 3
  // Revision = 20190425 d (0x013414D9)
  zigbee.major_rel = buf.get8(4);
  zigbee.minor_rel = buf.get8(5);
  zigbee.maint_rel = buf.get8(6);
  zigbee.revision = buf.get32(7);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"MajorRel\":%d,\"MinorRel\":%d"
                  ",\"MaintRel\":%d,\"Revision\":%d}}"),
                  ZIGBEE_STATUS_CC_VERSION,
                  zigbee.major_rel, zigbee.minor_rel,
                  zigbee.maint_rel, zigbee.revision);

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));

  if ((0x02 == zigbee.major_rel) && ((0x06 == zigbee.minor_rel) || (0x07 == zigbee.minor_rel))) {
  	return 0;	  // version 2.6.x and 2.7.x are ok
  } else {
    return ZIGBEE_LABEL_UNSUPPORTED_VERSION;  // abort
  }
}
#endif // USE_ZIGBEE_ZNP

#ifdef USE_ZIGBEE_EZSP
int32_t EZ_ReceiveCheckVersion(int32_t res, SBuffer &buf) {
  uint8_t protocol_version = buf.get8(2);
  uint8_t stack_type = buf.get8(3);
  zigbee.ezsp_version = buf.get16(4);

  zigbee.major_rel = (zigbee.ezsp_version & 0xF000) >> 12;
  zigbee.minor_rel = (zigbee.ezsp_version & 0x0F00) >> 8;
  zigbee.maint_rel = (zigbee.ezsp_version & 0x00F0) >> 4;
  zigbee.revision = zigbee.ezsp_version & 0x000F;

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"Version\":\"%d.%d.%d.%d\",\"Protocol\":%d"
                  ",\"Stack\":%d}}"),
                  ZIGBEE_STATUS_EZ_VERSION,
                  zigbee.major_rel, zigbee.minor_rel,
                  zigbee.maint_rel, zigbee.revision,
                  protocol_version,
                  stack_type
                  );

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));

  if (0x08 == protocol_version) {
    if ((zigbee.ezsp_version & 0xFF00) == 0x6700) {
      // If v6.7 there is a bug so we need to change the response
      ZBW(ZBR_SET_OK2, 0x00, 0x00 /*high*/, 0x00 /*ok*/)
    }
  	return 0;	  // protocol v8 is ok
  } else {
    return ZIGBEE_LABEL_UNSUPPORTED_VERSION;  // abort
  }
}

bool EZ_reset_config = false;

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
int32_t Z_SwitchDeviceType(int32_t res, SBuffer &buf) {
  switch (Settings->zb_pan_id) {
    case 0xFFFF:    return ZIGBEE_LABEL_INIT_ROUTER;
    case 0xFFFE:    return ZIGBEE_LABEL_INIT_DEVICE;
    default:        return 0;   // continue
  }
}

//
// Helper function, checks if the incoming buffer matches the 2-bytes prefix, i.e. message type in PMEM
//
bool Z_ReceiveMatchPrefix(const SBuffer &buf, const uint8_t *match) {
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
int32_t ZNP_ReceivePermitJoinStatus(int32_t res, const SBuffer &buf) {
  // we received a PermitJoin status change
  uint8_t     duration = buf.get8(2);
  uint8_t     status_code;
  const char* message;

  if (!zigbee.zb3 && (0xFF == duration)) {
    status_code = ZIGBEE_STATUS_PERMITJOIN_OPEN_XX;
    message = PSTR("Enable Pairing mode until next boot");
    zigbee.permit_end_time = -1;   // In ZNP mode, declare permitjoin open
  } else if (duration > 0) {
    status_code = ZIGBEE_STATUS_PERMITJOIN_OPEN_60;
    message = PSTR("Enable Pairing mode for %d seconds");
    zigbee.permit_end_time = -1;   // In ZNP mode, declare permitjoin open
  } else {
    status_code = ZIGBEE_STATUS_PERMITJOIN_CLOSE;
    message = PSTR("Disable Pairing mode");
    zigbee.permit_end_time = 0;   // In ZNP mode, declare permitjoin closed
  }
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"Message\":\""),
                  status_code);
  ResponseAppend_P(message, duration);
  ResponseAppend_P(PSTR("\"}}"));

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
  return -1;
}

//
// ZNP only
//
int32_t ZNP_ReceiveNodeDesc(int32_t res, const SBuffer &buf) {
  // Received ZDO_NODE_DESC_RSP
  // Z_ShortAddress    srcAddr = buf.get16(2);
  uint8_t           status  = buf.get8(4);
  // Z_ShortAddress    nwkAddr = buf.get16(5);
  uint8_t           logicalType = buf.get8(7);
  // uint8_t           apsFlags = buf.get8(8);
  // uint8_t           MACCapabilityFlags = buf.get8(9);
  // uint16_t          manufacturerCapabilities = buf.get16(10);
  // uint8_t           maxBufferSize = buf.get8(12);
  // uint16_t          maxInTransferSize = buf.get16(13);
  // uint16_t          serverMask = buf.get16(15);
  // uint16_t          maxOutTransferSize = buf.get16(17);
  // uint8_t           descriptorCapabilities = buf.get8(19);


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

    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  }

  return -1;
}

//
// Porcess Receive Active Endpoint
//
int32_t Z_ReceiveActiveEp(int32_t res, const SBuffer &buf) {
  // Received ZDO_ACTIVE_EP_RSP
#ifdef USE_ZIGBEE_ZNP
  // Z_ShortAddress    srcAddr = buf.get16(2);
  // uint8_t           status  = buf.get8(4);
  Z_ShortAddress    nwkAddr = buf.get16(5);
  uint8_t           activeEpCount = buf.get8(7);
  uint8_t*          activeEpList = (uint8_t*) buf.charptr(8);
#endif
#ifdef USE_ZIGBEE_EZSP
  // uint8_t           status  = buf.get8(0);
  Z_ShortAddress    nwkAddr = buf.get16(1);
  uint8_t           activeEpCount = buf.get8(3);
  uint8_t*          activeEpList = (uint8_t*) buf.charptr(4);
#endif

  // device is reachable
  zigbee_devices.deviceWasReached(nwkAddr);
  for (uint32_t i = 0; i < activeEpCount; i++) {
    uint8_t ep = activeEpList[i];
    zigbee_devices.getShortAddr(nwkAddr).addEndpoint(ep);
    if ((i < USE_ZIGBEE_AUTOBIND_MAX_ENDPOINTS) && (ep < USE_ZIGBEE_AUTOBIND_MAX_CLUSTER)) {
      zigbee_devices.queueTimer(nwkAddr, 0 /* groupaddr */, 1500, ep /* fake cluster as ep */, ep, Z_CAT_EP_DESC, 0 /* value */, &Z_SendSimpleDescReq);
    }
  }

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"ActiveEndpoints\":["),
                  ZIGBEE_STATUS_ACTIVE_EP);
  for (uint32_t i = 0; i < activeEpCount; i++) {
    if (i > 0) { ResponseAppend_P(PSTR(",")); }
    ResponseAppend_P(PSTR("\"0x%02X\""), activeEpList[i]);
  }
  ResponseAppend_P(PSTR("]}}"));
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));

  Z_SendDeviceInfoRequest(nwkAddr);       // probe for ModelId and ManufId

  return -1;
}

// list of clusters that need bindings
const uint8_t Z_bindings[] PROGMEM = {
  Cx0001, Cx0006, Cx0008, Cx0102, Cx0201, Cx0300,
  Cx0400, Cx0402, Cx0403, Cx0405, Cx0406,
  Cx0500, Cx0B04,
};

int32_t Z_ClusterToCxBinding(uint16_t cluster) {
  uint8_t cx = ClusterToCx(cluster);
  for (uint32_t i=0; i<nitems(Z_bindings); i++) {
    if (pgm_read_byte(&Z_bindings[i]) == cx) {
      return i;
    }
  }
  return -1;
}

void Z_AutoBindDefer(uint16_t shortaddr, uint8_t endpoint, const SBuffer &buf,
                    size_t in_index, size_t in_len, size_t out_index, size_t out_len) {
  // We use bitmaps to mark clusters that need binding and config attributes
  // All clusters in 'in' and 'out' are bounded
  // Only cluster in 'in' receive configure attribute requests
  uint32_t cluster_map = 0;     // max 32 clusters to bind
  uint32_t cluster_in_map = 0;  // map of clusters only in 'in' group, to be bounded

  // First enumerate all clusters to bind, from in or out clusters
  // scan in clusters
  for (uint32_t i=0; i<in_len; i++) {
    uint16_t cluster = buf.get16(in_index + i*2);
    uint32_t found_cx = Z_ClusterToCxBinding(cluster);    // convert to Cx of -1 if not found
    if (found_cx >= 0) {
      bitSet(cluster_map, found_cx);
      bitSet(cluster_in_map, found_cx);
    }
  }
  // scan out clusters
  for (uint32_t i=0; i<out_len; i++) {
    uint16_t cluster = buf.get16(out_index + i*2);
    uint32_t found_cx = Z_ClusterToCxBinding(cluster);    // convert to Cx of -1 if not found
    if (found_cx >= 0) {
      bitSet(cluster_map, found_cx);
    }
  }

  // if IAS device, request the device type
  if (bitRead(cluster_map, Z_ClusterToCxBinding(0x0500))) {
    // send a read command to cluster 0x0500, attribute 0x0001 (ZoneType) - to read the type of sensor
    zigbee_devices.queueTimer(shortaddr, 0 /* groupaddr */, 2000, 0x0500, endpoint, Z_CAT_READ_ATTRIBUTE, 0x0001, &Z_SendSingleAttributeRead);
    zigbee_devices.queueTimer(shortaddr, 0 /* groupaddr */, 2000, 0x0500, endpoint, Z_CAT_CIE_ATTRIBUTE, 0 /* value */, &Z_WriteCIEAddress);
    zigbee_devices.queueTimer(shortaddr, 0 /* groupaddr */, 2000, 0x0500, endpoint, Z_CAT_CIE_ENROLL, 1 /* zone */, &Z_SendCIEZoneEnrollResponse);
  }

  // if Plug, request the multipliers and divisors for Voltage, Current and Power
  if (bitRead(cluster_in_map, Z_ClusterToCxBinding(0x0B04))) {
    zigbee_devices.queueTimer(shortaddr, 0 /* groupaddr */, 2000, 0x0B04, endpoint, Z_CAT_READ_ATTRIBUTE, 0 /* ignore */, &Z_SendSinglePlugMulDivAttributesRead);
  }

  // enqueue bind requests
  for (uint32_t i=0; i<nitems(Z_bindings); i++) {
    if (bitRead(cluster_map, i)) {
      uint16_t cluster = CxToCluster(pgm_read_byte(&Z_bindings[i]));
      if ((cluster == 0x0001) && (!Z_BatteryReportingDeviceSpecific(shortaddr))) { continue; }
      zigbee_devices.queueTimer(shortaddr, 0 /* groupaddr */, 2000, cluster, endpoint, Z_CAT_BIND, 0 /* value */, &Z_AutoBind);
    }
  }

  // enqueue config attribute requests
  for (uint32_t i=0; i<nitems(Z_bindings); i++) {
    if (bitRead(cluster_in_map, i)) {
      uint16_t cluster = CxToCluster(pgm_read_byte(&Z_bindings[i]));
      // don't configure Battery reporting if `SetOption143 0` or if device is on the exception list
      if ((cluster == 0x0001) && (!Settings->flag5.zigbee_no_batt_autoprobe) || !Z_BatteryReportingDeviceSpecific(shortaddr)) { continue; }
      zigbee_devices.queueTimer(shortaddr, 0 /* groupaddr */, 2000, cluster, endpoint, Z_CAT_CONFIG_ATTR, 0 /* value */, &Z_AutoConfigReportingForCluster);
    }
  }
}

int32_t Z_ReceiveSimpleDesc(int32_t res, const SBuffer &buf) {
#ifdef USE_ZIGBEE_ZNP
  // Received ZDO_SIMPLE_DESC_RSP
  // Z_ShortAddress    srcAddr = buf.get16(2);
  uint8_t           status  = buf.get8(4);
  Z_ShortAddress    nwkAddr = buf.get16(5);
  // uint8_t           lenDescriptor = buf.get8(7);
  uint8_t           endpoint = buf.get8(8);
  uint16_t          profileId = buf.get16(9);  // The profile Id for this endpoint.
  uint16_t          deviceId = buf.get16(11);   // The Device Description Id for this endpoint.
  uint8_t           deviceVersion = buf.get8(13); // 0 – Version 1.00
  uint8_t           numInCluster = buf.get8(14);
  uint8_t           numOutCluster = buf.get8(15 + numInCluster*2);
  const size_t      numInIndex = 15;
  const size_t      numOutIndex = 16 + numInCluster*2;
#endif
#ifdef USE_ZIGBEE_EZSP
  uint8_t           status = buf.get8(0);
  Z_ShortAddress    nwkAddr = buf.get16(1);
  // uint8_t           lenDescriptor = buf.get8(3);
  uint8_t           endpoint = buf.get8(4);
  uint16_t          profileId = buf.get16(5);  // The profile Id for this endpoint.
  uint16_t          deviceId = buf.get16(7);   // The Device Description Id for this endpoint.
  uint8_t           deviceVersion = buf.get8(9); // 0 – Version 1.00
  uint8_t           numInCluster = buf.get8(10);
  uint8_t           numOutCluster = buf.get8(11 + numInCluster*2);
  const size_t      numInIndex = 11;
  const size_t      numOutIndex = 12 + numInCluster*2;
#endif
  bool is_tuya_protocol = false;

  if (0 == status) {
    // device is reachable
    zigbee_devices.deviceWasReached(nwkAddr);

    if (!Settings->flag4.zb_disable_autobind) {
      Z_AutoBindDefer(nwkAddr, endpoint, buf, numInIndex, numInCluster, numOutIndex, numOutCluster);
    }

    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                    "\"Status\":%d,\"Device\":\"0x%04X\",\"Endpoint\":\"0x%02X\""
                    ",\"ProfileId\":\"0x%04X\",\"DeviceId\":\"0x%04X\",\"DeviceVersion\":%d"
                    ",\"InClusters\":["),
                    ZIGBEE_STATUS_SIMPLE_DESC, nwkAddr, endpoint,
                    profileId, deviceId, deviceVersion);
    for (uint32_t i = 0; i < numInCluster; i++) {
      if (i > 0) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"0x%04X\""), buf.get16(numInIndex + i*2));
      if (buf.get16(numInIndex + i*2) == 0xEF00) { is_tuya_protocol = true; }   // tuya protocol
    }
    ResponseAppend_P(PSTR("],\"OutClusters\":["));
    for (uint32_t i = 0; i < numOutCluster; i++) {
      if (i > 0) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"0x%04X\""), buf.get16(numOutIndex + i*2));
    }
    ResponseAppend_P(PSTR("]}}"));
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  }

  // If tuya protocol, change the model information
  if (is_tuya_protocol) {
    Z_Device & device = zigbee_devices.getShortAddr(nwkAddr);
    device.addEndpoint(endpoint);
    device.data.get<Z_Data_Mode>(endpoint).setConfig(ZM_Tuya);
    zigbee_devices.dirty();
  }

  return -1;
}

//
// Handle IEEEAddr incoming message
//
// Same works for both ZNP and EZSP
int32_t Z_ReceiveIEEEAddr(int32_t res, const SBuffer &buf) {
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
    zigbee_devices.deviceWasReached(nwkAddr);
    // Ping response
    const char * friendlyName = zigbee_devices.getFriendlyName(nwkAddr);

    Response_P(PSTR("{\"" D_JSON_ZIGBEE_PING "\":{\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""
                    ",\"" D_JSON_ZIGBEE_IEEE "\":\"0x%_X\""), nwkAddr, &ieeeAddr);
    if (friendlyName) {
      ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), friendlyName);
    }
    ResponseAppend_P(PSTR("}}"));

    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  }
  return -1;
}
//
// Report any AF_DATA_CONFIRM message
// Ex: {"ZbConfirm":{"Endpoint":1,"Status":0,"StatusMessage":"SUCCESS"}}
//
int32_t ZNP_DataConfirm(int32_t res, const SBuffer &buf) {
  uint8_t           status = buf.get8(2);
  uint8_t           endpoint = buf.get8(3);
  //uint8_t           transId = buf.get8(4);    // unused

  if (status) {   // only report errors
    Response_P(PSTR("{\"" D_JSON_ZIGBEE_CONFIRM "\":{\"" D_CMND_ZIGBEE_ENDPOINT "\":%d"
                      ",\"" D_JSON_ZIGBEE_STATUS "\":%d"
                      ",\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\""
                      "}}"), endpoint, status, getZigbeeStatusMessage(status).c_str());
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
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
int32_t ZNP_ReceiveStateChange(int32_t res, const SBuffer &buf) {
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

    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
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
int32_t Z_ReceiveEndDeviceAnnonce(int32_t res, const SBuffer &buf) {
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

  // record if we already knew the ieeeAddr for this device
  // this will influence the decision whether we do auto-binding or not
  const Z_Device & device_before = zigbee_devices.findShortAddr(nwkAddr);
  bool ieee_already_known = false;
  if (device_before.valid() && (device_before.longaddr != 0) && (device_before.longaddr == ieeeAddr)) {
    ieee_already_known = true;
  }

  zigbee_devices.updateDevice(nwkAddr, ieeeAddr);
  // device is reachable
  zigbee_devices.deviceWasReached(nwkAddr);

  bool power_source = (capabilities & 0x04);
  if (power_source) {
    zigbee_devices.deviceHasNoBattery(nwkAddr);
  }

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"0x%_X\",\"ShortAddr\":\"0x%04X\""
                  ",\"PowerSource\":%s,\"ReceiveWhenIdle\":%s,\"Security\":%s}}"),
                  ZIGBEE_STATUS_DEVICE_ANNOUNCE, &ieeeAddr, nwkAddr,
                  power_source ? PSTR("true") : PSTR("false"),
                  (capabilities & 0x08) ? PSTR("true") : PSTR("false"),
                  (capabilities & 0x40) ? PSTR("true") : PSTR("false")
                  );
  // query the state of the bulb (for Alexa)
  uint32_t wait_ms = 2000;    // wait for 2s
  Z_Query_Bulb(nwkAddr, 0xFF, wait_ms);    // 0xFF means iterate on all endpoints

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  // Continue the discovery process and auto-binding only if the device was unknown or if PermitJoin is ongoing
  if (!ieee_already_known || zigbee.permit_end_time) {
    Z_SendActiveEpReq(nwkAddr);
  }
  return -1;
}

//
// Handle Receive TC Dev Ind incoming message
// 45CA
//
int32_t ZNP_ReceiveTCDevInd(int32_t res, const SBuffer &buf) {
  Z_ShortAddress    srcAddr = buf.get16(2);
  Z_IEEEAddress     ieeeAddr = buf.get64(4);
  Z_ShortAddress    parentNw = buf.get16(12);

  zigbee_devices.updateDevice(srcAddr, ieeeAddr);
  // device is reachable
  zigbee_devices.deviceWasReached(srcAddr);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                  "\"Status\":%d,\"IEEEAddr\":\"0x%_X\",\"ShortAddr\":\"0x%04X\""
                  ",\"ParentNetwork\":\"0x%04X\"}}"),
                  ZIGBEE_STATUS_DEVICE_INDICATION, &ieeeAddr, srcAddr, parentNw
                  );

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  return -1;
}

//
// Handle Bind Rsp incoming message
//
int32_t Z_BindRsp(int32_t res, const SBuffer &buf) {
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

  // device is reachable
  zigbee_devices.deviceWasReached(nwkAddr);

  const char * friendlyName = zigbee_devices.getFriendlyName(nwkAddr);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_BIND "\":{\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""), nwkAddr);
  if (friendlyName) {
    ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), friendlyName);
  }
  ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_STATUS "\":%d"
                  ",\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\""
                  "}}"), status, msg.c_str());

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));

  return -1;
}

//
// Handle Unbind Rsp incoming message
//
int32_t Z_UnbindRsp(int32_t res, const SBuffer &buf) {
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

  // device is reachable
  zigbee_devices.deviceWasReached(nwkAddr);

  const char * friendlyName = zigbee_devices.getFriendlyName(nwkAddr);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_UNBIND "\":{\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""), nwkAddr);
  if (friendlyName) {
    ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), friendlyName);
  }
  ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_STATUS "\":%d"
                  ",\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\""
                  "}}"), status, msg.c_str());

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));

  return -1;
}

//
// Handle MgMt Bind Rsp incoming message
//
int32_t Z_MgmtBindRsp(int32_t res, const SBuffer &buf) {
  return Z_Mgmt_Lqi_Bind_Rsp(res, buf, false);
}

// Return false, true or null (if unknown)
const char * TrueFalseNull(uint32_t value) {
  if (value == 0) {
    return PSTR("false");
  } else if (value == 1) {
    return PSTR("true");
  } else {
    return PSTR("null");
  }
}

const char * Z_DeviceRelationship(uint32_t value) {
  switch (value) {
    case 0: return PSTR("Parent");
    case 1: return PSTR("Child");
    case 2: return PSTR("Sibling");
    case 4: return PSTR("Previous");
    case 3:
    default: return PSTR("None");
  }
}

const char * Z_DeviceType(uint32_t value) {
  switch (value) {
    case 0: return PSTR("Coordinator");
    case 1: return PSTR("Router");
    case 2: return PSTR("Device");
    default: return PSTR("Unknown");
  }
}

//
// Combined code for MgmtLqiRsp and MgmtBindRsp
//
// If the response has a follow-up, send more requests automatically
//
int32_t Z_Mgmt_Lqi_Bind_Rsp(int32_t res, const SBuffer &buf, boolean lqi) {
#ifdef USE_ZIGBEE_ZNP
  uint16_t    shortaddr   = buf.get16(2);
  uint8_t     status      = buf.get8(4);
  uint8_t     total       = buf.get8(5);
  uint8_t     start       = buf.get8(6);
  uint8_t     len         = buf.get8(7);
  const size_t prefix_len = 8;
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
  uint16_t    shortaddr   = buf.get16(buf.len()-2);
  uint8_t     status      = buf.get8(0);
  uint8_t     total       = buf.get8(1);
  uint8_t     start       = buf.get8(2);
  uint8_t     len         = buf.get8(3);
  const size_t prefix_len = 4;
#endif // USE_ZIGBEE_EZSP

  // device is reachable
  zigbee_devices.deviceWasReached(shortaddr);
  bool non_empty = false;     // check whether the response contains relevant information

  const char * friendlyName = zigbee_devices.getFriendlyName(shortaddr);

  Response_P(PSTR("{\"%s\":{\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""),
                  lqi ? PSTR(D_JSON_ZIGBEE_MAP) : PSTR(D_JSON_ZIGBEE_BIND_STATE), shortaddr);
  if (friendlyName) {
    ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), friendlyName);
  }
  ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_STATUS "\":%d"
                        ",\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\""
                        ",\"Total\":%d"
                        ",\"Start\":%d"
                        ",\"%s\":["
                        ), status, getZigbeeStatusMessage(status).c_str(), total, start + 1,
                           lqi ? PSTR("Map") : PSTR("Bindings"));

  if (lqi) {
    uint32_t idx = prefix_len;
    for (uint32_t i = 0; i < len; i++) {
      if (idx + 22 > buf.len()) { break; }   // size 22 for EZSP

      //uint64_t    extpanid   = buf.get16(idx);     // unused
      // uint64_t    m_longaddr  = buf.get64(idx + 8);
      uint16_t    m_shortaddr = buf.get16(idx + 16);
      uint8_t     m_dev_type  = buf.get8(idx + 18);
      uint8_t     m_permitjoin = buf.get8(idx + 19);
      uint8_t     m_depth     = buf.get8(idx + 20);
      uint8_t     m_lqi       = buf.get8(idx + 21);
      idx += 22;

      non_empty = true;
      if (i > 0) {
        ResponseAppend_P(PSTR(","));
      }
      ResponseAppend_P(PSTR("{\"Device\":\"0x%04X\","), m_shortaddr);

      const char * friendlyName = zigbee_devices.getFriendlyName(m_shortaddr);
      if (friendlyName) {
        ResponseAppend_P(PSTR("\"Name\":\"%s\","), friendlyName);
      }
      ResponseAppend_P(PSTR("\"DeviceType\":\"%s\","
                            "\"RxOnWhenIdle\":%s,"
                            "\"Relationship\":\"%s\","
                            "\"PermitJoin\":%s,"
                            "\"Depth\":%d,"
                            "\"LinkQuality\":%d"
                            "}"
                            ),
                            Z_DeviceType(m_dev_type & 0x03),
                            TrueFalseNull((m_dev_type & 0x0C) >> 2),
                            Z_DeviceRelationship((m_dev_type & 0x70) >> 4),
                            TrueFalseNull(m_permitjoin & 0x02),
                            m_depth,
                            m_lqi);

      // detect any router
      Z_Device & device = zigbee_devices.findShortAddr(m_shortaddr);
      if (device.valid()) {
        if ((m_dev_type & 0x03) == 1) {   // it is a router
          device.setRouter(true);
        }
      }

      // Add information to zigbee mapper
      // Z_Mapper_Edge::Edge_Type edge_type;
      // switch ((m_dev_type & 0x70) >> 4) {
      //   case 0: edge_type = Z_Mapper_Edge::Parent;       break;
      //   case 1: edge_type = Z_Mapper_Edge::Child;        break;
      //   case 2: edge_type = Z_Mapper_Edge::Sibling;      break;
      //   default: edge_type = Z_Mapper_Edge::Unknown;     break;

      // }
      // Z_Mapper_Edge edge(m_shortaddr, shortaddr, m_lqi, edge_type);
      Z_Mapper_Edge edge(m_shortaddr, shortaddr, m_lqi);
      zigbee_mapper.addEdge(edge);
    }

    ResponseAppend_P(PSTR("]}}"));
  } else {  // Bind

    uint32_t idx = prefix_len;
    for (uint32_t i = 0; i < len; i++) {
      if (idx + 14 > buf.len()) { break; }   // overflow, frame size is between 14 and 21

      //uint64_t    srcaddr   = buf.get16(idx);     // unused
      uint8_t     srcep     = buf.get8(idx + 8);
      uint16_t    cluster   = buf.get16(idx + 9);
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
        //AddLog(LOG_LEVEL_INFO, PSTR("ZNP_MgmtBindRsp unknwon address mode %d"), addrmode);
        break;                                      // abort for any other value since we don't know the length of the field
      }

      non_empty = true;
      if (i > 0) {
        ResponseAppend_P(PSTR(","));
      }
      ResponseAppend_P(PSTR("{\"Cluster\":\"0x%04X\",\"Endpoint\":%d,"), cluster, srcep);
      if (Z_Addr_Group == addrmode) {               // Group address mode
        ResponseAppend_P(PSTR("\"ToGroup\":%d}"), group);
      } else if (Z_Addr_IEEEAddress == addrmode) {  // IEEE address mode
        ResponseAppend_P(PSTR("\"ToDevice\":\"0x%_X\",\"ToEndpoint\":%d}"), &dstaddr, dstep);
      }
    }

    ResponseAppend_P(PSTR("]}}"));
  }

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_MAP));

  // Check if there are more values waiting, if so re-send a new request to get other values
  // Only send a new request if the current was non-empty. This avoids an infinite loop if the device announces more slots that it actually has.
  if ((non_empty) && (start + len < total)) {
    // there are more values to read
#ifdef USE_ZIGBEE_ZNP
      Z_Send_State_or_Map(shortaddr, start + len, lqi ? ZDO_MGMT_LQI_REQ : ZDO_MGMT_BIND_REQ);
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
      Z_Send_State_or_Map(shortaddr, start + len, lqi ? ZDO_Mgmt_Lqi_req : ZDO_Mgmt_Bind_req);
#endif // USE_ZIGBEE_EZSP
  }

  return -1;
}

//
// Handle MgMt Bind Rsp incoming message
//
int32_t Z_MgmtLqiRsp(int32_t res, const SBuffer &buf) {
  return Z_Mgmt_Lqi_Bind_Rsp(res, buf, true);
}

#ifdef USE_ZIGBEE_EZSP
//
// Handle Parent Annonce Rsp incoming message
//
// rsp: true = ZDO_Parent_annce_rsp, false = ZDO_Parent_annce
int32_t EZ_ParentAnnceRsp(int32_t res, const SBuffer &buf, bool rsp) {
  size_t prefix_len;
  uint8_t     status;
  uint8_t     num_children;
  uint16_t    shortaddr   = buf.get16(buf.len()-2);
  if (rsp) {
    status      = buf.get8(0);
    num_children = buf.get8(1);
    prefix_len = 2;
  } else {
    status      = 0;
    num_children = buf.get8(0);
    prefix_len = 1;
  }

  // device is reachable
  zigbee_devices.deviceWasReached(shortaddr);

  const char * friendlyName = zigbee_devices.getFriendlyName(shortaddr);

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_PARENT "\":{\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\""), shortaddr);
  if (friendlyName) {
    ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_NAME "\":\"%s\""), friendlyName);
  }
  if (rsp) {
    ResponseAppend_P(PSTR(",\"" D_JSON_ZIGBEE_STATUS "\":%d"
                          ",\"" D_JSON_ZIGBEE_STATUS_MSG "\":\"%s\""
                          ), status, getZigbeeStatusMessage(status).c_str());
  }
  ResponseAppend_P(PSTR(",\"Children\":%d"
                        ",\"ChildInfo\":["
                        ), num_children);

  uint32_t idx = prefix_len;
  for (uint32_t i = 0; i < num_children; i++) {
    if (idx + 8 > buf.len()) { break; }   // overflow, frame size is between 14 and 21

    uint64_t    child_ieee = buf.get64(idx);
    idx += 8;

    if (i > 0) {
      ResponseAppend_P(PSTR(","));
    }
    ResponseAppend_P(PSTR("\"0x%_X\""), &child_ieee);
  }

  ResponseAppend_P(PSTR("]}}"));

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_BIND_STATE));

  return -1;
}
#endif // USE_ZIGBEE_EZSP

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
// Probe the clusters_out on the first endpoint
//
// Send ZDO_SIMPLE_DESC_REQ to get full list of supported Clusters for a specific endpoint
void Z_SendSimpleDescReq(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
#ifdef USE_ZIGBEE_ZNP
  uint8_t SimpleDescReq[] = { Z_SREQ | Z_ZDO, ZDO_SIMPLE_DESC_REQ,  // 2504
              Z_B0(shortaddr), Z_B1(shortaddr), Z_B0(shortaddr), Z_B1(shortaddr),
              endpoint };
  ZigbeeZNPSend(SimpleDescReq, sizeof(SimpleDescReq));
#endif
#ifdef USE_ZIGBEE_EZSP
  uint8_t SimpleDescReq[] = { Z_B0(shortaddr), Z_B1(shortaddr), endpoint };
  EZ_SendZDO(shortaddr, ZDO_SIMPLE_DESC_REQ, SimpleDescReq, sizeof(SimpleDescReq));
#endif
}


//
// Send AF Info Request
// Queue requests for the device
// 1. Request for 'ModelId' and 'Manufacturer': 0000/0005, 0000/0004
// 2. Auto-bind to coordinator:
//    Iterate among
//
void Z_SendDeviceInfoRequest(uint16_t shortaddr) {
  ZCLFrame zcl(12);   // message is 12 bytes
  zcl.shortaddr = shortaddr;
  zcl.cluster = 0x0000;
  zcl.cmd = ZCL_READ_ATTRIBUTES;
  zcl.clusterSpecific = false;
  zcl.needResponse = true;
  zcl.direct = false;   // discover route
  zcl.payload.add16(0x0005);
  zcl.payload.add16(0x0004);
  // Tuya needs a magic spell reading more attributes
  // cf https://github.com/zigpy/zha-device-handlers/issues/2042
  zcl.payload.add16(0x0000);  // Manufacturer Name
  zcl.payload.add16(0x0001);  // Application Version
  zcl.payload.add16(0x0007);  // Power Source
  zcl.payload.add16(0xfffe);  // Unknown
  zigbeeZCLSendCmd(zcl);
}

//
// Send single attribute read request in Timer
//
void Z_SendSingleAttributeRead(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  ZCLFrame zcl(2);   // message is 2 bytes
  zcl.shortaddr = shortaddr;
  zcl.cluster = cluster;
  zcl.dstendpoint = endpoint;
  zcl.cmd = ZCL_READ_ATTRIBUTES;
  zcl.clusterSpecific = false;
  zcl.needResponse = true;
  zcl.direct = false;   // discover route
  zcl.payload.add16(value);    // 04000500
  zigbeeZCLSendCmd(zcl);
}

//
// Send single attribute read request in Timer
//
void Z_SendSinglePlugMulDivAttributesRead(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  ZCLFrame zcl(12);   // message is 12 bytes
  zcl.shortaddr = shortaddr;
  zcl.cluster = 0x0B04;
  zcl.dstendpoint = endpoint;
  zcl.cmd = ZCL_READ_ATTRIBUTES;
  zcl.clusterSpecific = false;
  zcl.needResponse = true;
  zcl.direct = false;   // discover route
  zcl.payload.add16(0x0600);
  zcl.payload.add16(0x0601);
  zcl.payload.add16(0x0602);
  zcl.payload.add16(0x0603);
  zcl.payload.add16(0x0604);
  zcl.payload.add16(0x0605);
  zigbeeZCLSendCmd(zcl);
}

//
// Write CIE address
//
void Z_WriteCIEAddress(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Sending CIE Address for Cluster %d in Endpoint %d of Device 0x%04X"), cluster, endpoint, shortaddr);
  ZCLFrame zcl(12);   // message is 12 bytes
  zcl.shortaddr = shortaddr;
  zcl.cluster = 0x0500;
  zcl.dstendpoint = endpoint;
  zcl.cmd = ZCL_WRITE_ATTRIBUTES;
  zcl.clusterSpecific = false;
  zcl.needResponse = true;
  zcl.direct = false;   // discover route
  zcl.payload.add16(0x0010);    // attribute 0x0010
  zcl.payload.add8(ZEUI64);
  zcl.payload.add64(localIEEEAddr);
  zigbeeZCLSendCmd(zcl);
}


//
// Send CIE Zone Enroll Response
//
void Z_SendCIEZoneEnrollResponse(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Sending Enroll Zone %d for Cluster %d in Endpoint %d of Device 0x%04X"), Z_B0(value), cluster, endpoint, shortaddr);
  ZCLFrame zcl(2);   // message is 2 bytes
  zcl.shortaddr = shortaddr;
  zcl.cluster = 0x0500;
  zcl.dstendpoint = endpoint;
  zcl.cmd = 0x00;   // Zone Enroll Response
  zcl.clusterSpecific = true;
  zcl.needResponse = true;
  zcl.direct = false;   // discover route
  zcl.payload.add8(0x00);   // success
  zcl.payload.add8(Z_B0(value)); // ZoneID
  zigbeeZCLSendCmd(zcl);
}

//
// Auto-bind some clusters to the coordinator's endpoint 0x01
//
void Z_AutoBind(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  uint64_t srcLongAddr = zigbee_devices.getDeviceLongAddr(shortaddr);

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "auto-bind `ZbBind {\"Device\":\"0x%04X\",\"Endpoint\":%d,\"Cluster\":\"0x%04X\"}`"),
                                  shortaddr, endpoint, cluster);
#ifdef USE_ZIGBEE_ZNP
  SBuffer buf(34);
  buf.add8(Z_SREQ | Z_ZDO);
  buf.add8(ZDO_BIND_REQ);
  buf.add16(shortaddr);
  buf.add64(srcLongAddr);
  buf.add8(endpoint);
  buf.add16(cluster);
  buf.add8(Z_Addr_IEEEAddress);         // DstAddrMode - 0x03 = ADDRESS_64_BIT
  buf.add64(localIEEEAddr);
  buf.add8(0x01);   // toEndpoint

  ZigbeeZNPSend(buf.getBuffer(), buf.len());
#endif // USE_ZIGBEE_ZNP

#ifdef USE_ZIGBEE_EZSP
  SBuffer buf(24);

  // ZDO message payload (see Zigbee spec 2.4.3.2.2)
  buf.add64(srcLongAddr);
  buf.add8(endpoint);
  buf.add16(cluster);
  buf.add8(Z_Addr_IEEEAddress);         // DstAddrMode - 0x03 = ADDRESS_64_BIT
  buf.add64(localIEEEAddr);
  buf.add8(0x01);   // toEndpoint

  EZ_SendZDO(shortaddr, ZDO_BIND_REQ, buf.buf(), buf.len());
#endif // USE_ZIGBEE_EZSP
}

//
// Auto-bind some clusters to the coordinator's endpoint 0x01
//

// the structure below indicates which attributes need to be configured for attribute reporting
typedef struct Z_autoAttributeReporting_t {
  uint16_t cluster;
  uint16_t attr_id;
  uint16_t min_interval;    // minimum interval in seconds (consecutive reports won't happen before this value)
  uint16_t max_interval;    // maximum interval in seconds (attribut will always be reported after this interval)
  float    report_change;   // for non discrete attributes, the value change that triggers a report
} Z_autoAttributeReporting_t;

// Note the attribute must be registered in the converter list, used to retrieve the type of the attribute
const Z_autoAttributeReporting_t Z_autoAttributeReporting[] PROGMEM = {
  { 0x0001, 0x0020,    60*60, USE_ZIGBEE_MAXTIME_BATT,  USE_ZIGBEE_AUTOBIND_BATTVOLTAGE },      // BatteryVoltage
  { 0x0001, 0x0021,    60*60, USE_ZIGBEE_MAXTIME_BATT,  USE_ZIGBEE_AUTOBIND_BATTPERCENT },      // BatteryPercentage
  { 0x0006, 0x0000,        1,   USE_ZIGBEE_MAXTIME_LIGHT,    0 },      // Power
  { 0x0102, 0x0008,        1,   USE_ZIGBEE_MAXTIME_LIFT, USE_ZIGBEE_AUTOBIND_LIFT },      // CurrentPositionLiftPercentage
  { 0x0201, 0x0000,       60,   USE_ZIGBEE_MAXTIME_TRV,  USE_ZIGBEE_AUTOBIND_TEMPERATURE },      // LocalTemperature
  { 0x0201, 0x0008,       60,   USE_ZIGBEE_MAXTIME_TRV,  USE_ZIGBEE_AUTOBIND_HEATDEMAND  },      // PIHeatingDemand
  { 0x0201, 0x0012,       60,   USE_ZIGBEE_MAXTIME_TRV,  USE_ZIGBEE_AUTOBIND_TEMPERATURE },      // OccupiedHeatingSetpoint
  { 0x0008, 0x0000,        1,   USE_ZIGBEE_MAXTIME_LIGHT,    5 },      // Dimmer
  { 0x0300, 0x0000,        1,   USE_ZIGBEE_MAXTIME_LIGHT,    5 },      // Hue
  { 0x0300, 0x0001,        1,   USE_ZIGBEE_MAXTIME_LIGHT,    5 },      // Sat
  { 0x0300, 0x0003,        1,   USE_ZIGBEE_MAXTIME_LIGHT,  100 },      // X
  { 0x0300, 0x0004,        1,   USE_ZIGBEE_MAXTIME_LIGHT,  100 },      // Y
  { 0x0300, 0x0007,        1,   USE_ZIGBEE_MAXTIME_LIGHT,    5 },      // CT
  { 0x0300, 0x0008,        1,   USE_ZIGBEE_MAXTIME_LIGHT,    0 },      // ColorMode
  { 0x0400, 0x0000,       10,   USE_ZIGBEE_MAXTIME_SENSOR,  USE_ZIGBEE_AUTOBIND_ILLUMINANCE },      // Illuminance (5 lux)
  { 0x0402, 0x0000,       30,   USE_ZIGBEE_MAXTIME_SENSOR,  USE_ZIGBEE_AUTOBIND_TEMPERATURE },      // Temperature (0.2 °C)
  { 0x0403, 0x0000,       30,   USE_ZIGBEE_MAXTIME_SENSOR,  USE_ZIGBEE_AUTOBIND_PRESSURE    },      // Pressure (1 hPa)
  { 0x0405, 0x0000,       30,   USE_ZIGBEE_MAXTIME_SENSOR,  USE_ZIGBEE_AUTOBIND_HUMIDITY    },      // Humidity (1 %)
  { 0x0406, 0x0000,       10,   USE_ZIGBEE_MAXTIME_SENSOR,    0 },      // Occupancy
  { 0x0500, 0x0002,        1,   USE_ZIGBEE_MAXTIME_SENSOR,    0 },      // ZoneStatus
};

//
// Called by Device Auto-config
// Configures default values for the most common Attribute Rerporting configurations
//
// Note: must be of type `Z_DeviceTimer`
void Z_AutoConfigReportingForCluster(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  // Buffer, max 12 bytes per attribute
  SBuffer buf(12*6);


  Response_P(PSTR("ZbSend {\"Device\":\"0x%04X\",\"Config\":{"), shortaddr);

  boolean comma = false;
  for (uint32_t i=0; i<nitems(Z_autoAttributeReporting); i++) {
    uint16_t conv_cluster = pgm_read_word(&(Z_autoAttributeReporting[i].cluster));
    uint16_t attr_id = pgm_read_word(&(Z_autoAttributeReporting[i].attr_id));

    if (conv_cluster == cluster) {
      uint16_t min_interval = pgm_read_word(&(Z_autoAttributeReporting[i].min_interval));
      uint16_t max_interval = pgm_read_word(&(Z_autoAttributeReporting[i].max_interval));
      float  report_change_raw = Z_autoAttributeReporting[i].report_change;
      double report_change = report_change_raw;
      // uint8_t attr_type;
      // int8_t  multiplier;

      Z_attribute_match attr_matched = Z_findAttributeMatcherById(shortaddr, cluster, attr_id, false);
      if (attr_matched.found()) {
        if (comma) { ResponseAppend_P(PSTR(",")); }
        comma = true;
        ResponseAppend_P(PSTR("\"%s\":{\"MinInterval\":%d,\"MaxInterval\":%d"), attr_matched.name, min_interval, max_interval);

        buf.add8(0);            // direction, always 0
        buf.add16(attr_id);
        buf.add8(attr_matched.zigbee_type);
        buf.add16(min_interval);
        buf.add16(max_interval);
        if (!Z_isDiscreteDataType(attr_matched.zigbee_type)) {   // report_change is only valid for non-discrete data types (numbers)
          ZbApplyMultiplierForWrites(report_change, attr_matched.multiplier, attr_matched.divider, attr_matched.base);
          // encode value
          int32_t res = encodeSingleAttribute(buf, report_change, "", attr_matched.zigbee_type);
          if (res < 0) {
            AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "internal error, unsupported attribute type"));
          } else {
            Z_attribute attr;
            attr.setKeyName(PSTR("ReportableChange"), true);    // true because in PMEM
            attr.setFloat(report_change_raw);
            ResponseAppend_P(PSTR(",%s"), attr.toString().c_str());
          }
        }
        ResponseAppend_P(PSTR("}"));
      }
    }
  }
  ResponseAppend_P(PSTR("}}"));

  if (buf.len() > 0) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "auto-bind `%s`"), ResponseData());
    ZCLFrame zcl(buf.len());   // message is 4 bytes
    zcl.shortaddr = shortaddr;
    zcl.cluster = cluster;
    zcl.dstendpoint = endpoint;
    zcl.cmd = ZCL_CONFIGURE_REPORTING;
    zcl.clusterSpecific = false;  /* not cluster specific */
    zcl.needResponse = false;     /* noresponse */
    zcl.direct = false;           /* discover route */
    zcl.payload.addBuffer(buf);
    zigbeeZCLSendCmd(zcl);
  }
}

//
// Handle trustCenterJoinHandler
// 2400
//
#ifdef USE_ZIGBEE_EZSP
int32_t EZ_ReceiveTCJoinHandler(int32_t res, const SBuffer &buf) {
  uint16_t      srcAddr = buf.get16(2);
  uint64_t      ieeeAddr = buf.get64(4);
  uint8_t       status = buf.get8(12);
  uint8_t       decision = buf.get8(13);
  uint16_t      parentNw = buf.get16(14);

  if (EMBER_DEVICE_LEFT != status) {    // ignore message if the device is leaving
    zigbee_devices.updateDevice(srcAddr, ieeeAddr);

    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{"
                    "\"Status\":%d,\"IEEEAddr\":\"0x%_X\",\"ShortAddr\":\"0x%04X\""
                    ",\"ParentNetwork\":\"0x%04X\""
                    ",\"JoinStatus\":%d,\"Decision\":%d"
                    "}}"),
                    ZIGBEE_STATUS_DEVICE_INDICATION, &ieeeAddr, srcAddr, parentNw,
                    status, decision
                    );

    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  }
  return -1;
}
#endif // USE_ZIGBEE_EZSP

//
// Parse incoming ZCL message.
//
// This code is common to ZNP and EZSP
void Z_IncomingMessage(class ZCLFrame &zcl_received) {
  uint16_t srcaddr = zcl_received.getSrcAddr();
  uint16_t groupid = zcl_received.getGroupAddr();
  uint16_t clusterid = zcl_received.getClusterId();
  uint8_t  linkquality = zcl_received.getLinkQuality();
  uint8_t  srcendpoint = zcl_received.getSrcEndpoint();
  linkquality = linkquality != 0xFF ? linkquality : 0xFE;   // avoid 0xFF (reserved for unknown)

  bool            defer_attributes = false;     // do we defer attributes reporting to coalesce

  // log the packet details
  zcl_received.log();

#ifdef USE_BERRY
  // Berry pre-process messages
  callBerryZigbeeDispatcher("frame_received", &zcl_received, nullptr, srcaddr);
#endif // USE_BERRY

  // create the device entry if it does not exist and if it's not the local device
  Z_Device & device = (srcaddr != localShortAddr) ? zigbee_devices.getShortAddr(srcaddr) :
                                                    device_unk;
  if (device.valid()) {
    device.setLQI(linkquality != 0xFF ? linkquality : 0xFE);       // EFR32 has a different scale for LQI
    device.setLastSeenNow();
    zigbee_devices.deviceWasReached(srcaddr);
  }

  char shortaddr[8];
  snprintf_P(shortaddr, sizeof(shortaddr), PSTR("0x%04X"), srcaddr);

  Z_attribute_list attr_list;
  attr_list.lqi = linkquality;
  attr_list.src_ep = srcendpoint;
  if (groupid) {      // TODO we miss the group_id == 0 here
    attr_list.group_id = groupid;
  }

  // uint8_t cmdid = zcl_received.getCmdId();
  bool cmd_ignore = false;      // ignore the command in later processing

  if (zcl_received.isClusterSpecificCommand()) {
    // Cluster-specific command
    zcl_received.parseClusterSpecificCommand(attr_list);
    Z_Query_Battery(srcaddr);   // do battery auto-probing when receiving commands
  } else {
    // General cluster command
    switch (zcl_received.getCmdId()) {
      case ZCL_DEFAULT_RESPONSE:
        zcl_received.parseResponse();   // Zigbee general "Default Response", publish ZbResponse message
        cmd_ignore = true;
        break;
      case ZCL_REPORT_ATTRIBUTES:
      case ZCL_WRITE_ATTRIBUTES:
        zcl_received.parseReportAttributes(attr_list);    // Zigbee report attributes from sensors

        // since we receive a sensor value, and the device is still awake,
        // try to read the battery value
        if (clusterid != 0x0001) {    // avoid sending Battery probe if we already received info from cluster 0x0001
          Z_Query_Battery(srcaddr);
        }
        if (clusterid && zcl_received.getCmdId() == ZCL_REPORT_ATTRIBUTES) { defer_attributes = true; }  // defer attributes reporting except for cluster 0x0000 or Write Attribute
        break;
      case ZCL_READ_ATTRIBUTES_RESPONSE:
        zcl_received.parseReadAttributesResponse(attr_list);
        if (clusterid) { defer_attributes = true; }  // defer attributes reporting except for cluster 0x0000
        break;
      case ZCL_READ_ATTRIBUTES:
        zcl_received.parseReadAttributes(srcaddr, attr_list);
        // never defer read_attributes, so the auto-responder can send response back on a per cluster basis
        break;
      case ZCL_READ_REPORTING_CONFIGURATION_RESPONSE:
        zcl_received.parseReadConfigAttributes(srcaddr, attr_list);
        break;
      case ZCL_CONFIGURE_REPORTING_RESPONSE:
        zcl_received.parseConfigAttributes(srcaddr, attr_list);
        break;
      case ZCL_WRITE_ATTRIBUTES_RESPONSE:
        zcl_received.parseWriteAttributesResponse(attr_list);
        break;
      default:
        attr_list.addAttributeCmd(clusterid, zcl_received.getCmdId(), zcl_received.getDirection(), true /* general command */).setBuf(zcl_received.payload, 0, zcl_received.payload.len());
        break;
    }
  }

  // unless attributes are ignored, post-process and publish them
  if (!cmd_ignore) {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE D_JSON_ZIGBEEZCL_RAW_RECEIVED ": {\"0x%04X\":{%s}}"), srcaddr, attr_list.toString(false, false).c_str()); // don't include battery

#ifdef USE_BERRY
    // Berry pre-process messages
    callBerryZigbeeDispatcher("attributes_raw", &zcl_received, &attr_list, srcaddr);
#endif // USE_BERRY

    // discard the message if it was sent by us (broadcast or group loopback)
    if (srcaddr == localShortAddr) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE  "loopback message, ignoring"));
      return;     // abort the rest of message management
    }

    zcl_received.generateSyntheticAttributes(attr_list);
    zcl_received.removeInvalidAttributes(attr_list);
    zcl_received.applySynonymAttributes(attr_list);
    zcl_received.computeSyntheticAttributes(attr_list);
    zcl_received.generateCallBacks(attr_list);      // set deferred callbacks, ex: Occupancy
    Z_postProcessAttributes(srcaddr, zcl_received.getSrcEndpoint(), attr_list);

#ifdef USE_BERRY
    // Berry pre-process messages
    callBerryZigbeeDispatcher("attributes_refined", &zcl_received, &attr_list, srcaddr);
#endif // USE_BERRY

    if (!attr_list.isEmpty()) {
      if (defer_attributes) {
        // Prepare for publish
        if (zigbee_devices.jsonIsConflict(srcaddr, attr_list)) {
          // there is conflicting values, force a publish of the previous message now and don't coalesce
          zigbee_devices.jsonPublishFlush(srcaddr);
        }
        zigbee_devices.jsonAppend(srcaddr, attr_list);
          zigbee_devices.setTimer(srcaddr, 0 /* groupaddr */, USE_ZIGBEE_COALESCE_ATTR_TIMER, 0 /*clusterid*/, srcendpoint, Z_CAT_READ_ATTR, 0, &Z_PublishAttributes);
      } else {
        // Publish immediately
        zigbee_devices.jsonPublishNow(srcaddr, attr_list);
      }
    }
  }
}

#ifdef USE_ZIGBEE_EZSP

/*********************************************************************************************\
 * Send ZDO Message
\*********************************************************************************************/

void EZ_SendZDO(uint16_t shortaddr, uint16_t cmd, const unsigned char *payload, size_t payload_len, bool retry) {
  SBuffer buf(payload_len + 22);
  uint8_t seq = zigbee_devices.getNextSeqNumber(0x0000);

  if (shortaddr < 0xFFFC) {
    // send unicast
    buf.add16(EZSP_sendUnicast);

    buf.add8(EMBER_OUTGOING_DIRECT);    // 00
    buf.add16(shortaddr);               // dest addr
    // ApsFrame
    buf.add16(0x0000);                  // ZOD profile
    buf.add16(cmd);                     // ZDO cmd in cluster
    buf.add8(0);                        // srcEp
    buf.add8(0);                        // dstEp
    if (retry) {
      buf.add16(EMBER_APS_OPTION_ENABLE_ROUTE_DISCOVERY | EMBER_APS_OPTION_RETRY);      // APS frame
    } else {
      buf.add16(EMBER_APS_OPTION_ENABLE_ROUTE_DISCOVERY);      // APS frame
    }
    buf.add16(0x0000);                  // groupId
    buf.add8(seq);
    // end of ApsFrame
    buf.add8(0x01);                     // tag TODO
    buf.add8(payload_len + 1);        // insert seq number
    buf.add8(seq);
    buf.addBuffer(payload, payload_len);
  } else {
    // send broadcast
    buf.add16(EZSP_sendBroadcast);
    buf.add16(shortaddr);               // dest addr
    // ApsFrame
    buf.add16(0x0000);                  // ZOD profile
    buf.add16(cmd);                     // ZDO cmd in cluster
    buf.add8(0);                        // srcEp
    buf.add8(0);                        // dstEp
    buf.add16(0x00);      // APS frame
    buf.add16(0x0000);                  // groupId
    buf.add8(seq);
    // end of ApsFrame
    buf.add8(0x1E);                     // radius
    buf.add8(0x01);                     // tag TODO
    buf.add8(payload_len + 1);        // insert seq number
    buf.add8(seq);
    buf.addBuffer(payload, payload_len);
  }

  ZigbeeEZSPSendCmd(buf.buf(), buf.len());
}

/*********************************************************************************************\
 * Send specific EZSP messages
\*********************************************************************************************/

int32_t EZ_IncomingMessage(int32_t res, const SBuffer &buf) {
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
  int8_t          linkrssi = buf.get8(15);
  uint8_t         linkquality = ZNP_RSSI2Lqi(linkrssi);   // don't take EZSP LQI but calculate our own based on ZNP
  uint16_t        srcaddr = buf.get16(16);
  // uint8_t         bindingindex = buf.get8(18);      // not sure we need this one as a coordinator
  // uint8_t         addressindex = buf.get8(19);      // not sure how to handle this one
  // offset 20 is len, and buffer starts at offset 21


  if ((0x0000 == profileid) && (0x00 == srcendpoint))  {
    // ZDO request
    // Report LQI
    if (srcaddr != localShortAddr) {
      Z_Device & device = zigbee_devices.getShortAddr(srcaddr);
      device.setLQI(linkquality);
      device.setLastSeenNow();
    }
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
      case ZDO_Simple_Desc_rsp:
        return Z_ReceiveSimpleDesc(res, zdo_buf);
      case ZDO_Bind_rsp:
        return Z_BindRsp(res, zdo_buf);
      case ZDO_Unbind_rsp:
        return Z_UnbindRsp(res, zdo_buf);
      case ZDO_Mgmt_Lqi_rsp:
        return Z_MgmtLqiRsp(res, zdo_buf);
      case ZDO_Mgmt_Bind_rsp:
        return Z_MgmtBindRsp(res, zdo_buf);
      case ZDO_Parent_annce:
        return EZ_ParentAnnceRsp(res, zdo_buf, false);
      case ZDO_Parent_annce_rsp:
        return EZ_ParentAnnceRsp(res, zdo_buf, true);
      default:
        break;
    }
  } else {
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
// Callback for resetting the NCP, called by the state machine
//
// value = 0 : drive reset pin and halt MCU
// value = 1 : release the reset pin, restart
int32_t EZ_Reset_Device(uint8_t value) {
/*
  // we use Led4i to drive the reset pin. Since it is reverted we need to pass 1 to start reset, and 0 to release reset
  if (PinUsed(GPIO_LED1, ZIGBEE_EZSP_RESET_LED - 1)) {
    SetLedPowerIdx(ZIGBEE_EZSP_RESET_LED - 1, value ? 0 : 1);
*/
  if (PinUsed(GPIO_ZIGBEE_RST)) {
    digitalWrite(Pin(GPIO_ZIGBEE_RST), value);
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

int32_t EZ_Recv_Default(int32_t res, const SBuffer &buf) {
  // Default message handler for new messages
  if (zigbee.init_phase) {
    // if still during initialization phase, ignore any unexpected message
  	return -1;	// ignore message
  } else {
    uint16_t ezsp_command_index = buf.get16(0);

    switch (ezsp_command_index) {
      case EZSP_incomingMessageHandler:
        return EZ_IncomingMessage(res, buf);
      case EZSP_trustCenterJoinHandler:
        return EZ_ReceiveTCJoinHandler(res, buf);
      case EZSP_incomingRouteErrorHandler:
        return EZ_RouteError(res, buf);
      case EZSP_permitJoining:
        return EZ_PermitJoinRsp(res, buf);
      case EZSP_messageSentHandler:
        return EZ_MessageSent(res, buf);
      case EZSP_energyScanResultHandler:
        return EZSP_EnergyScanResult(res, buf);
      case EZSP_scanCompleteHandler:
        return EZSP_EnergyScanComplete(res, buf);
    }
    return -1;
  }
}

#endif // USE_ZIGBEE_EZSP

/*********************************************************************************************\
 * Callbacks
\*********************************************************************************************/

// Publish the received values once they have been coalesced
void Z_PublishAttributes(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  zigbee_devices.jsonPublishFlush(shortaddr);
}

/*********************************************************************************************\
 * Global dispatcher for incoming messages
\*********************************************************************************************/

#ifdef USE_ZIGBEE_ZNP

//
// Callback for resetting the NCP, called by the state machine
//
// value = 0 : drive reset pin and halt MCU
// value = 1 : release the reset pin, restart
int32_t ZNP_Reset_Device(uint8_t value) {
/*
  // we use Led4i to drive the reset pin. Since it is reverted we need to pass 1 to start reset, and 0 to release reset
  if (PinUsed(GPIO_LED1, ZIGBEE_EZSP_RESET_LED - 1)) {
    SetLedPowerIdx(ZIGBEE_EZSP_RESET_LED - 1, value ? 0 : 1);
*/
  if (PinUsed(GPIO_ZIGBEE_RST)) {
    digitalWrite(Pin(GPIO_ZIGBEE_RST), value);
  } else {
    // no GPIO so we use software Reset instead
    if (value) {  // send reset only when we are supposed to release reset
      // flush the serial buffer, sending 0xFF 256 times.
      ZigbeeZNPFlush();
      ZigbeeZNPSend(ZBS_RESET, sizeof(ZBS_RESET));
    }
  }
  return 0;                              // continue
}

int32_t ZNP_ReceiveAfIncomingMessage(int32_t res, const SBuffer &buf) {
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
  { { Z_AREQ | Z_ZDO, ZDO_SIMPLE_DESC_RSP},         &Z_ReceiveSimpleDesc},            // 4584
  { { Z_AREQ | Z_ZDO, ZDO_IEEE_ADDR_RSP },          &Z_ReceiveIEEEAddr },             // 4581
  { { Z_AREQ | Z_ZDO, ZDO_BIND_RSP },               &Z_BindRsp },                   // 45A1
  { { Z_AREQ | Z_ZDO, ZDO_UNBIND_RSP },             &Z_UnbindRsp },                 // 45A2
  { { Z_AREQ | Z_ZDO, ZDO_MGMT_LQI_RSP },           &Z_MgmtLqiRsp },                // 45B1
  { { Z_AREQ | Z_ZDO, ZDO_MGMT_BIND_RSP },          &Z_MgmtBindRsp },               // 45B3
};

/*********************************************************************************************\
 * Default resolver
\*********************************************************************************************/

int32_t ZNP_Recv_Default(int32_t res, const SBuffer &buf) {
  // Default message handler for new messages
  if (zigbee.init_phase) {
    // if still during initialization phase, ignore any unexpected message
  	return -1;	// ignore message
  } else {
    for (uint32_t i = 0; i < nitems(Z_DispatchTable); i++) {
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
// Callback for loading preparing EEPROM, called by the state machine
//
int32_t Z_Prepare_Storage(uint8_t value) {
#ifdef USE_ZIGBEE_EEPROM
  ZFS::initOrFormat();
#endif
  return 0;                              // continue
}

//
// Callback for loading Zigbee configuration, called by the state machine
//
int32_t Z_Load_Devices(uint8_t value) {
  // try to hidrate from known devices
  loadZigbeeDevices();
  return 0;                              // continue
}

//
// Callback for loading Zigbee data from EEPROM, called by the state machine
//
int32_t Z_Load_Data(uint8_t value) {
  hydrateDevicesData();
  return 0;                              // continue
}

static void Z_Query_Bulb_inner(uint16_t shortaddr, uint8_t ep, uint32_t &wait_ms) {
  const uint32_t inter_message_ms = 100;    // wait 100ms between messages
  if (ep == 0) { ep = zigbee_devices.findFirstEndpoint(shortaddr); }
  if (ep) {
    if (0 <= zigbee_devices.getHueBulbtype(shortaddr, ep)) {
      zigbee_devices.setTimer(shortaddr, 0 /* groupaddr */, wait_ms, 0x0006, ep, Z_CAT_READ_CLUSTER, 0 /* value */, &Z_ReadAttrCallback);
      wait_ms += inter_message_ms;
      zigbee_devices.setTimer(shortaddr, 0 /* groupaddr */, wait_ms, 0x0008, ep, Z_CAT_READ_CLUSTER, 0 /* value */, &Z_ReadAttrCallback);
      wait_ms += inter_message_ms;
      zigbee_devices.setTimer(shortaddr, 0 /* groupaddr */, wait_ms, 0x0300, ep, Z_CAT_READ_CLUSTER, 0 /* value */, &Z_ReadAttrCallback);
      wait_ms += inter_message_ms;
      zigbee_devices.setTimer(shortaddr, 0, wait_ms + Z_CAT_REACHABILITY_TIMEOUT, 0, ep, Z_CAT_REACHABILITY, 0 /* value */, &Z_Unreachable);
      wait_ms += 1000;              // wait 1 second between devices
    }
  }
}

//
// Query the state of a bulb (light) if its type allows it
//
// ep==0 is default endpoint
// ep==0xFF iterates on all endpoints
void Z_Query_Bulb(uint16_t shortaddr, uint8_t ep, uint32_t &wait_ms) {
  if (ep != 0xFF) {
    Z_Query_Bulb_inner(shortaddr, ep, wait_ms);   // try a single endpoint
  } else {
    // iterate on all endpoints
    const Z_Device & device = zigbee_devices.findShortAddr(shortaddr);
    if (!device.valid()) { return; }
    for (uint32_t i = 0; i < endpoints_max; i++) {
      ep = device.endpoints[i];
      if (ep == 0) { break; }
      Z_Query_Bulb_inner(shortaddr, ep, wait_ms);   // try a single endpoint
    }
  }
}

//
// Query the status of the battery (auto-probe)
//
void Z_Query_Battery(uint16_t shortaddr) {
  if (Settings->flag5.zigbee_no_batt_autoprobe) { return; }   // don't do auto-probe if `SetOption143 1`
  if (0 == shortaddr) { return; }
  Z_Device & device = zigbee_devices.findShortAddr(shortaddr);
  if (device.valid()) {
    uint32_t now = Rtc.utc_time;
    if (now < START_VALID_TIME) { return; }     // internal time is not valid
    if (device.hasNoBattery()) { return; }      // device is known to have no battery
    if (device.batt_last_seen + USE_ZIGBEE_BATT_REPROBE > now) { return; }     // battery status is fresh enough
    if (device.batt_last_probed + USE_ZIGBEE_BATT_REPROBE_PAUSE > now) { return; }  // battery has been probed soon enough

    uint8_t endpoint = zigbee_devices.findFirstEndpoint(shortaddr);
    if (endpoint) {   // send only if we know the endpoint
      device.batt_last_probed = now;                   // we are probing now
      zigbee_devices.setTimer(shortaddr, 0 /* groupaddr */, 0 /* now */, 0x0001, endpoint, Z_CAT_READ_CLUSTER, 0 /* value */, &Z_ReadAttrCallback);
      AddLog(LOG_LEVEL_INFO, PSTR("ZIG: Battery auto-probe "
             "`ZbSend {\"Device\":\"0x%04X\",\"Read\":{\"BatteryPercentage\":true,\"BatteryVoltage\":true}}`"), shortaddr);
    }
  }
}

//
// Send messages to query the state of each Hue emulated light
//
int32_t Z_Query_Bulbs(uint8_t value) {
  if (!Settings->flag5.zb_disable_autoquery) {
    // Scan all devices and send deferred requests to know the state of bulbs
    uint32_t wait_ms = 1000;                  // start with 1.0 s delay
    for (uint32_t i = 0; i < zigbee_devices.devicesSize(); i++) {
      const Z_Device &device = zigbee_devices.devicesAt(i);
      Z_Query_Bulb(device.shortaddr, 0xFF, wait_ms);   // 0xFF means all endpoints
    }
  }
  return 0;                              // continue
}

//
// Z_ZbAutoload - autoload all definitions from filesystem
// files with ending '.zb' suffix
//
int32_t Z_ZbAutoload(uint8_t value) {
  ZbAutoload();
  return 0;
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
void ZCLFrame::autoResponder(const uint16_t *attr_list_ids, size_t attr_len) {
  Z_attribute_list attr_list;

  for (uint32_t i=0; i<attr_len; i++) {
    uint16_t attr_id = attr_list_ids[i];
    uint32_t ccccaaaa = (cluster << 16) | attr_id;
    Z_attribute attr;
    attr.setKeyId(cluster, attr_id);

    switch (ccccaaaa) {
      case 0x00000004: attr.setStr(PSTR(USE_ZIGBEE_MANUFACTURER));                break;    // Manufacturer
      case 0x00000005: attr.setStr(PSTR(USE_ZIGBEE_MODELID));                     break;    // ModelId
#ifdef USE_LIGHT
      case 0x00060000: attr.setUInt(Light.power ? 1 : 0);                         break;    // Power
      case 0x00080000: attr.setUInt(LightGetDimmer(0));                           break;    // Dimmer

      case 0x03000000:  // Hue
      case 0x03000001:  // Sat
      case 0x03000003:  // X
      case 0x03000004:  // Y
      case 0x03000007:  // CT
        {
          uint16_t hue;
          uint8_t  sat;
          float XY[2];
          LightGetHSB(&hue, &sat, nullptr);
          LightGetXY(&XY[0], &XY[1]);
          uint16_t uxy[2];
          for (uint32_t i = 0; i < nitems(XY); i++) {
            uxy[i] = XY[i] * 65536.0f;
            uxy[i] = (uxy[i] > 0xFEFF) ? uxy[i] : 0xFEFF;
          }
          if (0x0000 == attr_id) { attr.setUInt(changeUIntScale(hue, 0, 360, 0, 254)); }
          if (0x0001 == attr_id) { attr.setUInt(changeUIntScale(sat, 0, 255, 0, 254)); }
          if (0x0003 == attr_id) { attr.setUInt(uxy[0]); }
          if (0x0004 == attr_id) { attr.setUInt(uxy[1]); }
          if (0x0007 == attr_id) { attr.setUInt(LightGetColorTemp()); }
        }
        break;
#endif
      case 0x000A0000:    // Time
        attr.setUInt((Rtc.utc_time > START_VALID_TIME) ? Rtc.utc_time - 946684800 : Rtc.utc_time);
        break;
      case 0x000AFF00:    // TimeEpoch - Tasmota specific
        attr.setUInt(Rtc.utc_time);
        break;
      case 0x000A0001:    // TimeStatus
        attr.setUInt((Rtc.utc_time > START_VALID_TIME) ? 0x02 : 0x00);
        break;
      case 0x000A0002:    // TimeZone
        attr.setUInt(Rtc.time_timezone * 60);
        break;
      case 0x000A0007:    // LocalTime    // TODO take DST
        attr.setUInt(Rtc.time_timezone * 60 + ((Rtc.utc_time > START_VALID_TIME) ? Rtc.utc_time - 946684800 : Rtc.utc_time));
        break;
    }
    if (!attr.isNone()) {
      Z_parseAttributeKey(shortaddr, attr, cluster);
      attr_list.addAttribute(cluster, attr_id) = attr;
    }
  }

  SBuffer buf(200);
  for (const auto & attr : attr_list) {
    if (!ZbAppendWriteBuf(buf, attr, true)) {   // true = need status indicator in Read Attribute Responses
      return;   // error
    }
  }

  if (buf.len() > 0) {
    // we have a non-empty output

    // log first
    AddLog(LOG_LEVEL_INFO, PSTR("ZIG: Auto-responder: ZbSend {\"Device\":\"0x%04X\""
                                          ",\"Cluster\":\"0x%04X\""
                                          ",\"Endpoint\":%d"
                                          ",\"Response\":%s}"
                                          ),
                                          shortaddr, cluster, srcendpoint,
                                          attr_list.toString().c_str());

    // send
    // all good, send the packet
    ZCLFrame zcl(buf.len());   // message is 4 bytes
    zcl.shortaddr = shortaddr;
    zcl.cluster = cluster;
    zcl.dstendpoint = srcendpoint;
    zcl.cmd = ZCL_READ_ATTRIBUTES_RESPONSE;
    zcl.clusterSpecific = false;  /* not cluster specific */
    zcl.needResponse = false;     /* noresponse */
    zcl.direct = true;            /* direct response */
    if (localShortAddr == 0x0000) {
      zcl.direction = 1;          // if we are coordinator, then response is from client to server
    }
    zcl.setTransac(transactseq);
    zcl.payload.addBuffer(buf);
    zigbeeZCLSendCmd(zcl);
  }
}

#endif // USE_ZIGBEE
