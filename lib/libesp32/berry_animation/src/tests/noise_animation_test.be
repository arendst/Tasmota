# Test suite for NoiseAnimation
#
# This test verifies that the NoiseAnimation works correctly
# with different parameters and color providers.

import animation
import string

# Test basic NoiseAnimation creation and functionality
def test_noise_animation_basic()
  print("Testing basic NoiseAnimation...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  # Test with default parameters
  var noise_anim = animation.noise_animation(engine)
  
  assert(noise_anim != nil, "NoiseAnimation should be created")
  assert(noise_anim.scale == 50, "Default scale should be 50")
  assert(noise_anim.speed == 30, "Default speed should be 30")
  assert(noise_anim.octaves == 1, "Default octaves should be 1")
  assert(noise_anim.is_running == false, "Animation should not be running initially")
  
  print("✓ Basic NoiseAnimation test passed")
end

# Test NoiseAnimation with custom parameters
def test_noise_animation_custom()
  print("Testing NoiseAnimation with custom parameters...")
  
  # Create LED strip and engine
  var strip = global.Leds(20)
  var engine = animation.create_engine(strip)
  
  # Test with custom parameters
  var noise_anim = animation.noise_animation(engine)
  noise_anim.color = 0xFF00FF00
  noise_anim.scale = 100
  noise_anim.speed = 80
  noise_anim.octaves = 2
  noise_anim.persistence = 200
  noise_anim.seed = 12345
  noise_anim.priority = 15
  noise_anim.duration = 5000
  noise_anim.loop = false
  
  assert(noise_anim.scale == 100, "Custom scale should be 100")
  assert(noise_anim.speed == 80, "Custom speed should be 80")
  assert(noise_anim.octaves == 2, "Custom octaves should be 2")
  assert(noise_anim.persistence == 200, "Custom persistence should be 200")
  assert(noise_anim.seed == 12345, "Custom seed should be 12345")
  assert(noise_anim.priority == 15, "Custom priority should be 15")
  assert(noise_anim.duration == 5000, "Custom duration should be 5000")
  assert(noise_anim.loop == false, "Custom loop should be false")
  
  print("✓ Custom NoiseAnimation test passed")
end

# Test NoiseAnimation parameter changes
def test_noise_animation_parameters()
  print("Testing NoiseAnimation parameter changes...")
  
  # Create LED strip and engine
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  var noise_anim = animation.noise_animation(engine)
  
  # Test parameter changes via virtual member assignment
  noise_anim.scale = 75
  assert(noise_anim.scale == 75, "Scale should be updated to 75")
  
  noise_anim.speed = 120
  assert(noise_anim.speed == 120, "Speed should be updated to 120")
  
  noise_anim.octaves = 3
  assert(noise_anim.octaves == 3, "Octaves should be updated to 3")
  
  # Test that current_colors array adapts to engine strip length
  var initial_size = size(noise_anim.current_colors)
  assert(initial_size == 15, "Current colors array should match engine strip length")
  
  print("✓ NoiseAnimation parameter test passed")
end

# Test NoiseAnimation update and render
def test_noise_animation_update_render()
  print("Testing NoiseAnimation update and render...")
  
  # Create LED strip and engine
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var noise_anim = animation.noise_animation(engine)
  noise_anim.color = 0xFFFF0000
  noise_anim.scale = 60
  noise_anim.speed = 40
  
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
  
  # Create LED strip and engine
  var strip = global.Leds(15)
  var engine = animation.create_engine(strip)
  
  # Test noise_rainbow
  var rainbow_noise = animation.noise_rainbow(engine)
  assert(rainbow_noise != nil, "noise_rainbow should create animation")
  assert(rainbow_noise.scale == 50, "Rainbow noise should have correct scale")
  assert(rainbow_noise.speed == 30, "Rainbow noise should have correct speed")
  assert(rainbow_noise.octaves == 1, "Rainbow noise should have correct octaves")
  
  # Test noise_single_color
  var single_noise = animation.noise_single_color(engine)
  assert(single_noise != nil, "noise_single_color should create animation")
  assert(single_noise.scale == 50, "Single color noise should have correct scale")
  assert(single_noise.speed == 30, "Single color noise should have correct speed")
  assert(single_noise.color == 0xFFFFFFFF, "Single color noise should have white color")
  
  # Test noise_fractal
  var fractal_noise = animation.noise_fractal(engine)
  assert(fractal_noise != nil, "noise_fractal should create animation")
  assert(fractal_noise.scale == 30, "Fractal noise should have correct scale")
  assert(fractal_noise.octaves == 3, "Fractal noise should have correct octaves")
  
  print("✓ Noise constructor functions test passed")
end

# Test NoiseAnimation string representation
def test_noise_tostring()
  print("Testing NoiseAnimation string representation...")
  
  # Create LED strip and engine
  var strip = global.Leds(12)
  var engine = animation.create_engine(strip)
  
  var noise_anim = animation.noise_animation(engine)
  noise_anim.scale = 75
  noise_anim.speed = 45
  noise_anim.octaves = 2
  noise_anim.persistence = 150
  
  var str_repr = str(noise_anim)
  
  assert(type(str_repr) == "string", "String representation should be a string")
  assert(string.find(str_repr, "NoiseAnimation") >= 0, "String should contain 'NoiseAnimation'")
  assert(string.find(str_repr, "75") >= 0, "String should contain scale value")
  assert(string.find(str_repr, "45") >= 0, "String should contain speed value")
  
  print("✓ NoiseAnimation string representation test passed")
end

# Test integer color conversion to gradient
def test_noise_integer_color_conversion()
  print("Testing NoiseAnimation integer color conversion...")
  
  # Create LED strip and engine
  var strip = global.Leds(5)
  var engine = animation.create_engine(strip)
  
  var noise_anim = animation.noise_animation(engine)
  
  # Set an integer color - should be converted to gradient provider
  noise_anim.color = 0xFFFF0000  # Red
  
  # Check the raw parameter value (should be a color provider)
  var raw_color = noise_anim.get_param("color")
  
  # Test that the raw parameter is a color provider (the conversion worked)
  assert(animation.is_color_provider(raw_color), "Integer color should be converted to color provider")
  
  print("✓ NoiseAnimation integer color conversion test passed")
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
    test_noise_integer_color_conversion()
    
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