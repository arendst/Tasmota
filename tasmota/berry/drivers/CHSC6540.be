#-
 - I2C driver for the Touch Screen driver CHSC6540 of the M5Stack Tough
 -
 - This is based on 
 - https://github.com/m5stack/M5Tough/blob/master/src/M5Touch.cpp
 - https://github.com/m5stack/M5Tough/blob/master/src/M5Touch.h
 -#

 class CHSC6540 : I2C_Driver
  var tp_int    # gpio used as INT - going low when the screen is touched
  # prevous values
  var touched, x, y   # previous values (bool, int, int) to be repeated when not touched
  
  def init()
    # set current values
    self.x = 0
    self.y = 0
    self.touched = false

    self.tp_int = gpio.pin(gpio.INTERRUPT, 0)
    super(self).init("CHSC6540", 0x2E)

    tasmota.add_driver(self)
    # check that display is present
    import introspect
    if !introspect.module("display")
      tasmota.log("I2C: can't start CHSC6540 without display enabled", 3)
      self.wire = nil
    end

    if self.tp_int < 0
      tasmota.log("I2C: can't start CHSC6540 without INTERRUPT-1 gpio configured", 3)
      self.wire = nil
    else
      gpio.pin_mode(self.tp_int, gpio.INPUT_PULLUP)
    end

    # all good, configure device
    if self.wire
      self.write8(0x5A, 0x5A)   # INT mode change

      tasmota.add_driver(self)
    end
  end

  # is the screen pressed - i.e. TP_INT is low
  def is_pressed()
    if self.wire == nil   return end
    return gpio.digital_read(self.tp_int) == 0
  end

  def every_50ms()
    if self.wire == nil   return end

    self.touched = self.is_pressed()
    # tasmota.log("DEBUG> int="+str(self.tp_int)+" touched="+str(self.touched), 2)
    if self.touched
      var raw_read = self.wire.read_bytes(self.addr, 0x02, 11)    # read a series of 11 bytes at from register 0x02
      var pts = raw_read[0]
      if pts <= 0 || pts > 2 return end     # wrong
      # supports multi-touch
      #var p0f = (raw_read[4] & 0x10) != 0   # unused for now
      self.x = raw_read.get(1,-2) & 0x0FFF
      self.y = raw_read.get(3,-2) & 0x0FFF
      # tasmota.log(format("I2C: screen pressed x=%i y=%i", self.x, self.y), 2)
      # var p1x = raw_read.get(7,2) & 0x0FFF
      # var p1y = raw_read.get(9,2) & 0x0FFF
   end

    # return values
    import display
    display.touch_update(self.touched ? 1 : 0, self.x, self.y, 0)
  end

end

return CHSC6540()
