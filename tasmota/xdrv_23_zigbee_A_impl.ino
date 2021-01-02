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

#define XDRV_23                    23

#include "UnishoxStrings.h"

const char kZbCommands[] PROGMEM = D_PRFX_ZB "|"    // prefix
#ifdef USE_ZIGBEE_ZNP
  D_CMND_ZIGBEEZNPSEND "|" D_CMND_ZIGBEEZNPRECEIVE "|"
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
  D_CMND_ZIGBEE_EZSP_SEND "|" D_CMND_ZIGBEE_EZSP_RECEIVE "|" D_CMND_ZIGBEE_EZSP_LISTEN "|"
#endif // USE_ZIGBEE_EZSP
  D_CMND_ZIGBEE_PERMITJOIN "|"
  D_CMND_ZIGBEE_STATUS "|" D_CMND_ZIGBEE_RESET "|" D_CMND_ZIGBEE_SEND "|" D_CMND_ZIGBEE_PROBE "|"
  D_CMND_ZIGBEE_INFO "|" D_CMND_ZIGBEE_FORGET "|" D_CMND_ZIGBEE_SAVE "|" D_CMND_ZIGBEE_NAME "|"
  D_CMND_ZIGBEE_BIND "|" D_CMND_ZIGBEE_UNBIND "|" D_CMND_ZIGBEE_PING "|" D_CMND_ZIGBEE_MODELID "|"
  D_CMND_ZIGBEE_LIGHT "|" D_CMND_ZIGBEE_OCCUPANCY "|"
  D_CMND_ZIGBEE_RESTORE "|" D_CMND_ZIGBEE_BIND_STATE "|" D_CMND_ZIGBEE_MAP "|" D_CMND_ZIGBEE_LEAVE "|"
  D_CMND_ZIGBEE_CONFIG "|" D_CMND_ZIGBEE_DATA
  ;

void (* const ZigbeeCommand[])(void) PROGMEM = {
#ifdef USE_ZIGBEE_ZNP
  &CmndZbZNPSend, &CmndZbZNPReceive,
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
  &CmndZbEZSPSend, &CmndZbEZSPReceive, &CmndZbEZSPListen,
#endif // USE_ZIGBEE_EZSP
  &CmndZbPermitJoin,
  &CmndZbStatus, &CmndZbReset, &CmndZbSend, &CmndZbProbe,
  &CmndZbInfo, &CmndZbForget, &CmndZbSave, &CmndZbName,
  &CmndZbBind, &CmndZbUnbind, &CmndZbPing, &CmndZbModelId,
  &CmndZbLight, &CmndZbOccupancy,
  &CmndZbRestore, &CmndZbBindState, &CmndZbMap, CmndZbLeave,
  &CmndZbConfig, CmndZbData,
  };

/********************************************************************************************/

// Initialize internal structures
void ZigbeeInit(void)
{
// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Winvalid-offsetof"
// Serial.printf(">>> offset %d %d %d\n", Z_offset(Z_Data_Light, dimmer), Z_offset(Z_Data_Light, x), Z_offset(Z_Data_Thermo, temperature));
// #pragma GCC diagnostic pop
  // Check if settings in Flash are set
  if (PinUsed(GPIO_ZIGBEE_RX) && PinUsed(GPIO_ZIGBEE_TX)) {
    if (0 == Settings.zb_channel) {
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE D_ZIGBEE_RANDOMIZING_ZBCONFIG));
      uint64_t mac64 = 0;     // stuff mac address into 64 bits
      WiFi.macAddress((uint8_t*) &mac64);
      uint32_t esp_id = ESP_getChipId();
#ifdef ESP8266
      uint32_t flash_id = ESP.getFlashChipId();
#endif  // ESP8266
#ifdef ESP32
      uint32_t flash_id = 0;
#endif  // ESP32

      uint16_t  pan_id = (mac64 & 0x3FFF);
      if (0x0000 == pan_id) { pan_id = 0x0001; }    // avoid extreme values
      if (0x3FFF == pan_id) { pan_id = 0x3FFE; }    // avoid extreme values
      Settings.zb_pan_id = pan_id;

      Settings.zb_ext_panid = 0xCCCCCCCC00000000L | (mac64 & 0x00000000FFFFFFFFL);
      Settings.zb_precfgkey_l = (mac64 << 32) | (esp_id << 16) | flash_id;
      Settings.zb_precfgkey_h = (mac64 << 32) | (esp_id << 16) | flash_id;
      Settings.zb_channel = USE_ZIGBEE_CHANNEL;
      Settings.zb_txradio_dbm = USE_ZIGBEE_TXRADIO_DBM;
    }

    if (Settings.zb_txradio_dbm < 0) {
      Settings.zb_txradio_dbm = -Settings.zb_txradio_dbm;
#ifdef USE_ZIGBEE_EZSP
      EZ_reset_config = true;         // force reconfigure of EZSP
#endif
      SettingsSave(2);
    }

#ifdef USE_ZIGBEE_EZSP
    // Check the I2C EEprom
    Wire.beginTransmission(USE_ZIGBEE_ZBBRIDGE_EEPROM);
    uint8_t error = Wire.endTransmission();
    if (0 == error) {
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE D_ZIGBEE_EEPROM_FOUND_AT_ADDRESS " 0x%02X"), USE_ZIGBEE_ZBBRIDGE_EEPROM);
      zigbee.eeprom_present = true;
    }
#endif
  }

  // update commands with the current settings
#ifdef USE_ZIGBEE_ZNP
  ZNP_UpdateConfig(Settings.zb_channel, Settings.zb_pan_id, Settings.zb_ext_panid, Settings.zb_precfgkey_l, Settings.zb_precfgkey_h);
#endif
#ifdef USE_ZIGBEE_EZSP
  EZ_UpdateConfig(Settings.zb_channel, Settings.zb_pan_id, Settings.zb_ext_panid, Settings.zb_precfgkey_l, Settings.zb_precfgkey_h, Settings.zb_txradio_dbm);
#endif

  ZigbeeInitSerial();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#ifdef USE_ZIGBEE_ZNP
// Do a factory reset of the CC2530
const unsigned char ZIGBEE_FACTORY_RESET[] PROGMEM =
  { Z_SREQ | Z_SAPI, SAPI_WRITE_CONFIGURATION, CONF_STARTUP_OPTION, 0x01 /* len */, 0x01 /* STARTOPT_CLEAR_CONFIG */};
//"2605030101";  // Z_SREQ | Z_SAPI, SAPI_WRITE_CONFIGURATION, CONF_STARTUP_OPTION, 0x01 len, 0x01 STARTOPT_CLEAR_CONFIG
#endif // USE_ZIGBEE_ZNP

void CmndZbReset(void) {
  if (ZigbeeSerial) {
    switch (XdrvMailbox.payload) {
    case 1:
#ifdef USE_ZIGBEE_ZNP
      ZigbeeZNPSend(ZIGBEE_FACTORY_RESET, sizeof(ZIGBEE_FACTORY_RESET));
#endif // USE_ZIGBEE_ZNP
      eraseZigbeeDevices();
      // no break - this is intended
    case 2:   // fall through
      Settings.zb_txradio_dbm = - abs(Settings.zb_txradio_dbm);
      TasmotaGlobal.restart_flag = 2;
#ifdef USE_ZIGBEE_ZNP
      ResponseCmndChar_P(PSTR(D_JSON_ZIGBEE_CC2530 " " D_JSON_RESET_AND_RESTARTING));
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
      ResponseCmndChar_P(PSTR(D_JSON_ZIGBEE_EZSP " " D_JSON_RESET_AND_RESTARTING));
#endif // USE_ZIGBEE_EZSP
      break;
    default:
      ResponseCmndChar_P(PSTR(D_ZIGBEE_RESET_1_OR_2));
    }
  }
}

/********************************************************************************************/
//
// High-level function
// Send a command specified as an HEX string for the workload.
// The target endpoint is computed if zero, i.e. sent to the first known endpoint of the device.
//
// Inputs:
// - shortaddr: 16-bits short address, or 0x0000 if group address
// - groupaddr: 16-bits group address, or 0x0000 if unicast using shortaddr
// - endpoint:  8-bits target endpoint (source is always 0x01), if 0x00, it will be guessed from ZbStatus information (basically the first endpoint of the device)
// - clusterSpecific: boolean, is the message general cluster or cluster specific, used to create the FC byte of ZCL
// - clusterIf: 16-bits cluster number
// - param:     pointer to HEX string for payload, should not be nullptr
// Returns: None
//
void zigbeeZCLSendStr(uint16_t shortaddr, uint16_t groupaddr, uint8_t endpoint, bool clusterSpecific, uint16_t manuf,
                       uint16_t cluster, uint8_t cmd, const char *param) {
  size_t size = param ? strlen(param) : 0;
  SBuffer buf((size+2)/2);    // actual bytes buffer for data

  if (param) {
    while (*param) {
      uint8_t code = parseHex_P(&param, 2);
      buf.add8(code);
    }
  }

  if ((0 == endpoint) && (BAD_SHORTADDR != shortaddr)) {
    // endpoint is not specified, let's try to find it from shortAddr, unless it's a group address
    endpoint = zigbee_devices.findFirstEndpoint(shortaddr);
    //AddLog_P(LOG_LEVEL_DEBUG, PSTR("ZbSend: guessing endpoint 0x%02X"), endpoint);
  }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("ZbSend: shortaddr 0x%04X, groupaddr 0x%04X, cluster 0x%04X, endpoint 0x%02X, cmd 0x%02X, data %s"),
    shortaddr, groupaddr, cluster, endpoint, cmd, param);

  if ((0 == endpoint) && (BAD_SHORTADDR != shortaddr)) {     // endpoint null is ok for group address
    AddLog_P(LOG_LEVEL_INFO, PSTR("ZbSend: unspecified endpoint"));
    return;
  }

  // everything is good, we can send the command

  uint8_t seq = zigbee_devices.getNextSeqNumber(shortaddr);
  ZigbeeZCLSend_Raw(ZigbeeZCLSendMessage({
    shortaddr,
    groupaddr,
    cluster /*cluster*/,
    endpoint,
    cmd,
    manuf,  /* manuf */
    clusterSpecific /* not cluster specific */,
    true /* response */,
    false /* discover route */,
    seq,  /* zcl transaction id */
    buf.getBuffer(), buf.len()
  }));
  // now set the timer, if any, to read back the state later
  if (clusterSpecific) {
    if (!Settings.flag5.zb_disable_autoquery) {
      // read back attribute value unless it is disabled
      sendHueUpdate(shortaddr, groupaddr, cluster, endpoint);
    }
  }
}

// Special encoding for multiplier:
// multiplier == 0: ignore
// multiplier == 1: ignore
// multiplier > 0: divide by the multiplier
// multiplier < 0: multiply by the -multiplier (positive)
void ZbApplyMultiplier(double &val_d, int8_t multiplier) {
  if ((0 != multiplier) && (1 != multiplier)) {
    if (multiplier > 0) {         // inverse of decoding
      val_d = val_d / multiplier;
    } else {
      val_d = val_d * (-multiplier);
    }
  }
}

//
// Write Tuya-Moes attribute
//
bool ZbTuyaWrite(SBuffer & buf, const Z_attribute & attr) {
  double val_d = attr.getFloat();
  const char * val_str = attr.getStr();

  if (attr.key_is_str) { return false; }    // couldn't find attr if so skip
  if (attr.isNum() && (1 != attr.attr_multiplier)) {
    ZbApplyMultiplier(val_d, attr.attr_multiplier);
  }
  uint32_t u32 = val_d;
  int32_t  i32 = val_d;

  uint8_t tuyatype = (attr.key.id.attr_id >> 8);
  uint8_t dpid = (attr.key.id.attr_id & 0xFF);
  buf.add8(tuyatype);
  buf.add8(dpid);

  // the next attribute is length 16 bits in big endian
  // high byte is always 0x00
  buf.add8(0);

  switch (tuyatype) {
    case 0x00:      // raw
      {
        SBuffer buf_raw = SBuffer::SBufferFromHex(val_str, strlen(val_str));
        if (buf_raw.len() > 255) { return false; }
        buf.add8(buf_raw.len());
        buf.addBuffer(buf_raw);
      }
      break;
    case 0x01:      // Boolean = uint8
    case 0x04:      // enum uint8
      buf.add8(1);
      buf.add8(u32);
      break;
    case 0x02:      // int32
      buf.add8(4);
      buf.add32BigEndian(i32);
      break;
    case 0x03:      // String
      {
        uint32_t s_len = strlen(val_str);
        if (s_len > 255) { return false; }
        buf.add8(s_len);
        buf.addBuffer(val_str, s_len);
      }
      break;
    case 0x05:      // bitmap 1/2/4 so we use 4 bytes
      buf.add8(4);
      buf.add32BigEndian(u32);
      break;
    default:
      return false;
  }
  return true;
}

