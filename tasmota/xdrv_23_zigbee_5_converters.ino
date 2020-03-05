/*
  xdrv_23_zigbee_converters.ino - zigbee support for Tasmota

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
 * ZCL
\*********************************************************************************************/

typedef union ZCLHeaderFrameControl_t {
  struct {
    uint8_t frame_type : 2;           // 00 = across entire profile, 01 = cluster specific
    uint8_t manuf_specific : 1;       // Manufacturer Specific Sub-field
    uint8_t direction : 1;            // 0 = tasmota to zigbee, 1 = zigbee to tasmota
    uint8_t disable_def_resp : 1;     // don't send back default response
    uint8_t reserved : 3;
  } b;
  uint32_t d8;                         // raw 8 bits field
} ZCLHeaderFrameControl_t;


class ZCLFrame {
public:

  ZCLFrame(uint8_t frame_control, uint16_t manuf_code, uint8_t transact_seq, uint8_t cmd_id,
    const char *buf, size_t buf_len, uint16_t clusterid, uint16_t groupid,
    uint16_t srcaddr, uint8_t srcendpoint, uint8_t dstendpoint, uint8_t wasbroadcast,
    uint8_t linkquality, uint8_t securityuse, uint8_t seqnumber,
    uint32_t timestamp):
    _cmd_id(cmd_id), _manuf_code(manuf_code), _transact_seq(transact_seq),
    _payload(buf_len ? buf_len : 250),      // allocate the data frame from source or preallocate big enough
    _cluster_id(clusterid), _group_id(groupid),
    _srcaddr(srcaddr), _srcendpoint(srcendpoint), _dstendpoint(dstendpoint), _wasbroadcast(wasbroadcast),
    _linkquality(linkquality), _securityuse(securityuse), _seqnumber(seqnumber),
    _timestamp(timestamp)
    {
      _frame_control.d8 = frame_control;
      _payload.addBuffer(buf, buf_len);
    };


  void log(void) {
    char hex_char[_payload.len()*2+2];
		ToHex_P((unsigned char*)_payload.getBuffer(), _payload.len(), hex_char, sizeof(hex_char));
    Response_P(PSTR("{\"" D_JSON_ZIGBEEZCL_RECEIVED "\":{"
                    "\"groupid\":%d," "\"clusterid\":%d," "\"srcaddr\":\"0x%04X\","
                    "\"srcendpoint\":%d," "\"dstendpoint\":%d," "\"wasbroadcast\":%d,"
                    "\"" D_CMND_ZIGBEE_LINKQUALITY "\":%d," "\"securityuse\":%d," "\"seqnumber\":%d,"
                    "\"timestamp\":%d,"
                    "\"fc\":\"0x%02X\",\"manuf\":\"0x%04X\",\"transact\":%d,"
                    "\"cmdid\":\"0x%02X\",\"payload\":\"%s\"}}"),
                    _group_id, _cluster_id, _srcaddr,
                    _srcendpoint, _dstendpoint, _wasbroadcast,
                    _linkquality, _securityuse, _seqnumber,
                    _timestamp,
                    _frame_control, _manuf_code, _transact_seq, _cmd_id,
                    hex_char);
    if (Settings.flag3.tuya_serial_mqtt_publish) {
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR));
      XdrvRulesProcess();
    } else {
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "%s"), mqtt_data);
    }
  }

  static ZCLFrame parseRawFrame(const SBuffer &buf, uint8_t offset, uint8_t len, uint16_t clusterid, uint16_t groupid,
                                uint16_t srcaddr, uint8_t srcendpoint, uint8_t dstendpoint, uint8_t wasbroadcast,
                                uint8_t linkquality, uint8_t securityuse, uint8_t seqnumber,
                                uint32_t timestamp) { // parse a raw frame and build the ZCL frame object
    uint32_t i = offset;
    ZCLHeaderFrameControl_t frame_control;
    uint16_t manuf_code = 0;
    uint8_t transact_seq;
    uint8_t cmd_id;

    frame_control.d8 = buf.get8(i++);
    if (frame_control.b.manuf_specific) {
      manuf_code = buf.get16(i);
      i += 2;
    }
    transact_seq = buf.get8(i++);
    cmd_id = buf.get8(i++);
    ZCLFrame zcl_frame(frame_control.d8, manuf_code, transact_seq, cmd_id,
                       (const char *)(buf.buf() + i), len + offset - i,
                       clusterid, groupid,
                       srcaddr, srcendpoint, dstendpoint, wasbroadcast,
                       linkquality, securityuse, seqnumber,
                       timestamp);
    return zcl_frame;
  }

  bool isClusterSpecificCommand(void) {
    return _frame_control.b.frame_type & 1;
  }

  static void generateAttributeName(const JsonObject& json, uint16_t cluster, uint16_t attr, char *key, size_t key_len);
  void parseRawAttributes(JsonObject& json, uint8_t offset = 0);
  void parseReadAttributes(JsonObject& json, uint8_t offset = 0);
  void parseResponse(void);
  void parseClusterSpecificCommand(JsonObject& json, uint8_t offset = 0);
  void postProcessAttributes(uint16_t shortaddr, JsonObject& json);

  inline void setGroupId(uint16_t groupid) {
    _group_id = groupid;
  }

  inline void setClusterId(uint16_t clusterid) {
    _cluster_id = clusterid;
  }

  inline uint8_t getCmdId(void) const {
    return _cmd_id;
  }

  inline uint16_t getClusterId(void) const {
    return _cluster_id;
  }

  inline uint16_t getSrcEndpoint(void) const {
    return _srcendpoint;
  }

  const SBuffer &getPayload(void) const {
    return _payload;
  }

  uint16_t getManufCode(void) const {
    return _manuf_code;
  }

private:
  ZCLHeaderFrameControl_t _frame_control = { .d8 = 0 };
  uint16_t                _manuf_code = 0;      // optional
  uint8_t                 _transact_seq = 0;    // transaction sequence number
  uint8_t                 _cmd_id = 0;
  uint16_t                _cluster_id = 0;
  uint16_t                _group_id = 0;
  SBuffer                 _payload;
  // information from decoded ZCL frame
  uint16_t                _srcaddr;
  uint8_t                 _srcendpoint;
  uint8_t                 _dstendpoint;
  uint8_t                 _wasbroadcast;
  uint8_t                 _linkquality;
  uint8_t                 _securityuse;
  uint8_t                 _seqnumber;
  uint32_t                _timestamp;
};

// Zigbee ZCL converters

