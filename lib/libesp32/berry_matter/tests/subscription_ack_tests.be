#!/usr/bin/env -S ../../berry/berry -g
# Established subscription report acknowledgement-order regressions.
# Run from this tests directory: ../../berry/berry -g subscription_ack_tests.be
# Optional first argument: directory holding baseline Matter_IM.be and
# Matter_IM_Message.be. Other production sources load from ../src/embedded.
# Real IM, subscriptions, generators, event queues and TLV serialization run.
# Frames, time, encryption, transport and persistence are simulated. This does
# not validate Wi-Fi, a controller, MRP retransmission or firmware heap limits.
import global
import introspect
var matter = module("matter")
introspect.setmodule("matter", matter)
var tasmota
var log
var crypto = module("crypto")
crypto.random = def (n) return bytes("0100") end
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
  if size(_argv) > 1 && (name == "Matter_IM.be" || name == "Matter_IM_Message.be")
    directory = _argv[1] + "/"
  end
  var file = open(directory + name)
  var source = file.read()
  file.close()
  compile(source)()
end
class Plugin
  var wide, large_value
  def init()
    self.wide = false
    self.large_value = bytes()
    while size(self.large_value) < 600 self.large_value.add(0x58, 1) end
  end
  def get_endpoint() return 34 end
  def get_cluster_list_sorted() return [59] end
  def get_attribute_list_bytes(cluster) return bytes("00010002") end
  def read_attribute(session, ctx, tlv)
    return self.wide ? tlv.set(0x10, self.large_value) : tlv.set(0x04, ctx.attribute)
  end
end
class MemoryEventHandler : matter.EventHandler
  # Only replace persistence; real event queues and matching run.
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
  var device, im, sent, acks, reliable_pending
  def init(device)
    self.device = device
    self.sent = []
    self.acks = []
    self.reliable_pending = {}
  end
  def send_response_frame(frame)
    # Snapshot values because response objects may be reused for later chunks.
    self.sent.push({"opcode": frame.opcode, "exchange": frame.exchange_id,
                    "counter": frame.message_counter,
                    "ack": frame.ack_message_counter,
                    "payload": frame.payload == nil ? nil : bytes(frame.payload.tohex())})
    if frame.x_flag_r self.reliable_pending[frame.message_counter] = true end
  end
  def send_encrypted_ack(frame, reliable)
    self.acks.push({"counter": frame.message_counter, "reliable": reliable})
  end
  def receive(frame)
    # Match MessageHandler post-decryption dispatch: piggyback ACK retires a
    # transport packet, but only a standalone ACK enters the IM ACK path.
    if frame.ack_message_counter != nil
      self.reliable_pending.remove(frame.ack_message_counter)
    end
    if frame.protocol_id == 0
      if self.im.process_incoming_ack(frame) self.im.send_enqueued(self) end
    else
      if self.im.process_incoming(frame)
        self.im.send_enqueued(self)
      else
        self.send_encrypted_ack(frame, true)
      end
    end
  end
