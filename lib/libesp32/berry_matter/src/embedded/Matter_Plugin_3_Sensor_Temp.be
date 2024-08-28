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
