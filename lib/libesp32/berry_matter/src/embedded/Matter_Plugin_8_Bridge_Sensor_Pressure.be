#
# Matter_Plugin_Bridge_Sensor_Pressure.be - implements Pressure Sensor via HTTP to Tasmota
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
# Matter 1.4.1 Bridge Variant - HTTP Remote Pressure Sensor
#################################################################################
# This is a BRIDGE variant that inherits from Matter_Plugin_Sensor_Pressure.
# It communicates with a remote Tasmota device via HTTP to read pressure values.
#
# DEVICE TYPE: Pressure Sensor (0x0305)
# See Matter_Plugin_3_Sensor_Pressure.be for complete Matter 1.4.1 specifications
# including Pressure Measurement cluster (0x0403) details.
#
# BRIDGE BEHAVIOR:
# - Polls remote Tasmota device via HTTP using UPDATE_CMD
# - Parses JSON response to extract pressure value
# - Converts pressure to 0.1 kPa units for Matter
# - Inherits all cluster implementations from base class
#
# CONFIGURATION:
# - TYPE: "http_pressure" - Plugin identifier in Matter configuration
# - BRIDGE: true - Marks this as a bridged device
# - Requires endpoint configuration with remote device URL
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_Sensor_Pressure,weak

class Matter_Plugin_Bridge_Sensor_Pressure : Matter_Plugin_Sensor_Pressure
  static var BRIDGE = true                          # flag as bridged device
  static var TYPE = "http_pressure"                 # name of the plug-in in json
end
matter.Plugin_Bridge_Sensor_Pressure = Matter_Plugin_Bridge_Sensor_Pressure