// from https://github.com/Koenkk/zigbee-shepherd-converters/blob/638d29f0cace6343052b9a4e7fd60980fa785479/converters/fromZigbee.js#L55
// Input voltage in mV, i.e. 3000 = 3.000V
// Output percentage from 0 to 100 as int
uint8_t toPercentageCR2032(uint32_t voltage) {
  uint32_t percentage;
  if (voltage < 2100) {
      percentage = 0;
  } else if (voltage < 2440) {
      percentage = 6 - ((2440 - voltage) * 6) / 340;
  } else if (voltage < 2740) {
      percentage = 18 - ((2740 - voltage) * 12) / 300;
  } else if (voltage < 2900) {
      percentage = 42 - ((2900 - voltage) * 24) / 160;
  } else if (voltage < 3000) {
      percentage = 100 - ((3000 - voltage) * 58) / 100;
  } else if (voltage >= 3000) {
      percentage = 100;
  }
  return percentage;
}


uint32_t parseSingleAttribute(JsonObject& json, char *attrid_str, class SBuffer &buf,
                              uint32_t offset, uint32_t len) {

  uint32_t i = offset;
  uint32_t attrtype = buf.get8(i++);

  // fallback - enter a null value
  json[attrid_str] = (char*) nullptr;

  // now parse accordingly to attr type
  switch (attrtype) {
    case 0x00:      // nodata
    case 0xFF:      // unk
      break;
    case 0x10:      // bool
      {
        uint8_t val_bool = buf.get8(i++);
        if (0xFF != val_bool) {
          json[attrid_str] = (bool) (val_bool ? true : false);
        }
      }
      break;
    case 0x20:      // uint8
      {
        uint8_t uint8_val = buf.get8(i);
        i += 1;
        if (0xFF != uint8_val) {
          json[attrid_str] = uint8_val;
        }
      }
      break;
    case 0x21:      // uint16
      {
        uint16_t uint16_val = buf.get16(i);
        i += 2;
        if (0xFFFF != uint16_val) {
          json[attrid_str] = uint16_val;
        }
      }
      break;
    case 0x23:      // uint32
      {
        uint32_t uint32_val = buf.get32(i);
        i += 4;
        if (0xFFFFFFFF != uint32_val) {
          json[attrid_str] = uint32_val;
        }
      }
      break;
    // Note: uint40, uint48, uint56, uint64 are stored as Hex
    case 0x24:    // uint40
    case 0x25:    // uint48
    case 0x26:    // uint56
    case 0x27:    // uint64
      {
        uint8_t len = attrtype - 0x1F;   // 5 - 8
        // print as HEX
        char hex[2*len+1];
        ToHex_P(buf.buf(i), len, hex, sizeof(hex));
        json[attrid_str] = hex;
        i += len;
      }
      break;
    case 0x28:      // uint8
      {
        int8_t int8_val = buf.get8(i);
        i += 1;
        if (0x80 != int8_val) {
          json[attrid_str] = int8_val;
        }
      }
      break;
    case 0x29:      // uint16
      {
        int16_t int16_val = buf.get16(i);
        i += 2;
        if (0x8000 != int16_val) {
          json[attrid_str] = int16_val;
        }
      }
      break;
    case 0x2B:      // uint16
      {
        int32_t int32_val = buf.get32(i);
        i += 4;
        if (0x80000000 != int32_val) {
          json[attrid_str] = int32_val;
        }
      }
      break;
    // Note: int40, int48, int56, int64 are not stored as Hex
    case 0x2C:    // int40
    case 0x2D:    // int48
    case 0x2E:    // int56
    case 0x2F:    // int64
      {
        uint8_t len = attrtype - 0x27;   // 5 - 8
        // print as HEX
        char hex[2*len+1];
        ToHex_P(buf.buf(i), len, hex, sizeof(hex));
        json[attrid_str] = hex;
        i += len;
      }
      break;

    case 0x41:      // octet string, 1 byte len
    case 0x42:      // char string, 1 byte len
    case 0x43:      // octet string, 2 bytes len
    case 0x44:      // char string, 2 bytes len
      // For strings, default is to try to do a real string, but reverts to octet stream if null char is present or on some exceptions
      {
        bool parse_as_string = true;
        uint32_t len = (attrtype <= 0x42) ? buf.get8(i) : buf.get16(i);    // len is 8 or 16 bits
        i += (attrtype <= 0x42) ? 1 : 2;                                   // increment pointer
        if (i + len > buf.len()) {        // make sure we don't get past the buffer
          len = buf.len() - i;
        }

        // check if we can safely use a string
        if ((0x41 == attrtype) || (0x43 == attrtype)) { parse_as_string = false; }
        // else {
        //   for (uint32_t j = 0; j < len; j++) {
        //     if (0x00 == buf.get8(i+j)) {
        //       parse_as_string = false;
        //       break;
        //     }
        //   }
        // }

        if (parse_as_string) {
          char str[len+1];
          strncpy(str, buf.charptr(i), len);
          str[len] = 0x00;
          json[attrid_str] = str;
        } else {
          // print as HEX
          char hex[2*len+1];
          ToHex_P(buf.buf(i), len, hex, sizeof(hex));
          json[attrid_str] = hex;
        }

        i += len;
        break;
      }
      i += buf.get8(i) + 1;
      break;

    case 0x08:      // data8
    case 0x18:      // map8
      {
        uint8_t uint8_val = buf.get8(i);
        i += 1;
        json[attrid_str] = uint8_val;
      }
      break;
    case 0x09:      // data16
    case 0x19:      // map16
      {
        uint16_t uint16_val = buf.get16(i);
        i += 2;
        json[attrid_str] = uint16_val;
      }
      break;
    case 0x0B:      // data32
    case 0x1B:      // map32
      {
        uint32_t uint32_val = buf.get32(i);
        i += 4;
        json[attrid_str] = uint32_val;
      }
      break;
    // enum
    case 0x30:      // enum8
    case 0x31:      // enum16
      i += attrtype - 0x2F;
      break;

    // TODO
    case 0x39:      // float
      {
        uint32_t uint32_val = buf.get32(i);
        float  * float_val = (float*) &uint32_val;
        i += 4;
        json[attrid_str] = *float_val;
      }
      break;

    case 0xE0:      // ToD
    case 0xE1:      // date
    case 0xE2:      // UTC
      i += 4;
      break;

    case 0xE8:      // clusterId
    case 0xE9:      // attribId
      i += 2;
      break;
    case 0xEA:      // bacOID
      i += 4;
      break;

    case 0xF0:      // EUI64
      i += 8;
      break;
    case 0xF1:      // key128
      i += 16;
      break;

    // Other un-implemented data types
    case 0x0A:      // data24
    case 0x0C:      // data40
    case 0x0D:      // data48
    case 0x0E:      // data56
    case 0x0F:      // data64
      i += attrtype - 0x07;   // 2-8
      break;
    // map<x>
    case 0x1A:      // map24
    case 0x1C:      // map40
    case 0x1D:      // map48
    case 0x1E:      // map56
    case 0x1F:      // map64
      i += attrtype - 0x17;
      break;
    // semi
    case 0x38:      // semi (float on 2 bytes)
      i += 2;
      break;
    case 0x3A:      // double precision
      {
        uint64_t uint64_val = buf.get64(i);
        double  * double_val = (double*) &uint64_val;
        i += 8;
        json[attrid_str] = *double_val;
      }
      break;
  }

  // String pp;    // pretty print
  // json[attrid_str].prettyPrintTo(pp);
  // // now store the attribute
  // AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "ZCL attribute decoded, id %s, type 0x%02X, val=%s"),
  //                                attrid_str, attrtype, pp.c_str());
  return i - offset;    // how much have we increased the index
}

