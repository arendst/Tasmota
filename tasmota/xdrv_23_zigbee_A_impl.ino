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

#define XDRV_23                    23

const char kZbCommands[] PROGMEM = D_PRFX_ZB "|"    // prefix
#ifdef USE_ZIGBEE_ZNP
  D_CMND_ZIGBEEZNPSEND "|" D_CMND_ZIGBEEZNPRECEIVE "|"
#endif // USE_ZIGBEE_ZNP
#ifdef USE_ZIGBEE_EZSP
  D_CMND_ZIGBEE_EZSP_SEND "|" D_CMND_ZIGBEE_EZSP_RECEIVE "|" D_CMND_ZIGBEE_EZSP_LISTEN "|"
#endif // USE_ZIGBEE_EZSP
  D_CMND_ZIGBEE_PERMITJOIN "|"
  D_CMND_ZIGBEE_STATUS "|" D_CMND_ZIGBEE_RESET "|" D_CMND_ZIGBEE_SEND "|" D_CMND_ZIGBEE_PROBE "|"
  D_CMND_ZIGBEE_FORGET "|" D_CMND_ZIGBEE_SAVE "|" D_CMND_ZIGBEE_NAME "|"
  D_CMND_ZIGBEE_BIND "|" D_CMND_ZIGBEE_UNBIND "|" D_CMND_ZIGBEE_PING "|" D_CMND_ZIGBEE_MODELID "|"
  D_CMND_ZIGBEE_LIGHT "|" D_CMND_ZIGBEE_RESTORE "|" D_CMND_ZIGBEE_BIND_STATE "|"
  D_CMND_ZIGBEE_CONFIG
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
  &CmndZbForget, &CmndZbSave, &CmndZbName,
  &CmndZbBind, &CmndZbUnbind, &CmndZbPing, &CmndZbModelId,
  &CmndZbLight, &CmndZbRestore, &CmndZbBindState,
  &CmndZbConfig,
  };

/********************************************************************************************/

