# Test suite for NoiseAnimation
#
# This test verifies that the NoiseAnimation works correctly
# with different parameters and color providers.

import animation
import string

# Test basic NoiseAnimation creation and functionality
def test_noise_animation_basic()
  print("Testing basic NoiseAnimation...")
  
  # Test with default parameters
  var noise_anim = animation.noise_animation(nil, nil, nil, nil, nil, nil, 10, 10, 0, true, "test_noise")
  
  assert(noise_anim != nil, "NoiseAnimation should be created")
  assert(noise_anim.scale == 50, "Default scale should be 50")
  assert(noise_anim.speed == 30, "Default speed should be 30")
  assert(noise_anim.octaves == 1, "Default octaves should be 1")
  assert(noise_anim.strip_length == 10, "Strip length should be 10")
  assert(noise_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic NoiseAnimation test passed")
end

# Test NoiseAnimation with custom parameters
def test_noise_animation_custom()
  print("Testing NoiseAnimation with custom parameters...")
  
  # Test with custom parameters
  var noise_anim = animation.noise_animation(0xFF00FF00, 100, 80, 2, 200, 12345, 20, 15, 5000, false, "custom_noise")
  
  assert(noise_anim.scale == 100, "Custom scale should be 100")
  assert(noise_anim.speed == 80, "Custom speed should be 80")
  assert(noise_anim.octaves == 2, "Custom octaves should be 2")
  assert(noise_anim.persistence == 200, "Custom persistence should be 200")
  assert(noise_anim.seed == 12345, "Custom seed should be 12345")
  assert(noise_anim.strip_length == 20, "Custom strip length should be 20")
  assert(noise_anim.priority == 15, "Custom priority should be 15")
  assert(noise_anim.duration == 5000, "Custom duration should be 5000")
  assert(noise_anim.loop == 0, "Custom loop should be 0 (false)")
  
  print("✓ Custom NoiseAnimation test passed")
end

# Test NoiseAnimation parameter changes
def test_noise_animation_parameters()
  print("Testing NoiseAnimation parameter changes...")
  
  var noise_anim = animation.noise_animation(nil, nil, nil, nil, nil, nil, 15, 10, 0, true, "param_test")
  
  # Test parameter changes
  noise_anim.set_param("scale", 75)
  assert(noise_anim.scale == 75, "Scale should be updated to 75")
  
  noise_anim.set_param("speed", 120)
  assert(noise_anim.speed == 120, "Speed should be updated to 120")
  
  noise_anim.set_param("octaves", 3)
  assert(noise_anim.octaves == 3, "Octaves should be updated to 3")
  
  noise_anim.set_param("strip_length", 25)
  assert(noise_anim.strip_length == 25, "Strip length should be updated to 25")
  assert(noise_anim.current_colors.size() == 25, "Current colors array should be resized")
  
  print("✓ NoiseAnimation parameter test passed")
end

# Test NoiseAnimation update and render
def test_noise_animation_update_render()
  print("Testing NoiseAnimation update and render...")
  
  var noise_anim = animation.noise_animation(0xFFFF0000, 60, 40, 1, 128, nil, 10, 10, 0, true, "update_test")
  var frame = animation.frame_buffer(10)
  
  # Start animation
  noise_anim.start(1000)
  assert(noise_anim.is_running == true, "Animation should be running after start")
  
  # Test update
  var result = noise_anim.update(1500)
  assert(result == true, "Update should return true for running animation")
  
  # Test render
  result = noise_anim.render(frame, 1500)
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
  
  print("✓ NoiseAnimation update/render test passed")
end

# Test global constructor functions
def test_noise_constructors()
  print("Testing noise constructor functions...")
  
  # Test noise_rainbow
  var rainbow_noise = animation.noise_rainbow(80, 60, 15, 12)
  assert(rainbow_noise != nil, "noise_rainbow should create animation")
  assert(rainbow_noise.scale == 80, "Rainbow noise should have correct scale")
  assert(rainbow_noise.speed == 60, "Rainbow noise should have correct speed")
  assert(rainbow_noise.strip_length == 15, "Rainbow noise should have correct strip length")
  assert(rainbow_noise.priority == 12, "Rainbow noise should have correct priority")
  
  # Test noise_single_color
  var single_noise = animation.noise_single_color(0xFF00FFFF, 90, 70, 20, 8)
  assert(single_noise != nil, "noise_single_color should create animation")
  assert(single_noise.scale == 90, "Single color noise should have correct scale")
  assert(single_noise.speed == 70, "Single color noise should have correct speed")
  
  # Test noise_fractal
  var fractal_noise = animation.noise_fractal(0xFFFFFF00, 40, 30, 3, 25, 15)
  assert(fractal_noise != nil, "noise_fractal should create animation")
  assert(fractal_noise.scale == 40, "Fractal noise should have correct scale")
  assert(fractal_noise.octaves == 3, "Fractal noise should have correct octaves")
  
  print("✓ Noise constructor functions test passed")
end

# Test NoiseAnimation string representation
def test_noise_tostring()
  print("Testing NoiseAnimation string representation...")
  
  var noise_anim = animation.noise_animation(nil, 75, 45, 2, 150, nil, 12, 10, 0, true, "string_test")
  var str_repr = str(noise_anim)
  
  assert(type(str_repr) == "string", "String representation should be a string")
  assert(string.find(str_repr, "NoiseAnimation") >= 0, "String should contain 'NoiseAnimation'")
  assert(string.find(str_repr, "75") >= 0, "String should contain scale value")
  assert(string.find(str_repr, "45") >= 0, "String should contain speed value")
  
  print("✓ NoiseAnimation string representation test passed")
end

# Run all tests
def run_noise_animation_tests()
  print("=== NoiseAnimation Tests ===")
  
  try
    test_noise_animation_basic()
    test_noise_animation_custom()
    test_noise_animation_parameters()
    test_noise_animation_update_render()
    test_noise_constructors()
    test_noise_tostring()
    
    print("=== All NoiseAnimation tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_noise_animation_tests = run_noise_animation_tests

run_noise_animation_tests()

return run_noise_animation_tests