//
// Send Attribute Write, apply mutlipliers before
//
bool ZbAppendWriteBuf(SBuffer & buf, const Z_attribute & attr, bool prepend_status_ok) {
  double val_d = attr.getFloat();
  const char * val_str = attr.getStr();

  if (attr.key_is_str) { return false; }    // couldn't find attr if so skip
  if (attr.isNum() && (1 != attr.attr_multiplier)) {
    ZbApplyMultiplier(val_d, attr.attr_multiplier);
  }

  // push the value in the buffer
  buf.add16(attr.key.id.attr_id);        // prepend with attribute identifier
  if (prepend_status_ok) {
    buf.add8(Z_SUCCESS);  // status OK = 0x00
  }
  buf.add8(attr.attr_type);     // prepend with attribute type
  int32_t res = encodeSingleAttribute(buf, val_d, val_str, attr.attr_type);
  if (res < 0) {
    // remove the attribute type we just added
    // buf.setLen(buf.len() - (operation == ZCL_READ_ATTRIBUTES_RESPONSE ? 4 : 3));
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE D_ZIGBEE_UNSUPPORTED_ATTRIBUTE_TYPE " %04X/%04X '0x%02X'"), attr.key.id.cluster, attr.key.id.attr_id, attr.attr_type);
    return false;
  }
  return true;
}

//
// Parse "Report", "Write", "Response" or "Config" attribute
// Operation is one of: ZCL_REPORT_ATTRIBUTES (0x0A), ZCL_WRITE_ATTRIBUTES (0x02) or ZCL_READ_ATTRIBUTES_RESPONSE (0x01)
//
void ZbSendReportWrite(class JsonParserToken val_pubwrite, class ZigbeeZCLSendMessage & packet) {
  SBuffer buf(200);       // buffer to store the binary output of attibutes

  if (nullptr == XdrvMailbox.command) {
    XdrvMailbox.command = (char*) "";             // prevent a crash when calling ReponseCmndChar and there was no previous command
  }

  bool tuya_protocol = zigbee_devices.isTuyaProtocol(packet.shortaddr, packet.endpoint);

  // iterate on keys
  for (auto key : val_pubwrite.getObject()) {
    JsonParserToken value = key.getValue();

    Z_attribute attr;
    attr.setKeyName(key.getStr());
    if (Z_parseAttributeKey(attr, tuya_protocol ? 0xEF00 : 0xFFFF)) {   // favor tuya protocol if needed
      // Buffer ready, do some sanity checks

      // all attributes must use the same cluster
      if (0xFFFF == packet.cluster) {
        packet.cluster = attr.key.id.cluster;       // set the cluster for this packet
      } else if (packet.cluster != attr.key.id.cluster) {
        ResponseCmndChar_P(PSTR(D_ZIGBEE_TOO_MANY_CLUSTERS));
        return;
      }

    } else {
      if (attr.key_is_str) {
        Response_P(PSTR("{\"%s\":\"%s'%s'\"}"), XdrvMailbox.command, PSTR(D_ZIGBEE_UNKNOWN_ATTRIBUTE " "), key);
        return;
      }
      if (Zunk == attr.attr_type) {
        Response_P(PSTR("{\"%s\":\"%s'%s'\"}"), XdrvMailbox.command, PSTR(D_ZIGBEE_UNSUPPORTED_ATTRIBUTE_TYPE " "), key);
        return;
      }
    }

    // copy value from input to attribute, in numerical or string format
    if (value.isStr()) {
      attr.setStr(value.getStr());
    } else if (value.isNum()) {
      attr.setFloat(value.getFloat());
    }

    double   val_d = 0;             // I try to avoid `double` but this type capture both float and (u)int32_t without prevision loss
    const char* val_str = "";       // variant as string
    ////////////////////////////////////////////////////////////////////////////////
    // Split encoding depending on message
    if (packet.cmd != ZCL_CONFIGURE_REPORTING) {
      if ((packet.cluster == 0XEF00) && (packet.cmd == ZCL_WRITE_ATTRIBUTES)) {
        // special case of Tuya / Moes / Lidl attributes
        if (buf.len() == 0) {
          // add the prefix to data
          buf.add8(0);      // status
          buf.add8(zigbee_devices.getNextSeqNumber(packet.shortaddr));
        }
        packet.clusterSpecific = true;
        packet.cmd = 0x00;
        if (!ZbTuyaWrite(buf, attr)) {
          return;   // error
        }
      } else if (!ZbAppendWriteBuf(buf, attr, packet.cmd == ZCL_READ_ATTRIBUTES_RESPONSE)) { // general case
        return;   // error
      }
    } else {
      // ////////////////////////////////////////////////////////////////////////////////
      // ZCL_CONFIGURE_REPORTING
      if (!value.isObject()) {
        ResponseCmndChar_P(PSTR(D_ZIGBEE_JSON_REQUIRED));
        return;
      }
      JsonParserObject attr_config = value.getObject();
      bool attr_direction = false;

      uint32_t dir = attr_config.getUInt(PSTR("DirectionReceived"), 0);
      if (dir) { attr_direction = true; }

      // read MinInterval and MaxInterval, default to 0xFFFF if not specified
      uint16_t attr_min_interval = attr_config.getUInt(PSTR("MinInterval"), 0xFFFF);
      uint16_t attr_max_interval = attr_config.getUInt(PSTR("MaxInterval"), 0xFFFF);

      // read ReportableChange
      JsonParserToken val_attr_rc = attr_config[PSTR("ReportableChange")];
      if (val_attr_rc) {
        val_d = val_attr_rc.getFloat();
        val_str = val_attr_rc.getStr();
        ZbApplyMultiplier(val_d, attr.attr_multiplier);
      }

      // read TimeoutPeriod
      uint16_t attr_timeout = attr_config.getUInt(PSTR("TimeoutPeriod"), 0x0000);

      bool attr_discrete = Z_isDiscreteDataType(attr.attr_type);

      // all fields are gathered, output the butes into the buffer, ZCL 2.5.7.1
      // common bytes
      buf.add8(attr_direction ? 0x01 : 0x00);
      buf.add16(attr.key.id.attr_id);
      if (attr_direction) {
        buf.add16(attr_timeout);
      } else {
        buf.add8(attr.attr_type);
        buf.add16(attr_min_interval);
        buf.add16(attr_max_interval);
        if (!attr_discrete) {
          int32_t res = encodeSingleAttribute(buf, val_d, val_str, attr.attr_type);
          if (res < 0) {
            Response_P(PSTR("{\"%s\":\"%s'%s' 0x%02X\"}"), XdrvMailbox.command, PSTR(D_ZIGBEE_UNSUPPORTED_ATTRIBUTE_TYPE " "), key, attr.attr_type);
            return;
          }
        }
      }
    }
  }

  // did we have any attribute?
  if (0 == buf.len()) {
    ResponseCmndChar_P(PSTR(D_ZIGBEE_NO_ATTRIBUTE));
    return;
  }

  // all good, send the packet
  packet.transacId = zigbee_devices.getNextSeqNumber(packet.shortaddr);
  packet.msg = buf.getBuffer();
  packet.len = buf.len();
  ZigbeeZCLSend_Raw(packet);
  ResponseCmndDone();
}

// Parse the "Send" attribute and send the command
void ZbSendSend(class JsonParserToken val_cmd, uint16_t device, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint16_t manuf) {
  uint8_t  cmd = 0;
  String   cmd_str = "";          // the actual low-level command, either specified or computed
  const char *cmd_s = "";                 // pointer to payload string
  bool     clusterSpecific = true;

  static char delim[] = ", ";     // delimiters for parameters
  // probe the type of the argument
  // If JSON object, it's high level commands
  // If String, it's a low level command
  if (val_cmd.isObject()) {
    // we have a high-level command
    JsonParserObject cmd_obj = val_cmd.getObject();
    int32_t cmd_size = cmd_obj.size();
    if (cmd_size > 1) {
      Response_P(PSTR(D_ZIGBEE_TOO_MANY_COMMANDS), cmd_size);
      return;
    } else if (1 == cmd_size) {
      // We have exactly 1 command, parse it
      JsonParserKey key = cmd_obj.getFirstElement();
      JsonParserToken value = key.getValue();
      uint32_t x = 0, y = 0, z = 0;
      uint16_t cmd_var;
      uint16_t local_cluster_id;

      const __FlashStringHelper* tasmota_cmd = zigbeeFindCommand(key.getStr(), &local_cluster_id, &cmd_var);
      if (tasmota_cmd) {
        cmd_str = tasmota_cmd;
      } else {
        Response_P(PSTR(D_ZIGBEE_UNRECOGNIZED_COMMAND), key.getStr());
        return;
      }
      // check cluster
      if (0xFFFF == cluster) {
        cluster = local_cluster_id;
      } else if (cluster != local_cluster_id) {
        ResponseCmndChar_P(PSTR(D_ZIGBEE_TOO_MANY_CLUSTERS));
        return;
      }

      // parse the JSON value, depending on its type fill in x,y,z
      if (value.isNum()) {
        x = value.getUInt();    // automatic conversion to 0/1
      // if (value.is<bool>()) {
      // //   x = value.as<bool>() ? 1 : 0;
      // } else if
      // } else if (value.is<unsigned int>()) {
      //   x = value.as<unsigned int>();
      } else {
        // if non-bool or non-int, trying char*
        const char *s_const = value.getStr(nullptr);
        // const char *s_const = value.as<const char*>();
        if (s_const != nullptr) {
          char s[strlen(s_const)+1];
          strcpy(s, s_const);
          if ((nullptr != s) && (0x00 != *s)) {     // ignore any null or empty string, could represent 'null' json value
            char *sval = strtok(s, delim);
            if (sval) {
              x = ZigbeeAliasOrNumber(sval);
              sval = strtok(nullptr, delim);
              if (sval) {
                y = ZigbeeAliasOrNumber(sval);
                sval = strtok(nullptr, delim);
                if (sval) {
                  z = ZigbeeAliasOrNumber(sval);
                }
              }
            }
          }
        }
      }

      //AddLog_P(LOG_LEVEL_DEBUG, PSTR("ZbSend: command_template = %s"), cmd_str.c_str());
      if (0xFF == cmd_var) {      // if command number is a variable, replace it with x
        cmd = x;
        x = y;                  // and shift other variables
        y = z;
      } else {
        cmd = cmd_var;          // or simply copy the cmd number
      }
      cmd_str = zigbeeCmdAddParams(cmd_str.c_str(), x, y, z);   // fill in parameters
      //AddLog_P(LOG_LEVEL_DEBUG, PSTR("ZbSend: command_final    = %s"), cmd_str.c_str());
      cmd_s = cmd_str.c_str();
    } else {
      // we have zero command, pass through until last error for missing command
    }
  } else if (val_cmd.isStr()) {
    // low-level command
    // Now parse the string to extract cluster, command, and payload
    // Parse 'cmd' in the form "AAAA_BB/CCCCCCCC" or "AAAA!BB/CCCCCCCC"
    // where AA is the cluster number, BBBB the command number, CCCC... the payload
    // First delimiter is '_' for a global command, or '!' for a cluster specific command
    const char * data = val_cmd.getStr();
    uint16_t local_cluster_id = parseHex(&data, 4);

    // check cluster
    if (0xFFFF == cluster) {
      cluster = local_cluster_id;
    } else if (cluster != local_cluster_id) {
      ResponseCmndChar_P(PSTR(D_ZIGBEE_TOO_MANY_CLUSTERS));
      return;
    }

    // delimiter
    if (('_' == *data) || ('!' == *data)) {
      if ('_' == *data) { clusterSpecific = false; }
      data++;
    } else {
      ResponseCmndChar_P(PSTR(D_ZIGBEE_WRONG_DELIMITER));
      return;
    }
    // parse cmd number
    cmd = parseHex(&data, 2);

    // move to end of payload
    // delimiter is optional
    if ('/' == *data) { data++; }   // skip delimiter

    cmd_s = data;
  } else {
    // we have an unsupported command type, just ignore it and fallback to missing command
  }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR("ZigbeeZCLSend device: 0x%04X, group: 0x%04X, endpoint:%d, cluster:0x%04X, cmd:0x%02X, send:\"%s\""),
            device, groupaddr, endpoint, cluster, cmd, cmd_s);
  zigbeeZCLSendStr(device, groupaddr, endpoint, clusterSpecific, manuf, cluster, cmd, cmd_s);
  ResponseCmndDone();
}


