#
# Matter_Plugin_Bridge_Sensor_Occupancy.be - implements Occupancy Sensor via HTTP to Tasmota
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
# Matter 1.4.1 Bridge Variant - HTTP Remote Occupancy Sensor
#################################################################################
# This is a BRIDGE variant that inherits from Matter_Plugin_Sensor_Occupancy.
# It communicates with a remote Tasmota device via HTTP to read occupancy state.
#
# DEVICE TYPE: Occupancy Sensor (0x0107)
# See Matter_Plugin_3_Sensor_Occupancy.be for complete Matter 1.4.1 specifications
# including Occupancy Sensing cluster (0x0406) details.
#
# BRIDGE BEHAVIOR:
# - Polls remote Tasmota device via HTTP using UPDATE_CMD ("Status 10")
# - Parses JSON response to extract occupancy state from Switch<x>
# - Maps Tasmota switch state to Matter Occupancy bitmap
# - Inherits all cluster implementations from base class
# - UPDATE_TIME: 5000ms (5 seconds) for responsive occupancy detection
#
# CONFIGURATION:
# - TYPE: "http_occupancy" - Plugin identifier in Matter configuration
# - BRIDGE: true - Marks this as a bridged device
# - Requires endpoint configuration with remote device URL
# - ARG: Switch number on remote device (e.g., "1" for Switch1)
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_Sensor_Occupancy,weak

class Matter_Plugin_Bridge_Sensor_Occupancy : Matter_Plugin_Sensor_Occupancy
  static var BRIDGE = true
  static var TYPE = "http_occupancy"                # name of the plug-in in json
  # static var DISPLAY_NAME = "Occupancy"             # display name of the plug-in
  # static var ARG  = "switch"                        # additional argument name (or empty if none)
  # static var ARG_HINT = "Switch<x> number"
  # static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var UPDATE_TIME = 5000                     # update every 5s
  static var UPDATE_CMD = "Status 10"               # command to send for updates
end
matter.Plugin_Bridge_Sensor_Occupancy = Matter_Plugin_Bridge_Sensor_Occupancy
