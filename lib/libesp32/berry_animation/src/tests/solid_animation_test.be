# Test file for unified solid() function
#
# This file contains tests for the unified animation.solid() function
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/solid_animation_test.be

import animation

print("Imported animation module")

# Create a solid animation with default color (white)
var anim = animation.solid(0xFFFFFFFF, 10, 0, false, 255, "test_solid")
print("Created solid animation with white color")

# Start the animation
anim.start(tasmota.millis())

# Test that it's created successfully
print(f"Animation created: {anim}")
print(f"Animation type: {type(anim)}")

# Test default values
print(f"Default priority: {anim.priority}")
print(f"Default opacity: {anim.opacity}")
print(f"Default duration: {anim.duration}")
print(f"Default loop: {anim.loop}")

# Test with custom parameters - red color
var red_anim = animation.solid(0xFFFF0000)
print(f"Red animation created")

# Test with all parameters
var blue_anim = animation.solid(0xFF0000FF, 20, 5000, true, 200, "test_blue")
print(f"Blue animation - priority: {blue_anim.priority}, duration: {blue_anim.duration}, loop: {blue_anim.loop}")

# Test with solid color provider
var solid_provider = animation.solid_color_provider(0xFF00FF00)  # Green
var green_anim = animation.solid(solid_provider)
print("Green animation with color provider created")

# Test rendering
var frame = animation.frame_buffer(5)
red_anim.start(tasmota.millis())
red_anim.render(frame, tasmota.millis())
print("Rendering test completed")

# Test that animations can be composed
# animation.pulse(color, min_brightness, max_brightness, pulse_period, priority, duration, loop, name)
var pulse_anim = animation.pulse(0xFFFF0000, 0, 255, 1000, 10, 0, false, "test_pulse")
print(f"Pulse animation created: {pulse_anim}")

# Validate key test results
assert(anim != nil, "Solid animation should be created")
assert(anim.is_running, "Solid animation should be running after start")
assert(pulse_anim != nil, "Pulse animation using solid should be created")
assert(type(pulse_anim) == "instance", "Pulse animation should be an instance")
assert(red_anim != nil, "Red animation should be created")
assert(blue_anim != nil, "Blue animation should be created")
assert(green_anim != nil, "Green animation should be created")

print("All unified solid() tests completed successfully!")
return true