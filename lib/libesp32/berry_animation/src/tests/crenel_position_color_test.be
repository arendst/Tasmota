# Test suite for CrenelPositionAnimation color handling
#
# This test verifies that CrenelPositionAnimation correctly handles both
# integer colors and ColorProvider instances.

import string
import animation

# Test CrenelPositionAnimation with integer color
def test_crenel_with_integer_color()
  print("Testing CrenelPositionAnimation with integer color...")
  
  # Create engine and strip for testing
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var frame = animation.frame_buffer(10)
  var red_color = 0xFFFF0000  # Red
  
  # Create animation with new parameterized pattern
  var crenel = animation.crenel_position_animation(engine)
  
  # Set parameters via virtual member assignment
  crenel.color = red_color
  crenel.back_color = 0xFF000000  # transparent
  crenel.pos = 0
  crenel.pulse_size = 3
  crenel.low_size = 2
  crenel.nb_pulse = 2
  crenel.priority = 10
  crenel.duration = 0  # infinite
  crenel.loop = true
  crenel.opacity = 255
  crenel.name = "test_crenel_int"
  
  # Start and render
  crenel.start()
  crenel.update(1000)
  frame.clear()
  var result = crenel.render(frame, engine.time_ms)
  
  assert(result == true, "Render should succeed with integer color")
  assert(crenel.is_running == true, "Animation should be running")
  
  print("✓ CrenelPositionAnimation with integer color test passed")
end

# Test CrenelPositionAnimation with ColorProvider
def test_crenel_with_color_provider()
  print("Testing CrenelPositionAnimation with ColorProvider...")
  
  # Create engine and strip for testing
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var frame = animation.frame_buffer(10)
  var blue_color = 0xFF0000FF  # Blue
  
  # Create a solid color provider
  var color_provider = animation.static_color(engine)
  color_provider.color = blue_color
  
  # Create animation with new parameterized pattern
  var crenel = animation.crenel_position_animation(engine)
  
  # Set parameters via virtual member assignment
  crenel.color = color_provider  # ColorProvider
  crenel.back_color = 0xFF000000  # transparent
  crenel.pos = 1
  crenel.pulse_size = 2
  crenel.low_size = 3
  crenel.nb_pulse = 1
  crenel.priority = 10
  crenel.duration = 0  # infinite
  crenel.loop = true
  crenel.opacity = 255
  crenel.name = "test_crenel_provider"
  
  # Start and render
  crenel.start()
  crenel.update(1000)
  frame.clear()
  var result = crenel.render(frame, engine.time_ms)
  
  assert(result == true, "Render should succeed with ColorProvider")
  assert(crenel.is_running == true, "Animation should be running")
  
  print("✓ CrenelPositionAnimation with ColorProvider test passed")
end

# Test CrenelPositionAnimation with dynamic ColorProvider
def test_crenel_with_dynamic_color_provider()
  print("Testing CrenelPositionAnimation with dynamic ColorProvider...")
  
  # Create engine and strip for testing
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var frame = animation.frame_buffer(10)
  
  # Create a palette color provider that changes over time
  var palette_provider = animation.color_cycle(engine)
  palette_provider.palette = bytes("FF0000FFFF00FF00FFFF0000FFFFFF00")  # BGRY palette in AARRGGBB format
  palette_provider.cycle_period = 2000  # 2 second cycle
  
  # Create animation with new parameterized pattern
  var crenel = animation.crenel_position_animation(engine)
  
  # Set parameters via virtual member assignment
  crenel.color = palette_provider  # dynamic ColorProvider
  crenel.back_color = 0xFF000000  # transparent
  crenel.pos = 0
  crenel.pulse_size = 4
  crenel.low_size = 1
  crenel.nb_pulse = -1  # infinite
  crenel.priority = 10
  crenel.duration = 0  # infinite
  crenel.loop = true
  crenel.opacity = 255
  crenel.name = "test_crenel_dynamic"
  
  # Start and render at different times to verify color changes
  crenel.start()
  
  # Render at time 0
  crenel.update(0)
  frame.clear()
  var result1 = crenel.render(frame, engine.time_ms)
  assert(result1 == true, "First render should succeed")
  
  # Render at time 1000 (different color expected)
  engine.time_ms = 1000  # Simulate time passage
  crenel.update(1000)
  frame.clear()
  var result2 = crenel.render(frame, engine.time_ms)
  assert(result2 == true, "Second render should succeed")
  
  print("✓ CrenelPositionAnimation with dynamic ColorProvider test passed")
end

