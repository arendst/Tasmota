#
# Direct port of animate_demo_breathe.be to new animation framework
# Original: Uses old animate.core(), animate.pulse(), animate.palette(), animate.oscillator()
# Ported: Uses new AnimationEngine, BreatheAnimation, RichPaletteColorProvider, OscillatorValueProvider
#
# Visual effect: Breathing red/orange pulse that oscillates in brightness
#

import animation

# Same palette as original (black to red to orange)
var PALETTE_BLACK_RED = bytes(
  "00000000"    # black (value 0)
  "88880000"    # red (value 136)
  "FFFF5500"    # orange (value 255)
)

var duration = 3000
var leds = 25

# Create LED strip (same as original)
var strip = global.Leds(leds, gpio.pin(gpio.WS2812, 0))

# Create animation engine (replaces animate.core)
var engine = animation.create_engine(strip)

# Create palette color provider (replaces animate.palette)
var palette_provider = animation.rich_palette_color_provider(PALETTE_BLACK_RED, duration, 1, 255)
palette_provider.set_range(0, 255)

# Create oscillator for brightness control (replaces animate.oscillator)
var brightness_osc = animation.smooth(50, 255, duration)

# Create breathe animation (replaces animate.pulse)
var breathe_anim = animation.breathe_animation(0xFF0000, 100, duration, true, "breathe_port")

# Use oscillator to control the color provider's brightness dynamically
# This simulates the original callback mechanism
breathe_anim.set_param_value("color", palette_provider)
breathe_anim.set_param_value("opacity", brightness_osc)

# Add animation to engine and start
engine.add_animation(breathe_anim)
engine.start()

print("Breathe demo ported - breathing red/orange pulse")
print("Duration:", duration, "ms, LEDs:", leds)
print("Press Ctrl+C to stop")