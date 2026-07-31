#
# Matter_Plugin_OnOff_Power.be - implements the behavior for a Relay (OnOff) with
# Electrical Power Measurement (smart plug with power monitoring)
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
# Matter 1.4.1 Device Specification - On/Off Plug-in Unit (0x010A) with Power
#################################################################################
# Device Type: On/Off Plug-in Unit (0x010A)
# Device Type Revision: 3 (Matter 1.4.1)
# Class: Simple | Scope: Endpoint
#
# Same device type as the plain `relay` plugin (`Matter_Plugin_OnOff`), with the
# addition of the Electrical Power Measurement cluster so the plug reports live
# Voltage/Current/Power to the Matter controller (Apple Home, Google Home,
# SmartThings and similar all display power for plugs exposing this cluster).
#
# NOTES:
# - Not a strict Matter "Electrical Sensor" (0x0510) composition; this follows
#   the common real-world practice of adding the measurement cluster directly
#   on the plug endpoint, as done by many commercial Matter plugs.
# - Single measurement channel: reads the (non-indexed) `ENERGY` JSON object,
#   i.e. plugs with a single energy-monitoring chip (HLW8012, BL0937, CSE7766,
#   ADE7953, PZEM, etc.). Multi-channel/multi-phase energy (`Voltage1/2`,
#   `Power1/2`, ...) is not handled by this plugin.
#################################################################################

#################################################################################
# Matter 1.4.1 Electrical Power Measurement Cluster (0x0090)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# FEATURES:
# - Bit 1 (ALTC): AlternatingCurrent - AC measurement (M for Tasmota energy sensors)
#
# ATTRIBUTES (exposed by this plugin):
# ID     | Name              | Type   | Unit      | Tasmota mapping
# -------|-------------------|--------|-----------|------------------------------
# 0x0000 | PowerMode         | enum8  |           | fixed AC (2)
# 0x0001 | NumberOfMeasurementTypes | uint8 |      | fixed 8
# 0x0002 | Accuracy          | list[MeasurementAccuracyStruct] | | fixed, declarative
# 0x0008 | ActivePower       | int64  | mW        | ENERGY.Power * 1000
# 0x0009 | ReactivePower     | int64  | mVAR      | ENERGY.ReactivePower * 1000
# 0x000A | ApparentPower     | int64  | mVA       | ENERGY.ApparentPower * 1000
# 0x000B | RMSVoltage        | int64  | mV        | ENERGY.Voltage * 1000
# 0x000C | RMSCurrent        | int64  | mA        | ENERGY.Current * 1000
# 0x000D | RMSPower          | int64  | mW        | ENERGY.Power * 1000 (same as ActivePower)
# 0x000E | Frequency         | int64  | mHz       | ENERGY.Frequency * 1000
# 0x0011 | PowerFactor       | int64  | 1/10000   | ENERGY.Factor * 10000
#
# All value attributes are nullable (`X` quality): report `null` until the
# first successful sensor read.
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_OnOff_Power,weak

