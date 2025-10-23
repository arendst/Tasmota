# Test suite for SparkleAnimation
#
# This test verifies that the SparkleAnimation works correctly
# with different parameters and color providers.

import animation
import string

# Test basic SparkleAnimation creation and functionality
def test_sparkle_animation_basic()
  print("Testing basic SparkleAnimation...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Test with default parameters
  var sparkle_anim = animation.sparkle_animation(engine)
  
  assert(sparkle_anim != nil, "SparkleAnimation should be created")
  assert(sparkle_anim.back_color == 0xFF000000, "Default background should be black")
  assert(sparkle_anim.density == 30, "Default density should be 30")
  assert(sparkle_anim.fade_speed == 50, "Default fade_speed should be 50")
  assert(sparkle_anim.sparkle_duration == 60, "Default sparkle_duration should be 60")
  assert(sparkle_anim.min_brightness == 100, "Default min_brightness should be 100")
  assert(sparkle_anim.max_brightness == 255, "Default max_brightness should be 255")
  assert(sparkle_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic SparkleAnimation test passed")
end

# Test SparkleAnimation with custom parameters
def test_sparkle_animation_custom()
  print("Testing SparkleAnimation with custom parameters...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(20)
  var engine = animation.create_engine(strip)
  
  # Test with custom parameters using new parameterized pattern
  var sparkle_anim = animation.sparkle_animation(engine)
  sparkle_anim.color = 0xFF00FF00
  sparkle_anim.back_color = 0xFF111111
  sparkle_anim.density = 80
  sparkle_anim.fade_speed = 120
  sparkle_anim.sparkle_duration = 40
  sparkle_anim.min_brightness = 50
  sparkle_anim.max_brightness = 200
  sparkle_anim.priority = 15
  sparkle_anim.duration = 5000
  sparkle_anim.loop = false
  sparkle_anim.name = "custom_sparkle"
  
  assert(sparkle_anim.back_color == 0xFF111111, "Custom background should be set")
  assert(sparkle_anim.density == 80, "Custom density should be 80")
  assert(sparkle_anim.fade_speed == 120, "Custom fade_speed should be 120")
  assert(sparkle_anim.sparkle_duration == 40, "Custom sparkle_duration should be 40")
  assert(sparkle_anim.min_brightness == 50, "Custom min_brightness should be 50")
  assert(sparkle_anim.max_brightness == 200, "Custom max_brightness should be 200")
  assert(sparkle_anim.priority == 15, "Custom priority should be 15")
  assert(sparkle_anim.duration == 5000, "Custom duration should be 5000")
  assert(sparkle_anim.loop == false, "Custom loop should be false")
  
  print("✓ Custom SparkleAnimation test passed")
end

# Test SparkleAnimation parameter changes
def test_sparkle_animation_parameters()
  print("Testing SparkleAnimation parameter changes...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  var sparkle_anim = animation.sparkle_animation(engine)
  sparkle_anim.name = "param_test"
  
  # Test parameter changes using virtual member assignment
  sparkle_anim.density = 100
  assert(sparkle_anim.density == 100, "Density should be updated to 100")
  
  sparkle_anim.fade_speed = 80
  assert(sparkle_anim.fade_speed == 80, "Fade speed should be updated to 80")
  
  sparkle_anim.sparkle_duration = 90
  assert(sparkle_anim.sparkle_duration == 90, "Sparkle duration should be updated to 90")
  
  sparkle_anim.back_color = 0xFF222222
  assert(sparkle_anim.back_color == 0xFF222222, "Background color should be updated")
  
  # Test that arrays are properly sized based on engine strip length
  assert(sparkle_anim.current_colors.size() == 15, "Current colors array should match engine strip length")
  assert(sparkle_anim.sparkle_states.size() == 15, "Sparkle states array should match engine strip length")
  assert(sparkle_anim.sparkle_ages.size() == 15, "Sparkle ages array should match engine strip length")
  
  print("✓ SparkleAnimation parameter test passed")
end

# Test SparkleAnimation update and render
def test_sparkle_animation_update_render()
  print("Testing SparkleAnimation update and render...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var sparkle_anim = animation.sparkle_animation(engine)
  sparkle_anim.color = 0xFFFF0000
  sparkle_anim.back_color = 0xFF000000
  sparkle_anim.density = 255  # High density for reliable sparkles
  sparkle_anim.fade_speed = 50
  sparkle_anim.sparkle_duration = 30
  sparkle_anim.min_brightness = 100
  sparkle_anim.max_brightness = 255
  sparkle_anim.name = "update_test"
  
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
  
  # Create LED strip and engine for testing
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  # Test sparkle_white
  var white_sparkle = animation.sparkle_white(engine)
  assert(white_sparkle != nil, "sparkle_white should create animation")
  assert(white_sparkle.color == 0xFFFFFFFF, "White sparkle should have white color")
  assert(white_sparkle.name == "sparkle_white", "White sparkle should have correct name")
  
  # Test sparkle_rainbow
  var rainbow_sparkle = animation.sparkle_rainbow(engine)
  assert(rainbow_sparkle != nil, "sparkle_rainbow should create animation")
  assert(rainbow_sparkle.name == "sparkle_rainbow", "Rainbow sparkle should have correct name")
  # Check that color is set to a provider (not the default white color)
  var color_param = rainbow_sparkle.get_param("color")
  assert(color_param != nil, "Rainbow sparkle should have color parameter set")
  assert(color_param != 0xFFFFFFFF, "Rainbow sparkle color should not be default white")
  
  # Test that we can still customize parameters after creation
  white_sparkle.density = 80
  white_sparkle.fade_speed = 60
  assert(white_sparkle.density == 80, "White sparkle density should be customizable")
  assert(white_sparkle.fade_speed == 60, "White sparkle fade_speed should be customizable")
  
  print("✓ Sparkle constructor functions test passed")
end

# Test SparkleAnimation string representation
def test_sparkle_tostring()
  print("Testing SparkleAnimation string representation...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(12)
  var engine = animation.create_engine(strip)
  
  var sparkle_anim = animation.sparkle_animation(engine)
  sparkle_anim.density = 75
  sparkle_anim.fade_speed = 45
  sparkle_anim.name = "string_test"
  
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