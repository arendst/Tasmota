#!/usr/bin/env -S ../../berry/berry -g
# Initial subscription lifecycle regressions.
# Run from this directory: ../../berry/berry -g subscription_setup_tests.be
# Optional first argument: directory holding baseline Matter_IM_Message.be.
# Real IM dispatch, subscriptions, generators and TLV encoding/decoding run.
# Time, frames, encryption, transport and event persistence are simulated.
# These host tests do not measure device heap, Wi-Fi or MRP retransmission.
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
  if size(_argv) > 1 && name == "Matter_IM_Message.be"
    directory = _argv[1] + "/"
  end
  var file = open(directory + name)
  var source = file.read()
  file.close()
  compile(source)()
end
class Plugin
  var wide, large_value
  def init(wide)
    self.wide = wide
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
  def load_event_no_persisted()
    if self.counter_event_no == nil self.counter_event_no = int64(0) end
    self.counter_event_no_persisted = self.counter_event_no.add(1000)
  end
end
class Device
  var debug, plugins, events, message_handler
  def init(wide)
    self.debug = false
    self.plugins = [Plugin(wide)]
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
    # Frames can be reused for later chunks; save independent packet snapshots.
    self.sent.push({"opcode": frame.opcode, "exchange": frame.exchange_id,
                    "counter": frame.message_counter,
                    "payload": frame.payload == nil ? nil : bytes(frame.payload.tohex())})
    if frame.x_flag_r self.reliable_pending[frame.message_counter] = true end
  end
  def send_encrypted_ack(frame, reliable)
    self.acks.push({"counter": frame.message_counter, "reliable": reliable})
  end
  def receive(frame)
    # MessageHandler retires transport packets for piggyback ACKs, but calls
    # the IM ACK dispatcher only for standalone ACK messages.
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
  def init(wide)
    tasmota.now = 0
    self.device = Device(wide)
    self.im = matter.IM(self.device)
    self.transport = Transport(self.device)
    self.transport.im = self.im
    self.device.message_handler = self.transport
    self.session = Session(self.transport)
    self.sub = self.request()
  end
  def request()
    var request = matter.TLV.Matter_TLV_struct()
    request.add_TLV(0, 0x08, true)  # keep other subscriptions in this session
    request.add_TLV(1, 0x04, 0)
    request.add_TLV(2, 0x04, 60)
    request.add_array(3).add_obj(nil, matter.AttributePathIB())
    request.add_TLV(7, 0x08, false)  # FabricFiltered
    request.add_TLV(0xFF, 0x04, matter.SubscribeRequestMessage().InteractionModelRevision)
    var incoming = Frame(self.session, self.session.next_exchange(), 3, true)
    incoming.raw = request.tlv2raw()
    self.transport.receive(incoming)
    return self.im.subs_shop.subs[-1]
  end
  def latest() return self.transport.sent[-1] end
  def decode(report) return matter.TLV.parse(report["payload"]) end
  def standalone_ack(report)
    var reply = Frame(self.session, report["exchange"], 0x10, false)
    reply.ack_message_counter = report["counter"]
    self.transport.receive(reply)
  end
  def status(report, code)
    var reply = Frame(self.session, report["exchange"], 1, true)
    reply.ack_message_counter = report["counter"]
    var value = matter.StatusResponseMessage()
    value.status = code
    reply.raw = value.to_TLV().tlv2raw()
    self.transport.receive(reply)
    return reply
  end
  def establish(final_ack)
    assert(self.sub.expiration == nil && self.sub.wait_status)
    var report = self.latest()
    assert(report["opcode"] == 5 && self.decode(report).findsubval(3) == false)
    self.status(report, 0)
    var response = self.latest()
    assert(response["opcode"] == 4, "SubscribeResponse not sent")
    var decoded = self.decode(response)
    assert(decoded.findsubval(0) == self.sub.subscription_id && decoded.findsubval(2) == 60)
    assert(self.sub.expiration == 55000 && !self.sub.wait_status)
    if final_ack self.standalone_ack(response) end
    return response
  end
  def expire_at(deadline)
    tasmota.now = deadline
    self.im.expire_sendqueue()
  end
  def assert_removed()
    assert(size(self.im.send_queue) == 0, "failed exchange remained queued")
    assert(size(self.im.subs_shop.subs) == 0, "incomplete subscription remained registered")
  end
  def assert_established()
    assert(size(self.im.subs_shop.subs) == 1, "expected exactly one registered subscription")
    assert(self.im.subs_shop.subs[0] == self.sub, "established subscription was removed")
    assert(self.sub.expiration != nil, "subscription never established")
  end
  def start_update()
    tasmota.now += 1000
    self.sub.attribute_updated_ctx(matter.Path(34, 59, 1), false)
    self.im.subs_shop.every_50ms()
    var report = self.latest()
    assert(report["opcode"] == 5 && self.sub.wait_status)
    return report
  end
  def assert_chunk(report, attribute, more)
    assert(report["opcode"] == 5 && self.decode(report).findsubval(3) == more)
    var values = self.decode(report).findsubval(1)
    assert(size(values) == 1, "chunk must contain one real attribute")
    var data = matter.AttributeReportIB().from_TLV(values[0]).attribute_data
    assert(data.path.attribute == attribute && data.data == self.device.plugins[0].large_value,
           "initial report changed attribute data")
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
run("unanswered setup expires at existing five-second deadline", def ()
  var f = Fixture(false)
  assert(f.im.send_queue[0].expiration == 5000)
  f.expire_at(4999)
  assert(size(f.im.subs_shop.subs) == 1 && size(f.im.send_queue) == 1)
  f.expire_at(5000)
  f.assert_removed()
end)
run("standalone ACK cannot establish setup and preserves timeout extension", def ()
  var f = Fixture(false)
  tasmota.now = 1000
  var count = size(f.transport.sent)
  f.standalone_ack(f.latest())
  assert(f.sub.expiration == nil && f.sub.wait_status)
  assert(size(f.transport.sent) == count && size(f.im.send_queue) == 1)
  assert(f.im.send_queue[0].expiration == 6000)
  f.expire_at(5999)
  assert(size(f.im.subs_shop.subs) == 1)
  f.expire_at(6000)
  f.assert_removed()
end)
run("error Status during initial report removes incomplete setup", def ()
  var f = Fixture(false)
  var count = size(f.transport.sent)
  var ack_count = size(f.transport.acks)
  var reply = f.status(f.latest(), 1)
  f.assert_removed()
  assert(size(f.transport.sent) == count, "error generated SubscribeResponse")
  assert(size(f.transport.acks) == ack_count + 1 &&
         f.transport.acks[-1]["counter"] == reply.message_counter &&
         f.transport.acks[-1]["reliable"] == true,
         "error Status did not receive its acknowledgement")
end)
run("partial initial report with no Status removes incomplete setup", def ()
  var f = Fixture(true)
  f.assert_chunk(f.latest(), 1, true)
  f.expire_at(5000)
  f.assert_removed()
end)
run("successful first chunk followed by missing final Status is removed", def ()
  var f = Fixture(true)
  var first = f.latest()
  f.assert_chunk(first, 1, true)
  tasmota.now = 1000
  f.status(first, 0)
  var last = f.latest()
  f.assert_chunk(last, 2, false)
  assert(last["exchange"] == first["exchange"] && last["counter"] != first["counter"])
  assert(f.sub.expiration == nil && f.im.send_queue[0].expiration == 6000)
  f.expire_at(6000)
  f.assert_removed()
end)
run("error Status after successful first chunk removes incomplete setup", def ()
  var f = Fixture(true)
  f.status(f.latest(), 0)
  f.assert_chunk(f.latest(), 2, false)
  f.status(f.latest(), 1)
  f.assert_removed()
end)
run("successful single-report setup completes with final ACK", def ()
  var f = Fixture(false)
  f.establish(true)
  f.assert_established()
  assert(size(f.im.send_queue) == 0 && !f.sub.wait_status)
end)
run("successful chunked setup retains both values and establishes", def ()
  var f = Fixture(true)
  f.assert_chunk(f.latest(), 1, true)
  f.status(f.latest(), 0)
  f.assert_chunk(f.latest(), 2, false)
  f.establish(true)
  f.assert_established()
  assert(size(f.im.send_queue) == 0 && !f.sub.wait_status)
end)
run("missing final SubscribeResponse ACK preserves established subscription", def ()
  var f = Fixture(false)
  f.establish(false)
  assert(size(f.im.send_queue) == 1)
  f.expire_at(5000)
  f.assert_established()
  assert(size(f.im.send_queue) == 0 && !f.sub.wait_status)
  f.start_update()
  assert(size(f.im.send_queue) == 1, "established subscription cannot deliver later update")
end)
run("late error on final SubscribeResponse preserves established subscription", def ()
  var f = Fixture(false)
  var response = f.establish(false)
  f.status(response, 1)
  f.assert_established()
  assert(size(f.im.send_queue) == 0 && !f.sub.wait_status)
end)
for error : [false, true]
  for ack_first : [false, true]
    var name = error ? "stale initial exchange error" : "stale initial exchange timeout"
    name += ack_first ? " with update ACK before Status" : " with update Status before ACK"
    run(name, def ()
      var f = Fixture(false)
      var response = f.establish(false)
      var report = f.start_update()
      assert(report["exchange"] != response["exchange"] && size(f.im.send_queue) == 2)
      if error f.status(response, 1) else f.expire_at(5000) end
      f.assert_established()
      assert(f.sub.wait_status && size(f.im.send_queue) == 1,
             "stale setup disrupted the in-progress update")
      assert(f.im.send_queue[0].get_exchangeid() == report["exchange"])
      if ack_first
        f.standalone_ack(report)
        assert(f.sub.wait_status && size(f.im.send_queue) == 1,
               "transport ACK completed the update before StatusResponse")
      end
      f.status(report, 0)
      f.assert_established()
      assert(!f.sub.wait_status && size(f.im.send_queue) == 0,
             "StatusResponse did not immediately complete the update")
      var expiration = f.sub.expiration
      var sent_count = size(f.transport.sent)
      f.standalone_ack(report)
      f.assert_established()
      assert(!f.sub.wait_status && size(f.im.send_queue) == 0,
             "late transport ACK disrupted the completed update")
      assert(f.sub.expiration == expiration && size(f.transport.sent) == sent_count,
             "late transport ACK changed subscription state or sent another report")
    end)
  end
