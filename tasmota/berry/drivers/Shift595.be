# Sample Relay driver for 595 shift registers
# -------------------------------------------------------------------------
# You can cascade multiple '595 for a total of 28 relays (including other native relays)
# Include the following lines in 'preinit.be'
#   import Shift595
#   tasmota.add_driver(Shift595(srclk,ser,rclk,count,inverted))
# where
#   srclk     : GPIO number connected to the 595's SRCLK pin (serial shift clock)
#   ser       : GPIO number connected to the 595's SER pin (serial data in)
#   rclk      : GPIO number connected to the 595's RCLK pin (transfer to output)
#   count     : number of used output pins / relays (must be consecutive)
#   inverted  : if true, relay is driven by a 0/LOW inverted (instead of 1/HIGH)
#
# To use multiple '595, connects
#   all SRCLK together to GPIO srclk
#   all RCLK together to GPIO rclk
#   GPIO ser to SER input of first '595
#   QH' output of first '595 to SER input of 2nd '595 and so on
# -------------------------------------------------------------------------

import string
import gpio

class Shift595

  var srclk, ser, rclk, count, inverted
  var first, state

  def log_relay(relay)
    var s = ""
    for r:relay if r s+='1' else s+='0' end end
    log("595: "+s,2)
  end

  def shift_out(relay)
    self.log_relay(relay)

    for i:0..size(relay)-1
      var bit
      bit = (relay[size(relay)-1 - i]) ? int(!self.inverted) : int(self.inverted)
      gpio.digital_write(self.srclk, 0)
      gpio.digital_write(self.ser, bit)
      gpio.digital_write(self.srclk, 1)
    end

    gpio.digital_write(self.rclk, 1)
    gpio.digital_write(self.rclk, 0)
    gpio.digital_write(self.srclk, 0)
  end

  def init(srclk, ser, rclk, count, inverted)
    self.srclk= srclk
    self.ser = ser
    self.rclk = rclk
    self.count = count
    self.inverted = inverted
    self.first = tasmota.global.devices_present
    tasmota.global.devices_present += count

    gpio.pin_mode(self.srclk, gpio.OUTPUT)
    gpio.pin_mode(self.ser,    gpio.OUTPUT)
    gpio.pin_mode(self.rclk,   gpio.OUTPUT)
    gpio.digital_write(self.srclk, 0)
    gpio.digital_write(self.ser,    0)
    gpio.digital_write(self.rclk,   0)

    self.state = 0
  end

  def set_power_handler(cmd, idx)
    var new_state = tasmota.get_power()[self.first..self.first+self.count]
    if self.state != new_state
      self.state = new_state
      self.shift_out(self.state)
    end
  end

end

return Shift595 # allow using 'import' instead of 'load()'
