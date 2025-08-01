# Pulse Position Animation Demo with LED Values
#
# This example demonstrates the Pulse Position animation effect
# which creates a pulse at a specific position with optional slew (fade) regions.
#
# The demo creates various pulse configurations and shows actual LED values.
#
# Command to run demo:
#    ./berry -s -g -m lib/libesp32/berry_animation lib/libesp32/berry_animation/examples/pulse_position_animation_demo.be

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

def test_pulse_position(name, pulse_anim, strip, controller)
  print(f"\n=== {name} ===")
  
  # Clear any existing animations
  controller.clear()
  
  # Add and start the animation
  controller.add_animation(pulse_anim)
  pulse_anim.start()
  controller.start()
  
  # Update once to render the pulse
  controller.on_tick()
  
  # Show the results
  print(f"Animation: {pulse_anim}")
  dump_rgb(strip)
  dump(strip)
  
  # Stop the animation
  controller.stop()
end

print("Starting Pulse Position animation demo with LED values...")

# Create a mock LED strip with 10 pixels for better visualization
var strip = global.Leds(10)

# Create an animation engine for the strip
var controller = animation.create_engine(strip)

print("Testing various pulse position configurations...")

# Test 1: Basic pulse without slew
var basic_pulse = animation.pulse_position(0xFF00FF00, 3, 0, 1, 0, true, "basic_pulse")
basic_pulse.set_pos(2)
test_pulse_position("Basic Pulse (Green, pos=2, size=3, no slew)", basic_pulse, strip, controller)

# Test 2: Pulse with slew
var slew_pulse = animation.pulse_position(0xFFFF0000, 2, 2, 1, 0, true, "slew_pulse")
slew_pulse.set_pos(4)
test_pulse_position("Pulse with Slew (Red, pos=4, size=2, slew=2)", slew_pulse, strip, controller)

# Test 3: Pulse with background color
var bg_pulse = animation.pulse_position(0xFFFFFF00, 1, 1, 1, 0, true, "bg_pulse")
bg_pulse.set_pos(6)
bg_pulse.set_back_color(0xFF000080)  # Dark blue background
test_pulse_position("Pulse with Background (Yellow on Blue, pos=6, size=1, slew=1)", bg_pulse, strip, controller)

# Test 4: Wide pulse with large slew
var wide_pulse = animation.pulse_position(0xFF0080FF, 2, 3, 1, 0, true, "wide_pulse")
wide_pulse.set_pos(3)
test_pulse_position("Wide Pulse (Purple, pos=3, size=2, slew=3)", wide_pulse, strip, controller)

# Test 5: Edge case - pulse at beginning
var edge_pulse1 = animation.pulse_position(0xFFFF8000, 2, 1, 1, 0, true, "edge_pulse1")
edge_pulse1.set_pos(0)
test_pulse_position("Edge Pulse at Start (Orange, pos=0, size=2, slew=1)", edge_pulse1, strip, controller)

# Test 6: Edge case - pulse at end
var edge_pulse2 = animation.pulse_position(0xFF8000FF, 2, 1, 1, 0, true, "edge_pulse2")
edge_pulse2.set_pos(8)
test_pulse_position("Edge Pulse at End (Magenta, pos=8, size=2, slew=1)", edge_pulse2, strip, controller)

# Test 7: Zero-width pulse (should show only slew)
var zero_pulse = animation.pulse_position(0xFF00FFFF, 0, 2, 1, 0, true, "zero_pulse")
zero_pulse.set_pos(5)
test_pulse_position("Zero-width Pulse (Cyan, pos=5, size=0, slew=2)", zero_pulse, strip, controller)

# Test 8: Parameter updates - demonstrate different configurations
print("\n=== Parameter Variations ===")

# Test different colors
var red_pulse = animation.pulse_position(0xFFFF0000, 1, 0, 1, 0, true, "red_pulse")
red_pulse.set_pos(2)
test_pulse_position("Red Pulse", red_pulse, strip, controller)

var blue_pulse = animation.pulse_position(0xFF0000FF, 1, 0, 1, 0, true, "blue_pulse")
blue_pulse.set_pos(2)
test_pulse_position("Blue Pulse (same position)", blue_pulse, strip, controller)

# Test different sizes
var small_pulse = animation.pulse_position(0xFF00FF00, 1, 1, 1, 0, true, "small_pulse")
small_pulse.set_pos(4)
test_pulse_position("Small Pulse with Slew", small_pulse, strip, controller)

var large_pulse = animation.pulse_position(0xFF00FF00, 4, 1, 1, 0, true, "large_pulse")
large_pulse.set_pos(2)
test_pulse_position("Large Pulse with Slew", large_pulse, strip, controller)

print("\nPulse Position animation demo completed")
print("All tests demonstrate the pulse positioning with optional slew regions")

return true