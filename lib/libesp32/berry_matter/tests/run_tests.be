#!/usr/bin/env -S ../../berry/berry -g
#
# Focused host-side tests for the Matter 1.6.1 metadata, fixed Tasmota ACL
# profile, Group Key Management, and Groups persistence.
#

import global
import os
import string
import sys

sys.path().push("../src/embedded")
import matter

# Firmware globals required while compiling the embedded sources.
var globs = "path,ctypes_bytes_dyn,tasmota,ccronexpr,gpio,light,webclient,load,MD5,lv,light_state,udp,tcpclientasync,log,"
            "lv_clock,lv_clock_icon,lv_signal_arcs,lv_signal_bars,lv_wifi_arcs_icon,lv_wifi_arcs,"
            "lv_wifi_bars_icon,_lvgl,"
for name : string.split(globs, ",")
  global.(name) = nil
end

# Load embedded sources in the same order as solidify_all.be.
var files = os.listdir("../src/embedded")
for i : 1..size(files)-1
  var value = files[i]
  var j = i
  while j > 0 && files[j-1] > value
    files[j] = files[j-1]
    j -= 1
  end
  files[j] = value
end
for name : files
  if name[0] == '.' continue end
  var source_file = open("../src/embedded/" + name)
  var source = source_file.read()
  source_file.close()
  compile(source)()
end
matter.get_attribute_name = def (cluster, attribute) return nil end

class TestTasmota
  var utc_time, local_time
  def init()
    self.utc_time = 1
    self.local_time = 1700000000
  end
  def rtc_utc() return self.utc_time end
  def rtc(mode) return self.local_time end
  def millis() return 0 end
  def loglevel(level) return false end
end
tasmota = TestTasmota()
log = def (*args) end
var crypto = module("crypto")

class TestCounter
  var value
  def init() self.value = 0 end
  def next() self.value += 1 return self.value end
  def reset(value) self.value = value ? value : 0 end
  def val() return self.value end
  static def is_greater(left, right) return left > right end
end
matter.Counter = TestCounter

class TestFabric : matter.Fabric
  def persist_pre() end
  def persist_post() end
  def does_persist() return true end
end
matter.Fabric = TestFabric

# Avoid hardware-facing Root initialization while testing protocol helpers.
class TestRoot : matter.Plugin_Root
  def init() self.endpoint = 0 end
end

class TestStore
  var saves, fabrics
  def init()
    self.saves = 0
    self.fabrics = []
  end
  def save_fabrics() self.saves += 1 end
end

class TestSession
  var fabric
  def init(fabric) self.fabric = fabric end
  def is_PASE() return false end
  def get_fabric() return self.fabric end
  def get_fabric_index() return self.fabric ? self.fabric.get_fabric_index() : nil end
end

class TestDevice
  static var GROUP_TRANSPORT_READY = true
  var plugins, configuration_version, sessions
  def init(plugin)
    self.plugins = [plugin]
    self.configuration_version = 1
  end
  def attribute_updated(endpoint, cluster, attribute, fabric_specific) end
  def k2l(value)
    var result = []
    for key : value.keys() result.push(key) end
    for i : 1..size(result)-1
      var current = result[i]
      var j = i
      while j > 0 && result[j-1] > current
        result[j] = result[j-1]
        j -= 1
      end
      result[j] = current
    end
    return result
  end
end

class TestNoGroupDevice
  static var GROUP_TRANSPORT_READY = false
end

class TestEndpointPlugin : matter.Plugin_Device
  def init(device)
    self.device = device
    self.endpoint = 1
  end
end

class TestMessage
  var exchange_id, session
  def init(exchange_id, session)
    self.exchange_id = exchange_id
    self.session = session
  end
end

class TestConfigurationDevice : matter.Device
  var configuration_version, updates
  def init()
    self.configuration_version = 1
    self.updates = 0
  end
  def attribute_updated(endpoint, cluster, attribute, fabric_specific)
    self.updates += 1
  end
  def get_active_endpoints(exclude_zero) return [] end
end

var TLV = matter.TLV
var root = TestRoot()
var store = TestStore()
var fabric = matter.Fabric(store)
fabric.set_fabric_index(1)
var session = TestSession(fabric)
var device = TestDevice(root)
device.sessions = store
root.device = device
store.fabrics = [fabric]

# Matter 1.6.1 metadata and GKM attributes are directly readable.
var ctx = matter.Path(0, 0x0028, 0x0000)
assert(root.read_attribute(session, ctx, TLV.Matter_TLV_item()).val == 21)
ctx.attribute = 0x0015
assert(root.read_attribute(session, ctx, TLV.Matter_TLV_item()).val == 0x01060100)
ctx.attribute = 0x0018
assert(root.read_attribute(session, ctx, TLV.Matter_TLV_item()).val == 1)
assert(matter.StatusResponseMessage().InteractionModelRevision == 13)

