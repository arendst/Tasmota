#!/usr/bin/env -S ../../berry/berry -g
# SubscribeRequest DataVersionFilter memory regressions.
# Run from this directory: ../../berry/berry -g subscription_filter_memory_tests.be
# Optional first argument: directory holding baseline Matter_IM.be and Matter_IM_Data.be.
# The baseline must fail only the two "not kept in memory" cases; all others must pass.
# Real IM dispatch, subscriptions, generators and TLV encoding/decoding run.
# Time, frames, encryption, transport and event persistence are simulated.
# Memory is checked as TLV node counts on the host, not as ESP32 heap measurements.
import global
import introspect
var matter = module("matter")
introspect.setmodule("matter", matter)
var tasmota
var log
var crypto = module("crypto")
var next_id = 0
crypto.random = def (n)
  next_id += 1
  return bytes().add(next_id, n)
end
introspect.setmodule("crypto", crypto)
class TestTasmota
  var now
  def init() self.now = 0 end
  def millis() return self.now end
  def time_reached(deadline) return self.now >= deadline end
  def rtc(mode) return 1700000000 end
  def loglevel(level) return false end
end
tasmota = TestTasmota()
log = def (*args) end
for name : ["Matter_TLV.be", "Matter_IM_Data.be", "Matter_Path_0.be",
            "Matter_Path_1_PathGenerator.be", "Matter_Path_1_EventGenerator.be",
            "Matter_EventHandler.be", "Matter_IM_Subscription.be",
            "Matter_IM_Message.be", "Matter_IM.be"]
  var directory = "../src/embedded/"
  if size(_argv) > 1 && (name == "Matter_IM.be" || name == "Matter_IM_Data.be")
    directory = _argv[1] + "/"
  end
  var file = open(directory + name)
  var source = file.read()
  file.close()
  compile(source)()
end
class Plugin
  def get_endpoint() return 34 end
  def get_cluster_list_sorted() return [59] end
  def get_attribute_list_bytes(cluster) return bytes("00010002") end
  def read_attribute(session, ctx, tlv) return tlv.set(0x04, ctx.attribute) end
end
class MemoryEventHandler : matter.EventHandler
  def load_event_no_persisted()
    if self.counter_event_no == nil self.counter_event_no = int64(0) end
    self.counter_event_no_persisted = self.counter_event_no.add(1000)
  end
end
class Device
  var debug, plugins, events, message_handler
  def init()
    self.debug = false
    self.plugins = [Plugin()]
    self.events = MemoryEventHandler(self)
  end
end
class Session
  var _message_handler, local_session_id, counter, exchange
  def init(handler)
    self._message_handler = handler
    self.local_session_id = 1
    self.counter = 100
    self.exchange = 40
  end
  def next_counter() self.counter += 1 return self.counter end
  def next_exchange() self.exchange += 1 return self.exchange end
end
class Frame
  var session, exchange_id, opcode, protocol_id, message_counter
  var x_flag_r, ack_message_counter, payload, raw, app_payload_idx
  def init(session, exchange_id, opcode, reliable)
    self.session = session
    self.exchange_id = exchange_id
    self.opcode = opcode
    self.protocol_id = opcode == 0x10 ? 0 : 1
    self.message_counter = session.next_counter()
    self.x_flag_r = reliable
    self.app_payload_idx = 0
  end
  def get_node_id() return nil end
  def build_response(opcode, reliable, previous)
    var result = Frame(self.session, self.exchange_id, opcode, reliable)
    if self.x_flag_r result.ack_message_counter = self.message_counter end
    return result
  end
  def build_standalone_ack(reliable) return self.build_response(0x10, reliable) end
  def encode_frame(payload) self.payload = payload end
  def encrypt() end
  static def initiate_response(handler, session, opcode, reliable)
    return Frame(session, session.next_exchange(), opcode, reliable)
  end
end
matter.Frame = Frame
class Transport
  var device, im, sent, acks
  def init(device) self.device = device self.sent = [] self.acks = [] end
  def send_response_frame(frame)
    self.sent.push({"opcode": frame.opcode, "exchange": frame.exchange_id, "counter": frame.message_counter,
                    "payload": frame.payload == nil ? nil : bytes(frame.payload.tohex())})
  end
  def send_encrypted_ack(frame, reliable) self.acks.push(frame.message_counter) end
  def receive(frame)
    if frame.protocol_id == 0
      if self.im.process_incoming_ack(frame) self.im.send_enqueued(self) end
    elif self.im.process_incoming(frame)
      self.im.send_enqueued(self)
    else
      self.send_encrypted_ack(frame, true)
    end
  end
