#
# Matter_EventHandler.be - suppport for Matter Events
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import matter

#@ solidify:Matter_EventHandler,weak
#@ solidify:Matter_EventQueued,weak

#################################################################################
# Matter_Event_Queued class
#
# This class encapsulates any element within teh event queue
# Takes a `Matter_EventDataIB`
#
# Invariants:
#    `priority` is guaranteed to be in range 0..2
#################################################################################
class Matter_EventQueued
  # common elements
  var endpoint
  var cluster
  var event_id
  var is_urgent
  var priority
  var data0, data1, data2
  var event_no
  var epoch_timestamp
  var raw_tlv                           # content encoded as full TLV

  def init(event_no, endpoint, cluster, event_id, is_urgent, priority, data0, data1, data2)
    self.event_no = event_no
    self.endpoint = endpoint
    self.cluster = cluster
    self.event_id = event_id
    self.is_urgent = is_urgent
    self.priority = priority
    # priority
    if (self.priority < 0)                        self.priority = 0                       end
    if (self.priority > matter.EVENT_CRITICAL)    self.priority = matter.EVENT_CRITICAL   end
    # epoch_time
    self.epoch_timestamp = tasmota.rtc('utc')
    if (self.epoch_timestamp < 1700000000)
      self.epoch_timestamp = tasmota.rtc('config_time')    # no valid time, take the last config time
    end
    self.data0 = data0
    self.data1 = data1
    self.data2 = data2
  end

  #################################################################################
  # to_raw_bytes
  #
  # Computes a complete EventReportIB structure as bytes()
  #
  # It is memoized in self.raw_tlv, but is cleaned after sending messages to
  # free up some space
  def to_raw_bytes()
    if (self.raw_tlv == nil)
      self.raw_tlv = bytes(64)
      self.eventreport2raw(self.raw_tlv, self.endpoint, self.cluster, self.event_id, self.is_urgent, self.priority, self.event_no, self.epoch_timestamp, self.data0, self.data1, self.data2)
      # var event_report = matter.EventReportIB()
      # var event_ib = matter.EventDataIB()
      # event_report.event_data = event_ib
      # var event_path = matter.EventPathIB()
      # event_path.endpoint = self.endpoint
      # event_path.cluster = self.cluster
      # event_path.event = self.event_id
      # event_path.is_urgent = self.is_urgent
      # event_ib.path = event_path
      # event_ib.priority = self.priority
      # event_ib.event_number = self.event_no
      # event_ib.epoch_timestamp = self.epoch_timestamp
      # event_ib.data = matter.TLV.Matter_TLV_struct()
      # if (self.data0 != nil)   event_ib.data.add_obj(0, self.data0)   end
      # if (self.data1 != nil)   event_ib.data.add_obj(1, self.data1)   end
      # if (self.data2 != nil)   event_ib.data.add_obj(2, self.data2)   end

      # self.raw_tlv = event_report.to_TLV().tlv2raw()     # bytes()
    end
    return self.raw_tlv
  end

  #############################################################
  # eventreport2raw
  #
  # 15 = EventReportIB
  #   3501 = EventDataIB
  #     
  def eventreport2raw(raw, endpoint, cluster, event, is_urgent, priority, event_no, epoch_timestamp, data0, data1, data2)
    # open struct EventReportIB
    raw.add(0x15, 1)              # add 15
    # open sturct EventDataIB
    raw.add(0x3501, -2)           # add 3501
    # add path
    self.eventpath2raw(raw, endpoint, cluster, event, is_urgent)
    # add EventNumber
    raw.add(0x2701, -2)           # 2701 FFFFFFFFFFFFFFFF
    raw.add(event_no.low32(), 4)  # int64 low part
    raw.add(event_no.high32(), 4) # int64 high part
    # priority
    raw.add(0x2402, -2)           # add 2402
    raw.add(priority, 1)          # add priority as one byte
    # EpochTimestamp
    raw.add(0x2603, -2)           # add 2603
    raw.add(epoch_timestamp, 4)   # add epoch as 32 bits
    # Data struct
    raw.add(0x3507, -2)           # add 3507

    # data0
    if (data0 != nil)
      data0.tag_sub = 0
      data0.tlv2raw(raw)
    end
    # data1
    if (data1 != nil)
      data1.tag_sub = 1
      data1.tlv2raw(raw)
    end
    # data2
    if (data2 != nil)
      data2.tag_sub = 2
      data2.tlv2raw(raw)
    end

    # close struct Data
    raw.add(0x18, 1)           # add 18
    # close struct EventDataIB
    raw.add(0x18, 1)           # add 18
    # close struct EventReportIB
    raw.add(0x18, 1)           # add 18
  end

  #############################################################
  # eventpath2raw
  #
  # Encodes endpoint/cluster/attribute as `EventPathIB` elements
  # Takes sub-tag
  #
  #   1 = EventPathIB
  #     0 = Node
  #     1 = Endpoint
  #     2 = Cluster
  #     3 = Event
  #     4 = IsUrgent
  #
  #     3700 		0 = LIST
  #       2401 01   1 = 1U (U1)
  #       2402 39 	2 = 0x39U (U1)
  #       2403 11 	3 = 0x11U (U1)
  #       2904      4 = true
  #     18
  def eventpath2raw(raw, endpoint, cluster, event, is_urgent)
    # open struct
    raw.add(0x3700, -2)           # add 3700
    # add endpoint
    if endpoint <= 0xFF       # endpoint is 16 bits max
      raw.add(0x2401, -2)         # add 2401
      raw.add(endpoint, 1)
    else
      raw.add(0x2501, -2)         # add 2501
      raw.add(endpoint, 2)
    end
    # add cluster
    if cluster <= 0xFF        # cluster is 32 bits max
      raw.add(0x2402, -2)         # add 2402
      raw.add(cluster, 1)
    elif cluster <= 0xFFFF
      raw.add(0x2502, -2)         # add 2502
      raw.add(cluster, 2)
    else
      raw.add(0x2602, -2)         # add 2602
      raw.add(cluster, 4)
    end
    # add event
    if event <= 0xFF       # cluster is 32 bits max
      raw.add(0x2403, -2)          # add 2403
      raw.add(event, 1)
    else
      raw.add(0x2503, -2)          # add 2503
      raw.add(event, 2)
    end
    # IsUrgent
    if is_urgent
      raw.add(0x2904, -2)          # add 2904
    else
      raw.add(0x2804, -2)          # add 2804
    end
    # close
    raw.add(0x18, 1)               # add 18
  end

  #################################################################################
  # compact
  #
  # Remove the local cache of `raw_tlv` to save space (it can always be recreated)
  def compact()
    self.raw_tlv = nil
  end

