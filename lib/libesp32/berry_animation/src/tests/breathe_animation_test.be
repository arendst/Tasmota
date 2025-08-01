# Test file for Breathe animation effect
#
# This file contains tests for the BreatheAnimation class
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/breathe_animation_test.be

print("Testing BreatheAnimation...")

# First import the core animation module
import animation
print("Imported animation module")

# Create a breathe animation with default parameters
var anim = animation.breathe_animation()
print("Created breathe animation with defaults")

# Test default values
print(f"Default color: 0x{anim.color :08x}")
print(f"Default min_brightness: {anim.min_brightness}")
print(f"Default max_brightness: {anim.max_brightness}")
print(f"Default breathe_period: {anim.breathe_period}")
print(f"Default curve_factor: {anim.curve_factor}")

# Test with custom parameters
var blue_breathe = animation.breathe_animation(0xFF0000FF, 20, 200, 4000, 3)
print(f"Blue breathe animation color: 0x{blue_breathe.color :08x}")
print(f"Blue breathe animation min_brightness: {blue_breathe.min_brightness}")
print(f"Blue breathe animation max_brightness: {blue_breathe.max_brightness}")
print(f"Blue breathe animation breathe_period: {blue_breathe.breathe_period}")
print(f"Blue breathe animation curve_factor: {blue_breathe.curve_factor}")

# Test from_rgb static method
var red_breathe = animation.breathe_animation.from_rgb(0xFFFF0000, 10, 180, 3000, 2, 1, 255)
print(f"Red breathe animation color: 0x{red_breathe.color :08x}")

# Test parameter setters
blue_breathe.set_min_brightness(30)
blue_breathe.set_max_brightness(220)
blue_breathe.set_breathe_period(3500)
blue_breathe.set_curve_factor(4)
print(f"Updated blue breathe min_brightness: {blue_breathe.min_brightness}")
print(f"Updated blue breathe max_brightness: {blue_breathe.max_brightness}")
print(f"Updated blue breathe breathe_period: {blue_breathe.breathe_period}")
print(f"Updated blue breathe curve_factor: {blue_breathe.curve_factor}")

# Test update method
var start_time = tasmota.millis()
blue_breathe.start(start_time)
print(f"Started blue breathe animation at time: {start_time}")

# Test at different points in the cycle
var quarter_cycle = start_time + (blue_breathe.breathe_period / 10)
blue_breathe.update(quarter_cycle)
print(f"Brightness at 1/10 cycle: {blue_breathe.current_brightness}")

var quarter_cycle = start_time + (blue_breathe.breathe_period / 8)
blue_breathe.update(quarter_cycle)
print(f"Brightness at 1/8 cycle: {blue_breathe.current_brightness}")

var quarter_cycle = start_time + (3 * blue_breathe.breathe_period / 10)
blue_breathe.update(quarter_cycle)
print(f"Brightness at 3/10 cycle: {blue_breathe.current_brightness}")

var quarter_cycle = start_time + (blue_breathe.breathe_period / 4)
blue_breathe.update(quarter_cycle)
print(f"Brightness at 1/4 cycle: {blue_breathe.current_brightness}")

var half_cycle = start_time + (blue_breathe.breathe_period / 2)
blue_breathe.update(half_cycle)
print(f"Brightness at 1/2 cycle: {blue_breathe.current_brightness}")

var three_quarter_cycle = start_time + (3 * blue_breathe.breathe_period / 4)
blue_breathe.update(three_quarter_cycle)
print(f"Brightness at 3/4 cycle: {blue_breathe.current_brightness}")

var full_cycle = start_time + blue_breathe.breathe_period
blue_breathe.update(full_cycle)
print(f"Brightness at full cycle: {blue_breathe.current_brightness}")

# Test rendering
var frame = animation.frame_buffer(5)
blue_breathe.render(frame, tasmota.millis())
print(f"First pixel after rendering: 0x{frame.get_pixel_color(0) :08x}")

# Validate key test results
assert(anim != nil, "Default breathe animation should be created")
assert(blue_breathe != nil, "Custom breathe animation should be created")
assert(blue_breathe.color == 0xFF0000FF, "Blue breathe should have correct color")
assert(blue_breathe.min_brightness == 30, "Min brightness should be updated to 30")
assert(blue_breathe.max_brightness == 220, "Max brightness should be updated to 220")
assert(blue_breathe.breathe_period == 3500, "Breathe period should be updated to 3500")
assert(blue_breathe.curve_factor == 4, "Curve factor should be updated to 4")
assert(blue_breathe.is_running, "Blue breathe should be running after start")
assert(frame.get_pixel_color(0) != 0x00000000, "First pixel should not be black after rendering")

print("All tests completed successfully!")
return true