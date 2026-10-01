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
  var utc_time, local_time, now
  def init()
    self.utc_time = 1
    self.local_time = 1700000000
    self.now = 0
  end
  def rtc_utc() return self.utc_time end
  def rtc(mode) return self.local_time end
  def millis() return self.now end
  def time_reached(deadline) return self.now >= deadline end
  def loglevel(level) return false end
end
tasmota = TestTasmota()
var test_tasmota = tasmota
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

class TestTimedSession
  var local_session_id
  def init(local_session_id)
    self.local_session_id = local_session_id
  end
end

class TestGarageEvents
  var published
  def init() self.published = [] end
  def publish_event(endpoint, cluster, event_id, urgent, priority, data0, data1, data2)
    self.published.push(event_id)
  end
end

class TestGarageDevice
  var tick, events
  def init()
    self.tick = 1
    self.events = TestGarageEvents()
  end
  def attribute_updated(endpoint, cluster, attribute, fabric_specific) end
end

class TestGarageTasmota
  var locked, fail, position, direction, target, inverted
  def init()
    self.locked = false
    self.fail = false
    self.position = 50
    self.direction = 1
    self.target = 100
    self.inverted = false
  end
  def shutter_state()
    return {"Shutter1": {"Position": self.position, "Direction": self.direction, "Target": self.target, "Tilt": 0}}
  end
  def cmd(command, mute)
    if command == "Status 13"
      return {"StatusSHT": {"SHT0": {"Opt": self.inverted ? "00001" : "00000"}}}
    elif command == "ShutterPosition1"
      return self.shutter_state()
    end
    if self.fail  return nil end
    if self.locked
      var response = {}
      response[command] = "Locked"
      return response
    end
    if command == "ShutterStop1"
      # Real driver: idle Stop answers ResponseCmndDone() without index
      if self.direction == 0  return {"ShutterStop": "Done"} end
      self.direction = 0
      self.target = self.position
    elif command == "ShutterClose1"
      self.direction = -1
      self.target = 0
    elif command == "ShutterOpen1"
      self.direction = 1
      self.target = 100
    else
      return nil
    end
    return self.shutter_state()
  end
  def loglevel(level) return false end
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

# Timed interactions are keyed by exchange ID, expire, and are consumed once.
var im = matter.IM(device)
var timed_session = TestTimedSession(10)
var timed_msg = TestMessage(55, timed_session)
tasmota.now = 100
im.timed_exchanges[timed_msg.exchange_id] = 200
assert(im.check_timed_request(timed_msg, true))
assert(!im.check_timed_request(timed_msg, true))
assert(size(im.timed_exchanges) == 0)
im.timed_exchanges[timed_msg.exchange_id] = 99
assert(!im.check_timed_request(timed_msg, true))
assert(size(im.timed_exchanges) == 0)
print("  timed interactions are one-shot and expire: OK")

# A locked or failed Tasmota shutter command must not be reported as Matter
# SUCCESS, and a rejected command must not mutate the signature-position flag.
var garage_tasmota = TestGarageTasmota()
tasmota = garage_tasmota
var garage_device = TestGarageDevice()
var garage = matter.Plugin_GarageDoor(garage_device, 1, {"shutter": 0})
garage.update_shadow()
garage.shadow_signature = true
garage_tasmota.locked = true
ctx = matter.Path(1, 0x0104, nil)
ctx.command = 0
assert(garage.invoke_request(nil, nil, ctx) == nil)
assert(ctx.status == 0xCB)
assert(garage.shadow_signature)
assert(garage_tasmota.direction == 1)

var move_to = TLV.Matter_TLV_struct()
move_to.add_TLV(0, 0x04, garage.TP_CLOSE)
ctx = matter.Path(1, 0x0104, nil)
ctx.command = 1
assert(garage.invoke_request(nil, move_to, ctx) == nil)
assert(ctx.status == 0xCB)
assert(garage.shadow_signature)

garage_tasmota.locked = false
garage_tasmota.fail = true
ctx = matter.Path(1, 0x0104, nil)
ctx.command = 0
assert(garage.invoke_request(nil, nil, ctx) == nil)
assert(ctx.status == 0x01)
assert(garage.shadow_signature)

