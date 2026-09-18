#################################################################################
# Specialized driver for M5Stack UnitC6L Lora
#
# in preinit.be: import pi4ioe5v6408_M5Stack_UnitC6L
#################################################################################

import gpio

class PI4IOE5V6408_UnitC6L : I2C_Driver
  var virtual_buttons

  def init(addr)
    if (addr == nil) 
      addr = 0x43
    end
    super(self, I2C_Driver).init("PI4IOE5V6408", addr)

    if self.wire
      var id = self.wire.read(self.addr, 0x01, 1)
      if id == 0
        self.wire = nil

        log("I2C: PI4IOE5V6408 not detected", 3)

        return                      # wrong device
      end

      self.virtual_buttons = [-1, -1]

      # M5Unified src/utility/Power_Class.cpp reg_data_array_for_lorac6
      #  PI4IO E0
      #  P0 (UnitC6L + NessoN1:BTN1)
      #  P1 (UnitC6L:NC / NessoN1:BTN2)
      #  P2-P5 NC
      #  P5 LNA Enable
      #  P6 RF Switch
      #  P7 LoRa Reset
      # for LoraC6 internal IOEXP
      self.write8(0x03, 0xE0)       # PI4IO_REG_IO_DIR
      self.write8(0x05, 0x80)       # PI4IO_REG_OUT_SET
      self.write8(0x07, 0x1C)       # PI4IO_REG_OUT_H_IM
      self.write8(0x0D, 0xC3)       # PI4IO_REG_PULL_SEL
      self.write8(0x0B, 0xC3)       # PI4IO_REG_PULL_EN
      self.write8(0x09, 0x03)       # PI4IO_REG_IN_DEF_STA
      self.write8(0x11, 0xFC)       # PI4IO_REG_INT_MASK

      # Example code and docs
      self.write_bit(0x05, 7, 0)    # drive RESET Low
      tasmota.delay(100)            # the recommended delay is 100ms
      self.write_bit(0x05, 7, 1)    # re-enable SX_NRST
      self.write_bit(0x05, 6, 1)    # enable SX_ANT_SW
      self.write_bit(0x05, 5, 1)    # enable SX_LNA_EN

      # Get virtual button index or -1
      self.virtual_buttons[0] = gpio.add_virtual_button(0)

      tasmota.add_driver(self)      # register this driver
    end
  end

  def deinit()
    # Free virtual buttons if any
    for i: 0..1
      if self.virtual_buttons[i] >= 0
        gpio.remove_virtual_button(self.virtual_buttons[i])
      end
    end
    tasmota.remove_driver(self)
  end

  # Get 1 if button is pressed, 0 if not
  def get_button(num)
    var mask = 0x01 << num
    return (self.wire.read(self.addr, 0x0F, 1) & mask) ? 0 : 1
  end

  # update buttons
  def update_all()
    for i: 0..1
      # M5UnitC6L and NessoN1
      if self.virtual_buttons[i] >= 0
        gpio.set_virtual_button_state(self.virtual_buttons[i], self.get_button(i))
      end
    end
  end

  # update every 50ms
  def every_50ms()
    self.update_all()
  end
end

return PI4IOE5V6408_UnitC6L()
