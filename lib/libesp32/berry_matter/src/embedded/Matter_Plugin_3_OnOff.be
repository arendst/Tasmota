#
# Matter_Plugin_OnOff.be - implements the behavior for a Relay (OnOff)
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
# Matter 1.4.1 Device Specification
#################################################################################
# Device Type: On/Off Plug-in Unit (0x010A)
# Device Type Revision: 3 (Matter 1.4.1)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0003: Identify (M)
# - 0x0004: Groups (M)
# - 0x0062: Scenes Management (P, M)
# - 0x0006: On/Off (M)
# - 0x0008: Level Control (O)
# - 0x0406: Occupancy Sensing (C, O)
#
# ELEMENT OVERRIDES:
# - Identify: TriggerEffect cmd M
# - Scenes Management: CopyScene cmd P, M
# - On/Off: Lighting feature M
# - Level Control: OnOff feature M, Lighting feature M, CurrentLevel 1-254, MinLevel 1, MaxLevel 254
#
# NOTES:
# - Generic on/off control for relays, switches, and plugs
# - Inherits all functionality from On/Off Light (0x0100)
# - Different device type ID but same cluster requirements
# - Used for non-lighting applications (relays, smart plugs, etc.)
# - For Tasmota: Maps to Power<x> commands and relay control
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_OnOff,weak

class Matter_Plugin_OnOff : Matter_Plugin_Light0
  static var TYPE = "relay"                         # name of the plug-in in json
  static var DISPLAY_NAME = "Relay"                         # display name of the plug-in

  static var TYPES = { 0x010A: 3 }                  # On/Off Plug-in Unit - Matter 1.4.1 Device Library Rev 3

  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none
  # var shadow_onoff                                    # (bool) status of the light power on/off
  # var tasmota_relay_index                             # Relay number in Tasmota (zero based)

end
matter.Plugin_OnOff = Matter_Plugin_OnOff
