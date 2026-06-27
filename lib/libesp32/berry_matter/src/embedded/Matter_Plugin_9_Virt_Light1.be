#
# Matter_Plugin_Virt_Light1.be - implements the behavior for a virtual Light 1 channel (Dimmer)
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
# Matter 1.4.1 Virtual Variant - Dimmable Light
#################################################################################
# INHERITS FROM: Matter_Plugin_Light1 (Matter_Plugin_3_Light1.be)
# VARIANT TYPE: Virtual (Matter Bridge Protocol)
# DEVICE TYPE: Dimmable Light (0x0101) - See base class for full specification
# CLUSTERS: On/Off + Level Control - See Matter_Plugin_3_Light1.be
# TYPE: "v_light1" | VIRTUAL: true
# See base class for complete Matter 1.4.1 cluster specifications
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Virt_Light1,weak

class Matter_Plugin_Virt_Light1 : Matter_Plugin_Light1
  static var TYPE = "v_light1"                      # name of the plug-in in json
  static var DISPLAY_NAME = "v.Light 1 Dimmer"                # display name of the plug-in

  static var SCHEMA = nil                          # no parameter
  static var VIRTUAL = true                         # virtual device
end
matter.Plugin_Virt_Light1 = Matter_Plugin_Virt_Light1