class Matter_Plugin_OnOff_Power : Matter_Plugin_OnOff
  static var TYPE = "relay_power"                   # name of the plug-in in json
  static var DISPLAY_NAME = "Relay + Power"         # display name of the plug-in

  static var UPDATE_CMD = "Status 10"               # command to send for updates (bridge mode)
  static var UPDATE_TIME = 5000                     # update sensor every 5s
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0090: [0,1,2,8,9,0x0A,0x0B,0x0C,0x0D,0x0E,0x11],   # Electrical Power Measurement
  })

  # MeasurementTypeEnum values used below (Matter 1.4.1 Common Data Types)
  static var MT_ACTIVE_POWER   = 5
  static var MT_REACTIVE_POWER = 6
  static var MT_APPARENT_POWER = 7
  static var MT_RMS_VOLTAGE    = 8
  static var MT_RMS_CURRENT    = 9
  static var MT_RMS_POWER      = 10
  static var MT_FREQUENCY      = 11
  static var MT_POWER_FACTOR   = 12

  var shadow_voltage                                # (real) Voltage in Volts
  var shadow_current                                # (real) Current in Amps
  var shadow_active_power                           # (real) Active power in Watts
  var shadow_apparent_power                         # (real) Apparent power in VA
  var shadow_reactive_power                         # (real) Reactive power in VAR
  var shadow_frequency                              # (real) Frequency in Hz
  var shadow_power_factor                           # (real) Power factor 0.00-1.00

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    device.add_read_sensors_schedule(self.UPDATE_TIME)
  end

  #############################################################
  # parse a single ENERGY value, firing `attribute_updated` if changed
  #
  # nrg: the `ENERGY` map from the sensor JSON
  # key: JSON key to read (ex: "Voltage")
  # old_val: previous shadow value
  # attribute: Matter attribute id to notify on change
  # attribute2: optional second attribute id sharing the same value (ex: RMSPower)
  def _parse_energy_value(nrg, key, old_val, attribute, attribute2)
    var val = nrg.find(key)
    if val != nil
      val = real(val)
      if val != old_val
        self.attribute_updated(0x0090, attribute)
        if attribute2 != nil
          self.attribute_updated(0x0090, attribute2)
        end
      end
      return val
    end
    return old_val
  end

  #############################################################
  # parse sensor
  #
  # Reads the (non-indexed) `ENERGY` object from `tasmota.read_sensors()`
  def parse_sensors(payload)
    var nrg = payload.find("ENERGY")
    if nrg != nil
      self.shadow_voltage        = self._parse_energy_value(nrg, "Voltage", self.shadow_voltage, 0x000B)
      self.shadow_current        = self._parse_energy_value(nrg, "Current", self.shadow_current, 0x000C)
      self.shadow_active_power   = self._parse_energy_value(nrg, "Power", self.shadow_active_power, 0x0008, 0x000D)
      self.shadow_apparent_power = self._parse_energy_value(nrg, "ApparentPower", self.shadow_apparent_power, 0x000A)
      self.shadow_reactive_power = self._parse_energy_value(nrg, "ReactivePower", self.shadow_reactive_power, 0x0009)
      self.shadow_frequency      = self._parse_energy_value(nrg, "Frequency", self.shadow_frequency, 0x000E)
      self.shadow_power_factor   = self._parse_energy_value(nrg, "Factor", self.shadow_power_factor, 0x0011)
    end
    super(self).parse_sensors(payload)
  end

  #############################################################
  # build the (fixed/declarative) Accuracy list attribute
  #
  # list[MeasurementAccuracyStruct] - one entry per measurement type exposed
  def _build_accuracy_list()
    var TLV = matter.TLV
    var acc = TLV.Matter_TLV_array()
    # [MeasurementType, MinMeasuredValue, MaxMeasuredValue] in the attribute's native unit
    var specs = [
      [self.MT_RMS_VOLTAGE,    0,          300000],     # 0-300 V
      [self.MT_RMS_CURRENT,    0,          100000],     # 0-100 A
      [self.MT_ACTIVE_POWER,   -50000000,  50000000],   # +/-50 kW
      [self.MT_REACTIVE_POWER, -50000000,  50000000],   # +/-50 kVAR
      [self.MT_APPARENT_POWER, 0,          50000000],   # 0-50 kVA
      [self.MT_RMS_POWER,      -50000000,  50000000],   # +/-50 kW
      [self.MT_FREQUENCY,      0,          100000],     # 0-100 Hz
      [self.MT_POWER_FACTOR,   -10000,     10000],      # -1.00 .. 1.00
    ]
    for s: specs
      var e = acc.add_struct()
      e.add_TLV(0, 0x04 #-TLV.U1-#, s[0])              # MeasurementType
      e.add_TLV(1, 0x08 #-TLV.BOOL-#, true)            # Measured
      e.add_TLV(2, 0x03 #-TLV.I8-#, s[1])              # MinMeasuredValue
      e.add_TLV(3, 0x03 #-TLV.I8-#, s[2])              # MaxMeasuredValue
      var ranges = e.add_array(4)                      # AccuracyRanges
      var r = ranges.add_struct()
      r.add_TLV(0, 0x03 #-TLV.I8-#, s[1])              # RangeMin
      r.add_TLV(1, 0x03 #-TLV.I8-#, s[2])              # RangeMax
      r.add_TLV(2, 0x05 #-TLV.U2-#, 500)               # PercentMax = 5.00%
    end
    return acc
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0090              # ========== Electrical Power Measurement 2.13 ==========
      if   attribute == 0x0000          #  ---------- PowerMode / enum8 ----------
        return tlv_solo.set(0x04 #-TLV.U1-#, 2)          # AC
      elif attribute == 0x0001          #  ---------- NumberOfMeasurementTypes / u8 ----------
        return tlv_solo.set(0x04 #-TLV.U1-#, 8)
      elif attribute == 0x0002          #  ---------- Accuracy / list[MeasurementAccuracyStruct] ----------
        return self._build_accuracy_list()
      elif attribute == 0x0008          #  ---------- ActivePower / i64 (mW) ----------
        return tlv_solo.set_or_nil(0x03 #-TLV.I8-#, self.shadow_active_power != nil ? int(self.shadow_active_power * 1000) : nil)
      elif attribute == 0x0009          #  ---------- ReactivePower / i64 (mVAR) ----------
        return tlv_solo.set_or_nil(0x03 #-TLV.I8-#, self.shadow_reactive_power != nil ? int(self.shadow_reactive_power * 1000) : nil)
      elif attribute == 0x000A          #  ---------- ApparentPower / i64 (mVA) ----------
        return tlv_solo.set_or_nil(0x03 #-TLV.I8-#, self.shadow_apparent_power != nil ? int(self.shadow_apparent_power * 1000) : nil)
      elif attribute == 0x000B          #  ---------- RMSVoltage / i64 (mV) ----------
        return tlv_solo.set_or_nil(0x03 #-TLV.I8-#, self.shadow_voltage != nil ? int(self.shadow_voltage * 1000) : nil)
      elif attribute == 0x000C          #  ---------- RMSCurrent / i64 (mA) ----------
        return tlv_solo.set_or_nil(0x03 #-TLV.I8-#, self.shadow_current != nil ? int(self.shadow_current * 1000) : nil)
      elif attribute == 0x000D          #  ---------- RMSPower / i64 (mW) ----------
        return tlv_solo.set_or_nil(0x03 #-TLV.I8-#, self.shadow_active_power != nil ? int(self.shadow_active_power * 1000) : nil)
      elif attribute == 0x000E          #  ---------- Frequency / i64 (mHz) ----------
        return tlv_solo.set_or_nil(0x03 #-TLV.I8-#, self.shadow_frequency != nil ? int(self.shadow_frequency * 1000) : nil)
      elif attribute == 0x0011          #  ---------- PowerFactor / i64 (1/10000) ----------
        return tlv_solo.set_or_nil(0x03 #-TLV.I8-#, self.shadow_power_factor != nil ? int(self.shadow_power_factor * 10000) : nil)
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    super(self).web_values()
    if self.shadow_active_power != nil
      import webserver
      webserver.content_send(format(" %.1fW", self.shadow_active_power))
    end
  end
  #############################################################
  #############################################################

end
matter.Plugin_OnOff_Power = Matter_Plugin_OnOff_Power
