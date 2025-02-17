#-------------------------------------------------------------
 - Generic driver for AXP2102 - solidified
 -------------------------------------------------------------#

# Specs
# DCDC1 : 1.5-3.4V，                      2000mA
# DCDC2 : 0.5-1.2V，1.22-1.54V,           2000mA
# DCDC3 : 0.5-1.2V，1.22-1.54V, 1.6-3.4V, 2000mA
# DCDC4 : 0.5-1.2V, 1.22-1.84V,           1500mA
# DCDC5 : 1.2V    , 1.4-3.7V,             1000mA
#
# RTCLDO1/2 : 1.8V/2.5V/3V/3.3V,            30mA
# ALDO1~4 : 0.5-3.5V, 100mV/step            300mA

# LDO index:
# 0=ALDO1 ~ 3=ALDO4 / 4=BLDO1 / 5=BLDO2

 #@ solidify:AXP2102
class AXP2102 : I2C_Driver
  def init(addr)
    if (addr == nil)  addr = 0x34   end   # default address is 0x34
    super(self).init("AXP2102", addr)
  end

  # Return True = Battery Exist
  def battery_present()
    return bool(self.wire.read(self.addr, 0x00, 1) & 0x08)
  end

  # # Battery Charging Status
  def get_battery_charging_status()
    var val = (self.read8(0x01) >> 5) & 0x03
    # 1:charge / 2:dischage / 0:stanby
    return (val == 1) ? 1 : ((val == 2) ? -1 : 0)
  end

  def is_charging()
    return (self.read8(0x01) & 0x60) == 0x20
  end 
  
  def get_battery_level()
    return self.read8(0xA4)
  end

  # AXP chip temperature in °C
  def get_temp()
    return 22 + ((7274 - self.read16(0x3C)) / 20);
  end

  def get_bat_power()
    return 0
  end

  def get_bat_voltage()
    return self.read14(0x34) / 1000.0
  end
  def get_bat_current()
    return 0
  end
  def get_bat_charge_current()
    return 0
  end
  def get_aps_voltage()
    return 0
  end
  def get_vbus_good()
    return bool(self.wire.read(self.addr, 0x00, 1) & 0x20)
  end
  def get_vbus_voltage()
    if (!self.get_vbus_good()) return 0   end
    var vbus = self.read14(0x38)
    if (vbus > 16375)   return 0    end
    return vbus / 1000.0
  end
  def get_vbus_current()
    return 0
  end

  # set LDO voltage
  #   0=ALDO1 ~ 3=ALDO4 / 4=BLDO1 / 5=BLDO2
  #   voltage: (mV) 0.5-3.5V in 100mV steps
  def set_ldo_voltage(num, voltage)
    if (num < 0 || num > 5)    return  end
    var reg_volt = num + 0x92
    voltage -= 500
    # convert voltage to value
    var val = (voltage < 0) ? 0 : voltage / 100
    if (val > 0x1E)   val = 0x1E    end
    self.write8(reg_volt, val)

    var reg90bit = 1 << num
    if (voltage < 0)
      self.write8(0x90, self.read8(0x90) & ~reg90bit)    # bit off
    else
      self.write8(0x90, self.read8(0x90) |  reg90bit)    # bit on
    end
  end

  # det DLDO voltage
  def set_dldo_voltage(num, voltage)
    if (num < 0 || num > 1)    return  end
    var reg_volt = num + 0x99
    voltage -= 500
    # convert voltage to value
    var val = (voltage < 0) ? 0 : voltage / (num ? 50 : 100)
    if (val > (num ? 0x13 : 0x1C))  val = num ? 0x13 : 0x1C   end
    self.write8(reg_volt, val)

    var reg = 0x90 + num
    var bit = num ? 0x01 : 0x80
    if (voltage < 0)
      self.write8(0x90, self.read8(reg) & ~bit)    # bit off
    else
      self.write8(0x90, self.read8(reg) |  bit)    # bit on
    end
  end

  # get LDO enable
  def get_ldo_enable(num)
    var res = false
    if (num <= 5)
      var reg90bit = 1 << num
      res = bool(self.read8(0x90) & reg90bit)
    end
    return res
  end

  def set_battery_charge(enable)
    self.write8(0x18, (self.read8(0x18) & 0xFD) | ((enable ? 1 : 0) << 1))
  end

  static var _pre_charge = [0, 25, 50, 75, 100, 125, 150, 175, 200, 255]
  def set_pre_charge_current(max_mA)
    if (max_mA < 0)     max_mA = 0      end
    if (max_mA > 200)   max_mA = 200    end
    var i = 0
    while (self._pre_charge[i] <= max_mA)
      i += 1
    end
    self.write8(0x61, i)
  end

  static var _charge = [ 25, 30, 35, 40, 60, 80, 100, 120, 140, 160, 180, 200, 255]
  def set_charge_current(max_mA)
    if (max_mA < 0)     max_mA = 0      end
    if (max_mA > 200)   max_mA = 200    end
    var i = 0
    while (self._charge[i] <= max_mA)
      i += 1
    end
    i += 4
    self.write8(0x62, i)
  end

  static var _charge_voltage = [410, 420, 435, 440, 460, 655]
  def set_charge_voltage(max_mV)
    if (max_mV < 0)     max_mV = 0    end
    if (max_mV > 460)   max_mV = 460  end
    var i = 0
    while (self._charge_voltage[i] <= max_mV)
      i += 1
    end
    i += 1
    if (i >= 6)   i = 0   end
    self.write8(0x64, i)
  end

end
return AXP2102
