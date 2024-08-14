#
# Matter_Plugin_Bridge_Sensor_Contact.be - implements Air Quality Sensor via HTTP to Tasmota
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

#@ solidify:Matter_Plugin_Bridge_Sensor_Air_Quality,weak

class Matter_Plugin_Bridge_Sensor_Air_Quality : Matter_Plugin_Sensor_Air_Quality
  static var BRIDGE = true
  static var TYPE = "http_airquality"               # name of the plug-in in json
  static var UPDATE_TIME = 5000                     # update every 5s
  static var UPDATE_CMD = "Status 10"               # command to send for updates
end
matter.Plugin_Bridge_Sensor_Air_Quality = Matter_Plugin_Bridge_Sensor_Air_Quality
