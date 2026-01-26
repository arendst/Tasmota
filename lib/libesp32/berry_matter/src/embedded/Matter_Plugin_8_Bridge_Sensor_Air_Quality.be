#
# Matter_Plugin_Bridge_Sensor_Air_Quality.be - implements Air Quality Sensor via HTTP to Tasmota
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
# Matter 1.4.1 Bridge Variant - HTTP Remote Air Quality Sensor
#################################################################################
# This is a BRIDGE variant that inherits from Matter_Plugin_Sensor_Air_Quality.
# It communicates with a remote Tasmota device via HTTP to read air quality data.
#
# DEVICE TYPE: Air Quality Sensor (0x002C)
# See Matter_Plugin_2_Sensor_Air_Quality.be for complete Matter 1.4.1 specifications
# including Air Quality cluster (0x005B) and Concentration Measurement clusters.
#
# BRIDGE BEHAVIOR:
# - Polls remote Tasmota device via HTTP using UPDATE_CMD ("Status 10")
# - Parses JSON response to extract multiple air quality measurements:
#   * CO2 concentration (Carbon Dioxide Concentration Measurement 0x040D)
#   * PM1 concentration (PM1 Concentration Measurement 0x042C)
#   * PM2.5 concentration (PM2.5 Concentration Measurement 0x042A)
#   * PM10 concentration (PM10 Concentration Measurement 0x042D)
#   * TVOC concentration (Total Volatile Organic Compounds 0x042E)
#   * NO2 concentration (Nitrogen Dioxide Concentration Measurement 0x0413)
# - Converts values to appropriate Matter units (ppm, µg/m³)
# - Calculates overall AirQuality enum based on measurements
# - Inherits all cluster implementations from base class
# - UPDATE_TIME: 5000ms (5 seconds) for responsive air quality monitoring
#
# CONFIGURATION:
# - TYPE: "http_airquality" - Plugin identifier in Matter configuration
# - BRIDGE: true - Marks this as a bridged device
# - Requires endpoint configuration with remote device URL
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_Sensor_Air_Quality,weak

class Matter_Plugin_Bridge_Sensor_Air_Quality : Matter_Plugin_Sensor_Air_Quality
  static var BRIDGE = true
  static var TYPE = "http_airquality"               # name of the plug-in in json
  static var UPDATE_TIME = 5000                     # update every 5s
  static var UPDATE_CMD = "Status 10"               # command to send for updates
end
matter.Plugin_Bridge_Sensor_Air_Quality = Matter_Plugin_Bridge_Sensor_Air_Quality
