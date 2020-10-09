/*
  xdrv_23_zigbee_2a_devices_impl.ino - zigbee support for Tasmota

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
 * Implementation
\*********************************************************************************************/

//
// Create a new Z_Device entry in _devices. Only to be called if you are sure that no
// entry with same shortaddr or longaddr exists.
//
Z_Device & Z_Devices::createDeviceEntry(uint16_t shortaddr, uint64_t longaddr) {
  if ((BAD_SHORTADDR == shortaddr) && !longaddr) { return (Z_Device&) device_unk; }      // it is not legal to create this entry
  Z_Device device(shortaddr, longaddr);

  dirty();
  return _devices.addHead(device);
}

void Z_Devices::freeDeviceEntry(Z_Device *device) {
  if (device->manufacturerId) { free(device->manufacturerId); }
  if (device->modelId) { free(device->modelId); }
  if (device->friendlyName) { free(device->friendlyName); }
  free(device);
}

//
// Scan all devices to find a corresponding shortaddr
// Looks info device.shortaddr entry
// In:
//    shortaddr (not BAD_SHORTADDR)
// Out:
//    reference to device, or to device_unk if not found
//    (use foundDevice() to check if found)
Z_Device & Z_Devices::findShortAddr(uint16_t shortaddr) {
  for (auto & elem : _devices) {
    if (elem.shortaddr == shortaddr) { return elem; }
  }
  return (Z_Device&) device_unk;
}
const Z_Device & Z_Devices::findShortAddr(uint16_t shortaddr) const {
  for (const auto & elem : _devices) {
    if (elem.shortaddr == shortaddr) { return elem; }
  }
  return device_unk;
}
//
// Scan all devices to find a corresponding longaddr
// Looks info device.longaddr entry
// In:
//    longaddr (non null)
// Out:
//    index in _devices of entry, -1 if not found
//
Z_Device & Z_Devices::findLongAddr(uint64_t longaddr) {
  if (!longaddr) { return (Z_Device&) device_unk; }
  for (auto &elem : _devices) {
    if (elem.longaddr == longaddr) { return elem; }
  }
  return (Z_Device&) device_unk;
}
const Z_Device & Z_Devices::findLongAddr(uint64_t longaddr) const {
  if (!longaddr) { return device_unk; }
  for (const auto &elem : _devices) {
    if (elem.longaddr == longaddr) { return elem; }
  }
  return device_unk;
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
      if (elem.friendlyName) {
        if (strcasecmp(elem.friendlyName, name) == 0) { return found; }
      }
      found++;
    }
  }
  return -1;
}

uint16_t Z_Devices::isKnownLongAddr(uint64_t longaddr) const {
  const Z_Device & device = findLongAddr(longaddr);
  if (foundDevice(device)) {
    return device.shortaddr;    // can be zero, if not yet registered
  } else {
    return BAD_SHORTADDR;
  }
}

uint16_t Z_Devices::isKnownIndex(uint32_t index) const {
  if (index < devicesSize()) {
    const Z_Device & device = devicesAt(index);
    return device.shortaddr;
  } else {
    return BAD_SHORTADDR;
  }
}

uint16_t Z_Devices::isKnownFriendlyName(const char * name) const {
  if ((!name) || (0 == strlen(name))) { return BAD_SHORTADDR; }         // Error
  int32_t found = findFriendlyName(name);
  if (found >= 0) {
    const Z_Device & device = devicesAt(found);
    return device.shortaddr;    // can be zero, if not yet registered
  } else {
    return BAD_SHORTADDR;
  }
}

uint64_t Z_Devices::getDeviceLongAddr(uint16_t shortaddr) const {
  return findShortAddr(shortaddr).longaddr;     // if unknown, it reverts to the Unknown device and longaddr is 0x00
}

//
// We have a seen a shortaddr on the network, get the corresponding device object
//
Z_Device & Z_Devices::getShortAddr(uint16_t shortaddr) {
  if (BAD_SHORTADDR == shortaddr) { return (Z_Device&) device_unk; }   // this is not legal
  Z_Device & device = findShortAddr(shortaddr);
  if (foundDevice(device)) {
    return device;
  }
  return createDeviceEntry(shortaddr, 0);
}