// Parse the "Send" attribute and send the command
void ZbSendRead(JsonParserToken val_attr, ZigbeeZCLSendMessage & packet) {
  // ZbSend {"Device":"0xF289","Cluster":0,"Endpoint":3,"Read":5}
  // ZbSend {"Device":"0xF289","Cluster":"0x0000","Endpoint":"0x0003","Read":"0x0005"}
  // ZbSend {"Device":"0xF289","Cluster":0,"Endpoint":3,"Read":[5,6,7,4]}
  // ZbSend {"Device":"0xF289","Endpoint":3,"Read":{"ModelId":true}}
  // ZbSend {"Device":"0xF289","Read":{"ModelId":true}}

  // ZbSend {"Device":"0xF289","ReadConig":{"Power":true}}
  // ZbSend {"Device":"0xF289","Cluster":6,"Endpoint":3,"ReadConfig":0}

  // params
  size_t   attrs_len = 0;
  uint8_t* attrs = nullptr;       // empty string is valid
  size_t   attr_item_len = 2;     // how many bytes per attribute, standard for "Read"
  size_t   attr_item_offset = 0;  // how many bytes do we offset to store attribute
  if (ZCL_READ_REPORTING_CONFIGURATION == packet.cmd) {
    attr_item_len = 3;
    attr_item_offset = 1;
  }

  if (val_attr.isArray()) {
    // value is an array []
    JsonParserArray attr_arr = val_attr.getArray();
    attrs_len = attr_arr.size() * attr_item_len;
    attrs = (uint8_t*) calloc(attrs_len, 1);

    uint32_t i = 0;
    for (auto value : attr_arr) {
      uint16_t val = value.getUInt();
      i += attr_item_offset;
      attrs[i++] = val & 0xFF;
      attrs[i++] = val >> 8;
      i += attr_item_len - 2 - attr_item_offset;    // normally 0
    }
  } else if (val_attr.isObject()) {
    // value is an object {}
    JsonParserObject attr_obj = val_attr.getObject();
    attrs_len = attr_obj.size() * attr_item_len;
    attrs = (uint8_t*) calloc(attrs_len, 1);
    uint32_t actual_attr_len = 0;

    // iterate on keys
    for (auto key : attr_obj) {
      JsonParserToken value = key.getValue();

      bool found = false;
      // scan attributes to find by name, and retrieve type
      for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
        const Z_AttributeConverter *converter = &Z_PostProcess[i];
        uint16_t local_attr_id = pgm_read_word(&converter->attribute);
        uint16_t local_cluster_id = CxToCluster(pgm_read_byte(&converter->cluster_short));
        // uint8_t  local_type_id = pgm_read_byte(&converter->type);

        if ((pgm_read_word(&converter->name_offset)) && (0 == strcasecmp_P(key.getStr(), Z_strings + pgm_read_word(&converter->name_offset)))) {
          // match name
          // check if there is a conflict with cluster
          // TODO
          if (!(value.getBool()) && attr_item_offset) {
            // If value is false (non-default) then set direction to 1 (for ReadConfig)
            attrs[actual_attr_len] = 0x01;
          }
          actual_attr_len += attr_item_offset;
          attrs[actual_attr_len++] = local_attr_id & 0xFF;
          attrs[actual_attr_len++] = local_attr_id >> 8;
          actual_attr_len += attr_item_len - 2 - attr_item_offset;    // normally 0
          found = true;
          // check cluster
          if (0xFFFF == packet.cluster) {
            packet.cluster = local_cluster_id;
          } else if (packet.cluster != local_cluster_id) {
            ResponseCmndChar_P(PSTR(D_ZIGBEE_TOO_MANY_CLUSTERS));
            if (attrs) { free(attrs); }
            return;
          }
          break;    // found, exit loop
        }
      }
      if (!found) {
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE D_ZIGBEE_UNKNWON_ATTRIBUTE), key.getStr());
      }
    }

    attrs_len = actual_attr_len;
  } else {
    // value is a literal
    if (0xFFFF != packet.cluster) {
      uint16_t val = val_attr.getUInt();
      attrs_len = attr_item_len;
      attrs = (uint8_t*) calloc(attrs_len, 1);
      attrs[0 + attr_item_offset] = val & 0xFF;    // little endian
      attrs[1 + attr_item_offset] = val >> 8;
    }
  }

  if (attrs_len > 0) {
    // all good, send the packet
    packet.transacId = zigbee_devices.getNextSeqNumber(packet.shortaddr);
    packet.msg = attrs;
    packet.len = attrs_len;
    ZigbeeZCLSend_Raw(packet);
    ResponseCmndDone();
  } else {
    ResponseCmndChar_P(PSTR(D_ZIGBEE_MISSING_PARAM));
  }

  if (attrs) { free(attrs); }
}

//
// Command `ZbSend`
//
// Examples:
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"0006/0000":0}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"Power":0}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"AqaraRotate":0}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"AqaraRotate":12.5}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"006/0000%39":12.5}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"AnalogInApplicationType":1000000}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"TimeZone":-1000000}}
// ZbSend {"Device":"0x0000","Endpoint":1,"Write":{"Manufacturer":"Tasmota","ModelId":"Tasmota Z2T Router"}}
void CmndZbSend(void) {
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":1} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":"3"} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":"0xFF"} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":null} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":false} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":true} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":"true"} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"ShutterClose":null} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Power":1} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Color":"1,2"} }
  // ZbSend { "device":"0x1234", "endpoint":"0x03", "send":{"Color":"0x1122,0xFFEE"} }
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  JsonParser parser(XdrvMailbox.data);
  JsonParserObject root = parser.getRootObject();
  if (!root) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

  // params
  uint16_t device = BAD_SHORTADDR;    // BAD_SHORTADDR is broadcast, so considered invalid
  uint16_t groupaddr = 0x0000;        // group address valid only if device == BAD_SHORTADDR
  uint16_t cluster = 0xFFFF;          // no default
  uint8_t  endpoint = 0x00;           // 0x00 is invalid for the dst endpoint
  uint16_t manuf = 0x0000;            // Manuf Id in ZCL frame


  // parse "Device" and "Group"
  JsonParserToken val_device = root[PSTR(D_CMND_ZIGBEE_DEVICE)];
  if (val_device) {
    device = zigbee_devices.parseDeviceFromName(val_device.getStr()).shortaddr;
    if (BAD_SHORTADDR == device) { ResponseCmndChar_P(PSTR(D_ZIGBEE_INVALID_PARAM)); return; }
  }
  if (BAD_SHORTADDR == device) {     // if not found, check if we have a group
    JsonParserToken val_group = root[PSTR(D_CMND_ZIGBEE_GROUP)];
    if (val_group) {
      groupaddr = val_group.getUInt();
    } else {                  // no device nor group
      ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE));
      return;
    }
  }
  // from here, either device has a device shortaddr, or if BAD_SHORTADDR then use group address
  // Note: groupaddr == 0 is valid

  // read other parameters
  cluster = root.getUInt(PSTR(D_CMND_ZIGBEE_CLUSTER), cluster);
  endpoint = root.getUInt(PSTR(D_CMND_ZIGBEE_ENDPOINT), endpoint);
  manuf = root.getUInt(PSTR(D_CMND_ZIGBEE_MANUF), manuf);

  // infer endpoint
  if (BAD_SHORTADDR == device) {
    endpoint = 0xFF;                  // endpoint not used for group addresses, so use a dummy broadcast endpoint
  } else if (0 == endpoint) {         // if it was not already specified, try to guess it
    endpoint = zigbee_devices.findFirstEndpoint(device);
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("ZIG: guessing endpoint %d"), endpoint);
  }
  if (0 == endpoint) {                // after this, if it is still zero, then it's an error
      ResponseCmndChar_P(PSTR("Missing endpoint"));
      return;
  }
  // from here endpoint is valid and non-zero
  // cluster may be already specified or 0xFFFF

  JsonParserToken val_cmd = root[PSTR(D_CMND_ZIGBEE_SEND)];
  JsonParserToken val_read = root[PSTR(D_CMND_ZIGBEE_READ)];
  JsonParserToken val_write = root[PSTR(D_CMND_ZIGBEE_WRITE)];
  JsonParserToken val_publish = root[PSTR(D_CMND_ZIGBEE_REPORT)];
  JsonParserToken val_response = root[PSTR(D_CMND_ZIGBEE_RESPONSE)];
  JsonParserToken val_read_config = root[PSTR(D_CMND_ZIGBEE_READ_CONFIG)];
  JsonParserToken val_config = root[PSTR(D_CMND_ZIGBEE_CONFIG)];
  uint32_t multi_cmd = ((bool)val_cmd) + ((bool)val_read) + ((bool)val_write) + ((bool)val_publish)
                     + ((bool)val_response) + ((bool)val_read_config) + ((bool)val_config);
  if (multi_cmd > 1) {
    ResponseCmndChar_P(PSTR("Can only have one of: 'Send', 'Read', 'Write', 'Report', 'Reponse', 'ReadConfig' or 'Config'"));
    return;
  }
  // from here we have one and only one command

  // collate information in a ready to send packet
  ZigbeeZCLSendMessage packet({
    device,
    groupaddr,
    cluster /*cluster*/,
    endpoint,
    ZCL_READ_ATTRIBUTES,
    manuf,  /* manuf */
    false /* not cluster specific */,
    false /* no response */,
    false /* discover route */,
    0,  /* zcl transaction id */
    nullptr, 0
  });

  if (val_cmd) {
    // "Send":{...commands...}
    // we accept either a string or a JSON object
    ZbSendSend(val_cmd, device, groupaddr, cluster, endpoint, manuf);
  } else if (val_read) {
    // "Read":{...attributes...}, "Read":attribute or "Read":[...attributes...]
    // we accept eitehr a number, a string, an array of numbers/strings, or a JSON object
    packet.cmd = ZCL_READ_ATTRIBUTES;
    ZbSendRead(val_read, packet);
  } else if (val_write) {
    // only KSON object
    if (!val_write.isObject()) {
      ResponseCmndChar_P(PSTR(D_ZIGBEE_MISSING_PARAM));
      return;
    }
    // "Write":{...attributes...}
    packet.cmd = ZCL_WRITE_ATTRIBUTES;
    ZbSendReportWrite(val_write, packet);
  } else if (val_publish) {
    // "Publish":{...attributes...}
    // only KSON object
    if (!val_publish.isObject()) {
      ResponseCmndChar_P(PSTR(D_ZIGBEE_MISSING_PARAM));
      return;
    }
    packet.cmd = ZCL_REPORT_ATTRIBUTES;
    ZbSendReportWrite(val_publish, packet);
  } else if (val_response) {
    // "Report":{...attributes...}
    // only KSON object
    if (!val_response.isObject()) {
      ResponseCmndChar_P(PSTR(D_ZIGBEE_MISSING_PARAM));
      return;
    }
    packet.cmd = ZCL_READ_ATTRIBUTES_RESPONSE;
    ZbSendReportWrite(val_response, packet);
  } else if (val_read_config) {
    // "ReadConfg":{...attributes...}, "ReadConfg":attribute or "ReadConfg":[...attributes...]
    // we accept eitehr a number, a string, an array of numbers/strings, or a JSON object
    packet.cmd = ZCL_READ_REPORTING_CONFIGURATION;
    ZbSendRead(val_read_config, packet);
  } else if (val_config) {
    // "Config":{...attributes...}
    // only JSON object
    if (!val_config.isObject()) {
      ResponseCmndChar_P(PSTR(D_ZIGBEE_MISSING_PARAM));
      return;
    }
    packet.cmd = ZCL_CONFIGURE_REPORTING;
    ZbSendReportWrite(val_config, packet);
  } else {
    Response_P(PSTR("Missing zigbee 'Send', 'Write', 'Report' or 'Response'"));
    return;
  }
}

