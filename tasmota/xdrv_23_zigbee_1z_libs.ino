/*
  xdrv_23_zigbee_1z_libs.ino - zigbee support for Tasmota, JSON replacement libs

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
 * Replacement libs for JSON to output a list of attributes
\*********************************************************************************************/

// simplified version of strcmp accepting both arguments to be in PMEM, and accepting nullptr arguments
// inspired from https://code.woboq.org/userspace/glibc/string/strcmp.c.html
int strcmp_PP(const char *p1, const char *p2) {
  if (p1 == p2) { return 0; }         // equality
  if (!p1)      { return -1; }        // first string is null
  if (!p2)      { return 1; }         // second string is null
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do {
    c1 = (unsigned char) pgm_read_byte(s1);
    s1++;
    c2 = (unsigned char) pgm_read_byte(s2);
    s2++;
    if (c1 == '\0')
      return c1 - c2;
  }
  while (c1 == c2);
  return c1 - c2;
}

/*********************************************************************************************\
 *
 * Variables for Rules from last Zigbee message received
 *
\*********************************************************************************************/

typedef struct Z_LastMessageVars {
  uint16_t    device;               // device short address
  uint16_t    groupaddr;            // group address
  uint16_t    cluster;              // cluster id
  uint8_t     endpoint;             // source endpoint
} Z_LastMessageVars;

Z_LastMessageVars gZbLastMessage;

uint16_t Z_GetLastDevice(void) { return gZbLastMessage.device; }
uint16_t Z_GetLastGroup(void) { return gZbLastMessage.groupaddr; }
uint16_t Z_GetLastCluster(void) { return gZbLastMessage.cluster; }
uint8_t  Z_GetLastEndpoint(void) { return gZbLastMessage.endpoint; }

/*********************************************************************************************\
 *
 * Class for single attribute
 *
\*********************************************************************************************/

enum class Za_type : uint8_t {
  Za_none,     // empty, translates into null in JSON
  // numericals
  Za_bool,     // boolean, translates to true/false, uses uval32 to store
  Za_uint,     // unsigned 32 int, uses uval32
  Za_int,      // signed 32 int, uses ival32
  Za_float,    // float 32, uses fval
  // non-nummericals
  Za_raw,      // bytes buffer, uses bval
  Za_str,      // string, uses sval
  // sub_objects
  Za_obj,      // json sub-object
  Za_arr,      // array sub-object (string add-only)
};

class Z_attribute {
public:

  // attribute key, either cluster+attribute_id or plain name
  union {
    struct {
      uint16_t cluster;
      uint16_t attr_id;
    }                   id;
    char              * key;
  } key;
  // attribute value
  union {
    uint32_t            uval32;
    int32_t             ival32;
    float               fval;
    SBuffer*            bval;
    char*               sval;
    class Z_attribute_list   * objval;
    class JsonGeneratorArray * arrval;
  } val;
  Za_type       type;             // uint8_t in size, type of attribute, see above
  bool          key_is_str;       // is the key a string?
  bool          key_is_pmem;      // is the string in progmem, so we don't need to make a copy
  bool          val_str_raw;      // if val is String, it is raw JSON and should not be escaped
  uint8_t       key_suffix;       // append a suffix to key (default is 1, explicitly output if >1)
  uint8_t       attr_type;        // [opt] type of the attribute, default to Zunk (0xFF)
  uint8_t       attr_multiplier;  // [opt] multiplier for attribute, defaults to 0x01 (no change)

  // Constructor with all defaults
  Z_attribute():
    key{ .id = { 0x0000, 0x0000 } },
    val{ .uval32 = 0x0000 },
    type(Za_type::Za_none),
    key_is_str(false),
    key_is_pmem(false),
    val_str_raw(false),
    key_suffix(1),
    attr_type(0xFF),
    attr_multiplier(1)
    {};

  Z_attribute(const Z_attribute & rhs) {
    deepCopy(rhs);
  }

  Z_attribute & operator = (const Z_attribute & rhs) {
    freeKey();
    freeVal();
    deepCopy(rhs);
    return *this;
  }

