#
# Matter_Plugin_Sensor_Contact.be - implements the behavior for a Contact Sensor
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

class Matter_Plugin_Sensor_Contact : Matter_Plugin_Device
  static var TYPE = "contact"                       # name of the plug-in in json
  static var DISPLAY_NAME = "Contact"                       # display name of the plug-in
  static var ARG  = "switch"                        # additional argument name (or empty if none)
  static var ARG_HINT = "Switch<x> number"
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var UPDATE_TIME = 750                      # update every 750ms
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Contact")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0045: [0,0xFFFC,0xFFFD],                      # Boolean State p.70 - no writable
  })
  static var TYPES = { 0x0015: 1 }                  # Contact Sensor, rev 1

  var tasmota_switch_index                          # Switch number in Tasmota (one based)
  var shadow_contact

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    self.shadow_contact = false
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    self.tasmota_switch_index = int(config.find(self.ARG #-'switch'-#, 1))
    if self.tasmota_switch_index <= 0    self.tasmota_switch_index = 1    end
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    super(self).update_shadow()
    if !self.VIRTUAL
      import json
      var ret = tasmota.cmd("Status 8", true)
      if ret != nil
        var j = json.load(ret)
        if j != nil
          var state = false
          state = (j.find("Switch" + str(self.tasmota_switch_index)) == "ON")

          if self.shadow_contact != state
            self.attribute_updated(0x0045, 0x0000)
            self.shadow_contact = state
          end
        end
      end
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
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload_json)
    var val_onoff = payload_json.find("Contact")
    if val_onoff != nil
      val_onoff = bool(val_onoff)
      if self.shadow_contact != val_onoff
        self.attribute_updated(0x0045, 0x0000)
        self.shadow_contact = val_onoff
      end
    end
    super(self).update_virtual(payload_json)
  end

  #############################################################
  # append_state_json
  #
  # Output the current state in JSON
  # New values need to be appended with `,"key":value` (including prefix comma)
  def append_state_json()
    return f',"Contact":{int(self.shadow_contact)}'
  end

end
matter.Plugin_Sensor_Contact = Matter_Plugin_Sensor_Contact
