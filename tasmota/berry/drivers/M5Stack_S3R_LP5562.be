###############################################################
# Specialized driver for LP5562 on M5Stack S3R
#
# This driver controls the backlighting of the screen
###############################################################

# This code is ported from https://github.com/m5stack/M5GFX/blob/master/src/M5GFX.cpp
# `struct Light_M5StackAtomS3R : public lgfx::ILight`
class M5Stack_S3R_LP5562 : I2C_Driver
  def init(addr)
    if (addr == nil)  addr = 0x30   end   # default address is 0x30
    super(self, I2C_Driver).init("LP5562", 0x30)

    if self.wire      # if found
      import display

      self.write8(0x00, 0x40)
      tasmota.delay(1)
      self.write8(0x08, 0x01)
      self.write8(0x70, 0x00)

      self.set_displaydimmer(display.dimmer())   # apply dimmer value on start

      tasmota.add_driver(self)    # register this driver
    end
  end

  # Dimmer in percentage
  def set_displaydimmer(x)
    self.write8(0x0e, tasmota.scale_uint(x, 0, 100, 0, 255))
  end

  # respond to display events
  def display(cmd, idx, payload, raw)
    if cmd == "dim" || cmd == "power"
      self.set_displaydimmer(idx)
    end
  end

end

return M5Stack_S3R_LP5562()
