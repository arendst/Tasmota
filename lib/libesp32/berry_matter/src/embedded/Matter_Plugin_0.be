#
# Matter_Plugin_0.be - generic superclass for all Matter plugins, used to define specific behaviors (light, switch, media...)
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

# Matter modules for extensibility
# template but not actually used

#@ solidify:Matter_Plugin,weak

class Matter_Plugin
  # Global type system for plugins
  static var TYPE = ""                      # name of the plug-in in json
  static var DISPLAY_NAME = ""                      # display name of the plug-in
  static var ARG  = ""                      # additional argument name (or empty if none)
  static var ARG_TYPE = / x -> str(x)       # function to convert argument to the right type
  static var ARG_HINT = "_Not used_"          # Hint for entering the Argument (inside 'placeholder')
  # Behavior of the plugin, frequency at which `update_shadow()` is called
  static var UPDATE_TIME = 5000             # default is every 5 seconds
  static var VIRTUAL = false                # set to true only for virtual devices
  static var BRIDGE = false                 # set to true only for bridged devices (ESP8266 or OpenBK)
  var update_next                           # next timestamp for update
  # Configuration of the plugin: clusters and type
  static var CLUSTERS = matter.consolidate_clusters(_class, {
    0x001D: [0,1,2,3],                      # Descriptor Cluster 9.5 p.453
  })
  static var COMMANDS = {
    0x001D: nil,                                    # Descriptor Cluster 9.5 p.453
  }
  # static var TYPES = { <device_type>: <revision> }    # needs to be defined for each endpoint
  # `FEATURE_MAPS` contains any non-zero value per cluster, if not present default to `0`
  static var FEATURE_MAPS = {               # feature map per cluster
    0x0031: 0x04,                           # Put Eth for now which should work for any on-network
    0x0102: 1 + 4,                          # Lift + PA_LF
    0x0202: 2,                              # Fan: Auto
  }
  # `CLUSTER_REVISIONS` contains revision numbers for each cluster, or `1` if not present
  static var CLUSTER_REVISIONS = {
    0x0003: 4,                              # New data model format and notation
    0x0004: 4,                              # New data model format and notation
    0x0005: 5,                              # "new data model format and notation"
    0x0006: 5,                              # Addition of Dead Front behavior and associated FeatureMap entry
    0x0008: 5,                              # "new data model format and notation"
    0x001D: 2,                              # Semantic tag list; TagList feature
    # 0x001F: 1,                            # Initial Release
    0x0028: 2,                              # Added ProductAppearance attribute
    # 0x002A: 1,                            # Initial Release
    # 0x002B: 1,                            # Initial Release
    # 0x002C: 1,                            # Initial Release
    # 0x0030: 1,                            # Initial Release
    # 0x0031: 1,                            # Initial Release
    # 0x0032: 1,                            # Initial Release
    # 0x0033: 1,                            # Initial Release
    # 0x0034: 1,                            # Initial Release
    0x0038: 2,                              #
    # 0x003B: 1,                            # Initial Release
    # 0x003C: 1,                            # Initial Release
    # 0x003E: 1,                            # Initial Release
    0x003F: 2,                              # Clarify KeySetWrite validation and behavior on invalid epoch key lengths
    # 0x0040: 1,                            # Initial Release
    # 0x0041: 1,                            # Initial Release
    # 0x0042: 1,                            # Initial Release
    # 0x005B: 1,                            # Initial Release
    # 0x005C: 1,                            # Initial Release
    0x0101: 7,                              # Added support for European door locks (unbolt feature)
    0x0102: 5,                              # New data model format and notation
    0x0200: 4,                              # Added feature map
    0x0201: 6,                              # Introduced the LTNE feature and adapted text (spec issue #5778)
    0x0202: 4,                              # Change conformance for FanModeSe­ quenceEnum
    0x0204: 2,                              # New data model format and notation, added "Conversion of Temperature Values for Display" section
    0x0300: 6,                              # Added clarifications to Scenes support for Matter
    0x0301: 4,                              # New data model format and notation
    0x0400: 3,                              # New data model format and notation
    0x0402: 4,                              # New data model format and notation
    0x0403: 3,                              # New data model format and notation
    0x0404: 3,                              # New data model format and notation
    0x0405: 3,                              # New data model format and notation
    0x0406: 3,                              # New data model format and notation
    0x0407: 3,                              # New data model format and notation
    0x0408: 3,                              # New data model format and notation
  }
  # Accepted Update commands for virtual devices
  static var UPDATE_COMMANDS = []
  var device                                # reference to the `device` global object
  var endpoint                              # current endpoint
  var tick                                  # tick value when it was last updated
  var node_label                            # name of the endpoint, used only in bridge mode, "" if none

  #############################################################
  # MVC Model
  #
  # Model linking the plugin to the Tasmota behavior
  #############################################################

  #############################################################
  # Constructor
  #
  # device: contains the root device object so the plugin can "call home"
  # endpoint: (int) the endpoint number (16 bits)
  # arguments: (map) the map for all complementary arguments that are plugin specific
  def init(device, endpoint, config)
    self.device = device
    self.endpoint = endpoint
    self.parse_configuration(config)
    self.node_label = config.find("name", "")
  end

  # proxy for the same method in IM
  def ack_request(ctx)
    var msg = ctx.msg
    if msg != nil
      self.device.message_handler.im.send_ack_now(msg)
    end
    ctx.msg = nil
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  # TO BE OVERRIDEN
  def parse_configuration(config)
  end

  #############################################################
  # is_local_device
  #
  # Returns true if it's a local device, or false for a
  # remotely device controlled via HTTP
  def is_local_device()
    return !(self.BRIDGE)
  end

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This method should collect the data from the local or remote device
  # and call `parse_status(<data>)` when data is available.
  #
  # TO BE OVERRIDDEN
  # This call is synnchronous and blocking.
  def update_shadow()
    self.tick = self.device.tick
  end

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This call is synnchronous and blocking.
  def update_shadow_lazy()
    if self.tick != self.device.tick
      self.update_shadow()
      self.tick = self.device.tick
    end
  end

  #############################################################
  # signal that an attribute has been changed
  #
  # If `endpoint` is `nil`, send to all endpoints
  def attribute_updated(cluster, attribute, fabric_specific)
    self.device.attribute_updated(self.endpoint, cluster, attribute, fabric_specific)
  end

  #############################################################
  # generate a new event
  #
  def publish_event(cluster, event_id, priority, data0, data1, data2)
    self.device.events.publish_event(self.endpoint, cluster, event_id, true #-urgent-#, priority, data0, data1, data2)
  end
  # def publish_event_non_urgent(cluster, event, priority, data0, data1, data2)
  #   self.device.events.publish_event(self.endpoint, cluster, event, false #-non_urgent-#, priority, data0, data1, data2)
  # end
#- testing

var root = matter_device.plugins[0]
var tlv_solo = matter.TLV.Matter_TLV_item()
tlv_solo.set(matter.TLV.U4, 42)
root.publish_event(0x001D, 0, matter.EVENT_CRITICAL, tlv_solo)
matter_device.events.dump()

-#

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

  #############################################################
  # consolidate_clusters
  #
  # Build a consolidated map of all the `CLUSTERS` static vars
  # from the inheritance hierarchy
  def get_clusters()
    return self.CLUSTERS
    # def real_super(o) return super(o) end   # enclose `super()` in a static function to disable special behavior for super in instances
    # var ret = {}
    # var o = self        # start with self
    # while o != nil      # when we rich highest class, `super()` returns `nil`
    #   var CL = o.CLUSTERS
    #   for k: CL.keys()
    #     # check if key already exists
    #     if !ret.contains(k)   ret[k] = []   end
    #     for attr: CL[k]       # iterate on values
    #       if ret[k].find(attr) == nil
    #         ret[k].push(attr)
    #       end
    #     end
    #   end

    #   o = real_super(o)
    # end
    # return ret
  end

  #############################################################
  # consolidate_update_commands
  #
  # Return consolidated "update commands" for this class
  def consolidate_update_commands()
    return self.UPDATE_COMMANDS
  end

  #############################################################
  # Publish to MQTT a command received from controller
  #
  # we limit to 3 commands (to we need more?)
  def publish_command(key1, value1, key2, value2, key3, value3)
    import json
    var payload = f"{json.dump(key1)}:{json.dump(value1)}"
    if key2 != nil
      payload = f"{payload},{json.dump(key2)}:{json.dump(value2)}"
    end
    if key3 != nil
      payload = f"{payload},{json.dump(key3)}:{json.dump(value3)}"
    end
    matter.publish_command('MtrReceived', self.endpoint, self.node_label, payload)
  end

  #############################################################
  # Which endpoints does it handle (list of numbers)
  def get_endpoint()
    return self.endpoint
  end
  def get_cluster_list_sorted()
    return self.device.k2l(self.CLUSTERS)
  end
  def contains_cluster(cluster)
    return self.CLUSTERS.contains(cluster)
  end
  # def get_attribute_list(cluster)
  #   return self.clusters.find(cluster, [])
  # end
  # returns as a constant bytes of 16-bit ints, big endian
  def get_attribute_list_bytes(cluster)
    return self.CLUSTERS.find(cluster, nil)
  end
  def contains_attribute(cluster, attribute)
    var attr_list = self.CLUSTERS.find(cluster)
    # log(f"MTR: contains_attribute {cluster=} {attribute=} {attr_list=}")
    if attr_list != nil
      var idx = 0
      var attr_sz = size(attr_list) / 2     # group of 16-bit integers, big endian
      while idx < attr_sz
        if attr_list.get(idx * 2, -2) == attribute
          return true
        end
        idx += 1
      end
    end
    return false
  end

  def set_name(n)
    if n != self.node_label
      self.attribute_updated(0x0039, 0x0005)
    end
    self.node_label = n
  end
  def get_name()    return self.node_label  end

  #############################################################
  # MVC Model
  #
  # View reading attributes
  #############################################################
  #############################################################
  # read attribute
  #
  # Arg:
  #   `tlv_solo` contains an instance of `Matter_TLV_item` to avoid allocating a new object with TLV.create_TLV
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    if   cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========

      if   attribute == 0x0000          # ---------- DeviceTypeList / list[DeviceTypeStruct] ----------
        var dtl = TLV.Matter_TLV_array()
        var types = self.TYPES
        for dt: types.keys()
          var d1 = dtl.add_struct()
          d1.add_TLV(0, TLV.U2, dt)     # DeviceType
          d1.add_TLV(1, TLV.U2, types[dt])      # Revision
        end
        return dtl
      elif attribute == 0x0001          # ---------- ServerList / list[cluster-id] ----------
        var sl = TLV.Matter_TLV_array()
        for cl: self.get_cluster_list_sorted()
          sl.add_TLV(nil, TLV.U4, cl)
        end
        return sl
      elif attribute == 0x0002          # ---------- ClientList / list[cluster-id] ----------
        var cl = TLV.Matter_TLV_array()
        return cl
      elif attribute == 0x0003          # ---------- PartsList / list[endpoint-no]----------
        var pl = TLV.Matter_TLV_array()
        return pl
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0)    #
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return tlv_solo.set(TLV.U4, 1)    # "Initial Release"
      end

    end

    # Handle attributes 0xFFF8 - 0xFFFD for all clusters
    if   attribute == 0xFFF8            # GeneratedCommandList
      var gcl = TLV.Matter_TLV_array()
      return gcl                        # return empty list
    elif attribute == 0xFFFB            # AttributeList
      var acli = TLV.Matter_TLV_array()
      var attr_list_bytes = self.get_attribute_list_bytes(cluster)
      var attr_list_bytes_sz = (attr_list_bytes != nil) ? size(attr_list_bytes) : 0
      var idx = 0
      while idx < attr_list_bytes_sz
        acli.add_TLV(nil, TLV.U2, attr_list_bytes.get(idx * 2, -2))
        idx += 1
      end
      return acli                       # TODO, empty list for now
    elif attribute == 0xFFFA            # EventList
      var el = TLV.Matter_TLV_array()
      return el                         # return empty list
    elif attribute == 0xFFF9            # AcceptedCommandList
      var al = TLV.Matter_TLV_array()
      return al                         # TODO
    elif attribute == 0xFFFC            # FeatureMap
      var featuremap = self.FEATURE_MAPS.find(cluster, 0)
      return tlv_solo.set(TLV.U4, featuremap)
    elif attribute == 0xFFFD            # ClusterRevision
      var clusterrevision = self.CLUSTER_REVISIONS.find(cluster, 1)
      return tlv_solo.set(TLV.U4, clusterrevision)
    end

    # no handler found, return nil
    return nil
  end

  #############################################################
  # read event
  # TODO
  def read_event(session, endpoint, cluster, eventid)
    return nil
  end

  #############################################################
  # subscribe attribute
  # TODO
  def subscribe_attribute(session, endpoint, cluster, attribute)
    return nil
  end

  #############################################################
  # subscribe event
  # TODO
  def subscribe_event(session, endpoint, cluster, eventid)
    return nil
  end

  #############################################################
  # MVC Model
  #
  # Controller write attributes
  #############################################################
  #############################################################
  # write attribute
  def write_attribute(session, ctx, write_data)
    return nil
  end

  #############################################################
  # MVC Model
  #
  # Controller invoke request
  #############################################################
  #############################################################
  # invoke command
  def invoke_request(session, val, ctx)
    return nil
  end

  #############################################################
  # timed request
  # TODO - should we even support this?
  def timed_request(session, val, ctx)
    return nil
  end

  #############################################################
  # parse sensor
  #
  # The device calls regularly `tasmota.read_sensors()` and converts
  # it to json.
  def parse_sensors(payload)
  end

  #############################################################
  # every_250ms
  #
  # check if the timer expired and update_shadow() needs to be called
  def every_250ms()
    if self.update_next == nil
      self.update_next = matter.jitter(self.UPDATE_TIME)
    else
      if tasmota.time_reached(self.update_next)
        if self.tick != self.device.tick
          self.update_shadow()                                # call update_shadow if not already called
        end
        self.update_next = tasmota.millis(self.UPDATE_TIME)   # rearm timer
      end
    end
  end

  #############################################################
  # UI Methods
  #############################################################
  # ui_conf_to_string
  #
  # Convert the current plugin parameters to a single string
  static def ui_conf_to_string(cl, conf)
    var arg_name = cl.ARG
    var arg = arg_name ? str(conf.find(arg_name, '')) : ''
    # print("MTR: ui_conf_to_string", conf, cl, arg_name, arg)
    return arg
  end

  #############################################################
  # ui_string_to_conf
  #
  # Convert the string in UI to actual parameters added to the map
  static def ui_string_to_conf(cl, conf, arg)
    var arg_name = cl.ARG
    var arg_type = cl.ARG_TYPE
    if arg && arg_name
      conf[arg_name] = arg_type(arg)
    end
    # print("ui_string_to_conf", conf, arg)
    return conf
  end

  #############################################################
  # append_state_json
  #
  # Output the current state in JSON
  # Takes the JSON string prefix
  # New values need to be appended with `,"key":value` (including prefix comma)
  def append_state_json()
    return ""
  end

  # This is to be called by matter_device to get the full state JSON
  # including "Ep":<ep>,"Name"="<friendly_name"
  def state_json()
    import json
    var ep_name = self.node_label ? f',"Name":{json.dump(self.node_label)}' : ""
    var state = self.append_state_json()
    if state
      var ret = f'{{"Ep":{self.endpoint:i}{ep_name}{state}}}'
      return ret
    else
      return nil
    end
  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  # The map is pre-cleaned and contains only keys declared in
  # `self.UPDATE_COMMANDS` with the adequate case
  # (no need to handle case-insensitive)
  def update_virtual(payload)
    # pass
  end

  #######################################################################
  # _parse_update_virtual: parse a single value out of MtrUpdate JSON
  #
  # Used internally by `update_virtual`
  #
  # Args
  #   payload: the native payload (converted from JSON) from MtrUpdate
  #   key: key name in the JSON payload to read from, do nothing if key does not exist or content is `null`
  #   old_val: previous value, used to detect a change or return the value unchanged
  #   type_func: type enforcer for value, typically `int`, `bool`, `str`, `number`, `real`
  #   cluster/attribute: in case the value has change, publish a change to cluster/attribute
  #
  # Returns:
  #   `old_val` if key does not exist, JSON value is `null`, or value is unchanged
  #   or new value from JSON (which is the new shadow value)
  #
  def _parse_update_virtual(payload, key, old_val, type_func, cluster, attribute)
    var val = payload.find(key)
    if (val != nil)
      val = type_func(val)
      if (val != old_val)
        self.attribute_updated(cluster, attribute)
      end
      return val
    end
    return old_val
  end
  
end

matter.Plugin = Matter_Plugin
