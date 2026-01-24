#
# Matter_Plugin_Sensor_Light.be - implements the behavior for a Light Sensor
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
# Matter 1.4.1 Device Specification - Light Sensor (0x0106)
#################################################################################
# Device Type: Light Sensor (0x0106)
# Device Type Revision: 3 (Matter 1.4.1 Device Library)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0400: Illuminance Measurement (M) - Light level measurement
# - 0x0003: Identify (M) - Device identification
# - 0x001D: Descriptor (M) - Inherited from base class
#
# CLUSTERS (Client):
# - 0x0004: Groups (O) - [Zigbee] Group management
#
# NOTES:
# - Measures illuminance in lux (lx)
# - Logarithmic scale for wide dynamic range (1 lx to 3.576 Mlx)
# - Suitable for ambient light sensing, daylight harvesting
#################################################################################

#################################################################################
# Matter 1.4.1 Illuminance Measurement Cluster (0x0400)
#################################################################################
# Cluster Revision: 3 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# DATA TYPES:
# - LightSensorTypeEnum(enum8):
#   * 0: Photodiode
#   * 1: CMOS
#   * 64-254: Manufacturer specific
#
# ATTRIBUTES:
# ID     | Name              | Type  | Constraint              | Quality | Default | Access | Conf
# -------|-------------------|-------|-------------------------|---------|---------|--------|-----
# 0x0000 | MeasuredValue     | uint16| 0, MinMeasuredValue-    | P,X     | 0       | R V    | M
#        |                   |       | MaxMeasuredValue        |         |         |        |
# 0x0001 | MinMeasuredValue  | uint16| 1-65533                 | X       | null    | R V    | M
# 0x0002 | MaxMeasuredValue  | uint16| min(MinMeasuredValue+1) | X       | null    | R V    | M
# 0x0003 | Tolerance         | uint16| max 2048                |         | 0       | R V    | O
# 0x0004 | LightSensorType   | Light-| all                     | X       | null    | R V    | O
#        |                   | Sensor|                         |         |         |        |
#        |                   | Type- |                         |         |         |        |
#        |                   | Enum  |                         |         |         |        |
#
# Quality Flags:
# - P: Periodic reporting (changes reported automatically)
# - X: Nullable (null = unknown/invalid)
#
# Access Control:
# - R: Read
# - V: View privilege required
#
# VALUE ENCODING:
# - MeasuredValue = 10000 × log₁₀(Illuminance[lx]) + 1
# - Valid range: 1 lx ≤ Illuminance ≤ 3.576 Mlx
# - Formula provides logarithmic scale for wide dynamic range
#
# EXAMPLES:
# - 1 lx → MeasuredValue = 10000 × log₁₀(1) + 1 = 1
# - 10 lx → MeasuredValue = 10000 × log₁₀(10) + 1 = 10001
# - 100 lx → MeasuredValue = 10000 × log₁₀(100) + 1 = 20001
# - 1000 lx → MeasuredValue = 10000 × log₁₀(1000) + 1 = 30001
# - 10000 lx → MeasuredValue = 10000 × log₁₀(10000) + 1 = 40001
#
# TYPICAL RANGES:
# - Moonlight: ~0.1 lx
# - Indoor lighting: 100-500 lx
# - Office lighting: 320-500 lx
# - Overcast day: 1000 lx
# - Full daylight: 10000-25000 lx
# - Direct sunlight: 32000-100000 lx
#
# TASMOTA IMPLEMENTATION:
# - Reads illuminance from Tasmota sensor JSON (Status 10)
# - Converts using logarithmic formula: log10(val + 1) * 10000
# - Handles negative values by returning 0
# - MinMeasuredValue: 1 (minimum detectable light)
# - MaxMeasuredValue: 0xFFFE (65534, maximum range)
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Illuminance,weak

class Matter_Plugin_Sensor_Illuminance : Matter_Plugin_Sensor
  static var TYPE = "illuminance"                   # name of the plug-in in json
  static var DISPLAY_NAME = "Illuminance"           # display name of the plug-in
  static var JSON_NAME = "Illuminance"              # Name of the sensor attribute in JSON payloads
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Illuminance")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0400: [0,1,2],                                # Illuminance Measurement p.95 - no writable
  })
  static var TYPES = { 0x0106: 3 }                  # Illuminance Sensor - Matter 1.4.1 Device Library Rev 3

  #############################################################
  # Pre-process value
  #
  # This must be overriden.
  # This allows to convert the raw sensor value to the target one, typically int
  def pre_value(val)
    if val == nil   return nil    end
    import math

    if val < 0
      return 0
    else
      return math.log10(val + 1) * 10000
    end
  end

  #############################################################
  # Called when the value changed compared to shadow value
  #
  # This must be overriden.
  # This is where you call `self.attribute_updated(<cluster>, <attribute>)`
  def value_changed()
    self.attribute_updated(0x0400, 0x0000)
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0400              # ========== Illuminance Measurement 2.2 p.95 ==========
      if   attribute == 0x0000          #  ---------- MeasuredValue / i16 ----------
        return tlv_solo.set_or_nil(TLV.U2, int(self.shadow_value))
      elif attribute == 0x0001          #  ---------- MinMeasuredValue / i16 ----------
        return tlv_solo.set(TLV.U2, 1)  # 1 lux
      elif attribute == 0x0002          #  ---------- MaxMeasuredValue / i16 ----------
        return tlv_solo.set(TLV.U2, 0xFFFE)
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
    webserver.content_send(format("&#128261; %i lux",
                                         int(self.shadow_value)))
  end
  #############################################################
  #############################################################

end
matter.Plugin_Sensor_Illuminance = Matter_Plugin_Sensor_Illuminance
