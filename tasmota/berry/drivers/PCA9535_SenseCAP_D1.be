#################################################################################
# Specialized driver for SeedStudio SenseCAP D1 display
#################################################################################

import PCA9535

class PCA9535_SenseCAP_D1 : PCA9535
  def init()
    super(self).init(0x20)

    if self.wire
      self.write_gpio(0x05, 1)            # set IO0.5 (LCD_RESET) high
      self.write_gpio(0x07, 1)            # set IO0.7 (TOUCH_RESET) high
      self.write_gpio(0x08, 1)            # set IO1.0 (RP2040_RESET) high
      self.config_gpio(0x05, 0)           # configure IO0.5 (LCD_RESET) as output
      self.config_gpio(0x07, 0)           # configure IO0.7 (TOUCH_RESET) as output
      self.config_gpio(0x08, 0)           # configure IO1.0 (RP2040_RESET) as output
      # reset display at boot
      self.reset_display()
    end
  end

  # hardware reset for the MCU RP2040
  def reset_RP2040()
    self.write_gpio(0x08, 0)              # drive RESET Low
    tasmota.delay(2)                      # the recommended delay is 1ms, we take some margin
    self.write_gpio(0x08, 1)              # drive RESET High
  end

  # reset both display and touch screen
  def reset_display()
    self.write_gpio(0x05, 0)
    self.write_gpio(0x07, 0)
    tasmota.delay(2)
    self.write_gpio(0x05, 1)
    self.write_gpio(0x07, 1)
    tasmota.delay(50)
  end

end

return PCA9535_SenseCAP_D1()
