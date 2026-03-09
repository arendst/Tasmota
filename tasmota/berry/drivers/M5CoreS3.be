#-------------------------------------------------------------
 - Specialized driver for M5CoreS3 using AXP2101 and AW9523
 -------------------------------------------------------------#
class M5CoreS3 : AXP2102
  def init()
    super(self).init()

    if self.wire

      # From https://github.com/m5stack/M5Unified/blob/b8cfec7fed046242da7f7b8024a4e92004a51ff7/src/utility/Power_Class.cpp#L61
      # M5.In_I2C.bitOn(aw9523_i2c_addr, 0x03, 0b10000000, i2c_freq);  // SY7088 BOOST_EN
      var aw9523 = I2C_Driver("AW9523", 0x58)
      aw9523.write8(0x03, aw9523.read8(0x03) | 0x80)  # SY7088 BOOST_EN

      # From https://github.com/m5stack/M5Unified/blob/b8cfec7fed046242da7f7b8024a4e92004a51ff7/src/utility/Power_Class.cpp#L62
      # _pmic = Power_Class::pmic_t::pmic_axp2101;
      # Axp2101.begin();
      # static constexpr std::uint8_t reg_data_array[] =
      # { 0x90, 0xBF  // LDOS ON/OFF control 0
      # , 0x92, 18 -5 // ALDO1 set to 1.8v // for AW88298
      # , 0x93, 33 -5 // ALDO2 set to 3.3v // for ES7210
      # , 0x94, 33 -5 // ALDO3 set to 3.3v // for camera
      # , 0x95, 33 -5 // ALDO3 set to 3.3v // for TF card slot
      # , 0x27, 0x00 // PowerKey Hold=1sec / PowerOff=4sec
      # , 0x69, 0x11 // CHGLED setting
      # , 0x10, 0x30 // PMU common config
      # , 0x30, 0x0F // ADC enabled (for voltage measurement)
      # };
      self.write8(0x90, 0xBF)     # LDOS ON/OFF control 0
      self.write8(0x92, 18 -5)    # ALDO1 set to 1.8v // for AW88298
      self.write8(0x93, 33 -5)    # ALDO2 set to 3.3v // for ES7210
      self.write8(0x94, 33 -5)    # ALDO3 set to 3.3v // for camera
      self.write8(0x95, 33 -5)    # ALDO4 set to 3.3v // for TF card slot
      self.write8(0x27, 0x00)     # PowerKey Hold=1sec / PowerOff=4sec
      self.write8(0x69, 0x11)     # CHGLED setting
      self.write8(0x10, 0x30)     # PMU common config
      self.write8(0x30, 0x0F)     # ADC enabled (for voltage measurement)

      var chk_aw = aw9523.read8(0x10)
      if chk_aw == 0x23
        var result = 0
        var reg0x02 = (result == 0) ? 0x07 : 0x05;
        var reg0x03 = (result == 0) ? 0x83 : 0x03;
        aw9523.write8(0x02, aw9523.read8(0x02) | reg0x02)  # port0 output ctrl
        aw9523.write8(0x03, aw9523.read8(0x03) | reg0x03)  # port1 output ctrl
        aw9523.write8(0x04, 0x18)  # CONFIG_P0
        aw9523.write8(0x05, 0x0C)  # CONFIG_P1
        aw9523.write8(0x11, 0x10)  # GCR P0 port is Push-Pull mode.
        aw9523.write8(0x12, 0xFF)  # LEDMODE_P0
        aw9523.write8(0x13, 0xFF)  # LEDMODE_P1

        aw9523.write8(0x03, aw9523.read8(0x03) & ~0x02)  # LCD_RST)
#        delay(10)
        aw9523.write8(0x03, aw9523.read8(0x03) | 0x02)  # LCD_RST)
      end

      tasmota.add_driver(self)
    end
  end

  # set LCD backlight voltage on DLDO1
  def set_lcd_voltage(voltage)
    if (voltage < 2500)  voltage = 2500 end
    if (voltage > 3300)  voltage = 3300 end
    self.set_dldo_voltage(0, voltage)   # 0=DLD01
  end

  # Speaker enable
  def set_speaker_enable(state)
    self.set_ldo_voltage(2, state ? 3300 : 0)   # 2 = ALDO3
  end

  # Dimmer in percentage
  def set_displaydimmer(x)
    var v = tasmota.scale_uint(x, 0, 100, 2500, 3300)
    self.set_lcd_voltage(v)
  end

  # respond to display events
  def display(cmd, idx, payload, raw)
    if cmd == "dim" || cmd == "power"
      self.set_displaydimmer(idx)
    end
  end

end

return M5CoreS3()
