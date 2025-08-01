# Filled Animation Demo for Berry Animation Framework
#
# This example demonstrates the use of the animation.filled_animation class with different color providers.

import animation

# Create a strip and controller
var strip_width = 30
var controller = animation.animation_controller(strip_width)

print("animation.filled_animation Demo")
print("====================")

# Example 1: Solid color
print("\n1. Creating a filled animation with a solid color...")
var solid_anim = animation.filled_animation(0xFF0000FF, 10, 255, 0, true, "red_fill")
controller.add_animation(solid_anim)
solid_anim.start()

# Wait a moment to see the solid color
tasmota.delay(2000)
solid_anim.stop()
controller.remove_animation(solid_anim)

# Example 2: Color cycle provider
print("\n2. Creating a filled animation with a color cycle provider...")
var cycle_anim = animation.color_cycle_animation(
  [0xFF0000FF, 0xFF00FF00, 0xFFFF0000],  # RGB colors
  3000,                                  # 3 second cycle period
  1,                                     # Sine transition
  10                                     # Priority
)
controller.add_animation(cycle_anim)
cycle_anim.start()

# Let the color cycle run for a few seconds
tasmota.delay(5000)
cycle_anim.stop()
controller.remove_animation(cycle_anim)

# Example 3: Rich palette provider
print("\n3. Creating a filled animation with a rich palette provider...")
var rainbow_anim = animation.rich_palette_animation(
  animation.PALETTE_RAINBOW,  # Use the rainbow palette
  5000,                       # 5 second cycle period
  1,                          # Sine transition
  255,                        # Full brightness
  10                          # Priority
)
controller.add_animation(rainbow_anim)
rainbow_anim.start()

# Let the rainbow animation run for a few seconds
tasmota.delay(5000)
rainbow_anim.stop()
controller.remove_animation(rainbow_anim)

# Example 4: Composite provider
print("\n4. Creating a filled animation with a composite provider...")
var palette_provider = animation.rich_palette_color_provider(
  animation.PALETTE_RAINBOW,  # Use the rainbow palette
  5000,                       # 5 second cycle period
  1,                          # Sine transition
  255                         # Full brightness
)

var cycle_provider = animation.color_cycle_color_provider(
  [0xFF0000FF, 0xFF00FF00, 0xFFFF0000],  # RGB colors
  3000,                                  # 3 second cycle period
  1                                      # Sine transition
)

var composite_anim = animation.composite(
  [palette_provider, cycle_provider],  # List of providers to combine
  0,                                   # Overlay blend mode
  10                                   # Priority
)
controller.add_animation(composite_anim)
composite_anim.start()

# Let the composite animation run for a few seconds
tasmota.delay(5000)
composite_anim.stop()
controller.remove_animation(composite_anim)

# Example 5: Changing color provider dynamically
print("\n5. Changing color provider dynamically...")
var dynamic_anim = animation.filled_animation(0xFF0000FF, 10, 255, 0, true, "dynamic")
controller.add_animation(dynamic_anim)
dynamic_anim.start()

# Show solid color first
tasmota.delay(2000)

# Change to color cycle provider
print("  Changing to color cycle provider...")
dynamic_anim.set_color_provider(cycle_provider)
tasmota.delay(3000)

# Change to rich palette provider
print("  Changing to rich palette provider...")
dynamic_anim.set_color_provider(palette_provider)
tasmota.delay(3000)

# Change back to solid color
print("  Changing back to solid color...")
dynamic_anim.set_color(0x00FF00FF)  # Green
tasmota.delay(2000)

dynamic_anim.stop()
controller.remove_animation(dynamic_anim)

print("\nanimation.filled Demo completed!")