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
    0x001D: [0,1,2,3,0xFFFC,0xFFFD],                # Descriptor Cluster 9.5 p.453
    0x0003: [0,1,0xFFFC,0xFFFD],                    # Identify 1.2 p.16
    0x0004: [0,0xFFFC,0xFFFD],                      # Groups 1.3 p.21
    0x0005: [0,1,2,3,4,5,0xFFFC,0xFFFD],            # Scenes 1.4 p.30 - no writable
    0x0006: [0,0xFFFC,0xFFFD],                      # On/Off 1.5 p.48
    0x0008: [0,15,17,0xFFFC,0xFFFD]                 # Level Control 1.6 p.57
  }
  static var TYPES = { 0x010A: 2 }       # On/Off Light

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
        return TLV.create_TLV(TLV.U4, 0)    # 0 = no Level Control for Lighting
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 1)    # 0 = no Level Control for Lighting
      end

    # ====================================================================================================
    elif cluster == 0x0003              # ========== Identify 1.2 p.16 ==========
      if   attribute == 0x0000          #  ---------- IdentifyTime / u2 ----------
        return TLV.create_TLV(TLV.U2, 0)      # no identification in progress
      elif attribute == 0x0001          #  ---------- IdentifyType / enum8 ----------
        return TLV.create_TLV(TLV.U1, 0)      # IdentifyType = 0x00 None
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0)    # 0 = no Level Control for Lighting
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 4)    # 0 = no Level Control for Lighting
      end

    # ====================================================================================================
    elif cluster == 0x0004              # ========== Groups 1.3 p.21 ==========
      if   attribute == 0x0000          #  ----------  ----------
        return nil                      # TODO
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0)    # 0 = no Level Control for Lighting
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 4)    # 0 = no Level Control for Lighting
      end

    # ====================================================================================================
    elif cluster == 0x0005              # ========== Scenes 1.4 p.30 - no writable ==========
      if   attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0)    # 0 = no Level Control for Lighting
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 4)    # 0 = no Level Control for Lighting
      end

    # ====================================================================================================
    elif cluster == 0x0006              # ========== On/Off 1.5 p.48 ==========
      if   attribute == 0x0000          #  ---------- OnOff / bool ----------
        return TLV.create_TLV(TLV.BOOL, self.onoff)
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0)    # 0 = no Level Control for Lighting
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 4)    # 0 = no Level Control for Lighting
      end

    # ====================================================================================================
    elif cluster == 0x0008              # ========== Level Control 1.6 p.57 ==========
      if   attribute == 0x0000          #  ---------- CurrentLevel / u1 ----------
        return TLV.create_TLV(TLV.U1, 0x88)
      elif attribute == 0x000F          #  ---------- Options / map8 ----------
        return TLV.create_TLV(TLV.U1, 0)    # 0 = no Level Control for Lighting
      elif attribute == 0x0010          #  ---------- OnLevel / u1 ----------
        return TLV.create_TLV(TLV.U1, 1)    # 0 = no Level Control for Lighting
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0)    # 0 = no Level Control for Lighting
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 4)    # 0 = no Level Control for Lighting
      end
      
    end
    # no match found, return that the attribute is unsupported  end
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
    # ====================================================================================================
    elif cluster == 0x0006              # ========== On/Off 1.5 p.48 ==========
      if   command == 0x0000            # ---------- Off ----------
        if self.onoff   self.onoff_changed(ctx) end
        self.onoff = false
        return true
      elif command == 0x0001            # ---------- On ----------
        if !self.onoff   self.onoff_changed(ctx) end
        self.onoff = true
        return true
      elif command == 0x0002            # ---------- Toggle ----------
        self.onoff_changed(ctx)
        self.onoff = !self.onoff
        return true
      end
    # ====================================================================================================
    elif cluster == 0x0008              # ========== Level Control 1.6 p.57 ==========
      if   command == 0x0000            # ---------- MoveToLevel ----------
        return true
      elif command == 0x0001            # ---------- Move ----------
        return true
      elif command == 0x0002            # ---------- Step ----------
        return true
      elif command == 0x0003            # ---------- Stop ----------
        return true
      elif command == 0x0004            # ---------- MoveToLevelWithOnOff ----------
        return true
      elif command == 0x0005            # ---------- MoveWithOnOff ----------
        return true
      elif command == 0x0006            # ---------- StepWithOnOff ----------
        return true
      elif command == 0x0007            # ---------- StopWithOnOff ----------
        return true
      end
    end
  end

  #############################################################
  # Signal that onoff attribute changed
  def onoff_changed(ctx)
    self.attribute_updated(ctx.endpoint, 0x0006, 0x0000)
  end

end
matter.Plugin_OnOff = Matter_Plugin_OnOff
  