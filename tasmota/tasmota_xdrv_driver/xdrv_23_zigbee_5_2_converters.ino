/*
  xdrv_23_zigbee_converters.ino - zigbee support for Tasmota

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

//
// Find attribute matcher by name
//
class Z_attribute_match Z_findAttributeMatcherByName(uint16_t shortaddr, const char *name) {
  const Z_Device & device = zigbee_devices.findShortAddr(shortaddr);
  // works even if the device is unknown

  Z_attribute_match matched_attr = Z_plugin_matchAttributeByName(device.modelId, device.manufacturerId, name);
  if (!matched_attr.found()) {
    for (uint32_t i = 0; i < nitems(Z_PostProcess); i++) {
      const Z_AttributeConverter *converter = &Z_PostProcess[i];
      if (0 == pgm_read_word(&converter->name_offset)) { continue; }         // avoid strcasecmp_P() from crashing
      if (0 == strcasecmp_P(name, Z_strings + pgm_read_word(&converter->name_offset))) {
        matched_attr.cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
        matched_attr.attribute = pgm_read_word(&converter->attribute);
        matched_attr.name = (Z_strings + pgm_read_word(&converter->name_offset));
        int8_t multiplier8 = CmToMultiplier(pgm_read_byte(&converter->multiplier_idx));
        if (multiplier8 > 1) { matched_attr.multiplier = multiplier8; }
        if (multiplier8 < 0) { matched_attr.divider = -multiplier8; }
        matched_attr.zigbee_type = pgm_read_byte(&converter->type);
        uint8_t conv_mapping = pgm_read_byte(&converter->mapping);
        matched_attr.map_type = (Z_Data_Type) ((conv_mapping & 0xF0)>>4);
        matched_attr.map_offset = (conv_mapping & 0x0F);
        break;
      }
    }
  }
  return matched_attr;
}

//
// Find attribute matcher by name
//
// attr_glob: do Attr marked as 0xFFFF match any attribute
class Z_attribute_match Z_findAttributeMatcherById(uint16_t shortaddr, uint16_t cluster, uint16_t attr_id, bool attr_glob) {
  const Z_Device & device = zigbee_devices.findShortAddr(shortaddr);
  // works even if the device is unknown

  Z_attribute_match matched_attr = Z_plugin_matchAttributeById(device.modelId, device.manufacturerId, cluster, attr_id);
  if (!matched_attr.found()) {
    for (uint32_t i = 0; i < nitems(Z_PostProcess); i++) {
      const Z_AttributeConverter *converter = &Z_PostProcess[i];
      uint16_t conv_cluster = CxToCluster(pgm_read_byte(&converter->cluster_short));
      uint16_t conv_attr_id = pgm_read_word(&converter->attribute);

      if ((conv_cluster == cluster) && (conv_attr_id == attr_id || (attr_glob && conv_attr_id == 0xFFFF))) {
        matched_attr.cluster = cluster;
        matched_attr.attribute = attr_id;
        matched_attr.name = (Z_strings + pgm_read_word(&converter->name_offset));
        int8_t multiplier8 = CmToMultiplier(pgm_read_byte(&converter->multiplier_idx));
        if (multiplier8 > 1) { matched_attr.multiplier = multiplier8; }
        if (multiplier8 < 0) { matched_attr.divider = -multiplier8; }
        matched_attr.zigbee_type = pgm_read_byte(&converter->type);
        uint8_t conv_mapping = pgm_read_byte(&converter->mapping);
        matched_attr.map_type = (Z_Data_Type) ((conv_mapping & 0xF0)>>4);
        matched_attr.map_offset = (conv_mapping & 0x0F);
        break;
      }
    }
  }
  return matched_attr;
}

class ZCLFrame {
public:

  // constructor used when creating a message from scratch to send it later
  ZCLFrame(void);            // allocate 16 bytes by default
  ZCLFrame(size_t size);

  // constructore used when receiving a Zigbee frame and populating the class
  ZCLFrame(uint8_t frame_control, uint16_t manuf_code, uint8_t transact_seq, uint8_t cmd_id,
    const char *buf, size_t buf_len, uint16_t clusterid, uint16_t groupaddr,
    uint16_t srcaddr, uint8_t srcendpoint, uint8_t dstendpoint, uint8_t wasbroadcast,
    uint8_t linkquality, uint8_t securityuse, uint8_t seqnumber):
    manuf(manuf_code), transactseq(transact_seq), cmd(cmd_id),
    payload(buf_len ? buf_len : 250),      // allocate the data frame from source or preallocate big enough
    cluster(clusterid), groupaddr(groupaddr),
    shortaddr(srcaddr), srcendpoint(srcendpoint), dstendpoint(dstendpoint), _wasbroadcast(wasbroadcast),
    _linkquality(linkquality), _securityuse(securityuse), _seqnumber(seqnumber)
    {
      _frame_control.d8 = frame_control;
      direction = _frame_control.b.direction;
      clusterSpecific = (_frame_control.b.frame_type != 0);
      needResponse = !_frame_control.b.disable_def_resp;
      payload.addBuffer(buf, buf_len);
    };


  void log(void) {
    Response_P(PSTR("{\"" D_JSON_ZIGBEEZCL_RECEIVED "\":{"
                    "\"groupid\":%d," "\"clusterid\":\"0x%04X\"," "\"srcaddr\":\"0x%04X\","
                    "\"srcendpoint\":%d," "\"dstendpoint\":%d," "\"wasbroadcast\":%d,"
                    "\"" D_CMND_ZIGBEE_LINKQUALITY "\":%d," "\"securityuse\":%d," "\"seqnumber\":%d,"
                    "\"fc\":\"0x%02X\","
                    "\"frametype\":%d,\"direction\":%d,\"disableresp\":%d,"
                    "\"manuf\":\"0x%04X\",\"transact\":%d,"
                    "\"cmdid\":\"0x%02X\",\"payload\":\"%_B\"}}"),
                    groupaddr, cluster, shortaddr,
                    srcendpoint, dstendpoint, _wasbroadcast,
                    _linkquality, _securityuse, _seqnumber,
                    _frame_control,
                    _frame_control.b.frame_type, direction, _frame_control.b.disable_def_resp,
                    manuf, transactseq, cmd,
                    &payload);
    if (Settings->flag3.tuya_serial_mqtt_publish) {
      MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR));
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "%s"), ResponseData());
    }
  }

  static ZCLFrame parseRawFrame(const SBuffer &buf, uint8_t offset, uint8_t len, uint16_t clusterid, uint16_t groupid,
                                uint16_t srcaddr, uint8_t srcendpoint, uint8_t dstendpoint, uint8_t wasbroadcast,
                                uint8_t linkquality, uint8_t securityuse, uint8_t seqnumber) { // parse a raw frame and build the ZCL frame object
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
                       linkquality, securityuse, seqnumber);
    return zcl_frame;
  }

  bool isClusterSpecificCommand(void) const { return _frame_control.b.frame_type & 1; }
  uint8_t getDirection(void)          const { return direction; }

  // parsers for received messages
  void parseReportAttributes(Z_attribute_list& attr_list);
  void generateSyntheticAttributes(Z_attribute_list& attr_list);
  void removeInvalidAttributes(Z_attribute_list& attr_list);
  void applySynonymAttributes(Z_attribute_list& attr_list);
  void computeSyntheticAttributes(Z_attribute_list& attr_list);
  void generateCallBacks(Z_attribute_list& attr_list);
  void parseReadAttributes(uint16_t shortaddr, Z_attribute_list& attr_list);
  void parseReadAttributesResponse(Z_attribute_list& attr_list);
  void parseReadConfigAttributes(uint16_t shortaddr, Z_attribute_list& attr_list);
  void parseConfigAttributes(uint16_t shortaddr, Z_attribute_list& attr_list);
  void parseWriteAttributesResponse(Z_attribute_list& attr_list);
  void parseResponse(void);
  void parseResponse_inner(uint8_t cmd, bool cluster_specific, uint8_t status);
  void parseClusterSpecificCommand(Z_attribute_list& attr_list);

  // synthetic attributes converters
  void syntheticAqaraSensor(Z_attribute_list &attr_list, class Z_attribute &attr);
  void syntheticAqaraSensor2(Z_attribute_list &attr_list, class Z_attribute &attr);
  void syntheticAqaraCubeOrButton(Z_attribute_list &attr_list, class Z_attribute &attr);
  void syntheticAqaraVibration(Z_attribute_list &attr_list, class Z_attribute &attr);
  void syntheticAnalogValue(Z_attribute_list &attr_list, class Z_attribute &attr);

  // handle read attributes auto-responder
  void autoResponder(const uint16_t *attr_list_ids, size_t attr_len);

  inline void setGroupId(uint16_t groupid) {
    groupaddr = groupid;
  }

  inline void setClusterId(uint16_t clusterid) {
    cluster = clusterid;
  }

  inline uint16_t getSrcAddr(void) const { return shortaddr; }
  inline uint16_t getGroupAddr(void) const { return groupaddr; }
  inline uint16_t getClusterId(void) const { return cluster; }
  inline uint8_t  getLinkQuality(void) const { return _linkquality; }
  inline uint8_t getCmdId(void) const { return cmd; }
  inline uint16_t getSrcEndpoint(void) const { return srcendpoint; }
  const SBuffer &getPayload(void) const { return payload; }
  uint16_t getManufCode(void) const { return manuf; }

  inline void setTransac(uint8_t _transac) { transactseq = _transac; transacSet = true; }

  inline bool validShortaddr(void) const { return BAD_SHORTADDR != shortaddr; }
  inline bool validCluster(void)   const { return 0xFFFF != cluster; }
  inline bool validEndpoint(void)  const { return 0x00 != dstendpoint; }
  inline bool validCmd(void)       const { return 0xFF != cmd; }

public:
  uint16_t                manuf = 0;      // optional
  uint8_t                 transactseq = 0;    // transaction sequence number
  uint8_t                 cmd = 0;
  SBuffer                 payload;
  uint16_t                cluster = 0xFFFF;   // invalid cluster by default
  uint16_t                groupaddr = 0;
  // information from decoded ZCL frame
  uint16_t                shortaddr = BAD_SHORTADDR;   // BAD_SHORTADDR is broadcast, so considered invalid
  uint8_t                 dstendpoint = 0x00;        // 0x00 is invalid for the dst endpoint
  // attributes used in send-only mode
  bool                    clusterSpecific = false;
  bool                    needResponse = true;
  bool                    direct = false;                  // true if direct, false if discover router
  bool                    transacSet = false;              // is transac already set

  uint8_t                 srcendpoint = 0x00;        // 0x00 is invalid for the src endpoint
  bool                    direction = false;         // false = client to server (default), true = server to client (rare)

  // below private attributes are not used when sending a message
private:
  ZCLHeaderFrameControl_t _frame_control = { .d8 = 0 };
  bool                    _wasbroadcast = false;
  uint8_t                 _linkquality = 0x00;
  uint8_t                 _securityuse = 0;           // not used by Z2T, logging only
  uint8_t                 _seqnumber = 0;             // not used by Z2T, logging only
};

// define constructor seperately to avoid inlining and reduce Flash size
ZCLFrame::ZCLFrame(void) : payload(12) {};
ZCLFrame::ZCLFrame(size_t size) : payload(size) {};

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

//
// Appends the attribute value to Write or to Report
// Adds to buf:
// - n bytes: value (typically between 1 and 4 bytes, or bigger for strings)
// returns number of bytes of attribute, or <0 if error
// If the value is `NAN`, the value encoded is the "zigbee invalid value"
int32_t encodeSingleAttribute(SBuffer &buf, double val_d, const char *val_str, uint8_t attrtype) {
  uint32_t len = Z_getDatatypeLen(attrtype);    // pre-compute length, overloaded for variable length attributes
  bool nan = isnan(val_d);
  uint32_t u32 = nan ? 0xFFFFFFFF : roundf(val_d);
  int32_t  i32 = roundf(val_d);
  float    f32 = val_d;

  switch (attrtype) {
    // unsigned 8
    case Zbool:       // bool
    case Zuint8:      // uint8
    case Zenum8:      // enum8
    case Zdata8:      // data8
    case Zmap8:       // map8
      buf.add8(u32);
      break;
    // unsigned 16
    case Zuint16:     // uint16
    case Zenum16:     // enum16
    case Zdata16:     // data16
    case Zmap16:      // map16
      buf.add16(u32);
      break;
    // unisgned 32
    case Zuint24:
      buf.add16(u32);
      buf.add8(u32 >> 16);
      break;
    // unisgned 24
    case Zuint32:     // uint32
    case Zdata32:     // data32
    case Zmap32:      // map32
    case ZUTC:        // UTC - epoch 32 bits, seconds since 1-Jan-2000
      buf.add32(u32);
      break;

    // signed 8
    case Zint8:      // int8
      buf.add8(nan ? 0x80 : i32);
      break;
    case Zint16:      // int16
      buf.add16(nan ? 0x8000 : i32);
      break;
    case Zint32:      // int32
      buf.add32(nan ? 0x80000000 : i32);
      break;

    case Zsingle:      // float
      buf.add32( *((uint32_t*)&f32) );    // cast float as uint32_t
      break;

    case Zuint48:       // added for energy value
      {
        uint64_t u64 = val_d;
        buf.add32(u64);
        buf.add8(u64 >> 32);
      }
      break;

    case Zstring:
    case Zstring16:
      {
        if (nullptr == val_str) { return -2; }
        size_t val_len = strlen(val_str);
        if (val_len > 32) { val_len = 32; }
        len = val_len + 1;
        buf.add8(val_len);
        if (Zstring16 == attrtype) {
          buf.add8(0);    // len is on 2 bytes
          len++;
        }
        for (uint32_t i = 0; i < val_len; i++) {
          buf.add8(val_str[i]);
        }
      }
      break;

    default:
      return -1;
  }
  return len;
}

//
// parse a single attribute
//
// Input:
//   attr: attribute object to store to
//   buf:  the buffer to read from
//   offset: location in the buffer to read from
//   attrtype: type of attribute (byte) or -1 to read from the stream as first byte
// Output:
//   return: the length in bytes of the attribute
uint32_t parseSingleAttribute(Z_attribute & attr, const SBuffer &buf,
                              uint32_t offset, int32_t attrtype = -1) {

  uint32_t i = offset;
  if (attrtype < 0) {
    attrtype = buf.get8(i++);
  }

  // fallback - enter a null value
  attr.setNone();   // set to null by default

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
          attr.setUInt(uint8_val);
        }
      }
      break;
    case Zuint16:      // uint16
    case Zenum16:      // enum16
      {
        uint16_t uint16_val = buf.get16(i);
        // i += 2;
        if (0xFFFF != uint16_val) {
          attr.setUInt(uint16_val);
        }
      }
      break;
    case Zuint24:
      {
        uint32_t uint24_val = buf.get16(i) + (buf.get8(i+2) >> 16);
        // i += 3;
        if (0xFFFFFF != uint24_val) {
          attr.setUInt(uint24_val);
        }
      }
      break;
    case Zuint32:      // uint32
    case ZUTC:      // UTC
      {
        uint32_t uint32_val = buf.get32(i);
        // i += 4;
        if (0xFFFFFFFF != uint32_val) {
          attr.setUInt(uint32_val);
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
        // print as HEX "0x...."
        char hex[2*len+3];
        snprintf_P(hex, sizeof(hex), PSTR("0x"));
        for (uint32_t j=0; j<len; j++) {
          snprintf_P(hex, sizeof(hex), PSTR("%s%02X"), hex, buf.get8(i+len-j-1));
        }
        attr.setStr(hex);
        // i += len;
      }
      break;
    case Zint8:      // int8
      {
        int8_t int8_val = buf.get8(i);
        // i += 1;
        if (0x80 != int8_val) {
          attr.setInt(int8_val);
        }
      }
      break;
    case Zint16:      // int16
      {
        int16_t int16_val = buf.get16(i);
        // i += 2;
        if (0x8000 != int16_val) {
          attr.setInt(int16_val);
        }
      }
      break;
    case Zint32:      // int32
      {
        int32_t int32_val = buf.get32(i);
        // i += 4;
        if (-0x80000000 != int32_val) {
          attr.setInt(int32_val);
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
        len = (attrtype <= Zstring) ? buf.get8(i) : buf.get16(i);    // len is 8 or 16 bits
        i += (attrtype <= Zstring) ? 1 : 2;                                   // increment pointer
        if (i + len > buf.len()) {        // make sure we don't get past the buffer
          len = buf.len() - i;
        }

        // check if we can safely use a string
        if ((Zoctstr == attrtype) || (Zoctstr16 == attrtype)) { parse_as_string = false; }

        if (parse_as_string) {
          char str[len+1];
          strncpy(str, buf.charptr(i), len);
          str[len] = 0x00;
          attr.setStr(str);
        } else {
          attr.setBuf(buf, i, len);
        }

        // i += len;
        // break;
      }
      // i += buf.get8(i) + 1;
      break;

    case Zstruct:
      {
        uint16_t struct_size = buf.get16(i);
        len = 2;
        if (0xFFFF != struct_size) {
          if (struct_size > 16) { struct_size = 16; }
          // parse inner attributes - supports only fixed length for now
          for (uint32_t j = 0; j < struct_size; j++) {
            uint8_t attr_type = buf.get8(i+len);
            len += Z_getDatatypeLen(attr_type) + 1;
          }
        attr.setBuf(buf, i, len);
        }
      }
      break;

    case Zdata8:      // data8
    case Zmap8:      // map8
      {
        uint8_t uint8_val = buf.get8(i);
        // i += 1;
        attr.setUInt(uint8_val);
      }
      break;
    case Zdata16:      // data16
    case Zmap16:      // map16
      {
        uint16_t uint16_val = buf.get16(i);
        // i += 2;
        attr.setUInt(uint16_val);
      }
      break;
    case Zdata24:      // data16
    case Zmap24:      // map16
      {
        uint32_t uint32_val = buf.get16(i);
        uint8_t high = buf.get8(i+2);
        uint32_val = uint32_val | (high << 16);
        // i += 3;
        attr.setUInt(uint32_val);
      }
      break;
    case Zdata32:      // data32
    case Zmap32:      // map32
      {
        uint32_t uint32_val = buf.get32(i);
        // i += 4;
        attr.setUInt(uint32_val);
      }
      break;

    case Zsingle:      // float
      {
        uint32_t uint32_val = buf.get32(i);
        float  * float_val = (float*) &uint32_val;
        // i += 4;
        attr.setFloat(*float_val);
      }
      break;

    // All other fixed size, convert to a HEX dump
    case ZToD:      // ToD
    case Zdate:      // date
    case ZclusterId:      // clusterId
    case ZattribId:      // attribId
    case ZbacOID:      // bacOID
    case ZEUI64:      // EUI64
    case Zkey128:      // key128
    case Zsemi:      // semi (float on 2 bytes)
      {
          attr.setBuf(buf, i, len);
      }
      // i += 16;
      break;

    // Other un-implemented data types
    case Zdata40:      // data40
    case Zdata48:      // data48
    case Zdata56:      // data56
    case Zdata64:      // data64
      break;
    // map<x>
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
        attr.setFloat((float) *double_val);
      }
      break;
  }
  i += len;

  return i - offset;    // how much have we increased the index
}

// First pass, parse all attributes in their native format
void ZCLFrame::parseReportAttributes(Z_attribute_list& attr_list) {
  uint32_t i = 0;
  uint32_t len = payload.len();

  if (ZCL_WRITE_ATTRIBUTES == getCmdId()) {
    attr_list.addAttribute(PSTR("Command"), true).setStr(PSTR("Write"));
  }

  while (len >= i + 3) {
    uint16_t attrid = payload.get16(i);
    i += 2;

    // exception for Xiaomi lumi.weather - specific field to be treated as octet and not char
    if ((0x0000 == cluster) && (0xFF01 == attrid)) {
      if (0x42 == payload.get8(i)) {
        payload.set8(i, 0x41);   // change type from 0x42 to 0x41
      }
    }

    // TODO look for suffix
    Z_attribute & attr = attr_list.addAttribute(cluster, attrid);

    i += parseSingleAttribute(attr, payload, i);
  }

  // Issue Philips outdoor motion sensor SML002, see https://github.com/Koenkk/zigbee2mqtt/issues/897
  // The sensor expects the coordinator to send a Default Response to acknowledge the attribute reporting
  if (0 == _frame_control.b.disable_def_resp) {
    // the device expects a default response
    ZCLFrame zcl(2);   // message is 2 bytes
    zcl.shortaddr = shortaddr;
    zcl.cluster = cluster;
    zcl.dstendpoint = srcendpoint;
    zcl.cmd = ZCL_DEFAULT_RESPONSE;
    zcl.manuf = manuf;
    zcl.clusterSpecific = false;  /* not cluster specific */
    zcl.needResponse = false;     /* noresponse */
    zcl.direct = true;            /* direct no retry */
    zcl.setTransac(transactseq);
    zcl.payload.add8(cmd);
    zcl.payload.add8(0);    // Status = OK
    zigbeeZCLSendCmd(zcl);
  }
}

