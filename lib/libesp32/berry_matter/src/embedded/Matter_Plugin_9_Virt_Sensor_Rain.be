
#
# Matter_Plugin_9_Virt_Sensor_Rain.be - implements Virtual Rain Sensor
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
# Matter 1.4.1 Virtual Variant - Matter Bridge Protocol Rain Sensor
#################################################################################
# This is a VIRTUAL variant that inherits from Matter_Plugin_Sensor_Rain.
# It exposes a virtual sensor that can be controlled via Matter bridge protocol.
#
# DEVICE TYPE: Rain Sensor (0x0044)
# See Matter_Plugin_3_Sensor_Rain.be for complete Matter 1.4.1 specifications
# including Boolean State cluster (0x0045) details.
#
# VIRTUAL BEHAVIOR:
# - Exposes a virtual rain sensor endpoint
# - Values can be set via Matter commands from controller
# - No physical sensor or HTTP polling required
# - Useful for testing, simulation, or custom integrations
# - Inherits all cluster implementations from base class
#
# CONFIGURATION:
# - TYPE: "v_rain" - Plugin identifier in Matter configuration
# - DISPLAY_NAME: "v.Rain" - Shows as virtual rain in UI
# - VIRTUAL: true - Marks this as a virtual device
# - ARG: "" - No argument needed (no physical sensor to map)
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Virt_Sensor_Rain,weak

class Matter_Plugin_Virt_Sensor_Rain  : Matter_Plugin_Sensor_Rain
  static var TYPE = "v_rain"                        # name of the plug-in in json
  static var DISPLAY_NAME = "v.Rain"                # display name of the plug-in
  static var ARG  = ""                              # no arg for virtual device
  static var ARG_HINT = "_Not used_"                # Hint for entering the Argument (inside 'placeholder')
  static var VIRTUAL = true                         # virtual device
end
matter.Plugin_Virt_Sensor_Rain  = Matter_Plugin_Virt_Sensor_Rain 
