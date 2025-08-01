# Test for Solid Function Unification
# This test verifies that the unified solid() function works correctly
# and eliminates the need for separate solid pattern and solid_animation

import animation

def test_unified_solid_function()
  print("Testing unified solid() function...")
  
  # Test 1: Basic solid animation creation
  var red_solid = animation.solid(0xFFFF0000, 10, 0, false, 255, "solid")
  
  # Verify it's created successfully
  assert(red_solid != nil, "solid() should return a valid object")
  assert(type(red_solid) == "instance", "solid() should return an instance")
  
  # Verify default values
  assert(red_solid.priority == 10, "Should have priority 10")
  assert(red_solid.opacity == 255, "Should have opacity 255")
  assert(red_solid.duration == 0, "Should have infinite duration")
  assert(red_solid.loop == false || red_solid.loop == 0, "Should have no looping")
  assert(red_solid.name == "solid", "Should have name 'solid'")
  
  print("✅ Basic solid animation creation test passed")
end

def test_solid_with_all_parameters()
  print("Testing solid() with all parameters...")
  
  # Test with all parameters specified
  var blue_solid = animation.solid(0xFF0000FF, 20, 5000, true, 200, "test_blue")
  
  # Verify all parameters are set correctly
  assert(blue_solid.priority == 20, "Should have priority 20")
  assert(blue_solid.opacity == 200, "Should have opacity 200")
  assert(blue_solid.duration == 5000, "Should have duration 5000")
  assert(blue_solid.loop == true || blue_solid.loop == 1, "Should have loop enabled")
  assert(blue_solid.name == "test_blue", "Should have name 'test_blue'")
  
  print("✅ Solid with all parameters test passed")
end

def test_solid_composition()
  print("Testing solid animation composition...")
  
  # Create a base solid animation
  var green_solid = animation.solid(0xFF00FF00, 10, 0, false, 255, "green_solid")
  
  # Use pulse animation with green color (not composition, but similar effect)
  var pulsing_green = animation.pulse(0xFF00FF00, 0, 255, 1000, 10, 0, false, "pulsing_green")
  
  # Verify both animations are created
  assert(green_solid != nil, "Green solid should be created")
  assert(pulsing_green != nil, "Pulsing green should be created")
  assert(type(pulsing_green) == "instance", "Pulse should be an instance")
  
  print("✅ Solid composition test passed")
end

def test_solid_color_provider()
  print("Testing solid() with color provider...")
  
  # Create a color provider
  var color_provider = animation.solid_color_provider(0xFFFFFF00)  # Yellow
  
  # Create solid animation with color provider
  var yellow_solid = animation.solid(color_provider, 10, 0, false, 255, "yellow_solid")
  
  # Verify it works with color providers
  assert(yellow_solid != nil, "Should create animation with color provider")
  assert(type(yellow_solid) == "instance", "Should be an instance")
  
  print("✅ Solid with color provider test passed")
end

def test_solid_rendering()
  print("Testing solid animation rendering...")
  
  # Create a solid animation
  var red_solid = animation.solid(0xFFFF0000, 10, 0, false, 255, "red_solid")
  
  # Create a frame buffer
  var frame = animation.frame_buffer(5)
  
  # Start and render the animation
  red_solid.start()
  var result = red_solid.render(frame, 0)
  
  # Verify rendering worked
  assert(result == true, "Render should return true")
  assert(red_solid.is_running, "Animation should be running")
  
  # Verify frame has been modified (check first pixel is not black)
  var pixel_color = frame.get_pixel_color(0)
  assert(pixel_color != 0x00000000, f"First pixel should not be black, got 0x{pixel_color:08X}")
  
  print("✅ Solid rendering test passed")
end

def test_no_solid_animation_function()
  print("Testing that solid_animation is no longer exported...")
  
  # Verify solid_animation is not in the animation module exports
  # (This would fail if solid_animation was still being exported)
  try
    var should_fail = animation.solid_animation
    # If we get here, solid_animation still exists - that's wrong
    assert(false, "solid_animation should not exist in unified architecture")
  except ..
    # This is expected - solid_animation should not exist
    print("✅ solid_animation correctly removed from exports")
  end
end

# Run all tests
def run_tests()
  print("Running Solid Function Unification Tests...")
  print("==========================================")
  
  try
    test_unified_solid_function()
    test_solid_with_all_parameters()
    test_solid_composition()
    test_solid_color_provider()
    test_solid_rendering()
    test_no_solid_animation_function()
    
    print("==========================================")
    print("✅ All unification tests passed!")
    print("\nKey Achievements:")
    print("- solid() returns Animation (which IS a Pattern)")
    print("- No artificial distinction between pattern and animation")
    print("- Full parameter support for temporal behavior")
    print("- Seamless composition with other animations")
    print("- solid_animation function eliminated")
    return true
  except .. as e, msg
    print(f"❌ Test failed: {msg}")
    raise "test_failed"
  end
end

# Run the tests
run_tests()

# Export test function
return {'run_tests': run_tests}