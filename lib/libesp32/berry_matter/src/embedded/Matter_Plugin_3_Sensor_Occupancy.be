#
# Matter_Plugin_3_Sensor_Occupancy.be - implements the behavior for a Occupany Switch
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

#@ solidify:Matter_Plugin_Sensor_Occupancy,weak

class Matter_Plugin_Sensor_Occupancy : Matter_Plugin_Sensor_Boolean
  static var TYPE = "occupancy"                     # name of the plug-in in json
  static var DISPLAY_NAME = "Occupancy"                     # display name of the plug-in
  # static var ARG  = "switch"                        # additional argument name (or empty if none)
  # static var ARG_HINT = "Switch<x> number"
  # static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  # static var UPDATE_TIME = 750                      # update every 750ms
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Occupancy")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0406: [0,1,2],                                # Occupancy Sensing p.105 - no writable
  })
  static var TYPES = { 0x0107: 2 }                  # Occupancy Sensor, rev 2

  # var tasmota_switch_index                          # Switch number in Tasmota (one based)
  # var shadow_bool_value

  #############################################################
  # value_updated
  #
  # This is triggered when a new value is changed, for subscription
  # This method is meant to be overloaded and maximize shared code
  def value_updated()
    self.attribute_updated(0x0406, 0x0000)
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0406              # ========== Occupancy Sensing ==========
      if   attribute == 0x0000          #  ---------- Occupancy / U8 ----------
        return tlv_solo.set_or_nil(TLV.U1, self.shadow_bool_value)
      elif attribute == 0x0001          #  ---------- OccupancySensorType / enum8 ----------
        return tlv_solo.set(TLV.U1, 3)  # physical contact
      elif attribute == 0x0002          #  ---------- OccupancySensorTypeBitmap / u8 ----------
        return tlv_solo.set(TLV.U1, 0)  # unknown
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload)
    self.shadow_bool_value = self._parse_update_virtual(payload, "Occupancy", self.shadow_bool_value, bool, 0x0406, 0x0000)
    super(self).update_virtual(payload)
  end

  #############################################################
  # For Bridge devices
  #############################################################
  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    import webserver
    self.web_values_prefix()        # display '| ' and name if present
    webserver.content_send(format("Occupancy%i %s", self.shadow_bool_value, self.web_value_onoff(self.shadow_occupancy)))
  end

  # Show prefix before web value
  def web_values_prefix()
    import webserver
    var name = self.get_name()
    if !name
      name = "Switch" + str(self.shadow_bool_value)
    end
    webserver.content_send(format(self.PREFIX, name ? webserver.html_escape(name) : ""))
  end
  #############################################################
  #############################################################
  
end
matter.Plugin_Sensor_Occupancy = Matter_Plugin_Sensor_Occupancy
