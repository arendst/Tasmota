# Test suite for JitterAnimation
#
# This test verifies that the JitterAnimation works correctly
# with different jitter types and parameters.

import animation
import string

# Test basic JitterAnimation creation and functionality
def test_jitter_animation_basic()
  print("Testing basic JitterAnimation...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Create a simple source animation
  var source = animation.solid(engine)
  source.color = 0xFFFF0000
  
  # Test with default parameters
  var jitter_anim = animation.jitter_animation(engine)
  jitter_anim.source_animation = source
  
  assert(jitter_anim != nil, "JitterAnimation should be created")
  assert(jitter_anim.jitter_intensity == 100, "Default jitter_intensity should be 100")
  assert(jitter_anim.jitter_frequency == 60, "Default jitter_frequency should be 60")
  assert(jitter_anim.jitter_type == 0, "Default jitter_type should be 0")
  assert(jitter_anim.position_range == 50, "Default position_range should be 50")
  assert(jitter_anim.color_range == 30, "Default color_range should be 30")
  assert(jitter_anim.brightness_range == 40, "Default brightness_range should be 40")
  
  print("✓ Basic JitterAnimation test passed")
end

# Test JitterAnimation with custom parameters
def test_jitter_animation_custom()
  print("Testing JitterAnimation with custom parameters...")
  
  # Create LED strip and engine
  var strip = global.Leds(20)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF00FF00
  
  # Test with custom parameters
  var jitter_anim = animation.jitter_animation(engine)
  jitter_anim.source_animation = source
  jitter_anim.jitter_intensity = 150
  jitter_anim.jitter_frequency = 120
  jitter_anim.jitter_type = 2
  jitter_anim.position_range = 80
  jitter_anim.color_range = 60
  jitter_anim.brightness_range = 70
  
  assert(jitter_anim.jitter_intensity == 150, "Custom jitter_intensity should be 150")
  assert(jitter_anim.jitter_frequency == 120, "Custom jitter_frequency should be 120")
  assert(jitter_anim.jitter_type == 2, "Custom jitter_type should be 2")
  assert(jitter_anim.position_range == 80, "Custom position_range should be 80")
  assert(jitter_anim.color_range == 60, "Custom color_range should be 60")
  assert(jitter_anim.brightness_range == 70, "Custom brightness_range should be 70")
  
  print("✓ Custom JitterAnimation test passed")
end

# Test JitterAnimation parameter changes
def test_jitter_animation_parameters()
  print("Testing JitterAnimation parameter changes...")
  
  # Create LED strip and engine
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF0000FF
  
  var jitter_anim = animation.jitter_animation(engine)
  jitter_anim.source_animation = source
  
  # Test parameter changes using virtual member assignment
  jitter_anim.jitter_intensity = 180
  assert(jitter_anim.jitter_intensity == 180, "Jitter intensity should be updated to 180")
  
  jitter_anim.jitter_frequency = 100
  assert(jitter_anim.jitter_frequency == 100, "Jitter frequency should be updated to 100")
  
  jitter_anim.jitter_type = 3
  assert(jitter_anim.jitter_type == 3, "Jitter type should be updated to 3")
  
  jitter_anim.position_range = 80
  assert(jitter_anim.position_range == 80, "Position range should be updated to 80")
  
  jitter_anim.color_range = 50
  assert(jitter_anim.color_range == 50, "Color range should be updated to 50")
  
  jitter_anim.brightness_range = 60
  assert(jitter_anim.brightness_range == 60, "Brightness range should be updated to 60")
  
  # Test that arrays are properly sized based on engine strip length
  assert(jitter_anim.current_colors.size() == 15, "Current colors array should match strip length")
  assert(jitter_anim.jitter_offsets.size() == 15, "Jitter offsets array should match strip length")
  
  print("✓ JitterAnimation parameter test passed")
end

# Test JitterAnimation jitter types
def test_jitter_animation_types()
  print("Testing JitterAnimation jitter types...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFFFFFF00
  
  # Test position jitter (type 0)
  var position_jitter = animation.jitter_position(engine)
  position_jitter.source_animation = source
  assert(position_jitter.jitter_type == 0, "Position jitter should have type 0")
  
  # Test color jitter (type 1)
  var color_jitter = animation.jitter_color(engine)
  color_jitter.source_animation = source
  assert(color_jitter.jitter_type == 1, "Color jitter should have type 1")
  
  # Test brightness jitter (type 2)
  var brightness_jitter = animation.jitter_brightness(engine)
  brightness_jitter.source_animation = source
  assert(brightness_jitter.jitter_type == 2, "Brightness jitter should have type 2")
  
  # Test all jitter (type 3)
  var all_jitter = animation.jitter_all(engine)
  all_jitter.source_animation = source
  assert(all_jitter.jitter_type == 3, "All jitter should have type 3")
  
  print("✓ JitterAnimation types test passed")
end

# Test JitterAnimation update and render
def test_jitter_animation_update_render()
  print("Testing JitterAnimation update and render...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFFFF00FF
  
  var jitter_anim = animation.jitter_animation(engine)
  jitter_anim.source_animation = source
  
  var frame = animation.frame_buffer(10)
  
  # Start animation
  jitter_anim.start(1000)
  
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
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF00FFFF
  
  var jitter_anim = animation.jitter_animation(engine)
  jitter_anim.source_animation = source
  
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
  
  # Create LED strip and engine
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFFAAAAAA
  
  # Test jitter_position
  var position_jitter = animation.jitter_position(engine)
  position_jitter.source_animation = source
  position_jitter.jitter_intensity = 120
  position_jitter.jitter_frequency = 80
  assert(position_jitter != nil, "jitter_position should create animation")
  assert(position_jitter.jitter_intensity == 120, "Position jitter should have correct intensity")
  assert(position_jitter.jitter_frequency == 80, "Position jitter should have correct frequency")
  assert(position_jitter.jitter_type == 0, "Position jitter should have type 0")
  
  # Test jitter_color
  var color_jitter = animation.jitter_color(engine)
  color_jitter.source_animation = source
  color_jitter.jitter_intensity = 100
  color_jitter.jitter_frequency = 60
  assert(color_jitter != nil, "jitter_color should create animation")
  assert(color_jitter.jitter_intensity == 100, "Color jitter should have correct intensity")
  assert(color_jitter.jitter_type == 1, "Color jitter should have type 1")
  
  # Test jitter_brightness
  var brightness_jitter = animation.jitter_brightness(engine)
  brightness_jitter.source_animation = source
  brightness_jitter.jitter_intensity = 80
  brightness_jitter.jitter_frequency = 40
  assert(brightness_jitter != nil, "jitter_brightness should create animation")
  assert(brightness_jitter.jitter_intensity == 80, "Brightness jitter should have correct intensity")
  assert(brightness_jitter.jitter_type == 2, "Brightness jitter should have type 2")
  
  # Test jitter_all
  var all_jitter = animation.jitter_all(engine)
  all_jitter.source_animation = source
  all_jitter.jitter_intensity = 150
  all_jitter.jitter_frequency = 100
  assert(all_jitter != nil, "jitter_all should create animation")
  assert(all_jitter.jitter_intensity == 150, "All jitter should have correct intensity")
  assert(all_jitter.jitter_type == 3, "All jitter should have type 3")
  
  print("✓ Jitter constructor functions test passed")
end

# Test JitterAnimation color jitter effects
def test_jitter_animation_color_effects()
  print("Testing JitterAnimation color effects...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF808080
  
  var jitter_anim = animation.jitter_animation(engine)
  jitter_anim.source_animation = source
  jitter_anim.jitter_type = 1
  jitter_anim.color_range = 50
  
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
  
  # Create LED strip and engine
  var strip = global.Leds(12)
  var engine = animation.create_engine(strip)
  
  var source = animation.solid(engine)
  source.color = 0xFF666666
  
  var jitter_anim = animation.jitter_animation(engine)
  jitter_anim.source_animation = source
  jitter_anim.jitter_type = 2
  jitter_anim.jitter_intensity = 100
  jitter_anim.jitter_frequency = 60
  
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