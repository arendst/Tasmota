#
# Matter_Plugin_Bridge_Sensor_Soil.be - implements base class for a Soil Sensor via HTTP to Tasmota
#
# Copyright (C) 2026  Ludovic BOUÉ
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
# Matter 1.6.0 Bridge Variant - Soil Sensor via HTTP
#################################################################################
# INHERITS FROM: Matter_Plugin_Sensor_Soil (Matter_Plugin_3_Sensor_Soil.be)
# VARIANT TYPE: Bridge (Remote HTTP Device)
# DEVICE TYPE: Soil Sensor (0x0045) - See base class
# CLUSTERS: Soil Measurement (0x0430) - See Matter_Plugin_3_Sensor_Soil.be
# TYPE: "http_soil" | BRIDGE: true
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_Sensor_Soil,weak

class Matter_Plugin_Bridge_Sensor_Soil : Matter_Plugin_Sensor_Soil
  static var BRIDGE = true                          # flag as bridged device
  static var TYPE = "http_soil"                     # name of the plug-in in json
end
matter.Plugin_Bridge_Sensor_Soil = Matter_Plugin_Bridge_Sensor_Soil
