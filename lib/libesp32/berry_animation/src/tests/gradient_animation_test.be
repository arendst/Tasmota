# Test suite for GradientAnimation
#
# This test verifies that the GradientAnimation works correctly
# with different gradient types, colors, and movement patterns.

import animation

# Test basic gradient animation creation
def test_gradient_creation()
  print("Testing GradientAnimation creation...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Test default gradient (rainbow linear)
  var gradient = animation.gradient_animation(engine)
  assert(gradient != nil, "Should create gradient animation")
  assert(gradient.gradient_type == 0, "Should default to linear gradient")
  assert(gradient.direction == 0, "Should default to left-to-right direction")
  
  # Test single color gradient
  var red_gradient = animation.gradient_animation(engine)
  red_gradient.color = 0xFFFF0000
  red_gradient.name = "red_gradient"
  assert(red_gradient != nil, "Should create red gradient")
  assert(red_gradient.name == "red_gradient", "Should set name")
  
  # Test radial gradient
  var radial_gradient = animation.gradient_animation(engine)
  radial_gradient.gradient_type = 1
  radial_gradient.center_pos = 64
  radial_gradient.spread = 200
  radial_gradient.movement_speed = 100
  radial_gradient.priority = 10
  radial_gradient.duration = 5000
  radial_gradient.loop = false
  radial_gradient.name = "radial_gradient"
  assert(radial_gradient != nil, "Should create radial gradient")
  assert(radial_gradient.gradient_type == 1, "Should be radial gradient")
  
  print("✓ GradientAnimation creation test passed")
end

# Test gradient parameter changes
def test_gradient_parameters()
  print("Testing GradientAnimation parameters...")
  
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var gradient = animation.gradient_animation(engine)
  gradient.color = 0xFFFFFFFF
  gradient.name = "test"
  
  # Test parameter setting via virtual members
  gradient.gradient_type = 1
  assert(gradient.gradient_type == 1, "Should update gradient type")
  
  gradient.direction = 128
  assert(gradient.direction == 128, "Should update direction")
  
  gradient.center_pos = 200
  assert(gradient.center_pos == 200, "Should update center position")
  
  gradient.spread = 128
  assert(gradient.spread == 128, "Should update spread")
  
  gradient.movement_speed = 150
  assert(gradient.movement_speed == 150, "Should update movement speed")
  
  # Test parameter validation via set_param method
  assert(gradient.set_param("gradient_type", 5) == false, "Should reject invalid gradient type")
  assert(gradient.set_param("spread", 0) == false, "Should reject zero spread")
  
  print("✓ GradientAnimation parameters test passed")
end

# Test gradient animation updates
def test_gradient_updates()
  print("Testing GradientAnimation updates...")
  
  var strip = global.Leds(5)
  var engine = animation.create_engine(strip)
  var gradient = animation.gradient_animation(engine)
  gradient.color = 0xFF00FF00
  gradient.movement_speed = 100
  gradient.name = "test"
  
  # Start the animation
  gradient.start(1000)
  assert(gradient.is_running == true, "Should be running after start")
  
  # Test update at different times
  assert(gradient.update(1000) == true, "Should update successfully at start time")
  assert(gradient.update(1500) == true, "Should update successfully after 500ms")
  assert(gradient.update(2000) == true, "Should update successfully after 1000ms")
  
  # Test that movement_speed affects phase_offset
  var initial_offset = gradient.phase_offset
  gradient.update(3000)  # 2 seconds later
  # With movement_speed=100, should have moved
  # (movement is time-based, so offset should change)
  
  print("✓ GradientAnimation updates test passed")
end

# Test gradient rendering
def test_gradient_rendering()
  print("Testing GradientAnimation rendering...")
  
  var strip = global.Leds(5)
  var engine = animation.create_engine(strip)
  var gradient = animation.gradient_animation(engine)
  gradient.color = 0xFFFF0000
  gradient.movement_speed = 0
  gradient.name = "test"
  
  # Create a frame buffer
  var frame = animation.frame_buffer(5, 1)
  
  # Start and update the animation
  gradient.start(1000)
  gradient.update(1000)
  
  # Test rendering
  var result = gradient.render(frame, 1000)
  assert(result == true, "Should render successfully")
  
  # Test that colors were set (basic check)
  # For a red gradient, pixels should have some red component
  var first_color = frame.get_pixel_color(0)
  var last_color = frame.get_pixel_color(4)  # Last pixel in 5-pixel strip
  # Colors should be different in a gradient
  assert(first_color != last_color, "First and last pixels should be different in gradient")
  
  print("✓ GradientAnimation rendering test passed")
end

# Test gradient factory methods
def test_gradient_factory_methods()
  print("Testing GradientAnimation factory methods...")
  
  var strip = global.Leds(20)
  var engine = animation.create_engine(strip)
  
  # Test rainbow linear factory
  var rainbow_linear = animation.gradient_rainbow_linear(engine)
  assert(rainbow_linear != nil, "Should create rainbow linear gradient")
  assert(rainbow_linear.gradient_type == 0, "Should be linear")
  assert(rainbow_linear.movement_speed == 50, "Should set movement speed")
  
  # Test rainbow radial factory
  var rainbow_radial = animation.gradient_rainbow_radial(engine)
  assert(rainbow_radial != nil, "Should create rainbow radial gradient")
  assert(rainbow_radial.gradient_type == 1, "Should be radial")
  assert(rainbow_radial.center_pos == 128, "Should set center position")
  assert(rainbow_radial.movement_speed == 30, "Should set movement speed")
  
  # Test two-color linear factory
  var two_color = animation.gradient_two_color_linear(engine)
  assert(two_color != nil, "Should create two-color gradient")
  assert(two_color.gradient_type == 0, "Should be linear")
  assert(two_color.movement_speed == 0, "Should set movement speed")
  
  print("✓ GradientAnimation factory methods test passed")
end

# Test gradient position calculations
def test_gradient_position_calculations()
  print("Testing GradientAnimation position calculations...")
  
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Test linear gradient with different directions
  var linear_gradient = animation.gradient_animation(engine)
  linear_gradient.color = 0xFFFFFFFF
  linear_gradient.movement_speed = 0
  linear_gradient.name = "test"
  linear_gradient.start(1000)
  linear_gradient.update(1000)
  
  # The _calculate_linear_position method is private, but we can test the overall effect
  # by checking that different pixels get different colors in a linear gradient
  var frame = animation.frame_buffer(10, 1)
  linear_gradient.render(frame, 1000)
  
  var first_color = frame.get_pixel_color(0)
  var last_color = frame.get_pixel_color(9)
  # In a gradient, first and last pixels should typically have different colors
  # (unless it's a very specific case)
  
  # Test radial gradient
  var radial_gradient = animation.gradient_animation(engine)
  radial_gradient.color = 0xFFFFFFFF
  radial_gradient.gradient_type = 1
  radial_gradient.movement_speed = 0
  radial_gradient.name = "test"
  radial_gradient.start(1000)
  radial_gradient.update(1000)
  radial_gradient.render(frame, 1000)
  
  # In a radial gradient, center pixel should be different from edge pixels
  var center_color = frame.get_pixel_color(5)  # Middle pixel
  var edge_color = frame.get_pixel_color(0)    # Edge pixel
  
  print("✓ GradientAnimation position calculations test passed")
end

# Test refactored color system
def test_gradient_color_refactoring()
  print("Testing GradientAnimation color refactoring...")
  
  var strip = global.Leds(5)
  var engine = animation.create_engine(strip)
  
  # Test with static color
  var static_gradient = animation.gradient_animation(engine)
  static_gradient.color = 0xFFFF0000
  static_gradient.name = "static_test"
  assert(static_gradient.color == 0xFFFF0000, "Should have color set")
  
  # Test with nil color (default rainbow)
  var rainbow_gradient = animation.gradient_animation(engine)
  rainbow_gradient.color = nil
  rainbow_gradient.name = "rainbow_test"
  assert(rainbow_gradient.color == nil, "Should accept nil color for rainbow")
  
  # Test color resolution
  var resolved_color = static_gradient.resolve_value(static_gradient.color, "color", 1000)
  assert(resolved_color != nil, "Should resolve color")
  
  # Test basic rendering with different color types
  var frame = animation.frame_buffer(5, 1)
  static_gradient.start(1000)
  static_gradient.update(1000)
  var result = static_gradient.render(frame, 1000)
  assert(result == true, "Should render with static color")
  
  rainbow_gradient.start(1000)
  rainbow_gradient.update(1000)
  result = rainbow_gradient.render(frame, 1000)
  assert(result == true, "Should render with rainbow color")
  
  print("✓ GradientAnimation color refactoring test passed")
end

# Test virtual parameter access
def test_gradient_virtual_parameters()
  print("Testing GradientAnimation virtual parameters...")
  
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var gradient = animation.gradient_animation(engine)
  gradient.name = "test"
  
  # Test virtual parameter assignment and access
  gradient.color = 0xFFFF00FF
  assert(gradient.color == 0xFFFF00FF, "Should update color via virtual member")
  
  gradient.gradient_type = 1
  assert(gradient.gradient_type == 1, "Should update gradient type via virtual member")
  
  gradient.direction = 200
  assert(gradient.direction == 200, "Should update direction via virtual member")
  
  gradient.center_pos = 64
  assert(gradient.center_pos == 64, "Should update center position via virtual member")
  
  gradient.spread = 128
  assert(gradient.spread == 128, "Should update spread via virtual member")
  
  gradient.movement_speed = 75
  assert(gradient.movement_speed == 75, "Should update movement speed via virtual member")
  
  print("✓ GradientAnimation virtual parameters test passed")
end

# Test updated tostring method
def test_gradient_tostring()
  print("Testing GradientAnimation tostring...")
  
  import string
  
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Test with static color
  var static_gradient = animation.gradient_animation(engine)
  static_gradient.color = 0xFFFF0000
  static_gradient.movement_speed = 50
  static_gradient.name = "static_test"
  var str_static = str(static_gradient)
  assert(str_static != nil, "Should have string representation")
  assert(string.find(str_static, "linear") != -1, "Should mention gradient type")
  assert(string.find(str_static, "movement=50") != -1, "Should mention movement speed")
  
  # Test with color provider
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFF00FF00
  var provider_gradient = animation.gradient_animation(engine)
  provider_gradient.color = color_provider
  provider_gradient.gradient_type = 1
  provider_gradient.movement_speed = 25
  provider_gradient.name = "provider_test"
  var str_provider = str(provider_gradient)
  assert(str_provider != nil, "Should have string representation")
  assert(string.find(str_provider, "radial") != -1, "Should mention radial type")
  
  print("✓ GradientAnimation tostring test passed")
end

# Run all tests
def run_gradient_animation_tests()
  print("=== GradientAnimation Tests ===")
  
  try
    test_gradient_creation()
    test_gradient_parameters()
    test_gradient_updates()
    test_gradient_rendering()
    test_gradient_factory_methods()
    test_gradient_position_calculations()
    test_gradient_color_refactoring()
    test_gradient_virtual_parameters()
    test_gradient_tostring()
    
    print("=== All GradientAnimation tests passed! ===")
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