// find the Device object by its longaddr (unique key if not null)
Z_Device & Z_Devices::getLongAddr(uint64_t longaddr) {
  if (!longaddr) { return (Z_Device&) device_unk; }
  Z_Device & device = findLongAddr(longaddr);
  if (foundDevice(device)) {
    return device;
  }
  return createDeviceEntry(0, longaddr);
}

// Remove device from list, return true if it was known, false if it was not recorded
bool Z_Devices::removeDevice(uint16_t shortaddr) {
  Z_Device & device = findShortAddr(shortaddr);
  if (foundDevice(device)) {
    _devices.remove(&device);
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
  Z_Device * s_found = &findShortAddr(shortaddr); // is there already a shortaddr entry
  Z_Device * l_found = &findLongAddr(longaddr);      // is there already a longaddr entry

  if (foundDevice(*s_found) && foundDevice(*l_found)) {  // both shortaddr and longaddr are already registered
    if (s_found == l_found) {
    } else {                                        // they don't match
      // the device with longaddr got a new shortaddr
      l_found->shortaddr = shortaddr;      // update the shortaddr corresponding to the longaddr
      // erase the previous shortaddr
      freeDeviceEntry(s_found);
      _devices.remove(s_found);
      dirty();
    }
  } else if (foundDevice(*s_found)) {
    // shortaddr already exists but longaddr not
    // add the longaddr to the entry
    s_found->longaddr = longaddr;
    dirty();
  } else if (foundDevice(*l_found)) {
    // longaddr entry exists, update shortaddr
    l_found->shortaddr = shortaddr;
    dirty();
  } else {
    // neither short/lonf addr are found.
    if ((BAD_SHORTADDR != shortaddr) || longaddr) {
      createDeviceEntry(shortaddr, longaddr);
    }
  }
}

//
// Clear all endpoints
//
void Z_Devices::clearEndpoints(uint16_t shortaddr) {
  Z_Device &device = getShortAddr(shortaddr);
  for (uint32_t i = 0; i < endpoints_max; i++) {
    device.endpoints[i] = 0;
    // no dirty here because it doesn't make sense to store it, does it?
  }
}

//
// Add an endpoint to a shortaddr
//
void Z_Devices::addEndpoint(uint16_t shortaddr, uint8_t endpoint) {
  if (0x00 == endpoint) { return; }
  Z_Device &device = getShortAddr(shortaddr);

  for (uint32_t i = 0; i < endpoints_max; i++) {
    if (endpoint == device.endpoints[i]) {
      return;     // endpoint already there
    }
    if (0 == device.endpoints[i]) {
      device.endpoints[i] = endpoint;
      dirty();
      return;
    }
  }
}

//
// Count the number of known endpoints
//
uint32_t Z_Devices::countEndpoints(uint16_t shortaddr) const {
  uint32_t count_ep = 0;
  const Z_Device & device =findShortAddr(shortaddr);
  if (!foundDevice(device)) return 0;

  for (uint32_t i = 0; i < endpoints_max; i++) {
    if (0 != device.endpoints[i]) {
      count_ep++;
    }
  }
  return count_ep;
}

// Find the first endpoint of the device
uint8_t Z_Devices::findFirstEndpoint(uint16_t shortaddr) const {
  // When in router of end-device mode, the coordinator was not probed, in this case always talk to endpoint 1
  if (0x0000 == shortaddr) { return 1; }
  return findShortAddr(shortaddr).endpoints[0];   // returns 0x00 if no endpoint
}

void Z_Devices::setStringAttribute(char*& attr, const char * str) {
  if (nullptr == str)  { return; }                    // ignore a null parameter
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
    attr = (char*) malloc(str_len + 1);
    strlcpy(attr, str, str_len + 1);
  }
  dirty();
}

