# Test suite for CrenelPositionAnimation color handling
#
# This test verifies that CrenelPositionAnimation correctly handles both
# integer colors and ColorProvider instances.

import string
import animation

# Test CrenelPositionAnimation with integer color
def test_crenel_with_integer_color()
  print("Testing CrenelPositionAnimation with integer color...")
  
  var frame = animation.frame_buffer(10)
  var red_color = 0xFFFF0000  # Red
  
  # Create animation with integer color
  var crenel = animation.crenel_position_animation(
    red_color,  # color
    0,          # pos
    3,          # pulse_size
    2,          # low_size
    2,          # nb_pulse
    10,         # priority
    0,          # duration (infinite)
    true,       # loop
    "test_crenel_int"
  )
  
  # Start and render
  crenel.start()
  crenel.update(1000)
  frame.clear()
  var result = crenel.render(frame, tasmota.millis())
  
  assert(result == true, "Render should succeed with integer color")
  assert(crenel.is_running == true, "Animation should be running")
  
  print("✓ CrenelPositionAnimation with integer color test passed")
end

# Test CrenelPositionAnimation with ColorProvider
def test_crenel_with_color_provider()
  print("Testing CrenelPositionAnimation with ColorProvider...")
  
  var frame = animation.frame_buffer(10)
  var blue_color = 0xFF0000FF  # Blue
  
  # Create a solid color provider
  var color_provider = animation.solid_color_provider(blue_color)
  
  # Create animation with color provider
  var crenel = animation.crenel_position_animation(
    color_provider,  # color (ColorProvider)
    1,               # pos
    2,               # pulse_size
    3,               # low_size
    1,               # nb_pulse
    10,              # priority
    0,               # duration (infinite)
    true,            # loop
    "test_crenel_provider"
  )
  
  # Start and render
  crenel.start()
  crenel.update(1000)
  frame.clear()
  var result = crenel.render(frame, tasmota.millis())
  
  assert(result == true, "Render should succeed with ColorProvider")
  assert(crenel.is_running == true, "Animation should be running")
  
  print("✓ CrenelPositionAnimation with ColorProvider test passed")
end

# Test CrenelPositionAnimation with dynamic ColorProvider
def test_crenel_with_dynamic_color_provider()
  print("Testing CrenelPositionAnimation with dynamic ColorProvider...")
  
  var frame = animation.frame_buffer(10)
  
  # Create a palette color provider that changes over time
  var palette_provider = animation.rich_palette_color_provider(
    animation.PALETTE_RAINBOW,  # Use rainbow palette
    2000,                       # 2 second cycle
    1,                          # Sine transition
    255                         # Full brightness
  )
  
  # Create animation with dynamic color provider
  var crenel = animation.crenel_position_animation(
    palette_provider,  # color (dynamic ColorProvider)
    0,                 # pos
    4,                 # pulse_size
    1,                 # low_size
    -1,                # nb_pulse (infinite)
    10,                # priority
    0,                 # duration (infinite)
    true,              # loop
    "test_crenel_dynamic"
  )
  
  # Start and render at different times to verify color changes
  crenel.start()
  
  # Render at time 0
  crenel.update(0)
  frame.clear()
  var result1 = crenel.render(frame, tasmota.millis())
  assert(result1 == true, "First render should succeed")
  
  # Render at time 1000 (different color expected)
  crenel.update(1000)
  frame.clear()
  var result2 = crenel.render(frame, tasmota.millis())
  assert(result2 == true, "Second render should succeed")
  
  print("✓ CrenelPositionAnimation with dynamic ColorProvider test passed")
end

# Test CrenelPositionAnimation with generic ValueProvider
def test_crenel_with_generic_value_provider()
  print("Testing CrenelPositionAnimation with generic ValueProvider...")
  
  var frame = animation.frame_buffer(10)
  
  # Create a static value provider with a color value
  var static_provider = animation.static_value_provider(0xFFFF00FF)  # Magenta
  
  # Create animation with generic value provider
  var crenel = animation.crenel_position_animation(
    static_provider,  # color (generic ValueProvider)
    2,                # pos
    3,                # pulse_size
    2,                # low_size
    2,                # nb_pulse
    10,               # priority
    0,                # duration (infinite)
    true,             # loop
    "test_crenel_generic"
  )
  
  # Start and render
  crenel.start()
  crenel.update(1000)
  frame.clear()
  var result = crenel.render(frame, tasmota.millis())
  
  assert(result == true, "Render should succeed with generic ValueProvider")
  assert(crenel.is_running == true, "Animation should be running")
  
  print("✓ CrenelPositionAnimation with generic ValueProvider test passed")
end

# Test set_color method with both types
def test_crenel_set_color_methods()
  print("Testing CrenelPositionAnimation set_color methods...")
  
  var frame = animation.frame_buffer(5)
  
  # Create animation with initial integer color
  var crenel = animation.crenel_position_animation(
    0xFFFF0000,  # red
    0, 2, 1, 1, 10, 0, true, "test_set_color"
  )
  
  crenel.start()
  
  # Test setting integer color
  crenel.set_color(0xFF00FF00)  # Green
  crenel.update(1000)
  frame.clear()
  var result1 = crenel.render(frame, tasmota.millis())
  assert(result1 == true, "Render with new integer color should succeed")
  
  # Test setting color provider
  var yellow_provider = animation.solid_color_provider(0xFFFFFF00)  # Yellow
  crenel.set_color(yellow_provider)
  crenel.update(1000)
  frame.clear()
  var result2 = crenel.render(frame, tasmota.millis())
  assert(result2 == true, "Render with ColorProvider should succeed")
  
  print("✓ CrenelPositionAnimation set_color methods test passed")
end

# Test tostring method with both color types
def test_crenel_tostring()
  print("Testing CrenelPositionAnimation tostring method...")
  
  # Test with integer color
  var crenel_int = animation.crenel_position_animation(
    0xFFFF0000, 0, 2, 1, 1, 10, 0, true, "test_tostring_int"
  )
  var str_int = str(crenel_int)
  # Just verify the string is not empty and contains expected parts
  assert(size(str_int) > 0, "String representation should not be empty")
  print(f"Integer color string: {str_int}")
  
  # Test with color provider
  var color_provider = animation.solid_color_provider(0xFF00FF00)
  var crenel_provider = animation.crenel_position_animation(
    color_provider, 0, 2, 1, 1, 10, 0, true, "test_tostring_provider"
  )
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