# Unified Solid Animation Demo
#
# This example demonstrates how to use the unified solid() function
# to display a static color on an LED strip.

import global
import tasmota
import animation

# Using global.Leds instead of MockLEDStrip
import global

# Helper function to display LED strip state
def show_strip_state(strip, width)
  print("LED Strip state:")
  var output = "["
  for i: 0..width-1
    var color = strip.get_pixel_color(i)
    var r = (color >> 16) & 0xFF
    var g = (color >> 8) & 0xFF
    var b = color & 0xFF
    output += format("RGB(%d,%d,%d)", r, g, b)
    if i < width - 1
      output += ", "
    end
  end
  output += "]"
  print(output)
end

# Create an LED strip with 10 pixels
var strip = global.Leds(10)

# Create an animation controller for the strip
var controller = animation.animation_controller(strip)

# Create a solid animation with red color (unified function)
var solid_red = animation.solid(0xFFFF0000)  # Red color (ARGB format)

# Add the animation to the controller
controller.add_animation(solid_red)

# Start the controller
controller.start()

# Simulate a few ticks to update the animation
print("Initial state (red):")
controller.on_tick(tasmota.millis())
strip.show()
show_strip_state(strip, 10)

# Change the color to green
print("\nChanging color to green:")
solid_red.set_color(0xFF00FF00)  # Green color
controller.on_tick(tasmota.millis() + 100)
strip.show()
show_strip_state(strip, 10)

# Create a second solid animation with blue color and higher priority
var solid_blue = animation.solid(0xFF0000FF, 10)  # Blue color, priority 10
controller.add_animation(solid_blue)

# Update to show the blue animation (higher priority)
print("\nAdding blue animation with higher priority:")
controller.on_tick(tasmota.millis() + 300)
strip.show()
show_strip_state(strip, 10)

# Note: Opacity is now controlled at the pixel level through alpha channels
print("\nNote: Opacity is now controlled through per-pixel alpha channels")
controller.on_tick(tasmota.millis() + 400)
strip.show()
show_strip_state(strip, 10)

# Stop the blue animation
print("\nStopping blue animation (should return to green):")
solid_blue.stop()
controller.on_tick(tasmota.millis() + 500)
strip.show()
show_strip_state(strip, 10)

# Stop the controller
print("\nStopping controller:")
controller.stop()
strip.clear()
strip.show()
show_strip_state(strip, 10)

print("\nUnified solid animation demo completed!")