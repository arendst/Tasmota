#
# Matter_Plugin_Sensor_Humidity.be - implements the behavior for a Humidity Sensor
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
# Device Type: Humidity Sensor (0x0307)
# Device Type Revision: 2 (Matter 1.4.1)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0405: Relative Humidity Measurement (M)
# - 0x0003: Identify (M)
# - 0x0004: Groups (C, [Zigbee])
#
# NOTES:
# - Simple sensor device that reports relative humidity measurements
# - Groups cluster only required for Zigbee devices
#################################################################################

#################################################################################
# Matter 1.4.1 Relative Humidity Measurement Cluster (0x0405)
#################################################################################
# Cluster Revision: 3 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# PURPOSE:
# Provides relative humidity measurement capability with configurable range.
#
# ATTRIBUTES:
# ID     | Name              | Type   | Constraint                      | Quality | Default | Access | Conf
# -------|-------------------|--------|---------------------------------|---------|---------|--------|-----
# 0x0000 | MeasuredValue     | uint16 | MinMeasuredValue-MaxMeasuredValue| XP     | null    | R V    | M
# 0x0001 | MinMeasuredValue  | uint16 | max9999                         | X       | null    | R V    | M
# 0x0002 | MaxMeasuredValue  | uint16 | (MinMeasuredValue+1)-10000      | X       | null    | R V    | M
# 0x0003 | Tolerance         | uint16 | max2048                         |         | 0       | R V    | O
# 0xFFFC | FeatureMap        | map32  | all                             | F       | 0       | R V    | M
# 0xFFFD | ClusterRevision   | uint16 | all                             | F       | 3       | R V    | M
#
# DATA TYPES:
# - MeasuredValue: uint16 in units of 0.01% (1/100th of a percent)
#   - Range: 0-10000 (0.00% to 100.00%)
#   - Example: 5000 = 50.00%, 9550 = 95.50%
#   - null (0xFFFF) indicates value is not available
#
# QUALITY FLAGS:
# - X: Nullable (can be null if measurement unavailable)
# - P: Periodic reporting (changes reported automatically)
#
# ATTRIBUTES DETAIL:
# - MeasuredValue: Current relative humidity reading in 0.01% units
#   - null when sensor is not available or reading is invalid
#   - Must be within MinMeasuredValue and MaxMeasuredValue range
#
# - MinMeasuredValue: Minimum humidity the sensor can measure
#   - null if minimum is unknown
#   - Typically 0 (0%) or 500 (5%) based on sensor capabilities
#
# - MaxMeasuredValue: Maximum humidity the sensor can measure
#   - null if maximum is unknown
#   - Typically 10000 (100%) or 9500 (95%) based on sensor capabilities
#   - Must be greater than MinMeasuredValue
#
# - Tolerance: Maximum expected measurement error in 0.01% units
#   - 0 if tolerance is unknown
#   - Example: 200 = ±2.00% tolerance
#
# IMPLEMENTATION NOTES:
# - Tasmota reports humidity as percentage (0-100)
# - This plugin multiplies by 100 to convert to Matter's 0.01% units
# - Typical range: 0-100% (0-10000 in Matter units)
# - Sensor readings are filtered and matched from Tasmota's JSON sensor data
# - Value is stored in shadow_value as int (0-10000)
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Humidity,weak

class Matter_Plugin_Sensor_Humidity : Matter_Plugin_Sensor
  static var TYPE = "humidity"                      # name of the plug-in in json
  static var DISPLAY_NAME = "Humidity"              # display name of the plug-in
  static var JSON_NAME = "Humidity"                 # Name of the sensor attribute in JSON payloads
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Humidity")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0405: [0,1,2],                                # Humidity Measurement p.102 - no writable
  })
  static var TYPES = { 0x0307: 2 }                  # Humidity Sensor, rev 2

  #############################################################
  # Pre-process value
  #
  # This must be overriden.
  # This allows to convert the raw sensor value to the target one, typically int
  def pre_value(val)
    return val != nil ? int(val * 100) : nil    # 1/100th of percentage
  end

  #############################################################
  # Called when the value changed compared to shadow value
  #
  # This must be overriden.
  # This is where you call `self.attribute_updated(<cluster>, <attribute>)`
  def value_changed()
    self.attribute_updated(0x0405, 0x0000)
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0405              # ========== Humidity Measurement 2.4 p.98 ==========
      if   attribute == 0x0000          #  ---------- Humidity / u16 ----------
        return tlv_solo.set_or_nil(TLV.U2, int(self.shadow_value))
      elif attribute == 0x0001          #  ---------- MinMeasuredValue / u16 ----------
        return tlv_solo.set(TLV.U2, 500)  # 0%
      elif attribute == 0x0002          #  ---------- MaxMeasuredValue / u16 ----------
        return tlv_solo.set(TLV.U2, 10000)  # 100%
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
    webserver.content_send(format("&#x1F4A7; %2.0f%%",
                                         self.shadow_value != nil ? real(self.shadow_value) / 100 : nil))
  end
  #############################################################
  #############################################################

end
matter.Plugin_Sensor_Humidity = Matter_Plugin_Sensor_Humidity
