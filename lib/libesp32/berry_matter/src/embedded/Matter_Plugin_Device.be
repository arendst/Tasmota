#
# Matter_Plugin_Device.be - implements the behavior for a standard Device
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

# dummy declaration for solidification
class Matter_Plugin end

#@ solidify:Matter_Plugin_Device,weak

class Matter_Plugin_Device : Matter_Plugin
  static var CLUSTERS  = {
    0x001D: [0,1,2,3,0xFFFC,0xFFFD],                # Descriptor Cluster 9.5 p.453
    0x0003: [0,1,0xFFFC,0xFFFD],                    # Identify 1.2 p.16
    0x0004: [0,0xFFFC,0xFFFD],                      # Groups 1.3 p.21
  }
  static var TYPES = { 0x0000: 0 }                  # fake type

  #############################################################
  # Constructor
  def init(device, endpoint, tasmota_relay_index)
    super(self).init(device, endpoint)
    self.clusters = self.CLUSTERS
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx)
    import string
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
        cl.add_TLV(nil, TLV.U2, 0x0006)
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
    elif cluster == 0x0003              # ========== Identify 1.2 p.16 ==========
      if   attribute == 0x0000          #  ---------- IdentifyTime / u2 ----------
        return TLV.create_TLV(TLV.U2, 0)      # no identification in progress
      elif attribute == 0x0001          #  ---------- IdentifyType / enum8 ----------
        return TLV.create_TLV(TLV.U1, 0)      # IdentifyType = 0x00 None
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0)    # no features
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 4)    # "new data model format and notation"
      end

    # ====================================================================================================
    elif cluster == 0x0004              # ========== Groups 1.3 p.21 ==========
      if   attribute == 0x0000          #  ----------  ----------
        return nil                      # TODO
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0)#
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 4)# "new data model format and notation"
      end

    else
      return super(self).read_attribute(session, ctx)
    end
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

    else
      return super(self).invoke_request(session, val, ctx)
    end
  end

end
matter.Plugin_Device = Matter_Plugin_Device
