#
# Direct port of animate_demo_palette_background.be to new animation framework
# Original: Uses old animate.core() with add_background_animator()
# Ported: Uses new AnimationEngine with animation.filled_animation and rainbow palette
#
# Visual effect: Cycling rainbow background across all LEDs
#

import animation

var duration = 10000
var leds = 5 * 5  # 25 LEDs for M5Stack matrix

# Create LED strip (same as original)
var strip = global.Leds(leds, gpio.pin(gpio.WS2812, 0))

# Create animation engine (replaces animate.core)
var engine = animation.create_engine(strip)

# Create rainbow palette background (replaces animate.palette with PALETTE_RAINBOW_WHITE)
# Using the built-in rainbow palette with smooth transitions
var rainbow_provider = animation.rich_palette_color_provider.rainbow(duration, 1, 255)

# Create filled animation as background (replaces add_background_animator)
var background_anim = animation.filled_animation(rainbow_provider, 0, 0, true, "rainbow_background")

# Add animation to engine and start
engine.add_animation(background_anim)
engine.start()

print("Palette background demo ported - cycling rainbow background")
print("Duration:", duration, "ms, LEDs:", leds)
print("Press Ctrl+C to stop")