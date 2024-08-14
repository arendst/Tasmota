#
# Matter_Plugin_1_Aggregator.be - implements the Aggregator endpoint
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
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited               # Descriptor Cluster 9.5 p.453
    0x0003: [0,1,],                   # Identify 1.2 p.16
  })
  static var TYPES = { 0x000E: 1 }    # Aggregator

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0003              # ========== Identify 1.2 p.16 ==========
      if   attribute == 0x0000          #  ---------- IdentifyTime / u2 ----------
        return tlv_solo.set(TLV.U2, 0)      # no identification in progress
      elif attribute == 0x0001          #  ---------- IdentifyType / enum8 ----------
        return tlv_solo.set(TLV.U1, 0)      # IdentifyType = 0x00 None
      end

    elif cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========
      if attribute == 0x0002          # ---------- ClientList / list[cluster-id] ----------
        var pl = TLV.Matter_TLV_array()
        # from connectedhome reference implementation
        pl.add_TLV(nil, TLV.U2, 0x001E)     # Binding
        return pl        
      # overwrite PartsList
      elif attribute == 0x0003          # ---------- PartsList / list[endpoint-no]----------
        var pl = TLV.Matter_TLV_array()
        var eps = self.device.get_active_endpoints(true)
        for ep: eps
          if ep != matter.AGGREGATOR_ENDPOINT
            pl.add_TLV(nil, TLV.U2, ep)     # add each endpoint
          end
        end
        return pl
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # Invoke a command
  #
  # returns a TLV object if successful, contains the response
  #   or an `int` to indicate a status
  def invoke_request(session, val, ctx)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var command = ctx.command

    # ====================================================================================================
    if   cluster == 0x0003              # ========== Identify 1.2 p.16 ==========

      if   command == 0x0000            # ---------- Identify ----------
        # ignore
        return true
      elif command == 0x0001            # ---------- IdentifyQuery ----------
        # create IdentifyQueryResponse
        # ID=1
        #  0=Certificate (octstr)
        var iqr = TLV.Matter_TLV_struct()
        iqr.add_TLV(0, TLV.U2, 0)       # Timeout
        ctx.command = 0x00              # IdentifyQueryResponse
        return iqr
      elif command == 0x0040            # ---------- TriggerEffect ----------
        # ignore
        return true
      end
      
    else
      return super(self).invoke_request(session, val, ctx)
    end
  end

end
matter.Plugin_Aggregator = Matter_Plugin_Aggregator
