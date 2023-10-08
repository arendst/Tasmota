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
  var device                                        # reference to matter_device
  var probe_update_time_map                         # number of milliseconds to wait for each async command (map)
  var probe_next_timestamp_map                      # timestamp for last probe (in millis()) for each `Status <x>`
                                                    # if timestamp is `0`, this should be scheduled in priority
  var async_cb_map                                  # list of callbacks to call with `cb(http_status, payload)`

  # information about current async command in-flight
  var current_cmd                                   # current async command
                                                    # `nil` if current request is synchronous
  var reachable                                     # is the device reachable
  var reachable_utc                                 # last tick when the reachability was seen (avoids sending superfluous ping commands)
                                                  
  # information gathered about the remote device (name, version...)
  static var UPDATE_TIME = 5000                     # update every 5s until first response
  static var UPDATE_TIME2 = 300000                  # then update every 5 minutes
  static var UPDATE_CMD0 = "Status"                 # command to send for updates
  static var UPDATE_CMD2 = "Status 2"               # command to send for updates
  static var UPDATE_CMD5 = "Status 5"               # command to send for updates
  var info                                          # as a map
                                                  
  #############################################################
  # init
  def init(device, addr, timeout, fastloop)
    self.device = device                            # if device is null, it's a temporary object not linked to a device
    self.probe_update_time_map    = {}
    self.probe_next_timestamp_map = {}
    self.async_cb_map = {}
    self.current_cmd = nil
    self.reachable = false                          # force a valid bool value
    super(self).init(addr, 80, timeout, fastloop)

    # set up some reading information about the device
    self.info = {}
    if self.device
      # we need different callbacks per command (don't create a single one for both calls)
      self.add_schedule(self.UPDATE_CMD0, self.UPDATE_TIME, / status,payload,cmd -> self.parse_status_response(status,payload,cmd))
      self.add_schedule(self.UPDATE_CMD2, self.UPDATE_TIME, / status,payload,cmd -> self.parse_status_response(status,payload,cmd))
      self.add_schedule(self.UPDATE_CMD5, self.UPDATE_TIME, / status,payload,cmd -> self.parse_status_response(status,payload,cmd))
    end
  end

  #############################################################
  # get/set remote_info map
  def get_info()      return self.info                    end
  def set_info(v)     self.info = v                       end
  def info_changed()  self.device.save_param()            end     # send a signal to global device that remote information changed

  #############################################################
  # parse response for `Status` and `Status 2`
  def parse_status_response(status, payload, cmd)
    if status != nil && status > 0
      # device is known to be reachable
      self.device_is_alive(true)

      import json
      var j = json.load(payload)
      var code = nil                        # index of Status
      if j
        # filter
        if   j.contains("Status")           # Status 0 (actually `Status` wihtout any number)
          j = j["Status"]
          code = 0
        elif j.contains("StatusFWR")        # Status 2
          j = j["StatusFWR"]
          code = 2
        elif j.contains("StatusNET")        # Status 5
          j = j["StatusNET"]
          code = 5
        end
        # convert to shadow values
        self.parse_update(j, code)          # call parser
      end
    end
  end
  
  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This call is synnchronous and blocking.
  def parse_update(data, index)
    var changed = false
    if index == 0                               # Status
      var device_name = data.find("DeviceName")              # we consider 'Tasmota' as the non-information default
      if device_name == "Tasmota"   device_name = nil   end

      # did the value changed?
      if self.info.find('name') != device_name
        if device_name != nil
          self.info['name'] = device_name
        else
          self.info.remove("name")
        end
        tasmota.log(f"MTR: update '{self.addr}' name='{device_name}'", 3)
        changed = true
      end

      # reduce the update time after a read is succesful
      self.change_schedule(self.UPDATE_CMD0, self.UPDATE_TIME2)
    elif index == 2                             # Status 2
      var version = data.find("Version")
      var hardware = data.find("Hardware")

      if self.info.find('version') != version
        if version != nil
          self.info['version'] = version
        else
          self.info.remove('version')
        end
        tasmota.log(f"MTR: update '{self.addr}' version='{version}'", 3)
        changed = true
      end

      if self.info.find('hardware') != hardware
        if hardware != nil
          self.info['hardware'] = hardware
        else
          self.info.remove('hardware')
        end
        tasmota.log(f"MTR: update '{self.addr}' hardware='{hardware}'", 3)
        changed = true
      end

      # reduce the update time after a read is succesful
      self.change_schedule(self.UPDATE_CMD2, self.UPDATE_TIME2)
    elif index == 5
      var mac = data.find("Mac")

      # did the value changed?
      if self.info.find('mac') != mac
        if mac != nil
          self.info['mac'] = mac
        else
          self.info.remove("mac")
        end
        tasmota.log(f"MTR: update '{self.addr}' mac='{mac}'", 3)
        changed = true
      end

      # reduce the update time after a read is succesful
      self.change_schedule(self.UPDATE_CMD5, self.UPDATE_TIME2)
    end

    if changed      self.info_changed()   end
  end

  #############################################################
  # device is alive, update reachable_utc
  def device_is_alive(alive)
    if alive
      # device is known to be reachable
      self.reachable = true
      self.reachable_utc = tasmota.rtc_utc()
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
  # Change schedule of current cmd
  def change_schedule(cmd, update_time)
    if self.probe_update_time_map.contains(cmd)
      self.probe_update_time_map[cmd] = update_time
      self.probe_next_timestamp_map[cmd] = matter.jitter(update_time)
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
      seconds = tasmota.rtc_utc() - self.reachable_utc
    end
    return matter.seconds_to_dhm(seconds)
  end
end
matter.HTTP_remote = Matter_HTTP_remote
