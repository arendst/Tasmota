#
# Matter_Plugin_2_Fan.be - implements the behavior for a generic Fan
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
# Matter 1.4.1 Device Specification - Fan (0x002B)
#################################################################################
# Device Type: Fan (0x002B)
# Device Type Revision: 3 (Matter 1.4.1 Device Library)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0202: Fan Control (M) - Fan speed and mode control
# - 0x0003: Identify (M) - Device identification
# - 0x0004: Groups (M) - Group management
# - 0x0062: Scenes Management (P) - Scene control (PROVISIONAL)
# - 0x001D: Descriptor (M) - Inherited from base class
#
# NOTES:
# - Controls fan speed with multiple modes (Off, Low, Medium, High)
# - Supports percentage-based speed control (0-100%)
# - Automatic mode conversion between discrete modes and percentage
# - Typical applications: ceiling fans, ventilation fans, HVAC fans
#################################################################################

#################################################################################
# Matter 1.4.1 Fan Control Cluster (0x0202)
#################################################################################
# Cluster Revision: 4 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# FEATURES:
# - Bit 0 (SPD): MultiSpeed - Multi-speed control (O)
# - Bit 1 (AUT): Auto - Auto mode (O)
# - Bit 2 (RCK): Rocking - Rocking/oscillation (O)
# - Bit 3 (WND): Wind - Wind modes (O)
# - Bit 4 (STEP): Step - Step command (O)
# - Bit 5 (DIR): AirflowDirection - Airflow direction control (O)
#
# DATA TYPES:
# - FanModeEnum(enum8):
#   * 0: Off
#   * 1: Low
#   * 2: Medium
#   * 3: High
#   * 4: On (deprecated)
#   * 5: Auto
#   * 6: Smart (deprecated)
# - FanModeSequenceEnum(enum8):
#   * 0: Off/Low/Med/High
#   * 1: Off/Low/High
#   * 2: Off/Low/Med/High/Auto
#   * 3: Off/Low/High/Auto
#   * 4: Off/High/Auto
#   * 5: Off/High
#
# ATTRIBUTES:
# ID     | Name            | Type           | Constraint | Quality | Default | Access | Conf
# -------|-----------------|----------------|------------|---------|---------|--------|-----
# 0x0000 | FanMode         | FanModeEnum    | desc       | N       | 0       | RW VO  | M
# 0x0001 | FanModeSequence | FanMode-       | desc       | F       | 2       | R V    | M
#        |                 | SequenceEnum   |            |         |         |        |
# 0x0002 | PercentSetting  | percent        | all        | X,N     | 0       | RW VO  | M
# 0x0003 | PercentCurrent  | percent        | all        | P       | 0       | R V    | M
# 0x0004 | SpeedMax        | uint8          | 1-100      | F       | 1       | R V    | SPD
# 0x0005 | SpeedSetting    | uint8          | 0-SpeedMax | X,N     | 0       | RW VO  | SPD
# 0x0006 | SpeedCurrent    | uint8          | 0-SpeedMax | P       | 0       | R V    | SPD
#
# Quality Flags:
# - N: Non-volatile (persists across reboots)
# - P: Periodic reporting (changes reported automatically)
# - X: Nullable (null = unknown/invalid)
# - F: Fixed (cannot be changed)
#
# Access Control:
# - R: Read, W: Write
# - V: View privilege, O: Operate privilege
#
# COMMANDS:
# ID   | Name | Dir  | Response | Access | Conf
# -----|------|------|----------|--------|-----
# 0x00 | Step | C→S  | Y        | O      | STEP
#
# Step Command: {Direction:StepDirectionEnum, Wrap:bool(O), LowestOff:bool(O)}
#
# TASMOTA IMPLEMENTATION:
# - FanMode: 0=Off, 1=Low, 2=Medium, 3=High
# - PercentSetting: 0-100% speed control
# - Automatic conversion between mode and percentage:
#   * Mode 0 (Off) → 0%
#   * Mode 1 (Low) → ~33%
#   * Mode 2 (Medium) → ~67%
#   * Mode 3 (High) → 100%
# - FanModeSequence: 2 (Off/Low/Med/High/Auto) - Auto not supported
# - Commands published: FanMode, FanSpeed (0-100), FanSpeed255 (0-255)
#
# MODE/SPEED SYNCHRONIZATION:
# - Changing FanMode updates PercentSetting automatically
# - Changing PercentSetting updates FanMode automatically
# - 0% always sets mode to Off
# - 1-100% maps to Low/Medium/High based on percentage
#
# EXAMPLES:
# - Set to Off: FanMode=0 → PercentSetting=0
# - Set to Low: FanMode=1 → PercentSetting=33
# - Set to Medium: FanMode=2 → PercentSetting=67
# - Set to High: FanMode=3 → PercentSetting=100
# - Set to 50%: PercentSetting=50 → FanMode=2 (Medium)
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Fan,weak

