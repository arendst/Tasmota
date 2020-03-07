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

#include <vector>
#include <map>

#ifndef ZIGBEE_SAVE_DELAY_SECONDS
#define ZIGBEE_SAVE_DELAY_SECONDS 10;               // wait for 10s before saving Zigbee info
#endif
const uint16_t kZigbeeSaveDelaySeconds = ZIGBEE_SAVE_DELAY_SECONDS;    // wait for x seconds

typedef int32_t (*Z_DeviceTimer)(uint16_t shortaddr, uint16_t cluster, uint16_t endpoint, uint32_t value);

typedef struct Z_Device {
  uint16_t              shortaddr;      // unique key if not null, or unspecified if null
  uint64_t              longaddr;       // 0x00 means unspecified
  uint32_t              firstSeen;      // date when the device was first seen
  uint32_t              lastSeen;       // date when the device was last seen
  String                manufacturerId;
  String                modelId;
  String                friendlyName;
  std::vector<uint32_t> endpoints;      // encoded as high 16 bits is endpoint, low 16 bits is ProfileId
  std::vector<uint32_t> clusters_in;    // encoded as high 16 bits is endpoint, low 16 bits is cluster number
  std::vector<uint32_t> clusters_out;   // encoded as high 16 bits is endpoint, low 16 bits is cluster number
  // below are per device timers, used for example to query the new state of the device
  uint32_t              timer;          // millis() when to fire the timer, 0 if no timer
  uint16_t              cluster;        // cluster to use for the timer
  uint16_t              endpoint;       // endpoint to use for timer
  uint32_t              value;          // any raw value to use for the timer
  Z_DeviceTimer         func;           // function to call when timer occurs
  // json buffer used for attribute reporting
  DynamicJsonBuffer    *json_buffer;
  JsonObject           *json;
  // sequence number for Zigbee frames
  uint8_t              seqNumber;
} Z_Device;

// All devices are stored in a Vector
// Invariants:
// - shortaddr is unique if not null
// - longaddr is unique if not null
// - shortaddr and longaddr cannot be both null
// - clusters_in and clusters_out containt only endpoints listed in endpoints
class Z_Devices {
public:
  Z_Devices() {};

  // Probe the existence of device keys
  // Results:
  // - 0x0000 = not found
  // - 0xFFFF = bad parameter
  // - 0x<shortaddr> = the device's short address
  uint16_t isKnownShortAddr(uint16_t shortaddr) const;
  uint16_t isKnownLongAddr(uint64_t  longaddr) const;
  uint16_t isKnownIndex(uint32_t index) const;
  uint16_t isKnownFriendlyName(const char * name) const;

  uint64_t getDeviceLongAddr(uint16_t shortaddr) const;

  // Add new device, provide ShortAddr and optional longAddr
  // If it is already registered, update information, otherwise create the entry
  void updateDevice(uint16_t shortaddr, uint64_t longaddr = 0);

  // Add an endpoint to a device
  void addEndoint(uint16_t shortaddr, uint8_t endpoint);

  // Add endpoint profile
  void addEndointProfile(uint16_t shortaddr, uint8_t endpoint, uint16_t profileId);

  // Add cluster
  void addCluster(uint16_t shortaddr, uint8_t endpoint, uint16_t cluster, bool out);

  uint8_t findClusterEndpointIn(uint16_t shortaddr, uint16_t cluster);

  void setManufId(uint16_t shortaddr, const char * str);
  void setModelId(uint16_t shortaddr, const char * str);
  void setFriendlyName(uint16_t shortaddr, const char * str);
  const String * getFriendlyName(uint16_t shortaddr) const;
  const String * getModelId(uint16_t shortaddr) const;

  // device just seen on the network, update the lastSeen field
  void updateLastSeen(uint16_t shortaddr);

  // get next sequence number for (increment at each all)
  uint8_t getNextSeqNumber(uint16_t shortaddr);

  // Dump json
  String dump(uint32_t dump_mode, uint16_t status_shortaddr = 0) const;

  // Timers
  void resetTimer(uint32_t shortaddr);
  void setTimer(uint32_t shortaddr, uint32_t wait_ms, uint16_t cluster, uint16_t endpoint, uint32_t value, Z_DeviceTimer func);
  void runTimer(void);

