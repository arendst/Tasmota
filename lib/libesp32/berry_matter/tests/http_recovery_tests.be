#!/usr/bin/env -S ../../berry/berry -g
# Run from this directory: ../../berry/berry -g http_recovery_tests.be
# Optional first argument: baseline directory containing Matter_HTTP_remote.be
# Matter_zz_Device.be and Matter_Plugin_1_Device.be. The baseline must fail the first recovery assertion.
# Production notification routing, HTTP callbacks, plugins, subscriptions and
# ReportData TLV encoding run. Hardware initialization, time, random generation,
# encrypted frames and transport are simulated. No network or persistence.
import global
import os
import string
import sys
import introspect
sys.path().push("../src/embedded")
import matter
var globs = "path,ctypes_bytes_dyn,tasmota,ccronexpr,gpio,light,webclient,load,MD5,lv,light_state,udp,tcpclientasync,log,"
            "lv_clock,lv_clock_icon,lv_signal_arcs,lv_signal_bars,lv_wifi_arcs_icon,lv_wifi_arcs,"
            "lv_wifi_bars_icon,_lvgl,"
for name : string.split(globs, ",") global.(name) = nil end
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
  var directory = "../src/embedded/"
  if size(_argv) > 1 && (name == "Matter_HTTP_remote.be" || name == "Matter_zz_Device.be" || name == "Matter_Plugin_1_Device.be")
    directory = _argv[1] + "/"
  end
  var file = open(directory + name)
  var source = file.read()
  file.close()
  compile(source)()
end
class TestTasmota
  var now, utc
  def init() self.now = 0 self.utc = 1700000000 end
  def millis(offset) return self.now + (offset == nil ? 0 : offset) end
  def rtc_utc() return self.utc end
  def rtc(mode) return self.utc end
  def time_reached(deadline) return self.now >= deadline end
  def loglevel(level) return false end
end
tasmota = TestTasmota()
log = def (*args) end
var crypto = module("crypto")
var next_id = 0
crypto.random = def (count)
  next_id += 1
  return bytes().add(next_id, count)
end
introspect.setmodule("crypto", crypto)
# Replace only network construction; state transitions and callbacks are real.
class MemoryRemote : matter.HTTP_remote
  def init(device)
    self.device = device
    self.reachable = false
    self.probe_update_time_map = {}
    self.probe_next_timestamp_map = {}
    self.async_cb_map = {}
    self.info = {}
  end
end
class MQTTBoundary
  var reachable
  def init() self.reachable = true end
  def add_async_cb(cb, cmd) end
end
# Only persistent counter reservation is replaced; event queuing and TLV are real.
class MemoryEventHandler : matter.EventHandler
  def load_event_no_persisted()
    if self.counter_event_no == nil self.counter_event_no = int64(0) end
    self.counter_event_no_persisted = self.counter_event_no.add(self.EVENT_NO_INCR)
  end
end
class MemoryDevice : matter.Device
  var notifications
  def init(native)
    self.plugins = []
    self.http_remotes = {}
    self.plugins_config_remotes = {}
    self.notifications = []
    self.events = MemoryEventHandler(self)
    self.tick = 1
    self.debug = false
    self.disable_bridge_mode = native == true
  end
  def register_http_remote(addr, timeout)
    if !self.http_remotes.contains(addr)
      self.http_remotes[addr] = MemoryRemote(self)
    end
    return self.http_remotes[addr]
  end
  def register_mqtt_remote(topic) return MQTTBoundary() end
  def attribute_updated(endpoint, cluster, attribute, fabric_specific)
    self.notifications.push([endpoint, cluster, attribute])
    super(self).attribute_updated(endpoint, cluster, attribute, fabric_specific)
  end
end
class PlainPlugin
  # Root and other plugins need not contain an http_remote member.
  def get_endpoint() return 0 end
end
class Session
  var _message_handler, local_session_id, counter, exchange
  def init(handler)
    self._message_handler = handler
    self.local_session_id = 1
    self.counter = 10
    self.exchange = 20
  end
  def next_counter() self.counter += 1 return self.counter end
  def next_exchange() self.exchange += 1 return self.exchange end
end
class Frame
  var session, exchange_id, opcode, message_counter, payload
  var x_flag_r, ack_message_counter
  def encode_frame(payload) self.payload = payload end
  def encrypt() end
  static def initiate_response(handler, session, opcode, reliable)
    var frame = Frame()
    frame.session = session
    frame.exchange_id = session.next_exchange()
    frame.message_counter = session.next_counter()
    frame.opcode = opcode
    frame.x_flag_r = reliable
    return frame
  end
