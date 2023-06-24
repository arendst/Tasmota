#
# Matter_HTTP_remote.be - implements an interface to query remotely Tasmota device via HTTP
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

#@ solidify:Matter_HTTP_remote,weak

# dummy declaration for solidification
class Matter_HTTP_async end

#############################################################
# This class has the following purposes:
#
# 1. Have multiple classes registered for async requests
#    and dispatch to all instances results calling `cb(status, payload, index)`
#    Async requests are currently only `Status <x>` requests
#    and are dispatched with the `<x>` index
#
# 2. Implement rate limiting when sending async `Status <x>`
#    requests to align with the lowest cycle time.
#    I.e. if  plugin sends every 2 seconds and another every 3 seconds
#    we only send every 2 seconds and dispatch results to all plugins.

class Matter_HTTP_remote : Matter_HTTP_async
  var probe_update_time_map                         # number of milliseconds to wait for each async command (map)
  var probe_next_timestamp_map                      # timestamp for last probe (in millis()) for each `Status <x>`
                                                    # if timestamp is `0`, this should be scheduled in priority
  var async_cb_map                                  # list of callbacks to call with `cb(http_status, payload)`

  # information about current async command in-flight
  var current_cmd                                   # current async command
                                                    # `nil` if current request is synchronous
  var reachable                                     # is the device reachable
  var reachable_utc                                 # last tick when the reachability was seen (avoids sending superfluous ping commands)
                                                  
                                                  
  #############################################################
  # init
  def init(addr, timeout, fastloop)
    self.probe_update_time_map    = {}
    self.probe_next_timestamp_map = {}
    self.async_cb_map = {}
    self.current_cmd = nil
    self.reachable = false                          # force a valid bool value
    super(self).init(addr, 80, timeout, fastloop)
  end

  #############################################################
  # device is alive, update reachable_utc
  def device_is_alive(alive)
    if alive
      # device is known to be reachable
      self.reachable = true
      self.reachable_utc = tasmota.rtc()['utc']
    else
      self.reachable = false
    end
  end

  #############################################################
  # Add an async command to scheduler
  #
  # cmd: the Tasmota command as string
  # update_time: recurrence in ms (min time of all updates)
  # cb: callback to this command (optional)
  def add_schedule(cmd, update_time, cb)
    if !self.probe_update_time_map.contains(cmd) || update_time < self.probe_update_time_map[cmd]
      # if cmd is not already registered, or if the update_time needs to be reduced
      self.probe_update_time_map[cmd] = update_time
      self.probe_next_timestamp_map[cmd] = matter.jitter(update_time)
    end

    # do we add a cb?
    if cb != nil
      self.add_async_cb(cb, cmd)
    end
  end

  #############################################################
  # Add a callback to be called for a specific `cmd` or `nil` for all commands
  def add_async_cb(cb, cmd)
    self.async_cb_map[cb] = cmd
  end

  #############################################################
  # Send event to all registered cb
  #
  def dispatch_cb(status, payload)
    var idx = 0
    for cb: self.async_cb_map.keys()
      var cmd_filter = self.async_cb_map[cb]
      if cmd_filter == self.current_cmd || cmd_filter == nil      # match command or match any
        cb(status, payload, self.current_cmd)                     # call cb
      end
    end
  end

  #############################################################
  # probe_async
  #
  # Sends a command like `Status <x>` asynchronously to get device status.
  # Implement rate limiting for devices with multiple sub-devices
  def probe_async(cmd)
    import string
    import webserver
    if !tasmota.wifi()['up'] && !tasmota.eth()['up']    return nil    end       # no network

    self.current_cmd = cmd
    var cmd_url = "/cm?cmnd=" + string.tr(cmd, ' ', '+')
    tasmota.log(format("MTR: HTTP async request 'http://%s:%i%s'", self.addr, self.port, cmd_url), 3)
    var ret = self.begin(cmd_url)
  end

  #############################################################
  # begin_sync
  #
  # Synchronous (blocking version)
  #
  # returns nil if something went wrong
  # returns the payload as string
  # Note: sync request aborts any ongoing async request
  def call_sync(cmd, timeout)
    import string
    import webserver
    if !tasmota.wifi()['up'] && !tasmota.eth()['up']    return nil    end       # no network

    self.current_cmd = nil
    var cmd_url = "/cm?cmnd=" + string.tr(cmd, ' ', '+')
    tasmota.log(format("MTR: HTTP sync request 'http://%s:%i%s'", self.addr, self.port, cmd_url), 3)
    var ret = super(self).begin_sync(cmd_url, timeout)
    var payload_short = (ret) ? ret : 'nil'
    if size(payload_short) > 30   payload_short = payload_short[0..29] + '...'   end
    tasmota.log(format("MTR: HTTP sync-resp  in %i ms from %s: [%i] '%s'", tasmota.millis() - self.time_start, self.addr, size(self.payload), payload_short), 3)
    return ret
  end

  def event_http_finished()
    if self.current_cmd == nil    return  end       # do nothing if sync request
    var payload_short = (self.payload != nil) ? self.payload : 'nil'
    if size(payload_short) > 30   payload_short = payload_short[0..29] + '...'   end
    tasmota.log(format("MTR: HTTP async-resp in %i ms from %s: [%i] '%s'", tasmota.millis() - self.time_start, self.addr, size(self.payload), payload_short), 3)
    self.dispatch_cb(self.http_status, self.payload)
  end
  def event_http_failed()
    if self.current_cmd == nil    return  end       # do nothing if sync request
    tasmota.log("MTR: HTTP failed", 3)
    self.dispatch_cb(self.http_status, nil)
  end
  def event_http_timeout()
    if self.current_cmd == nil    return  end       # do nothing if sync request
    tasmota.log(format("MTR: HTTP timeout http_status=%i phase=%i tcp_status=%i size_payload=%i", self.http_status, self.phase, self.status, size(self.payload)), 3)
    self.dispatch_cb(self.http_status, nil)
  end

  #############################################################
  # scheduler
  #
  # check if the timer expired and update_shadow() needs to be called
  def scheduler()
    var cmd = nil                                 # keep trakck of command
    for k: self.probe_next_timestamp_map.keys()
      if self.probe_next_timestamp_map[k] == 0
        cmd = k                                   # schedule in priority
        break
      end
    end

    if cmd == nil                                   # if no priotity, find first eligible
      for k: self.probe_next_timestamp_map.keys()
        if tasmota.time_reached(self.probe_next_timestamp_map[k])
          cmd = k
          break
        end
      end
    end

    if cmd == nil     return    end                 # nothing to do

    if self.tcp_connected                         # we have already an async request in-flight
      self.probe_next_timestamp_map[cmd] = 0        # mark as priority scheduling for near future
      return
    end

    # set new next timestamp
    self.probe_next_timestamp_map[cmd] = tasmota.millis(self.probe_update_time_map[cmd])    # add update_time for command

    # trigger the command
    self.probe_async(cmd)
  end

  #############################################################
  # web_last_seen
  #
  # Show when the device was last seen
  def web_last_seen()
    import webserver

    var seconds = -1                      # default if no known value
    if self.reachable_utc != nil
      seconds = tasmota.rtc()['utc'] - self.reachable_utc
    end
    return matter.seconds_to_dhm(seconds)
  end
end
matter.HTTP_remote = Matter_HTTP_remote