//
// Extract attributes hidden in other compound attributes
//
void ZCLFrame::generateSyntheticAttributes(Z_attribute_list& attr_list) {
  // scan through attributes and apply specific converters
  for (auto &attr : attr_list) {
    if (attr.key_is_str && attr.key_is_cmd) { continue; }    // pass if key is a name
    uint32_t ccccaaaa = (attr.cluster << 16) | attr.attr_id;

    switch (ccccaaaa) {      // 0xccccaaaa . c=cluster, a=attribute
      case 0x0000FF01:
        syntheticAqaraSensor(attr_list, attr);
        break;
      case 0x0000FF02:
        syntheticAqaraSensor2(attr_list, attr);
        break;
      case 0x00120055:
        syntheticAqaraCubeOrButton(attr_list, attr);
        break;
      case 0x01010055:
      case 0x01010508:
        syntheticAqaraVibration(attr_list, attr);
        break;
      case 0x000C0055:    // Analog Value
        syntheticAnalogValue(attr_list, attr);
        break;
    }
  }
}

//
// Remove invalid values
//
void ZCLFrame::removeInvalidAttributes(Z_attribute_list& attr_list) {
  // scan through attributes and apply specific converters
  for (auto &attr : attr_list) {
    if (attr.key_is_str && attr.key_is_cmd) { continue; }    // pass if key is a name
    uint32_t ccccaaaa = (attr.cluster << 16) | attr.attr_id;

    switch (ccccaaaa) {      // 0xccccaaaa . c=cluster, a=attribute
      case 0x04020000:       // Temperature
        if (attr.getInt() <= -10000) {
          // #9978, remove temperature of -100.00°C sent by lumi.weather
          attr_list.removeAttribute(&attr);
        }
        break;
    }
  }
}


