#
# Matter_Plugin_Light3.be - implements the behavior for a Light with 3 channels (RGB)
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

#@ solidify:Matter_Plugin_Light3,weak

class Matter_Plugin_Light3 : Matter_Plugin_Light1
  static var TYPE = "light3"                                # name of the plug-in in json
  static var DISPLAY_NAME = "Light 3 RGB"                           # display name of the plug-in
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                                     # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                                     # Identify 1.2 p.16
    # 0x0004: inherited                                     # Groups 1.3 p.21
    # 0x0005: inherited                                     # Scenes 1.4 p.30 - no writable
    # 0x0006: inherited                                     # On/Off 1.5 p.48
    # 0x0008: inherited                                     # Level Control 1.6 p.57
    0x0300: [0,1,7,8,0xF,0x4001,0x400A,0xFFFC,0xFFFD],# Color Control 3.2 p.111
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Hue", "Sat")
  static var TYPES = { 0x010D: 2 }                  # Extended Color Light

  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none
  # var virtual                                       # (bool) is the device pure virtual (i.e. not related to a device implementation by Tasmota)
  # var shadow_onoff                                  # (bool) status of the light power on/off
  # var shadow_bri                                    # (int 0..254) brightness before Gamma correction - as per Matter 255 is not allowed
  var shadow_hue                                    # (int 0..254) hue of color, may need to be extended to 0..360 for value in degrees
  var shadow_sat                                    # (int 0..254) saturation of color

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    super(self).init(device, endpoint, arguments)
    self.shadow_hue = 0
    self.shadow_sat = 0
  end

  #############################################################
  # Update shadow
  #
  def update_shadow()
    super(self).update_shadow()
    if !self.VIRTUAL
      import light
      var light_status = light.get()
      if light_status != nil
        var hue = light_status.find('hue', nil)
        var sat = light_status.find('sat', nil)
        if hue != nil     hue = tasmota.scale_uint(hue, 0, 360, 0, 254)   else hue = self.shadow_hue      end
        if sat != nil     sat = tasmota.scale_uint(sat, 0, 255, 0, 254)   else sat = self.shadow_sat      end
        if hue != self.shadow_hue   self.attribute_updated(0x0300, 0x0000)   self.shadow_hue = hue   end
        if sat != self.shadow_sat   self.attribute_updated(0x0300, 0x0001)   self.shadow_sat = sat   end
      end
    end
  end

  #############################################################
  # set_hue_sat
  #
  # `hue` 0..254 or `nil`
  # `sat` 0..255 or `nil`
  def set_hue_sat(hue_254, sat_254)
    # sanity checks on values
    if hue_254 != nil
      if hue_254 < 0      hue_254 = 0     end
      if hue_254 > 254    hue_254 = 254   end
    end
    if sat_254 != nil
      if sat_254 < 0      sat_254 = 0     end
      if sat_254 > 254    sat_254 = 254   end
    end

    if !self.VIRTUAL
      var hue_360 = (hue_254 != nil) ? tasmota.scale_uint(hue_254, 0, 254, 0, 360) : nil
      var sat_255 = (sat_254 != nil) ? tasmota.scale_uint(sat_254, 0, 254, 0, 255) : nil

      if (hue_360 != nil) && (sat_255 != nil)
        light.set({'hue': hue_360, 'sat': sat_255})
      elif (hue_360 != nil)
        light.set({'hue': hue_360})
      else
        light.set({'sat': sat_255})
      end
      self.update_shadow()
    else
      if hue_254 != nil
        if hue_254 != self.shadow_hue   self.attribute_updated(0x0300, 0x0000)   self.shadow_hue = hue_254   end
      end
      if sat_254 != nil
        if sat_254 != self.shadow_sat   self.attribute_updated(0x0300, 0x0001)   self.shadow_sat = sat_254   end
      end
    end
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute
      
    # ====================================================================================================
    if   cluster == 0x0300              # ========== Color Control 3.2 p.111 ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- CurrentHue / u1 ----------
        return tlv_solo.set(TLV.U1, self.shadow_hue)
      elif attribute == 0x0001          #  ---------- CurrentSaturation / u2 ----------
        return tlv_solo.set(TLV.U1, self.shadow_sat)
      elif attribute == 0x0007          #  ---------- ColorTemperatureMireds / u2 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x0008          #  ---------- ColorMode / u1 ----------
        return tlv_solo.set(TLV.U1, 0)# 0 = CurrentHue and CurrentSaturation
      elif attribute == 0x000F          #  ---------- Options / u1 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x4001          #  ---------- EnhancedColorMode / u1 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x400A          #  ---------- ColorCapabilities / map2 ----------
        return tlv_solo.set(TLV.U1, 0x01)    # HS
      
      # Defined Primaries Information Attribute Set
      elif attribute == 0x0010          #  ---------- NumberOfPrimaries / u1 ----------
        return tlv_solo.set(TLV.U1, 0)

      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0x01)    # HS
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return tlv_solo.set(TLV.U4, 5)    # "new data model format and notation, FeatureMap support"
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
    import light
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var command = ctx.command

    # ====================================================================================================
    if   cluster == 0x0300              # ========== Color Control 3.2 p.111 ==========
      self.update_shadow_lazy()
      if   command == 0x0000            # ---------- MoveToHue ----------
        var hue_in = val.findsubval(0)  # Hue 0..254
        self.set_hue_sat(hue_in, nil)
        ctx.log = "hue:"+str(hue_in)
        self.publish_command('Hue', hue_in)
        return true
      elif command == 0x0001            # ---------- MoveHue ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0002            # ---------- StepHue ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0003            # ---------- MoveToSaturation ----------
        var sat_in = val.findsubval(0)  # Sat 0..254
        self.set_hue_sat(nil, sat_in)
        ctx.log = "sat:"+str(sat_in)
        self.publish_command('Sat', sat_in)
        return true
      elif command == 0x0004            # ---------- MoveSaturation ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0005            # ---------- StepSaturation ----------
        # TODO, we don't really support it
        return true
      elif command == 0x0006            # ---------- MoveToHueAndSaturation ----------
        var hue_in = val.findsubval(0)  # Hue 0..254
        var sat_in = val.findsubval(1)  # Sat 0..254
        self.set_hue_sat(hue_in, sat_in)
        ctx.log = "hue:"+str(hue_in)+" sat:"+str(sat_in)
        self.publish_command('Hue', hue_in, 'Sat', sat_in)
        return true
      elif command == 0x0047            # ---------- StopMoveStep ----------
        # TODO, we don't really support it
        return true
      end

    else
      return super(self).invoke_request(session, val, ctx)
    end

  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload_json)
    var val_hue = int(payload_json.find("Hue"))         # int or nil
    var val_sat = int(payload_json.find("Sat"))         # int or nil
    if (val_hue != nil) || (val_sat != nil)
      self.set_hue_sat(val_hue, val_sat)
    end
    super(self).update_virtual(payload_json)
  end

end
matter.Plugin_Light3 = Matter_Plugin_Light3
