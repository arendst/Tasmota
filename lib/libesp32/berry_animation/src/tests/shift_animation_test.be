# Test suite for ShiftAnimation
#
# This test verifies that the ShiftAnimation works correctly
# with different parameters and source animations.

import animation
import string

# Test basic ShiftAnimation creation and functionality
def test_shift_animation_basic()
  print("Testing basic ShiftAnimation...")
  
  # Create a simple source animation
  var source = animation.filled_animation(0xFFFF0000, 10, 0, true, "test_source")
  
  # Test with default parameters
  var shift_anim = animation.shift_animation(source, nil, nil, nil, 10, 10, 0, true, "test_shift")
  
  assert(shift_anim != nil, "ShiftAnimation should be created")
  assert(shift_anim.shift_speed == 128, "Default shift_speed should be 128")
  assert(shift_anim.direction == 1, "Default direction should be 1")
  assert(shift_anim.wrap_around == true, "Default wrap_around should be true")
  assert(shift_anim.strip_length == 10, "Strip length should be 10")
  assert(shift_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic ShiftAnimation test passed")
end

# Test ShiftAnimation with custom parameters
def test_shift_animation_custom()
  print("Testing ShiftAnimation with custom parameters...")
  
  var source = animation.filled_animation(0xFF00FF00, 10, 0, true, "test_source")
  
  # Test with custom parameters
  var shift_anim = animation.shift_animation(source, 200, -1, false, 20, 15, 5000, false, "custom_shift")
  
  assert(shift_anim.shift_speed == 200, "Custom shift_speed should be 200")
  assert(shift_anim.direction == -1, "Custom direction should be -1")
  assert(shift_anim.wrap_around == false, "Custom wrap_around should be false")
  assert(shift_anim.strip_length == 20, "Custom strip length should be 20")
  assert(shift_anim.priority == 15, "Custom priority should be 15")
  assert(shift_anim.duration == 5000, "Custom duration should be 5000")
  assert(shift_anim.loop == 0, "Custom loop should be 0 (false)")
  
  print("✓ Custom ShiftAnimation test passed")
end

# Test ShiftAnimation parameter changes
def test_shift_animation_parameters()
  print("Testing ShiftAnimation parameter changes...")
  
  var source = animation.filled_animation(0xFF0000FF, 10, 0, true, "test_source")
  var shift_anim = animation.shift_animation(source, nil, nil, nil, 15, 10, 0, true, "param_test")
  
  # Test parameter changes
  shift_anim.set_param("shift_speed", 180)
  assert(shift_anim.shift_speed == 180, "Shift speed should be updated to 180")
  
  shift_anim.set_param("direction", -1)
  assert(shift_anim.direction == -1, "Direction should be updated to -1")
  
  shift_anim.set_param("wrap_around", 0)
  assert(shift_anim.wrap_around == false, "Wrap around should be updated to false")
  
  shift_anim.set_param("strip_length", 25)
  assert(shift_anim.strip_length == 25, "Strip length should be updated to 25")
  assert(shift_anim.current_colors.size() == 25, "Current colors array should be resized")
  
  print("✓ ShiftAnimation parameter test passed")
end

# Test ShiftAnimation update and render
def test_shift_animation_update_render()
  print("Testing ShiftAnimation update and render...")
  
  var source = animation.filled_animation(0xFFFFFF00, 10, 0, true, "test_source")
  var shift_anim = animation.shift_animation(source, 100, 1, true, 10, 10, 0, true, "update_test")
  var frame = animation.frame_buffer(10)
  
  # Start animation
  shift_anim.start(1000)
  assert(shift_anim.is_running == true, "Animation should be running after start")
  
  # Test update
  var result = shift_anim.update(1500)
  assert(result == true, "Update should return true for running animation")
  
  # Test render
  result = shift_anim.render(frame, 1500)
  assert(result == true, "Render should return true for running animation")
  
  # Check that colors were set
  var has_colors = false
  var i = 0
  while i < frame.width
    if frame.get_pixel_color(i) != 0xFF000000
      has_colors = true
      break
    end
    i += 1
  end
  assert(has_colors == true, "Frame should have non-black pixels after render")
  
  print("✓ ShiftAnimation update/render test passed")
end

# Test global constructor functions
def test_shift_constructors()
  print("Testing shift constructor functions...")
  
  var source = animation.filled_animation(0xFFFF00FF, 10, 0, true, "test_source")
  
  # Test shift_basic
  var basic_shift = animation.shift_basic(source, 150, 1, 15, 12)
  assert(basic_shift != nil, "shift_basic should create animation")
  assert(basic_shift.shift_speed == 150, "Basic shift should have correct speed")
  assert(basic_shift.direction == 1, "Basic shift should have correct direction")
  assert(basic_shift.strip_length == 15, "Basic shift should have correct strip length")
  assert(basic_shift.priority == 12, "Basic shift should have correct priority")
  
  # Test shift_scroll_right
  var scroll_right = animation.shift_scroll_right(source, 120, 20, 8)
  assert(scroll_right != nil, "shift_scroll_right should create animation")
  assert(scroll_right.shift_speed == 120, "Scroll right should have correct speed")
  assert(scroll_right.direction == 1, "Scroll right should have direction 1")
  
  # Test shift_scroll_left
  var scroll_left = animation.shift_scroll_left(source, 100, 25, 15)
  assert(scroll_left != nil, "shift_scroll_left should create animation")
  assert(scroll_left.shift_speed == 100, "Scroll left should have correct speed")
  assert(scroll_left.direction == -1, "Scroll left should have direction -1")
  
  print("✓ Shift constructor functions test passed")
end

# Test ShiftAnimation string representation
def test_shift_tostring()
  print("Testing ShiftAnimation string representation...")
  
  var source = animation.filled_animation(0xFF00FFFF, 10, 0, true, "test_source")
  var shift_anim = animation.shift_animation(source, 75, -1, true, 12, 10, 0, true, "string_test")
  var str_repr = str(shift_anim)
  
  assert(type(str_repr) == "string", "String representation should be a string")
  assert(string.find(str_repr, "ShiftAnimation") >= 0, "String should contain 'ShiftAnimation'")
  assert(string.find(str_repr, "left") >= 0, "String should contain direction")
  assert(string.find(str_repr, "75") >= 0, "String should contain speed value")
  
  print("✓ ShiftAnimation string representation test passed")
end

# Run all tests
def run_shift_animation_tests()
  print("=== ShiftAnimation Tests ===")
  
  try
    test_shift_animation_basic()
    test_shift_animation_custom()
    test_shift_animation_parameters()
    test_shift_animation_update_render()
    test_shift_constructors()
    test_shift_tostring()
    
    print("=== All ShiftAnimation tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_shift_animation_tests = run_shift_animation_tests

run_shift_animation_tests()

return run_shift_animation_tests