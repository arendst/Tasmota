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
  static var CLUSTERS  = {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    0x0003: [0,1,0xFFFC,0xFFFD],                    # Identify 1.2 p.16
    0x0004: [0,0xFFFC,0xFFFD],                      # Groups 1.3 p.21
    0x0005: [0,1,2,3,4,5,0xFFFC,0xFFFD],            # Scenes 1.4 p.30 - no writable
    0x0006: [0,0xFFFC,0xFFFD],                      # On/Off 1.5 p.48
    # 0x0008: [0,15,17,0xFFFC,0xFFFD]                 # Level Control 1.6 p.57
  }
  static var TYPES = { 0x010A: 2 }       # On/Off Light

  var tasmota_relay_index             # Relay number in Tasmota (zero based)
  var shadow_onoff                           # fake status for now # TODO

  #############################################################
  # Constructor
  def init(device, endpoint, tasmota_relay_index)
    super(self).init(device, endpoint)
    self.get_onoff()                        # read actual value
    if tasmota_relay_index == nil     tasmota_relay_index = 0   end
    self.tasmota_relay_index = tasmota_relay_index
  end

  #############################################################
  # Model
  #
  def set_onoff(v)
    tasmota.set_power(self.tasmota_relay_index, bool(v))
    self.get_onoff()
  end
  #############################################################
  # get_onoff
  #
  # Update shadow and signal any change
  def get_onoff()
    var state = tasmota.get_power(self.tasmota_relay_index)
    if state != nil
      if self.shadow_onoff != nil && self.shadow_onoff != bool(state)
        self.onoff_changed()      # signal any change
      end
      self.shadow_onoff = state
    end
    if self.shadow_onoff == nil   self.shadow_onoff = false   end     # avoid any `nil` value when initializing
    return self.shadow_onoff
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx)
    import string
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0003              # ========== Identify 1.2 p.16 ==========
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
        return TLV.create_TLV(TLV.BOOL, self.get_onoff())
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
    # ====================================================================================================
    elif cluster == 0x0005              # ========== Scenes 1.4 p.30 ==========
      # TODO
      return true
    # ====================================================================================================
    elif cluster == 0x0006              # ========== On/Off 1.5 p.48 ==========
      if   command == 0x0000            # ---------- Off ----------
        self.set_onoff(false)
        return true
      elif command == 0x0001            # ---------- On ----------
        self.set_onoff(true)
        return true
      elif command == 0x0002            # ---------- Toggle ----------
        self.set_onoff(!self.get_onoff())
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
  def onoff_changed()
    self.attribute_updated(nil, 0x0006, 0x0000)   # send to all endpoints
  end

  #############################################################
  # every_second
  def every_second()
    self.get_onoff()                    # force reading value and sending subscriptions
  end
end
matter.Plugin_OnOff = Matter_Plugin_OnOff
