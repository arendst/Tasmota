#
# Matter_Plugin_Sensor_Boolean.be - implements the behavior for an abstract boolean sensor - to be inherited
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

#@ solidify:Matter_Plugin_Sensor_Boolean,weak

class Matter_Plugin_Sensor_Boolean : Matter_Plugin_Device
  # static var TYPE = ""                              # name of the plug-in in json
  # static var DISPLAY_NAME = ""                      # display name of the plug-in
  static var ARG  = "switch"                        # additional argument name (or empty if none)
  static var ARG_HINT = "Switch<x> number"
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var UPDATE_TIME = 750                      # update every 750ms

  var tasmota_switch_index                          # Switch number in Tasmota (one based)
  var shadow_bool_value

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    self.shadow_bool_value = false
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    super(self).parse_configuration(config)
    self.tasmota_switch_index = int(config.find(self.ARG #-'switch'-#, 1))
    if self.tasmota_switch_index <= 0    self.tasmota_switch_index = 1    end
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    super(self).update_shadow()
    if !self.VIRTUAL
      var switch_str = "Switch" + str(self.tasmota_switch_index)

      var j = tasmota.cmd("Status 10", true)
      if j != nil   j = j.find("StatusSNS") end
      if j != nil && j.contains(switch_str)
        var state = (j.find(switch_str) == "ON")

        if (self.shadow_bool_value != state)
          self.value_updated()
        end
        self.shadow_bool_value = state
      end
    end
  end

  #############################################################
  # value_updated
  #
  # This is triggered when a new value is changed, for subscription
  # This method is meant to be overloaded and maximize shared code
  def value_updated()
  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload)
    self.shadow_bool_value = self._parse_update_virtual(payload, self.JSON_NAME, self.shadow_bool_value, bool, nil, nil)   # publishing cluster/attr is delegated to `value_updated()`
    self.value_updated()
    super(self).update_virtual(payload)
  end

  #############################################################
  # For Bridge devices
  #############################################################
  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This call is synnchronous and blocking.
  def parse_status(data, index)
    if index == 10                             # Status 10
      var state = false

      state = (data.find("Switch" + str(self.tasmota_switch_index)) == "ON")

      if self.shadow_bool_value != nil && self.shadow_bool_value != bool(state)
        self.value_updated()
      end
      self.shadow_bool_value = state
    end
  end
  #############################################################
  #############################################################
end
matter.Plugin_Sensor_Boolean = Matter_Plugin_Sensor_Boolean
