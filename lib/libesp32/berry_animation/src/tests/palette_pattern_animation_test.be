# Test for palette pattern animations
#
# This test verifies that the palette pattern animations work correctly with different color providers
# using the new parameterized class specification and engine-controlled timing.

import string
import animation

# Create a mock engine for testing
class MockEngine
  var time_ms
  var strip_length
  
  def init()
    self.time_ms = 1000  # Fixed time for testing
    self.strip_length = 10
  end
  
  def get_strip_length()
    return self.strip_length  # Mock strip length
  end
end

var mock_engine = MockEngine()

# Create a frame buffer for testing
var frame = animation.frame_buffer(10, 1)

# For simple testing, we'll use direct color values
# More complex color providers can be tested separately

# Create a simple mock color source that has get_color_for_value method
class MockColorSource
  def get_color_for_value(value, time_ms)
    # Return red for high values, blue for low values (expecting 0-255 range)
    return value > 127 ? 0xFF0000FF : 0x0000FFFF
  end
end
var mock_color_source = MockColorSource()

# Test 1: PaletteGradientAnimation
print("Test 3: PaletteGradientAnimation")
var gradient_anim = animation.palette_gradient_animation(mock_engine)
gradient_anim.color_source = mock_color_source
gradient_anim.shift_period = 3000  # 3 second shift period
gradient_anim.priority = 10
gradient_anim.duration = 0
gradient_anim.loop = false
gradient_anim.opacity = 255

assert(gradient_anim != nil, "Failed to create gradient animation")
assert(gradient_anim.shift_period == 3000, "Shift period should be 3000")

# Start the animation
# Note: When testing animations directly (not through engine_proxy), we must set start_time manually
gradient_anim.start_time = mock_engine.time_ms  # Set start_time manually for direct testing
gradient_anim.start(mock_engine.time_ms)
gradient_anim.update(mock_engine.time_ms)   # force first tick
assert(gradient_anim.is_running, "Animation should be running")

# Update and render
gradient_anim.update(mock_engine.time_ms)
frame.clear()
result = gradient_anim.render(frame, mock_engine.time_ms, mock_engine.strip_length)
assert(result, "Render should return true")

# Test parameter changes
gradient_anim.shift_period = 1500
assert(gradient_anim.shift_period == 1500, "Shift period should be updated to 1500")

# Test new parameters
gradient_anim.spatial_period = 5
assert(gradient_anim.spatial_period == 5, "Spatial period should be updated to 5")

gradient_anim.phase_shift = 64
assert(gradient_anim.phase_shift == 64, "Phase shift should be updated to 64")

# Test static gradient (shift_period = 0)
gradient_anim.shift_period = 0
assert(gradient_anim.shift_period == 0, "Shift period should be updated to 0 (static)")

# Test 3: Changing color sources dynamically
print("Test 3: Changing color sources dynamically")
var dynamic_anim = animation.palette_gradient_animation(mock_engine)
dynamic_anim.color_source = mock_color_source
dynamic_anim.shift_period = 1000
dynamic_anim.spatial_period = 3

# Start the animation
dynamic_anim.start_time = mock_engine.time_ms  # Set start_time manually for direct testing
dynamic_anim.start(mock_engine.time_ms)
dynamic_anim.update(mock_engine.time_ms)   # force first tick
assert(dynamic_anim.is_running, "Animation should be running")

# Update and render with initial color source
dynamic_anim.update(mock_engine.time_ms)
frame.clear()
result = dynamic_anim.render(frame, mock_engine.time_ms, mock_engine.strip_length)
assert(result, "Render should return true")

# Create another mock color source
class MockColorSource2
  def get_color_for_value(value, time_ms)
    # Return green for all values
    return 0x00FF00FF
  end
end
var mock_color_source2 = MockColorSource2()

# Change to a different color source
dynamic_anim.color_source = mock_color_source2
dynamic_anim.update(mock_engine.time_ms)
frame.clear()
result = dynamic_anim.render(frame, mock_engine.time_ms, mock_engine.strip_length)
assert(result, "Render should return true")

# Test 4: Parameter validation
print("Test 4: Parameter validation")
var validation_anim = animation.palette_gradient_animation(mock_engine)

# Test valid parameter values
validation_anim.shift_period = 500
assert(validation_anim.shift_period == 500, "Valid shift period should be accepted")

validation_anim.spatial_period = 1
assert(validation_anim.spatial_period == 1, "Valid spatial period should be accepted")

validation_anim.phase_shift = 128
assert(validation_anim.phase_shift == 128, "Valid phase shift should be accepted")

# Test 5: Animation with different color mapping
print("Test 5: Animation with different color mapping")
class MockRainbowColorSource
  def get_color_for_value(value, time_ms)
    # Simple rainbow mapping based on value (expecting 0-255 range)
    if value < 85
      return 0xFF0000FF  # Red
    elif value < 170
      return 0x00FF00FF  # Green
    else
      return 0x0000FFFF  # Blue
    end
  end
end
var rainbow_color_source = MockRainbowColorSource()

var rich_anim = animation.palette_gradient_animation(mock_engine)
rich_anim.color_source = rainbow_color_source
rich_anim.shift_period = 1000

# Start the animation
rich_anim.start_time = mock_engine.time_ms  # Set start_time manually for direct testing
rich_anim.start(mock_engine.time_ms)
rich_anim.update(mock_engine.time_ms)   # force first tick
assert(rich_anim.is_running, "Animation should be running")

# Update and render
rich_anim.update(mock_engine.time_ms)
frame.clear()
result = rich_anim.render(frame, mock_engine.time_ms, mock_engine.strip_length)
assert(result, "Render should return true")

# Test 6: Animation timing and synchronization
print("Test 6: Animation timing and synchronization")
var sync_time = mock_engine.time_ms + 1000

# Create multiple animations
var anim1 = animation.palette_gradient_animation(mock_engine)
anim1.color_source = mock_color_source
anim1.shift_period = 1000
anim1.spatial_period = 4

# Start both animations at the same time
anim1.start_time = sync_time  # Set start_time manually for direct testing
anim1.start(sync_time)
anim1.update(sync_time)   # force first tick

assert(anim1.start_time == sync_time, "Animation 1 should have correct start time")

# Test 7: Animation without color source (should handle gracefully)
print("Test 7: Animation without color source")
var no_color_anim = animation.palette_gradient_animation(mock_engine)
no_color_anim.shift_period = 1000
no_color_anim.spatial_period = 3
# Note: no color_source set

no_color_anim.start_time = mock_engine.time_ms  # Set start_time manually for direct testing
no_color_anim.start(mock_engine.time_ms)
no_color_anim.update(mock_engine.time_ms)
frame.clear()
result = no_color_anim.render(frame, mock_engine.time_ms, mock_engine.strip_length)
assert(!result, "Render should return false when no color source is set")

# Test 8: String representation
print("Test 8: String representation")
var str_anim = animation.palette_gradient_animation(mock_engine)
var str_repr = str_anim.tostring()
print(f"String representation: {str_repr}")
assert(str_repr != nil, "String representation should not be nil")
# The string representation might use the base class name, so let's check for that
assert(string.find(str_repr, "Animation") >= 0, "String should contain Animation in class name")

print("All palette pattern animation tests passed!")