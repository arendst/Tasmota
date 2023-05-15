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
  Zarrray   = 0x48,
  Zstruct   = 0x4C,
  Zset      = 0x50, Zbag = 0x51,
  ZToD      = 0xE0, Zdate = 0xE1, ZUTC = 0xE2,
  ZclusterId = 0xE8, ZattribId = 0xE9, ZbacOID = 0xEA,
  ZEUI64    = 0xF0, Zkey128 = 0xF1,
  Zunk      = 0xFF,
  // adding fake type for Tuya specific encodings
  Ztuya0    = Zoctstr,
  Ztuya1    = Zbool,
  Ztuya2    = Zint32,
  Ztuya3    = Zstring,
  Ztuya4    = Zuint8,
  Ztuya5    = Zuint32
};

const char Z_DATATYPES[] PROGMEM = 
  "nodata|"
  "data8|data16|data24|data32|data40|data48|data56|data64|"
  "bool|"
  "map8|map16|map24|map32|map40|map48|map56|map64|"
  "uint8|uint16|uint24|uint32|uint40|uint48|uint56|uint64|"
  "int8|int16|int24|int32|int40|int48|int56|int64|"
  "enum8|enum16|"
  "semi|single|double|"
  "octstr|string|octstr16|string16|"
  "array|struct|set|bag|"
  "ToD|date|UTC|"
  "clusterId|attribId|bacOID|"
  "EUI64|key128|"
  "unk"
;

const uint8_t Z_DATA_TYPES_CODE[] PROGMEM = {
  Znodata,
  Zdata8, Zdata16, Zdata24, Zdata32, Zdata40, Zdata48, Zdata56, Zdata64,
  Zbool,
  Zmap8, Zmap16, Zmap24, Zmap32, Zmap40, Zmap48, Zmap56, Zmap64,
  Zuint8, Zuint16, Zuint24, Zuint32, Zuint40, Zuint48, Zuint56, Zuint64,
  Zint8, Zint16, Zint24, Zint32, Zint40, Zint48, Zint56, Zint64,
  Zenum8, Zenum16,
  Zsemi, Zsingle, Zdouble,
  Zoctstr, Zstring, Zoctstr16, Zstring16,
  Zarrray, Zstruct, Zset, Zbag,
  ZToD, Zdate, ZUTC,
  ZclusterId, ZattribId, ZbacOID,
  ZEUI64, Zkey128,
  Zunk,
};

// convert a type into a name, or HEX if none found
void Z_getTypeByNumber(char *destination, size_t destination_size, uint8_t type) {
  for (uint32_t i = 0; i < ARRAY_SIZE(Z_DATA_TYPES_CODE); i++) {
    if (type == pgm_read_byte(&Z_DATA_TYPES_CODE[i])) {
      GetTextIndexed(destination, destination_size, i, Z_DATATYPES);
      return;
    }
  }
  snprintf(destination, destination_size, "%02X", type);
}

// convert a string to a type, or Zunk if no match
uint8_t Z_getTypeByName(const char *type) {
  if (type == nullptr) { return Zunk; }
  char type_found[16];
  int32_t ret = GetCommandCode(type_found, sizeof(type_found), type, Z_DATATYPES);
  if (ret < 0) {
    // try to decode hex
    size_t type_len = strlen_P(type);
    if (type_len > 0 && type_len <= 2) {
      char *type_end;
      ret = strtoul(type, &type_end, 16);
      if (type_end == type) { ret = Zunk; }   // could not decode
    }
    return ret;
  } else {
    return pgm_read_byte(&Z_DATA_TYPES_CODE[ret]);
  }
}
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

// is the type a discrete type, cf. section 2.6.2 of ZCL spec
bool Z_isDiscreteDataType(uint8_t t) {
  if ( ((t >= 0x20) && (t <= 0x2F)) ||      // uint8 - int64
       ((t >= 0x38) && (t <= 0x3A)) ||      // semi - double
       ((t >= 0xE0) && (t <= 0xE2))  ) {    // ToD - UTC
    return false;
  } else {
    return true;
  }
}

typedef struct Z_AttributeConverter {
  uint8_t     type;
  uint8_t     cluster_short;
  uint16_t    attribute;
  uint16_t    name_offset;
  uint8_t     multiplier_idx;     // multiplier index for numerical value, use CmToMultiplier(), (if > 0 multiply by x, if <0 device by x)
                                  // the high 4 bits are used to encode flags
                                  // currently: 0x80 = this parameter needs to be exported to ZbData
  uint8_t     mapping;        // high 4 bits = type, low 4 bits = offset in bytes from header
  // still room for a byte
} Z_AttributeConverter;

// Get offset in bytes of attributes, starting after the header (skipping first 4 bytes)
#define Z_OFFSET(c,a) (offsetof(class c, a) - sizeof(Z_Data))
#define Z_CLASS(c) c      // necessary to get a valid token without concatenation (which wouldn't work)
#define Z_MAPPING(c,a) (((((uint8_t)Z_CLASS(c)::type) & 0x0F) << 4) | Z_OFFSET(c,a))

// lines with this marker, will be used to export automatically data to `ZbData`
// at the condition Z_MAPPING() is also used
const uint8_t Z_EXPORT_DATA = 0x80;

// Cluster numbers are store in 8 bits format to save space,
// the following tables allows the conversion from 8 bits index Cx...
// to the 16 bits actual cluster number
enum Cx_cluster_short {
  Cx0000, Cx0001, Cx0002, Cx0003, Cx0004, Cx0005, Cx0006, Cx0007,
  Cx0008, Cx0009, Cx000A, Cx000B, Cx000C, Cx000D, Cx000E, Cx000F,
  Cx0010, Cx0011, Cx0012, Cx0013, Cx0014, Cx001A, Cx0020, Cx0021,
  Cx0100, Cx0101, Cx0102, Cx0201, Cx0202, Cx0203, Cx0204,
  Cx0300, Cx0301, Cx0400, Cx0401, Cx0402, Cx0403,
  Cx0404, Cx0405, Cx0406, Cx0500, Cx0702, Cx0B01, Cx0B04, Cx0B05,
  CxEF00, CxFC01, CxFC40, CxFCC0, CxFCCC,
};

const uint16_t Cx_cluster[] PROGMEM = {
  0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
  0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
  0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x001A, 0x0020, 0x0021,
  0x0100, 0x0101, 0x0102, 0x0201, 0x0202, 0x0203, 0x0204,
  0x0300, 0x0301, 0x0400, 0x0401, 0x0402, 0x0403,
  0x0404, 0x0405, 0x0406, 0x0500, 0x0702, 0x0B01, 0x0B04, 0x0B05,
  0xEF00, 0xFC01, 0xFC40, 0xFCC0, 0xFCCC,
};

uint16_t CxToCluster(uint8_t cx) {
  if (cx < nitems(Cx_cluster)) {
    return pgm_read_word(&Cx_cluster[cx]);
  }
  return 0xFFFF;
}

uint8_t ClusterToCx(uint16_t cluster) {
  for (uint32_t i=0; i<nitems(Cx_cluster); i++) {
    if (pgm_read_word(&Cx_cluster[i]) == cluster) {
      return i;
    }
  }
  return 0xFF;
}

// Multiplier contains only a limited set of values, so instead of storing the value
// we store an index in a table, and reduce it to 4 bits
enum Cm_multiplier_nibble {
  Cm0 = 0, Cm1 = 1, Cm2, Cm5, Cm10, Cm100,
  // negative numbers
  Cm_2, Cm_5, Cm_10, Cm_100
};

const int8_t Cm_multiplier[] PROGMEM = {
  0, 1, 2, 5, 10, 100,
  -2, -5, -10, -100,
};

int8_t CmToMultiplier(uint8_t cm) {
  cm = cm & 0x0F;     // get only low nibble
  if (cm < nitems(Cm_multiplier)) {
    return pgm_read_byte(&Cm_multiplier[cm]);
  }
  return 1;
}


//
// Plug-in mechanism for device specific attributes and commands
//

/* Example:

# GiEX garden watering https://www.aliexpress.com/item/1005004222098040.html
:TS0601,_TZE200_sh1btabb
EF00_0101,water_mode
EF00_0102,water_state
EF00_0365,irrigation_start_time
EF00_0366,irrigation_target
EF00_0267,cycle_irrigation_num_times
EF00_0268,irrigation_target
EF00_0269,cycle_irrigation_interval
EF00_026A,current_temperature
EF00_026C,battery
EF00_026F,water_consumed
EF00_0372,last_irrigation_duration

*/

void Z_setString(char*& attr, const char * str) {
  if (nullptr == str)  { str = PSTR(""); }    // nullptr is considered empty string
  size_t str_len = strlen(str);

  if ((nullptr == attr) && (0 == str_len)) { return; } // if both empty, don't do anything
  if (attr) {
    // we already have a value
    if (strcmp(attr, str) != 0) {
      // new value
      free(attr);      // free previous value
      attr = nullptr;
    } else {
      return;        // same value, don't change anything
    }
  }
  if (str_len) {
    if (str_len > 31) { str_len = 31; }
    attr = (char*) malloc(str_len + 1);
    strlcpy(attr, str, str_len + 1);
  }
}

//
//
// Class for a single attribute from a plugin
//
//
class Z_plugin_attribute {
public:
  
  Z_plugin_attribute(void) :
    type(Zunk),
    multiplier(1), divider(1), base(0),
    cluster(0xFFFF), attribute(0xFFFF), manuf(0),
    name(nullptr)
    {};
  
  ~Z_plugin_attribute(void) {
    if (name != nullptr) { free((void*)name); }
  }

  inline void setName(const char *_name) {
    Z_setString(this->name, _name);
  }

  void set(uint16_t cluster, uint16_t attribute, const char *_name, uint8_t type = Zunk) {
    this->cluster = cluster;
    this->attribute = attribute;
    Z_setString(this->name, _name);
    this->name = name;
    this->type = type;
  }

  uint8_t       type;             // zigbee type, Zunk by default
  uint32_t      multiplier;       // multiply by x (ignore if 0 or 1)
  uint32_t      divider;          // divide by x (ignore if 0 or 1)
  int32_t       base;             // add x (ignore if 0)
  uint16_t      cluster;          // cluster number
  uint16_t      attribute;        // attribute number
  uint16_t      manuf;            // manufacturer code, 0 if none
  char *        name;             // name of attribute once converted
};

//
// Class for an attribute synonym
//
class Z_attribute_synonym {
public:
  Z_attribute_synonym(void) :
    cluster(0xFFFF), attribute(0xFFFF), new_cluster(0xFFFF), new_attribute(0xFFFF),
    multiplier(1), divider(1), base(0)
    {};
  
  void set(uint16_t cluster, uint16_t attribute, uint16_t new_cluster, uint16_t new_attribute,
          uint32_t multiplier = 1, uint32_t divider = 1, int32_t base = 0) {
    this->cluster = cluster;
    this->attribute = attribute;
    this->new_cluster = new_cluster;
    this->new_attribute = new_attribute;
    this->multiplier = multiplier;
    this->divider = divider;
    this->base = base;
  }

  inline bool found(void) const { return cluster != 0xFFFF && attribute != 0xFFFF; }

  uint16_t      cluster;          // cluster to match
  uint16_t      attribute;        // attribute to match
  uint16_t      new_cluster;      // replace with this cluster
  uint16_t      new_attribute;    // replace with this attribute
  uint32_t      multiplier;       // multiply by x (ignore if 0 or 1)
  uint32_t      divider;          // divide by x (ignore if 0 or 1)
  int32_t       base;           // add x (ignore if 0)
};

//
//
// matcher for a device, based on ModelID and Manufacturer
//
//
class Z_plugin_matcher {
public:

