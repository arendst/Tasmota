#
# Matter_Plugin_Sensor_Air_Quality.be - implements the behavior for a Air_Quality sensor
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

#@ solidify:Matter_Plugin_Sensor_Air_Quality,weak

class Matter_Plugin_Sensor_Air_Quality : Matter_Plugin_Device
  static var TYPE = "airquality"                    # name of the plug-in in json
  static var DISPLAY_NAME = "Air Quality"           # display name of the plug-in
  static var ARG  = "airquality"                    # additional argument name (or empty if none)
  static var ARG_HINT = "Device key (ex: SCD40)"
#   static var ARG_TYPE = / x -> int(x)               # function to convert argument to the right type
  static var JSON_NAME = "AirQuality"               # Name of the sensor attribute in JSON payloads
  static var UPDATE_TIME = 10000                    # update every 10 s
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "AirQuality", "CO2", "PM1", "PM2.5", "PM10", "TVOC", "NO2")
  static var CLUSTERS  = matter.consolidate_clusters(_class, {
    0x005B: [0],                                    # Air Quality - no writable
    0x040D: [0,1,2,8,9],                            # Carbon Dioxide Concentration Measurement
    0x042C: [0,1,2,8,9],                            # PM1 Concentration Measurement
    0x042A: [0,1,2,8,9],                            # PM2.5 Concentration Measurement
    0x042D: [0,1,2,8,9],                            # PM10 Concentration Measurement
    0x042E: [0,1,2,8,9],                            # TVOC Total Volatile Organic Compounds Concentration Measurement
    0x0413: [0,1,2,8,9],                            # Nitrogen Dioxide Concentration Measurement
  })

  static var TYPES = { 0x002C: 1 }                  # Air Quality, rev 1

  var prefix                                        # key prefix in JSON
  var shadow_air_quality                            # Human readable air quality index
                                                    # 0: Unknown
                                                    # 1: Good
                                                    # 4: Poor
  # var shadow_pb0_3
  # var shadow_pb0_5
  # var shadow_pb1
  # var shadow_pb2_5
  # var shadow_pb5
  # var shadow_pb10
  var shadow_pm1
  var shadow_pm2_5
  # var shadow_pm5
  var shadow_pm10
  var shadow_co2
  # var shadow_eco2
  var shadow_no2
  var shadow_tvoc

  #############################################################
  # Constructor
  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    self.shadow_air_quality = false
    device.add_read_sensors_schedule(self.UPDATE_TIME)
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    self.prefix = str(config.find(self.ARG))
  end


  #############################################################
  # parse sensor
  #
  # parse the output from `ShutterPosition`
  # Ex: `{"Shutter1":{"Position":50,"Direction":0,"Target":50,"Tilt":30}}`
  def _parse_sensor_entry(v, key, old_val, func, cluster, attribute)
    var val = v.find(key)
    if (val != nil)
      val = func(val)
      if (val != nil) && (val != old_val)
        self.attribute_updated(cluster, attribute)   # CurrentPositionTiltPercent100ths
      end
      return val
    end
    return old_val
  end
  #
  def parse_sensors(payload)
    var v = payload.find(self.prefix)
    if (v != nil)
      # CO2
      self.shadow_co2 = self._parse_sensor_entry(v, "CarbonDioxide", self.shadow_co2, number, 0x040D, 0x0000)
      # PM1
      self.shadow_pm1 = self._parse_sensor_entry(v, "PM1", self.shadow_pm1, number, 0x042C, 0x0000)
      # PM2.5
      self.shadow_pm2_5 = self._parse_sensor_entry(v, "PM2.5", self.shadow_pm2_5, number, 0x042A, 0x0000)
      # PM10
      self.shadow_pm10 = self._parse_sensor_entry(v, "PM10", self.shadow_pm10, number, 0x042D, 0x0000)
      # TVOC
      self.shadow_tvoc = self._parse_sensor_entry(v, "TVOC", self.shadow_tvoc, number, 0x042E, 0x0000)
      # NO2
      self.shadow_no2 = self._parse_sensor_entry(v, "NO2", self.shadow_no2, number, 0x0413, 0x0000)
    end
    super(self).parse_sensors(payload)            # parse other shutter values
  end

  #############################################################
  # read an attribute
  #
  def read_attribute(session, ctx, tlv_solo)
    var TLV = matter.TLV
    var cluster = ctx.cluster
    var attribute = ctx.attribute
    var ret

    # mutualize code for all values
    # Args
    #   shadow_value: value of the shadow for the type
    #   unit: unit of value, `0`=PPM if not specified
    #
    # Returns
    #   `nil`: no attribute match
    #   <qany>: TLV value
    def handle_value(shadow_value, unit)
      if (unit == nil) unit = 0 end     # default unit is `0` = PPM
      if   attribute == 0x0000          #  ---------- Measured­ Value / float ----------
        return tlv_solo.set_or_nil(TLV.FLOAT, shadow_value)
      elif attribute == 0x0001          #  ---------- MinMeasured Value / float ----------
        return tlv_solo.set(TLV.NULL, nil)
      elif attribute == 0x0002          #  ---------- MaxMeasured Value / float ----------
        return tlv_solo.set(TLV.NULL, nil)
      elif attribute == 0x0008          #  ---------- MeasurementUnit / u8 ----------
        return tlv_solo.set(TLV.U1, unit)
      elif attribute == 0x0009          #  ---------- MeasurementMedium / u8 ----------
        return tlv_solo.set(TLV.U1, 0)  # 0 = Air
      elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
        return tlv_solo.set(TLV.U4, 1)  # MEA = NumericMeasurement
      end
      return nil
    end

    # ====================================================================================================
    if   cluster == 0x005B              # ========== Air Quality ==========
      if   attribute == 0x0000          #  ---------- AirQuality / U8 ----------
        return tlv_solo.set_or_nil(TLV.U1, self.shadow_air_quality)
      # elif attribute == 0xFFFC          #  ---------- FeatureMap / map32 ----------
      #   return tlv_solo.set(TLV.U4, 0)  #
      end

    # ====================================================================================================
    elif cluster == 0x040D              # ========== Carbon Dioxide Concentration Measurement ==========
      if (ret := handle_value(self.shadow_co2))
        return ret
      end
    # ====================================================================================================
    elif cluster == 0x042C              # ========== PM1 Concentration Measurement ==========
      if (ret := handle_value(self.shadow_pm1))
        return ret
      end
    # ====================================================================================================
    elif cluster == 0x042A              # ========== PM2.5 Concentration Measurement ==========
      if (ret := handle_value(self.shadow_pm2_5))
        return ret
      end
    # ====================================================================================================
    elif cluster == 0x042D              # ========== PM10 Concentration Measurement ==========
      if (ret := handle_value(self.shadow_pm10))
        return ret
      end
    # ====================================================================================================
    elif cluster == 0x042E              # ========== TVOC ==========
      if (ret := handle_value(self.shadow_tvoc))
        return ret
      end
    # ====================================================================================================
    elif cluster == 0x0413              # ========== NO2 ==========
      if (ret := handle_value(self.shadow_no2))
        return ret
      end

    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  #######################################################################
  # update_virtual
  #
  # Update internal state for virtual devices
  def update_virtual(payload)
    self.shadow_air_quality = self._parse_update_virtual(payload, "AirQuality", number, self.shadow_air_quality, 0x005B, 0x0000)
    self.shadow_co2 = self._parse_update_virtual(payload, "CO2", self.shadow_co2, number, 0x040D, 0x0000)
    self.shadow_pm1 = self._parse_update_virtual(payload, "PM1", self.shadow_pm1, number, 0x042C, 0x0000)
    self.shadow_pm2_5 = self._parse_update_virtual(payload, "PM2.5", self.shadow_pm2_5, number, 0x042A, 0x0000)
    self.shadow_pm10 = self._parse_update_virtual(payload, "PM10", self.shadow_pm10, number, 0x042D, 0x0000)
    self.shadow_tvoc = self._parse_update_virtual(payload, "TVOC", self.shadow_tvoc, number, 0x042E, 0x0000)
    super(self).update_virtual(payload)
  end

  #############################################################
  # For Bridge devices
  #############################################################
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
        webserver.content_send(f"{name}:{val:i} ")
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
  #############################################################
  #############################################################
end
matter.Plugin_Sensor_Air_Quality = Matter_Plugin_Sensor_Air_Quality
