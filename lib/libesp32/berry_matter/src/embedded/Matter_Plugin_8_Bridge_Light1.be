#
# Matter_Plugin_Bridge_Light1.be - implements the behavior for a remote generic Lighting (Dimmer) via HTTP
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

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Bridge_Light1,weak

class Matter_Plugin_Bridge_Light1 : Matter_Plugin_Light1
  static var BRIDGE = true
  static var TYPE = "http_light1"                   # name of the plug-in in json
  # static var DISPLAY_NAME = "Light 1 Dimmer"      # display name of the plug-in
  static var ARG  = "relay"                         # additional argument name (or empty if none)
  static var ARG_HINT = "Relay<x> number"
  static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var UPDATE_TIME = 3000                     # update every 3s
end
matter.Plugin_Bridge_Light1 = Matter_Plugin_Bridge_Light1
