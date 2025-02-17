######################################################################
# Driver for M5Stack 8encoder
#
# How to use:
#  - copy file to file-system
#  - add `import M5_8encoder` in `autoexec.be`
#
# This device is composed of 8 rotary encoders (numbered 0..7)
# each rotary encoder has a button embedded that can be pressed
# and an RGB Led right above it.
# A main switch (on/off) is also available at the far right
#
# The device is very sensitive to I2C read request format
# otherwise it blocks the entire I2C bus
# (I haven't found a way to free the bus without powering off the device)
#
# Read patterns;
# - 0x00-0x1F: 4 bytes max
# - 0x20-0x3F: 4 bytes max
# - 0x50-0x57: 1 byte max
# - 0x60     : 1 bytes max
# - 0x70-0x8A: 3 bytes max
# - 0xFE     : 1 byte max
#
# Write patterns:
# - 0x00-0x1F: multiple of 4 bytes
# - 0x40-0x47: 1..8 bytes (sending more than 8 might trigger a buffer overflow)
# - 0x70-0x8A: multiple of 3 bytes
# - 0xFF     : 1 byte
#
# https://docs.m5stack.com/en/unit/8Encoder
######################################################################

class M5_8encoder : I2C_Driver
  var switch
  var rotaries, colors, buttons

  def init(addr)
    if (addr == nil)    addr = 0x41   end
    super(self, I2C_Driver).init("M5_8encoder", addr)

    self.switch = 0
    self.rotaries = [0, 0, 0, 0, 0, 0, 0, 0]
    self.colors = [0, 0, 0, 0, 0, 0, 0, 0]
    self.buttons = [0, 0, 0, 0, 0, 0, 0, 0]

    if self.wire
      tasmota.add_driver(self)
    end
  end

  # read counter for <num> (0..7)
  def get_rotary_counter(num)
    return self.wire.read(self.addr, 0x00 + num * 4, -4)
  end

  # Read the current increment for rotary <num> (0..7)
  def get_rotary_increment(num)
    return self.wire.read(self.addr, 0x20 + num * 4, -4)
  end

  # reset the counter num (0..7)
  def rotary_counter_reset(num)
    self.wire.write(self.addr, 0x40 + num, 1, 1)
  end

  # update all rotary values
  def update_all()
    var i = 0
    while i < 8
      # rotaries
      var v = self.get_rotary_increment(i) / 2      # divide by 2, because they always increment by 2
      v += self.rotaries[i]
      if (v < 0)      v = 0     end
      if (v > 15)     v = 15    end
      self.rotaries[i] = v
      # buttons
      self.buttons[i] = self.get_rotary_pressed(i)
      # colors
      self.colors[i] = self.get_led_color(i)
      i += 1
    end
    self.get_switch()
  end

  # Read status of the far-right switch
  def get_switch()
    self.switch = self.wire.read(self.addr, 0x60, 1) ? 1 : 0
    return self.switch
  end

  # Get 1 if button is pressed, 0 if not
  def get_rotary_pressed(num)
    return self.wire.read(self.addr, 0x50 + num, 1) ? 1 : 0
  end

  # Read firmware version
  def get_firmware()
    return self.wire.read(self.addr, 0xFE, 1)
  end

  # Read I2C address from the register. We don't implement writing here as it is too dangerous
  def get_i2c_address()
    return self.wire.read(self.addr, 0xFF, 1)
  end

  def get_led_color(num)
    return self.wire.read(self.addr, 0x70 + 3 * num, 3)
  end

  def set_led_color(num, color)
    self.colors[num] = color
    self.wire.write(self.addr, 0x70 + 3 * num, color, 3)
  end

  def set_led_color_bytes(b)
    self.wire.write_bytes(self.addr, 0x70, b)
  end
  
  def get_led_color_all()
    var b = bytes()
    for i: 0..7
      b = b + self.wire.read_bytes(self.addr, 0x70 + 3 * i, 3)
    end
    return b
  end

  # clear all 8 leds at once
  def clear_led_all()
    var b = bytes()
    b.resize(24)
    self.wire.write_bytes(self.addr, 0x70, b)
  end

  def button_html(i)
    return f'<i class="bx" style="--cl:#{self.colors[i]:06X}"></i>'
  end

  # update every 50ms
  def every_50ms()
    self.update_all()
  end

  #- display sensor value in the web UI -#
  def web_sensor()
    if !self.wire return nil end  #- exit if not initialized -#
    var msg = format('<td colspan="2">'
             "{s}M5Stack 8encoder{m}Switch %s{e}"
             '<table style="width:100%%;text-align:center;">'
             "<style>"
             ".bx{height:14px;width:14px;display:inline-block;border:1px solid currentColor;background-color:var(--cl,#fff)}"
             "</style>"
             "<tr>{m}%s{m}%s{m}%s{m}%s{m}%s{m}%s{m}%s{m}%s{e}"
             "<tr>{m}%2i{m}%2i{m}%2i{m}%2i{m}%2i{m}%2i{m}%2i{m}%2i{e}"
             "<tr>{m}CH1{m}CH2{m}CH3{m}CH4{m}CH5{m}CH6{m}CH7{m}CH8{e}"
             '<tr></tr>'
             '</td>'
             '</table>'
              ,
              self.get_switch() ? "On" : "Off",
              self.button_html(0), self.button_html(1), self.button_html(2), self.button_html(3), self.button_html(4), self.button_html(5), self.button_html(6), self.button_html(7),
              self.rotaries[0], self.rotaries[1], self.rotaries[2], self.rotaries[3], self.rotaries[4], self.rotaries[5], self.rotaries[6], self.rotaries[7]
    ) 
    tasmota.web_send(msg)
  end

  #- add sensor value to teleperiod -#
  def json_append()
    if !self.wire return nil end  #- exit if not initialized -#
    var msg = format(',"M5_8encoder":{"Switch":%i,"Rotaries":[%i,%i,%i,%i,%i,%i,%i,%i]}',
                     self.get_switch(),
                     self.rotaries[0], self.rotaries[1], self.rotaries[2], self.rotaries[3], self.rotaries[4], self.rotaries[5], self.rotaries[6], self.rotaries[7]
                    )
    tasmota.response_append(msg)
  end
end

return M5_8encoder()
