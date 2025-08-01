# Crenel Position Animation Demo with LED Values
#
# This example demonstrates the Crenel Position animation effect
# which creates repeating rectangular pulses (square wave pattern) on the LED strip.
#
# The demo creates a green crenel pattern with customizable parameters and shows actual LED values.
#
# Command to run demo:
#    ./berry -s -g -m lib/libesp32/berry_animation lib/libesp32/berry_animation/examples/crenel_position_animation_demo.be

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

print("Starting Crenel Position animation demo with LED values...")

# Create a mock LED strip with 12 pixels for better pattern visibility
var strip = global.Leds(12)

# Create an animation engine for the strip
var engine = animation.create_engine(strip)

print("=== Demo 1: Basic crenel pattern ===")
# Create a crenel animation with green color
# Parameters:
# - Color: 0xFF00FF00 (green)
# - Pulse size: 2 pixels
# - Low size: 3 pixels (spacing between pulses)
# - Number of pulses: -1 (infinite)
# - Priority: 1
# - Duration: 0 (infinite)
# - Loop: true
var crenel_anim = animation.crenel_position(0xFF00FF00, 2, 3, -1, 1, 0, true, "demo_crenel")

# Add the animation to the engine and start it
engine.add_animation(crenel_anim)
crenel_anim.start()

# Start the engine
engine.start()

# Show the static pattern
engine.on_tick(tasmota.millis())
print("Basic crenel pattern (2 pixels on, 3 pixels off, infinite):")
dump_rgb(strip)
dump(strip)

print()
print("=== Demo 2: Crenel with background color ===")
# Set a dark blue background
crenel_anim.set_back_color(0xFF000080)
engine.on_tick(tasmota.millis() + 10)  # Force update with new time
print("Crenel with dark blue background:")
dump_rgb(strip)
dump(strip)

print()
print("=== Demo 3: Different pulse and spacing sizes ===")
# Change to 3 pixels on, 2 pixels off
crenel_anim.set_back_color(0xFF000000)  # Back to transparent
crenel_anim.set_pulse_size(3)
crenel_anim.set_low_size(2)
engine.on_tick(tasmota.millis() + 20)  # Force update with new time
print("Modified pattern (3 pixels on, 2 pixels off):")
dump_rgb(strip)
dump(strip)

print()
print("=== Demo 4: Limited number of pulses ===")
# Show only 2 pulses
crenel_anim.set_nb_pulse(2)
crenel_anim.set_pos(1)  # Start at position 1
engine.on_tick(tasmota.millis() + 30)  # Force update with new time
print("Limited to 2 pulses starting at position 1:")
dump_rgb(strip)
dump(strip)

print()
print("=== Demo 5: Single pixel pulses ===")
# Reset to infinite and show single pixel pulses
crenel_anim.set_nb_pulse(-1)
crenel_anim.set_pos(0)
crenel_anim.set_pulse_size(1)
crenel_anim.set_low_size(2)
crenel_anim.set_color(0xFFFF0000)  # Change to red
engine.on_tick(tasmota.millis() + 40)  # Force update with new time
print("Single pixel red pulses (1 pixel on, 2 pixels off):")
dump_rgb(strip)
dump(strip)

print()
print("=== Demo 6: Edge case - zero pulse size ===")
# Test with zero pulse size (should show nothing)
crenel_anim.set_pulse_size(0)
engine.on_tick(tasmota.millis() + 50)  # Force update with new time
print("Zero pulse size (should be empty):")
dump_rgb(strip)
dump(strip)

print()
print("=== Demo 7: Position offset ===")
# Reset pulse size and test position offset
crenel_anim.set_pulse_size(2)
crenel_anim.set_low_size(3)
crenel_anim.set_pos(2)  # Start at position 2
crenel_anim.set_color(0xFF0000FF)  # Change to blue
engine.on_tick(tasmota.millis() + 60)  # Force update with new time
print("Pattern starting at position 2 (blue):")
dump_rgb(strip)
dump(strip)

print()
print("=== Demo 8: Negative position (wrapping) ===")
# Test negative position
crenel_anim.set_pos(-1)
engine.on_tick(tasmota.millis() + 70)  # Force update with new time
print("Pattern with negative position -1:")
dump_rgb(strip)
dump(strip)

print("Crenel Position animation demo completed")

# Stop the animation
engine.stop()

return true