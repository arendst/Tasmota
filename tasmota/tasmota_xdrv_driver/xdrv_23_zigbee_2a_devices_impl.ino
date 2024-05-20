/*
  xdrv_23_zigbee_2a_devices_impl.ino - zigbee support for Tasmota

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
 * Implementation
\*********************************************************************************************/

Z_Device & Z_Devices::devicesAt(size_t i) const {
  Z_Device * devp = (Z_Device*) _devices.at(i);
  if (devp) {
    return *devp;
  } else {
    return device_unk;
  }
}

//
// Create a new Z_Device entry in _devices. Only to be called if you are sure that no
// entry with same shortaddr or longaddr exists.
//
Z_Device & Z_Devices::createDeviceEntry(uint16_t shortaddr, uint64_t longaddr) {
  if ((BAD_SHORTADDR == shortaddr) && !longaddr) { return device_unk; }      // it is not legal to create this entry
  Z_Device & device = _devices.addToLast();
  device.shortaddr = shortaddr;
  device.longaddr = longaddr;

  dirty();
  return device;
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
  return device_unk;
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
  if (!longaddr) { return device_unk; }
  for (auto &elem : _devices) {
    if (elem.longaddr == longaddr) { return elem; }
  }
  return device_unk;
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

//
// Scan all devices to find a corresponding friendlyNme
// Looks info device.friendlyName entry or the name of an endpoint
// In:
//    friendlyName (null terminated, should not be empty)
// Out:
//    index in _devices of entry, -1 if not found
//    ep == 0 means ep not found
//
int32_t Z_Devices::findFriendlyNameOrEPName(const char * name, uint8_t * ep) const {
  if (ep) { *ep = 0; }
  if (!name) { return -1; }              // if pointer is null
  size_t name_len = strlen(name);
  int32_t found = 0;
  if (name_len) {
    for (auto &elem : _devices) {
      if (elem.friendlyName) {
        if (strcasecmp(elem.friendlyName, name) == 0) { return found; }
      }
      uint8_t ep_found = elem.ep_names.findEPName(name);
      if (ep_found) {
        // found via ep name
        if (ep) { *ep = ep_found; }   // update ep
        return found;
      }
      found++;
    }
  }
  return -1;
}

Z_Device & Z_Devices::isKnownLongAddrDevice(uint64_t longaddr) const {
  return (Z_Device &) findLongAddr(longaddr);
}

Z_Device & Z_Devices::isKnownIndexDevice(uint32_t index) const {
  if (index < devicesSize()) {
    return devicesAt(index);
  } else {
    return device_unk;
  }
}

Z_Device & Z_Devices::isKnownFriendlyNameDevice(const char * name, uint8_t * ep) const {
  if ((!name) || (0 == strlen(name))) { return device_unk; }         // Error
  uint8_t ep_found;
  int32_t found = findFriendlyNameOrEPName(name, &ep_found);
  if (found >= 0) {
    if (ep) { *ep = ep_found; }
    return devicesAt(found);
  } else {
    return device_unk;
  }
}

uint64_t Z_Devices::getDeviceLongAddr(uint16_t shortaddr) const {
  return findShortAddr(shortaddr).longaddr;     // if unknown, it reverts to the Unknown device and longaddr is 0x00
}

//
// We have a seen a shortaddr on the network, get the corresponding device object
//
Z_Device & Z_Devices::getShortAddr(uint16_t shortaddr) {
  if (BAD_SHORTADDR == shortaddr) { return device_unk; }   // this is not legal
  Z_Device & device = findShortAddr(shortaddr);
  if (foundDevice(device)) {
    return device;
  }
  return createDeviceEntry(shortaddr, 0);
}

// find the Device object by its longaddr (unique key if not null)
Z_Device & Z_Devices::getLongAddr(uint64_t longaddr) {
  if (!longaddr) { return device_unk; }
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
Z_Device & Z_Devices::updateDevice(uint16_t shortaddr, uint64_t longaddr) {
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
      return *l_found;
    }
  } else if (foundDevice(*s_found)) {
    // shortaddr already exists but longaddr not
    // add the longaddr to the entry
    s_found->longaddr = longaddr;
    dirty();
    return *s_found;
  } else if (foundDevice(*l_found)) {
    // longaddr entry exists, update shortaddr
    l_found->shortaddr = shortaddr;
    dirty();
    return *l_found;
  } else {
    // neither short/lonf addr are found.
    if ((BAD_SHORTADDR != shortaddr) || longaddr) {
      return createDeviceEntry(shortaddr, longaddr);
    }
    return device_unk;
  }
  return device_unk;
}

