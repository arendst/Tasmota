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
var engine = animation.animation_engine(strip)

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
assert(anim.is_running == true, "Animation should be running after start")
assert(anim.start_time == 1000, "Animation start time should be 1000")
assert(anim.current_time == 1000, "Animation current time should be 1000")

# Test restart functionality - start() acts as restart
anim.start()
assert(anim.is_running == true, "Animation should be running after start")
var first_start_time = anim.start_time

# Start again - should restart with new time
engine.time_ms = 3000
anim.start()
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
assert(non_loop_anim.is_running == true, "Animation should be running after start")

# Update within duration
engine.time_ms = 2500
var result = non_loop_anim.update(engine.time_ms)
assert(result == true, "Update should return true when animation is still running")
assert(non_loop_anim.is_running == true, "Animation should still be running")
assert(non_loop_anim.current_time == 2500, "Current time should be updated")

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
loop_anim.start(4000)

# Update after one loop
engine.time_ms = 5100
result = loop_anim.update(engine.time_ms)
assert(result == true, "Update should return true for looping animation")
assert(loop_anim.is_running == true, "Looping animation should still be running after duration")
assert(loop_anim.start_time == 5000, "Start time should be adjusted for looping")

# Test get_progress
engine.time_ms = 4500
var non_loop_progress = animation.animation(engine)
non_loop_progress.priority = 1
non_loop_progress.duration = 1000
non_loop_progress.loop = false
non_loop_progress.opacity = 255
non_loop_progress.name = "progress"
non_loop_progress.color = 0xFF0000
non_loop_progress.start(4000)
non_loop_progress.update(engine.time_ms)
assert(non_loop_progress.get_progress() == 128, "Progress should be 128 at midpoint (500ms of 1000ms)")

# Test progress at start (0ms elapsed)
engine.time_ms = 4000
var start_progress = animation.animation(engine)
start_progress.priority = 1
start_progress.duration = 1000
start_progress.loop = false
start_progress.opacity = 255
start_progress.name = "start"
start_progress.color = 0xFF0000
start_progress.start(4000)
start_progress.update(engine.time_ms)
assert(start_progress.get_progress() == 0, "Progress should be 0 at start")

# Test progress at end (1000ms elapsed) - test before update stops the animation
engine.time_ms = 5000
var end_progress = animation.animation(engine)
end_progress.priority = 1
end_progress.duration = 1000
end_progress.loop = false
end_progress.opacity = 255
end_progress.name = "end"
end_progress.color = 0xFF0000
end_progress.start(4000)
end_progress.current_time = 5000  # Set current time manually to avoid stopping
assert(end_progress.get_progress() == 255, "Progress should be 255 at end")

# Test progress at quarter point (250ms elapsed)
engine.time_ms = 4250
var quarter_progress = animation.animation(engine)
quarter_progress.priority = 1
quarter_progress.duration = 1000
quarter_progress.loop = false
quarter_progress.opacity = 255
quarter_progress.name = "quarter"
quarter_progress.color = 0xFF0000
quarter_progress.start(4000)
quarter_progress.update(engine.time_ms)
assert(quarter_progress.get_progress() == 64, "Progress should be 64 at quarter point (250ms of 1000ms)")

# Test looping animation progress (should wrap around)
engine.time_ms = 5500  # 1500ms elapsed = 1.5 loops of 1000ms
var loop_progress = animation.animation(engine)
loop_progress.priority = 1
loop_progress.duration = 1000
loop_progress.loop = true
loop_progress.opacity = 255
loop_progress.name = "loop_progress"
loop_progress.color = 0xFF0000
loop_progress.start(4000)
loop_progress.current_time = 5500  # Set manually to avoid loop adjustment in update()
assert(loop_progress.get_progress() == 128, "Looping animation should wrap around (500ms into second loop)")

# Test infinite animation progress
var infinite_anim = animation.animation(engine)
infinite_anim.priority = 1
infinite_anim.duration = 0  # infinite
infinite_anim.loop = false
infinite_anim.opacity = 255
infinite_anim.name = "infinite"
infinite_anim.color = 0xFF0000
infinite_anim.start(4000)
assert(infinite_anim.get_progress() == 0, "Infinite animation should always return 0 progress")

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

# Test static parameter metadata access
var params_metadata = param_anim.get_params_metadata()
assert(params_metadata.contains("priority"), "Priority parameter should be defined")
assert(params_metadata["priority"]["min"] == 0, "Priority parameter min should be 0")
assert(params_metadata["priority"]["default"] == 10, "Priority parameter default should be 10")

# Test parameter validation and setting (using existing 'priority' parameter)
assert(param_anim.set_param("priority", 75) == true, "Valid parameter should be accepted")
assert(param_anim.get_param("priority", nil) == 75, "Parameter value should be updated")
assert(param_anim.set_param("priority", -1) == false, "Value below min should be rejected")
# Note: Type validation is not defined for priority parameter in PARAMS, so string values are accepted

# Test default values
assert(param_anim.get_param("unknown", "default") == "default", "Unknown parameter should return default")
assert(param_anim.get_param("priority", 0) == 75, "Known parameter should return current value")

# Test parameter metadata
var metadata = param_anim.get_param_metadata("priority")
assert(metadata != nil, "Metadata should exist for static parameter")
assert(metadata["min"] == 0, "Metadata should contain correct min value")

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