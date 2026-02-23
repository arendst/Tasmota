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

#################################################################################
# Matter 1.4.1 Device Specification - Air Quality Sensor (0x002C)
#################################################################################
# Device Type: Air Quality Sensor (0x002C)
# Device Type Revision: 1 (Matter 1.4.1 Device Library)
# Class: Simple | Scope: Endpoint
#
# CLUSTERS (Server):
# - 0x005B: Air Quality (M) - Overall air quality index
# - 0x040D: Carbon Dioxide Concentration Measurement (O) - CO2 levels
# - 0x042C: PM1 Concentration Measurement (O) - Particulate Matter 1.0µm
# - 0x042A: PM2.5 Concentration Measurement (O) - Particulate Matter 2.5µm
# - 0x042D: PM10 Concentration Measurement (O) - Particulate Matter 10µm
# - 0x042E: Total VOC Concentration Measurement (O) - Volatile Organic Compounds
# - 0x0413: Nitrogen Dioxide Concentration Measurement (O) - NO2 levels
# - 0x0003: Identify (M) - Device identification
# - 0x001D: Descriptor (M) - Inherited from base class
#
# NOTES:
# - Multi-sensor device for comprehensive air quality monitoring
# - Supports multiple concentration measurement types
# - Automatic air quality index calculation from CO2 if not provided
# - Typical applications: indoor air quality monitoring, HVAC control
#################################################################################

#################################################################################
# Matter 1.4.1 Air Quality Cluster (0x005B)
#################################################################################
# Cluster Revision: 1 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# DATA TYPES:
# - AirQualityEnum(enum8):
#   * 0: Unknown
#   * 1: Good
#   * 2: Fair
#   * 3: Moderate
#   * 4: Poor
#   * 5: VeryPoor
#   * 6: ExtremelyPoor
#
# ATTRIBUTES:
# ID     | Name       | Type          | Constraint | Quality | Default | Access | Conf
# -------|------------|---------------|------------|---------|---------|--------|-----
# 0x0000 | AirQuality | AirQuality-   | desc       |         | -       | R V    | M
#        |            | Enum          |            |         |         |        |
#
# TASMOTA IMPLEMENTATION:
# - Reads AirQuality from sensor JSON if available
# - Auto-calculates from CO2 if not provided:
#   * ≤750 ppm: Good (1)
#   * ≤1000 ppm: Fair (2)
#   * ≤1250 ppm: Moderate (3)
#   * ≤1500 ppm: Poor (4)
#   * ≤1750 ppm: VeryPoor (5)
#   * >1750 ppm: ExtremelyPoor (6)
#################################################################################

#################################################################################
# Matter 1.4.1 Concentration Measurement Clusters (0x040D, 0x042C, 0x042A, 0x042D, 0x042E, 0x0413)
#################################################################################
# Cluster Revision: 3 (Matter 1.4.1)
# Role: Application | Scope: Endpoint
#
# FEATURES (Tasmota Implementation):
# - Bit 0 (MEA): NumericMeasurement - Numeric concentration values (M)
#
# ATTRIBUTES (Common to all concentration clusters):
# ID     | Name              | Type   | Constraint | Quality | Default | Access | Conf
# -------|-------------------|--------|------------|---------|---------|--------|-----
# 0x0000 | MeasuredValue     | single | all        | X,P,Q   | null    | R V    | MEA
# 0x0001 | MinMeasuredValue  | single | all        | X       | null    | R V    | MEA
# 0x0002 | MaxMeasuredValue  | single | all        | X       | null    | R V    | MEA
# 0x0008 | MeasurementUnit   | Measure| desc       | F       | 0       | R V    | MEA
#        |                   | mentUnit|           |         |         |        |
#        |                   | Enum    |           |         |         |        |
# 0x0009 | MeasurementMedium | Measure| desc       | F       | 0       | R V    | MEA
#        |                   | mentMed|            |         |         |        |
#        |                   | iumEnum|            |         |         |        |
#
# MeasurementUnitEnum: PPM=0, PPB=1, PPT=2, MGM3=3, UGM3=4, NGM3=5, PM3=6
# MeasurementMediumEnum: Air=0, Water=1, Soil=2
#
# Quality Flags:
# - X: Nullable (null = not available)
# - P: Periodic reporting
# - Q: Quieter reporting
# - F: Fixed value
#
# CLUSTER MAPPING:
# - 0x040D: Carbon Dioxide (CO2) - PPM
# - 0x042C: PM1 - µg/m³
# - 0x042A: PM2.5 - µg/m³
# - 0x042D: PM10 - µg/m³
# - 0x042E: Total VOC - PPM
# - 0x0413: Nitrogen Dioxide (NO2) - PPM
#
# TASMOTA IMPLEMENTATION:
# - Reads from sensor JSON with configurable prefix (e.g., "SCD40")
# - MeasurementUnit: 0 (PPM) for all
# - MeasurementMedium: 0 (Air) for all
# - Values as floating point numbers
# - Update interval: 10 seconds
#################################################################################

