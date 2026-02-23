#
# Matter_Plugin_Bridge_Light3.be - implements the behavior for a remote generic Lighting (RGB) via HTTP
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
# Matter 1.4.1 Bridge Variant - Extended Color Light via HTTP
#################################################################################
# INHERITS FROM: Matter_Plugin_Light3 (Matter_Plugin_4_Light3.be)
# VARIANT TYPE: Bridge (Remote HTTP Device)
# DEVICE TYPE: Extended Color Light (0x010D) - See base class
# CLUSTERS: On/Off + Level Control + Color Control (HS) - See Matter_Plugin_4_Light3.be
# TYPE: "http_light3" | UPDATE_TIME: 3000ms
# See base class for complete Matter 1.4.1 cluster specifications
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_Light3,weak

class Matter_Plugin_Bridge_Light3 : Matter_Plugin_Light3
  static var BRIDGE = true
  static var TYPE = "http_light3"                   # name of the plug-in in json
  # static var DISPLAY_NAME = "Light 3 RGB"         # display name of the plug-in

  static var SCHEMA = "relay|"                      # arg name
                      "l:Relay number|"             # label (display name)
                      "t:i|"                        # type: int
                      "h:Relay<x> number"           # hint
  static var UPDATE_TIME = 3000                     # update every 3s
end
matter.Plugin_Bridge_Light3 = Matter_Plugin_Bridge_Light3
