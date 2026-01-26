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

#################################################################################
# Matter 1.4.1 Device Specification
#################################################################################
# Device Type: Extended Color Light (0x010D)
# Device Type Revision: 4 (Matter 1.4.1)
# Class: Simple | Scope: Endpoint
# Superset: Color Temperature Light (0x010C)
#
# CLUSTERS (Server):
# - 0x0003: Identify (M)
# - 0x0004: Groups (M)
# - 0x0062: Scenes Management (P, M)
# - 0x0006: On/Off (M)
# - 0x0008: Level Control (M)
# - 0x0300: Color Control (M)
# - 0x0406: Occupancy Sensing (C, O)
#
# ELEMENT OVERRIDES:
# - Identify: TriggerEffect cmd M
# - Scenes Management: CopyScene cmd P, M
# - On/Off: Lighting feature M
# - Level Control: OnOff feature M, Lighting feature M, CurrentLevel 1-254, MinLevel 1, MaxLevel 254
# - Color Control: HueSaturation O, EnhancedHue O, ColorLoop O, XY M, ColorTemperature M, RemainingTime M
#
# IMPORTANT (Matter 1.4.1):
# Extended Color Light MUST support BOTH HueSaturation AND ColorTemperature modes.
# This is a change from earlier versions where one mode was sufficient.
#################################################################################

#################################################################################
# Matter 1.4.1 Color Control Cluster (0x0300) - HueSaturation Mode
#################################################################################
# Cluster Revision: 7 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# FEATURES (for HS mode):
# - Bit 0 (HS): HueSaturation (M for this device)
# - Bit 4 (CT): ColorTemperature (M for Extended Color Light in Matter 1.4.1)
#
# ATTRIBUTES (HueSaturation Mode):
# ID     | Name                          | Type   | Constraint        | Quality | Default | Access | Conf
# -------|-------------------------------|--------|-------------------|---------|---------|--------|-----
# 0x0000 | CurrentHue                    | uint8  | 0-254             | SN      | 0       | RW VO  | HS
# 0x0001 | CurrentSaturation             | uint8  | 0-254             | SN      | 0       | RW VO  | HS
# 0x0007 | ColorTemperatureMireds        | uint16 | 0,PhysMin-PhysMax | SN      | 0x00FA  | RW VO  | CT
# 0x0008 | ColorMode                     | enum8  | desc              | S       | 0       | R V    | M
# 0x000F | Options                       | map8   | all               |         | 0       | RW VO  | M
# 0x0010 | NumberOfPrimaries             | uint8  | 0-6               | FX      | null    | R V    | O
# 0x4001 | EnhancedColorMode             | enum8  | desc              | S       | 0       | R V    | M
# 0x400A | ColorCapabilities             | map16  | all               | F       | 0       | R V    | M
# 0x400B | ColorTempPhysicalMinMireds    | uint16 | 0-0xFEFF          | F       | 0       | R V    | CT
# 0x400C | ColorTempPhysicalMaxMireds    | uint16 | 0-0xFEFF          | F       | 0xFEFF  | R V    | CT
# 0xFFFC | FeatureMap                    | map32  | all               | F       | 0       | R V    | M
#
# ColorMode/EnhancedColorMode values:
# - 0: CurrentHue and CurrentSaturation (used by this device for HS mode)
# - 1: CurrentX and CurrentY
# - 2: ColorTemperatureMireds
#
# ColorCapabilities bitmap:
# - Bit 0: HueSaturation (0x01 - used by this device)
# - Bit 1: EnhancedHue
# - Bit 2: ColorLoop
# - Bit 3: XY
# - Bit 4: ColorTemperature (0x10 - also required for Extended Color Light)
#
# COMMANDS (HueSaturation Mode):
# ID   | Name                    | Dir  | Response | Access | Conf
# -----|-------------------------|------|----------|--------|-----
# 0x0000 | MoveToHue              | C→S  | Y        | O      | HS
# 0x0001 | MoveHue                | C→S  | Y        | O      | HS
# 0x0002 | StepHue                | C→S  | Y        | O      | HS
# 0x0003 | MoveToSaturation       | C→S  | Y        | O      | HS
# 0x0004 | MoveSaturation         | C→S  | Y        | O      | HS
# 0x0005 | StepSaturation         | C→S  | Y        | O      | HS
# 0x0006 | MoveToHueAndSaturation | C→S  | Y        | O      | HS
# 0x0047 | StopMoveStep           | C→S  | Y        | O      | M
#
# MoveToHue: {Hue:uint8(0-254), Direction:enum8, TransitionTime:uint16, OptionsMask:map8, OptionsOverride:map8}
# MoveToSaturation: {Saturation:uint8(0-254), TransitionTime:uint16, OptionsMask:map8, OptionsOverride:map8}
# MoveToHueAndSaturation: {Hue:uint8(0-254), Saturation:uint8(0-254), TransitionTime:uint16, OptionsMask:map8, OptionsOverride:map8}
#
# NOTES:
# - Hue: 0-254 maps to 0-360 degrees (254 = 360°, not 255)
# - Saturation: 0-254 maps to 0-100% (254 = 100%, not 255)
# - Value 255 is reserved and should not be used
# - Extended Color Light must support both HS and CT modes (Matter 1.4.1 requirement)
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Light3,weak

