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

#################################################################################
# Matter 1.4.1 Base Device Class
#################################################################################
# CLASS: Matter_Plugin_Device (Base class for all device endpoints)
# INHERITS FROM: Matter_Plugin
#
# PURPOSE:
# - Base class for all Matter device implementations
# - Provides common device functionality (Identify, Groups, Scenes)
# - Handles bridge mode for remote Tasmota devices
# - Manages Bridged Device Basic Information cluster
# - Supports both local and remote (HTTP) devices
# - Provides Zigbee device integration support
#
# CLUSTERS (Common to all devices):
# - 0x0039: Bridged Device Basic Information (M) - Device metadata
# - 0x0003: Identify (M) - Device identification
# - 0x0004: Groups (M) - Group management
# - 0x0005: Scenes (M) - Scene management (deprecated, use 0x0062)
# - 0x001D: Descriptor (M) - Inherited from base class
#
# BRIDGE MODE:
# - Supports remote Tasmota devices via HTTP
# - Automatic device discovery and status polling
# - Configurable update intervals and timeouts
# - HTTP command execution with retry logic
# - Reachability monitoring
#
# ZIGBEE SUPPORT:
# - Integration with Tasmota Zigbee devices
# - Automatic attribute mapping
# - Event-driven updates from Zigbee coordinator
#
# VIRTUAL DEVICE SUPPORT:
# - Receives updates via Matter bridge protocol
# - JSON-based state updates
# - Supports all device types as virtual endpoints
#################################################################################

#################################################################################
# Matter 1.4.1 Bridged Device Basic Information Cluster (0x0039)
#################################################################################
# Cluster Revision: 3 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# ATTRIBUTES:
# ID     | Name                  | Type   | Constraint | Quality | Default | Access | Conf
# -------|-----------------------|--------|------------|---------|---------|--------|-----
# 0x0003 | ProductName           | string | max 32     |         | MS      | R V    | O
# 0x0005 | NodeLabel             | string | max 32     | N       | ""      | RW VM  | M
# 0x000A | SoftwareVersionString | string | max 64     |         | MS      | R V    | O
# 0x000F | SerialNumber          | string | max 32     |         | MS      | R V    | O
# 0x0011 | Reachable             | bool   | all        |         | TRUE    | R V    | M
# 0x0012 | UniqueID              | string | max 32     |         | MS      | R V    | O
#
# Quality Flags:
# - N: Non-volatile (persists across reboots)
# - MS: Manufacturer specific
#
# Access Control:
# - R: Read, W: Write
# - V: View privilege, M: Manage privilege
#
# TASMOTA IMPLEMENTATION:
# - ProductName: From DeviceName (local) or remote device name
# - NodeLabel: User-configurable endpoint name
# - SoftwareVersionString: Tasmota version (e.g., "13.4.0")
# - SerialNumber/UniqueID: MAC address
# - Reachable: true for local, HTTP ping status for remote
#
# BRIDGE MODE SPECIFICS:
# - Remote device info cached from HTTP responses
# - Periodic reachability checks
# - Automatic reconnection on network recovery
#################################################################################

#################################################################################
# Matter 1.4.1 Identify Cluster (0x0003)
#################################################################################
# See earlier documentation for full Identify cluster specification
# Provides device identification functionality (visual/audible indication)
#################################################################################

#################################################################################
# Matter 1.4.1 Groups Cluster (0x0004)
#################################################################################
# See earlier documentation for full Groups cluster specification
# Enables grouping multiple devices for simultaneous control
#################################################################################

