# Test suite for JitterAnimation
#
# This test verifies that the JitterAnimation works correctly
# with different jitter types and parameters.

import animation
import string

# Test basic JitterAnimation creation and functionality
def test_jitter_animation_basic()
  print("Testing basic JitterAnimation...")
  
  # Create a simple source animation
  var source = animation.filled_animation(0xFFFF0000, 10, 0, true, "test_source")
  
  # Test with default parameters
  var jitter_anim = animation.jitter_animation(source, nil, nil, nil, nil, nil, nil, 10, 10, 0, true, "test_jitter")
  
  assert(jitter_anim != nil, "JitterAnimation should be created")
  assert(jitter_anim.jitter_intensity == 100, "Default jitter_intensity should be 100")
  assert(jitter_anim.jitter_frequency == 60, "Default jitter_frequency should be 60")
  assert(jitter_anim.jitter_type == 0, "Default jitter_type should be 0")
  assert(jitter_anim.position_range == 50, "Default position_range should be 50")
  assert(jitter_anim.color_range == 30, "Default color_range should be 30")
  assert(jitter_anim.brightness_range == 40, "Default brightness_range should be 40")
  assert(jitter_anim.strip_length == 10, "Strip length should be 10")
  assert(jitter_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic JitterAnimation test passed")
end

# Test JitterAnimation with custom parameters
def test_jitter_animation_custom()
  print("Testing JitterAnimation with custom parameters...")
  
  var source = animation.filled_animation(0xFF00FF00, 10, 0, true, "test_source")
  
  # Test with custom parameters
  var jitter_anim = animation.jitter_animation(source, 150, 120, 2, 80, 60, 70, 20, 15, 5000, false, "custom_jitter")
  
  assert(jitter_anim.jitter_intensity == 150, "Custom jitter_intensity should be 150")
  assert(jitter_anim.jitter_frequency == 120, "Custom jitter_frequency should be 120")
  assert(jitter_anim.jitter_type == 2, "Custom jitter_type should be 2")
  assert(jitter_anim.position_range == 80, "Custom position_range should be 80")
  assert(jitter_anim.color_range == 60, "Custom color_range should be 60")
  assert(jitter_anim.brightness_range == 70, "Custom brightness_range should be 70")
  assert(jitter_anim.strip_length == 20, "Custom strip length should be 20")
  assert(jitter_anim.priority == 15, "Custom priority should be 15")
  assert(jitter_anim.duration == 5000, "Custom duration should be 5000")
  assert(jitter_anim.loop == 0, "Custom loop should be 0 (false)")
  
  print("✓ Custom JitterAnimation test passed")
end

# Test JitterAnimation parameter changes
def test_jitter_animation_parameters()
  print("Testing JitterAnimation parameter changes...")
  
  var source = animation.filled_animation(0xFF0000FF, 10, 0, true, "test_source")
  var jitter_anim = animation.jitter_animation(source, nil, nil, nil, nil, nil, nil, 15, 10, 0, true, "param_test")
  
  # Test parameter changes
  jitter_anim.set_param("jitter_intensity", 180)
  assert(jitter_anim.jitter_intensity == 180, "Jitter intensity should be updated to 180")
  
  jitter_anim.set_param("jitter_frequency", 100)
  assert(jitter_anim.jitter_frequency == 100, "Jitter frequency should be updated to 100")
  
  jitter_anim.set_param("jitter_type", 3)
  assert(jitter_anim.jitter_type == 3, "Jitter type should be updated to 3")
  
  jitter_anim.set_param("position_range", 80)
  assert(jitter_anim.position_range == 80, "Position range should be updated to 80")
  
  jitter_anim.set_param("color_range", 50)
  assert(jitter_anim.color_range == 50, "Color range should be updated to 50")
  
  jitter_anim.set_param("brightness_range", 60)
  assert(jitter_anim.brightness_range == 60, "Brightness range should be updated to 60")
  
  jitter_anim.set_param("strip_length", 25)
  assert(jitter_anim.strip_length == 25, "Strip length should be updated to 25")
  assert(jitter_anim.current_colors.size() == 25, "Current colors array should be resized")
  assert(jitter_anim.jitter_offsets.size() == 25, "Jitter offsets array should be resized")
  
  print("✓ JitterAnimation parameter test passed")
end

# Test JitterAnimation jitter types
def test_jitter_animation_types()
  print("Testing JitterAnimation jitter types...")
  
  var source = animation.filled_animation(0xFFFFFF00, 10, 0, true, "test_source")
  
  # Test position jitter (type 0)
  var position_jitter = animation.jitter_animation(source, 100, 60, 0, 50, 30, 40, 10, 10, 0, true, "position_test")
  assert(position_jitter.jitter_type == 0, "Position jitter should have type 0")
  
  # Test color jitter (type 1)
  var color_jitter = animation.jitter_animation(source, 100, 60, 1, 50, 30, 40, 10, 10, 0, true, "color_test")
  assert(color_jitter.jitter_type == 1, "Color jitter should have type 1")
  
  # Test brightness jitter (type 2)
  var brightness_jitter = animation.jitter_animation(source, 100, 60, 2, 50, 30, 40, 10, 10, 0, true, "brightness_test")
  assert(brightness_jitter.jitter_type == 2, "Brightness jitter should have type 2")
  
  # Test all jitter (type 3)
  var all_jitter = animation.jitter_animation(source, 100, 60, 3, 50, 30, 40, 10, 10, 0, true, "all_test")
  assert(all_jitter.jitter_type == 3, "All jitter should have type 3")
  
  print("✓ JitterAnimation types test passed")
end

# Test JitterAnimation update and render
def test_jitter_animation_update_render()
  print("Testing JitterAnimation update and render...")
  
  var source = animation.filled_animation(0xFFFF00FF, 10, 0, true, "test_source")
  var jitter_anim = animation.jitter_animation(source, 100, 60, 0, 50, 30, 40, 10, 10, 0, true, "update_test")
  var frame = animation.frame_buffer(10)
  
  # Start animation
  jitter_anim.start(1000)
  assert(jitter_anim.is_running == true, "Animation should be running after start")
  
  # Test update
  var result = jitter_anim.update(1500)
  assert(result == true, "Update should return true for running animation")
  
  # Test render
  result = jitter_anim.render(frame, 1500)
  assert(result == true, "Render should return true for running animation")
  
  # Check that jitter offsets were initialized
  assert(jitter_anim.jitter_offsets.size() == 10, "Jitter offsets should be initialized")
  var i = 0
  while i < jitter_anim.jitter_offsets.size()
    assert(type(jitter_anim.jitter_offsets[i]) == "int", "Jitter offset should be integer")
    i += 1
  end
  
  print("✓ JitterAnimation update/render test passed")
end

# Test JitterAnimation random generation
def test_jitter_animation_random()
  print("Testing JitterAnimation random generation...")
  
  var source = animation.filled_animation(0xFF00FFFF, 10, 0, true, "test_source")
  var jitter_anim = animation.jitter_animation(source, 100, 60, 0, 50, 30, 40, 10, 10, 0, true, "random_test")
  
  # Test random number generation
  var random1 = jitter_anim._random()
  var random2 = jitter_anim._random()
  assert(type(random1) == "int", "Random should return integer")
  assert(type(random2) == "int", "Random should return integer")
  assert(random1 != random2, "Sequential random calls should return different values")
  
  # Test random range
  var range_val = jitter_anim._random_range(10)
  assert(type(range_val) == "int", "Random range should return integer")
  assert(range_val >= -10 && range_val <= 10, "Random range should be within bounds")
  
  print("✓ JitterAnimation random generation test passed")
end

# Test global constructor functions
def test_jitter_constructors()
  print("Testing jitter constructor functions...")
  
  var source = animation.filled_animation(0xFFAAAAAA, 10, 0, true, "test_source")
  
  # Test jitter_position
  var position_jitter = animation.jitter_position(source, 120, 80, 15, 12)
  assert(position_jitter != nil, "jitter_position should create animation")
  assert(position_jitter.jitter_intensity == 120, "Position jitter should have correct intensity")
  assert(position_jitter.jitter_frequency == 80, "Position jitter should have correct frequency")
  assert(position_jitter.jitter_type == 0, "Position jitter should have type 0")
  assert(position_jitter.strip_length == 15, "Position jitter should have correct strip length")
  assert(position_jitter.priority == 12, "Position jitter should have correct priority")
  
  # Test jitter_color
  var color_jitter = animation.jitter_color(source, 100, 60, 20, 8)
  assert(color_jitter != nil, "jitter_color should create animation")
  assert(color_jitter.jitter_intensity == 100, "Color jitter should have correct intensity")
  assert(color_jitter.jitter_type == 1, "Color jitter should have type 1")
  
  # Test jitter_brightness
  var brightness_jitter = animation.jitter_brightness(source, 80, 40, 25, 15)
  assert(brightness_jitter != nil, "jitter_brightness should create animation")
  assert(brightness_jitter.jitter_intensity == 80, "Brightness jitter should have correct intensity")
  assert(brightness_jitter.jitter_type == 2, "Brightness jitter should have type 2")
  
  # Test jitter_all
  var all_jitter = animation.jitter_all(source, 150, 100, 30, 10)
  assert(all_jitter != nil, "jitter_all should create animation")
  assert(all_jitter.jitter_intensity == 150, "All jitter should have correct intensity")
  assert(all_jitter.jitter_type == 3, "All jitter should have type 3")
  
  print("✓ Jitter constructor functions test passed")
end

# Test JitterAnimation color jitter effects
def test_jitter_animation_color_effects()
  print("Testing JitterAnimation color effects...")
  
  var source = animation.filled_animation(0xFF808080, 10, 0, true, "test_source")
  var jitter_anim = animation.jitter_animation(source, 100, 60, 1, 50, 50, 40, 10, 10, 0, true, "color_test")
  
  # Test color jitter application
  var original_color = 0xFF808080  # Gray color
  var jittered_color = jitter_anim._apply_color_jitter(original_color, 0)
  
  assert(type(jittered_color) == "int", "Jittered color should be integer")
  # Color should be different due to jitter (though we can't predict exact value)
  # Just verify it's a valid color value
  assert((jittered_color >> 24) & 0xFF == 0xFF, "Alpha should be preserved")
  
  print("✓ JitterAnimation color effects test passed")
end

# Test JitterAnimation string representation
def test_jitter_tostring()
  print("Testing JitterAnimation string representation...")
  
  var source = animation.filled_animation(0xFF666666, 10, 0, true, "test_source")
  var jitter_anim = animation.jitter_animation(source, 100, 60, 2, 50, 30, 40, 12, 10, 0, true, "string_test")
  var str_repr = str(jitter_anim)
  
  assert(type(str_repr) == "string", "String representation should be a string")
  assert(string.find(str_repr, "JitterAnimation") >= 0, "String should contain 'JitterAnimation'")
  assert(string.find(str_repr, "brightness") >= 0, "String should contain type name")
  assert(string.find(str_repr, "100") >= 0, "String should contain intensity value")
  assert(string.find(str_repr, "60") >= 0, "String should contain frequency value")
  
  print("✓ JitterAnimation string representation test passed")
end

# Run all tests
def run_jitter_animation_tests()
  print("=== JitterAnimation Tests ===")
  
  try
    test_jitter_animation_basic()
    test_jitter_animation_custom()
    test_jitter_animation_parameters()
    test_jitter_animation_types()
    test_jitter_animation_update_render()
    test_jitter_animation_random()
    test_jitter_constructors()
    test_jitter_animation_color_effects()
    test_jitter_tostring()
    
    print("=== All JitterAnimation tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_jitter_animation_tests = run_jitter_animation_tests

run_jitter_animation_tests()

return run_jitter_animation_tests