#
# Example for M5Stack Led Matrix
# 5 x 5 WS2812
#
import animate
class Background_demo : Leds_animator
  static var PALETTE_STANDARD_TAG = bytes(
    "40"  "FF0000"    # red
    "40"  "FFA500"    # orange
    "40"  "FFFF00"    # yellow
    "40"  "00EE00"    # green
    "40"  "0000FF"    # blue
    "40"  "4B00FF"    # indigo
    "40"  "FF82FF"    # violet
    "00"  "FF0000"    # red
  )

  static var PALETTE_RAINBOW_WHITE = bytes(
    "50"  "FF0000"    # red
    "30"  "FF0000"    # red
    "50"  "FFA500"    # orange
    "30"  "FFA500"    # orange
    "50"  "FFFF00"    # yellow
    "30"  "FFFF00"    # yellow
    "50"  "00FF00"    # green
    "30"  "00FF00"    # green
    "50"  "0000FF"    # blue
    "30"  "0000FF"    # blue
    "50"  "FF00FF"    # indigo
    "30"  "FF00FF"    # indigo
    "50"  "FFFFFF"    # white
    "30"  "FFFFFF"    # white
    "00"  "FF0000"    # red
  )

  # duration in seconds
  def init(strip, duration)
    import animate
    super(self).init(strip)
    # var pal = animate.palette(self.PALETTE_STANDARD_TAG, duration)
    var pal = animate.palette(self.PALETTE_RAINBOW_WHITE, duration)
    self.add_background_animator(pal)
  end

  def animate()
  end
end

var strip = Leds(5 * 5, gpio.pin(gpio.WS2812, 0))
var r = Background_demo(strip, 20)
r.start()
