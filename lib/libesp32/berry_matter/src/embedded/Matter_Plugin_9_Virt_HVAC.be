#
# Matter_Plugin_9_Virt_HVAC.be - implements a virtual IR HVAC thermostat
#
# Copyright (C) 2026  Stephan Hadinger & Theo Arends
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
# Matter 1.4.1 Virtual Variant - HVAC / IRHVAC Thermostat
#################################################################################
# TYPE: "v_hvac" | VIRTUAL: true
# Maps Matter Thermostat writes to a complete Tasmota IRHVAC JSON command.
#################################################################################

import matter

#@ solidify:Matter_Plugin_Virt_HVAC,weak

class Matter_Plugin_Virt_HVAC : Matter_Plugin_Thermostat
  static var TYPE = "v_hvac"
  static var DISPLAY_NAME = "v.HVAC"
  static var SCHEMA = "vendor|l:Vendor|t:t|h:IRHVAC vendor/protocol, for example MITSUBISHI_AC or DAIKIN|r:1"
  static var SCHEMA2 = "model|l:Model|t:i|h:IRHVAC model, usually -1|d:-1"
  static var SCHEMA3 = "min_temp|l:Min temp|t:i|h:Minimum setpoint in Celsius|d:16"
  static var SCHEMA4 = "max_temp|l:Max temp|t:i|h:Maximum setpoint in Celsius|d:30"
  static var SCHEMA5 = "fan|l:Fan|t:s|d:Auto|o:Auto:Auto,Min:Min,Low:Low,Medium:Medium,High:High,Max:Max"
  static var SCHEMA6 = "temp_filter|l:Temp sensor|t:t|h:Tasmota sensor filter, for example SHT3X#Temperature or DS18B20#Temperature"
  static var UPDATE_TIME = 5000
  static var VIRTUAL = true

  var ir_vendor
  var ir_model
  var ir_fan
  var local_temp_filter
  var local_temp_matcher

  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    if self.local_temp_matcher
      device.add_read_sensors_schedule(self.UPDATE_TIME)
    end
  end

  def parse_configuration(config)
    super(self).parse_configuration(config)
    var vendor = config.find("vendor", nil)
    self.ir_vendor = vendor == nil ? "" : str(vendor)
    self.ir_model = config.find("model", -1)
    self.ir_fan = config.find("fan", "Auto")
    self.local_temp_filter = config.find("temp_filter")
    if self.local_temp_filter
      self.local_temp_matcher = tasmota.Rule_Matcher.parse(self.local_temp_filter)
    end
    if self.ir_vendor == ""
      log("MTR: v.HVAC endpoint " + str(self.endpoint) + " missing IRHVAC vendor; commands will be ignored", 2)
    end
  end

  def thermostat_state_changed()
    self.send_irhvac()
  end

  def mode_to_irhvac()
    if self.shadow_system_mode == 4
      return "Heat"
    elif self.shadow_system_mode == 1
      return "Auto"
    elif self.shadow_system_mode == 7
      return "Fan"
    elif self.shadow_system_mode == 8
      return "Dry"
    else
      return "Cool"
    end
  end

  def thermostat_feature_map()
    var feature_map = super(self).thermostat_feature_map()
    if self.local_temp_matcher == nil
      feature_map = feature_map | 0x40                # LTNE: LocalTemperatureNotExposed
    end
    return feature_map
  end

  def send_irhvac()
    if self.ir_vendor == nil || self.ir_vendor == ""
      return nil
    end

    import json
    var power = self.shadow_system_mode == 0 ? "Off" : "On"
    var temp = int(self.active_setpoint() / 100)
    var payload = {
      "Vendor": self.ir_vendor,
      "Model": self.ir_model,
      "Power": power,
      "Mode": self.mode_to_irhvac(),
      "Celsius": "On",
      "Temp": temp,
      "FanSpeed": self.ir_fan,
      "SwingV": "Off",
      "SwingH": "Off",
      "Quiet": self.option_state("Quiet"),
      "Turbo": self.option_state("Turbo"),
      "Econo": self.option_state("Econo"),
      "Light": self.option_state("Light"),
      "Filter": self.option_state("Filter"),
      "Clean": self.option_state("Clean"),
      "Beep": self.option_state("Beep"),
      "iFeel": self.option_state("iFeel"),
      "Sleep": -1
    }
    var cmd = "IRHVAC " + json.dump(payload)
    log("MTR: v.HVAC " + cmd, 2)
    var resp = tasmota.cmd(cmd, true)
    if isinstance(resp, map)
      var irhvac_resp = resp.find("IRHVAC", nil)
      if type(irhvac_resp) == 'string'
        log("MTR: v.HVAC endpoint " + str(self.endpoint) + " IRHVAC rejected vendor '" + self.ir_vendor + "': " + irhvac_resp, 2)
      end
    end
    return resp
  end

  def option_state(option_name)
    for pl: self.device.plugins
      if pl.TYPE == "v_hvac_option" && pl.hvac_ep == self.endpoint && pl.option_name == option_name
        return pl.shadow_onoff ? "On" : "Off"
      end
    end
    return "Off"
  end

  def read_attribute(session, ctx, tlv_solo)
    if ctx.cluster == 0x001D && ctx.attribute == 0x0003
      var TLV = matter.TLV
      var parts = TLV.Matter_TLV_array()
      for pl: self.device.plugins
        if pl.TYPE == "v_hvac_option" && pl.hvac_ep == self.endpoint
          parts.add_TLV(nil, 0x05 #-TLV.U2-#, pl.endpoint)
        end
      end
      return parts
    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  def update_virtual(payload)
    super(self).update_virtual(payload)
    self.send_irhvac()
  end

  def parse_sensors(payload)
    if self.local_temp_matcher
      var val = self.local_temp_matcher.match(payload)
      if isinstance(val, map)
        val = val.find("Temperature")
      end
      if val != nil
        val = real(val)
        if tasmota.get_option(8) == 1
          val = (val - 32) / 1.8
        end
        val = int(val * 100)
        if val != self.shadow_local_temperature
          self.shadow_local_temperature = val
          self.attribute_updated(0x0201, 0x0000)
        end
      end
    end
    super(self).parse_sensors(payload)
  end

end
matter.Plugin_Virt_HVAC = Matter_Plugin_Virt_HVAC
