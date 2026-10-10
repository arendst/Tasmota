#!/usr/bin/env -S ../../berry/berry -g
# Off-device soak test for the four virtual Matter buttons and real event queues.
# Run from lib/libesp32/berry_matter/tests:
#   ../../berry/berry -g button_stability_tests.be
# Reuse the existing source loader and regressions, avoiding a second bootstrap.
# No network or filesystem persistence occurs in the soak workload.
var regression_source = open("run_tests.be")
var regression_code = regression_source.read()
regression_source.close()
compile(regression_code)()
regression_code = nil

import gc
import matter

# Keep the soak's working objects independent of the main script's registers.
def test_button_stability()
  class MemoryEventHandler : matter.EventHandler
    var persistence_blocks
    # The sole EventHandler override replaces its filesystem persistence boundary.
    # Start just below the uint32 boundary to check event numbers cross it intact.
    def load_event_no_persisted()
      if self.counter_event_no == nil
        self.counter_event_no = int64.fromstring("4294967276")
        self.persistence_blocks = 0
      end
      self.counter_event_no_persisted = self.counter_event_no.add(self.EVENT_NO_INCR)
      self.persistence_blocks += 1
    end
  end

  class TestSubscriptionTransport
    var delivered, previous_number, encoded_bytes
    def init()
      self.delivered = 0
      self.encoded_bytes = 0
    end
    # Real EventHandler delivery stops at the subscription/transport boundary.
    # Serialize and decode every event through the production TLV implementation.
    def event_published(event)
      if self.previous_number != nil
        assert(event.event_no > self.previous_number, "event number did not increase")
      end
      self.previous_number = event.event_no
      var raw = event.to_raw_bytes()
      assert(event.to_raw_bytes() == raw, "cached serialization changed")
      var tlv = matter.TLV.parse(raw)
      var decoded = matter.EventReportIB().from_TLV(tlv).event_data
      assert(decoded.event_number == event.event_no, "event number changed in TLV")
      assert(decoded.path.endpoint == event.endpoint)
      assert(decoded.path.cluster == 0x003B)
      assert(decoded.path.event == event.event_id)
      assert(decoded.path.is_urgent && decoded.priority == 1)
      assert(decoded.epoch_timestamp == event.epoch_timestamp)
      var fields = tlv.findsub(1).findsub(7)
      assert(fields.findsubval(0) == 1)
      assert(fields.findsubval(1) == (event.data1 != nil ? event.data1.val : nil))
      assert(fields.findsubval(2) == nil)
      self.delivered += 1
      self.encoded_bytes += size(raw)
      # Model report completion, including cache regeneration for a retransmit.
      event.compact()
      assert(event.raw_tlv == nil)
      assert(event.to_raw_bytes() == raw, "retransmission serialization changed")
    end
  end

  class TestInteractionModel
    var subs_shop
    def init() self.subs_shop = TestSubscriptionTransport() end
  end
  class TestMessageHandler
    var im
    def init() self.im = TestInteractionModel() end
  end
  class StabilityDevice
    var events, message_handler, updates
    def init()
      self.updates = 0
      self.message_handler = TestMessageHandler()
      self.events = MemoryEventHandler(self)
    end
    def attribute_updated(endpoint, cluster, attribute, fabric_specific)
      assert(endpoint >= 34 && endpoint <= 37)
      assert(cluster == 0x003B && attribute == 1)
      self.updates += 1
    end
  end

  def assert_queues(handler)
    for queue : [handler.queue_debug, handler.queue_info, handler.queue_critical]
      assert(size(queue) <= handler.EVENT_QUEUE_SIZE_MAX, "unbounded event queue")
      var previous = nil
      for event : queue
        if previous != nil assert(event.event_no > previous) end
        previous = event.event_no
      end
    end
  end

  var device = StabilityDevice()
  var buttons = []
  for endpoint : 34..37
    buttons.push(matter.Plugin_Virt_Sensor_GenericSwitch_Btn(device, endpoint, {}))
  end
  var transport = device.message_handler.im.subs_shop
  var completed = 0

  def run_batch()
    var delivered_before = transport.delivered
    var updates_before = device.updates
    # The 20-gesture pattern covers all 4 endpoints with each of 1..5 presses.
    for gesture : 0..999
      var button = buttons[gesture % 4]
      var presses = (gesture % 5) + 1
      var previous = transport.delivered
      assert(button.update_virtual({"Presses": presses}) == nil)
      assert(transport.delivered - previous == presses * 3)
      for other : buttons assert(other.shadow_position == 0, "button remained pressed") end
      assert_queues(device.events)
      if gesture % 20 == 0 device.events.every_second() end
    end
    assert(transport.delivered - delivered_before == 9000)
    assert(device.updates - updates_before == 6000)
    assert(size(device.events.queue_debug) == 10)
    assert(size(device.events.queue_info) == 10)
    assert(size(device.events.queue_critical) == 0)
    device.events.compact()
    completed += 1000
  end

  # Warm all paths and fill the bounded queues before measuring retained memory.
  run_batch()
  var samples = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
  gc.collect()
  gc.collect()
  samples[0] = gc.allocated()
  for batch : 1..12
    run_batch()
    gc.collect()
    gc.collect()
    samples[batch] = gc.allocated()
  end
  var smallest = samples[0]
  var largest = samples[0]
  for sample : samples
    if sample < smallest smallest = sample end
    if sample > largest largest = sample end
  end
  print("Matter button soak gestures:", completed, "events:", transport.delivered,
        "serialized bytes:", transport.encoded_bytes)
  print("Retained bytes after equal 1000-gesture batches:", samples)
  print("Retained memory range:", largest - smallest,
        "persistence block reservations:", device.events.persistence_blocks)
  assert(completed == 13000 && transport.delivered == 117000)
  assert(device.events.counter_event_no.high32() == 1, "event number rollover was not exercised")
  assert(device.events.persistence_blocks > 100, "counter reservation boundary was not exercised")
  # Permit a small fixed interpreter bookkeeping fluctuation, never per-event growth.
  assert(largest - smallest <= 1024, "event processing leaks retained memory after GC")
  print("Matter virtual-button stability tests: OK")
end

test_button_stability()
