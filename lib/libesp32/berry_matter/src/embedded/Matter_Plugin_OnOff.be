#
# Matter_Plugin_OnOff.be - implements the behavior for a Relay (OnOff)
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
class Matter_Plugin end

#@ solidify:Matter_Plugin_OnOff,weak

class Matter_Plugin_OnOff : Matter_Plugin
  static var ENDPOINTS = [ 1 ]
  static var CLUSTERS  = {
    0x001D: [0,1,2,3],                # Descriptor Cluster 9.5 p.453
    0x0003: [0,1],                    # Identify 1.2 p.16
    0x0004: [0],                      # Groups 1.3 p.21
    0x0005: [0,1,2,3,4,5],            # Scenes 1.4 p.30 - no writable
    0x0006: [0,0xFFFC]                # On/Off 1.5 p.48
  }
  static var TYPES = [ 0x0100 ]       # On/Off Light

  var onoff                           # fake status for now # TODO

  #############################################################
  # Constructor
  def init(device)
    super(self).init(device)
    self.endpoints = self.ENDPOINTS
    self.clusters = self.CLUSTERS
    self.onoff = false                # fake status for now # TODO
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(msg, ctx)
    import string
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    if   cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========

      if   attribute == 0x0000          # ---------- DeviceTypeList / list[DeviceTypeStruct] ----------
        var dtl = TLV.Matter_TLV_array()
        for dt: self.TYPES
          var d1 = dtl.add_struct()
          d1.add_TLV(0, TLV.U2, dt)     # DeviceType
          d1.add_TLV(1, TLV.U2, 1)      # Revision
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
      end

    # ====================================================================================================
    elif cluster == 0x0003              # ========== Identify 1.2 p.16 ==========
      if   attribute == 0x0000          #  ---------- IdentifyTime / u2 ----------
        return TLV.create_TLV(TLV.U2, 0)      # no identification in progress
      elif attribute == 0x0001          #  ---------- IdentifyType / enum8 ----------
        return TLV.create_TLV(TLV.U1, 0)      # IdentifyType = 0x00 None
      end

    # ====================================================================================================
    elif cluster == 0x0006              # ========== On/Off 1.5 p.48 ==========
      if   attribute == 0x0000          #  ---------- OnOff / bool ----------
        return TLV.create_TLV(TLV.BOOL, self.onoff)
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0)    # 0 = no Level Control for Lighting
      end

    end
    # no match found, return that the attribute is unsupported  end
  end

  #############################################################
  # Invoke a command
  #
  # returns a TLV object if successful, contains the response
  #   or an `int` to indicate a status
  def invoke_request(msg, val, ctx)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var command = ctx.command
    var session = msg.session

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
    # ====================================================================================================
    elif cluster == 0x0006              # ========== On/Off 1.5 p.48 ==========
      if   command == 0x0000            # ---------- Off ----------
        self.onoff = false
        return true
      elif command == 0x0001            # ---------- On ----------
        self.onoff = true
        return true
      elif command == 0x0002            # ---------- Toggle ----------
        self.onoff = !self.onoff
        return true
      end
    end
  end
end
matter.Plugin_OnOff = Matter_Plugin_OnOff
  