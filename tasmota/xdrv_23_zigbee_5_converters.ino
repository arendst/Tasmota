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


enum Z_DataTypes {
  Znodata   = 0x00,
  Zdata8    = 0x08, Zdata16, Zdata24, Zdata32, Zdata40, Zdata48, Zdata56, Zdata64,
  Zbool     = 0x10,
  Zmap8     = 0x18, Zmap16, Zmap24, Zmap32, Zmap40, Zmap48, Zmap56, Zmap64,
  Zuint8    = 0x20, Zuint16, Zuint24, Zuint32, Zuint40, Zuint48, Zuint56, Zuint64,
  Zint8     = 0x28, Zint16, Zint24, Zint32, Zint40, Zint48, Zint56, Zint64,
  Zenum8    = 0x30, Zenum16 = 0x31,
  Zsemi     = 0x38, Zsingle = 0x39, Zdouble = 0x3A,
  Zoctstr   = 0x41, Zstring = 0x42, Zoctstr16 = 0x43, Zstring16 = 0x44,
  Arrray    = 0x48,
  Zstruct   = 0x4C,
  Zset      = 0x50, Zbag = 0x51,
  ZToD      = 0xE0, Zdate = 0xE1, ZUTC = 0xE2,
  ZclusterId = 0xE8, ZattribId = 0xE9, ZbacOID = 0xEA,
  ZEUI64    = 0xF0, Zkey128 = 0xF1,
  Zunk      = 0xFF
};

//
// get the lenth in bytes for a data-type
// return 0 if unknown of type specific
//
// Note: this code is smaller than a static array
uint8_t Z_getDatatypeLen(uint8_t t) {
  if ( ((t >= 0x08) && (t <= 0x0F)) ||      // data8 - data64
       ((t >= 0x18) && (t <= 0x2F)) ) {     // map/uint/int
    return (t & 0x07) + 1;
  } 
  switch (t) {
    case Zbool:
    case Zenum8:
      return 1;
    case Zenum16:
    case Zsemi:
    case ZclusterId:
    case ZattribId:
      return 2;
    case Zsingle:
    case ZToD:
    case Zdate:
    case ZUTC:
    case ZbacOID:
      return 4;
    case Zdouble:
    case ZEUI64:
      return 8;
    case Zkey128:
      return 16;
    case Znodata:
    default:
      return 0;
  }
}

// typedef struct Z_DataTypeMapping {
//   uint8_t     datatype;
//   uint8_t     len;        // len in bytes and add 0x80 if DISCRETE
// }