  // Append or clear attributes Json structure
  void jsonClear(uint16_t shortaddr);
  void jsonAppend(uint16_t shortaddr, const JsonObject &values);
  const JsonObject *jsonGet(uint16_t shortaddr);
  void jsonPublishFlush(uint16_t shortaddr);    // publish the json message and clear buffer
  bool jsonIsConflict(uint16_t shortaddr, const JsonObject &values);
  void jsonPublishNow(uint16_t shortaddr, JsonObject &values);

  // Iterator
  size_t devicesSize(void) const {
    return _devices.size();
  }
  const Z_Device &devicesAt(size_t i) const {
    return _devices.at(i);
  }

  // Remove device from list
  bool removeDevice(uint16_t shortaddr);

  // Mark data as 'dirty' and requiring to save in Flash
  void dirty(void);
  void clean(void);   // avoid writing to flash the last changes

  // Find device by name, can be short_addr, long_addr, number_in_array or name
  uint16_t parseDeviceParam(const char * param, bool short_must_be_known = false) const;

private:
  std::vector<Z_Device> _devices = {};
  uint32_t              _saveTimer = 0;   
  uint8_t               _seqNumber = 0;     // global seqNumber if device is unknown

  template < typename T>
  static bool findInVector(const std::vector<T>  & vecOfElements, const T  & element);

  template < typename T>
  static int32_t findEndpointInVector(const std::vector<T>  & vecOfElements, uint8_t element);

  // find the first endpoint match for a cluster
  static int32_t findClusterEndpoint(const std::vector<uint32_t>  & vecOfElements, uint16_t element);

  Z_Device & getShortAddr(uint16_t shortaddr);   // find Device from shortAddr, creates it if does not exist
  const Z_Device & getShortAddrConst(uint16_t shortaddr) const ;   // find Device from shortAddr, creates it if does not exist
  Z_Device & getLongAddr(uint64_t longaddr);     // find Device from shortAddr, creates it if does not exist

  int32_t findShortAddr(uint16_t shortaddr) const;
  int32_t findLongAddr(uint64_t longaddr) const;
  int32_t findFriendlyName(const char * name) const;

  void _updateLastSeen(Z_Device &device) {
    if (&device != nullptr) {
      device.lastSeen = Rtc.utc_time;
    }
  };

  // Create a new entry in the devices list - must be called if it is sure it does not already exist
  Z_Device & createDeviceEntry(uint16_t shortaddr, uint64_t longaddr = 0);
};

Z_Devices zigbee_devices = Z_Devices();

// Local coordinator information
uint64_t localIEEEAddr = 0;

// https://thispointer.com/c-how-to-find-an-element-in-vector-and-get-its-index/
template < typename T>
bool Z_Devices::findInVector(const std::vector<T>  & vecOfElements, const T  & element) {
	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

	if (it != vecOfElements.end()) {
		return true;
	} else {
		return false;
	}
}

template < typename T>
int32_t Z_Devices::findEndpointInVector(const std::vector<T>  & vecOfElements, uint8_t element) {
	// Find given element in vector

  int32_t found = 0;
  for (auto &elem : vecOfElements) {
    if ( ((elem >> 16) & 0xFF) == element) { return found; }
    found++;
  }

  return -1;
}

//
// Find the first endpoint match for a cluster, whether in or out
// Clusters are stored in the format 0x00EECCCC (EE=endpoint, CCCC=cluster number)
// In:
//    _devices.clusters_in or _devices.clusters_out
//    cluster number looked for
// Out:
//    Index of found Endpoint_Cluster number, or -1 if not found
//
int32_t Z_Devices::findClusterEndpoint(const std::vector<uint32_t>  & vecOfElements, uint16_t cluster) {
  int32_t found = 0;
  for (auto &elem : vecOfElements) {
    if ((elem & 0xFFFF) == cluster) { return found; }
    found++;
  }
  return -1;
}