import matter

# Matter plug-in for core behavior

#@ solidify:Matter_Plugin_Sensor_Air_Quality,weak

class Matter_Plugin_Sensor_Air_Quality : Matter_Plugin_Device
  static var TYPE = "airquality"                    # name of the plug-in in json
  static var DISPLAY_NAME = "Air Quality"           # display name of the plug-in

  static var SCHEMA = "airquality|"                  # arg name
                      "l:Air Quality|"              # label (display name)
                      "h:Device key (ex: SCD40)"    # hint (type defaults to text)
  static var SCHEMA2 = "co2|"                        # CO2 filter parameter
                       "l:CO2|"                     # label (display name)
                       "h:CO2 filter (ex: SCD40#CarbonDioxide)"
  static var SCHEMA3 = "no2|"                        # NO2 filter parameter
                       "l:NO2|"                     # label (display name)
                       "h:NO2 filter (ex: SEN55#NO2)"
  static var SCHEMA4 = "pm1|"                        # PM1 filter parameter
                       "l:PM1|"                     # label (display name)
                       "h:PM1 filter (ex: SEN55#PM1)"
  static var SCHEMA5 = "pm2_5|"                      # PM2.5 filter parameter
                       "l:PM2.5|"                   # label (display name)
                       "h:PM2.5 filter (ex: SEN55#PM2.5)"
  static var SCHEMA6 = "pm10|"                       # PM10 filter parameter
                       "l:PM10|"                    # label (display name)
                       "h:PM10 filter (ex: SEN55#PM10)"
  static var SCHEMA7 = "tvoc|"                       # TVOC filter parameter
                       "l:TVOC|"                    # label (display name)
                       "h:TVOC filter (ex: SEN55#TVOC)"
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

  # Individual filter prefixes for each measurement type
  var prefix_co2                                    # CO2 filter prefix
  var prefix_no2                                    # NO2 filter prefix
  var prefix_pm1                                    # PM1 filter prefix
  var prefix_pm2_5                                  # PM2.5 filter prefix
  var prefix_pm10                                   # PM10 filter prefix
  var prefix_tvoc                                   # TVOC filter prefix
  var prefix_airquality                             # AirQuality device key (e.g., "SCD40")
  var clusters_derived                              # dynamically computed cluster map based on config
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
    self.shadow_air_quality = 0
    device.add_read_sensors_schedule(self.UPDATE_TIME)
  end

  #############################################################
  # get_clusters
  #
  # Override to return only clusters for configured sensors.
  # Air Quality (0x005B) is mandatory and always present.
  # Optional concentration clusters are included only if their
  # filter is configured.
  def get_clusters()
    if self.clusters_derived != nil
      return self.clusters_derived
    end
    return self.CLUSTERS
  end

  #############################################################
  # _build_clusters
  #
  # Build the dynamic cluster map based on configured filters.
  # Called from parse_configuration() after filter prefixes are known.
  def _build_clusters()
    # start from the complete CLUSTERS and remove unconfigured ones
    var cl = {}
    var base = self.CLUSTERS
    for k: base.keys()
      cl[k] = base[k]
    end
    # remove optional concentration clusters that are not configured
    if !self.prefix_co2    cl.remove(0x040D) end  # CO2
    if !self.prefix_pm1    cl.remove(0x042C) end  # PM1
    if !self.prefix_pm2_5  cl.remove(0x042A) end  # PM2.5
    if !self.prefix_pm10   cl.remove(0x042D) end  # PM10
    if !self.prefix_tvoc   cl.remove(0x042E) end  # TVOC
    if !self.prefix_no2    cl.remove(0x0413) end  # NO2
    self.clusters_derived = cl
  end

  #############################################################
  # parse_configuration
  #
  # Parse configuration map
  def parse_configuration(config)
    super(self).parse_configuration(config)
    # Extract filter parameters from config map
    self.prefix_airquality = config.find("airquality")
    self.prefix_co2 = config.find("co2")
    self.prefix_no2 = config.find("no2")
    self.prefix_pm1 = config.find("pm1")
    self.prefix_pm2_5 = config.find("pm2_5")
    self.prefix_pm10 = config.find("pm10")
    self.prefix_tvoc = config.find("tvoc")
    # Build dynamic cluster map based on configured filters
    self._build_clusters()
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
        self.attribute_updated(cluster, attribute)
      end
      return val
    end
    return old_val
  end

  #############################################################
  # _parse_sensor_filter
  #
  # Parse a sensor value using a filter string (e.g., "SCD40#CarbonDioxide")
  # Filter format: "DeviceKey#AttributeName" or just "DeviceKey" (uses default attribute)
  # Returns the new value or old_val if not found
  def _parse_sensor_filter(payload, filter, default_attr, old_val, func, cluster, attribute)
    import string
    var device_key = filter
    var attr_name = default_attr
    var hash_idx = string.find(filter, '#')
    if hash_idx >= 0
      device_key = filter[0 .. hash_idx - 1]
      attr_name = filter[hash_idx + 1 ..]
    end
    # Find the device in payload
    var v = payload.find(device_key)
    if v != nil
      return self._parse_sensor_entry(v, attr_name, old_val, func, cluster, attribute)
    end
    return old_val
  end
  #
  def parse_sensors(payload)
    # Parse each measurement type only if its filter is configured
    if self.prefix_co2
      self.shadow_co2 = self._parse_sensor_filter(payload, self.prefix_co2, "CarbonDioxide", self.shadow_co2, number, 0x040D, 0x0000)
    end
    if self.prefix_no2
      self.shadow_no2 = self._parse_sensor_filter(payload, self.prefix_no2, "NO2", self.shadow_no2, number, 0x0413, 0x0000)
    end
    if self.prefix_pm1
      self.shadow_pm1 = self._parse_sensor_filter(payload, self.prefix_pm1, "PM1", self.shadow_pm1, number, 0x042C, 0x0000)
    end
    if self.prefix_pm2_5
      self.shadow_pm2_5 = self._parse_sensor_filter(payload, self.prefix_pm2_5, "PM2.5", self.shadow_pm2_5, number, 0x042A, 0x0000)
    end
    if self.prefix_pm10
      self.shadow_pm10 = self._parse_sensor_filter(payload, self.prefix_pm10, "PM10", self.shadow_pm10, number, 0x042D, 0x0000)
    end
    if self.prefix_tvoc
      self.shadow_tvoc = self._parse_sensor_filter(payload, self.prefix_tvoc, "TVOC", self.shadow_tvoc, number, 0x042E, 0x0000)
    end

    # AirQuality - from device key if available, otherwise compute from CO2
    if self.prefix_airquality
      var v = payload.find(self.prefix_airquality)
      if v != nil && v.contains("AirQuality")
        self.shadow_air_quality = self._parse_sensor_entry(v, "AirQuality", self.shadow_air_quality, number, 0x005B, 0x0000)
      else
        self.compute_air_quality()
      end
    else
      self.compute_air_quality()
    end
    super(self).parse_sensors(payload)
  end

  #############################################################
  # compute_air_quality
  #
  # If self.shadow_air_quality is unknown, try to compute from other attributes
  def compute_air_quality()
    # try to compute from available values
    var new_air_quality
    if (self.shadow_co2 != nil)
      var co2 = self.shadow_co2
      if (co2 <= 750)
        new_air_quality = 1
      elif (co2 <= 1000)
        new_air_quality = 2
      elif (co2 <= 1250)
        new_air_quality = 3
      elif (co2 <= 1500)
        new_air_quality = 4
      elif (co2 <= 1750)
        new_air_quality = 5
      else
        new_air_quality = 6
      end
    # any formula based on TVOC?
    end

    # do we have a new value for air_quality?
    if (new_air_quality != nil) && (new_air_quality != self.shadow_air_quality)
      self.shadow_air_quality = new_air_quality
      self.attribute_updated(0x005B, 0x0000)
    end
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
    self.shadow_co2 = self._parse_update_virtual(payload, "CO2", self.shadow_co2, number, 0x040D, 0x0000)
    self.shadow_pm1 = self._parse_update_virtual(payload, "PM1", self.shadow_pm1, number, 0x042C, 0x0000)
    self.shadow_pm2_5 = self._parse_update_virtual(payload, "PM2.5", self.shadow_pm2_5, number, 0x042A, 0x0000)
    self.shadow_pm10 = self._parse_update_virtual(payload, "PM10", self.shadow_pm10, number, 0x042D, 0x0000)
    self.shadow_tvoc = self._parse_update_virtual(payload, "TVOC", self.shadow_tvoc, number, 0x042E, 0x0000)
    if payload.contains("AirQuality")
      self.shadow_air_quality = self._parse_update_virtual(payload, "AirQuality", number, self.shadow_air_quality, 0x005B, 0x0000)
    else
      # try to compute from available values
      self.compute_air_quality()
    end
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
      var values = data.find(self.prefix_airquality)
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
    if self.prefix_pm1    web_values_single("PM1", self.shadow_pm1)       end
    if self.prefix_pm2_5  web_values_single("PM2.5", self.shadow_pm2_5)   end
    if self.prefix_pm10   web_values_single("PM10", self.shadow_pm10)     end
    if self.prefix_co2    web_values_single("CO2", self.shadow_co2)       end
    if self.prefix_no2    web_values_single("NO2", self.shadow_no2)       end
    if self.prefix_tvoc   web_values_single("TVOC", self.shadow_tvoc)     end
  end
  #############################################################
  #############################################################
end
matter.Plugin_Sensor_Air_Quality = Matter_Plugin_Sensor_Air_Quality
