#
# Matter_Plugin_Bridge_Light0.be - implements the behavior for a remote generic Lighting (OnOff only) via HTTP
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
# Matter 1.4.1 Bridge Variant - On/Off Light via HTTP
#################################################################################
# INHERITS FROM: Matter_Plugin_Light0 (Matter_Plugin_2_Light0.be)
# VARIANT TYPE: Bridge (Remote HTTP Device)
#
# DEVICE TYPE: On/Off Light (0x0100) - See base class for full specification
# CLUSTERS: Same as Matter_Plugin_Light0 - See base class documentation
#
# BRIDGE MODE SPECIFICS:
# - BRIDGE flag: true (enables HTTP remote device mode)
# - Communicates with remote Tasmota device via HTTP API
# - UPDATE_TIME: 3000ms (polls remote device every 3 seconds)
# - Inherits all cluster implementations from base class
# - Automatic state synchronization with remote device
# - Reachability monitoring via HTTP ping
#
# CONFIGURATION:
# - TYPE: "http_light0" (JSON configuration identifier)
# - Requires "url" parameter for remote device address
# - Example: {"type":"http_light0","url":"192.168.1.100"}
#
# NOTES:
# - All Matter cluster behavior inherited from Matter_Plugin_Light0
# - Only adds HTTP bridge communication layer
# - See Matter_Plugin_2_Light0.be for complete cluster specifications
# - See Matter_Plugin_1_Device.be for bridge mode implementation details
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_Light0,weak

class Matter_Plugin_Bridge_Light0 : Matter_Plugin_Light0
  static var BRIDGE = true                          # flag as bridged device
  static var TYPE = "http_light0"                   # name of the plug-in in json
  # static var DISPLAY_NAME = "Light 0 OnOff"       # display name of the plug-in
  static var UPDATE_TIME = 3000                     # update every 3s
end
matter.Plugin_Bridge_Light0 = Matter_Plugin_Bridge_Light0