end
class Fixture
  var device, im, transport, session, sub
  def init(events_only)
    tasmota.now = 0
    self.device = Device()
    self.im = matter.IM(self.device)
    self.transport = Transport(self.device)
    self.transport.im = self.im
    self.device.message_handler = self.transport
    self.session = Session(self.transport)
    var request = matter.TLV.Matter_TLV_struct()
    request.add_TLV(1, 0x04, 0)
    request.add_TLV(2, 0x04, 60)
    if events_only
      var path = matter.EventPathIB()
      path.endpoint = 34
      path.cluster = 59
      request.add_array(4).add_obj(nil, path)
    else
      request.add_array(3).add_obj(nil, matter.AttributePathIB())
    end
    var incoming = Frame(self.session, 11, 3, true)
    incoming.raw = request.tlv2raw()
    self.transport.receive(incoming)
    self.sub = self.im.subs_shop.subs[0]
  end
  def latest() return self.transport.sent[-1] end
  def decode(report) return matter.TLV.parse(report["payload"]) end
  def standalone_ack(report)
    var reply = Frame(self.session, report["exchange"], 0x10, false)
    reply.ack_message_counter = report["counter"]
    self.transport.receive(reply)
  end
  def status(report, code, piggyback)
    var reply = Frame(self.session, report["exchange"], 1, true)
    if piggyback reply.ack_message_counter = report["counter"] end
    var value = matter.TLV.Matter_TLV_struct()
    value.add_TLV(0, 0x04, code)
    reply.raw = value.tlv2raw()
    self.transport.receive(reply)
    return reply
  end
  def assert_status_ack(reply, previous_count, reliable)
    assert(size(self.transport.acks) == previous_count + 1,
           "StatusResponse must receive exactly one acknowledgement")
    var ack = self.transport.acks[-1]
    assert(ack["counter"] == reply.message_counter,
           "StatusResponse acknowledgement used the wrong counter")
    assert(ack["reliable"] == reliable,
           "StatusResponse acknowledgement used the wrong reliability flag")
  end
  def establish()
    assert(size(self.im.subs_shop.subs) == 1 && self.sub.wait_status)
    var initial = self.latest()
    assert(initial["opcode"] == 5)
    self.status(initial, 0, true)
    assert(self.latest()["opcode"] == 4, "initial SubscribeResponse not sent")
    self.standalone_ack(self.latest())
    assert(!self.sub.wait_status && self.sub.expiration == 55000)
    assert(size(self.im.send_queue) == 0, "initial exchange not completed")
    return self
  end
  def update()
    tasmota.now += 1000
    self.sub.attribute_updated_ctx(matter.Path(34, 59, 1), false)
    self.im.subs_shop.every_50ms()
    assert(self.sub.wait_status && size(self.im.send_queue) == 1)
    return self.latest()
  end
  def assert_active()
    assert(size(self.im.subs_shop.subs) == 1 && !self.sub.wait_status,
           "subscription should be ready for another update")
    assert(size(self.im.send_queue) == 0, "completed exchange remained queued")
  end
  def assert_removed()
    assert(size(self.im.subs_shop.subs) == 0, "failed subscription not removed")
    assert(size(self.im.send_queue) == 0, "failed exchange not removed")
  end
  def expire_after_report()
    tasmota.now += 5001
    self.im.expire_sendqueue()
  end
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
run("normal initial subscription remains unchanged", def ()
  var f = Fixture(false).establish()
  f.assert_active()
end)
run("standalone ACK then SuccessStatus preserves report and next update", def ()
  var f = Fixture(false).establish()
  var report = f.update()
  var count = size(f.transport.sent)
  f.standalone_ack(report)
  assert(size(f.im.send_queue) == 1 && f.sub.wait_status,
         "standalone ACK prematurely completed report")
  assert(size(f.transport.sent) == count, "ACK sent unexpected report")
  var acks_before = size(f.transport.acks)
  var status_reply = f.status(report, 0, false)
  f.assert_status_ack(status_reply, acks_before, true)
  f.assert_active()
  var next_report = f.update()
  assert(next_report["exchange"] != report["exchange"])
  assert(size(f.transport.sent) == count + 1, "subsequent update not delivered")
  f.status(next_report, 0, true)
  f.assert_active()
end)
for piggyback : [false, true]
  run(piggyback ? "SuccessStatus with piggyback ACK completes without later ACK"
                : "SuccessStatus without ACK completes without later ACK", def ()
    var f = Fixture(false).establish()
    var report = f.update()
    var acks_before = size(f.transport.acks)
    var status_reply = f.status(report, 0, piggyback)
    f.assert_status_ack(status_reply, acks_before, true)
    if piggyback assert(f.transport.reliable_pending.find(report["counter"]) == nil) end
    f.assert_active()
    f.expire_after_report()
    f.assert_active()
    f.update()
    assert(size(f.im.send_queue) == 1, "next update missing after timeout window")
  end)
