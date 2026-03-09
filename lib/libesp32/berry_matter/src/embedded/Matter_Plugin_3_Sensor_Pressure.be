#
# Matter_Plugin_Sensor_Pressure.be - implements the behavior for a Pressure Sensor
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
# Matter 1.4.1 Device Specification - Pressure Sensor (0x0305)
#################################################################################
# Device Type: Pressure Sensor (0x0305)
# Device Type Revision: 2 (Matter 1.4.1 Device Library)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0403: Pressure Measurement (M) - Atmospheric pressure measurement
# - 0x0003: Identify (M) - Device identification
# - 0x001D: Descriptor (M) - Inherited from base class
#
# CLUSTERS (Client):
# - 0x0004: Groups (O) - [Zigbee] Group management
#
# NOTES:
# - Measures atmospheric pressure in kPa or Pa
# - Supports both basic and extended range/resolution modes
# - Typical range: 30-110 kPa (300-1100 hPa)
#################################################################################

#################################################################################
# Matter 1.4.1 Pressure Measurement Cluster (0x0403)
#################################################################################
# Cluster Revision: 3 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# FEATURES:
# - Bit 0 (EXT): Extended - Extended range and resolution (O)
#
# ATTRIBUTES:
# ID     | Name              | Type  | Constraint              | Quality | Default | Access | Conf
# -------|-------------------|-------|-------------------------|---------|---------|--------|-----
# 0x0000 | MeasuredValue     | int16 | MinMeasuredValue-       | X,P     | null    | R V    | M
#        |                   |       | MaxMeasuredValue        |         |         |        |
# 0x0001 | MinMeasuredValue  | int16 | max 32766               | X       | null    | R V    | M
# 0x0002 | MaxMeasuredValue  | int16 | (MinMeasuredValue+1)-   | X       | null    | R V    | M
#        |                   |       | 32767                   |         |         |        |
# 0x0003 | Tolerance         | uint16| max 2048                |         | 0       | R V    | O
# 0x0010 | ScaledValue       | int16 | MinScaledValue-         | X       | 0       | R V    | EXT
#        |                   |       | MaxScaledValue          |         |         |        |
# 0x0011 | MinScaledValue    | int16 | max 32766               | X       | 0       | R V    | EXT
# 0x0012 | MaxScaledValue    | int16 | (MinScaledValue+1)-     | X       | 0       | R V    | EXT
#        |                   |       | 32767                   |         |         |        |
# 0x0013 | ScaledTolerance   | uint16| max 2048                |         | 0       | R V    | [EXT]
# 0x0014 | Scale             | int8  | min -127                |         | 0       | R V    | EXT
#
# Quality Flags:
# - X: Nullable (null = unknown/invalid)
# - P: Periodic reporting (changes reported automatically)
#
# Access Control:
# - R: Read
# - V: View privilege required
#
# VALUE ENCODING:
# - MeasuredValue = 10 × Pressure[kPa]
#   Example: 1013 hPa = 101.3 kPa → MeasuredValue = 1013
# - ScaledValue = 10^Scale × Pressure[Pa]
#   Example: Scale=-1, Pressure=101325 Pa → ScaledValue = 10132 (representing 101.32 kPa)
#
# TYPICAL RANGES:
# - Sea level: ~1013 hPa (101.3 kPa) → MeasuredValue = 1013
# - Min range: 300 hPa (30 kPa) → MeasuredValue = 300
# - Max range: 1100 hPa (110 kPa) → MeasuredValue = 1100
#
# TASMOTA IMPLEMENTATION:
# - Reads pressure from Tasmota sensor JSON (Status 10)
# - Converts to int16 in units of 0.1 kPa (1 hPa)
# - MinMeasuredValue: 500 (50 kPa / 500 hPa)
# - MaxMeasuredValue: 1500 (150 kPa / 1500 hPa)
# - Supports multiple pressure units via PressureUnit setting
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Pressure,weak

class Matter_Plugin_Sensor_Pressure : Matter_Plugin_Sensor
  static var TYPE = "pressure"                      # name of the plug-in in json
  static var DISPLAY_NAME = "Pressure"              # display name of the plug-in
  static var JSON_NAME = "Pressure"                 # Name of the sensor attribute in JSON payloads
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Pressure")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0403: [0,1,2],                                # Pressure Measurement
  })
  static var TYPES = { 0x0305: 2 }                  # Pressure Sensor, rev 2

  #############################################################
  # Pre-process value
  #
  # This must be overriden.
  # This allows to convert the raw sensor value to the target one, typically int
  def pre_value(val)
    return val != nil ? int(val) : nil
  end

  #############################################################
  # Called when the value changed compared to shadow value
  #
  # This must be overriden.
  # This is where you call `self.attribute_updated(<cluster>, <attribute>)`
  def value_changed()
    self.attribute_updated(0x0403, 0x0000)
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0403              # ========== Pressure Measurement 2.4 p.98 ==========
      if   attribute == 0x0000          #  ---------- MeasuredValue / i16 ----------
        return tlv_solo.set_or_nil(TLV.I2, int(self.shadow_value))
      elif attribute == 0x0001          #  ---------- MinMeasuredValue / i16 ----------
        return tlv_solo.set(TLV.I2, 500)  # 500 hPA
      elif attribute == 0x0002          #  ---------- MaxMeasuredValue / i16 ----------
        return tlv_solo.set(TLV.I2, 1500)  # 1500 hPA
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
    webserver.content_send(format("&#x26C5; %i hPa",
                                         int(self.shadow_value)))
  end
  #############################################################
  #############################################################

end
matter.Plugin_Sensor_Pressure = Matter_Plugin_Sensor_Pressure