// Initialize internal structures
void ZigbeeInit(void)
{
  // Check if settings in Flash are set
  if (PinUsed(GPIO_ZIGBEE_RX) && PinUsed(GPIO_ZIGBEE_TX)) {
    if (0 == Settings.zb_channel) {
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Randomizing Zigbee parameters, please check with 'ZbConfig'"));
      uint64_t mac64 = 0;     // stuff mac address into 64 bits
      WiFi.macAddress((uint8_t*) &mac64);
      uint32_t esp_id = ESP_getChipId();
#ifdef ESP8266
      uint32_t flash_id = ESP.getFlashChipId();
#else  // ESP32
      uint32_t flash_id = 0;
#endif  // ESP8266 or ESP32

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

uint32_t strToUInt(const JsonVariant &val) {
  // if the string starts with 0x, it is considered Hex, otherwise it is an int
  if (val.is<unsigned int>()) {
    return val.as<unsigned int>();
  } else {
    if (val.is<const char*>()) {
      String sval = val.as<String>();
      return strtoull(sval.c_str(), nullptr, 0);
    }
  }
  return 0;   // couldn't parse anything
}

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
      restart_flag = 2;
      ResponseCmndChar_P(PSTR(D_JSON_ZIGBEE_CC2530 " " D_JSON_RESET_AND_RESTARTING));
      break;
    default:
      ResponseCmndChar_P(PSTR(D_JSON_ONE_TO_RESET));
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
    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZbSend: guessing endpoint 0x%02X"), endpoint);
  }
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZbSend: shortaddr 0x%04X, groupaddr 0x%04X, cluster 0x%04X, endpoint 0x%02X, cmd 0x%02X, data %s"),
    shortaddr, groupaddr, cluster, endpoint, cmd, param);

  if ((0 == endpoint) && (BAD_SHORTADDR != shortaddr)) {     // endpoint null is ok for group address
    AddLog_P2(LOG_LEVEL_INFO, PSTR("ZbSend: unspecified endpoint"));
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
    seq,  /* zcl transaction id */
    buf.getBuffer(), buf.len()
  }));
  // now set the timer, if any, to read back the state later
  if (clusterSpecific) {
#ifndef USE_ZIGBEE_NO_READ_ATTRIBUTES   // read back attribute value unless it is disabled
    sendHueUpdate(shortaddr, groupaddr, cluster, endpoint);
#endif
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

// Parse "Report", "Write", "Response" or "Condig" attribute
// Operation is one of: ZCL_REPORT_ATTRIBUTES (0x0A), ZCL_WRITE_ATTRIBUTES (0x02) or ZCL_READ_ATTRIBUTES_RESPONSE (0x01)
void ZbSendReportWrite(const JsonObject &val_pubwrite, uint16_t device, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint16_t manuf, uint8_t operation) {
  SBuffer buf(200);       // buffer to store the binary output of attibutes

  if (nullptr == XdrvMailbox.command) {
    XdrvMailbox.command = (char*) "";             // prevent a crash when calling ReponseCmndChar and there was no previous command
  }

  // iterate on keys
  for (JsonObject::const_iterator it=val_pubwrite.begin(); it!=val_pubwrite.end(); ++it) {
    const char *key = it->key;
    const JsonVariant &value = it->value;

    uint16_t attr_id = 0xFFFF;
    uint16_t cluster_id = 0xFFFF;
    uint8_t  type_id = Znodata;
    int8_t   multiplier = 1;        // multiplier to adjust the key value
    double   val_d = 0;             // I try to avoid `double` but this type capture both float and (u)int32_t without prevision loss
    const char* val_str = "";       // variant as string

    // check if the name has the format "XXXX/YYYY" where XXXX is the cluster, YYYY the attribute id
    // alternative "XXXX/YYYY%ZZ" where ZZ is the type (for unregistered attributes)
    char * delimiter = strchr(key, '/');
    char * delimiter2 = strchr(key, '%');
    if (delimiter) {
      cluster_id = strtoul(key, &delimiter, 16);
      if (!delimiter2) {
        attr_id = strtoul(delimiter+1, nullptr, 16);
      } else {
        attr_id = strtoul(delimiter+1, &delimiter2, 16);
        type_id = strtoul(delimiter2+1, nullptr, 16);
      }
    }
    // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("cluster_id = 0x%04X, attr_id = 0x%04X"), cluster_id, attr_id);

    // do we already know the type, i.e. attribute and cluster are also known
    if (Znodata == type_id) {
      // scan attributes to find by name, and retrieve type
      for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
        const Z_AttributeConverter *converter = &Z_PostProcess[i];
        bool match = false;
        uint16_t local_attr_id = pgm_read_word(&converter->attribute);
        uint16_t local_cluster_id = CxToCluster(pgm_read_byte(&converter->cluster_short));
        uint8_t  local_type_id = pgm_read_byte(&converter->type);
        int8_t   local_multiplier = pgm_read_byte(&converter->multiplier);
        // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Try cluster = 0x%04X, attr = 0x%04X, type_id = 0x%02X"), local_cluster_id, local_attr_id, local_type_id);

        if (delimiter) {
          if ((cluster_id == local_cluster_id) && (attr_id == local_attr_id)) {
            type_id = local_type_id;
            break;
          }
        } else if (pgm_read_word(&converter->name_offset)) {
          // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Comparing '%s' with '%s'"), attr_name, converter->name);
          if (0 == strcasecmp_P(key, Z_strings + pgm_read_word(&converter->name_offset))) {
            // match
            cluster_id = local_cluster_id;
            attr_id = local_attr_id;
            type_id = local_type_id;
            multiplier = local_multiplier;
            break;
          }
        }
      }
    }

    // Buffer ready, do some sanity checks
    // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("cluster_id = 0x%04X, attr_id = 0x%04X, type_id = 0x%02X"), cluster_id, attr_id, type_id);
    if ((0xFFFF == attr_id) || (0xFFFF == cluster_id)) {
      Response_P(PSTR("{\"%s\":\"%s'%s'\"}"), XdrvMailbox.command, PSTR("Unknown attribute "), key);
      return;
    }
    if (Znodata == type_id) {
      Response_P(PSTR("{\"%s\":\"%s'%s'\"}"), XdrvMailbox.command, PSTR("Unknown attribute type for attribute "), key);
      return;
    }

    if (0xFFFF == cluster) {
      cluster = cluster_id;       // set the cluster for this packet
    } else if (cluster != cluster_id) {
      ResponseCmndChar_P(PSTR("No more than one cluster id per command"));
      return;
    }

    // ////////////////////////////////////////////////////////////////////////////////
    // Split encoding depending on message
    if (operation != ZCL_CONFIGURE_REPORTING) {
      // apply multiplier if needed
      val_d = value.as<double>();
      val_str = value.as<const char*>();
      ZbApplyMultiplier(val_d, multiplier);

      // push the value in the buffer
      buf.add16(attr_id);        // prepend with attribute identifier
      if (operation == ZCL_READ_ATTRIBUTES_RESPONSE) {
        buf.add8(Z_SUCCESS);  // status OK = 0x00
      }
      buf.add8(type_id);     // prepend with attribute type
      int32_t res = encodeSingleAttribute(buf, val_d, val_str, type_id);
      if (res < 0) {
        // remove the attribute type we just added
        // buf.setLen(buf.len() - (operation == ZCL_READ_ATTRIBUTES_RESPONSE ? 4 : 3));
        Response_P(PSTR("{\"%s\":\"%s'%s' 0x%02X\"}"), XdrvMailbox.command, PSTR("Unsupported attribute type "), key, type_id);
        return;
      }

    } else {
      // ////////////////////////////////////////////////////////////////////////////////
      // ZCL_CONFIGURE_REPORTING
      if (!value.is<JsonObject>()) {
        ResponseCmndChar_P(PSTR("Config requires JSON objects"));
        return;
      }
      JsonObject &attr_config = value.as<JsonObject>();
      bool attr_direction = false;

      const JsonVariant &val_attr_direction = GetCaseInsensitive(attr_config, PSTR("DirectionReceived"));
      if (nullptr != &val_attr_direction) {
        uint32_t dir = strToUInt(val_attr_direction);
        if (dir) {
          attr_direction = true;
        }
      }

      // read MinInterval and MaxInterval, default to 0xFFFF if not specified
      uint16_t attr_min_interval = 0xFFFF;
      uint16_t attr_max_interval = 0xFFFF;
      const JsonVariant &val_attr_min = GetCaseInsensitive(attr_config, PSTR("MinInterval"));
      if (nullptr != &val_attr_min) { attr_min_interval = strToUInt(val_attr_min); }
      const JsonVariant &val_attr_max = GetCaseInsensitive(attr_config, PSTR("MaxInterval"));
      if (nullptr != &val_attr_max) { attr_max_interval = strToUInt(val_attr_max); }

      // read ReportableChange
      const JsonVariant &val_attr_rc = GetCaseInsensitive(attr_config, PSTR("ReportableChange"));
      if (nullptr != &val_attr_rc) {
        val_d = val_attr_rc.as<double>();
        val_str = val_attr_rc.as<const char*>();
        ZbApplyMultiplier(val_d, multiplier);
      }

      // read TimeoutPeriod
      uint16_t attr_timeout = 0x0000;
      const JsonVariant &val_attr_timeout = GetCaseInsensitive(attr_config, PSTR("TimeoutPeriod"));
      if (nullptr != &val_attr_timeout) { attr_timeout = strToUInt(val_attr_timeout); }

      bool attr_discrete = Z_isDiscreteDataType(type_id);

      // all fields are gathered, output the butes into the buffer, ZCL 2.5.7.1
      // common bytes
      buf.add8(attr_direction ? 0x01 : 0x00);
      buf.add16(attr_id);
      if (attr_direction) {
        buf.add16(attr_timeout);
      } else {
        buf.add8(type_id);
        buf.add16(attr_min_interval);
        buf.add16(attr_max_interval);
        if (!attr_discrete) {
          int32_t res = encodeSingleAttribute(buf, val_d, val_str, type_id);
          if (res < 0) {
            Response_P(PSTR("{\"%s\":\"%s'%s' 0x%02X\"}"), XdrvMailbox.command, PSTR("Unsupported attribute type "), key, type_id);
            return;
          }
        }
      }
    }
  }

  // did we have any attribute?
  if (0 == buf.len()) {
    ResponseCmndChar_P(PSTR("No attribute in list"));
    return;
  }

  // all good, send the packet
  uint8_t seq = zigbee_devices.getNextSeqNumber(device);
  ZigbeeZCLSend_Raw(ZigbeeZCLSendMessage({
    device,
    groupaddr,
    cluster /*cluster*/,
    endpoint,
    operation,
    manuf,  /* manuf */
    false /* not cluster specific */,
    false /* no response */,
    seq,  /* zcl transaction id */
    buf.getBuffer(), buf.len()
  }));
  ResponseCmndDone();
}