  // Destructor, free memory that was allocated
  ~Z_attribute() {
    freeKey();
    freeVal();
  }

  // free any allocated memoruy for values
  void freeVal(void);

  // free any allocated memoruy for keys
  void freeKey(void);

  // set key name
  void setKeyName(const char * _key, bool pmem = false);
  // provide two entries and concat
  void setKeyName(const char * _key, const char * _key2);

  void setKeyId(uint16_t cluster, uint16_t attr_id);

  // Setters
  void setNone(void);
  void setUInt(uint32_t _val);
  void setBool(bool _val);
  void setInt(int32_t _val);
  void setFloat(float _val);

  void setBuf(const SBuffer &buf, size_t index, size_t len);

  // specific formatters
  void setHex32(uint32_t _val);
  void setHex64(uint64_t _val);

  // set the string value
  // PMEM argument is allowed
  // string will be copied, so it can be changed later
  // nullptr is allowed and considered as empty string
  // Note: memory is allocated only if string is non-empty
  void setStr(const char * _val);
  inline void setStrRaw(const char * _val) {
    setStr(_val);
    val_str_raw = true;
  }

  Z_attribute_list & newAttrList(void);
  JsonGeneratorArray & newJsonArray(void);

  inline bool isNum(void) const { return (type >= Za_type::Za_bool) && (type <= Za_type::Za_float); }
  inline bool isNone(void) const { return (type == Za_type::Za_none);}
  // get num values
  float getFloat(void) const;
  int32_t getInt(void) const;
  uint32_t getUInt(void) const;
  bool getBool(void) const;
  const SBuffer * getRaw(void) const;

  // always return a point to a string, if not defined then empty string.
  // Never returns nullptr
  const char * getStr(void) const;

  bool equalsKey(const Z_attribute & attr2, bool ignore_key_suffix = false) const;
  bool equalsKey(uint16_t cluster, uint16_t attr_id, uint8_t suffix = 0) const;
  bool equalsKey(const char * name, uint8_t suffix = 0) const;
  bool equalsVal(const Z_attribute & attr2) const;
  bool equals(const Z_attribute & attr2) const;

  String toString(bool prefix_comma = false) const;

  // copy value from one attribute to another, without changing its type
  void copyVal(const Z_attribute & rhs);

protected:
  void deepCopy(const Z_attribute & rhs);
};

/*********************************************************************************************\
 *
 * Class for attribute ordered list
 *
\*********************************************************************************************/


// Attribute list
// Contains meta-information:
// - source endpoint (is conflicting)
// - LQI (if not conflicting)
class Z_attribute_list : public LList<Z_attribute> {
public:
  uint8_t       src_ep;   // source endpoint, 0xFF if unknown
  uint8_t       lqi;      // linkquality, 0xFF if unknown
  uint16_t      group_id; // group address OxFFFF if inknown

  Z_attribute_list():
    LList<Z_attribute>(), // call superclass constructor
    src_ep(0xFF),
    lqi(0xFF),
    group_id(0xFFFF)
    {};

  // we don't define any destructor, the superclass destructor is automatically called

  // reset object to its initial state
  // free all allocated memory
  void reset(void) {
    LList<Z_attribute>::reset();
    src_ep = 0xFF;
    lqi = 0xFF;
    group_id = 0xFFFF;
  }

  inline bool isValidSrcEp(void) const { return 0xFF != src_ep; }
  inline bool isValidLQI(void) const { return 0xFF != lqi; }
  inline bool isValidGroupId(void) const { return 0xFFFF != group_id; }

  // the following addAttribute() compute the suffix and increments it
  // Add attribute to the list, given cluster and attribute id
  Z_attribute & addAttribute(uint16_t cluster, uint16_t attr_id, uint8_t suffix = 0);

  // Add attribute to the list, given name
  Z_attribute & addAttribute(const char * name, bool pmem = false, uint8_t suffix = 0);
  Z_attribute & addAttribute(const char * name, const char * name2, uint8_t suffix = 0);
  inline Z_attribute & addAttribute(const __FlashStringHelper * name, uint8_t suffix = 0) {
    return addAttribute((const char*) name, true, suffix);
  }
  // smaller version called often to reduce code size
  Z_attribute & addAttributePMEM(const char * name);

