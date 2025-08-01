# Unit tests for the Animation base class
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation -e "import tasmota" lib/libesp32/berry_animation/tests/animation_test.be

import string

# Import the animation module
import animation

# We don't need to mock tasmota.millis(), this version already has tasmota.set_millis() to fix arbitrary time

# Test Animation class
assert(animation.animation != nil, "Animation class should be defined")

# Test initialization
# New signature: (priority, duration, loop, opacity, name)
var anim = animation.animation(20, 5000, true, 255, "test_animation")
assert(anim.is_running == false, "Animation should not be running initially")
assert(anim.priority == 20, "Animation priority should be 20")
assert(anim.duration == 5000, "Animation duration should be 5000ms")
assert(anim.loop != 0, "Animation should be set to loop")
assert(anim.opacity == 255, "Animation opacity should be 255")
assert(anim.name == "test_animation", "Animation name should be 'test_animation'")

# Test default values
var default_anim = animation.animation()
assert(default_anim.priority == 10, "Default priority should be 10")
assert(default_anim.duration == 0, "Default duration should be 0 (infinite)")
assert(default_anim.loop == 0, "Default loop should be false")
assert(default_anim.opacity == 255, "Default opacity should be 255")
assert(default_anim.name == "animation", "Default name should be 'animation'")

# Test start method
tasmota.set_millis(1000)
anim.start()
assert(anim.is_running == true, "Animation should be running after start")
assert(anim.start_time == 1000, "Animation start time should be 1000")
assert(anim.current_time == 1000, "Animation current time should be 1000")

# Test stop method
anim.stop()
assert(anim.is_running == false, "Animation should not be running after stop")

# Test pause and resume
anim.start()
assert(anim.is_running == true, "Animation should be running after start")
anim.pause()
assert(anim.is_running == false, "Animation should not be running after pause")
anim.resume()
assert(anim.is_running == true, "Animation should be running after resume")

# Test update method with non-looping animation
var non_loop_anim = animation.animation(1, 1000, false, 255, "non_loop")
tasmota.set_millis(2000)
non_loop_anim.start(2000)
assert(non_loop_anim.is_running == true, "Animation should be running after start")

# Update within duration
tasmota.set_millis(2500)
var result = non_loop_anim.update(tasmota.millis())
assert(result == true, "Update should return true when animation is still running")
assert(non_loop_anim.is_running == true, "Animation should still be running")
assert(non_loop_anim.current_time == 2500, "Current time should be updated")

# Update after duration
tasmota.set_millis(3100)
result = non_loop_anim.update(tasmota.millis())
assert(result == false, "Update should return false when animation is complete")
assert(non_loop_anim.is_running == false, "Animation should stop after duration")

# Test update method with looping animation
var loop_anim = animation.animation(1, 1000, true, 255, "loop")
tasmota.set_millis(4000)
loop_anim.start(4000)

# Update after one loop
tasmota.set_millis(5100)
result = loop_anim.update(tasmota.millis())
assert(result == true, "Update should return true for looping animation")
assert(loop_anim.is_running == true, "Looping animation should still be running after duration")
assert(loop_anim.start_time == 5000, "Start time should be adjusted for looping")

# Test get_progress
tasmota.set_millis(4500)
var non_loop_progress = animation.animation(1, 1000, false, 255, "progress")
non_loop_progress.start(4000)
non_loop_progress.update(tasmota.millis())
assert(non_loop_progress.get_progress() == 128, "Progress should be 128 at midpoint (500ms of 1000ms)")

# Test progress at start (0ms elapsed)
tasmota.set_millis(4000)
var start_progress = animation.animation(1, 1000, false, 255, "start")
start_progress.start(4000)
start_progress.update(tasmota.millis())
assert(start_progress.get_progress() == 0, "Progress should be 0 at start")

# Test progress at end (1000ms elapsed) - test before update stops the animation
tasmota.set_millis(5000)
var end_progress = animation.animation(1, 1000, false, 255, "end")
end_progress.start(4000)
end_progress.current_time = 5000  # Set current time manually to avoid stopping
assert(end_progress.get_progress() == 255, "Progress should be 255 at end")

# Test progress at quarter point (250ms elapsed)
tasmota.set_millis(4250)
var quarter_progress = animation.animation(1, 1000, false, 255, "quarter")
quarter_progress.start(4000)
quarter_progress.update(tasmota.millis())
assert(quarter_progress.get_progress() == 64, "Progress should be 64 at quarter point (250ms of 1000ms)")

