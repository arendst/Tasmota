#-------------------------------------------------------------
 - IMPORTANT
 - THIS CLASS IS ALREADY BAKED IN TASMOTA
 -
 - It is here for debugging and documentation purpose only
 -------------------------------------------------------------#

#-------------------------------------------------------------
 - I2C_Driver class to simplify development of I2C drivers
 -
 - I2C_Driver(name, addr [, i2c_index]) -> nil
 -   name: name of I2C device for logging, or function to detect the model
 -   addr: I2C address of device, will probe all I2C buses for it
 -   i2c_index: (optional) check is the device is not disabled
 -------------------------------------------------------------#

class I2C_Driver
  var wire          #- wire object to reach the device, if nil then the module is not initialized -#
  var addr          #- I2C address of the device -#
  var name          #- model namme of the device, cannot be nil -#

  #- Init and look for device
   - Input:
   -   name_or_detect : name of the device (if string)
                        or function to detect the precise model(if function)
                            the function is passed a single argument `self`
                            and must return a string, or `nil` if the device is invalid
   -   addr : I2C address of device (int 0..255)
   -   i2c_index : Tasmota I2C index, see `I2CDEVICES.md` (int)
   --#
  def init(name_or_detect, addr, i2c_index)
    #- check if the i2c index is disabled by Tasmota configuration -#
    if i2c_index != nil && !tasmota.i2c_enabled(i2c_index) return end

    self.addr = addr                            #- address for AXP192 -#
    self.wire = tasmota.wire_scan(self.addr)    #- get the right I2C bus -#

    if self.wire
      #- find name of device, can be a string or a method -#
      if type(name_or_detect) == 'function'
        self.name = name_or_detect(self)
      else
        self.name = name_or_detect
      end
      #- if name is invalid, it means we can't detect device, abort -#
      if self.name == nil  self.wire = nil end

      if self.wire
        print("I2C:", self.name, "detected on bus", self.wire.bus)
      end
    end
  end

  #- write register with 8 bits value -#
  def write8(reg, val)
    return self.wire.write(self.addr, reg, val, 1)
  end

  # Set or clear a specific bit in a register
  # write_bit(reg:int, bit:int, state:bool) -> nil
  #   reg: I2C register number (0..255)
  #   bit: bit of I2C register to change (0..7)
  #   state: boolean value to write to specified bit
  def write_bit(reg, bit, state)
    if bit < 0 || bit > 7 return end
    var mark = 1 << bit
    if state    self.write8(reg, self.read8(reg) | mark)
    else        self.write8(reg, self.read8(reg) & (0xFF - mark))
    end
  end

  # read 8 bits
  def read8(reg)
    return self.wire.read(self.addr, reg, 1)
  end
  # read 12 bits
  def read12(reg)
    var buf = self.wire.read_bytes(self.addr, reg, 2)
    return (buf[0] << 4) + buf[1]
  end
  # read 13 bits
  def read13(reg)
    var buf = self.wire.read_bytes(self.addr, reg, 2)
    return (buf[0] << 5) + buf[1]
  end
  # read 24 bits
  def read24(reg)
    var buf = self.wire.read_bytes(self.addr, reg, 3)
    return (buf[0] << 16) + (buf[1] << 8) + buf[2]
  end
  # read 32 bits
  def read32(reg)
    var buf = self.wire.read_bytes(self.addr, reg, 4)
    return (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3]
  end
end

#- Example

d = I2C_Driver("MPU", 0x68, 58)

-#