  // Remove from list by reference, if null or not found, then do nothing
  inline void removeAttribute(const Z_attribute * attr) { remove(attr); }

  // dump the entire structure as JSON, starting from head (as parameter)
  // does not start not end with a comma
  // do we enclosed in brackets '{' '}'
  String toString(bool enclose_brackets = false) const;

  // find if attribute with same key already exists, return null if not found
  const Z_attribute * findAttribute(uint16_t cluster, uint16_t attr_id, uint8_t suffix = 0) const;
  const Z_attribute * findAttribute(const char * name, uint8_t suffix = 0) const;
  const Z_attribute * findAttribute(const Z_attribute &attr) const;   // suffis always count here
  // non-const variants
  inline Z_attribute * findAttribute(uint16_t cluster, uint16_t attr_id, uint8_t suffix = 0) {
    return (Z_attribute*) ((const Z_attribute_list*)this)->findAttribute(cluster, attr_id, suffix);
  }
  inline Z_attribute * findAttribute(const char * name, uint8_t suffix = 0) {
    return (Z_attribute*) (((const Z_attribute_list*)this)->findAttribute(name, suffix));
  }
  inline Z_attribute * findAttribute(const Z_attribute &attr) {
    return (Z_attribute*) ((const Z_attribute_list*)this)->findAttribute(attr);
  }

  // count matching attributes, ignoring suffix
  size_t countAttribute(uint16_t cluster, uint16_t attr_id) const ;
  size_t countAttribute(const char * name) const ;

  // if suffix == 0, we don't care and find the first match
  Z_attribute & findOrCreateAttribute(uint16_t cluster, uint16_t attr_id, uint8_t suffix = 0);
  Z_attribute & findOrCreateAttribute(const char * name, uint8_t suffix = 0);
  // always care about suffix
  Z_attribute & findOrCreateAttribute(const Z_attribute &attr);
  // replace attribute with new value, suffix does care
  Z_attribute & replaceOrCreate(const Z_attribute &attr);

  // merge with secondary list, return true if ok, false if conflict
  bool mergeList(const Z_attribute_list &list2);
};


Z_attribute & Z_attribute_list::addAttributePMEM(const char * name) {
  return addAttribute(name, true, 0);
}

/*********************************************************************************************\
 *
 * Implementation for Z_attribute
 *
\*********************************************************************************************/

// free any allocated memoruy for keys
void Z_attribute::freeKey(void) {
  if (key_is_str && key.key && !key_is_pmem) { delete[] key.key; }
  key.key = nullptr;
}

// set key name
void Z_attribute::setKeyName(const char * _key, bool pmem) {
  freeKey();
  key_is_str = true;
  key_is_pmem = pmem;
  if (pmem) {
    key.key = (char*) _key;
  } else {
    setKeyName(_key, nullptr);
  }
}
// provide two entries and concat
void Z_attribute::setKeyName(const char * _key, const char * _key2) {
  freeKey();
  key_is_str = true;
  key_is_pmem = false;
  if (_key) {
    size_t key_len = strlen_P(_key);
    if (_key2) {
      key_len += strlen_P(_key2);
    }
    key.key = new char[key_len+1];
    strcpy_P(key.key, _key);
    if (_key2) {
      strcat_P(key.key, _key2);
    }
  }
}

void Z_attribute::setKeyId(uint16_t cluster, uint16_t attr_id) {
  freeKey();
  key_is_str = false;
  key.id.cluster = cluster;
  key.id.attr_id = attr_id;
}

// Setters
void Z_attribute::setNone(void) {
  freeVal();     // free any previously allocated memory
  val.uval32 = 0;
  type = Za_type::Za_none;
}
void Z_attribute::setUInt(uint32_t _val) {
  freeVal();     // free any previously allocated memory
  val.uval32 = _val;
  type = Za_type::Za_uint;
}
void Z_attribute::setBool(bool _val) {
  freeVal();     // free any previously allocated memory
  val.uval32 = _val;
  type = Za_type::Za_bool;
}
void Z_attribute::setInt(int32_t _val) {
  freeVal();     // free any previously allocated memory
  val.ival32 = _val;
  type = Za_type::Za_int;
}
void Z_attribute::setFloat(float _val) {
  freeVal();     // free any previously allocated memory
  val.fval = _val;
  type = Za_type::Za_float;
}

