#################################################################################
# Generic driver for PCA9535 - solidified
#
# I2C IO Expander, similar to PCA9557
# Datasheet: https://www.nxp.com/docs/en/data-sheet/PCA9535_PCA9535C.pdf
#
# This expander is used in SeedStudio SenseCAP D1
#################################################################################

#@ solidify:PCA9535
class PCA9535 : I2C_Driver
  var last_read                                     # time when last read (avoid reading too often)
  var input_port                                    # shadow of registers 0+1 with state of input registers
  var output_port                                   # shadow of registers 2+3 with known outputs
  var inversion_port                                # shadow of registers 4+5 with input inversion
  var config_port                                   # shadow of registers 6+7 with Input/Output configurations

  def init(address)
    if (address == nil) address = 0x20 end          # default address is 0x20
    super(self).init("PCA9535", address)
    self.last_read = 0
    if self.wire
      # if detected
      self.read_all()
    end
  end

  def read_all()
    var now = tasmota.millis()
    if (now - self.last_read > 10)                  # if last read was more than 10 ms in the past
      self.input_port = self.read16LE(0x00)
      self.output_port = self.read16LE(0x02)
      self.inversion_port = self.read16LE(0x04)
      self.config_port = self.read16LE(0x06)
      self.last_read = tasmota.millis()
      if tasmota.loglevel(4)
        log(f"I2C: PCA9535 read input(0x{self.input_port:04X}) output(0x{self.output_port:04X}) inversion(0x{self.inversion_port:04X}) config(0x{self.config_port:04X})")
      end
    end
  end

  def config_all(v)
    self.config_port = int(v)
    self.write16LE(0x06, self.config_port)
  end
  # port: 0..15
  # mode: 0=output 1=input
  def config_gpio(port, mode)
    self.read_all()
    if (mode != 0 && mode != 1) raise "value_error", f"mode muste be 0 or 1" end
    var config_new = self._bit_set_to(self.config_port, port, mode)
    # write only if value changed
    if config_new != self.config_port
      self.config_all(config_new)
    end
  end

  def read_gpio(port)
    self.read_all()
    return (self.input_port & (1 << port)) ? 1 : 0
  end

  def write_gpio(port, v)
    self.read_all()
    v = (v ? 1 : 0)                     # ensure we have only 0/1 as values
    var output_new = self._bit_set_to(self.output_port, port, v)
    if output_new != self.output_port
      self.output_port = output_new
      self.write16LE(0x02, self.output_port)
    end
  end

  # inspired from https://stackoverflow.com/questions/47981/how-to-set-clear-and-toggle-a-single-bit
  # num: int value
  # n: bit to change
  # x: value 0/1
  static def _bit_set_to(num, n, x)
    return (num & ~(1 << n)) | (x << n)
  end
end

return PCA9535
