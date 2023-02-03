#
# Matter_Plugin_Relay.be - implements the behavior for a Relay (OnOff)
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

# Matter plug-in for core behavior

# dummy declaration for solidification
class Matter_Plugin end

#@ solidify:Matter_Plugin_Relay,weak

class Matter_Plugin_Relay : Matter_Plugin
  #############################################################
  # Constructor
  def init(device)
    super(self).init(device)
    self.endpoints = [ 1 ]
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(msg, endpoint, cluster, attribute)
    # no match found, return that the attribute is unsupported
  end

  #############################################################
  # Invoke a command
  #
  # returns a TLV object if successful, contains the response
  #   or an `int` to indicate a status
  def invoke_request(msg, val, ctx)
    # no match found, return that the command is unsupported
  end
end
matter.Plugin_core = Matter_Plugin_core
  