void Z_attribute::setBuf(const SBuffer &buf, size_t index, size_t len) {
  freeVal();
  if (len) {
    val.bval = new SBuffer(len);
    val.bval->addBuffer(buf.buf(index), len);
  }
  type = Za_type::Za_raw;
}

void Z_attribute::setHex32(uint32_t _val) {
  char hex[8];
  snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), _val);
  setStr(hex);
}
void Z_attribute::setHex64(uint64_t _val) {
  char hex[22];
  ext_snprintf_P(hex, sizeof(hex), PSTR("0x%_X"), &_val);
  setStr(hex);
}

// set the string value
// PMEM argument is allowed
// string will be copied, so it can be changed later
// nullptr is allowed and considered as empty string
// Note: memory is allocated only if string is non-empty
void Z_attribute::setStr(const char * _val) {
  freeVal();     // free any previously allocated memory
  val_str_raw = false;
  // val.sval is always nullptr after freeVal()
  if (_val) {
    size_t len = strlen_P(_val);
    if (len) {
      val.sval = new char[len+1];
      strcpy_P(val.sval, _val);
    }
  }
  type = Za_type::Za_str;
}

Z_attribute_list & Z_attribute::newAttrList(void) {
  freeVal();
  val.objval = new Z_attribute_list();
  type = Za_type::Za_obj;
  return *val.objval;
}

JsonGeneratorArray & Z_attribute::newJsonArray(void) {
  freeVal();
  val.arrval = new JsonGeneratorArray();
  type = Za_type::Za_arr;
  return *val.arrval;
}

// get num values
float Z_attribute::getFloat(void) const {
  switch (type) {
    case Za_type::Za_bool:
    case Za_type::Za_uint:    return (float) val.uval32;
    case Za_type::Za_int:     return (float) val.ival32;
    case Za_type::Za_float:   return val.fval;
    default:                  return 0.0f;
  }
}

int32_t Z_attribute::getInt(void) const {
  switch (type) {
    case Za_type::Za_bool:
    case Za_type::Za_uint:    return (int32_t) val.uval32;
    case Za_type::Za_int:     return val.ival32;
    case Za_type::Za_float:   return (int32_t) val.fval;
    default:                  return 0;
  }
}

uint32_t Z_attribute::getUInt(void) const {
  switch (type) {
    case Za_type::Za_bool:
    case Za_type::Za_uint:    return val.uval32;
    case Za_type::Za_int:     return (uint32_t) val.ival32;
    case Za_type::Za_float:   return (uint32_t) val.fval;
    default:                  return 0;
  }
}

bool Z_attribute::getBool(void) const {
  switch (type) {
    case Za_type::Za_bool:
    case Za_type::Za_uint:    return val.uval32 ? true : false;
    case Za_type::Za_int:     return val.ival32 ? true : false;
    case Za_type::Za_float:   return val.fval ? true : false;
    default:                  return false;
  }
}

const SBuffer * Z_attribute::getRaw(void) const {
  if (Za_type::Za_raw == type) { return val.bval; }
  return nullptr;
}

// always return a point to a string, if not defined then empty string.
// Never returns nullptr
const char * Z_attribute::getStr(void) const {
  if (Za_type::Za_str == type) { return val.sval; }
  return "";
}

bool Z_attribute::equalsKey(const Z_attribute & attr2, bool ignore_key_suffix) const {
  // check if keys are equal
  if (key_is_str != attr2.key_is_str) { return false; }
  if (key_is_str) {
    if (strcmp_PP(key.key, attr2.key.key)) { return false; }
  } else {
    if ((key.id.cluster != attr2.key.id.cluster) ||
        (key.id.attr_id != attr2.key.id.attr_id)) { return false; }
  }
  if (!ignore_key_suffix) {
    if (key_suffix != attr2.key_suffix) { return false; }
  }
  return true;
}