//
// Sets the ManufId for a device.
// No action taken if the device does not exist.
// Inputs:
// - shortaddr: 16-bits short address of the device. No action taken if the device is unknown
// - str:       string pointer, if nullptr it is considered as empty string
// Impact:
// - Any actual change in ManufId (i.e. setting a different value) triggers a `dirty()` and saving to Flash
//
void Z_Devices::setManufId(uint16_t shortaddr, const char * str) {
  setStringAttribute(getShortAddr(shortaddr).manufacturerId, str);
}

void Z_Devices::setModelId(uint16_t shortaddr, const char * str) {
  setStringAttribute(getShortAddr(shortaddr).modelId, str);
}

void Z_Devices::setFriendlyName(uint16_t shortaddr, const char * str) {
  setStringAttribute(getShortAddr(shortaddr).friendlyName, str);
}


void Z_Devices::setReachable(uint16_t shortaddr, bool reachable) {
  getShortAddr(shortaddr).setReachable(reachable);
}

void Z_Devices::setLQI(uint16_t shortaddr, uint8_t lqi) {
  if (shortaddr == localShortAddr) { return; }
  getShortAddr(shortaddr).lqi = lqi;
}

void Z_Devices::setLastSeenNow(uint16_t shortaddr) {
  if (shortaddr == localShortAddr) { return; }
  // Only update time if after 2020-01-01 0000.
  // Fixes issue where zigbee device pings before WiFi/NTP has set utc_time
  // to the correct time, and "last seen" calculations are based on the
  // pre-corrected last_seen time and the since-corrected utc_time.
  if (Rtc.utc_time < 1577836800) { return; }
  getShortAddr(shortaddr).last_seen = Rtc.utc_time;
}


void Z_Devices::setBatteryPercent(uint16_t shortaddr, uint8_t bp) {
  getShortAddr(shortaddr).batterypercent = bp;
}

// get the next sequance number for the device, or use the global seq number if device is unknown
uint8_t Z_Devices::getNextSeqNumber(uint16_t shortaddr) {
  Z_Device & device = findShortAddr(shortaddr);
  if (foundDevice(device)) {
    device.seqNumber += 1;
    return device.seqNumber;
  } else {
    _seqNumber += 1;
    return _seqNumber;
  }
}

// General Zigbee device profile support
void Z_Devices::setLightProfile(uint16_t shortaddr, uint8_t light_profile) {
  Z_Device &device = getShortAddr(shortaddr);
  if (device.setLightChannels(light_profile)) {
    dirty();
  }
}

// Returns the device profile or 0xFF if the device or profile is unknown
uint8_t Z_Devices::getLightProfile(uint16_t shortaddr) const {
  const Z_Device &device = findShortAddr(shortaddr);
  return device.getLightChannels();
}

int8_t Z_Devices::getHueBulbtype(uint16_t shortaddr) const {
  int8_t light_profile = getLightProfile(shortaddr);
  if (0x00 == (light_profile & 0xF0)) {
    return (light_profile & 0x07);
  } else {
    // not a bulb
    return -1;
  }
}

void Z_Devices::hideHueBulb(uint16_t shortaddr, bool hidden) {
  Z_Device &device = getShortAddr(shortaddr);
  if (device.hidden != hidden) {
    device.hidden = hidden;
    dirty();
  }
}
// true if device is not knwon or not a bulb - it wouldn't make sense to publish a non-bulb
bool Z_Devices::isHueBulbHidden(uint16_t shortaddr) const {
  const Z_Device & device = findShortAddr(shortaddr);
  if (foundDevice(device)) {
    return device.hidden;
  }
  return true;      // Fallback - Device is considered as hidden
}

// Deferred actions
// Parse for a specific category, of all deferred for a device if category == 0xFF
// Only with specific cluster number or for all clusters if cluster == 0xFFFF
void Z_Devices::resetTimersForDevice(uint16_t shortaddr, uint16_t groupaddr, uint8_t category, uint16_t cluster, uint8_t endpoint) {
  // iterate the list of deferred, and remove any linked to the shortaddr
  for (auto & defer : _deferred) {
    if ((defer.shortaddr == shortaddr) && (defer.groupaddr == groupaddr)) {
      if ((0xFF == category) || (defer.category == category)) {
        if ((0xFFFF == cluster) || (defer.cluster == cluster)) {
          if ((0xFF == endpoint) || (defer.endpoint == endpoint)) {
            _deferred.remove(&defer);
          }
        }
      }
    }
  }
}

