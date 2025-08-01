# Test suite for resolve_value() method
#
# This test verifies that the new resolve_value() method correctly
# handles both static values and value providers without requiring
# parameter name strings.

import animation

# Test that resolve_value() works with static values
def test_resolve_value_with_static()
  print("Testing resolve_value() with static values...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Test with various static value types
  assert(test_anim.resolve_value(42, "test_param", 1000) == 42, "Should return static integer")
  assert(test_anim.resolve_value(0xFF00FF00, "color", 1000) == 0xFF00FF00, "Should return static color")
  assert(test_anim.resolve_value("hello", "name", 1000) == "hello", "Should return static string")
  assert(test_anim.resolve_value(true, "flag", 1000) == true, "Should return static boolean")
  assert(test_anim.resolve_value(nil, "empty", 1000) == nil, "Should return nil")
  
  print("✓ resolve_value() with static values test passed")
end

# Test that resolve_value() works with ColorProviders
def test_resolve_value_with_color_provider()
  print("Testing resolve_value() with ColorProvider...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Create a ColorProvider
  var color_provider = animation.solid_color_provider(0xFF00FF00)  # Green
  
  # Test resolve_value() - should call get_color() for color providers
  var result = test_anim.resolve_value(color_provider, "color", 1000)
  
  assert(result == 0xFF00FF00, "Should return the color value")
  
  print("✓ resolve_value() with ColorProvider test passed")
end

# Test that resolve_value() works with generic ValueProviders
def test_resolve_value_with_value_provider()
  print("Testing resolve_value() with generic ValueProvider...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Create a generic ValueProvider
  class TestValueProvider : animation.value_provider
    var value
    
    def init(value)
      self.value = value
    end
    
    def get_value(time_ms)
      return self.value + (time_ms / 100)  # Time-based calculation
    end
  end
  
  var provider = TestValueProvider(10)
  
  # Test resolve_value() - should call get_value()
  var result = test_anim.resolve_value(provider, "test_param", 500)
  
  assert(result == 15, "Should return calculated value (10 + 500/100)")
  
  print("✓ resolve_value() with ValueProvider test passed")
end

# Test performance comparison between resolve_value() and get_param_value()
def test_resolve_value_performance()
  print("Testing resolve_value() performance...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  test_anim.register_param("test_color", {"default": 0xFFFFFFFF})
  
  # Create a color provider
  var color_provider = animation.solid_color_provider(0xFF00FF00)
  test_anim.set_param("test_color", color_provider)
  
  # Both methods should return the same result
  var result1 = test_anim.get_param_value("test_color", 1000)
  var result2 = test_anim.resolve_value(color_provider, "test_color", 1000)
  
  assert(result1 == result2, "Both methods should return the same result")
  assert(result1 == 0xFF00FF00, "Should return the correct color")
  
  print("✓ resolve_value() performance test passed")
end

# Test that resolve_value() is simpler to use than get_param_value()
def test_resolve_value_simplicity()
  print("Testing resolve_value() simplicity...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Simulate how it would be used in an animation effect
  var static_color = 0xFF0000FF  # Blue
  var dynamic_color = animation.solid_color_provider(0xFF00FF00)  # Green
  
  # With resolve_value(), we can use the same method for both
  var resolved_static = test_anim.resolve_value(static_color, "color", 1000)
  var resolved_dynamic = test_anim.resolve_value(dynamic_color, "color", 1000)
  
  assert(resolved_static == 0xFF0000FF, "Should resolve static color")
  assert(resolved_dynamic == 0xFF00FF00, "Should resolve dynamic color")
  
  # This is much simpler than having to register parameters and use strings
  # No need for: test_anim.register_param("color", {...})
  # No need for: test_anim.set_param("color", value)
  # No need for: test_anim.get_param_value("color", time_ms)
  
  print("✓ resolve_value() simplicity test passed")
end

# Test that resolve_value() calls specific get_XXX() methods when available
def test_resolve_value_with_specific_method()
  print("Testing resolve_value() with specific get_XXX() method...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Create a ValueProvider with a specific get_pulse_size() method
  class SpecificMethodProvider : animation.value_provider
    var base_value
    var get_value_called
    var get_pulse_size_called
    
    def init(base_value)
      self.base_value = base_value
      self.get_value_called = 0
      self.get_pulse_size_called = 0
    end
    
    def get_value(time_ms)
      self.get_value_called += 1
      return self.base_value
    end
    
    def get_pulse_size(time_ms)
      self.get_pulse_size_called += 1
      return self.base_value * 2  # Different calculation for specific method
    end
  end
  
  var specific_provider = SpecificMethodProvider(5)
  
  # Test resolve_value() with "pulse_size" - should call get_pulse_size(), not get_value()
  var result = test_anim.resolve_value(specific_provider, "pulse_size", 1000)
  
  assert(result == 10, "Should return the specific method result (5 * 2)")
  assert(specific_provider.get_pulse_size_called == 1, "Should call get_pulse_size() once")
  assert(specific_provider.get_value_called == 0, "Should NOT call get_value()")
  
  # Test resolve_value() with "other_param" - should fall back to get_value()
  var result2 = test_anim.resolve_value(specific_provider, "other_param", 1000)
  
  assert(result2 == 5, "Should return the generic method result")
  assert(specific_provider.get_value_called == 1, "Should call get_value() once")
  assert(specific_provider.get_pulse_size_called == 1, "get_pulse_size() call count unchanged")
  
  print("✓ resolve_value() with specific method test passed")
end

# Run all tests
def run_resolve_value_tests()
  print("=== resolve_value() Method Tests ===")
  
  try
    test_resolve_value_with_static()
    test_resolve_value_with_color_provider()
    test_resolve_value_with_value_provider()
    test_resolve_value_performance()
    test_resolve_value_simplicity()
    test_resolve_value_with_specific_method()
    
    print("=== All resolve_value() tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_resolve_value_tests = run_resolve_value_tests

run_resolve_value_tests()

return run_resolve_value_tests