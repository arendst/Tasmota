#
# Matter_Plugin_Virt_Light0.be - implements the behavior for a virtual Light (OnOff only)
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
# Matter 1.4.1 Virtual Variant - On/Off Light
#################################################################################
# INHERITS FROM: Matter_Plugin_Light0 (Matter_Plugin_2_Light0.be)
# VARIANT TYPE: Virtual (Matter Bridge Protocol)
#
# DEVICE TYPE: On/Off Light (0x0100) - See base class for full specification
# CLUSTERS: Same as Matter_Plugin_Light0 - See base class documentation
#
# VIRTUAL MODE SPECIFICS:
# - VIRTUAL flag: true (enables virtual device mode)
# - Receives state updates via Matter bridge protocol (MtrUpdate)
# - No direct hardware control - state managed by external controller
# - No ARG parameter required (virtual devices don't map to hardware)
# - State updates via JSON messages: {"Power":1} or {"Power":0}
#
# CONFIGURATION:
# - TYPE: "v_light0" (JSON configuration identifier)
# - DISPLAY_NAME: "v.Light 0 OnOff" (prefix 'v.' indicates virtual)
# - No hardware mapping required
# - Example: {"type":"v_light0","name":"Virtual Light"}
#
# USE CASES:
# - Matter bridge aggregating devices from other ecosystems
# - Virtual representations of cloud-connected devices
# - Testing and development without physical hardware
# - Multi-protocol bridges (Zigbee, Z-Wave, etc. to Matter)
#
# NOTES:
# - All Matter cluster behavior inherited from Matter_Plugin_Light0
# - Only changes state update mechanism (virtual vs hardware)
# - See Matter_Plugin_2_Light0.be for complete cluster specifications
# - See Matter_Plugin_1_Device.be for virtual device implementation
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Virt_Light0,weak

class Matter_Plugin_Virt_Light0 : Matter_Plugin_Light0
  static var TYPE = "v_light0"                      # name of the plug-in in json
  static var DISPLAY_NAME = "v.Light 0 OnOff"       # display name of the plug-in

  static var SCHEMA = nil                          # no parameter
  static var VIRTUAL = true                         # virtual device
end
matter.Plugin_Virt_Light0 = Matter_Plugin_Virt_Light0