end
class Fixture
  var device, im, transport, session
  def init()
    tasmota.now = 0
    self.device = Device()
    self.im = matter.IM(self.device)
    self.transport = Transport(self.device)
    self.transport.im = self.im
    self.device.message_handler = self.transport
    self.session = Session(self.transport)
  end
  # Deliver a raw SubscribeRequest, returns the raised exception name or nil
  def subscribe(raw, header)
    var incoming = Frame(self.session, self.session.next_exchange(), 3, true)
    incoming.raw = header == nil ? raw : header + raw
    incoming.app_payload_idx = header == nil ? 0 : size(header)
    try
      self.transport.receive(incoming)
    except .. as e
      return e
    end
    return nil
  end
  def status(report, code)
    var reply = Frame(self.session, report["exchange"], 1, true)
    reply.ack_message_counter = report["counter"]
    var value = matter.StatusResponseMessage()
    value.status = code
    reply.raw = value.to_TLV().tlv2raw()
    self.transport.receive(reply)
  end
end

# Request with the shape and size (1,166 bytes for 80 filters) recorded from Apple Home:
# 1 attribute path, 1 event path, 1 event filter and `count` DataVersionFilters.
# `bad_at` replaces one filter by a scalar.
def request(count, bad_at)
  var r = matter.TLV.Matter_TLV_struct()
  r.add_TLV(0, 0x08, true)                   # KeepSubscriptions
  r.add_TLV(1, 0x04, 0)                      # MinIntervalFloor
  r.add_TLV(2, 0x04, 60)                     # MaxIntervalCeiling
  r.add_array(3).add_obj(nil, matter.AttributePathIB())
  r.add_array(4).add_obj(nil, matter.EventPathIB())
  var event_filter = matter.EventFilterIB()
  event_filter.event_min = int64(0)
  r.add_array(5).add_obj(nil, event_filter)
  r.add_TLV(7, 0x08, false)                  # FabricFiltered
  if count != nil
    var versions = r.add_array(8)
    for i : 0..count-1
      if i == bad_at
        versions.add_TLV(nil, 0x04, 1)
      else
        var filter = matter.DataVersionFilterIB()
        filter.path = matter.ClusterPathIB()
        filter.path.endpoint = 34
        filter.path.cluster = 59 + i
        filter.data_version = (i < 77) ? i : 256 + i
        versions.add_obj(nil, filter)
      end
    end
  end
  r.add_TLV(0xFF, 0x04, matter.SubscribeRequestMessage().InteractionModelRevision)
  return r.tlv2raw()
end

var failures = 0
var passed = 0
def run(name, test)
  try
    test()
    passed += 1
    print("PASS", name)
  except .. as exception, message
    failures += 1
    print("FAIL", name, exception, message)
  end
end

def establishes(raw, header)
  var f = Fixture()
  assert(f.subscribe(raw, header) == nil, "request raised")
  assert(size(f.transport.acks) == 1, "expected one standalone ACK before the report")
  assert(size(f.im.subs_shop.subs) == 1, "subscription not created")
  var sub = f.im.subs_shop.subs[0]
  assert(size(sub.path_list) == 1 && size(sub.event_generators) == 1, "request paths lost")
  var report = f.transport.sent[-1]
  assert(report["opcode"] == 5, "initial report not sent")
  f.status(report, 0)
  var response = f.transport.sent[-1]
  assert(response["opcode"] == 4, "SubscribeResponse not sent")
  var decoded = matter.TLV.parse(response["payload"])
  assert(decoded.findsubval(0) == sub.subscription_id && decoded.findsubval(2) == 60)
  assert(sub.expiration != nil && !sub.wait_status, "subscription not established")
end

run("80-filter request is acknowledged, reported and established", def ()
  var raw = request(80)
  assert(size(raw) == 1166, "fixture differs from the recorded 1,166-byte request")
  establishes(raw)
end)

run("request without DataVersionFilters is unchanged", def ()
  establishes(request(nil))
end)

# Number of TLV nodes kept in a decoded tree
def tlv_nodes(item)
  var n = 1
  if isinstance(item.val, list) for child : item.val n += tlv_nodes(child) end end
  return n