//
// Create a new Z_Device entry in _devices. Only to be called if you are sure that no
// entry with same shortaddr or longaddr exists.
//
Z_Device & Z_Devices::createDeviceEntry(uint16_t shortaddr, uint64_t longaddr) {
  if (!shortaddr && !longaddr) { return *(Z_Device*) nullptr; }      // it is not legal to create an enrty with both short/long addr null
  Z_Device device = { shortaddr, longaddr,
                      Rtc.utc_time, Rtc.utc_time,
                      String(),   // ManufId
                      String(),   // DeviceId
                      String(),   // FriendlyName
                      std::vector<uint32_t>(),
                      std::vector<uint32_t>(),
                      std::vector<uint32_t>(),
                      0,0,0,0,
                      nullptr,
                      nullptr, nullptr,
                      0,          // seqNumber
                      };
  device.json_buffer = new DynamicJsonBuffer();
  _devices.push_back(device);
  dirty();
  return _devices.back();
}

//
// Scan all devices to find a corresponding shortaddr
// Looks info device.shortaddr entry
// In:
//    shortaddr (non null)
// Out:
//    index in _devices of entry, -1 if not found
//
int32_t Z_Devices::findShortAddr(uint16_t shortaddr) const {
  if (!shortaddr) { return -1; }              // does not make sense to look for 0x0000 shortaddr (localhost)
  int32_t found = 0;
  if (shortaddr) {
    for (auto &elem : _devices) {
      if (elem.shortaddr == shortaddr) { return found; }
      found++;
    }
  }
  return -1;
}
//
// Scan all devices to find a corresponding longaddr
// Looks info device.longaddr entry
// In:
//    longaddr (non null)
// Out:
//    index in _devices of entry, -1 if not found
//
int32_t Z_Devices::findLongAddr(uint64_t longaddr) const {
  if (!longaddr) { return -1; }
  int32_t found = 0;
  if (longaddr) {
    for (auto &elem : _devices) {
      if (elem.longaddr == longaddr) { return found; }
      found++;
    }
  }
  return -1;
}
//
// Scan all devices to find a corresponding friendlyNme
// Looks info device.friendlyName entry
// In:
//    friendlyName (null terminated, should not be empty)
// Out:
//    index in _devices of entry, -1 if not found
//
int32_t Z_Devices::findFriendlyName(const char * name) const {
  if (!name) { return -1; }              // if pointer is null
  size_t name_len = strlen(name);
  int32_t found = 0;
  if (name_len) {
    for (auto &elem : _devices) {
      if (elem.friendlyName == name) { return found; }
      found++;
    }
  }
  return -1;
}

// Probe if device is already known but don't create any entry
uint16_t Z_Devices::isKnownShortAddr(uint16_t shortaddr) const {
  int32_t found = findShortAddr(shortaddr);
  if (found >= 0) {
    return shortaddr;
  } else {
    return 0;   // unknown
  }
}

uint16_t Z_Devices::isKnownLongAddr(uint64_t longaddr) const {
  int32_t found = findLongAddr(longaddr);
  if (found >= 0) {
    const Z_Device & device = devicesAt(found);
    return device.shortaddr;    // can be zero, if not yet registered
  } else {
    return 0;
  }
}

uint16_t Z_Devices::isKnownIndex(uint32_t index) const {
  if (index < devicesSize()) {
    const Z_Device & device = devicesAt(index);
    return device.shortaddr;
  } else {
    return 0;
  }
}

uint16_t Z_Devices::isKnownFriendlyName(const char * name) const {
  if ((!name) || (0 == strlen(name))) { return 0xFFFF; }         // Error
  int32_t found = findFriendlyName(name);
  if (found >= 0) {
    const Z_Device & device = devicesAt(found);
    return device.shortaddr;    // can be zero, if not yet registered
  } else {
    return 0;
  }
}

uint64_t Z_Devices::getDeviceLongAddr(uint16_t shortaddr) const {
  const Z_Device & device = getShortAddrConst(shortaddr);
  return device.longaddr;
}

