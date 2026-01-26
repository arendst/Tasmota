#-------------------------------------------------------------
 - Specialized driver for AXP192 of M5Stack Core 2
 -------------------------------------------------------------#
class AXP192_M5Stack_Core2 : AXP192
  def init()
    super(self).init()

    if self.wire
      # Disable vbus hold limit
      self.write8(0x30, self.read8(0x30) & 0x04 | 0x02)

      # AXP192 GPIO1:OD OUTPUT
      self.write8(0x92, self.read8(0x92) & 0xF8)

      # AXP192 GPIO2:OD OUTPUT
      self.write8(0x93, self.read8(0x93) & 0xF8)

      # AXP192 RTC CHG rtc battery charging enabled
      self.write8(0x35, self.read8(0x35) & 0x1C | 0xA2)

      # esp32 power voltage was set to 3.35v
      self.set_esp_voltage(3350)

      # lcd backlight voltage was set to 2.80v
      self.set_lcd_voltage(2800)

      # lcd logic and sdcard voltage preset to 3.3v
      self.set_ldo_voltage(2, 3300)

      # vibrator voltage preset to 2v
      self.set_ldo_voltage(3, 2000)

      # turn on logic and sdcard
      self.set_ldo_enable(2, true)

      # turn on lcd backlight
      self.set_dcdc_enable(3, true)

      # turn green led on
      self.set_led(true)
  
      # set charging current to 100mA
      self.set_chg_current(0) # 0 = 100mA

      # AXP192 GPIO4 - TBC what this does really?
      self.write8(0x95, self.read8(0X95) & 0x72 | 0x84)
  
      # Button parameter setting
      # Current:
      #   Boot time = 512ms
      #   Long key press time setting = 1s
      #   Automatic shutdown function setting when the key duration is longer than the shutdown duration = turn on
      #   PWROK signal delay after power on = 64ms
      #   Shutdown duration setting = 4s
      self.write8(0x36, 0x4C)
  
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

      # Disable audio as ethernet uses the same GPIO
      self.write_gpio(2, 0)

      # Reset LCD Controller
      self.set_lcd_reset(false)
      tasmota.delay(100)   # wait for 100ms
      self.set_lcd_reset(true)
      tasmota.delay(200)   # wait for 200ms

      # Indicates whether the voltage of VBUS is higher than VHOLD when the external power supply VBUS is connected
      if self.read8(0X00) & 0x08
        self.write_bit(0x30, 7, true)
        # if v-bus can use, disable M-Bus 5V output to input
        self.set_buf_power_mode(true)
      else
        # if not, enable M-Bus 5V output
        self.set_buf_power_mode(false)
      end

      tasmota.add_driver(self)
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
    self.set_dc_voltage(3, voltage)
  end

  # set state of the green led, GPIO 1
  def set_led(state)
    self.write_gpio(1, !state)
  end

  # LCD Controller Reset pin
  def set_lcd_reset(state)
    self.write_gpio(4, state)
  end

  # Speaker enable
  def set_speaker_enable(state)
    self.write_gpio(2, state)
  end

  # Bus Power Mode
  def set_buf_power_mode(state)
    if (state)
      self.write8(0x12, self.read8(0x12) & 0xBF)          # set EXTEN to disable 5v boost
      self.write8(0x90, self.read8(0x90) & 0xF8 | 0x01)   # set GPIO0 to float, using enternal pulldown resistor to enable supply from BUS_5VS
    else
      self.write8(0x91, self.read8(0x91) & 0x0F | 0xF0)   # Set GPIO to 3.3V (LDO OUTPUT mode)
      self.write8(0x90, self.read8(0x90) & 0xF8 | 0x02)   # set GPIO0 to LDO OUTPUT , pullup N_VBUSEN to disable supply from BUS_5V
      self.write8(0x12, self.read8(0x12) | 0x40)          # set EXTEN to enable 5v boost
    end
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

  # respond to audio events
  def audio(cmd, idx, payload, raw)
    if cmd == "power"
      self.set_speaker_enable(idx ? 1 : 0)
    end
  end

end

return AXP192_M5Stack_Core2()