// Generate an attribute name based on cluster number, attribute, and suffix if duplicates
void ZCLFrame::generateAttributeName(const JsonObject& json, uint16_t cluster, uint16_t attr, char *key, size_t key_len) {
  uint32_t suffix = 1;

  snprintf_P(key, key_len, PSTR("%04X/%04X"), cluster, attr);
  while (json.containsKey(key)) {
    suffix++;
    snprintf_P(key, key_len, PSTR("%04X/%04X+%d"), cluster, attr, suffix);    // add "0008/0001+2" suffix if duplicate
  }
}

// First pass, parse all attributes in their native format
void ZCLFrame::parseRawAttributes(JsonObject& json, uint8_t offset) {
  uint32_t i = offset;
  uint32_t len = _payload.len();

  while (len >= i + 3) {
    uint16_t attrid = _payload.get16(i);
    i += 2;

    char key[16];
    generateAttributeName(json, _cluster_id, attrid, key, sizeof(key));

    // exception for Xiaomi lumi.weather - specific field to be treated as octet and not char
    if ((0x0000 == _cluster_id) && (0xFF01 == attrid)) {
      if (0x42 == _payload.get8(i)) {
        _payload.set8(i, 0x41);   // change type from 0x42 to 0x41
      }
    }
    i += parseSingleAttribute(json, key, _payload, i, len);
  }
}

// ZCL_READ_ATTRIBUTES_RESPONSE
void ZCLFrame::parseReadAttributes(JsonObject& json, uint8_t offset) {
  uint32_t i = offset;
  uint32_t len = _payload.len();

  while (len - i >= 4) {
    uint16_t attrid = _payload.get16(i);
    i += 2;
    uint8_t status = _payload.get8(i++);

    if (0 == status) {
      char key[16];
      generateAttributeName(json, _cluster_id, attrid, key, sizeof(key));

      i += parseSingleAttribute(json, key, _payload, i, len);
    }
  }
}

// ZCL_DEFAULT_RESPONSE
void ZCLFrame::parseResponse(void) {
  if (_payload.len() < 2) { return; }   // wrong format
  uint8_t cmd = _payload.get8(0);
  uint8_t status = _payload.get8(1);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  // "Device"
  char s[12];
  snprintf_P(s, sizeof(s), PSTR("0x%04X"), _srcaddr);
  json[F(D_JSON_ZIGBEE_DEVICE)] = s;
  // "Name"
  const String * friendlyName = zigbee_devices.getFriendlyName(_srcaddr);
  if (friendlyName) {
    json[F(D_JSON_ZIGBEE_NAME)] = *friendlyName;
  }
  // "Command"
  snprintf_P(s, sizeof(s), PSTR("%04X!%02X"), _cluster_id, cmd);
  json[F(D_JSON_ZIGBEE_CMD)] = s;
  // "Status"
  json[F(D_JSON_ZIGBEE_STATUS)] = status;
  // "StatusMessage"
  const __FlashStringHelper* statm = getZigbeeStatusMessage(status);
  if (statm) {
    json[F(D_JSON_ZIGBEE_STATUS_MSG)] = statm;
  }
  // Add Endpoint
  json[F(D_CMND_ZIGBEE_ENDPOINT)] = _srcendpoint;
  // Add Group if non-zero
  if (_group_id) {
    json[F(D_CMND_ZIGBEE_GROUP)] = _group_id;
  }
  // Add linkquality
  json[F(D_CMND_ZIGBEE_LINKQUALITY)] = _linkquality;

  String msg("");
  msg.reserve(100);
  json.printTo(msg);
  Response_P(PSTR("{\"" D_JSON_ZIGBEE_RESPONSE "\":%s}"), msg.c_str());
  MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
  XdrvRulesProcess();
}


// Parse non-normalized attributes
void ZCLFrame::parseClusterSpecificCommand(JsonObject& json, uint8_t offset) {
  convertClusterSpecific(json, _cluster_id, _cmd_id, _frame_control.b.direction, _payload);
}

// return value:
// 0 = keep initial value
// 1 = remove initial value
typedef int32_t (*Z_AttrConverter)(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr);
typedef struct Z_AttributeConverter {
  uint16_t cluster;
  uint16_t attribute;
  const char * name;
  Z_AttrConverter func;
} Z_AttributeConverter;