end
matter.Frame = Frame
class Transport
  var device, im, sent
  def init(device)
    self.device = device
    self.im = matter.IM(device)
    self.sent = []
  end
  def send_response_frame(frame)
    self.sent.push({"opcode": frame.opcode, "payload": bytes(frame.payload.tohex())})
  end
end
class Fixture
  var device, transport, remote, session, sub
  def init(native)
    tasmota.now = 0
    self.device = MemoryDevice(native)
    self.transport = Transport(self.device)
    self.device.message_handler = self.transport
    self.session = Session(self.transport)
    self.device.plugins.push(PlainPlugin())
    for endpoint : 17..19
      self.device.plugins.push(matter.Plugin_Bridge_OnOff(self.device, endpoint,
                                {"url": "192.0.2.1", "relay": endpoint - 15}))
    end
    self.remote = self.device.http_remotes["192.0.2.1"]
    self.device.plugins.push(matter.Plugin_Bridge_OnOff(self.device, 20,
                              {"url": "192.0.2.2", "relay": 1}))
    self.device.plugins.push(matter.Plugin_Bridge_OnOff(self.device, 21,
                              {"topic": "other-device", "relay": 1}))
    self.device.plugins.push(matter.Plugin_OnOff(self.device, 22, {"relay": 1}))
    # Model an established controller subscription to all Reachable attributes.
    # Initial subscription handshakes are outside this recovery test.
    var req = matter.SubscribeRequestMessage()
    req.min_interval_floor = 0
    req.max_interval_ceiling = 60
    req.fabric_filtered = false
    var path = matter.AttributePathIB()
    path.cluster = 0x0039
    path.attribute = 0x0011
    req.attributes_requests = [path]
    self.sub = self.transport.im.subs_shop.new_subscription(self.session, req)
    var events = matter.EventGenerator(self.device)
    events.start(nil, 0x0039, 0x03, int64(0))
    self.sub.set_event_generator_or_arr(events)
    self.sub.re_arm()
  end
  def assert_notifications(count, context)
    assert(size(self.device.notifications) == count,
           (context == nil ? "" : context + ": ") + "HTTP reachability change did not notify exactly the shared endpoints")
    for index : 0..count-1
      var update = self.device.notifications[index]
      assert(update[0] == 17 + index % 3, "unrelated endpoint was notified")
      assert(update[1] == 0x0039 && update[2] == 0x0011, "wrong attribute was notified")
    end
  end
  def assert_events(states)
    var count = size(states) * 3
    var queue = self.device.events.queue_debug
    assert(size(queue) == count, "missing or duplicate ReachableChanged events")
    assert(size(self.device.events.queue_info) == 0 && size(self.device.events.queue_critical) == 0)
    assert(self.device.events.get_last_event_no() == int64(count), "unexpected event sequence")
    for index : 0..count-1
      var event = queue[index]
      assert(event.endpoint == 17 + index % 3, "event on unrelated endpoint")
      assert(event.cluster == 0x0039 && event.event_id == 0x03 && event.priority == 1)
      assert(event.data0.val == states[index / 3] && type(event.data0.val) == 'bool')
      assert(event.data1 == nil && event.data2 == nil)
      assert(event.event_no == int64(index + 1))
    end
  end
  def assert_event_lists()
    var ctx = matter.Path(nil, 0x0039, 0xFFFA)
    var tlv = matter.TLV.Matter_TLV_item()
    for index : 1..6
      var plugin = self.device.plugins[index]
      ctx.endpoint = plugin.endpoint
      var events = plugin.read_attribute(self.session, ctx, tlv).val
      if index <= 4
        assert(size(events) == 1 && events[0].val == 3, "HTTP EventList must advertise ReachableChanged")
      else
        assert(size(events) == 0, "MQTT or local EventList changed")
      end
    end
  end
  def report(expected, event_states)
    assert(size(self.sub.updates) == 3, "recovery did not queue all shared Reachable attributes")
    var before = size(self.transport.sent)
    self.transport.im.subs_shop.every_50ms()
    assert(size(self.transport.sent) == before + 1, "recovery report was not sent")
    var frame = self.transport.sent[-1]
    assert(frame["opcode"] == 5, "expected ReportData")
    var report = matter.TLV.parse(frame["payload"])
    assert(report.findsubval(0) == self.sub.subscription_id, "wrong subscription")
    var attributes = report.findsubval(1)
    assert(size(attributes) == 3, "report included unrelated or missing attributes")
    var endpoints = {}
    var events = report.findsubval(2)
    assert(size(events) == size(event_states) * 3, "missing or duplicate events in subscription report")
    for index : 0..size(events)-1
      var data = events[index].findsub(1)
      var path = data.findsub(0)
      assert(path.findsubval(1) == 17 + index % 3)
      assert(path.findsubval(2) == 0x0039 && path.findsubval(3) == 0x03)
      assert(data.findsubval(1) == int64(index + 1) && data.findsubval(2) == 1)
      var fields = data.findsub(7)
      assert(fields.findsubval(0) == event_states[index / 3] && type(fields.findsubval(0)) == 'bool',
             "encoded ReachableChanged must contain field 0 boolean")
      assert(size(fields.val) == 1)
    end
    for entry : attributes
      var data = entry.findsub(1)
      var path = data.findsub(1)
      var endpoint = path.findsubval(2)
      assert(endpoint >= 17 && endpoint <= 19, "report included unrelated endpoint")
      assert(!endpoints.contains(endpoint), "duplicate endpoint in report")
      endpoints[endpoint] = true
      assert(path.findsubval(3) == 0x0039 && path.findsubval(4) == 0x0011)
      assert(data.findsubval(2) == expected, "Reachable value was stale when serialized")
    end
  end