garage_tasmota.fail = false
ctx = matter.Path(1, 0x0104, nil)
ctx.command = 0
assert(garage.invoke_request(nil, nil, ctx) == true)
assert(ctx.status == nil)
assert(!garage.shadow_signature)
assert(garage_tasmota.direction == 0)

# Stop on an idle door returns `{"ShutterStop":"Done"}` and is SUCCESS. It
# preserves OpenedAtSignature because no movement or target changed.
garage_tasmota.position = 100
garage_tasmota.target = 100
garage.set_signature(true)
garage.update_shadow()
assert(garage.current_position_enum() == garage.CP_SIGNATURE)
ctx = matter.Path(1, 0x0104, nil)
ctx.command = 0
assert(garage.invoke_request(nil, nil, ctx) == true)
assert(ctx.status == nil)
assert(garage.shadow_signature)
assert(garage.current_position_enum() == garage.CP_SIGNATURE)
assert(garage.target_position_enum() == garage.TP_SIGNATURE)

# ShutterInvert can change at runtime. The new inversion and the matching
# Tasmota position must be applied atomically, without reporting open as secure.
garage.set_signature(false)
assert(garage.current_position_enum() == garage.CP_OPENED)
garage_tasmota.inverted = true
garage_tasmota.position = 0
garage_tasmota.target = 0
garage.update_shadow()
assert(garage.shadow_shutter_inverted == 1)
assert(garage.current_position_enum() == garage.CP_OPENED)
ctx = matter.Path(1, 0x0104, 3)
assert(garage.read_attribute(nil, ctx, TLV.Matter_TLV_item()).findsubval(3) == false)

# A malformed scalar CommandFields value is INVALID_COMMAND, not an uncaught
# attribute_error while looking for fields on a non-structure TLV item.
var malformed_move_to = TLV.Matter_TLV_item().set(0x06, garage.TP_CLOSE)
ctx = matter.Path(1, 0x0104, nil)
ctx.command = 1
assert(garage.invoke_request(nil, malformed_move_to, ctx) == nil)
assert(ctx.status == 0x85)
print("  garage shutter command rejection is propagated to Matter: OK")
tasmota = test_tasmota

# Virtual garage doors never call the Tasmota shutter backend. Matter commands
# complete locally, while MtrUpdate can model an externally driven movement.
class TestVirtualGarageTasmota
  def cmd(command, mute) assert(false, "virtual garage called tasmota.cmd") end
  def loglevel(level) return false end
end
matter.publish_command = def (prefix, endpoint, name, payload) end
tasmota = TestVirtualGarageTasmota()
var virtual_garage = matter.Plugin_Virt_GarageDoor(garage_device, 2, {})
assert(virtual_garage.VIRTUAL)
assert(virtual_garage.UPDATE_COMMANDS.find("ShutterPos") != nil)
assert(virtual_garage.UPDATE_COMMANDS.find("ShutterTarget") != nil)
assert(virtual_garage.UPDATE_COMMANDS.find("ShutterDirection") != nil)
assert(virtual_garage.shadow_shutter_inverted == 0)
assert(virtual_garage.current_position_enum() == virtual_garage.CP_CLOSED)
assert(virtual_garage.target_position_enum() == virtual_garage.TP_CLOSE)
assert(!virtual_garage.is_moving())

garage_device.events.published.clear()
move_to = TLV.Matter_TLV_struct()
move_to.add_TLV(0, 0x04, virtual_garage.TP_OPEN)
ctx = matter.Path(2, 0x0104, nil)
ctx.command = 1
assert(virtual_garage.invoke_request(nil, move_to, ctx) == true)
assert(virtual_garage.current_position_enum() == virtual_garage.CP_OPENED)
assert(virtual_garage.target_position_enum() == virtual_garage.TP_OPEN)
assert(!virtual_garage.is_moving())
assert(garage_device.events.published.find(0x01) != nil)  # MovementCompleted
assert(garage_device.events.published.find(0x03) != nil)  # SecureStateChanged
assert(string.find(virtual_garage.state_json(), '"ShutterDirection":0') != nil)