//
// We have a seen a shortaddr on the network, get the corresponding
//
Z_Device & Z_Devices::getShortAddr(uint16_t shortaddr) {
  if (!shortaddr) { return *(Z_Device*) nullptr; }   // this is not legal
  int32_t found = findShortAddr(shortaddr);
  if (found >= 0) {
    return _devices[found];
  }
  //Serial.printf("Device entry created for shortaddr = 0x%02X, found = %d\n", shortaddr, found);
  return createDeviceEntry(shortaddr, 0);
}
// Same version but Const
const Z_Device & Z_Devices::getShortAddrConst(uint16_t shortaddr) const {
  if (!shortaddr) { return *(Z_Device*) nullptr; }   // this is not legal
  int32_t found = findShortAddr(shortaddr);
  if (found >= 0) {
    return _devices[found];
  }
  return *((Z_Device*)nullptr);
}

// find the Device object by its longaddr (unique key if not null)
Z_Device & Z_Devices::getLongAddr(uint64_t longaddr) {
  if (!longaddr) { return *(Z_Device*) nullptr; }
  int32_t found = findLongAddr(longaddr);
  if (found > 0) {
    return _devices[found];
  }
  return createDeviceEntry(0, longaddr);
}

// Remove device from list, return true if it was known, false if it was not recorded
bool Z_Devices::removeDevice(uint16_t shortaddr) {
  int32_t found = findShortAddr(shortaddr);
  if (found >= 0) {
    _devices.erase(_devices.begin() + found);
    dirty();
    return true;
  }
  return false;
}

//
// We have just seen a device on the network, update the info based on short/long addr
// In:
//    shortaddr
//    longaddr (both can't be null at the same time)
void Z_Devices::updateDevice(uint16_t shortaddr, uint64_t longaddr) {
  int32_t s_found = findShortAddr(shortaddr);       // is there already a shortaddr entry
  int32_t l_found = findLongAddr(longaddr);         // is there already a longaddr entry

  if ((s_found >= 0) && (l_found >= 0)) {           // both shortaddr and longaddr are already registered
    if (s_found == l_found) {
      updateLastSeen(shortaddr);                    // short/long addr match, all good
    } else {                                        // they don't match
      // the device with longaddr got a new shortaddr
      _devices[l_found].shortaddr = shortaddr;      // update the shortaddr corresponding to the longaddr
      // erase the previous shortaddr
      _devices.erase(_devices.begin() + s_found);
      updateLastSeen(shortaddr);
      dirty();
    }
  } else if (s_found >= 0) {
    // shortaddr already exists but longaddr not
    // add the longaddr to the entry
    _devices[s_found].longaddr = longaddr;
    updateLastSeen(shortaddr);
    dirty();
  } else if (l_found >= 0) {
    // longaddr entry exists, update shortaddr
    _devices[l_found].shortaddr = shortaddr;
    dirty();
  } else {
    // neither short/lonf addr are found.
    if (shortaddr || longaddr) {
      createDeviceEntry(shortaddr, longaddr);
    }
  }
}

//
// Add an endpoint to a shortaddr
//
void Z_Devices::addEndoint(uint16_t shortaddr, uint8_t endpoint) {
  if (!shortaddr) { return; }
  uint32_t ep_profile = (endpoint << 16);
  Z_Device &device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found
  _updateLastSeen(device);
  if (findEndpointInVector(device.endpoints, endpoint) < 0) {
    device.endpoints.push_back(ep_profile);
    dirty();
  }
}

void Z_Devices::addEndointProfile(uint16_t shortaddr, uint8_t endpoint, uint16_t profileId) {
  if (!shortaddr) { return; }
  uint32_t ep_profile = (endpoint << 16) | profileId;
  Z_Device &device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found
  _updateLastSeen(device);
  int32_t found = findEndpointInVector(device.endpoints, endpoint);
  if (found < 0) {
    device.endpoints.push_back(ep_profile);
    dirty();
  } else {
    if (device.endpoints[found] != ep_profile) {
      device.endpoints[found] = ep_profile;
      dirty();
    }
  }
}

void Z_Devices::addCluster(uint16_t shortaddr, uint8_t endpoint, uint16_t cluster, bool out) {
  if (!shortaddr) { return; }
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found
  _updateLastSeen(device);
  uint32_t ep_cluster = (endpoint << 16) | cluster;
  if (!out) {
    if (!findInVector(device.clusters_in, ep_cluster)) {
      device.clusters_in.push_back(ep_cluster);
      dirty();
    }
  } else { // out
    if (!findInVector(device.clusters_out, ep_cluster)) {
      device.clusters_out.push_back(ep_cluster);
      dirty();
    }
  }
}

