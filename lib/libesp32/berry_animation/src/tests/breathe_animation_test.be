# Test file for Breathe animation effect
#
# This file contains tests for the BreatheAnimation class following parameterized class specification
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/breathe_animation_test.be

print("Testing BreatheAnimation...")

# Import the core animation module
import animation
print("Imported animation module")

# Create LED strip and animation engine following specification
var strip = global.Leds(10)  # Use global.Leds() for testing as per specification
var engine = animation.create_engine(strip)
print("Created LED strip and animation engine")

# Create a breathe animation with engine-only parameter
var anim = animation.breathe_animation(engine)
print("Created breathe animation with defaults")

# Test default values
print(f"Default base_color: 0x{anim.base_color :08x}")
print(f"Default min_brightness: {anim.min_brightness}")
print(f"Default max_brightness: {anim.max_brightness}")
print(f"Default period: {anim.period}")
print(f"Default curve_factor: {anim.curve_factor}")

# Create another breathe animation and set custom parameters using virtual member assignment
var blue_breathe = animation.breathe_animation(engine)
blue_breathe.base_color = 0xFF0000FF
blue_breathe.min_brightness = 20
blue_breathe.max_brightness = 200
blue_breathe.period = 4000
blue_breathe.curve_factor = 3
blue_breathe.priority = 15
print(f"Blue breathe animation base_color: 0x{blue_breathe.base_color :08x}")
print(f"Blue breathe animation min_brightness: {blue_breathe.min_brightness}")
print(f"Blue breathe animation max_brightness: {blue_breathe.max_brightness}")
print(f"Blue breathe animation period: {blue_breathe.period}")
print(f"Blue breathe animation curve_factor: {blue_breathe.curve_factor}")

# Create red breathe animation with different parameters
var red_breathe = animation.breathe_animation(engine)
red_breathe.base_color = 0xFFFF0000
red_breathe.min_brightness = 10
red_breathe.max_brightness = 180
red_breathe.period = 3000
red_breathe.curve_factor = 2
print(f"Red breathe animation base_color: 0x{red_breathe.base_color :08x}")

# Test parameter updates using virtual member assignment
blue_breathe.min_brightness = 30
blue_breathe.max_brightness = 220
blue_breathe.period = 3500
blue_breathe.curve_factor = 4
print(f"Updated blue breathe min_brightness: {blue_breathe.min_brightness}")
print(f"Updated blue breathe max_brightness: {blue_breathe.max_brightness}")
print(f"Updated blue breathe period: {blue_breathe.period}")
print(f"Updated blue breathe curve_factor: {blue_breathe.curve_factor}")

# Test update method using engine time
engine.time_ms = 1000  # Set engine time for testing
var start_time = engine.time_ms
blue_breathe.start(start_time)
print(f"Started blue breathe animation at time: {start_time}")

# Cache period for performance (following specification)
var current_period = blue_breathe.period

# Test at different points in the cycle - check color output instead of brightness
engine.time_ms = start_time + (current_period / 10)
blue_breathe.update(engine.time_ms)
var color_1_10 = blue_breathe.color
print(f"Color at 1/10 cycle: 0x{color_1_10 :08x}")

engine.time_ms = start_time + (current_period / 8)
blue_breathe.update(engine.time_ms)
var color_1_8 = blue_breathe.color
print(f"Color at 1/8 cycle: 0x{color_1_8 :08x}")

engine.time_ms = start_time + (3 * current_period / 10)
blue_breathe.update(engine.time_ms)
var color_3_10 = blue_breathe.color
print(f"Color at 3/10 cycle: 0x{color_3_10 :08x}")

engine.time_ms = start_time + (current_period / 4)
blue_breathe.update(engine.time_ms)
var color_1_4 = blue_breathe.color
print(f"Color at 1/4 cycle: 0x{color_1_4 :08x}")

engine.time_ms = start_time + (current_period / 2)
blue_breathe.update(engine.time_ms)
var color_1_2 = blue_breathe.color
print(f"Color at 1/2 cycle: 0x{color_1_2 :08x}")

engine.time_ms = start_time + (3 * current_period / 4)
blue_breathe.update(engine.time_ms)
var color_3_4 = blue_breathe.color
print(f"Color at 3/4 cycle: 0x{color_3_4 :08x}")

engine.time_ms = start_time + current_period
blue_breathe.update(engine.time_ms)
var color_full = blue_breathe.color
print(f"Color at full cycle: 0x{color_full :08x}")

# Test rendering
var frame = animation.frame_buffer(5)
blue_breathe.render(frame, engine.time_ms)
print(f"First pixel after rendering: 0x{frame.get_pixel_color(0) :08x}")

# Test parameter validation
try
  blue_breathe.min_brightness = 300  # Should fail validation (max is 255)
  assert(false, "Should have failed validation for min_brightness > 255")
except "value_error"
  print("✓ Parameter validation working correctly")
end

try
  blue_breathe.curve_factor = 10  # Should fail validation (max is 5)
  assert(false, "Should have failed validation for curve_factor > 5")
except "value_error"
  print("✓ Curve factor validation working correctly")
end

# Test engine integration
engine.add(blue_breathe)
print("✓ Animation added to engine successfully")

# Validate key test results
assert(anim != nil, "Default breathe animation should be created")
assert(blue_breathe != nil, "Custom breathe animation should be created")
assert(red_breathe != nil, "Red breathe animation should be created")
assert(blue_breathe.base_color == 0xFF0000FF, "Blue breathe should have correct base_color")
assert(blue_breathe.min_brightness == 30, "Min brightness should be updated to 30")
assert(blue_breathe.max_brightness == 220, "Max brightness should be updated to 220")
assert(blue_breathe.period == 3500, "Breathe period should be updated to 3500")
assert(blue_breathe.curve_factor == 4, "Curve factor should be updated to 4")
assert(blue_breathe.is_running, "Blue breathe should be running after start")
assert(frame.get_pixel_color(0) != 0x00000000, "First pixel should not be black after rendering")
assert(blue_breathe.engine == engine, "Animation should have correct engine reference")
assert(blue_breathe.breathe_provider != nil, "Animation should have internal breathe provider")

print("All tests completed successfully!")
return true