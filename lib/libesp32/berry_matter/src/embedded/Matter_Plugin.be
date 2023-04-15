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

# Matter modules for extensibility
# template but not actually used

#@ solidify:Matter_Plugin,weak

class Matter_Plugin
  static var CLUSTERS = {
    0x001D: [0,1,2,3,0xFFFC,0xFFFD],                # Descriptor Cluster 9.5 p.453
  }
  var device                                # reference to the `device` global object
  var endpoint                              # current endpoint
  var clusters                              # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy

  #############################################################
  # MVC Model
  #
  # Model linking the plugin to the Tasmota behavior
  #############################################################

  #############################################################
  # Constructor
  #
  def init(device, endpoint)
    self.device = device
    self.endpoint = endpoint
    self.clusters = self.consolidate_clusters()
  end

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  def update_shadow()
  end

  #############################################################
  # signal that an attribute has been changed
  #
  # If `endpoint` is `nil`, send to all endpoints
  def attribute_updated(endpoint, cluster, attribute, fabric_specific)
    if endpoint == nil    endpoint = self.endpoint  end
    self.device.attribute_updated(endpoint, cluster, attribute, fabric_specific)
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
        for dt: self.TYPES.keys()
          var d1 = dtl.add_struct()
          d1.add_TLV(0, TLV.U2, dt)     # DeviceType
          d1.add_TLV(1, TLV.U2, self.TYPES[dt])      # Revision
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
  # every_second
  def every_second()
    self.update_shadow()                    # force reading value and sending subscriptions
  end
end

matter.Plugin = Matter_Plugin
