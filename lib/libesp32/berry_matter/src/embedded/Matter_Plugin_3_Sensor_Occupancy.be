#
# Matter_Plugin_3_Sensor_Occupancy.be - implements the behavior for an Occupancy Sensor
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
# Device Type: Occupancy Sensor (0x0107)
# Device Type Revision: 4 (Matter 1.4.1)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0003: Identify (M)
# - 0x0080: Boolean State Configuration (O)
# - 0x0406: Occupancy Sensing (M)
#
# NOTES:
# - Detects presence/occupancy using various sensing technologies
# - Supports PIR, ultrasonic, physical contact, and other sensor types
# - Boolean State Configuration cluster for alarm/sensitivity settings
#################################################################################

#################################################################################
# Matter 1.4.1 Occupancy Sensing Cluster (0x0406)
#################################################################################
# Cluster Revision: 5 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# PURPOSE:
# Provides occupancy detection using various sensing technologies including
# PIR, ultrasonic, physical contact, active infrared, radar, RF, and vision.
#
# FEATURES:
# Bit | Code  | Feature           | Conf  | Summary
# ----|-------|-------------------|-------|---------------------------
# 0   | OTHER | Other             | O.a+  | Other sensing technology
# 1   | PIR   | PIR               | O.a+  | Passive infrared sensing
# 2   | US    | Ultrasonic        | O.a+  | Ultrasonic sensing
# 3   | PHY   | PhysicalContact   | O.a+  | Physical contact sensing
# 4   | AIR   | ActiveInfrared    | O     | Active infrared sensing
# 5   | RAD   | Radar             | O     | Radar sensing
# 6   | RFS   | RFSensing         | O     | RF sensing
# 7   | VIS   | Vision            | O     | Vision-based sensing
#
# ATTRIBUTES:
# ID     | Name                              | Type                    | Constraint | Quality | Default | Access | Conf
# -------|-----------------------------------|-------------------------|------------|---------|---------|--------|-----
# 0x0000 | Occupancy                         | OccupancyBitmap         | all        | P       | 0       | R V    | M
# 0x0001 | OccupancySensorType               | OccupancySensorTypeEnum | desc       | F       | MS      | R V    | M
# 0x0002 | OccupancySensorTypeBitmap         | OccupancySensorTypeBitmap| desc      | F       | MS      | R V    | M
# 0x0003 | HoldTime                          | uint16                  | all        | N       | MS      | RW VM  | O
# 0x0004 | HoldTimeLimits                    | HoldTimeLimitsStruct    |            | F       | MS      | R V    | [HoldTime]
# 0x0010 | PIROccupiedToUnoccupiedDelay      | uint16                  | all        | N       | 0       | RW VM  | [PIR&HoldTime]
# 0x0011 | PIRUnoccupiedToOccupiedDelay      | uint16                  | all        | N       | 0       | RW VM  | [PIR&HoldTime]
# 0x0012 | PIRUnoccupiedToOccupiedThreshold  | uint8                   | 1-254      | N       | 1       | RW VM  | [PIR&HoldTime]
# 0x0020 | UltrasonicOccupiedToUnoccupiedDelay| uint16                 | all        | N       | 0       | RW VM  | [US&HoldTime]
# 0x0021 | UltrasonicUnoccupiedToOccupiedDelay| uint16                 | all        | N       | 0       | RW VM  | [US&HoldTime]
# 0x0022 | UltrasonicUnoccupiedToOccupiedThreshold| uint8              | 1-254      | N       | 1       | RW VM  | [US&HoldTime]
# 0x0030 | PhysicalContactOccupiedToUnoccupiedDelay| uint16            | all        | N       | 0       | RW VM  | [PHY&HoldTime]
# 0x0031 | PhysicalContactUnoccupiedToOccupiedDelay| uint16            | all        | N       | 0       | RW VM  | [PHY&HoldTime]
# 0x0032 | PhysicalContactUnoccupiedToOccupiedThreshold| uint8         | 1-254      | N       | 1       | RW VM  | [PHY&HoldTime]
# 0xFFFC | FeatureMap                        | map32                   | all        | F       | 0       | R V    | M
# 0xFFFD | ClusterRevision                   | uint16                  | all        | F       | 5       | R V    | M
#
# DATA TYPES:
# - OccupancyBitmap: Occupied=Bit0 (1=occupied, 0=unoccupied)
# - OccupancySensorTypeEnum: PIR=0, Ultrasonic=1, PIRAndUltrasonic=2, PhysicalContact=3
# - OccupancySensorTypeBitmap: PIR=Bit0, Ultrasonic=Bit1, PhysicalContact=Bit2
# - HoldTimeLimitsStruct: {HoldTimeMin:uint16, HoldTimeMax:uint16, HoldTimeDefault:uint16}
#
# QUALITY FLAGS:
# - P: Periodic reporting (changes reported automatically)
# - F: Fixed (cannot be changed)
# - N: NonVolatile (persists across power cycles)
#
# ATTRIBUTES DETAIL:
# - Occupancy: Bitmap indicating occupancy state
#   - Bit 0: 1=occupied, 0=unoccupied
#   - Other bits reserved
#
# - OccupancySensorType: Primary sensing technology (deprecated, use bitmap)
# - OccupancySensorTypeBitmap: All sensing technologies supported
#
# - HoldTime: Minimum time in seconds to remain in occupied state after last detection
# - Delay attributes: Time delays for state transitions (sensor-type specific)
# - Threshold attributes: Number of detections required for state change
#
# EVENTS:
# ID   | Name              | Priority | Access | Conf
# -----|-------------------|----------|--------|-----
# 0x00 | OccupancyChanged  | INFO     | V      | O
#
# IMPLEMENTATION NOTES:
# - Occupancy bitmap: 0=unoccupied, 1=occupied
# - For Tasmota: Maps to Switch state or motion sensor
# - OccupancySensorType: Set to 3 (PhysicalContact) for switch-based detection
# - HoldTime features added in Matter 1.4.1 for configurable delays
# - Value is stored in shadow_bool_value as boolean
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Occupancy,weak

