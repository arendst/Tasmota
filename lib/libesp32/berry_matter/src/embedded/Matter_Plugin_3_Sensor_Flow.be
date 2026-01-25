#
# Matter_Plugin_Sensor_Flow.be - implements the behavior for a Flow Sensor
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
# Matter 1.4.1 Device Specification - Flow Sensor (0x0306)
#################################################################################
# Device Type: Flow Sensor (0x0306)
# Device Type Revision: 2 (Matter 1.4.1 Device Library)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0404: Flow Measurement (M) - Fluid flow rate measurement
# - 0x0003: Identify (M) - Device identification
# - 0x001D: Descriptor (M) - Inherited from base class
#
# CLUSTERS (Client):
# - 0x0004: Groups (O) - [Zigbee] Group management
#
# NOTES:
# - Measures volumetric flow rate in m³/h
# - Typical applications: water flow, gas flow, air flow
# - Wide range support: 0-6553.4 m³/h
#################################################################################

#################################################################################
# Matter 1.4.1 Flow Measurement Cluster (0x0404)
#################################################################################
# Cluster Revision: 3 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# ATTRIBUTES:
# ID     | Name              | Type  | Constraint              | Quality | Default | Access | Conf
# -------|-------------------|-------|-------------------------|---------|---------|--------|-----
# 0x0000 | MeasuredValue     | uint16| MinMeasuredValue-       | X,P     | null    | R V    | M
#        |                   |       | MaxMeasuredValue        |         |         |        |
# 0x0001 | MinMeasuredValue  | uint16| max 65533               | X       | null    | R V    | M
# 0x0002 | MaxMeasuredValue  | uint16| min(MinMeasuredValue+1) | X       | null    | R V    | M
# 0x0003 | Tolerance         | uint16| max 2048                |         | 0       | R V    | O
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
# - MeasuredValue = 10 × Flow[m³/h]
#   Example: 5.5 m³/h → MeasuredValue = 55
#
# TYPICAL RANGES:
# - Water tap: 0.1-0.5 m³/h (1-5 units)
# - Shower: 0.5-1.0 m³/h (5-10 units)
# - Garden hose: 1-3 m³/h (10-30 units)
# - Industrial: 10-1000 m³/h (100-10000 units)
# - Maximum: 6553.4 m³/h (65534 units)
#
# TASMOTA IMPLEMENTATION:
# - Reads flow from Tasmota sensor JSON (Status 10)
# - Converts to uint16 in units of 0.1 m³/h
# - MinMeasuredValue: 0 (no flow)
# - MaxMeasuredValue: 65534 (6553.4 m³/h)
# - pre_value() multiplies by 10 for proper encoding
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Flow,weak

class Matter_Plugin_Sensor_Flow : Matter_Plugin_Sensor
  static var TYPE = "flow"                      # name of the plug-in in json
  static var DISPLAY_NAME = "Flow"              # display name of the plug-in
  static var JSON_NAME = "Flow"                 # Name of the sensor attribute in JSON payloads
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Flow")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0404: [0,1,2],                            # Flow Measurement
  })
  static var TYPES = { 0x0306: 2 }              # Flow Sensor - Matter 1.4.1 Device Library Rev 2

  #############################################################
  # Pre-process value
  #
  # This must be overriden.
  # This allows to convert the raw sensor value to the target one, typically int
  def pre_value(val)
    return val != nil ? int(val * 10) : nil     # MeasuredValue represents 10 x flow in m3/h
  end

  #############################################################
  # Called when the value changed compared to shadow value
  #
  # This must be overriden.
  # This is where you call `self.attribute_updated(<cluster>, <attribute>)`
  def value_changed()
    self.attribute_updated(0x0404, 0x0000)
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0404              # ========== Flow Measurement 2.4 p.98 ==========
      if   attribute == 0x0000          #  ---------- MeasuredValue / i16 ----------
        return tlv_solo.set_or_nil(TLV.U2, int(self.shadow_value)) # MeasuredValue represents 10 x flow in m3/h.
      elif attribute == 0x0001              #  ---------- MinMeasuredValue / i16 ----------
        return tlv_solo.set(TLV.U2, 0)      # 0 m3/h
      elif attribute == 0x0002              #  ---------- MaxMeasuredValue / i16 ----------
        return tlv_solo.set(TLV.U2, 65534)  # 65534 m3/h
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
    webserver.content_send(format("&#x26C5; %i m&sup3;/h",
                                         int(self.shadow_value)))
  end
  #############################################################
  #############################################################
end
matter.Plugin_Sensor_Flow = Matter_Plugin_Sensor_Flow
