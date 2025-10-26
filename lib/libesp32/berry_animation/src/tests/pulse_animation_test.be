# Test file for Pulse animation effect
#
# This file contains tests for the PulseAnimation class
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/pulse_animation_test.be

print("Testing PulseAnimation...")

# First import the core animation module
import animation
print("Imported animation module")

# Create LED strip and engine for testing
var strip = global.Leds(10)  # Use global.Leds() for testing as per specification
var engine = animation.create_engine(strip)
print("Created engine and LED strip")

# Create a pulse animation with new constructor (engine only)
var anim = animation.pulsating_animation(engine)
print("Created pulse animation with defaults")

# Test default values
print(f"Default base_color: 0x{anim.base_color :08x}")
print(f"Default min_brightness: {anim.min_brightness}")
print(f"Default max_brightness: {anim.max_brightness}")
print(f"Default period: {anim.period}")
print(f"Default curve_factor: {anim.curve_factor}")  # Should be 1 for pulsating

# Test with custom parameters using virtual member assignment
var blue_pulse = animation.pulsating_animation(engine)
blue_pulse.base_color = 0xFF0000FF
blue_pulse.min_brightness = 50
blue_pulse.max_brightness = 200
blue_pulse.period = 2000
print(f"Blue pulse animation base_color: 0x{blue_pulse.base_color :08x}")
print(f"Blue pulse animation min_brightness: {blue_pulse.min_brightness}")
print(f"Blue pulse animation max_brightness: {blue_pulse.max_brightness}")
print(f"Blue pulse animation period: {blue_pulse.period}")
print(f"Blue pulse animation curve_factor: {blue_pulse.curve_factor}")

# Test creating another pulse with different parameters
var red_pulse = animation.pulsating_animation(engine)
red_pulse.base_color = 0xFFFF0000  # Red color
red_pulse.min_brightness = 20
red_pulse.max_brightness = 180
red_pulse.period = 1500
print(f"Red pulse animation base_color: 0x{red_pulse.base_color :08x}")

# Test parameter updates using virtual member assignment
blue_pulse.min_brightness = 30
blue_pulse.max_brightness = 220
blue_pulse.period = 1800
print(f"Updated blue pulse min_brightness: {blue_pulse.min_brightness}")
print(f"Updated blue pulse max_brightness: {blue_pulse.max_brightness}")
print(f"Updated blue pulse period: {blue_pulse.period}")

# Test update method
var start_time = engine.time_ms
blue_pulse.start(start_time)
print(f"Started blue pulse animation at time: {start_time}")

# Test at different points in the cycle - check color output instead of brightness
engine.time_ms = start_time + (blue_pulse.period / 10)
blue_pulse.update(engine.time_ms)
var color_1_10 = blue_pulse.color
print(f"Color at 1/10 cycle: 0x{color_1_10 :08x}")

engine.time_ms = start_time + (blue_pulse.period / 8)
blue_pulse.update(engine.time_ms)
var color_1_8 = blue_pulse.color
print(f"Color at 1/8 cycle: 0x{color_1_8 :08x}")

engine.time_ms = start_time + (3 * blue_pulse.period / 10)
blue_pulse.update(engine.time_ms)
var color_3_10 = blue_pulse.color
print(f"Color at 3/10 cycle: 0x{color_3_10 :08x}")

engine.time_ms = start_time + (blue_pulse.period / 4)
blue_pulse.update(engine.time_ms)
var color_1_4 = blue_pulse.color
print(f"Color at 1/4 cycle: 0x{color_1_4 :08x}")

engine.time_ms = start_time + (blue_pulse.period / 2)
blue_pulse.update(engine.time_ms)
var color_1_2 = blue_pulse.color
print(f"Color at 1/2 cycle: 0x{color_1_2 :08x}")

engine.time_ms = start_time + (3 * blue_pulse.period / 4)
blue_pulse.update(engine.time_ms)
var color_3_4 = blue_pulse.color
print(f"Color at 3/4 cycle: 0x{color_3_4 :08x}")

engine.time_ms = start_time + blue_pulse.period
blue_pulse.update(engine.time_ms)
var color_full = blue_pulse.color
print(f"Color at full cycle: 0x{color_full :08x}")

# Test rendering
var frame = animation.frame_buffer(5)
blue_pulse.render(frame, engine.time_ms)
print(f"First pixel after rendering: 0x{frame.get_pixel_color(0) :08x}")

# Validate key test results
assert(anim != nil, "Default pulse animation should be created")
assert(blue_pulse != nil, "Custom pulse animation should be created")
assert(blue_pulse.base_color == 0xFF0000FF, "Blue pulse should have correct base_color")
assert(blue_pulse.min_brightness == 30, "Min brightness should be updated to 30")
assert(blue_pulse.max_brightness == 220, "Max brightness should be updated to 220")
assert(blue_pulse.period == 1800, "Pulse period should be updated to 1800")
assert(blue_pulse.curve_factor == 1, "Pulse should have curve_factor = 1 for pure sine wave")
assert(frame.get_pixel_color(0) != 0x00000000, "First pixel should not be black after rendering")
assert(blue_pulse.breathe_provider != nil, "Animation should have internal breathe provider")

print("All tests completed successfully!")
return true