end
run("failed second subscription removes only itself", def ()
  var f = Fixture(false)
  f.establish(true)
  var good = f.sub
  var failed = f.request()
  assert(failed != good && size(f.im.subs_shop.subs) == 2)
  f.status(f.latest(), 1)
  f.assert_established()
  assert(f.im.subs_shop.subs[0] == good && !good.wait_status)
end)
run("repeated cleanup cannot remove a different established subscription", def ()
  var f = Fixture(false)
  f.establish(true)
  var failed = f.request()
  var exchange = f.im.send_queue[0]
  f.expire_at(exchange.expiration)
  f.assert_established()
  # Callback delivery can be repeated; remove_self is intentionally reentrant.
  exchange.reached_timeout()
  exchange.status_error_received(nil)
  f.im.expire_sendqueue()
  f.assert_established()
  assert(size(f.im.send_queue) == 0 && failed.expiration == nil)
end)
run("late Status and ACK after setup cleanup cannot resurrect it", def ()
  var f = Fixture(false)
  var report = f.latest()
  f.expire_at(5000)
  f.assert_removed()
  var count = size(f.transport.sent)
  f.status(report, 0)
  f.status(report, 1)
  f.standalone_ack(report)
  f.assert_removed()
  assert(size(f.transport.sent) == count, "late packet resurrected setup")
end)
run("repeated failed setups do not accumulate after simulated hour", def ()
  var f = Fixture(false)
  var round = 0
  while round < 40
    if round > 0 f.sub = f.request() end
    if round % 2 == 0
      f.expire_at(tasmota.now + 5000)
    else
      f.status(f.latest(), 1)
    end
    f.assert_removed()
    round += 1
  end
  tasmota.now += 3600000
  var count = size(f.transport.sent)
  f.im.every_second()
  f.im.every_50ms()
  f.assert_removed()
  assert(size(f.transport.sent) == count, "removed setup emitted a later heartbeat")
end)
print("Subscription setup regressions:", passed, "passed,", failures, "failed")
assert(failures == 0, "subscription setup regression failure")