  Z_plugin_matcher(void) {};

  inline void setModel(const char *_model) {
    Z_setString(this->model, _model);
  }


  inline void setManuf(const char *_manuf) {
    Z_setString(this->manufacturer, _manuf);
  }

  ~Z_plugin_matcher(void) {
    if (model) { free((void*)model); }
    if (manufacturer) { free((void*)manufacturer); }
  }

  // check if a matches b, return true if so
  //
  // Special behavior:
  // - return true if `a` is empty or null
  // - return false if `b` is null
  // - matches start of `a` if `a` ends with `'*'`
  // - exact match otherwise
  static bool matchStar(const char *_a, const char *_b) {
    if (_a == nullptr || *_a == '\0') { return true; }
    if (_b == nullptr) { return false; }

    const char *a = _a;
    const char *b = _b;
    while (1) {
      if (a[0] == '*' && a[1] == '\0') {  // pattern ends with '*'
        return true;    // matches worked until now, accept match
      }
      if (*a != *b) {
        return false;
      }
      if (*a == '\0') {
        return true;
      }
      a++;
      b++;
    }
  }

  bool match(const char *match_model, const char *match_manuf) const {
    bool match = true;
    if (!matchStar(model, match_model)) {
      match = false;
    }
    if (!matchStar(manufacturer, match_manuf)) {
        match = false;
    }
    // AddLog(LOG_LEVEL_DEBUG, ">match device(%s, %s) compared to (%s, %s) = %i", match_model, match_manuf, model ? model : "", manufacturer ? manufacturer : "", match);
    return match;
  }

  char *        model = nullptr;
  char *        manufacturer = nullptr;
};

//
//
// Z_plugin_template : template for a group of devices
//
//
class Z_plugin_template {
public:

  LList<Z_plugin_matcher>   matchers;
  LList<Z_attribute_synonym> synonyms;
  LList<Z_plugin_attribute> attributes;
  char * filename = nullptr;              // the filename from which it was imported
                                          // needs to be freed by ZbUnload only (to allow reuse between mutliple instances)
};

//
//
// Z_plugin_templates
//
//
class Z_plugin_templates : public LList<Z_plugin_template> {
public:
  // match an attribute from the plug-in in-memory database
  // returns `nullptr` if none found
  // or a pointer to `Z_plugin_attribute`
  const Z_plugin_attribute * matchAttributeById(const char *model, const char *manufacturer, uint16_t cluster, uint16_t attribute);
  const Z_plugin_attribute * matchAttributeByName(const char *model, const char *manufacturer, const char *name);
  const Z_attribute_synonym * matchAttributeSynonym(const char *model, const char *manufacturer, uint16_t cluster, uint16_t attribute);
};

const Z_attribute_synonym * Z_plugin_templates::matchAttributeSynonym(const char *model, const char *manufacturer, uint16_t cluster, uint16_t attribute) {
  // scan through all templates
  for (const Z_plugin_template & tmpl : *this) {
    const LList<Z_plugin_matcher> & matchers = tmpl.matchers;       // get synonyms
    const LList<Z_attribute_synonym> & synonyms = tmpl.synonyms; // get synonyms

    for (const Z_plugin_matcher & mtch : matchers) {
      if (mtch.match(model, manufacturer)) {
        // got a match, apply template
        for (const Z_attribute_synonym & syn : synonyms) {
          if (syn.cluster == cluster && syn.attribute == attribute) {
            return &syn;
          }
        }
      }
    }
  }
  // no match
  return nullptr;
}

const Z_plugin_attribute * Z_plugin_templates::matchAttributeById(const char *model, const char *manufacturer, uint16_t cluster, uint16_t attribute) {
  // scan through all templates
  for (const Z_plugin_template & tmpl : *this) {
    const LList<Z_plugin_matcher> & matchers = tmpl.matchers;       // get synonyms
    const LList<Z_plugin_attribute> & attributes = tmpl.attributes; // get synonyms

    for (const Z_plugin_matcher & mtch : matchers) {
      if (mtch.match(model, manufacturer)) {
        // got a match, apply template
        for (const Z_plugin_attribute & attr : attributes) {
          if (attr.cluster == cluster && attr.attribute == attribute) {
            return &attr;
          }
        }
      }
    }
  }
  // no match
  return nullptr;
}

const Z_plugin_attribute * Z_plugin_templates::matchAttributeByName(const char *model, const char *manufacturer, const char * name) {
  // scan through all templates
  for (const Z_plugin_template & tmpl : *this) {
    const LList<Z_plugin_matcher> & matchers = tmpl.matchers;       // get synonyms
    const LList<Z_plugin_attribute> & attributes = tmpl.attributes; // get synonyms

    for (const Z_plugin_matcher & mtch : matchers) {
      if (mtch.match(model, manufacturer)) {
        // got a match, apply template
        for (const Z_plugin_attribute & attr : attributes) {
          if (0 == strcasecmp_P(name, attr.name ? attr.name : "")) {
            return &attr;
          }
        }
      }
    }
  }
  // no match
  return nullptr;
}

//
// Attribute match result
//
class Z_attribute_match {
public:
  inline bool found(void) const { return (cluster != 0xFFFF && attribute != 0xFFFF); }

  uint16_t cluster = 0xFFFF;
  uint16_t attribute = 0xFFFF;
  const char * name = nullptr;
  uint32_t multiplier = 1;
  uint32_t divider = 1;
  int32_t  base = 0;
  uint8_t zigbee_type = Znodata;
  uint8_t map_offset = 0;
  Z_Data_Type map_type = Z_Data_Type::Z_Unknown;
  uint16_t manuf = 0x0000;      // manuf code (if any)
};

Z_attribute_match Z_plugin_matchAttributeById(const char *model, const char *manufacturer, uint16_t cluster, uint16_t attribute);
Z_attribute_match Z_plugin_matchAttributeByName(const char *model, const char *manufacturer, const char *name);


