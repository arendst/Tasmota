#
# Matter_Plugin_9_Virt_Sensor_Flow.be - implements Virtual Flow Sensor
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
# Matter 1.4.1 Virtual Variant - Matter Bridge Protocol Flow Sensor
#################################################################################
# This is a VIRTUAL variant that inherits from Matter_Plugin_Sensor_Flow.
# It exposes a virtual sensor that can be controlled via Matter bridge protocol.
#
# DEVICE TYPE: Flow Sensor (0x0306)
# See Matter_Plugin_3_Sensor_Flow.be for complete Matter 1.4.1 specifications
# including Flow Measurement cluster (0x0404) details.
#
# VIRTUAL BEHAVIOR:
# - Exposes a virtual flow sensor endpoint
# - Values can be set via Matter commands from controller
# - No physical sensor or HTTP polling required
# - Useful for testing, simulation, or custom integrations
# - Inherits all cluster implementations from base class
#
# CONFIGURATION:
# - TYPE: "v_flow" - Plugin identifier in Matter configuration
# - DISPLAY_NAME: "v.Flow" - Shows as virtual flow in UI
# - VIRTUAL: true - Marks this as a virtual device
# - ARG: "" - No argument needed (no physical sensor to map)
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Virt_Sensor_Flow,weak

class Matter_Plugin_Virt_Sensor_Flow : Matter_Plugin_Sensor_Flow
  static var TYPE = "v_flow"                    # name of the plug-in in json
  static var DISPLAY_NAME = "v.Flow"            # display name of the plug-in
  static var ARG  = ""                              # no arg for virtual device
  static var ARG_HINT = "_Not used_"                # Hint for entering the Argument (inside 'placeholder')
  static var VIRTUAL = true                         # virtual device
end
matter.Plugin_Virt_Sensor_Flow = Matter_Plugin_Virt_Sensor_Flow
