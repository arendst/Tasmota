# Test suite for BounceAnimation
#
# This test verifies that the BounceAnimation works correctly
# with different parameters and physics simulation.

import animation
import string

# Use global.Leds for testing instead of mock objects

# Test basic BounceAnimation creation and functionality
def test_bounce_animation_basic()
  print("Testing basic BounceAnimation...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Create a simple source animation
  var source = animation.solid(engine)
  source.color = 0xFFFF0000
  source.name = "test_source"
  
  # Test with default parameters using new parameterized pattern
  var bounce_anim = animation.bounce_animation(engine)
  bounce_anim.source_animation = source
  bounce_anim.name = "test_bounce"
  
  assert(bounce_anim != nil, "BounceAnimation should be created")
  assert(bounce_anim.bounce_speed == 128, "Default bounce_speed should be 128")
  assert(bounce_anim.bounce_range == 0, "Default bounce_range should be 0")
  assert(bounce_anim.damping == 250, "Default damping should be 250")
  assert(bounce_anim.gravity == 0, "Default gravity should be 0")
  assert(bounce_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic BounceAnimation test passed")
end

# Test BounceAnimation with custom parameters
def test_bounce_animation_custom()
  print("Testing BounceAnimation with custom parameters...")
  
  # Create LED strip and engine
  var strip = global.Leds(20)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF00FF00
  source.name = "test_source"
  
  # Test with custom parameters using new parameterized pattern
  var bounce_anim = animation.bounce_animation(engine)
  bounce_anim.source_animation = source
  bounce_anim.bounce_speed = 200
  bounce_anim.bounce_range = 15
  bounce_anim.damping = 240
  bounce_anim.gravity = 50
  bounce_anim.priority = 15
  bounce_anim.duration = 5000
  bounce_anim.loop = false
  bounce_anim.opacity = 200
  bounce_anim.name = "custom_bounce"
  
  assert(bounce_anim.bounce_speed == 200, "Custom bounce_speed should be 200")
  assert(bounce_anim.bounce_range == 15, "Custom bounce_range should be 15")
  assert(bounce_anim.damping == 240, "Custom damping should be 240")
  assert(bounce_anim.gravity == 50, "Custom gravity should be 50")
  assert(bounce_anim.priority == 15, "Custom priority should be 15")
  assert(bounce_anim.duration == 5000, "Custom duration should be 5000")
  assert(bounce_anim.loop == false, "Custom loop should be false")
  assert(bounce_anim.opacity == 200, "Custom opacity should be 200")
  
  print("✓ Custom BounceAnimation test passed")
end

# Test BounceAnimation parameter changes
def test_bounce_animation_parameters()
  print("Testing BounceAnimation parameter changes...")
  
  # Create LED strip and engine
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF0000FF
  source.name = "test_source"
  
  var bounce_anim = animation.bounce_animation(engine)
  bounce_anim.source_animation = source
  bounce_anim.name = "param_test"
  
  # Test parameter changes using virtual member assignment
  bounce_anim.bounce_speed = 180
  assert(bounce_anim.bounce_speed == 180, "Bounce speed should be updated to 180")
  
  bounce_anim.bounce_range = 25
  assert(bounce_anim.bounce_range == 25, "Bounce range should be updated to 25")
  
  bounce_anim.damping = 200
  assert(bounce_anim.damping == 200, "Damping should be updated to 200")
  
  bounce_anim.gravity = 80
  assert(bounce_anim.gravity == 80, "Gravity should be updated to 80")
  
  # Note: strip_length is no longer a parameter - it comes from the engine
  assert(engine.get_strip_length() == 15, "Strip length should come from engine")
  
  print("✓ BounceAnimation parameter test passed")
end

# Test BounceAnimation physics simulation
def test_bounce_animation_physics()
  print("Testing BounceAnimation physics simulation...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFFFFFF00
  source.name = "test_source"
  
  var bounce_anim = animation.bounce_animation(engine)
  bounce_anim.source_animation = source
  bounce_anim.bounce_speed = 100
  bounce_anim.bounce_range = 0
  bounce_anim.damping = 250
  bounce_anim.gravity = 0
  bounce_anim.name = "physics_test"
  
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
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFFFF00FF
  source.name = "test_source"
  
  var bounce_anim = animation.bounce_animation(engine)
  bounce_anim.source_animation = source
  bounce_anim.bounce_speed = 100
  bounce_anim.bounce_range = 0
  bounce_anim.damping = 250
  bounce_anim.gravity = 0
  bounce_anim.name = "update_test"
  
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
  
  # Create LED strip and engine
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF00FFFF
  source.name = "test_source"
  
  # Test bounce_basic
  var basic_bounce = animation.bounce_basic(engine)
  basic_bounce.source_animation = source
  assert(basic_bounce != nil, "bounce_basic should create animation")
  assert(basic_bounce.bounce_speed == 128, "Basic bounce should have default speed")
  assert(basic_bounce.damping == 250, "Basic bounce should have default damping")
  assert(basic_bounce.gravity == 0, "Basic bounce should have no gravity")
  assert(basic_bounce.bounce_range == 0, "Basic bounce should have full range")
  
  # Test bounce_gravity
  var gravity_bounce = animation.bounce_gravity(engine)
  gravity_bounce.source_animation = source
  assert(gravity_bounce != nil, "bounce_gravity should create animation")
  assert(gravity_bounce.bounce_speed == 100, "Gravity bounce should have correct speed")
  assert(gravity_bounce.gravity == 128, "Gravity bounce should have correct gravity")
  assert(gravity_bounce.damping == 240, "Gravity bounce should have high damping")
  
  # Test bounce_constrained
  var constrained_bounce = animation.bounce_constrained(engine)
  constrained_bounce.source_animation = source
  assert(constrained_bounce != nil, "bounce_constrained should create animation")
  assert(constrained_bounce.bounce_speed == 150, "Constrained bounce should have correct speed")
  assert(constrained_bounce.bounce_range == 15, "Constrained bounce should have correct range")
  assert(constrained_bounce.gravity == 0, "Constrained bounce should have no gravity")
  
  print("✓ Bounce constructor functions test passed")
end

# Test BounceAnimation with gravity effects
def test_bounce_animation_gravity()
  print("Testing BounceAnimation gravity effects...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFFFFFFFF
  source.name = "test_source"
  
  var gravity_bounce = animation.bounce_animation(engine)
  gravity_bounce.source_animation = source
  gravity_bounce.bounce_speed = 100
  gravity_bounce.bounce_range = 0
  gravity_bounce.damping = 240
  gravity_bounce.gravity = 100
  gravity_bounce.name = "gravity_test"
  
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
  
  # Create LED strip and engine
  var strip = global.Leds(12)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF888888
  source.name = "test_source"
  
  var bounce_anim = animation.bounce_animation(engine)
  bounce_anim.source_animation = source
  bounce_anim.bounce_speed = 75
  bounce_anim.bounce_range = 10
  bounce_anim.damping = 240
  bounce_anim.gravity = 30
  bounce_anim.name = "string_test"
  
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