// list of post-processing directives
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"   // avoid warnings since we're using offsetof() in a risky way
const Z_AttributeConverter Z_PostProcess[] PROGMEM = {
  { Zuint8,   Cx0000, 0x0000,  Z_(ZCLVersion),           Cm1, 0 },
  { Zuint8,   Cx0000, 0x0001,  Z_(AppVersion),           Cm1, 0 },
  { Zuint8,   Cx0000, 0x0002,  Z_(StackVersion),         Cm1, 0 },
  { Zuint8,   Cx0000, 0x0003,  Z_(HWVersion),            Cm1, 0 },
  { Zstring,  Cx0000, 0x0004,  Z_(Manufacturer),         Cm1, 0 },    // record Manufacturer
  { Zstring,  Cx0000, 0x0005,  Z_(ModelId),              Cm1, 0 },    // record Model
  { Zstring,  Cx0000, 0x0006,  Z_(DateCode),             Cm1, 0 },
  { Zenum8,   Cx0000, 0x0007,  Z_(PowerSource),          Cm1, 0 },
  { Zenum8,   Cx0000, 0x0008,  Z_(GenericDeviceClass),   Cm1, 0 },
  { Zenum8,   Cx0000, 0x0009,  Z_(GenericDeviceType),    Cm1, 0 },
  { Zoctstr,  Cx0000, 0x000A,  Z_(ProductCode),          Cm1, 0 },
  { Zstring,  Cx0000, 0x000B,  Z_(ProductURL),           Cm1, 0 },
  { Zstring,  Cx0000, 0x0010,  Z_(LocationDescription),  Cm1, 0 },
  { Zenum8,   Cx0000, 0x0011,  Z_(PhysicalEnvironment),  Cm1, 0 },
  { Zbool,    Cx0000, 0x0012,  Z_(DeviceEnabled),        Cm1, 0 },
  { Zmap8,    Cx0000, 0x0013,  Z_(AlarmMask),            Cm1, 0 },
  { Zmap8,    Cx0000, 0x0014,  Z_(DisableLocalConfig),   Cm1, 0 },
  { Zstring,  Cx0000, 0x4000,  Z_(SWBuildID),            Cm1, 0 },
  { Zuint8,   Cx0000, 0x4005,  Z_(MullerLightMode),      Cm1, 0 },
  // Cmd 0x0A - Cluster 0x0000, attribute 0xFF01 - proprietary
  { Zmap8,    Cx0000, 0xFF01,  Z_(),                     Cm0, 0 },
  { Zmap8,    Cx0000, 0xFF02,  Z_(),                     Cm0, 0 },
  // { Zmap8,    Cx0000, 0xFF01,  Z_(),                     Cm0,  Z_AqaraSensor, 0 },
  // { Zmap8,    Cx0000, 0xFF02,  Z_(),                     Cm0,  Z_AqaraSensor2, 0 },

  // Power Configuration cluster
  { Zuint16,  Cx0001, 0x0000,  Z_(MainsVoltage),         Cm1, 0 },
  { Zuint8,   Cx0001, 0x0001,  Z_(MainsFrequency),       Cm1, 0 },
  { Zmap8,    Cx0001, 0x0010,  Z_(MainsAlarmMask),          Cm1, 0 },
  { Zuint16,  Cx0001, 0x0011,  Z_(MainsVoltageMinThreshold),Cm1, 0 },
  { Zuint16,  Cx0001, 0x0012,  Z_(MainsVoltageMaxThreshold),Cm1, 0 },
  { Zuint16,  Cx0001, 0x0013,  Z_(MainsVoltageDwellTripPoint),Cm1, 0 },
  { Zuint8,   Cx0001, 0x0020,  Z_(BatteryVoltage),       Cm_10, 0 },   // divide by 10
  { Zuint8,   Cx0001, 0x0021,  Z_(BatteryPercentage),    Cm_2, 0 },   // divide by 2
  { Zstring,  Cx0001, 0x0030,  Z_(BatteryManufacturer),   Cm1, 0 },
  { Zenum8,   Cx0001, 0x0031,  Z_(BatterySize),           Cm1, 0 },
  { Zuint16,  Cx0001, 0x0032,  Z_(BatteryAHrRating),      Cm1, 0 },
  { Zuint8,   Cx0001, 0x0033,  Z_(BatteryQuantity),           Cm1, 0 },
  { Zuint8,   Cx0001, 0x0034,  Z_(BatteryRatedVoltage),           Cm1, 0 },
  { Zmap8,    Cx0001, 0x0035,  Z_(BatteryAlarmMask),           Cm1, 0 },
  { Zuint8,   Cx0001, 0x0036,  Z_(BatteryVoltageMinThreshold),           Cm1, 0 },
  { Zuint8,   Cx0001, 0x0037,  Z_(BatteryVoltageThreshold1),           Cm1, 0 },
  { Zuint8,   Cx0001, 0x0038,  Z_(BatteryVoltageThreshold2),           Cm1, 0 },
  { Zuint8,   Cx0001, 0x0039,  Z_(BatteryVoltageThreshold3),           Cm1, 0 },
  { Zuint8,   Cx0001, 0x003A,  Z_(BatteryPercentageMinThreshold),           Cm1, 0 },
  { Zuint8,   Cx0001, 0x003B,  Z_(BatteryPercentageThreshold1),           Cm1, 0 },
  { Zuint8,   Cx0001, 0x003C,  Z_(BatteryPercentageThreshold2),           Cm1, 0 },
  { Zuint8,   Cx0001, 0x003D,  Z_(BatteryPercentageThreshold3),           Cm1, 0 },
  { Zmap32,   Cx0001, 0x003E,  Z_(BatteryAlarmState),           Cm1, 0 },
   // { Zuint8,   Cx0001, 0x0021,  Z_(BatteryPercentage),    Cm_2, Z_BatteryPercentage, 0 },   // divide by 2

  // Device Temperature Configuration cluster
  { Zint16,   Cx0002, 0x0000,  Z_(CurrentTemperature),   Cm1, 0 },
  { Zint16,   Cx0002, 0x0001,  Z_(MinTempExperienced),   Cm1, 0 },
  { Zint16,   Cx0002, 0x0002,  Z_(MaxTempExperienced),   Cm1, 0 },
  { Zuint16,  Cx0002, 0x0003,  Z_(OverTempTotalDwell),   Cm1, 0 },
  { Zmap8,    Cx0002, 0x0010,  Z_(DeviceTempAlarmMask),           Cm1, 0 },
  { Zint16,   Cx0002, 0x0011,  Z_(LowTempThreshold),           Cm1, 0 },
  { Zint16,   Cx0002, 0x0012,  Z_(HighTempThreshold),           Cm1, 0 },
  { Zuint24,  Cx0002, 0x0013,  Z_(LowTempDwellTripPoint),           Cm1, 0 },
  { Zuint24,  Cx0002, 0x0014,  Z_(HighTempDwellTripPoint),           Cm1, 0 },
  
  // Identify cluster
  { Zuint16,  Cx0003, 0x0000,  Z_(IdentifyTime),         Cm1, 0 },

  // Groups cluster
  { Zmap8,    Cx0004, 0x0000,  Z_(GroupNameSupport),     Cm1, 0 },

  // Scenes cluster
  { Zuint8,   Cx0005, 0x0000,  Z_(SceneCount),           Cm1, 0 },
  { Zuint8,   Cx0005, 0x0001,  Z_(CurrentScene),         Cm1, 0 },
  { Zuint16,  Cx0005, 0x0002,  Z_(CurrentGroup),         Cm1, 0 },
  { Zbool,    Cx0005, 0x0003,  Z_(SceneValid),           Cm1, 0 },
  { Zmap8,    Cx0005, 0x0004,  Z_(SceneNameSupport),           Cm1, 0 },
  { ZEUI64,   Cx0005, 0x0005,  Z_(LastConfiguredBy),           Cm1, 0 },
  //{ Zmap8,    Cx0005, 0x0004,  (NameSupport),           Cm1, 0 },

  // On/off cluster
  { Zbool,    Cx0006, 0x0000,  Z_(Power),               Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_OnOff, power) },
  { Zenum8,   Cx0006, 0x4003,  Z_(StartUpOnOff),        Cm1, 0 },
  { Zbool,    Cx0006, 0x8000,  Z_(Power),               Cm1, 0 },   // See 7280
  { Zbool,    Cx0006, 0x4000,  Z_(OnOff),               Cm1, 0 },
  { Zuint16,  Cx0006, 0x4001,  Z_(OnTime),              Cm1, 0 },
  { Zuint16,  Cx0006, 0x4002,  Z_(OffWaitTime),         Cm1, 0 },

  // On/Off Switch Configuration cluster
  { Zenum8,   Cx0007, 0x0000,  Z_(SwitchType),           Cm1, 0 },
  { Zenum8,   Cx0007, 0x0010,  Z_(SwitchActions),        Cm1, 0 },

  // Level Control cluster
  { Zuint8,   Cx0008, 0x0000,  Z_(Dimmer),                Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, dimmer) },
  { Zuint16,  Cx0008, 0x0001,  Z_(DimmerRemainingTime),   Cm1, 0 },
  { Zuint8,   Cx0008, 0x0002,  Z_(DimmerMinLevel),        Cm1, 0 },
  { Zuint8,   Cx0008, 0x0003,  Z_(DimmerMaxLevel),        Cm1, 0 },
  { Zuint16,  Cx0008, 0x0004,  Z_(DimmerCurrentFrequency),Cm1, 0 },
  { Zuint16,  Cx0008, 0x0005,  Z_(DimmerMinFrequency),    Cm1, 0 },
  { Zuint16,  Cx0008, 0x0006,  Z_(DimmerMaxFrequency),    Cm1, 0 },
  { Zuint16,  Cx0008, 0x0010,  Z_(OnOffTransitionTime),   Cm1, 0 },
  { Zuint8,   Cx0008, 0x0011,  Z_(OnLevel),               Cm1, 0 },
  { Zuint16,  Cx0008, 0x0012,  Z_(OnTransitionTime),      Cm1, 0 },
  { Zuint16,  Cx0008, 0x0013,  Z_(OffTransitionTime),     Cm1, 0 },
  { Zuint16,  Cx0008, 0x0014,  Z_(DefaultMoveRate),       Cm1, 0 },
  { Zmap8,    Cx0008, 0x000F,  Z_(DimmerOptions),         Cm1, 0 },
  { Zuint8,   Cx0008, 0x4000,  Z_(DimmerStartUpLevel),    Cm1, 0 },

  // Alarms cluster
  { Zuint16,  Cx0009, 0x0000,  Z_(AlarmCount),           Cm1, 0 },

  // Time cluster
  { ZUTC,     Cx000A, 0x0000,  Z_(Time),                 Cm1, 0 },
  { Zmap8,    Cx000A, 0x0001,  Z_(TimeStatus),           Cm1, 0 },
  { Zint32,   Cx000A, 0x0002,  Z_(TimeZone),             Cm1, 0 },
  { Zuint32,  Cx000A, 0x0003,  Z_(DstStart),             Cm1, 0 },
  { Zuint32,  Cx000A, 0x0004,  Z_(DstEnd),               Cm1, 0 },
  { Zint32,   Cx000A, 0x0005,  Z_(DstShift),             Cm1, 0 },
  { Zuint32,  Cx000A, 0x0006,  Z_(StandardTime),         Cm1, 0 },
  { Zuint32,  Cx000A, 0x0007,  Z_(LocalTime),            Cm1, 0 },
  { ZUTC,     Cx000A, 0x0008,  Z_(LastSetTime),          Cm1, 0 },
  { ZUTC,     Cx000A, 0x0009,  Z_(ValidUntilTime),       Cm1, 0 },
  { ZUTC,     Cx000A, 0xFF00,  Z_(TimeEpoch),            Cm1, 0 },    // Tasmota specific, epoch

  // RSSI Location cluster
  { Zdata8,   Cx000B, 0x0000,  Z_(LocationType),         Cm1, 0 },
  { Zenum8,   Cx000B, 0x0001,  Z_(LocationMethod),       Cm1, 0 },
  { Zuint16,  Cx000B, 0x0002,  Z_(LocationAge),          Cm1, 0 },
  { Zuint8,   Cx000B, 0x0003,  Z_(QualityMeasure),       Cm1, 0 },
  { Zuint8,   Cx000B, 0x0004,  Z_(NumberOfDevices),      Cm1, 0 },
  { Zint16,   Cx000B, 0x0010,  Z_(Coordinate1),           Cm1, 0 },
  { Zint16,   Cx000B, 0x0011,  Z_(Coordinate2),           Cm1, 0 },
  { Zint16,   Cx000B, 0x0012,  Z_(Coordinate3),           Cm1, 0 },
  { Zint16,   Cx000B, 0x0013,  Z_(LocationPower),           Cm1, 0 },
  { Zuint16,  Cx000B, 0x0014,  Z_(PathLossExponent),           Cm1, 0 },
  { Zuint16,  Cx000B, 0x0015,  Z_(ReportingPeriod),           Cm1, 0 },
  { Zuint16,  Cx000B, 0x0016,  Z_(CalculationPeriod),           Cm1, 0 },
  { Zuint8,   Cx000B, 0x0016,  Z_(NumberRSSIMeasurements),           Cm1, 0 },

  // Analog Input cluster
  // { 0xFF, Cx000C, 0x0004,  (AnalogInActiveText),   Cm1, 0 },
  { Zstring,  Cx000C, 0x001C,  Z_(AnalogInDescription),  Cm1, 0 },
  // { 0xFF, Cx000C, 0x002E,  (AnalogInInactiveText), Cm1, 0 },
  { Zsingle,  Cx000C, 0x0041,  Z_(AnalogInMaxValue),     Cm1, 0 },
  { Zsingle,  Cx000C, 0x0045,  Z_(AnalogInMinValue),     Cm1, 0 },
  { Zbool,    Cx000C, 0x0051,  Z_(AnalogInOutOfService), Cm1, 0 },
  { Zsingle,  Cx000C, 0x0055,  Z_(AnalogValue),          Cm1, 0 },
  // { 0xFF, Cx000C, 0x0057,  (AnalogInPriorityArray),Cm1, 0 },
  { Zenum8,   Cx000C, 0x0067,  Z_(AnalogInReliability),  Cm1, 0 },
  // { 0xFF, Cx000C, 0x0068,  (AnalogInRelinquishDefault),Cm1, 0 },
  { Zsingle,  Cx000C, 0x006A,  Z_(AnalogInResolution),   Cm1, 0 },
  { Zmap8,    Cx000C, 0x006F,  Z_(AnalogInStatusFlags),  Cm1, 0 },
  { Zenum16,  Cx000C, 0x0075,  Z_(AnalogInEngineeringUnits),Cm1, 0 },
  { Zuint32,  Cx000C, 0x0100,  Z_(AnalogInApplicationType),Cm1, 0 },
  { Zuint16,  Cx000C, 0xFF55,  Z_(AqaraRotate),          Cm1, 0 },
  { Zuint16,  Cx000C, 0xFF05,  Z_(Aqara_FF05),           Cm1, 0 },

  // Analog Output cluster
  { Zstring,  Cx000D, 0x001C,  Z_(AnalogOutDescription), Cm1, 0 },
  { Zsingle,  Cx000D, 0x0041,  Z_(AnalogOutMaxValue),    Cm1, 0 },
  { Zsingle,  Cx000D, 0x0045,  Z_(AnalogOutMinValue),    Cm1, 0 },
  { Zbool,    Cx000D, 0x0051,  Z_(AnalogOutOutOfService),Cm1, 0 },
  { Zsingle,  Cx000D, 0x0055,  Z_(AnalogOutValue),       Cm1, 0 },
  // { Zunk,     Cx000D, 0x0057,  (AnalogOutPriorityArray),Cm1, 0 },
  { Zenum8,   Cx000D, 0x0067,  Z_(AnalogOutReliability), Cm1, 0 },
  { Zsingle,  Cx000D, 0x0068,  Z_(AnalogOutRelinquishDefault), Cm1, 0 },
  { Zsingle,  Cx000D, 0x006A,  Z_(AnalogOutResolution),  Cm1, 0 },
  { Zmap8,    Cx000D, 0x006F,  Z_(AnalogOutStatusFlags), Cm1, 0 },
  { Zenum16,  Cx000D, 0x0075,  Z_(AnalogOutEngineeringUnits), Cm1, 0 },
  { Zuint32,  Cx000D, 0x0100,  Z_(AnalogOutApplicationType), Cm1, 0 },

  // Analog Value cluster
  { Zstring,  Cx000E, 0x001C,  Z_(AnalogDescription),    Cm1, 0 },
  { Zbool,    Cx000E, 0x0051,  Z_(AnalogOutOfService),   Cm1, 0 },
  { Zsingle,  Cx000E, 0x0055,  Z_(AnalogValue),          Cm1, 0 },
  { Zunk,     Cx000E, 0x0057,  Z_(AnalogPriorityArray),  Cm1, 0 },
  { Zenum8,   Cx000E, 0x0067,  Z_(AnalogReliability),    Cm1, 0 },
  { Zsingle,  Cx000E, 0x0068,  Z_(AnalogRelinquishDefault),Cm1, 0 },
  { Zmap8,    Cx000E, 0x006F,  Z_(AnalogStatusFlags),    Cm1, 0 },
  { Zenum16,  Cx000E, 0x0075,  Z_(AnalogEngineeringUnits),Cm1, 0 },
  { Zuint32,  Cx000E, 0x0100,  Z_(AnalogApplicationType),Cm1, 0 },

  // Binary Input cluster
  { Zstring,  Cx000F, 0x0004,  Z_(BinaryInActiveText),  Cm1, 0 },
  { Zstring,  Cx000F, 0x001C,  Z_(BinaryInDescription), Cm1, 0 },
  { Zstring,  Cx000F, 0x002E,  Z_(BinaryInInactiveText),Cm1, 0 },
  { Zbool,    Cx000F, 0x0051,  Z_(BinaryInOutOfService),Cm1, 0 },
  { Zenum8,   Cx000F, 0x0054,  Z_(BinaryInPolarity),    Cm1, 0 },
  { Zbool,    Cx000F, 0x0055,  Z_(BinaryInValue),       Cm1, 0 },
  // { 0xFF, Cx000F, 0x0057,  (BinaryInPriorityArray),Cm1, 0 },
  { Zenum8,   Cx000F, 0x0067,  Z_(BinaryInReliability), Cm1, 0 },
  { Zmap8,    Cx000F, 0x006F,  Z_(BinaryInStatusFlags), Cm1, 0 },
  { Zuint32,  Cx000F, 0x0100,  Z_(BinaryInApplicationType),Cm1, 0 },

  // Binary Output cluster
  { Zstring,  Cx0010, 0x0004,  Z_(BinaryOutActiveText),  Cm1, 0 },
  { Zstring,  Cx0010, 0x001C,  Z_(BinaryOutDescription), Cm1, 0 },
  { Zstring,  Cx0010, 0x002E,  Z_(BinaryOutInactiveText),Cm1, 0 },
  { Zuint32,  Cx0010, 0x0042,  Z_(BinaryOutMinimumOffTime),Cm1, 0 },
  { Zuint32,  Cx0010, 0x0043,  Z_(BinaryOutMinimumOnTime),Cm1, 0 },
  { Zbool,    Cx0010, 0x0051,  Z_(BinaryOutOutOfService),Cm1, 0 },
  { Zenum8,   Cx0010, 0x0054,  Z_(BinaryOutPolarity),    Cm1, 0 },
  { Zbool,    Cx0010, 0x0055,  Z_(BinaryOutValue),       Cm1, 0 },
  // { Zunk,     Cx0010, 0x0057,  (BinaryOutPriorityArray),Cm1, 0 },
  { Zenum8,   Cx0010, 0x0067,  Z_(BinaryOutReliability), Cm1, 0 },
  { Zbool,    Cx0010, 0x0068,  Z_(BinaryOutRelinquishDefault),Cm1, 0 },
  { Zmap8,    Cx0010, 0x006F,  Z_(BinaryOutStatusFlags), Cm1, 0 },
  { Zuint32,  Cx0010, 0x0100,  Z_(BinaryOutApplicationType),Cm1, 0 },

  // Binary Value cluster
  { Zstring,  Cx0011, 0x0004,  Z_(BinaryActiveText),     Cm1, 0 },
  { Zstring,  Cx0011, 0x001C,  Z_(BinaryDescription),    Cm1, 0 },
  { Zstring,  Cx0011, 0x002E,  Z_(BinaryInactiveText),   Cm1, 0 },
  { Zuint32,  Cx0011, 0x0042,  Z_(BinaryMinimumOffTime), Cm1, 0 },
  { Zuint32,  Cx0011, 0x0043,  Z_(BinaryMinimumOnTime),  Cm1, 0 },
  { Zbool,    Cx0011, 0x0051,  Z_(BinaryOutOfService),   Cm1, 0 },
  { Zbool,    Cx0011, 0x0055,  Z_(BinaryValue),          Cm1, 0 },
  // { Zunk,     Cx0011, 0x0057,  (BinaryPriorityArray),  Cm1, 0 },
  { Zenum8,   Cx0011, 0x0067,  Z_(BinaryReliability),    Cm1, 0 },
  { Zbool,    Cx0011, 0x0068,  Z_(BinaryRelinquishDefault),Cm1, 0 },
  { Zmap8,    Cx0011, 0x006F,  Z_(BinaryStatusFlags),    Cm1, 0 },
  { Zuint32,  Cx0011, 0x0100,  Z_(BinaryApplicationType),Cm1, 0 },

  // Multistate Input cluster
  // { Zunk,     Cx0012, 0x000E,  (MultiInStateText),     Cm1, 0 },
  { Zstring,  Cx0012, 0x001C,  Z_(MultiInDescription),   Cm1, 0 },
  { Zuint16,  Cx0012, 0x004A,  Z_(MultiInNumberOfStates),Cm1, 0 },
  { Zbool,    Cx0012, 0x0051,  Z_(MultiInOutOfService),  Cm1, 0 },
  { Zuint16,  Cx0012, 0x0055,  Z_(MultiInValue),         Cm1, 0 },
  // { Zuint16,  Cx0012, 0x0055,  Z_(MultiInValue),         Cm0,  Z_AqaraCube, 0 },
  // { Zuint16,  Cx0012, 0x0055,  Z_(MultiInValue),         Cm0,  Z_AqaraButton, 0 },
  { Zenum8,   Cx0012, 0x0067,  Z_(MultiInReliability),   Cm1, 0 },
  { Zmap8,    Cx0012, 0x006F,  Z_(MultiInStatusFlags),   Cm1, 0 },
  { Zuint32,  Cx0012, 0x0100,  Z_(MultiInApplicationType),Cm1, 0 },

  // Multistate output
  // { Zunk,     Cx0013, 0x000E,  (MultiOutStateText),    Cm1, 0 },
  { Zstring,  Cx0013, 0x001C,  Z_(MultiOutDescription),  Cm1, 0 },
  { Zuint16,  Cx0013, 0x004A,  Z_(MultiOutNumberOfStates),Cm1, 0 },
  { Zbool,    Cx0013, 0x0051,  Z_(MultiOutOutOfService), Cm1, 0 },
  { Zuint16,  Cx0013, 0x0055,  Z_(MultiOutValue),        Cm1, 0 },
  // { Zunk,     Cx0013, 0x0057,  (MultiOutPriorityArray),Cm1, 0 },
  { Zenum8,   Cx0013, 0x0067,  Z_(MultiOutReliability),  Cm1, 0 },
  { Zuint16,  Cx0013, 0x0068,  Z_(MultiOutRelinquishDefault),Cm1, 0 },
  { Zmap8,    Cx0013, 0x006F,  Z_(MultiOutStatusFlags),  Cm1, 0 },
  { Zuint32,  Cx0013, 0x0100,  Z_(MultiOutApplicationType),Cm1, 0 },

  // Multistate Value cluster
  // { Zunk,     Cx0014, 0x000E,  (MultiStateText),       Cm1, 0 },
  { Zstring,  Cx0014, 0x001C,  Z_(MultiDescription),     Cm1, 0 },
  { Zuint16,  Cx0014, 0x004A,  Z_(MultiNumberOfStates),  Cm1, 0 },
  { Zbool,    Cx0014, 0x0051,  Z_(MultiOutOfService),    Cm1, 0 },
  { Zuint16,  Cx0014, 0x0055,  Z_(MultiValue),           Cm1, 0 },
  { Zenum8,   Cx0014, 0x0067,  Z_(MultiReliability),     Cm1, 0 },
  { Zuint16,  Cx0014, 0x0068,  Z_(MultiRelinquishDefault),Cm1, 0 },
  { Zmap8,    Cx0014, 0x006F,  Z_(MultiStatusFlags),     Cm1, 0 },
  { Zuint32,  Cx0014, 0x0100,  Z_(MultiApplicationType), Cm1, 0 },

  // Power Profile cluster
  { Zuint8,   Cx001A, 0x0000,  Z_(TotalProfileNum),      Cm1, 0 },
  { Zbool,    Cx001A, 0x0001,  Z_(MultipleScheduling),   Cm1, 0 },
  { Zmap8,    Cx001A, 0x0002,  Z_(EnergyFormatting),     Cm1, 0 },
  { Zbool,    Cx001A, 0x0003,  Z_(EnergyRemote),         Cm1, 0 },
  { Zmap8,    Cx001A, 0x0004,  Z_(ScheduleMode),         Cm1, 0 },

  // Poll Control cluster
  { Zuint32,  Cx0020, 0x0000,  Z_(CheckinInterval),      Cm1, 0 },
  { Zuint32,  Cx0020, 0x0001,  Z_(LongPollInterval),     Cm1, 0 },
  { Zuint16,  Cx0020, 0x0002,  Z_(ShortPollInterval),    Cm1, 0 },
  { Zuint16,  Cx0020, 0x0003,  Z_(FastPollTimeout),      Cm1, 0 },
  { Zuint32,  Cx0020, 0x0004,  Z_(CheckinIntervalMin),   Cm1, 0 },
  { Zuint32,  Cx0020, 0x0005,  Z_(LongPollIntervalMin),  Cm1, 0 },
  { Zuint16,  Cx0020, 0x0006,  Z_(FastPollTimeoutMax),   Cm1, 0 },

  // Green Power
  // Server attributes
  { Zuint8,   Cx0021, 0x0000,  Z_(MaxSinkTableEntries),  Cm1, 0 },
  { Zoctstr16,Cx0021, 0x0001,  Z_(SinkTable),            Cm1, 0 },
  { Zmap8,    Cx0021, 0x0002,  Z_(CommunicationMode),    Cm1, 0 },
  { Zmap8,    Cx0021, 0x0003,  Z_(CcommissioningExitMode),Cm1, 0 },
  { Zuint16,  Cx0021, 0x0004,  Z_(CommissioningWindow),  Cm1, 0 },
  { Zmap8,    Cx0021, 0x0005,  Z_(SecurityLevel),        Cm1, 0 },
  { Zmap24,   Cx0021, 0x0006,  Z_(ServerFunctionality),        Cm1, 0 },
  { Zmap24,   Cx0021, 0x0007,  Z_(ServerActiveFunctionality),  Cm1, 0 },
  { Zuint8,   Cx0021, 0x0010,  Z_(MaxProxyTableEntries), Cm1, 0 },
  { Zoctstr16,Cx0021, 0x0011,  Z_(ProxyTable),           Cm1, 0 },
  { Zuint8,   Cx0021, 0x0012,  Z_(NotificationRetryNumber),Cm1, 0 },
  { Zuint8,   Cx0021, 0x0013,  Z_(NotificationRetryTimer),Cm1, 0 },
  { Zuint8,   Cx0021, 0x0014,  Z_(MaxSearchCounter),     Cm1, 0 },
  { Zoctstr16,Cx0021, 0x0015,  Z_(BlockedGPDID),         Cm1, 0 },
  { Zmap24,   Cx0021, 0x0016,  Z_(ClientFunctionality),  Cm1, 0 },
  { Zmap24,   Cx0021, 0x0017,  Z_(ClientActiveFunctionality),  Cm1, 0 },
  // Shared by Server and Client
  { Zmap8,    Cx0021, 0x0020,  Z_(SharedSecurityKeyType),Cm1, 0 },
  { Zkey128,  Cx0021, 0x0021,  Z_(SharedSecurityKey),    Cm1, 0 },
  { Zkey128,  Cx0021, 0x0022,  Z_(LinkKey),  Cm1, 0 },
  
  // Shade Configuration cluster
  { Zuint16,  Cx0100, 0x0000,  Z_(PhysicalClosedLimit),  Cm1, 0 },
  { Zuint8,   Cx0100, 0x0001,  Z_(MotorStepSize),        Cm1, 0 },
  { Zmap8,    Cx0100, 0x0002,  Z_(Status),               Cm1, 0 },
  { Zuint16,  Cx0100, 0x0010,  Z_(ClosedLimit),          Cm1, 0 },
  { Zenum8,   Cx0100, 0x0011,  Z_(Mode),                 Cm1, 0 },

  // Door Lock cluster
  { Zenum8,   Cx0101, 0x0000,  Z_(LockState),            Cm1, 0 },
  { Zenum8,   Cx0101, 0x0001,  Z_(LockType),             Cm1, 0 },
  { Zbool,    Cx0101, 0x0002,  Z_(ActuatorEnabled),      Cm1, 0 },
  { Zenum8,   Cx0101, 0x0003,  Z_(DoorState),            Cm1, 0 },
  { Zuint32,  Cx0101, 0x0004,  Z_(DoorOpenEvents),       Cm1, 0 },
  { Zuint32,  Cx0101, 0x0005,  Z_(DoorClosedEvents),     Cm1, 0 },
  { Zuint16,  Cx0101, 0x0006,  Z_(OpenPeriod),           Cm1, 0 },

  // Door locks
  { Zuint16,  Cx0101, 0x0010,  Z_(NumberOfLogRecordsSupported),           Cm1, 0 },
  { Zuint16,  Cx0101, 0x0011,  Z_(NumberOfTotalUsersSupported),           Cm1, 0 },
  { Zuint16,  Cx0101, 0x0012,  Z_(NumberOfPINUsersSupported),           Cm1, 0 },
  { Zuint16,  Cx0101, 0x0013,  Z_(NumberOfRFIDUsersSupported),           Cm1, 0 },
  { Zuint8,   Cx0101, 0x0014,  Z_(NumberOfWeekDaySchedulesSupportedPerUser),           Cm1, 0 },
  { Zuint8,   Cx0101, 0x0015,  Z_(NumberOfYearDaySchedulesSupportedPerUser),           Cm1, 0 },
  { Zuint8,   Cx0101, 0x0016,  Z_(NumberOfHolidaySchedulesSupported),           Cm1, 0 },
  { Zuint8,   Cx0101, 0x0017,  Z_(MaxPINCodeLength),           Cm1, 0 },
  { Zuint8,   Cx0101, 0x0018,  Z_(MinPINCodeLength),           Cm1, 0 },
  { Zuint8,   Cx0101, 0x0019,  Z_(MaxRFIDCodeLength),           Cm1, 0 },
  { Zuint8,   Cx0101, 0x0011,  Z_(MinRFIDCodeLength),           Cm1, 0 },
  { Zbool,    Cx0101, 0x0020,  Z_(LockEnableLogging),           Cm1, 0 },
  { Zstring,  Cx0101, 0x0021,  Z_(LockLanguage),           Cm1, 0 },
  { Zuint8,   Cx0101, 0x0022,  Z_(LockLEDSettings),           Cm1, 0 },
  { Zuint32,  Cx0101, 0x0023,  Z_(AutoRelockTime),           Cm1, 0 },
  { Zuint8,   Cx0101, 0x0024,  Z_(LockSoundVolume),           Cm1, 0 },
  { Zenum8,   Cx0101, 0x0025,  Z_(LockOperatingMode),           Cm1, 0 },
  { Zmap16,   Cx0101, 0x0026,  Z_(LockSupportedOperatingModes),           Cm1, 0 },
  { Zmap16,   Cx0101, 0x0027,  Z_(LockDefaultConfigurationRegister),           Cm1, 0 },
  { Zbool,    Cx0101, 0x0028,  Z_(LockEnableLocalProgramming),           Cm1, 0 },
  { Zbool,    Cx0101, 0x0029,  Z_(LockEnableOneTouchLocking),           Cm1, 0 },
  { Zbool,    Cx0101, 0x002A,  Z_(LockEnableInsideStatusLED),           Cm1, 0 },
  { Zbool,    Cx0101, 0x002B,  Z_(LockEnablePrivacyModeButton),           Cm1, 0 },
  { Zmap16,   Cx0101, 0x0040,  Z_(LockAlarmMask),           Cm1, 0 },
  { Zmap16,   Cx0101, 0x0041,  Z_(LockKeypadOperationEventMask),           Cm1, 0 },
  { Zmap16,   Cx0101, 0x0042,  Z_(LockRFOperationEventMask),           Cm1, 0 },
  { Zmap16,   Cx0101, 0x0043,  Z_(LockManualOperationEventMask),           Cm1, 0 },
  { Zmap16,   Cx0101, 0x0044,  Z_(LockRFIDOperationEventMask),           Cm1, 0 },
  { Zmap16,   Cx0101, 0x0045,  Z_(LockKeypadProgrammingEventMask),           Cm1, 0 },
  { Zmap16,   Cx0101, 0x0046,  Z_(LockRFProgrammingEventMask),           Cm1, 0 },
  { Zmap16,   Cx0101, 0x0047,  Z_(LockRFIDProgrammingEventMask),           Cm1, 0 },
  // Aqara Lumi Vibration Sensor
  { Zuint16,  Cx0101, 0x0055,  Z_(AqaraVibrationMode),   Cm1, 0 },
  { Zuint16,  Cx0101, 0x0503,  Z_(AqaraVibrationsOrAngle), Cm1, 0 },
  { Zuint32,  Cx0101, 0x0505,  Z_(AqaraVibration505),    Cm1, 0 },
  { Zuint48,  Cx0101, 0x0508,  Z_(AqaraAccelerometer),   Cm1, 0 },

  // Window Covering cluster
  { Zenum8,   Cx0102, 0x0000,  Z_(WindowCoveringType),   Cm1, 0 },
  { Zuint16,  Cx0102, 0x0001,  Z_(PhysicalClosedLimitLift),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0002,  Z_(PhysicalClosedLimitTilt),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0003,  Z_(CurrentPositionLift),  Cm1, 0 },
  { Zuint16,  Cx0102, 0x0004,  Z_(CurrentPositionTilt),  Cm1, 0 },
  { Zuint16,  Cx0102, 0x0005,  Z_(NumberofActuationsLift),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0006,  Z_(NumberofActuationsTilt),Cm1, 0 },
  { Zmap8,    Cx0102, 0x0007,  Z_(ConfigStatus),         Cm1, 0 },
  { Zuint8,   Cx0102, 0x0008,  Z_(CurrentPositionLiftPercentage),Cm1, 0 },
  { Zuint8,   Cx0102, 0x0009,  Z_(CurrentPositionTiltPercentage),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0010,  Z_(InstalledOpenLimitLift),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0011,  Z_(InstalledClosedLimitLift),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0012,  Z_(InstalledOpenLimitTilt),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0013,  Z_(InstalledClosedLimitTilt),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0014,  Z_(VelocityLift),         Cm1, 0 },
  { Zuint16,  Cx0102, 0x0015,  Z_(AccelerationTimeLift),Cm1, 0 },
  { Zuint16,  Cx0102, 0x0016,  Z_(DecelerationTimeLift), Cm1, 0 },
  { Zmap8,    Cx0102, 0x0017,  Z_(Mode),                 Cm1, 0 },
  { Zoctstr,  Cx0102, 0x0018,  Z_(IntermediateSetpointsLift),Cm1, 0 },
  { Zoctstr,  Cx0102, 0x0019,  Z_(IntermediateSetpointsTilt),Cm1, 0 },
  // Tuya specific, from Zigbee2MQTT https://github.com/Koenkk/zigbee-herdsman/blob/4fed7310d1e1e9d81e5148cf5b4d8ec98d03c687/src/zcl/definition/cluster.ts#L1772
  { Zenum8,   Cx0102, 0xF000,  Z_(TuyaMovingState),Cm1, 0 },
  { Zenum8,   Cx0102, 0xF001,  Z_(TuyaCalibration),Cm1, 0 },
  { Zenum8,   Cx0102, 0xF002,  Z_(TuyaMotorReversal),Cm1, 0 },
  { Zuint16,  Cx0102, 0xF003,  Z_(TuyaCalibrationTime),Cm1, 0 },

  // Thermostat
  { Zint16,   Cx0201, 0x0000,  Z_(LocalTemperature),  Cm_100, Z_MAPPING(Z_Data_Thermo, temperature) },
  { Zint16,   Cx0201, 0x0001,  Z_(OutdoorTemperature),Cm_100, 0 },
  { Zmap8,    Cx0201, 0x0002,  Z_(ThermostatOccupancy),           Cm1, 0 },
  { Zint16,   Cx0201, 0x0003,  Z_(AbsMinHeatSetpointLimit),           Cm1, 0 },
  { Zint16,   Cx0201, 0x0004,  Z_(AbsMaxHeatSetpointLimit),           Cm1, 0 },
  { Zint16,   Cx0201, 0x0005,  Z_(AbsMinCoolSetpointLimit),           Cm1, 0 },
  { Zint16,   Cx0201, 0x0006,  Z_(AbsMaxCoolSetpointLimit),           Cm1, 0 },
  { Zuint8,   Cx0201, 0x0007,  Z_(PICoolingDemand),      Cm1, Z_MAPPING(Z_Data_Thermo, th_setpoint) },
  { Zuint8,   Cx0201, 0x0008,  Z_(PIHeatingDemand),      Cm1, Z_MAPPING(Z_Data_Thermo, th_setpoint) },
  { Zmap8,    Cx0201, 0x0009,  Z_(HVACSystemTypeConfiguration),           Cm1, 0 },
  { Zint8,    Cx0201, 0x0010,  Z_(LocalTemperatureCalibration), Cm_10, 0 },
  { Zint16,   Cx0201, 0x0011,  Z_(OccupiedCoolingSetpoint), Cm_100, Z_MAPPING(Z_Data_Thermo, temperature_target) },
  { Zint16,   Cx0201, 0x0012,  Z_(OccupiedHeatingSetpoint), Cm_100, Z_MAPPING(Z_Data_Thermo, temperature_target) },
  { Zint16,   Cx0201, 0x0013,  Z_(UnoccupiedCoolingSetpoint), Cm_100, 0 },
  { Zint16,   Cx0201, 0x0014,  Z_(UnoccupiedHeatingSetpoint), Cm_100, 0 },
  { Zint16,   Cx0201, 0x0015,  Z_(MinHeatSetpointLimit),           Cm1, 0 },
  { Zint16,   Cx0201, 0x0016,  Z_(MaxHeatSetpointLimit),           Cm1, 0 },
  { Zint16,   Cx0201, 0x0017,  Z_(MinCoolSetpointLimit),           Cm1, 0 },
  { Zint16,   Cx0201, 0x0018,  Z_(MaxCoolSetpointLimit),           Cm1, 0 },
  { Zint8,    Cx0201, 0x0019,  Z_(MinSetpointDeadBand),           Cm1, 0 },
  { Zmap8,    Cx0201, 0x001D,  Z_(ThermostatAlarmMask),           Cm1, 0 },
  { Zenum8,   Cx0201, 0x001E,  Z_(ThermostatRunningMode),           Cm1, 0 },
  { Zmap8,    Cx0201, 0x001A,  Z_(RemoteSensing),        Cm1, 0 },
  { Zenum8,   Cx0201, 0x001B,  Z_(ControlSequenceOfOperation), Cm1, 0 },
  { Zenum8,   Cx0201, 0x001C,  Z_(SystemMode),           Cm1, 0 },
  // below is Eurotronic specific
  { Zenum8,   Cx0201, 0x4000, Z_(TRVMode),               Cm1, 0 },
  { Zuint8,   Cx0201, 0x4001, Z_(ValvePosition),         Cm1, 0 },
  { Zuint8,   Cx0201, 0x4002, Z_(EurotronicErrors),      Cm1, 0 },
  { Zint16,   Cx0201, 0x4003, Z_(CurrentTemperatureSetPoint), Cm_100, 0 },
  { Zuint24,  Cx0201, 0x4008, Z_(EurotronicHostFlags),   Cm1, 0 },
  // below are synthetic virtual attributes used to decode EurotronicHostFlags
  // Last byte acts as a field mask for the lowest byte value
  { Zbool,    Cx0201, 0xF002, Z_(TRVMirrorDisplay),      Cm1, 0 },
  { Zbool,    Cx0201, 0xF004, Z_(TRVBoost),              Cm1, 0 },
  { Zbool,    Cx0201, 0xF010, Z_(TRVWindowOpen),         Cm1, 0 },
  { Zbool,    Cx0201, 0xF080, Z_(TRVChildProtection),    Cm1, 0 },
  // below are virtual attributes to simplify ZbData import/export
  { Zuint8,   Cx0201, 0xFFF0,  Z_(ThSetpoint),           Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Thermo, th_setpoint) },
  { Zint16,   Cx0201, 0xFFF1,  Z_(TempTarget),        Cm_100 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Thermo, temperature_target) },

  // Fan Control
  { Zenum8,   Cx0202, 0x0000,  Z_(FanMode),               Cm1, 0 },
  { Zenum8,   Cx0202, 0x0001,  Z_(FanModeSequence),       Cm1, 0 },

  // Dehumidification Control
  { Zuint8,   Cx0203, 0x0000,  Z_(RelativeHumidity),           Cm1, 0 },
  { Zuint8,   Cx0203, 0x0001,  Z_(DehumidificationCooling),           Cm1, 0 },
  { Zuint8,   Cx0203, 0x0010,  Z_(RHDehumidificationSetpoint),           Cm1, 0 },
  { Zenum8,   Cx0203, 0x0011,  Z_(RelativeHumidityMode),           Cm1, 0 },
  { Zenum8,   Cx0203, 0x0012,  Z_(DehumidificationLockout),           Cm1, 0 },
  { Zuint8,   Cx0203, 0x0013,  Z_(DehumidificationHysteresis),           Cm1, 0 },
  { Zuint8,   Cx0203, 0x0014,  Z_(DehumidificationMaxCool),           Cm1, 0 },
  { Zenum8,   Cx0203, 0x0015,  Z_(RelativeHumidityDisplay),           Cm1, 0 },

  // Thermostat User Interface Con- figuration
  { Zenum8,   Cx0204, 0x0000,  Z_(TemperatureDisplayMode),           Cm1, 0 },
  { Zenum8,   Cx0204, 0x0001,  Z_(ThermostatKeypadLockout),           Cm1, 0 },
  { Zenum8,   Cx0204, 0x0002,  Z_(ThermostatScheduleProgrammingVisibility),           Cm1, 0 },

  // Color Control cluster
  { Zuint8,   Cx0300, 0x0000,  Z_(Hue),                  Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, hue) },
  { Zuint8,   Cx0300, 0x0001,  Z_(Sat),                  Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, sat) },
  { Zuint16,  Cx0300, 0x0002,  Z_(RemainingTime),        Cm1, 0 },
  { Zuint16,  Cx0300, 0x0003,  Z_(X),                    Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, x) },
  { Zuint16,  Cx0300, 0x0004,  Z_(Y),                    Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, y) },
  { Zenum8,   Cx0300, 0x0005,  Z_(DriftCompensation),    Cm1, 0 },
  { Zstring,  Cx0300, 0x0006,  Z_(CompensationText),     Cm1, 0 },
  { Zuint16,  Cx0300, 0x0007,  Z_(CT),                   Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, ct) },
  { Zenum8,   Cx0300, 0x0008,  Z_(ColorMode),            Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Light, colormode) },
  { Zuint8,   Cx0300, 0x0010,  Z_(NumberOfPrimaries),    Cm1, 0 },
  { Zuint16,  Cx0300, 0x0011,  Z_(Primary1X),            Cm1, 0 },
  { Zuint16,  Cx0300, 0x0012,  Z_(Primary1Y),            Cm1, 0 },
  { Zuint8,   Cx0300, 0x0013,  Z_(Primary1Intensity),    Cm1, 0 },
  { Zuint16,  Cx0300, 0x0015,  Z_(Primary2X),            Cm1, 0 },
  { Zuint16,  Cx0300, 0x0016,  Z_(Primary2Y),            Cm1, 0 },
  { Zuint8,   Cx0300, 0x0017,  Z_(Primary2Intensity),    Cm1, 0 },
  { Zuint16,  Cx0300, 0x0019,  Z_(Primary3X),            Cm1, 0 },
  { Zuint16,  Cx0300, 0x001A,  Z_(Primary3Y),            Cm1, 0 },
  { Zuint8,   Cx0300, 0x001B,  Z_(Primary3Intensity),    Cm1, 0 },
  { Zuint16,  Cx0300, 0x0020,  Z_(Primary4X),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x0021,  Z_(Primary4Y),           Cm1, 0 },
  { Zuint8,   Cx0300, 0x0022,  Z_(Primary4Intensity),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x0024,  Z_(Primary5X),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x0025,  Z_(Primary5Y),           Cm1, 0 },
  { Zuint8,   Cx0300, 0x0026,  Z_(Primary5Intensity),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x0028,  Z_(Primary6X),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x0029,  Z_(Primary6Y),           Cm1, 0 },
  { Zuint8,   Cx0300, 0x002A,  Z_(Primary6Intensity),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x0030,  Z_(WhitePointX),          Cm1, 0 },
  { Zuint16,  Cx0300, 0x0031,  Z_(WhitePointY),          Cm1, 0 },
  { Zuint16,  Cx0300, 0x0032,  Z_(ColorPointRX),         Cm1, 0 },
  { Zuint16,  Cx0300, 0x0033,  Z_(ColorPointRY),         Cm1, 0 },
  { Zuint8,   Cx0300, 0x0034,  Z_(ColorPointRIntensity), Cm1, 0 },
  { Zuint16,  Cx0300, 0x0036,  Z_(ColorPointGX),         Cm1, 0 },
  { Zuint16,  Cx0300, 0x0037,  Z_(ColorPointGY),         Cm1, 0 },
  { Zuint8,   Cx0300, 0x0038,  Z_(ColorPointGIntensity), Cm1, 0 },
  { Zuint16,  Cx0300, 0x003A,  Z_(ColorPointBX),         Cm1, 0 },
  { Zuint16,  Cx0300, 0x003B,  Z_(ColorPointBY),         Cm1, 0 },
  { Zuint8,   Cx0300, 0x003C,  Z_(ColorPointBIntensity), Cm1, 0 },
  { Zuint16,  Cx0300, 0x4000,  Z_(EnhancedCurrentHue),           Cm1, 0 },
  { Zenum8,   Cx0300, 0x4001,  Z_(EnhancedColorMode),           Cm1, 0 },
  { Zuint8,   Cx0300, 0x4002,  Z_(ColorLoopActive),           Cm1, 0 },
  { Zuint8,   Cx0300, 0x4003,  Z_(ColorLoopDirection),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x4004,  Z_(ColorLoopTime),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x4005,  Z_(ColorLoopStartEnhancedHue),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x4006,  Z_(ColorLoopStoredEnhancedHue),           Cm1, 0 },
  { Zmap16,   Cx0300, 0x400A,  Z_(ColorCapabilities),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x400B,  Z_(ColorTempPhysicalMinMireds),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x400C,  Z_(ColorTempPhysicalMaxMireds),           Cm1, 0 },
  { Zuint16,  Cx0300, 0x4010,  Z_(ColorStartUpColorTempireds),           Cm1, 0 },

  // Ballast Configuration
  { Zuint8,   Cx0301, 0x0000,  Z_(BallastPhysicalMinLevel),           Cm1, 0 },
  { Zuint8,   Cx0301, 0x0001,  Z_(BallastPhysicalMaxLevel),           Cm1, 0 },
  { Zmap8,    Cx0301, 0x0002,  Z_(BallastStatus),           Cm1, 0 },
  { Zuint8,   Cx0301, 0x0010,  Z_(BallastMinLevel),           Cm1, 0 },
  { Zuint8,   Cx0301, 0x0011,  Z_(BallastMaxLevel),           Cm1, 0 },
  { Zuint8,   Cx0301, 0x0012,  Z_(BallastPowerOnLevel),           Cm1, 0 },
  { Zuint16,  Cx0301, 0x0013,  Z_(BallastPowerOnFadeTime),           Cm1, 0 },
  { Zuint8,   Cx0301, 0x0014,  Z_(IntrinsicBallastFactor),           Cm1, 0 },
  { Zuint8,   Cx0301, 0x0015,  Z_(BallastFactorAdjustment),           Cm1, 0 },
  { Zuint8,   Cx0301, 0x0020,  Z_(BallastLampQuantity),           Cm1, 0 },
  { Zstring,  Cx0301, 0x0030,  Z_(LampType),           Cm1, 0 },
  { Zstring,  Cx0301, 0x0031,  Z_(LampManufacturer),           Cm1, 0 },
  { Zuint24,  Cx0301, 0x0032,  Z_(LampRatedHours),           Cm1, 0 },
  { Zuint24,  Cx0301, 0x0033,  Z_(LampBurnHours),           Cm1, 0 },
  { Zmap8,    Cx0301, 0x0034,  Z_(LampAlarmMode),           Cm1, 0 },
  { Zuint24,  Cx0301, 0x0035,  Z_(LampBurnHoursTripPoint),           Cm1, 0 },

  // Illuminance Measurement cluster
  { Zuint16,  Cx0400, 0x0000,  Z_(Illuminance),          Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_PIR, illuminance) }, // Illuminance (in Lux)
  { Zuint16,  Cx0400, 0x0001,  Z_(IlluminanceMinMeasuredValue),     Cm1, 0 },    //
  { Zuint16,  Cx0400, 0x0002,  Z_(IlluminanceMaxMeasuredValue),     Cm1, 0 },    //
  { Zuint16,  Cx0400, 0x0003,  Z_(IlluminanceTolerance),            Cm1, 0 },    //
  { Zenum8,   Cx0400, 0x0004,  Z_(IlluminanceLightSensorType),      Cm1, 0 },    //
  // { Zunk,     Cx0400, 0xFFFF,  Z_(),                    Cm0, 0 },    // Remove all other values

  // Illuminance Level Sensing cluster
  { Zenum8,   Cx0401, 0x0000,  Z_(IlluminanceLevelStatus),          Cm1, 0 },    // Illuminance (in Lux)
  { Zenum8,   Cx0401, 0x0001,  Z_(IlluminanceLightSensorType),      Cm1, 0 },    // LightSensorType
  { Zuint16,  Cx0401, 0x0010,  Z_(IlluminanceTargetLevel),          Cm1, 0 },    //
  // { Zunk,     Cx0401, 0xFFFF,  Z_(),                    Cm0, 0 },    // Remove all other values

  // Temperature Measurement cluster
  { Zint16,   Cx0402, 0x0000,  Z_(Temperature),          Cm_100 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Thermo, temperature) },
  { Zint16,   Cx0402, 0x0001,  Z_(TemperatureMinMeasuredValue),     Cm_100, 0 },    //
  { Zint16,   Cx0402, 0x0002,  Z_(TemperatureMaxMeasuredValue),     Cm_100, 0 },    //
  { Zuint16,  Cx0402, 0x0003,  Z_(TemperatureTolerance),            Cm_100, 0 },    //
  // { Zunk,     Cx0402, 0xFFFF,  Z_(),                    Cm0, 0 },     // Remove all other values

  // Pressure Measurement cluster
  { Zint16,   Cx0403, 0x0000,  Z_(Pressure),                     Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Thermo, pressure) },     // Pressure
  { Zint16,   Cx0403, 0x0001,  Z_(PressureMinMeasuredValue),     Cm1, 0 },    //
  { Zint16,   Cx0403, 0x0002,  Z_(PressureMaxMeasuredValue),     Cm1, 0 },    //
  { Zuint16,  Cx0403, 0x0003,  Z_(PressureTolerance),            Cm1, 0 },    //
  { Zint16,   Cx0403, 0x0010,  Z_(PressureScaledValue),          Cm1, 0 },    //
  { Zint16,   Cx0403, 0x0011,  Z_(PressureMinScaledValue),       Cm1, 0 },    //
  { Zint16,   Cx0403, 0x0012,  Z_(PressureMaxScaledValue),       Cm1, 0 },    //
  { Zuint16,  Cx0403, 0x0013,  Z_(PressureScaledTolerance),      Cm1, 0 },    //
  { Zint8,    Cx0403, 0x0014,  Z_(PressureScale),                Cm1, 0 },    //
  { Zint16,   Cx0403, 0xFFF0,  Z_(SeaPressure),                  Cm1, Z_MAPPING(Z_Data_Thermo, pressure) },     // Pressure at Sea Level, Tasmota specific
  // { Zunk,     Cx0403, 0xFFFF,  Z_(),                    Cm0, 0 },     // Remove all other Pressure values

  // Flow Measurement cluster
  { Zuint16,  Cx0404, 0x0000,  Z_(FlowRate),             Cm_10, 0 },    // Flow (in m3/h)
  { Zuint16,  Cx0404, 0x0001,  Z_(FlowMinMeasuredValue), Cm1, 0 },    //
  { Zuint16,  Cx0404, 0x0002,  Z_(FlowMaxMeasuredValue), Cm1, 0 },    //
  { Zuint16,  Cx0404, 0x0003,  Z_(FlowTolerance),        Cm1, 0 },    //
  // { Zunk,     Cx0404, 0xFFFF,  Z_(),                     Cm0, 0 },    // Remove all other values

  // Relative Humidity Measurement cluster
  { Zuint16,  Cx0405, 0x0000,  Z_(Humidity),                     Cm_100 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Thermo, humidity) },   // Humidity
  { Zuint16,  Cx0405, 0x0001,  Z_(HumidityMinMeasuredValue),     Cm1, 0 },    //
  { Zuint16,  Cx0405, 0x0002,  Z_(HumidityMaxMeasuredValue),     Cm1, 0 },    //
  { Zuint16,  Cx0405, 0x0003,  Z_(HumidityTolerance),            Cm1, 0 },    //
  // { Zunk,     Cx0405, 0xFFFF,  Z_(),                    Cm0, 0 },     // Remove all other values

  // Occupancy Sensing cluster
  { Zmap8,    Cx0406, 0x0000,  Z_(Occupancy),            Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_PIR, occupancy) }, // Occupancy (map8)
  { Zenum8,   Cx0406, 0x0001,  Z_(OccupancySensorType),  Cm1, 0 },    // OccupancySensorType
  { Zuint16,  Cx0406, 0x0010,  Z_(PIROccupiedToUnoccupiedDelay),           Cm1, 0 },
  { Zuint16,  Cx0406, 0x0011,  Z_(PIRUnoccupiedToOccupiedDelay),           Cm1, 0 },
  { Zuint8,   Cx0406, 0x0012,  Z_(PIRUnoccupiedToOccupiedThreshold),           Cm1, 0 },
  // { Zunk,     Cx0406, 0xFFFF,  Z_(),                    Cm0, 0 },    // Remove all other values

  // IAS Cluster (Intruder Alarm System)
  { Zenum8,   Cx0500, 0x0000,  Z_(ZoneState),             Cm1, 0 },    // Occupancy (map8)
  { Zenum16,  Cx0500, 0x0001,  Z_(ZoneType),              Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Alarm, zone_type) },    // Zone type for sensor
  { Zmap16,   Cx0500, 0x0002,  Z_(ZoneStatus),            Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Alarm, zone_status) },    // Zone status for sensor
  { ZEUI64,   Cx0500, 0x0010,  Z_(IASCIEAddress),           Cm1, 0 },
  { Zuint8,   Cx0500, 0x0011,  Z_(ZoneID),           Cm1, 0 },
  { Zuint8,   Cx0500, 0x0012,  Z_(NumberOfZoneSensitivityLevelsSupported),           Cm1, 0 },
  { Zuint8,   Cx0500, 0x0013,  Z_(CurrentZoneSensitivityLevel),           Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_CIE, Z_(CIE),           Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_PIR, Z_(Occupancy),     Cm1, 0 },    // normally converted to the actual Occupancy 0406/0000
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Contact, Z_(Contact),   Cm1, Z_MAPPING(Z_Data_Alarm, zone_status) },    // We fit the first bit in the LSB
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Fire, Z_(Fire),         Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Water, Z_(Water),        Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_CO, Z_(CO),             Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Personal, Z_(PersonalAlarm),Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Movement, Z_(Movement), Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_Panic, Z_(Panic),       Cm1, 0 },
  { Zuint8,   Cx0500, 0xFFF0 + ZA_GlassBreak, Z_(GlassBreak),Cm1, 0 },

  // Metering (Smart Energy) cluster
  { Zuint48,  Cx0702, 0x0000,  Z_(CurrentSummationDelivered),Cm1, 0 },
  { Zuint48,  Cx0702, 0x0001,  Z_(CurrentSummationReceived),Cm1, 0 },
  { Zuint48,  Cx0702, 0x0002,  Z_(CurrentMaxDemandDelivered),Cm1, 0 },
  { Zuint48,  Cx0702, 0x0003,  Z_(CurrentMaxDemandReceived),Cm1, 0 },
  { Zuint48,  Cx0702, 0x0004,  Z_(DFTSummation),         Cm1, 0 },
  { Zuint16,  Cx0702, 0x0005,  Z_(DailyFreezeTime),      Cm1, 0 },
  { Zint8,    Cx0702, 0x0006,  Z_(PowerFactor),          Cm1, 0 },
  { ZUTC,     Cx0702, 0x0007,  Z_(ReadingSnapShotTime),  Cm1, 0 },
  { ZUTC,     Cx0702, 0x0008,  Z_(CurrentMaxDemandDeliveredTime),Cm1, 0 },
  { ZUTC,     Cx0702, 0x0009,  Z_(CurrentMaxDemandReceivedTime),Cm1, 0 },
  { Zuint8,   Cx0702, 0x000A,  Z_(DefaultUpdatePeriod),  Cm1, 0 },
  { Zuint8,   Cx0702, 0x000B,  Z_(FastPollUpdatePeriod), Cm1, 0 },
  { Zuint48,  Cx0702, 0x000C,  Z_(CurrentBlockPeriodConsumptionDelivered),Cm1, 0 },
  { Zuint24,  Cx0702, 0x000D,  Z_(DailyConsumptionTarget),Cm1, 0 },
  { Zenum8,   Cx0702, 0x000E,  Z_(CurrentBlock),         Cm1, 0 },
  { Zenum8,   Cx0702, 0x000F,  Z_(ProfileIntervalPeriod),Cm1, 0 },
  { Zuint16,  Cx0702, 0x0010,  Z_(IntervalReadReportingPeriod),Cm1, 0 },
  { Zuint16,  Cx0702, 0x0011,  Z_(PresetReadingTime),    Cm1, 0 },
  { Zuint16,  Cx0702, 0x0012,  Z_(VolumePerReport),      Cm1, 0 },
  { Zuint8,   Cx0702, 0x0013,  Z_(FlowRestriction),      Cm1, 0 },
  { Zenum8,   Cx0702, 0x0014,  Z_(SupplyStatus),         Cm1, 0 },
  { Zuint48,  Cx0702, 0x0015,  Z_(CurrentInletEnergyCarrierSummation),Cm1, 0 },
  { Zuint48,  Cx0702, 0x0016,  Z_(CurrentOutletEnergyCarrierSummation),Cm1, 0 },
  { Zint24,   Cx0702, 0x0017,  Z_(InletTemperature),     Cm1, 0 },
  { Zint24,   Cx0702, 0x0018,  Z_(OutletTemperature),    Cm1, 0 },
  { Zint24,   Cx0702, 0x0019,  Z_(ControlTemperature),   Cm1, 0 },
  { Zint24,   Cx0702, 0x001A,  Z_(CurrentInletEnergyCarrierDemand),Cm1, 0 },
  { Zint24,   Cx0702, 0x001B,  Z_(CurrentOutletEnergyCarrierDemand),Cm1, 0 },
  { Zuint48,  Cx0702, 0x001C,  Z_(PreviousBlockPeriodConsumptionDelivered),Cm1, 0 },

  // Meter Identification cluster
  { Zstring,  Cx0B01, 0x0000,  Z_(CompanyName),          Cm1, 0 },
  { Zuint16,  Cx0B01, 0x0001,  Z_(MeterTypeID),          Cm1, 0 },
  { Zuint16,  Cx0B01, 0x0004,  Z_(DataQualityID),        Cm1, 0 },
  { Zstring,  Cx0B01, 0x0005,  Z_(CustomerName),         Cm1, 0 },
  { Zoctstr,  Cx0B01, 0x0006,  Z_(Model),                Cm1, 0 },
  { Zoctstr,  Cx0B01, 0x0007,  Z_(PartNumber),           Cm1, 0 },
  { Zoctstr,  Cx0B01, 0x0008,  Z_(ProductRevision),      Cm1, 0 },
  { Zoctstr,  Cx0B01, 0x000A,  Z_(SoftwareRevision),     Cm1, 0 },
  { Zstring,  Cx0B01, 0x000B,  Z_(UtilityName),          Cm1, 0 },
  { Zstring,  Cx0B01, 0x000C,  Z_(POD),                  Cm1, 0 },
  { Zint24,   Cx0B01, 0x000D,  Z_(AvailablePower),       Cm1, 0 },
  { Zint24,   Cx0B01, 0x000E,  Z_(PowerThreshold),       Cm1, 0 },

  // Electrical Measurement cluster
  { Zmap32,   Cx0B04, 0x0000,  Z_(ElectricalMeasurementType),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0100,  Z_(DCVoltage),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0101,  Z_(DCVoltageMin),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0102,  Z_(DCVoltageMax),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0103,  Z_(DCCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0104,  Z_(DCCurrentMin),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0105,  Z_(DCCurrentMax),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0106,  Z_(DCPower),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0107,  Z_(DCPowerMin),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0108,  Z_(DCPowerMax),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0200,  Z_(DCVoltageMultiplier),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0201,  Z_(DCVoltageDivisor),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0202,  Z_(DCCurrentMultiplier),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0203,  Z_(DCCurrentDivisor),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0204,  Z_(DCPowerMultiplier),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0205,  Z_(DCPowerDivisor),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0300,  Z_(ACFrequency),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0301,  Z_(ACFrequencyMin),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0302,  Z_(ACFrequencyMax),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0303,  Z_(NeutralCurrent),           Cm1, 0 },
  { Zint32,   Cx0B04, 0x0304,  Z_(TotalActivePower),           Cm1, 0 },
  { Zint32,   Cx0B04, 0x0305,  Z_(TotalReactivePower),           Cm1, 0 },
  { Zuint32,  Cx0B04, 0x0306,  Z_(TotalApparentPower),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0307,  Z_(Measured1stHarmonicCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0308,  Z_(Measured3rdHarmonicCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0309,  Z_(Measured5thHarmonicCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x030A,  Z_(Measured7thHarmonicCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x030B,  Z_(Measured9thHarmonicCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x030C,  Z_(Measured11thHarmonicCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x030D,  Z_(MeasuredPhase1stHarmonicCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x030E,  Z_(MeasuredPhase3rdHarmonicCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x030F,  Z_(MeasuredPhase5thHarmonicCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0310,  Z_(MeasuredPhase7thHarmonicCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0311,  Z_(MeasuredPhase9thHarmonicCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0312,  Z_(MeasuredPhase11thHarmonicCurrent),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0400,  Z_(ACFrequencyMultiplier),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0401,  Z_(ACFrequencyDivisor),           Cm1, 0 },
  { Zuint32,  Cx0B04, 0x0402,  Z_(PowerMultiplier),           Cm1, 0 },
  { Zuint32,  Cx0B04, 0x0403,  Z_(PowerDivisor),           Cm1, 0 },
  { Zint8,    Cx0B04, 0x0404,  Z_(HarmonicCurrentMultiplier),           Cm1, 0 },
  { Zint8,    Cx0B04, 0x0405,  Z_(PhaseHarmonicCurrentMultiplier),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0501,  Z_(LineCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0502,  Z_(ActiveCurrent),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0503,  Z_(ReactiveCurrent),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0505,  Z_(RMSVoltage),            Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Plug, mains_voltage) },
  { Zuint16,  Cx0B04, 0x0506,  Z_(RMSVoltageMin),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0507,  Z_(RMSVoltageMax),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0508,  Z_(RMSCurrent),            Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0509,  Z_(RMSCurrentMin),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x050A,  Z_(RMSCurrentMax),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x050B,  Z_(ActivePower),           Cm1 + Z_EXPORT_DATA, Z_MAPPING(Z_Data_Plug, mains_power) },
  { Zuint16,  Cx0B04, 0x050C,  Z_(ActivePowerMin),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x050D,  Z_(ActivePowerMax),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x050E,  Z_(ReactivePower),         Cm1, 0 },
  { Zint16,   Cx0B04, 0x050F,  Z_(ApparentPower),         Cm1, 0 },
  { Zint8,    Cx0B04, 0x0510,  Z_(PowerFactor),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0511,  Z_(AverageRMSVoltageMeasurementPeriod),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0512,  Z_(AverageRMSOverVoltageCounter),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0513,  Z_(AverageRMSUnderVoltageCounter),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0514,  Z_(RMSExtremeOverVoltagePeriod),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0515,  Z_(RMSExtremeUnderVoltagePeriod),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0516,  Z_(RMSVoltageSagPeriod),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0517,  Z_(RMSVoltageSwellPeriod),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0600,  Z_(ACVoltageMultiplier),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0601,  Z_(ACVoltageDivisor),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0602,  Z_(ACCurrentMultiplier),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0603,  Z_(ACCurrentDivisor),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0604,  Z_(ACPowerMultiplier),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0605,  Z_(ACPowerDivisor),           Cm1, 0 },
  { Zmap8,    Cx0B04, 0x0700,  Z_(DCOverloadAlarmsMask),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0701,  Z_(DCVoltageOverload),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0702,  Z_(DCCurrentOverload),           Cm1, 0 },
  { Zmap16,   Cx0B04, 0x0800,  Z_(ACAlarmsMask),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0801,  Z_(ACVoltageOverload),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0802,  Z_(ACCurrentOverload),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0803,  Z_(ACActivePowerOverload),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0804,  Z_(ACReactivePowerOverload),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0805,  Z_(AverageRMSOverVoltage),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0806,  Z_(AverageRMSUnderVoltage),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0807,  Z_(RMSExtremeOverVoltage),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0808,  Z_(RMSExtremeUnderVoltage),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0809,  Z_(RMSVoltageSag),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x080A,  Z_(RMSVoltageSwell),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0901,  Z_(LineCurrentPhB),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0902,  Z_(ActiveCurrentPhB),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0903,  Z_(ReactiveCurrentPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0905,  Z_(RMSVoltagePhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0906,  Z_(RMSVoltageMinPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0907,  Z_(RMSVoltageMaxPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0908,  Z_(RMSCurrentPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0909,  Z_(RMSCurrentMinPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x090A,  Z_(RMSCurrentMaxPhB),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x090B,  Z_(ActivePowerPhB),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x090C,  Z_(ActivePowerMinPhB),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x090D,  Z_(ActivePowerMaxPhB),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x090E,  Z_(ReactivePowerPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x090F,  Z_(ApparentPowerPhB),           Cm1, 0 },
  { Zint8,    Cx0B04, 0x0910,  Z_(PowerFactorPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0911,  Z_(AverageRMSVoltageMeasurementPeriodPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0912,  Z_(AverageRMSOverVoltageCounterPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0913,  Z_(AverageRMSUnderVoltageCounterPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0914,  Z_(RMSExtremeOverVoltagePeriodPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0915,  Z_(RMSExtremeUnderVoltagePeriodPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0916,  Z_(RMSVoltageSagPeriodPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0917,  Z_(RMSVoltageSwellPeriodPhB),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A01,  Z_(LineCurrentPhC),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0A02,  Z_(ActiveCurrentPhC),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0A03,  Z_(ReactiveCurrentPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A05,  Z_(RMSVoltagePhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A06,  Z_(RMSVoltageMinPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A07,  Z_(RMSVoltageMaxPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A08,  Z_(RMSCurrentPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A09,  Z_(RMSCurrentMinPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A0A,  Z_(RMSCurrentMaxPhC),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0A0B,  Z_(ActivePowerPhC),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0A0C,  Z_(ActivePowerMinPhC),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0A0D,  Z_(ActivePowerMaxPhC),           Cm1, 0 },
  { Zint16,   Cx0B04, 0x0A0E,  Z_(ReactivePowerPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A0F,  Z_(ApparentPowerPhC),           Cm1, 0 },
  { Zint8,    Cx0B04, 0x0A10,  Z_(PowerFactorPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A11,  Z_(AverageRMSVoltageMeasurementPeriodPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A12,  Z_(AverageRMSOverVoltageCounterPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A13,  Z_(AverageRMSUnderVoltageCounterPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A14,  Z_(RMSExtremeOverVoltagePeriodPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A15,  Z_(RMSExtremeUnderVoltagePeriodPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A16,  Z_(RMSVoltageSagPeriodPhC),           Cm1, 0 },
  { Zuint16,  Cx0B04, 0x0A17,  Z_(RMSVoltageSwellPeriodPhC),           Cm1, 0 },

  // Diagnostics cluster
  { Zuint16,  Cx0B05, 0x0000,  Z_(NumberOfResets),       Cm1, 0 },
  { Zuint16,  Cx0B05, 0x0001,  Z_(PersistentMemoryWrites),Cm1, 0 },
  { Zuint8,   Cx0B05, 0x011C,  Z_(LastMessageLQI),       Cm1, 0 },
  { Zuint8,   Cx0B05, 0x011D,  Z_(LastMessageRSSI),      Cm1, 0 },

  // Tuya Moes specific - 0xEF00
  // Mapping of Tuya type with internal mapping
  // 0x00 - Zoctstr (len N)
  // 0x01 - Ztuya1 (len 1) - equivalent to Zuint8 without invalid value handling
  // 0x02 - Ztuya4 (len 4) - equivalent to Zint32 in big endian and without invalid value handling
  // 0x03 - Zstr (len N)
  // 0x04 - Ztuya1 (len 1)
  // 0x05 - Ztuya4u (len 1/2/4) - equivalent to Zuint32
  // { Ztuya0,   CxEF00, 0x0070,  Z_(TuyaScheduleWorkdays), Cm1, 0 },
  // { Ztuya0,   CxEF00, 0x0071,  Z_(TuyaScheduleHolidays), Cm1, 0 },
  // { Ztuya1,   CxEF00, 0x0101,  Z_(Power),                Cm1, 0 },
  // { Ztuya1,   CxEF00, 0x0102,  Z_(Power2),               Cm1, 0 },
  // { Ztuya1,   CxEF00, 0x0103,  Z_(Power3),               Cm1, 0 },
  // { Ztuya1,   CxEF00, 0x0104,  Z_(Power4),               Cm1, 0 },
  // { Ztuya1,   CxEF00, 0x0107,  Z_(TuyaChildLock),        Cm1, 0 },
  // { Ztuya1,   CxEF00, 0x0112,  Z_(TuyaWindowDetection),  Cm1, 0 },
  // { Ztuya1,   CxEF00, 0x0114,  Z_(TuyaValveDetection),   Cm1, 0 },
  // { Ztuya1,   CxEF00, 0x0174,  Z_(TuyaAutoLock),         Cm1, 0 },
  // { Zint16,   CxEF00, 0x0202,  Z_(TuyaTempTarget),       Cm_10, Z_MAPPING(Z_Data_Thermo, temperature_target) },
  // { Zint16,   CxEF00, 0x0203,  Z_(LocalTemperature),     Cm_10, Z_MAPPING(Z_Data_Thermo, temperature) },  // will be overwritten by actual LocalTemperature
  // { Zuint8,   CxEF00, 0x0203,  Z_(Dimmer),               Cm1, Z_MAPPING(Z_Data_Light, dimmer) },  // will be overwritten by actual LocalTemperature
  // { Zmap8,    CxEF00, 0x0203,  Z_(Occupancy),            Cm1, Z_MAPPING(Z_Data_PIR, occupancy) },  // will be overwritten by actual LocalTemperature
  // { Ztuya2,   CxEF00, 0x0215,  Z_(TuyaBattery),          Cm1, 0 },   // TODO check equivalent?
  // { Ztuya2,   CxEF00, 0x0266,  Z_(TuyaMinTemp),          Cm1, 0 },
  // { Ztuya2,   CxEF00, 0x0267,  Z_(TuyaMaxTemp),          Cm1, 0 },
  // { Ztuya2,   CxEF00, 0x0269,  Z_(TuyaBoostTime),        Cm1, 0 },
  // { Ztuya2,   CxEF00, 0x026B,  Z_(TuyaComfortTemp),      Cm1, 0 },
  // { Ztuya2,   CxEF00, 0x026C,  Z_(TuyaEcoTemp),          Cm1, 0 },
  // { Zuint8,   CxEF00, 0x026D,  Z_(TuyaValvePosition),    Cm1, Z_MAPPING(Z_Data_Thermo, th_setpoint) },
  // { Ztuya2,   CxEF00, 0x0272,  Z_(TuyaAwayTemp),         Cm1, 0 },
  // { Ztuya2,   CxEF00, 0x0275,  Z_(TuyaAwayDays),         Cm1, 0 },
  // { Ztuya4,   CxEF00, 0x0404,  Z_(TuyaPreset),           Cm1, 0 },
  // { Ztuya4,   CxEF00, 0x0405,  Z_(TuyaFanMode),          Cm1, 0 },
  // { Ztuya4,   CxEF00, 0x046A,  Z_(TuyaForceMode),        Cm1, 0 },
  // { Ztuya4,   CxEF00, 0x046F,  Z_(TuyaWeekSelect),       Cm1, 0 },

  // Legrand BTicino - Manuf code 0x1021
  { Zdata16,  CxFC01, 0x0000,  Z_(LegrandOpt1),          Cm1, 0 },
  { Zbool,    CxFC01, 0x0001,  Z_(LegrandOpt2),          Cm1, 0 },
  { Zbool,    CxFC01, 0x0002,  Z_(LegrandOpt3),          Cm1, 0 },

  // Legrand - Manuf code 0x1021
  { Zenum8,   CxFC40, 0x0000,  Z_(LegrandHeatingMode),   Cm1, 0 },

  // Aqara Opple spacific
  { Zuint8,   CxFCC0, 0x0009,  Z_(OppleMode),            Cm1, 0 },

  // Terncy specific - 0xFCCC
  { Zuint16, CxFCCC, 0x001A,  Z_(TerncyDuration),        Cm1, 0 },
  { Zint16,  CxFCCC, 0x001B,  Z_(TerncyRotate),          Cm1, 0 },
};
#pragma GCC diagnostic pop

#endif // USE_ZIGBEE
