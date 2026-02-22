#
# Matter_Plugin_9_Zigbee_Humidity.be - implements Zigbee Humidity Sensor
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
# Matter 1.4.1 Zigbee Variant - Zigbee2Matter Bridge Humidity Sensor
#################################################################################
# This is a ZIGBEE variant that inherits from Matter_Plugin_Sensor_Humidity.
# It bridges Zigbee humidity sensors to Matter protocol.
#
# DEVICE TYPE: Humidity Sensor (0x0307)
# See Matter_Plugin_3_Sensor_Humidity.be for complete Matter 1.4.1 specifications
# including Relative Humidity Measurement cluster (0x0405) details.
#
# ZIGBEE BEHAVIOR:
# - Bridges Zigbee devices to Matter using zigbee_mapper
# - Listens for Zigbee attribute reports (ZIGBEE_NAME: "Humidity")
# - Automatically converts Zigbee humidity values to Matter format
# - Maps Zigbee device to Matter endpoint
# - Inherits all cluster implementations from base class
# - VIRTUAL: true - Required for Zigbee device mapping
#
# CONFIGURATION:
# - TYPE: "z_humidity" - Plugin identifier in Matter configuration
# - DISPLAY_NAME: "Zig Humidity" - Shows as Zigbee humidity in UI
# - ZIGBEE: true - Marks this as a Zigbee bridge device
# - ARG: "zigbee_device" - Zigbee device identifier (short address or friendly name)
# - Requires Tasmota Zigbee coordinator functionality
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Zigbee_Humidity,weak

class Matter_Plugin_Zigbee_Humidity : Matter_Plugin_Sensor_Humidity
  static var ZIGBEE = true
  static var TYPE = "z_humidity"                    # name of the plug-in in json
  static var DISPLAY_NAME = "Zig Humidity"          # display name of the plug-in
  static var ZIGBEE_NAME = "Humidity"               # name of zigbee attribute with sensor reported

  static var SCHEMA = "zigbee_device|"              # arg name
                      "l:Device|"                   # label (display name)
                      "h:Device"                    # hint (type defaults to text)
  static var VIRTUAL = true                         # virtual device, necessary for Zigbee mapping
  var zigbee_mapper                                 # required for zigbee device

end
matter.Plugin_Zigbee_Humidity = Matter_Plugin_Zigbee_Humidity