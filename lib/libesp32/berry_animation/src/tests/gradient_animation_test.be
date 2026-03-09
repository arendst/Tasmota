# Test suite for gradient
#
# This test verifies that the simplified gradient works correctly
# with linear and radial gradients using beacon-based rendering.

import animation

# Test basic gradient animation creation
def test_gradient_creation()
  print("Testing gradient creation...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Test default gradient
  var gradient = animation.gradient(engine)
  assert(gradient != nil, "Should create gradient animation")
  assert(gradient.gradient_type == 0, "Should default to linear gradient")
  assert(gradient.direction == 0, "Should default to forward direction")
  assert(gradient.color1 == 0xFFFF0000, "Should default to red color1")
  assert(gradient.color2 == 0xFF0000FF, "Should default to blue color2")
  
  # Test radial gradient
  var radial_gradient = animation.gradient(engine)
  radial_gradient.gradient_type = 1
  radial_gradient.color1 = 0xFF000000
  radial_gradient.color2 = 0xFFFFFFFF
  assert(radial_gradient != nil, "Should create radial gradient")
  assert(radial_gradient.gradient_type == 1, "Should be radial gradient")
  
  print("✓ gradient creation test passed")
end

# Test gradient parameter changes
def test_gradient_parameters()
  print("Testing gradient parameters...")
  
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var gradient = animation.gradient(engine)
  
  # Test parameter setting via virtual members
  gradient.gradient_type = 1
  assert(gradient.gradient_type == 1, "Should update gradient type")
  
  gradient.direction = 1
  assert(gradient.direction == 1, "Should update direction")
  
  gradient.color1 = 0xFF0000FF
  assert(gradient.color1 == 0xFF0000FF, "Should update color1")
  
  gradient.color2 = 0xFFFF0000
  assert(gradient.color2 == 0xFFFF0000, "Should update color2")
  
  # Test parameter validation via set_param method
  assert(gradient.set_param("gradient_type", 5) == false, "Should reject invalid gradient type")
  assert(gradient.set_param("direction", 5) == false, "Should reject invalid direction")
  
  print("✓ gradient parameters test passed")
end

# Test gradient animation updates
def test_gradient_updates()
  print("Testing gradient updates...")
  
  var strip = global.Leds(5)
  var engine = animation.create_engine(strip)
  var gradient = animation.gradient(engine)
  gradient.color1 = 0xFF000000
  gradient.color2 = 0xFF00FF00
  
  # Start the animation
  gradient.start_time = 1000
  gradient.start(1000)
  assert(gradient.is_running == true, "Should be running after start")
  
  # Test update at different times
  gradient.update(1000)
  assert(gradient.is_running == true, "Should be running after update")
  gradient.update(1500)
  assert(gradient.is_running == true, "Should be running after update at 500ms")
  
  print("✓ gradient updates test passed")
end

# Test gradient rendering
def test_gradient_rendering()
  print("Testing gradient rendering...")
  
  var strip = global.Leds(5)
  var engine = animation.create_engine(strip)
  var gradient = animation.gradient(engine)
  gradient.color1 = 0xFF000000  # Black
  gradient.color2 = 0xFFFF0000  # Red
  
  # Create a frame buffer
  var frame = animation.frame_buffer(5)
  
  # Start and update the animation
  gradient.start_time = 1000
  gradient.start(1000)
  gradient.update(1000)
  
  # Test rendering
  var result = gradient.render(frame, 1000, engine.strip_length)
  assert(result == true, "Should render successfully")
  
  # Test that colors were set (basic check)
  var first_color = frame.get_pixel_color(0)
  var last_color = frame.get_pixel_color(4)
  # Colors should be different in a gradient (black to red)
  assert(first_color != last_color, "First and last pixels should be different in gradient")
  
  print("✓ gradient rendering test passed")
end

# Test linear gradient direction
def test_gradient_direction()
  print("Testing gradient direction...")
  
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Test forward direction (color1 -> color2)
  var forward_gradient = animation.gradient(engine)
  forward_gradient.color1 = 0xFF000000  # Black
  forward_gradient.color2 = 0xFFFF0000  # Red
  forward_gradient.direction = 0
  forward_gradient.start_time = 1000
  forward_gradient.start(1000)
  forward_gradient.update(1000)
  
  var frame1 = animation.frame_buffer(10)
  forward_gradient.render(frame1, 1000, engine.strip_length)
  var forward_first = frame1.get_pixel_color(0)
  var forward_last = frame1.get_pixel_color(9)
  
  # Test reverse direction (color2 -> color1)
  var reverse_gradient = animation.gradient(engine)
  reverse_gradient.color1 = 0xFF000000  # Black
  reverse_gradient.color2 = 0xFFFF0000  # Red
  reverse_gradient.direction = 1
  reverse_gradient.start_time = 1000
  reverse_gradient.start(1000)
  reverse_gradient.update(1000)
  
  var frame2 = animation.frame_buffer(10)
  reverse_gradient.render(frame2, 1000, engine.strip_length)
  var reverse_first = frame2.get_pixel_color(0)
  var reverse_last = frame2.get_pixel_color(9)
  
  # Forward: first should be darker (black), last should be brighter (red)
  # Reverse: first should be brighter (red), last should be darker (black)
  print(f"  Forward: first=0x{forward_first:08X}, last=0x{forward_last:08X}")
  print(f"  Reverse: first=0x{reverse_first:08X}, last=0x{reverse_last:08X}")
  
  print("✓ gradient direction test passed")
end

# Test radial gradient
def test_radial_gradient()
  print("Testing gradient radial mode...")
  
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var radial_gradient = animation.gradient(engine)
  radial_gradient.gradient_type = 1  # Radial
  radial_gradient.color1 = 0xFF000000  # Black at center
  radial_gradient.color2 = 0xFFFFFFFF  # White at edges
  radial_gradient.start_time = 1000
  radial_gradient.start(1000)
  radial_gradient.update(1000)
  
  var frame = animation.frame_buffer(10)
  radial_gradient.render(frame, 1000, engine.strip_length)
  
  # In radial gradient with direction=0, color1 at center, color2 at edges
  var edge_color = frame.get_pixel_color(0)
  var center_color = frame.get_pixel_color(5)
  
  print(f"  Edge (0): 0x{edge_color:08X}")
  print(f"  Center (5): 0x{center_color:08X}")
  
  # Edge should be brighter than center (white vs black)
  var edge_brightness = ((edge_color >> 16) & 0xFF) + ((edge_color >> 8) & 0xFF) + (edge_color & 0xFF)
  var center_brightness = ((center_color >> 16) & 0xFF) + ((center_color >> 8) & 0xFF) + (center_color & 0xFF)
  assert(edge_brightness > center_brightness, "Edge should be brighter than center in radial gradient")
  
  print("✓ gradient radial mode test passed")
end

# Run all tests
def run_gradient_animation_tests()
  print("=== gradient Tests ===")
  
  try
    test_gradient_creation()
    test_gradient_parameters()
    test_gradient_updates()
    test_gradient_rendering()
    test_gradient_direction()
    test_radial_gradient()
    
    print("=== All gradient tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_gradient_animation_tests = run_gradient_animation_tests

run_gradient_animation_tests()

return run_gradient_animation_tests
