#
# Matter_Plugin_Bridge_Sensor_Contact.be - implements Contact Sensor via HTTP to Tasmota
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

#@ solidify:Matter_Plugin_Bridge_Sensor_Contact,weak

class Matter_Plugin_Bridge_Sensor_Contact : Matter_Plugin_Bridge_HTTP
  static var TYPE = "http_contact"                  # name of the plug-in in json
  static var DISPLAY_NAME = "Contact"                       # display name of the plug-in
  static var ARG  = "switch"                        # additional argument name (or empty if none)
  static var ARG_HINT = "Switch<x> number"
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var UPDATE_TIME = 5000                     # update every 5s
  static var UPDATE_CMD = "Status 8"                # command to send for updates

  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0045: [0,0xFFFC,0xFFFD],                      # Boolean State p.70 - no writable
  })
  static var TYPES = { 0x0015: 1 }                  # Contact Sensor, rev 1

  var tasmota_switch_index                          # Switch number in Tasmota (one based)
  var shadow_contact

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    super(self).init(device, endpoint, arguments)
    self.tasmota_switch_index = int(arguments.find(self.ARG #-'switch'-#, 1))
    if self.tasmota_switch_index <= 0    self.tasmota_switch_index = 1    end
  end

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This call is synnchronous and blocking.
  def parse_update(data, index)
    if index == 8                              # Status 8 
      var state = false

      state = (data.find("Switch" + str(self.tasmota_switch_index)) == "ON")

      if self.shadow_contact != nil && self.shadow_contact != bool(state)
        self.attribute_updated(0x0045, 0x0000)
      end
      self.shadow_contact = state
    end
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
        if self.shadow_contact != nil
          return tlv_solo.set(TLV.BOOL, self.shadow_contact)
        else
          return tlv_solo.set(TLV.NULL, nil)
        end
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0)
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return tlv_solo.set(TLV.U4, 1)    # 1 = Initial release
      end

    else
      return super(self).read_attribute(session, ctx, tlv_solo)
    end
  end

  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    import webserver
    self.web_values_prefix()        # display '| ' and name if present
    webserver.content_send(format("Contact%i %s", self.tasmota_switch_index, self.web_value_onoff(self.shadow_contact)))
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
end
matter.Plugin_Bridge_Sensor_Contact = Matter_Plugin_Bridge_Sensor_Contact
