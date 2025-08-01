# Test suite for ScaleAnimation
#
# This test verifies that the ScaleAnimation works correctly
# with different scale modes and parameters.

import animation
import string

# Test basic ScaleAnimation creation and functionality
def test_scale_animation_basic()
  print("Testing basic ScaleAnimation...")
  
  # Create a simple source animation
  var source = animation.filled_animation(0xFFFF0000, 10, 0, true, "test_source")
  
  # Test with default parameters
  var scale_anim = animation.scale_animation(source, nil, nil, nil, nil, nil, 10, 10, 0, true, "test_scale")
  
  assert(scale_anim != nil, "ScaleAnimation should be created")
  assert(scale_anim.scale_factor == 128, "Default scale_factor should be 128")
  assert(scale_anim.scale_speed == 0, "Default scale_speed should be 0")
  assert(scale_anim.scale_mode == 0, "Default scale_mode should be 0")
  assert(scale_anim.scale_center == 128, "Default scale_center should be 128")
  assert(scale_anim.interpolation == 1, "Default interpolation should be 1")
  assert(scale_anim.strip_length == 10, "Strip length should be 10")
  assert(scale_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic ScaleAnimation test passed")
end

# Test ScaleAnimation with custom parameters
def test_scale_animation_custom()
  print("Testing ScaleAnimation with custom parameters...")
  
  var source = animation.filled_animation(0xFF00FF00, 10, 0, true, "test_source")
  
  # Test with custom parameters
  var scale_anim = animation.scale_animation(source, 200, 80, 1, 100, 0, 20, 15, 5000, false, "custom_scale")
  
  assert(scale_anim.scale_factor == 200, "Custom scale_factor should be 200")
  assert(scale_anim.scale_speed == 80, "Custom scale_speed should be 80")
  assert(scale_anim.scale_mode == 1, "Custom scale_mode should be 1")
  assert(scale_anim.scale_center == 100, "Custom scale_center should be 100")
  assert(scale_anim.interpolation == 0, "Custom interpolation should be 0")
  assert(scale_anim.strip_length == 20, "Custom strip length should be 20")
  assert(scale_anim.priority == 15, "Custom priority should be 15")
  assert(scale_anim.duration == 5000, "Custom duration should be 5000")
  assert(scale_anim.loop == 0, "Custom loop should be 0 (false)")
  
  print("✓ Custom ScaleAnimation test passed")
end

# Test ScaleAnimation parameter changes
def test_scale_animation_parameters()
  print("Testing ScaleAnimation parameter changes...")
  
  var source = animation.filled_animation(0xFF0000FF, 10, 0, true, "test_source")
  var scale_anim = animation.scale_animation(source, nil, nil, nil, nil, nil, 15, 10, 0, true, "param_test")
  
  # Test parameter changes
  scale_anim.set_param("scale_factor", 180)
  assert(scale_anim.scale_factor == 180, "Scale factor should be updated to 180")
  
  scale_anim.set_param("scale_speed", 100)
  assert(scale_anim.scale_speed == 100, "Scale speed should be updated to 100")
  
  scale_anim.set_param("scale_mode", 2)
  assert(scale_anim.scale_mode == 2, "Scale mode should be updated to 2")
  
  scale_anim.set_param("scale_center", 200)
  assert(scale_anim.scale_center == 200, "Scale center should be updated to 200")
  
  scale_anim.set_param("interpolation", 0)
  assert(scale_anim.interpolation == 0, "Interpolation should be updated to 0")
  
  scale_anim.set_param("strip_length", 25)
  assert(scale_anim.strip_length == 25, "Strip length should be updated to 25")
  assert(scale_anim.current_colors.size() == 25, "Current colors array should be resized")
  
  print("✓ ScaleAnimation parameter test passed")
end

# Test ScaleAnimation scale modes
def test_scale_animation_modes()
  print("Testing ScaleAnimation scale modes...")
  
  var source = animation.filled_animation(0xFFFFFF00, 10, 0, true, "test_source")
  
  # Test static mode (0)
  var static_scale = animation.scale_animation(source, 150, 0, 0, 128, 1, 10, 10, 0, true, "static_test")
  assert(static_scale.scale_mode == 0, "Static scale should have mode 0")
  var static_factor = static_scale._get_current_scale_factor()
  assert(static_factor == 150, "Static mode should return set scale factor")
  
  # Test oscillate mode (1)
  var oscillate_scale = animation.scale_animation(source, 128, 60, 1, 128, 1, 10, 10, 0, true, "oscillate_test")
  assert(oscillate_scale.scale_mode == 1, "Oscillate scale should have mode 1")
  # For oscillate mode, the factor will vary based on phase
  var oscillate_factor = oscillate_scale._get_current_scale_factor()
  assert(type(oscillate_factor) == "int", "Oscillate mode should return integer factor")
  
  # Test grow mode (2)
  var grow_scale = animation.scale_animation(source, 128, 60, 2, 128, 1, 10, 10, 0, true, "grow_test")
  assert(grow_scale.scale_mode == 2, "Grow scale should have mode 2")
  var grow_factor = grow_scale._get_current_scale_factor()
  assert(type(grow_factor) == "int", "Grow mode should return integer factor")
  
  # Test shrink mode (3)
  var shrink_scale = animation.scale_animation(source, 128, 60, 3, 128, 1, 10, 10, 0, true, "shrink_test")
  assert(shrink_scale.scale_mode == 3, "Shrink scale should have mode 3")
  var shrink_factor = shrink_scale._get_current_scale_factor()
  assert(type(shrink_factor) == "int", "Shrink mode should return integer factor")
  
  print("✓ ScaleAnimation modes test passed")
end

# Test ScaleAnimation interpolation
def test_scale_animation_interpolation()
  print("Testing ScaleAnimation interpolation...")
  
  var source = animation.filled_animation(0xFF808080, 10, 0, true, "test_source")
  var scale_anim = animation.scale_animation(source, 128, 0, 0, 128, 1, 10, 10, 0, true, "interp_test")
  
  # Test color interpolation
  var color1 = 0xFF800000  # Dark red
  var color2 = 0xFFFF0000  # Bright red
  var interpolated = scale_anim._interpolate_colors(color1, color2, 128)  # 50% blend
  
  assert(type(interpolated) == "int", "Interpolated color should be integer")
  # Should be somewhere between the two colors
  var interp_red = (interpolated >> 16) & 0xFF
  var color1_red = (color1 >> 16) & 0xFF
  var color2_red = (color2 >> 16) & 0xFF
  assert(interp_red > color1_red && interp_red < color2_red, "Interpolated red should be between input colors")
  
  print("✓ ScaleAnimation interpolation test passed")
end

# Test ScaleAnimation sine approximation
def test_scale_animation_sine()
  print("Testing ScaleAnimation sine approximation...")
  
  var source = animation.filled_animation(0xFF000000, 10, 0, true, "test_source")
  var scale_anim = animation.scale_animation(source, 128, 0, 0, 128, 1, 10, 10, 0, true, "sine_test")
  
  # Test sine function at key points
  var sine_0 = scale_anim._sine(0)
  var sine_64 = scale_anim._sine(64)   # Quarter wave
  var sine_128 = scale_anim._sine(128) # Half wave
  var sine_192 = scale_anim._sine(192) # Three quarter wave
  
  assert(type(sine_0) == "int", "Sine should return integer")
  assert(type(sine_64) == "int", "Sine should return integer")
  assert(type(sine_128) == "int", "Sine should return integer")
  assert(type(sine_192) == "int", "Sine should return integer")
  
  # Basic sine wave properties (approximate)
  assert(sine_0 < sine_64, "Sine should increase in first quarter")
  assert(sine_64 > sine_128, "Sine should decrease in second quarter")
  assert(sine_128 > sine_192, "Sine should continue decreasing in third quarter")
  
  print("✓ ScaleAnimation sine test passed")
end

# Test ScaleAnimation update and render
def test_scale_animation_update_render()
  print("Testing ScaleAnimation update and render...")
  
  var source = animation.filled_animation(0xFFFF00FF, 10, 0, true, "test_source")
  var scale_anim = animation.scale_animation(source, 150, 0, 0, 128, 1, 10, 10, 0, true, "update_test")
  var frame = animation.frame_buffer(10)
  
  # Start animation
  scale_anim.start(1000)
  assert(scale_anim.is_running == true, "Animation should be running after start")
  
  # Test update
  var result = scale_anim.update(1500)
  assert(result == true, "Update should return true for running animation")
  
  # Test render
  result = scale_anim.render(frame, 1500)
  assert(result == true, "Render should return true for running animation")
  
  # Check that colors were calculated
  assert(scale_anim.current_colors.size() == 10, "Current colors should be initialized")
  var i = 0
  while i < scale_anim.current_colors.size()
    assert(type(scale_anim.current_colors[i]) == "int", "Color should be integer")
    i += 1
  end
  
  print("✓ ScaleAnimation update/render test passed")
end

# Test global constructor functions
def test_scale_constructors()
  print("Testing scale constructor functions...")
  
  var source = animation.filled_animation(0xFF00FFFF, 10, 0, true, "test_source")
  
  # Test scale_static
  var static_scale = animation.scale_static(source, 200, 15, 12)
  assert(static_scale != nil, "scale_static should create animation")
  assert(static_scale.scale_factor == 200, "Static scale should have correct factor")
  assert(static_scale.scale_speed == 0, "Static scale should have no animation")
  assert(static_scale.scale_mode == 0, "Static scale should have mode 0")
  assert(static_scale.strip_length == 15, "Static scale should have correct strip length")
  assert(static_scale.priority == 12, "Static scale should have correct priority")
  
  # Test scale_oscillate
  var oscillate_scale = animation.scale_oscillate(source, 100, 20, 8)
  assert(oscillate_scale != nil, "scale_oscillate should create animation")
  assert(oscillate_scale.scale_speed == 100, "Oscillate scale should have correct speed")
  assert(oscillate_scale.scale_mode == 1, "Oscillate scale should have mode 1")
  assert(oscillate_scale.interpolation == 1, "Oscillate scale should use linear interpolation")
  
  # Test scale_grow
  var grow_scale = animation.scale_grow(source, 80, 25, 15)
  assert(grow_scale != nil, "scale_grow should create animation")
  assert(grow_scale.scale_speed == 80, "Grow scale should have correct speed")
  assert(grow_scale.scale_mode == 2, "Grow scale should have mode 2")
  
  print("✓ Scale constructor functions test passed")
end

# Test ScaleAnimation string representation
def test_scale_tostring()
  print("Testing ScaleAnimation string representation...")
  
  var source = animation.filled_animation(0xFF444444, 10, 0, true, "test_source")
  var scale_anim = animation.scale_animation(source, 150, 80, 1, 100, 1, 12, 10, 0, true, "string_test")
  var str_repr = str(scale_anim)
  
  assert(type(str_repr) == "string", "String representation should be a string")
  assert(string.find(str_repr, "ScaleAnimation") >= 0, "String should contain 'ScaleAnimation'")
  assert(string.find(str_repr, "oscillate") >= 0, "String should contain mode name")
  assert(string.find(str_repr, "150") >= 0, "String should contain factor value")
  assert(string.find(str_repr, "80") >= 0, "String should contain speed value")
  
  print("✓ ScaleAnimation string representation test passed")
end

# Run all tests
def run_scale_animation_tests()
  print("=== ScaleAnimation Tests ===")
  
  try
    test_scale_animation_basic()
    test_scale_animation_custom()
    test_scale_animation_parameters()
    test_scale_animation_modes()
    test_scale_animation_interpolation()
    test_scale_animation_sine()
    test_scale_animation_update_render()
    test_scale_constructors()
    test_scale_tostring()
    
    print("=== All ScaleAnimation tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_scale_animation_tests = run_scale_animation_tests

run_scale_animation_tests()

return run_scale_animation_tests