// Look for the best endpoint match to send a command for a specific Cluster ID
// return 0x00 if none found
uint8_t Z_Devices::findClusterEndpointIn(uint16_t shortaddr, uint16_t cluster){
  int32_t short_found = findShortAddr(shortaddr);
  if (short_found < 0)  return 0;     // avoid creating an entry if the device was never seen
  Z_Device &device = getShortAddr(shortaddr);
  if (&device == nullptr) { return 0; }                 // don't crash if not found
  int32_t found = findClusterEndpoint(device.clusters_in, cluster);
  if (found >= 0) {
    return (device.clusters_in[found] >> 16) & 0xFF;
  } else {
    return 0;
  }
}


void Z_Devices::setManufId(uint16_t shortaddr, const char * str) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found
  _updateLastSeen(device);
  if (!device.manufacturerId.equals(str)) {
    dirty();
  }
  device.manufacturerId = str;
}
void Z_Devices::setModelId(uint16_t shortaddr, const char * str) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found
  _updateLastSeen(device);
  if (!device.modelId.equals(str)) {
    dirty();
  }
  device.modelId = str;
}
void Z_Devices::setFriendlyName(uint16_t shortaddr, const char * str) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found
  _updateLastSeen(device);
  if (!device.friendlyName.equals(str)) {
    dirty();
  }
  device.friendlyName = str;
}

const String * Z_Devices::getFriendlyName(uint16_t shortaddr) const {
  int32_t found = findShortAddr(shortaddr);
  if (found >= 0) {
    const Z_Device & device = devicesAt(found);
    if (device.friendlyName.length() > 0) {
      return &device.friendlyName;
    }
  }
  return nullptr;
}

const String * Z_Devices::getModelId(uint16_t shortaddr) const {
  int32_t found = findShortAddr(shortaddr);
  if (found >= 0) {
    const Z_Device & device = devicesAt(found);
    if (device.modelId.length() > 0) {
      return &device.modelId;
    }
  }
  return nullptr;
}

// device just seen on the network, update the lastSeen field
void Z_Devices::updateLastSeen(uint16_t shortaddr) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found
  _updateLastSeen(device);
}

// get the next sequance number for the device, or use the global seq number if device is unknown
uint8_t Z_Devices::getNextSeqNumber(uint16_t shortaddr) {
  int32_t short_found = findShortAddr(shortaddr);
  if (short_found >= 0) {
    Z_Device &device = getShortAddr(shortaddr);
    device.seqNumber += 1;
    return device.seqNumber;
  } else {
    _seqNumber += 1;
    return _seqNumber;
  }
}

// Per device timers
//
// Reset the timer for a specific device
void Z_Devices::resetTimer(uint32_t shortaddr) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found
  device.timer = 0;
  device.func = nullptr;
}

// Set timer for a specific device
void Z_Devices::setTimer(uint32_t shortaddr, uint32_t wait_ms, uint16_t cluster, uint16_t endpoint, uint32_t value, Z_DeviceTimer func) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found

  device.cluster = cluster;
  device.endpoint = endpoint;
  device.value = value;
  device.func = func;
  device.timer = wait_ms + millis();
}

// Run timer at each tick
void Z_Devices::runTimer(void) {
  for (std::vector<Z_Device>::iterator it = _devices.begin(); it != _devices.end(); ++it) {
    Z_Device &device = *it;
    uint16_t shortaddr = device.shortaddr;

    uint32_t timer = device.timer;
    if ((timer) && TimeReached(timer)) {
      device.timer = 0;       // cancel the timer before calling, so the callback can set another timer
      // trigger the timer
      (*device.func)(device.shortaddr, device.cluster, device.endpoint, device.value);
    }
  }
  // save timer
  if ((_saveTimer) && TimeReached(_saveTimer)) {
    saveZigbeeDevices();
    _saveTimer = 0;
  }
}