//
// Command `ZbBind`
//
void ZbBindUnbind(bool unbind) {    // false = bind, true = unbind
  // ZbBind {"Device":"<device>", "Endpoint":<endpoint>, "Cluster":<cluster>, "ToDevice":"<to_device>", "ToEndpoint":<to_endpoint>, "ToGroup":<to_group> }
  // ZbUnbind {"Device":"<device>", "Endpoint":<endpoint>, "Cluster":<cluster>, "ToDevice":"<to_device>", "ToEndpoint":<to_endpoint>, "ToGroup":<to_group> }

  // local endpoint is always 1, IEEE addresses are calculated
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  JsonParser parser(XdrvMailbox.data);
  JsonParserObject root = parser.getRootObject();
  if (!root) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

  // params
  uint16_t dstDevice = BAD_SHORTADDR;      // BAD_SHORTADDR is broadcast, so considered invalid
  uint64_t dstLongAddr = 0;
  uint8_t  endpoint = 0x00;         // 0x00 is invalid for the src endpoint
  uint8_t  toendpoint = 0x01;       // default dest endpoint to 0x01
  uint16_t toGroup = 0x0000;        // group address
  uint16_t cluster  = 0;            // cluster 0 is default

  // Information about source device: "Device", "Endpoint", "Cluster"
  //  - the source endpoint must have a known IEEE address
  const Z_Device & src_device = zigbee_devices.parseDeviceFromName(root.getStr(PSTR(D_CMND_ZIGBEE_DEVICE), nullptr));
  if (!src_device.valid()) { ResponseCmndChar_P(PSTR("Unknown source device")); return; }
  // check if IEEE address is known
  uint64_t srcLongAddr = src_device.longaddr;
  if (0 == srcLongAddr) { ResponseCmndChar_P(PSTR("Unknown source IEEE address")); return; }
  // look for source endpoint
  endpoint = root.getUInt(PSTR(D_CMND_ZIGBEE_ENDPOINT), endpoint);
  if (0 == endpoint) { endpoint = zigbee_devices.findFirstEndpoint(src_device.shortaddr); }
  // look for source cluster
  JsonParserToken val_cluster = root[PSTR(D_CMND_ZIGBEE_CLUSTER)];
  if (val_cluster) {
    cluster = val_cluster.getUInt(cluster);   // first convert as number
    if (0 == cluster) {
      zigbeeFindAttributeByName(val_cluster.getStr(), &cluster, nullptr, nullptr);
    }
  }

  // Or Group Address - we don't need a dstEndpoint in this case
  JsonParserToken to_group = root[PSTR("ToGroup")];
  if (to_group) { toGroup = to_group.getUInt(toGroup); }

  // Either Device address
  // In this case the following parameters are mandatory
  //  - "ToDevice" and the device must have a known IEEE address
  //  - "ToEndpoint"
  JsonParserToken dst_device = root[PSTR("ToDevice")];

  // If no target is specified, we default to coordinator 0x0000
  if ((!to_group) && (!dst_device)) {
    dstDevice = 0x0000;
    dstLongAddr = localIEEEAddr;
    toendpoint = 1;
  }

  if (dst_device) {
    const Z_Device & dstDevice = zigbee_devices.parseDeviceFromName(dst_device.getStr(nullptr));
    if (!dstDevice.valid()) { ResponseCmndChar_P(PSTR("Unknown dest device")); return; }
    dstLongAddr = dstDevice.longaddr;
  }

  if (!to_group) {
    if (0 == dstLongAddr) { ResponseCmndChar_P(PSTR("Unknown dest IEEE address")); return; }
    toendpoint = root.getUInt(PSTR("ToEndpoint"), toendpoint);
  }

  // make sure we don't have conflicting parameters
  if (to_group && dst_device) { ResponseCmndChar_P(PSTR("Cannot have both \"ToDevice\" and \"ToGroup\"")); return; }

#ifdef USE_ZIGBEE_ZNP
  SBuffer buf(34);
  buf.add8(Z_SREQ | Z_ZDO);
  if (unbind) {
    buf.add8(ZDO_UNBIND_REQ);
  } else {
    buf.add8(ZDO_BIND_REQ);
  }
  buf.add16(src_device.shortaddr);
  buf.add64(srcLongAddr);
  buf.add8(endpoint);
  buf.add16(cluster);
  if (!to_group) {
    buf.add8(Z_Addr_IEEEAddress);         // DstAddrMode - 0x03 = ADDRESS_64_BIT
    buf.add64(dstLongAddr);
    buf.add8(toendpoint);
  } else {
    buf.add8(Z_Addr_Group);               // DstAddrMode - 0x01 = GROUP_ADDRESS
    buf.add16(toGroup);
  }

  ZigbeeZNPSend(buf.getBuffer(), buf.len());
#endif // USE_ZIGBEE_ZNP

#ifdef USE_ZIGBEE_EZSP
  SBuffer buf(24);

  // ZDO message payload (see Zigbee spec 2.4.3.2.2)
  buf.add64(srcLongAddr);
  buf.add8(endpoint);
  buf.add16(cluster);
  if (!to_group) {
    buf.add8(Z_Addr_IEEEAddress);         // DstAddrMode - 0x03 = ADDRESS_64_BIT
    buf.add64(dstLongAddr);
    buf.add8(toendpoint);
  } else {
    buf.add8(Z_Addr_Group);               // DstAddrMode - 0x01 = GROUP_ADDRESS
    buf.add16(toGroup);
  }

  EZ_SendZDO(src_device.shortaddr, unbind ? ZDO_UNBIND_REQ : ZDO_BIND_REQ, buf.buf(), buf.len());
#endif // USE_ZIGBEE_EZSP

  ResponseCmndDone();
}

//
// Command ZbBind
//
void CmndZbBind(void) {
  ZbBindUnbind(false);
}

//
// Command ZbBind
//
void CmndZbUnbind(void) {
  ZbBindUnbind(true);
}

//
// ZbLeave - ask for a device to leave the network
//
void CmndZbLeave(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  uint16_t shortaddr = zigbee_devices.parseDeviceFromName(XdrvMailbox.data).shortaddr;
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE)); return; }

#ifdef USE_ZIGBEE_ZNP
  SBuffer buf(14);
  buf.add8(Z_SREQ | Z_ZDO);             // 25
  buf.add8(ZDO_MGMT_LEAVE_REQ);         // 34
  buf.add16(shortaddr);                 // shortaddr
  buf.add64(0);                         // remove self
  buf.add8(0x00);                       // don't rejoin and don't remove children

  ZigbeeZNPSend(buf.getBuffer(), buf.len());
#endif // USE_ZIGBEE_ZNP


#ifdef USE_ZIGBEE_EZSP
  // ZDO message payload (see Zigbee spec 2.4.3.3.4)
  SBuffer buf(10);
  buf.add64(0);                         // remove self
  buf.add8(0x00);                       // don't rejoin and don't remove children

  EZ_SendZDO(shortaddr, ZDO_MGMT_LEAVE_REQ, buf.getBuffer(), buf.len());
#endif // USE_ZIGBEE_EZSP

  ResponseCmndDone();
}



void CmndZbBindState_or_Map(bool map) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  uint16_t parsed_shortaddr;;
  uint16_t shortaddr = zigbee_devices.parseDeviceFromName(XdrvMailbox.data, &parsed_shortaddr).shortaddr;
  if (BAD_SHORTADDR == shortaddr) {
    if ((map) && (parsed_shortaddr != shortaddr)) {
      shortaddr = parsed_shortaddr;   // allow a non-existent address when ZbMap
    } else {
      ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE));
      return;
    }
  }
  uint8_t index = XdrvMailbox.index - 1;   // change default 1 to 0
  uint16_t zdo_cmd;
#ifdef USE_ZIGBEE_ZNP
   zdo_cmd = map ? ZDO_MGMT_LQI_REQ : ZDO_MGMT_BIND_REQ;
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
  zdo_cmd = map ? ZDO_Mgmt_Lqi_req : ZDO_Mgmt_Bind_req;
#endif // USE_ZIGBEE_EZSP

  Z_Send_State_or_Map(shortaddr, index, zdo_cmd);
  ResponseCmndDone();
}

//
// Command `ZbBindState`
// `ZbBindState<x>` as index if it does not fit. If default, `1` starts at the beginning
//
void CmndZbBindState(void) {
  CmndZbBindState_or_Map(false);
}

void ZigbeeMapAllDevices(void) {
  // we can't abort a mapping in progress
  if (zigbee.mapping_in_progress) { return; }
  // defer sending ZbMap to each device
  zigbee_mapper.reset();    // clear all data in Zigbee mapper

  const static uint32_t DELAY_ZBMAP = 2000;   // wait for 1s between commands
  uint32_t wait_ms = DELAY_ZBMAP;
  zigbee.mapping_in_progress = true;          // mark mapping in progress

  zigbee_devices.setTimer(0x0000, 0, 0 /*wait_ms*/, 0, 0, Z_CAT_ALWAYS, 0 /* value = index */, &Z_Map);
  for (const auto & device : zigbee_devices.getDevices()) {
    zigbee_devices.setTimer(device.shortaddr, 0, wait_ms, 0, 0, Z_CAT_ALWAYS, 0 /* value = index */, &Z_Map);
    wait_ms += DELAY_ZBMAP;
  }
  wait_ms += DELAY_ZBMAP*2;
  zigbee_devices.setTimer(BAD_SHORTADDR, 0, wait_ms, 0, 0, Z_CAT_ALWAYS, 0 /* value = index */, &Z_Map);
  zigbee.mapping_end_time = wait_ms + millis();
}

//
// Command `ZbMap`
// `ZbMap<x>` as index if it does not fit. If default, `1` starts at the beginning
//
void CmndZbMap(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  RemoveSpace(XdrvMailbox.data);

  if (strlen(XdrvMailbox.data) == 0) {
    ZigbeeMapAllDevices();
    ResponseCmndDone();
  } else {
  CmndZbBindState_or_Map(true);
  }
}

// Probe a specific device to get its endpoints and supported clusters
void CmndZbProbe(void) {
  CmndZbProbeOrPing(true);
}

//
// Common code for `ZbProbe` and `ZbPing`
//
void CmndZbProbeOrPing(boolean probe) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  uint16_t shortaddr = zigbee_devices.parseDeviceFromName(XdrvMailbox.data).shortaddr;
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE)); return; }

  // set a timer for Reachable - 2s default value
  zigbee_devices.setTimer(shortaddr, 0, Z_CAT_REACHABILITY_TIMEOUT, 0, 0, Z_CAT_REACHABILITY, 0 /* value */, &Z_Unreachable);

  // everything is good, we can send the command
  Z_SendIEEEAddrReq(shortaddr);
  if (probe) {
    Z_SendActiveEpReq(shortaddr);
  }
  ResponseCmndDone();
}

// Ping a device, actually a simplified version of ZbProbe
void CmndZbPing(void) {
  CmndZbProbeOrPing(false);
}

//
// Command `ZbName`
// Specify, read or erase a Friendly Name
//
void CmndZbName(void) {
  // Syntax is:
  //  ZbName <device_id>,<friendlyname>  - assign a friendly name
  //  ZbName <device_id>                 - display the current friendly name
  //  ZbName <device_id>,                - remove friendly name
  //
  // Where <device_id> can be: short_addr, long_addr, device_index, friendly_name

  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }

  // check if parameters contain a comma ','
  char *p;
  strtok_r(XdrvMailbox.data, ",", &p);

  // parse first part, <device_id>
  Z_Device & device = zigbee_devices.parseDeviceFromName(XdrvMailbox.data);  // it's the only case where we create a new device
  if (!device.valid()) { ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE)); return; }

  if (p == nullptr) {
    const char * friendlyName = device.friendlyName;
    Response_P(PSTR("{\"0x%04X\":{\"" D_JSON_ZIGBEE_NAME "\":\"%s\"}}"), device.shortaddr, friendlyName ? friendlyName : "");
  } else {
    if (strlen(p) > 32) { p[32] = 0x00; }     // truncate to 32 chars max
    device.setFriendlyName(p);
    Response_P(PSTR("{\"0x%04X\":{\"" D_JSON_ZIGBEE_NAME "\":\"%s\"}}"), device.shortaddr, p);
  }
}

