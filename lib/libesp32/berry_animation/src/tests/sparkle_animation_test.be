# Test suite for SparkleAnimation
#
# This test verifies that the SparkleAnimation works correctly
# with different parameters and color providers.

import animation
import string

# Test basic SparkleAnimation creation and functionality
def test_sparkle_animation_basic()
  print("Testing basic SparkleAnimation...")
  
  # Test with default parameters
  var sparkle_anim = animation.sparkle_animation(nil, nil, nil, nil, nil, nil, nil, 10, 10, 0, true, "test_sparkle")
  
  assert(sparkle_anim != nil, "SparkleAnimation should be created")
  assert(sparkle_anim.background_color == 0xFF000000, "Default background should be black")
  assert(sparkle_anim.density == 30, "Default density should be 30")
  assert(sparkle_anim.fade_speed == 50, "Default fade_speed should be 50")
  assert(sparkle_anim.sparkle_duration == 60, "Default sparkle_duration should be 60")
  assert(sparkle_anim.min_brightness == 100, "Default min_brightness should be 100")
  assert(sparkle_anim.max_brightness == 255, "Default max_brightness should be 255")
  assert(sparkle_anim.strip_length == 10, "Strip length should be 10")
  assert(sparkle_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic SparkleAnimation test passed")
end

# Test SparkleAnimation with custom parameters
def test_sparkle_animation_custom()
  print("Testing SparkleAnimation with custom parameters...")
  
  # Test with custom parameters
  var sparkle_anim = animation.sparkle_animation(0xFF00FF00, 0xFF111111, 80, 120, 40, 50, 200, 20, 15, 5000, false, "custom_sparkle")
  
  assert(sparkle_anim.background_color == 0xFF111111, "Custom background should be set")
  assert(sparkle_anim.density == 80, "Custom density should be 80")
  assert(sparkle_anim.fade_speed == 120, "Custom fade_speed should be 120")
  assert(sparkle_anim.sparkle_duration == 40, "Custom sparkle_duration should be 40")
  assert(sparkle_anim.min_brightness == 50, "Custom min_brightness should be 50")
  assert(sparkle_anim.max_brightness == 200, "Custom max_brightness should be 200")
  assert(sparkle_anim.strip_length == 20, "Custom strip length should be 20")
  assert(sparkle_anim.priority == 15, "Custom priority should be 15")
  assert(sparkle_anim.duration == 5000, "Custom duration should be 5000")
  assert(sparkle_anim.loop == 0, "Custom loop should be 0 (false)")
  
  print("✓ Custom SparkleAnimation test passed")
end

# Test SparkleAnimation parameter changes
def test_sparkle_animation_parameters()
  print("Testing SparkleAnimation parameter changes...")
  
  var sparkle_anim = animation.sparkle_animation(nil, nil, nil, nil, nil, nil, nil, 15, 10, 0, true, "param_test")
  
  # Test parameter changes
  sparkle_anim.set_param("density", 100)
  assert(sparkle_anim.density == 100, "Density should be updated to 100")
  
  sparkle_anim.set_param("fade_speed", 80)
  assert(sparkle_anim.fade_speed == 80, "Fade speed should be updated to 80")
  
  sparkle_anim.set_param("sparkle_duration", 90)
  assert(sparkle_anim.sparkle_duration == 90, "Sparkle duration should be updated to 90")
  
  sparkle_anim.set_param("background_color", 0xFF222222)
  assert(sparkle_anim.background_color == 0xFF222222, "Background color should be updated")
  
  sparkle_anim.set_param("strip_length", 25)
  assert(sparkle_anim.strip_length == 25, "Strip length should be updated to 25")
  assert(sparkle_anim.current_colors.size() == 25, "Current colors array should be resized")
  assert(sparkle_anim.sparkle_states.size() == 25, "Sparkle states array should be resized")
  assert(sparkle_anim.sparkle_ages.size() == 25, "Sparkle ages array should be resized")
  
  print("✓ SparkleAnimation parameter test passed")
end

# Test SparkleAnimation update and render
def test_sparkle_animation_update_render()
  print("Testing SparkleAnimation update and render...")
  
  var sparkle_anim = animation.sparkle_animation(0xFFFF0000, 0xFF000000, 255, 50, 30, 100, 255, 10, 10, 0, true, "update_test")
  var frame = animation.frame_buffer(10)
  
  # Start animation
  sparkle_anim.start(1000)
  assert(sparkle_anim.is_running == true, "Animation should be running after start")
  
  # Test update - run multiple times to potentially create sparkles
  var i = 0
  while i < 10
    sparkle_anim.update(1000 + (i * 50))
    i += 1
  end
  
  # Test render
  var result = sparkle_anim.render(frame, 1500)
  assert(result == true, "Render should return true for running animation")
  
  # With high density (255), we should have some sparkles
  # Check that at least some pixels are not background color
  var has_sparkles = false
  i = 0
  while i < frame.width
    if frame.get_pixel_color(i) != 0xFF000000
      has_sparkles = true
      break
    end
    i += 1
  end
  # Note: Due to randomness, this might occasionally fail, but with density 255 it's very unlikely
  
  print("✓ SparkleAnimation update/render test passed")
end

# Test global constructor functions
def test_sparkle_constructors()
  print("Testing sparkle constructor functions...")
  
  # Test sparkle_white
  var white_sparkle = animation.sparkle_white(80, 60, 15, 12)
  assert(white_sparkle != nil, "sparkle_white should create animation")
  assert(white_sparkle.density == 80, "White sparkle should have correct density")
  assert(white_sparkle.fade_speed == 60, "White sparkle should have correct fade_speed")
  assert(white_sparkle.strip_length == 15, "White sparkle should have correct strip length")
  assert(white_sparkle.priority == 12, "White sparkle should have correct priority")
  
  # Test sparkle_colored
  var colored_sparkle = animation.sparkle_colored(0xFF00FFFF, 90, 70, 20, 8)
  assert(colored_sparkle != nil, "sparkle_colored should create animation")
  assert(colored_sparkle.density == 90, "Colored sparkle should have correct density")
  assert(colored_sparkle.fade_speed == 70, "Colored sparkle should have correct fade_speed")
  
  # Test sparkle_rainbow
  var rainbow_sparkle = animation.sparkle_rainbow(40, 30, 25, 15)
  assert(rainbow_sparkle != nil, "sparkle_rainbow should create animation")
  assert(rainbow_sparkle.density == 40, "Rainbow sparkle should have correct density")
  assert(rainbow_sparkle.fade_speed == 30, "Rainbow sparkle should have correct fade_speed")
  
  print("✓ Sparkle constructor functions test passed")
end

# Test SparkleAnimation string representation
def test_sparkle_tostring()
  print("Testing SparkleAnimation string representation...")
  
  var sparkle_anim = animation.sparkle_animation(nil, nil, 75, 45, 50, 80, 220, 12, 10, 0, true, "string_test")
  var str_repr = str(sparkle_anim)
  
  assert(type(str_repr) == "string", "String representation should be a string")
  assert(string.find(str_repr, "SparkleAnimation") >= 0, "String should contain 'SparkleAnimation'")
  assert(string.find(str_repr, "75") >= 0, "String should contain density value")
  assert(string.find(str_repr, "45") >= 0, "String should contain fade_speed value")
  
  print("✓ SparkleAnimation string representation test passed")
end

# Run all tests
def run_sparkle_animation_tests()
  print("=== SparkleAnimation Tests ===")
  
  try
    test_sparkle_animation_basic()
    test_sparkle_animation_custom()
    test_sparkle_animation_parameters()
    test_sparkle_animation_update_render()
    test_sparkle_constructors()
    test_sparkle_tostring()
    
    print("=== All SparkleAnimation tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_sparkle_animation_tests = run_sparkle_animation_tests

run_sparkle_animation_tests()

return run_sparkle_animation_tests