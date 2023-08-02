# Simple driver for M5Stack DigiClock I2C 7 segments LED
#
# https://docs.m5stack.com/en/unit/digi_clock

class M5Stack_DigiClock
  static var I2C_ADDR = 0x30                # default I2C address is 0x30
  var led
  var addr

  def init(addr)
    if !addr    addr = self.I2C_ADDR end

    self.addr = addr
    self.led = tasmota.wire_scan(addr)

    if self.led == nil    raise "configuration_error", "Could not find DigiClock I2C device" end
  end

  def set_brightness(b)
    if b < 0      b = 0 end
    if b > 8      b = 8 end
    self.led.write(self.addr, 0x30, b, 1)
  end

  def set_text(t)
    self.led.write_bytes(0x30, 0x20, bytes().fromstring(str(t)))
  end
end

return M5Stack_DigiClock

#-

var led = M5Stack_DigiClock()
led.set_brightness(1)
led.set_text("1234")

-#
