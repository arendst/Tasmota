# Breathe Animation Demo with LED Values
#
# This example demonstrates the Breathe animation effect
# which creates a smooth, natural breathing effect with customizable parameters.
#
# The demo creates a blue breathing effect with a 3-second period and shows actual LED values.
#
# Command to run demo:
#    ./berry -s -g -m lib/libesp32/berry_animation lib/libesp32/berry_animation/examples/breathe_animation_demo.be

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

print("Starting Breathe animation demo with LED values...")

# Create a mock LED strip with 5 pixels (reduced for readability)
var strip = global.Leds(5)

# Create an animation controller for the strip
var controller = animation.animation_controller(strip)

# Create a breathe animation with blue color
# Parameters:
# - Color: 0xFF0000FF (blue)
# - Min brightness: 10
# - Max brightness: 200
# - Breathe period: 3000ms (3 seconds)
# - Curve factor: 3 (medium-high curve for more natural breathing)
# - Priority: 1
# - Duration: 0 (infinite)
# - Loop: true
var breathe_anim = animation.breathe_animation(0xFF0000FF, 10, 200, 3000, 3, 1, 255, 0, true)

# Add the animation to the controller and start it
controller.add_animation(breathe_anim)
breathe_anim.start()

# Start the controller
controller.start()

# Simulate the fast_loop for a full breathing cycle
print("Simulating animation updates for one full breathing cycle...")

var start_time = tasmota.millis()
var end_time = start_time + 3500  # Run for slightly more than one cycle
var update_interval = 300  # Update every 300ms for demonstration

# Simulate updates
var current_time = start_time
while current_time < end_time
  # Update the controller with the current time
  controller.on_tick(current_time)
  
  # Show the current state with detailed LED values
  print(f"Time: {current_time - start_time}ms, Brightness: {breathe_anim.current_brightness}")
  dump_rgb(strip)  # Show RGB components
  dump(strip)      # Show hex values for reference
  
  # Increment time
  current_time += update_interval
end

print("Breathe animation demo completed")

# Stop the animation
controller.stop()

return true