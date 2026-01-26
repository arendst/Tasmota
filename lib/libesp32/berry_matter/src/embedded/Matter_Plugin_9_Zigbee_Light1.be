#
# Matter_Plugin_9_Zigbee_Light1.be - implements Zigbee Dimmable Light
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
# Matter 1.4.1 Zigbee Variant - Zigbee2Matter Bridge Dimmable Light
#################################################################################
# This is a ZIGBEE variant that inherits from Matter_Plugin_Light1.
# It bridges Zigbee dimmable lights to Matter protocol.
#
# DEVICE TYPE: Dimmable Light (0x0101)
# See Matter_Plugin_3_Light1.be for complete Matter 1.4.1 specifications
# including On/Off cluster (0x0006) and Level Control cluster (0x0008) details.
#
# ZIGBEE BEHAVIOR:
# - Bridges Zigbee dimmable lights to Matter using zigbee_mapper
# - Listens for Zigbee attribute reports (Power, Dimmer)
# - Sends Zigbee commands when Matter controller changes state or level
# - Automatically converts between Zigbee and Matter formats
# - Maps Zigbee device to Matter endpoint
# - Inherits all cluster implementations from base class
# - VIRTUAL: true - Required for Zigbee device mapping
#
# CONFIGURATION:
# - TYPE: "z_light1" - Plugin identifier in Matter configuration
# - DISPLAY_NAME: "Zig Light 1 Dimmer" - Shows as Zigbee dimmable light in UI
# - ZIGBEE: true - Marks this as a Zigbee bridge device
# - ARG: "zigbee_device" - Zigbee device identifier (short address or friendly name)
# - Requires Tasmota Zigbee coordinator functionality
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Zigbee_Light1,weak

class Matter_Plugin_Zigbee_Light1 : Matter_Plugin_Light1
  static var ZIGBEE = true
  static var TYPE = "z_light1"                      # name of the plug-in in json
  static var DISPLAY_NAME = "Zig Light 1 Dimmer"    # display name of the plug-in
  static var ARG  = "zigbee_device"                 # zigbee device
  static var ARG_TYPE = / x -> str(x)               # function to convert argument to the right type
  static var ARG_HINT = "Device"                    # Hint for entering the Argument (inside 'placeholder')
  static var VIRTUAL = true                         # virtual device, necessary for Zigbee mapping
  var zigbee_mapper                                 # required for zigbee device

end
matter.Plugin_Zigbee_Light1 = Matter_Plugin_Zigbee_Light1
