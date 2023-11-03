#
# Matter_Plugin_Aggregator.be - implements the Aggregator endpoint
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

# Matter plug-in for root behavior

#@ solidify:Matter_Plugin_Aggregator,weak

class Matter_Plugin_Aggregator : Matter_Plugin
  static var TYPE = "aggregator"      # name of the plug-in in json
  static var DISPLAY_NAME = "Aggregator"      # display name of the plug-in
#   static var CLUSTERS  = {
#     # 0x001D: inherited               # Descriptor Cluster 9.5 p.453
#   }
  static var TYPES = { 0x000E: 1 }    # Aggregator

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    if   cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========

      # overwrite PartsList
      if   attribute == 0x0003          # ---------- PartsList / list[endpoint-no]----------
        var pl = TLV.Matter_TLV_array()
        var eps = self.device.get_active_endpoints(true)
        for ep: eps
          if ep < 0xFF00
            pl.add_TLV(nil, TLV.U2, ep)     # add each endpoint
          end
        end
        return pl
      else
        return super(self).read_attribute(session, ctx, tlv_solo)
      end

    else
      return super(self).read_attribute(session, ctx, tlv_solo)

    end
    # no match found, return that the attribute is unsupported
  end

end
matter.Plugin_Aggregator = Matter_Plugin_Aggregator
  