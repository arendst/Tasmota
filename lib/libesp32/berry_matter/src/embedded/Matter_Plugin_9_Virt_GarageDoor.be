#
# Matter_Plugin_9_Virt_GarageDoor.be - virtual Matter Garage Door
#
# Copyright (C) 2026  Ludovic BOUÉ
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
# Matter 1.6.1 Virtual Closure - Garage Door
#################################################################################
# INHERITS FROM: Matter_Plugin_GarageDoor
# TYPE: "v_garage" | VIRTUAL: true
#
# Matter MoveTo commands complete immediately in the local virtual state and
# publish MtrReceived with ShutterPos, ShutterTarget and ShutterDirection.
#
# External state can be injected with:
#   MtrUpdate {"Name":"Garage","ShutterPos":100,
#              "ShutterTarget":100,"ShutterDirection":0}
#   MtrUpdate {"Name":"Garage","ShutterPos":50,
#              "ShutterTarget":0,"ShutterDirection":-1}
#
# Values follow the Tasmota shutter convention:
#   ShutterPos/Target: 0 = fully closed, 100 = fully open
#   ShutterDirection: -1 = closing, 0 = stopped, 1 = opening
#################################################################################

import matter

#@ solidify:Matter_Plugin_Virt_GarageDoor,weak

class Matter_Plugin_Virt_GarageDoor : Matter_Plugin_GarageDoor
  static var TYPE = "v_garage"
  static var DISPLAY_NAME = "v.Garage Door"
  static var SCHEMA = nil
  static var VIRTUAL = true
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "ShutterPos", "ShutterTarget", "ShutterDirection")
end
matter.Plugin_Virt_GarageDoor = Matter_Plugin_Virt_GarageDoor