//
// Command `ZbName`
// Specify, read or erase a ModelId, only for debug purposes
//
void CmndZbModelId(void) {
  // Syntax is:
  //  ZbName <device_id>,<friendlyname>  - assign a friendly name
  //  ZbName <device_id>                 - display the current friendly name
  //  ZbName <device_id>,                - remove friendly name
  //
  // Where <device_id> can be: short_addr, long_addr, device_index, friendly_name

  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }

  // check if parameters contain a comma ','
  char *p;
  strtok_r(XdrvMailbox.data, ",", &p);

  // parse first part, <device_id>
  Z_Device & device = zigbee_devices.parseDeviceFromName(XdrvMailbox.data);  // in case of short_addr, it must be already registered
  if (!device.valid()) { ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE)); return; }

  if (p != nullptr) {
    device.setModelId(p);
  }
  const char * modelId = device.modelId;
  Response_P(PSTR("{\"0x%04X\":{\"" D_JSON_ZIGBEE_MODELID "\":\"%s\"}}"), device.shortaddr, modelId ? modelId : "");
}

//
// Command `ZbLight`
// Specify, read or erase a Light type for Hue/Alexa integration
void CmndZbLight(void) {
  // Syntax is:
  //  ZbLight <device_id>,<x>            - assign a bulb type 0-5
  //  ZbLight <device_id>                - display the current bulb type and status
  //
  // Where <device_id> can be: short_addr, long_addr, device_index, friendly_name

  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }

  // check if parameters contain a comma ','
  char *p;
  strtok_r(XdrvMailbox.data, ", ", &p);

  // parse first part, <device_id>
  Z_Device & device = zigbee_devices.parseDeviceFromName(XdrvMailbox.data);  // in case of short_addr, it must be already registered
  if (!device.valid()) { ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE)); return; }

  if (p) {
    int8_t bulbtype = strtol(p, nullptr, 10);
    if (bulbtype > 5)  { bulbtype = 5; }
    if (bulbtype < -1) { bulbtype = -1; }
    device.setLightChannels(bulbtype);
  }
  Z_attribute_list attr_list;
  device.jsonLightState(attr_list);

  device.jsonPublishAttrList(PSTR(D_PRFX_ZB D_CMND_ZIGBEE_LIGHT), attr_list);         // publish as ZbReceived

  ResponseCmndDone();
}
//
// Command `ZbOccupancy`
// Specify, read or erase the Occupancy detector configuration
void CmndZbOccupancy(void) {
  // Syntax is:
  //  ZbOccupancy <device_id>,<x>            - set the occupancy time-out
  //  ZbOccupancy <device_id>                - display the configuration
  //
  // List of occupancy time-outs:
  // 0xF = default (90 s)
  // 0x0 = no time-out
  // 0x1 = 15 s
  // 0x2 = 30 s
  // 0x3 = 45 s
  // 0x4 = 60 s
  // 0x5 = 75 s
  // 0x6 = 90 s -- default
  // 0x7 = 105 s
  // 0x8 = 120 s
  // Where <device_id> can be: short_addr, long_addr, device_index, friendly_name

  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }

  // check if parameters contain a comma ','
  char *p;
  strtok_r(XdrvMailbox.data, ", ", &p);

  // parse first part, <device_id>
  Z_Device & device = zigbee_devices.parseDeviceFromName(XdrvMailbox.data);  // in case of short_addr, it must be already registered
  if (!device.valid()) { ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE)); return; }

  int8_t occupancy_time = -1;
  if (p) {
    Z_Data_PIR & pir = (Z_Data_PIR&) device.data.getByType(Z_Data_Type::Z_PIR);
    occupancy_time = strtol(p, nullptr, 10);
    pir.setTimeoutSeconds(occupancy_time);
    zigbee_devices.dirty();
  } else {
    const Z_Data_PIR & pir_found = (const Z_Data_PIR&) device.data.find(Z_Data_Type::Z_PIR);
    if (&pir_found != nullptr) {
      occupancy_time = pir_found.getTimeoutSeconds();
    }
  }
  Response_P(PSTR("{\"" D_PRFX_ZB D_CMND_ZIGBEE_OCCUPANCY "\":%d}"), occupancy_time);

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_ZB D_CMND_ZIGBEE_LIGHT));
  ResponseCmndDone();
}

//
// Command `ZbForget`
// Remove an old Zigbee device from the list of known devices, use ZigbeeStatus to know all registered devices
//
void CmndZbForget(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  Z_Device & device = zigbee_devices.parseDeviceFromName(XdrvMailbox.data);  // in case of short_addr, it must be already registered
  if (!device.valid()) { ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE)); return; }

  // everything is good, we can send the command
  if (zigbee_devices.removeDevice(device.shortaddr)) {
    ResponseCmndDone();
  } else {
    ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE));
  }
}

//
// Command `ZbInfo`
// Display all information known about a device, this equivalent to `2bStatus3` with a simpler JSON output
//
void CmndZbInfo_inner(const Z_Device & device) {
    Z_attribute_list attr_list;
    device.jsonDumpSingleDevice(attr_list, 3, false);   // don't add Device/Name
    device.jsonPublishAttrList(PSTR(D_JSON_ZIGBEE_INFO), attr_list);         // publish as ZbReceived
}
void CmndZbInfo(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  RemoveSpace(XdrvMailbox.data);

  if (strlen(XdrvMailbox.data) == 0) {
    // if empty, dump for all values
    for (const auto & device : zigbee_devices.getDevices()) {
      CmndZbInfo_inner(device);
    }
  } else {    // try JSON
    Z_Device & device = zigbee_devices.parseDeviceFromName(XdrvMailbox.data);  // in case of short_addr, it must be already registered
    if (!device.valid()) { ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE)); return; }

    // everything is good, we can send the command

    Z_attribute_list attr_list;
    device.jsonDumpSingleDevice(attr_list, 3, false);   // don't add Device/Name
    device.jsonPublishAttrList(PSTR(D_JSON_ZIGBEE_INFO), attr_list);         // publish as ZbReceived
  }

  ResponseCmndDone();
}

//
// Command `ZbSave`
// Save Zigbee information to flash
//
void CmndZbSave(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  switch (XdrvMailbox.payload) {
    case 2:       // save only data
      hibernateAllData();
      break;
    case -1:      // dump configuration
      loadZigbeeDevices(true);    // dump only
      break;
    case -2:
      hydrateDevicesDataFromEEPROM();
      break;
#ifdef Z_EEPROM_DEBUG
    case -10:
      { // reinit EEPROM
      ZFS::erase();
      }
      break;
#endif
    default:
      saveZigbeeDevices();
      break;
  }
  ResponseCmndDone();
}


// Restore a device configuration previously exported via `ZbStatus2``
// Format:
// Either the entire `ZbStatus3` export, or an array or just the device configuration.
// If array, if can contain multiple devices
//   ZbRestore {"ZbStatus3":[{"Device":"0x5ADF","Name":"Petite_Lampe","IEEEAddr":"0x90FD9FFFFE03B051","ModelId":"TRADFRI bulb E27 WS opal 980lm","Manufacturer":"IKEA of Sweden","Endpoints":["0x01","0xF2"]}]}
//   ZbRestore [{"Device":"0x5ADF","Name":"Petite_Lampe","IEEEAddr":"0x90FD9FFFFE03B051","ModelId":"TRADFRI bulb E27 WS opal 980lm","Manufacturer":"IKEA of Sweden","Endpoints":["0x01","0xF2"]}]
//   ZbRestore {"Device":"0x5ADF","Name":"Petite_Lampe","IEEEAddr":"0x90FD9FFFFE03B051","ModelId":"TRADFRI bulb E27 WS opal 980lm","Manufacturer":"IKEA of Sweden","Endpoints":["0x01","0xF2"]}
void CmndZbRestore(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  RemoveSpace(XdrvMailbox.data);

  if (strlen(XdrvMailbox.data) == 0) {
    // if empty, log values for all devices
    restoreDumpAllDevices();
  } else if (XdrvMailbox.data[0] == '{') {    // try JSON
    JsonParser parser(XdrvMailbox.data);
    JsonParserToken root = parser.getRoot();

    if (!parser || !(root.isObject() || root.isArray())) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

    // Check is root contains `ZbStatus<x>` key, if so change the root
    JsonParserToken zbstatus = root.getObject().findStartsWith(PSTR("ZbStatus"));
    if (zbstatus) {
      root = zbstatus;
    }

    // check if the root is an array
    if (root.isArray()) {
      JsonParserArray arr = JsonParserArray(root);
      for (const auto elt : arr) {
        // call restore on each item
        if (elt.isObject()) {
          int32_t res = zigbee_devices.deviceRestore(JsonParserObject(elt));
          if (res < 0) {
            ResponseCmndChar_P(PSTR("Restore failed"));
            return;
          }
        }
      }
    } else if (root.isObject()) {
      int32_t res = zigbee_devices.deviceRestore(JsonParserObject(root));
      if (res < 0) {
        ResponseCmndChar_P(PSTR("Restore failed"));
        return;
      }
      // call restore on a single object
    } else {
      ResponseCmndChar_P(PSTR(D_ZIGBEE_MISSING_PARAM));
      return;
    }
  } else {  // try hex
    SBuffer buf = SBuffer::SBufferFromHex(XdrvMailbox.data, strlen(XdrvMailbox.data));
    // do a sanity check, the first byte must equal the length of the buffer
    if (buf.get8(0) == buf.len()) {
      // good, we can hydrate
      hydrateSingleDevice(buf);
    } else {
      ResponseCmndChar_P(PSTR("Restore failed"));
      return;
    }
  }
  ResponseCmndDone();
}

//
// Command `ZbPermitJoin`
// Allow or Deny pairing of new Zigbee devices
//
void CmndZbPermitJoin(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }

  uint32_t payload = XdrvMailbox.payload;
  uint8_t  duration = 60;               // default 60s

  if (payload <= 0) {
    duration = 0;
  }

// ZNP Version
#ifdef USE_ZIGBEE_ZNP
  if (99 == payload) {
    duration = 0xFF;                    // unlimited time
  }

  SBuffer buf(34);
  buf.add8(Z_SREQ | Z_ZDO);             // 25
  buf.add8(ZDO_MGMT_PERMIT_JOIN_REQ);   // 36
  buf.add8(0x0F);                       // AddrMode
  buf.add16(0xFFFC);                    // DstAddr
  buf.add8(duration);
  buf.add8(0x00);                       // TCSignificance

  ZigbeeZNPSend(buf.getBuffer(), buf.len());

#endif // USE_ZIGBEE_ZNP

// EZSP VERSION
#ifdef USE_ZIGBEE_EZSP
  if (99 == payload) {
    ResponseCmndChar_P(PSTR("Unlimited time not supported")); return;
  }

  SBuffer buf(3);
  buf.add16(EZSP_permitJoining);
  buf.add8(duration);
  ZigbeeEZSPSendCmd(buf.getBuffer(), buf.len());

  // send ZDO_Mgmt_Permit_Joining_req to all routers
  buf.setLen(0);
  buf.add8(duration);
  buf.add8(0x01);       // TC_Significance - This field shall always have a value of 1, indicating a request to change the Trust Center policy. If a frame is received with a value of 0, it shall be treated as having a value of 1.
  EZ_SendZDO(0xFFFC, ZDO_Mgmt_Permit_Joining_req, buf.buf(), buf.len());

  // Set Timer after the end of the period, and reset a non-expired previous timer
  if (duration > 0) {
    // Log pairing mode enabled
    Response_P(PSTR("{\"" D_JSON_ZIGBEE_STATE "\":{\"Status\":21,\"Message\":\"Pairing mode enabled\"}}"));
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEE_STATE));
    zigbee.permit_end_time = millis() + duration * 1000;
  } else {
    zigbee.permit_end_time = millis();
  }
#endif // USE_ZIGBEE_EZSP

  ResponseCmndDone();
}

#ifdef USE_ZIGBEE_EZSP
//
// `ZbListen`: add a multicast group to listen to
// Overcomes a current limitation that EZSP only shows messages from multicast groups it listens too
//
// Ex: `ZbListen 99`, `ZbListen2 100`
void CmndZbEZSPListen(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }

  int32_t  index = XdrvMailbox.index;   // 0 is reserved for group 0 (auto-config)
  int32_t  group = XdrvMailbox.payload;

  if (group <= 0) {
    group = 0;
  } else if (group > 0xFFFF) {
    group = 0xFFFF;
  }

  SBuffer buf(8);
  buf.add16(EZSP_setMulticastTableEntry);
  buf.add8(index);
  buf.add16(group);   // group
  buf.add8(0x01);       // endpoint
  buf.add8(0x00);       // network index
  ZigbeeEZSPSendCmd(buf.getBuffer(), buf.len());

  ResponseCmndDone();
}