void Z_Devices::jsonClear(uint16_t shortaddr) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found

  device.json = nullptr;
  device.json_buffer->clear();
}

void CopyJsonVariant(JsonObject &to, const String &key, const JsonVariant &val) {
  to.remove(key);    // force remove to have metadata like LinkQuality at the end

  if (val.is<char*>()) {
    String sval = val.as<String>();       // force a copy of the String value
    to.set(key, sval);
  } else if (val.is<JsonArray>()) {
    JsonArray &nested_arr = to.createNestedArray(key);
    CopyJsonArray(nested_arr, val.as<JsonArray>());
  } else if (val.is<JsonObject>()) {
    JsonObject &nested_obj = to.createNestedObject(key);
    CopyJsonObject(nested_obj, val.as<JsonObject>());
  } else {
    to.set(key, val);
  }
}

void CopyJsonArray(JsonArray &to, const JsonArray &arr) {
  for (auto v : arr) {
    if (v.is<char*>()) {
      String sval = v.as<String>();       // force a copy of the String value
      to.add(sval);
    } else if (v.is<JsonArray>()) {
    } else if (v.is<JsonObject>()) {
    } else {
      to.add(v);
    }
  }
}

void CopyJsonObject(JsonObject &to, const JsonObject &from) {
  for (auto kv : from) {
    String key_string = kv.key;
    JsonVariant &val = kv.value;

    CopyJsonVariant(to, key_string, val);
  }
}

// does the new payload conflicts with the existing payload, i.e. values would be overwritten
bool Z_Devices::jsonIsConflict(uint16_t shortaddr, const JsonObject &values) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return false; }                 // don't crash if not found
  if (&values == nullptr) { return false; }

  if (nullptr == device.json) {
    return false;                                           // if no previous value, no conflict
  }

  // compare groups
  uint16_t group1 = 0;
  uint16_t group2 = 0;
  if (device.json->containsKey(D_CMND_ZIGBEE_GROUP)) {
    group1 = device.json->get<unsigned int>(D_CMND_ZIGBEE_GROUP);
  }
  if (values.containsKey(D_CMND_ZIGBEE_GROUP)) {
    group2 = values.get<unsigned int>(D_CMND_ZIGBEE_GROUP);
  }
  if (group1 != group2) {
    return true;      // if group addresses differ, then conflict
  }

  // parse all other parameters
  for (auto kv : values) {
    String key_string = kv.key;

    if (0 == strcasecmp_P(kv.key, PSTR(D_CMND_ZIGBEE_GROUP))) {
      // ignore group, it was handled already
    } else if (0 == strcasecmp_P(kv.key, PSTR(D_CMND_ZIGBEE_ENDPOINT))) {
      // attribute "Endpoint" or "Group"
      if (device.json->containsKey(kv.key)) {
        if (kv.value.as<unsigned int>() != device.json->get<unsigned int>(kv.key)) {
          return true;
        }
      }
    } else if (strcasecmp_P(kv.key, PSTR(D_CMND_ZIGBEE_LINKQUALITY))) {  // exception = ignore duplicates for LinkQuality
      if (device.json->containsKey(kv.key)) {
        return true;          // conflict!
      }
    }
  }
  return false;
}

void Z_Devices::jsonAppend(uint16_t shortaddr, const JsonObject &values) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found
  if (&values == nullptr) { return; }

  if (nullptr == device.json) {
    device.json = &(device.json_buffer->createObject());
  }
  // Prepend Device, will be removed later if redundant
  char sa[8];
  snprintf_P(sa, sizeof(sa), PSTR("0x%04X"), shortaddr);
  device.json->set(F(D_JSON_ZIGBEE_DEVICE), sa);
  // Prepend Friendly Name if it has one
  const String * fname = zigbee_devices.getFriendlyName(shortaddr);
  if (fname) {
    device.json->set(F(D_JSON_ZIGBEE_NAME), (char*)fname->c_str());   // (char*) forces ArduinoJson to make a copy of the cstring
  }

  // copy all values from 'values' to 'json'
  CopyJsonObject(*device.json, values);
}

const JsonObject *Z_Devices::jsonGet(uint16_t shortaddr) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return nullptr; }                 // don't crash if not found
  return device.json;
}