class Matter_Plugin_Light3 : Matter_Plugin_Light1
  static var TYPE = "light3"                                # name of the plug-in in json
  static var DISPLAY_NAME = "Light 3 RGB"                    # display name of the plug-in
  static var ARG  = ""                                      # no arg for native light
  static var ARG_HINT = "_Not used_"                        # Hint for entering the Argument (inside 'placeholder')
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                                     # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                                     # Identify 1.2 p.16
    # 0x0004: inherited                                     # Groups 1.3 p.21
    # 0x0062: inherited                                     # Scenes Management 1.4 (PROVISIONAL) - replaces 0x0005
    # 0x0006: inherited                                     # On/Off 1.5 p.48
    # 0x0008: inherited                                     # Level Control 1.6 p.57
    0x0300: [0,1,8,0xF,0x4001,0x400A],                      # Color Control 3.2 p.111 - HS mode only
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Hue", "Sat")
  static var TYPES = { 0x010D: 4 }                  # Extended Color Light - Matter 1.4.1 Device Library Rev 4

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
    if !self.VIRTUAL && !self.BRIDGE
      import light
      super(self).update_shadow()
      var light_status = light.get(self.light_index)
      if light_status != nil
        var hue = light_status.find('hue', nil)
        var sat = light_status.find('sat', nil)
        if hue != nil     hue = tasmota.scale_uint(hue, 0, 360, 0, 254)   else hue = self.shadow_hue      end
        if sat != nil     sat = tasmota.scale_uint(sat, 0, 255, 0, 254)   else sat = self.shadow_sat      end
        if hue != self.shadow_hue   self.attribute_updated(0x0300, 0x0000)   self.shadow_hue = hue   end
        if sat != self.shadow_sat   self.attribute_updated(0x0300, 0x0001)   self.shadow_sat = sat   end
      end
    else
      super(self).update_shadow()
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

    if self.BRIDGE
      if hue_254 != nil
        var hue_360 = tasmota.scale_uint(hue_254, 0, 254, 0, 360)
        var ret = self.call_remote_sync("HSBColor1", hue_360)
        if ret != nil
          self.parse_status(ret, 11)        # update shadow from return value
        end
      end
      if sat_254 != nil
        var sat_100 = tasmota.scale_uint(sat_254, 0, 254, 0, 100)
        var ret = self.call_remote_sync("HSBColor2", sat_100)
        if ret != nil
          self.parse_status(ret, 11)        # update shadow from return value
        end
      end
    elif self.VIRTUAL
      if hue_254 != nil
        if hue_254 != self.shadow_hue   self.attribute_updated(0x0300, 0x0000)   self.shadow_hue = hue_254   end
      end
      if sat_254 != nil
        if sat_254 != self.shadow_sat   self.attribute_updated(0x0300, 0x0001)   self.shadow_sat = sat_254   end
      end
    else
      var hue_360 = (hue_254 != nil) ? tasmota.scale_uint(hue_254, 0, 254, 0, 360) : nil
      var sat_255 = (sat_254 != nil) ? tasmota.scale_uint(sat_254, 0, 254, 0, 255) : nil

      if (hue_360 != nil) && (sat_255 != nil)
        light.set({'hue': hue_360, 'sat': sat_255}, self.light_index)
      elif (hue_360 != nil)
        light.set({'hue': hue_360}, self.light_index)
      else
        light.set({'sat': sat_255}, self.light_index)
      end
      self.update_shadow()
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
        return tlv_solo.set_or_nil(TLV.U1, self.shadow_hue)
      elif attribute == 0x0001          #  ---------- CurrentSaturation / u2 ----------
        return tlv_solo.set_or_nil(TLV.U1, self.shadow_sat)
      elif attribute == 0x0008          #  ---------- ColorMode / u1 ----------
        return tlv_solo.set(TLV.U1, 0)# 0 = CurrentHue and CurrentSaturation
      elif attribute == 0x000F          #  ---------- Options / u1 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x4001          #  ---------- EnhancedColorMode / u1 ----------
        return tlv_solo.set(TLV.U1, 0)
      elif attribute == 0x400A          #  ---------- ColorCapabilities / map2 ----------
        return tlv_solo.set(TLV.U2, 0x01)    # HS
      
      # Defined Primaries Information Attribute Set
      elif attribute == 0x0010          #  ---------- NumberOfPrimaries / u1 ----------
        return tlv_solo.set(TLV.U1, 0)

      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0x01)    # HS
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
  def update_virtual(payload)
    var val_hue = int(payload.find("Hue"))         # int or nil
    var val_sat = int(payload.find("Sat"))         # int or nil
    if (val_hue != nil) || (val_sat != nil)
      self.set_hue_sat(val_hue, val_sat)
    end
    super(self).update_virtual(payload)
  end

  #############################################################
  # For Bridge devices
  #############################################################
  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This call is synnchronous and blocking.
  def parse_status(data, index)
    super(self).parse_status(data, index)

    if index == 11                              # Status 11
      var hsb = data.find("HSBColor")
      if hsb
        import string
        var hsb_list = string.split(hsb, ",")
        var hue = int(hsb_list[0])
        var sat = int(hsb_list[1])
        # dimmer is already available

        if hue != nil     hue = tasmota.scale_uint(hue, 0, 360, 0, 254)   else hue = self.shadow_hue      end
        if sat != nil     sat = tasmota.scale_uint(sat, 0, 100, 0, 254)   else sat = self.shadow_sat      end
        if hue != self.shadow_hue   self.attribute_updated(0x0300, 0x0000)   self.shadow_hue = hue   end
        if sat != self.shadow_sat   self.attribute_updated(0x0300, 0x0001)   self.shadow_sat = sat   end
      end
    end
  end
  
  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    import webserver
    self.web_values_prefix()        # display '| ' and name if present
    webserver.content_send(format("%s %s %s",
                              self.web_value_onoff(self.shadow_onoff), self.web_value_dimmer(),
                              self.web_value_RGB()))
  end

  # Show on/off value as html
  def web_value_RGB()
    if self.shadow_hue != nil && self.shadow_sat != nil
      var l = light_state(3)      # RGB virtual light state object
      l.set_bri(255)              # set full brightness to get full range RGB
      l.set_huesat(tasmota.scale_uint(self.shadow_hue, 0, 254, 0, 360), tasmota.scale_uint(self.shadow_sat, 0, 254, 0, 255))
      var rgb_hex = format("#%02X%02X%02X", l.r, l.g, l.b)
      var rgb_html = format('<i class="bxm" style="--cl:%s"></i>%s', rgb_hex, rgb_hex)
      return rgb_html
    end
    return ""
  end
  #############################################################
  #############################################################

end
matter.Plugin_Light3 = Matter_Plugin_Light3
