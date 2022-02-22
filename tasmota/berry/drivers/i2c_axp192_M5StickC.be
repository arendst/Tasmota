#-------------------------------------------------------------
 - Specialized driver for AXP192 of M5StickC
 -------------------------------------------------------------#
class AXP192_M5StickC : AXP192
  def init()
    super(self, AXP192).init()
    if self.wire
      # Disable vbus hold limit
      self.write8(0x30, 0x80)

      # lcd backlight voltage was set to 2.80v
      self.set_lcd_voltage(2800)

      # set LDO3 3.0V TFT
      self.set_ldo_voltage(3, 3000)

      # Set ADC sample rate to 200hz
      self.write8(0x84, 0xF2)

      # set charging current to 100mA
      self.set_chg_current(0) # 0 = 100mA
      
      # esp32 power voltage was set to 3.3v
      self.set_esp_voltage(3300)

      # PEK key parameter setting
      # Current:
      #   Boot time setting = 512ms
      #   Long press time setting = 1s
      #   Automatic shutdown function setting when the key duration is longer than the shutdown duration = turn on
      #   PWROK signal delay after power on = 64ms
      #   Shutdown duration setting = 4s
      self.write8(0x36, 0x4C)

      # Mic GPIO0 - floating
      self.write8(0x90, 0x07)

      # enable LDO2, LDO3, DCDC1, DCDC3
      self.set_ldo_enable(2, true)
      self.set_ldo_enable(3, true)
      self.set_dcdc_enable(1, true)
      self.set_dcdc_enable(3, true)

      # enable external power on HAT connector (5V)
      self.set_exten(true)

      # Set temperature protection
      self.write8(0x39, 0xFC)

      # Enable RTC BAT charge
      self.write8(0x35, 0xA2)
      # Write1Byte(0x35, 0xa2 & (disableRTC ? 0x7F : 0xFF));

      # Enable bat detection
      self.write8(0x32, 0x46)

      # Set Power off voltage 3.0v
      self.write8(0x31, self.read8(0x31) & 0xF8 | 0x04)
      
      # ADC all-on
      # Bit 7: Battery voltage ADC enable
      # Bit 6: Battery current ADC enable
      # Bit 5: ACIN voltage ADC enable
      # Bit 4: ACIN current ADC enable
      # Bit 3: VBUS voltage ADC enable
      # Bit 2: VBUS current ADC enable
      # Bit 1: APS voltage ADC enable
      # Bit 0: TS pin ADC function enable
      self.write8(0x82, 0xFF)
    end
  end

  # set ESP voltage on DCDC1
  def set_esp_voltage(voltage)
    if voltage >= 3000 && voltage <= 3400
      self.set_dc_voltage(1, voltage)
    end
  end

  # set LCD backlight voltage on DCDC3
  def set_lcd_voltage(voltage)
    if voltage < 2500  voltage = 2500 end
    if voltage > 3300  voltage = 3300 end
    self.set_ldo_voltage(2, voltage)
  end

  # LCD Controller Reset pin
  def set_lcd_reset(state)
    self.set_ldo_enable(3, state)
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

axp = AXP192_M5StickC()
tasmota.add_driver(axp)
