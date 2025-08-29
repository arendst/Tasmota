# Test for animation.solid
#
# This test verifies that the animation.solidectly with different color providers
# using the new parameterized class specification and engine-controlled timing.

import animation

# Create a mock engine for testing
class MockEngine
  var time_ms
  
  def init()
    self.time_ms = 1000  # Fixed time for testing
  end
  
  def get_strip_length()
    return 10  # Mock strip length
  end
end

var mock_engine = MockEngine()

# Create a frame buffer for testing
var frame = animation.frame_buffer(10, 1)

# Test 1: animation.solid with a solid color
print("Test 1: animation.solid with a solid color")
var solid_anim = animation.solid(mock_engine)
solid_anim.color = 0xFF0000FF
solid_anim.priority = 10
solid_anim.duration = 0
solid_anim.loop = false  # Use boolean instead of integer
solid_anim.opacity = 255
solid_anim.name = "solid_test"
assert(solid_anim != nil, "Failed to create solid animation")

# Start the animation
solid_anim.start()
assert(solid_anim.is_running, "Animation should be running")

# Update and render
solid_anim.update(mock_engine.time_ms)
frame.clear()
var result = solid_anim.render(frame, mock_engine.time_ms)
assert(result, "Render should return true")

# Check the color of the first pixel
var pixel_color = frame.get_pixel_color(0)
assert(pixel_color == 0xFF0000FF, f"Expected 0xFF0000FF, got {pixel_color:08X}")

# Test 2: animation.solid with a color cycle provider
print("Test 2: animation.solid with a color cycle provider")
var cycle_provider = animation.color_cycle(mock_engine)
cycle_provider.palette = bytes("FF0000FFFF00FF00FFFF0000")  # BGR colors in AARRGGBB format
cycle_provider.cycle_period = 1000  # 1 second cycle period
# Note: transition_type removed - now uses "brutal" color switching

var cycle_anim = animation.solid(mock_engine)
cycle_anim.color = cycle_provider
cycle_anim.priority = 10
cycle_anim.duration = 0
cycle_anim.loop = false  # Use boolean instead of integer
cycle_anim.opacity = 255
cycle_anim.name = "cycle_test"
assert(cycle_anim != nil, "Failed to create cycle animation")

# Start the animation
cycle_anim.start()
assert(cycle_anim.is_running, "Animation should be running")

# Update and render
cycle_anim.update(mock_engine.time_ms)
frame.clear()
result = cycle_anim.render(frame, mock_engine.time_ms)
assert(result, "Render should return true")

# Test 3: animation.solid with a rich palette provider
print("Test 3: animation.solid with a rich palette provider")
var rich_provider = animation.rich_palette(mock_engine)
rich_provider.palette = animation.PALETTE_RAINBOW  # Use the rainbow palette
rich_provider.cycle_period = 1000  # 1 second cycle period
# Note: transition_type removed - rich palette uses smooth transitions
rich_provider.brightness = 255  # Full brightness

var palette_anim = animation.solid(mock_engine)
palette_anim.color = rich_provider
palette_anim.priority = 10
palette_anim.duration = 0
palette_anim.loop = false  # Use boolean instead of integer
palette_anim.opacity = 255
palette_anim.name = "palette_test"
assert(palette_anim != nil, "Failed to create palette animation")

# Start the animation
palette_anim.start()
assert(palette_anim.is_running, "Animation should be running")

# Update and render
palette_anim.update(mock_engine.time_ms)
frame.clear()
result = palette_anim.render(frame, mock_engine.time_ms)
assert(result, "Render should return true")

# Test 4: animation.solid with a composite provider
print("Test 4: animation.solid with a composite provider")
var rich_provider2 = animation.rich_palette(mock_engine)
rich_provider2.palette = animation.PALETTE_RAINBOW
rich_provider2.cycle_period = 1000
# Note: transition_type removed
rich_provider2.brightness = 255

var composite_provider = animation.composite_color(mock_engine)
composite_provider.providers = [cycle_provider, rich_provider2]
composite_provider.blend_mode = 0  # Overlay blend mode

var composite_anim = animation.solid(mock_engine)
composite_anim.color = composite_provider
composite_anim.priority = 10
composite_anim.duration = 0
composite_anim.loop = false  # Use boolean instead of integer
composite_anim.opacity = 255
composite_anim.name = "composite_test"
assert(composite_anim != nil, "Failed to create composite animation")

# Start the animation
composite_anim.start()
assert(composite_anim.is_running, "Animation should be running")

# Update and render
composite_anim.update(mock_engine.time_ms)
frame.clear()
result = composite_anim.render(frame, mock_engine.time_ms)
assert(result, "Render should return true")

# Test 5: Changing color provider dynamically
print("Test 5: Changing color provider dynamically")
var dynamic_anim = animation.solid(mock_engine)
dynamic_anim.color = 0xFF0000FF
dynamic_anim.priority = 10
dynamic_anim.duration = 0
dynamic_anim.loop = false  # Use boolean instead of integer
dynamic_anim.opacity = 255
dynamic_anim.name = "dynamic_test"
assert(dynamic_anim != nil, "Failed to create dynamic animation")

# Start the animation
dynamic_anim.start()
assert(dynamic_anim.is_running, "Animation should be running")

# Update and render with initial color
dynamic_anim.update(mock_engine.time_ms)
frame.clear()
result = dynamic_anim.render(frame, mock_engine.time_ms)
assert(result, "Render should return true")

# Check the color of the first pixel
pixel_color = frame.get_pixel_color(0)
assert(pixel_color == 0xFF0000FF, f"Expected 0xFF0000FF, got {pixel_color:08X}")

# Change to a different color using virtual member assignment
dynamic_anim.color = 0x00FF00FF  # Green
dynamic_anim.update(mock_engine.time_ms)
frame.clear()
result = dynamic_anim.render(frame, mock_engine.time_ms)
assert(result, "Render should return true")

# Check the color of the first pixel
pixel_color = frame.get_pixel_color(0)
assert(pixel_color == 0x00FF00FF, f"Expected 0x00FF00FF, got {pixel_color:08X}")

# Change to a color provider using virtual member assignment
dynamic_anim.color = cycle_provider
dynamic_anim.update(mock_engine.time_ms)
frame.clear()
result = dynamic_anim.render(frame, mock_engine.time_ms)
assert(result, "Render should return true")

print("All tests passed!")