#
# Matter_Plugin.be - generic superclass for all Matter plugins, used to define specific behaviors (light, switch, media...)
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
  static var NAME = ""                      # display name of the plug-in
  static var ARG  = ""                      # additional argument name (or empty if none)
  static var ARG_TYPE = / x -> str(x)       # function to convert argument to the right type
  static var ARG_HINT = "_Not used_"          # Hint for entering the Argument (inside 'placeholder')
  # Behavior of the plugin, frequency at which `update_shadow()` is called
  static var UPDATE_TIME = 5000             # default is every 5 seconds
  var update_next                           # next timestamp for update
  # Configuration of the plugin: clusters and type
  static var CLUSTERS = {
    0x001D: [0,1,2,3,0xFFFC,0xFFFD],                # Descriptor Cluster 9.5 p.453
    0x0039: [0x11],                                 # Bridged Device Basic Information 9.13 p.485
  }
  var device                                # reference to the `device` global object
  var endpoint                              # current endpoint
  var clusters                              # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  var tick                                  # tick value when it was last updated

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
    self.clusters = self.consolidate_clusters()
    self.parse_configuration(config)
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
    return true
  end

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This method should collect the data from the local or remote device
  # and call `parse_update(<data>)` when data is available.
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
  # consolidate_clusters
  #
  # Build a consolidated map of all the `CLUSTERS` static vars
  # from the inheritance hierarchy
  def consolidate_clusters()
    def real_super(o) return super(o) end   # enclose `super()` in a static function to disable special behavior for super in instances
    var ret = {}
    var o = self        # start with self
    while o != nil      # when we rich highest class, `super()` returns `nil`
      var CL = o.CLUSTERS
      for k: CL.keys()
        # check if key already exists
        if !ret.contains(k)   ret[k] = []   end
        for attr: CL[k]       # iterate on values
          if ret[k].find(attr) == nil
            ret[k].push(attr)
          end
        end
      end

      o = real_super(o)
    end
    return ret
  end

  #############################################################
  # Which endpoints does it handle (list of numbers)
  def get_endpoint()
    return self.endpoint
  end
  def get_cluster_list(ep)
    var ret = []
    for k: self.clusters.keys()
      ret.push(k)
    end
    return ret
  end
  def get_attribute_list(ep, cluster)
    return self.clusters.find(cluster, [])
  end

  #############################################################
  # Does it handle this endpoint and this cluster
  def has(cluster, endpoint)
    return self.clusters.contains(cluster) && self.endpoints.find(endpoint) != nil
  end

  #############################################################
  # MVC Model
  #
  # View reading attributes
  #############################################################
  #############################################################
  # read attribute
  def read_attribute(session, ctx)
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
        for cl: self.get_cluster_list()
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
        return TLV.create_TLV(TLV.U4, 0)    #
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 1)    # "Initial Release"
      end

    # ====================================================================================================
    elif cluster == 0x0039              # ========== Bridged Device Basic Information 9.13 p.485 ==========

      if   attribute == 0x0011          #  ---------- Reachable / bool ----------
        return TLV.create_TLV(TLV.BOOL, 1)     # by default we are reachable
      else
        return super(self).read_attribute(session, ctx)   # rest is handled by 0x0028
      end
    else
      return nil
    end
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

end

matter.Plugin = Matter_Plugin