end
# Startup/recovery false -> true: one remote covers three relay endpoints.
var f = Fixture()
f.remote.device_is_alive(true)
f.assert_notifications(3)
assert(f.remote.reachable && f.remote.reachable_utc == tasmota.utc)
f.assert_events([true])
f.report(true, [true])
f.assert_event_lists()
# Repeated replies refresh last-seen time without creating a report storm.
tasmota.utc += 5
f.remote.device_is_alive(true)
f.assert_notifications(3)
assert(f.remote.reachable_utc == tasmota.utc)
assert(size(f.sub.updates) == 0)
f.assert_events([true])
# Failure transition and repeated failure; report reflects the latest state.
f = Fixture()
f.remote.device_is_alive(false)
f.assert_notifications(0)
f.assert_events([])
f.remote.device_is_alive(true)
f.remote.device_is_alive(false)
f.assert_notifications(6)
assert(!f.remote.reachable)
f.remote.device_is_alive(false)
f.assert_notifications(6)
f.assert_events([true, false])
f.report(false, [true, false])
# Real registered Status 11 callbacks: all OnOff states remain unchanged.
# A controller still needs the Reachable update when the backend recovers.
f = Fixture()
assert(size(f.remote.async_cb_map) == 3)
f.remote.current_cmd = "Status 11"
f.remote.dispatch_cb(1, '{"StatusSTS":{"POWER2":"OFF","POWER3":"OFF","POWER4":"OFF"}}')
f.assert_notifications(3)
for index : 1..3 assert(!f.device.plugins[index].shadow_onoff) end
f.assert_events([true])
f.report(true, [true])
f.remote.dispatch_cb(1, '{"StatusSTS":{"POWER2":"OFF","POWER3":"OFF","POWER4":"OFF"}}')
f.assert_notifications(3)
assert(size(f.sub.updates) == 0)
f.assert_events([true])
# Native composition hides Bridged Device Basic Information from all endpoints.
# The backend must still track availability, but cannot report a hidden cluster.
f = Fixture(true)
for index : 1..6 assert(!f.device.plugins[index].contains_cluster(0x0039)) end
f.remote.device_is_alive(true)
assert(f.remote.reachable && f.remote.reachable_utc == tasmota.utc)
f.assert_notifications(0, "native composition")
f.assert_events([])
f.remote.device_is_alive(false)
assert(!f.remote.reachable)
f.assert_notifications(0, "native composition")
f.assert_events([])
f.remote.current_cmd = "Status 11"
f.remote.dispatch_cb(1, '{"StatusSTS":{"POWER2":"OFF","POWER3":"OFF","POWER4":"OFF"}}')
assert(f.remote.reachable)
f.assert_notifications(0, "native composition")
f.assert_events([])
assert(size(f.sub.updates) == 0 && f.sub.update_event_no == nil)
f.transport.im.subs_shop.every_50ms()
assert(size(f.transport.sent) == 0, "native composition reported the hidden bridge cluster")

# Temporary, unregistered HTTP probes have no Matter device to notify.
var probe = MemoryRemote(nil)
probe.device_is_alive(true)
assert(probe.reachable && probe.reachable_utc == tasmota.utc)
probe.device_is_alive(true)
probe.device_is_alive(false)
probe.device_is_alive(false)
assert(!probe.reachable)
print("HTTP recovery: shared endpoints, transitions, deduplication, Status 11, nil probes, native composition, EventList, ReachableChanged and encoded reports passed")
