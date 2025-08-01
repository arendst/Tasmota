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

# Create a pulse animation with default parameters
var anim = animation.pulse_animation()
print("Created pulse animation with defaults")

# Test default values
print(f"Default color: 0x{anim.color :08x}")
print(f"Default min_brightness: {anim.min_brightness}")
print(f"Default max_brightness: {anim.max_brightness}")
print(f"Default pulse_period: {anim.pulse_period}")

# Test with custom parameters
var blue_pulse = animation.pulse_animation(0xFF0000FF, 50, 200, 2000)
print(f"Blue pulse animation color: 0x{blue_pulse.color :08x}")
print(f"Blue pulse animation min_brightness: {blue_pulse.min_brightness}")
print(f"Blue pulse animation max_brightness: {blue_pulse.max_brightness}")
print(f"Blue pulse animation pulse_period: {blue_pulse.pulse_period}")

# Test from_rgb static method
var red_pulse = animation.pulse_animation.from_rgb(0xFF0000FF, 20, 180, 1500, 1, 255)
print(f"Red pulse animation color: 0x{red_pulse.color :08x}")

# Test parameter setters
blue_pulse.set_min_brightness(30)
blue_pulse.set_max_brightness(220)
blue_pulse.set_pulse_period(1800)
print(f"Updated blue pulse min_brightness: {blue_pulse.min_brightness}")
print(f"Updated blue pulse max_brightness: {blue_pulse.max_brightness}")
print(f"Updated blue pulse pulse_period: {blue_pulse.pulse_period}")

# Test update method
var start_time = tasmota.millis()
blue_pulse.start(start_time)
print(f"Started blue pulse animation at time: {start_time}")

# Test at different points in the cycle
var quarter_cycle = start_time + (blue_pulse.pulse_period / 10)
blue_pulse.update(quarter_cycle)
print(f"Brightness at 1/10 cycle: {blue_pulse.current_brightness}")

var quarter_cycle = start_time + (blue_pulse.pulse_period / 8)
blue_pulse.update(quarter_cycle)
print(f"Brightness at 1/8 cycle: {blue_pulse.current_brightness}")

var quarter_cycle = start_time + (3 * blue_pulse.pulse_period / 10)
blue_pulse.update(quarter_cycle)
print(f"Brightness at 3/10 cycle: {blue_pulse.current_brightness}")

var quarter_cycle = start_time + (blue_pulse.pulse_period / 4)
blue_pulse.update(quarter_cycle)
print(f"Brightness at 1/4 cycle: {blue_pulse.current_brightness}")

var half_cycle = start_time + (blue_pulse.pulse_period / 2)
blue_pulse.update(half_cycle)
print(f"Brightness at 1/2 cycle: {blue_pulse.current_brightness}")

var three_quarter_cycle = start_time + (3 * blue_pulse.pulse_period / 4)
blue_pulse.update(three_quarter_cycle)
print(f"Brightness at 3/4 cycle: {blue_pulse.current_brightness}")

var full_cycle = start_time + blue_pulse.pulse_period
blue_pulse.update(full_cycle)
print(f"Brightness at full cycle: {blue_pulse.current_brightness}")

# Test rendering
var frame = animation.frame_buffer(5)
blue_pulse.render(frame, tasmota.millis())
print(f"First pixel after rendering: 0x{frame.get_pixel_color(0) :08x}")

# Validate key test results
assert(anim != nil, "Default pulse animation should be created")
assert(blue_pulse != nil, "Custom pulse animation should be created")
assert(blue_pulse.color == 0xFF0000FF, "Blue pulse should have correct color")
assert(blue_pulse.min_brightness == 30, "Min brightness should be updated to 30")
assert(blue_pulse.max_brightness == 220, "Max brightness should be updated to 220")
assert(blue_pulse.pulse_period == 1800, "Pulse period should be updated to 1800")
assert(blue_pulse.is_running, "Blue pulse should be running after start")
assert(frame.get_pixel_color(0) != 0x00000000, "First pixel should not be black after rendering")

print("All tests completed successfully!")
return true