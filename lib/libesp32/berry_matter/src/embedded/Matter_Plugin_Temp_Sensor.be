#
# Matter_Plugin_Temp_Sensor.be - implements the behavior for a Temperature Sensor
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

# Matter plug-in for core behavior

# dummy declaration for solidification
class Matter_Plugin_Device end

#@ solidify:Matter_Plugin_Temp_Sensor,weak

class Matter_Plugin_Temp_Sensor : Matter_Plugin_Device
  static var CLUSTERS  = {
    # 0x001D: inherited                             # Descriptor Cluster 9.5 p.453
    # 0x0003: inherited                             # Identify 1.2 p.16
    # 0x0004: inherited                             # Groups 1.3 p.21
    0x0402: [0,1,2],                                # Temperature Measurement p.97 - no writable
  }
  static var TYPES = { 0x0302: 2 }                  # Temperature Sensor, rev 2

  var tasmota_sensor_filter                         # Rule-type filter to the value, like "ESP32#Temperature"
  var tasmota_sensor_matcher                        # Actual matcher object
  var shadow_temperature                            # fake status for now # TODO

  #############################################################
  # Constructor
  def init(device, endpoint, sensor_filter)
    super(self).init(device, endpoint)
    self.tasmota_sensor_filter = sensor_filter
    self.tasmota_sensor_matcher = tasmota.Rule_Matcher.parse(sensor_filter)
  end

  #############################################################
  # parse sensor
  #
  # The device calls regularly `tasmota.read_sensors()` and converts
  # it to json.
  def parse_sensors(payload)
    if self.tasmota_sensor_matcher
      var val = real(self.tasmota_sensor_matcher.match(payload))
      if val != nil
        # import string
        # tasmota.log(string.format("MTR: update temperature for endpoint %i - %.1f C", self.endpoint,), 3)
        if val != self.shadow_temperature
          self.attribute_updated(nil, 0x0402, 0x0000)
        end
        self.shadow_temperature = val
      end
    end
  end

  #############################################################
  # get_temperature
  #
  # Update shadow and signal any change
  def get_temperature()
    return self.shadow_temperature
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx)
    import string
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    # ====================================================================================================
    if   cluster == 0x0402              # ========== Temperature Measurement 2.3 p.97 ==========
      if   attribute == 0x0000          #  ---------- MeasuredValue / i16 (*100) ----------
        if self.shadow_temperature != nil
          return TLV.create_TLV(TLV.I2, int(self.shadow_temperature * 100))
        else
          return TLV.create_TLV(TLV.NULL, nil)
        end
      elif attribute == 0x0001          #  ---------- MinMeasuredValue / i16 (*100) ----------
        return TLV.create_TLV(TLV.I2, -5000)  # -50 °C
      elif attribute == 0x0002          #  ---------- MaxMeasuredValue / i16 (*100) ----------
        return TLV.create_TLV(TLV.I2, 15000)  # 150 °C
      end

    else
      return super(self).read_attribute(session, ctx)
    end
  end

  #############################################################
  # every_second
  def every_second()
    self.get_temperature()              # force reading value and sending subscriptions
  end
end
matter.Plugin_Temp_Sensor = Matter_Plugin_Temp_Sensor
