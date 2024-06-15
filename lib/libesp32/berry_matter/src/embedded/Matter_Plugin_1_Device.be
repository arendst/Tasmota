#
# Matter_Plugin_1_Device.be - implements the behavior for a standard Device
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

#@ solidify:Matter_Plugin_Device.GetOptionReader,weak
#@ solidify:Matter_Plugin_Device,weak

class Matter_Plugin_Device : Matter_Plugin
  # Following arguments are specific to bridge devices
  # static var TYPE = ""                              # name of the plug-in in json
  # static var DISPLAY_NAME = ""                      # display name of the plug-in
  # static var ARG  = ""                              # additional argument name (or empty if none)
  static var ARG_HTTP = "url"                       # domain name
  # static var UPDATE_TIME = 3000                     # update every 3s
  static var UPDATE_CMD = "Status 11"               # command to send for updates
  static var PROBE_TIMEOUT = 1700                   # timeout of 1800 ms for probing, which gives at least 1s for TCP recovery
  static var SYNC_TIMEOUT = 500                     # timeout of 700 ms for probing

  var http_remote                                   # instance of Matter_HTTP_remote

  # clusters for general devices
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    0x0039: [3,5,0x0A,0x0F,0x11,0x12],              # Bridged Device Basic Information 9.13 p.485
    0x0003: [0,1],                                  # Identify 1.2 p.16
    0x0004: [0],                                    # Groups 1.3 p.21
    0x0005: [0,1,2,3,4,5],                          # Scenes 1.4 p.30 - no writable
  })
  static var TYPES = { 0x0013: 1 }                  # fake type
  static var NON_BRIDGE_VENDOR = [ 0x1217, 0x1381 ] # Fabric VendorID not supporting Bridge mode
  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    super(self).init(device, endpoint, arguments)

    if self.BRIDGE
      var addr = arguments.find(self.ARG_HTTP)
      self.http_remote = self.device.register_http_remote(addr, self.PROBE_TIMEOUT)
      self.register_cmd_cb()
    end
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0003              # ========== Identify 1.2 p.16 ==========
      if   attribute == 0x0000          #  ---------- IdentifyTime / u2 ----------
        return tlv_solo.set(TLV.U2, 0)      # no identification in progress
      elif attribute == 0x0001          #  ---------- IdentifyType / enum8 ----------
        return tlv_solo.set(TLV.U1, 0)      # IdentifyType = 0x00 None
      end

    # ====================================================================================================
    elif cluster == 0x0004              # ========== Groups 1.3 p.21 ==========
      if   attribute == 0x0000          #  ----------  ----------
        return nil                      # TODO
      end

    # ====================================================================================================
    elif cluster == 0x0005              # ========== Scenes 1.4 p.30 - no writable ==========

    # ====================================================================================================
    elif cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========

      if   attribute == 0x0000          # ---------- DeviceTypeList / list[DeviceTypeStruct] ----------
        # for device sub-classes, automatically add the Bridge Node type `0x0013: 1`
        # unless the fabric doesn't support bridge mode (currently Alexa)
        var dtl = TLV.Matter_TLV_array()
        var types = self.TYPES
        for dt: types.keys()
          var d1 = dtl.add_struct()
          d1.add_TLV(0, TLV.U2, dt)     # DeviceType
          d1.add_TLV(1, TLV.U2, types[dt])      # Revision
        end
        # if fabric is not Alexa
        if (self.NON_BRIDGE_VENDOR.find(session.get_admin_vendor()) == nil) && (!self.device.disable_bridge_mode)
          var d1 = dtl.add_struct()
          d1.add_TLV(0, TLV.U2, 0x0013)     # DeviceType
          d1.add_TLV(1, TLV.U2, 1)      # Revision
        end
        return dtl
      end

    # ====================================================================================================
    elif cluster == 0x0039             # ========== Bridged Device Basic Information 9.13 p.485 ==========
      import string

      if   attribute == 0x0003          #  ---------- ProductName / string ----------
        if self.BRIDGE
          var name = self.http_remote.get_info().find("name")
          return tlv_solo.set_or_nil(TLV.UTF1, name)
        else
          return tlv_solo.set(TLV.UTF1, tasmota.cmd("DeviceName", true)['DeviceName'])
        end
      elif attribute == 0x0005          #  ---------- NodeLabel / string ----------
        return tlv_solo.set(TLV.UTF1, self.get_name())
      elif attribute == 0x000A          #  ---------- SoftwareVersionString / string ----------
        if self.BRIDGE
          var version_full = self.http_remote.get_info().find("version")
          if version_full
            var version_end = string.find(version_full, '(')
            if version_end > 0    version_full = version_full[0..version_end - 1]   end
            return tlv_solo.set(TLV.UTF1, version_full)
          else
            return tlv_solo.set(TLV.NULL, nil)
          end
        else
          var version_full = tasmota.cmd("Status 2", true)['StatusFWR']['Version']
          var version_end = string.find(version_full, '(')
          if version_end > 0    version_full = version_full[0..version_end - 1]   end
          return tlv_solo.set(TLV.UTF1, version_full)
        end
      elif attribute == 0x000F || attribute == 0x0012          #  ---------- SerialNumber / string ----------
        if self.BRIDGE
          var mac = self.http_remote.get_info().find("mac")
          return tlv_solo.set_or_nil(TLV.UTF1, mac)
        else
          return tlv_solo.set(TLV.UTF1, tasmota.wifi().find("mac", ""))
        end
      elif attribute == 0x0011          #  ---------- Reachable / bool ----------
        if self.BRIDGE
          return tlv_solo.set(TLV.BOOL, self.http_remote.reachable)     # TODO find a way to do a ping
        else
          return tlv_solo.set(TLV.BOOL, 1)     # by default we are reachable
        end
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # Invoke a command
  #
  # returns a TLV object if successful, contains the response
  #   or an `int` to indicate a status
  def invoke_request(session, val, ctx)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var command = ctx.command

    # ====================================================================================================
    if   cluster == 0x0003              # ========== Identify 1.2 p.16 ==========

      if   command == 0x0000            # ---------- Identify ----------
        # ignore
        return true
      elif command == 0x0001            # ---------- IdentifyQuery ----------
        # create IdentifyQueryResponse
        # ID=1
        #  0=Certificate (octstr)
        var iqr = TLV.Matter_TLV_struct()
        iqr.add_TLV(0, TLV.U2, 0)       # Timeout
        ctx.command = 0x00              # IdentifyQueryResponse
        return iqr
      elif command == 0x0040            # ---------- TriggerEffect ----------
        # ignore
        return true
      end
    # ====================================================================================================
    elif cluster == 0x0004              # ========== Groups 1.3 p.21 ==========
      # TODO
      return true

    # ====================================================================================================
    elif cluster == 0x0005              # ========== Scenes 1.4 p.30 ==========
      # TODO
      return true
      
    else
      return super(self).invoke_request(session, val, ctx)
    end
  end

  #############################################################
  # append_state_json
  #
  # Output the current state in JSON.
  # The JSON is build via introspection to see what attributes
  # exist and need to be output
  # New values need to be appended with `,"key":value` (including prefix comma)
  def append_state_json()
    import introspect
    import json
    var ret = ""

    # ret: string
    # attribute: attrbute name
    # key: in json
    def _stats_json_inner(attribute, key)
      import introspect
      import json
      var val
      if (val := introspect.get(self, attribute)) != nil
        if type(val) == 'bool'    val = int(val)  end         # transform bool into 1/0
        ret += f',"{key}":{json.dump(val)}'
      end
    end

    # If sensor with JSON_NAME using `val`
    var json_name = introspect.get(self, 'JSON_NAME')
    if json_name && introspect.contains(self, 'shadow_value')
      var val = (self.shadow_value != nil) ? json.dump(self.shadow_value) : "null"
      ret += f',"{json_name}":{val}'
    end

    # lights
    # print(f'{self=} {type(self)} {introspect.members(self)=}')
    _stats_json_inner("shadow_onoff",      "Power")
    _stats_json_inner("shadow_bri",        "Bri")
    _stats_json_inner("shadow_ct",         "CT")
    _stats_json_inner("shadow_hue",        "Hue")
    _stats_json_inner("shadow_sat",        "Sat")
    # shutters
    _stats_json_inner("shadow_shutter_pos",    "ShutterPos")
    _stats_json_inner("shadow_shutter_target", "ShutterTarget")
    _stats_json_inner("shadow_shutter_tilt",   "ShutterTilt")

    # sensors
    _stats_json_inner("shadow_contact",    "Contact")
    _stats_json_inner("shadow_occupancy",  "Occupancy")

    # air quality
    _stats_json_inner("shadow_air_quality",     "AirQuality")
    _stats_json_inner("shadow_co2",             "CO2")
    _stats_json_inner("shadow_pm1",             "PM1")
    _stats_json_inner("shadow_pm2_5",           "PM2.5")
    _stats_json_inner("shadow_pm10",            "PM10")
    _stats_json_inner("shadow_tvoc",            "TVOC")
    
    # print(ret)
    return ret
  end

  #############################################################
  # For Bridge devices
  #############################################################
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
  # and call `parse_status(<data>, <index>)` when data is available.
  def update_shadow()
    if self.BRIDGE && self.tick != self.device.tick     # don't force an update if we just received it
      var ret = self.call_remote_sync(self.UPDATE_CMD)
      if ret
        self.parse_http_response(1, ret, self.UPDATE_CMD)
      end
    end
    self.tick = self.device.tick
  end

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # TO BE OVERRIDDEN
  def parse_status(data, index)
  end

  #############################################################
  # call_remote_sync
  #
  # Call a remote Tasmota device, returns Berry native map or nil
  # arg can be nil, in this case `cmd` has it all
  def call_remote_sync(cmd, arg)
    if self.BRIDGE
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
        log("MTR: HTTP GET retrying", 3)
      end
      self.http_remote.device_is_alive(false)
      return nil
    end
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
    if self.BRIDGE
      self.tick = self.device.tick        # avoid new force update in same tick
      self.http_remote.parse_status_response_and_call_method(status, payload, cmd, self, self.parse_status)
    end
  end

  #############################################################
  # every_250ms
  #
  # check if the timer expired and update_shadow() needs to be called
  def every_250ms()
    if self.BRIDGE
      self.http_remote.scheduler()          # defer to HTTP scheduler
      # avoid calling update_shadow() since it's not applicable for HTTP remote
  else
      super(self).every_250ms()
    end
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
  #############################################################
  #############################################################

  #######################################################################
  # _parse_sensor_entry: internal helper function
  #
  # Used internally by `update_virtual`
  #
  # Args
  #   payload: the native payload (converted from JSON) from MtrUpdate
  #   key: key name in the JSON payload to read from, do nothing if key does not exist or content is `null`
  #   type_func: type enforcer for value, typically `int`, `bool`, `str`, `number`, `real`
  #   old_val: previous value, used to detect a change or return the value unchanged
  #   cluster/attribute: in case the value has change, publish a change to cluster/attribute
  #
  # Returns:
  #   `old_val` if key does not exist, JSON value is `null`, or value is unchanged
  #   or new value from JSON (which is the new shadow value)
  #
  def _parse_sensor_entry(payload, key, old_val, type_func, cluster, attribute)
    var val = payload.find(key)
    if (val != nil)
      val = type_func(val)
      if val != old_val
        self.attribute_updated(cluster, attribute)   # CurrentPositionTiltPercent100ths
      end
      return val
    end
    return old_val
  end

end
matter.Plugin_Device = Matter_Plugin_Device
