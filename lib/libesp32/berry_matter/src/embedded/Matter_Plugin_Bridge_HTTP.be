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
  static var PROBE_TIMEOUT = 700                    # timeout of 700 ms for probing
  static var SYNC_TIMEOUT = 500                     # timeout of 700 ms for probing
  static var CLUSTERS  = {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    # 0x0006: [0,0xFFFC,0xFFFD],                    # On/Off 1.5 p.48

    # 0x0028: [0,1,2,3,4,5,6,7,8,9,0x0A,0x0F,0x12,0x13],# Basic Information Cluster cluster 11.1 p.565
    0x0039: [0x11]                                  # Bridged Device Basic Information 9.13 p.485

  }
  # static var TYPES = { 0x010A: 2 }       # On/Off Light

  var reachable                                     # is the device reachable
  var reachable_tick                                # last tick when the reachability was seen (avoids sending superfluous ping commands)

  var http_shadow_map                               # map of shadows
                                                    # <x>: map from json - memorize the result from `Status <x>``
                                                    # each contain a `_tick` attribute to known when they were last loaded
  var http_remote                                   # instance of Matter_HTTP_remote
  var next_probe_timestamp                          # timestamp for next probe (in millis())

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    import string
    super(self).init(device, endpoint, arguments)

    var addr = arguments.find(self.ARG_HTTP)
    self.http_shadow_map = {}
    self.reachable = false                          # force a valid bool value
    self.next_probe_timestamp = nil
    self.http_remote = matter.HTTP_remote(addr, 80, self.PROBE_TIMEOUT)
    self.http_remote.set_cb(/s,p-> self.parse_http_response(s, p))
  end

  #############################################################
  # return the map of all types, add the bridged type
  def get_types()
    var types = {}
    for k: self.TYPES.keys()
      types[k] = self.TYPES[k]
    end
    # Add Bridged Node
    types[0x0013] = 1                   # Bridged Node, v1
    return types
  end

  #############################################################
  # call_remote_async
  #
  # Call a remote Tasmota device, returns nothing, callback is called when data arrives
  def call_remote_async(cmd, arg)
    import string
    if !tasmota.wifi()['up'] && !tasmota.eth()['up']    return nil    end       # no network

    var url = string.format("/cm?cmnd=%s%%20%s", cmd, arg ? arg : '')
    var ret = self.http_remote.begin(url, self.PROBE_TIMEOUT)
    end

  #############################################################
  # call_remote_sync
  #
  # Call a remote Tasmota device, returns Berry native map or nil
  def call_remote_sync(cmd, arg)
    # if !self.http_remote  return nil  end
    import string
    if !tasmota.wifi()['up'] && !tasmota.eth()['up']    return nil    end       # no network

    var retry = 2         # try 2 times if first failed
    var url = string.format("/cm?cmnd=%s%%20%s", cmd, arg ? arg : '')
    while retry > 0
      var ret = self.http_remote.begin_sync(url, self.SYNC_TIMEOUT)
      if ret != nil
        # device is known to be reachable
        self.reachable = true
        self.reachable_tick = self.device.tick
        return ret
      end
      retry -= 1
      tasmota.log("MTR: HTTP GET retrying", 3)
    end
    self.reachable = false
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
  def parse_http_response(status, payload)
    if status > 0
      # device is known to be reachable
      self.reachable = true
      var tick = self.device.tick
      self.reachable_tick = tick

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

        if code != nil
          j['_tick'] = tick
          self.http_shadow_map[code] = j
        end
        # convert to shadow values
        self.parse_update(j, code)          # call parser
      end
    end
  end

  #############################################################
  # is_reachable()
  #
  # Pings the device and checks if it's reachable
  def is_reachable_sync()
    if self.device.tick != self.reachable_tick
      var ret = self.call_remote_sync("", "")        # empty command works as a ping
      self.reachable = (ret != nil)
      # self.reachable_tick = cur_tick    # done by caller
    end
    return self.reachable
  end

  #############################################################
  # get_remote_status_sync()
  #
  # Get remote `Status <x>` values of sensors, sync and blocking
  def get_remote_status_lazy(x, sync)
    var cur_tick = self.device.tick
    var shadow_x = self.http_shadow_map.find(x)
    if shadow_x
      if shadow_x.find('_tick') == cur_tick
        return shadow_x          # we have already updated in this tick
      end
    end
    return self.call_remote_sync("Status", str(x))
  end

  #############################################################
  # probe_shadow_async
  #
  # ### TO BE OVERRIDDEN - DON'T CALL SUPER - default is just calling `update_shadow()`
  # This is called on a regular basis, depending on the type of plugin.
  # Whenever the data is returned, call `update_shadow(<data>)` to update values
  def probe_shadow_async()
    # self.call_remote_async(<cmd>, <data>)
  end

  #############################################################
  # read attribute
  #
  def read_attribute(session, ctx)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0039              # ========== Bridged Device Basic Information 9.13 p.485 ==========

      if   attribute == 0x0000          #  ---------- DataModelRevision / CommissioningWindowStatus ----------
        # return TLV.create_TLV(TLV.U2, 1)
      elif attribute == 0x0011          #  ---------- Reachable / bool ----------
        return TLV.create_TLV(TLV.BOOL, self.reachable)     # TODO find a way to do a ping
      end

    else
      return super(self).read_attribute(session, ctx)
    end
  end

  #############################################################
  # UI Methods
  #############################################################
  # ui_conf_to_string
  #
  # Convert the current plugin parameters to a single string
  static def ui_conf_to_string(cl, conf)
    var s = super(_class).ui_conf_to_string(cl, conf)

    var url = str(conf.find(_class.ARG_HTTP, ''))
    var arg = s + "," + url
    # print("MTR: ui_conf_to_string", conf, cl, arg)
    return arg
  end

  #############################################################
  # ui_string_to_conf
  #
  # Convert the string in UI to actual parameters added to the map
  static def ui_string_to_conf(cl, conf, arg)
    import string
    var elts = string.split(arg + ',', ',', 3)     # add ',' at the end to be sure to have at least 2 arguments
    conf[_class.ARG_HTTP] = elts[1]
    super(_class).ui_string_to_conf(cl, conf, elts[0])
    # print("ui_string_to_conf", conf, arg)
    return conf
  end

end
matter.Plugin_Bridge_HTTP = Matter_Plugin_Bridge_HTTP
