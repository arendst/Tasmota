#
# Matter_Plugin_Light2.be - implements the behavior for a Light with 2 channel (CT)
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
# Device Type: Color Temperature Light (0x010C)
# Device Type Revision: 4 (kept deliberately, Groupcast rev 5 not implemented)
# Class: Simple | Scope: Endpoint
# Superset: Dimmable Light (0x0101)
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
# - Color Control: ColorTemperature feature M, RemainingTime 0x0002 attr M
#################################################################################

#################################################################################
# Matter 1.6.1 Color Control Cluster (0x0300) - CT
#################################################################################
# The Color Control server is shared and lives in Matter_Plugin_Light1, gated by
# `CC_FEAT`. FeatureMap / ColorCapabilities = 0x10 (CT), cluster revision 6 kept.
#
# ATTRIBUTES:
# ID     | Name                            | Type   | Constraint        | Quality | Conf
# -------|---------------------------------|--------|-------------------|---------|-----
# 0x0002 | RemainingTime                   | uint16 | 0-65534           | Q       | M (always 0)
# 0x0007 | ColorTemperatureMireds          | uint16 | PhysMin-PhysMax   | NQ      | CT
# 0x0008 | ColorMode                       | enum8  | desc              | N       | M (always 2)
# 0x000F | Options                         | map8   | bit 0 only        |         | M (RW, volatile)
# 0x0010 | NumberOfPrimaries               | uint8  | 0-6               | FX      | M (0)
# 0x4001 | EnhancedColorMode               | enum8  | desc              | N       | M (always 2)
# 0x400A | ColorCapabilities               | map16  | all               |         | M (0x10)
# 0x400B | ColorTempPhysicalMinMireds      | uint16 | 0-0xFEFF          |         | CT
# 0x400C | ColorTempPhysicalMaxMireds      | uint16 | 0-0xFEFF          |         | CT
# 0x400D | CoupleColorTempToLevelMinMireds | uint16 | PhysMin-CT        |         | CT (= PhysMin)
# 0x4010 | StartUpColorTemperatureMireds   | uint16 | 1-0xFEFF          | XN      | CT (RW)
# 0xFFFC | FeatureMap                      | map32  | all               | F       | M (0x10)
#
# COMMANDS (AcceptedCommandList 0x0A, 0x47, 0x4B, 0x4C):
# 0x000A | MoveToColorTemperature: {ColorTemperatureMireds:uint16, TransitionTime:uint16, OptionsMask:map8, OptionsOverride:map8}
# 0x0047 | StopMoveStep: {OptionsMask:map8, OptionsOverride:map8}
# 0x004B | MoveColorTemperature: {MoveMode:enum8, Rate:uint16, ColorTemperatureMinimumMireds:uint16, ColorTemperatureMaximumMireds:uint16, OptionsMask:map8, OptionsOverride:map8}
# 0x004C | StepColorTemperature: {StepMode:enum8, StepSize:uint16, TransitionTime:uint16, ColorTemperatureMinimumMireds:uint16, ColorTemperatureMaximumMireds:uint16, OptionsMask:map8, OptionsOverride:map8}
# - MoveToColorTemperature and StepColorTemperature are applied instantly, TransitionTime is ignored
# - MoveColorTemperature and StopMoveStep are validated and accepted as no-ops
#
# NOTES:
# - ColorTemperatureMireds: 1,000,000 / Kelvin (e.g., 153 = 6535K, 500 = 2000K)
# - Standard range: 153-500 mireds (2000K-6535K)
# - Alexa emulation mode (SetOption82): 200-380 mireds (2632K-5000K)
# - StartUpColorTemperatureMireds is persisted as `ct_startup` in the endpoint configuration
#   and applied when Matter starts (not for bridged or Zigbee lights)
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Light2,weak

class Matter_Plugin_Light2 : Matter_Plugin_Light1
  static var TYPE = "light2"                                # name of the plug-in in json
  static var DISPLAY_NAME = "Light 2 CT"                    # display name of the plug-in

  static var SCHEMA = nil                                   # no parameter
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                                     # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                                     # Identify 1.2 p.16
    # 0x0004: inherited                                     # Groups 1.3 p.21
    # 0x0062: inherited                                     # Scenes Management 1.4 (PROVISIONAL) - replaces 0x0005
    # 0x0006: inherited                                     # On/Off 1.5 p.48
    # 0x0008: inherited                                     # Level Control 1.6 p.57
    0x0300: [2,7,8,0xF,0x10,0x4001,0x400A,0x400B,0x400C,0x400D,0x4010],   # Color Control 3.2 p.111 (CT)
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "CT")
  static var TYPES = { 0x010C: 4 }                  # Color Temperature Light - Device Library Rev 4
  static var CC_FEAT = 0x10                                 # CT

  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none
  # var shadow_onoff                                  # (bool) status of the light power on/off
  # var shadow_bri                                    # (int 0..254) brightness before Gamma correction - as per Matter 255 is not allowed
  # var light_index                                   # index number when using `light.get()` and `light.set()`
  # var shadow_ct, ct_min, ct_max, ct_startup         # Color Control CT, see Light1
  # var shadow_color_mode, cc_options                 # Color Control, see Light1

  #############################################################
  # Constructor
  def init(device, endpoint, arguments)
    super(self).init(device, endpoint, arguments)
    if !self.BRIDGE
      import light
      if (light.get(1) != nil)
        self.light_index = 1                        # split RGB/CT: CT is light 1
      end
    end
    self.cc_init(arguments)                         # after light_index: StartUp CT targets the right channel
  end

end
matter.Plugin_Light2 = Matter_Plugin_Light2
