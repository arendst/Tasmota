# Test suite for WaveAnimation
#
# This test verifies that the WaveAnimation works correctly
# with different parameters and color providers.

import animation
import string

# Test basic WaveAnimation creation and functionality
def test_wave_animation_basic()
  print("Testing basic WaveAnimation...")
  
  # Create engine and animation
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var wave_anim = animation.wave_animation(engine)
  
  assert(wave_anim != nil, "WaveAnimation should be created")
  assert(wave_anim.back_color == 0xFF000000, "Default background should be black")
  assert(wave_anim.wave_type == 0, "Default wave_type should be 0 (sine)")
  assert(wave_anim.amplitude == 128, "Default amplitude should be 128")
  assert(wave_anim.frequency == 32, "Default frequency should be 32")
  assert(wave_anim.phase == 0, "Default phase should be 0")
  assert(wave_anim.wave_speed == 50, "Default wave_speed should be 50")
  assert(wave_anim.center_level == 128, "Default center_level should be 128")
  assert(wave_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic WaveAnimation test passed")
end

# Test WaveAnimation with custom parameters
def test_wave_animation_custom()
  print("Testing WaveAnimation with custom parameters...")
  
  # Create engine and animation with custom parameters
  var strip = global.Leds(20)
  var engine = animation.create_engine(strip)
  var wave_anim = animation.wave_animation(engine)
  
  # Set custom parameters using virtual member access
  wave_anim.color = 0xFF00FF00
  wave_anim.back_color = 0xFF111111
  wave_anim.wave_type = 2
  wave_anim.amplitude = 200
  wave_anim.frequency = 60
  wave_anim.phase = 45
  wave_anim.wave_speed = 80
  wave_anim.center_level = 100
  wave_anim.priority = 15
  wave_anim.duration = 5000
  wave_anim.loop = false
  
  assert(wave_anim.back_color == 0xFF111111, "Custom background should be set")
  assert(wave_anim.wave_type == 2, "Custom wave_type should be 2")
  assert(wave_anim.amplitude == 200, "Custom amplitude should be 200")
  assert(wave_anim.frequency == 60, "Custom frequency should be 60")
  assert(wave_anim.phase == 45, "Custom phase should be 45")
  assert(wave_anim.wave_speed == 80, "Custom wave_speed should be 80")
  assert(wave_anim.center_level == 100, "Custom center_level should be 100")
  assert(wave_anim.priority == 15, "Custom priority should be 15")
  assert(wave_anim.duration == 5000, "Custom duration should be 5000")
  assert(wave_anim.loop == false, "Custom loop should be false")
  
  print("✓ Custom WaveAnimation test passed")
end

# Test WaveAnimation parameter changes
def test_wave_animation_parameters()
  print("Testing WaveAnimation parameter changes...")
  
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  var wave_anim = animation.wave_animation(engine)
  
  # Test parameter changes using virtual member access
  wave_anim.wave_type = 1
  assert(wave_anim.wave_type == 1, "Wave type should be updated to 1")
  
  wave_anim.amplitude = 180
  assert(wave_anim.amplitude == 180, "Amplitude should be updated to 180")
  
  wave_anim.frequency = 75
  assert(wave_anim.frequency == 75, "Frequency should be updated to 75")
  
  wave_anim.wave_speed = 120
  assert(wave_anim.wave_speed == 120, "Wave speed should be updated to 120")
  
  wave_anim.back_color = 0xFF222222
  assert(wave_anim.back_color == 0xFF222222, "Background color should be updated")
  
  print("✓ WaveAnimation parameter test passed")
end

# Test WaveAnimation update and render
def test_wave_animation_update_render()
  print("Testing WaveAnimation update and render...")
  
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var wave_anim = animation.wave_animation(engine)
  
  # Set parameters
  wave_anim.color = 0xFFFF0000
  wave_anim.back_color = 0xFF000000
  wave_anim.wave_type = 0
  wave_anim.amplitude = 150
  wave_anim.frequency = 40
  wave_anim.wave_speed = 60
  
  var frame = animation.frame_buffer(10)
  
  # Start animation
  wave_anim.start(1000)
  assert(wave_anim.is_running == true, "Animation should be running after start")
  
  # Test update
  var result = wave_anim.update(1500)
  assert(result == true, "Update should return true for running animation")
  
  # Test render
  result = wave_anim.render(frame, 1500)
  assert(result == true, "Render should return true for running animation")
  
  # Check that colors were set (should not all be black with high amplitude)
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
  
  print("✓ WaveAnimation update/render test passed")
end

# Test different wave types
def test_wave_types()
  print("Testing different wave types...")
  
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  var frame = animation.frame_buffer(10)
  
  # Test each wave type
  var wave_types = [0, 1, 2, 3]  # sine, triangle, square, sawtooth
  var i = 0
  while i < 4
    var wave_anim = animation.wave_animation(engine)
    wave_anim.color = 0xFFFF0000
    wave_anim.back_color = 0xFF000000
    wave_anim.wave_type = wave_types[i]
    wave_anim.amplitude = 200
    wave_anim.frequency = 50
    wave_anim.wave_speed = 0  # No movement for testing
    
    wave_anim.start(1000)
    wave_anim.update(1000)
    var result = wave_anim.render(frame, 1000)
    assert(result == true, f"Wave type {wave_types[i]} should render successfully")
    
    i += 1
  end
  
  print("✓ Wave types test passed")
end

# Test global constructor functions
def test_wave_constructors()
  print("Testing wave constructor functions...")
  
  var strip = global.Leds(30)
  var engine = animation.create_engine(strip)
  
  # Test wave_rainbow_sine
  var rainbow_wave = animation.wave_rainbow_sine(engine)
  assert(rainbow_wave != nil, "wave_rainbow_sine should create animation")
  assert(rainbow_wave.frequency == 32, "Rainbow wave should have default frequency")
  assert(rainbow_wave.wave_speed == 50, "Rainbow wave should have default wave_speed")
  assert(rainbow_wave.wave_type == 0, "Rainbow wave should be sine type")
  
  # Test wave_single_sine
  var single_wave = animation.wave_single_sine(engine)
  assert(single_wave != nil, "wave_single_sine should create animation")
  assert(single_wave.frequency == 32, "Single wave should have default frequency")
  assert(single_wave.wave_speed == 50, "Single wave should have default wave_speed")
  assert(single_wave.wave_type == 0, "Single wave should be sine type")
  
  # Test wave_custom
  var custom_wave = animation.wave_custom(engine)
  assert(custom_wave != nil, "wave_custom should create animation")
  assert(custom_wave.wave_type == 2, "Custom wave should have square wave type")
  assert(custom_wave.frequency == 40, "Custom wave should have correct frequency")
  assert(custom_wave.wave_speed == 30, "Custom wave should have correct wave_speed")
  
  print("✓ Wave constructor functions test passed")
end

# Test WaveAnimation string representation
def test_wave_tostring()
  print("Testing WaveAnimation string representation...")
  
  var strip = global.Leds(12)
  var engine = animation.create_engine(strip)
  var wave_anim = animation.wave_animation(engine)
  
  # Set parameters
  wave_anim.wave_type = 1
  wave_anim.amplitude = 150
  wave_anim.frequency = 75
  wave_anim.wave_speed = 45
  wave_anim.center_level = 100
  
  var str_repr = str(wave_anim)
  
  assert(type(str_repr) == "string", "String representation should be a string")
  assert(string.find(str_repr, "WaveAnimation") >= 0, "String should contain 'WaveAnimation'")
  assert(string.find(str_repr, "triangle") >= 0, "String should contain wave type name")
  assert(string.find(str_repr, "75") >= 0, "String should contain frequency value")
  assert(string.find(str_repr, "45") >= 0, "String should contain wave_speed value")
  
  print("✓ WaveAnimation string representation test passed")
end

# Run all tests
def run_wave_animation_tests()
  print("=== WaveAnimation Tests ===")
  
  try
    test_wave_animation_basic()
    test_wave_animation_custom()
    test_wave_animation_parameters()
    test_wave_animation_update_render()
    test_wave_types()
    test_wave_constructors()
    test_wave_tostring()
    
    print("=== All WaveAnimation tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_wave_animation_tests = run_wave_animation_tests

run_wave_animation_tests()

return run_wave_animation_tests