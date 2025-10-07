# Test for gradient rainbow functionality with light_state HSV conversion
import animation

print("Testing gradient rainbow with light_state HSV conversion...")

# Create LED strip and engine
var strip = global.Leds(10)
var engine = animation.create_engine(strip)

# Test rainbow gradient (nil color)
var rainbow_gradient = animation.gradient_animation(engine)
rainbow_gradient.color = nil  # Should use rainbow
rainbow_gradient.movement_speed = 0  # Static for testing

# Start and update
rainbow_gradient.start(1000)
rainbow_gradient.update(1000)

# Create frame and render
var frame = animation.frame_buffer(10, 1)
var result = rainbow_gradient.render(frame, 1000)
assert(result == true, "Should render rainbow gradient successfully")

# Check that different pixels have different colors (rainbow effect)
var colors = []
var i = 0
while i < 10
  colors.push(frame.get_pixel_color(i))
  i += 1
end

# Verify that we have some color variation (not all the same)
var first_color = colors[0]
var has_variation = false
i = 1
while i < size(colors)
  if colors[i] != first_color
    has_variation = true
    break
  end
  i += 1
end

assert(has_variation, "Rainbow gradient should have color variation across pixels")

# Test that colors have proper alpha channel (should be 0xFF)
i = 0
while i < size(colors)
  var alpha = (colors[i] >> 24) & 0xFF
  assert(alpha == 0xFF, f"Color at pixel {i} should have full alpha (0xFF), got 0x{alpha:02X}")
  i += 1
end

print("✓ Gradient rainbow with light_state HSV conversion test passed!")

return true