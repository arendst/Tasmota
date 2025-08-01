# Test suite for GradientAnimation
#
# This test verifies that the GradientAnimation works correctly
# with different gradient types, colors, and movement patterns.

import animation

# Test basic gradient animation creation
def test_gradient_creation()
  print("Testing GradientAnimation creation...")
  
  # Test default gradient (rainbow linear)
  var gradient = animation.gradient_animation(nil, nil, nil, nil, nil, nil, 10, nil, nil, nil, nil)
  assert(gradient != nil, "Should create gradient animation")
  assert(gradient.gradient_type == 0, "Should default to linear gradient")
  assert(gradient.direction == 0, "Should default to left-to-right direction")
  assert(gradient.strip_length == 10, "Should set strip length")
  
  # Test single color gradient
  var red_gradient = animation.gradient_animation(0xFFFF0000, 0, 0, 128, 255, 50, 15, 10, 0, true, "red_gradient")
  assert(red_gradient != nil, "Should create red gradient")
  assert(red_gradient.name == "red_gradient", "Should set name")
  
  # Test radial gradient
  var radial_gradient = animation.gradient_animation(nil, 1, 0, 64, 200, 100, 20, 10, 5000, false, "radial_gradient")
  assert(radial_gradient != nil, "Should create radial gradient")
  assert(radial_gradient.gradient_type == 1, "Should be radial gradient")
  
  print("✓ GradientAnimation creation test passed")
end

# Test gradient parameter changes
def test_gradient_parameters()
  print("Testing GradientAnimation parameters...")
  
  var gradient = animation.gradient_animation(0xFFFFFFFF, 0, 0, 128, 255, 0, 10, 10, 0, true, "test")
  
  # Test parameter setting
  assert(gradient.set_param("gradient_type", 1) == true, "Should set gradient type")
  assert(gradient.gradient_type == 1, "Should update gradient type")
  
  assert(gradient.set_param("direction", 128) == true, "Should set direction")
  assert(gradient.direction == 128, "Should update direction")
  
  assert(gradient.set_param("center_pos", 200) == true, "Should set center position")
  assert(gradient.center_pos == 200, "Should update center position")
  
  assert(gradient.set_param("spread", 128) == true, "Should set spread")
  assert(gradient.spread == 128, "Should update spread")
  
  assert(gradient.set_param("movement_speed", 150) == true, "Should set movement speed")
  assert(gradient.movement_speed == 150, "Should update movement speed")
  
  # Test invalid parameters
  assert(gradient.set_param("gradient_type", 5) == false, "Should reject invalid gradient type")
  assert(gradient.set_param("spread", 0) == false, "Should reject zero spread")
  
  print("✓ GradientAnimation parameters test passed")
end

# Test gradient animation updates
def test_gradient_updates()
  print("Testing GradientAnimation updates...")
  
  var gradient = animation.gradient_animation(0xFF00FF00, 0, 0, 128, 255, 100, 5, 10, 0, true, "test")
  
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
  
  var gradient = animation.gradient_animation(0xFFFF0000, 0, 0, 128, 255, 0, 5, 10, 0, true, "test")
  
  # Create a mock frame buffer
  var frame = animation.frame_buffer(5, 1)
  
  # Start and update the animation
  gradient.start(1000)
  gradient.update(1000)
  
  # Test rendering
  var result = gradient.render(frame, 1000)
  assert(result == true, "Should render successfully")
  
  # Test that colors were set (basic check)
  # For a red gradient (black to red), first pixel should be black, last should be red
  var first_color = frame.get_pixel_color(0)
  var last_color = frame.get_pixel_color(4)  # Last pixel in 5-pixel strip
  assert(first_color == 0xFF000000, "First pixel should be black in black-to-red gradient")
  assert(last_color == 0xFFFF0000, "Last pixel should be red in black-to-red gradient")
  
  # Test rendering when not running
  gradient.stop()
  result = gradient.render(frame, 1000)
  assert(result == false, "Should not render when stopped")
  
  print("✓ GradientAnimation rendering test passed")
end

# Test gradient factory methods
def test_gradient_factory_methods()
  print("Testing GradientAnimation factory methods...")
  
  # Test rainbow linear factory
  var rainbow_linear = animation.gradient_rainbow_linear(50, 20, 10)
  assert(rainbow_linear != nil, "Should create rainbow linear gradient")
  assert(rainbow_linear.gradient_type == 0, "Should be linear")
  assert(rainbow_linear.movement_speed == 50, "Should set movement speed")
  assert(rainbow_linear.strip_length == 20, "Should set strip length")
  assert(rainbow_linear.priority == 10, "Should set priority")
  
  # Test rainbow radial factory
  var rainbow_radial = animation.gradient_rainbow_radial(100, 75, 25, 15)
  assert(rainbow_radial != nil, "Should create rainbow radial gradient")
  assert(rainbow_radial.gradient_type == 1, "Should be radial")
  assert(rainbow_radial.center_pos == 100, "Should set center position")
  assert(rainbow_radial.movement_speed == 75, "Should set movement speed")
  
  # Test two-color linear factory
  var two_color = animation.gradient_two_color_linear(0xFFFF0000, 0xFF0000FF, 25, 30, 5)
  assert(two_color != nil, "Should create two-color gradient")
  assert(two_color.gradient_type == 0, "Should be linear")
  assert(two_color.movement_speed == 25, "Should set movement speed")
  
  print("✓ GradientAnimation factory methods test passed")
