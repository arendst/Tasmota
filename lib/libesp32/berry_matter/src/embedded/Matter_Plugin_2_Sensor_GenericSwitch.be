#
# Matter_Plugin_Sensor_GenericSwitch.be - implements the behavior for a Generic Switch
#
# Copyright (C) 2023-2024  Stephan Hadinger & Theo Arends
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

#@ solidify:Matter_Plugin_Sensor_GenericSwitch,weak

class Matter_Plugin_Sensor_GenericSwitch : Matter_Plugin_Device
  static var TYPE = "gensw"                         # name of the plug-in in json
  static var DISPLAY_NAME = "Generic Switch"        # display name of the plug-in
  static var ARG  = "switch"                        # additional argument name (or empty if none)
  static var ARG_HINT = "Switch<x> number"
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var UPDATE_TIME = 750                      # update every 750ms    - TODO still necessary?
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0039: [3,5,0x0A,0x0F,0x11,0x12],              # Bridged Device Basic Information 9.13 p.485
    # 0x0003: [0,1],                                  # Identify 1.2 p.16
    # 0x0004: [0],                                    # Groups 1.3 p.21
    # 0x0005: [0,1,2,3,4,5],                          # Scenes 1.4 p.30 - no writable
    0x003B: [0, 1, 2],                              # Switch 1.12
  })
  static var TYPES = { 0x000F: 2 }                  # Generic Switch, rev 2

  var tasmota_switch_index                          # Switch number in Tasmota (one based)
  var shadow_value

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    self.tasmota_switch_index = int(config.find(self.ARG #-'relay'-#, 1))
    if self.tasmota_switch_index <= 0    self.tasmota_switch_index = 1    end
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    super(self).update_shadow()
    self.shadow_value = false
    # TODO
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x003B              # ========== Generic Switch, 1.12 ==========
    #   self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- NumberOfPositions / uint8 ----------
        return tlv_solo.set(TLV.U1, 2)  # default to 2 positions
      elif attribute == 0x0001          #  ---------- CurrentPosition / uint8 ----------
        return tlv_solo.set(TLV.U1, 0)  # TODO read value
      elif attribute == 0x0002          #  ---------- MultiPressMax / uint8 ----------
        return tlv_solo.set(TLV.U1, 2)  # up to double press

      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0x02 + 0x04 + 0x08)    # MomentarySwitch + MomentarySwitchRelease + MomentarySwitchLongPress
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # append_state_json
  #
  # Output the current state in JSON
  # New values need to be appended with `,"key":value` (including prefix comma)
  #
  # Override the default behavior to use the key `OnOff` instead of `Power`
  def append_state_json()
    return f',"Switch":{int(self.shadow_onoff)}'
  end

end
matter.Plugin_Sensor_GenericSwitch = Matter_Plugin_Sensor_GenericSwitch
