# lv_touch_3_buttons
#
# Handles a simple case with 3 physical buttons below the screen, like in M5Stack
#
# LVGL must be already started to get the screen coordinates.
# Touches are simulated as actual touch screen:
#  x: is spread at coordinates: 1/6, 1/2, 5/6
#  y: 10 pixels from botton

class lv_touch_3_buttons
  var gpios           # (array) physical GPIO numbers for each button, -1 in not assigned
  var x_coords        # (array) x coordinates for each button
  var y_coords        # (array) y coordinates for each button
  var active_low      # (bool) true if button is active low
  # prevous values
  var touched, x, y   # previous values (bool, int, int) to be repeated when not touched

  static ACTIVE_HIGH = false
  static ACTIVE_LOW = true

  # Arguments:
  #   Physical GPIOs, generally through `gpio.pin(gpio.GPIO_INPUT, 0), gpio.pin(gpio.GPIO_INPUT, 1), gpio.pin(gpio.GPIO_INPUT, 2)`
  #
  # Pre-condition:
  #   LVGL must be already started
  def init(btn1, btn2, btn3, active_low)
    import global
    if !global.contains("lv") return end    # abort if LVGL is not there
    lv.start()    # make sure LVGL is started, or things can go really wrong

    # set current values
    self.x = 0
    self.y = 0
    self.touched = false
    #
    self.active_low = active_low
    self.gpios = [-1, -1, -1]
    # store only valid gpios
    btn1 = int(btn1)
    if btn1 >= 0    self.gpios[0] = btn1 end
    btn2 = int(btn2)
    if btn2 >= 0    self.gpios[1] = btn2 end
    btn3 = int(btn3)
    if btn3 >= 0    self.gpios[2] = btn3 end

    # compute coordinates
    var hres = lv.get_hor_res()
    var vres = lv.get_ver_res()       # should be 240
    self.x_coords = [ hres / 6, hres / 2, hres * 5 / 6]
    self.y_coords = [ vres - 10, vres - 10, vres - 10]

    # add self to drivers
    tasmota.add_driver(self)
  end

  # scan every 50ms
  def every_50ms()
    import display

    var i = 0
    var x, y
    var touched = false           # is there any button pressed
    while i < size(self.gpios)
      var gp = self.gpios[i]
      if gp >= 0                  # skip invalid gpio
        var in = bool(gpio.digital_read(gp))
        in = self.active_low ? !in : in     # invert if active low
        if in && !touched                       # first button touched
          x = self.x_coords[i]
          y = self.y_coords[i]
        end
        touched = touched || in
      end
      i += 1
    end

    # if touched, change x/y
    if touched
      self.x = x
      self.y = y
    end
    self.touched = touched
    # return values
    display.touch_update(self.touched ? 1 : 0, self.x, self.y, 0)
  end
end

return lv_touch_3_buttons(gpio.pin(gpio.GPIO_INPUT, 0), gpio.pin(gpio.GPIO_INPUT, 1), gpio.pin(gpio.GPIO_INPUT, 2), true)

#-
lv_btn3 = lv_touch_3_buttons(gpio.pin(gpio.GPIO_INPUT, 0), gpio.pin(gpio.GPIO_INPUT, 1), gpio.pin(gpio.GPIO_INPUT, 2), lv_touch_3_buttons.ACTIVE_LOW)
tasmota.add_driver(lv_btn3)
-#
