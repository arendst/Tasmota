#
# Matter_IM_Path.be - suppport for Matter simple Path object
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

#@ solidify:Matter_Path,weak

#################################################################################
# Matter_Path
#
# Used to store all the elements of the reponse to an attribute or command
#################################################################################
class Matter_Path
  var endpoint                # endpoint or `nil` if expansion
  var cluster                 # cluster or `nil` if expansion
  var attribute               # attribute or `nil` if expansion
  var command                 # command
  var status                  # status to be returned (matter.SUCCESS or matter.<ERROR>)
  var log                     # any string that needs to be logged (used to show significant parameters for commands)

  def tostring()
    try
      var s = ""
      s += (self.endpoint  != nil ? format("[%02X]", self.endpoint) : "[**]")
      s += (self.cluster   != nil ? format("%04X/", self.cluster) : "****/")
      s += (self.attribute != nil ? format("%04X", self.attribute) : "")
      s += (self.command   != nil ? format("%04X", self.command) : "")
      if self.attribute == nil && self.command == nil     s += "****" end
      return s
    except .. as e, m
      return "Exception> " + str(e) + ", " + str(m)
    end
  end

end
matter.Path = Matter_Path
