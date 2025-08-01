# CrenelPositionAnimation Color Demo
#
# This example demonstrates the fixed CrenelPositionAnimation that can handle
# both integer colors and ColorProvider instances.

import animation

print("=== CrenelPositionAnimation Color Demo ===")

# Create a frame buffer for testing
var frame_width = 20
var frame = animation.frame_buffer(frame_width)

# Create an animation engine
var strip = global.Leds(frame_width, 1)
var engine = animation.create_engine(strip)

print("1. Creating CrenelPositionAnimation with integer color...")

# Create a crenel animation with a static red color
var red_crenel = animation.crenel_position_animation(
  0xFFFF0000,  # Red color (integer)
  0,           # pos
  3,           # pulse_size
  2,           # low_size
  4,           # nb_pulse
  10,          # priority
  0,           # duration (infinite)
  true,        # loop
  "red_crenel"
)

# Add to engine and render
engine.add_animation(red_crenel)
engine.start()

# Simulate a frame update
engine.on_tick(tasmota.millis())
print("✓ Red crenel animation created and rendered successfully")

print("2. Creating CrenelPositionAnimation with SolidColorProvider...")

# Create a solid color provider for blue
var blue_provider = animation.solid_color_provider(0xFF0000FF)  # Blue

# Create a crenel animation with the color provider
var blue_crenel = animation.crenel_position_animation(
  blue_provider,  # Blue ColorProvider
  5,              # pos
  2,              # pulse_size
  3,              # low_size
  3,              # nb_pulse
  15,             # priority (higher than red)
  0,              # duration (infinite)
  true,           # loop
  "blue_crenel"
)

# Add to engine
engine.add_animation(blue_crenel)

# Simulate another frame update
engine.on_tick(tasmota.millis())
print("✓ Blue crenel animation with ColorProvider created and rendered successfully")

print("3. Creating CrenelPositionAnimation with dynamic RichPaletteColorProvider...")

# Create a dynamic color provider that cycles through rainbow colors
var rainbow_provider = animation.rich_palette_color_provider(
  animation.PALETTE_RAINBOW,  # Rainbow palette
  3000,                       # 3 second cycle
  1,                          # Sine transition
  255                         # Full brightness
)

# Create a crenel animation with the dynamic color provider
var rainbow_crenel = animation.crenel_position_animation(
  rainbow_provider,  # Dynamic rainbow ColorProvider
  10,                # pos
  4,                 # pulse_size
  1,                 # low_size
  2,                 # nb_pulse
  20,                # priority (highest)
  0,                 # duration (infinite)
  true,              # loop
  "rainbow_crenel"
)

# Add to engine
engine.add_animation(rainbow_crenel)

# Simulate frame updates at different times to show color changes
for time_offset : [0, 1000, 2000, 3000]
  engine.on_tick(tasmota.millis() + time_offset)
  print(f"✓ Rainbow crenel rendered at time offset {time_offset}ms")
end

print("4. Testing set_color method with both types...")

# Test changing from integer to ColorProvider
var test_crenel = animation.crenel_position_animation(
  0xFF00FF00,  # Start with green integer
  15, 2, 1, 1, 5, 0, true, "test_crenel"
)

engine.add_animation(test_crenel)
engine.on_tick(tasmota.millis())
print("✓ Test crenel created with integer color")

# Change to a ColorProvider
var yellow_provider = animation.solid_color_provider(0xFFFFFF00)  # Yellow
test_crenel.set_color(yellow_provider)
engine.on_tick(tasmota.millis())
print("✓ Test crenel color changed to ColorProvider")

# Change back to integer color
test_crenel.set_color(0xFFFF00FF)  # Magenta
engine.on_tick(tasmota.millis())
print("✓ Test crenel color changed back to integer")

print("5. Displaying string representations...")

print(f"Red crenel: {red_crenel}")
print(f"Blue crenel: {blue_crenel}")
print(f"Rainbow crenel: {rainbow_crenel}")
print(f"Test crenel: {test_crenel}")

print("6. Testing with generic ValueProvider (not ColorProvider)...")

# Create a static value provider with a color value
var static_provider = animation.static_value_provider(0xFFFFFF00)  # Yellow
var static_crenel = animation.crenel_position_animation(
  static_provider,  # Generic ValueProvider
  0, 1, 4, 1, 5, 0, true, "static_provider_crenel"
)

engine.add_animation(static_crenel)
engine.on_tick(tasmota.millis())
print("✓ Generic ValueProvider crenel created and rendered successfully")

print("=== CrenelPositionAnimation Color Demo completed successfully! ===")
print("")
print("Summary of the enhanced fix:")
print("- CrenelPositionAnimation now correctly handles integer colors and ANY ValueProvider subclass")
print("- The parameter system accepts ValueProvider instances for integer parameters")
print("- ColorProvider instances use get_color(time_ms) for optimal color handling")
print("- Generic ValueProvider instances use get_value(time_ms) for flexibility")
print("- Range validation is bypassed for ValueProviders (since they provide dynamic values)")
print("- The set_color() method accepts integers, ColorProviders, and any ValueProvider")
print("- String representation handles all types appropriately")
print("- All existing functionality with integer colors remains unchanged")
print("- The type system is now extensible for future ValueProvider subclasses")
print("- Clean, single method call: var color = self.get_param_value('color', tasmota.millis())")