void ZigbeeGlowPermitJoinLight(void) {
  static const uint16_t cycle_time = 1000;    // cycle up and down in 1000 ms
  static const uint16_t half_cycle_time = cycle_time / 2;    // cycle up and down in 1000 ms
  if (zigbee.permit_end_time) {
    uint16_t led_power = 0;         // turn led off
    // permit join is ongoing
    if (TimeReached(zigbee.permit_end_time)) {
      zigbee.permit_end_time = 0;   // disable timer
      Z_PermitJoinDisable();
    } else {
      uint32_t millis_to_go = millis() - zigbee.permit_end_time;
      uint32_t sub_second = millis_to_go % cycle_time;
      if (sub_second <= half_cycle_time) {
        led_power = changeUIntScale(sub_second, 0, half_cycle_time, 0, 1023);
      } else {
        led_power = changeUIntScale(sub_second, half_cycle_time, cycle_time, 1023, 0);
      }
      led_power = ledGamma10_10(led_power);
    }

    // change the led state
    uint32_t led_pin = Pin(GPIO_LEDLNK);
    if (led_pin < 99) {
      analogWrite(led_pin, TasmotaGlobal.ledlnk_inverted ? 1023 - led_power : led_power);
    }
  }
}
#endif // USE_ZIGBEE_EZSP

//
// Command `ZbStatus`
//
void CmndZbStatus(void) {
  if (ZigbeeSerial) {
    if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
    String dump;

    if (0 == XdrvMailbox.index) {
      dump = zigbee_devices.dumpCoordinator();
    } else {
      Z_Device & device = zigbee_devices.parseDeviceFromName(XdrvMailbox.data);
      if (XdrvMailbox.data_len > 0) {
        if (!device.valid()) { ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE)); return; }
        dump = zigbee_devices.dumpDevice(XdrvMailbox.index, device);
      } else {
        if (XdrvMailbox.index >= 2) { ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE)); return; }
        dump = zigbee_devices.dumpDevice(XdrvMailbox.index, *(Z_Device*)nullptr);
      }
    }

    Response_P(PSTR("{\"%s%d\":%s}"), XdrvMailbox.command, XdrvMailbox.index, dump.c_str());
  }
}

//
// Command `ZbData`
//
void CmndZbData(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  RemoveSpace(XdrvMailbox.data);

  if (strlen(XdrvMailbox.data) == 0) {
    // if empty, log values for all devices
    for (const auto & device : zigbee_devices.getDevices()) {
      hibernateDeviceData(device, true);    // simple log, no mqtt
    }
  } else {
    // check if parameters contain a comma ','
    char *p;
    strtok_r(XdrvMailbox.data, ",", &p);

    // parse first part, <device_id>
    Z_Device & device = zigbee_devices.parseDeviceFromName(XdrvMailbox.data);  // in case of short_addr, it must be already registered
    if (!device.valid()) { ResponseCmndChar_P(PSTR(D_ZIGBEE_UNKNOWN_DEVICE)); return; }

    if (p) {
      // set ZbData
      const SBuffer buf = SBuffer::SBufferFromHex(p, strlen(p));
      hydrateDeviceData(device, buf, 0, buf.len());
    } else {
      // non-JSON, export current data
      // ZbData 0x1234
      // ZbData Device_Name
      hibernateDeviceData(device, true);    // mqtt
    }
  }

  ResponseCmndDone();
}

//
// Command `ZbConfig`
//
void CmndZbConfig(void) {
  // ZbConfig
  // ZbConfig {"Channel":11,"PanID":"0x1A63","ExtPanID":"0xCCCCCCCCCCCCCCCC","KeyL":"0x0F0D0B0907050301L","KeyH":"0x0D0C0A0806040200L"}
  uint8_t     zb_channel     = Settings.zb_channel;
  uint16_t    zb_pan_id      = Settings.zb_pan_id;
  uint64_t    zb_ext_panid   = Settings.zb_ext_panid;
  uint64_t    zb_precfgkey_l = Settings.zb_precfgkey_l;
  uint64_t    zb_precfgkey_h = Settings.zb_precfgkey_h;
  int8_t      zb_txradio_dbm = Settings.zb_txradio_dbm;

  // if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  RemoveSpace(XdrvMailbox.data);
  if (strlen(XdrvMailbox.data) > 0) {
    JsonParser parser(XdrvMailbox.data);
    JsonParserObject root = parser.getRootObject();
    if (!root) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }
    // Channel

    zb_channel      = root.getUInt(PSTR("Channel"), zb_channel);
    zb_pan_id       = root.getUInt(PSTR("PanID"), zb_pan_id);
    zb_ext_panid    = root.getULong(PSTR("ExtPanID"), zb_ext_panid);
    zb_precfgkey_l  = root.getULong(PSTR("KeyL"), zb_precfgkey_l);
    zb_precfgkey_h  = root.getULong(PSTR("KeyH"), zb_precfgkey_h);
    zb_txradio_dbm  = root.getInt(PSTR("TxRadio"), zb_txradio_dbm);

    if (zb_channel < 11) { zb_channel = 11; }
    if (zb_channel > 26) { zb_channel = 26; }
    // if network key is zero, we generate a truly random key with a hardware generator from ESP
    if ((0 == zb_precfgkey_l) && (0 == zb_precfgkey_h)) {
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE D_ZIGBEE_GENERATE_KEY));
      zb_precfgkey_l = (uint64_t)HwRandom() << 32 | HwRandom();
      zb_precfgkey_h = (uint64_t)HwRandom() << 32 | HwRandom();
    }

    // Check if a parameter was changed after all
    if ( (zb_channel      != Settings.zb_channel) ||
         (zb_pan_id       != Settings.zb_pan_id) ||
         (zb_ext_panid    != Settings.zb_ext_panid) ||
         (zb_precfgkey_l  != Settings.zb_precfgkey_l) ||
         (zb_precfgkey_h  != Settings.zb_precfgkey_h) ||
         (zb_txradio_dbm  != Settings.zb_txradio_dbm) ) {
      Settings.zb_channel      = zb_channel;
      Settings.zb_pan_id       = zb_pan_id;
      Settings.zb_ext_panid    = zb_ext_panid;
      Settings.zb_precfgkey_l  = zb_precfgkey_l;
      Settings.zb_precfgkey_h  = zb_precfgkey_h;
      Settings.zb_txradio_dbm  = zb_txradio_dbm;
      TasmotaGlobal.restart_flag = 2;    // save and reboot
    }
  }

  // display the current or new configuration
  char hex_ext_panid[20] = "0x";
  Uint64toHex(zb_ext_panid, &hex_ext_panid[2], 64);
  char hex_precfgkey_l[20] = "0x";
  Uint64toHex(zb_precfgkey_l, &hex_precfgkey_l[2], 64);
  char hex_precfgkey_h[20] = "0x";
  Uint64toHex(zb_precfgkey_h, &hex_precfgkey_h[2], 64);

  // {"ZbConfig":{"Channel":11,"PanID":"0x1A63","ExtPanID":"0xCCCCCCCCCCCCCCCC","KeyL":"0x0F0D0B0907050301L","KeyH":"0x0D0C0A0806040200L"}}
  Response_P(PSTR("{\"" D_PRFX_ZB D_JSON_ZIGBEE_CONFIG "\":{"
                  "\"Channel\":%d"
                  ",\"PanID\":\"0x%04X\""
                  ",\"ExtPanID\":\"%s\""
                  ",\"KeyL\":\"%s\""
                  ",\"KeyH\":\"%s\""
                  ",\"TxRadio\":%d"
                  "}}"),
                  zb_channel, zb_pan_id,
                  hex_ext_panid,
                  hex_precfgkey_l, hex_precfgkey_h,
                  zb_txradio_dbm);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/


const char ZB_WEB_U[] PROGMEM =
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // index 0
    //=ZB_WEB_CSS
    "</table>{t}"         // Terminate current two column table and open new table
    "<style>"
    // Table CSS
    ".ztd td:not(:first-child){width:20px;font-size:70%%}"
    ".ztd td:last-child{width:45px}"
    ".ztd .bt{margin-right:10px;}" // Margin right should be half of the not-first width
    ".htr{line-height:20px}"
    // Lighting
    ".bx{height:14px;width:14px;display:inline-block;border:1px solid currentColor;background-color:var(--cl,#fff)}"
    // Signal Strength Indicator
    ".si{display:inline-flex;align-items:flex-end;height:15px;padding:0}"
    ".si i{width:3px;margin-right:1px;border-radius:3px;background-color:#%06x}"
    ".si .b0{height:25%%}.si .b1{height:50%%}.si .b2{height:75%%}.si .b3{height:100%%}.o30{opacity:.3}"
    "</style>"

    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // index 1
    // Visual indicator for PermitJoin Active
    //=ZB_WEB_PERMITJOIN_ACTIVE
    "<p><b>[ <span style='color:#080;'>%s</span> ]</b></p>"

    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // index 2
    // Start of vis.js box
    //=ZB_WEB_VIS_JS_BEFORE
    "<script type=\"text/javascript\" src=\"https://unpkg.com/vis-network/standalone/umd/vis-network.min.js\"></script>"
    "<div id=\"mynetwork\" style=\"background-color:#fff;color:#000;width:800px;height:400px;border:1px solid lightgray;resize:both;\">Unable to load vis.js</div>"
    "<script type=\"text/javascript\">"
    "var container=document.getElementById(\"mynetwork\");"
    "var options={groups:{o:{shape:\"circle\",color:\"#d55\"},r:{shape:\"box\",color:\"#fb7\"},e:{shape:\"ellipse\",color:\"#adf\"}}};"
    "var data={"

    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // index 3
    // End of vis.js box
    //=ZB_WEB_VIS_JS_AFTER
    "};"
    "var network=new vis.Network(container,data,options);</script>"
    // "<p></p><form action='zbr' method='get'><button>Zigbee Map Refresh</button></form>"

    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // index 4
    // Auto-refresh
    //=ZB_WEB_AUTO_REFRESH
    "<script>setTimeout(function(){location.reload();},1990);</script>"

    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // index 5
    // Auto-refresh
    //=ZB_WEB_MAP_REFRESH
    "<p></p><form action='zbr' method='get'><button>%s</button></form>"

    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // index 6
    // Style
    //=ZB_WEB_STATUS_LINE
    "<tr class='ztd htr'>"
    "<td><b title='0x%04X %s - %s'>%s</b></td>" // name
    "<td>%s</td>" // sbatt (Battery Indicator)
    "<td><div title='LQI %s' class='si'>" // slqi
    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //=ZB_WEB_BATTERY
    "<i class=\"bt\" title=\"%d%%\" style=\"--bl:%dpx\"></i>"
    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //=ZB_WEB_LAST_SEEN
    "<td style=\"color:#%02x%02x%02x\">&#x1F557;%02d%c"
    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //=ZB_WEB_COLOR_RGB
    " <i class=\"bx\" style=\"--cl:#%02X%02X%02X\"></i>#%02X%02X%02X"
    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //=ZB_WEB_LINE_START
    "<tr class='htr'><td colspan=\"4\">&#9478;"
    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //=ZB_WEB_LIGHT_CT
    " <span title=\"CT %d\"><small>&#9898; </small>%dK</span>"
    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //=ZB_WEB_END_STATUS
    "</div></td>" // Close LQI
    "%s{e}" // dhm (Last Seen)
    "\0"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //=ZB_WEB_LINE_END
    "</div></td>" // Close LQI
    "%s{e}" // dhm (Last Seen)
    "\0"
    ;   // end of list

// Use the tool at https://tasmota.hadinger.fr/util and choose "Compress Strings template with Unishox"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++ DO NOT EDIT BELOW ++++++++++++++++++++
// ++++++++++++++++++++vvvvvvvvvvvvvvvvvvv++++++++++++++++++++
enum {
  ZB_WEB_CSS=0,
  ZB_WEB_PERMITJOIN_ACTIVE=507,
  ZB_WEB_VIS_JS_BEFORE=561,
  ZB_WEB_VIS_JS_AFTER=1034,
  ZB_WEB_AUTO_REFRESH=1098,
  ZB_WEB_MAP_REFRESH=1164,
  ZB_WEB_STATUS_LINE=1230,
  ZB_WEB_BATTERY=1338,
  ZB_WEB_LAST_SEEN=1388,
  ZB_WEB_COLOR_RGB=1436,
  ZB_WEB_LINE_START=1496,
  ZB_WEB_LIGHT_CT=1536,
  ZB_WEB_END_STATUS=1591,
  ZB_WEB_LINE_END=1608,
};