// Set timer for a specific device
void Z_Devices::setTimer(uint16_t shortaddr, uint16_t groupaddr, uint32_t wait_ms, uint16_t cluster, uint8_t endpoint, uint8_t category, uint32_t value, Z_DeviceTimer func) {
  // First we remove any existing timer for same device in same category, except for category=0x00 (they need to happen anyway)
  if (category >= Z_CLEAR_DEVICE) {     // if category == 0, we leave all previous timers
    resetTimersForDevice(shortaddr, groupaddr, category, category >= Z_CLEAR_DEVICE_CLUSTER ? cluster : 0xFFFF, category >= Z_CLEAR_DEVICE_CLUSTER_ENDPOINT ? endpoint : 0xFF);    // remove any cluster
  }

  // Now create the new timer
  Z_Deferred & deferred = _deferred.addHead();
  deferred = { wait_ms + millis(),   // timer
                          shortaddr,
                          groupaddr,
                          cluster,
                          endpoint,
                          category,
                          value,
                          func };
}

// Set timer after the already queued events
// I.e. the wait_ms is not counted from now, but from the last event queued, which is 'now' or in the future
void Z_Devices::queueTimer(uint16_t shortaddr, uint16_t groupaddr, uint32_t wait_ms, uint16_t cluster, uint8_t endpoint, uint8_t category, uint32_t value, Z_DeviceTimer func) {
  Z_Device & device = getShortAddr(shortaddr);
  uint32_t now_millis = millis();
  if (TimeReached(device.defer_last_message_sent)) {
    device.defer_last_message_sent = now_millis;
  }
  // defer_last_message_sent equals now or a value in the future
  device.defer_last_message_sent += wait_ms;

  // for queueing we don't clear the backlog, so we force category to Z_CAT_ALWAYS
  setTimer(shortaddr, groupaddr, (device.defer_last_message_sent - now_millis), cluster, endpoint, Z_CAT_ALWAYS, value, func);
}

// Run timer at each tick
// WARNING: don't set a new timer within a running timer, this causes memory corruption
void Z_Devices::runTimer(void) {
  // visit all timers
  for (auto & defer : _deferred) {
    uint32_t timer = defer.timer;
    if (TimeReached(timer)) {
      (*defer.func)(defer.shortaddr, defer.groupaddr, defer.cluster, defer.endpoint, defer.value);
      _deferred.remove(&defer);
    }
  }

  // check if we need to save to Flash
  if ((_saveTimer) && TimeReached(_saveTimer)) {
    saveZigbeeDevices();
    _saveTimer = 0;
  }
}

// does the new payload conflicts with the existing payload, i.e. values would be overwritten
// true - one attribute (except LinkQuality) woudl be lost, there is conflict
// false - new attributes can be safely added
bool Z_Devices::jsonIsConflict(uint16_t shortaddr, const Z_attribute_list &attr_list) const {
  const Z_Device & device = findShortAddr(shortaddr);

  if (!foundDevice(device)) { return false; }
  if (attr_list.isEmpty()) {
    return false;                                           // if no previous value, no conflict
  }

  // compare groups
  if (device.attr_list.isValidGroupId() && attr_list.isValidGroupId()) {
    if (device.attr_list.group_id != attr_list.group_id) { return true; }     // groups are in conflict
  }

  // compare src_ep
  if (device.attr_list.isValidSrcEp() && attr_list.isValidSrcEp()) {
    if (device.attr_list.src_ep != attr_list.src_ep) { return true; }
  }
  
  // LQI does not count as conflicting

  // parse all other parameters
  for (const auto & attr : attr_list) {
    const Z_attribute * curr_attr = device.attr_list.findAttribute(attr);
    if (nullptr != curr_attr) {
      if (!curr_attr->equalsVal(attr)) {
        return true;    // the value already exists and is different - conflict!
      }
    }
  }
  return false;
}

void Z_Devices::jsonAppend(uint16_t shortaddr, const Z_attribute_list &attr_list) {
  Z_Device & device = getShortAddr(shortaddr);
  device.attr_list.mergeList(attr_list);
}

