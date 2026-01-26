#
# Matter_Path_1_EventGenerator.be - suppport for Matter event path generator from available events
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

#@ solidify:Matter_EventGenerator,weak

#################################################################################
# Matter_EventGenerator
#
# Has the same interface as PathGenerator
#
# Returns a Path that reads an event patching the current pattern
#################################################################################
class Matter_EventGenerator
  var device                  # reference of device main object
  var path_in_endpoint        # input endpoint filter (nil or int)
  var path_in_cluster         # input cluster filter (nil or int)
  var path_in_event           # input event filter (nil or int)
  var path_in_event_min       # input event filter for minimum event requested (nil or int)
  # current status
  var event_no                # event number of the last one returned or nil of none returned yet

  # reused at each output
  var finished                # finished (no more events matching)

  #################################################################################
  # simple constructor
  #
  def init(device)
    self.device = device
  end

  #################################################################################
  # start generator
  #
  # `in_endpoint`: endpoint number filter (int or nil for all)
  # `in_cluster`: cluster number filter (int or nil for all)
  # `in_attribute`: attribute number filter (int or nil for all)
  # `in_fabric_filtered`: is the filter fabric-filtered (nil or false or true) - currently stored but ignored
  def start(in_endpoint, in_cluster, in_event, in_event_min)
    self.reset()
    self.path_in_endpoint = in_endpoint
    self.path_in_cluster = in_cluster
    self.path_in_event = in_event
    self.path_in_event_min = in_event_min
    self.event_no = in_event_min
    self.finished = false
  end

  #################################################################################
  # reset
  #
  def reset()
    var n = nil
    self.event_no = n
    self.finished = true
  end

  #################################################################################
  # restart_from
  #
  # Restart from this event number (not included)
  def restart_from(event_min)
    self.finished = false
    self.event_no = event_min
  end

  ################################################################################
  # is_finished
  #
  # Returns `true` if we have exhausted the generator
  def is_finished()
    return self.finished
  end

  ################################################################################
  # finished
  #
  # not used for events
  def get_pi()
    return nil
  end

  ################################################################################
  # next_event
  #
  # Generate next concrete path
  # Returns:
  # - a path object (that is valid until next call)
  # - if 'direct' (concrete path), ctx.status contains the appropriate error code if the path value is not supported
  # - `nil` if no more objects
  def next_event()
    if (self.finished)    return nil    end
    while true
      var next_event = self.device.events.find_min_no(self.event_no)
      if (next_event == nil)                      # eventually there are no events left, so the loop stops
        self.reset()
        return nil
      end
      # next_event != nil and we know that ((next_event.event_no > self.path_in_event_min) or (path_in_event_min == nil))
      self.event_no = next_event.event_no         # update smallest found event, even if it does not match, because we need it to find the next one matching
      # check if the event matches the pattern

      if self.event_is_match(next_event)
        # log(f"MTR: Path generator([{self.path_in_endpoint}]{self.path_in_cluster}/{self.path_in_event}>{self.path_in_event_min}} event_no={self.event_no} event={matter.inspect(next_event)})", 3)
        return next_event
      end
    end
  end

  ################################################################################
  # event_is_match
  #
  # Returns true if the event passed as argument matches the filter
  # but does not check the event number, because it is filtered out before
  def event_is_match(next_event)
    var match = true
    if (self.path_in_endpoint != nil)
      match = match && (self.path_in_endpoint == next_event.endpoint)
    end
    if (self.path_in_cluster != nil)
      match = match && (self.path_in_cluster == next_event.cluster)
    end
    if (self.path_in_event != nil)
      match = match && (self.path_in_event == next_event.event_id)
    end
    return match
  end
end
matter.EventGenerator = Matter_EventGenerator
