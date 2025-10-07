# Unit tests for the Animation base class
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/animation_test.be

import string

# Import the animation module
import animation

# We control time through the animation engine's time_ms property for testing

# Create animation engine for testing
var strip = global.Leds()
var engine = animation.create_engine(strip)

# Test Animation class
assert(animation.animation != nil, "Animation class should be defined")

# Test initialization
# New signature: (engine) - all other parameters set via virtual members
var anim = animation.animation(engine)
anim.priority = 20
anim.duration = 5000
anim.loop = true  # Use boolean for loop parameter
anim.opacity = 255
anim.name = "test_animation"
anim.color = 0xFF0000
assert(anim.is_running == false, "Animation should not be running initially")
assert(anim.priority == 20, "Animation priority should be 20")
assert(anim.duration == 5000, "Animation duration should be 5000ms")
assert(anim.loop == true, "Animation should be set to loop")
assert(anim.opacity == 255, "Animation opacity should be 255")
assert(anim.name == "test_animation", "Animation name should be 'test_animation'")
assert(anim.color == 0xFF0000, "Animation color should be red")

# Test default values
var default_anim = animation.animation(engine)
assert(default_anim.priority == 10, "Default priority should be 10")
assert(default_anim.duration == 0, "Default duration should be 0 (infinite)")
assert(default_anim.loop == false, "Default loop should be false")
assert(default_anim.opacity == 255, "Default opacity should be 255")
assert(default_anim.name == "animation", "Default name should be 'animation'")
assert(default_anim.color == 0xFFFFFFFF, "Default color should be white")

# Test start method
engine.time_ms = 1000
anim.start()
anim.update()
assert(anim.is_running == true, "Animation should be running after start")
assert(anim.start_time == 1000, "Animation start time should be 1000")

# Test restart functionality - start() acts as restart
engine.time_ms = 2000
anim.start()
assert(anim.is_running == true, "Animation should be running after start")
assert(anim.start_time == 2000, "Animation start time should be 2000")
var first_start_time = anim.start_time

# Start again - should restart with new time
engine.time_ms = 3000
anim.start(engine.time_ms)
assert(anim.is_running == true, "Animation should still be running after restart")
assert(anim.start_time == 3000, "Animation should have new start time after restart")

# Test update method with non-looping animation
var non_loop_anim = animation.animation(engine)
non_loop_anim.priority = 1
non_loop_anim.duration = 1000
non_loop_anim.loop = false
non_loop_anim.opacity = 255
non_loop_anim.name = "non_loop"
non_loop_anim.color = 0xFF0000
engine.time_ms = 2000
non_loop_anim.start(2000)
non_loop_anim.update(2000)
assert(non_loop_anim.is_running == true, "Animation should be running after start")

# Update within duration
engine.time_ms = 2500
var result = non_loop_anim.update(engine.time_ms)
assert(result == true, "Update should return true when animation is still running")
assert(non_loop_anim.is_running == true, "Animation should still be running")

# Update after duration
engine.time_ms = 3100
result = non_loop_anim.update(engine.time_ms)
assert(result == false, "Update should return false when animation is complete")
assert(non_loop_anim.is_running == false, "Animation should stop after duration")

# Test update method with looping animation
var loop_anim = animation.animation(engine)
loop_anim.priority = 1
loop_anim.duration = 1000
loop_anim.loop = true
loop_anim.opacity = 255
loop_anim.name = "loop"
loop_anim.color = 0xFF0000
engine.time_ms = 4000
loop_anim.start(engine.time_ms)
loop_anim.update(engine.time_ms)    # update must be explictly called to start time

# Update after one loop
engine.time_ms = 5100
result = loop_anim.update(engine.time_ms)
assert(result == true, "Update should return true for looping animation")
assert(loop_anim.is_running == true, "Looping animation should still be running after duration")
assert(loop_anim.start_time == 5000, "Start time should be adjusted for looping")

# Test direct parameter assignment (no setter methods needed)
var setter_anim = animation.animation(engine)
setter_anim.priority = 20
assert(setter_anim.priority == 20, "Priority should be updated")
setter_anim.duration = 2000
assert(setter_anim.duration == 2000, "Duration should be updated")
setter_anim.loop = true
assert(setter_anim.loop == true, "Loop should be updated")

# Test parameter handling with static parameters
var param_anim = animation.animation(engine)

# Test parameter validation and setting (using existing 'priority' parameter)
assert(param_anim.set_param("priority", 75) == true, "Valid parameter should be accepted")
assert(param_anim.get_param("priority", nil) == 75, "Parameter value should be updated")
assert(param_anim.set_param("priority", -1) == false, "Value below min should be rejected")
# Note: Type validation is not defined for priority parameter in PARAMS, so string values are accepted

# Test default values
assert(param_anim.get_param("unknown", "default") == "default", "Unknown parameter should return default")
assert(param_anim.get_param("priority", 0) == 75, "Known parameter should return current value")

# Test parameter definition using _has_param and _get_param_def
assert(param_anim._has_param("priority") == true, "Should have priority parameter")
var param_def = param_anim._get_param_def("priority")
assert(param_def != nil, "Parameter definition should exist for static parameter")
# Use static methods to access encoded constraint data
assert(param_anim.constraint_mask(param_def, "min") == 0x01, "Parameter definition should have min constraint")
assert(param_anim.constraint_find(param_def, "min", nil) == 0, "Parameter definition should contain correct min value")

# Test updating multiple static parameters
# Test individual parameter updates using existing static parameters
var priority_result = param_anim.set_param("priority", 60)
var color_result = param_anim.set_param("color", 0xFF00FF00)
var opacity_result = param_anim.set_param("opacity", 128)
assert(priority_result == true, "Priority parameter update should succeed")
assert(color_result == true, "Color parameter update should succeed")
assert(opacity_result == true, "Opacity parameter update should succeed")
assert(param_anim.get_param("priority", nil) == 60, "Priority parameter should be updated")
assert(param_anim.get_param("color", nil) == 0xFF00FF00, "Color parameter should be updated")
assert(param_anim.get_param("opacity", nil) == 128, "Opacity parameter should be updated")

# Test parameter validation with invalid values
var valid_priority_result = param_anim.set_param("priority", 50)  # Valid
var valid_color_result = param_anim.set_param("color", 0xFF0000FF)  # Valid
var invalid_opacity_result = param_anim.set_param("opacity", 300)  # Invalid: above max (255)
assert(valid_priority_result == true, "Valid priority parameter should succeed")
assert(valid_color_result == true, "Valid color parameter should succeed")

# Test render method (base implementation should do nothing)
# Create a frame buffer for testing
var frame = animation.frame_buffer(10)
result = setter_anim.render(frame, engine.time_ms)
assert(result == false, "Base render method should return false")

# Test tostring method
var anim_str = str(anim)
assert(string.find(anim_str, "Animation") >= 0, "String representation should contain 'Animation'")
assert(string.find(anim_str, anim.name) >= 0, "String representation should contain the animation name")

print("All Animation tests passed!")