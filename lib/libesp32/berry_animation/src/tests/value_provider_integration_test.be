# Integration Test: Value Provider Auto-Registration
#
# This test demonstrates the practical use case where value providers
# automatically register with the engine and receive update() calls.

import animation

def test_auto_registration_with_animation()
  print("=== Testing Value Provider Auto-Registration with Animation ===")
  
  # Create engine
  var strip = Leds(30)
  var engine = animation.create_engine(strip)
  
  # Create a value provider - it auto-registers with engine
  var oscillator = animation.triangle(engine)
  oscillator.min_value = 0
  oscillator.max_value = 255
  oscillator.duration = 2000
    
  # Create an animation that uses the oscillator
  var beacon = animation.beacon_animation(engine)
  beacon.color = 0xFFFF0000
  beacon.pos = oscillator
  beacon.beacon_size = 3
  
  # Start the animation (which starts the oscillator)
  beacon.start(1000)
  oscillator.start(1000)
  
  # Start the engine
  engine.run()
  
  # Simulate a few update cycles
  engine.on_tick(1000)
  engine.on_tick(1100)
  engine.on_tick(1200)
  
  # The oscillator should have been updated by the engine
  var value1 = oscillator.produce_value("pos", 1200)
  assert(value1 != nil, "Oscillator should produce values")
  print(f"✓ Oscillator producing values: {value1}")
  
  # Stop the engine
  engine.stop()
  
  print("✓ Auto-registration integration test passed")
end

def test_multiple_providers_coordination()
  print("\n=== Testing Multiple Value Providers Coordination ===")
  
  # Create engine
  var strip = Leds(30)
  var engine = animation.create_engine(strip)
  
  # Create multiple value providers - all auto-register
  var position_osc = animation.triangle(engine)
  position_osc.min_value = 0
  position_osc.max_value = 29
  position_osc.duration = 3000
  
  var brightness_osc = animation.smooth(engine)
  brightness_osc.min_value = 50
  brightness_osc.max_value = 255
  brightness_osc.duration = 2000
  
  var color_cycle = animation.color_cycle(engine)
  color_cycle.palette = bytes("FFFF0000" "FF00FF00" "FF0000FF")
  color_cycle.cycle_period = 5000
  
  # Create animation using all providers
  var beacon = animation.beacon_animation(engine)
  beacon.color = color_cycle
  beacon.pos = position_osc
  beacon.opacity = brightness_osc
  beacon.beacon_size = 2
  
  # Start everything
  position_osc.start(1000)
  brightness_osc.start(1000)
  color_cycle.start(1000)
  beacon.start(1000)
  engine.run()
  
  # Simulate updates
  engine.on_tick(1000)
  engine.on_tick(2000)
  engine.on_tick(3000)
  
  # All providers should be producing values
  var pos_val = position_osc.produce_value("pos", 3000)
  var bright_val = brightness_osc.produce_value("opacity", 3000)
  var color_val = color_cycle.produce_value("color", 3000)
  
  assert(pos_val != nil, "Position oscillator should produce values")
  assert(bright_val != nil, "Brightness oscillator should produce values")
  assert(color_val != nil, "Color cycle should produce values")
  
  print(f"✓ All providers producing values: pos={pos_val}, brightness={bright_val}, color=0x{color_val:08X}")
  
  engine.stop()
  
  print("✓ Multiple providers coordination test passed")
end

def test_template_animation_scenario()
  print("\n=== Testing Template Animation Scenario ===")
  
  # Simulate what happens in a template animation
  var strip = Leds(30)
  var engine = animation.create_engine(strip)
  
  # In a template animation, the engine_proxy is 'self'
  # For this test, we'll use the root_animation as the proxy
  var proxy = engine.root_animation
  
  # Template creates value providers - they auto-register with engine
  var sweep = animation.triangle(engine)
  sweep.min_value = 0
  sweep.max_value = 29
  sweep.duration = 4000
  
  # Template creates animations using the provider
  var beacon1 = animation.beacon_animation(engine)
  beacon1.color = 0xFFFF0000
  beacon1.pos = sweep
  beacon1.beacon_size = 2
  
  var beacon2 = animation.beacon_animation(engine)
  beacon2.color = 0xFF0000FF
  beacon2.pos = sweep  # Same provider used by multiple animations
  beacon2.beacon_size = 1
  
  # Start everything
  sweep.start(1000)
  beacon1.start(1000)
  beacon2.start(1000)
  engine.run()
  
  # Simulate updates - the sweep provider is updated once per frame
  # even though it's used by multiple animations
  engine.on_tick(1000)
  engine.on_tick(2000)
  
  var sweep_val = sweep.produce_value("pos", 2000)
  assert(sweep_val != nil, "Sweep should produce values")
  print(f"✓ Shared provider producing values: {sweep_val}")
  
  engine.stop()
  
  print("✓ Template animation scenario test passed")
end

# Run all integration tests
print("=== Value Provider Integration Tests ===\n")

try
  test_auto_registration_with_animation()
  test_multiple_providers_coordination()
  test_template_animation_scenario()
  
  print("\n=== All Integration Tests Passed! ===")
except .. as e, msg
  print(f"\nIntegration test failed: {e} - {msg}")
  raise "test_failed"
end