// Parse the "Send" attribute and send the command
void ZbSendSend(const JsonVariant &val_cmd, uint16_t device, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint16_t manuf) {
  uint8_t  cmd = 0;
  String   cmd_str = "";          // the actual low-level command, either specified or computed
  const char *cmd_s;                 // pointer to payload string
  bool     clusterSpecific = true;

  static char delim[] = ", ";     // delimiters for parameters
  // probe the type of the argument
  // If JSON object, it's high level commands
  // If String, it's a low level command
  if (val_cmd.is<JsonObject>()) {
    // we have a high-level command
    const JsonObject &cmd_obj = val_cmd.as<const JsonObject&>();
    int32_t cmd_size = cmd_obj.size();
    if (cmd_size > 1) {
      Response_P(PSTR("Only 1 command allowed (%d)"), cmd_size);
      return;
    } else if (1 == cmd_size) {
      // We have exactly 1 command, parse it
      JsonObject::const_iterator it = cmd_obj.begin();    // just get the first key/value
      String key = it->key;
      const JsonVariant& value = it->value;
      uint32_t x = 0, y = 0, z = 0;
      uint16_t cmd_var;
      uint16_t local_cluster_id;

      const __FlashStringHelper* tasmota_cmd = zigbeeFindCommand(key.c_str(), &local_cluster_id, &cmd_var);
      if (tasmota_cmd) {
        cmd_str = tasmota_cmd;
      } else {
        Response_P(PSTR("Unrecognized zigbee command: %s"), key.c_str());
        return;
      }
      // check cluster
      if (0xFFFF == cluster) {
        cluster = local_cluster_id;
      } else if (cluster != local_cluster_id) {
        ResponseCmndChar_P(PSTR("No more than one cluster id per command"));
        return;
      }

      // parse the JSON value, depending on its type fill in x,y,z
      if (value.is<bool>()) {
        x = value.as<bool>() ? 1 : 0;
      } else if (value.is<unsigned int>()) {
        x = value.as<unsigned int>();
      } else {
        // if non-bool or non-int, trying char*
        const char *s_const = value.as<const char*>();
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

      //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZbSend: command_template = %s"), cmd_str.c_str());
      if (0xFF == cmd_var) {      // if command number is a variable, replace it with x
        cmd = x;
        x = y;                  // and shift other variables
        y = z;
      } else {
        cmd = cmd_var;          // or simply copy the cmd number
      }
      cmd_str = zigbeeCmdAddParams(cmd_str.c_str(), x, y, z);   // fill in parameters
      //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZbSend: command_final    = %s"), cmd_str.c_str());
      cmd_s = cmd_str.c_str();
    } else {
      // we have zero command, pass through until last error for missing command
    }
  } else if (val_cmd.is<const char*>()) {
    // low-level command
    cmd_str = val_cmd.as<String>();
    // Now parse the string to extract cluster, command, and payload
    // Parse 'cmd' in the form "AAAA_BB/CCCCCCCC" or "AAAA!BB/CCCCCCCC"
    // where AA is the cluster number, BBBB the command number, CCCC... the payload
    // First delimiter is '_' for a global command, or '!' for a cluster specific command
    const char * data = cmd_str.c_str();
    uint16_t local_cluster_id = parseHex(&data, 4);

    // check cluster
    if (0xFFFF == cluster) {
      cluster = local_cluster_id;
    } else if (cluster != local_cluster_id) {
      ResponseCmndChar_P(PSTR("No more than one cluster id per command"));
      return;
    }

    // delimiter
    if (('_' == *data) || ('!' == *data)) {
      if ('_' == *data) { clusterSpecific = false; }
      data++;
    } else {
      ResponseCmndChar_P(PSTR("Wrong delimiter for payload"));
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

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZigbeeZCLSend device: 0x%04X, group: 0x%04X, endpoint:%d, cluster:0x%04X, cmd:0x%02X, send:\"%s\""),
            device, groupaddr, endpoint, cluster, cmd, cmd_s);
  zigbeeZCLSendStr(device, groupaddr, endpoint, clusterSpecific, manuf, cluster, cmd, cmd_s);
  ResponseCmndDone();
}


// Parse the "Send" attribute and send the command
void ZbSendRead(const JsonVariant &val_attr, uint16_t device, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint16_t manuf, uint8_t operation) {
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
  if (ZCL_READ_REPORTING_CONFIGURATION == operation) {
    attr_item_len = 3;
    attr_item_offset = 1;
  }

  uint16_t val = strToUInt(val_attr);
  if (val_attr.is<JsonArray>()) {
    const JsonArray& attr_arr = val_attr.as<const JsonArray&>();
    attrs_len = attr_arr.size() * attr_item_len;
    attrs = (uint8_t*) calloc(attrs_len, 1);

    uint32_t i = 0;
    for (auto value : attr_arr) {
      uint16_t val = strToUInt(value);
      i += attr_item_offset;
      attrs[i++] = val & 0xFF;
      attrs[i++] = val >> 8;
      i += attr_item_len - 2 - attr_item_offset;    // normally 0
    }
  } else if (val_attr.is<JsonObject>()) {
    const JsonObject& attr_obj = val_attr.as<const JsonObject&>();
    attrs_len = attr_obj.size() * attr_item_len;
    attrs = (uint8_t*) calloc(attrs_len, 1);
    uint32_t actual_attr_len = 0;

    // iterate on keys
    for (JsonObject::const_iterator it=attr_obj.begin(); it!=attr_obj.end(); ++it) {
      const char *key = it->key;
      const JsonVariant &value = it->value;      // we don't need the value here, only keys are relevant

      bool found = false;
      // scan attributes to find by name, and retrieve type
      for (uint32_t i = 0; i < ARRAY_SIZE(Z_PostProcess); i++) {
        const Z_AttributeConverter *converter = &Z_PostProcess[i];
        bool match = false;
        uint16_t local_attr_id = pgm_read_word(&converter->attribute);
        uint16_t local_cluster_id = CxToCluster(pgm_read_byte(&converter->cluster_short));
        // uint8_t  local_type_id = pgm_read_byte(&converter->type);

        if ((pgm_read_word(&converter->name_offset)) && (0 == strcasecmp_P(key, Z_strings + pgm_read_word(&converter->name_offset)))) {
          // match name
          // check if there is a conflict with cluster
          // TODO
          if (!value && attr_item_offset) {
            // If value is false (non-default) then set direction to 1 (for ReadConfig)
            attrs[actual_attr_len] = 0x01;
          }
          actual_attr_len += attr_item_offset;
          attrs[actual_attr_len++] = local_attr_id & 0xFF;
          attrs[actual_attr_len++] = local_attr_id >> 8;
          actual_attr_len += attr_item_len - 2 - attr_item_offset;    // normally 0
          found = true;
          // check cluster
          if (0xFFFF == cluster) {
            cluster = local_cluster_id;
          } else if (cluster != local_cluster_id) {
            ResponseCmndChar_P(PSTR("No more than one cluster id per command"));
            if (attrs) { free(attrs); }
            return;
          }
          break;    // found, exit loop
        }
      }
      if (!found) {
        AddLog_P2(LOG_LEVEL_INFO, PSTR("ZIG: Unknown attribute name (ignored): %s"), key);
      }
    }

    attrs_len = actual_attr_len;
  } else {
    attrs_len = attr_item_len;
    attrs = (uint8_t*) calloc(attrs_len, 1);
    attrs[0 + attr_item_offset] = val & 0xFF;    // little endian
    attrs[1 + attr_item_offset] = val >> 8;
  }

  if (attrs_len > 0) {
    uint8_t seq = zigbee_devices.getNextSeqNumber(device);
    ZigbeeZCLSend_Raw(ZigbeeZCLSendMessage({
      device,
      groupaddr,
      cluster /*cluster*/,
      endpoint,
      operation,
      manuf,  /* manuf */
      false /* not cluster specific */,
      true /* response */,
      seq,  /* zcl transaction id */
      attrs, attrs_len
    }));
    ResponseCmndDone();
  } else {
    ResponseCmndChar_P(PSTR("Missing parameters"));
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
  DynamicJsonBuffer jsonBuf;
  const JsonObject &json = jsonBuf.parseObject((const char*) XdrvMailbox.data);
  if (!json.success()) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

  // params
  uint16_t device = BAD_SHORTADDR;    // BAD_SHORTADDR is broadcast, so considered invalid
  uint16_t groupaddr = 0x0000;        // group address valid only if device == BAD_SHORTADDR
  uint16_t cluster = 0xFFFF;          // no default
  uint8_t  endpoint = 0x00;           // 0x00 is invalid for the dst endpoint
  uint16_t manuf = 0x0000;            // Manuf Id in ZCL frame


  // parse "Device" and "Group"
  const JsonVariant &val_device = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_DEVICE));
  if (nullptr != &val_device) {
    device = zigbee_devices.parseDeviceParam(val_device.as<char*>());
    if (BAD_SHORTADDR == device) { ResponseCmndChar_P(PSTR("Invalid parameter")); return; }
  }
  if (BAD_SHORTADDR == device) {     // if not found, check if we have a group
    const JsonVariant &val_group = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_GROUP));
    if (nullptr != &val_group) {
      groupaddr = strToUInt(val_group);
    } else {                  // no device nor group
      ResponseCmndChar_P(PSTR("Unknown device"));
      return;
    }
  }
  // from here, either device has a device shortaddr, or if BAD_SHORTADDR then use group address
  // Note: groupaddr == 0 is valid

  // read other parameters
  const JsonVariant &val_cluster = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_CLUSTER));
  if (nullptr != &val_cluster) { cluster = strToUInt(val_cluster); }
  const JsonVariant &val_endpoint = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_ENDPOINT));
  if (nullptr != &val_endpoint) { endpoint = strToUInt(val_endpoint); }
  const JsonVariant &val_manuf = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_MANUF));
  if (nullptr != &val_manuf) { manuf = strToUInt(val_manuf); }

  // infer endpoint
  if (BAD_SHORTADDR == device) {
    endpoint = 0xFF;                  // endpoint not used for group addresses, so use a dummy broadcast endpoint
  } else if (0 == endpoint) {         // if it was not already specified, try to guess it
    endpoint = zigbee_devices.findFirstEndpoint(device);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("ZIG: guessing endpoint %d"), endpoint);
  }
  if (0 == endpoint) {                // after this, if it is still zero, then it's an error
      ResponseCmndChar_P(PSTR("Missing endpoint"));
      return;
  }
  // from here endpoint is valid and non-zero
  // cluster may be already specified or 0xFFFF

  const JsonVariant &val_cmd = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_SEND));
  const JsonVariant &val_read = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_READ));
  const JsonVariant &val_write = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_WRITE));
  const JsonVariant &val_publish = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_REPORT));
  const JsonVariant &val_response = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_RESPONSE));
  const JsonVariant &val_read_config = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_READ_CONFIG));
  const JsonVariant &val_config = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_CONFIG));
  uint32_t multi_cmd = (nullptr != &val_cmd) + (nullptr != &val_read) + (nullptr != &val_write) + (nullptr != &val_publish)
                     + (nullptr != &val_response) + (nullptr != &val_read_config) + (nullptr != &val_config);
  if (multi_cmd > 1) {
    ResponseCmndChar_P(PSTR("Can only have one of: 'Send', 'Read', 'Write', 'Report', 'Reponse', 'ReadConfig' or 'Config'"));
    return;
  }
  // from here we have one and only one command

  if (nullptr != &val_cmd) {
    // "Send":{...commands...}
    // we accept either a string or a JSON object
    ZbSendSend(val_cmd, device, groupaddr, cluster, endpoint, manuf);
  } else if (nullptr != &val_read) {
    // "Read":{...attributes...}, "Read":attribute or "Read":[...attributes...]
    // we accept eitehr a number, a string, an array of numbers/strings, or a JSON object
    ZbSendRead(val_read, device, groupaddr, cluster, endpoint, manuf, ZCL_READ_ATTRIBUTES);
  } else if (nullptr != &val_write) {
    // only KSON object
    if (!val_write.is<JsonObject>()) {
      ResponseCmndChar_P(PSTR("Missing parameters"));
      return;
    }
    // "Write":{...attributes...}
    ZbSendReportWrite(val_write, device, groupaddr, cluster, endpoint, manuf, ZCL_WRITE_ATTRIBUTES);
  } else if (nullptr != &val_publish) {
    // "Publish":{...attributes...}
    // only KSON object
    if (!val_publish.is<JsonObject>()) {
      ResponseCmndChar_P(PSTR("Missing parameters"));
      return;
    }
    ZbSendReportWrite(val_publish, device, groupaddr, cluster, endpoint, manuf, ZCL_REPORT_ATTRIBUTES);
  } else if (nullptr != &val_response) {
    // "Report":{...attributes...}
    // only KSON object
    if (!val_response.is<JsonObject>()) {
      ResponseCmndChar_P(PSTR("Missing parameters"));
      return;
    }
    ZbSendReportWrite(val_response, device, groupaddr, cluster, endpoint, manuf, ZCL_READ_ATTRIBUTES_RESPONSE);
  } else if (nullptr != &val_read_config) {
    // "ReadConfg":{...attributes...}, "ReadConfg":attribute or "ReadConfg":[...attributes...]
    // we accept eitehr a number, a string, an array of numbers/strings, or a JSON object
    ZbSendRead(val_read_config, device, groupaddr, cluster, endpoint, manuf, ZCL_READ_REPORTING_CONFIGURATION);
  } else if (nullptr != &val_config) {
    // "Config":{...attributes...}
    // only JSON object
    if (!val_config.is<JsonObject>()) {
      ResponseCmndChar_P(PSTR("Missing parameters"));
      return;
    }
   ZbSendReportWrite(val_config, device, groupaddr, cluster, endpoint, manuf, ZCL_CONFIGURE_REPORTING);
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
  DynamicJsonBuffer jsonBuf;
  const JsonObject &json = jsonBuf.parseObject((const char*) XdrvMailbox.data);
  if (!json.success()) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

  // params
  uint16_t srcDevice = BAD_SHORTADDR;         // BAD_SHORTADDR is broadcast, so considered invalid
  uint16_t dstDevice = BAD_SHORTADDR;      // BAD_SHORTADDR is broadcast, so considered invalid
  uint64_t dstLongAddr = 0;
  uint8_t  endpoint = 0x00;         // 0x00 is invalid for the src endpoint
  uint8_t  toendpoint = 0x00;       // 0x00 is invalid for the dst endpoint
  uint16_t toGroup = 0x0000;        // group address
  uint16_t cluster  = 0;            // 0xFFFF is invalid
  uint32_t group = 0xFFFFFFFF;      // 16 bits values, otherwise 0xFFFFFFFF is unspecified

  // Information about source device: "Device", "Endpoint", "Cluster"
  //  - the source endpoint must have a known IEEE address
  const JsonVariant &val_device = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_DEVICE));
  if (nullptr != &val_device) {
    srcDevice = zigbee_devices.parseDeviceParam(val_device.as<char*>());
  }
  if ((nullptr == &val_device) || (BAD_SHORTADDR == srcDevice)) { ResponseCmndChar_P(PSTR("Unknown source device")); return; }
  // check if IEEE address is known
  uint64_t srcLongAddr = zigbee_devices.getDeviceLongAddr(srcDevice);
  if (0 == srcLongAddr) { ResponseCmndChar_P(PSTR("Unknown source IEEE address")); return; }
  // look for source endpoint
  const JsonVariant &val_endpoint = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_ENDPOINT));
  if (nullptr != &val_endpoint) { endpoint = strToUInt(val_endpoint); }
  else { endpoint = zigbee_devices.findFirstEndpoint(srcDevice); }
  // look for source cluster
  const JsonVariant &val_cluster = GetCaseInsensitive(json, PSTR(D_CMND_ZIGBEE_CLUSTER));
  if (nullptr != &val_cluster) {
    cluster = strToUInt(val_cluster);   // first convert as number
    if (0 == cluster) {
      zigbeeFindAttributeByName(val_cluster.as<const char*>(), &cluster, nullptr, nullptr);
    }
  }

  // Or Group Address - we don't need a dstEndpoint in this case
  const JsonVariant &to_group = GetCaseInsensitive(json, PSTR("ToGroup"));
  if (nullptr != &to_group) { toGroup = strToUInt(to_group); }

  // Either Device address
  // In this case the following parameters are mandatory
  //  - "ToDevice" and the device must have a known IEEE address
  //  - "ToEndpoint"
  const JsonVariant &dst_device = GetCaseInsensitive(json, PSTR("ToDevice"));

  // If no target is specified, we default to coordinator 0x0000
  if ((nullptr == &to_group) && (nullptr == &dst_device)) {
    dstDevice = 0x0000;
  }

  if ((nullptr != &dst_device) || (BAD_SHORTADDR != dstDevice)) {
    if (BAD_SHORTADDR == dstDevice) {
      dstDevice = zigbee_devices.parseDeviceParam(dst_device.as<char*>());
      if (BAD_SHORTADDR == dstDevice) { ResponseCmndChar_P(PSTR("Invalid parameter")); return; }
    }
    if (0x0000 == dstDevice) {
      dstLongAddr = localIEEEAddr;
    } else {
      dstLongAddr = zigbee_devices.getDeviceLongAddr(dstDevice);
    }
    if (0 == dstLongAddr) { ResponseCmndChar_P(PSTR("Unknown dest IEEE address")); return; }

    const JsonVariant &val_toendpoint = GetCaseInsensitive(json, PSTR("ToEndpoint"));
    if (nullptr != &val_toendpoint) { toendpoint = strToUInt(val_toendpoint); }
    else { toendpoint = 0x01; }   // default to endpoint 1
  }

  // make sure we don't have conflicting parameters
  if (&to_group && dstLongAddr) { ResponseCmndChar_P(PSTR("Cannot have both \"ToDevice\" and \"ToGroup\"")); return; }
  if (!&to_group && !dstLongAddr) { ResponseCmndChar_P(PSTR("Missing \"ToDevice\" or \"ToGroup\"")); return; }