void Z_Devices::jsonPublishFlush(uint16_t shortaddr) {
  Z_Device & device = getShortAddr(shortaddr);
  if (&device == nullptr) { return; }                 // don't crash if not found
  JsonObject * json = device.json;
  if (json == nullptr) { return; }                    // abort if nothing in buffer

  const String * fname = zigbee_devices.getFriendlyName(shortaddr);
  bool use_fname = (Settings.flag4.zigbee_use_names) && (fname);    // should we replace shortaddr with friendlyname?

  // if (use_fname) {
  //   // we need to add the Device short_addr inside the JSON
  //   char sa[8];
  //   snprintf_P(sa, sizeof(sa), PSTR("0x%04X"), shortaddr);
  //   json->set(F(D_JSON_ZIGBEE_DEVICE), sa);
  // } else if (fname) {
  //   json->set(F(D_JSON_NAME), (char*) fname);
  // }

  // Remove redundant "Name" or "Device"
  if (use_fname) {
    json->remove(F(D_JSON_ZIGBEE_NAME));
  } else {
    json->remove(F(D_JSON_ZIGBEE_DEVICE));
  }

  String msg = "";
  json->printTo(msg);
  zigbee_devices.jsonClear(shortaddr);

  if (use_fname) {
    Response_P(PSTR("{\"" D_JSON_ZIGBEE_RECEIVED "\":{\"%s\":%s}}"), fname->c_str(), msg.c_str());
  } else {
    Response_P(PSTR("{\"" D_JSON_ZIGBEE_RECEIVED "\":{\"0x%04X\":%s}}"), shortaddr, msg.c_str());
  }
  if (Settings.flag4.zigbee_distinct_topics) {
    char subtopic[16];
    snprintf_P(subtopic, sizeof(subtopic), PSTR("%04X/" D_RSLT_SENSOR), shortaddr);
    MqttPublishPrefixTopic_P(TELE, subtopic, Settings.flag.mqtt_sensor_retain);
  } else {
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
  }
  XdrvRulesProcess();
}

void Z_Devices::jsonPublishNow(uint16_t shortaddr, JsonObject & values) {
  jsonPublishFlush(shortaddr);    // flush any previous buffer
  jsonAppend(shortaddr, values);
  jsonPublishFlush(shortaddr);    // publish now
}

void Z_Devices::dirty(void) {
  _saveTimer = kZigbeeSaveDelaySeconds * 1000 + millis();
}
void Z_Devices::clean(void) {
  _saveTimer = 0;
}

// Parse the command parameters for either:
// - a short address starting with "0x", example: 0x1234
// - a long address starting with "0x", example: 0x7CB03EBB0A0292DD
// - a number 0..99, the index number in ZigbeeStatus
// - a friendly name, between quotes, example: "Room_Temp"
uint16_t Z_Devices::parseDeviceParam(const char * param, bool short_must_be_known) const {
  if (nullptr == param) { return 0; }
  size_t param_len = strlen(param);
  char dataBuf[param_len + 1];
  strcpy(dataBuf, param);
  RemoveSpace(dataBuf);
  uint16_t shortaddr = 0;

  if (strlen(dataBuf) < 4) {
    // simple number 0..99
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 99)) {
      shortaddr = zigbee_devices.isKnownIndex(XdrvMailbox.payload - 1);
    }
  } else if ((dataBuf[0] == '0') && (dataBuf[1] == 'x')) {
    // starts with 0x
    if (strlen(dataBuf) < 18) {
      // expect a short address
      shortaddr = strtoull(dataBuf, nullptr, 0);
      if (short_must_be_known) {
        shortaddr = zigbee_devices.isKnownShortAddr(shortaddr);
      }
      // else we don't check if it's already registered to force unregistered devices
    } else {
      // expect a long address
      uint64_t longaddr = strtoull(dataBuf, nullptr, 0);
      shortaddr = zigbee_devices.isKnownLongAddr(longaddr);
    }
  } else {
    // expect a Friendly Name
    shortaddr = zigbee_devices.isKnownFriendlyName(dataBuf);
  }

  return shortaddr;
}

