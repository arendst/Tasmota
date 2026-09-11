#
# Matter_Plugin_Bridge_OnOff_Power.be - implements the behavior for a remote
# Relay (OnOff) with Electrical Power Measurement via HTTP
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
# Matter 1.4.1 Bridge Variant - On/Off Plug-in Unit with Power Measurement via HTTP
#################################################################################
# INHERITS FROM: Matter_Plugin_OnOff_Power (Matter_Plugin_3_OnOff_Power.be)
# VARIANT TYPE: Bridge (Remote HTTP Device)
#
# DEVICE TYPE: On/Off Plug-in Unit (0x010A) - See base class for specification
# CLUSTERS: On/Off + Electrical Power Measurement (0x0090) - See base class
#
# BRIDGE MODE SPECIFICS:
# - BRIDGE flag: true (enables HTTP remote device mode)
# - TYPE: "http_relay_power" (JSON configuration identifier)
# - DISPLAY_NAME: "Relay + Power" (user-friendly name)
# - Polls the remote Tasmota device with `Status 10` (UPDATE_CMD, inherited)
#   and reads its (non-indexed) `ENERGY` object the same way the local
#   plugin reads `tasmota.read_sensors()` - see `parse_status()` in the
#   base class for the shared parsing logic.
#
# CONFIGURATION:
# - Requires "url" parameter for remote device address
# - Requires "relay" parameter for relay number (1-based)
# - Example: {"type":"http_relay_power","url":"192.168.1.100","relay":1}
#
# NOTES:
# - Functionally identical to Matter_Plugin_OnOff_Power but polls a remote
#   Tasmota device via HTTP instead of reading local sensors.
# - Single measurement channel only, same limitation as the local plugin.
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_OnOff_Power,weak

class Matter_Plugin_Bridge_OnOff_Power : Matter_Plugin_OnOff_Power
  static var BRIDGE = true                          # flag as bridged device
  static var TYPE = "http_relay_power"               # name of the plug-in in json
  static var DISPLAY_NAME = "Relay + Power"          # display name of the plug-in
end
matter.Plugin_Bridge_OnOff_Power = Matter_Plugin_Bridge_OnOff_Power