#ifdef USE_ZIGBEE_ZNP
  SBuffer buf(34);
  buf.add8(Z_SREQ | Z_ZDO);
  if (unbind) {
    buf.add8(ZDO_UNBIND_REQ);
  } else {
    buf.add8(ZDO_BIND_REQ);
  }
  buf.add16(srcDevice);
  buf.add64(srcLongAddr);
  buf.add8(endpoint);
  buf.add16(cluster);
  if (dstLongAddr) {
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
  if (dstLongAddr) {
    buf.add8(Z_Addr_IEEEAddress);         // DstAddrMode - 0x03 = ADDRESS_64_BIT
    buf.add64(dstLongAddr);
    buf.add8(toendpoint);
  } else {
    buf.add8(Z_Addr_Group);               // DstAddrMode - 0x01 = GROUP_ADDRESS
    buf.add16(toGroup);
  }

  EZ_SendZDO(srcDevice, unbind ? ZDO_UNBIND_REQ : ZDO_BIND_REQ, buf.buf(), buf.len());
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
// Command `ZbBindState`
// `ZbBindState<x>` as index if it does not fit. If default, `1` starts at the beginning
//
void CmndZbBindState(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data);
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }
  uint8_t index = XdrvMailbox.index - 1;   // change default 1 to 0

