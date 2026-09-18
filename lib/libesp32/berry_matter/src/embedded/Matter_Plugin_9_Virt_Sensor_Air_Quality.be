#
# Matter_Plugin_9_Virt_Sensor_Air_Quality.be - implements Virtual Air Quality Sensor
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
# Matter 1.4.1 Virtual Variant - Matter Bridge Protocol Air Quality Sensor
#################################################################################
# This is a VIRTUAL variant that inherits from Matter_Plugin_Sensor_Air_Quality.
# It exposes a virtual sensor that can be controlled via Matter bridge protocol.
#
# DEVICE TYPE: Air Quality Sensor (0x002C)
# See Matter_Plugin_2_Sensor_Air_Quality.be for complete Matter 1.4.1 specifications
# including Air Quality cluster (0x005B) and Concentration Measurement clusters.
#
# VIRTUAL BEHAVIOR:
# - Exposes a virtual air quality sensor endpoint with multiple measurements:
#   * CO2 concentration (Carbon Dioxide Concentration Measurement 0x040D)
#   * PM1 concentration (PM1 Concentration Measurement 0x042C)
#   * PM2.5 concentration (PM2.5 Concentration Measurement 0x042A)
#   * PM10 concentration (PM10 Concentration Measurement 0x042D)
#   * TVOC concentration (Total Volatile Organic Compounds 0x042E)
#   * NO2 concentration (Nitrogen Dioxide Concentration Measurement 0x0413)
# - Values can be set via Matter commands from controller
# - No physical sensor or HTTP polling required
# - Useful for testing, simulation, or custom integrations
# - Inherits all cluster implementations from base class
#
# CONFIGURATION:
# - TYPE: "v_airquality" - Plugin identifier in Matter configuration
# - DISPLAY_NAME: "v.Air Quality" - Shows as virtual air quality in UI
# - VIRTUAL: true - Marks this as a virtual device
# - ARG: "" - No argument needed (no physical sensor to map)
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Virt_Sensor_Air_Quality,weak

class Matter_Plugin_Virt_Sensor_Air_Quality : Matter_Plugin_Sensor_Air_Quality
  static var TYPE = "v_airquality"                  # name of the plug-in in json
  static var DISPLAY_NAME = "v.Air Quality"         # display name of the plug-in

  static var SCHEMA = nil                          # no parameter
  static var VIRTUAL = true                         # virtual device
end
matter.Plugin_Virt_Sensor_Air_Quality = Matter_Plugin_Virt_Sensor_Air_Quality
