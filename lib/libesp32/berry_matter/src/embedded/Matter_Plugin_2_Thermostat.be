#
# Matter_Plugin_2_Thermostat.be - implements a generic Thermostat
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
# Matter 1.4.1 Device Specification - Thermostat (0x0301)
#################################################################################
# CLUSTERS (Server):
# - 0x0201: Thermostat
#
# MVP:
# - LocalTemperature, OccupiedCoolingSetpoint, OccupiedHeatingSetpoint
# - ControlSequenceOfOperation, SystemMode, ThermostatRunningState
# - Local shadow state only; subclasses can publish native commands
#################################################################################

import matter

#@ solidify:Matter_Plugin_Thermostat,weak

class Matter_Plugin_Thermostat : Matter_Plugin_Device
  static var TYPE = "thermostat"
  static var DISPLAY_NAME = "Thermostat"
  static var CLUSTERS = matter.consolidate_clusters(_class, {
    0x0201: [0,0x11,0x12,0x1B,0x1C,0x29],        # Thermostat
  })
  static var UPDATE_COMMANDS = matter.UC_LIST(_class, "SystemMode", "Temp", "CoolSetpoint", "HeatSetpoint")
  static var TYPES = { 0x0301: 4 }                # Thermostat - Matter 1.4.1 Device Library Rev 4

  var shadow_local_temperature
  var shadow_cooling_setpoint
  var shadow_heating_setpoint
  var shadow_system_mode
  var shadow_running_state
  var min_temp
  var max_temp

  #############################################################
  # Thermostat SystemMode:
  #   0: Off
  #   1: Auto
  #   3: Cool
  #   4: Heat
  #   7: Fan only
  #   8: Dry

  def init(device, endpoint, config)
    super(self).init(device, endpoint, config)
    if self.min_temp == nil   self.min_temp = 1600   end
    if self.max_temp == nil   self.max_temp = 3000   end
    self.shadow_local_temperature = config.find("local_temp", nil)
    if self.shadow_local_temperature != nil
      self.shadow_local_temperature = int(self.shadow_local_temperature) * 100
    end
    self.shadow_cooling_setpoint = self.clamp_temp(int(config.find("cool_temp", 2400)))
    self.shadow_heating_setpoint = self.clamp_temp(int(config.find("heat_temp", 2000)))
    self.shadow_system_mode = int(config.find("mode", 0))
    self.shadow_running_state = self.compute_running_state()
  end

  def parse_configuration(config)
    super(self).parse_configuration(config)
    self.min_temp = int(config.find("min_temp", 16)) * 100
    self.max_temp = int(config.find("max_temp", 30)) * 100
  end

  def clamp_temp(temp)
    temp = int(temp)
    if temp < self.min_temp   temp = self.min_temp   end
    if temp > self.max_temp   temp = self.max_temp   end
    return temp
  end

  def active_setpoint()
    if self.shadow_system_mode == 4
      return self.shadow_heating_setpoint
    else
      return self.shadow_cooling_setpoint
    end
  end

  def compute_running_state()
    if self.shadow_system_mode == 3
      return 0x0002                                   # Cool Stage 1
    elif self.shadow_system_mode == 4
      return 0x0001                                   # Heat Stage 1
    elif self.shadow_system_mode == 7
      return 0x0004                                   # Fan
    elif self.shadow_system_mode == 8
      return 0x0004                                   # Dry: closest exposed state is fan
    else
      return 0
    end
  end

  def thermostat_feature_map()
    return 0x23                                       # HEAT + COOL + AUTO
  end

  def set_system_mode(mode)
    mode = int(mode)
    if mode != 0 && mode != 1 && mode != 3 && mode != 4 && mode != 7 && mode != 8
      return false
    end
    if mode != self.shadow_system_mode
      self.shadow_system_mode = mode
      self.attribute_updated(0x0201, 0x001C)
      var running_state = self.compute_running_state()
      if running_state != self.shadow_running_state
        self.shadow_running_state = running_state
        self.attribute_updated(0x0201, 0x0029)
      end
    end
    return true
  end

  def set_cooling_setpoint(temp)
    temp = self.clamp_temp(temp)
    if temp != self.shadow_cooling_setpoint
      self.shadow_cooling_setpoint = temp
      self.attribute_updated(0x0201, 0x0011)
    end
  end

  def set_heating_setpoint(temp)
    temp = self.clamp_temp(temp)
    if temp != self.shadow_heating_setpoint
      self.shadow_heating_setpoint = temp
      self.attribute_updated(0x0201, 0x0012)
    end
  end

  def read_attribute(session, ctx, tlv_solo)
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    if cluster == 0x0201                              # Thermostat
      self.update_shadow_lazy()
      if attribute == 0xFFFC                          # FeatureMap / map32
        return tlv_solo.set(0x06 #-TLV.U4-#, self.thermostat_feature_map())
      elif attribute == 0x0000                        # LocalTemperature / int16
        if self.shadow_local_temperature == nil
          return tlv_solo.set(0x14 #-TLV.NULL-#, nil)
        else
          return tlv_solo.set(0x01 #-TLV.I2-#, self.shadow_local_temperature)
        end
      elif attribute == 0x0011                        # OccupiedCoolingSetpoint / int16
        return tlv_solo.set(0x01 #-TLV.I2-#, self.shadow_cooling_setpoint)
      elif attribute == 0x0012                        # OccupiedHeatingSetpoint / int16
        return tlv_solo.set(0x01 #-TLV.I2-#, self.shadow_heating_setpoint)
      elif attribute == 0x001B                        # ControlSequenceOfOperation / enum8
        return tlv_solo.set(0x04 #-TLV.U1-#, 4)       # Cooling and heating
      elif attribute == 0x001C                        # SystemMode / enum8
        return tlv_solo.set(0x04 #-TLV.U1-#, self.shadow_system_mode)
      elif attribute == 0x0029                        # ThermostatRunningState / bitmap16
        return tlv_solo.set(0x05 #-TLV.U2-#, self.shadow_running_state)
      end
    end
    return super(self).read_attribute(session, ctx, tlv_solo)
  end

  def write_attribute(session, ctx, write_data)
    var cluster = ctx.cluster
    var attribute = ctx.attribute

    if cluster == 0x0201                              # Thermostat
      self.update_shadow_lazy()
      if attribute == 0x0011                          # OccupiedCoolingSetpoint
        if type(write_data) == 'int'
          self.set_cooling_setpoint(write_data)
          self.thermostat_state_changed()
          self.publish_command('CoolSetpoint', self.shadow_cooling_setpoint, 'Temp', self.active_setpoint(), nil, nil)
          return true
        end
      elif attribute == 0x0012                        # OccupiedHeatingSetpoint
        if type(write_data) == 'int'
          self.set_heating_setpoint(write_data)
          self.thermostat_state_changed()
          self.publish_command('HeatSetpoint', self.shadow_heating_setpoint, 'Temp', self.active_setpoint(), nil, nil)
          return true
        end
      elif attribute == 0x001C                        # SystemMode
        if type(write_data) == 'int' && self.set_system_mode(write_data)
          self.thermostat_state_changed()
          self.publish_command('SystemMode', self.shadow_system_mode, 'Temp', self.active_setpoint(), nil, nil)
          return true
        end
      end
      ctx.status = 0x87 #-matter.CONSTRAINT_ERROR-#
      return false
    end
    return nil
  end

  def thermostat_state_changed()
  end

  def update_virtual(payload)
    var val_mode = payload.find("SystemMode")
    if val_mode != nil
      self.set_system_mode(int(val_mode))
    end
    var val_cool = payload.find("CoolSetpoint")
    if val_cool != nil
      self.set_cooling_setpoint(int(val_cool))
    end
    var val_heat = payload.find("HeatSetpoint")
    if val_heat != nil
      self.set_heating_setpoint(int(val_heat))
    end
    var val_temp = payload.find("Temp")
    if val_temp != nil
      self.set_cooling_setpoint(int(val_temp))
      self.set_heating_setpoint(int(val_temp))
    end
    var val_local = payload.find("LocalTemperature")
    if val_local != nil
      val_local = int(val_local)
      if val_local != self.shadow_local_temperature
        self.shadow_local_temperature = val_local
        self.attribute_updated(0x0201, 0x0000)
      end
    end
  end

end
matter.Plugin_Thermostat = Matter_Plugin_Thermostat