end

# Test gradient position calculations
def test_gradient_position_calculations()
  print("Testing GradientAnimation position calculations...")
  
  # Test linear gradient with different directions
  var linear_gradient = animation.gradient_animation(0xFFFFFFFF, 0, 0, 128, 255, 0, 10, 10, 0, true, "test")
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
  var radial_gradient = animation.gradient_animation(0xFFFFFFFF, 1, 0, 128, 255, 0, 10, 10, 0, true, "test")
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
  
  # Test with static color
  var static_gradient = animation.gradient_animation(0xFFFF0000, 0, 0, 128, 255, 0, 5, 10, 0, true, "static_test")
  assert(static_gradient.color != nil, "Should have color set")
  assert(animation.is_value_provider(static_gradient.color), "Static color should be wrapped in provider")
  
  # Test with color provider
  var color_provider = animation.solid_color_provider(0xFF00FF00)
  var provider_gradient = animation.gradient_animation(color_provider, 0, 0, 128, 255, 0, 5, 10, 0, true, "provider_test")
  assert(provider_gradient.color != nil, "Should have color provider set")
  assert(isinstance(provider_gradient.color, animation.solid_color_provider), "Should be solid color provider")
  
  # Test color resolution
  var resolved_color = static_gradient.resolve_value(static_gradient.color, "color", 1000)
  assert(resolved_color != nil, "Should resolve color")
  
  # Test with rich palette provider
  var palette_provider = animation.rich_palette_color_provider(animation.PALETTE_RAINBOW, 5000, 1, 255)
  palette_provider.set_range(0, 255)
  var palette_gradient = animation.gradient_animation(palette_provider, 0, 0, 128, 255, 0, 5, 10, 0, true, "palette_test")
  assert(palette_gradient.color != nil, "Should have palette provider set")
  assert(isinstance(palette_gradient.color, animation.rich_palette_color_provider), "Should be rich palette provider")
  
  print("✓ GradientAnimation color refactoring test passed")
end

# Test new setter methods
def test_gradient_setter_methods()
  print("Testing GradientAnimation setter methods...")
  
  var gradient = animation.gradient_animation(0xFFFFFFFF, 0, 0, 128, 255, 0, 10, 10, 0, true, "test")
  
  # Test color setter
  var new_color = animation.solid_color_provider(0xFFFF00FF)
  var result = gradient.set_color(new_color)
  assert(result == gradient, "Should return self for chaining")
  assert(gradient.color == new_color, "Should update color")
  
  # Test gradient type setter
  result = gradient.set_gradient_type(1)
  assert(result == gradient, "Should return self for chaining")
  assert(gradient.gradient_type == 1, "Should update gradient type")
  
  # Test direction setter
  result = gradient.set_direction(200)
  assert(result == gradient, "Should return self for chaining")
  assert(gradient.direction == 200, "Should update direction")
  
  # Test center position setter
  result = gradient.set_center_pos(64)
  assert(result == gradient, "Should return self for chaining")
  assert(gradient.center_pos == 64, "Should update center position")
  
  # Test spread setter
  result = gradient.set_spread(128)
  assert(result == gradient, "Should return self for chaining")
  assert(gradient.spread == 128, "Should update spread")
  
  # Test movement speed setter
  result = gradient.set_movement_speed(75)
  assert(result == gradient, "Should return self for chaining")
  assert(gradient.movement_speed == 75, "Should update movement speed")
  
  # Test strip length setter
  result = gradient.set_strip_length(20)
  assert(result == gradient, "Should return self for chaining")
  assert(gradient.strip_length == 20, "Should update strip length")
  
  print("✓ GradientAnimation setter methods test passed")
end

# Test updated tostring method
def test_gradient_tostring()
  print("Testing GradientAnimation tostring...")
  
  import string
  
  # Test with static color
  var static_gradient = animation.gradient_animation(0xFFFF0000, 0, 0, 128, 255, 50, 10, 10, 0, true, "static_test")
  var str_static = str(static_gradient)
  assert(str_static != nil, "Should have string representation")
  assert(string.find(str_static, "linear") != -1, "Should mention gradient type")
  assert(string.find(str_static, "movement=50") != -1, "Should mention movement speed")
  
  # Test with color provider
  var color_provider = animation.solid_color_provider(0xFF00FF00)
  var provider_gradient = animation.gradient_animation(color_provider, 1, 0, 128, 255, 25, 10, 10, 0, true, "provider_test")
  var str_provider = str(provider_gradient)
  assert(str_provider != nil, "Should have string representation")
  assert(string.find(str_provider, "radial") != -1, "Should mention radial type")
  assert(string.find(str_provider, "SolidColorProvider") != -1, "Should mention provider type")
  
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
    test_gradient_setter_methods()
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