#
# Matter_Plugin_Bridge_OnOff.be - implements the behavior for a Relay via HTTP (OnOff)
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
# Matter 1.4.1 Bridge Variant - On/Off Plug-in Unit via HTTP
#################################################################################
# INHERITS FROM: Matter_Plugin_Bridge_Light0 → Matter_Plugin_Light0
# VARIANT TYPE: Bridge (Remote HTTP Device)
#
# DEVICE TYPE: On/Off Plug-in Unit (0x010A) - See Matter_Plugin_3_OnOff.be
# CLUSTERS: Same as On/Off Light - See base class documentation
#
# BRIDGE MODE SPECIFICS:
# - TYPE: "http_relay" (JSON configuration identifier)
# - DISPLAY_NAME: "Relay" (user-friendly name)
# - Inherits HTTP bridge functionality from Matter_Plugin_Bridge_Light0
# - Communicates with remote Tasmota relay via HTTP API
# - Custom web display showing relay number and state
#
# CONFIGURATION:
# - Requires "url" parameter for remote device address
# - Requires "relay" parameter for relay number (1-based)
# - Example: {"type":"http_relay","url":"192.168.1.100","relay":1}
#
# NOTES:
# - Functionally identical to Bridge Light0 but with different device type
# - Device type 0x010A (On/Off Plug-in Unit) vs 0x0100 (On/Off Light)
# - See Matter_Plugin_3_OnOff.be for device type specification
# - See Matter_Plugin_2_Light0.be for cluster specifications
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_OnOff,weak

class Matter_Plugin_Bridge_OnOff : Matter_Plugin_Bridge_Light0
  static var TYPE = "http_relay"                    # name of the plug-in in json
  static var DISPLAY_NAME = "Relay"               # display name of the plug-in

  static var SCHEMA = "relay|"                      # arg name
                      "l:Relay number|"             # label (display name)
                      "t:i|"                        # type: int
                      "h:Relay<x> number"           # hint
  static var TYPES = { 0x010A: 2 }                  # On/Off Plug-in Unit

  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    import webserver
    self.web_values_prefix()        # display '| ' and name if present
    webserver.content_send(format("Relay %i %s", self.tasmota_relay_index, self.web_value_onoff(self.shadow_onoff)))
  end

end
matter.Plugin_Bridge_OnOff = Matter_Plugin_Bridge_OnOff
