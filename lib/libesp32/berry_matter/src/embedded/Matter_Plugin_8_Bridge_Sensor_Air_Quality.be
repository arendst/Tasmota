#
# Matter_Plugin_Bridge_Sensor_Contact.be - implements Air Quality Sensor via HTTP to Tasmota
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

#@ solidify:Matter_Plugin_Bridge_Sensor_Air_Quality,weak

class Matter_Plugin_Bridge_Sensor_Air_Quality : Matter_Plugin_Sensor_Air_Quality
  static var BRIDGE = true
  static var TYPE = "http_airquality"               # name of the plug-in in json
  # static var DISPLAY_NAME = "Air Quality"           # display name of the plug-in
  # static var ARG  = "airquality"                    # additional argument name (or empty if none)
  # static var ARG_HINT = "Sensor Model"
#   static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var UPDATE_TIME = 5000                     # update every 5s
  static var UPDATE_CMD = "Status 10"               # command to send for updates

  # static var CLUSTERS  = matter.consolidate_clusters(_class, {
  #   0x005B: [0],                                    # Air Quality - no writable
  #   0x040D: [0,1,2,8,9],                            # Carbon Dioxide Concentration Measurement
  #   0x042C: [0,1,2,8,9],                            # PM1 Concentration Measurement
  #   0x042A: [0,1,2,8,9],                            # PM2.5 Concentration Measurement
  #   0x042D: [0,1,2,8,9],                            # PM10 Concentration Measurement
  #   0x042E: [0,1,2,8,9],                            # TVOC Total Volatile Organic Compounds Concentration Measurement
  #   0x0413: [0,1,2,8,9],                            # Nitrogen Dioxide Concentration Measurement
  # })

  # static var TYPES = { 0x002C: 1 }                  # Air Quality, rev 1

  # var prefix                                        # key prefix in JSON
  # var shadow_air_quality                            # Human readable air quality index
                                                    # 0: Unknown
                                                    # 1: Good
                                                    # 4: Poor
  # # var shadow_pb0_3
  # # var shadow_pb0_5
  # # var shadow_pb1
  # # var shadow_pb2_5
  # # var shadow_pb5
  # # var shadow_pb10
  # var shadow_pm1
  # var shadow_pm2_5
  # # var shadow_pm5
  # var shadow_pm10
  # var shadow_co2
  # # var shadow_eco2
  # var shadow_no2
  # var shadow_tvoc

  #############################################################
  # Stub for updating shadow values (local copies of what we published to the Matter gateway)
  #
  # This call is synnchronous and blocking.
  def parse_status(data, index)
    if index == 10                             # Status 10
      var values = data.find(self.prefix)
    end
  end

  #############################################################
  # web_values
  #
  # Show values of the remote device as HTML
  def web_values()
    def web_values_single(name, val)
      if val != nil
        import webserver
        webserver.content_send(f"{name} {val:i} ")
      end
    end

    self.web_values_prefix()        # display '| ' and name if present
    web_values_single("Air", self.shadow_air_quality)
    web_values_single("PM1", self.shadow_pm1)
    web_values_single("PM2.5", self.shadow_pm2_5)
    web_values_single("PM10", self.shadow_pm10)
    web_values_single("CO2", self.shadow_co2)
    web_values_single("NO2", self.shadow_no2)
    web_values_single("TVOC", self.shadow_tvoc)
  end

end
matter.Plugin_Bridge_Sensor_Air_Quality = Matter_Plugin_Bridge_Sensor_Air_Quality
