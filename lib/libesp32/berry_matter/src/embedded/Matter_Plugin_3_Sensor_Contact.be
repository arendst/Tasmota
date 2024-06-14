#
# Matter_Plugin_3_Sensor_Contact.be - implements the behavior for a Contact Sensor
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

#@ solidify:Matter_Plugin_Sensor_Contact,weak

class Matter_Plugin_Sensor_Contact : Matter_Plugin_Sensor_Boolean
  static var TYPE = "contact"                       # name of the plug-in in json
  static var DISPLAY_NAME = "Contact"                       # display name of the plug-in
  # static var ARG  = "switch"                        # additional argument name (or empty if none)
  # static var ARG_HINT = "Switch<x> number"
  # static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  # static var UPDATE_TIME = 750                      # update every 750ms
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Contact")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0045: [0],                                    # Boolean State p.70 - no writable
  })
  static var TYPES = { 0x0015: 1 }                  # Contact Sensor, rev 1

  # var tasmota_switch_index                          # Switch number in Tasmota (one based)
  # var shadow_bool_value

  #############################################################
  # value_updated
  #
  # This is triggered when a new value is changed, for subscription
  # This method is meant to be overloaded and maximize shared code
  def value_updated()
    self.attribute_updated(0x0045, 0x0000)
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0045              # ========== Boolean State ==========
      if   attribute == 0x0000          #  ---------- StateValue / bool ----------
        return tlv_solo.set_or_nil(TLV.BOOL, self.shadow_bool_value)
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload)
    self.shadow_bool_value = self._parse_update_virtual(payload, "Contact", self.shadow_bool_value, bool, 0x0045, 0x0000)
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
    webserver.content_send(format("Contact%i %s", self.tasmota_switch_index, self.web_value_onoff(self.shadow_bool_value)))
  end

  # Show prefix before web value
  def web_values_prefix()
    import webserver
    var name = self.get_name()
    if !name
      name = "Switch" + str(self.tasmota_switch_index)
    end
    webserver.content_send(format(self.PREFIX, name ? webserver.html_escape(name) : ""))
  end
  #############################################################
  #############################################################
end
matter.Plugin_Sensor_Contact = Matter_Plugin_Sensor_Contact
