#
# Matter_Plugin_Bridge_Sensor_Waterleak.be - implements Water Leak Sensor via HTTP to Tasmota
#
# Copyright (C) 2024  Stephan Hadinger & Theo Arends
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
# Matter 1.4.1 Bridge Variant - HTTP Remote Water Leak Detector
#################################################################################
# This is a BRIDGE variant that inherits from Matter_Plugin_Sensor_Waterleak.
# It communicates with a remote Tasmota device via HTTP to read water leak state.
#
# DEVICE TYPE: Water Leak Detector (0x0043)
# See Matter_Plugin_3_Sensor_Waterleak.be for complete Matter 1.4.1 specifications
# including Boolean State cluster (0x0045) details.
#
# BRIDGE BEHAVIOR:
# - Polls remote Tasmota device via HTTP using UPDATE_CMD ("Status 10")
# - Parses JSON response to extract leak detection state from Switch<x>
# - Maps Tasmota switch state to Matter StateValue (true=leak detected, false=no leak)
# - Inherits all cluster implementations from base class
# - UPDATE_TIME: 5000ms (5 seconds) for responsive leak detection
#
# CONFIGURATION:
# - TYPE: "http_waterleak" - Plugin identifier in Matter configuration
# - BRIDGE: true - Marks this as a bridged device
# - Requires endpoint configuration with remote device URL
# - ARG: Switch number on remote device (e.g., "1" for Switch1)
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_Sensor_Waterleak,weak

class Matter_Plugin_Bridge_Sensor_Waterleak : Matter_Plugin_Sensor_Waterleak
  static var BRIDGE = true
  static var TYPE = "http_waterleak"                  # name of the plug-in in json
  static var UPDATE_TIME = 5000                     # update every 5s
  static var UPDATE_CMD = "Status 10"               # command to send for updates
end
matter.Plugin_Bridge_Sensor_Waterleak = Matter_Plugin_Bridge_Sensor_Waterleak
