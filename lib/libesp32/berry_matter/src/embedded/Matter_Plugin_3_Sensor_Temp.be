#
# Matter_Plugin_Sensor_Temp.be - implements the behavior for a Temperature Sensor
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
# Device Type: Temperature Sensor (0x0302)
# Device Type Revision: 2 (Matter 1.4.1)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0402: Temperature Measurement (M)
# - 0x0003: Identify (M)
# - 0x0004: Groups (C, [Zigbee])
#
# NOTES:
# - Simple sensor device that reports temperature measurements
# - Groups cluster only required for Zigbee devices
#################################################################################

#################################################################################
# Matter 1.4.1 Temperature Measurement Cluster (0x0402)
#################################################################################
# Cluster Revision: 4 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# PURPOSE:
# Provides temperature measurement capability with configurable range and tolerance.
#
# ATTRIBUTES:
# ID     | Name              | Type        | Constraint                      | Quality | Default | Access | Conf
# -------|-------------------|-------------|---------------------------------|---------|---------|--------|-----
# 0x0000 | MeasuredValue     | temperature | MinMeasuredValue-MaxMeasuredValue| XP      | null    | R V    | M
# 0x0001 | MinMeasuredValue  | temperature | -27315-32766                    | X       | null    | R V    | M
# 0x0002 | MaxMeasuredValue  | temperature | min(MinMeasuredValue+1)         | X       | null    | R V    | M
# 0x0003 | Tolerance         | uint16      | max2048                         |         | 0       | R V    | O
# 0xFFFC | FeatureMap        | map32       | all                             | F       | 0       | R V    | M
# 0xFFFD | ClusterRevision   | uint16      | all                             | F       | 4       | R V    | M
#
# DATA TYPES:
# - temperature: int16 in units of 0.01°C
#   - Range: -273.15°C to 327.67°C (-27315 to 32767)
#   - Example: 2500 = 25.00°C, -500 = -5.00°C
#   - null (0x8000) indicates value is not available
#
# QUALITY FLAGS:
# - X: Nullable (can be null if measurement unavailable)
# - P: Periodic reporting (changes reported automatically)
#
# ATTRIBUTES DETAIL:
# - MeasuredValue: Current temperature reading in 0.01°C units
#   - null when sensor is not available or reading is invalid
#   - Must be within MinMeasuredValue and MaxMeasuredValue range
#
# - MinMeasuredValue: Minimum temperature the sensor can measure
#   - null if minimum is unknown
#   - Typically set based on sensor hardware capabilities
#
# - MaxMeasuredValue: Maximum temperature the sensor can measure
#   - null if maximum is unknown
#   - Must be greater than MinMeasuredValue
#
# - Tolerance: Maximum expected measurement error in 0.01°C units
#   - 0 if tolerance is unknown
#   - Example: 50 = ±0.50°C tolerance
#
# IMPLEMENTATION NOTES:
# - Tasmota reports temperature in Celsius or Fahrenheit based on SetOption8
# - This plugin converts Fahrenheit to Celsius before reporting to Matter
# - Temperature is multiplied by 100 to convert to Matter's 0.01°C units
# - Typical range: -50°C to 150°C (-5000 to 15000 in Matter units)
# - Sensor readings are filtered and matched from Tasmota's JSON sensor data
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Temp,weak

class Matter_Plugin_Sensor_Temp : Matter_Plugin_Sensor
  static var TYPE = "temperature"                   # name of the plug-in in json
  static var DISPLAY_NAME = "Temperature"           # display name of the plug-in
  static var JSON_NAME = "Temperature"              # Name of the sensor attribute in JSON payloads
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Temperature")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0402: [0,1,2],                                # Temperature Measurement p.97 - no writable
  })
  static var TYPES = { 0x0302: 2 }                  # Temperature Sensor, rev 2

  #############################################################
  # Pre-process value
  #
  # This must be overriden.
  # This allows to convert the raw sensor value to the target one, typically int
  def pre_value(val)
    # TODO simplify
    if self.BRIDGE
      if self.temp_unit == self.TEMP_F          # Fahrenheit
        val = (val - 32) / 1.8
      end
      return val != nil ? int(val * 100) : nil
    else
      if tasmota.get_option(8) == 1         # Fahrenheit
        val = (val - 32) / 1.8
      end
      return val != nil ? int(val * 100) : nil
    end
  end

  #############################################################
  # Called when the value changed compared to shadow value
  #
  # This must be overriden.
  # This is where you call `self.attribute_updated(<cluster>, <attribute>)`
  def value_changed()
    self.attribute_updated(0x0402, 0x0000)
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0402              # ========== Temperature Measurement 2.3 p.97 ==========
      if   attribute == 0x0000          #  ---------- MeasuredValue / i16 (*100) ----------
        return tlv_solo.set_or_nil(TLV.I2, self.shadow_value)
      elif attribute == 0x0001          #  ---------- MinMeasuredValue / i16 (*100) ----------
        return tlv_solo.set(TLV.I2, -5000)  # -50 °C
      elif attribute == 0x0002          #  ---------- MaxMeasuredValue / i16 (*100) ----------
        return tlv_solo.set(TLV.I2, 15000)  # 150 °C
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
    webserver.content_send(format("&#x2600;&#xFE0F; %.1f °C",
                                         self.shadow_value != nil ? real(self.shadow_value) / 100 : nil))
  end
  #############################################################
  #############################################################

end
matter.Plugin_Sensor_Temp = Matter_Plugin_Sensor_Temp