//
// Apply synonyms from the plug-in synonym definitions
//
void ZCLFrame::applySynonymAttributes(Z_attribute_list& attr_list) {
  Z_Device & device = zigbee_devices.findShortAddr(shortaddr);

  String modelId((char*) device.modelId);
  // scan through attributes and apply specific converters
  for (auto &attr : attr_list) {
    if (attr.key_is_str) { continue; }    // pass if key is a name

    // first apply synonyms from plugins
    Z_attribute_synonym syn = Z_plugin_matchAttributeSynonym(device.modelId, device.manufacturerId,
                                                              attr.cluster, attr.attr_id);
    if (syn.found()) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("ZIG: apply synonym %04X/%04X with %04X/%04X (mul:%i div:%i)"), attr.cluster, attr.attr_id, syn.new_cluster, syn.new_attribute, syn.multiplier, syn.divider);
      if (syn.new_attribute == 0xFFFF) {    // if attr is 0xFFFF, remove attribute
        attr_list.removeAttribute(&attr);
      } else {
        attr.setKeyId(syn.new_cluster, syn.new_attribute);
        if ((syn.multiplier != 1 && syn.multiplier != 0) || (syn.divider != 1 && syn.divider != 0) || (syn.base != 0)) {
          // we need to change the value
          float fval = attr.getFloat();
          if (syn.multiplier != 1 && syn.multiplier != 0) {
            fval = fval * syn.multiplier;
          }
          if (syn.divider != 1 && syn.divider != 0) {
            fval = fval / syn.divider;
          }
          if (syn.base != 0) {
            fval = fval + syn.base;
          }
          attr.setFloat(fval);
        }
      }
    }
  }
}

//
// Compute new attributes based on the standard set
// Note: both function are now split to compute on extracted attributes
//
void ZCLFrame::computeSyntheticAttributes(Z_attribute_list& attr_list) {
  Z_Device & device = zigbee_devices.findShortAddr(shortaddr);

  String modelId((char*) device.modelId);
  // scan through attributes and apply specific converters
  for (auto &attr : attr_list) {
    if (attr.key_is_str) { continue; }    // pass if key is a name

    uint32_t ccccaaaa = (attr.cluster << 16) | attr.attr_id;

    switch (ccccaaaa) {      // 0xccccaaaa . c=cluster, a=attribute
      case 0x00010020:       // BatteryVoltage
        if (attr_list.countAttribute(0x0001,0x0021) == 0) {   // if it does not already contain BatteryPercentage
          uint32_t mv = attr.getUInt()*100;
          attr_list.addAttribute(0x0001, 0x0021).setUInt(toPercentageCR2032(mv) * 2);
        }
        break;
      case 0x02010008:    // Pi Heating Demand - solve Eutotronic bug
      case 0x02014008:    // Eurotronic Host Flags decoding
        {
          const char * manufacturer_c = zigbee_devices.getManufacturerId(shortaddr);  // null if unknown
          String manufacturerId((char*) manufacturer_c);
          if (manufacturerId.equals(F("Eurotronic"))) {
            if (ccccaaaa == 0x02010008) {
              // Eurotronic does not report 0..100 but 0..255, including 255 which is normally an ivalid value
              uint8_t valve = attr.getUInt();
              if (attr.isNone()) { valve = 255; }
              uint8_t valve_100 = changeUIntScale(valve, 0, 255, 0, 100);
              attr.setUInt(valve_100);
            } else if (ccccaaaa == 0x02014008) {
              uint32_t mode = attr.getUInt();
              if (mode & 0x02) { attr_list.addAttribute(0x0201, 0xF002).setUInt(1); }
              if (mode & 0x04) { attr_list.addAttribute(0x0201, 0xF004).setUInt(1); }
              if (mode & 0x10) { attr_list.addAttribute(0x0201, 0xF010).setUInt(1); }
              if (mode & 0x80) { attr_list.addAttribute(0x0201, 0xF080).setUInt(1); }
            }
          }
        }
        break;
      case 0x03000000:    // Hue
      case 0x03000001:    // Sat
      case 0x03000003:    // X
      case 0x03000004:    // Y
        {                 // generate synthetic RGB
          const Z_attribute * attr_rgb = attr_list.findAttribute(PSTR("RGB"));
          if (attr_rgb == nullptr) {      // make sure we didn't already computed it
            uint8_t brightness = 255;
            if (device.valid()) {
              const Z_Data_Light & light = device.data.find<Z_Data_Light>(srcendpoint);
              if ((&light != &z_data_unk) && (light.validDimmer())) {
                // Dimmer has a valid value
                brightness = changeUIntScale(light.getDimmer(), 0, 254, 0, 255);   // range is 0..255
              }
            }

            const Z_attribute * attr_hue = attr_list.findAttribute(0x0300, 0x0000);
            const Z_attribute * attr_sat = attr_list.findAttribute(0x0300, 0x0001);
            const Z_attribute * attr_x   = attr_list.findAttribute(0x0300, 0x0003);
            const Z_attribute * attr_y   = attr_list.findAttribute(0x0300, 0x0004);
            if (attr_hue && attr_sat) {
              uint8_t sat = changeUIntScale(attr_sat->getUInt(), 0, 254, 0, 255);
              uint16_t hue = changeUIntScale(attr_hue->getUInt(), 0, 254, 0, 360);
              Z_Data_Light::toRGBAttributesHSB(attr_list, hue, sat, brightness);
            } else if (attr_x && attr_y) {
              Z_Data_Light::toRGBAttributesXYB(attr_list, attr_x->getUInt(), attr_y->getUInt(), brightness);
            }
          }
        }
        break;
      case 0x04030000:    // SeaPressure
        {
          int16_t pressure = attr.getInt();
          int16_t pressure_sealevel = (pressure / FastPrecisePow(1.0 - ((float)Settings->altitude / 44330.0f), 5.255f)) - 21.6f;
          attr_list.addAttribute(0x0403, 0xFFF0).setInt(pressure_sealevel);
          // We create a synthetic attribute 0403/FFF0 to indicate sea level
        }
        break;
      case 0x05000002:    // ZoneStatus
        {
          const Z_Data_Alarm & alarm = (const Z_Data_Alarm&) zigbee_devices.getShortAddr(shortaddr).data.find(Z_Data_Type::Z_Alarm, srcendpoint);
          if (&alarm != nullptr) {
            alarm.convertZoneStatus(attr_list, attr.getUInt());
          }
        }
        break;
      // convert AC multipliers/dividers
      case 0x0B040600 ... 0x0B040605:   // cluser 0x0B04 - attr 0x0600..0x0605
        {
          uint16_t val = attr.getUInt();
          Z_Data_Plug & plug = device.data.get<Z_Data_Plug>();
          if (&plug != &z_data_unk) {
            switch (ccccaaaa) {
              case 0x0B040600:  plug.setACVoltageMul(val);    break;
              case 0x0B040601:  plug.setACVoltageDiv(val);    break;
              case 0x0B040602:  plug.setACCurrentMul(val);    break;
              case 0x0B040603:  plug.setACCurrentDiv(val);    break;
              case 0x0B040604:  plug.setACPowerMul(val);      break;
              case 0x0B040605:  plug.setACPowerDiv(val);      break;
            }
          }
          // AddLog(LOG_LEVEL_INFO, ">>>: cluster=0x%04X attr=0x%04X v=%i", attr.cluster, attr.attr_id, attr.getUInt());
        }
        break;
    }
  }
}