# Time Synchronization attributes use the Matter epoch (2000-01-01), not Unix.
tasmota.utc_time = 1790675381
tasmota.local_time = 1790678981
ctx = matter.Path(0, 0x0038, 0x0000)
assert(root.read_attribute(session, ctx, TLV.Matter_TLV_item()).val ==
       int64.fromstring("843990581000000"))
ctx.attribute = 0x0007
assert(root.read_attribute(session, ctx, TLV.Matter_TLV_item()).val ==
       int64.fromstring("843994181000000"))
tasmota.utc_time = 1
ctx.attribute = 0x0000
assert(root.read_attribute(session, ctx, TLV.Matter_TLV_item()).typ == 0x14)
print("  Matter epoch conversion for UTC and local time: OK")

ctx = matter.Path(0, 0x003F, 0xFFFB)
var attribute_list = root.read_attribute(session, ctx, TLV.Matter_TLV_item())
var attribute_ids = []
for item : attribute_list.val attribute_ids.push(item.val) end
assert(attribute_ids.find(0) != nil)
assert(attribute_ids.find(1) != nil)
assert(attribute_ids.find(2) != nil)
assert(attribute_ids.find(3) != nil)
print("  Matter 1.6.1 metadata and GKM declaration: OK")

# Tasmota exposes one fixed wildcard CASE/Administer entry. Commissioner ACL
# writes are successful compatibility no-ops and cannot change that entry.
ctx = matter.Path(0, 0x001F, 0)
var acl = root.read_attribute(session, ctx, TLV.Matter_TLV_item())
assert(size(acl.val) == 1)
var acl_entry = acl.val[0]
assert(acl_entry.findsubval(1) == 5)
assert(acl_entry.findsubval(2) == 2)
assert(acl_entry.findsub(3).typ == 0x14)
assert(acl_entry.findsub(4).typ == 0x14)
assert(acl_entry.findsubval(0xFE) == 1)
var commissioner_acl = TLV.Matter_TLV_array()
var commissioner_entry = commissioner_acl.add_struct(nil)
commissioner_entry.add_TLV(1, 0x04, 5)
commissioner_entry.add_TLV(2, 0x04, 2)
var commissioner_subjects = commissioner_entry.add_array(3)
commissioner_subjects.add_TLV(nil, 0x07, int64.fromu32(0x12345678))
commissioner_entry.add_TLV(4, 0x14, nil)
assert(root.write_attribute(session, ctx, commissioner_acl.val))
assert(ctx.status == nil)
var acl_after_write = root.read_attribute(session, ctx, TLV.Matter_TLV_item())
assert(size(acl_after_write.val) == 1)
assert(acl_after_write.val[0].findsubval(1) == 5)
assert(acl_after_write.val[0].findsubval(2) == 2)
assert(acl_after_write.val[0].findsub(3).typ == 0x14)
assert(acl_after_write.val[0].findsub(4).typ == 0x14)
ctx.attribute = 4
assert(root.read_attribute(session, ctx, TLV.Matter_TLV_item()).val == 1)
print("  fixed wildcard ACL and commissioner write compatibility: OK")

# AttributePathIB must preserve absent, null (append), and numeric ListIndex.
var path_tlv = TLV.Matter_TLV_list()
path_tlv.add_TLV(2, 0x05, 0)
path_tlv.add_TLV(3, 0x06, 0x003F)
path_tlv.add_TLV(4, 0x06, 0)
var decoded_path = matter.AttributePathIB().from_TLV(path_tlv)
assert(!decoded_path.list_index_present)
path_tlv.add_TLV(5, 0x14, nil)
decoded_path = matter.AttributePathIB().from_TLV(path_tlv)
assert(decoded_path.list_index_present && decoded_path.list_index_is_null)
print("  ListIndex decoding for chunked GroupKeyMap writes: OK")

# Install a valid key set and verify persistence restores uint64 epoch times.
var write = TLV.Matter_TLV_struct()
var key_set = write.add_struct(0)
key_set.add_TLV(0, 0x05, 7)
key_set.add_TLV(1, 0x04, 0)
key_set.add_TLV(2, 0x10, bytes("00112233445566778899AABBCCDDEEFF"))
key_set.add_TLV(3, 0x07, int64(1000))
key_set.add_TLV(4, 0x14, nil)
key_set.add_TLV(5, 0x14, nil)
key_set.add_TLV(6, 0x14, nil)
key_set.add_TLV(7, 0x14, nil)
ctx = matter.Path(0, 0x003F, nil)
ctx.command = 0
assert(root.invoke_request(session, write, ctx) == true)
assert(fabric.find_group_key_set(7) != nil)

import json
var persisted = json.load(json.dump(fabric.get_group_key_sets()))
var reloaded = matter.Fabric(store)
reloaded.group_key_sets = persisted
reloaded.hydrate_post()
assert(isinstance(reloaded.find_group_key_set(7).find("time0"), int64))

# Malformed persisted epoch strings are ignored instead of aborting hydration
# and preventing this fabric and every following fabric from loading.
assert(matter.Fabric.uint64_from_json("damaged") == nil)
var damaged = matter.Fabric(store)
damaged.group_key_sets = [{"id": 8, "time0": "damaged"}]
damaged.hydrate_post()
assert(damaged.find_group_key_set(8) != nil)
print("  Group key-set write and persistence: OK")