move_to = TLV.Matter_TLV_struct()
move_to.add_TLV(0, 0x04, virtual_garage.TP_SIGNATURE)
ctx = matter.Path(2, 0x0104, nil)
ctx.command = 1
assert(virtual_garage.invoke_request(nil, move_to, ctx) == true)
assert(virtual_garage.current_position_enum() == virtual_garage.CP_SIGNATURE)
assert(virtual_garage.target_position_enum() == virtual_garage.TP_SIGNATURE)

virtual_garage.update_virtual({
  "ShutterPos": 50,
  "ShutterTarget": 0,
  "ShutterDirection": -3
})
assert(virtual_garage.current_position_enum() == virtual_garage.CP_PARTIAL)
assert(virtual_garage.target_position_enum() == virtual_garage.TP_CLOSE)
assert(virtual_garage.shadow_shutter_direction == -1)
assert(virtual_garage.is_moving())

ctx = matter.Path(2, 0x0104, nil)
ctx.command = 0
assert(virtual_garage.invoke_request(nil, nil, ctx) == true)
assert(!virtual_garage.is_moving())
assert(virtual_garage.shadow_shutter_target == 50)
assert(virtual_garage.target_position_enum() == nil)

virtual_garage.update_virtual({
  "ShutterPos": -10,
  "ShutterTarget": 120,
  "ShutterDirection": 0
})
assert(virtual_garage.shadow_shutter_pos == 0)
assert(virtual_garage.shadow_shutter_target == 100)
assert(virtual_garage.current_position_enum() == virtual_garage.CP_CLOSED)
assert(virtual_garage.target_position_enum() == virtual_garage.TP_OPEN)
print("  virtual garage door commands and updates: OK")
tasmota = test_tasmota