// list of post-processing directives
const Z_AttributeConverter Z_PostProcess[] PROGMEM = {
  { 0x0000, 0x0000,  "ZCLVersion",           &Z_Copy },
  { 0x0000, 0x0001,  "AppVersion",           &Z_Copy },
  { 0x0000, 0x0002,  "StackVersion",         &Z_Copy },
  { 0x0000, 0x0003,  "HWVersion",            &Z_Copy },
  { 0x0000, 0x0004,  "Manufacturer",         &Z_ManufKeep },    // record Manufacturer
  { 0x0000, 0x0005,  D_JSON_MODEL D_JSON_ID, &Z_ModelKeep },    // record Model
  { 0x0000, 0x0006,  "DateCode",             &Z_Copy },
  { 0x0000, 0x0007,  "PowerSource",          &Z_Copy },
  { 0x0000, 0x4000,  "SWBuildID",            &Z_Copy },
  { 0x0000, 0xFFFF,  nullptr,                &Z_Remove },    // Remove all other values
  // Cmd 0x0A - Cluster 0x0000, attribute 0xFF01 - proprietary
  { 0x0000, 0xFF01,  nullptr,                &Z_AqaraSensor },    // Occupancy (map8)

  // Power Configuration cluster
  { 0x0001, 0x0000,  "MainsVoltage",         &Z_Copy },
  { 0x0001, 0x0001,  "MainsFrequency",       &Z_Copy },
  { 0x0001, 0x0020,  "BatteryVoltage",       &Z_FloatDiv10 },
  { 0x0001, 0x0021,  "BatteryPercentageRemaining",&Z_Copy },

  // Device Temperature Configuration cluster
  { 0x0002, 0x0000,  "CurrentTemperature",   &Z_Copy },
  { 0x0002, 0x0001,  "MinTempExperienced",   &Z_Copy },
  { 0x0002, 0x0002,  "MaxTempExperienced",   &Z_Copy },
  { 0x0002, 0x0003,  "OverTempTotalDwell",   &Z_Copy },

  // On/off cluster
  { 0x0006, 0x0000,  "Power",                &Z_Copy },
  { 0x0006, 0x8000,  "Power",                &Z_Copy },   // See 7280

  // On/Off Switch Configuration cluster
  { 0x0007, 0x0000,  "SwitchType",           &Z_Copy },

  // Level Control cluster
  { 0x0008, 0x0000,  "Dimmer",               &Z_Copy },
  // { 0x0008, 0x0001,  "RemainingTime",        &Z_Copy },
  // { 0x0008, 0x0010,  "OnOffTransitionTime",  &Z_Copy },
  // { 0x0008, 0x0011,  "OnLevel",              &Z_Copy },
  // { 0x0008, 0x0012,  "OnTransitionTime",     &Z_Copy },
  // { 0x0008, 0x0013,  "OffTransitionTime",    &Z_Copy },
  // { 0x0008, 0x0014,  "DefaultMoveRate",      &Z_Copy },

  // Alarms cluster
  { 0x0009, 0x0000,  "AlarmCount",           &Z_Copy },
  // Time cluster
  { 0x000A, 0x0000,  "Time",                 &Z_Copy },
  { 0x000A, 0x0001,  "TimeStatus",           &Z_Copy },
  { 0x000A, 0x0002,  "TimeZone",             &Z_Copy },
  { 0x000A, 0x0003,  "DstStart",             &Z_Copy },
  { 0x000A, 0x0004,  "DstStart",             &Z_Copy },
  { 0x000A, 0x0005,  "DstShift",             &Z_Copy },
  { 0x000A, 0x0006,  "StandardTime",         &Z_Copy },
  { 0x000A, 0x0007,  "LocalTime",            &Z_Copy },
  { 0x000A, 0x0008,  "LastSetTime",          &Z_Copy },
  { 0x000A, 0x0009,  "ValidUntilTime",       &Z_Copy },
  // RSSI Location cluster
  { 0x000B, 0x0000,  "LocationType",         &Z_Copy },
  { 0x000B, 0x0000,  "LocationMethod",       &Z_Copy },
  { 0x000B, 0x0000,  "LocationAge",          &Z_Copy },
  { 0x000B, 0x0000,  "QualityMeasure",       &Z_Copy },
  { 0x000B, 0x0000,  "NumberOfDevices",      &Z_Copy },
  // Analog Input cluster
  { 0x000C, 0x0004,  "AnalogInActiveText",   &Z_Copy },
  { 0x000C, 0x001C,  "AnalogInDescription",  &Z_Copy },
  { 0x000C, 0x002E,  "AnalogInInactiveText", &Z_Copy },
  { 0x000C, 0x0041,  "AnalogInMaxValue",     &Z_Copy },
  { 0x000C, 0x0045,  "AnalogInMinValue",     &Z_Copy },
  { 0x000C, 0x0051,  "AnalogInOutOfService", &Z_Copy },
  { 0x000C, 0x0055,  "AqaraRotate",          &Z_Copy },
  { 0x000C, 0x0057,  "AnalogInPriorityArray",&Z_Copy },
  { 0x000C, 0x0067,  "AnalogInReliability",  &Z_Copy },
  { 0x000C, 0x0068,  "AnalogInRelinquishDefault",&Z_Copy },
  { 0x000C, 0x006A,  "AnalogInResolution",   &Z_Copy },
  { 0x000C, 0x006F,  "AnalogInStatusFlags",  &Z_Copy },
  { 0x000C, 0x0075,  "AnalogInEngineeringUnits",&Z_Copy },
  { 0x000C, 0x0100,  "AnalogInApplicationType",&Z_Copy },
  { 0x000C, 0xFF05,  "Aqara_FF05",           &Z_Copy },
  // Analog Output cluster
  { 0x000D, 0x001C,  "AnalogOutDescription", &Z_Copy },
  { 0x000D, 0x0041,  "AnalogOutMaxValue",    &Z_Copy },
  { 0x000D, 0x0045,  "AnalogOutMinValue",    &Z_Copy },
  { 0x000D, 0x0051,  "AnalogOutOutOfService",&Z_Copy },
  { 0x000D, 0x0055,  "AnalogOutValue",       &Z_Copy },
  { 0x000D, 0x0057,  "AnalogOutPriorityArray",&Z_Copy },
  { 0x000D, 0x0067,  "AnalogOutReliability", &Z_Copy },
  { 0x000D, 0x0068,  "AnalogOutRelinquishDefault",&Z_Copy },
  { 0x000D, 0x006A,  "AnalogOutResolution",  &Z_Copy },
  { 0x000D, 0x006F,  "AnalogOutStatusFlags", &Z_Copy },
  { 0x000D, 0x0075,  "AnalogOutEngineeringUnits",&Z_Copy },
  { 0x000D, 0x0100,  "AnalogOutApplicationType",&Z_Copy },
  // Analog Value cluster
  { 0x000E, 0x001C,  "AnalogDescription",    &Z_Copy },
  { 0x000E, 0x0051,  "AnalogOutOfService",   &Z_Copy },
  { 0x000E, 0x0055,  "AnalogValue",          &Z_Copy },
  { 0x000E, 0x0057,  "AnalogPriorityArray",  &Z_Copy },
  { 0x000E, 0x0067,  "AnalogReliability",    &Z_Copy },
  { 0x000E, 0x0068,  "AnalogRelinquishDefault",&Z_Copy },
  { 0x000E, 0x006F,  "AnalogStatusFlags",    &Z_Copy },
  { 0x000E, 0x0075,  "AnalogEngineeringUnits",&Z_Copy },
  { 0x000E, 0x0100,  "AnalogApplicationType",&Z_Copy },
  // Binary Input cluster
  { 0x000F, 0x0004,  "BinaryInActiveText",  &Z_Copy },
  { 0x000F, 0x001C,  "BinaryInDescription", &Z_Copy },
  { 0x000F, 0x002E,  "BinaryInInactiveText",&Z_Copy },
  { 0x000F, 0x0051,  "BinaryInOutOfService",&Z_Copy },
  { 0x000F, 0x0054,  "BinaryInPolarity",    &Z_Copy },
  { 0x000F, 0x0055,  "BinaryInValue",       &Z_Copy },
  { 0x000F, 0x0057,  "BinaryInPriorityArray",&Z_Copy },
  { 0x000F, 0x0067,  "BinaryInReliability", &Z_Copy },
  { 0x000F, 0x006F,  "BinaryInStatusFlags", &Z_Copy },
  { 0x000F, 0x0100,  "BinaryInApplicationType",&Z_Copy },
  // Binary Output cluster
  { 0x0010, 0x0004,  "BinaryOutActiveText",  &Z_Copy },
  { 0x0010, 0x001C,  "BinaryOutDescription", &Z_Copy },
  { 0x0010, 0x002E,  "BinaryOutInactiveText",&Z_Copy },
  { 0x0010, 0x0042,  "BinaryOutMinimumOffTime",&Z_Copy },
  { 0x0010, 0x0043,  "BinaryOutMinimumOnTime",&Z_Copy },
  { 0x0010, 0x0051,  "BinaryOutOutOfService",&Z_Copy },
  { 0x0010, 0x0054,  "BinaryOutPolarity",    &Z_Copy },
  { 0x0010, 0x0055,  "BinaryOutValue",       &Z_Copy },
  { 0x0010, 0x0057,  "BinaryOutPriorityArray",&Z_Copy },
  { 0x0010, 0x0067,  "BinaryOutReliability", &Z_Copy },
  { 0x0010, 0x0068,  "BinaryOutRelinquishDefault",&Z_Copy },
  { 0x0010, 0x006F,  "BinaryOutStatusFlags", &Z_Copy },
  { 0x0010, 0x0100,  "BinaryOutApplicationType",&Z_Copy },
  // Binary Value cluster
  { 0x0011, 0x0004,  "BinaryActiveText",     &Z_Copy },
  { 0x0011, 0x001C,  "BinaryDescription",    &Z_Copy },
  { 0x0011, 0x002E,  "BinaryInactiveText",   &Z_Copy },
  { 0x0011, 0x0042,  "BinaryMinimumOffTime", &Z_Copy },
  { 0x0011, 0x0043,  "BinaryMinimumOnTime",  &Z_Copy },
  { 0x0011, 0x0051,  "BinaryOutOfService",   &Z_Copy },
  { 0x0011, 0x0055,  "BinaryValue",          &Z_Copy },
  { 0x0011, 0x0057,  "BinaryPriorityArray",  &Z_Copy },
  { 0x0011, 0x0067,  "BinaryReliability",    &Z_Copy },
  { 0x0011, 0x0068,  "BinaryRelinquishDefault",&Z_Copy },
  { 0x0011, 0x006F,  "BinaryStatusFlags",    &Z_Copy },
  { 0x0011, 0x0100,  "BinaryApplicationType",&Z_Copy },
  // Multistate Input cluster
  { 0x0012, 0x000E,  "MultiInStateText",     &Z_Copy },
  { 0x0012, 0x001C,  "MultiInDescription",   &Z_Copy },
  { 0x0012, 0x004A,  "MultiInNumberOfStates",&Z_Copy },
  { 0x0012, 0x0051,  "MultiInOutOfService",  &Z_Copy },
  { 0x0012, 0x0055,  "MultiInValue",         &Z_AqaraCube },
  { 0x0012, 0x0067,  "MultiInReliability",   &Z_Copy },
  { 0x0012, 0x006F,  "MultiInStatusFlags",   &Z_Copy },
  { 0x0012, 0x0100,  "MultiInApplicationType",&Z_Copy },
  // Multistate output
  { 0x0013, 0x000E,  "MultiOutStateText",    &Z_Copy },
  { 0x0013, 0x001C,  "MultiOutDescription",  &Z_Copy },
  { 0x0013, 0x004A,  "MultiOutNumberOfStates",&Z_Copy },
  { 0x0013, 0x0051,  "MultiOutOutOfService", &Z_Copy },
  { 0x0013, 0x0055,  "MultiOutValue",        &Z_Copy },
  { 0x0013, 0x0057,  "MultiOutPriorityArray",&Z_Copy },
  { 0x0013, 0x0067,  "MultiOutReliability",  &Z_Copy },
  { 0x0013, 0x0068,  "MultiOutRelinquishDefault",&Z_Copy },
  { 0x0013, 0x006F,  "MultiOutStatusFlags",  &Z_Copy },
  { 0x0013, 0x0100,  "MultiOutApplicationType",&Z_Copy },
  // Multistate Value cluster
  { 0x0014, 0x000E,  "MultiStateText",       &Z_Copy },
  { 0x0014, 0x001C,  "MultiDescription",     &Z_Copy },
  { 0x0014, 0x004A,  "MultiNumberOfStates",  &Z_Copy },
  { 0x0014, 0x0051,  "MultiOutOfService",    &Z_Copy },
  { 0x0014, 0x0055,  "MultiValue",           &Z_Copy },
  { 0x0014, 0x0067,  "MultiReliability",     &Z_Copy },
  { 0x0014, 0x0068,  "MultiRelinquishDefault",&Z_Copy },
  { 0x0014, 0x006F,  "MultiStatusFlags",     &Z_Copy },
  { 0x0014, 0x0100,  "MultiApplicationType", &Z_Copy },
  // Power Profile cluster
  { 0x001A, 0x0000,  "TotalProfileNum",      &Z_Copy },
  { 0x001A, 0x0001,  "MultipleScheduling",   &Z_Copy },
  { 0x001A, 0x0002,  "EnergyFormatting",     &Z_Copy },
  { 0x001A, 0x0003,  "EnergyRemote",         &Z_Copy },
  { 0x001A, 0x0004,  "ScheduleMode",         &Z_Copy },
  // Poll Control cluster
  { 0x0020, 0x0000,  "CheckinInterval",      &Z_Copy },
  { 0x0020, 0x0001,  "LongPollInterval",     &Z_Copy },
  { 0x0020, 0x0002,  "ShortPollInterval",    &Z_Copy },
  { 0x0020, 0x0003,  "FastPollTimeout",      &Z_Copy },
  { 0x0020, 0x0004,  "CheckinIntervalMin",   &Z_Copy },
  { 0x0020, 0x0005,  "LongPollIntervalMin",  &Z_Copy },
  { 0x0020, 0x0006,  "FastPollTimeoutMax",   &Z_Copy },
  // Shade Configuration cluster
  { 0x0100, 0x0000,  "PhysicalClosedLimit",  &Z_Copy },
  { 0x0100, 0x0001,  "MotorStepSize",        &Z_Copy },
  { 0x0100, 0x0002,  "Status",               &Z_Copy },
  { 0x0100, 0x0010,  "ClosedLimit",          &Z_Copy },
  { 0x0100, 0x0011,  "Mode",                 &Z_Copy },
  // Door Lock cluster
  { 0x0101, 0x0000,  "LockState",            &Z_Copy },
  { 0x0101, 0x0001,  "LockType",             &Z_Copy },
  { 0x0101, 0x0002,  "ActuatorEnabled",      &Z_Copy },
  { 0x0101, 0x0003,  "DoorState",            &Z_Copy },
  { 0x0101, 0x0004,  "DoorOpenEvents",       &Z_Copy },
  { 0x0101, 0x0005,  "DoorClosedEvents",     &Z_Copy },
  { 0x0101, 0x0006,  "OpenPeriod",           &Z_Copy },
  // Aqara Lumi Vibration Sensor
  { 0x0101, 0x0055,  "AqaraVibrationMode",   &Z_AqaraVibration },
  { 0x0101, 0x0503,  "AqaraVibrationsOrAngle", &Z_Copy },
  { 0x0101, 0x0505,  "AqaraVibration505",    &Z_Copy },
  { 0x0101, 0x0508,  "AqaraAccelerometer",   &Z_AqaraVibration },
  // Window Covering cluster
  { 0x0102, 0x0000,  "WindowCoveringType",   &Z_Copy },
  { 0x0102, 0x0001,  "PhysicalClosedLimitLift",&Z_Copy },
  { 0x0102, 0x0002,  "PhysicalClosedLimitTilt",&Z_Copy },
  { 0x0102, 0x0003,  "CurrentPositionLift",  &Z_Copy },
  { 0x0102, 0x0004,  "CurrentPositionTilt",  &Z_Copy },
  { 0x0102, 0x0005,  "NumberofActuationsLift",&Z_Copy },
  { 0x0102, 0x0006,  "NumberofActuationsTilt",&Z_Copy },
  { 0x0102, 0x0007,  "ConfigStatus",         &Z_Copy },
  { 0x0102, 0x0008,  "CurrentPositionLiftPercentage",&Z_Copy },
  { 0x0102, 0x0009,  "CurrentPositionTiltPercentage",&Z_Copy },
  { 0x0102, 0x0010,  "InstalledOpenLimitLift",&Z_Copy },
  { 0x0102, 0x0011,  "InstalledClosedLimitLift",&Z_Copy },
  { 0x0102, 0x0012,  "InstalledOpenLimitTilt",&Z_Copy },
  { 0x0102, 0x0013,  "InstalledClosedLimitTilt",&Z_Copy },
  { 0x0102, 0x0014,  "VelocityLift",         &Z_Copy },
  { 0x0102, 0x0015,  "AccelerationTimeLift",&Z_Copy },
  { 0x0102, 0x0016,  "DecelerationTimeLift", &Z_Copy },
  { 0x0102, 0x0017,  "Mode",                 &Z_Copy },
  { 0x0102, 0x0018,  "IntermediateSetpointsLift",&Z_Copy },
  { 0x0102, 0x0019,  "IntermediateSetpointsTilt",&Z_Copy },

  // Color Control cluster
  { 0x0300, 0x0000,  "Hue",                  &Z_Copy },
  { 0x0300, 0x0001,  "Sat",                  &Z_Copy },
  { 0x0300, 0x0002,  "RemainingTime",        &Z_Copy },
  { 0x0300, 0x0003,  "X",                    &Z_Copy },
  { 0x0300, 0x0004,  "Y",                    &Z_Copy },
  { 0x0300, 0x0005,  "DriftCompensation",    &Z_Copy },
  { 0x0300, 0x0006,  "CompensationText",     &Z_Copy },
  { 0x0300, 0x0007,  "CT",                   &Z_Copy },
  { 0x0300, 0x0008,  "ColorMode",            &Z_Copy },
  { 0x0300, 0x0010,  "NumberOfPrimaries",    &Z_Copy },
  { 0x0300, 0x0011,  "Primary1X",            &Z_Copy },
  { 0x0300, 0x0012,  "Primary1Y",            &Z_Copy },
  { 0x0300, 0x0013,  "Primary1Intensity",    &Z_Copy },
  { 0x0300, 0x0015,  "Primary2X",            &Z_Copy },
  { 0x0300, 0x0016,  "Primary2Y",            &Z_Copy },
  { 0x0300, 0x0017,  "Primary2Intensity",    &Z_Copy },
  { 0x0300, 0x0019,  "Primary3X",            &Z_Copy },
  { 0x0300, 0x001A,  "Primary3Y",            &Z_Copy },
  { 0x0300, 0x001B,  "Primary3Intensity",    &Z_Copy },
  { 0x0300, 0x0030,  "WhitePointX",          &Z_Copy },
  { 0x0300, 0x0031,  "WhitePointY",          &Z_Copy },
  { 0x0300, 0x0032,  "ColorPointRX",         &Z_Copy },
  { 0x0300, 0x0033,  "ColorPointRY",         &Z_Copy },
  { 0x0300, 0x0034,  "ColorPointRIntensity", &Z_Copy },
  { 0x0300, 0x0036,  "ColorPointGX",         &Z_Copy },
  { 0x0300, 0x0037,  "ColorPointGY",         &Z_Copy },
  { 0x0300, 0x0038,  "ColorPointGIntensity", &Z_Copy },
  { 0x0300, 0x003A,  "ColorPointBX",         &Z_Copy },
  { 0x0300, 0x003B,  "ColorPointBY",         &Z_Copy },
  { 0x0300, 0x003C,  "ColorPointBIntensity", &Z_Copy },

  // Illuminance Measurement cluster
  { 0x0400, 0x0000,  D_JSON_ILLUMINANCE,     &Z_Copy },    // Illuminance (in Lux)
  { 0x0400, 0x0001,  "IlluminanceMinMeasuredValue",     &Z_Copy },    //
  { 0x0400, 0x0002,  "IlluminanceMaxMeasuredValue",     &Z_Copy },    //
  { 0x0400, 0x0003,  "IlluminanceTolerance",            &Z_Copy },    //
  { 0x0400, 0x0004,  "IlluminanceLightSensorType",      &Z_Copy },    //
  { 0x0400, 0xFFFF,  nullptr,                &Z_Remove },    // Remove all other values

  // Illuminance Level Sensing cluster
  { 0x0401, 0x0000,  "IlluminanceLevelStatus",          &Z_Copy },    // Illuminance (in Lux)
  { 0x0401, 0x0001,  "IlluminanceLightSensorType",      &Z_Copy },    // LightSensorType
  { 0x0401, 0xFFFF,  nullptr,                &Z_Remove },    // Remove all other values

  // Temperature Measurement cluster
  { 0x0402, 0x0000,  D_JSON_TEMPERATURE,     &Z_FloatDiv100 },   // Temperature
  { 0x0402, 0x0001,  "TemperatureMinMeasuredValue",     &Z_FloatDiv100 },    //
  { 0x0402, 0x0002,  "TemperatureMaxMeasuredValue",     &Z_FloatDiv100 },    //
  { 0x0402, 0x0003,  "TemperatureTolerance",            &Z_FloatDiv100 },    //
  { 0x0402, 0xFFFF,  nullptr,                &Z_Remove },     // Remove all other values

  // Pressure Measurement cluster
  { 0x0403, 0x0000,  D_JSON_PRESSURE_UNIT,   &Z_AddPressureUnit },     // Pressure Unit
  { 0x0403, 0x0000,  D_JSON_PRESSURE,        &Z_Copy },     // Pressure
  { 0x0403, 0x0001,  "PressureMinMeasuredValue",     &Z_Copy },    //
  { 0x0403, 0x0002,  "PressureMaxMeasuredValue",     &Z_Copy },    //
  { 0x0403, 0x0003,  "PressureTolerance",            &Z_Copy },    //
  { 0x0403, 0x0010,  "PressureScaledValue",          &Z_Copy },    //
  { 0x0403, 0x0011,  "PressureMinScaledValue",       &Z_Copy },    //
  { 0x0403, 0x0012,  "PressureMaxScaledValue",       &Z_Copy },    //
  { 0x0403, 0x0013,  "PressureScaledTolerance",      &Z_Copy },    //
  { 0x0403, 0x0014,  "PressureScale",                &Z_Copy },    //
  { 0x0403, 0xFFFF,  nullptr,                &Z_Remove },     // Remove all other Pressure values

  // Flow Measurement cluster
  { 0x0404, 0x0000,  D_JSON_FLOWRATE,        &Z_FloatDiv10 },    // Flow (in m3/h)
  { 0x0404, 0x0001,  "FlowMinMeasuredValue", &Z_Copy },    //
  { 0x0404, 0x0002,  "FlowMaxMeasuredValue", &Z_Copy },    //
  { 0x0404, 0x0003,  "FlowTolerance",        &Z_Copy },    //
  { 0x0404, 0xFFFF,  nullptr,                &Z_Remove },    // Remove all other values

  // Relative Humidity Measurement cluster
  { 0x0405, 0x0000,  D_JSON_HUMIDITY,        &Z_FloatDiv100 },   // Humidity
  { 0x0405, 0x0001,  "HumidityMinMeasuredValue",     &Z_Copy },    //
  { 0x0405, 0x0002,  "HumidityMaxMeasuredValue",     &Z_Copy },    //
  { 0x0405, 0x0003,  "HumidityTolerance",            &Z_Copy },    //
  { 0x0405, 0xFFFF,  nullptr,                &Z_Remove },     // Remove all other values

  // Occupancy Sensing cluster
  { 0x0406, 0x0000,  OCCUPANCY,              &Z_Copy },    // Occupancy (map8)
  { 0x0406, 0x0001,  "OccupancySensorType",  &Z_Copy },    // OccupancySensorType
  { 0x0406, 0xFFFF,  nullptr,                &Z_Remove },    // Remove all other values

  // Meter Identification cluster
  { 0x0B01, 0x0000,  "CompanyName",          &Z_Copy },
  { 0x0B01, 0x0001,  "MeterTypeID",          &Z_Copy },
  { 0x0B01, 0x0004,  "DataQualityID",        &Z_Copy },
  { 0x0B01, 0x0005,  "CustomerName",         &Z_Copy },
  { 0x0B01, 0x0006,  "Model",                &Z_Copy },
  { 0x0B01, 0x0007,  "PartNumber",           &Z_Copy },
  { 0x0B01, 0x000A,  "SoftwareRevision",     &Z_Copy },
  { 0x0B01, 0x000C,  "POD",                  &Z_Copy },
  { 0x0B01, 0x000D,  "AvailablePower",       &Z_Copy },
  { 0x0B01, 0x000E,  "PowerThreshold",       &Z_Copy },

  // Diagnostics cluster
  { 0x0B05, 0x0000,  "NumberOfResets",       &Z_Copy },
  { 0x0B05, 0x0001,  "PersistentMemoryWrites",&Z_Copy },
  { 0x0B05, 0x011C,  "LastMessageLQI",       &Z_Copy },
  { 0x0B05, 0x011D,  "LastMessageRSSI",      &Z_Copy },

};

