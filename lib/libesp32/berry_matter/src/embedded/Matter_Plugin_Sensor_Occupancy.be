#
# Matter_Plugin_Sensor_Occupancy.be - implements the behavior for a Occupany Switch
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

#@ solidify:Matter_Plugin_Sensor_Occupancy,weak

class Matter_Plugin_Sensor_Occupancy : Matter_Plugin_Device
  static var TYPE = "occupancy"                     # name of the plug-in in json
  static var NAME = "Occupancy"                     # display name of the plug-in
  static var ARG  = "switch"                        # additional argument name (or empty if none)
  static var ARG_HINT = "Enter Switch<x> number"
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var UPDATE_TIME = 5000                     # update every 250ms
  static var CLUSTERS  = {
    0x0406: [0,1,2,0xFFFC,0xFFFD],                  # Occupancy Sensing p.105 - no writable
  }
  static var TYPES = { 0x0107: 2 }                  # Occupancy Sensor, rev 2

  var tasmota_switch_index                          # Switch number in Tasmota (one based)
  var shadow_occupancy

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
    var switch_str = "Switch" + str(self.tasmota_switch_index)

    var j = tasmota.cmd("Status 8", true)
    if j != nil   j = j.find("StatusSNS") end
    if j != nil && j.contains(switch_str)
      var state = (j.find(switch_str) == "ON")

      if (self.shadow_occupancy != state)
        self.attribute_updated(0x0406, 0x0000)
      end
      self.shadow_occupancy = state
    end
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0406              # ========== Occupancy Sensing ==========
      if   attribute == 0x0000          #  ---------- Occupancy / U8 ----------
        if self.shadow_occupancy != nil
          return TLV.create_TLV(TLV.U1, self.shadow_occupancy)
        else
          return TLV.create_TLV(TLV.NULL, nil)
        end
      elif attribute == 0x0001          #  ---------- OccupancySensorType / enum8 ----------
        return TLV.create_TLV(TLV.U1, 3)  # physical contact
      elif attribute == 0x0002          #  ---------- OccupancySensorTypeBitmap / u8 ----------
        return TLV.create_TLV(TLV.U1, 0)  # unknown
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return TLV.create_TLV(TLV.U4, 0)
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return TLV.create_TLV(TLV.U4, 3)    # 4 = New data model format and notation
      end

    else
      return super(self).read_attribute(session, ctx)
    end
  end

end
matter.Plugin_Sensor_Occupancy = Matter_Plugin_Sensor_Occupancy