# Test looping animation progress (should wrap around)
tasmota.set_millis(5500)  # 1500ms elapsed = 1.5 loops of 1000ms
var loop_progress = animation.animation(1, 1000, true, 255, "loop_progress")
loop_progress.start(4000)
loop_progress.current_time = 5500  # Set manually to avoid loop adjustment in update()
assert(loop_progress.get_progress() == 128, "Looping animation should wrap around (500ms into second loop)")

# Test infinite animation progress
var infinite_anim = animation.animation(1, 0, false, 255, "infinite")
infinite_anim.start(4000)
assert(infinite_anim.get_progress() == 0, "Infinite animation should always return 0 progress")

# Test setter methods
var setter_anim = animation.animation()
setter_anim.set_priority(20)
assert(setter_anim.priority == 20, "Priority should be updated")
setter_anim.set_duration(2000)
assert(setter_anim.duration == 2000, "Duration should be updated")
setter_anim.set_loop(true)
assert(setter_anim.loop != 0, "Loop should be updated")

# Test parameter handling
var param_anim = animation.animation()

# Test parameter registration
param_anim.register_param("speed", {"min": 1, "max": 100, "default": 50})
assert(param_anim.params.contains("speed"), "Parameter should be registered")
assert(param_anim.params["speed"]["min"] == 1, "Parameter min should be 1")
assert(param_anim.params["speed"]["max"] == 100, "Parameter max should be 100")
assert(param_anim.params["speed"]["default"] == 50, "Parameter default should be 50")

# Test parameter validation and setting
assert(param_anim.set_param("speed", 75) == true, "Valid parameter should be accepted")
assert(param_anim.get_param("speed", nil) == 75, "Parameter value should be updated")
assert(param_anim.set_param("speed", 0) == false, "Value below min should be rejected")
assert(param_anim.set_param("speed", 101) == false, "Value above max should be rejected")
assert(param_anim.set_param("speed", "invalid") == false, "Invalid type should be rejected")

# Test default values
assert(param_anim.get_param("unknown", "default") == "default", "Unknown parameter should return default")
assert(param_anim.get_param("speed", 0) == 75, "Known parameter should return current value")

# Test parameter metadata
var metadata = param_anim.get_param_metadata("speed")
assert(metadata != nil, "Metadata should exist for registered parameter")
assert(metadata["min"] == 1, "Metadata should contain correct min value")

# Test updating multiple parameters
param_anim.register_param("color", {"default": 0xFFFF0000})
param_anim.register_param("intensity", {"min": 0, "max": 10, "default": 5})

# Test individual parameter updates (update_params method removed)
var speed_result = param_anim.set_param("speed", 60)
var color_result = param_anim.set_param("color", 0xFF00FF00)
var intensity_result = param_anim.set_param("intensity", 8)
assert(speed_result == true, "Speed parameter update should succeed")
assert(color_result == true, "Color parameter update should succeed")
assert(intensity_result == true, "Intensity parameter update should succeed")
assert(param_anim.get_param("speed", nil) == 60, "Speed parameter should be updated")
assert(param_anim.get_param("color", nil) == 0xFF00FF00, "Color parameter should be updated")
assert(param_anim.get_param("intensity", nil) == 8, "Intensity parameter should be updated")

# Test parameter validation with invalid values
var invalid_speed_result = param_anim.set_param("speed", 60)  # Valid
var invalid_color_result = param_anim.set_param("color", 0xFF0000FF)  # Valid
var invalid_intensity_result = param_anim.set_param("intensity", 15)  # Invalid: above max
assert(invalid_speed_result == true, "Valid speed parameter should succeed")
assert(invalid_color_result == true, "Valid color parameter should succeed")
assert(invalid_intensity_result == false, "Invalid intensity parameter should fail")
assert(param_anim.get_param("intensity", nil) == 8, "Invalid parameters should not change existing value")

# Test render method (base implementation should do nothing)
# Create a frame buffer for testing
var frame = animation.frame_buffer(10)
result = setter_anim.render(frame, tasmota.millis())
assert(result == false, "Base render method should return false")

# Test tostring method
var anim_str = str(anim)
assert(string.find(anim_str, "Animation") >= 0, "String representation should contain 'Animation'")
assert(string.find(anim_str, anim.name) >= 0, "String representation should contain the animation name")

print("All Animation tests passed!")