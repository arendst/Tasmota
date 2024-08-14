#
# Matter_Plugin_Bridge_Sensor_Temp.be - implements base class for a Temperature Sensor via HTTP to Tasmota
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

#@ solidify:Matter_Plugin_Bridge_Sensor_Temp,weak

class Matter_Plugin_Bridge_Sensor_Temp : Matter_Plugin_Sensor_Temp
  static var BRIDGE = true                          # flag as bridged device
  static var TYPE = "http_temperature"              # name of the plug-in in json
end
matter.Plugin_Bridge_Sensor_Temp = Matter_Plugin_Bridge_Sensor_Temp