# Test CrenelPositionAnimation with generic ValueProvider
def test_crenel_with_generic_value_provider()
  print("Testing CrenelPositionAnimation with generic ValueProvider...")
  
  # Create engine and strip for testing
  var strip = global.Leds(10)
  var engine = animation.create_engine(strip)
  
  var frame = animation.frame_buffer(10)
  
  # Create a static value provider with a color value
  var static_provider = animation.static_value(engine)
  static_provider.value = 0xFFFF00FF  # Magenta
  
  # Create animation with new parameterized pattern
  var crenel = animation.crenel_position_animation(engine)
  
  # Set parameters via virtual member assignment
  crenel.color = static_provider  # generic ValueProvider
  crenel.back_color = 0xFF000000  # transparent
  crenel.pos = 2
  crenel.pulse_size = 3
  crenel.low_size = 2
  crenel.nb_pulse = 2
  crenel.priority = 10
  crenel.duration = 0  # infinite
  crenel.loop = true
  crenel.opacity = 255
  crenel.name = "test_crenel_generic"
  
  # Start and render
  crenel.start()
  crenel.update(1000)
  frame.clear()
  var result = crenel.render(frame, engine.time_ms)
  
  assert(result == true, "Render should succeed with generic ValueProvider")
  assert(crenel.is_running == true, "Animation should be running")
  
  print("✓ CrenelPositionAnimation with generic ValueProvider test passed")
end

# Test direct color assignment with both types
def test_crenel_set_color_methods()
  print("Testing CrenelPositionAnimation direct color assignment...")
  
  # Create engine and strip for testing
  var strip = global.Leds(5)
  var engine = animation.create_engine(strip)
  
  var frame = animation.frame_buffer(5)
  
  # Create animation with new parameterized pattern
  var crenel = animation.crenel_position_animation(engine)
  
  # Set initial parameters
  crenel.color = 0xFFFF0000  # red
  crenel.back_color = 0xFF000000  # transparent
  crenel.pos = 0
  crenel.pulse_size = 2
  crenel.low_size = 1
  crenel.nb_pulse = 1
  crenel.priority = 10
  crenel.duration = 0  # infinite
  crenel.loop = true
  crenel.opacity = 255
  crenel.name = "test_set_color"
  
  crenel.start()
  
  # Test setting integer color via direct assignment
  crenel.color = 0xFF00FF00  # Green
  crenel.update(1000)
  frame.clear()
  var result1 = crenel.render(frame, engine.time_ms)
  assert(result1 == true, "Render with new integer color should succeed")
  
  # Test setting color provider via direct assignment
  var yellow_provider = animation.static_color(engine)
  yellow_provider.color = 0xFFFFFF00  # Yellow
  crenel.color = yellow_provider
  crenel.update(1000)
  frame.clear()
  var result2 = crenel.render(frame, engine.time_ms)
  assert(result2 == true, "Render with ColorProvider should succeed")
  
  print("✓ CrenelPositionAnimation direct color assignment test passed")
end

# Test tostring method with both color types
def test_crenel_tostring()
  print("Testing CrenelPositionAnimation tostring method...")
  
  # Create engine and strip for testing
  var strip = global.Leds(5)
  var engine = animation.create_engine(strip)
  
  # Test with integer color
  var crenel_int = animation.crenel_position_animation(engine)
  crenel_int.color = 0xFFFF0000
  crenel_int.back_color = 0xFF000000
  crenel_int.pos = 0
  crenel_int.pulse_size = 2
  crenel_int.low_size = 1
  crenel_int.nb_pulse = 1
  crenel_int.priority = 10
  crenel_int.duration = 0
  crenel_int.loop = true
  crenel_int.opacity = 255
  crenel_int.name = "test_tostring_int"
  
  var str_int = str(crenel_int)
  # Just verify the string is not empty and contains expected parts
  assert(size(str_int) > 0, "String representation should not be empty")
  print(f"Integer color string: {str_int}")
  
  # Test with color provider
  var color_provider = animation.static_color(engine)
  color_provider.color = 0xFF00FF00
  
  var crenel_provider = animation.crenel_position_animation(engine)
  crenel_provider.color = color_provider
  crenel_provider.back_color = 0xFF000000
  crenel_provider.pos = 0
  crenel_provider.pulse_size = 2
  crenel_provider.low_size = 1
  crenel_provider.nb_pulse = 1
  crenel_provider.priority = 10
  crenel_provider.duration = 0
  crenel_provider.loop = true
  crenel_provider.opacity = 255
  crenel_provider.name = "test_tostring_provider"
  
  var str_provider = str(crenel_provider)
  # Just verify the string is not empty
  assert(size(str_provider) > 0, "String representation should not be empty")
  print(f"ColorProvider string: {str_provider}")
  
  print("✓ CrenelPositionAnimation tostring method test passed")
end

# Run all tests
def run_crenel_color_tests()
  print("=== CrenelPositionAnimation Color Handling Tests ===")
  
  try
    test_crenel_with_integer_color()
    test_crenel_with_color_provider()
    test_crenel_with_dynamic_color_provider()
    test_crenel_with_generic_value_provider()
    test_crenel_set_color_methods()
    test_crenel_tostring()
    
    print("=== All CrenelPositionAnimation color tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_crenel_color_tests = run_crenel_color_tests

run_crenel_color_tests()

return run_crenel_color_tests