// ======================================================================
// Record Manuf
int32_t Z_ManufKeep(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  json[new_name] = value;
  zigbee_devices.setManufId(shortaddr, value.as<const char*>());
  return 1;
}
//
int32_t Z_ModelKeep(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  json[new_name] = value;
  zigbee_devices.setModelId(shortaddr, value.as<const char*>());
  return 1;
}

// ======================================================================
// Remove attribute
int32_t Z_Remove(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  return 1;   // remove original key
}

// Copy value as-is
int32_t Z_Copy(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  json[new_name] = value;
  return 1;   // remove original key
}

// Add pressure unit
int32_t Z_AddPressureUnit(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  json[new_name] = F(D_UNIT_PRESSURE);
  return 0;   // keep original key
}

// Convert int to float and divide by 100
int32_t Z_FloatDiv100(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  json[new_name] = ((float)value) / 100.0f;
  return 1;   // remove original key
}
// Convert int to float and divide by 10
int32_t Z_FloatDiv10(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  json[new_name] = ((float)value) / 10.0f;
  return 1;   // remove original key
}
// Convert int to float and divide by 10
int32_t Z_FloatDiv2(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  json[new_name] = ((float)value) / 2.0f;
  return 1;   // remove original key
}

// Publish a message for `"Occupancy":0` when the timer expired
int32_t Z_OccupancyCallback(uint16_t shortaddr, uint16_t cluster, uint16_t endpoint, uint32_t value) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json[F(OCCUPANCY)] = 0;
  zigbee_devices.jsonPublishNow(shortaddr, json);
}

