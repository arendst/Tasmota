#
# Matter_Plugin_Light5.be - implements the behavior for a Light with 5 channels (RGB+CT)
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
# Matter 1.6.1 - Extended Color Light (0x010D), RGB + Color Temperature
#################################################################################
# INHERITS FROM: Matter_Plugin_Light3 (Matter_Plugin_4_Light3.be)
# Light3 provides the HS and XY shadows and the HS<->XY conversion. The Color Control
# server itself is shared and lives in Matter_Plugin_Light1, gated by `CC_FEAT`.
#
# This class only enables the ColorTemperature (CT) feature on top of HS + XY:
# - FeatureMap / ColorCapabilities = 0x19 (HS | XY | CT), cluster revision 6 kept
# - ColorMode / EnhancedColorMode: 0 = CurrentHue and CurrentSaturation,
#   1 = CurrentX and CurrentY, 2 = ColorTemperatureMireds
# - ColorTemperatureMireds 0x0007, ColorTempPhysicalMinMireds 0x400B, ColorTempPhysicalMaxMireds 0x400C
#   (153..500, or 200..380 with SetOption82), CoupleColorTempToLevelMinMireds 0x400D (= physical min),
#   StartUpColorTemperatureMireds 0x4010 (RW, nullable, persisted as `ct_startup`)
# - AcceptedCommandList: 0x00..0x0A, 0x47, 0x4B, 0x4C
# - MoveTo* and Step* are applied instantly, TransitionTime is ignored;
#   Move* and StopMoveStep are validated and accepted as no-ops; RemainingTime is always 0
#
# REQUIREMENTS AND UPGRADE NOTES:
# - `light5` requires a linked RGB+CT light (5 channels, SetOption37 < 128 i.e. not split,
#   SetOption68 0). Split RGB/CT lights are exposed as `light3` + `light2`.
# - Autoconf only creates `light5` for linked 5-channel lights.
# - Already commissioned devices keep their persisted endpoint map, they are not switched
#   to `light5` after upgrade. Uncommissioned devices get `light5`, later endpoints may shift.
# - Virtual variant: `v_light5`, MtrUpdate keys: Power, Bri, Hue, Sat, CT (CT wins if both)
# - Remote RGB+CT devices are bridged as `light3` until a Bridge_Light5 exists.
#   A future Bridge_Light5 inherits the remote `set_ct()` and CT parsing from Light1
#   and only needs to infer the color mode in `parse_status()`.
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Light5,weak

class Matter_Plugin_Light5 : Matter_Plugin_Light3
  static var TYPE = "light5"                                # name of the plug-in in json
  static var DISPLAY_NAME = "Light 5 RGB+CT"                # display name of the plug-in

  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                                     # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                                     # Identify 1.2 p.16
    # 0x0004: inherited                                     # Groups 1.3 p.21
    # 0x0062: inherited                                     # Scenes Management 1.4 (PROVISIONAL) - replaces 0x0005
    # 0x0006: inherited                                     # On/Off 1.5 p.48
    # 0x0008: inherited                                     # Level Control 1.6 p.57
    0x0300: [7,0x400B,0x400C,0x400D,0x4010],                # Color Control 3.2 p.111 - add CT to inherited HS + XY
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "CT")
  static var TYPES = { 0x010D: 4 }                  # Extended Color Light - Matter 1.6.1 Device Library (rev 4 kept deliberately, Groupcast rev 5 not implemented)
  static var CC_FEAT = 0x19                                 # HS | XY | CT

  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none
  # var virtual                                       # (bool) is the device pure virtual (i.e. not related to a device implementation by Tasmota)
  # var shadow_onoff                                  # (bool) status of the light power on/off
  # var shadow_bri                                    # (int 0..254) brightness before Gamma correction - as per Matter 255 is not allowed
  # var shadow_hue, shadow_sat, shadow_x, shadow_y    # HS and XY, see Light3
  # var shadow_ct, ct_min, ct_max, ct_startup         # Color Control CT, see Light1
  # var shadow_color_mode, cc_options                 # Color Control, see Light1

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    super(self).init(device, endpoint, arguments)   # Light3 seeds HS/XY, cc_init() seeds CT, mode, Options, StartUp CT

    if !self.VIRTUAL && !self.BRIDGE
      # Tasmota reports `colormode` only for a linked RGB+CT light (RGBW/RGBCW, not split)
      import light
      var light_status = light.get(self.light_index)
      if (light_status == nil) || (light_status.find('colormode') == nil)
        log("MTR: light5 requires a linked RGB+CT light (SetOption37 < 128, SetOption68 0)", 2)
      end
    end
  end

end
matter.Plugin_Light5 = Matter_Plugin_Light5