bool Z_attribute::equalsKey(uint16_t cluster, uint16_t attr_id, uint8_t suffix) const {
  if (!key_is_str) {
    if ((key.id.cluster == cluster) && (key.id.attr_id == attr_id)) {
      if (suffix) {
        if (key_suffix == suffix) { return true; }
      } else {
        return true;
      }
    }
  }
  return false;
}

bool Z_attribute::equalsKey(const char * name, uint8_t suffix) const {
  if (key_is_str) {
    if (0 == strcmp_PP(key.key, name)) {
      if (suffix) {
        if (key_suffix == suffix) { return true; }
      } else {
        return true;
      }
    }
  }
  return false;
}

bool Z_attribute::equalsVal(const Z_attribute & attr2) const {
  if (type != attr2.type) { return false; }
  if ((type >= Za_type::Za_bool) && (type <= Za_type::Za_float)) {
    // numerical value
    if (val.uval32 != attr2.val.uval32) { return false; }
  } else if (type == Za_type::Za_raw) {
    // compare 2 Static buffers
    return SBuffer::equalsSBuffer(val.bval, attr2.val.bval);
  } else if (type == Za_type::Za_str) {
    // if (val_str_raw != attr2.val_str_raw) { return false; }
    if (strcmp_PP(val.sval, attr2.val.sval)) { return false; }
  } else if (type == Za_type::Za_obj) {
    return false;   // TODO for now we'll assume sub-objects are always different
  } else if (type == Za_type::Za_arr) {
    return false;   // TODO for now we'll assume sub-objects are always different
  }
  return true;
}

bool Z_attribute::equals(const Z_attribute & attr2) const {
  return equalsKey(attr2) && equalsVal(attr2);
}

String Z_attribute::toString(bool prefix_comma) const {
  String res("");
  if (prefix_comma) { res += ','; }
  res += '"';
  // compute the attribute name
  if (key_is_str) {
    if (key.key) { res += EscapeJSONString(key.key); }
    else         { res += F("null"); }   // shouldn't happen
    if (key_suffix > 1) {
      res += key_suffix;
    }
  } else {
    char attr_name[12];
    snprintf_P(attr_name, sizeof(attr_name), PSTR("%04X/%04X"), key.id.cluster, key.id.attr_id);
    res += attr_name;
    if (key_suffix > 1) {
      res += '+';
      res += key_suffix;
    }
  }
  res += F("\":");
  // value part
  switch (type) {
  case Za_type::Za_none:
    res += F("null");
    break;
  case Za_type::Za_bool:
    res += val.uval32 ? F("true") : F("false");
    break;
  case Za_type::Za_uint:
    res += val.uval32;
    break;
  case Za_type::Za_int:
    res += val.ival32;
    break;
  case Za_type::Za_float:
    {
      String fstr(val.fval, 2);
      size_t last = fstr.length() - 1;
      // remove trailing zeros
      while (fstr[last] == '0') {
        fstr.remove(last--);
      }
      // remove trailing dot
      if (fstr[last] == '.') {
        fstr.remove(last);
      }
      res += fstr;
    }
    break;
  case Za_type::Za_raw:
    res += '"';
    if (val.bval) {
      size_t blen = val.bval->len();
      // print as HEX
      char hex[2*blen+1];
      ToHex_P(val.bval->getBuffer(), blen, hex, sizeof(hex));
      res += hex;
    }
    res += '"';
    break;
  case Za_type::Za_str:
    if (val_str_raw) {
      if (val.sval) { res += val.sval; }
    } else {
      res += '"';
      if (val.sval) {
        res += EscapeJSONString(val.sval);    // escape JSON chars
      }
      res += '"';
    }
    break;
  case Za_type::Za_obj:
    res += '{';
    if (val.objval) {
      res += val.objval->toString();
    }
    res += '}';
    break;
  case Za_type::Za_arr:
    if (val.arrval) {
      res += val.arrval->toString();
    } else {
      // res += '[';
      // res += ']';
      res += F("[]");
    }
    break;
  }

  return res;
}

