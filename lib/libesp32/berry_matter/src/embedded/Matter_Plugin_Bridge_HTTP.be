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
  static var HTTP_TIMEOUT = 300                     # wait for 300ms max, since we're on LAN
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

  var tasmota_http                                  # domain name for HTTP, ex: 'http://192.168.1.10/'
  var tasmota_status_8                              # remote `Status 8` sensor values (last known)
  var tasmota_status_11                             # remote `Status 11` light values (last known)
                                                    # each contain a `_tick` attribute to known when they were last loaded
  var reachable                                     # is the device reachable
  var reachable_tick                                # last tick when the reachability was seen (avoids sending superfluous ping commands)

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    import string
    super(self).init(device, endpoint, arguments)

    var http = arguments.find(self.ARG_HTTP)
    if http
      if string.find(http, '://') < 0
        http = "http://" + http + "/"
      end
      self.tasmota_http = http
    else
      tasmota.log(string.format("MTR: ERROR: 'url' is not configured for endpoint %i", endpoint), 2)
    end
    self.tasmota_status_8 = nil
    self.tasmota_status_11 = nil
    self.reachable = false                          # force a valid bool value
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
  # call_remote
  #
  # Call a remote Tasmota device, returns Berry native map or nil
  def call_remote(cmd, arg)
    if !self.tasmota_http  return nil  end
    import json
    import string
    if !tasmota.wifi()['up'] && !tasmota.eth()['up']    return nil    end       # no network
    var retry = 2         # try 2 times if first failed
    while retry > 0
      var cl = webclient()
      cl.set_timeouts(1000, 1000)
      cl.set_follow_redirects(false)
      var url = string.format("%scm?cmnd=%s%%20%s", self.tasmota_http, cmd, arg ? arg : '')
      tasmota.log("MTR: HTTP GET "+url, 3)
      cl.begin(url)
      var r = cl.GET()
      tasmota.log("MTR: HTTP GET code=" + str(r), 3)
      if r == 200
        var s = cl.get_string()
        cl.close()
        tasmota.log("MTR: HTTP GET payload=" + s, 3)
        var j = json.load(s)
        # device is known to be reachable
        self.reachable = true
        self.reachable_tick = self.device.tick
        return j
      end
      cl.close()

      retry -= 1
      tasmota.log("MTR: HTTP GET retrying", 3)
    end
    self.reachable = false
    return nil
  end

  #############################################################
  # is_reachable()
  #
  # Pings the device and checks if it's reachable
  def is_reachable()
    if self.device.tick != self.reachable_tick
      var ret = self.call_remote("", "")        # empty command works as a ping
      self.reachable = (ret != nil)
      # self.reachable_tick = cur_tick    # done by caller
    end
    return self.reachable
  end

  #############################################################
  # get_status_8()
  #
  # Get remote `Status 8` values of sensors, and cache for the current tick
  def get_status_8()
    var cur_tick = self.device.tick
    if self.tasmota_status_8 == nil  ||  self.tasmota_status_8.find("_tick") != cur_tick
      var ret = self.call_remote("Status", "8")        # call `Status 8`
      if ret
        ret["_tick"] = cur_tick
        self.tasmota_status_8 = ret
        return ret
      else
        return nil
      end
    else
      return self.tasmota_status_8                    # return cached value
    end
  end

  #############################################################
  # get_status_11()
  #
  # Get remote `Status 11` values of sensors, and cache for the current tick
  def get_status_11()
    var cur_tick = self.device.tick
    if self.tasmota_status_11 == nil  ||  self.tasmota_status_11.find("_tick") != cur_tick
      var ret = self.call_remote("Status", "11")      # call `Status 8`
      if ret
        ret["_tick"] = cur_tick
        self.tasmota_status_11 = ret
        return ret
      else
        return nil
      end
    else
      return self.tasmota_status_11                   # return cached value
    end
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
        return TLV.create_TLV(TLV.BOOL, true)     # TODO find a way to do a ping
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
    print("MTR: ui_conf_to_string", conf, cl, arg)
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
    print("ui_string_to_conf", conf, arg)
    return conf
  end

end
matter.Plugin_Bridge_HTTP = Matter_Plugin_Bridge_HTTP