#################################################################################
# Matter 1.4.1 Scenes Cluster (0x0005) - DEPRECATED
#################################################################################
# NOTE: Scenes cluster 0x0005 is deprecated in Matter 1.4.1
# Use Scenes Management cluster 0x0062 instead (PROVISIONAL)
# Retained for backward compatibility
#################################################################################

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
  var mqtt_remote                                   # instance of Matter_MQTT_remote
  var clusters                                      # per-instance cluster map for the selected composition mode

  # clusters for general devices
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    0x0039: [3,5,0x0A,0x0F,0x11,0x12],              # Bridged Device Basic Information 9.13 p.485
    0x0003: [0,1],                                  # Identify 1.2 p.16
    0x0004: [0],                                    # Groups 1.3 p.21
    0x0005: [0,1,2,3,4,5],                          # Scenes 1.4 p.30 - no writable
  })
  static var TYPES = { 0x0013: 1 }                  # fake type
  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    # CLUSTERS is a shared solidified map. Build a private filtered map for
    # native composition rather than mutating class metadata.
    self.clusters = self.CLUSTERS
    if device.disable_bridge_mode
      var non_bridge_clusters = {}
      for cluster: self.CLUSTERS.keys()
        if cluster != 0x0039
          non_bridge_clusters[cluster] = self.CLUSTERS[cluster]
        end
      end
      self.clusters = non_bridge_clusters
    end

    # Zigbee code, activated only when `ZIGBEE` is true
    # attribute `zigbee_mapper` needs to be defined for classes with `ZIGBEE` true
    if self.ZIGBEE
      self.zigbee_mapper = device.create_zb_mapper(self)  # needs to exist before `parse_configuration()` is called
    end

    super(self).init(device, endpoint, arguments)

    if self.BRIDGE
      var topic = arguments.find("topic")
      if topic
        # MQTT bridge mode
        self.mqtt_remote = self.device.register_mqtt_remote(topic)
      else
        # HTTP bridge mode (default)
        var addr = arguments.find(self.ARG_HTTP)
        self.http_remote = self.device.register_http_remote(addr, self.PROBE_TIMEOUT)
      end
      self.register_cmd_cb()
    end
  end

  #############################################################
  # Return the mode-specific cluster map used by Descriptor and IM routing.
  def get_clusters()
    return self.clusters
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map, handling case of Zigbee configuration
  def parse_configuration(config)
    # super(self).parse_configuration(config)   # not necessary because the superclass does nothing
    if self.ZIGBEE && self.zigbee_mapper
      self.zigbee_mapper.parse_configuration(config)
    end
  end
  
  #############################################################
  # Called when the value changed compared to shadow value
  #
  # This must be overriden.
  # This is where you call `self.attribute_updated(<cluster>, <attribute>)`
  def value_changed()
    # self.attribute_updated(0x0402, 0x0000)
  end

  #############################################################
  # Pre-process value
  #
  # This must be overriden.
  # This allows to convert the raw sensor value to the target one, typically int
  def pre_value(val)
    return val
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
        return tlv_solo.set(0x06 #-TLV.U4-#, 0)      # no identification in progress
      elif attribute == 0x0001          #  ---------- IdentifyType / enum8 ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, 0)      # IdentifyType = 0x00 None
      end

    # ====================================================================================================
    elif cluster == 0x0004              # ========== Groups 1.3 p.21 ==========
      if   attribute == 0x0000          #  ---------- NameSupport ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.device.GROUP_TRANSPORT_READY ? 0x80 : 0)
      elif attribute == 0xFFFC          # ---------- FeatureMap ----------
        return tlv_solo.set(0x06 #-TLV.U4-#, self.device.GROUP_TRANSPORT_READY ? 1 : 0)
      end

    # ====================================================================================================
    elif cluster == 0x0005              # ========== Scenes 1.4 p.30 - no writable ==========

    # ====================================================================================================
    elif cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========

      if   attribute == 0x0000          # ---------- DeviceTypeList / list[DeviceTypeStruct] ----------
        # Device subclasses report their primary type first. Bridge mode then
        # adds the Bridged Node utility type to every application endpoint.
        var dtl = TLV.Matter_TLV_array()
        var types = self.TYPES
        for dt: types.keys()
          var d1 = dtl.add_struct()
          d1.add_TLV(0, 0x06 #-TLV.U4-#, dt)     # DeviceType
          d1.add_TLV(1, 0x06 #-TLV.U4-#, types[dt])      # Revision
        end
        # Bridged Node is part of the global bridge composition. Descriptor
        # identity must not vary with the fabric reading this fixed attribute.
        if !self.device.disable_bridge_mode
          var d1 = dtl.add_struct()
          d1.add_TLV(0, 0x06 #-TLV.U4-#, 0x0013)     # DeviceType
          d1.add_TLV(1, 0x06 #-TLV.U4-#, 1)          # Revision
        end
        return dtl
      end

    # ====================================================================================================
    elif cluster == 0x0039             # ========== Bridged Device Basic Information 9.13 p.485 ==========
      import string

      if   attribute == 0x0003          #  ---------- ProductName / string (not nullable) ----------
        if self.BRIDGE
          var remote = self.mqtt_remote ? self.mqtt_remote : self.http_remote
          var name = remote ? remote.get_info().find("name", "") : ""
          return tlv_solo.set(0x0C #-TLV.UTF1-#, name)
        else
          return tlv_solo.set(0x0C #-TLV.UTF1-#, tasmota.cmd("DeviceName", true)['DeviceName'])
        end
      elif attribute == 0x0005          #  ---------- NodeLabel / string ----------
        return tlv_solo.set(0x0C #-TLV.UTF1-#, self.get_name())
      elif attribute == 0x000A          #  ---------- SoftwareVersionString / string (not nullable) ----------
        if self.BRIDGE
          var remote = self.mqtt_remote ? self.mqtt_remote : self.http_remote
          var version_full = remote ? remote.get_info().find("version") : nil
          if version_full
            var version_end = string.find(version_full, '(')
            if version_end > 0    version_full = version_full[0..version_end - 1]   end
            return tlv_solo.set(0x0C #-TLV.UTF1-#, version_full)
          else
            return tlv_solo.set(0x0C #-TLV.UTF1-#, "")     # spec is not nullable, return empty until first poll
          end
        else
          var version_full = tasmota.cmd("Status 2", true)['StatusFWR']['Version']
          var version_end = string.find(version_full, '(')
          if version_end > 0    version_full = version_full[0..version_end - 1]   end
          return tlv_solo.set(0x0C #-TLV.UTF1-#, version_full)
        end
      elif attribute == 0x000F || attribute == 0x0012          #  ---------- SerialNumber / UniqueID / string (not nullable) ----------
        if self.BRIDGE
          var remote = self.mqtt_remote ? self.mqtt_remote : self.http_remote
          var mac = remote ? remote.get_info().find("mac", "") : ""
          return tlv_solo.set(0x0C #-TLV.UTF1-#, mac)
        else
          return tlv_solo.set(0x0C #-TLV.UTF1-#, tasmota.wifi().find("mac", ""))
        end
      elif attribute == 0x0011          #  ---------- Reachable / bool ----------
        if self.BRIDGE
          var remote = self.mqtt_remote ? self.mqtt_remote : self.http_remote
          var reachable = remote ? remote.reachable : false
          return tlv_solo.set(0x08 #-TLV.BOOL-#, reachable)
        else
          return tlv_solo.set(0x08 #-TLV.BOOL-#, 1)     # by default we are reachable
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
        iqr.add_TLV(0, 0x06 #-TLV.U4-#, 0)       # Timeout
        ctx.command = 0x00              # IdentifyQueryResponse
        return iqr
      elif command == 0x0040            # ---------- TriggerEffect ----------
        # ignore
        return true
      end
    # ====================================================================================================
    elif cluster == 0x0004              # ========== Groups 1.3 p.21 ==========
      # Persist membership management only when encrypted multicast reception
      # is available. Until then commands fail closed instead of advertising a
      # group configuration whose messages would be silently discarded.
      var fabric = session.get_fabric()
      if fabric == nil
        ctx.status = 0x7E #-matter.UNSUPPORTED_ACCESS-#
        return nil
      end

      if command == 0x0000              # ---------- AddGroup ----------
        var group_id = val.findsubval(0)
        var group_name = val.findsubval(1, "")
        var status = 0
        if group_id == nil
          ctx.status = 0x85 #-matter.INVALID_COMMAND-#
          return nil
        elif !self.device.GROUP_TRANSPORT_READY
          status = 0x01 #-matter.FAILURE-#
        elif type(group_name) != 'string' || size(group_name) > 16
          status = 0x87 #-matter.CONSTRAINT_ERROR-#
        elif group_id == nil || group_id < 1 || group_id > 0xFEFF
          status = 0x87 #-matter.CONSTRAINT_ERROR-#
        elif fabric.find_group(group_id) == nil && size(fabric.get_group_table()) >= 4
          status = 0x89 #-matter.RESOURCE_EXHAUSTED-#
        else
          fabric.add_group_endpoint(group_id, self.endpoint, group_name)
        end
        var response = TLV.Matter_TLV_struct()
        response.add_TLV(0, 0x06 #-TLV.U4-#, status)
        response.add_TLV(1, 0x06 #-TLV.U4-#, group_id)
        ctx.command = 0x0000             # AddGroupResponse
        return response

      elif command == 0x0001            # ---------- ViewGroup ----------
        var group_id = val.findsubval(0)
        if group_id == nil
          ctx.status = 0x85 #-matter.INVALID_COMMAND-#
          return nil
        end
        var group = fabric.find_group(group_id)
        var member = self.device.GROUP_TRANSPORT_READY &&
                     group != nil && group.find("endpoints", []).find(self.endpoint) != nil
        var response = TLV.Matter_TLV_struct()
        response.add_TLV(0, 0x06 #-TLV.U4-#,
                         group_id == 0 ? 0x87 #-matter.CONSTRAINT_ERROR-# :
                         member ? 0 : 0x8B #-matter.NOT_FOUND-#)
        response.add_TLV(1, 0x06 #-TLV.U4-#, group_id)
        response.add_TLV(2, 0x0C #-TLV.UTF1-#, member ? group.find("name", "") : "")
        ctx.command = 0x0001             # ViewGroupResponse
        return response

      elif command == 0x0002            # ---------- GetGroupMembership ----------
        var requested = val.findsub(0)
        var response = TLV.Matter_TLV_struct()
        response.add_TLV(0, 0x06 #-TLV.U4-#,
                         self.device.GROUP_TRANSPORT_READY ? 4 - size(fabric.get_group_table()) : 0)
        var groups = response.add_array(1)
        if self.device.GROUP_TRANSPORT_READY
          for group : fabric.get_group_table()
            if group.find("endpoints", []).find(self.endpoint) == nil
              continue
            end
            var include = requested == nil || size(requested.val) == 0
            if !include
              for requested_id : requested.val
                if requested_id.val == group.find("group_id")
                  include = true
                  break
                end
              end
            end
            if include
              groups.add_TLV(nil, 0x06 #-TLV.U4-#, group.find("group_id"))
            end
          end
        end
        ctx.command = 0x0002             # GetGroupMembershipResponse
        return response

      elif command == 0x0003            # ---------- RemoveGroup ----------
        var group_id = val.findsubval(0)
        if group_id == nil
          ctx.status = 0x85 #-matter.INVALID_COMMAND-#
          return nil
        end
        var removed = group_id != 0 && fabric.remove_group_endpoint(group_id, self.endpoint)
        var response = TLV.Matter_TLV_struct()
        response.add_TLV(0, 0x06 #-TLV.U4-#,
                         group_id == 0 ? 0x87 #-matter.CONSTRAINT_ERROR-# :
                         removed ? 0 : 0x8B #-matter.NOT_FOUND-#)
        response.add_TLV(1, 0x06 #-TLV.U4-#, group_id)
        ctx.command = 0x0003             # RemoveGroupResponse
        return response

      elif command == 0x0004            # ---------- RemoveAllGroups ----------
        fabric.remove_all_groups_for_endpoint(self.endpoint)
        return true

      elif command == 0x0005            # ---------- AddGroupIfIdentifying ----------
        var group_id = val.findsubval(0)
        if group_id == nil
          ctx.status = 0x85 #-matter.INVALID_COMMAND-#
          return nil
        elif group_id == 0
          ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#
          return nil
        end
        var group_name = val.findsubval(1, "")
        if type(group_name) != 'string' || size(group_name) > 16
          ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#
          return nil
        end
        # IdentifyTime is always zero in the base device implementation, so
        # the mandatory command succeeds without changing group membership.
        # This remains valid while multicast group transport is disabled.
        return true
      end

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
    _stats_json_inner("shadow_shutter_direction", "ShutterDirection")
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
  # Return false and set a Matter failure when an MQTT command cannot be sent.
  def mqtt_command_ready(ctx)
    if self.mqtt_remote && !self.mqtt_remote.can_send()
      ctx.status = 0x01 #-matter.FAILURE-#
      return false
    end
    return true
  end

  #############################################################
  # register_cmd_cb
  #
  # Register recurrent command and callback
  # Defined as a separate method to allow override
  def register_cmd_cb()
    # MQTT bridge mode - register callbacks for state updates via subscriptions
    if self.mqtt_remote
      # Register parse_status as callback for all status codes
      # Status 0=Status, 2=StatusFWR, 5=StatusNET, 10=StatusSNS, 11=StatusSTS
      self.mqtt_remote.add_async_cb(/ status,payload,cmd -> self.parse_status(payload, status), nil)
      return
    end

    # HTTP bridge mode
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
      # MQTT bridge mode - no polling, state updates come via subscriptions
      if self.mqtt_remote
        self.tick = self.device.tick
        return
      end

      # HTTP bridge mode
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

      # MQTT bridge mode - return cached state
      if self.mqtt_remote
        var ret = self.mqtt_remote.call_sync(cmd, self.SYNC_TIMEOUT)
        return ret
      end

      # HTTP bridge mode
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
    if self.BRIDGE && self.http_remote
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
      # MQTT bridge mode - no polling needed
      if self.mqtt_remote
        return
      end

      # HTTP bridge mode
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

  #############################################################
  # For Zigbee devices
  #############################################################
  #############################################################
  # attributes_refined
  #
  # Filtered to only events for this endpoint
  #
  # Can be called only if `self.ZIGBEE` is true
  def zigbee_received(frame, attr_list)
    import math
    log(f"MTR: zigbee_received Ox{self.zigbee_mapper.shortaddr:04X} {attr_list=} {type(attr_list)=}", 3)
    var idx = 0
    while (idx < size(attr_list))
      var entry = attr_list[idx]
      if (entry.key == self.ZIGBEE_NAME)
        var val = self.pre_value(entry.val)
        var update_list = { self.JSON_NAME : val }   # Matter temperature is 1/100th of degrees
        self.update_virtual(update_list)
        log(f"MTR: [{self.endpoint:02X}] {self.JSON_NAME} updated {update_list}", 3)
        return nil
      end
      idx += 1
    end
  end

end
matter.Plugin_Device = Matter_Plugin_Device
