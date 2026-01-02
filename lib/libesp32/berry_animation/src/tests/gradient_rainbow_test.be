# Test for gradient color variation
import animation

print("Testing gradient color variation...")

# Create LED strip and engine
var strip = global.Leds(10)
var engine = animation.create_engine(strip)

# Test linear gradient with two colors
var gradient = animation.gradient_animation(engine)
gradient.color1 = 0xFF0000FF  # Blue
gradient.color2 = 0xFFFF0000  # Red
gradient.gradient_type = 0  # Linear
gradient.direction = 0  # Forward (blue to red)

# Start and update
gradient.start(1000)
gradient.update(1000)

# Create frame and render
var frame = animation.frame_buffer(10)
var result = gradient.render(frame, 1000, engine.strip_length)
assert(result == true, "Should render gradient successfully")

# Check that different pixels have different colors (gradient effect)
var colors = []
var i = 0
while i < 10
  colors.push(frame.get_pixel_color(i))
  i += 1
end

# Verify that we have some color variation (not all the same)
var first_color = colors[0]
var last_color = colors[9]
var has_variation = first_color != last_color

assert(has_variation, "Gradient should have color variation across pixels")

# Test that colors have proper alpha channel (should be 0xFF)
i = 0
while i < size(colors)
  var alpha = (colors[i] >> 24) & 0xFF
  assert(alpha == 0xFF, f"Color at pixel {i} should have full alpha (0xFF), got 0x{alpha:02X}")
  i += 1
end

# Print colors for debugging
print(f"  First pixel: 0x{first_color:08X}")
print(f"  Last pixel: 0x{last_color:08X}")

print("✓ Gradient color variation test passed!")

return true