// Set deferred callbacks for Occupancy
// TODO make delay a parameter
void ZCLFrame::generateCallBacks(Z_attribute_list& attr_list) {
  static const uint32_t OCCUPANCY_TIMEOUT = 90 * 1000;  // 90 s
  // scan through attributes and apply specific converters
  for (auto &attr : attr_list) {
    if (attr.key_is_str && attr.key_is_cmd) { continue; }    // pass if key is a name
    uint32_t ccccaaaa = (attr.cluster << 16) | attr.attr_id;

    switch (ccccaaaa) {       // 0xccccaaaa . c=cluster, a=attribute
      case 0x04060000:        // Occupancy
        uint32_t occupancy = attr.getUInt();
        if (occupancy) {
          uint32_t pir_timer = OCCUPANCY_TIMEOUT;
          const Z_Data_PIR & pir_found = (const Z_Data_PIR&) zigbee_devices.getShortAddr(shortaddr).data.find(Z_Data_Type::Z_PIR, srcendpoint);
          if (&pir_found != nullptr) {
            pir_timer = pir_found.getTimeoutSeconds() * 1000;
          }
          if (pir_timer > 0) {
            zigbee_devices.setTimer(shortaddr, 0 /* groupaddr */, pir_timer, cluster, srcendpoint, Z_CAT_VIRTUAL_OCCUPANCY, 0, &Z_OccupancyCallback);
          }
        } else {
          zigbee_devices.resetTimersForDevice(shortaddr, 0 /* groupaddr */, Z_CAT_VIRTUAL_OCCUPANCY);
        }
        break;
    }
  }
}


// A command has been sent to a device this device, or to a group
// Set timers to read back values.
// If it's a device address, also set a timer for reachability test
void sendHueUpdate(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint = 0) {
  uint32_t wait_ms = 0xFFFF;

  switch (cluster) {
    case 0x0006:
      wait_ms = 200;    // wait 0.2 s
      break;
    case 0x0008:
      wait_ms = 1050;   // wait 1.0 s
      break;
    case 0x0102:
      wait_ms = 10000;   // wait 10.0 s
      break;
    case 0x0300:
      wait_ms = 1050;   // wait 1.0 s
      break;
    default:
      break;
  }
  if (0xFFFF != wait_ms) {
    if ((BAD_SHORTADDR != shortaddr) && (0 == endpoint)) {
      endpoint = zigbee_devices.findFirstEndpoint(shortaddr);
    }
    if ((BAD_SHORTADDR == shortaddr) || (endpoint)) {   // send if group address or endpoint is known
      zigbee_devices.queueTimer(shortaddr, groupaddr, wait_ms, cluster, endpoint, Z_CAT_READ_CLUSTER, 0 /* value */, &Z_ReadAttrCallback);
      if (BAD_SHORTADDR != shortaddr) {      // reachability test is not possible for group addresses, since we don't know the list of devices in the group
        zigbee_devices.setTimer(shortaddr, groupaddr, wait_ms + Z_CAT_REACHABILITY_TIMEOUT, cluster, endpoint, Z_CAT_REACHABILITY, 0 /* value */, &Z_Unreachable);
      }

    }
  }
}

// ZCL_READ_ATTRIBUTES
void ZCLFrame::parseReadAttributes(uint16_t shortaddr, Z_attribute_list& attr_list) {
  uint32_t i = 0;
  uint32_t len = payload.len();

  uint16_t read_attr_ids[len/2];

  attr_list.addAttributePMEM(PSTR(D_CMND_ZIGBEE_CLUSTER)).setUInt(cluster);

  JsonGeneratorArray attr_numbers;
  Z_attribute_list attr_names;
  while (len >= 2 + i) {
    uint16_t attrid = payload.get16(i);
    attr_numbers.add((uint32_t)attrid);
    read_attr_ids[i/2] = attrid;

    // find the attribute name
    Z_attribute_match matched_attr = Z_findAttributeMatcherById(shortaddr, cluster, attrid, false);
    if (matched_attr.found()) {
      attr_names.addAttribute(matched_attr.name, true).setBool(true);
    }
    i += 2;
  }
  attr_list.addAttributePMEM(PSTR("Read")).setStrRaw(attr_numbers.toString().c_str());
  attr_list.addAttributePMEM(PSTR("ReadNames")).setStrRaw(attr_names.toString(true).c_str());

  // call auto-responder only if src address if different from ourselves and it was a broadcast
  if (shortaddr != localShortAddr || !_wasbroadcast) {
    autoResponder(read_attr_ids, len/2);
  }
}

// ZCL_CONFIGURE_REPORTING_RESPONSE
void ZCLFrame::parseConfigAttributes(uint16_t shortaddr, Z_attribute_list& attr_list) {
  uint32_t len = payload.len();

  Z_attribute_list attr_config_list;
  for (uint32_t i=0; len >= i+4; i+=4) {
    uint8_t  status = payload.get8(i);
    uint16_t attr_id = payload.get8(i+2);

    Z_attribute_list attr_config_response;
    attr_config_response.addAttributePMEM(PSTR("Status")).setUInt(status);
    attr_config_response.addAttributePMEM(PSTR("StatusMsg")).setStr(getZigbeeStatusMessage(status).c_str());

    Z_attribute_match attr_matched = Z_findAttributeMatcherById(shortaddr, cluster, attr_id, false);
    if (attr_matched.found()) {
      const __FlashStringHelper* attr_name = (const __FlashStringHelper*) attr_matched.name;
      attr_config_list.addAttribute(attr_name).setStrRaw(attr_config_response.toString(true).c_str());
    } else {
      attr_config_list.addAttribute(cluster, attr_id).setStrRaw(attr_config_response.toString(true).c_str());
    }
  }

  Z_attribute &attr_1 = attr_list.addAttributePMEM(PSTR("ConfigResponse"));
  attr_1.setStrRaw(attr_config_list.toString(true).c_str());
}

// ZCL_WRITE_ATTRIBUTES_RESPONSE
void ZCLFrame::parseWriteAttributesResponse(Z_attribute_list& attr_list) {
  parseResponse_inner(ZCL_WRITE_ATTRIBUTES_RESPONSE, false, payload.get8(0));
}

