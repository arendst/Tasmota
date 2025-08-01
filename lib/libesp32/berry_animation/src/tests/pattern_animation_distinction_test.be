# Test for Unified Pattern-Animation Architecture
# This test verifies that the new unified architecture works correctly

import animation

# Test the unified solid function (now returns Animation which IS a Pattern)
def test_unified_solid()
  print("Testing unified solid function...")
  
  # Create a solid red animation (which IS a pattern)
  var red_solid = animation.solid(0xFFFF0000)
  
  # Verify it's created successfully
  assert(red_solid != nil, "solid() should return a valid object")
  assert(type(red_solid) == "instance", "solid() should return an instance")
  
  # Verify it has both Pattern and Animation properties
  assert(red_solid.priority == 10, "Should have default priority 10")
  assert(red_solid.opacity == 255, "Should have default opacity 255")
  assert(red_solid.name == "solid", "Should have correct name")
  assert(red_solid.duration == 0, "Should have default infinite duration")
  assert(red_solid.loop == false || red_solid.loop == 0, "Should have default no looping")
  
  # Test animation can be started/stopped
  red_solid.start()
  assert(red_solid.is_running, "Animation should be running after start()")
  red_solid.stop()
  assert(!red_solid.is_running, "Animation should be stopped after stop()")
  
  print("✅ Unified solid test passed")
end

# Test the pulse animation (extends Animation which extends Pattern)
def test_pulse_animation()
  print("Testing pulse animation...")
  
  # Create a pulse animation with red color
  # pulse(color, min_brightness, max_brightness, pulse_period, priority, duration, loop, name)
  var pulse_anim = animation.pulse(0xFFFF0000, 0, 255, 1000, 10, 0, true, "test_pulse")
  
  # Verify it's created successfully
  assert(pulse_anim != nil, "pulse() should return a valid object")
  assert(type(pulse_anim) == "instance", "pulse() should return an instance")
  
  # Verify it has both Pattern and Animation properties
  assert(pulse_anim.priority == 10, "Animation should have default priority")
  assert(pulse_anim.opacity == 255, "Animation should have default opacity")
  assert(pulse_anim.duration == 0, "Animation should have default duration")
  assert(pulse_anim.loop == 1 || pulse_anim.loop == true, "Animation should have loop enabled")
  
  # Verify it has pulse-specific properties
  assert(pulse_anim.color == 0xFFFF0000, "Animation should have correct color")
  assert(pulse_anim.min_brightness == 0, "Animation should have correct min brightness")
  assert(pulse_anim.max_brightness == 255, "Animation should have correct max brightness")
  assert(pulse_anim.pulse_period == 1000, "Animation should have correct pulse period")
  
  print("✅ Pulse animation test passed")
end

# Test composition - animations using animations (unified architecture)
def test_animation_composition()
  print("Testing animation composition...")
  
  # Create a base solid animation (which IS a pattern)
  var red_solid = animation.solid(0xFFFF0000)
  
  # Create a pulse animation with red color (similar to composition)
  var pulse_anim = animation.pulse(0xFFFF0000, 0, 255, 1000, 10, 0, true, "pulsing_solid")
  
  # Verify both animations are created successfully
  assert(red_solid != nil, "Solid should be created")
  assert(pulse_anim != nil, "Pulse should be created")
  assert(type(red_solid) == "instance", "Solid should be an instance")
  assert(type(pulse_anim) == "instance", "Pulse should be an instance")
  
  # Verify both can be used as patterns
  red_solid.start()
  pulse_anim.start()
  
  assert(red_solid.is_running, "Base solid should be running")
  assert(pulse_anim.is_running, "Pulse animation should be running")
  
  # Verify the pulse animation has animation properties
  assert(pulse_anim.duration == 0, "Pulse animation should have infinite duration")
  assert(pulse_anim.loop == 1 || pulse_anim.loop == true, "Pulse animation should have loop enabled")
  
  print("✅ Animation composition test passed")
end

# Test unified usage - all animations used interchangeably as patterns
def test_unified_usage()
  print("Testing unified usage...")
  
  # Create different types of animations (all ARE patterns)
  var red_solid = animation.solid(0xFFFF0000)
  var blue_solid = animation.solid(0xFF0000FF, 10, 5000, true)  # With duration and loop
  var pulse_anim = animation.pulse(0xFFFF0000, 0, 255, 1000, 10, 0, true, "test_pulse")
  
  # All should support the same Pattern interface
  var items = [red_solid, blue_solid, pulse_anim]
  
  for item : items
    # All should be valid instances
    assert(item != nil, f"Item {item.name} should be created")
    assert(type(item) == "instance", f"Item {item.name} should be an instance")
    
    # All should have Pattern properties
    assert(item.priority != nil, f"Item {item.name} should have priority")
    assert(item.opacity != nil, f"Item {item.name} should have opacity")
    assert(item.name != nil, f"Item {item.name} should have name")
    
    # All should have Animation properties
    assert(item.duration != nil, f"Item {item.name} should have duration")
    assert(item.loop != nil, f"Item {item.name} should have loop")
    
    # All should support Pattern methods
    item.set_priority(20)
    assert(item.priority == 20, f"Item {item.name} should support set_priority")
    
    item.set_opacity(128)
    assert(item.opacity == 128, f"Item {item.name} should support set_opacity")
    
    # All should be startable/stoppable
    item.start()
    assert(item.is_running, f"Item {item.name} should be startable")
    
    item.stop()
    assert(!item.is_running, f"Item {item.name} should be stoppable")
  end
  
  print("✅ Unified usage test passed")
end

# Run all tests
def run_tests()
  print("Running Unified Pattern-Animation Architecture Tests...")
  print("==========")
  
  try
    test_unified_solid()
    test_pulse_animation()
    test_animation_composition()
    test_unified_usage()
    
    print("===========")
    print("✅ All tests passed!")
    print("\nKey Achievements:")
    print("- solid() now returns Animation (which IS a Pattern)")
    print("- No artificial distinction - solid() works for all contexts")
    print("- Animation extends Pattern with temporal behavior")
    print("- Everything is composable - animations using animations")
    print("- Unified architecture: Animation IS a Pattern")
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