class Matter_Plugin_Fan : Matter_Plugin_Device
  static var TYPE = "fan"                           # name of the plug-in in json
  static var DISPLAY_NAME = "Fan"                   # display name of the plug-in
  # static var ARG  = ""                              # additional argument name (or empty if none)
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    # 0x0062: inherited                             # Scenes Management 1.4 (PROVISIONAL) - replaces 0x0005
    0x0202: [0,1,2,3],                              # Fan Control
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "FanMode", "FanSpeed", "FanSpeed255")
  static var TYPES = { 0x002B: 3 }                  # Fan - Matter 1.4.1 Device Library Rev 3

  # Inherited
  # var device                                        # reference to the `device` global object
  # var endpoint                                      # current endpoint
  # var clusters                                      # map from cluster to list of attributes, typically constructed from CLUSTERS hierachy
  # var tick                                          # tick value when it was last updated
  # var node_label                                    # name of the endpoint, used only in bridge mode, "" if none
  var shadow_fan_mode
  var shadow_fan_speed_pct
  #############################################################
  # FanMode:
  #     0: Off
  #     1: Low
  #     2: Medium
  #     3: High
  #     4: On -- deprecated
  #     5: Auto -- not declared as supported
  #     6: Smart -- deprecated

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    self.shadow_fan_mode = 0    # Off by default
    self.shadow_fan_speed_pct = 0
  end

  #############################################################
  # Model
  #
  def set_fan_mode(fan_mode)
    fan_mode = int(fan_mode)
    if (fan_mode < 0) fan_mode = 0 end        # force positive
    if fan_mode != self.shadow_fan_mode
      self.attribute_updated(0x0202, 0x0000)
      self.shadow_fan_mode = int(fan_mode)
      # compute new speed
      var new_speed_pct = self.shadow_fan_speed_pct
      if self.shadow_fan_mode == 0            # set to Off, we need to adjust speed to 0 (4.4.6.1.1)
        new_speed_pct = 0
      elif self.shadow_fan_mode > 3           # Auto mode or unsupported modes, since we don't support AUTO, set speed to max
        self.shadow_fan_mode = 3              # HIGH
        new_speed_pct = 100
      else                                    # set to value
        new_speed_pct = tasmota.scale_uint(fan_mode, 0, 3, 0, 100)
      end
      # adjust and advertize if speed changed
      if self.shadow_fan_speed_pct != new_speed_pct
        self.shadow_fan_speed_pct = new_speed_pct
        self.attribute_updated(0x0202, 0x0002)
      end
    end
  end

  def set_fan_speed_pct(fan_speed_pct)
    # guard value
    fan_speed_pct = int(fan_speed_pct)
    if (fan_speed_pct < 0)    fan_speed_pct = 0     end
    if (fan_speed_pct > 100)  fan_speed_pct = 100   end
    if fan_speed_pct != self.shadow_fan_speed_pct
      self.attribute_updated(0x0202, 0x0002)
      self.shadow_fan_speed_pct = fan_speed_pct
      # adjust mode if needed
      var new_mode = self.shadow_fan_mode
      if (fan_speed_pct == 0)
        new_mode = 0
      else
        new_mode = tasmota.scale_uint(fan_speed_pct, 1, 100, 1, 3)
      end
      # adjust and advertize if mode changed
      if (new_mode != self.shadow_fan_mode)
        self.shadow_fan_mode = new_mode
        self.attribute_updated(0x0202, 0x0000)
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
    if   cluster == 0x0202              # ========== Fan ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- FanMode / enum8 ----------
        return tlv_solo.set(TLV.U1, self.shadow_fan_mode)
      elif attribute == 0x0001          #  ---------- FanModeSequence / enum8 ----------
        return tlv_solo.set(TLV.U1, 2)  # Off/Low/Med/High/Auto
      elif attribute == 0x0002          #  ---------- PercentSetting / enum8 ----------
        return tlv_solo.set(TLV.U1, self.shadow_fan_speed_pct)
      elif attribute == 0x0003          #  ---------- PercentSetting / enum8 ----------
        return tlv_solo.set(TLV.U1, self.shadow_fan_speed_pct)
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # MVC Model
  #
  # Controller write attributes
  #############################################################
  #############################################################
  # write attribute
  def write_attribute(session, ctx, write_data)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute
    
    # ====================================================================================================
    if   cluster == 0x0202              # ========== Fan ==========
      self.update_shadow_lazy()
      if   attribute == 0x0000          #  ---------- FanMode / enum8 ----------
        if type(write_data) == 'int'
          self.set_fan_mode(write_data)
          self.publish_command('FanMode', self.shadow_fan_mode, 'FanSpeed', self.shadow_fan_speed_pct, 'FanSpeed255', tasmota.scale_uint(self.shadow_fan_speed_pct, 0, 100, 0, 255))
          return true
        else
          ctx.status = matter.CONSTRAINT_ERROR
          return false
        end
      elif attribute == 0x0002          #  ---------- PercentSetting / enum8 ----------
        if type(write_data) == 'int'
          self.set_fan_speed_pct(write_data)
          self.publish_command('FanMode', self.shadow_fan_mode, 'FanSpeed', self.shadow_fan_speed_pct, 'FanSpeed255', tasmota.scale_uint(self.shadow_fan_speed_pct, 0, 100, 0, 255))
          return true
        else
          ctx.status = matter.CONSTRAINT_ERROR
          return false
        end
      end

    end
    # return super(self).read_attribute(session, ctx, tlv_solo)   # not useful as there is nothing in superclass
    return nil
  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload)
    var val_fan_mode = payload.find("FanMode")
    if val_fan_mode != nil
      self.set_fan_mode(int(val_fan_mode))
    end
    var val_fan_speed255 = payload.find("FanSpeed255")
    if val_fan_speed255 != nil
      self.set_fan_speed_pct(tasmota.scale_uint(int(val_fan_speed255), 0, 255, 0, 100))
    end
    var val_fan_speed = payload.find("FanSpeed")
    if val_fan_speed != nil
      self.set_fan_speed_pct(int(val_fan_speed))
    end
    # super(self).update_virtual(payload)   # not useful as there is nothing in superclass
  end

end
matter.Plugin_Fan = Matter_Plugin_Fan
