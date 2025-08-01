# Color Provider Demo for Berry Animation Framework
#
# This example demonstrates the use of color providers to generate colors
# for different purposes, including filling frames and creating patterns.

import tasmota
import animation

# Create a frame buffer for testing
var frame_width = 30
var frame = animation.frame_buffer(frame_width, 1)

# Create a rich palette color provider
print("Creating a rich palette color provider...")
var palette_provider = animation.rich_palette_color_provider(
  animation.PALETTE_RAINBOW,  # Use the rainbow palette
  5000,                       # 5 second cycle period
  1,                          # Sine transition
  255                         # Full brightness
)

# Create a color cycle color provider
print("Creating a color cycle color provider...")
var cycle_provider = animation.color_cycle_color_provider(
  [0xFF0000FF, 0xFF00FF00, 0xFFFF0000],  # RGB colors
  3000,                                  # 3 second cycle period
  1                                      # Sine transition
)

# Create a solid color provider
print("Creating a solid color provider...")
var solid_provider = animation.solid_color_provider(0xFFFF0000)  # Red

# Create a composite color provider that combines the palette and cycle providers
print("Creating a composite color provider...")
var composite_provider = animation.composite_color_provider(
  [palette_provider, cycle_provider],  # List of providers to combine
  0                                   # Overlay blend mode
)

# Demonstrate getting colors from providers
print("Demonstrating color providers...")

# Get colors at different times
var time_ms = tasmota.millis()
print(f"Current time: {time_ms} ms")

print("Colors at current time:")
print(f"  Palette provider: 0x{palette_provider.get_color(time_ms):08X}")
print(f"  Cycle provider: 0x{cycle_provider.get_color(time_ms):08X}")
print(f"  Solid provider: 0x{solid_provider.get_color(time_ms):08X}")
print(f"  Composite provider: 0x{composite_provider.get_color(time_ms):08X}")

# Get colors for different values
print("Colors for value 50:")
print(f"  Palette provider: 0x{palette_provider.get_color_for_value(50, time_ms):08X}")
print(f"  Cycle provider: 0x{cycle_provider.get_color_for_value(50, time_ms):08X}")
print(f"  Solid provider: 0x{solid_provider.get_color_for_value(50, time_ms):08X}")
print(f"  Composite provider: 0x{composite_provider.get_color_for_value(50, time_ms):08X}")

# Create a pattern function that generates a wave pattern
def wave_pattern(pixel_index, time_ms, animation)
  var wave_period = 2000   # 2 second period
  var wave_length = 30     # 30 pixel wavelength
  
  # Calculate the wave position
  var position = (time_ms % wave_period) / wave_period
  var offset = int(position * wave_length)
  
  # Calculate the wave value (0-100)
  var pos_in_wave = (pixel_index + offset) % wave_length
  var angle = tasmota.scale_uint(pos_in_wave, 0, wave_length, 0, 32767)  # 0 to 2π in fixed-point
  var sine_value = tasmota.sine_int(angle)  # -4096 to 4096
  
  # Map sine value from -4096..4096 to 0..100
  var value = tasmota.scale_int(sine_value, -4096, 4096, 0, 100)
  
  return value
end

# Fill the frame with colors from different providers
print("Filling frame with colors from different providers...")

# Fill with palette provider
frame.clear()
for i:0..frame_width-1
  var value = wave_pattern(i, time_ms, nil)
  var color = palette_provider.get_color_for_value(value, time_ms)
  frame.set_pixel_color(i, color)
end
print("Frame filled with palette provider colors")

# Fill with cycle provider
frame.clear()
for i:0..frame_width-1
  var value = wave_pattern(i, time_ms, nil)
  var color = cycle_provider.get_color_for_value(value, time_ms)
  frame.set_pixel_color(i, color)
end
print("Frame filled with cycle provider colors")

# Fill with composite provider
frame.clear()
for i:0..frame_width-1
  var value = wave_pattern(i, time_ms, nil)
  var color = composite_provider.get_color_for_value(value, time_ms)
  frame.set_pixel_color(i, color)
end
print("Frame filled with composite provider colors")

# Create a PalettePatternAnimation that uses a color provider
print("Creating a PalettePatternAnimation with a color provider...")
var pattern_animation = animation.palette_pattern(
  palette_provider,    # Use the palette provider as the color source
  wave_pattern,        # Use the wave pattern function
  frame_width,         # Frame width
  10,                  # Priority
  0,                   # Duration (0 = infinite)
  true,                # Loop
  "wave_pattern"       # Name
)

# Start the animation
pattern_animation.start()

# Update and render the animation
print("Updating and rendering the animation...")
pattern_animation.update(time_ms)
frame.clear()
pattern_animation.render(frame, time_ms)
print("Animation rendered to frame")

print("Color Provider Demo completed successfully!")