// Clear the router flag for each device, called at the beginning of ZbMap
void Z_Devices::clearDeviceRouterInfo(void) {
  for (Z_Device & device : zigbee_devices._devices) {
    if (device.valid()) {
      device.setRouter(false);
    }
  }
}

//
// Clear all endpoints
//
void Z_Device::clearEndpoints(void) {
  for (uint32_t i = 0; i < endpoints_max; i++) {
    endpoints[i] = 0;
    // no dirty here because it doesn't make sense to store it, does it?
  }
}

//
// Add an endpoint to a shortaddr
// return true if a change was made
//
bool Z_Device::addEndpoint(uint8_t endpoint) {
  if ((0x00 == endpoint) || (endpoint > 240 && endpoint != 0xF2)) { return false; }

  for (uint32_t i = 0; i < endpoints_max; i++) {
    if (endpoint == endpoints[i]) {
      return false;     // endpoint already there
    }
    if (0 == endpoints[i]) {
      endpoints[i] = endpoint;
      return true;
    }
  }
  return false;
}

//
// Count the number of known endpoints
//
uint32_t Z_Device::countEndpoints(void) const {
  uint32_t count_ep = 0;
  for (uint32_t i = 0; i < endpoints_max; i++) {
    if (0 != endpoints[i]) {
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

// set a name to an endpoint, must exist in the list or return `false`
bool Z_Device::setEPName(uint8_t ep, const char * name) {
  if ((0x00 == ep) || (ep > 240 && ep != 0xF2)) { return false; }

  for (uint32_t i = 0; i < endpoints_max; i++) {
    if (ep == endpoints[i]) {
      ep_names.setEPName(ep, name);
      return true;
    }
  }
  return false;
}

void Z_Device::setStringAttribute(char*& attr, const char * str) {
  if (nullptr == str)  { str = ""; }    // nullptr is considered empty string, don't use PROGMEM to avoid crash
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
  zigbee_devices.dirty();
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
void Z_Device::setManufId(const char * str) {
  setStringAttribute(manufacturerId, str);
}

void Z_Device::setModelId(const char * str) {
  setStringAttribute(modelId, str);
}

void Z_Device::setFriendlyName(const char * str) {
  setStringAttribute(friendlyName, str);
}

void Z_Device::setFriendlyEPName(uint8_t ep, const char * str) {
  ep_names.setEPName(ep, str);
}

// needs to push the implementation here to use Z_Device static method
void Z_EP_Name::setName(const char *new_name) {
  Z_Device::setStringAttribute(name, new_name);
}

void Z_Device::setLastSeenNow(void) {
  // Only update time if after 2020-01-01 0000.
  // Fixes issue where zigbee device pings before WiFi/NTP has set utc_time
  // to the correct time, and "last seen" calculations are based on the
  // pre-corrected last_seen time and the since-corrected utc_time.
  if (Rtc.utc_time < START_VALID_TIME) { return; }
  last_seen = Rtc.utc_time;
}

void Z_Devices::deviceWasReached(uint16_t shortaddr) {
  // since we just receveived data from the device, it is reachable
  zigbee_devices.resetTimersForDevice(shortaddr, 0 /* groupaddr */, Z_CAT_REACHABILITY);    // remove any reachability timer already there
  Z_Device & device = findShortAddr(shortaddr);
  if (device.valid()) {
    device.setReachable(true);     // mark device as reachable
  }
}

void Z_Devices::deviceHasNoBattery(uint16_t shortaddr) {
  Z_Device & device = findShortAddr(shortaddr);
  if (device.valid()) {
    device.setHasNoBattery();     // mark device as reachable
  }
}

// get the next sequance number for the device, or use the global seq number if device is unknown
uint8_t Z_Devices::getNextSeqNumber(uint16_t shortaddr) {
  Z_Device & device = findShortAddr(shortaddr);
  if (foundDevice(device)) {
    device.seqNumber += 1;
    return device.seqNumber;
  } else {
    _seqnumber += 1;
    return _seqnumber;
  }
}

// returns: dirty flag, did we change the value of the object
void Z_Device::setLightChannels(int8_t channels, uint8_t ep) {
  if (channels >= 0) { 
    if (ep) {   // if ep is not zero, the endpoint must exist
      bool found = false;
      for (uint32_t i = 0; i < endpoints_max; i++) {
        if (ep == endpoints[i]) { found = true; break; }
      }
      if (!found) {
        AddLog(LOG_LEVEL_INFO, D_LOG_ZIGBEE "cannot set light type to unknown ep=%i", ep);
        return;
      }
    } else {
      // if ep == 0, use first endpoint, or zero if no endpoint is known
      ep = endpoints[0];
    }
    // retrieve of create light object
    Z_Data_Light & light = data.get<Z_Data_Light>(ep);
    if (channels != light.getConfig()) {
      light.setConfig(channels);
      zigbee_devices.dirty();
    }
    Z_Data_OnOff & onoff = data.get<Z_Data_OnOff>(ep);
    (void)onoff;
  } else {
    // remove light / onoff object if any
    for (auto & data_elt : data) {
      if ((data_elt.getType() == Z_Data_Type::Z_Light) ||
          (data_elt.getType() == Z_Data_Type::Z_OnOff)) {
        if (ep == 0 || data_elt.getEndpoint() == ep) {    // if remove ep==0 then remove all definitions
          // remove light object
          data.remove(&data_elt);
          zigbee_devices.dirty();
        }
      }
    }
  }
}

int8_t Z_Devices::getHueBulbtype(uint16_t shortaddr, uint8_t ep) const {
  const Z_Device &device = findShortAddr(shortaddr);
  return device.getHueBulbtype(ep);
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
  if (device.attr_list.validGroupId() && attr_list.validGroupId()) {
    if (device.attr_list.group_id != attr_list.group_id) { return true; }     // groups are in conflict
  }

  // compare src_ep
  if (device.attr_list.validSrcEp() && attr_list.validSrcEp()) {
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

//
// internal function to publish device information with respect to all `SetOption`s
//
void Z_Device::jsonPublishAttrList(const char * json_prefix, const Z_attribute_list &attr_list, bool include_time) const {
  const char * local_friendfly_name;     // friendlyname publish can depend on the source endpoint
  local_friendfly_name = ep_names.getEPName(attr_list.src_ep);    // check if this ep has a specific name
  if (local_friendfly_name == nullptr) {
    // if no ep-specific name, get the device name
    local_friendfly_name = friendlyName;
  }
  bool use_fname = (Settings->flag4.zigbee_use_names) && (local_friendfly_name);    // should we replace shortaddr with friendlyname?

  ResponseClear(); // clear string

  // Do we prefix with `ZbReceived`?
  if (!Settings->flag4.remove_zbreceived && !Settings->flag5.zb_received_as_subtopic) {
    if (include_time && Rtc.utc_time >= START_VALID_TIME) {
      // Add time if needed (and if time is valide)
      ResponseAppendTimeFormat(Settings->flag2.time_format);
      ResponseAppend_P(PSTR(",\"%s\":"), json_prefix);
    } else {
      ResponseAppend_P(PSTR("{\"%s\":"), json_prefix);
    }
  }

  // What key do we use, shortaddr or name?
  if (!Settings->flag5.zb_omit_json_addr) {
    if (use_fname) {
      ResponseAppend_P(PSTR("{\"%s\":"), local_friendfly_name);
    } else {
      ResponseAppend_P(PSTR("{\"0x%04X\":"), shortaddr);
    }
  }
  ResponseAppend_P(PSTR("{"));

  // Add "Device":"0x...."
  ResponseAppend_P(PSTR("\"" D_JSON_ZIGBEE_DEVICE "\":\"0x%04X\","), shortaddr);
  // Add "Name":"xxx" if name is present
  if (local_friendfly_name) {
    ResponseAppend_P(PSTR("\"" D_JSON_ZIGBEE_NAME "\":\"%s\","), EscapeJSONString(local_friendfly_name).c_str());
  }
  // Add all other attributes
  ResponseAppend_P(PSTR("%s}"), attr_list.toString(false).c_str());

  if (!Settings->flag5.zb_omit_json_addr) {
    ResponseAppend_P(PSTR("}"));
  }

  if (!Settings->flag4.remove_zbreceived && !Settings->flag5.zb_received_as_subtopic) {
    ResponseAppend_P(PSTR("}"));
  }

#ifdef USE_INFLUXDB
  InfluxDbProcess(1);        // Use a copy of ResponseData
#endif

  if (Settings->flag4.zigbee_distinct_topics) {
    char subtopic[TOPSZ];
    if (Settings->flag4.zb_topic_fname && local_friendfly_name && strlen(local_friendfly_name)) {
      // Clean special characters
      char stemp[TOPSZ];
      strlcpy(stemp, local_friendfly_name, sizeof(stemp));
      MakeValidMqtt(0, stemp);
      if (Settings->flag5.zigbee_hide_bridge_topic) {
        snprintf_P(subtopic, sizeof(subtopic), PSTR("%s"), stemp);
      } else {
        snprintf_P(subtopic, sizeof(subtopic), PSTR("%s/%s"), TasmotaGlobal.mqtt_topic, stemp);
      }
    } else {
      if (Settings->flag5.zigbee_hide_bridge_topic) {
        snprintf_P(subtopic, sizeof(subtopic), PSTR("%04X"), shortaddr);
      } else {
        snprintf_P(subtopic, sizeof(subtopic), PSTR("%s/%04X"), TasmotaGlobal.mqtt_topic, shortaddr);
      }
    }
    if (Settings->flag5.zb_topic_endpoint) {
      if (attr_list.validSrcEp()) {
        snprintf_P(subtopic, sizeof(subtopic), PSTR("%s_%d"), subtopic, attr_list.src_ep);
      }
    }
    char stopic[TOPSZ];
    if (Settings->flag5.zb_received_as_subtopic)
      GetTopic_P(stopic, TELE, subtopic, json_prefix);
    else
      GetTopic_P(stopic, TELE, subtopic, PSTR(D_RSLT_SENSOR));
    MqttPublish(stopic, Settings->flag.mqtt_sensor_retain);
  } else {
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings->flag.mqtt_sensor_retain);
  }
  XdrvRulesProcess(0);     // apply rules
}

void Z_Devices::jsonPublishFlush(uint16_t shortaddr) {
  Z_Device & device = getShortAddr(shortaddr);
  if (!device.valid()) { return; }                 // safeguard
  Z_attribute_list &attr_list = device.attr_list;

  if (!attr_list.isEmpty()) {
    // save parameters is global variables to be used by Rules
    gZbLastMessage.device = shortaddr;                // %zbdevice%
    gZbLastMessage.groupaddr = attr_list.group_id;      // %zbgroup%
    gZbLastMessage.endpoint = attr_list.src_ep;    // %zbendpoint%

    // add battery percentage from last known value
    if (device.validBatteryPercent()) {
      attr_list.setBattPercent(device.batt_percent);
    }

    device.jsonPublishAttrList(PSTR(D_JSON_ZIGBEE_RECEIVED), attr_list, Settings->flag5.zigbee_include_time);
    attr_list.reset();    // clear the attributes
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
//
// In case the device is not found, the parsed 0x.... short address is passed to *parsed_shortaddr
Z_Device & Z_Devices::parseDeviceFromName(const char * param, uint16_t * parsed_shortaddr, uint8_t * ep, int32_t mailbox_payload) {
  if (ep) { *ep = 0; }   // mark as not found
  if (nullptr == param) { return device_unk; }
  size_t param_len = strlen(param);
  char dataBuf[param_len + 1];
  strcpy(dataBuf, param);
  TrimSpace(dataBuf);
  if (parsed_shortaddr != nullptr) { *parsed_shortaddr = BAD_SHORTADDR; }   // if it goes wrong, mark as bad

  if ((dataBuf[0] >= '0') && (dataBuf[0] <= '9') && (strlen(dataBuf) < 4)) {
    // simple number 0..99
    if ((mailbox_payload > 0) && (mailbox_payload <= 99)) {
      return isKnownIndexDevice(mailbox_payload - 1);
    } else {
      return device_unk;
    }
  } else if ((dataBuf[0] == '0') && ((dataBuf[1] == 'x') || (dataBuf[1] == 'X'))) {
    // starts with 0x
    if (strlen(dataBuf) < 18) {
      // expect a short address
      uint16_t shortaddr = strtoull(dataBuf, nullptr, 0);
      if (parsed_shortaddr != nullptr) { *parsed_shortaddr = shortaddr; }   // return the parsed shortaddr even if the device doesn't exist
      return (Z_Device&) findShortAddr(shortaddr);   // if not found, it reverts to the unknown_device with address BAD_SHORTADDR
    } else {
      // expect a long address
      uint64_t longaddr = strtoull(dataBuf, nullptr, 0);
      return isKnownLongAddrDevice(longaddr);
    }
  } else {
    // expect a Friendly Name
    return isKnownFriendlyNameDevice(dataBuf, ep);
  }
}

/*********************************************************************************************\
 *
 * Methods below build a JSON representation of device data
 * Used by: ZbLight, ZbStatus, ZbInfo
 *
\*********************************************************************************************/

// Add "Device":"0x1234","Name":"FrienflyName"
void Z_Device::jsonAddDeviceNamme(Z_attribute_list & attr_list) const {
  const char * fname = friendlyName;

  attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_DEVICE)).setHex32(shortaddr);
  if (fname) {
    attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_NAME)).setStr(fname);
  }
}

// Add "Names":{"1":"name1","2":"name2"}
void Z_Device::jsonAddEPName(Z_attribute_list & attr_list) const {
  String s = ep_names.tojson();
  if (s.length() > 0) {
    attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_NAMES)).setStrRaw(s.c_str());
  }
}

// Add "IEEEAddr":"0x1234567812345678"
void Z_Device::jsonAddIEEE(Z_attribute_list & attr_list) const {
  attr_list.addAttributePMEM(PSTR("IEEEAddr")).setHex64(longaddr);
}
// Add "ModelId":"","Manufacturer":""
void Z_Device::jsonAddModelManuf(Z_attribute_list & attr_list) const {
  if (modelId) {
    attr_list.addAttributePMEM(PSTR(D_JSON_MODEL D_JSON_ID)).setStr(modelId);
  }
  if (manufacturerId) {
    attr_list.addAttributePMEM(PSTR("Manufacturer")).setStr(manufacturerId);
  }
}

// Add "Endpoints":[...]
void Z_Device::jsonAddEndpoints(Z_attribute_list & attr_list) const {
  JsonGeneratorArray arr_ep;
  for (uint32_t i = 0; i < endpoints_max; i++) {
    uint8_t endpoint = endpoints[i];
    if (0x00 == endpoint) { break; }
    arr_ep.add((uint32_t)endpoint);
  }
  attr_list.addAttributePMEM(PSTR("Endpoints")).setStrRaw(arr_ep.toString().c_str());
}
// Add "Config":["",""...]
void Z_Device::jsonAddConfig(Z_attribute_list & attr_list) const {
  JsonGeneratorArray arr_data;
  for (auto & data_elt : data) {
    char key[8];
    if (data_elt.validConfig()) {
      snprintf_P(key, sizeof(key), PSTR("?%02X.%1X"), data_elt.getEndpoint(), data_elt.getConfig());
    } else {
      snprintf_P(key, sizeof(key), PSTR("?%02X"), data_elt.getEndpoint());
    }
    key[0] = Z_Data::DataTypeToChar(data_elt.getType());
    arr_data.addStr(key);
  }
  attr_list.addAttributePMEM(PSTR("Config")).setStrRaw(arr_data.toString().c_str());
}
// Add "HueEmulation":false, "Matter":false
void Z_Device::jsonAddEmulation(Z_attribute_list & attr_list) const {
  if (!isAdvertizeHue())      { attr_list.addAttributePMEM(PSTR("HueEmulation")).setBool(false); }
  if (!isAdvertizeMatter())   { attr_list.addAttributePMEM(PSTR("Matter")).setBool(false); }
}
// Add All data attributes
void Z_Device::jsonAddDataAttributes(Z_attribute_list & attr_list) const {
  // show internal data - mostly last known values
  for (auto & data_elt : data) {
    data_elt.toAttributes(attr_list);
    if (data_elt.getType() == Z_Data_Type::Z_Light) {   // since we don't have virtual methods, do an explicit test
      ((Z_Data_Light&)data_elt).toRGBAttributes(attr_list);
    }
  }
}
// Add "BatteryPercentage", "LastSeen", "LastSeenEpoch", "LinkQuality", "HueEmulation" (opt) and "Matter" (opt)
void Z_Device::jsonAddDeviceAttributes(Z_attribute_list & attr_list) const {
  attr_list.addAttributePMEM(PSTR("Reachable")).setBool(getReachable());
  if (validBatteryPercent())  { attr_list.addAttributePMEM(PSTR("BatteryPercentage")).setUInt(batt_percent); }
  if (validBattLastSeen()) {
    attr_list.addAttributePMEM(PSTR("BatteryLastSeenEpoch")).setUInt(batt_last_seen);
  }
  if (validLastSeen())        {
    if (Rtc.utc_time >= last_seen) {
      attr_list.addAttributePMEM(PSTR("LastSeen")).setUInt(Rtc.utc_time - last_seen);
    }
    attr_list.addAttributePMEM(PSTR("LastSeenEpoch")).setUInt(last_seen);
  }
  if (validLqi())             { attr_list.addAttributePMEM(PSTR(D_CMND_ZIGBEE_LINKQUALITY)).setUInt(lqi); }
}


// Display the tracked status for a light
void Z_Device::jsonLightState(Z_attribute_list & attr_list) const {
  if (valid()) {
    // dump all known values
    attr_list.addAttributePMEM(PSTR("Reachable")).setBool(getReachable());
    if (validPower())        { attr_list.addAttributePMEM(PSTR("Power")).setUInt(getPower()); }
    int32_t light_mode = -1;
    const Z_Data_Light & light = data.find<Z_Data_Light>(0);
    if (&light != &z_data_unk) {
      if (light.validConfig()) {
        light_mode = light.getConfig();
      }
      light.toAttributes(attr_list);
      // Exception, we need to convert Hue to 0..360 instead of 0..254
      if (light.validHue()) {
        attr_list.findOrCreateAttribute(PSTR("Hue")).setUInt(light.getHue());
      }
      light.toRGBAttributes(attr_list);
    }
    attr_list.addAttributePMEM(PSTR("Light")).setInt(light_mode);
  }
}

// Dump the internal memory of Zigbee devices - does not include "Device" and "Name"
// Mode = 1: simple dump of devices addresses
// Mode = 2: simple dump of devices addresses and names, endpoints, light
// Mode = 3: dump last known data attributes
// String Z_Device::dumpSingleDevice(uint32_t dump_mode, bool add_device_name, bool add_brackets) const {
void Z_Device::jsonDumpSingleDevice(Z_attribute_list & attr_list, uint32_t dump_mode, bool add_name) const {
  if (add_name) {
    jsonAddDeviceNamme(attr_list);
  }
  if (dump_mode >= 2) {
    jsonAddEPName(attr_list);
    jsonAddIEEE(attr_list);
    jsonAddModelManuf(attr_list);
    jsonAddEndpoints(attr_list);
    jsonAddConfig(attr_list);
    jsonAddEmulation(attr_list);
  }
  if (dump_mode >= 3) {
    jsonAddDataAttributes(attr_list);
    // add device wide attributes
    jsonAddDeviceAttributes(attr_list);
  }
}

// Dump coordinator specific data
String Z_Devices::dumpCoordinator(void) const {
  Z_attribute_list attr_list;

  attr_list.addAttributePMEM(PSTR(D_JSON_ZIGBEE_DEVICE)).setHex32(localShortAddr);
  attr_list.addAttributePMEM(PSTR("IEEEAddr")).setHex64(localIEEEAddr);
  attr_list.addAttributePMEM(PSTR("TotalDevices")).setUInt(zigbee_devices.devicesSize());

  return attr_list.toString(true);
}

// If &device == nullptr, then dump all
String Z_Devices::dumpDevice(uint32_t dump_mode, const Z_Device & device) const {
  JsonGeneratorArray json_arr;

  if (&device == &device_unk) {
    if (dump_mode < 2) {
      // dump light mode for all devices
      for (const auto & device2 : _devices) {
        Z_attribute_list attr_list;
        device2.jsonDumpSingleDevice(attr_list, dump_mode, true);
        json_arr.addStrRaw(attr_list.toString(true).c_str());
      }
    }
  } else {
    Z_attribute_list attr_list;
    device.jsonDumpSingleDevice(attr_list, dump_mode, true);
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
// Ex: {"Device":"0x5ADF","Name":"IKEA_Light","IEEEAddr":"0x90FD9FFFFE03B051","ModelId":"TRADFRI bulb E27 WS opal 980lm","Manufacturer":"IKEA of Sweden","Endpoints":["0x01","0xF2"],"HueEmulation":false}
int32_t Z_Devices::deviceRestore(JsonParserObject json) {

  // params
  uint16_t shortaddr = 0x0000;                 // 0x0000 is coordinator so considered invalid
  uint64_t ieeeaddr = 0x0000000000000000LL; // 0 means unknown
  const char * modelid = nullptr;
  const char * manufid = nullptr;
  const char * friendlyname = nullptr;

  // read mandatory "Device"
  JsonParserToken val_device = json[PSTR("Device")];
  if (val_device) {
    shortaddr = (uint32_t) val_device.getUInt(shortaddr);
  } else {
    return -1;        // missing "Device" attribute
  }

  ieeeaddr      = json.getULong(PSTR("IEEEAddr"), ieeeaddr); // read "IEEEAddr" 64 bits in format "0x0000000000000000"
  friendlyname  = json.getStr(PSTR("Name"), nullptr);  // read "Name"
  modelid       = json.getStr(PSTR("ModelId"), nullptr);
  manufid       = json.getStr(PSTR("Manufacturer"), nullptr);

  // update internal device information
  updateDevice(shortaddr, ieeeaddr);
  Z_Device & device = getShortAddr(shortaddr);
  if (modelid) { device.setModelId(modelid); }
  if (manufid) { device.setManufId(manufid); }
  if (friendlyname) { device.setFriendlyName(friendlyname); }

  // read "Endpoints"
  JsonParserToken val_endpoints = json[PSTR("Endpoints")];
  if (val_endpoints.isArray()) {
    JsonParserArray arr_ep = JsonParserArray(val_endpoints);
    device.clearEndpoints();     // clear even if array is empty
    for (auto ep_elt : arr_ep) {
      uint8_t ep = ep_elt.getUInt();
      if (ep) { device.addEndpoint(ep); }
    }
  }

  // read "Names"
  JsonParserToken val_names = json[PSTR("Names")];
  if (val_names.isObject()) {
    JsonParserObject attr_names = val_names.getObject();
    // iterate on keys
    for (auto key : attr_names) {
      int32_t ep = key.getUInt();
      if (ep > 255) { ep = 0; }   // ep == 0 is invalid
      const char * ep_name = key.getValue().getStr();
      if (!ep || !device.setEPName(ep, ep_name)) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "ignoring ep=%i name='%s'"), ep, ep_name);
      }
    }
  }


  // read "Config"
  JsonParserToken val_config = json[PSTR("Config")];
  if (val_config.isArray()) {
    JsonParserArray arr_config = JsonParserArray(val_config);
    device.data.reset();  // remove existing configuration
    for (auto config_elt : arr_config) {
      const char * conf_str = config_elt.getStr();
      Z_Data_Type data_type;
      uint8_t ep = 0;
      uint8_t config = 0xF;   // default = no config

      if (Z_Data::ConfigToZData(conf_str, &data_type, &ep, &config)) {
        Z_Data & data = device.data.getByType(data_type, ep);
        if (&data != nullptr) {
          data.setConfig(config);
        }
      } else {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Ignoring config '%s'"), conf_str);
      }
    }
  }

  // read "HueEmulation" and "Matter"
  bool hue_emulation = json.getBool(PSTR("HueEmulation"), false);
  bool matter        = json.getBool(PSTR("Matter"), false);
  device.setAdvertizeHue(hue_emulation);
  device.setAdvertizeMatter(matter);

  return 0;
}

