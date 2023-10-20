#
# Matter_Plugin_Light0.be - implements the behavior for a generic Lighting (OnOff only)
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

#@ solidify:Matter_Plugin_Light0,weak

class Matter_Plugin_Light0 : Matter_Plugin_OnOff
  static var TYPE = "light0"                        # name of the plug-in in json
  static var DISPLAY_NAME = "Light 0 On"                    # display name of the plug-in
  static var TYPES = { 0x0100: 2 }                  # OnOff Light, but not actually used because Relay is managed by OnOff

  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none
  # var shadow_onoff                                  # (bool) status of the light power on/off

end
matter.Plugin_Light0 = Matter_Plugin_Light0
