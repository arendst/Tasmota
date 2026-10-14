#
# Matter_Plugin_Sensor_Soil.be - implements the behavior for a Soil Sensor
#
# Copyright (C) 2026  Ludovic BOUÉ
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
# Matter 1.6.0 Device Specification
#################################################################################
# Device Type: Soil Sensor (0x0045)
# Device Type Revision: 1 (Matter 1.5+)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x0430: Soil Measurement (M)
# - 0x0003: Identify (M)
# - 0x0402: Temperature Measurement (O) - not exposed by this plugin
#
# NOTES:
# - Reads the `Moisture` value (percent) reported by Tasmota soil sensors,
#   e.g. Adafruit Seesaw capacitive soil sensor (xsns_81), or BLE plant sensors
# - Soil temperature (optional on this device type) can be exposed with a
#   separate `temperature` endpoint using the same sensor filter prefix
#################################################################################

#################################################################################
# Matter 1.6.0 Soil Measurement Cluster (0x0430)
#################################################################################
# Cluster Revision: 1 (Matter 1.5+)
# Role: Application | Scope: Endpoint | PICS: SOIL
#
# ATTRIBUTES:
# ID     | Name                           | Type                    | Quality | Conf
# -------|--------------------------------|-------------------------|---------|-----
# 0x0000 | SoilMoistureMeasurementLimits  | MeasurementAccuracyStruct | F     | M
# 0x0001 | SoilMoistureMeasuredValue      | percent (uint8)         | X       | M
#
# SoilMoistureMeasurementLimits constraints (spec 2.15.4.1):
# - MeasurementType = SoilMoisture (17)
# - MinMeasuredValue 0..99, MaxMeasuredValue (Min+1)..100
# - a single AccuracyRanges entry covering [Min, Max], with only PercentMax
#   set; PercentMax (percent100ths) must be <= 10 (checked by TC_SOIL_2_1)
#
# SoilMoistureMeasuredValue: water content of the soil in percent,
# null until the first reading.
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Soil,weak

class Matter_Plugin_Sensor_Soil : Matter_Plugin_Sensor
  static var TYPE = "soil"                          # name of the plug-in in json
  static var DISPLAY_NAME = "Soil Moisture"         # display name of the plug-in
  static var JSON_NAME = "Moisture"                 # Name of the sensor attribute in JSON payloads
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "Moisture")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0430: [0,1],                                  # Soil Measurement 2.15 - no writable
  })
  static var TYPES = { 0x0045: 1 }                  # Soil Sensor, rev 1

  # static var MT_SOIL_MOISTURE = 17                # MeasurementTypeEnum SoilMoisture
  # static var PERCENT_MAX = 10                     # percent100ths, upper bound checked by TC_SOIL_2_1 (and used by the CHIP reference server)

  #############################################################
  # Pre-process value
  #
  # Tasmota reports moisture in percent, Matter uses a uint8 percent
  # Clamp in the float domain before converting, since `int()` of NaN
  # or out-of-range floats is undefined, then round to nearest
  def pre_value(val)
    if val == nil || val != val   return nil   end   # nil or NaN
    if val < 0      val = 0      end
    if val > 100    val = 100    end
    return int(val + 0.5)
  end

  #############################################################
  # Called when the value changed compared to shadow value
  def value_changed()
    self.attribute_updated(0x0430, 0x0001)
  end

  #############################################################
  # update_virtual
  #
  # Clamp values received from `MtrUpdate {"Name":"...", "Moisture":42}`
  def update_virtual(payload)
    var val = payload.find(self.JSON_NAME)
    if val != nil
      payload[self.JSON_NAME] = self.pre_value(real(val))
    end
    super(self).update_virtual(payload)
  end

  #############################################################
  # build SoilMoistureMeasurementLimits
  def _build_limits()
    var TLV = matter.TLV
    var lim = TLV.Matter_TLV_struct()
    lim.add_TLV(0, 0x06 #-TLV.U4-#, 17 #-MT_SOIL_MOISTURE-#)  # MeasurementType
    lim.add_TLV(1, 0x08 #-TLV.BOOL-#, true)                   # Measured
    lim.add_TLV(2, 0x03 #-TLV.I8-#, 0)                        # MinMeasuredValue
    lim.add_TLV(3, 0x03 #-TLV.I8-#, 100)                      # MaxMeasuredValue
    var ranges = lim.add_array(4)                             # AccuracyRanges
    var r = ranges.add_struct()
    r.add_TLV(0, 0x03 #-TLV.I8-#, 0)                          # RangeMin
    r.add_TLV(1, 0x03 #-TLV.I8-#, 100)                        # RangeMax
    r.add_TLV(2, 0x06 #-TLV.U4-#, 10 #-PERCENT_MAX-#)         # PercentMax
    return lim
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0430              # ========== Soil Measurement 2.15 ==========
      if   attribute == 0x0000          #  ---------- SoilMoistureMeasurementLimits / MeasurementAccuracyStruct ----------
        return self._build_limits()
      elif attribute == 0x0001          #  ---------- SoilMoistureMeasuredValue / percent ----------
        return tlv_solo.set_or_nil(0x06 #-TLV.U4-#, self.shadow_value != nil ? int(self.shadow_value) : nil)
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    import webserver
    self.web_values_prefix()        # display '| ' and name if present
    webserver.content_send(format("&#x1F331; %i%%", self.shadow_value != nil ? int(self.shadow_value) : nil))
  end
  #############################################################
  #############################################################

end
matter.Plugin_Sensor_Soil = Matter_Plugin_Sensor_Soil
