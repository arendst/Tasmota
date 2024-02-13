#
# Example for M5Stack Led Matrix
# 5 x 5 WS2812
#
import animate

var duration = 10000
var strip = Leds(5 * 5, gpio.pin(gpio.WS2812, 0))
var anim = animate.core(strip)
anim.add_background_animator(animate.palette(animate.PALETTE_RAINBOW_WHITE, duration))
anim.start()