// ZCL_READ_REPORTING_CONFIGURATION_RESPONSE
void ZCLFrame::parseReadConfigAttributes(uint16_t shortaddr, Z_attribute_list& attr_list) {
  uint32_t i = 0;
  uint32_t len = payload.len();

  Z_attribute &attr_root = attr_list.addAttributePMEM(PSTR("ReadConfig"));
  Z_attribute_list attr_1;

  while (len >= i + 4) {
    uint8_t  status = payload.get8(i);
    uint8_t  direction = payload.get8(i+1);
    uint16_t attrid = payload.get16(i+2);

    Z_attribute_list attr_2;
    if (direction) {
      attr_2.addAttributePMEM(PSTR("DirectionReceived")).setBool(true);
    }

    // find the multiplier
    uint32_t multiplier = 1;
    uint32_t divider = 1;
    int32_t base = 0;
    Z_attribute_match matched_attr = Z_findAttributeMatcherById(shortaddr, cluster, attrid, false);
    if (matched_attr.found()) {
      attr_2.addAttribute(matched_attr.name, true).setBool(true);
      multiplier = matched_attr.multiplier;
      divider = matched_attr.divider;
      base = matched_attr.base;
    }
    i += 4;
    if (0 != status) {
      attr_2.addAttributePMEM(PSTR("Status")).setUInt(status);
      attr_2.addAttributePMEM(PSTR("StatusMsg")).setStr(getZigbeeStatusMessage(status).c_str());
    } else {
      // no error, decode data
      if (direction) {
        // only Timeout period is present
        uint16_t attr_timeout = payload.get16(i);
        i += 2;
        attr_2.addAttributePMEM(PSTR("TimeoutPeriod")).setUInt((0xFFFF == attr_timeout) ? -1 : attr_timeout);
      } else {
        // direction == 0, we have a data type
        uint8_t attr_type = payload.get8(i);
        bool attr_discrete = Z_isDiscreteDataType(attr_type);
        uint16_t attr_min_interval = payload.get16(i+1);
        uint16_t attr_max_interval = payload.get16(i+3);
        i += 5;
        attr_2.addAttributePMEM(PSTR("MinInterval")).setUInt((0xFFFF == attr_min_interval) ? -1 : attr_min_interval);
        attr_2.addAttributePMEM(PSTR("MaxInterval")).setUInt((0xFFFF == attr_max_interval) ? -1 : attr_max_interval);
        if (!attr_discrete) {
          // decode Reportable Change
          Z_attribute &attr_change = attr_2.addAttributePMEM(PSTR("ReportableChange"));
          i += parseSingleAttribute(attr_change, payload, i, attr_type);
          
          if ((multiplier != 1 && multiplier != 0) || (divider != 1 && divider != 0) || (base != 0)) {
            float fval = attr_change.getFloat();
            if (multiplier != 1 && multiplier != 0) {
              fval = fval * multiplier;
            }
            if (divider != 1 && divider != 0) {
              fval = fval / divider;
            }
            if (base != 0) {
              fval = fval + base;
            }
            attr_change.setFloat(fval);
          }
        }
      }
    }
    attr_1.addAttribute(cluster, attrid).setStrRaw(attr_2.toString(true).c_str());
  }
  attr_root.setStrRaw(attr_1.toString(true).c_str());
}

// ZCL_READ_ATTRIBUTES_RESPONSE
void ZCLFrame::parseReadAttributesResponse(Z_attribute_list& attr_list) {
  uint32_t i = 0;
  uint32_t len = payload.len();

  while (len >= i + 4) {
    uint16_t attrid = payload.get16(i);
    i += 2;
    uint8_t status = payload.get8(i++);

    if (0 == status) {
      Z_attribute & attr = attr_list.addAttribute(cluster, attrid);
      i += parseSingleAttribute(attr, payload, i);
    }
  }
}


void ZCLFrame::parseResponse_inner(uint8_t cmd, bool cluster_specific, uint8_t status) {
  Z_attribute_list attr_list;

  // "Device"
  char s[12];
  snprintf_P(s, sizeof(s), PSTR("0x%04X"), shortaddr);
  attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_DEVICE)).setStr(s);
  // "Name"
  const char * friendlyName = zigbee_devices.getFriendlyName(shortaddr);
  if (friendlyName) {
    attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_NAME)).setStr(friendlyName);
  }
  // "Command"
  snprintf_P(s, sizeof(s), PSTR("%04X%c%02X"), cluster, cluster_specific ? '!' : '_', cmd);
  attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_CMD)).setStr(s);
  // "Status"
  attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_STATUS)).setUInt(status);
  // "StatusMessage"
  attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_STATUS_MSG)).setStr(getZigbeeStatusMessage(status).c_str());
  // Add Endpoint
  attr_list.addAttributePMEM(PSTR(D_CMND_ZIGBEE_ENDPOINT)).setUInt(srcendpoint);
  // Add Group if non-zero
  if (groupaddr) {     // TODO what about group zero
    attr_list.group_id = groupaddr;
  }
  // Add linkquality
  attr_list.lqi = _linkquality;

  Response_P(PSTR("{\"" D_JSON_ZIGBEE_RESPONSE "\":%s}"), attr_list.toString(true).c_str());
  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_ZIGBEEZCL_RECEIVED));
}

// ZCL_DEFAULT_RESPONSE
void ZCLFrame::parseResponse(void) {
  if (payload.len() < 2) { return; }   // wrong format
  uint8_t cmd = payload.get8(0);
  uint8_t status = payload.get8(1);

  parseResponse_inner(cmd, true, status);
}

/*********************************************************************************************\
 * Callbacks
\*********************************************************************************************/
// Reset the debounce marker
void Z_ResetDebounce(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  zigbee_devices.getShortAddr(shortaddr).debounce_endpoint = 0;
}

// Parse non-normalized attributes
void ZCLFrame::parseClusterSpecificCommand(Z_attribute_list& attr_list) {
  // Check if debounce is active and if the packet is a duplicate
  Z_Device & device = zigbee_devices.getShortAddr(shortaddr);
  if ((device.debounce_endpoint != 0) && (device.debounce_endpoint == srcendpoint) && (device.debounce_transact == transactseq)) {
    // this is a duplicate, drop the packet
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "Discarding duplicate command from 0x%04X, endpoint %d"), shortaddr, srcendpoint);
  } else {
    // reset the duplicate marker, parse the packet normally, and set a timer to reset the marker later (which will discard any existing timer for the same device/endpoint)
    device.debounce_endpoint = srcendpoint;
    device.debounce_transact = transactseq;
    zigbee_devices.setTimer(shortaddr, 0 /* groupaddr */, USE_ZIGBEE_DEBOUNCE_COMMANDS, 0 /*clusterid*/, srcendpoint, Z_CAT_DEBOUNCE_CMD, 0, &Z_ResetDebounce);

    bool cmd_parsed = false;
    if (srcendpoint == 0xF2 && dstendpoint == 0xF2 && cluster == 0x0021) {
      // handle Green Power commands
      cmd_parsed = convertGPSpecific(attr_list, cmd, direction, shortaddr, _wasbroadcast, payload);
    }
    // was it successfully parsed already?
    if (!cmd_parsed) {
      // handle normal commands
      convertClusterSpecific(attr_list, cluster, cmd, direction, shortaddr, srcendpoint, payload);
      if (!Settings->flag5.zb_disable_autoquery) {
      // read attributes unless disabled
        if (!direction) {    // only handle server->client (i.e. device->coordinator)
          if (_wasbroadcast) {                // only update for broadcast messages since we don't see unicast from device to device and we wouldn't know the target
            sendHueUpdate(BAD_SHORTADDR, groupaddr, cluster);
          }
        }
      }
    }
  }
  // Send Default Response to acknowledge the command
  if (0 == _frame_control.b.disable_def_resp) {
    // the device expects a default response
    ZCLFrame zcl(2);   // message is 4 bytes
    zcl.shortaddr = shortaddr;
    zcl.cluster = cluster;
    zcl.dstendpoint = srcendpoint;
    zcl.cmd = ZCL_DEFAULT_RESPONSE;
    zcl.manuf = manuf;
    zcl.clusterSpecific = false;  /* not cluster specific */
    zcl.needResponse = false;     /* noresponse */
    zcl.direct = true;            /* direct no retry */
    zcl.setTransac(transactseq);
    zcl.payload.add8(cmd);
    zcl.payload.add8(0x00);   // Status = OK
    zigbeeZCLSendCmd(zcl);
  }
}

// ======================================================================
// Convert AnalogValue according to the device type
void ZCLFrame::syntheticAnalogValue(Z_attribute_list &attr_list, class Z_attribute &attr) {
  const char * modelId_c = zigbee_devices.getModelId(shortaddr);  // null if unknown
  String modelId((char*) modelId_c);

  if (modelId.startsWith(F("lumi.sensor_cube"))) {
    attr.setKeyId(0x000C, 0xFF55);    // change to AqaraRotate
  }
  if (modelId.startsWith(F("lumi.ctrl"))) {
    attr.setKeyId(0x0B04, 0x050B);    // change to ActivePower
  }
}


