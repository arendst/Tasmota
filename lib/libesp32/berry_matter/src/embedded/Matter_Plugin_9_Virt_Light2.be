#
# Matter_Plugin_Virt_Light2.be - implements the behavior for a virtual Light with 2 channel (CT)
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
# Matter 1.4.1 Virtual Variant - Color Temperature Light
#################################################################################
# INHERITS FROM: Matter_Plugin_Light2 (Matter_Plugin_4_Light2.be)
# VARIANT TYPE: Virtual (Matter Bridge Protocol)
# DEVICE TYPE: Color Temperature Light (0x010C) - See base class
# CLUSTERS: On/Off + Level Control + Color Control (CT) - See Matter_Plugin_4_Light2.be
# TYPE: "v_light2" | VIRTUAL: true
# See base class for complete Matter 1.4.1 cluster specifications
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Virt_Light2,weak

class Matter_Plugin_Virt_Light2 : Matter_Plugin_Light2
  static var TYPE = "v_light2"                      # name of the plug-in in json
  static var DISPLAY_NAME = "v.Light 2 CT"                # display name of the plug-in

  static var SCHEMA = nil                          # no parameter
  static var VIRTUAL = true                         # virtual device
end
matter.Plugin_Virt_Light2 = Matter_Plugin_Virt_Light2
