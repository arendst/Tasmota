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
  static var ENDPOINTS = [ 1 ]
  static var CLUSTERS  = {
    0x001D: [0,1,2,3],
    0x0003: [],
    0x0004: [],
    0x0005: [],
    0x0006: [0],
    0x0008: [],
#    0x0406: []
  }
  static var TYPES = [ 0x0100 ]     # On/Off Light

  #############################################################
  # Constructor
  def init(device)
    super(self).init(device)
    self.endpoints = self.ENDPOINTS
    self.clusters = self.CLUSTERS
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(msg, ctx)
    import string
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    if   cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========

      if   attribute == 0x0000          # ---------- DeviceTypeList / list[DeviceTypeStruct] ----------
        var dtl = TLV.Matter_TLV_array()
        var d1 = dtl.add_struct()
        d1.add_TLV(0, TLV.U2, self.TYPES[0])      # DeviceType
        d1.add_TLV(1, TLV.U2, 1)      # Revision
        return dtl
      elif attribute == 0x0001          # ---------- ServerList / list[cluster-id] ----------
        var sl = TLV.Matter_TLV_array()
        for cl: self.get_cluster_list()
          sl.add_TLV(nil, TLV.U4, cl)
        end
        return sl
      elif attribute == 0x0002          # ---------- ClientList / list[cluster-id] ----------
        var cl = TLV.Matter_TLV_array()
        cl.add_TLV(nil, TLV.U2, 0x0006)
        return cl
      elif attribute == 0x0003          # ---------- PartsList / list[endpoint-no]----------
        var pl = TLV.Matter_TLV_array()
        return pl
      end
    end
    # no match found, return that the attribute is unsupported  end
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
  