// ======================================================================
// New version of synthetic attribute generation
void ZCLFrame::syntheticAqaraSensor(Z_attribute_list &attr_list, class Z_attribute &attr) {
  const SBuffer * buf = attr.getRaw();
  if (buf) {
    const SBuffer & buf2 = *buf;
    uint32_t i = 0;
    uint32_t len = buf2.len();

    const char * modelId_c = zigbee_devices.getModelId(shortaddr);  // null if unknown
    String modelId((char*) modelId_c);

    while (len >= 2 + i) {
      uint8_t attrid = buf2.get8(i++);

      Z_attribute attr;     // temporary attribute
      i += parseSingleAttribute(attr, buf2, i);
      int32_t ival32 = attr.getInt();
      uint32_t uval32 = attr.getUInt();
      bool translated = false;    // were we able to translate to a known format?
      if (0x01 == attrid) {
        float batteryvoltage = attr.getFloat() / 100;
        attr_list.addAttribute(0x0001, 0x0020).setFloat(batteryvoltage);
        uint8_t batterypercentage = toPercentageCR2032(uval32);
        attr_list.addAttribute(0x0001, 0x0021).setUInt(batterypercentage * 2);
      } else if (0x03 == attrid) {
        attr_list.addAttributePMEM("AqaraTemperature").copyVal(attr);   // Temperature
      } else if ((nullptr != modelId) && ((0 == getManufCode()) || (0x115F == getManufCode()))) {
        translated = true;
        if (modelId.startsWith(F("lumi.sensor_magnet"))) {   // door / window sensor
          if (0x64 == attrid) {
            attr_list.addAttribute(0x0500, 0xFFF0 + ZA_Contact).copyVal(attr);   // Contact
          }
        } else if (modelId.startsWith(F("lumi.sensor_smoke"))) {   // gas leak
          if (0x64 == attrid) {
            attr_list.addAttributePMEM(PSTR("SmokeDensity")).copyVal(attr);
          }
        } else if (modelId.startsWith(F("lumi.sensor_wleak"))) {   // gas leak
          if (0x64 == attrid) {
            attr_list.addAttributePMEM(PSTR("Water")).copyVal(attr);
          }
        } else if (modelId.startsWith(F("lumi.sensor_natgas"))) {   // gas leak
          if (0x64 == attrid) {
            attr_list.addAttributePMEM(PSTR("GasDensity")).copyVal(attr);
          }
        } else if (modelId.startsWith(F("lumi.sensor_ht")) ||
            modelId.equals(F("lumi.sens")) ||
            modelId.startsWith(F("lumi.weather"))) {     // Temp sensor
          // Filter according to prefix of model name
          // onla Aqara Temp/Humidity has manuf_code of zero. If non-zero we skip the parameters
          if (0x64 == attrid) {
            attr_list.addAttribute(0x0402, 0x0000).setInt(ival32);   // Temperature
          } else if (0x65 == attrid) {
            attr_list.addAttribute(0x0405, 0x0000).setUInt(uval32);         // Humidity * 100
          } else if (0x66 == attrid) {
            attr_list.addAttribute(0x0403, 0x0000).setUInt((ival32 + 50) / 100);  // Pressure
          }
        } else if (modelId.startsWith(F("lumi.plug")) || modelId.startsWith(F("lumi.ctrl"))) {
          if (0x64 == attrid) {
            attr_list.addAttribute(0x0006, 0x0000).setInt(uval32);          // Power (on/off)
          } else if (0x98 == attrid) {
            attr_list.addAttribute(0x0B04, 0x050B).setInt(ival32);          // Active Power
          } else if (0x95 == attrid) {
            attr_list.addAttribute(0x0702, 0x0000).setUInt(uval32);         // EnergyDelivered
          }
        } else {
          translated = false;     // we didn't find a match
        }
      }
      if (!translated) {
        if (attrid >= 100) {    // payload is always above 0x64 or 100
          char attr_name[12];
          snprintf_P(attr_name, sizeof(attr_name), PSTR("Xiaomi_%02X"), attrid);
          attr_list.addAttribute(attr_name).copyVal(attr);
        }
      }
    }
  }
}

void ZCLFrame::syntheticAqaraSensor2(class Z_attribute_list &attr_list, class Z_attribute &attr) {
  const SBuffer * buf = attr.getRaw();
  if (buf) {
    const SBuffer & buf2 = *buf;
    uint32_t len = buf2.len();

    // Look for battery value which is the first attribute of type 0x21
    uint16_t struct_size = buf2.get16(0);
    size_t struct_len = 2;
    if (0xFFFF != struct_size) {
      if (struct_size > 16) { struct_size = 16; }
      for (uint32_t j = 0; (j < struct_size) && (struct_len < len); j++) {
        uint8_t attr_type = buf2.get8(struct_len);
        if (0x21 == attr_type) {
          uint16_t val = buf2.get16(struct_len+1);
          float batteryvoltage = (float)val / 100;
          attr_list.addAttribute(0x0001, 0x0020).setFloat(batteryvoltage);
          uint8_t batterypercentage = toPercentageCR2032(val);
          attr_list.addAttribute(0x0001, 0x0021).setUInt(batterypercentage * 2);
          break;
        }
        struct_len += Z_getDatatypeLen(attr_type) + 1;
      }
    }
  }
  attr_list.removeAttribute(&attr);
}

// Aqara Cube and Button
void ZCLFrame::syntheticAqaraCubeOrButton(class Z_attribute_list &attr_list, class Z_attribute &attr) {
  const char * modelId_c = zigbee_devices.findShortAddr(shortaddr).modelId;  // null if unknown
  String modelId((char*) modelId_c);

  if (modelId.startsWith(F("lumi.sensor_cube"))) {   // only for Aqara cube
    int32_t val = attr.getInt();
    static const char *aqara_cube = PSTR("AqaraCube");
    static const char *aqara_cube_side = PSTR("AqaraCubeSide");
    static const char *aqara_cube_from_side = PSTR("AqaraCubeFromSide");

    switch (val) {
      case 0:
        attr_list.addAttributePMEM(aqara_cube).setStr(PSTR("shake"));
        break;
      case 2:
        attr_list.addAttributePMEM(aqara_cube).setStr(PSTR("wakeup"));
        break;
      case 3:
        attr_list.addAttributePMEM(aqara_cube).setStr(PSTR("fall"));
        break;
      case 64 ... 127:
        attr_list.addAttributePMEM(aqara_cube).setStr(PSTR("flip90"));
        attr_list.addAttributePMEM(aqara_cube_side).setInt(val % 8);
        attr_list.addAttributePMEM(aqara_cube_from_side).setInt((val - 64) / 8);
        break;
      case 128 ... 132:
        attr_list.addAttributePMEM(aqara_cube).setStr(PSTR("flip180"));
        attr_list.addAttributePMEM(aqara_cube_side).setInt(val - 128);
        break;
      case 256 ... 261:
        attr_list.addAttributePMEM(aqara_cube).setStr(PSTR("slide"));
        attr_list.addAttributePMEM(aqara_cube_side).setInt(val - 256);
        break;
      case 512 ... 517:
        attr_list.addAttributePMEM(aqara_cube).setStr(PSTR("tap"));
        attr_list.addAttributePMEM(aqara_cube_side).setInt(val - 512);
        break;
    }
    attr_list.removeAttribute(&attr);
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
  } else if (modelId.startsWith(F("lumi.remote")) || modelId.startsWith(F("lumi.sensor_swit"))) {   // only for Aqara buttons WXKG11LM & WXKG12LM, 'swit' because of #9923
    int32_t val = attr.getInt();
    static const char *aqara_click = PSTR("click");    // deprecated
    static const char *aqara_action = PSTR("action");  // deprecated
    Z_attribute & attr_click = attr_list.addAttribute(PSTR("Click"), true);

    switch (val) {
      case 0:
        attr_list.addAttributePMEM(aqara_action).setStr(PSTR("hold"));            // deprecated
        attr_click.setStr(PSTR("hold"));
        break;
      case 1:
        attr_list.addAttributePMEM(aqara_click).setStr(PSTR("single"));            // deprecated
        attr_click.setStr(PSTR("single"));
        break;
      case 2:
        attr_list.addAttributePMEM(aqara_click).setStr(PSTR("double"));            // deprecated
        attr_click.setStr(PSTR("double"));
        break;
      case 3:
        attr_click.setStr(PSTR("triple"));
        break;
      case 4:
        attr_click.setStr(PSTR("quadruple"));
        break;
      case 16:
        attr_list.addAttributePMEM(aqara_action).setStr(PSTR("hold"));            // deprecated
        attr_click.setStr(PSTR("hold"));
        break;
      case 17:
        attr_list.addAttributePMEM(aqara_action).setStr(PSTR("release"));            // deprecated
        attr_click.setStr(PSTR("release"));
        break;
      case 18:
        attr_list.addAttributePMEM(aqara_action).setStr(PSTR("shake"));            // deprecated
        attr_click.setStr(PSTR("shake"));
        break;
      case 255:
        attr_list.addAttributePMEM(aqara_action).setStr(PSTR("release"));            // deprecated
        attr_click.setStr(PSTR("release"));
        break;
      default:
        attr_list.addAttributePMEM(aqara_click).setUInt(val);
        attr_click.setStr(PSTR("release"));
        break;
    }
  }
}