#ifdef USE_ZIGBEE_ZNP
  SBuffer buf(10);
  buf.add8(Z_SREQ | Z_ZDO);             // 25
  buf.add8(ZDO_MGMT_BIND_REQ);          // 33
  buf.add16(shortaddr);                 // shortaddr
  buf.add8(index);                      // StartIndex = 0

  ZigbeeZNPSend(buf.getBuffer(), buf.len());
#endif // USE_ZIGBEE_ZNP


#ifdef USE_ZIGBEE_EZSP
  // ZDO message payload (see Zigbee spec 2.4.3.3.4)
  uint8_t buf[] = { index };           // index = 0

  EZ_SendZDO(shortaddr, ZDO_Mgmt_Bind_req, buf, sizeof(buf));
#endif // USE_ZIGBEE_EZSP

  ResponseCmndDone();
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
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data);
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }

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
  char *str = strtok_r(XdrvMailbox.data, ", ", &p);

  // parse first part, <device_id>
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data, true);  // in case of short_addr, it must be already registered
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }

  if (p == nullptr) {
    const char * friendlyName = zigbee_devices.getFriendlyName(shortaddr);
    Response_P(PSTR("{\"0x%04X\":{\"" D_JSON_ZIGBEE_NAME "\":\"%s\"}}"), shortaddr, friendlyName ? friendlyName : "");
  } else {
    if (strlen(p) > 32) { p[32] = 0x00; }     // truncate to 32 chars max
    zigbee_devices.setFriendlyName(shortaddr, p);
    Response_P(PSTR("{\"0x%04X\":{\"" D_JSON_ZIGBEE_NAME "\":\"%s\"}}"), shortaddr, p);
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
  char *str = strtok_r(XdrvMailbox.data, ", ", &p);

  // parse first part, <device_id>
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data, true);  // in case of short_addr, it must be already registered
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }

  if (p == nullptr) {
    const char * modelId = zigbee_devices.getModelId(shortaddr);
    Response_P(PSTR("{\"0x%04X\":{\"" D_JSON_ZIGBEE_MODELID "\":\"%s\"}}"), shortaddr, modelId ? modelId : "");
  } else {
    zigbee_devices.setModelId(shortaddr, p);
    Response_P(PSTR("{\"0x%04X\":{\"" D_JSON_ZIGBEE_MODELID "\":\"%s\"}}"), shortaddr, p);
  }
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
  char *str = strtok_r(XdrvMailbox.data, ", ", &p);

  // parse first part, <device_id>
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data, true);  // in case of short_addr, it must be already registered
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }

  if (p) {
    int8_t bulbtype = strtol(p, nullptr, 10);
    if (bulbtype > 5)  { bulbtype = 5; }
    if (bulbtype < -1) { bulbtype = -1; }
    zigbee_devices.setHueBulbtype(shortaddr, bulbtype);
  }
  String dump = zigbee_devices.dumpLightState(shortaddr);
  Response_P(PSTR("{\"" D_PRFX_ZB D_CMND_ZIGBEE_LIGHT "\":%s}"), dump.c_str());

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_ZB D_CMND_ZIGBEE_LIGHT));
  ResponseCmndDone();
}

