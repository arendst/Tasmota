# PalettePattern Animation Demo
#
# This example demonstrates how to use the RichPaletteAnimation as a color provider
# for the PalettePatternAnimation to create complex visual effects.

# Import the animation module
import global
import tasmota
import animation

# Create an animation controller for a strip of 30 LEDs
var strip = global.Leds(30, 1)  # Assuming global.Leds class is available in Tasmota
var controller = animation.animation_controller(strip)

# Example 1: Wave pattern with rainbow palette
print("Example 1: Wave pattern with rainbow palette")

# Create a rainbow palette that doesn't animate on its own (no rendering)
var rainbow_palette = animation.rich_palette_animation(animation.PALETTE_RAINBOW, 5000)

# Create a wave pattern that uses the rainbow palette as a color source
var wave_pattern = animation.palette_pattern.wave(rainbow_palette, 30, 3000, 15, 10, 255)

# Add the animations to the controller
controller.add_animation(rainbow_palette)
controller.add_animation(wave_pattern)

# Start the animations
controller.start()
tasmota.delay(10000)  # Run for 10 seconds
controller.stop()
controller.clear()

# Example 2: Gradient pattern with fire palette
print("Example 2: Gradient pattern with fire palette")

# Create a fire palette
var fire_palette = animation.rich_palette_animation(animation.PALETTE_FIRE, 3000)

# Create a gradient pattern that uses the fire palette as a color source
var gradient_pattern = animation.palette_pattern.gradient(fire_palette, 30, 5000, 10, 255)

# Add the animations to the controller
controller.add_animation(fire_palette)
controller.add_animation(gradient_pattern)

# Start the animations
controller.start()
tasmota.delay(10000)  # Run for 10 seconds
controller.stop()
controller.clear()

# Example 3: Value meter with ocean palette
print("Example 3: Value meter with ocean palette")

# Create an ocean palette
var ocean_palette = animation.rich_palette_animation(animation.PALETTE_OCEAN, 1000)

# Create a value function that oscillates between 0 and 100
def value_function(time_ms, animation)
  # Oscillate between 0 and 100 over 5 seconds using scale_uint and sine_int for better precision
  var angle = tasmota.scale_uint(time_ms % 5000, 0, 5000, 0, 32767)  # 0 to 2π in fixed-point
  var cosine_value = tasmota.sine_int(angle + 8192)  # Offset by π/2 to get cosine
  
  # Map cosine value from -4096..4096 to 0..100
  return tasmota.scale_int(cosine_value, -4096, 4096, 0, 100)
end

# Create a value meter that uses the ocean palette as a color source
var meter_pattern = animation.palette_pattern.value_meter(ocean_palette, 30, value_function, 10, 255)

# Add the animations to the controller
controller.add_animation(ocean_palette)
controller.add_animation(meter_pattern)

# Start the animations
controller.start()
tasmota.delay(10000)  # Run for 10 seconds
controller.stop()
controller.clear()

# Example 4: Custom pattern with custom palette
print("Example 4: Custom pattern with custom palette")

# Create a custom palette
var custom_palette = animation.rich_palette_animation.from_bytes(bytes(
  "00FF0000"    # Red (value 0)
  "3200FF00"    # Green (value 50)
  "64FFFF00"    # Yellow (value 100)
  "96FF00FF"    # Purple (value 150)
  "C80000FF"    # Blue (value 200)
  "FFFFFFFF"    # White (value 255)
), 5000)

# Set the range for the palette (0-200 instead of 0-100)
custom_palette.set_range(0, 200)

# Create a custom pattern function
def custom_pattern_func(pixel_index, time_ms, animation)
  # Create a pattern that depends on both position and time using scale_uint for better precision
  var time_factor = tasmota.scale_uint(time_ms % 10000, 0, 10000, 0, 100)
  var position_factor = tasmota.scale_uint(pixel_index, 0, animation.frame_width - 1, 0, 100)
  
  # Calculate a value between 0 and 200
  var value = position_factor + time_factor
  
  return value
end

# Create a custom pattern that uses the custom palette as a color source
var custom_pattern = animation.palette_pattern(custom_palette, custom_pattern_func, 30, 10, 255)

# Add the animations to the controller
controller.add_animation(custom_palette)
controller.add_animation(custom_pattern)

# Start the animations
controller.start()
tasmota.delay(10000)  # Run for 10 seconds
controller.stop()
controller.clear()

print("PalettePattern animation demo completed")

return true