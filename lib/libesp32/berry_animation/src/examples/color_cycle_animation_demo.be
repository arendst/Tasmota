# ColorCycle Animation Demo
#
# This example demonstrates the Filled animation with a color cycle provider
# which smoothly transitions between colors in a palette.

# Import the animation module
import global
import tasmota
import animation

# Create an animation controller for a strip of 30 LEDs
var strip = global.Leds(30, 1)  # Assuming global.Leds class is available in Tasmota
var controller = animation.animation_controller(strip)

# Create a color cycle animation with a custom palette
var palette = [
  0xFFFF0000,  # Red
  0xFFFF7F00,  # Orange
  0xFFFFFF00,  # Yellow
  0xFF00FF00,  # Green
  0xFF0000FF,  # Blue
  0xFF4B0082,  # Indigo
  0xFF8F00FF   # Violet
]

# Create the animation with a 5-second cycle period and sine transition
# Using the factory method for color_cycle
var rainbow_cycle = animation.color_cycle_animation(palette, 5000, 1, 0, 255)

# Add the animation to the controller
controller.add_animation(rainbow_cycle)

# Start the animation
controller.start()

print("Running ColorCycle animation demo")
print("Press Ctrl+C to stop")

# In a real application, the animation would continue running
# Here we'll just wait for a while to demonstrate
tasmota.delay(30000)  # Run for 30 seconds

# Stop the animation
controller.stop()

print("ColorCycle animation demo completed")

return true