// Aqara vibration device
void ZCLFrame::syntheticAqaraVibration(class Z_attribute_list &attr_list, class Z_attribute &attr) {
  switch (attr.attr_id) {
    case 0x0055:
      {
        int32_t ivalue = attr.getInt();
        const char * svalue;
        switch (ivalue) {
          case 1: svalue = PSTR("vibrate"); break;
          case 2: svalue = PSTR("tilt"); break;
          case 3: svalue = PSTR("drop"); break;
          default: svalue = PSTR("unknown"); break;
        }
        attr.setStr(svalue);
      }
      break;
    case 0x0503:
      break;
    case 0x0505:
      break;
    case 0x0508:
      {
        // see https://github.com/Koenkk/zigbee2mqtt/issues/295 and http://faire-ca-soi-meme.fr/domotique/2018/09/03/test-xiaomi-aqara-vibration-sensor/
        // report accelerometer measures
        const SBuffer * buf = attr.getRaw();
        if (buf) {
          const SBuffer & buf2 = *buf;
          int16_t x, y, z;
          z = buf2.get16(0);
          y = buf2.get16(2);
          x = buf2.get16(4);
          char temp[32];
          snprintf_P(temp, sizeof(temp), PSTR("[%i,%i,%i]"), x, y, z);
          attr.setStrRaw(temp);
          // calculate angles
          float X = x;
          float Y = y;
          float Z = z;
          int32_t Angle_X = 0.5f + atanf(X/sqrtf(z*z+y*y)) * f_180pi;
          int32_t Angle_Y = 0.5f + atanf(Y/sqrtf(x*x+z*z)) * f_180pi;
          int32_t Angle_Z = 0.5f + atanf(Z/sqrtf(x*x+y*y)) * f_180pi;
          snprintf_P(temp, sizeof(temp), PSTR("[%i,%i,%i]"), Angle_X, Angle_Y, Angle_Z);
          attr_list.addAttributePMEM(PSTR("AqaraAngles")).setStrRaw(temp);
        }
      }
      break;
  }
}

/// Publish a message for `"Occupancy":0` when the timer expired
void Z_OccupancyCallback(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  Z_attribute_list attr_list;
  attr_list.addAttribute(0x0406, 0x0000).setUInt(0);        // Occupancy
  Z_postProcessAttributes(shortaddr, endpoint, attr_list);  // make sure all is updated accordingly
  zigbee_devices.jsonPublishNow(shortaddr, attr_list);
}

// ======================================================================
void Z_postProcessAttributes(uint16_t shortaddr, uint16_t src_ep, class Z_attribute_list& attr_list) {
  Z_Device & device = zigbee_devices.getShortAddr(shortaddr);
  uint8_t count_ep = device.countEndpoints();

  for (auto &attr : attr_list) {
    // add endpoint suffix if needed
    if ((Settings->flag4.zb_index_ep) && (src_ep != 1) && (count_ep > 1)) {
      // we need to add suffix if the suffix is not already different from 1
      if (attr.key_suffix == 1) {
        attr.key_suffix = src_ep;
      }
    }

    // attr is Z_attribute&
    if (!attr.key_is_str && !attr.key_is_cmd) {
      uint16_t cluster = attr.cluster;
      uint16_t attribute = attr.attr_id;
      uint32_t ccccaaaa = (attr.cluster << 16) | attr.attr_id;
      // Look for an entry in the converter table
      bool found = false;

      // first search in device plug-ins
      Z_attribute_match matched_attr = Z_findAttributeMatcherById(shortaddr, cluster, attribute, true);
      found = matched_attr.found();
      // special case for Tuya attributes, also search for type `FF` if not found
      if (!found && cluster == 0xEF00) {
        // search for attribute `FFxx` for wildcard types
        matched_attr = Z_findAttributeMatcherById(shortaddr, cluster, 0xFF00 | (attribute & 0x00FF), true);
        found = matched_attr.found();
      }

      float fval = attr.getFloat();
      if (found && (matched_attr.map_type != Z_Data_Type::Z_Unknown)) {
        // We apply an automatic mapping to Z_Data_XXX object
        // First we find or instantiate the correct Z_Data_XXX according to the endpoint
        // Then store the attribute at the attribute addres (via offset) and according to size 8/16/32 bits

        // add the endpoint if it was not already known
        device.addEndpoint(src_ep);
        // we don't apply the multiplier, but instead store in Z_Data_XXX object
        Z_Data & data = device.data.getByType(matched_attr.map_type, src_ep);
        uint8_t * attr_address = ((uint8_t*)&data) + sizeof(Z_Data) + matched_attr.map_offset;
        uint32_t uval32 = attr.getUInt();     // call converter to uint only once
        int32_t  ival32 = attr.getInt();     // call converter to int only once
        // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_ZIGBEE "Mapping type=%d offset=%d zigbee_type=%02X value=%d\n"), (uint8_t) matched_attr.matched_attr, matched_attr.map_offset, matched_attr.zigbee_type, ival32);
        switch (ccccaaaa) {
          case 0xEF000202:
          case 0xEF000203:    // need to convert Tuya temperatures from 1/10 to 1/00 °C
            ival32 = ival32 * 10;
            break;
        }

        switch (matched_attr.zigbee_type) {
          case Zenum8:
          case Zmap8:
          case Zbool:
          case Zuint8:  *(uint8_t*)attr_address  = uval32;          break;
          case Zenum16:
          case Zmap16:
          case Zuint16: *(uint16_t*)attr_address = uval32;          break;
          case Zuint32: *(uint32_t*)attr_address = uval32;          break;
          case Zint8:   *(int8_t*)attr_address   = ival32;           break;
          case Zint16:  *(int16_t*)attr_address  = ival32;           break;
          case Zint32:  *(int32_t*)attr_address  = ival32;           break;
        }
        if (Z_Data_Set::updateData(data)) {
          zigbee_devices.dirty();
        }
      }

      uint16_t uval16 = attr.getUInt();     // call converter to uint only once
      int16_t  ival16 = attr.getInt();     // call converter to int only once
      // update any internal structure
      switch (ccccaaaa) {
        case 0x00000004: device.setManufId(attr.getStr());                            break;
        case 0x00000005: device.setModelId(attr.getStr());                            break;
        case 0x00010021:
          {
            // if both BatteryVoltage and BatteryPercentage are zero, ignore
            // Behavior seen on Aqara device when requesting cluster 0x0001
            bool valid_batt_voltage = true;
            const Z_attribute * battVoltage_attr = attr_list.findAttribute(0x0001, 0x0020);
            if (battVoltage_attr != nullptr) {
              if (battVoltage_attr->getUInt() == 0) {
                valid_batt_voltage = false;
              }
            }
            if (valid_batt_voltage || uval16 > 0) {
              device.setBatteryPercent(uval16 / 2);     // set value only if both are not zero
            }
          }
          break;
        case 0x00060000:
        case 0x00068000: device.setPower(attr.getBool(), src_ep);                     break;
        // apply multiplier/divisor to AC values
        case 0x0B040505:    // RMSVoltage
        case 0x0B040508:    // RMSCurrent
        case 0x0B04050B:    // ActivePower
          {
            const Z_Data_Plug & plug = device.data.find<Z_Data_Plug>();
            if (&plug != &z_data_unk) {
              switch (ccccaaaa) {
                case 0x0B040505:  fval = fval * plug.getACVoltageMul() / plug.getACVoltageDiv();    break;
                case 0x0B040508:  fval = fval * plug.getACCurrentMul() / plug.getACCurrentDiv();    break;
                case 0x0B04050B:  fval = fval * plug.getACPowerMul() / plug.getACPowerDiv();    break;
              }
              attr.setFloat(fval);
            }
          }
          break;
      }

      // now apply the multiplier to make it human readable
      if (found) {
        if (0 == matched_attr.multiplier)  { attr_list.removeAttribute(&attr); continue; }      // remove attribute if multiplier is zero
        if ((matched_attr.multiplier != 1 && matched_attr.multiplier != 0) || (matched_attr.divider != 1 && matched_attr.divider != 0) || (matched_attr.base != 0)) {
          if (matched_attr.multiplier != 1 && matched_attr.multiplier != 0) {
            fval = fval * matched_attr.multiplier;
          }
          if (matched_attr.divider != 1 && matched_attr.divider != 0) {
            fval = fval / matched_attr.divider;
          }
          if (matched_attr.base != 0) {
            fval = fval + matched_attr.base;
          }
          attr.setFloat(fval);
        }
      }

      // Replace cluster/attribute with name
      if (found) {
        if (0x00 != pgm_read_byte(matched_attr.name)) {// if name is not null, replace it
          attr.setKeyName(matched_attr.name, true);    // PMEM string so no need to copy
        }
      }
    }
  }
}

// Internal search function
void Z_parseAttributeKey_inner(uint16_t shortaddr, class Z_attribute & attr, uint16_t preferred_cluster) {
  if (attr.key_is_str) {
    // find the attribute name
    Z_attribute_match matched_attr = Z_findAttributeMatcherByName(shortaddr, attr.key);
    if (matched_attr.found()) {
      if ((preferred_cluster == 0xFFFF) ||    // any cluster
          (matched_attr.cluster == preferred_cluster)) {
        // match
        attr.setKeyId(matched_attr.cluster, matched_attr.attribute);
        attr.attr_type = matched_attr.zigbee_type;
        attr.attr_multiplier = matched_attr.multiplier;
        attr.attr_divider = matched_attr.divider;
        attr.attr_base = matched_attr.base;
        attr.manuf = matched_attr.manuf;
      }
    }
  } else {
    // find by cluster/attribute
    Z_attribute_match matched_attr = Z_findAttributeMatcherById(shortaddr, attr.cluster, attr.attr_id, false);
    if (matched_attr.found()) {
      attr.attr_type = matched_attr.zigbee_type;
      attr.manuf = matched_attr.manuf;
    }
  }
}

