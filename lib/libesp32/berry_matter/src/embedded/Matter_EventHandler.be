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
  var event_no
  var raw_tlv                           # content encoded as full TLV

  def init(event_ib)
    self.endpoint = event_ib.path.endpoint
    self.cluster  = event_ib.path.cluster
    self.event_id = event_ib.path.event
    self.is_urgent = event_ib.path.is_urgent
    self.priority = event_ib.priority
    if (self.priority < 0)                        self.priority = 0                       end
    if (self.priority > matter.EVENT_CRITICAL)    self.priority = matter.EVENT_CRITICAL   end
    self.event_no = int64.toint64(event_ib.event_number)    # int64
    self.raw_tlv = event_ib.to_TLV().tlv2raw()                # bytes()
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
# Matter_IM class
#################################################################################
class Matter_EventHandler
  static var EVENT_NO_INCR = 1000       # counter increased when persisting
  static var EVENT_NO_FILENAME = "_matter_event_no"
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
    var event_no_str = str(persist.find(self.EVENT_NO_FILENAME, "0"))
    self.counter_event_no = int64.fromstring(event_no_str)
    self.counter_event_no_persisted = self.counter_event_no.add(self.EVENT_NO_INCR)
    # save back next slot
    persist.setmember(self.EVENT_NO_FILENAME, self.counter_event_no_persisted.tostring())
    persist.save()
  end

  #####################################################################
  # Enqueue event
  #
  # Takes `Matter_EventDataIB`
  #####################################################################
  def queue_event(ev_ib)
    var ev_queued = matter.EventQueued(ev_ib)

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

  #####################################################################
  # Events handling
  #####################################################################
  # Get next event number
  def get_next_event_no()
    self.counter_event_no = self.counter_event_no.add(1)
    if self.counter_event_no >= self.counter_event_no_persisted
      self.load_event_no_persisted()      # force an increment like done during boot
    end
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

end
matter.EventHandler = Matter_EventHandler

#-

# Unit tests


-#