// Compressed from 1625 to 1111, -31.6%
const char ZB_WEB[] PROGMEM = "\x00\x66\x3D\x0E\xCA\xB1\xC1\x33\xF0\xF6\xD1\xEE\x3D\x3D\x46\x41\x33\xF0\xE8\x6D"
                             "\xA1\x15\x08\x79\xF6\x51\xDD\x3C\xCC\x6F\xFD\x47\x58\x62\xB4\x21\x0E\xF1\xED\x1F"
                             "\xD1\x28\x51\xE6\x72\x99\x0C\x36\x1E\x0C\x67\x51\xD7\xED\x36\xB3\xCC\xE7\x99\xF4"
                             "\x7D\x1E\xE2\x04\x3C\x40\x2B\x04\x3C\x28\x10\xB0\x93\x99\xA4\x30\xD8\x08\x36\x8E"
                             "\x83\xA8\xF6\x8D\xBF\x8F\x6F\x1D\x7F\xD1\xE1\x54\x79\x9C\x8C\x86\x1B\x0F\x07\xB8"
                             "\xE8\x2A\x2B\xBE\x7B\x42\xDE\x67\x58\xA7\xA3\xC2\xA8\xF3\x39\x4C\x86\x1B\x0F\x71"
                             "\xD0\x71\xB0\xF6\x82\x14\xC3\x93\x08\x61\xB0\xF0\x08\x39\x49\xC9\x84\x30\xD8\x78"
                             "\x13\x7C\x30\x2B\x32\x3C\xF7\x82\xDE\x67\x58\xE0\xB0\x33\x43\xC0\xEC\xF8\x8F\xE7"
                             "\x99\xC8\x43\x0D\x8B\xD8\x16\x88\x83\x17\xFF\xBE\xA2\x0F\x02\xCF\x9E\x07\x58\x66"
                             "\x83\xDF\xC1\x7C\x21\xD6\x1E\x05\x9F\x3C\xCC\xEF\xE7\x74\xEB\x3A\xC3\x08\xEA\x3C"
                             "\x8C\x18\x30\x77\x8F\x71\xD3\xDA\x7B\x41\x2B\x33\x30\x13\x36\x1E\x2C\x2D\x1E\xE3"
                             "\xAF\x69\x8D\xF1\xE6\x60\x26\x6C\x3A\xDF\x08\x78\x04\x3D\xCC\xE6\x90\xC3\x61\xE0"
                             "\x65\x88\x26\xF0\xF1\xE6\x71\x9E\xE3\xA1\x7B\x56\x82\x17\x0A\x07\x2C\x86\x1B\x0F"
                             "\x2A\x01\x93\xC2\x30\xC3\x60\x21\x6F\xC7\x5F\xEC\x4D\x17\xE3\xCC\xE5\x90\xC3\x60"
                             "\x26\xEE\x47\x91\xF4\x71\xF1\x1B\x0F\x71\xD3\xDA\x8E\x83\x8E\x32\x04\x3E\x16\xCE"
                             "\x56\x9F\x47\xD1\x02\x15\x03\x90\x81\x0E\x81\xCD\x64\x08\x94\x0E\x51\x02\x1D\x03"
                             "\x9E\x20\x45\xC1\x0E\x59\x02\x27\x12\xE7\x1B\x3E\x8F\xA3\xDC\x74\x2C\x39\x6C\xF6"
                             "\x96\x0C\xB0\xF6\x8C\x8F\x33\xA1\xCB\x3D\xC7\xA1\xD8\x40\x83\xCA\x24\xE1\x7C\xF4"
                             "\x18\x7E\x1E\x83\x8F\xC3\xDE\x47\xA7\x86\x5F\x2F\x51\x90\x4C\xF8\x7D\x82\x16\xCE"
                             "\x71\xFD\x9E\x0F\xB3\xF0\xFA\x2F\x1E\x87\x67\x86\x5F\x1F\x88\xF7\xCF\x43\xB0\x71"
                             "\xF8\x7A\x1D\x83\x0F\xC9\xC2\xF9\xE9\xE0\xFF\xA3\x29\x51\x90\xC6\x7C\x3D\x94\xCD"
                             "\x94\x76\x1A\xEC\xCE\xC1\x06\x91\xEC\x5E\xF8\x67\xC3\xD8\x2A\x2B\xA8\x67\x8F\x33"
                             "\xB0\xEC\x17\xC3\x0D\x07\x8E\x81\xE0\xD3\xB0\xCF\x7C\x75\xF3\xA1\xFC\xF9\xA1\xD9"
                             "\xEA\xBE\x12\xC2\xCE\x67\x60\xB1\xA2\x02\x3D\x73\xA0\xDD\xE3\xA1\xAF\xC7\xB0\xFC"
                             "\x3D\x0E\xC0\x41\xCB\x0F\xC3\xD0\x4D\x33\x5A\x21\xF0\xF6\x0D\x32\x04\x2C\x2A\x01"
                             "\xF6\x02\x17\x2A\x01\xC7\xB0\x13\x78\x9C\x30\x60\xC1\xE0\x10\xF8\x1C\x38\xD9\x02"
                             "\x17\x32\x27\x3E\xD9\x0C\x36\x02\x1F\x22\x47\x31\xB2\x04\x4E\x3A\x01\x1B\x98\xA0"
                             "\xB4\x78\x55\x0F\x7E\xCC\x8F\x1F\x7E\xD3\x6B\x3C\xC7\x65\x0A\x3C\x1E\xC3\xF0\x85"
                             "\xF5\x8E\x09\xAA\xC4\x16\x58\x88\xCF\x7C\x74\x35\xF8\xF4\x3B\x04\xD3\x33\xF0\x16"
                             "\x78\x63\x3F\x0C\xEF\xE8\x3C\xEA\xBD\xE7\xF3\xE0\x98\x18\xB1\xAF\xA8\xE8\x3C\xE8"
                             "\x98\x4C\x6B\xEA\x21\xC6\x45\xA2\x1D\xD0\x46\xE0\xC8\xEF\x1E\x0C\xEF\xEB\x06\x56"
                             "\xE7\x78\xF8\x7B\x47\xBF\x82\xC6\x78\xF3\x3D\xB8\x79\x9E\xDF\x0A\xB1\x8C\xF3\x3D"
                             "\x81\xEF\xC3\x09\x9E\xC3\xA8\x10\x78\x3D\x3D\x87\x90\x87\x37\x4F\x61\xEE\x3A\x8B"
                             "\xE0\x89\x70\x76\x1B\x01\x16\xC9\x81\xC7\x3C\x7B\x0F\x71\xD4\x4C\x11\x2C\xB0\x82"
                             "\xD1\x9E\x04\x6C\x6A\xC4\x30\x7B\x0F\x71\xEE\x3D\xC7\x83\x3B\xFA\x12\xEA\xCF\x87"
                             "\xB6\x70\xBE\x08\x32\x41\x0B\x6C\x3E\x73\x1F\x46\x7B\xE3\xA1\x70\x20\xCC\x3B\xA0"
                             "\x89\xC1\x49\xD4\x25\xD5\x9D\x40\x85\xC0\x29\xDE\x3C\x02\x27\x20\xC0\x87\xCB\xA9"
                             "\xF9\xE7\x45\x5A\x35\xE0\xBA\x3B\xA6\x05\xF0\x75\xB9\xC7\x74\xEF\x1E\xD0\xB0\x3B"
                             "\xAD\xCE\x3A\x7D\x85\x96\x21\xDD\x3B\xC7\x83\xDC\x75\x1C\x89\x32\x04\x8C\x78\x61"
                             "\xF8\x7A\x1D\x83\x0F\xC3\xD0\xC6\x7C\x6A\xB0\xEB\x73\x8F\x87\xD9\xB4\x77\xCF\xB4"
                             "\x35\xD0\xAC\x10\xF8\x7D\x8F\x3A\x3E\xCF\xC3\xD0\x70\xBA\xAC\xE3\xF0\xFA\xF1\xE8"
                             "\x76\x02\x14\x73\xD0\xEC\x31\x9F\x1A\x7E\x4E\x17\xCF\x4A\xFA\x0C\x2B\xF7\x8F\x87"
                             "\xD9\xB6\x84\x42\xAB\xE7\xD9\xF8\x7A\x50\x87\xE1\xE8\x39\x56\xD0\x4C\xF8\x7D\x9C"
                             "\x64\x6C\x3E\x8E\x3C\x22\x36\x23\xEB\xC8\xEB\x47\xD7\x81\x07\xA0\x7E\x38\xFC\x3D"
                             "\x0E\xCA\x10\xFC\x3D\x28\x43\xF0\xFA\xF0\x22\x47\x3D\x04\xD3\x30\x43\xC4\x88\x22"
                             "\x35\x16\xA3\xEB\xC7\xD8\x21\xE7\x1E\xD3\xEC\xFC\x9C\x2F\x9E\x9A\x08\x52\xCF\x60"
                             "\xEA\x3D\x80\x85\x82\x9E\xC3\xE8\x43\xE8\xFA\x04\x4E\x7F\x8E\xB3\xAC\x70\x47\x99"
                             "\xF4\x20\xC3\x61\xEC\x3F\x0F\x43\xB3\x4F\xC9\xC2\xF9\xE9\x42\x02\x1D\x70\x44\xE8"
                             "\xA7\x1C\xA2\x36\x1F\x47\x1D\x11\xB0\xFA\x38\xE8\x8D\x87\xB0\xFC\x3F\x47\x91\xB0"
                             "\xE4\x22\x30\x73\x77\xC7\x83\xE9\xD1\x08\x7D\x07\x38\x5F\x40\x8D\x9F\x9B\x01\x1B"
                             "\x32\x0C\x23\xCC\xF2\x3E\x8E\x3A\x22\x36\x1F\x47\x1D\x11\x1B\x0F\xA3\x8E\x88\x8D"
                             "\x80\x83\x9D\x82\x44\xF0\x47\xE1\x98\x10\xF8\x62\x41\xE0\x5E\x19\x7C\x7C\x3D\x87"
                             "\x30\xF6\x1F\x87\xE8\xF2\x59\xEF\x9E\x0A\x70\xBE\x08\x5D\x15\xA0\x42\xE0\x6C\x83"
                             "\x2A\x2B\x47\xD0\x87\xB0\xFC\x3D\x3C\x36\xC2\x08\xFC\x3F\x47\x91\xC5\xF5\xF3\xC1"
                             "\xDC\x3D\x0E\xC2\x04\x19\x87\xD0\x84\x68\x08\x5D\x16\xC9\xC2\xF8\x21\x74\x18\x4E"
                             "\xCA\x10\xFC\x3E\xBC\x7B\x59\xEE\x04\xC9\xB3\x85\xF3";

// ++++++++++++++++++++^^^^^^^^^^^^^^^^^^^++++++++++++++++++++
// ++++++++++++++++++++ DO NOT EDIT ABOVE ++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

extern "C" {
  // comparator function used to sort Zigbee devices by alphabetical order (if friendlyname)
  // then by shortaddr if they don't have friendlyname
  int device_cmp(const void * a, const void * b) {
    const Z_Device &dev_a = zigbee_devices.devicesAt(*(uint8_t*)a);
    const Z_Device &dev_b = zigbee_devices.devicesAt(*(uint8_t*)b);
    const char * fn_a = dev_a.friendlyName;
    const char * fn_b = dev_b.friendlyName;

    if (fn_a && fn_b) {
      return strcasecmp(fn_a, fn_b);
    } else if (!fn_a && !fn_b) {
      return (int32_t)dev_a.shortaddr - (int32_t)dev_b.shortaddr;
    } else {
      if (fn_a) return -1;
      else      return 1;
    }
  }


// Convert seconds to a string representing days, hours or minutes present in the n-value.
// The string will contain the most coarse time only, rounded down (61m == 01h, 01h37m == 01h).
// Inputs:
// - seconds: uint32_t representing some number of seconds
// Outputs:
// - char for unit (d for day, h for hour, m for minute)
// - the hex color to be used to display the text
//
  uint32_t convert_seconds_to_dhm(uint32_t seconds,  char *unit, uint8_t *color){
    static uint32_t conversions[3] = {24 * 3600, 3600, 60};
    static char     units[3] = { 'd', 'h', 'm'};   // day, hour, minute
    uint8_t color_text_8 = WebColor(COL_TEXT) & 0xFF;    // color of text on 8 bits
    uint8_t color_back_8 = WebColor(COL_BACKGROUND) & 0xFF;    // color of background on 8 bits
    uint8_t  colors[3] = { (uint8_t) changeUIntScale(6, 0, 16, color_back_8, color_text_8),   // 6/16 of text
                           (uint8_t) changeUIntScale(10, 0, 16, color_back_8, color_text_8),  // 10/16 of text color
                           color_text_8};
    for(int i = 0; i < 3; ++i) {
      *color = colors[i];
      *unit = units[i];
      if (seconds > conversions[i]) {    // always pass even if 00m
        return seconds / conversions[i];
      }
    }
    return 0;
  }
} // extern "C"