// Aqara Cube
int32_t Z_AqaraCube(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  json[new_name] = value;   // copy the original value
  int32_t val = value;
  const __FlashStringHelper *aqara_cube = F("AqaraCube");
  const __FlashStringHelper *aqara_cube_side = F("AqaraCubeSide");
  const __FlashStringHelper *aqara_cube_from_side = F("AqaraCubeFromSide");

  switch (val) {
    case 0:
      json[aqara_cube] = F("shake");
      break;
    case 2:
      json[aqara_cube] = F("wakeup");
      break;
    case 3:
      json[aqara_cube] = F("fall");
      break;
    case 64 ... 127:
      json[aqara_cube] = F("flip90");
      json[aqara_cube_side] = val % 8;
      json[aqara_cube_from_side] = (val - 64) / 8;
      break;
    case 128 ... 132:
      json[aqara_cube] = F("flip180");
      json[aqara_cube_side] = val - 128;
      break;
    case 256 ... 261:
      json[aqara_cube] = F("slide");
      json[aqara_cube_side] = val - 256;
      break;
    case 512 ... 517:
      json[aqara_cube] = F("tap");
      json[aqara_cube_side] = val - 512;
      break;
  }

  //     Source: https://github.com/kirovilya/ioBroker.zigbee
  //         +---+
  //         | 2 |
  //     +---+---+---+
  //     | 4 | 0 | 1 |
  //     +---+---+---+
  //         |M5I|
  //         +---+
  //         | 3 |
  //         +---+
  //     Side 5 is with the MI logo, side 3 contains the battery door.
  //     presentValue = 0 = shake
  //     presentValue = 2 = wakeup
  //     presentValue = 3 = fly/fall
  //     presentValue = y + x * 8 + 64 = 90º Flip from side x on top to side y on top
  //     presentValue = x + 128 = 180º flip to side x on top
  //     presentValue = x + 256 = push/slide cube while side x is on top
  //     presentValue = x + 512 = double tap while side x is on top

  return 1;
}

