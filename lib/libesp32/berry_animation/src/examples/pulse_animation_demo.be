# Pulse Animation Demo with LED Values
#
# This example demonstrates how to use the Pulse animation effect
# to create a pulsing light effect on an LED strip.
#
# The demo creates a pulse animation with customizable parameters
# and shows how to integrate it with the animation controller.
# It also displays the actual LED values.

import global
import tasmota
import animation

def dump_rgb(strip)
  var output = "LEDs: ["
  for i:0..strip.length()-1
    var color = strip.get_pixel_color(i)
    var r = (color >> 16) & 0xFF
    var g = (color >> 8) & 0xFF
    var b = color & 0xFF
    
    if i > 0
      output += ", "
    end
    output += format("R:%d,G:%d,B:%d", r, g, b)
  end
  output += "]"
  print(output)
end

def dump(strip)
  var output = "["
  for i:0..strip.length()-1
    if i > 0
      output += ", "
    end
    output += format("0x%06X", strip.get_pixel_color(i) & 0xFFFFFF)
  end
  output += "]"
  print(output)
end

# Create a mock LED strip with 5 pixels (reduced for readability)
var strip = global.Leds(5)

# Create an animation controller for the strip
var controller = animation.animation_controller(strip)

# Create a pulse animation with blue color
# Parameters: color, min_brightness, max_brightness, pulse_period, priority
var blue_pulse = animation.pulse_animation(0xFF0000FF, 20, 200, 2000, 1)

# Add the animation to the controller
controller.add_animation(blue_pulse)

# Start the animation
blue_pulse.start()
controller.start()

# Simulate the fast_loop for a few seconds
print("Starting pulse animation demo with LED values...")
print("Simulating animation updates for one full pulse cycle...")

var start_time = tasmota.millis()
var end_time = start_time + 2500  # Run for slightly more than one cycle
var update_interval = 200  # Update every 200ms for demonstration

# Simulate updates
var current_time = start_time
while current_time < end_time
  # Update the controller with the current time
  controller.on_tick(current_time)
  
  # Show the current state with detailed LED values
  print(f"Time: {current_time - start_time}ms, Pulse state: {(current_time - start_time) % 2000 / 2000 * 100}%")
  dump_rgb(strip)  # Show RGB components
  dump(strip)      # Show hex values for reference
  
  # Increment time
  current_time += update_interval
end

# Stop the animation
controller.stop()
print("Pulse animation demo completed")

return true