#
# Matter_Plugin_9_Virt_Sensor_GenericSwitch_Btn.be - virtual Generic Switch
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

import matter

#@ solidify:Matter_Plugin_Virt_Sensor_GenericSwitch_Btn,weak

class Matter_Plugin_Virt_Sensor_GenericSwitch_Btn : Matter_Plugin_Sensor_GenericSwitch_Btn
  static var TYPE = "v_gensw"
  static var DISPLAY_NAME = "v.Generic Switch/Button"
  static var SCHEMA = nil
  static var VIRTUAL = true
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Presses")

  # Physical button callbacks are broadcast to all plugins; virtual endpoints
  # receive their gestures only through MtrUpdate.
  def button_handler(button, mode, state, press_counter) end

  # Each update describes a completed gesture, including repeated equal values.
  # Omitting Presses queries state; invalid values emit no events.
  def update_virtual(payload)
    if !payload.contains("Presses") return end
    var presses = payload["Presses"]
    if type(presses) != 'int' || presses < 1 || presses > 5
      return "Invalid 'Presses' attribute (expected integer 1..5)"
    end

    for press : 1..presses
      super(self).button_handler(self.tasmota_switch_index, 1, 1, press - 1)
      super(self).button_handler(self.tasmota_switch_index, 1, 0, press - 1)
    end
    super(self).button_handler(self.tasmota_switch_index, 2, 0, presses)
  end
end
matter.Plugin_Virt_Sensor_GenericSwitch_Btn = Matter_Plugin_Virt_Sensor_GenericSwitch_Btn
