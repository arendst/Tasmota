# Test suite for ScaleAnimation
#
# This test verifies that the ScaleAnimation works correctly
# with different scale modes and parameters.

import animation
import string

# Test basic ScaleAnimation creation and functionality
def test_scale_animation_basic()
  print("Testing basic ScaleAnimation...")
  
  # Create LED strip and engine using global.Leds
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Create a simple source animation
  var source = animation.solid(engine)
  source.color = 0xFFFF0000
  
  # Test with default parameters
  var scale_anim = animation.scale_animation(engine)
  scale_anim.source_animation = source
  
  assert(scale_anim != nil, "ScaleAnimation should be created")
  assert(scale_anim.scale_factor == 128, "Default scale_factor should be 128")
  assert(scale_anim.scale_speed == 0, "Default scale_speed should be 0")
  assert(scale_anim.scale_mode == 0, "Default scale_mode should be 0")
  assert(scale_anim.scale_center == 128, "Default scale_center should be 128")
  assert(scale_anim.interpolation == 1, "Default interpolation should be 1")
  
  print("✓ Basic ScaleAnimation test passed")
end

# Test ScaleAnimation with custom parameters
def test_scale_animation_custom()
  print("Testing ScaleAnimation with custom parameters...")
  
  # Create LED strip and engine
  var strip = global.Leds(20)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF00FF00
  
  # Test with custom parameters
  var scale_anim = animation.scale_animation(engine)
  scale_anim.source_animation = source
  scale_anim.scale_factor = 200
  scale_anim.scale_speed = 80
  scale_anim.scale_mode = 1
  scale_anim.scale_center = 100
  scale_anim.interpolation = 0
  
  assert(scale_anim.scale_factor == 200, "Custom scale_factor should be 200")
  assert(scale_anim.scale_speed == 80, "Custom scale_speed should be 80")
  assert(scale_anim.scale_mode == 1, "Custom scale_mode should be 1")
  assert(scale_anim.scale_center == 100, "Custom scale_center should be 100")
  assert(scale_anim.interpolation == 0, "Custom interpolation should be 0")
  
  print("✓ Custom ScaleAnimation test passed")
end

# Test ScaleAnimation parameter changes
def test_scale_animation_parameters()
  print("Testing ScaleAnimation parameter changes...")
  
  # Create LED strip and engine
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF0000FF
  
  var scale_anim = animation.scale_animation(engine)
  scale_anim.source_animation = source
  
  # Test parameter changes using virtual member assignment
  scale_anim.scale_factor = 180
  assert(scale_anim.scale_factor == 180, "Scale factor should be updated to 180")
  
  scale_anim.scale_speed = 100
  assert(scale_anim.scale_speed == 100, "Scale speed should be updated to 100")
  
  scale_anim.scale_mode = 2
  assert(scale_anim.scale_mode == 2, "Scale mode should be updated to 2")
  
  scale_anim.scale_center = 200
  assert(scale_anim.scale_center == 200, "Scale center should be updated to 200")
  
  scale_anim.interpolation = 0
  assert(scale_anim.interpolation == 0, "Interpolation should be updated to 0")
  
  # Strip length is now managed by engine, test that buffers are properly sized
  var current_strip_length = engine.get_strip_length()
  assert(scale_anim.current_colors.size() == current_strip_length, "Current colors array should match engine strip length")
  
  print("✓ ScaleAnimation parameter test passed")
end