// Aqara Vibration Sensor - special proprietary attributes
int32_t Z_AqaraVibration(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  //json[new_name] = value;
  switch (attr) {
    case 0x0055:
      {
        int32_t ivalue = value;
        const __FlashStringHelper * svalue;
        switch (ivalue) {
          case 1: svalue = F("vibrate"); break;
          case 2: svalue = F("tilt"); break;
          case 3: svalue = F("drop"); break;
          default: svalue = F("unknown"); break;
        }
        json[new_name] = svalue;
      }
      break;
    // case 0x0503:
    //   break;
    // case 0x0505:
    //   break;
    case 0x0508:
      {
        // see https://github.com/Koenkk/zigbee2mqtt/issues/295 and http://faire-ca-soi-meme.fr/domotique/2018/09/03/test-xiaomi-aqara-vibration-sensor/
        // report accelerometer measures
        String hex = value;
        SBuffer buf2 = SBuffer::SBufferFromHex(hex.c_str(), hex.length());
        int16_t x, y, z;
        z = buf2.get16(0);
        y = buf2.get16(2);
        x = buf2.get16(4);
        JsonArray& xyz = json.createNestedArray(new_name);
        xyz.add(x);
        xyz.add(y);
        xyz.add(z);
        // calculate angles
        float X = x;
        float Y = y;
        float Z = z;
        int32_t Angle_X = 0.5f + atanf(X/sqrtf(z*z+y*y)) * f_180pi;
        int32_t Angle_Y = 0.5f + atanf(Y/sqrtf(x*x+z*z)) * f_180pi;
        int32_t Angle_Z = 0.5f + atanf(Z/sqrtf(x*x+y*y)) * f_180pi;
        JsonArray& angles = json.createNestedArray(F("AqaraAngles"));
        angles.add(Angle_X);
        angles.add(Angle_Y);
        angles.add(Angle_Z);
      }
      break;
  }
  return 1;   // remove original key
}