//
// Command `ZbForget`
// Remove an old Zigbee device from the list of known devices, use ZigbeeStatus to know all registered devices
//
void CmndZbForget(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data);
  if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }

  // everything is good, we can send the command
  if (zigbee_devices.removeDevice(shortaddr)) {
    ResponseCmndDone();
  } else {
    ResponseCmndChar_P(PSTR("Unknown device"));
  }
}

//
// Command `ZbSave`
// Save Zigbee information to flash
//
void CmndZbSave(void) {
  if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  saveZigbeeDevices();
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
  DynamicJsonBuffer jsonBuf;
  const JsonVariant json_parsed = jsonBuf.parse((const char*) XdrvMailbox.data);   // const to force a copy of parameter
  const JsonVariant * json = &json_parsed;    // root of restore, to be changed if needed
  bool success = false;

  // check if parsing succeeded
  if (json_parsed.is<JsonObject>()) {
    success = json_parsed.as<const JsonObject&>().success();
  } else if (json_parsed.is<JsonArray>()) {
    success = json_parsed.as<const JsonArray&>().success();
  }
  if (!success) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

  // Check is root contains `ZbStatus<x>` key, if so change the root
  const JsonVariant * zbstatus = &startsWithCaseInsensitive(*json, PSTR("ZbStatus"));
  if (nullptr != zbstatus) {
    json = zbstatus;
  }

  // check if the root is an array
  if (json->is<JsonArray>()) {
    const JsonArray& arr = json->as<const JsonArray&>();
    for (auto elt : arr) {
      // call restore on each item
      int32_t res = zigbee_devices.deviceRestore(elt);
      if (res < 0) {
        ResponseCmndChar_P(PSTR("Restore failed"));
        return;
      }
    }
  } else if (json->is<JsonObject>()) {
    int32_t res = zigbee_devices.deviceRestore(*json);
    if (res < 0) {
      ResponseCmndChar_P(PSTR("Restore failed"));
      return;
    }
    // call restore on a single object
  } else {
    ResponseCmndChar_P(PSTR("Missing parameters"));
    return;
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

  uint16_t dstAddr = 0xFFFC;            // default addr

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
#endif // USE_ZIGBEE_EZSP

//
// Command `ZbStatus`
//
void CmndZbStatus(void) {
  if (ZigbeeSerial) {
    if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
    uint16_t shortaddr = zigbee_devices.parseDeviceParam(XdrvMailbox.data);
    if (XdrvMailbox.payload > 0) {
      if (BAD_SHORTADDR == shortaddr) { ResponseCmndChar_P(PSTR("Unknown device")); return; }
    }

    String dump = zigbee_devices.dump(XdrvMailbox.index, shortaddr);
    Response_P(PSTR("{\"%s%d\":%s}"), XdrvMailbox.command, XdrvMailbox.index, dump.c_str());
  }
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
  uint8_t     zb_txradio_dbm = Settings.zb_txradio_dbm;

  // if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  RemoveAllSpaces(XdrvMailbox.data);
  if (strlen(XdrvMailbox.data) > 0) {
    DynamicJsonBuffer jsonBuf;
    const JsonObject &json = jsonBuf.parseObject((const char*) XdrvMailbox.data);
    if (!json.success()) { ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON)); return; }

    // Channel
    const JsonVariant &val_channel = GetCaseInsensitive(json, PSTR("Channel"));
    if (nullptr != &val_channel) { zb_channel = strToUInt(val_channel); }
    if (zb_channel < 11) { zb_channel = 11; }
    if (zb_channel > 26) { zb_channel = 26; }
    // PanID
    const JsonVariant &val_pan_id = GetCaseInsensitive(json, PSTR("PanID"));
    if (nullptr != &val_pan_id) { zb_pan_id = strToUInt(val_pan_id); }
    // ExtPanID
    const JsonVariant &val_ext_pan_id = GetCaseInsensitive(json, PSTR("ExtPanID"));
    if (nullptr != &val_ext_pan_id) { zb_ext_panid = strtoull(val_ext_pan_id.as<const char*>(), nullptr, 0); }
    // KeyL
    const JsonVariant &val_key_l = GetCaseInsensitive(json, PSTR("KeyL"));
    if (nullptr != &val_key_l) { zb_precfgkey_l = strtoull(val_key_l.as<const char*>(), nullptr, 0); }
    // KeyH
    const JsonVariant &val_key_h = GetCaseInsensitive(json, PSTR("KeyH"));
    if (nullptr != &val_key_h) { zb_precfgkey_h = strtoull(val_key_h.as<const char*>(), nullptr, 0); }
    // TxRadio dBm
    const JsonVariant &val_txradio = GetCaseInsensitive(json, PSTR("TxRadio"));
    if (nullptr != &val_txradio) { zb_txradio_dbm = strToUInt(val_txradio); }

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
      restart_flag = 2;    // save and reboot
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

extern "C" {
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
      return 1;
    }
  }
}