// Dump the internal memory of Zigbee devices
// Mode = 1: simple dump of devices addresses
// Mode = 2: simple dump of devices addresses and names
// Mode = 3: Mode 2 + also dump the endpoints, profiles and clusters
String Z_Devices::dump(uint32_t dump_mode, uint16_t status_shortaddr) const {
  DynamicJsonBuffer jsonBuffer;
  JsonArray& json = jsonBuffer.createArray();
  JsonArray& devices = json;

  for (std::vector<Z_Device>::const_iterator it = _devices.begin(); it != _devices.end(); ++it) {
    const Z_Device& device = *it;
    uint16_t shortaddr = device.shortaddr;
    char hex[22];

    // ignore non-current device, if specified device is non-zero
    if ((status_shortaddr) && (status_shortaddr != shortaddr)) { continue; }

    JsonObject& dev = devices.createNestedObject();

    snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), shortaddr);
    dev[F(D_JSON_ZIGBEE_DEVICE)] = hex;

    if (device.friendlyName.length() > 0) {
      dev[F(D_JSON_ZIGBEE_NAME)] = device.friendlyName;
    }

    if (2 <= dump_mode) {
      hex[0] = '0';   // prefix with '0x'
      hex[1] = 'x';
      Uint64toHex(device.longaddr, &hex[2], 64);
      dev[F("IEEEAddr")] = hex;
      if (device.modelId.length() > 0) {
        dev[F(D_JSON_MODEL D_JSON_ID)] = device.modelId;
      }
      if (device.manufacturerId.length() > 0) {
        dev[F("Manufacturer")] = device.manufacturerId;
      }
    }

    // If dump_mode == 2, dump a lot more details
    if (3 <= dump_mode) {
      JsonObject& dev_endpoints = dev.createNestedObject(F("Endpoints"));
      for (std::vector<uint32_t>::const_iterator ite = device.endpoints.begin() ; ite != device.endpoints.end(); ++ite) {
        uint32_t ep_profile = *ite;
        uint8_t endpoint = (ep_profile >> 16) & 0xFF;
        uint16_t profileId = ep_profile & 0xFFFF;

        snprintf_P(hex, sizeof(hex), PSTR("0x%02X"), endpoint);
        JsonObject& ep = dev_endpoints.createNestedObject(hex);

        snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), profileId);
        ep[F("ProfileId")] = hex;

        int32_t found = -1;
        for (uint32_t i = 0; i < sizeof(Z_ProfileIds) / sizeof(Z_ProfileIds[0]); i++) {
          if (pgm_read_word(&Z_ProfileIds[i]) == profileId) {
            found = i;
            break;
          }
        }
        if (found > 0) {
          GetTextIndexed(hex, sizeof(hex), found, Z_ProfileNames);
          ep[F("ProfileIdName")] = hex;
        }

        ep.createNestedArray(F("ClustersIn"));
        ep.createNestedArray(F("ClustersOut"));
      }

      for (std::vector<uint32_t>::const_iterator itc = device.clusters_in.begin() ; itc != device.clusters_in.end(); ++itc) {
        uint16_t cluster = *itc & 0xFFFF;
        uint8_t  endpoint = (*itc >> 16) & 0xFF;

        snprintf_P(hex, sizeof(hex), PSTR("0x%02X"), endpoint);
        JsonArray &cluster_arr = dev_endpoints[hex][F("ClustersIn")];

        snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), cluster);
        cluster_arr.add(hex);
      }

      for (std::vector<uint32_t>::const_iterator itc = device.clusters_out.begin() ; itc != device.clusters_out.end(); ++itc) {
        uint16_t cluster = *itc & 0xFFFF;
        uint8_t  endpoint = (*itc >> 16) & 0xFF;

        snprintf_P(hex, sizeof(hex), PSTR("0x%02X"), endpoint);
        JsonArray &cluster_arr = dev_endpoints[hex][F("ClustersOut")];

        snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), cluster);
        cluster_arr.add(hex);
      }
    }
  }
  String payload = "";
  payload.reserve(200);
  json.printTo(payload);
  return payload;
}

#endif // USE_ZIGBEE
