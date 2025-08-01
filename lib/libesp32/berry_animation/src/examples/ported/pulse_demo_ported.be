#
# Direct port of animate_demo_pulse.be to new animation framework
# Original: Uses old animate.core(), animate.pulse(), animate.oscillator(), animate.palette()
# Ported: Uses new AnimationEngine, PulsePositionAnimation, OscillatorValueProvider, RichPaletteColorProvider
#
# Visual effect: Moving pulse with oscillating position and cycling color
#

import animation

var duration = 10000
var leds = 5 * 5  # 25 LEDs for M5Stack matrix

# Create LED strip (same as original)
var strip = global.Leds(leds, gpio.pin(gpio.WS2812, 0))

# Create animation engine (replaces animate.core)
var engine = animation.create_engine(strip)

# Set background color (replaces anim.set_back_color)
# Note: In new framework, we create a background animation instead
var background_anim = animation.solid(0x2222AA, 0, 0, true, "background")
engine.add_animation(background_anim)

# Create position oscillator (replaces animate.oscillator(-3, 26, 5000, animate.COSINE))
var pos_oscillator = animation.smooth(-3, 26, 5000)

# Create color palette provider (replaces animate.palette(animate.PALETTE_STANDARD_VAL, 30000))
# Using a custom palette that matches PALETTE_STANDARD_VAL behavior
var color_palette = bytes(
  "00FF0000"    # Red (value 0)
  "40FFFF00"    # Yellow (value 64)
  "8000FF00"    # Green (value 128)
  "C000FFFF"    # Cyan (value 192)
  "FF0000FF"    # Blue (value 255)
)
var color_provider = animation.rich_palette_color_provider(color_palette, 30000, 1, 255)

# Create pulse animation (replaces animate.pulse(0xFF4444, 2, 1))
var pulse_anim = animation.pulse_position(0xFF4444, 2, 1, 10, 0, true, "moving_pulse")

# Apply dynamic parameters using value providers
pulse_anim.set_param_value("pos", pos_oscillator)      # Oscillating position
pulse_anim.set_param_value("color", color_provider)    # Cycling color

# Add pulse animation with higher priority than background
pulse_anim.priority = 10
engine.add_animation(pulse_anim)

# Simulate brightness setting (original had anim.set_bri(60))
# In new framework, this would be handled by the LED strip or global opacity
# For demonstration, we'll set the pulse opacity
pulse_anim.set_param_value("opacity", 153)  # 60% of 255

engine.start()

print("Pulse demo ported - moving pulse with oscillating position and cycling color")
print("Duration:", duration, "ms, LEDs:", leds)
print("Background: Blue, Pulse: Moving with color cycling")
print("Press Ctrl+C to stop")