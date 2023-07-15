#
# Matter_Profiler.be - suppport for Matter profiler framework
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

#@ solidify:Matter_Profiler,weak

#################################################################################
# Matter_Profiler
#
# Used to store all the elements of the reponse to an attribute or command
#################################################################################
class Matter_Profiler
  static var PREALLOCATED = 50
  var millis
  var names
  var active

  def init()
    self.active = false
    self.millis = list()
    self.millis.resize(self.PREALLOCATED)
    self.names = list()
    self.names.resize(self.PREALLOCATED)
  end

  def set_active(v)
    self.active = bool(v)
  end

  def start()
    if !self.active  return end
    self.millis.resize(0)
    self.names.resize(0)
    self.log("start")
  end

  def log(name)
    if !self.active  return end
    self.millis.push(tasmota.millis())
    self.names.push(name)
  end

  def dump(loglevel)
    if !self.active  return end
    self.log("<--end-->")
    tasmota.log("MTR: Profiler dump:", loglevel)
    var origin = self.millis[0]
    var idx = 1
    while idx < size(self.millis)
      tasmota.log(f"MTR:   {self.millis[idx] - origin:4i} '{self.names[idx]}'", loglevel)
      idx += 1
    end
  end

end
matter.Profiler = Matter_Profiler
