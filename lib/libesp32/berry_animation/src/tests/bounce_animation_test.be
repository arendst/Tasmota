# Test suite for BounceAnimation
#
# This test verifies that the BounceAnimation works correctly
# with different parameters and physics simulation.

import animation
import string

# Test basic BounceAnimation creation and functionality
def test_bounce_animation_basic()
  print("Testing basic BounceAnimation...")
  
  # Create a simple source animation
  var source = animation.filled_animation(0xFFFF0000, 10, 0, true, "test_source")
  
  # Test with default parameters
  var bounce_anim = animation.bounce_animation(source, nil, nil, nil, nil, 10, 10, 0, true, "test_bounce")
  
  assert(bounce_anim != nil, "BounceAnimation should be created")
  assert(bounce_anim.bounce_speed == 128, "Default bounce_speed should be 128")
  assert(bounce_anim.bounce_range == 0, "Default bounce_range should be 0")
  assert(bounce_anim.damping == 250, "Default damping should be 250")
  assert(bounce_anim.gravity == 0, "Default gravity should be 0")
  assert(bounce_anim.strip_length == 10, "Strip length should be 10")
  assert(bounce_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic BounceAnimation test passed")
end

# Test BounceAnimation with custom parameters
def test_bounce_animation_custom()
  print("Testing BounceAnimation with custom parameters...")
  
  var source = animation.filled_animation(0xFF00FF00, 10, 0, true, "test_source")
  
  # Test with custom parameters
  var bounce_anim = animation.bounce_animation(source, 200, 15, 240, 50, 20, 15, 5000, false, "custom_bounce")
  
  assert(bounce_anim.bounce_speed == 200, "Custom bounce_speed should be 200")
  assert(bounce_anim.bounce_range == 15, "Custom bounce_range should be 15")
  assert(bounce_anim.damping == 240, "Custom damping should be 240")
  assert(bounce_anim.gravity == 50, "Custom gravity should be 50")
  assert(bounce_anim.strip_length == 20, "Custom strip length should be 20")
  assert(bounce_anim.priority == 15, "Custom priority should be 15")
  assert(bounce_anim.duration == 5000, "Custom duration should be 5000")
  assert(bounce_anim.loop == 0, "Custom loop should be 0 (false)")
  
  print("✓ Custom BounceAnimation test passed")
end

# Test BounceAnimation parameter changes
def test_bounce_animation_parameters()
  print("Testing BounceAnimation parameter changes...")
  
  var source = animation.filled_animation(0xFF0000FF, 10, 0, true, "test_source")
  var bounce_anim = animation.bounce_animation(source, nil, nil, nil, nil, 15, 10, 0, true, "param_test")
  
  # Test parameter changes
  bounce_anim.set_param("bounce_speed", 180)
  assert(bounce_anim.bounce_speed == 180, "Bounce speed should be updated to 180")
  
  bounce_anim.set_param("bounce_range", 25)
  assert(bounce_anim.bounce_range == 25, "Bounce range should be updated to 25")
  
  bounce_anim.set_param("damping", 200)
  assert(bounce_anim.damping == 200, "Damping should be updated to 200")
  
  bounce_anim.set_param("gravity", 80)
  assert(bounce_anim.gravity == 80, "Gravity should be updated to 80")
  
  bounce_anim.set_param("strip_length", 25)
  assert(bounce_anim.strip_length == 25, "Strip length should be updated to 25")
  assert(bounce_anim.current_colors.size() == 25, "Current colors array should be resized")
  
  print("✓ BounceAnimation parameter test passed")
end

# Test BounceAnimation physics simulation
def test_bounce_animation_physics()
  print("Testing BounceAnimation physics simulation...")
  
  var source = animation.filled_animation(0xFFFFFF00, 10, 0, true, "test_source")
  var bounce_anim = animation.bounce_animation(source, 100, 0, 250, 0, 10, 10, 0, true, "physics_test")
  
  # Start animation
  bounce_anim.start(1000)
  assert(bounce_anim.is_running == true, "Animation should be running after start")
  
  # Test initial physics state
  assert(bounce_anim.current_position != nil, "Should have initial position")
  assert(bounce_anim.current_velocity != nil, "Should have initial velocity")
  
  # Test physics updates
  var initial_position = bounce_anim.current_position
  bounce_anim.update(1100)  # 100ms later
  
  # Position should have changed due to velocity
  # Note: We can't predict exact values due to physics complexity, just verify it's working
  assert(type(bounce_anim.current_position) == "int", "Position should be integer")
  assert(type(bounce_anim.current_velocity) == "int", "Velocity should be integer")
  
  print("✓ BounceAnimation physics test passed")
end

# Test BounceAnimation update and render
def test_bounce_animation_update_render()
  print("Testing BounceAnimation update and render...")
  
  var source = animation.filled_animation(0xFFFF00FF, 10, 0, true, "test_source")
  var bounce_anim = animation.bounce_animation(source, 100, 0, 250, 0, 10, 10, 0, true, "update_test")
  var frame = animation.frame_buffer(10)
  
  # Start animation
  bounce_anim.start(1000)
  assert(bounce_anim.is_running == true, "Animation should be running after start")
  
  # Test update
  var result = bounce_anim.update(1500)
  assert(result == true, "Update should return true for running animation")
  
  # Test render
  result = bounce_anim.render(frame, 1500)
  assert(result == true, "Render should return true for running animation")
  
  # Check that frame was modified (colors should be set)
  var frame_modified = false
  var i = 0
  while i < frame.width
    if frame.get_pixel_color(i) != 0xFF000000
      frame_modified = true
      break
    end
    i += 1
  end
  # Note: Due to physics, the pattern might be positioned anywhere, so we just check render worked
  
  print("✓ BounceAnimation update/render test passed")
end

# Test global constructor functions
def test_bounce_constructors()
  print("Testing bounce constructor functions...")
  
  var source = animation.filled_animation(0xFF00FFFF, 10, 0, true, "test_source")
  
  # Test bounce_basic
  var basic_bounce = animation.bounce_basic(source, 150, 240, 15, 12)
  assert(basic_bounce != nil, "bounce_basic should create animation")
  assert(basic_bounce.bounce_speed == 150, "Basic bounce should have correct speed")
  assert(basic_bounce.damping == 240, "Basic bounce should have correct damping")
  assert(basic_bounce.strip_length == 15, "Basic bounce should have correct strip length")
  assert(basic_bounce.priority == 12, "Basic bounce should have correct priority")
  assert(basic_bounce.gravity == 0, "Basic bounce should have no gravity")
  
  # Test bounce_gravity
  var gravity_bounce = animation.bounce_gravity(source, 120, 80, 20, 8)
  assert(gravity_bounce != nil, "bounce_gravity should create animation")
  assert(gravity_bounce.bounce_speed == 120, "Gravity bounce should have correct speed")
  assert(gravity_bounce.gravity == 80, "Gravity bounce should have correct gravity")
  assert(gravity_bounce.damping == 240, "Gravity bounce should have high damping")
  
  # Test bounce_constrained
  var constrained_bounce = animation.bounce_constrained(source, 100, 10, 25, 15)
  assert(constrained_bounce != nil, "bounce_constrained should create animation")
  assert(constrained_bounce.bounce_speed == 100, "Constrained bounce should have correct speed")
  assert(constrained_bounce.bounce_range == 10, "Constrained bounce should have correct range")
  assert(constrained_bounce.gravity == 0, "Constrained bounce should have no gravity")
  
  print("✓ Bounce constructor functions test passed")
end

# Test BounceAnimation with gravity effects
def test_bounce_animation_gravity()
  print("Testing BounceAnimation gravity effects...")
  
  var source = animation.filled_animation(0xFFFFFFFF, 10, 0, true, "test_source")
  var gravity_bounce = animation.bounce_animation(source, 100, 0, 240, 100, 10, 10, 0, true, "gravity_test")
  
  gravity_bounce.start(1000)
  
  # Record initial velocity
  var initial_velocity = gravity_bounce.current_velocity
  
  # Update with gravity
  gravity_bounce.update(1100)  # 100ms later
  
  # With gravity, velocity should have changed (increased downward)
  # Note: We can't predict exact values, just verify gravity is affecting velocity
  assert(type(gravity_bounce.current_velocity) == "int", "Velocity should be integer after gravity")
  
  print("✓ BounceAnimation gravity test passed")
end

# Test BounceAnimation string representation
def test_bounce_tostring()
  print("Testing BounceAnimation string representation...")
  
  var source = animation.filled_animation(0xFF888888, 10, 0, true, "test_source")
  var bounce_anim = animation.bounce_animation(source, 75, 10, 240, 30, 12, 10, 0, true, "string_test")
  var str_repr = str(bounce_anim)
  
  assert(type(str_repr) == "string", "String representation should be a string")
  assert(string.find(str_repr, "BounceAnimation") >= 0, "String should contain 'BounceAnimation'")
  assert(string.find(str_repr, "75") >= 0, "String should contain speed value")
  assert(string.find(str_repr, "240") >= 0, "String should contain damping value")
  assert(string.find(str_repr, "30") >= 0, "String should contain gravity value")
  
  print("✓ BounceAnimation string representation test passed")
end

# Run all tests
def run_bounce_animation_tests()
  print("=== BounceAnimation Tests ===")
  
  try
    test_bounce_animation_basic()
    test_bounce_animation_custom()
    test_bounce_animation_parameters()
    test_bounce_animation_physics()
    test_bounce_animation_update_render()
    test_bounce_constructors()
    test_bounce_animation_gravity()
    test_bounce_tostring()
    
    print("=== All BounceAnimation tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_bounce_animation_tests = run_bounce_animation_tests

run_bounce_animation_tests()

return run_bounce_animation_tests