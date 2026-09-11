#
# Matter_Plugin_Virt_OnOff_Power.be - implements the behavior for a Virtual Relay
# with Electrical Power Measurement
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

#################################################################################
# Matter 1.4.1 Virtual Variant - On/Off Plug-in Unit with Power Measurement
#################################################################################
# INHERITS FROM: Matter_Plugin_OnOff_Power (Matter_Plugin_3_OnOff_Power.be)
# VARIANT TYPE: Virtual (Matter Bridge Protocol)
#
# DEVICE TYPE: On/Off Plug-in Unit (0x010A) - See base class for specification
# CLUSTERS: On/Off + Electrical Power Measurement (0x0090) - See base class
#
# VIRTUAL MODE SPECIFICS:
# - VIRTUAL flag: true (enables virtual device mode)
# - TYPE: "v_relay_power" (JSON configuration identifier)
# - DISPLAY_NAME: "v.Relay+Power" (prefix 'v.' indicates virtual)
# - No direct hardware control - state and measurements managed by external
#   controller via the `MtrUpdate` command
#
# CONFIGURATION:
# - No ARG parameter required (virtual devices don't map to hardware)
# - Example: {"type":"v_relay_power","name":"Virtual Plug"}
#
# TESTING:
#   MtrUpdate {"Name":"Virtual Plug", "Power":1}
#   MtrUpdate {"Name":"Virtual Plug", "Voltage":230.5, "Current":0.42, "ActivePower":95.6}
#   MtrUpdate {"Name":"Virtual Plug", "ApparentPower":98.1, "ReactivePower":12.0, "Factor":0.97, "Frequency":50.0}
#
# NOTES:
# - `Power` remains the On/Off boolean command (inherited); the wattage value
#   is sent as `ActivePower` to avoid ambiguity - see Matter_Plugin_3_OnOff_Power.be
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Virt_OnOff_Power,weak

class Matter_Plugin_Virt_OnOff_Power : Matter_Plugin_OnOff_Power
  static var TYPE = "v_relay_power"                 # name of the plug-in in json
  static var DISPLAY_NAME = "v.Relay+Power"         # display name of the plug-in

  static var SCHEMA = nil                          # no parameter
  static var VIRTUAL = true                         # virtual device
end
matter.Plugin_Virt_OnOff_Power = Matter_Plugin_Virt_OnOff_Power
