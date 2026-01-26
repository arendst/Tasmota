#
# Matter_Plugin_9_Zigbee_Light2.be - implements Zigbee Color Temperature Light
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
# Matter 1.4.1 Zigbee Variant - Zigbee2Matter Bridge Color Temperature Light
#################################################################################
# This is a ZIGBEE variant that inherits from Matter_Plugin_Light2.
# It bridges Zigbee color temperature lights to Matter protocol.
#
# DEVICE TYPE: Color Temperature Light (0x010C)
# See Matter_Plugin_4_Light2.be for complete Matter 1.4.1 specifications
# including On/Off (0x0006), Level Control (0x0008), and Color Control (0x0300) clusters.
#
# ZIGBEE BEHAVIOR:
# - Bridges Zigbee CT lights to Matter using zigbee_mapper
# - Listens for Zigbee attribute reports (Power, Dimmer, CT)
# - Sends Zigbee commands when Matter controller changes state, level, or color temperature
# - Automatically converts between Zigbee and Matter formats
# - Maps Zigbee device to Matter endpoint
# - Inherits all cluster implementations from base class
# - VIRTUAL: true - Required for Zigbee device mapping
#
# CONFIGURATION:
# - TYPE: "z_light2" - Plugin identifier in Matter configuration
# - DISPLAY_NAME: "Zig Light 2 CT" - Shows as Zigbee CT light in UI
# - ZIGBEE: true - Marks this as a Zigbee bridge device
# - ARG: "zigbee_device" - Zigbee device identifier (short address or friendly name)
# - Requires Tasmota Zigbee coordinator functionality
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Zigbee_Light2,weak

class Matter_Plugin_Zigbee_Light2 : Matter_Plugin_Light2
  static var ZIGBEE = true
  static var TYPE = "z_light2"                      # name of the plug-in in json
  static var DISPLAY_NAME = "Zig Light 2 CT"        # display name of the plug-in
  static var ARG  = "zigbee_device"                 # zigbee device
  static var ARG_TYPE = / x -> str(x)               # function to convert argument to the right type
  static var ARG_HINT = "Device"                    # Hint for entering the Argument (inside 'placeholder')
  static var VIRTUAL = true                         # virtual device, necessary for Zigbee mapping
  var zigbee_mapper                                 # required for zigbee device

end
matter.Plugin_Zigbee_Light2 = Matter_Plugin_Zigbee_Light2
