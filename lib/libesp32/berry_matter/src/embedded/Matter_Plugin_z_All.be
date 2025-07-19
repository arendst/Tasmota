#
# Matter_Plugin_z_All.be - list all plugin classes in a solidified map
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

# this is used to solidify plugins list in Matter_Device

######################################################################
# Register all classes to the `matter` module before solidification
#
# Scans all classes starting with `Plugin_`
# Add to a map from class.TYPE -> class
# and set an attribute `plugins_classes` to the module `matter`
# before final solidification.
######################################################################
def register_native_classes()
  var plugins_classes = {}
  # try to register any class that starts with 'Plugin_'
  import introspect
  import string
  for k: introspect.members(matter)
    var v = introspect.get(matter, k)
    if type(v) == 'class' && string.find(k, "Plugin_") == 0
        var typ = introspect.get(v, 'TYPE')      # make sure we don't crash if TYPE does not exist
        if typ
          plugins_classes[typ] = v
        end
    end
  end
  return plugins_classes
end

plugins_classes = register_native_classes()

matter.plugins_classes = plugins_classes
