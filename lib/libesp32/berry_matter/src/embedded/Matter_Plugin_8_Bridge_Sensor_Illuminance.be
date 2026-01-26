#
# Matter_Plugin_Bridge_Sensor_Illuminance.be - implements Light/Illuminance Sensor via HTTP to Tasmota
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
# Matter 1.4.1 Bridge Variant - HTTP Remote Illuminance Sensor
#################################################################################
# This is a BRIDGE variant that inherits from Matter_Plugin_Sensor_Illuminance.
# It communicates with a remote Tasmota device via HTTP to read light levels.
#
# DEVICE TYPE: Light Sensor (0x0106)
# See Matter_Plugin_3_Sensor_Illuminance.be for complete Matter 1.4.1 specifications
# including Illuminance Measurement cluster (0x0400) details.
#
# BRIDGE BEHAVIOR:
# - Polls remote Tasmota device via HTTP using UPDATE_CMD
# - Parses JSON response to extract illuminance value
# - Converts illuminance to logarithmic scale (10000 * log10(lux) + 1)
# - Inherits all cluster implementations from base class
#
# CONFIGURATION:
# - TYPE: "http_illuminance" - Plugin identifier in Matter configuration
# - BRIDGE: true - Marks this as a bridged device
# - Requires endpoint configuration with remote device URL
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_Sensor_Illuminance,weak

class Matter_Plugin_Bridge_Sensor_Illuminance : Matter_Plugin_Sensor_Illuminance
  static var BRIDGE = true                          # flag as bridged device
  static var TYPE = "http_illuminance"              # name of the plug-in in json  
end
matter.Plugin_Bridge_Sensor_Illuminance = Matter_Plugin_Bridge_Sensor_Illuminance
