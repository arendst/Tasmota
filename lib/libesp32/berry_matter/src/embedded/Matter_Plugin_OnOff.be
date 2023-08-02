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

import matter

# Matter plug-in for core behavior

# dummy declaration for solidification
class Matter_Plugin_Device end

#@ solidify:Matter_Plugin_OnOff,weak

class Matter_Plugin_OnOff : Matter_Plugin_Device
  static var TYPE = "relay"                         # name of the plug-in in json
  static var NAME = "Relay"                         # display name of the plug-in
  static var ARG  = "relay"                         # additional argument name (or empty if none)
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var ARG_HINT = "Relay<x> number"
  static var UPDATE_TIME = 250                      # update every 250ms
  static var CLUSTERS  = {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0005: inherited                             # Scenes 1.4 p.30 - no writable
    0x0006: [0,0xFFFC,0xFFFD],                      # On/Off 1.5 p.48
  }
  static var TYPES = { 0x010A: 2 }                  # On/Off Plug-in Unit

  var tasmota_relay_index             # Relay number in Tasmota (zero based)
  var shadow_onoff                           # fake status for now # TODO

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    self.shadow_onoff = false
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    self.tasmota_relay_index = int(config.find(self.ARG #-'relay'-#, 1))
    if self.tasmota_relay_index <= 0    self.tasmota_relay_index = 1    end
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    var state = tasmota.get_power(self.tasmota_relay_index - 1)
    if state != nil
      if self.shadow_onoff != nil && self.shadow_onoff != bool(state)
        self.attribute_updated(0x0006, 0x0000)
      end
      self.shadow_onoff = state
    end
    super(self).update_shadow()
  end

  #############################################################
  # Model
  #
  def set_onoff(v)
    tasmota.set_power(self.tasmota_relay_index - 1, bool(v))
    self.update_shadow()
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0006              # ========== On/Off 1.5 p.48 ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- OnOff / bool ----------
        return tlv_solo.set(TLV.BOOL, self.shadow_onoff)
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0)    # 0 = no Level Control for Lighting
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return tlv_solo.set(TLV.U4, 4)    # 0 = no Level Control for Lighting
      end

    else
      return super(self).read_attribute(session, ctx, tlv_solo)
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
    if   cluster == 0x0006              # ========== On/Off 1.5 p.48 ==========
      self.update_shadow_lazy()
      if   command == 0x0000            # ---------- Off ----------
        self.set_onoff(false)
        self.update_shadow()
        return true
      elif command == 0x0001            # ---------- On ----------
        self.set_onoff(true)
        self.update_shadow()
        return true
      elif command == 0x0002            # ---------- Toggle ----------
        self.set_onoff(!self.shadow_onoff)
        self.update_shadow()
        return true
      end
    end

  end

end
matter.Plugin_OnOff = Matter_Plugin_OnOff
