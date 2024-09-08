#
# Matter_Plugin_3_Sensor_OnOff.be - implements the behavior for a Occupany Switch
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

#@ solidify:Matter_Plugin_Sensor_OnOff,weak

class Matter_Plugin_Sensor_OnOff : Matter_Plugin_Sensor_Boolean
  static var TYPE = "onoff"                         # name of the plug-in in json
  static var DISPLAY_NAME = "OnOff Sensor"                  # display name of the plug-in
  # static var ARG  = "switch"                        # additional argument name (or empty if none)
  # static var ARG_HINT = "Switch<x> number"
  # static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  # static var UPDATE_TIME = 750                      # update every 750ms
  static var JSON_NAME = "OnOff"                    # Name of the sensor attribute in JSON payloads
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "OnOff")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0006: [0],                                    # On/Off 1.5 p.48
  })
  static var TYPES = { 0x0850: 2 }                  # OnOff Sensor, rev 2

  # var tasmota_switch_index                          # Switch number in Tasmota (one based)
  # var shadow_bool_value

  #############################################################
  # value_updated
  #
  # This is triggered when a new value is changed, for subscription
  # This method is meant to be overloaded and maximize shared code
  def value_updated()
    self.attribute_updated(0x0006, 0x0000)
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
        return tlv_solo.set(TLV.BOOL, self.shadow_bool_value)
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
    return f',"OnOff":{int(self.shadow_bool_value)}'
  end

end
matter.Plugin_Sensor_OnOff = Matter_Plugin_Sensor_OnOff