void Z_Devices::jsonPublishFlush(uint16_t shortaddr) {
  Z_Device & device = getShortAddr(shortaddr);
  if (!device.valid()) { return; }                 // safeguard
  Z_attribute_list &attr_list = device.attr_list;

  if (!attr_list.isEmpty()) {
    const char * fname = zigbee_devices.getFriendlyName(shortaddr);
    bool use_fname = (Settings.flag4.zigbee_use_names) && (fname);    // should we replace shortaddr with friendlyname?

    // save parameters is global variables to be used by Rules
    gZbLastMessage.device = shortaddr;                // %zbdevice%
    gZbLastMessage.groupaddr = attr_list.group_id;      // %zbgroup%
    gZbLastMessage.endpoint = attr_list.src_ep;    // %zbendpoint%

    mqtt_data[0] = 0; // clear string
    // Do we prefix with `ZbReceived`?
    if (!Settings.flag4.remove_zbreceived) {
      Response_P(PSTR("{\"" D_JSON_ZIGBEE_RECEIVED "\":"));
    }
    // What key do we use, shortaddr or name?
    if (use_fname) {
      Response_P(PSTR("%s{\"%s\":{"), mqtt_data, fname);
    } else {
      Response_P(PSTR("%s{\"0x%04X\":{"), mqtt_data, shortaddr);
    }
    // Add "Device":"0x...."
    Response_P(PSTR("%s\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\","), mqtt_data, shortaddr);
    // Add "Name":"xxx" if name is present
    if (fname) {
      Response_P(PSTR("%s\"" D_JSON_ZIGBEE_NAME "\":\"%s\","), mqtt_data, EscapeJSONString(fname).c_str());
    }
    // Add all other attributes
    Response_P(PSTR("%s%s}}"), mqtt_data, attr_list.toString().c_str());
    
    if (!Settings.flag4.remove_zbreceived) {
      Response_P(PSTR("%s}"), mqtt_data);
    }
    attr_list.reset();    // clear the attributes

    if (Settings.flag4.zigbee_distinct_topics) {
      if (Settings.flag4.zb_topic_fname && fname) {
        //Clean special characters and check size of friendly name
        char stemp[TOPSZ];
        strlcpy(stemp, (!strlen(fname)) ? MQTT_TOPIC : fname, sizeof(stemp));
        MakeValidMqtt(0, stemp);
        //Create topic with Prefix3 and cleaned up friendly name
        char frtopic[TOPSZ];
        snprintf_P(frtopic, sizeof(frtopic), PSTR("%s/%s/" D_RSLT_SENSOR), SettingsText(SET_MQTTPREFIX3), stemp);
        MqttPublish(frtopic, Settings.flag.mqtt_sensor_retain);
      } else {
        char subtopic[16];
        snprintf_P(subtopic, sizeof(subtopic), PSTR("%04X/" D_RSLT_SENSOR), shortaddr);
        MqttPublishPrefixTopic_P(TELE, subtopic, Settings.flag.mqtt_sensor_retain);
      }
    } else {
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
    }
    XdrvRulesProcess();     // apply rules
  }
}