Z_Data_Light & Z_Devices::getLight(uint16_t shortaddr, uint8_t ep) {
  return getShortAddr(shortaddr).data.get<Z_Data_Light>(ep);
}

bool Z_Devices::isTuyaProtocol(uint16_t shortaddr, uint8_t ep) const {
  const Z_Device & device = findShortAddr(shortaddr);
  if (device.valid()) {
    const Z_Data_Mode & mode = device.data.getConst<Z_Data_Mode>(ep);
    if (&mode != nullptr) {
      return mode.isTuyaProtocol();
    }
  }
  return false;
}

/*********************************************************************************************\
 * Device specific data handlers
\*********************************************************************************************/
void Z_Device::setPower(bool power_on, uint8_t ep) {
  data.get<Z_Data_OnOff>(ep).setPower(power_on);
}

bool Z_Device::validPower(uint8_t ep) const {
  const Z_Data_OnOff & onoff = data.find<Z_Data_OnOff>(ep);
  return (&onoff != &z_data_unk);
}

bool Z_Device::getPower(uint8_t ep) const {
  const Z_Data_OnOff & onoff = data.find<Z_Data_OnOff>(ep);
  if (&onoff != &z_data_unk)  return onoff.getPower();
  return false;
}

#endif // USE_ZIGBEE