// copy value from one attribute to another, without changing its type
void Z_attribute::copyVal(const Z_attribute & rhs) {
  freeVal();
  // copy value
  val.uval32 = 0x00000000;
  type = rhs.type;
  if (rhs.isNum()) {
    val.uval32 = rhs.val.uval32;
  } else if (rhs.type == Za_type::Za_raw) {
    if (rhs.val.bval) {
      val.bval = new SBuffer(rhs.val.bval->len());
      val.bval->addBuffer(*(rhs.val.bval));
    }
  } else if (rhs.type == Za_type::Za_str) {
    if (rhs.val.sval) {
      size_t s_len = strlen_P(rhs.val.sval);
      val.sval = new char[s_len+1];
      strcpy_P(val.sval, rhs.val.sval);
    }
  }
  val_str_raw = rhs.val_str_raw;
}

// free any allocated memoruy for values
void Z_attribute::freeVal(void) {
  switch (type) {
    case Za_type::Za_raw:
      if (val.bval) { delete val.bval; val.bval = nullptr; }
      break;
    case Za_type::Za_str:
      if (val.sval) { delete[] val.sval; val.sval = nullptr; }
      break;
    case Za_type::Za_obj:
      if (val.objval) { delete val.objval; val.objval = nullptr; }
      break;
    case Za_type::Za_arr:
      if (val.arrval) { delete val.arrval; val.arrval = nullptr; }
      break;
    default:
      break;
  }
}

void Z_attribute::deepCopy(const Z_attribute & rhs) {
  // copy key
  if (!rhs.key_is_str) {
    key.id.cluster = rhs.key.id.cluster;
    key.id.attr_id = rhs.key.id.attr_id;
  } else {
    if (rhs.key_is_pmem) {
      key.key = rhs.key.key;      // PMEM, don't copy
    } else {
      key.key = nullptr;
      if (rhs.key.key) {
        size_t key_len = strlen_P(rhs.key.key);
        if (key_len) {
          key.key = new char[key_len+1];
          strcpy_P(key.key, rhs.key.key);
        }
      }
    }
  }
  key_is_str = rhs.key_is_str;
  key_is_pmem = rhs.key_is_pmem;
  key_suffix = rhs.key_suffix;
  attr_type = rhs.attr_type;
  attr_multiplier = rhs.attr_multiplier;
  // copy value
  copyVal(rhs);
  // don't touch next pointer
}

/*********************************************************************************************\
 *
 * Implementation for Z_attribute_list
 *
\*********************************************************************************************/
// add a cluster/attr_id attribute at the end of the list
Z_attribute & Z_attribute_list::addAttribute(uint16_t cluster, uint16_t attr_id, uint8_t suffix) {
  Z_attribute & attr = addToLast();
  attr.key.id.cluster = cluster;
  attr.key.id.attr_id = attr_id;
  attr.key_is_str = false;
  if (!suffix) { attr.key_suffix = countAttribute(attr.key.id.cluster, attr.key.id.attr_id); }
  else { attr.key_suffix = suffix; }
  return attr;
}

// add a cluster/attr_id attribute at the end of the list
Z_attribute & Z_attribute_list::addAttribute(const char * name, bool pmem, uint8_t suffix) {
  Z_attribute & attr = addToLast();
  attr.setKeyName(name, pmem);
  if (!suffix) { attr.key_suffix = countAttribute(attr.key.key); }
  else { attr.key_suffix = suffix; }
  return attr;
}

Z_attribute & Z_attribute_list::addAttribute(const char * name, const char * name2, uint8_t suffix) {
  Z_attribute & attr = addToLast();
  attr.setKeyName(name, name2);
  if (!suffix) { attr.key_suffix = countAttribute(attr.key.key); }
  else { attr.key_suffix = suffix; }
  return attr;
}