void Z_Devices::jsonPublishNow(uint16_t shortaddr, Z_attribute_list &attr_list) {
  jsonPublishFlush(shortaddr);    // flush any previous buffer
  jsonAppend(shortaddr, attr_list);
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
  if (nullptr == param) { return BAD_SHORTADDR; }
  size_t param_len = strlen(param);
  char dataBuf[param_len + 1];
  strcpy(dataBuf, param);
  RemoveSpace(dataBuf);
  uint16_t shortaddr = BAD_SHORTADDR;    // start with unknown

  if (strlen(dataBuf) < 4) {
    // simple number 0..99
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 99)) {
      shortaddr = zigbee_devices.isKnownIndex(XdrvMailbox.payload - 1);
    }
  } else if ((dataBuf[0] == '0') && ((dataBuf[1] == 'x') || (dataBuf[1] == 'X'))) {
    // starts with 0x
    if (strlen(dataBuf) < 18) {
      // expect a short address
      shortaddr = strtoull(dataBuf, nullptr, 0);
      if (short_must_be_known) {
        shortaddr = zigbee_devices.findShortAddr(shortaddr).shortaddr;   // if not found, it reverts to the unknown_device with address BAD_SHORTADDR
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

// Display the tracked status for a light
String Z_Devices::dumpLightState(uint16_t shortaddr) const {
  Z_attribute_list attr_list;
  char hex[8];

  const Z_Device & device = findShortAddr(shortaddr);
  const char * fname = getFriendlyName(shortaddr);
  bool use_fname = (Settings.flag4.zigbee_use_names) && (fname);    // should we replace shortaddr with friendlyname?
  snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), shortaddr);

  attr_list.addAttribute(F(D_JSON_ZIGBEE_DEVICE)).setStr(hex);
  if (fname) {
    attr_list.addAttribute(F(D_JSON_ZIGBEE_NAME)).setStr(fname);
  }

  if (foundDevice(device)) {
    // dump all known values
    attr_list.addAttribute(F("Reachable")).setBool(device.getReachable());
    if (device.validPower())        { attr_list.addAttribute(F("Power")).setUInt(device.getPower()); }
    const Z_Data_Light & light = device.data.find<Z_Data_Light>(0);
    if (&light != nullptr) {
      light.toAttributes(attr_list, Z_Data_Light::type);
      // Exception, we need to convert Hue to 0..360 instead of 0..254
      if (light.validHue()) {
        attr_list.findOrCreateAttribute(PSTR("Hue")).setUInt(light.getHue());
      }
    }
    // Z_Data_Light::toAttributes(attr_list, device.data.find<Z_Data_Light>(0));
  }
  
  Z_attribute_list attr_list_root;
  Z_attribute * attr_root;
  if (use_fname) {
    attr_root = &attr_list_root.addAttribute(fname);
  } else {
    attr_root = &attr_list_root.addAttribute(hex);
  }
  attr_root->setStrRaw(attr_list.toString(true).c_str());
  return attr_list_root.toString(true);
}

// Dump the internal memory of Zigbee devices
// Mode = 1: simple dump of devices addresses
// Mode = 2: simple dump of devices addresses and names, endpoints, light
String Z_Devices::dump(uint32_t dump_mode, uint16_t status_shortaddr) const {
  Z_json_array json_arr;

  for (const auto & device : _devices) {
    uint16_t shortaddr = device.shortaddr;
    char hex[22];

    // ignore non-current device, if device specified
    if ((BAD_SHORTADDR != status_shortaddr) && (status_shortaddr != shortaddr)) { continue; }

    Z_attribute_list attr_list;

    snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), shortaddr);
    attr_list.addAttribute(F(D_JSON_ZIGBEE_DEVICE)).setStr(hex);

    if (device.friendlyName > 0) {
      attr_list.addAttribute(F(D_JSON_ZIGBEE_NAME)).setStr(device.friendlyName);
    }

    if (2 <= dump_mode) {
      hex[0] = '0';   // prefix with '0x'
      hex[1] = 'x';
      Uint64toHex(device.longaddr, &hex[2], 64);
      attr_list.addAttribute(F("IEEEAddr")).setStr(hex);
      if (device.modelId) {
        attr_list.addAttribute(F(D_JSON_MODEL D_JSON_ID)).setStr(device.modelId);
      }
      int8_t bulbtype = getHueBulbtype(shortaddr);
      if (bulbtype >= 0) {
        attr_list.addAttribute(F(D_JSON_ZIGBEE_LIGHT)).setInt(bulbtype);   // sign extend, 0xFF changed as -1
      }
      if (device.manufacturerId) {
        attr_list.addAttribute(F("Manufacturer")).setStr(device.manufacturerId);
      }
      Z_json_array arr_ep;
      for (uint32_t i = 0; i < endpoints_max; i++) {
        uint8_t endpoint = device.endpoints[i];
        if (0x00 == endpoint) { break; }
        arr_ep.add(endpoint);
      }
      attr_list.addAttribute(F("Endpoints")).setStrRaw(arr_ep.toString().c_str());
    }
    json_arr.addStrRaw(attr_list.toString(true).c_str());
  }
  return json_arr.toString();
}