class Matter_Plugin_Sensor_Occupancy : Matter_Plugin_Sensor_Boolean
  static var TYPE = "occupancy"                     # name of the plug-in in json
  static var DISPLAY_NAME = "Occupancy"                     # display name of the plug-in
  # static var ARG  = "switch"                        # additional argument name (or empty if none)
  # static var ARG_HINT = "Switch<x> number"
  # static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  # static var UPDATE_TIME = 750                      # update every 750ms
  static var JSON_NAME = "Occupancy"                # Name of the sensor attribute in JSON payloads
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Occupancy")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0406: [0,1,2],                                # Occupancy Sensing p.105 - no writable
  })
  static var TYPES = { 0x0107: 4 }                  # Occupancy Sensor - Matter 1.4.1 Device Library Rev 4

  # var tasmota_switch_index                          # Switch number in Tasmota (one based)
  # var shadow_bool_value

  #############################################################
  # value_updated
  #
  # This is triggered when a new value is changed, for subscription
  # This method is meant to be overloaded and maximize shared code
  def value_updated()
    self.attribute_updated(0x0406, 0x0000)
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0406              # ========== Occupancy Sensing ==========
      if   attribute == 0x0000          #  ---------- Occupancy / U8 ----------
        return tlv_solo.set_or_nil(TLV.U1, self.shadow_bool_value)
      elif attribute == 0x0001          #  ---------- OccupancySensorType / enum8 ----------
        return tlv_solo.set(TLV.U1, 3)  # physical contact
      elif attribute == 0x0002          #  ---------- OccupancySensorTypeBitmap / u8 ----------
        return tlv_solo.set(TLV.U1, 0)  # unknown
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # For Bridge devices
  #############################################################
  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    import webserver
    self.web_values_prefix()        # display '| ' and name if present
    webserver.content_send(format("Occupancy%i %s", self.shadow_bool_value, self.web_value_onoff(self.shadow_occupancy)))
  end

  # Show prefix before web value
  def web_values_prefix()
    import webserver
    var name = self.get_name()
    if !name
      name = "Switch" + str(self.shadow_bool_value)
    end
    webserver.content_send(format(self.PREFIX, name ? webserver.html_escape(name) : ""))
  end
  #############################################################
  #############################################################
  
end
matter.Plugin_Sensor_Occupancy = Matter_Plugin_Sensor_Occupancy
