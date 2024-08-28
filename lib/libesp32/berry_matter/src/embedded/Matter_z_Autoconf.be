#
# Matter_z_Autoconf.be - implements autoconfiguration of Matter from Tasmota native features
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

#@ solidify:Matter_Autoconf,weak

class Matter_Autoconf
  var device                          # reference to the main Device instance

  #############################################################
  def init(device)
    self.device = device
  end

  #############################################################
  # Load plugins configuration from json
  #
  # 'config' is a map
  # Ex:
  #   {'32': {'filter': 'AXP192#Temperature', 'type': 'temperature'}, '40': {'filter': 'BMP280#Pressure', 'type': 'pressure'}, '34': {'filter': 'SHT3X#Temperature', 'type': 'temperature'}, '33': {'filter': 'BMP280#Temperature', 'type': 'temperature'}, '1': {'relay': 0, 'type': 'relay'}, '56': {'filter': 'SHT3X#Humidity', 'type': 'humidity'}, '0': {'type': 'root'}}
  def instantiate_plugins_from_config(config)
    var endpoints = self.device.k2l_num(config)
    var plugins = self.device.plugins

    # start with mandatory endpoint 0 for root node
    plugins.push(matter.Plugin_Root(self.device, 0, {}))
    log("MTR: Configuring endpoints", 2)
    log(format("MTR:   endpoint = %5i type:%s%s", 0, 'root', ''), 2)

    # always include an aggregator for dynamic endpoints
    plugins.push(matter.Plugin_Aggregator(self.device, matter.AGGREGATOR_ENDPOINT, {}))
    log(format("MTR:   endpoint = %5i type:%s%s", matter.AGGREGATOR_ENDPOINT, 'aggregator', ''), 2)

    for ep: endpoints
      if ep == 0  continue end          # skip endpoint 0
      try
        var plugin_conf = config[str(ep)]
        # log(format("MTR: endpoint %i config %s", ep, plugin_conf), 3)

        var pi_class_name = plugin_conf.find('type')
        if pi_class_name == nil   log("MTR: no class name, skipping", 3)  continue end
        if pi_class_name == 'root'  log("MTR: only one root node allowed", 3)  continue end
        var pi_class = self.device.plugins_classes.find(pi_class_name)
        if pi_class == nil        log("MTR: unknown class name '"+str(pi_class_name)+"' skipping", 2)  continue  end

        var pi = pi_class(self.device, ep, plugin_conf)
        plugins.push(pi)

        log(format("MTR:   endpoint = %5i type:%s%s", ep, pi_class_name, self.device.conf_to_log(plugin_conf)), 2)
      except .. as e, m
        log("MTR: Exception" + str(e) + "|" + str(m), 2)
      end
    end

    tasmota.publish_result('{"Matter":{"Initialized":1}}', 'Matter')
  end

  #############################################################
  # Autoconfigure device from template to map
  #
  # Applies only if there are no plugins already configured
  def autoconf_device_map()
    import json
    var m = {}

    # check if we have a light
    var endpoint = matter.START_ENDPOINT
    var light_present = 0

    import light
    var light_status = light.get(0)
    if light_status != nil
      var channels_count = size(light_status.find('channels', ""))
      light_present = 1
      if channels_count > 0
        if   channels_count == 1
          m[str(endpoint)] = {'type':'light1'}
          endpoint += 1
          # check if we have secondary Dimmer lights (SetOption68 1)
          var idx = 1
          var light_status_i
          while (light_status_i := light.get(idx)) != nil
            m[str(endpoint)] = {'type':'light1','light':idx + 1}    # arg is 1-based so add 1
            endpoint += 1
            light_present += 1
            idx += 1
          end
        elif channels_count == 2
          m[str(endpoint)] = {'type':'light2'}
          endpoint += 1
        elif channels_count == 3
          m[str(endpoint)] = {'type':'light3'}
          endpoint += 1
          # check if we have a split second light (SetOption37 128) with 4/5 channels
          var light_status1 = light.get(1)
          if (light_status1 != nil)
            var channels_count1 = size(light_status1.find('channels', ""))

            if (channels_count1 == 1)
              m[str(endpoint)] = {'type':'light1'}
              endpoint += 1
              light_present += 1
            elif (channels_count1 == 2)
              m[str(endpoint)] = {'type':'light2'}
              endpoint += 1
              light_present += 1
            end
          end
        elif channels_count == 4
          # not supported yet
        else # only option left is 5 channels
          # not supported yet
        end
      end
    end

    # handle shutters before relays (as we steal relays for shutters)
    var r_st13 = tasmota.cmd("Status 13", true)     # issue `Status 13`
    var relays_reserved = []                        # list of relays that are used for non-relay (shutters)
    log("MTR: Status 13 = "+str(r_st13), 3)

    if r_st13 != nil && r_st13.contains('StatusSHT')
      r_st13 = r_st13['StatusSHT']        # skip root
      # Shutter is enabled, iterate
      var idx = 0
      while true
        var k = 'SHT' + str(idx)                    # SHT is zero based
        if !r_st13.contains(k)   break     end           # no more SHTxxx
        var d = r_st13[k]
        log(format("MTR: '%s' = %s", k, str(d)), 3)
        var relay1 = d.find('Relay1', -1)           # relay base 1 or -1 if none
        var relay2 = d.find('Relay2', -1)           # relay base 1 or -1 if none

        if relay1 > 0    relays_reserved.push(relay1 - 1)    end   # mark relay1/2 as non-relays
        if relay2 > 0    relays_reserved.push(relay2 - 1)    end

        log(f"MTR: {relay1=} {relay2=}", 3)
        # is there tilt support
        var tilt_array = d.find('TiltConfig')
        var tilt_config = tilt_array && (tilt_array[2] > 0)
        # add shutter to definition
        m[str(endpoint)] = {'type': tilt_config ? 'shutter+tilt' : 'shutter', 'shutter':idx}
        endpoint += 1
        idx += 1
      end

    end

    # how many relays are present
    var relay_count = size(tasmota.get_power())
    var relay_index = 0         # start at index 0
    relay_count -= light_present  # last power(s) taken for lights

    while relay_index < relay_count
      if relays_reserved.find(relay_index) == nil   # if relay is actual relay
        m[str(endpoint)] = {'type':'relay','relay':relay_index + 1}     # Relay index start with 1
        endpoint += 1
      end
      relay_index += 1
    end

    # auto-detect sensors
    var sensors = json.load(tasmota.read_sensors())

    var sensors_list = self.autoconf_sensors_list(sensors)

    for s: sensors_list
      m[str(endpoint)] = s
      endpoint += 1
    end

    # tasmota.publish_result('{"Matter":{"Initialized":1}}', 'Matter')    # MQTT is not yet connected
    return m
  end

  #############################################################
  # Autoconfigure from sensors
  #
  # Returns an ordered list
  def autoconf_sensors_list(sensors)
    var k2l = self.device.k2l
    var ret = []
    # temperature sensors
    for k1: k2l(sensors)
      var sensor_2 = sensors[k1]
      if isinstance(sensor_2, map) && sensor_2.contains("Temperature")
        var temp_rule = k1 + "#Temperature"
        ret.push({'type':'temperature','filter':temp_rule})
      end
    end

    # pressure sensors
    for k1: k2l(sensors)
      var sensor_2 = sensors[k1]
      if isinstance(sensor_2, map) && sensor_2.contains("Pressure")
        var temp_rule = k1 + "#Pressure"
        ret.push({'type':'pressure','filter':temp_rule})
      end
    end

    # light sensors
    for k1: k2l(sensors)
      var sensor_2 = sensors[k1]
      if isinstance(sensor_2, map) && sensor_2.contains("Illuminance")
        var temp_rule = k1 + "#Illuminance"
        ret.push({'type':'illuminance','filter':temp_rule})
      end
    end

    # huidity sensors
    for k1: k2l(sensors)
      var sensor_2 = sensors[k1]
      if isinstance(sensor_2, map) && sensor_2.contains("Humidity")
        var temp_rule = k1 + "#Humidity"
        ret.push({'type':'humidity','filter':temp_rule})
      end
    end

    return ret
  end

end
matter.Autoconf = Matter_Autoconf
