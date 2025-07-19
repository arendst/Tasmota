#
# H_bridge class in Berry to pilot a H-bridge device
#

class H_bridge
  var gpio1, gpio2
  var max

  # init(phy_gpio1, phy_gpio2) - intialize H-bridge with the 2 GPIOs used to control it
  def init(gpio1, gpio2)
    self.gpio1 = gpio1
    self.gpio2 = gpio2
    self.max = 1023     # max value of duty
  end

  # set the value of both PWM values
  def set(v1, v2)
    if v1 < 0   v1 = 0 end
    if v2 < 0   v2 = 0 end
    if v1 + v2 > self.max
      raise "value_error", "the sum of duties must not exceed 100%"
    end

    import gpio
    gpio.set_pwm(self.gpio1, v1, 0)
    gpio.set_pwm(self.gpio2, v2, v1)    # dephase by value v1
  end
end
