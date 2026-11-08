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
# Matter 1.6.1 Device Specification
#################################################################################
# Device Type: Extended Color Light (0x010D)
# Device Type Revision: 4 (kept deliberately, Groupcast rev 5 not implemented)
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
# IMPLEMENTATION (light3 = RGB):
# - FeatureMap / ColorCapabilities = 0x09 (HS | XY), cluster revision 6 kept
# - CT is not implemented: RGB-only hardware has no CT channel (use light5 for RGB+CT)
# - XY is derived from HS through the native `light_state` conversion
#################################################################################

#################################################################################
# Matter 1.6.1 Color Control Cluster (0x0300) - HueSaturation + XY
#################################################################################
# The Color Control server is shared and lives in Matter_Plugin_Light1, gated by
# `CC_FEAT`. Light3 only provides the HS and XY shadows and the HS<->XY conversion.
#
# ATTRIBUTES:
# ID     | Name                          | Type   | Constraint        | Quality | Conf
# -------|-------------------------------|--------|-------------------|---------|-----
# 0x0000 | CurrentHue                    | uint8  | 0-254             | NQ      | HS
# 0x0001 | CurrentSaturation             | uint8  | 0-254             | NQ      | HS
# 0x0002 | RemainingTime                 | uint16 | 0-65534           | Q       | M (always 0)
# 0x0003 | CurrentX                      | uint16 | 0-65279           | NQ      | XY
# 0x0004 | CurrentY                      | uint16 | 0-65279           | NQ      | XY
# 0x0008 | ColorMode                     | enum8  | desc              | N       | M
# 0x000F | Options                       | map8   | bit 0 only        |         | M (RW, volatile)
# 0x0010 | NumberOfPrimaries             | uint8  | 0-6               | FX      | M (0)
# 0x4001 | EnhancedColorMode             | enum8  | desc              | N       | M
# 0x400A | ColorCapabilities             | map16  | all               |         | M (0x09)
# 0xFFFC | FeatureMap                    | map32  | all               | F       | M (0x09)
#
# ColorMode/EnhancedColorMode values:
# - 0: CurrentHue and CurrentSaturation
# - 1: CurrentX and CurrentY
#
# COMMANDS (AcceptedCommandList 0x00..0x09, 0x47):
# 0x00 MoveToHue, 0x01 MoveHue, 0x02 StepHue, 0x03 MoveToSaturation, 0x04 MoveSaturation,
# 0x05 StepSaturation, 0x06 MoveToHueAndSaturation, 0x07 MoveToColor, 0x08 MoveColor,
# 0x09 StepColor, 0x47 StopMoveStep
# - MoveTo* and Step* are applied instantly, TransitionTime is ignored
# - Move* and StopMoveStep are validated and accepted as no-ops, RemainingTime is always 0
#
# NOTES:
# - Hue: 0-254 maps to 0-360 degrees (254 = 360°, not 255)
# - Saturation: 0-254 maps to 0-100% (254 = 100%, not 255)
# - Value 255 is reserved and should not be used
# - After an XY command the commanded CurrentX/CurrentY are reported; any later HS change
#   recomputes them from HS
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Light3,weak