int32_t Z_AqaraSensor(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr) {
  String hex = value;
  SBuffer buf2 = SBuffer::SBufferFromHex(hex.c_str(), hex.length());
  uint32_t i = 0;
  uint32_t len = buf2.len();
  char tmp[] = "tmp";   // for obscure reasons, it must be converted from const char* to char*, otherwise ArduinoJson gets confused

  JsonVariant sub_value;
  const String * modelId = zigbee_devices.getModelId(shortaddr);  // null if unknown

  while (len - i >= 2) {
    uint8_t attrid = buf2.get8(i++);

    i += parseSingleAttribute(json, tmp, buf2, i, len);
    float val = json[tmp];
    json.remove(tmp);
    bool translated = false;    // were we able to translate to a known format?
    if (0x01 == attrid) {
      json[F(D_JSON_VOLTAGE)] = val / 1000.0f;
      json[F("Battery")] = toPercentageCR2032(val);
    } else if ((nullptr != modelId) && (0 == zcl->getManufCode())) {
      translated = true;
      if (modelId->startsWith(F("lumi.sensor_ht")) ||
          modelId->startsWith(F("lumi.weather"))) {     // Temp sensor
        // Filter according to prefix of model name
        // onla Aqara Temp/Humidity has manuf_code of zero. If non-zero we skip the parameters
        if (0x64 == attrid) {
          json[F(D_JSON_TEMPERATURE)] = val / 100.0f;
        } else if (0x65 == attrid) {
          json[F(D_JSON_HUMIDITY)] = val / 100.0f;
        } else if (0x66 == attrid) {
          json[F(D_JSON_PRESSURE)] = val / 100.0f;
          json[F(D_JSON_PRESSURE_UNIT)] = F(D_UNIT_PRESSURE);   // hPa
        }
      } else if (modelId->startsWith(F("lumi.sensor_smoke"))) {   // gas leak
        if (0x64 == attrid) {
          json[F("SmokeDensity")] = val;
        }
      } else if (modelId->startsWith(F("lumi.sensor_natgas"))) {   // gas leak
        if (0x64 == attrid) {
          json[F("GasDensity")] = val;
        }
      } else {
        translated = false;     // we didn't find a match
      }
 //   } else if (0x115F == zcl->getManufCode()) {      // Aqara Motion Sensor, still unknown field
    }
    if (!translated) {
      if (attrid >= 100) {    // payload is always above 0x64 or 100
        char attr_name[12];
        snprintf_P(attr_name, sizeof(attr_name), PSTR("Xiaomi_%02X"), attrid);
        json[attr_name] = val;
      }
    }
  }
  return 1;   // remove original key
}
// ======================================================================

void ZCLFrame::postProcessAttributes(uint16_t shortaddr, JsonObject& json) {
  // iterate on json elements
  for (auto kv : json) {
    String key_string = kv.key;
    const char * key = key_string.c_str();
    JsonVariant& value = kv.value;
    // Check that format looks like "CCCC/AAAA" or "CCCC/AAAA+d"
    char * delimiter = strchr(key, '/');
    char * delimiter2 = strchr(key, '+');
    if (delimiter) {
      uint16_t attribute;
      uint16_t suffix = 1;
      uint16_t cluster = strtoul(key, &delimiter, 16);
      if (!delimiter2) {
        attribute = strtoul(delimiter+1, nullptr, 16);
      } else {
        attribute = strtoul(delimiter+1, &delimiter2, 16);
        suffix = strtoul(delimiter2+1, nullptr, 10);
      }

      // Iterate on filter
      for (uint32_t i = 0; i < sizeof(Z_PostProcess) / sizeof(Z_PostProcess[0]); i++) {
        const Z_AttributeConverter *converter = &Z_PostProcess[i];
        uint16_t conv_cluster = pgm_read_word(&converter->cluster);
        uint16_t conv_attribute = pgm_read_word(&converter->attribute);

        if ((conv_cluster == cluster) &&
            ((conv_attribute == attribute) || (conv_attribute == 0xFFFF)) ) {
          String new_name_str = converter->name;
          if (suffix > 1) { new_name_str += suffix; }   // append suffix number
          int32_t drop = (*converter->func)(this, shortaddr, json, key, value, new_name_str, conv_cluster, conv_attribute);
          if (drop) {
            json.remove(key);
          }

        }
      }
    }
  }
}

#endif // USE_ZIGBEE