// Restore a single device configuration based on json export
// Input: json element as expported by `ZbStatus2``
// Mandatory attribue: `Device`
//
// Returns:
//  0 : Ok
// <0 : Error
//
// Ex: {"Device":"0x5ADF","Name":"IKEA_Light","IEEEAddr":"0x90FD9FFFFE03B051","ModelId":"TRADFRI bulb E27 WS opal 980lm","Manufacturer":"IKEA of Sweden","Endpoints":["0x01","0xF2"]}
int32_t Z_Devices::deviceRestore(JsonParserObject json) {

  // params
  uint16_t device = 0x0000;                 // 0x0000 is coordinator so considered invalid
  uint64_t ieeeaddr = 0x0000000000000000LL; // 0 means unknown
  const char * modelid = nullptr;
  const char * manufid = nullptr;
  const char * friendlyname = nullptr;
  int8_t   bulbtype = -1;
  size_t   endpoints_len = 0;

  // read mandatory "Device"
  JsonParserToken val_device = json[PSTR("Device")];
  if (val_device) {
    device = (uint32_t) val_device.getUInt(device);
  } else {
    return -1;        // missing "Device" attribute
  }

  ieeeaddr      = json.getULong(PSTR("IEEEAddr"), ieeeaddr); // read "IEEEAddr" 64 bits in format "0x0000000000000000"
  friendlyname  = json.getStr(PSTR("Name"), nullptr);  // read "Name"
  modelid       = json.getStr(PSTR("ModelId"), nullptr);
  manufid       = json.getStr(PSTR("Manufacturer"), nullptr);
  JsonParserToken tok_bulbtype = json[PSTR(D_JSON_ZIGBEE_LIGHT)];

  // update internal device information
  updateDevice(device, ieeeaddr);
  if (modelid) { setModelId(device, modelid); }
  if (manufid) { setManufId(device, manufid); }
  if (friendlyname) { setFriendlyName(device, friendlyname); }
  if (tok_bulbtype) { setLightProfile(device, tok_bulbtype.getInt()); }

  // read "Endpoints"
  JsonParserToken val_endpoints = json[PSTR("Endpoints")];
  if (val_endpoints.isArray()) {
    JsonParserArray arr_ep = JsonParserArray(val_endpoints);
    clearEndpoints(device);     // clear even if array is empty
    for (auto ep_elt : arr_ep) {
      uint8_t ep = ep_elt.getUInt();
      if (ep) { addEndpoint(device, ep); }
    }
  }

  return 0;
}

Z_Data_Light & Z_Devices::getLight(uint16_t shortaddr) {
  return getShortAddr(shortaddr).data.get<Z_Data_Light>();
}

/*********************************************************************************************\
 * Export device specific attributes to ZbData
\*********************************************************************************************/
void Z_Device::toAttributes(Z_attribute_list & attr_list) const {
  if (validLqi())             { attr_list.addAttribute(PSTR(D_CMND_ZIGBEE_LINKQUALITY)).setUInt(lqi); }
  if (validBatteryPercent())  { attr_list.addAttribute(PSTR("BatteryPercentage")).setUInt(batterypercent); }
  if (validLastSeen())        { attr_list.addAttribute(PSTR("LastSeen")).setUInt(last_seen); }
}

/*********************************************************************************************\
 * Device specific data handlers
\*********************************************************************************************/
void Z_Device::setPower(bool power_on, uint8_t ep) {
  data.get<Z_Data_OnOff>(ep).setPower(power_on);
}

bool Z_Device::validPower(uint8_t ep) const {
  const Z_Data_OnOff & onoff = data.find<Z_Data_OnOff>(ep);
  return (&onoff != nullptr);
}

bool Z_Device::getPower(uint8_t ep) const {
  const Z_Data_OnOff & onoff = data.find<Z_Data_OnOff>(ep);
  if (&onoff != nullptr)  return onoff.getPower();
  return false;
}

#endif // USE_ZIGBEE
