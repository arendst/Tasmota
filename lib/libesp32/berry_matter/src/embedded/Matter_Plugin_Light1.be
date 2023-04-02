#
# Matter_Plugin_Light1.be - implements the behavior for a Light with 1 channel (Dimmer)
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

#@ solidify:Matter_Plugin_Light1,weak

class Matter_Plugin_Light1 : Matter_Plugin
  static var CLUSTERS  = {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    0x0003: [0,1,0xFFFC,0xFFFD],                    # Identify 1.2 p.16
    0x0004: [0,0xFFFC,0xFFFD],                      # Groups 1.3 p.21
    0x0005: [0,1,2,3,4,5,0xFFFC,0xFFFD],            # Scenes 1.4 p.30 - no writable
    0x0006: [0,0xFFFC,0xFFFD],                      # On/Off 1.5 p.48
    0x0008: [0,2,3,0x0F,0x11,0xFFFC,0xFFFD],                # Level Control 1.6 p.57
  }
  static var TYPES = { 0x0101: 2 }                  # Dimmable Light

  var shadow_bri
  var shadow_onoff

  #############################################################
  # Constructor
  def init(device, endpoint)
    super(self).init(device, endpoint)
    self.shadow_bri = 0
    self.shadow_onoff = false
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    import light
    var light_status = light.get()
    var bri = light_status.find('bri', nil)
    var pow = light_status.find('power', nil)
    if bri != nil     bri = tasmota.scale_uint(bri, 0, 255, 0, 254)   else bri = self.shadow_bri      end
    if pow != self.shadow_onoff self.attribute_updated(nil, 0x0006, 0x0000)   self.shadow_onoff = pow end
    if bri != self.shadow_bri   self.attribute_updated(nil, 0x0008, 0x0000)   self.shadow_bri = bri   end
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
        return TLV.create_TLV(TLV.BOOL, self.shadow_onoff)
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0)    # 0 = no Level Control for Lighting
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 4)    # 0 = no Level Control for Lighting
      end

    # ====================================================================================================
    elif cluster == 0x0008              # ========== Level Control 1.6 p.57 ==========
      if   attribute == 0x0000          #  ---------- CurrentLevel / u1 ----------
        return TLV.create_TLV(TLV.U1, self.shadow_bri)
      elif attribute == 0x0002          #  ---------- MinLevel / u1 ----------
        return TLV.create_TLV(TLV.U1, 0)
      elif attribute == 0x0003          #  ---------- MaxLevel / u1 ----------
        return TLV.create_TLV(TLV.U1, 254)
      elif attribute == 0x000F          #  ---------- Options / map8 ----------
        return TLV.create_TLV(TLV.U1, 0)    #
      elif attribute == 0x0011          #  ---------- OnLevel / u1 ----------
        return TLV.create_TLV(TLV.U1, self.shadow_bri)
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0X01)    # OnOff
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 5)    # "new data model format and notation"
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
    import light
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
        light.set({'power':false})
        self.update_shadow()
        return true
      elif command == 0x0001            # ---------- On ----------
        light.set({'power':true})
        self.update_shadow()
        return true
      elif command == 0x0002            # ---------- Toggle ----------
        light.set({'power':!self.shadow_onoff})
        self.update_shadow()
        return true
      end
    # ====================================================================================================
    elif cluster == 0x0008              # ========== Level Control 1.6 p.57 ==========
      if   command == 0x0000            # ---------- MoveToLevel ----------
        var bri_in = val.findsubval(0)  # Hue 0..254
        var bri = tasmota.scale_uint(bri_in, 0, 254, 0, 255)
        light.set({'bri': bri})
        self.update_shadow()
        ctx.log = "bri:"+str(bri_in)
        return true
      elif command == 0x0001            # ---------- Move ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0002            # ---------- Step ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0003            # ---------- Stop ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0004            # ---------- MoveToLevelWithOnOff ----------
        var bri_in = val.findsubval(0)  # Hue 0..254
        var bri = tasmota.scale_uint(bri_in, 0, 254, 0, 255)
        var onoff = bri > 0
        light.set({'bri': bri, 'power': onoff})
        self.update_shadow()
        ctx.log = "bri:"+str(bri_in)
        return true
      elif command == 0x0005            # ---------- MoveWithOnOff ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0006            # ---------- StepWithOnOff ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0007            # ---------- StopWithOnOff ----------
        # TODO, we don't really support it
        return true
      end
    end
  end

  #############################################################
  # every_second
  def every_second()
    self.update_shadow()                    # force reading value and sending subscriptions
  end
end
matter.Plugin_Light1 = Matter_Plugin_Light1
