#
# Matter_Plugin_Device.be - implements the behavior for a standard Device
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

#@ solidify:Matter_Plugin_Device,weak

class Matter_Plugin_Device : Matter_Plugin
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    0x0039: [3,5,0x0A,0x0F,0x11,0x12],              # Bridged Device Basic Information 9.13 p.485
    0x0003: [0,1,0xFFFC,0xFFFD],                    # Identify 1.2 p.16
    0x0004: [0,0xFFFC,0xFFFD],                      # Groups 1.3 p.21
    0x0005: [0,1,2,3,4,5,0xFFFC,0xFFFD],            # Scenes 1.4 p.30 - no writable
  })
  static var TYPES = { 0x0013: 1 }                  # fake type
  static var NON_BRIDGE_VENDOR = [ 0x1217, 0x1381 ] # Fabric VendorID not supporting Bridge mode
  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none

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
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0)    # no features
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return tlv_solo.set(TLV.U4, 4)    # "new data model format and notation"
      end

    # ====================================================================================================
    elif cluster == 0x0004              # ========== Groups 1.3 p.21 ==========
      if   attribute == 0x0000          #  ----------  ----------
        return nil                      # TODO
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0)#
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return tlv_solo.set(TLV.U4, 4)# "new data model format and notation"
      end

    # ====================================================================================================
    elif cluster == 0x0005              # ========== Scenes 1.4 p.30 - no writable ==========
      if   attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0)    # 0 = no Level Control for Lighting
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return tlv_solo.set(TLV.U4, 4)    # 0 = no Level Control for Lighting
      end

    # ====================================================================================================
    elif cluster == 0x001D              # ========== Descriptor Cluster 9.5 p.453 ==========

      if   attribute == 0x0000          # ---------- DeviceTypeList / list[DeviceTypeStruct] ----------
        # for device sub-classes, automatically add the Bridge Node type `0x0013: 1`
        # unless the fabric doesn't support bridge mode (currently Alexa)
        var dtl = TLV.Matter_TLV_array()
        var types = self.TYPES
        for dt: types.keys()
          var d1 = dtl.add_struct()
          d1.add_TLV(0, TLV.U2, dt)     # DeviceType
          d1.add_TLV(1, TLV.U2, types[dt])      # Revision
        end
        # if fabric is not Alexa
        if (self.NON_BRIDGE_VENDOR.find(session.get_admin_vendor()) == nil) && (!self.device.disable_bridge_mode)
          var d1 = dtl.add_struct()
          d1.add_TLV(0, TLV.U2, 0x0013)     # DeviceType
          d1.add_TLV(1, TLV.U2, 1)      # Revision
        end
        return dtl
      else
        return super(self).read_attribute(session, ctx, tlv_solo)
      end

    # ====================================================================================================
    elif cluster == 0x0039              # ========== Bridged Device Basic Information 9.13 p.485 ==========
      import string

      if   attribute == 0x0003          #  ---------- ProductName / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.cmd("DeviceName", true)['DeviceName'])
      elif attribute == 0x0005          #  ---------- NodeLabel / string ----------
        return tlv_solo.set(TLV.UTF1, self.get_name())
      elif attribute == 0x000A          #  ---------- SoftwareVersionString / string ----------
        var version_full = tasmota.cmd("Status 2", true)['StatusFWR']['Version']
        var version_end = string.find(version_full, '(')
        if version_end > 0    version_full = version_full[0..version_end - 1]   end
        return tlv_solo.set(TLV.UTF1, version_full)
      elif attribute == 0x000F          #  ---------- SerialNumber / string ----------
        return tlv_solo.set(TLV.UTF1, tasmota.wifi().find("mac", ""))
      elif attribute == 0x0011          #  ---------- Reachable / bool ----------
        return tlv_solo.set(TLV.BOOL, 1)     # by default we are reachable
      elif attribute == 0x0012          #  ---------- UniqueID / string 32 max ----------
        return tlv_solo.set(TLV.UTF1, tasmota.wifi().find("mac", ""))
      else
        return super(self).read_attribute(session, ctx, tlv_solo)
      end

    else
      return super(self).read_attribute(session, ctx, tlv_solo)
    end
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
    # ====================================================================================================
    elif cluster == 0x0004              # ========== Groups 1.3 p.21 ==========
      # TODO
      return true

    # ====================================================================================================
    elif cluster == 0x0005              # ========== Scenes 1.4 p.30 ==========
      # TODO
      return true
      
    else
      return super(self).invoke_request(session, val, ctx)
    end
  end

  #############################################################
  # append_state_json
  #
  # Output the current state in JSON.
  # The JSON is build via introspection to see what attributes
  # exist and need to be output
  # New values need to be appended with `,"key":value` (including prefix comma)
  def append_state_json()
    import introspect
    var ret = ""

    # ret: string
    # attribute: attrbute name
    # key: in json
    def _stats_json_inner(attribute, key)
      import introspect
      import json
      var val
      if (val := introspect.get(self, attribute)) != nil
        if type(val) == 'bool'    val = int(val)  end         # transform bool into 1/0
        ret += f',"{key}":{json.dump(val)}'
      end
    end

    # lights
    # print(f'{self=} {type(self)} {introspect.members(self)=}')
    _stats_json_inner("shadow_onoff",      "Power")
    _stats_json_inner("shadow_bri",        "Bri")
    _stats_json_inner("shadow_ct",         "CT")
    _stats_json_inner("shadow_hue",        "Hue")
    _stats_json_inner("shadow_sat",        "Sat")
    # shutters
    _stats_json_inner("shadow_shutter_pos",    "ShutterPos")
    _stats_json_inner("shadow_shutter_target", "ShutterTarget")
    _stats_json_inner("shadow_shutter_tilt",   "ShutterTilt")

    # sensors
    _stats_json_inner("shadow_contact",    "Contact")
    _stats_json_inner("shadow_occupancy",  "Occupancy")
    # print(ret)
    return ret
  end

end
matter.Plugin_Device = Matter_Plugin_Device
