#
# Matter_Plugin_Bridge_Sensor_Humidity.be - implements base class for a Humidity Sensor via HTTP to Tasmota
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

#@ solidify:Matter_Plugin_Bridge_Sensor_Humidity,weak

class Matter_Plugin_Bridge_Sensor_Humidity : Matter_Plugin_Bridge_Sensor
  static var TYPE = "http_humidity"                 # name of the plug-in in json
  static var DISPLAY_NAME = "Humidity"            # display name of the plug-in

  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x0405: [0,1,2,0xFFFC,0xFFFD],                  # Humidity Measurement p.102 - no writable
  })
  static var TYPES = { 0x0307: 2 }                  # Humidity Sensor, rev 2

  #############################################################
  # Called when the value changed compared to shadow value
  #
  # This must be overriden.
  # This is where you call `self.attribute_updated(<cluster>, <attribute>)`
  def value_changed()
    self.attribute_updated(0x0405, 0x0000)
  end

  #############################################################
  # Pre-process value
  #
  # This must be overriden.
  # This allows to convert the raw sensor value to the target one, typically int
  def pre_value(val)
    return val != nil ? int(val * 100) : nil    # 1/100th of percentage
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
        if self.shadow_value != nil
          return tlv_solo.set(TLV.U2, int(self.shadow_value))
        else
          return tlv_solo.set(TLV.NULL, nil)
        end
      elif attribute == 0x0001          #  ---------- MinMeasuredValue / u16 ----------
        return tlv_solo.set(TLV.U2, 500)  # 0%
      elif attribute == 0x0002          #  ---------- MaxMeasuredValue / u16 ----------
        return tlv_solo.set(TLV.U2, 10000)  # 100%
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 0)    # 0 = no Extended Range
      elif attribute == 0xFFFD          #  ---------- ClusterRevision / u2 ----------
        return tlv_solo.set(TLV.U4, 3)    # 3 = New data model format and notation
      end

    else
      return super(self).read_attribute(session, ctx, tlv_solo)
    end
  end

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
  
end
matter.Plugin_Bridge_Sensor_Humidity = Matter_Plugin_Bridge_Sensor_Humidity