class Matter_Plugin_Light3 : Matter_Plugin_Light1
  static var TYPE = "light3"                                # name of the plug-in in json
  static var DISPLAY_NAME = "Light 3 RGB"                    # display name of the plug-in

  static var SCHEMA = nil                                   # no parameter
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                                     # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                                     # Identify 1.2 p.16
    # 0x0004: inherited                                     # Groups 1.3 p.21
    # 0x0062: inherited                                     # Scenes Management 1.4 (PROVISIONAL) - replaces 0x0005
    # 0x0006: inherited                                     # On/Off 1.5 p.48
    # 0x0008: inherited                                     # Level Control 1.6 p.57
    0x0300: [0,1,2,3,4,8,0xF,0x10,0x4001,0x400A],           # Color Control 3.2 p.111 - HS + XY, RemainingTime, NumberOfPrimaries
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Hue", "Sat")
  static var TYPES = { 0x010D: 4 }                  # Extended Color Light - Device Library Rev 4
  static var CC_FEAT = 0x09                                 # HS | XY

  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none
  # var virtual                                       # (bool) is the device pure virtual (i.e. not related to a device implementation by Tasmota)
  # var shadow_onoff                                  # (bool) status of the light power on/off
  # var shadow_bri                                    # (int 0..254) brightness before Gamma correction - as per Matter 255 is not allowed
  # var shadow_color_mode, cc_options                 # Color Control, see Light1
  var shadow_hue                                    # (int 0..254) hue of color, may need to be extended to 0..360 for value in degrees
  var shadow_sat                                    # (int 0..254) saturation of color
  var shadow_x, shadow_y                            # (int 0..0xFEFF) CurrentX/CurrentY
  var ls_conv                                       # light_state used for HS<->XY and the web swatch, allocated once

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    super(self).init(device, endpoint, arguments)
    self.shadow_hue = 0
    self.shadow_sat = 0
    self.hs_to_xy()                                 # seed CurrentX/CurrentY from HS (not nullable)
    self.cc_init(arguments)                         # Color Control mode, Options (and CT for Light5)
  end

  #############################################################
  # cc_update
  #
  # Shadow sync from Light1's single `light.get()` map (local light)
  def cc_update(st)
    var hue = st.find('hue')
    var sat = st.find('sat')
    self.set_shadow_hs((hue != nil) ? tasmota.scale_uint(hue, 0, 360, 0, 254) : nil,
                       (sat != nil) ? tasmota.scale_uint(sat, 0, 255, 0, 254) : nil)
    super(self).cc_update(st)                       # CT and colormode for Light5
  end

  # update HS shadows (nil = unchanged), report changes, recompute XY only when HS changed
  def set_shadow_hs(hue, sat)
    var chg = false
    if hue != nil && hue != self.shadow_hue   self.shadow_hue = hue   self.attribute_updated(0x0300, 0x0000)   chg = true   end
    if sat != nil && sat != self.shadow_sat   self.shadow_sat = sat   self.attribute_updated(0x0300, 0x0001)   chg = true   end
    if chg   self.hs_to_xy()   end
  end

  # update XY shadows, report changes
  def set_shadow_xy(x, y)
    if x != self.shadow_x   self.shadow_x = x   self.attribute_updated(0x0300, 0x0003)   end
    if y != self.shadow_y   self.shadow_y = y   self.attribute_updated(0x0300, 0x0004)   end
  end

  # one light_state per plugin: light_state has no deinit and its native object is never freed
  def get_ls()
    if self.ls_conv == nil   self.ls_conv = light_state(3)   end      # 3 = RGB
    return self.ls_conv
  end

  # recompute CurrentX/CurrentY from the HS shadows
  def hs_to_xy()
    var l = self.get_ls()
    l.set_huesat(tasmota.scale_uint(self.shadow_hue, 0, 254, 0, 360), tasmota.scale_uint(self.shadow_sat, 0, 254, 0, 255))
    self.set_shadow_xy(self.xy_u16(l.x), self.xy_u16(l.y))
  end

  # CIE coordinate (real) -> Matter uint16, clamped in float domain before int()
  def xy_u16(f)
    if !(f >= 0)   f = 0       end              # also catches NaN
    if f > 0.996   f = 0.996   end              # 0.996 * 65536 < 0xFEFF
    return int(f * 65536 + 0.5)
  end

  # CIE xy (Matter uint16) -> [hue 0..254, sat 0..254]
  def xy_to_hs(x, y)
    var l = self.get_ls()
    l.set_xy(x / 65536.0, y / 65536.0)
    return [tasmota.scale_uint(l.hue, 0, 360, 0, 254), tasmota.scale_uint(l.sat, 0, 255, 0, 254)]
  end

  #############################################################
  # set_xy
  #
  # Drive the light through Hue/Sat in XY mode, report the commanded XY exactly
  # Returns [hue, sat] sent to the light
  def set_xy(x, y)
    var hs = self.xy_to_hs(x, y)
    self.set_hue_sat(hs[0], hs[1], 1)               # mode 1 directly: no 1->0->1 ColorMode report
    self.set_shadow_xy(x, y)                        # overrides the HS->XY recomputation done by the shadow update
    return hs
  end

  #############################################################
  # set_hue_sat
  #
  # `hue` 0..254 or `nil`
  # `sat` 0..255 or `nil`
  # `mode` ColorMode to switch to, `nil` = 0 (HS)
  def set_hue_sat(hue_254, sat_254, mode)
    self.set_color_mode((mode != nil) ? mode : 0)   # before update_shadow()
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
      self.set_shadow_hs(hue_254, sat_254)
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

        self.set_shadow_hs((hue != nil) ? tasmota.scale_uint(hue, 0, 360, 0, 254) : nil,
                           (sat != nil) ? tasmota.scale_uint(sat, 0, 100, 0, 254) : nil)
      end
    end
  end

  # Show RGB color as html
  def web_value_RGB()
    if self.shadow_hue != nil && self.shadow_sat != nil
      var l = self.get_ls()       # RGB virtual light state object
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
