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

#@ solidify:Matter_Plugin_Bridge_HTTP.GetOptionReader,weak
#@ solidify:Matter_Plugin_Bridge_HTTP,weak

class Matter_Plugin_Bridge_HTTP : Matter_Plugin_Device
  static var TYPE = ""                              # name of the plug-in in json
  static var DISPLAY_NAME = ""                              # display name of the plug-in
  static var ARG  = ""                              # additional argument name (or empty if none)
  static var ARG_HTTP = "url"                       # domain name
  static var UPDATE_TIME = 3000                     # update every 3s
  static var UPDATE_CMD = "Status 11"               # command to send for updates
  static var PROBE_TIMEOUT = 1700                   # timeout of 1800 ms for probing, which gives at least 1s for TCP recovery
  static var SYNC_TIMEOUT = 500                     # timeout of 700 ms for probing
  # static var CLUSTERS  = {
  #   # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
  #   # 0x0003: inherited                             # Identify 1.2 p.16
  #   # 0x0004: inherited                             # Groups 1.3 p.21
  #   # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
  #   # 0x0006: [0,0xFFFC,0xFFFD],                    # On/Off 1.5 p.48

  #   # 0x0028: [0,1,2,3,4,5,6,7,8,9,0x0A,0x0F,0x12,0x13],# Basic Information Cluster cluster 11.1 p.565
  #   # 0x0039: [0x11]                                  # Bridged Device Basic Information 9.13 p.485

  # }

  var http_remote                                   # instance of Matter_HTTP_remote

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
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
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0039              # ========== Bridged Device Basic Information 9.13 p.485 ==========
      import string

      if   attribute == 0x0003         #  ---------- ProductName / string ----------
        var name = self.http_remote.get_info().find("name")
        if name
          return tlv_solo.set(TLV.UTF1, name)
        else
          return tlv_solo.set(TLV.NULL, nil)
        end
      elif attribute == 0x000A          #  ---------- SoftwareVersionString / string ----------
        var version_full = self.http_remote.get_info().find("version")
        if version_full
          var version_end = string.find(version_full, '(')
          if version_end > 0    version_full = version_full[0..version_end - 1]   end
          return tlv_solo.set(TLV.UTF1, version_full)
        else
          return tlv_solo.set(TLV.NULL, nil)
        end
      elif attribute == 0x000F || attribute == 0x0012          #  ---------- SerialNumber || UniqueID / string ----------
        var mac = self.http_remote.get_info().find("mac")
        if mac
          return tlv_solo.set(TLV.UTF1, mac)
        else
          return tlv_solo.set(TLV.NULL, nil)
        end
      elif attribute == 0x0011          #  ---------- Reachable / bool ----------
        # self.is_reachable_lazy_sync()   # Not needed anymore
        return tlv_solo.set(TLV.BOOL, self.http_remote.reachable)     # TODO find a way to do a ping
      else
        return super(self).read_attribute(session, ctx, tlv_solo)
      end

    else
      return super(self).read_attribute(session, ctx, tlv_solo)
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
  static var PREFIX = "| <i>%s</i> "
  def web_values()
    import webserver
    self.web_values_prefix()
    webserver.content_send("&lt;-- (" + self.DISPLAY_NAME + ") --&gt;")
  end

  # Show prefix before web value
  def web_values_prefix()
    import webserver
    var name = self.get_name()
    webserver.content_send(format(self.PREFIX, name ? webserver.html_escape(name) : ""))
  end

  # Show on/off value as html
  def web_value_onoff(onoff)
    var onoff_html = (onoff != nil ? (onoff ? "<b>On</b>" : "Off") : "")
    return onoff_html
  end

  #############################################################
  # GetOption reader to decode `SetOption<x>` values from `Status 3`
  static class GetOptionReader
    var flag, flag2, flag3, flag4, flag5, flag6

    def init(j)
      if j == nil  raise "value_error", "invalid json"  end
      var so = j['SetOption']
      self.flag  = bytes().fromhex(so[0]).reverse()
      self.flag2 = bytes().fromhex(so[1])
      self.flag3 = bytes().fromhex(so[2]).reverse()
      self.flag4 = bytes().fromhex(so[3]).reverse()
      self.flag5 = bytes().fromhex(so[4]).reverse()
      self.flag6 = bytes().fromhex(so[5]).reverse()
    end
    def getoption(x)
      if   x < 32  # SetOption0 .. 31 = Settings->flag
        return self.flag.getbits(x, 1)
      elif x < 50  # SetOption32 .. 49 = Settings->param
        return self.flag2.get(x - 32, 1)
      elif x < 82  # SetOption50 .. 81 = Settings->flag3
        return self.flag3.getbits(x - 50, 1)
      elif x < 114 # SetOption82 .. 113 = Settings->flag4
        return self.flag4.getbits(x - 82, 1)
      elif x < 146 # SetOption114 .. 145 = Settings->flag5
        return self.flag5.getbits(x - 114, 1)
      elif x < 178 # SetOption146 .. 177 = Settings->flag6
        return self.flag6.getbits(x - 146, 1)
      end
    end
  end

  #- Examples

  import json

  var p = '{"SerialLog":2,"WebLog":3,"MqttLog":0,"SysLog":0,"LogHost":"","LogPort":514,"SSId":["Livebox-781A",""],"TelePeriod":300,"Resolution":"558180C0","SetOption":["00008009","2805C80001800600003C5A0A192800000000","00000080","00006000","00006000","00000020"]}'
  var j = json.load(p)

  var gor = matter.Plugin_Bridge_HTTP.GetOptionReader(j)
  assert(gor.getoption(151) == 1)
  assert(gor.getoption(150) == 0)
  assert(gor.getoption(32) == 40)
  assert(gor.getoption(37) == 128)

  -#
end
matter.Plugin_Bridge_HTTP = Matter_Plugin_Bridge_HTTP