// const Z_DataTypeMapping Z_types[] PROGMEM = {
//   { Znodata,          0 },
//   { Zdata8,          0 },
// };

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
    const char *buf, size_t buf_len, uint16_t clusterid, uint16_t groupaddr,
    uint16_t srcaddr, uint8_t srcendpoint, uint8_t dstendpoint, uint8_t wasbroadcast,
    uint8_t linkquality, uint8_t securityuse, uint8_t seqnumber,
    uint32_t timestamp):
    _cmd_id(cmd_id), _manuf_code(manuf_code), _transact_seq(transact_seq),
    _payload(buf_len ? buf_len : 250),      // allocate the data frame from source or preallocate big enough
    _cluster_id(clusterid), _groupaddr(groupaddr),
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
                    _groupaddr, _cluster_id, _srcaddr,
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
    _groupaddr = groupid;
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
  uint16_t                _groupaddr = 0;
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
                              uint32_t offset, uint32_t buflen) {

  uint32_t i = offset;
  uint32_t attrtype = buf.get8(i++);

  // fallback - enter a null value
  json[attrid_str] = (char*) nullptr;

  uint32_t len = Z_getDatatypeLen(attrtype);    // pre-compute lenght, overloaded for variable length attributes

  // now parse accordingly to attr type
  switch (attrtype) {
    // case Znodata:      // nodata
    // case Zunk:      // unk
    //   break;
    case Zbool:      // bool
    case Zuint8:      // uint8
    case Zenum8:      // enum8
      {
        uint8_t uint8_val = buf.get8(i);
        // i += 1;
        if (0xFF != uint8_val) {
          json[attrid_str] = uint8_val;
        }
      }
      break;
    case Zuint16:      // uint16
    case Zenum16:      // enum16
      {
        uint16_t uint16_val = buf.get16(i);
        // i += 2;
        if (0xFFFF != uint16_val) {
          json[attrid_str] = uint16_val;
        }
      }
      break;
    case Zuint32:      // uint32
      {
        uint32_t uint32_val = buf.get32(i);
        // i += 4;
        if (0xFFFFFFFF != uint32_val) {
          json[attrid_str] = uint32_val;
        }
      }
      break;
    // Note: uint40, uint48, uint56, uint64 are displayed as Hex
    // Note: int40, int48, int56, int64 are displayed as Hex
    case Zuint40:    // uint40
    case Zuint48:    // uint48
    case Zuint56:    // uint56
    case Zuint64:    // uint64
    case Zint40:    // int40
    case Zint48:    // int48
    case Zint56:    // int56
    case Zint64:    // int64
      {
        // uint8_t len = attrtype - 0x27;   // 5 - 8
        // print as HEX
        char hex[2*len+1];
        ToHex_P(buf.buf(i), len, hex, sizeof(hex));
        json[attrid_str] = hex;
        // i += len;
      }
      break;
    case Zint8:      // int8
      {
        int8_t int8_val = buf.get8(i);
        // i += 1;
        if (0x80 != int8_val) {
          json[attrid_str] = int8_val;
        }
      }
      break;
    case Zint16:      // int16
      {
        int16_t int16_val = buf.get16(i);
        // i += 2;
        if (0x8000 != int16_val) {
          json[attrid_str] = int16_val;
        }
      }
      break;
    case Zint32:      // int32
      {
        int32_t int32_val = buf.get32(i);
        // i += 4;
        if (0x80000000 != int32_val) {
          json[attrid_str] = int32_val;
        }
      }
      break;

    case Zoctstr:      // octet string, 1 byte len
    case Zstring:      // char string, 1 byte len
    case Zoctstr16:      // octet string, 2 bytes len
    case Zstring16:      // char string, 2 bytes len
      // For strings, default is to try to do a real string, but reverts to octet stream if null char is present or on some exceptions
      {
        bool parse_as_string = true;
        len = (attrtype <= 0x42) ? buf.get8(i) : buf.get16(i);    // len is 8 or 16 bits
        i += (attrtype <= 0x42) ? 1 : 2;                                   // increment pointer
        if (i + len > buf.len()) {        // make sure we don't get past the buffer
          len = buf.len() - i;
        }

        // check if we can safely use a string
        if ((0x41 == attrtype) || (0x43 == attrtype)) { parse_as_string = false; }

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

        // i += len;
        // break;
      }
      // i += buf.get8(i) + 1;
      break;

    case Zdata8:      // data8
    case Zmap8:      // map8
      {
        uint8_t uint8_val = buf.get8(i);
        // i += 1;
        json[attrid_str] = uint8_val;
      }
      break;
    case Zdata16:      // data16
    case Zmap16:      // map16
      {
        uint16_t uint16_val = buf.get16(i);
        // i += 2;
        json[attrid_str] = uint16_val;
      }
      break;
    case Zdata32:      // data32
    case Zmap32:      // map32
      {
        uint32_t uint32_val = buf.get32(i);
        // i += 4;
        json[attrid_str] = uint32_val;
      }
      break;

    case Zsingle:      // float
      {
        uint32_t uint32_val = buf.get32(i);
        float  * float_val = (float*) &uint32_val;
        // i += 4;
        json[attrid_str] = *float_val;
      }
      break;

    // TODO
    case ZToD:      // ToD
    case Zdate:      // date
    case ZUTC:      // UTC
    case ZclusterId:      // clusterId
    case ZattribId:      // attribId
    case ZbacOID:      // bacOID
    case ZEUI64:      // EUI64
    case Zkey128:      // key128
    case Zsemi:      // semi (float on 2 bytes)
      break;

    // Other un-implemented data types
    case Zdata24:      // data24
    case Zdata40:      // data40
    case Zdata48:      // data48
    case Zdata56:      // data56
    case Zdata64:      // data64
      break;
    // map<x>
    case Zmap24:      // map24
    case Zmap40:      // map40
    case Zmap48:      // map48
    case Zmap56:      // map56
    case Zmap64:      // map64
      break;
    case Zdouble:      // double precision
      {
        uint64_t uint64_val = buf.get64(i);
        double  * double_val = (double*) &uint64_val;
        // i += 8;
        json[attrid_str] = *double_val;
      }
      break;
  }
  i += len;

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
  const char * friendlyName = zigbee_devices.getFriendlyName(_srcaddr);
  if (friendlyName) {
    json[F(D_JSON_ZIGBEE_NAME)] = (char*) friendlyName;
  }
  // "Command"
  snprintf_P(s, sizeof(s), PSTR("%04X!%02X"), _cluster_id, cmd);
  json[F(D_JSON_ZIGBEE_CMD)] = s;
  // "Status"
  json[F(D_JSON_ZIGBEE_STATUS)] = status;
  // "StatusMessage"
  json[F(D_JSON_ZIGBEE_STATUS_MSG)] = getZigbeeStatusMessage(status);
  // Add Endpoint
  json[F(D_CMND_ZIGBEE_ENDPOINT)] = _srcendpoint;
  // Add Group if non-zero
  if (_groupaddr) {
    json[F(D_CMND_ZIGBEE_GROUP)] = _groupaddr;
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
  sendHueUpdate(_srcaddr, _groupaddr, _cluster_id, _cmd_id, _frame_control.b.direction);
}

// return value:
// 0 = keep initial value
// 1 = remove initial value
typedef int32_t (*Z_AttrConverter)(const class ZCLFrame *zcl, uint16_t shortaddr, JsonObject& json, const char *name, JsonVariant& value, const String &new_name, uint16_t cluster, uint16_t attr);
typedef struct Z_AttributeConverter {
  uint8_t  type;
  uint8_t  cluster_short;
  uint16_t attribute;
  const char * name;
  Z_AttrConverter func;
} Z_AttributeConverter;

enum Cx_cluster_short {
  Cx0000, Cx0001, Cx0002, Cx0003, Cx0004, Cx0005, Cx0006, Cx0007,
  Cx0008, Cx0009, Cx000A, Cx000B, Cx000C, Cx000D, Cx000E, Cx000F, 
  Cx0010, Cx0011, Cx0012, Cx0013, Cx0014, Cx001A, Cx0020, Cx0100,
  Cx0101, Cx0102, Cx0300, Cx0400, Cx0401, Cx0402, Cx0403, Cx0404,
  Cx0405, Cx0406, Cx0B01, Cx0B05,
};

const uint16_t Cx_cluster[] PROGMEM = {
  0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
  0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F, 
  0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x001A, 0x0020, 0x0100,
  0x0101, 0x0102, 0x0300, 0x0400, 0x0401, 0x0402, 0x0403, 0x0404,
  0x0405, 0x0406, 0x0B01, 0x0B05,
};

uint16_t CxToCluster(uint8_t cx) {
  if (cx < sizeof(Cx_cluster)/sizeof(Cx_cluster[0])) {
    return pgm_read_word(&Cx_cluster[cx]);
  }
  return 0xFFFF;
}

ZF(ZCLVersion) ZF(AppVersion) ZF(StackVersion) ZF(HWVersion) ZF(Manufacturer) ZF(ModelId)
ZF(DateCode) ZF(PowerSource) ZF(SWBuildID) ZF(Power) ZF(SwitchType) ZF(Dimmer)
ZF(MainsVoltage) ZF(MainsFrequency) ZF(BatteryVoltage) ZF(BatteryPercentage)
ZF(CurrentTemperature) ZF(MinTempExperienced) ZF(MaxTempExperienced) ZF(OverTempTotalDwell)
ZF(SceneCount) ZF(CurrentScene) ZF(CurrentGroup) ZF(SceneValid)
ZF(AlarmCount) ZF(Time) ZF(TimeStatus) ZF(TimeZone) ZF(DstStart) ZF(DstEnd)
ZF(DstShift) ZF(StandardTime) ZF(LocalTime) ZF(LastSetTime) ZF(ValidUntilTime)

ZF(LocationType) ZF(LocationMethod) ZF(LocationAge) ZF(QualityMeasure) ZF(NumberOfDevices)

ZF(AnalogInActiveText) ZF(AnalogInDescription) ZF(AnalogInInactiveText) ZF(AnalogInMaxValue)
ZF(AnalogInMinValue) ZF(AnalogInOutOfService) ZF(AqaraRotate) ZF(AnalogInPriorityArray)
ZF(AnalogInReliability) ZF(AnalogInRelinquishDefault) ZF(AnalogInResolution) ZF(AnalogInStatusFlags)
ZF(AnalogInEngineeringUnits) ZF(AnalogInApplicationType) ZF(Aqara_FF05)

ZF(AnalogOutDescription) ZF(AnalogOutMaxValue) ZF(AnalogOutMinValue) ZF(AnalogOutOutOfService)
ZF(AnalogOutValue) ZF(AnalogOutPriorityArray) ZF(AnalogOutReliability) ZF(AnalogOutRelinquishDefault)
ZF(AnalogOutResolution) ZF(AnalogOutStatusFlags) ZF(AnalogOutEngineeringUnits) ZF(AnalogOutApplicationType)

ZF(AnalogDescription) ZF(AnalogOutOfService) ZF(AnalogValue) ZF(AnalogPriorityArray) ZF(AnalogReliability)
ZF(AnalogRelinquishDefault) ZF(AnalogStatusFlags) ZF(AnalogEngineeringUnits) ZF(AnalogApplicationType)

ZF(BinaryInActiveText) ZF(BinaryInDescription) ZF(BinaryInInactiveText) ZF(BinaryInOutOfService)
ZF(BinaryInPolarity) ZF(BinaryInValue) ZF(BinaryInPriorityArray) ZF(BinaryInReliability)
ZF(BinaryInStatusFlags) ZF(BinaryInApplicationType)

ZF(BinaryOutActiveText) ZF(BinaryOutDescription) ZF(BinaryOutInactiveText) ZF(BinaryOutMinimumOffTime)
ZF(BinaryOutMinimumOnTime) ZF(BinaryOutOutOfService) ZF(BinaryOutPolarity) ZF(BinaryOutValue)
ZF(BinaryOutPriorityArray) ZF(BinaryOutReliability) ZF(BinaryOutRelinquishDefault) ZF(BinaryOutStatusFlags)
ZF(BinaryOutApplicationType)

ZF(BinaryActiveText) ZF(BinaryDescription) ZF(BinaryInactiveText) ZF(BinaryMinimumOffTime)
ZF(BinaryMinimumOnTime) ZF(BinaryOutOfService) ZF(BinaryValue) ZF(BinaryPriorityArray) ZF(BinaryReliability)
ZF(BinaryRelinquishDefault) ZF(BinaryStatusFlags) ZF(BinaryApplicationType)

ZF(MultiInStateText) ZF(MultiInDescription) ZF(MultiInNumberOfStates) ZF(MultiInOutOfService)
ZF(MultiInValue) ZF(MultiInReliability) ZF(MultiInStatusFlags) ZF(MultiInApplicationType)

ZF(MultiOutStateText) ZF(MultiOutDescription) ZF(MultiOutNumberOfStates) ZF(MultiOutOutOfService)
ZF(MultiOutValue) ZF(MultiOutPriorityArray) ZF(MultiOutReliability) ZF(MultiOutRelinquishDefault)
ZF(MultiOutStatusFlags) ZF(MultiOutApplicationType)

ZF(MultiStateText) ZF(MultiDescription) ZF(MultiNumberOfStates) ZF(MultiOutOfService) ZF(MultiValue)
ZF(MultiReliability) ZF(MultiRelinquishDefault) ZF(MultiStatusFlags) ZF(MultiApplicationType)

ZF(TotalProfileNum) ZF(MultipleScheduling) ZF(EnergyFormatting) ZF(EnergyRemote) ZF(ScheduleMode)

ZF(CheckinInterval) ZF(LongPollInterval) ZF(ShortPollInterval) ZF(FastPollTimeout) ZF(CheckinIntervalMin)
ZF(LongPollIntervalMin) ZF(FastPollTimeoutMax)

ZF(PhysicalClosedLimit) ZF(MotorStepSize) ZF(Status) ZF(ClosedLimit) ZF(Mode)

ZF(LockState) ZF(LockType) ZF(ActuatorEnabled) ZF(DoorState) ZF(DoorOpenEvents)
ZF(DoorClosedEvents) ZF(OpenPeriod)

ZF(AqaraVibrationMode) ZF(AqaraVibrationsOrAngle) ZF(AqaraVibration505) ZF(AqaraAccelerometer)

ZF(WindowCoveringType) ZF(PhysicalClosedLimitLift) ZF(PhysicalClosedLimitTilt) ZF(CurrentPositionLift)
ZF(CurrentPositionTilt) ZF(NumberofActuationsLift) ZF(NumberofActuationsTilt) ZF(ConfigStatus)
ZF(CurrentPositionLiftPercentage) ZF(CurrentPositionTiltPercentage) ZF(InstalledOpenLimitLift)
ZF(InstalledClosedLimitLift) ZF(InstalledOpenLimitTilt) ZF(InstalledClosedLimitTilt) ZF(VelocityLift)
ZF(AccelerationTimeLift) ZF(DecelerationTimeLift) ZF(IntermediateSetpointsLift)
ZF(IntermediateSetpointsTilt)

ZF(Hue) ZF(Sat) ZF(RemainingTime) ZF(X) ZF(Y) ZF(DriftCompensation) ZF(CompensationText) ZF(CT)
ZF(ColorMode) ZF(NumberOfPrimaries) ZF(Primary1X) ZF(Primary1Y) ZF(Primary1Intensity) ZF(Primary2X)
ZF(Primary2Y) ZF(Primary2Intensity) ZF(Primary3X) ZF(Primary3Y) ZF(Primary3Intensity) ZF(WhitePointX)
ZF(WhitePointY) ZF(ColorPointRX) ZF(ColorPointRY) ZF(ColorPointRIntensity) ZF(ColorPointGX) ZF(ColorPointGY)
ZF(ColorPointGIntensity) ZF(ColorPointBX) ZF(ColorPointBY) ZF(ColorPointBIntensity)

ZF(Illuminance) ZF(IlluminanceMinMeasuredValue) ZF(IlluminanceMaxMeasuredValue) ZF(IlluminanceTolerance)
ZF(IlluminanceLightSensorType) ZF(IlluminanceLevelStatus) ZF(IlluminanceTargetLevel)

ZF(Temperature) ZF(TemperatureMinMeasuredValue) ZF(TemperatureMaxMeasuredValue) ZF(TemperatureTolerance)

ZF(PressureUnit) ZF(Pressure) ZF(PressureMinMeasuredValue) ZF(PressureMaxMeasuredValue) ZF(PressureTolerance)
ZF(PressureScaledValue) ZF(PressureMinScaledValue) ZF(PressureMaxScaledValue) ZF(PressureScaledTolerance)
ZF(PressureScale)

ZF(FlowRate) ZF(FlowMinMeasuredValue) ZF(FlowMaxMeasuredValue) ZF(FlowTolerance)

ZF(Humidity) ZF(HumidityMinMeasuredValue) ZF(HumidityMaxMeasuredValue) ZF(HumidityTolerance)

ZF(Occupancy) ZF(OccupancySensorType)

ZF(CompanyName) ZF(MeterTypeID) ZF(DataQualityID) ZF(CustomerName) ZF(Model) ZF(PartNumber)
ZF(SoftwareRevision) ZF(POD) ZF(AvailablePower) ZF(PowerThreshold) ZF(ProductRevision) ZF(UtilityName)

ZF(NumberOfResets) ZF(PersistentMemoryWrites) ZF(LastMessageLQI) ZF(LastMessageRSSI)
// list of post-processing directives
const Z_AttributeConverter Z_PostProcess[] PROGMEM = {
  { Zuint8,   Cx0000, 0x0000,  Z(ZCLVersion),           &Z_Copy },
  { Zuint8,   Cx0000, 0x0001,  Z(AppVersion),           &Z_Copy },
  { Zuint8,   Cx0000, 0x0002,  Z(StackVersion),         &Z_Copy },
  { Zuint8,   Cx0000, 0x0003,  Z(HWVersion),            &Z_Copy },
  { Zstring,  Cx0000, 0x0004,  Z(Manufacturer),         &Z_ManufKeep },    // record Manufacturer
  { Zstring,  Cx0000, 0x0005,  Z(ModelId), &Z_ModelKeep },    // record Model
  { Zstring,  Cx0000, 0x0006,  Z(DateCode),             &Z_Copy },
  { Zenum8,   Cx0000, 0x0007,  Z(PowerSource),          &Z_Copy },
  { Zstring,  Cx0000, 0x4000,  Z(SWBuildID),            &Z_Copy },
  { Zunk,     Cx0000, 0xFFFF,  nullptr,                &Z_Remove },    // Remove all other values
  // Cmd 0x0A - Cluster 0x0000, attribute 0xFF01 - proprietary
  { Zmap8,    Cx0000, 0xFF01,  nullptr,                &Z_AqaraSensor },    // Occupancy (map8)

  // Power Configuration cluster
  { Zuint16,  Cx0001, 0x0000,  Z(MainsVoltage),         &Z_Copy },
  { Zuint8,   Cx0001, 0x0001,  Z(MainsFrequency),       &Z_Copy },
  { Zuint8,   Cx0001, 0x0020,  Z(BatteryVoltage),       &Z_FloatDiv10 },
  { Zuint8,   Cx0001, 0x0021,  Z(BatteryPercentage),    &Z_Copy },

  // Device Temperature Configuration cluster
  { Zint16,   Cx0002, 0x0000,  Z(CurrentTemperature),   &Z_Copy },
  { Zint16,   Cx0002, 0x0001,  Z(MinTempExperienced),   &Z_Copy },
  { Zint16,   Cx0002, 0x0002,  Z(MaxTempExperienced),   &Z_Copy },
  { Zuint16,  Cx0002, 0x0003,  Z(OverTempTotalDwell),   &Z_Copy },

  // Scenes cluster
  { Zuint8,   Cx0005, 0x0000,  Z(SceneCount),           &Z_Copy },
  { Zuint8,   Cx0005, 0x0001,  Z(CurrentScene),         &Z_Copy },
  { Zuint16,  Cx0005, 0x0002,  Z(CurrentGroup),         &Z_Copy },
  { Zbool,    Cx0005, 0x0003,  Z(SceneValid),           &Z_Copy },
  //{ Zmap8,    Cx0005, 0x0004,  Z(NameSupport),           &Z_Copy },

  // On/off cluster
  { Zbool,    Cx0006,    0x0000,  Z(Power),                &Z_Copy },
  { Zbool,    Cx0006,    0x8000,  Z(Power),                &Z_Copy },   // See 7280

  // On/Off Switch Configuration cluster
  { Zenum8,   Cx0007, 0x0000,  Z(SwitchType),           &Z_Copy },

  // Level Control cluster
  { Zuint8,   Cx0008, 0x0000,  Z(Dimmer),               &Z_Copy },
  // { Zuint16, Cx0008, 0x0001,  Z(RemainingTime",        &Z_Copy },
  // { Zuint16, Cx0008, 0x0010,  Z(OnOffTransitionTime",  &Z_Copy },
  // { Zuint8, Cx0008, 0x0011,  Z(OnLevel",              &Z_Copy },
  // { Zuint16, Cx0008, 0x0012,  Z(OnTransitionTime",     &Z_Copy },
  // { Zuint16, Cx0008, 0x0013,  Z(OffTransitionTime",    &Z_Copy },
  // { Zuint16, Cx0008, 0x0014,  Z(DefaultMoveRate",      &Z_Copy },

  // Alarms cluster
  { Zuint16,  Cx0009, 0x0000,  Z(AlarmCount),           &Z_Copy },

  // Time cluster
  { ZUTC,     Cx000A, 0x0000,  Z(Time),                 &Z_Copy },
  { Zmap8,    Cx000A, 0x0001,  Z(TimeStatus),           &Z_Copy },
  { Zint32,   Cx000A, 0x0002,  Z(TimeZone),             &Z_Copy },
  { Zuint32,  Cx000A, 0x0003,  Z(DstStart),             &Z_Copy },
  { Zuint32,  Cx000A, 0x0004,  Z(DstEnd),             &Z_Copy },
  { Zint32,   Cx000A, 0x0005,  Z(DstShift),             &Z_Copy },
  { Zuint32,  Cx000A, 0x0006,  Z(StandardTime),         &Z_Copy },
  { Zuint32,  Cx000A, 0x0007,  Z(LocalTime),            &Z_Copy },
  { ZUTC,     Cx000A, 0x0008,  Z(LastSetTime),          &Z_Copy },
  { ZUTC,     Cx000A, 0x0009,  Z(ValidUntilTime),       &Z_Copy },

  // RSSI Location cluster
  { Zdata8,   Cx000B, 0x0000,  Z(LocationType),         &Z_Copy },
  { Zenum8,   Cx000B, 0x0001,  Z(LocationMethod),       &Z_Copy },
  { Zuint16,  Cx000B, 0x0002,  Z(LocationAge),          &Z_Copy },
  { Zuint8,   Cx000B, 0x0003,  Z(QualityMeasure),       &Z_Copy },
  { Zuint8,   Cx000B, 0x0004,  Z(NumberOfDevices),      &Z_Copy },

  // Analog Input cluster
  // { 0xFF, Cx000C, 0x0004,  Z(AnalogInActiveText),   &Z_Copy },
  { Zstring,  Cx000C, 0x001C,  Z(AnalogInDescription),  &Z_Copy },
  // { 0xFF, Cx000C, 0x002E,  Z(AnalogInInactiveText), &Z_Copy },
  { Zsingle,  Cx000C, 0x0041,  Z(AnalogInMaxValue),     &Z_Copy },
  { Zsingle,  Cx000C, 0x0045,  Z(AnalogInMinValue),     &Z_Copy },
  { Zbool,    Cx000C, 0x0051,  Z(AnalogInOutOfService), &Z_Copy },
  { Zsingle,  Cx000C, 0x0055,  Z(AqaraRotate),          &Z_Copy },
  // { 0xFF, Cx000C, 0x0057,  Z(AnalogInPriorityArray),&Z_Copy },
  { Zenum8,   Cx000C, 0x0067,  Z(AnalogInReliability),  &Z_Copy },
  // { 0xFF, Cx000C, 0x0068,  Z(AnalogInRelinquishDefault),&Z_Copy },
  { Zsingle,  Cx000C, 0x006A,  Z(AnalogInResolution),   &Z_Copy },
  { Zmap8,    Cx000C, 0x006F,  Z(AnalogInStatusFlags),  &Z_Copy },
  { Zenum16,  Cx000C, 0x0075,  Z(AnalogInEngineeringUnits),&Z_Copy },
  { Zuint32,  Cx000C, 0x0100,  Z(AnalogInApplicationType),&Z_Copy },
  { Zunk,     Cx000C, 0xFF05,  Z(Aqara_FF05),           &Z_Copy },

  // Analog Output cluster
  { Zstring,  Cx000D, 0x001C,  Z(AnalogOutDescription), &Z_Copy },
  { Zsingle,  Cx000D, 0x0041,  Z(AnalogOutMaxValue),    &Z_Copy },
  { Zsingle,  Cx000D, 0x0045,  Z(AnalogOutMinValue),    &Z_Copy },
  { Zbool,    Cx000D, 0x0051,  Z(AnalogOutOutOfService),&Z_Copy },
  { Zsingle,  Cx000D, 0x0055,  Z(AnalogOutValue),       &Z_Copy },
  { Zunk,     Cx000D, 0x0057,  Z(AnalogOutPriorityArray),&Z_Copy },
  { Zenum8,   Cx000D, 0x0067,  Z(AnalogOutReliability), &Z_Copy },
  { Zsingle,  Cx000D, 0x0068,  Z(AnalogOutRelinquishDefault),&Z_Copy },
  { Zsingle,  Cx000D, 0x006A,  Z(AnalogOutResolution),  &Z_Copy },
  { Zmap8,    Cx000D, 0x006F,  Z(AnalogOutStatusFlags), &Z_Copy },
  { Zenum16,  Cx000D, 0x0075,  Z(AnalogOutEngineeringUnits),&Z_Copy },
  { Zuint32,  Cx000D, 0x0100,  Z(AnalogOutApplicationType),&Z_Copy },

  // Analog Value cluster
  { Zstring,  Cx000E, 0x001C,  Z(AnalogDescription),    &Z_Copy },
  { Zbool,    Cx000E, 0x0051,  Z(AnalogOutOfService),   &Z_Copy },
  { Zsingle,  Cx000E, 0x0055,  Z(AnalogValue),          &Z_Copy },
  { Zunk,     Cx000E, 0x0057,  Z(AnalogPriorityArray),  &Z_Copy },
  { Zenum8,   Cx000E, 0x0067,  Z(AnalogReliability),    &Z_Copy },
  { Zsingle,  Cx000E, 0x0068,  Z(AnalogRelinquishDefault),&Z_Copy },
  { Zmap8,    Cx000E, 0x006F,  Z(AnalogStatusFlags),    &Z_Copy },
  { Zenum16,  Cx000E, 0x0075,  Z(AnalogEngineeringUnits),&Z_Copy },
  { Zuint32,  Cx000E, 0x0100,  Z(AnalogApplicationType),&Z_Copy },

  // Binary Input cluster
  { Zstring,  Cx000F, 0x0004,  Z(BinaryInActiveText),  &Z_Copy },
  { Zstring,  Cx000F, 0x001C,  Z(BinaryInDescription), &Z_Copy },
  { Zstring,  Cx000F, 0x002E,  Z(BinaryInInactiveText),&Z_Copy },
  { Zbool,    Cx000F, 0x0051,  Z(BinaryInOutOfService),&Z_Copy },
  { Zenum8,   Cx000F, 0x0054,  Z(BinaryInPolarity),    &Z_Copy },
  { Zstring,  Cx000F, 0x0055,  Z(BinaryInValue),       &Z_Copy },
  // { 0xFF, Cx000F, 0x0057,  Z(BinaryInPriorityArray),&Z_Copy },
  { Zenum8,   Cx000F, 0x0067,  Z(BinaryInReliability), &Z_Copy },
  { Zmap8,    Cx000F, 0x006F,  Z(BinaryInStatusFlags), &Z_Copy },
  { Zuint32,  Cx000F, 0x0100,  Z(BinaryInApplicationType),&Z_Copy },

  // Binary Output cluster
  { Zstring,  Cx0010, 0x0004,  Z(BinaryOutActiveText),  &Z_Copy },
  { Zstring,  Cx0010, 0x001C,  Z(BinaryOutDescription), &Z_Copy },
  { Zstring,  Cx0010, 0x002E,  Z(BinaryOutInactiveText),&Z_Copy },
  { Zuint32,  Cx0010, 0x0042,  Z(BinaryOutMinimumOffTime),&Z_Copy },
  { Zuint32,  Cx0010, 0x0043,  Z(BinaryOutMinimumOnTime),&Z_Copy },
  { Zbool,    Cx0010, 0x0051,  Z(BinaryOutOutOfService),&Z_Copy },
  { Zenum8,   Cx0010, 0x0054,  Z(BinaryOutPolarity),    &Z_Copy },
  { Zbool,    Cx0010, 0x0055,  Z(BinaryOutValue),       &Z_Copy },
  { Zunk,     Cx0010, 0x0057,  Z(BinaryOutPriorityArray),&Z_Copy },
  { Zenum8,   Cx0010, 0x0067,  Z(BinaryOutReliability), &Z_Copy },
  { Zbool,    Cx0010, 0x0068,  Z(BinaryOutRelinquishDefault),&Z_Copy },
  { Zmap8,    Cx0010, 0x006F,  Z(BinaryOutStatusFlags), &Z_Copy },
  { Zuint32,  Cx0010, 0x0100,  Z(BinaryOutApplicationType),&Z_Copy },

  // Binary Value cluster
  { Zstring,  Cx0011, 0x0004,  Z(BinaryActiveText),     &Z_Copy },
  { Zstring,  Cx0011, 0x001C,  Z(BinaryDescription),    &Z_Copy },
  { Zstring,  Cx0011, 0x002E,  Z(BinaryInactiveText),   &Z_Copy },
  { Zuint32,  Cx0011, 0x0042,  Z(BinaryMinimumOffTime), &Z_Copy },
  { Zuint32,  Cx0011, 0x0043,  Z(BinaryMinimumOnTime),  &Z_Copy },
  { Zbool,    Cx0011, 0x0051,  Z(BinaryOutOfService),   &Z_Copy },
  { Zbool,    Cx0011, 0x0055,  Z(BinaryValue),          &Z_Copy },
  { Zunk,     Cx0011, 0x0057,  Z(BinaryPriorityArray),  &Z_Copy },
  { Zenum8,   Cx0011, 0x0067,  Z(BinaryReliability),    &Z_Copy },
  { Zbool,    Cx0011, 0x0068,  Z(BinaryRelinquishDefault),&Z_Copy },
  { Zmap8,    Cx0011, 0x006F,  Z(BinaryStatusFlags),    &Z_Copy },
  { Zuint32,  Cx0011, 0x0100,  Z(BinaryApplicationType),&Z_Copy },

  // Multistate Input cluster
  { Zunk,     Cx0012, 0x000E,  Z(MultiInStateText),     &Z_Copy },
  { Zstring,  Cx0012, 0x001C,  Z(MultiInDescription),   &Z_Copy },
  { Zuint16,  Cx0012, 0x004A,  Z(MultiInNumberOfStates),&Z_Copy },
  { Zbool,    Cx0012, 0x0051,  Z(MultiInOutOfService),  &Z_Copy },
  { Zuint16,  Cx0012, 0x0055,  Z(MultiInValue),         &Z_AqaraCube },
  { Zenum8,   Cx0012, 0x0067,  Z(MultiInReliability),   &Z_Copy },
  { Zmap8,    Cx0012, 0x006F,  Z(MultiInStatusFlags),   &Z_Copy },
  { Zuint32,  Cx0012, 0x0100,  Z(MultiInApplicationType),&Z_Copy },

  // Multistate output
  { Zunk,     Cx0013, 0x000E,  Z(MultiOutStateText),    &Z_Copy },
  { Zstring,  Cx0013, 0x001C,  Z(MultiOutDescription),  &Z_Copy },
  { Zuint16,  Cx0013, 0x004A,  Z(MultiOutNumberOfStates),&Z_Copy },
  { Zbool,    Cx0013, 0x0051,  Z(MultiOutOutOfService), &Z_Copy },
  { Zuint16,  Cx0013, 0x0055,  Z(MultiOutValue),        &Z_Copy },
  // { Zunk,     Cx0013, 0x0057,  Z(MultiOutPriorityArray),&Z_Copy },
  { Zenum8,   Cx0013, 0x0067,  Z(MultiOutReliability),  &Z_Copy },
  { Zuint16,  Cx0013, 0x0068,  Z(MultiOutRelinquishDefault),&Z_Copy },
  { Zmap8,    Cx0013, 0x006F,  Z(MultiOutStatusFlags),  &Z_Copy },
  { Zuint32,  Cx0013, 0x0100,  Z(MultiOutApplicationType),&Z_Copy },

  // Multistate Value cluster
  { Zunk,     Cx0014, 0x000E,  Z(MultiStateText),       &Z_Copy },
  { Zstring,  Cx0014, 0x001C,  Z(MultiDescription),     &Z_Copy },
  { Zuint16,  Cx0014, 0x004A,  Z(MultiNumberOfStates),  &Z_Copy },
  { Zbool,    Cx0014, 0x0051,  Z(MultiOutOfService),    &Z_Copy },
  { Zuint16,  Cx0014, 0x0055,  Z(MultiValue),           &Z_Copy },
  { Zenum8,   Cx0014, 0x0067,  Z(MultiReliability),     &Z_Copy },
  { Zuint16,  Cx0014, 0x0068,  Z(MultiRelinquishDefault),&Z_Copy },
  { Zmap8,    Cx0014, 0x006F,  Z(MultiStatusFlags),     &Z_Copy },
  { Zuint32,  Cx0014, 0x0100,  Z(MultiApplicationType), &Z_Copy },

  // Power Profile cluster
  { Zuint8,   Cx001A, 0x0000,  Z(TotalProfileNum),      &Z_Copy },
  { Zbool,    Cx001A, 0x0001,  Z(MultipleScheduling),   &Z_Copy },
  { Zmap8,    Cx001A, 0x0002,  Z(EnergyFormatting),     &Z_Copy },
  { Zbool,    Cx001A, 0x0003,  Z(EnergyRemote),         &Z_Copy },
  { Zmap8,    Cx001A, 0x0004,  Z(ScheduleMode),         &Z_Copy },

  // Poll Control cluster
  { Zuint32,  Cx0020, 0x0000,  Z(CheckinInterval),      &Z_Copy },
  { Zuint32,  Cx0020, 0x0001,  Z(LongPollInterval),     &Z_Copy },
  { Zuint16,  Cx0020, 0x0002,  Z(ShortPollInterval),    &Z_Copy },
  { Zuint16,  Cx0020, 0x0003,  Z(FastPollTimeout),      &Z_Copy },
  { Zuint32,  Cx0020, 0x0004,  Z(CheckinIntervalMin),   &Z_Copy },
  { Zuint32,  Cx0020, 0x0005,  Z(LongPollIntervalMin),  &Z_Copy },
  { Zuint16,  Cx0020, 0x0006,  Z(FastPollTimeoutMax),   &Z_Copy },

  // Shade Configuration cluster
  { Zuint16,  Cx0100, 0x0000,  Z(PhysicalClosedLimit),  &Z_Copy },
  { Zuint8,   Cx0100, 0x0001,  Z(MotorStepSize),        &Z_Copy },
  { Zmap8,    Cx0100, 0x0002,  Z(Status),               &Z_Copy },
  { Zuint16,  Cx0100, 0x0010,  Z(ClosedLimit),          &Z_Copy },
  { Zenum8,   Cx0100, 0x0011,  Z(Mode),                 &Z_Copy },

  // Door Lock cluster
  { Zenum8,   Cx0101, 0x0000,  Z(LockState),            &Z_Copy },
  { Zenum8,   Cx0101, 0x0001,  Z(LockType),             &Z_Copy },
  { Zbool,    Cx0101, 0x0002,  Z(ActuatorEnabled),      &Z_Copy },
  { Zenum8,   Cx0101, 0x0003,  Z(DoorState),            &Z_Copy },
  { Zuint32,  Cx0101, 0x0004,  Z(DoorOpenEvents),       &Z_Copy },
  { Zuint32,  Cx0101, 0x0005,  Z(DoorClosedEvents),     &Z_Copy },
  { Zuint16,  Cx0101, 0x0006,  Z(OpenPeriod),           &Z_Copy },

  // Aqara Lumi Vibration Sensor
  { Zunk,     Cx0101, 0x0055,  Z(AqaraVibrationMode),   &Z_AqaraVibration },
  { Zunk,     Cx0101, 0x0503,  Z(AqaraVibrationsOrAngle), &Z_Copy },
  { Zunk,     Cx0101, 0x0505,  Z(AqaraVibration505),    &Z_Copy },
  { Zunk,     Cx0101, 0x0508,  Z(AqaraAccelerometer),   &Z_AqaraVibration },
  
  // Window Covering cluster
  { Zenum8,   Cx0102, 0x0000,  Z(WindowCoveringType),   &Z_Copy },
  { Zuint16,  Cx0102, 0x0001,  Z(PhysicalClosedLimitLift),&Z_Copy },
  { Zuint16,  Cx0102, 0x0002,  Z(PhysicalClosedLimitTilt),&Z_Copy },
  { Zuint16,  Cx0102, 0x0003,  Z(CurrentPositionLift),  &Z_Copy },
  { Zuint16,  Cx0102, 0x0004,  Z(CurrentPositionTilt),  &Z_Copy },
  { Zuint16,  Cx0102, 0x0005,  Z(NumberofActuationsLift),&Z_Copy },
  { Zuint16,  Cx0102, 0x0006,  Z(NumberofActuationsTilt),&Z_Copy },
  { Zmap8,    Cx0102, 0x0007,  Z(ConfigStatus),         &Z_Copy },
  { Zuint8,   Cx0102, 0x0008,  Z(CurrentPositionLiftPercentage),&Z_Copy },
  { Zuint8,   Cx0102, 0x0009,  Z(CurrentPositionTiltPercentage),&Z_Copy },
  { Zuint16,  Cx0102, 0x0010,  Z(InstalledOpenLimitLift),&Z_Copy },
  { Zuint16,  Cx0102, 0x0011,  Z(InstalledClosedLimitLift),&Z_Copy },
  { Zuint16,  Cx0102, 0x0012,  Z(InstalledOpenLimitTilt),&Z_Copy },
  { Zuint16,  Cx0102, 0x0013,  Z(InstalledClosedLimitTilt),&Z_Copy },
  { Zuint16,  Cx0102, 0x0014,  Z(VelocityLift),         &Z_Copy },
  { Zuint16,  Cx0102, 0x0015,  Z(AccelerationTimeLift),&Z_Copy },
  { Zuint16,  Cx0102, 0x0016,  Z(DecelerationTimeLift), &Z_Copy },
  { Zmap8,    Cx0102, 0x0017,  Z(Mode),                 &Z_Copy },
  { Zoctstr,  Cx0102, 0x0018,  Z(IntermediateSetpointsLift),&Z_Copy },
  { Zoctstr,  Cx0102, 0x0019,  Z(IntermediateSetpointsTilt),&Z_Copy },

  // Color Control cluster
  { Zuint8,   Cx0300, 0x0000,  Z(Hue),                  &Z_Copy },
  { Zuint8,   Cx0300, 0x0001,  Z(Sat),                  &Z_Copy },
  { Zuint16,  Cx0300, 0x0002,  Z(RemainingTime),        &Z_Copy },
  { Zuint16,  Cx0300, 0x0003,  Z(X),                    &Z_Copy },
  { Zuint16,  Cx0300, 0x0004,  Z(Y),                    &Z_Copy },
  { Zenum8,   Cx0300, 0x0005,  Z(DriftCompensation),    &Z_Copy },
  { Zstring,  Cx0300, 0x0006,  Z(CompensationText),     &Z_Copy },
  { Zuint16,  Cx0300, 0x0007,  Z(CT),                   &Z_Copy },
  { Zenum8,   Cx0300, 0x0008,  Z(ColorMode),            &Z_Copy },
  { Zuint8,   Cx0300, 0x0010,  Z(NumberOfPrimaries),    &Z_Copy },
  { Zuint16,  Cx0300, 0x0011,  Z(Primary1X),            &Z_Copy },
  { Zuint16,  Cx0300, 0x0012,  Z(Primary1Y),            &Z_Copy },
  { Zuint8,   Cx0300, 0x0013,  Z(Primary1Intensity),    &Z_Copy },
  { Zuint16,  Cx0300, 0x0015,  Z(Primary2X),            &Z_Copy },
  { Zuint16,  Cx0300, 0x0016,  Z(Primary2Y),            &Z_Copy },
  { Zuint8,   Cx0300, 0x0017,  Z(Primary2Intensity),    &Z_Copy },
  { Zuint16,  Cx0300, 0x0019,  Z(Primary3X),            &Z_Copy },
  { Zuint16,  Cx0300, 0x001A,  Z(Primary3Y),            &Z_Copy },
  { Zuint8,   Cx0300, 0x001B,  Z(Primary3Intensity),    &Z_Copy },
  { Zuint16,  Cx0300, 0x0030,  Z(WhitePointX),          &Z_Copy },
  { Zuint16,  Cx0300, 0x0031,  Z(WhitePointY),          &Z_Copy },
  { Zuint16,  Cx0300, 0x0032,  Z(ColorPointRX),         &Z_Copy },
  { Zuint16,  Cx0300, 0x0033,  Z(ColorPointRY),         &Z_Copy },
  { Zuint8,   Cx0300, 0x0034,  Z(ColorPointRIntensity), &Z_Copy },
  { Zuint16,  Cx0300, 0x0036,  Z(ColorPointGX),         &Z_Copy },
  { Zuint16,  Cx0300, 0x0037,  Z(ColorPointGY),         &Z_Copy },
  { Zuint8,   Cx0300, 0x0038,  Z(ColorPointGIntensity), &Z_Copy },
  { Zuint16,  Cx0300, 0x003A,  Z(ColorPointBX),         &Z_Copy },
  { Zuint16,  Cx0300, 0x003B,  Z(ColorPointBY),         &Z_Copy },
  { Zuint8,   Cx0300, 0x003C,  Z(ColorPointBIntensity), &Z_Copy },

  // Illuminance Measurement cluster
  { Zuint16,  Cx0400, 0x0000,  Z(Illuminance),           &Z_Copy },    // Illuminance (in Lux)
  { Zuint16,  Cx0400, 0x0001,  Z(IlluminanceMinMeasuredValue),     &Z_Copy },    //
  { Zuint16,  Cx0400, 0x0002,  Z(IlluminanceMaxMeasuredValue),     &Z_Copy },    //
  { Zuint16,  Cx0400, 0x0003,  Z(IlluminanceTolerance),            &Z_Copy },    //
  { Zenum8,   Cx0400, 0x0004,  Z(IlluminanceLightSensorType),      &Z_Copy },    //
  { Zunk,     Cx0400, 0xFFFF,  nullptr,                &Z_Remove },    // Remove all other values

  // Illuminance Level Sensing cluster
  { Zenum8,   Cx0401, 0x0000,  Z(IlluminanceLevelStatus),          &Z_Copy },    // Illuminance (in Lux)
  { Zenum8,   Cx0401, 0x0001,  Z(IlluminanceLightSensorType),      &Z_Copy },    // LightSensorType
  { Zuint16,  Cx0401, 0x0010,  Z(IlluminanceTargetLevel),          &Z_Copy },    // 
  { Zunk,     Cx0401, 0xFFFF,  nullptr,                &Z_Remove },    // Remove all other values

  // Temperature Measurement cluster
  { Zint16,   Cx0402, 0x0000,  Z(Temperature),          &Z_FloatDiv100 },   // Temperature
  { Zint16,   Cx0402, 0x0001,  Z(TemperatureMinMeasuredValue),     &Z_FloatDiv100 },    //
  { Zint16,   Cx0402, 0x0002,  Z(TemperatureMaxMeasuredValue),     &Z_FloatDiv100 },    //
  { Zuint16,  Cx0402, 0x0003,  Z(TemperatureTolerance),            &Z_FloatDiv100 },    //
  { Zunk,     Cx0402, 0xFFFF,  nullptr,                &Z_Remove },     // Remove all other values

  // Pressure Measurement cluster
  { Zunk,     Cx0403, 0x0000,  Z(PressureUnit),                 &Z_AddPressureUnit },     // Pressure Unit
  { Zint16,   Cx0403, 0x0000,  Z(Pressure),                     &Z_Copy },     // Pressure
  { Zint16,   Cx0403, 0x0001,  Z(PressureMinMeasuredValue),     &Z_Copy },    //
  { Zint16,   Cx0403, 0x0002,  Z(PressureMaxMeasuredValue),     &Z_Copy },    //
  { Zuint16,  Cx0403, 0x0003,  Z(PressureTolerance),            &Z_Copy },    //
  { Zint16,   Cx0403, 0x0010,  Z(PressureScaledValue),          &Z_Copy },    //
  { Zint16,   Cx0403, 0x0011,  Z(PressureMinScaledValue),       &Z_Copy },    //
  { Zint16,   Cx0403, 0x0012,  Z(PressureMaxScaledValue),       &Z_Copy },    //
  { Zuint16,  Cx0403, 0x0013,  Z(PressureScaledTolerance),      &Z_Copy },    //
  { Zint8,    Cx0403, 0x0014,  Z(PressureScale),                &Z_Copy },    //
  { Zunk,     Cx0403, 0xFFFF,  nullptr,                &Z_Remove },     // Remove all other Pressure values

  // Flow Measurement cluster
  { Zuint16,  Cx0404, 0x0000,  Z(FlowRate),             &Z_FloatDiv10 },    // Flow (in m3/h)
  { Zuint16,  Cx0404, 0x0001,  Z(FlowMinMeasuredValue), &Z_Copy },    //
  { Zuint16,  Cx0404, 0x0002,  Z(FlowMaxMeasuredValue), &Z_Copy },    //
  { Zuint16,  Cx0404, 0x0003,  Z(FlowTolerance),        &Z_Copy },    //
  { Zunk,     Cx0404, 0xFFFF,  nullptr,                &Z_Remove },    // Remove all other values

  // Relative Humidity Measurement cluster
  { Zuint16,  Cx0405, 0x0000,  Z(Humidity),             &Z_FloatDiv100 },   // Humidity
  { Zuint16,  Cx0405, 0x0001,  Z(HumidityMinMeasuredValue),     &Z_Copy },    //
  { Zuint16,  Cx0405, 0x0002,  Z(HumidityMaxMeasuredValue),     &Z_Copy },    //
  { Zuint16,  Cx0405, 0x0003,  Z(HumidityTolerance),            &Z_Copy },    //
  { Zunk,     Cx0405, 0xFFFF,  nullptr,                &Z_Remove },     // Remove all other values

  // Occupancy Sensing cluster
  { Zmap8,    Cx0406, 0x0000,  Z(Occupancy),            &Z_Copy },    // Occupancy (map8)
  { Zenum8,   Cx0406, 0x0001,  Z(OccupancySensorType),  &Z_Copy },    // OccupancySensorType
  { Zunk,     Cx0406, 0xFFFF,  nullptr,                &Z_Remove },    // Remove all other values

  // Meter Identification cluster
  { Zstring,  Cx0B01, 0x0000,  Z(CompanyName),          &Z_Copy },
  { Zuint16,  Cx0B01, 0x0001,  Z(MeterTypeID),          &Z_Copy },
  { Zuint16,  Cx0B01, 0x0004,  Z(DataQualityID),        &Z_Copy },
  { Zstring,  Cx0B01, 0x0005,  Z(CustomerName),         &Z_Copy },
  { Zoctstr,  Cx0B01, 0x0006,  Z(Model),                &Z_Copy },
  { Zoctstr,  Cx0B01, 0x0007,  Z(PartNumber),           &Z_Copy },
  { Zoctstr,  Cx0B01, 0x0008,  Z(ProductRevision),      &Z_Copy },
  { Zoctstr,  Cx0B01, 0x000A,  Z(SoftwareRevision),     &Z_Copy },
  { Zstring,  Cx0B01, 0x000B,  Z(UtilityName),          &Z_Copy },
  { Zstring,  Cx0B01, 0x000C,  Z(POD),                  &Z_Copy },
  { Zint24,   Cx0B01, 0x000D,  Z(AvailablePower),       &Z_Copy },
  { Zint24,   Cx0B01, 0x000E,  Z(PowerThreshold),       &Z_Copy },

  // Diagnostics cluster
  { Zuint16,  Cx0B05, 0x0000,  Z(NumberOfResets),       &Z_Copy },
  { Zuint16,  Cx0B05, 0x0001,  Z(PersistentMemoryWrites),&Z_Copy },
  { Zuint8,   Cx0B05, 0x011C,  Z(LastMessageLQI),       &Z_Copy },
  { Zuint8,   Cx0B05, 0x011D,  Z(LastMessageRSSI),      &Z_Copy },

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
int32_t Z_OccupancyCallback(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
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
  const char * modelId_c = zigbee_devices.getModelId(shortaddr);  // null if unknown
  String modelId((char*) modelId_c);

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
      if (modelId.startsWith(F("lumi.sensor_ht")) ||
          modelId.startsWith(F("lumi.weather"))) {     // Temp sensor
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
      } else if (modelId.startsWith(F("lumi.sensor_smoke"))) {   // gas leak
        if (0x64 == attrid) {
          json[F("SmokeDensity")] = val;
        }
      } else if (modelId.startsWith(F("lumi.sensor_natgas"))) {   // gas leak
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

      // see if we need to update the Hue bulb status
      if ((cluster == 0x0006) && ((attribute == 0x0000) || (attribute == 0x8000))) {
        bool power = value;
        zigbee_devices.updateHueState(shortaddr, &power, nullptr, nullptr, nullptr,
                                        nullptr, nullptr, nullptr, nullptr, nullptr);
      } else if ((cluster == 0x0008) && (attribute == 0x0000)) {
        uint8_t dimmer = value;
        zigbee_devices.updateHueState(shortaddr, nullptr, nullptr, &dimmer, nullptr,
                                        nullptr, nullptr, nullptr, nullptr, nullptr);
      } else if ((cluster == 0x0300) && (attribute == 0x0000)) {
        uint16_t hue8 = value;
        uint16_t hue = changeUIntScale(hue8, 0, 254, 0, 360);     // change range from 0..254 to 0..360
        zigbee_devices.updateHueState(shortaddr, nullptr, nullptr, nullptr, nullptr,
                                        nullptr, &hue, nullptr, nullptr, nullptr);
      } else if ((cluster == 0x0300) && (attribute == 0x0001)) {
        uint8_t sat = value;
        zigbee_devices.updateHueState(shortaddr, nullptr, nullptr, nullptr, &sat,
                                        nullptr, nullptr, nullptr, nullptr, nullptr);
      } else if ((cluster == 0x0300) && (attribute == 0x0003)) {
        uint16_t x = value;
        zigbee_devices.updateHueState(shortaddr, nullptr, nullptr, nullptr, nullptr,
                                        nullptr, nullptr, &x, nullptr, nullptr);
      } else if ((cluster == 0x0300) && (attribute == 0x0004)) {
        uint16_t y = value;
        zigbee_devices.updateHueState(shortaddr, nullptr, nullptr, nullptr, nullptr,
                                        nullptr, nullptr, nullptr, &y, nullptr), nullptr;
      } else if ((cluster == 0x0300) && (attribute == 0x0007)) {
        uint16_t ct = value;
        zigbee_devices.updateHueState(shortaddr, nullptr, nullptr, nullptr, nullptr,
                                        &ct, nullptr, nullptr, nullptr, nullptr);
      } else if ((cluster == 0x0300) && (attribute == 0x0008)) {
        uint8_t colormode = value;
        zigbee_devices.updateHueState(shortaddr, nullptr, &colormode, nullptr, nullptr,
                                        nullptr, nullptr, nullptr, nullptr, nullptr);
      }

      // Iterate on filter
      for (uint32_t i = 0; i < sizeof(Z_PostProcess) / sizeof(Z_PostProcess[0]); i++) {
        const Z_AttributeConverter *converter = &Z_PostProcess[i];
        uint16_t conv_cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
        uint16_t conv_attribute = pgm_read_word(&converter->attribute);

        if ((conv_cluster == cluster) &&
            ((conv_attribute == attribute) || (conv_attribute == 0xFFFF)) ) {
          String new_name_str = (const __FlashStringHelper*) converter->name;
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
