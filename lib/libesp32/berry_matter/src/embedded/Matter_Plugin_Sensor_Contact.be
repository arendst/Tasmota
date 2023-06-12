#
# Matter_Plugin_Sensor_Contact.be - implements the behavior for a Occupany Switch
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

#@ solidify:Matter_Plugin_Sensor_Contact,weak

class Matter_Plugin_Sensor_Contact : Matter_Plugin_Device
  static var TYPE = "contact"                     # name of the plug-in in json
  static var NAME = "Contact"                     # display name of the plug-in
  static var ARG  = "switch"                        # additional argument name (or empty if none)
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var UPDATE_TIME = 5000                     # update every 250ms
  static var CLUSTERS  = {
    0x0015: [0,1,2,0xFFFC,0xFFFD],                  # Contact Sensing p.105 - no writable
  }
  static var TYPES = { 0x0107: 2 }                  # Contact Sensor, rev 2

  var tasmota_switch_index                          # Switch number in Tasmota (one based)
  var shadow_Contact

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

    import json
    var ret = tasmota.cmd("Status 8", true)
    if ret != nil
      var j = json.load(ret)
      if j != nil
        var state = false
        state = (j.find("Switch" + str(self.tasmota_switch_index)) == "ON")

        if self.shadow_Contact != nil && self.shadow_Contact != bool(state)
          self.attribute_updated(0x0015, 0x0000)
        end
        self.shadow_Contact = state
      end
    end
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
    // CHIP_DEVICE_CONFIG_DEVICE_TYPE 0x0015 // Contact sensor
    if   cluster == 0x0015              # ========== Contact Sensing ==========
      if   attribute == 0x0000          #  ---------- Contact / U8 ----------
        if self.shadow_Contact != nil
          return TLV.create_TLV(TLV.U1, self.shadow_Contact)
        else
          return TLV.create_TLV(TLV.NULL, nil)
        end
      elif attribute == 0x0001          #  ---------- ContactSensorType / enum8 ----------
        return TLV.create_TLV(TLV.U1, 3)  # physical contact
      elif attribute == 0x0002          #  ---------- ContactSensorTypeBitmap / u8 ----------
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
matter.Plugin_Sensor_Contact = Matter_Plugin_Sensor_Contact