String Z_attribute_list::toString(bool enclose_brackets) const {
  String res = "";
  if (enclose_brackets) { res += '{'; }
  bool prefix_comma = false;
  for (const auto & attr : *this) {
    res += attr.toString(prefix_comma);
    prefix_comma = true;
  }
  // add source endpoint
  if (0xFF != src_ep) {
    if (prefix_comma) { res += ','; }
    prefix_comma = true;
    res += F("\"" D_CMND_ZIGBEE_ENDPOINT "\":");
    res += src_ep;
  }
  // add group address
  if (0xFFFF != group_id) {
    if (prefix_comma) { res += ','; }
    prefix_comma = true;
    res += F("\"" D_CMND_ZIGBEE_GROUP "\":");
    res += group_id;
  }
  // add lqi
  if (0xFF != lqi) {
    if (prefix_comma) { res += ','; }
    prefix_comma = true;
    res += F("\"" D_CMND_ZIGBEE_LINKQUALITY "\":");
    res += lqi;
  }
  if (enclose_brackets) { res += '}'; }
  // done
  return res;
}

// suffis always count here
const Z_attribute * Z_attribute_list::findAttribute(const Z_attribute &attr) const {
  uint8_t  suffix = attr.key_suffix;
  if (attr.key_is_str) {
    return findAttribute(attr.key.key, suffix);
  } else {
    return findAttribute(attr.key.id.cluster, attr.key.id.attr_id, suffix);
  }
}

const Z_attribute * Z_attribute_list::findAttribute(uint16_t cluster, uint16_t attr_id, uint8_t suffix) const {
  for (const auto & attr : *this) {
    if (attr.equalsKey(cluster, attr_id, suffix)) { return &attr; }
  }
  return nullptr;
}
size_t Z_attribute_list::countAttribute(uint16_t cluster, uint16_t attr_id) const {
  size_t count = 0;
  for (const auto & attr : *this) {
    if (attr.equalsKey(cluster, attr_id, 0)) { count++; }
  }
  return count;
}

// return the existing attribute or create a new one
Z_attribute & Z_attribute_list::findOrCreateAttribute(uint16_t cluster, uint16_t attr_id, uint8_t suffix) {
  Z_attribute * found = findAttribute(cluster, attr_id, suffix);
  return found ? *found : addAttribute(cluster, attr_id, suffix);
}

const Z_attribute * Z_attribute_list::findAttribute(const char * name, uint8_t suffix) const {
  for (const auto & attr : *this) {
    if (attr.equalsKey(name, suffix)) { return &attr; }
  }
  return nullptr;
}
size_t Z_attribute_list::countAttribute(const char * name) const {
  size_t count = 0;
  for (const auto & attr : *this) {
    if (attr.equalsKey(name, 0)) { count++; }
  }
  return count;
}
// return the existing attribute or create a new one
Z_attribute & Z_attribute_list::findOrCreateAttribute(const char * name, uint8_t suffix) {
  Z_attribute * found = findAttribute(name, suffix);
  return found ? *found : addAttribute(name, suffix);
}

// same but passing a Z_attribute as key
Z_attribute & Z_attribute_list::findOrCreateAttribute(const Z_attribute &attr) {
  Z_attribute & ret = attr.key_is_str ? findOrCreateAttribute(attr.key.key, attr.key_suffix)
                                      : findOrCreateAttribute(attr.key.id.cluster, attr.key.id.attr_id, attr.key_suffix);
  ret.key_suffix = attr.key_suffix;
  return ret;
}
// replace the entire content with new attribute or create
Z_attribute & Z_attribute_list::replaceOrCreate(const Z_attribute &attr) {
  Z_attribute &new_attr = findOrCreateAttribute(attr);
  new_attr.copyVal(attr);
  return new_attr;
}


bool Z_attribute_list::mergeList(const Z_attribute_list &attr_list) {
  // Check source endpoint
  if (0xFF == src_ep) {
    src_ep = attr_list.src_ep;
  } else if (0xFF != attr_list.src_ep) {
    if (src_ep != attr_list.src_ep) { return false; }
  }
  // Check group address
  if (0xFFFF == group_id) {
    group_id = attr_list.group_id;
  } else if (0xFFFF != attr_list.group_id) {
    if (group_id != attr_list.group_id) { return false; }
  }
  // copy LQI
  if (0xFF != attr_list.lqi) {
    lqi = attr_list.lqi;
  }
  // merge attributes
  for (auto & attr : attr_list) {
    replaceOrCreate(attr);
  }
  return true;
}

#endif // USE_ZIGBEE