end
class ProbeIM : matter.IM
  var seen
  def subscribe_request(msg, val, versions_idx)
    self.seen = val
    return super(self).subscribe_request(msg, val, versions_idx)
  end
end

run("DataVersionFilters are not kept in memory while setting up", def ()
  var f = Fixture()
  f.im = ProbeIM(f.device)
  f.transport.im = f.im
  assert(f.subscribe(request(80)) == nil, "request raised")
  var empty = tlv_nodes(matter.TLV.parse(request(0), 0))
  assert(tlv_nodes(f.im.seen) == empty, f"IM kept {tlv_nodes(f.im.seen) - empty} filter nodes in the tree")
end)

run("no parsed piece of the request holds all the filters at once", def ()
  var largest = 0
  var parse = matter.TLV.parse
  matter.TLV.parse = def (b, idx, parent)
    var item = parse(b, idx, parent)
    var nodes = tlv_nodes(item)
    if nodes > largest   largest = nodes   end
    return item
  end
  var error
  try
    error = Fixture().subscribe(request(80))
  except .. as e, m
    error = e
  end
  matter.TLV.parse = parse
  assert(error == nil, "request raised")
  assert(largest < 50, f"one parse built {largest} TLV nodes")
end)

run("public from_TLV still decodes every filter", def ()
  var query = matter.SubscribeRequestMessage().from_TLV(matter.TLV.parse(request(80), 0))
  assert(size(query.data_version_filters) == 80)
  assert(query.data_version_filters[79].data_version == 256 + 79)
end)

run("invalid filter is rejected after the standalone ACK, as before", def ()
  var f = Fixture()
  var error = f.subscribe(request(80, 40))
  assert(error != nil, "invalid filter accepted")
  assert(size(f.transport.acks) == 1, "ACK boundary changed")
  assert(size(f.im.subs_shop.subs) == 0 && size(f.im.send_queue) == 0, "subscription created")
end)

run("truncated filters are rejected before the ACK, as before", def ()
  var raw = request(80)
  var f = Fixture()
  var error = f.subscribe(raw[0 .. size(raw) / 2])
  assert(error != nil, "truncated request accepted")
  assert(size(f.transport.acks) == 0, "ACK sent for unparsable request")
  assert(size(f.im.subs_shop.subs) == 0, "subscription created")
end)

# Raw struct members of `r` without the enclosing 0x15 ... 0x18
def members(r) var b = r.tlv2raw() return b[1 .. size(b) - 2] end
def filter_array(count, bad_at)
  var s = matter.TLV.Matter_TLV_struct()
  var versions = s.add_array(8)
  for i : 0..count-1
    if i == bad_at versions.add_TLV(nil, 0x04, 1) continue end
    var filter = matter.DataVersionFilterIB()
    filter.path = matter.ClusterPathIB()
    filter.path.endpoint = 1
    filter.path.cluster = 6 + i
    filter.data_version = i
    versions.add_obj(nil, filter)
  end
  return members(s)
end
def no_filters()
  var raw = request(nil)
  return raw[1 .. size(raw) - 2]
end

run("filters are found after a frame header", def ()
  var header = bytes("A1A2A3A4A5A6A7A8A9")
  establishes(request(80), header)
  var f = Fixture()
  assert(f.subscribe(request(80, 40), header) != nil, "invalid filter accepted")
  assert(size(f.transport.acks) == 1 && size(f.im.subs_shop.subs) == 0)
end)

run("filters before the other fields", def ()
  establishes(bytes("15") + filter_array(80) + no_filters() + bytes("18"))
end)

run("only the first DataVersionFilters field is validated, as before", def ()
  establishes(bytes("15") + no_filters() + filter_array(3) + filter_array(3, 1) + bytes("18"))
  var f = Fixture()
  assert(f.subscribe(bytes("15") + no_filters() + filter_array(3, 1) + filter_array(3) + bytes("18")) != nil)
  assert(size(f.transport.acks) == 1 && size(f.im.subs_shop.subs) == 0)
end)

run("empty DataVersionFilters", def ()
  establishes(request(0))
end)

run("non-struct request is rejected after the standalone ACK, as before", def ()
  var f = Fixture()
  assert(f.subscribe(bytes("0401")) != nil, "scalar request accepted")
  assert(size(f.transport.acks) == 1 && size(f.im.subs_shop.subs) == 0)
end)

print("Subscription filter memory regressions:", passed, "passed,", failures, "failed")
assert(failures == 0, "subscription filter memory regression failure")
