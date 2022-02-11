#-------------------------------------------------------------
 - Generic driver for AXP202 - solidified
 -------------------------------------------------------------#
class AXP202 : I2C_Driver
  def init()
    super(self, I2C_Driver).init("AXP202", 0x35)
  end

  # Return True = Battery Exist
  def battery_present()
    if self.wire.read(self.addr, 0x01, 1) & 0x20 return true
    else return false
    end
  end

  # Input Power Status ???
  def get_input_power_status()
    return self.wire.read(self.addr, 0x00, 1)
  end

  # Battery Charging Status
  def get_battery_chargin_status()
    return self.wire.read(self.addr, 0x01, 1)
  end

  # AXP chip temperature in °C
  def get_temp()
    return self.read12(0x5E) * 0.1 - 144.7
  end

  def get_bat_power()
    return self.read24(0x70) * 0.00055
  end

  def get_bat_voltage()
    return self.read12(0x78) * 0.0011
  end
  def get_bat_current()
    return (self.read13(0x7A) - self.read13(0x7C)) * 0.5
  end
  def get_bat_charge_current()
    return self.read13(0x7A) * 0.5
  end
  def get_aps_voltage()
    return self.read12(0x7E) * 0.0014
  end
  def get_vbus_voltage()
    return self.read12(0x5A) * 0.0017
  end
  def get_vbus_current()
    return self.read12(0x5C) * 0.375
  end

  # set LDO voltage
  #   ldo: 2/3/4
  #   voltage: (mV) 1800mV - 3300mV in 100mV steps
  def set_ldo_voltage(ldo, voltage)
    if voltage > 3300   voltage = 15 end

    if ldo == 2 || ldo == 4
      voltage = (voltage / 100) - 18
      if ldo == 2
        self.write8(0x28, self.read8(0x28) & 0x0F | ((voltage & 0x0F) << 4))
      else
        self.write8(0x28, self.read8(0x28) & 0xF0 | (voltage & 0x0F))
      end
    else
      # ldo 3, range is 0v7~3v3
      voltage = (voltage / 25) - 28
      self.write8(0x28, self.read8(0x29) & 0x80 | (voltage & 0x7F))
    end
  end

  # set EXTEN which enables external power on M5Stack, powering Hat with 5V
  def set_exten(state)
    self.write_bit(0x12, 0, state)
  end

  # VBUS current-limit selection set to not limit
  def set_limiting_off()
    self.write8(0x30, self.read8(0x30) | 0x03)
  end

  # set DCDC enable, 2/3
  def set_dcdc_enable(dcdc, state)
    # if dcdc == 1  self.write_bit(0x12, 0, state) end # no dcdc1 on AXP202
    if dcdc == 2  self.write_bit(0x12, 4, state) end
    if dcdc == 3  self.write_bit(0x12, 1, state) end
  end

  # set LDO enable, 2/3/4 (LDO 1 is always on)
  def set_ldo_enable(ldo, state)
    if ldo == 2   self.write_bit(0x12, 2, state) end
    if ldo == 3   self.write_bit(0x12, 6, state) end
    if ldo == 4   self.write_bit(0x12, 3, state) end
  end

#   # set GPIO output state 0/1/2 and 3/4
#   def write_gpio(gpio, state)
#     if   gpio >= 0 && gpio <= 2
#       self.write_bit(0x94, gpio, state)
#     elif gpio >= 3 && gpio <= 4
#       self.write_bit(0x96, gpio - 3, state)
#     end
#   end

  # Set voltage on DC-DC 2/3
  #   dcdc: 2/3 (warning some C libs start at 0)
  #   voltage: 
  def set_dc_voltage(dcdc, voltage)
    if dcdc < 2 || dcdc > 3 return end
    var v
    if voltage < 700    v = 0
    elif voltage > 3500 v = 112
    elif dcdc == 2 && voltage > 2275  v = 63  # dcdc2 is limited to 2.275V
    else v = (voltage - 700) / 25
    end

    var            addr = 0x23
    if   dcdc == 3 addr = 0x27
    # elif dcdc == 2 addr = 0x23
    end

    self.write8(addr, self.read8(addr) & 0x80 | (v & 0x7F))
  end

  # Set charging current
  # current = 300+(val)*100 mA, range 300-1800
  def set_chg_current_ma(ma)
    var current_code = (ma - 300) / 100
    self.write8(0x33, self.read8(0x33) & 0xF0 | (current_code & 0x0F))
  end

  # set shutdown time, possible values: 4, 6, 8, 10
  def set_shutdown_time(st)
    var val = 0     # 4s
    if st == 6    val = 1 end
    if st == 8    val = 2 end
    if st == 10   val = 3 end
    self.write8(0x36, self.read8(0x36) & 0xFC | (val & 0x03))
  end

  # change charging led mode
  # 0=off, 1=blink_1Hz, 2=blink_4Hz, 3=low_level
  def set_chg_led_mode(mode)
    var val = self.read8(0x32)
    val = (val & 0xCF) | 0x08
    val = val | ((mode & 0x03) << 4)
    self.write8(0x32, val)
  end

#   # // Low Volt Level 1, when APS Volt Output < 3.4496 V
#   # // Low Volt Level 2, when APS Volt Output < 3.3992 V, then this flag is SET (0x01)
#   # // Flag will reset once battery volt is charged above Low Volt Level 1
#   # // Note: now AXP192 have the Shutdown Voltage of 3.0V (B100) Def in REG 31H
#   def get_warning_level()
#     return self.read12(0x47) & 1
#   end

#   #- display sensor value in the web UI -#
#   def web_sensor()
#     if !self.wire return nil end  #- exit if not initialized -#
#     import string
#     var msg = string.format(
#              "{s}VBus Voltage{m}%.3f V{e}"..
#              "{s}VBus Current{m}%.1f mA{e}"..
#              "{s}Batt Voltage{m}%.3f V{e}"..
#              "{s}Batt Current{m}%.1f mA{e}"..
#              #"{s}Batt Power{m}%.3f{e}"..
#              "{s}Temp AXP{m}%.1f &deg;C{e}",
#              self.get_vbus_voltage(), self.get_vbus_voltage(),
#              self.get_bat_voltage(), self.get_bat_current(),
#              #self.get_bat_power(),
#              self.get_temp()
#             )
#     tasmota.web_send_decimal(msg)
#   end

#   #- add sensor value to teleperiod -#
#   def json_append()
#     if !self.wire return nil end  #- exit if not initialized -#
#     # import string
#     # var ax = int(self.accel[0] * 1000)
#     # var ay = int(self.accel[1] * 1000)
#     # var az = int(self.accel[2] * 1000)
#     # var msg = string.format(",\"MPU6886\":{\"AX\":%i,\"AY\":%i,\"AZ\":%i,\"GX\":%i,\"GY\":%i,\"GZ\":%i}",
#     #           ax, ay, az, self.gyro[0], self.gyro[1], self.gyro[2])
#     # tasmota.response_append(msg)
#   end
end
