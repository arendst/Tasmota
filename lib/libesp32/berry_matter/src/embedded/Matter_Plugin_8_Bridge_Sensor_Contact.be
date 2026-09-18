#
# Matter_Plugin_Bridge_Sensor_Contact.be - implements Contact Sensor via HTTP to Tasmota
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
# Matter 1.4.1 Bridge Variant - HTTP Remote Contact Sensor
#################################################################################
# This is a BRIDGE variant that inherits from Matter_Plugin_Sensor_Contact.
# It communicates with a remote Tasmota device via HTTP to read contact state.
#
# DEVICE TYPE: Contact Sensor (0x0015)
# See Matter_Plugin_3_Sensor_Contact.be for complete Matter 1.4.1 specifications
# including Boolean State cluster (0x0045) details.
#
# BRIDGE BEHAVIOR:
# - Polls remote Tasmota device via HTTP using UPDATE_CMD ("Status 10")
# - Parses JSON response to extract contact state from Switch<x>
# - Maps Tasmota switch state to Matter StateValue (true=contact, false=no contact)
# - Inherits all cluster implementations from base class
# - UPDATE_TIME: 5000ms (5 seconds) for responsive contact detection
#
# CONFIGURATION:
# - TYPE: "http_contact" - Plugin identifier in Matter configuration
# - BRIDGE: true - Marks this as a bridged device
# - Requires endpoint configuration with remote device URL
# - ARG: Switch number on remote device (e.g., "1" for Switch1)
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_Sensor_Contact,weak

class Matter_Plugin_Bridge_Sensor_Contact : Matter_Plugin_Sensor_Contact
  static var BRIDGE = true
  static var TYPE = "http_contact"                  # name of the plug-in in json
  # static var DISPLAY_NAME = "Contact"               # display name of the plug-in
  # static var ARG  = "switch"                        # additional argument name (or empty if none)
  # static var ARG_HINT = "Switch<x> number"
  static var UPDATE_TIME = 5000                     # update every 5s
  static var UPDATE_CMD = "Status 10"               # command to send for updates
end
matter.Plugin_Bridge_Sensor_Contact = Matter_Plugin_Bridge_Sensor_Contact
