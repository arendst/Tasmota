#
# Matter_Plugin_Bridge_Sensor_Flow.be - implements Flow Sensor via HTTP to Tasmota
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
# Matter 1.4.1 Bridge Variant - HTTP Remote Flow Sensor
#################################################################################
# This is a BRIDGE variant that inherits from Matter_Plugin_Sensor_Flow.
# It communicates with a remote Tasmota device via HTTP to read flow rate values.
#
# DEVICE TYPE: Flow Sensor (0x0306)
# See Matter_Plugin_3_Sensor_Flow.be for complete Matter 1.4.1 specifications
# including Flow Measurement cluster (0x0404) details.
#
# BRIDGE BEHAVIOR:
# - Polls remote Tasmota device via HTTP using UPDATE_CMD
# - Parses JSON response to extract flow rate value
# - Converts flow rate to 0.1 m³/h units for Matter
# - Inherits all cluster implementations from base class
#
# CONFIGURATION:
# - TYPE: "http_flow" - Plugin identifier in Matter configuration
# - BRIDGE: true - Marks this as a bridged device
# - Requires endpoint configuration with remote device URL
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_Sensor_Flow,weak

class Matter_Plugin_Bridge_Sensor_Flow : Matter_Plugin_Sensor_Flow
  static var BRIDGE = true                          # flag as bridged device
  static var TYPE = "http_flow"                 # name of the plug-in in json
end
matter.Plugin_Bridge_Sensor_Flow = Matter_Plugin_Bridge_Sensor_Flow