end
run("late standalone ACK after SuccessStatus is harmless", def ()
  var f = Fixture(false).establish()
  var report = f.update()
  f.status(report, 0, true)
  f.standalone_ack(report)
  f.assert_active()
end)
run("missing Status after standalone ACK expires at existing deadline", def ()
  var f = Fixture(false).establish()
  var report = f.update()
  tasmota.now += 100
  f.standalone_ack(report)
  assert(size(f.im.send_queue) == 1, "ACK removed report before timeout")
  # Preserve the existing ACK timeout extension rather than changing policy.
  var deadline = f.im.send_queue[0].expiration
  tasmota.now = deadline - 1
  f.im.expire_sendqueue()
  assert(size(f.im.subs_shop.subs) == 1)
  tasmota.now = deadline
  f.im.expire_sendqueue()
  f.assert_removed()
end)
run("missing all responses removes subscription on timeout", def ()
  var f = Fixture(false).establish()
  f.update()
  f.expire_after_report()
  f.assert_removed()
end)
run("error Status removes subscription", def ()
  var f = Fixture(false).establish()
  var report = f.update()
  f.status(report, 1, true)
  f.assert_removed()
end)
run("chunked attributes advance only on Status and final ACK cannot finish", def ()
  var f = Fixture(false).establish()
  f.device.plugins[0].wide = true
  tasmota.now = 1000
  f.sub.attribute_updated_ctx(matter.Path(34, 59, 1), false)
  f.sub.attribute_updated_ctx(matter.Path(34, 59, 2), false)
  f.im.subs_shop.every_50ms()
  var first = f.latest()
  assert(f.decode(first).findsubval(3) == true, "fixture did not produce chunks")
  var first_attributes = f.decode(first).findsubval(1)
  assert(size(first_attributes) == 1)
  var first_data = matter.AttributeReportIB().from_TLV(first_attributes[0]).attribute_data
  assert(first_data.path.attribute == 1 && first_data.data == f.device.plugins[0].large_value,
         "first chunk changed the attribute or value")
  var count = size(f.transport.sent)
  f.standalone_ack(first)
  assert(size(f.transport.sent) == count, "ACK advanced chunk before Status")
  var acks_before = size(f.transport.acks)
  var status_reply = f.status(first, 0, true)
  f.assert_status_ack(status_reply, acks_before, false)
  assert(size(f.transport.sent) == count + 1, "Status did not advance chunk")
  var last = f.latest()
  assert(last["exchange"] == first["exchange"])
  assert(last["counter"] != first["counter"])
  assert(f.decode(last).findsubval(3) == false)
  var last_attributes = f.decode(last).findsubval(1)
  assert(size(last_attributes) == 1)
  var last_data = matter.AttributeReportIB().from_TLV(last_attributes[0]).attribute_data
  assert(last_data.path.attribute == 2 && last_data.data == f.device.plugins[0].large_value,
         "last chunk changed the attribute or value")
  f.standalone_ack(last)
  assert(size(f.im.send_queue) == 1 && f.sub.wait_status)
  acks_before = size(f.transport.acks)
  status_reply = f.status(last, 0, false)
  f.assert_status_ack(status_reply, acks_before, true)
  f.assert_active()
end)
run("empty subscribed report cannot complete through unrelated queue flush", def ()
  var f = Fixture(false).establish()
  tasmota.now = 1000
  f.im.send_subscribe_update(f.sub)
  f.sub.clear_before_arm()
  var report = f.latest()
  assert(f.decode(report).findsubval(1) == nil)
  assert(f.decode(report).findsubval(2) == nil)
  assert(f.decode(report).findsubval(4) == false)
  var count = size(f.transport.sent)
  f.im.send_enqueued(f.transport)
  assert(f.sub.wait_status && size(f.im.send_queue) == 1,
         "queue flush prematurely re-armed empty report")
  assert(size(f.transport.sent) == count)
  f.standalone_ack(report)
  assert(size(f.im.send_queue) == 1 && f.sub.wait_status)
  f.status(report, 0, false)
  f.assert_active()
end)
run("real event queue and generator survive ACK-before-Status delivery", def ()
  var f = Fixture(true).establish()
  for event_id : [1, 3]
    tasmota.now += 1000
    f.device.events.publish_event(34, 59, event_id, true, 1,
                                 matter.TLV.Matter_TLV_item().set(0x04, 1), nil, nil)
    f.im.subs_shop.every_50ms()
    var report = f.latest()
    var data = f.decode(report)
    assert(data.findsubval(1) == nil, "event-only fixture unexpectedly reported attributes")
    var events = data.findsubval(2)
    assert(size(events) == 1, "event report missing or repeated")
    var decoded = matter.EventReportIB().from_TLV(events[0]).event_data
    assert(decoded.path.endpoint == 34 && decoded.path.cluster == 59)
    assert(decoded.path.event == event_id)
    assert(decoded.event_number == f.device.events.counter_event_no)
    f.standalone_ack(report)
    assert(size(f.im.send_queue) == 1 && f.sub.wait_status)
    f.status(report, 0, false)
    f.assert_active()
  end
end)
run("heartbeat still completes with standalone ACK only", def ()
  var f = Fixture(false).establish()
  tasmota.now = f.sub.expiration
  f.im.subs_shop.every_50ms()
  var heartbeat = f.latest()
  assert(f.decode(heartbeat).findsubval(4) == true)
  f.standalone_ack(heartbeat)
  f.assert_active()
  f.expire_after_report()
  f.assert_active()
end)
run("unacknowledged heartbeat still removes subscription on timeout", def ()
  var f = Fixture(false).establish()
  tasmota.now = f.sub.expiration
  f.im.subs_shop.every_50ms()
  f.expire_after_report()
  f.assert_removed()
end)
print("Subscription ACK-order regressions:", passed, "passed,", failures, "failed")
assert(failures == 0, "subscription ACK-order regression failure")