# KeySetRead redacts key material and KeySetReadAllIndices includes IPK ID zero.
write = TLV.Matter_TLV_struct()
write.add_TLV(0, 0x05, 7)
ctx.command = 1
var response = root.invoke_request(session, write, ctx)
assert(ctx.command == 2)
assert(response.findsub(0).findsubval(0) == 7)
assert(response.findsub(0).findsub(2).typ == 0x14)
ctx.command = 4
response = root.invoke_request(session, TLV.Matter_TLV_struct(), ctx)
assert(ctx.command == 5)
assert(response.findsub(0).val[0].val == 0)
print("  Group key-set read and index enumeration: OK")

# GroupKeyMap uses atomic empty-list-plus-append staging.
var map_entry = TLV.Matter_TLV_struct()
map_entry.add_TLV(1, 0x05, 2)
map_entry.add_TLV(2, 0x05, 7)
var msg = TestMessage(40, session)
root.begin_write_request(msg)
ctx = matter.Path(0, 0x003F, 0)
ctx.list_index_present = false
ctx.list_write_final = false
assert(root.write_attribute(session, ctx, []))
ctx.list_index_present = true
ctx.list_index_is_null = true
ctx.list_write_final = true
ctx.write_tlv = map_entry
assert(root.write_attribute(session, ctx, map_entry.val))
root.end_write_request(msg, false)
assert(size(fabric.get_group_key_map()) == 1)
assert(fabric.get_group_key_map()[0].find("group_id") == 2)

# A pending map is also committed when the final message has no map operation.
var map_entry_2 = TLV.Matter_TLV_struct()
map_entry_2.add_TLV(1, 0x05, 3)
map_entry_2.add_TLV(2, 0x05, 7)
msg = TestMessage(41, session)
root.begin_write_request(msg)
ctx.list_index_present = false
ctx.list_index_is_null = false
ctx.list_write_final = false
assert(root.write_attribute(session, ctx, []))
ctx.list_index_present = true
ctx.list_index_is_null = true
ctx.write_tlv = map_entry_2
assert(root.write_attribute(session, ctx, map_entry_2.val))
root.end_write_request(msg, true)
root.begin_write_request(TestMessage(41, session))
root.end_write_request(TestMessage(41, session), false)
assert(fabric.get_group_key_map()[0].find("group_id") == 3)
print("  atomic and multi-message GroupKeyMap writes: OK")

# Removing a key set also removes mappings that reference it.
ctx = matter.Path(0, 0x003F, nil)
ctx.command = 3
write = TLV.Matter_TLV_struct()
write.add_TLV(0, 0x05, 7)
assert(root.invoke_request(session, write, ctx) == true)
assert(fabric.find_group_key_set(7) == nil)
assert(size(fabric.get_group_key_map()) == 0)
print("  Group key-set removal and map cleanup: OK")

# Production remains fail-closed until encrypted multicast receive is ready.
var no_group_device = TestNoGroupDevice()
root.device = no_group_device
ctx = matter.Path(0, 0x003F, 2)
assert(root.read_attribute(session, ctx, TLV.Matter_TLV_item()).val == 0)
ctx.attribute = 0
ctx.list_index_present = false
ctx.list_write_final = true
assert(!root.write_attribute(session, ctx, []))
assert(ctx.status == 0x88)
var endpoint_plugin = TestEndpointPlugin(no_group_device)
var add_group = TLV.Matter_TLV_struct()
add_group.add_TLV(0, 0x05, 4)
add_group.add_TLV(1, 0x0C, "Unavailable")
ctx = matter.Path(1, 0x0004, nil)
ctx.command = 0
assert(endpoint_plugin.invoke_request(session, add_group, ctx).findsubval(0) == 1)

# AddGroupIfIdentifying is mandatory and must succeed as a no-op when the
# endpoint is not identifying, even while multicast transport is disabled.
var add_if_identifying = TLV.Matter_TLV_struct()
add_if_identifying.add_TLV(0, 0x05, 4)
add_if_identifying.add_TLV(1, 0x0C, "Unavailable")
ctx.command = 5
assert(endpoint_plugin.invoke_request(session, add_if_identifying, ctx) == true)
add_if_identifying = TLV.Matter_TLV_struct()
add_if_identifying.add_TLV(0, 0x05, 0)
ctx.command = 5
assert(endpoint_plugin.invoke_request(session, add_if_identifying, ctx) == nil)
assert(ctx.status == 0x87)
print("  unavailable group transport fails closed: OK")

# ConfigurationVersion changes with endpoint composition and is persisted.
var config_device = TestConfigurationDevice()
config_device.signal_endpoints_changed()
assert(config_device.configuration_version == 2)
assert(config_device.updates == 2)
print("  dynamic ConfigurationVersion: OK")

print("Matter fixed-ACL and Group Key Management tests: OK")
