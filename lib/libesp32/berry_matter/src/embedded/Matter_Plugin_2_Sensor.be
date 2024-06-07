#
# Matter_Plugin_Sensor.be - implements the behavior for a generic Sensor
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

#@ solidify:Matter_Plugin_Sensor,weak

class Matter_Plugin_Sensor : Matter_Plugin_Device
  static var ARG  = "filter"                        # additional argument name (or empty if none)
  static var ARG_HINT = "Filter pattern"
  static var UPDATE_CMD = "Status 10"               # command to send for updates
  static var UPDATE_TIME = 5000                     # update sensor every 5s
  static var JSON_NAME = ""                         # Name of the sensor attribute in JSON payloads
  var tasmota_sensor_filter                         # Rule-type filter to the value, like "ESP32#Temperature"
  var tasmota_sensor_matcher                        # Actual matcher object
  var shadow_value                                  # Last known value
  
  var temp_unit                                     # temperature unit, "C" or "F"
  static var TEMP_C = "C"
  static var TEMP_F = "F"
  var pressure_unit                                 # pressure unit, "hPa" or "mmHg" or "inHg"
  static var PRESSURE_HPA = "hPa"
  static var PRESSURE_MMHG = "mmHg"
  static var PRESSURE_INHG = "inHg"

  #############################################################
  # Constructor
  #
  # device: contains the root device object so the plugin can "call home"
  # endpoint: (int) the endpoint number (16 bits)
  # arguments: (map) the map for all complementary arguments that are plugin specific
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    device.add_read_sensors_schedule(self.UPDATE_TIME)
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    self.tasmota_sensor_filter = config.find(self.ARG#-'filter'-#)
    if self.tasmota_sensor_filter
      self.tasmota_sensor_matcher = tasmota.Rule_Matcher.parse(self.tasmota_sensor_filter)
    end
    self.temp_unit = self.TEMP_C
    self.pressure_unit = self.PRESSURE_HPA
  end

  #############################################################
  # parse sensor
  #
  # The device calls regularly `tasmota.read_sensors()` and converts
  # it to json.
  def parse_sensors(payload)
    if !self.VIRTUAL && self.tasmota_sensor_matcher
      var val = self.tasmota_sensor_matcher.match(payload)
      if isinstance(val, map)           # if the filter returns a map, we use the default JSON_NAME like "Temperature"
        val = val.find(self.JSON_NAME)
      end
      val = self.pre_value(real(val))
      if (val != nil) && (val != self.shadow_value)
        self.value_changed()
        self.shadow_value = val
      end
    end
  end

  #############################################################
  # Called when the value changed compared to shadow value
  #
  # This must be overriden.
  # This is where you call `self.attribute_updated(<cluster>, <attribute>)`
  def value_changed()
    # self.attribute_updated(0x0402, 0x0000)
  end

  #############################################################
  # Pre-process value
  #
  # This must be overriden.
  # This allows to convert the raw sensor value to the target one, typically int
  def pre_value(val)
    return val
  end

  #############################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload)
    var val = payload.find(self.JSON_NAME)
    if val != nil
      if type(val) == 'bool'    val = int(val)    end
      
      if self.shadow_value != val
        self.value_changed()
        self.shadow_value = val
      end
    end
    super(self).update_virtual(payload)
  end

  #############################################################
  # For Bridge devices
  #############################################################
  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # TO BE OVERRIDDEN
  def parse_status(data, index)
    if index == 10                               # Status 10
      if data.contains("TempUnit")
        self.temp_unit = data["TempUnit"]
      end
      if data.contains("PressureUnit")
        self.pressure_unit = data["PressureUnit"]
      end
      if self.tasmota_sensor_matcher
        var val = self.pre_value(real(self.tasmota_sensor_matcher.match(data)))
        if val != nil
          if val != self.shadow_value
            self.value_changed()
            self.shadow_value = val
          end
        end
      end
    end
  end

  #############################################################
  # Return the first item in the filter
  def filter_name_html()
    if self.tasmota_sensor_filter
      import string
      import webserver
      return webserver.html_escape(string.split(self.tasmota_sensor_filter, '#')[0])
    end
    return ""
  end

  # Show prefix before web value
  def web_values_prefix()
    import webserver
    var name = self.get_name()
    if (!name)    name = self.filter_name_html()    end
    webserver.content_send(format(self.PREFIX, name ? webserver.html_escape(name) : ""))
  end
  #############################################################
  #############################################################

end
matter.Plugin_Sensor = Matter_Plugin_Sensor
