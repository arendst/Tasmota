#
# Matter_Plugin_Virt_OnOff.be - implements the behavior for a Virtual Relay
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
# Matter 1.4.1 Virtual Variant - On/Off Plug-in Unit (Relay)
#################################################################################
# INHERITS FROM: Matter_Plugin_OnOff (Matter_Plugin_3_OnOff.be)
# VARIANT TYPE: Virtual (Matter Bridge Protocol)
#
# DEVICE TYPE: On/Off Plug-in Unit (0x010A) - See base class for specification
# CLUSTERS: Same as Matter_Plugin_OnOff - See base class documentation
#
# VIRTUAL MODE SPECIFICS:
# - VIRTUAL flag: true (enables virtual device mode)
# - TYPE: "v_relay" (JSON configuration identifier)
# - DISPLAY_NAME: "v.Relay" (prefix 'v.' indicates virtual)
# - Receives state updates via Matter bridge protocol (MtrUpdate)
# - No direct hardware control - state managed by external controller
# - State updates via JSON messages: {"Power":1} or {"Power":0}
#
# CONFIGURATION:
# - No ARG parameter required (virtual devices don't map to hardware)
# - Example: {"type":"v_relay","name":"Virtual Relay"}
#
# USE CASES:
# - Virtual representation of smart plugs from other ecosystems
# - Cloud-connected outlet control
# - Multi-protocol bridge endpoints
# - Testing relay control without physical hardware
#
# NOTES:
# - Functionally identical to virtual Light0 but with different device type
# - Device type 0x010A (On/Off Plug-in Unit) vs 0x0100 (On/Off Light)
# - See Matter_Plugin_3_OnOff.be for device type specification
# - See Matter_Plugin_2_Light0.be for cluster specifications
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Virt_OnOff,weak

class Matter_Plugin_Virt_OnOff : Matter_Plugin_OnOff
  static var TYPE = "v_relay"                       # name of the plug-in in json
  static var DISPLAY_NAME = "v.Relay"                     # display name of the plug-in
  static var ARG  = ""                              # no arg for virtual device
  static var ARG_HINT = "_Not used_"          # Hint for entering the Argument (inside 'placeholder')
  static var VIRTUAL = true                         # virtual device
end
matter.Plugin_Virt_OnOff = Matter_Plugin_Virt_OnOff
