#
# Matter_Plugin_Light2.be - implements the behavior for a Light with 2 channel (CT)
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
class Matter_Plugin_Light1 end

#@ solidify:Matter_Plugin_Light2,weak

class Matter_Plugin_Light2 : Matter_Plugin_Light1
  static var CLUSTERS  = {
    # 0x001D: inherited                                     # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                                     # Identify 1.2 p.16
    # 0x0004: inherited                                     # Groups 1.3 p.21
    # 0x0005: inherited                                     # Scenes 1.4 p.30 - no writable
    # 0x0006: inherited                                     # On/Off 1.5 p.48
    # 0x0008: inherited                                     # Level Control 1.6 p.57
    0x0300: [7,8,0xF,0x400B,0x400C,0xFFFC,0xFFFD],          # Color Control 3.2 p.111
  }
  static var TYPES = { 0x010C: 2 }                  # Color Temperature Light

  var shadow_ct
  var ct_min, ct_max

  #############################################################
  # Constructor
  def init(device, endpoint)
    super(self).init(device, endpoint)
    self.shadow_ct = 325
    self.update_ct_minmax()
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    import light
    self.update_ct_minmax()
    super(self).update_shadow()
    var light_status = light.get()
    var ct = light_status.find('ct', nil)
    if ct  == nil     ct = self.shadow_ct      end
    if ct  != self.shadow_ct    self.attribute_updated(nil, 0x0300, 0x0007)   self.shadow_ct = ct   end
  end

  #############################################################
  # Update ct_min/max
  #
  def update_ct_minmax()
    var ct_alexa_mode = tasmota.get_option(82)      # if set, range is 200..380 instead of 153...500
    self.ct_min = ct_alexa_mode ? 200 : 153
    self.ct_max = ct_alexa_mode ? 380 : 500
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
    if   cluster == 0x0300              # ========== Color Control 3.2 p.111 ==========
      if   attribute == 0x0007          #  ---------- ColorTemperatureMireds / u2 ----------
        return TLV.create_TLV(TLV.U1, self.shadow_ct)
      elif attribute == 0x0008          #  ---------- ColorMode / u1 ----------
        return TLV.create_TLV(TLV.U1, 2)# 2 = ColorTemperatureMireds
      elif attribute == 0x000F          #  ---------- Options / u1 ----------
        return TLV.create_TLV(TLV.U1, 0)
      elif attribute == 0x400B          #  ---------- ColorTempPhysicalMinMireds / u2 ----------
        return TLV.create_TLV(TLV.U1, self.ct_min)
      elif attribute == 0x400C          #  ---------- ColorTempPhysicalMaxMireds / u2 ----------
        return TLV.create_TLV(TLV.U1, self.ct_max)
      
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0x10)    # CT
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 5)    # "new data model format and notation, FeatureMap support"
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
    if   cluster == 0x0300              # ========== Color Control 3.2 p.111 ==========
      if   command == 0x000A            # ---------- MoveToColorTemperature ----------
        var ct_in = val.findsubval(0)  # CT
        if ct_in < self.ct_min  ct_in = self.ct_min   end
        if ct_in > self.ct_max  ct_in = self.ct_max   end
        light.set({'ct': ct_in})
        self.update_shadow()
        ctx.log = "ct:"+str(ct_in)
        return true
      elif command == 0x0047            # ---------- StopMoveStep ----------
        # TODO, we don't really support it
        return true
      elif command == 0x004B            # ---------- MoveColorTemperature ----------
        # TODO, we don't really support it
        return true
      elif command == 0x004C            # ---------- StepColorTemperature ----------
        # TODO, we don't really support it
        return true
      end

    else
      return super(self).invoke_request(session, val, ctx)
    end

  end

end
matter.Plugin_Light2 = Matter_Plugin_Light2
