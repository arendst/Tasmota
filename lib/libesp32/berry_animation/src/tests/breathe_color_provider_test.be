# Test file for Breathe Color Provider
#
# This file contains tests for the BreatheColorProvider class following parameterized class specification
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/breathe_color_provider_test.be

print("Testing BreatheColorProvider...")

# Import the core animation module
import animation
print("Imported animation module")

# Create LED strip and animation engine following specification
var strip = global.Leds(10)  # Use global.Leds() for testing as per specification
var engine = animation.create_engine(strip)
print("Created LED strip and animation engine")

# Create a breathe color provider with engine-only parameter
var provider = animation.breathe_color(engine)
print("Created breathe color provider with defaults")

# Test default values
print(f"Default base_color: 0x{provider.base_color :08x}")
print(f"Default min_brightness: {provider.min_brightness}")
print(f"Default max_brightness: {provider.max_brightness}")
print(f"Default duration: {provider.duration}")
print(f"Default curve_factor: {provider.curve_factor}")
print(f"Default form: {provider.form}")

# Verify inherited oscillator defaults
print(f"Inherited min_value: {provider.min_value}")
print(f"Inherited max_value: {provider.max_value}")

# Create another breathe color provider and set custom parameters using virtual member assignment
var blue_breathe = animation.breathe_color(engine)
blue_breathe.base_color = 0xFF0000FF
blue_breathe.min_brightness = 20
blue_breathe.max_brightness = 200
blue_breathe.duration = 4000
blue_breathe.curve_factor = 3
print(f"Blue breathe color provider base_color: 0x{blue_breathe.base_color :08x}")
print(f"Blue breathe color provider min_brightness: {blue_breathe.min_brightness}")
print(f"Blue breathe color provider max_brightness: {blue_breathe.max_brightness}")
print(f"Blue breathe color provider duration: {blue_breathe.duration}")
print(f"Blue breathe color provider curve_factor: {blue_breathe.curve_factor}")

# Create red breathe color provider with different parameters
var red_breathe = animation.breathe_color(engine)
red_breathe.base_color = 0xFFFF0000
red_breathe.min_brightness = 10
red_breathe.max_brightness = 180
red_breathe.duration = 3000
red_breathe.curve_factor = 2
print(f"Red breathe color provider base_color: 0x{red_breathe.base_color :08x}")

# Test parameter updates using virtual member assignment
blue_breathe.min_brightness = 30
blue_breathe.max_brightness = 220
blue_breathe.duration = 3500
blue_breathe.curve_factor = 4
print(f"Updated blue breathe min_brightness: {blue_breathe.min_brightness}")
print(f"Updated blue breathe max_brightness: {blue_breathe.max_brightness}")
print(f"Updated blue breathe duration: {blue_breathe.duration}")
print(f"Updated blue breathe curve_factor: {blue_breathe.curve_factor}")

# Test start method using engine time
engine.time_ms = 1000  # Set engine time for testing
var start_time = engine.time_ms
blue_breathe.start(start_time)
blue_breathe.produce_value(nil, start_time)   # force first tick
print(f"Started blue breathe color provider at time: {start_time}")

# Cache duration for performance (following specification)
var current_duration = blue_breathe.duration

# Test produce_value method at different points in the cycle
engine.time_ms = start_time + (current_duration / 10)
var color_1_10 = blue_breathe.produce_value("color", engine.time_ms)
print(f"Color at 1/10 cycle: 0x{color_1_10 :08x}")

engine.time_ms = start_time + (current_duration / 8)
var color_1_8 = blue_breathe.produce_value("color", engine.time_ms)
print(f"Color at 1/8 cycle: 0x{color_1_8 :08x}")

engine.time_ms = start_time + (3 * current_duration / 10)
var color_3_10 = blue_breathe.produce_value("color", engine.time_ms)
print(f"Color at 3/10 cycle: 0x{color_3_10 :08x}")

engine.time_ms = start_time + (current_duration / 4)
var color_1_4 = blue_breathe.produce_value("color", engine.time_ms)
print(f"Color at 1/4 cycle: 0x{color_1_4 :08x}")

engine.time_ms = start_time + (current_duration / 2)
var color_1_2 = blue_breathe.produce_value("color", engine.time_ms)
print(f"Color at 1/2 cycle: 0x{color_1_2 :08x}")

engine.time_ms = start_time + (3 * current_duration / 4)
var color_3_4 = blue_breathe.produce_value("color", engine.time_ms)
print(f"Color at 3/4 cycle: 0x{color_3_4 :08x}")

engine.time_ms = start_time + current_duration
var color_full = blue_breathe.produce_value("color", engine.time_ms)
print(f"Color at full cycle: 0x{color_full :08x}")

# Test curve factor effects
var curve_1_provider = animation.breathe_color(engine)
curve_1_provider.base_color = 0xFF00FF00  # Green
curve_1_provider.curve_factor = 1
curve_1_provider.duration = 2000
curve_1_provider.min_brightness = 50  # Set non-zero minimum to see differences
curve_1_provider.max_brightness = 255
curve_1_provider.start(engine.time_ms)
curve_1_provider.produce_value(nil, start_time)   # force first tick

var curve_5_provider = animation.breathe_color(engine)
curve_5_provider.base_color = 0xFF00FF00  # Green
curve_5_provider.curve_factor = 5
curve_5_provider.duration = 2000
curve_5_provider.min_brightness = 50  # Set non-zero minimum to see differences
curve_5_provider.max_brightness = 255
curve_5_provider.start(engine.time_ms)
curve_5_provider.produce_value(nil, start_time)   # force first tick

