# Test for animation.filled_animation
#
# This test verifies that the animation.filled_animation works correctly with different color providers.

import animation

# Create a frame buffer for testing
var frame = animation.frame_buffer(10, 1)

# Test 1: animation.filled_animation with a solid color
print("Test 1: animation.filled_animation with a solid color")
var solid_anim = animation.filled_animation(0xFF0000FF, 10, 255, 0, true, "solid_test")
assert(solid_anim != nil, "Failed to create solid animation")

# Start the animation
solid_anim.start()
assert(solid_anim.is_running, "Animation should be running")

# Update and render
solid_anim.update(tasmota.millis())
frame.clear()
var result = solid_anim.render(frame, tasmota.millis())
assert(result, "Render should return true")

# Check the color of the first pixel
var pixel_color = frame.get_pixel_color(0)
assert(pixel_color == 0xFF0000FF, f"Expected 0xFF0000FF, got {pixel_color:08X}")

# Test 2: animation.filled_animation with a color cycle provider
print("Test 2: animation.filled_animation with a color cycle provider")
var cycle_provider = animation.color_cycle_color_provider(
  [0xFF0000FF, 0xFF00FF00, 0xFFFF0000],  # RGB colors
  1000,                                  # 1 second cycle period
  0                                      # Linear transition
)
var cycle_anim = animation.filled_animation(cycle_provider, 10, 255, 0, true, "cycle_test")
assert(cycle_anim != nil, "Failed to create cycle animation")

# Start the animation
cycle_anim.start()
assert(cycle_anim.is_running, "Animation should be running")

# Update and render
cycle_anim.update(tasmota.millis())
frame.clear()
result = cycle_anim.render(frame, tasmota.millis())
assert(result, "Render should return true")

# Test 3: animation.filled_animation with a rich palette provider
print("Test 3: animation.filled_animation with a rich palette provider")
var palette_anim = animation.rich_palette_animation(
  animation.PALETTE_RAINBOW,  # Use the rainbow palette
  1000,                       # 1 second cycle period
  0,                          # Linear transition
  255,                        # Full brightness
  10                          # Priority
)
assert(palette_anim != nil, "Failed to create palette animation")

# Start the animation
palette_anim.start()
assert(palette_anim.is_running, "Animation should be running")

# Update and render
palette_anim.update(tasmota.millis())
frame.clear()
result = palette_anim.render(frame, tasmota.millis())
assert(result, "Render should return true")

# Test 4: animation.filled_animation with a composite provider
print("Test 4: animation.filled_animation with a composite provider")
var composite_anim = animation.composite_animation(
  [cycle_provider, animation.rich_palette_color_provider(animation.PALETTE_RAINBOW, 1000, 0, 255)],
  0,                                   # Overlay blend mode
  10                                   # Priority
)
assert(composite_anim != nil, "Failed to create composite animation")

# Start the animation
composite_anim.start()
assert(composite_anim.is_running, "Animation should be running")

# Update and render
composite_anim.update(tasmota.millis())
frame.clear()
result = composite_anim.render(frame, tasmota.millis())
assert(result, "Render should return true")

# Test 5: Changing color provider dynamically
print("Test 5: Changing color provider dynamically")
var dynamic_anim = animation.filled_animation(0xFF0000FF, 10, 255, 0, true, "dynamic_test")
assert(dynamic_anim != nil, "Failed to create dynamic animation")

# Start the animation
dynamic_anim.start()
assert(dynamic_anim.is_running, "Animation should be running")

# Update and render with initial color
dynamic_anim.update(tasmota.millis())
frame.clear()
result = dynamic_anim.render(frame, tasmota.millis())
assert(result, "Render should return true")

# Check the color of the first pixel
pixel_color = frame.get_pixel_color(0)
assert(pixel_color == 0xFF0000FF, f"Expected 0xFF0000FF, got {pixel_color:08X}")

# Change to a different color
dynamic_anim.set_color(0x00FF00FF)  # Green
dynamic_anim.update(tasmota.millis())
frame.clear()
result = dynamic_anim.render(frame, tasmota.millis())
assert(result, "Render should return true")

# Check the color of the first pixel
pixel_color = frame.get_pixel_color(0)
assert(pixel_color == 0x00FF00FF, f"Expected 0x00FF00FF, got {pixel_color:08X}")

# Change to a color provider
dynamic_anim.set_color(cycle_provider)
dynamic_anim.update(tasmota.millis())
frame.clear()
result = dynamic_anim.render(frame, tasmota.millis())
assert(result, "Render should return true")

print("All tests passed!")