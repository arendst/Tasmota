# Test suite for ShiftAnimation
#
# This test verifies that the ShiftAnimation works correctly
# with different parameters and source animations.

import animation
import string

# Test basic ShiftAnimation creation and functionality
def test_shift_animation_basic()
  print("Testing basic ShiftAnimation...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Create a simple source animation
  var source = animation.solid(engine)
  source.color = 0xFFFF0000
  
  # Test with default parameters
  var shift_anim = animation.shift_animation(engine)
  shift_anim.source_animation = source
  
  assert(shift_anim != nil, "ShiftAnimation should be created")
  assert(shift_anim.shift_speed == 128, "Default shift_speed should be 128")
  assert(shift_anim.direction == 1, "Default direction should be 1")
  assert(shift_anim.wrap_around == true, "Default wrap_around should be true")
  assert(shift_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic ShiftAnimation test passed")
end

# Test ShiftAnimation with custom parameters
def test_shift_animation_custom()
  print("Testing ShiftAnimation with custom parameters...")
  
  # Create LED strip and engine
  var strip = global.Leds(20)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF00FF00
  
  # Test with custom parameters
  var shift_anim = animation.shift_animation(engine)
  shift_anim.source_animation = source
  shift_anim.shift_speed = 200
  shift_anim.direction = -1
  shift_anim.wrap_around = false
  shift_anim.priority = 15
  shift_anim.duration = 5000
  shift_anim.loop = false
  
  assert(shift_anim.shift_speed == 200, "Custom shift_speed should be 200")
  assert(shift_anim.direction == -1, "Custom direction should be -1")
  assert(shift_anim.wrap_around == false, "Custom wrap_around should be false")
  assert(shift_anim.priority == 15, "Custom priority should be 15")
  assert(shift_anim.duration == 5000, "Custom duration should be 5000")
  assert(shift_anim.loop == false, "Custom loop should be false")
  
  print("✓ Custom ShiftAnimation test passed")
end

# Test ShiftAnimation parameter changes
def test_shift_animation_parameters()
  print("Testing ShiftAnimation parameter changes...")
  
  # Create LED strip and engine
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF0000FF
  
  var shift_anim = animation.shift_animation(engine)
  shift_anim.source_animation = source
  
  # Test parameter changes using virtual member assignment
  shift_anim.shift_speed = 180
  assert(shift_anim.shift_speed == 180, "Shift speed should be updated to 180")
  
  shift_anim.direction = -1
  assert(shift_anim.direction == -1, "Direction should be updated to -1")
  
  shift_anim.wrap_around = false
  assert(shift_anim.wrap_around == false, "Wrap around should be updated to false")
  
  # Test method-based parameter setting
  var success = shift_anim.set_param("shift_speed", 200)
  assert(success == true, "set_param should return true for valid parameter")
  assert(shift_anim.shift_speed == 200, "Shift speed should be updated via set_param")
  
  print("✓ ShiftAnimation parameter test passed")
end

# Test ShiftAnimation update and render
def test_shift_animation_update_render()
  print("Testing ShiftAnimation update and render...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFFFFFF00
  
  var shift_anim = animation.shift_animation(engine)
  shift_anim.source_animation = source
  shift_anim.shift_speed = 100
  
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

# Test factory functions
def test_shift_constructors()
  print("Testing shift factory functions...")
  
  # Create LED strip and engine
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFFFF00FF
  
  # Test shift_scroll_right
  var scroll_right = animation.shift_scroll_right(engine)
  assert(scroll_right != nil, "shift_scroll_right should create animation")
  assert(scroll_right.shift_speed == 128, "Scroll right should have default speed")
  assert(scroll_right.direction == 1, "Scroll right should have direction 1")
  assert(scroll_right.wrap_around == true, "Scroll right should wrap around")
  
  # Test shift_scroll_left
  var scroll_left = animation.shift_scroll_left(engine)
  assert(scroll_left != nil, "shift_scroll_left should create animation")
  assert(scroll_left.shift_speed == 128, "Scroll left should have default speed")
  assert(scroll_left.direction == -1, "Scroll left should have direction -1")
  assert(scroll_left.wrap_around == true, "Scroll left should wrap around")
  
  # Test shift_fast_scroll
  var fast_scroll = animation.shift_fast_scroll(engine)
  assert(fast_scroll != nil, "shift_fast_scroll should create animation")
  assert(fast_scroll.shift_speed == 200, "Fast scroll should have speed 200")
  assert(fast_scroll.direction == 1, "Fast scroll should have direction 1")
  assert(fast_scroll.wrap_around == true, "Fast scroll should wrap around")
  
  # Test setting source animation on factory-created animations
  scroll_right.source_animation = source
  assert(scroll_right.source_animation == source, "Should be able to set source animation")
  
  print("✓ Shift factory functions test passed")
end

# Test ShiftAnimation string representation
def test_shift_tostring()
  print("Testing ShiftAnimation string representation...")
  
  # Create LED strip and engine
  var strip = global.Leds(12)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF00FFFF
  
  var shift_anim = animation.shift_animation(engine)
  shift_anim.source_animation = source
  shift_anim.shift_speed = 75
  shift_anim.direction = -1
  
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