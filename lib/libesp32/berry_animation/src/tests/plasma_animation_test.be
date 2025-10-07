# Test suite for PlasmaAnimation
#
# This test verifies that the PlasmaAnimation works correctly
# with different parameters and color providers.

import animation
import string

# Test basic PlasmaAnimation creation and functionality
def test_plasma_animation_basic()
  print("Testing basic PlasmaAnimation...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Test with default parameters
  var plasma_anim = animation.plasma_animation(engine)
  plasma_anim.name = "test_plasma"
  
  assert(plasma_anim != nil, "PlasmaAnimation should be created")
  assert(plasma_anim.freq_x == 32, "Default freq_x should be 32")
  assert(plasma_anim.freq_y == 23, "Default freq_y should be 23")
  assert(plasma_anim.phase_x == 0, "Default phase_x should be 0")
  assert(plasma_anim.phase_y == 64, "Default phase_y should be 64")
  assert(plasma_anim.time_speed == 50, "Default time_speed should be 50")
  assert(plasma_anim.blend_mode == 0, "Default blend_mode should be 0")
  assert(plasma_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic PlasmaAnimation test passed")
end

# Test PlasmaAnimation with custom parameters
def test_plasma_animation_custom()
  print("Testing PlasmaAnimation with custom parameters...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(20)
  var engine = animation.create_engine(strip)
  
  # Test with custom parameters using virtual member assignment
  var plasma_anim = animation.plasma_animation(engine)
  plasma_anim.color = 0xFF00FF00
  plasma_anim.freq_x = 50
  plasma_anim.freq_y = 40
  plasma_anim.phase_x = 30
  plasma_anim.phase_y = 80
  plasma_anim.time_speed = 120
  plasma_anim.blend_mode = 1
  plasma_anim.priority = 15
  plasma_anim.duration = 5000
  plasma_anim.loop = false
  plasma_anim.name = "custom_plasma"
  
  assert(plasma_anim.freq_x == 50, "Custom freq_x should be 50")
  assert(plasma_anim.freq_y == 40, "Custom freq_y should be 40")
  assert(plasma_anim.phase_x == 30, "Custom phase_x should be 30")
  assert(plasma_anim.phase_y == 80, "Custom phase_y should be 80")
  assert(plasma_anim.time_speed == 120, "Custom time_speed should be 120")
  assert(plasma_anim.blend_mode == 1, "Custom blend_mode should be 1")
  assert(plasma_anim.priority == 15, "Custom priority should be 15")
  assert(plasma_anim.duration == 5000, "Custom duration should be 5000")
  assert(plasma_anim.loop == false, "Custom loop should be false")
  
  print("✓ Custom PlasmaAnimation test passed")
end

# Test PlasmaAnimation parameter changes
def test_plasma_animation_parameters()
  print("Testing PlasmaAnimation parameter changes...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  var plasma_anim = animation.plasma_animation(engine)
  plasma_anim.name = "param_test"
  
  # Test parameter changes using virtual member assignment
  plasma_anim.freq_x = 60
  assert(plasma_anim.freq_x == 60, "freq_x should be updated to 60")
  
  plasma_anim.freq_y = 45
  assert(plasma_anim.freq_y == 45, "freq_y should be updated to 45")
  
  plasma_anim.time_speed = 80
  assert(plasma_anim.time_speed == 80, "time_speed should be updated to 80")
  
  plasma_anim.blend_mode = 2
  assert(plasma_anim.blend_mode == 2, "blend_mode should be updated to 2")
  
  # Test that strip length comes from engine
  var strip_length = engine.get_strip_length()
  assert(strip_length == 15, "Strip length should come from engine")
  
  print("✓ PlasmaAnimation parameter test passed")
end

# Test PlasmaAnimation update and render
def test_plasma_animation_update_render()
  print("Testing PlasmaAnimation update and render...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var plasma_anim = animation.plasma_animation(engine)
  plasma_anim.color = 0xFFFF0000
  plasma_anim.freq_x = 40
  plasma_anim.freq_y = 30
  plasma_anim.time_speed = 60
  plasma_anim.name = "update_test"
  
  var frame = animation.frame_buffer(10)
  
  # Start animation
  plasma_anim.start(1000)
  assert(plasma_anim.is_running == true, "Animation should be running after start")
  
  # Test update
  var result = plasma_anim.update(1500)
  assert(result == true, "Update should return true for running animation")
  
  # Test render
  result = plasma_anim.render(frame, 1500)
  assert(result == true, "Render should return true for running animation")
  
  # Check that colors were set (should not all be black)
  var has_non_black = false
  var i = 0
  while i < frame.width
    if frame.get_pixel_color(i) != 0xFF000000
      has_non_black = true
      break
    end
    i += 1
  end
  assert(has_non_black == true, "Frame should have non-black pixels after render")
  
  print("✓ PlasmaAnimation update/render test passed")
end

# Test global constructor functions
def test_plasma_constructors()
  print("Testing plasma constructor functions...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  # Test plasma_rainbow
  var rainbow_plasma = animation.plasma_rainbow(engine)
  assert(rainbow_plasma != nil, "plasma_rainbow should create animation")
  assert(rainbow_plasma.time_speed == 50, "Rainbow plasma should have correct time_speed")
  assert(rainbow_plasma.name == "plasma_rainbow", "Rainbow plasma should have correct name")
  
  # Test plasma_fast
  var fast_plasma = animation.plasma_fast(engine)
  assert(fast_plasma != nil, "plasma_fast should create animation")
  assert(fast_plasma.time_speed == 150, "Fast plasma should have correct time_speed")
  assert(fast_plasma.freq_x == 48, "Fast plasma should have correct freq_x")
  assert(fast_plasma.freq_y == 35, "Fast plasma should have correct freq_y")
  
  print("✓ Plasma constructor functions test passed")
end

# Test PlasmaAnimation string representation
def test_plasma_tostring()
  print("Testing PlasmaAnimation string representation...")
  
  # Create LED strip and engine for testing
  var strip = global.Leds(12)
  var engine = animation.create_engine(strip)
  
  var plasma_anim = animation.plasma_animation(engine)
  plasma_anim.freq_x = 55
  plasma_anim.freq_y = 35
  plasma_anim.phase_x = 10
  plasma_anim.phase_y = 70
  plasma_anim.time_speed = 85
  plasma_anim.blend_mode = 1
  plasma_anim.name = "string_test"
  
  var str_repr = str(plasma_anim)
  
  assert(type(str_repr) == "string", "String representation should be a string")
  assert(string.find(str_repr, "PlasmaAnimation") >= 0, "String should contain 'PlasmaAnimation'")
  assert(string.find(str_repr, "55") >= 0, "String should contain freq_x value")
  assert(string.find(str_repr, "35") >= 0, "String should contain freq_y value")
  
  print("✓ PlasmaAnimation string representation test passed")
end

# Run all tests
def run_plasma_animation_tests()
  print("=== PlasmaAnimation Tests ===")
  
  try
    test_plasma_animation_basic()
    test_plasma_animation_custom()
    test_plasma_animation_parameters()
    test_plasma_animation_update_render()
    test_plasma_constructors()
    test_plasma_tostring()
    
    print("=== All PlasmaAnimation tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_plasma_animation_tests = run_plasma_animation_tests

run_plasma_animation_tests()

return run_plasma_animation_tests