# Compare curve effects at different cycle points where differences will be visible
# Test at 3/8 cycle (375ms) where cosine is around 0.7, giving more room for curve differences
engine.time_ms = engine.time_ms + 375  # 3/8 of 2000ms cycle
var curve_1_color = curve_1_provider.produce_value("color", engine.time_ms)
var curve_5_color = curve_5_provider.produce_value("color", engine.time_ms)
print(f"Curve factor 1 at 3/8 cycle: 0x{curve_1_color :08x}")
print(f"Curve factor 5 at 3/8 cycle: 0x{curve_5_color :08x}")

# Test pulsating color provider factory
var pulsating = animation.pulsating_color(engine)
print(f"Pulsating provider curve_factor: {pulsating.curve_factor}")
print(f"Pulsating provider duration: {pulsating.duration}")

# Test parameter validation
try
  blue_breathe.min_brightness = 300  # Should fail validation (max is 255)
  assert(false, "Should have failed validation for min_brightness > 255")
except "value_error"
  print("✓ Min brightness validation working correctly")
end

try
  blue_breathe.max_brightness = 300  # Should fail validation (max is 255)
  assert(false, "Should have failed validation for max_brightness > 255")
except "value_error"
  print("✓ Max brightness validation working correctly")
end

try
  blue_breathe.curve_factor = 10  # Should fail validation (max is 5)
  assert(false, "Should have failed validation for curve_factor > 5")
except "value_error"
  print("✓ Curve factor validation working correctly")
end

try
  blue_breathe.curve_factor = 0  # Should fail validation (min is 1)
  assert(false, "Should have failed validation for curve_factor < 1")
except "value_error"
  print("✓ Curve factor minimum validation working correctly")
end

# Test brightness range behavior
var brightness_test = animation.breathe_color(engine)
brightness_test.base_color = 0xFFFFFFFF  # White
brightness_test.min_brightness = 50
brightness_test.max_brightness = 200
brightness_test.duration = 1000
brightness_test.start(engine.time_ms)
brightness_test.produce_value(nil, start_time)   # force first tick

# Test at quarter cycle (should be near minimum for this cosine implementation)
engine.time_ms = engine.time_ms + 250  # Quarter cycle
var min_color = brightness_test.produce_value("color", engine.time_ms)
var min_brightness_actual = min_color & 0xFF  # Blue component should match brightness
print(f"Min brightness test - expected around 53, got: {min_brightness_actual}")

# Test at three-quarter cycle (should be near maximum for this cosine implementation)
engine.time_ms = engine.time_ms + 500  # Move to 3/4 cycle
var max_color = brightness_test.produce_value("color", engine.time_ms)
var max_brightness_actual = max_color & 0xFF  # Blue component should match brightness
print(f"Max brightness test - expected around 200, got: {max_brightness_actual}")

# Test color preservation (alpha channel should be preserved)
var alpha_test = animation.breathe_color(engine)
alpha_test.base_color = 0x80FF0000  # Red with 50% alpha
alpha_test.start(engine.time_ms)
alpha_test.produce_value(nil, start_time)   # force first tick
var alpha_color = alpha_test.produce_value("color", engine.time_ms)
var alpha_actual = (alpha_color >> 24) & 0xFF
print(f"Alpha preservation test - expected 128, got: {alpha_actual}")

# Test tostring method
print(f"Provider string representation: {blue_breathe.tostring()}")

# Validate key test results
assert(provider != nil, "Default breathe color provider should be created")
assert(blue_breathe != nil, "Custom breathe color provider should be created")
assert(red_breathe != nil, "Red breathe color provider should be created")
assert(pulsating != nil, "Pulsating color provider should be created")
assert(blue_breathe.base_color == 0xFF0000FF, "Blue breathe should have correct base color")
assert(blue_breathe.min_brightness == 30, "Min brightness should be updated to 30")
assert(blue_breathe.max_brightness == 220, "Max brightness should be updated to 220")
assert(blue_breathe.duration == 3500, "Duration should be updated to 3500")
assert(blue_breathe.curve_factor == 4, "Curve factor should be updated to 4")
assert(blue_breathe.form == animation.COSINE, "Form should be COSINE")
assert(blue_breathe.min_value == 0, "Inherited min_value should be 0")
assert(blue_breathe.max_value == 255, "Inherited max_value should be 255")
assert(blue_breathe.engine == engine, "Provider should have correct engine reference")
assert(pulsating.curve_factor == 1, "Pulsating provider should have curve_factor 1")
assert(pulsating.duration == 1000, "Pulsating provider should have duration 1000")
assert(alpha_actual == 128, "Alpha channel should be preserved")

# Test that colors are different at different cycle points (breathing effect working)
assert(color_1_10 != color_1_2, "Colors should be different at different cycle points")
assert(color_1_4 != color_3_4, "Colors should be different at quarter vs three-quarter cycle")

# Test that curve factors produce different results
assert(curve_1_color != curve_5_color, "Different curve factors should produce different colors")

# Test brightness range is respected (allowing for curve factor and timing variations)
assert(min_brightness_actual >= 45 && min_brightness_actual <= 65, "Min brightness should be around 53")
assert(max_brightness_actual >= 150 && max_brightness_actual <= 170, "Max brightness should be around 160")

print("All tests completed successfully!")
return true