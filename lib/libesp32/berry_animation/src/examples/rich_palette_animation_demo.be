# RichPalette Animation Demo with LED Values
#
# This example demonstrates the Filled animation with a rich palette provider
# which smoothly transitions between colors in a compact palette format.
# It also displays the actual LED values during the animation.

import global
import tasmota
import animation
import math

# Helper functions to display LED values
def dump_rgb(strip)
  var output = "LEDs: ["
  for i:0..math.min(4, strip.length()-1)  # Show first 5 LEDs for readability
    var color = strip.get_pixel_color(i)
    var r = (color >> 16) & 0xFF
    var g = (color >> 8) & 0xFF
    var b = color & 0xFF
    
    if i > 0
      output += ", "
    end
    output += format("R:%d,G:%d,B:%d", r, g, b)
  end
  if strip.length() > 5
    output += ", ..."  # Indicate there are more LEDs
  end
  output += "]"
  print(output)
end

def dump_hex(strip)
  var output = "Hex: ["
  for i:0..math.min(4, strip.length()-1)  # Show first 5 LEDs for readability
    if i > 0
      output += ", "
    end
    output += format("0x%06X", strip.get_pixel_color(i) & 0xFFFFFF)
  end
  if strip.length() > 5
    output += ", ..."  # Indicate there are more LEDs
  end
  output += "]"
  print(output)
end

def simulate_animation(controller, strip, duration, interval, description)
  print(f"\n--- {description} ---")
  
  var start_time = tasmota.millis()
  var end_time = start_time + duration
  var update_interval = interval
  
  # Simulate updates
  var current_time = start_time
  while current_time < end_time
    # Update the controller with the current time
    controller.on_tick(current_time)
    
    # Show the current state with detailed LED values
    print(f"Time: {current_time - start_time}ms, Progress: {(current_time - start_time) * 100 / duration}%")
    dump_rgb(strip)
    dump_hex(strip)
    
    # Increment time
    current_time += update_interval
  end
  
  print(f"--- End of {description} ---\n")
end

# Create a mock LED strip with 10 pixels (reduced for readability)
var strip = global.Leds(10)

# Create an animation controller for the strip
var controller = animation.animation_controller(strip)

print("Starting RichPalette animation demo with LED values...")

# Example 1: Rainbow palette
print("\nExample 1: Rainbow palette")
var rainbow = animation.rich_palette_animation(animation.PALETTE_RAINBOW, 2000, 1, 255, 0, 255)
rainbow.start()  # Start the animation
controller.add_animation(rainbow)
controller.start()

# Simulate the animation for one cycle
simulate_animation(controller, strip, 2500, 200, "Rainbow Palette Animation")

controller.stop()
controller.clear()

# Example 2: Fire effect palette
print("\nExample 2: Fire effect palette")
var fire_palette = animation.rich_palette_animation(animation.PALETTE_FIRE, 2000, 1, 255, 0, 255)
fire_palette.start()  # Start the animation
controller.add_animation(fire_palette)
controller.start()

# Simulate the animation for one cycle
simulate_animation(controller, strip, 2500, 200, "Fire Effect Animation")

controller.stop()
controller.clear()

# Example 3: Tick-based palette
print("\nExample 3: Tick-based palette")
var tick_palette = animation.rich_palette_animation(animation.PALETTE_SUNSET_TICKS, 2000, 1, 255, 0, 255)
tick_palette.start()  # Start the animation
controller.add_animation(tick_palette)
controller.start()

# Simulate the animation for one cycle
simulate_animation(controller, strip, 2500, 200, "Tick-based Palette Animation")

controller.stop()
controller.clear()

# Example 4: Custom palette with get_color_for_value demonstration
print("\nExample 4: Custom palette with get_color_for_value demonstration")
var custom_palette_bytes = bytes(
  "00FF0000"    # Red (value 0)
  "3200FF00"    # Green (value 50)
  "64FFFF00"    # Yellow (value 100)
  "96FF00FF"    # Purple (value 150)
  "C80000FF"    # Blue (value 200)
  "FFFFFFFF"    # White (value 255)
)

# Create a rich palette provider first
var rich_palette_provider = animation.rich_palette_color_provider(custom_palette_bytes, 2000, 1, 255)

# Set the range for the palette (0-200)
rich_palette_provider.set_range(0, 200)

# Create a filled animation with this provider
var custom_palette = animation.filled_animation(rich_palette_provider, 0, 255)

# Demonstrate get_color_for_value
print("Demonstrating get_color_for_value:")
for value : [0, 25, 50, 75, 100, 125, 150, 175, 200]
  var color = rich_palette_provider.get_color_for_value(value, tasmota.millis())
  var r = (color >> 16) & 0xFF
  var g = (color >> 8) & 0xFF
  var b = color & 0xFF
  print(f"Value: {value}, Color: 0x{color & 0xFFFFFF:06X} (R:{r}, G:{g}, B:{b})")
end

# Now use the palette for animation
custom_palette.start()  # Start the animation
controller.add_animation(custom_palette)
controller.start()

# Simulate the animation for one cycle
simulate_animation(controller, strip, 2500, 200, "Custom Palette Animation")

controller.stop()
controller.clear()

# Example 5: Comparing linear vs sine transition
print("\nExample 5: Linear vs Sine transition")

# Linear transition
print("\nLinear transition:")
var linear = animation.rich_palette_animation(animation.PALETTE_RGB, 2000, 0, 255, 0, 255)  # Linear transition
linear.start()  # Start the animation
controller.add_animation(linear)
controller.start()

# Simulate the animation for one cycle
simulate_animation(controller, strip, 2500, 200, "Linear Transition Animation")

controller.stop()
controller.clear()

# Sine transition
print("\nSine transition:")
var sine = animation.rich_palette_animation(animation.PALETTE_RGB, 2000, 1, 255, 0, 255)  # Sine transition
sine.start()  # Start the animation
controller.add_animation(sine)
controller.start()

# Simulate the animation for one cycle
simulate_animation(controller, strip, 2500, 200, "Sine Transition Animation")

controller.stop()
controller.clear()

print("\nRichPalette animation demo completed")

return true