const char HTTP_BTN_ZB_BUTTONS[] PROGMEM =
  "<button onclick='la(\"&zbj=1\");'>" D_ZIGBEE_PERMITJOIN "</button>"
  "<p></p>"
  "<form action='zbm' method='get'><button>" D_ZIGBEE_MAP "</button></form>";

void ZigbeeShow(bool json)
{
  if (json) {
    return;
#ifdef USE_WEBSERVER
  } else {
    UnishoxStrings msg(ZB_WEB);
    uint32_t zigbee_num = zigbee_devices.devicesSize();
    if (!zigbee_num) { return; }
    if (zigbee_num > 255) { zigbee_num = 255; }

    WSContentSend_P(msg[ZB_WEB_CSS], WebColor(COL_TEXT));
    // WSContentSend_compressed(ZB_WEB, 0);

    // sort elements by name, then by id
    uint8_t sorted_idx[zigbee_num];
    for (uint32_t i = 0; i < zigbee_num; i++) {
      sorted_idx[i] = i;
    }
    qsort(sorted_idx, zigbee_num, sizeof(sorted_idx[0]), device_cmp);

    uint32_t now = Rtc.utc_time;

    for (uint32_t i = 0; i < zigbee_num; i++) {
      const Z_Device &device = zigbee_devices.devicesAt(sorted_idx[i]);
      uint16_t shortaddr = device.shortaddr;
      char *name = (char*) device.friendlyName;

      char sdevice[33];
      if (nullptr == name) {
        snprintf_P(sdevice, sizeof(sdevice), PSTR(D_DEVICE " 0x%04X"), shortaddr);
        name = sdevice;
      }

      char sbatt[64];
      snprintf_P(sbatt, sizeof(sbatt), PSTR("&nbsp;"));
      if (device.validBatteryPercent()) {
        snprintf_P(sbatt, sizeof(sbatt),
          msg[ZB_WEB_BATTERY],
          device.batterypercent, changeUIntScale(device.batterypercent, 0, 100, 0, 14)
        );
      }

      uint32_t num_bars = 0;

      char slqi[4];
      slqi[0] = '-';
      slqi[1] = '\0';
      if (device.validLqi()){
        num_bars = changeUIntScale(device.lqi, 0, 254, 0, 4);
        snprintf_P(slqi, sizeof(slqi), PSTR("%d"), device.lqi);
      }

      WSContentSend_PD(msg[ZB_WEB_STATUS_LINE],
      shortaddr,
      device.modelId ? device.modelId : "",
      device.manufacturerId ? device.manufacturerId : "",
      name, sbatt, slqi);

      if(device.validLqi()) {
          for(uint32_t j = 0; j < 4; ++j) {
            WSContentSend_PD(PSTR("<i class='b%d%s'></i>"), j, (num_bars < j) ? PSTR(" o30") : PSTR(""));
          }
      }
      char dhm[48];
      snprintf_P(dhm, sizeof(dhm), PSTR("<td>&nbsp;"));
      if (device.validLastSeen()) {
        char unit;
        uint8_t color;
        uint16_t val = convert_seconds_to_dhm(now - device.last_seen, &unit, &color);
        if (val < 100) {
          snprintf_P(dhm, sizeof(dhm), msg[ZB_WEB_LAST_SEEN],
                                      color, color, color, val, unit);
        }
      }

      WSContentSend_PD(msg[ZB_WEB_END_STATUS], dhm );

      // Sensors
      const Z_Data_Thermo & thermo = device.data.find<Z_Data_Thermo>();

      if (&thermo != nullptr) {
        bool validTemp = thermo.validTemperature();
        bool validTempTarget = thermo.validTempTarget();
        bool validThSetpoint = thermo.validThSetpoint();
        bool validHumidity = thermo.validHumidity();
        bool validPressure = thermo.validPressure();

        if (validTemp || validTempTarget || validThSetpoint || validHumidity || validPressure) {
          WSContentSend_P(msg[ZB_WEB_LINE_START]);
          if (validTemp) {
            char buf[12];
            dtostrf(thermo.getTemperature() / 100.0f, 3, 1, buf);
            WSContentSend_PD(PSTR(" &#x2600;&#xFE0F; %s°C"), buf);
          }
          if (validTempTarget) {
            char buf[12];
            dtostrf(thermo.getTempTarget() / 100.0f, 3, 1, buf);
            WSContentSend_PD(PSTR(" &#127919; %s°C"), buf);
          }
          if (validThSetpoint) {
            WSContentSend_PD(PSTR(" &#9881;&#65039; %d%%"), thermo.getThSetpoint());
          }
          if (validHumidity) {
            WSContentSend_P(PSTR(" &#x1F4A7; %d%%"), (uint16_t)(thermo.getHumidity() / 100.0f + 0.5f));
          }
          if (validPressure) {
            WSContentSend_P(PSTR(" &#x26C5; %d hPa"), thermo.getPressure());
          }

          WSContentSend_P(PSTR("{e}"));
        }
      }

      // Light, switches and plugs
      const Z_Data_OnOff & onoff = device.data.find<Z_Data_OnOff>();
      bool onoff_display = (&onoff != nullptr) ? onoff.validPower() : false;
      const Z_Data_Light & light = device.data.find<Z_Data_Light>();
      bool light_display = (&light != nullptr) ? light.validDimmer() : false;
      const Z_Data_Plug & plug = device.data.find<Z_Data_Plug>();
      bool plug_voltage = (&plug != nullptr) ? plug.validMainsVoltage() : false;
      bool plug_power = (&plug != nullptr) ? plug.validMainsPower() : false;
      if (onoff_display || light_display || plug_voltage || plug_power) {
        int8_t channels = device.getLightChannels();
        if (channels < 0) { channels = 5; }     // if number of channel is unknown, display all known attributes
        WSContentSend_P(msg[ZB_WEB_LINE_START]);
        if (onoff_display) {
          WSContentSend_P(PSTR(" %s"), onoff.getPower() ? PSTR(D_ON) : PSTR(D_OFF));
        }
        if (&light != nullptr) {
          if (light.validDimmer() && (channels >= 1)) {
            WSContentSend_P(PSTR(" &#128261; %d%%"), changeUIntScale(light.getDimmer(),0,254,0,100));
          }
          if (light.validCT() && ((channels == 2) || (channels == 5))) {
            uint32_t ct_k = (((1000000 / light.getCT()) + 25) / 50) * 50;
            WSContentSend_P(msg[ZB_WEB_LIGHT_CT], light.getCT(), ct_k);
          }
          if (light.validHue() && light.validSat() && (channels >= 3)) {
            uint8_t r,g,b;
            uint8_t sat = changeUIntScale(light.getSat(), 0, 254, 0, 255);    // scale to 0..255
            HsToRgb(light.getHue(), sat, &r, &g, &b);
            WSContentSend_P(msg[ZB_WEB_COLOR_RGB], r,g,b,r,g,b);
          } else if (light.validX() && light.validY() && (channels >= 3)) {
            uint8_t r,g,b;
            XyToRgb(light.getX() / 65535.0f, light.getY() / 65535.0f, &r, &g, &b);
            WSContentSend_P(msg[ZB_WEB_COLOR_RGB], r,g,b,r,g,b);
          }
        }
        if (plug_voltage || plug_power) {
          WSContentSend_P(PSTR(" &#9889; "));
          if (plug_voltage) {
            WSContentSend_P(PSTR(" %dV"), plug.getMainsVoltage());
          }
          if (plug_power) {
            WSContentSend_P(PSTR(" %dW"), plug.getMainsPower());
          }
        }
        WSContentSend_P(PSTR("{e}"));
      }
    }

    WSContentSend_P(msg[ZB_WEB_LINE_END]);  // Terminate current multi column table and open new table
    if (zigbee.permit_end_time) {
      // PermitJoin in progress

      WSContentSend_P(msg[ZB_WEB_PERMITJOIN_ACTIVE], D_ZIGBEE_PERMITJOIN_ACTIVE);
    }
#endif
  }
}

// Web handler to refresh the map, the redirect to show map
void ZigbeeMapRefresh(void) {
  if ((!zigbee.init_phase) && (!zigbee.mapping_in_progress)) {
    ZigbeeMapAllDevices();
  }
  Webserver->sendHeader("Location","/zbm");        // Add a header to respond with a new location for the browser to go to the home page again
  Webserver->send(302);
}

// Display a graphical representation of the Zigbee map using vis.js network
void ZigbeeShowMap(void) {
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Zigbee Mapper"));

  // if no map, then launch a new mapping
  if ((!zigbee.init_phase) && (!zigbee.mapping_ready) && (!zigbee.mapping_in_progress)) {
    ZigbeeMapAllDevices();
  }

  UnishoxStrings msg(ZB_WEB);
  WSContentStart_P(PSTR(D_ZIGBEE_MAPPING_TITLE));
  WSContentSendStyle();

  if (zigbee.init_phase) {
    WSContentSend_P(PSTR(D_ZIGBEE_NOT_STARTED));
  } else if (zigbee.mapping_in_progress) {
    int32_t mapping_remaining = 1 + (zigbee.mapping_end_time - millis()) / 1000;
    if (mapping_remaining < 0) { mapping_remaining = 0; }
    WSContentSend_P(PSTR(D_ZIGBEE_MAPPING_IN_PROGRESS_SEC), mapping_remaining);
    WSContentSend_P(msg[ZB_WEB_AUTO_REFRESH]);
  } else if (!zigbee.mapping_ready) {
    WSContentSend_P(PSTR(D_ZIGBEE_MAPPING_NOT_PRESENT));
  } else {
    WSContentSend_P(msg[ZB_WEB_VIS_JS_BEFORE]);

    zigbee_mapper.dumpInternals();

    WSContentSend_P(msg[ZB_WEB_VIS_JS_AFTER]);
    WSContentSend_P(msg[ZB_WEB_MAP_REFRESH], PSTR(D_ZIGBEE_MAP_REFRESH));
  }
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv23(uint8_t function)
{
  bool result = false;

  if (zigbee.active) {
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        if (!zigbee.init_phase) {
          zigbee_devices.runTimer();
        }
        break;
      case FUNC_LOOP:
#ifdef USE_ZIGBEE_EZSP
        if (ZigbeeUploadXmodem()) {
          return false;
        }
#endif
        if (ZigbeeSerial) {
          ZigbeeInputLoop();
          ZigbeeOutputLoop();   // send any outstanding data
#ifdef USE_ZIGBEE_EZSP
          ZigbeeGlowPermitJoinLight();
#endif // USE_ZIGBEE_EZSP
        }
        if (zigbee.state_machine) {
          ZigbeeStateMachine_Run();
        }
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        ZigbeeShow(false);
        break;
      // GUI xmodem
      case FUNC_WEB_ADD_HANDLER:
#ifdef USE_ZIGBEE_EZSP
        WebServer_on(PSTR("/" WEB_HANDLE_ZIGBEE_XFER), HandleZigbeeXfer);
#endif  // USE_ZIGBEE_EZSP
        WebServer_on(PSTR("/zbm"), ZigbeeShowMap, HTTP_GET);     // add web handler for Zigbee map
        WebServer_on(PSTR("/zbr"), ZigbeeMapRefresh, HTTP_GET);     // add web handler for Zigbee map refresh
        break;
      case FUNC_WEB_ADD_MAIN_BUTTON:
        WSContentSend_P(HTTP_BTN_ZB_BUTTONS);
        break;
#endif  // USE_WEBSERVER
      case FUNC_PRE_INIT:
        ZigbeeInit();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kZbCommands, ZigbeeCommand);
        break;
      case FUNC_SAVE_BEFORE_RESTART:
#ifdef USE_ZIGBEE_EZSP
        hibernateAllData();
#endif  // USE_ZIGBEE_EZSP
        restoreDumpAllDevices();
        break;
    }
  }
  return result;
}

#endif // USE_ZIGBEE
