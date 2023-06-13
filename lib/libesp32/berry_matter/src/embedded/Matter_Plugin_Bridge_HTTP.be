#
# Matter_Plugin_Bridge_HTTP.be - implements base class for a Bridge via HTTP
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

# Matter plug-in for core behavior

# dummy declaration for solidification
class Matter_Plugin_Device end

#@ solidify:Matter_Plugin_Bridge_HTTP,weak

class Matter_Plugin_Bridge_HTTP : Matter_Plugin_Device
  static var TYPE = ""                              # name of the plug-in in json
  static var NAME = ""                              # display name of the plug-in
  static var ARG  = ""                              # additional argument name (or empty if none)
  static var ARG_HTTP = "url"                       # domain name
  static var UPDATE_TIME = 3000                     # update every 3s
  static var UPDATE_CMD = "Status 11"               # command to send for updates
  static var PROBE_TIMEOUT = 1700                   # timeout of 1800 ms for probing, which gives at least 1s for TCP recovery
  static var SYNC_TIMEOUT = 500                     # timeout of 700 ms for probing
  static var CLUSTERS  = {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    # 0x0006: [0,0xFFFC,0xFFFD],                    # On/Off 1.5 p.48

    # 0x0028: [0,1,2,3,4,5,6,7,8,9,0x0A,0x0F,0x12,0x13],# Basic Information Cluster cluster 11.1 p.565
    # 0x0039: [0x11]                                  # Bridged Device Basic Information 9.13 p.485

  }

  var http_remote                                   # instance of Matter_HTTP_remote

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    import string
    super(self).init(device, endpoint, arguments)

    var addr = arguments.find(self.ARG_HTTP)
    self.http_remote = self.device.register_http_remote(addr, self.PROBE_TIMEOUT)
    self.register_cmd_cb()
  end

  #############################################################
  # is_local_device
  #
  # Returns true if it's a local device, or false for a
  # remotely device controlled via HTTP
  def is_local_device()
    return false
  end
  
  #############################################################
  # register_cmd_cb
  #
  # Register recurrent command and callback
  # Defined as a separate method to allow override
  def register_cmd_cb()
    self.http_remote.add_schedule(self.UPDATE_CMD, self.UPDATE_TIME,
                                  / status,payload,cmd -> self.parse_http_response(status,payload,cmd))
  end

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This method should collect the data from the local or remote device
  # and call `parse_update(<data>)` when data is available.
  def update_shadow()
    self.tick = self.device.tick
    var ret = self.call_remote_sync(self.UPDATE_CMD)
    if ret
      self.parse_http_response(1, ret, self.UPDATE_CMD)
    end
  end

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # TO BE OVERRIDDEN
  def parse_update(data, index)
  end

  #############################################################
  # call_remote_sync
  #
  # Call a remote Tasmota device, returns Berry native map or nil
  # arg can be nil, in this case `cmd` has it all
  def call_remote_sync(cmd, arg)
    # if !self.http_remote  return nil  end
    import string
    import json

    var retry = 2         # try 2 times if first failed
    if arg != nil     cmd = cmd + ' ' + str(arg)      end
    while retry > 0
      var ret = self.http_remote.call_sync(cmd, self.SYNC_TIMEOUT)
      if ret != nil
        self.http_remote.device_is_alive(true)
        var j = json.load(ret)
        return j
      end
      retry -= 1
      tasmota.log("MTR: HTTP GET retrying", 3)
    end
    self.http_remote.device_is_alive(false)
    return nil
  end

  #############################################################
  # parse_http_response
  #
  # Parse response from HTTP API and update shadows
  # We support:
  #     `Status  8`: {"StatusSNS":{ [...] }}
  #     `Status 11`: {"StatusSTS":{ [...] }}
  #     `Status 13`: {"StatusSHT":{ [...] }}
  def parse_http_response(status, payload, cmd)
    if status > 0
      # device is known to be reachable
      self.http_remote.device_is_alive(true)
      var tick = self.device.tick

      import json
      var j = json.load(payload)
      var code = nil                        # index of Status
      if j
        # filter
        if   j.contains("StatusSNS")        # Status 8
          j = j["StatusSNS"]
          code = 8
        elif j.contains("StatusSTS")        # Status 11
          j = j["StatusSTS"]
          code = 11
        elif j.contains("StatusSHT")        # Status 13
          j = j["StatusSTS"]
          code = 13
        end
        # convert to shadow values
        self.parse_update(j, code)          # call parser
      end
    end
  end

  # #############################################################
  # # is_reachable()
  # #
  # # Pings the device and checks if it's reachable
  # def is_reachable_lazy_sync()
  #   var cur_tick = self.device.tick
  #   if cur_tick != self.tick
  #     var ret = self.call_remote_sync("", "")        # empty command works as a ping
  #     self.http_remote.device_is_alive(ret != nil)
  #   end
  #   return self.http_remote.reachable
  # end

  #############################################################
  # read attribute
  #
  def read_attribute(session, ctx)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0039              # ========== Bridged Device Basic Information 9.13 p.485 ==========

      if   attribute == 0x0011          #  ---------- Reachable / bool ----------
        # self.is_reachable_lazy_sync()   # Not needed anymore
        return TLV.create_TLV(TLV.BOOL, self.http_remote.reachable)     # TODO find a way to do a ping
      else
        return super(self).read_attribute(session, ctx)
      end

    else
      return super(self).read_attribute(session, ctx)
    end
  end

  #############################################################
  # every_250ms
  #
  # check if the timer expired and update_shadow() needs to be called
  def every_250ms()
    self.http_remote.scheduler()          # defer to HTTP scheduler
    # avoid calling update_shadow() since it's not applicable for HTTP remote
  end

  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    import webserver
    webserver.content_send("| &lt;-- (" + self.NAME + ") --&gt;")
  end

  # Show on/off value as html
  def web_value_onoff(onoff)
    var onoff_html = (onoff != nil ? (onoff ? "<b>On</b>" : "Off") : "")
    return onoff_html
  end
end
matter.Plugin_Bridge_HTTP = Matter_Plugin_Bridge_HTTP
