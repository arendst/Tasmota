# Test suite for Motion Effects (Bounce, Scale, Jitter)
#
# This test verifies that the motion effect animations work correctly
# with different parameters and source animations.

import animation
import string

# Test basic BounceAnimation creation and functionality
def test_bounce_animation_basic()
  print("Testing basic BounceAnimation...")
  
  # Create engine and source animation
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var source = animation.solid(engine)
  source.color = 0xFFFF0000
  
  # Create bounce animation with engine
  var bounce_anim = animation.bounce_animation(engine)
  bounce_anim.source_animation = source
  
  assert(bounce_anim != nil, "BounceAnimation should be created")
  assert(bounce_anim.bounce_speed == 128, "Default bounce_speed should be 128")
  assert(bounce_anim.bounce_range == 0, "Default bounce_range should be 0")
  assert(bounce_anim.damping == 250, "Default damping should be 250")
  assert(bounce_anim.gravity == 0, "Default gravity should be 0")
  
  assert(bounce_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic BounceAnimation test passed")
end

# Test basic ScaleAnimation creation and functionality
def test_scale_animation_basic()
  print("Testing basic ScaleAnimation...")
  
  # Create engine and source animation
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var source = animation.solid(engine)
  source.color = 0xFF00FF00
  
  # Create scale animation with engine
  var scale_anim = animation.scale_animation(engine)
  scale_anim.source_animation = source
  
  assert(scale_anim != nil, "ScaleAnimation should be created")
  assert(scale_anim.scale_factor == 128, "Default scale_factor should be 128")
  assert(scale_anim.scale_speed == 0, "Default scale_speed should be 0")
  assert(scale_anim.scale_mode == 0, "Default scale_mode should be 0")
  assert(scale_anim.scale_center == 128, "Default scale_center should be 128")
  assert(scale_anim.interpolation == 1, "Default interpolation should be 1")
  assert(scale_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic ScaleAnimation test passed")
end

# Test basic JitterAnimation creation and functionality
def test_jitter_animation_basic()
  print("Testing basic JitterAnimation...")
  
  # Create engine and source animation
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var source = animation.solid(engine)
  source.color = 0xFF0000FF
  
  # Create jitter animation with engine
  var jitter_anim = animation.jitter_animation(engine)
  jitter_anim.source_animation = source
  
  assert(jitter_anim != nil, "JitterAnimation should be created")
  assert(jitter_anim.jitter_intensity == 100, "Default jitter_intensity should be 100")
  assert(jitter_anim.jitter_frequency == 60, "Default jitter_frequency should be 60")
  assert(jitter_anim.jitter_type == 0, "Default jitter_type should be 0")
  assert(jitter_anim.position_range == 50, "Default position_range should be 50")
  assert(jitter_anim.color_range == 30, "Default color_range should be 30")
  assert(jitter_anim.brightness_range == 40, "Default brightness_range should be 40")
  assert(jitter_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic JitterAnimation test passed")
end

# Test motion effects with custom parameters
def test_motion_effects_custom()
  print("Testing motion effects with custom parameters...")
  
  var strip = global.Leds(20)
  var engine = animation.create_engine(strip)
  var source = animation.solid(engine)
  source.color = 0xFFFFFF00
  
  # Test bounce with custom parameters
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
  
  assert(bounce_anim.bounce_speed == 200, "Custom bounce_speed should be 200")
  assert(bounce_anim.bounce_range == 15, "Custom bounce_range should be 15")
  assert(bounce_anim.damping == 240, "Custom damping should be 240")
  assert(bounce_anim.gravity == 50, "Custom gravity should be 50")
  
  # Test scale with custom parameters
  var scale_anim = animation.scale_animation(engine)
  scale_anim.source_animation = source
  scale_anim.scale_factor = 200
  scale_anim.scale_speed = 80
  scale_anim.scale_mode = 1
  scale_anim.scale_center = 100
  scale_anim.interpolation = 0
  scale_anim.priority = 15
  scale_anim.duration = 5000
  scale_anim.loop = false
  scale_anim.opacity = 200
  
  assert(scale_anim.scale_factor == 200, "Custom scale_factor should be 200")
  assert(scale_anim.scale_speed == 80, "Custom scale_speed should be 80")
  assert(scale_anim.scale_mode == 1, "Custom scale_mode should be 1")
  assert(scale_anim.scale_center == 100, "Custom scale_center should be 100")
  assert(scale_anim.interpolation == 0, "Custom interpolation should be 0")
  
  # Test jitter with custom parameters
  var jitter_anim = animation.jitter_animation(engine)
  jitter_anim.source_animation = source
  jitter_anim.jitter_intensity = 150
  jitter_anim.jitter_frequency = 120
  jitter_anim.jitter_type = 2
  jitter_anim.position_range = 80
  jitter_anim.color_range = 60
  jitter_anim.brightness_range = 70
  jitter_anim.priority = 15
  jitter_anim.duration = 5000
  jitter_anim.loop = false
  jitter_anim.opacity = 200
  
  assert(jitter_anim.jitter_intensity == 150, "Custom jitter_intensity should be 150")
  assert(jitter_anim.jitter_frequency == 120, "Custom jitter_frequency should be 120")
  assert(jitter_anim.jitter_type == 2, "Custom jitter_type should be 2")
  assert(jitter_anim.position_range == 80, "Custom position_range should be 80")
  assert(jitter_anim.color_range == 60, "Custom color_range should be 60")
  assert(jitter_anim.brightness_range == 70, "Custom brightness_range should be 70")
  
  print("✓ Custom motion effects test passed")
end

# Test motion effects update and render
def test_motion_effects_update_render()
  print("Testing motion effects update and render...")
  
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var source = animation.solid(engine)
  source.color = 0xFFFF00FF
  var frame = animation.frame_buffer(10)
  
  # Test bounce update/render
  var bounce_anim = animation.bounce_animation(engine)
  bounce_anim.source_animation = source
  bounce_anim.bounce_speed = 100
  bounce_anim.bounce_range = 0
  bounce_anim.damping = 250
  bounce_anim.gravity = 0
  
  bounce_anim.start(1000)
  assert(bounce_anim.is_running == true, "Bounce animation should be running after start")
  
  var result = bounce_anim.update(1500)
  assert(result == true, "Bounce update should return true for running animation")
  
  result = bounce_anim.render(frame, 1500)
  assert(result == true, "Bounce render should return true for running animation")
  
  # Test scale update/render
  var scale_anim = animation.scale_animation(engine)
  scale_anim.source_animation = source
  scale_anim.scale_factor = 150
  scale_anim.scale_speed = 0
  scale_anim.scale_mode = 0
  scale_anim.scale_center = 128
  scale_anim.interpolation = 1
  
  scale_anim.start(2000)
  assert(scale_anim.is_running == true, "Scale animation should be running after start")
  
  result = scale_anim.update(2500)
  assert(result == true, "Scale update should return true for running animation")
  
  result = scale_anim.render(frame, 2500)
  assert(result == true, "Scale render should return true for running animation")
  
  # Test jitter update/render
  var jitter_anim = animation.jitter_animation(engine)
  jitter_anim.source_animation = source
  jitter_anim.jitter_intensity = 100
  jitter_anim.jitter_frequency = 60
  jitter_anim.jitter_type = 0
  jitter_anim.position_range = 50
  jitter_anim.color_range = 30
  jitter_anim.brightness_range = 40
  
  jitter_anim.start(3000)
  assert(jitter_anim.is_running == true, "Jitter animation should be running after start")
  
  result = jitter_anim.update(3500)
  assert(result == true, "Jitter update should return true for running animation")
  
  result = jitter_anim.render(frame, 3500)
  assert(result == true, "Jitter render should return true for running animation")
  
  print("✓ Motion effects update/render test passed")
end

# Test global constructor functions
def test_motion_effects_constructors()
  print("Testing motion effects constructor functions...")
  
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  var source = animation.solid(engine)
  source.color = 0xFF00FFFF
  
  # Test bounce constructors
  var basic_bounce = animation.bounce_basic(engine)
  assert(basic_bounce != nil, "bounce_basic should create animation")
  basic_bounce.source_animation = source
  basic_bounce.bounce_speed = 150
  basic_bounce.damping = 240
  assert(basic_bounce.bounce_speed == 150, "Basic bounce should have correct speed")
  assert(basic_bounce.damping == 240, "Basic bounce should have correct damping")
  
  var gravity_bounce = animation.bounce_gravity(engine)
  assert(gravity_bounce != nil, "bounce_gravity should create animation")
  gravity_bounce.source_animation = source
  gravity_bounce.bounce_speed = 120
  gravity_bounce.gravity = 80
  assert(gravity_bounce.bounce_speed == 120, "Gravity bounce should have correct speed")
  assert(gravity_bounce.gravity == 80, "Gravity bounce should have correct gravity")
  
  # Test scale constructors
  var static_scale = animation.scale_static(engine)
  assert(static_scale != nil, "scale_static should create animation")
  static_scale.source_animation = source
  static_scale.scale_factor = 200
  assert(static_scale.scale_factor == 200, "Static scale should have correct factor")
  
  var oscillate_scale = animation.scale_oscillate(engine)
  assert(oscillate_scale != nil, "scale_oscillate should create animation")
  oscillate_scale.source_animation = source
  oscillate_scale.scale_speed = 100
  assert(oscillate_scale.scale_speed == 100, "Oscillate scale should have correct speed")
  assert(oscillate_scale.scale_mode == 1, "Oscillate scale should have correct mode")
  
  # Test jitter constructors
  var position_jitter = animation.jitter_position(engine)
  assert(position_jitter != nil, "jitter_position should create animation")
  position_jitter.source_animation = source
  position_jitter.jitter_intensity = 120
  assert(position_jitter.jitter_intensity == 120, "Position jitter should have correct intensity")
  assert(position_jitter.jitter_type == 0, "Position jitter should have correct type")
  
  var color_jitter = animation.jitter_color(engine)
  assert(color_jitter != nil, "jitter_color should create animation")
  color_jitter.source_animation = source
  assert(color_jitter.jitter_type == 1, "Color jitter should have correct type")
  
  print("✓ Motion effects constructor functions test passed")
end

# Test string representations
def test_motion_effects_tostring()
  print("Testing motion effects string representations...")
  
  var strip = global.Leds(12)
  var engine = animation.create_engine(strip)
  var source = animation.solid(engine)
  source.color = 0xFFFFFFFF
  
  # Test bounce string representation
  var bounce_anim = animation.bounce_animation(engine)
  bounce_anim.source_animation = source
  bounce_anim.bounce_speed = 75
  bounce_anim.bounce_range = 10
  bounce_anim.damping = 240
  bounce_anim.gravity = 30
  
  var str_repr = str(bounce_anim)
  assert(type(str_repr) == "string", "Bounce string representation should be a string")
  assert(string.find(str_repr, "BounceAnimation") >= 0, "String should contain 'BounceAnimation'")
  
  # Test scale string representation
  var scale_anim = animation.scale_animation(engine)
  scale_anim.source_animation = source
  scale_anim.scale_factor = 150
  scale_anim.scale_speed = 80
  scale_anim.scale_mode = 1
  scale_anim.scale_center = 100
  scale_anim.interpolation = 1
  
  str_repr = str(scale_anim)
  assert(type(str_repr) == "string", "Scale string representation should be a string")
  assert(string.find(str_repr, "ScaleAnimation") >= 0, "String should contain 'ScaleAnimation'")
  assert(string.find(str_repr, "oscillate") >= 0, "String should contain mode name")
  
  # Test jitter string representation
  var jitter_anim = animation.jitter_animation(engine)
  jitter_anim.source_animation = source
  jitter_anim.jitter_intensity = 100
  jitter_anim.jitter_frequency = 60
  jitter_anim.jitter_type = 2
  jitter_anim.position_range = 50
  jitter_anim.color_range = 30
  jitter_anim.brightness_range = 40
  
  str_repr = str(jitter_anim)
  assert(type(str_repr) == "string", "Jitter string representation should be a string")
  assert(string.find(str_repr, "JitterAnimation") >= 0, "String should contain 'JitterAnimation'")
  assert(string.find(str_repr, "brightness") >= 0, "String should contain type name")
  
  print("✓ Motion effects string representation test passed")
end

# Run all tests
def run_motion_effects_tests()
  print("=== Motion Effects Tests ===")
  
  try
    test_bounce_animation_basic()
    test_scale_animation_basic()
    test_jitter_animation_basic()
    test_motion_effects_custom()
    test_motion_effects_update_render()
    test_motion_effects_constructors()
    test_motion_effects_tostring()
    
    print("=== All Motion Effects tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_motion_effects_tests = run_motion_effects_tests

run_motion_effects_tests()

return run_motion_effects_tests