# Keep this test group scoped independently from the main regression script.
def test_generic_switch()
  # Generic Switch events are recorded with endpoint and typed fields, so the
  # tests exercise the real dispatch and event construction without network I/O.
  class TestButtonEvents
    var published
    def init() self.published = [] end
    def publish_event(endpoint, cluster, event_id, urgent, priority, data0, data1, data2)
      self.published.push({
        "endpoint": endpoint, "cluster": cluster, "event": event_id,
        "urgent": urgent, "priority": priority,
        "data0": data0 ? data0.val : nil, "type0": data0 ? data0.typ : nil,
        "data1": data1 ? data1.val : nil, "type1": data1 ? data1.typ : nil,
        "data2": data2 ? data2.val : nil
      })
    end
  end

  class TestButtonDevice : matter.Device
    var updates
    def init()
      self.plugins = []
      self.events = TestButtonEvents()
      self.updates = []
    end
    def attribute_updated(endpoint, cluster, attribute, fabric_specific)
      self.updates.push([endpoint, cluster, attribute])
    end
  end

  class TestButtonTasmota : TestTasmota
    var response
    def find_key_i(object, name)
      for key : object.keys()
        if string.toupper(key) == string.toupper(name) return key end
      end
      return nil
    end
    def find_list_i(values, name)
      var i = 0
      while i < size(values)
        if string.toupper(values[i]) == string.toupper(name) return i end
        i += 1
      end
      return nil
    end
    def resp_cmnd(value) self.response = value end
    def resp_cmnd_str(value) self.response = value end
    def resp_cmnd_done() self.response = "Done" end
  end

  # The event sequence and its fields are the contract consumed by controllers:
  # press/release for each tap, ongoing count from tap 2, then one final count.
  def assert_button_sequence(events, endpoint, presses)
    assert(size(events) == presses * 3)
    var cursor = 0
    for press : 1..presses
      assert(events[cursor]["event"] == 1)
      assert(events[cursor]["data1"] == nil)
      cursor += 1
      if press > 1
        assert(events[cursor]["event"] == 5)
        assert(events[cursor]["data1"] == press)
        assert(events[cursor]["type1"] >= 0x04 && events[cursor]["type1"] <= 0x07)
        cursor += 1
      end
      assert(events[cursor]["event"] == 3)
      assert(events[cursor]["data1"] == nil)
      cursor += 1
    end
    assert(events[cursor]["event"] == 6)
    assert(events[cursor]["data1"] == presses)
    assert(events[cursor]["type1"] >= 0x04 && events[cursor]["type1"] <= 0x07)
    for event : events
      assert(event["endpoint"] == endpoint)
      assert(event["cluster"] == 0x003B)
      assert(event["urgent"])
      assert(event["priority"] == 1)
      assert(event["data0"] == 1 && event["type0"] >= 0x04 && event["type0"] <= 0x07)
      assert(event["data2"] == nil)
    end
  end

  var button_device = TestButtonDevice()
  var button_one = matter.Plugin_Sensor_GenericSwitch_Btn(button_device, 33, {"button": 1, "name": 'Button "one"'})
  var button_two = matter.Plugin_Sensor_GenericSwitch_Btn(button_device, 34, {"button": 2, "name": "Button two"})
  button_device.plugins = [button_one, button_two]
  ctx = matter.Path(33, 0x003B, 1)
  var position = button_one.read_attribute(nil, ctx, TLV.Matter_TLV_item())
  assert(position.typ >= 0x04 && position.typ <= 0x07 && position.val == 0, "new button CurrentPosition must be a non-null released value")
  assert(button_two.shadow_position == 0)
  assert(json.load(button_one.state_json())["Switch"] == 0)
  assert(json.load(button_one.state_json())["Name"] == 'Button "one"')

  # Device dispatch broadcasts callbacks; only the configured button may react.
  button_device.button_handler(9, 1, 1, 0)
  assert(size(button_device.events.published) == 0)
  assert(size(button_device.updates) == 0)
  button_device.button_handler(1, 1, 1, 0)
  assert(button_one.shadow_position == 1 && button_two.shadow_position == 0)
  assert(json.load(button_one.state_json())["Switch"] == 1)
  button_device.button_handler(1, 1, 0, 0)
  button_device.button_handler(1, 2, 0, 1)
  assert_button_sequence(button_device.events.published, 33, 1)
  assert(button_one.shadow_position == 0 && button_two.shadow_position == 0)
  for update : button_device.updates assert(update[0] == 33) end
  button_device.events.published.clear()
  button_device.updates.clear()
  button_device.button_handler(2, 1, 1, 0)
  button_device.button_handler(2, 1, 0, 0)
  button_device.button_handler(2, 2, 0, 1)
  assert_button_sequence(button_device.events.published, 34, 1)
  for update : button_device.updates assert(update[0] == 34) end

  ctx = matter.Path(33, 0x003B, 0xFFFA)
  var advertised_events = []
  for event : button_one.read_attribute(nil, ctx, TLV.Matter_TLV_item()).val
    advertised_events.push(event.val)
  end
  assert(size(advertised_events) == 4)
  for event_id : [1, 3, 5, 6] assert(advertised_events.find(event_id) != nil) end
  ctx.attribute = 0xFFFC
  assert(button_one.read_attribute(nil, ctx, TLV.Matter_TLV_item()).val == 0x16)
  ctx.attribute = 2
  assert(button_one.read_attribute(nil, ctx, TLV.Matter_TLV_item()).val == 5)
  print("  local Generic Switch initialization, routing and metadata: OK")

  # Virtual buttons reuse the declared event contract and remain released after
  # each completed sequence. Identical requests represent distinct user actions.
  var virtual_button = matter.Plugin_Virt_Sensor_GenericSwitch_Btn(button_device, 35, {"name": "Virtual button"})
  var virtual_button_two = matter.Plugin_Virt_Sensor_GenericSwitch_Btn(button_device, 36, {"name": "Other virtual button"})
  button_device.plugins.push(virtual_button)
  button_device.plugins.push(virtual_button_two)
  assert(virtual_button.VIRTUAL && virtual_button.TYPE == "v_gensw")
  assert(virtual_button.UPDATE_COMMANDS.find("Presses") != nil)
  ctx = matter.Path(35, 0x003B, 1)
  position = virtual_button.read_attribute(nil, ctx, TLV.Matter_TLV_item())
  assert(position.typ >= 0x04 && position.typ <= 0x07 && position.val == 0)
  for presses : 1..5
    button_device.events.published.clear()
    virtual_button.update_virtual({"Presses": presses})
    assert_button_sequence(button_device.events.published, 35, presses)
    for event : button_device.events.published
      assert(advertised_events.find(event["event"]) != nil)
    end
    assert(virtual_button.shadow_position == 0 && virtual_button_two.shadow_position == 0)
    assert(json.load(virtual_button.state_json())["Switch"] == 0)
  end
  button_device.events.published.clear()
  virtual_button.update_virtual({"Presses": 1})
  virtual_button.update_virtual({"Presses": 1})
  assert(size(button_device.events.published) == 6)
  assert(button_device.events.published[2]["event"] == 6)
  assert(button_device.events.published[5]["event"] == 6)
  var prior_updates = size(button_device.updates)
  for invalid : [0, 6, -1, 1.0, 1.5, "1", true, nil, [], {}]
    assert(type(virtual_button.update_virtual({"Presses": invalid})) == "string")
    assert(size(button_device.events.published) == 6)
    assert(size(button_device.updates) == prior_updates)
    assert(virtual_button.shadow_position == 0)
  end
  assert(virtual_button.update_virtual({}) == nil)
  assert(size(button_device.events.published) == 6)
  assert(size(button_device.updates) == prior_updates)

  # Physical callbacks must not turn a virtual button into a second copy of a
  # hardware button, even when its inherited default button index happens to fit.
  button_device.events.published.clear()
  button_device.button_handler(1, 1, 1, 0)
  button_device.button_handler(1, 1, 0, 0)
  button_device.button_handler(1, 2, 0, 1)
  assert_button_sequence(button_device.events.published, 33, 1)
  assert(virtual_button.shadow_position == 0 && virtual_button_two.shadow_position == 0)
  print("  virtual Generic Switch sequences, validation and isolation: OK")

  # Exercise the real MtrUpdate dispatch/filtering path, not just update_virtual.
  # Case-insensitive keys, numeric endpoint selection and exact friendly-name
  # selection must each reach one endpoint and return valid, released state JSON.
  tasmota = TestButtonTasmota()
  button_device.events.published.clear()
  button_device.MtrUpdate("MtrUpdate", 1, "", {"ep": 35, "pReSsEs": 2})
  assert_button_sequence(button_device.events.published, 35, 2)
  var button_response = json.load(tasmota.response)["MtrUpdate"]
  assert(button_response["Ep"] == 35 && button_response["Switch"] == 0)
  button_device.events.published.clear()
  button_device.MtrUpdate("MtrUpdate", 1, "", {"NAME": "Other virtual button", "Presses": 1})
  assert_button_sequence(button_device.events.published, 36, 1)
  button_response = json.load(tasmota.response)["MtrUpdate"]
  assert(button_response["Ep"] == 36 && button_response["Switch"] == 0)
  button_device.events.published.clear()
  button_device.MtrUpdate("MtrUpdate", 1, "", {"Ep": 35, "Presses": 1, "Unexpected": 1})
  assert(size(button_device.events.published) == 0)
  assert(string.find(tasmota.response, "Invalid attribute") != nil)
  prior_updates = size(button_device.updates)
  for invalid : [0, 6, -1, 1.0, 1.5, "1", true, nil, [], {}]
    button_device.MtrUpdate("MtrUpdate", 1, "", {"Ep": 35, "Presses": invalid})
    assert(string.find(tasmota.response, "expected integer 1..5") != nil)
    assert(size(button_device.events.published) == 0)
    assert(size(button_device.updates) == prior_updates)
    assert(virtual_button.shadow_position == 0 && virtual_button_two.shadow_position == 0)
  end
  button_device.MtrUpdate("MtrUpdate", 1, "", {"Ep": 35})
  button_response = json.load(tasmota.response)["MtrUpdate"]
  assert(button_response["Ep"] == 35 && button_response["Switch"] == 0)
  assert(size(button_device.events.published) == 0)
  assert(size(button_device.updates) == prior_updates)
  button_device.MtrUpdate("MtrUpdate", 1, "", {"Ep": 33, "Presses": 1})
  assert(size(button_device.events.published) == 0)
  assert(tasmota.response == "Device is not virtual")
  tasmota = test_tasmota
  print("  MtrUpdate virtual button routing and response: OK")
end
test_generic_switch()

# ConfigurationVersion changes with endpoint composition and is persisted.
var config_device = TestConfigurationDevice()
config_device.signal_endpoints_changed()
assert(config_device.configuration_version == 2)
assert(config_device.updates == 2)
print("  dynamic ConfigurationVersion: OK")

print("Matter fixed-ACL and Group Key Management tests: OK")