void ZigbeeShow(bool json)
{
  if (json) {
    return;
#ifdef USE_WEBSERVER
  } else {
    uint32_t zigbee_num = zigbee_devices.devicesSize();
    if (!zigbee_num) { return; }
    if (zigbee_num > 255) { zigbee_num = 255; }

    // Calculate fixed column width for best visual result (Theos opinion)
    const uint8_t px_batt = 30;                         // Battery icon is 20px, add 10px as separator
    const uint8_t px_lqi = (strlen(D_LQI) + 4) * 10;        // LQI 254   = 70px

    WSContentSend_P(PSTR("</table>{t}"));  // Terminate current two column table and open new table
    WSContentSend_P(PSTR("<style>.bx{height:14px;width:14px;display:inline-block;border:1px solid currentColor;background-color:var(--cl,#fff)}</style>"));

    // sort elements by name, then by id
    uint8_t sorted_idx[zigbee_num];
    for (uint32_t i = 0; i < zigbee_num; i++) {
      sorted_idx[i] = i;
    }
    qsort(sorted_idx, zigbee_num, sizeof(sorted_idx[0]), device_cmp);

    for (uint32_t i = 0; i < zigbee_num; i++) {
      const Z_Device &device = zigbee_devices.devicesAt(sorted_idx[i]);
      uint16_t shortaddr = device.shortaddr;
      {   // exxplicit scope to free up stack allocated strings
        char *name = (char*) device.friendlyName;
        char sdevice[33];
        if (nullptr == name) {
          snprintf_P(sdevice, sizeof(sdevice), PSTR(D_DEVICE " 0x%04X"), shortaddr);
          name = sdevice;
        }

        char slqi[8];
        snprintf_P(slqi, sizeof(slqi), PSTR("-"));
        if (device.validLqi()) {
          snprintf_P(slqi, sizeof(slqi), PSTR("%d"), device.lqi);
        }

        char sbatt[64];
        snprintf_P(sbatt, sizeof(sbatt), PSTR("&nbsp;"));
        if (device.validBatteryPercent()) {
          snprintf_P(sbatt, sizeof(sbatt), PSTR("<i class=\"bt\" title=\"%d%%\" style=\"--bl:%dpx\"></i>"), device.batterypercent, changeUIntScale(device.batterypercent, 0, 100, 0, 14));
        }

        if (!i) {  // First row needs style info
          WSContentSend_PD(PSTR("<tr><td><b>%s</b></td><td style='width:%dpx'>%s</td><td style='width:%dpx'>" D_LQI " %s{e}"),
            name, px_batt, sbatt, px_lqi, slqi);
        } else {   // Following rows don't need style info so reducing ajax package
          WSContentSend_PD(PSTR("<tr><td><b>%s</b></td><td>%s</td><td>" D_LQI " %s{e}"), name, sbatt, slqi);
        }
      }

      // Sensor
      bool temperature_ok = device.validTemperature();
      bool humidity_ok    = device.validHumidity();
      bool pressure_ok    = device.validPressure();

      if (temperature_ok || humidity_ok || pressure_ok) {
        WSContentSend_P(PSTR("<tr><td colspan=\"3\">&#9478;"));
        if (temperature_ok) {
          char buf[12];
          dtostrf(device.temperature / 10.0f, 3, 1, buf);
          WSContentSend_PD(PSTR(" &#x2600;&#xFE0F; %s°C"), buf);
        }
        if (humidity_ok) {
          WSContentSend_P(PSTR(" &#x1F4A7; %d%%"), device.humidity);
        }
        if (pressure_ok) {
          WSContentSend_P(PSTR(" &#x26C5; %d hPa"), device.pressure);
        }
        WSContentSend_P(PSTR("{e}"));
      }

      // Light, switches and plugs
      bool power_ok = device.validPower();
      if (power_ok) {
        uint8_t channels = device.getLightChannels();
        if (0xFF == channels) { channels = 5; }     // if number of channel is unknown, display all known attributes
        WSContentSend_P(PSTR("<tr><td colspan=\"3\">&#9478; %s"), device.getPower() ? PSTR(D_ON) : PSTR(D_OFF));
        if (device.validDimmer() && (channels >= 1)) {
          WSContentSend_P(PSTR(" &#128261; %d%%"), changeUIntScale(device.dimmer,0,254,0,100));
        }
        if (device.validCT() && ((channels == 2) || (channels == 5))) {
          uint32_t ct_k = (((1000000 / device.ct) + 25) / 50) * 50;
          WSContentSend_P(PSTR(" <span title=\"CT %d\"><small>&#9898; </small>%dK</span>"), device.ct, ct_k);
        }
        if (device.validHue() && device.validSat() && (channels >= 3)) {
          uint8_t r,g,b;
          uint8_t sat = changeUIntScale(device.sat, 0, 254, 0, 255);    // scale to 0..255
          LightStateClass::HsToRgb(device.hue, sat, &r, &g, &b);
          WSContentSend_P(PSTR(" <i class=\"bx\" style=\"--cl:#%02X%02X%02X\"></i>#%02X%02X%02X"), r,g,b,r,g,b);
        } else if (device.validX() && device.validY() && (channels >= 3)) {
          uint8_t r,g,b;
          LightStateClass::XyToRgb(device.x / 65535.0f, device.y / 65535.0f, &r, &g, &b);
          WSContentSend_P(PSTR(" <i class=\"bx\" style=\"--cl:#%02X%02X%02X\"></i> #%02X%02X%02X"), r,g,b,r,g,b);
        }
        if (device.validMainsPower() || device.validMainsVoltage()) {
          WSContentSend_P(PSTR(" &#9889; "));
          if (device.validMainsVoltage()) {
            WSContentSend_P(PSTR(" %dV"), device.mains_voltage);
          }
          if (device.validMainsPower()) {
            WSContentSend_P(PSTR(" %dW"), device.mains_power);
          }
        }
      }
    }

    WSContentSend_P(PSTR("</table>{t}"));  // Terminate current multi column table and open new table
#endif
  }
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
        }
        if (zigbee.state_machine) {
          ZigbeeStateMachine_Run();
        }
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        ZigbeeShow(false);
        break;
#ifdef USE_ZIGBEE_EZSP
      // GUI xmodem
      case FUNC_WEB_ADD_HANDLER:
        Webserver->on("/" WEB_HANDLE_ZIGBEE_XFER, HandleZigbeeXfer);
        break;
#endif  // USE_ZIGBEE_EZSP
#endif  // USE_WEBSERVER
      case FUNC_PRE_INIT:
        ZigbeeInit();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kZbCommands, ZigbeeCommand);
        break;
    }
  }
  return result;
}

#endif // USE_ZIGBEE