# Test ScaleAnimation scale modes
def test_scale_animation_modes()
  print("Testing ScaleAnimation scale modes...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFFFFFF00
  
  # Test static mode (0)
  var static_scale = animation.scale_animation(engine)
  static_scale.source_animation = source
  static_scale.scale_factor = 150
  static_scale.scale_mode = 0
  assert(static_scale.scale_mode == 0, "Static scale should have mode 0")
  var static_factor = static_scale._get_current_scale_factor()
  assert(static_factor == 150, "Static mode should return set scale factor")
  
  # Test oscillate mode (1)
  var oscillate_scale = animation.scale_animation(engine)
  oscillate_scale.source_animation = source
  oscillate_scale.scale_speed = 60
  oscillate_scale.scale_mode = 1
  assert(oscillate_scale.scale_mode == 1, "Oscillate scale should have mode 1")
  # For oscillate mode, the factor will vary based on phase
  var oscillate_factor = oscillate_scale._get_current_scale_factor()
  assert(type(oscillate_factor) == "int", "Oscillate mode should return integer factor")
  
  # Test grow mode (2)
  var grow_scale = animation.scale_animation(engine)
  grow_scale.source_animation = source
  grow_scale.scale_speed = 60
  grow_scale.scale_mode = 2
  assert(grow_scale.scale_mode == 2, "Grow scale should have mode 2")
  var grow_factor = grow_scale._get_current_scale_factor()
  assert(type(grow_factor) == "int", "Grow mode should return integer factor")
  
  # Test shrink mode (3)
  var shrink_scale = animation.scale_animation(engine)
  shrink_scale.source_animation = source
  shrink_scale.scale_speed = 60
  shrink_scale.scale_mode = 3
  assert(shrink_scale.scale_mode == 3, "Shrink scale should have mode 3")
  var shrink_factor = shrink_scale._get_current_scale_factor()
  assert(type(shrink_factor) == "int", "Shrink mode should return integer factor")
  
  print("✓ ScaleAnimation modes test passed")
end

# Test ScaleAnimation interpolation
def test_scale_animation_interpolation()
  print("Testing ScaleAnimation interpolation...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF808080
  
  var scale_anim = animation.scale_animation(engine)
  scale_anim.source_animation = source
  
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
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF000000
  
  var scale_anim = animation.scale_animation(engine)
  scale_anim.source_animation = source
  
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
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFFFF00FF
  
  var scale_anim = animation.scale_animation(engine)
  scale_anim.source_animation = source
  scale_anim.scale_factor = 150
  
  var frame = animation.frame_buffer(10)
  
  # Start animation
  scale_anim.start(1000)
  
  # Test update
  var result = scale_anim.update(1500)
  assert(result == true, "Update should return true for running animation")
  
  # Test render
  result = scale_anim.render(frame, 1500)
  assert(result == true, "Render should return true for running animation")
  
  # Check that colors were calculated
  var current_strip_length = engine.get_strip_length()
  assert(scale_anim.current_colors.size() == current_strip_length, "Current colors should match strip length")
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
  
  # Create LED strip and engine
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF00FFFF
  
  # Test scale_static
  var static_scale = animation.scale_static(engine)
  static_scale.source_animation = source
  static_scale.scale_factor = 200
  assert(static_scale != nil, "scale_static should create animation")
  assert(static_scale.scale_factor == 200, "Static scale should have correct factor")
  assert(static_scale.scale_speed == 0, "Static scale should have no animation")
  assert(static_scale.scale_mode == 0, "Static scale should have mode 0")
  
  # Test scale_oscillate
  var oscillate_scale = animation.scale_oscillate(engine)
  oscillate_scale.source_animation = source
  assert(oscillate_scale != nil, "scale_oscillate should create animation")
  assert(oscillate_scale.scale_speed == 128, "Oscillate scale should have medium speed")
  assert(oscillate_scale.scale_mode == 1, "Oscillate scale should have mode 1")
  assert(oscillate_scale.interpolation == 1, "Oscillate scale should use linear interpolation")
  
  # Test scale_grow
  var grow_scale = animation.scale_grow(engine)
  grow_scale.source_animation = source
  grow_scale.scale_speed = 80
  assert(grow_scale != nil, "scale_grow should create animation")
  assert(grow_scale.scale_speed == 80, "Grow scale should have correct speed")
  assert(grow_scale.scale_mode == 2, "Grow scale should have mode 2")
  
  print("✓ Scale constructor functions test passed")
end

# Test ScaleAnimation string representation
def test_scale_tostring()
  print("Testing ScaleAnimation string representation...")
  
  # Create LED strip and engine
  var strip = global.Leds(12)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF444444
  
  var scale_anim = animation.scale_animation(engine)
  scale_anim.source_animation = source
  scale_anim.scale_factor = 150
  scale_anim.scale_speed = 80
  scale_anim.scale_mode = 1
  scale_anim.scale_center = 100
  
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