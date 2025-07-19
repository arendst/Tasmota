#
# Example for M5Stack Led Matrix
# 5 x 5 WS2812
#
import animate

var PALETTE_BLACK_RED = bytes(
    "00"  "000000"    # black
    "88"  "880000"    # red
    "FF"  "FF5500"    # orange
  )
  
  var duration = 3000
  var leds = 25
  
  var strip = Leds(leds, gpio.pin(gpio.WS2812, 0))
  var anim = animate.core(strip, 100)
  anim.set_back_color(0x000000)
  
  var pulse = animate.pulse(0xFF0000, leds, 0)
  var palette = animate.palette(PALETTE_BLACK_RED)
  palette.set_range(0, 255)
  palette.set_cb(pulse, pulse.set_color)
  
  var osc1 = animate.oscillator(50, 255, duration, animate.COSINE)
  osc1.set_cb(palette, palette.set_value)
  
  anim.start()