end
matter.EventQueued = Matter_EventQueued

# elements are made of `Matter_EventDataIB`
# var path                        # 
    # var node                        # u64 as bytes
    # var endpoint                    # u16
    # var cluster                     # u32
    # var event                       # u32
    # var is_urgent                   # bool
# var event_number                # u64 as bytes
# var priority                    # u8
# # one of
# var epoch_timestamp             # u64
# var system_timestamp            # u64
# var delta_epoch_timestamp       # u64
# var delta_system_timestamp      # u64
# # data
# var data                        # any TLV

# EVENT_DEBUG=0
# EVENT_INFO=1
# EVENT_CRITICAL=2

#################################################################################
# Matter_EventHandler class
#
# Keep track of events in 3 queues for DEBUG/INFO/CRITICAL levels
#
# Invariants:
#    All 3 queues (debug/info/critical) have events in sorted order by `event_no`
#################################################################################
class Matter_EventHandler
  static var EVENT_NO_INCR = 1000       # counter increased when persisting, default value from Matter spec
  static var EVENT_NO_KEY = "_matter_event_no"
  static var EVENT_QUEUE_SIZE_MAX = 10  # each queue is 10 elements depth

  # circular buffers
  var queue_debug                       # queue of events for level DEBUG
  var queue_info                        # queue of events for level INFO
  var queue_critical                    # queue of events for level CRITICAL

  var device                          # link back to matter_device top object
  # Events
  var counter_event_no                # event number, monotonically increasing even after restarts
  var counter_event_no_persisted      # the nest number persisted for after the restart

  def init(device)
    self.device = device
    self.queue_debug = []
    self.queue_info = []
    self.queue_critical = []
    self.load_event_no_persisted()    # initializes self.counter_event_no and self.counter_event_no_persisted
  end

  #####################################################################
  # load_event_no_persisted
  #
  # Load the next acceptable event_no from `persist` and persist it
  # with a predefined gap `self.EVENT_NO_INCR` (default 1000)
  def load_event_no_persisted()
    import persist
    var event_no_str = str(persist.find(self.EVENT_NO_KEY, "0"))
    self.counter_event_no = int64.fromstring(event_no_str)
    self.counter_event_no_persisted = self.counter_event_no.add(self.EVENT_NO_INCR)
    # save back next slot
    persist.setmember(self.EVENT_NO_KEY, self.counter_event_no_persisted.tostring())
    persist.save()
  end

  #####################################################################
  # Enqueue event
  #
  # Takes `EventQueued`
  #####################################################################
  def queue_event(ev_queued)
    var cur_prio = ev_queued.priority

    # we reuse the same logic as connectedhomeip
    # https://github.com/project-chip/connectedhomeip/blob/master/src/app/EventManagement.h
    #
    # Here is a copy of the original comment:

    #---------------------------------------------------------------------------------
    * A newly generated event will be placed in the lowest-priority (in practice
    * DEBUG) buffer, the one associated with the first LogStorageResource.  If
    * there is no space in that buffer, space will be created by evicting the
    * oldest event currently in that buffer, until enough space is available.
    *
    * When an event is evicted from a buffer, there are two possibilities:
    *
    * 1) If the next LogStorageResource has a priority that is no higher than the
    *    event's priority, the event will be moved to that LogStorageResource's
    *    buffer.  This may in turn require events to be evicted from that buffer.
    * 2) If the next LogStorageResource has a priority that is higher than the
    *    event's priority, then the event is just dropped.
    *
    * This means that LogStorageResources at a given priority level are reserved
    * for events of that priority level or higher priority.
    *
    * As a simple example, assume there are only two priority levels, DEBUG and
    * CRITICAL, and two LogStorageResources with those priorities.  In that case,
    * old CRITICAL events will not start getting dropped until both buffers are
    * full, while old DEBUG events will start getting dropped once the DEBUG
    * LogStorageResource buffer is full.
    ---------------------------------------------------------------------------------#

    # first step, always add to DEBUG queue
    self.queue_debug.push(ev_queued)
    # if DEBUG queue is full
    if size(self.queue_debug) > self.EVENT_QUEUE_SIZE_MAX
      # remove first (oldest element)
      var ev_debug_removed = self.queue_debug.pop(0)
      if ev_debug_removed.priority > matter.EVENT_DEBUG
        # if removed item is higher than DEBUG, push to INFO queue
        self.queue_info.push(ev_debug_removed)
        # if INFO queue is full
        if size(self.queue_info) > self.EVENT_QUEUE_SIZE_MAX
          # remove first (oldest element)
          var ev_info_removed = self.queue_info.pop(0)
          if ev_info_removed.priority > matter.EVENT_INFO
            # if removed item is higher than INFO, push to CRITICAL queue
            self.queue_critical.push(ev_info_removed)
            # if CRITICAL queue is full
            if size(self.queue_critical) > self.EVENT_QUEUE_SIZE_MAX
              # remove first (oldest element)
              var ev_critical_removed = self.queue_critical.pop(0)
            end
          end
        end
      end
    end
  end

  #############################################################
  # dispatch every second click to sub-objects that need it
  def every_second()
    self.compact()
  end

  #####################################################################
  # Enqueue event
  #
  # Remove the cached `raw_tlv` from all events in queues
  # to save some space
  def compact()
    def compact_queue(q)
      var i = 0
      while i < size(q)
        q[i].compact()
        i += 1
      end
    end
    compact_queue(self.queue_debug)
    compact_queue(self.queue_info)
    compact_queue(self.queue_critical)
  end

  #####################################################################
  # Events handling
  #####################################################################
  # Get next event number
  #
  # Also make sure that we don't go above the persisted last number,
  # in such case increase the persisted number and use the next chunk
  def get_next_event_no()
    self.counter_event_no = self.counter_event_no.add(1)
    if self.counter_event_no >= self.counter_event_no_persisted
      self.load_event_no_persisted()      # force an increment like done during boot
    end
    return self.counter_event_no
  end
  # Get last event number (all events sent up to now are lower or equal than this value)
  def get_last_event_no()
    return self.counter_event_no
  end

  #####################################################################
  # Dump events for debugging
  #####################################################################
  def dump()
    tasmota.log(f"MTR: Events queues sizes: critical {size(self.queue_critical)}, info {size(self.queue_info)}, debug {size(self.queue_debug)}", 2)
    var cnt = [0, 0, 0]        # counters
    for ev: self.queue_debug      cnt[ev.priority] += 1   end
    for ev: self.queue_info       cnt[ev.priority] += 1   end
    for ev: self.queue_critical   cnt[ev.priority] += 1   end
    tasmota.log(f"MTR: Events by types: critical {cnt[2]}, info {cnt[1]}, debug {cnt[0]}", 2)
  end

  #####################################################################
  # find_min_no
  #
  # Find the next event number just above the provided value,
  # or the smallest if nil is passed
  #
  # Arg:
  #   - event_min: minimal event number (strictly above),
  #                or `nil` if smallest number
  #
  # Returns:
  #   - event_no (int) or `nil` of none found
  #####################################################################
  def find_min_no(event_min_no)
    # fail fast if `event_min_no` is the same as `counter_event_no`
    # meaning that we dont have any more events since last report
    if (event_min_no != nil) && (event_min_no >= self.counter_event_no)
      return nil
    end

    #####################################################################
    # Find the next available event right above `event_min_no`
    #
    # TODO: since queues are sorted, we can abort searching when we reach
    # an event below `event_min_no`
    #
    # Arg
    #   - event_smallest: the event found up to now (from previous queues)
    #   - q: the queue oject
    #   - event_min_no: minimum acceptable event number (int64) or nil if any
    def find_in_queue(event_smallest, q, event_min_no)
      var i = size(q) - 1
      while i >= 0 
        var cur_event = q[i]
        # fail fast: since queues are sorted, we can abort searching when we reach an event below `event_min_no`
        if (event_min_no != nil) && (cur_event.event_no <= event_min_no)
          return event_smallest
        end

        # we know that event_no is in acceptable range, is it smaller?
        if (event_smallest == nil) || (cur_event.event_no < event_smallest.event_no)
          event_smallest = cur_event
        end

        i -= 1
      end
      return event_smallest
    end

    var event
    # look in debug
    event = find_in_queue(event, self.queue_debug, event_min_no)
    # look in info
    event = find_in_queue(event, self.queue_info, event_min_no)
    # look in critical
    event = find_in_queue(event, self.queue_critical, event_min_no)

    return event
  end

  #############################################################
  # generate a new event
  #
  def publish_event(endpoint, cluster, event_id, is_urgent, priority, data0, data1, data2)
    var new_event = matter.EventQueued(self.get_next_event_no(), endpoint, cluster, event_id, is_urgent, priority, data0, data1, data2)
    if tasmota.loglevel(3)
      var data_str = ''
      if (data0 != nil)   data_str = str(data0)             end
      if (data1 != nil)   data_str += ", " + str(data1)     end
      if (data2 != nil)   data_str += ", " + str(data2)     end
      if (cluster == 0x0028) && (event_id == 0x00)
        # put the software version in a readable format
        var val = data0.val
        data_str = format("%i.%i.%i.%i", (val >> 24) & 0xFF, (val >> 16) & 0xFF, (val >> 8) & 0xFF, val & 0xFF)
      end
      var priority_str = (priority == 2) ? "CRIT  " : (priority == 1) ? "INFO  " : "DEBUG "
      var event_name = matter.get_event_name(cluster, event_id)
      event_name = (event_name != nil) ? "(" + event_name + ") " : ""
      log(f"MTR: +Add_Event ({priority_str}{new_event.event_no:8s}) [{new_event.endpoint:02X}]{new_event.cluster:04X}/{new_event.event_id:02X} {event_name}- {data_str}", 2)
    end
    self.queue_event(new_event)
    # check if we have a subscription interested in this new event
    self.device.message_handler.im.subs_shop.event_published(new_event)
  end

end
matter.EventHandler = Matter_EventHandler

#-

# Unit tests


-#

