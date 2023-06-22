#
# Matter_Plugin_Light0.be - implements the behavior for a generic Lighting (OnOff only)
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

# Matter plug-in for core behavior

# dummy declaration for solidification
class Matter_Plugin_Device end

#@ solidify:Matter_Plugin_Light0,weak

class Matter_Plugin_Light0 : Matter_Plugin_Device
  static var TYPE = "light0"                        # name of the plug-in in json
  static var NAME = "Light 0 On"                    # display name of the plug-in
  static var UPDATE_TIME = 250                      # update every 250ms
  static var CLUSTERS  = {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    0x0006: [0,0xFFFC,0xFFFD],                      # On/Off 1.5 p.48
  }
  static var TYPES = { 0x0100: 2 }                  # OnOff Light, but not actually used because Relay is managed by OnOff

  var shadow_onoff

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    self.shadow_onoff = false
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    import light
    var light_status = light.get()
    if light_status != nil
      var pow = light_status.find('power', nil)
      if pow != self.shadow_onoff self.attribute_updated(0x0006, 0x0000)   self.shadow_onoff = pow end
    end
    super(self).update_shadow()
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0006              # ========== On/Off 1.5 p.48 ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- OnOff / bool ----------
        return TLV.create_TLV(TLV.BOOL, self.shadow_onoff)
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
    import light
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var command = ctx.command

    # ====================================================================================================
    if   cluster == 0x0006              # ========== On/Off 1.5 p.48 ==========
      self.update_shadow_lazy()
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
    end
  end

end
matter.Plugin_Light0 = Matter_Plugin_Light0