//
// Given an attribute string, retrieve all attribute details.
// Input: the attribute has a key name, either: <cluster>/<attr> or <cluster>/<attr>%<type> or "<attribute_name>"
// Ex: "0008/0000", "0008/0000%20", "Dimmer"
// Use:
//    Z_attribute attr;
//    attr.setKeyName("0008/0000%20")
//    if (Z_parseAttributeKey(attr)) {
//      // ok
//    }
//
// Output:
//   The `attr` attribute has the correct cluster, attr_id, attr_type, attr_multiplier
//   Note: the attribute value is unchanged and unparsed
//
// Note: if the type is specified in the key, the multiplier is not applied, no conversion happens
bool Z_parseAttributeKey(uint16_t shortaddr, class Z_attribute & attr, uint16_t preferred_cluster) {
  // check if the name has the format "XXXX/YYYY" where XXXX is the cluster, YYYY the attribute id
  // alternative "XXXX/YYYY%ZZ" where ZZ is the type (for unregistered attributes)
  if (attr.key_is_str) {
    const char * key = attr.key;
    char * delimiter = strchr(key, '/');
    char * delimiter2 = strchr(key, '%');
    if (delimiter) {
      uint16_t attr_id = 0xFFFF;
      uint16_t cluster_id = 0xFFFF;
      uint8_t  type_id = Zunk;

      cluster_id = strtoul(key, &delimiter, 16);
      if (!delimiter2) {
        attr_id = strtoul(delimiter+1, nullptr, 16);
      } else {
        attr_id = strtoul(delimiter+1, &delimiter2, 16);
        type_id = Z_getTypeByName(delimiter2+1);
      }
      attr.setKeyId(cluster_id, attr_id);
      attr.attr_type = type_id;
    }
  }
  // AddLog(LOG_LEVEL_DEBUG, PSTR("cluster_id = 0x%04X, attr_id = 0x%04X"), cluster_id, attr_id);

  // do we already know the type, i.e. attribute and cluster are also known
  if ((Zunk == attr.attr_type) && (preferred_cluster != 0xFFFF)) {
    Z_parseAttributeKey_inner(shortaddr, attr, preferred_cluster);   // try to find with the selected cluster
  }
  if (Zunk == attr.attr_type) {
    Z_parseAttributeKey_inner(shortaddr, attr, 0xFFFF);    // try again with any cluster
  }
  // special case for Tuya attributes, where Zunk is allowed
  if (Zunk == attr.attr_type) {
    if (!attr.key_is_str && attr.cluster == 0xEF00) {
      return true;
    }
    return false;   // couldn't find any match
  }
  return true;
}

// generic toAttributes() based on declaration in the attribute array
// can be overloaded for specific objects
// Input:
//  the Json object to add attributes to
//  the type of object (necessary since the type system is unaware of the actual sub-type)
void Z_Data::toAttributes(Z_attribute_list & attr_list) const {
  Z_Data_Type type = getType();
  // iterate through attributes to see which ones need to be exported
  for (uint32_t i = 0; i < nitems(Z_PostProcess); i++) {
    const Z_AttributeConverter *converter = &Z_PostProcess[i];
    uint8_t conv_export = pgm_read_byte(&converter->multiplier_idx) & Z_EXPORT_DATA;
    uint8_t conv_mapping = pgm_read_byte(&converter->mapping);
    uint16_t multiplier = 1;
    uint16_t divider = 1;
    int8_t multiplier8 = CmToMultiplier(pgm_read_byte(&converter->multiplier_idx));
    if (multiplier8 > 1) { multiplier = multiplier8; }
    if (multiplier8 < 0) { divider = -multiplier8; }
    Z_Data_Type map_type = (Z_Data_Type) ((conv_mapping & 0xF0)>>4);
    uint8_t map_offset = (conv_mapping & 0x0F);

    if ((conv_export != 0) && (map_type == type)) {
      // we need to export this attribute
      const char * conv_name = Z_strings + pgm_read_word(&converter->name_offset);
      uint8_t zigbee_type = pgm_read_byte(&converter->type);                    // zigbee type to select right size 8/16/32 bits
      uint8_t * attr_address = ((uint8_t*)this) + sizeof(Z_Data) + map_offset;   // address of attribute in memory

      int32_t data_size = 0;
      int32_t ival32;
      uint32_t uval32;
      switch (zigbee_type) {
        case Zenum8:
        case Zmap8:
        case Zbool:
        case Zuint8:  uval32 = *(uint8_t*)attr_address;   if (uval32 !=  0x000000FF) data_size = 8;   break;
        case Zmap16:
        case Zenum16:
        case Zuint16: uval32 = *(uint16_t*)attr_address;  if (uval32 !=  0x0000FFFF) data_size = 16;  break;
        case Zuint32: uval32 = *(uint32_t*)attr_address;  if (uval32 !=  0xFFFFFFFF) data_size = 32;  break;
        case Zint8:   ival32 = *(int8_t*)attr_address;    if (ival32 != -0x80)       data_size = -8;  break;
        case Zint16:  ival32 = *(int16_t*)attr_address;   if (ival32 != -0x8000)     data_size = -16; break;
        case Zint32:  ival32 = *(int32_t*)attr_address;   if (ival32 != -0x80000000) data_size = -32; break;
      }
      if (data_size != 0) {
        Z_attribute & attr = attr_list.addAttribute(conv_name);

        float fval;
        if (data_size > 0) { fval = uval32; }
        else               { fval = ival32; }
        if ((multiplier != 1 && multiplier != 0) || (divider != 1 && divider != 0)) {
          if (multiplier != 1 && multiplier != 0) {
            fval = fval * multiplier;
          }
          if (divider != 1 && divider != 0) {
            fval = fval / divider;
          }
        }
        // special case for plugs, with parametric multiplier/divisor
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"   // avoid warnings since we're using offsetof() in a risky way
        const Z_Data_Plug * plug = (Z_Data_Plug*) this;
        if (map_type == Z_Data_Type::Z_Plug) {
          if (map_offset == Z_OFFSET(Z_Data_Plug, mains_voltage)) {
            fval = fval * plug->getACVoltageMul() / plug->getACVoltageDiv();
          } else if (map_offset == Z_OFFSET(Z_Data_Plug, mains_power)) {
            fval = fval * plug->getACPowerMul() / plug->getACPowerDiv();
          }
        }
        attr.setFloat(fval);
#pragma GCC diagnostic pop
      }
    }
  }
}

// Add both attributes RGB and RGBb based on the inputs
// r,g,b are expected to be 100% brightness
// brightness is expected 0..255
void Z_Data_Light::toRGBAttributesRGBb(Z_attribute_list & attr_list, uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
  SBuffer rgb(3);
  rgb.add8(r);
  rgb.add8(g);
  rgb.add8(b);
  attr_list.addAttribute(PSTR("RGB"), true).setBuf(rgb, 0, 3);
  // now blend with brightness
  r = changeUIntScale(r, 0, 255, 0, brightness);
  g = changeUIntScale(g, 0, 255, 0, brightness);
  b = changeUIntScale(b, 0, 255, 0, brightness);
  rgb.set8(0, r);
  rgb.set8(1, g);
  rgb.set8(2, b);
  attr_list.addAttribute(PSTR("RGBb"), true).setBuf(rgb, 0, 3);
}

// Convert from Hue/Sat + Brightness to RGB+RGBb
// sat: 0..255
// hue: 0..359
// brightness: 0..255
void Z_Data_Light::toRGBAttributesHSB(Z_attribute_list & attr_list, uint16_t hue, uint8_t sat, uint8_t brightness) {
  uint8_t r,g,b;
  HsToRgb(hue, sat, &r, &g, &b);
  Z_Data_Light::toRGBAttributesRGBb(attr_list, r, g, b, brightness);
}

// Convert X/Y to RGB and RGBb
// X: 0..65535
// Y: 0..65535
// brightness: 0..255
void Z_Data_Light::toRGBAttributesXYB(Z_attribute_list & attr_list, uint16_t x, uint16_t y, uint8_t brightness) {
  uint8_t r,g,b;
  XyToRgb(x / 65535.0f, y / 65535.0f, &r, &g, &b);
  Z_Data_Light::toRGBAttributesRGBb(attr_list, r, g, b, brightness);
}

void Z_Data_Light::toRGBAttributes(Z_attribute_list & attr_list) const {
  uint8_t brightness = 255;
  if (validDimmer()) {
    brightness = changeUIntScale(getDimmer(), 0, 254, 0, 255);   // range is 0..255
  }
  if (validHue() && validSat()) {
    uint8_t sat = changeUIntScale(getSat(), 0, 254, 0, 255);
    Z_Data_Light::toRGBAttributesHSB(attr_list, getHue(), sat, brightness);
  } else if (validX() && validY()) {
    Z_Data_Light::toRGBAttributesXYB(attr_list, getX(), getY(), brightness);
  }
}

//
// Check if this device needs Battery reporting
// This is useful for IKEA or Philips devices that tend to drain battery quickly when Battery reporting is set
//
bool Z_BatteryReportingDeviceSpecific(uint16_t shortaddr) {
  const Z_Device & device = zigbee_devices.findShortAddr(shortaddr);
  if (device.manufacturerId) {
    String manuf_c(device.manufacturerId);
    if ((manuf_c.startsWith(F("IKEA"))) || (manuf_c.startsWith(F("Philips")))) {
      return false;
    }
  }
  return true;
}

#endif // USE_ZIGBEE
