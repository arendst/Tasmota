# Test suite for get_param_value() method enhancement
#
# This test verifies that the enhanced get_param_value() method correctly
# handles ColorProviders with optimal get_color() calls.

import animation

# Test that get_param_value() calls get_color() for ColorProviders
def test_get_param_value_with_color_provider()
  print("Testing get_param_value() with ColorProvider...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Register a color parameter
  test_anim.register_param("test_color", {"default": 0xFFFFFFFF})
  
  # Create a ColorProvider that we can track calls on
  class TrackingColorProvider : animation.color_provider
    var color
    var get_color_called
    var get_value_called
    
    def init(color)
      self.color = color
      self.get_color_called = 0
      self.get_value_called = 0
    end
    
    def get_color(time_ms)
      self.get_color_called += 1
      return self.color
    end
    
    def get_value(time_ms)
      self.get_value_called += 1
      return self.color
    end
  end
  
  var tracking_provider = TrackingColorProvider(0xFF00FF00)  # Green
  
  # Set the ColorProvider
  test_anim.set_param("test_color", tracking_provider)
  
  # Call get_param_value() - should call get_color(), not get_value()
  var result = test_anim.get_param_value("test_color", 1000)
  
  assert(result == 0xFF00FF00, "Should return the color value")
  assert(tracking_provider.get_color_called == 1, "Should call get_color() once")
  assert(tracking_provider.get_value_called == 0, "Should NOT call get_value()")
  
  print("✓ get_param_value() with ColorProvider test passed")
end

# Test that get_param_value() calls get_value() for generic ValueProviders
def test_get_param_value_with_generic_provider()
  print("Testing get_param_value() with generic ValueProvider...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Register a parameter
  test_anim.register_param("test_param", {"default": 0})
  
  # Create a generic ValueProvider that we can track calls on
  class TrackingValueProvider : animation.value_provider
    var value
    var get_value_called
    
    def init(value)
      self.value = value
      self.get_value_called = 0
    end
    
    def get_value(time_ms)
      self.get_value_called += 1
      return self.value
    end
  end
  
  var tracking_provider = TrackingValueProvider(42)
  
  # Set the ValueProvider
  test_anim.set_param("test_param", tracking_provider)
  
  # Call get_param_value() - should call get_value()
  var result = test_anim.get_param_value("test_param", 1000)
  
  assert(result == 42, "Should return the value")
  assert(tracking_provider.get_value_called == 1, "Should call get_value() once")
  
  print("✓ get_param_value() with generic ValueProvider test passed")
end

# Test that get_param_value() calls specific get_XXX() methods when available
def test_get_param_value_with_specific_method()
  print("Testing get_param_value() with specific get_XXX() method...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Register a parameter
  test_anim.register_param("pulse_size", {"default": 1})
  
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
  
  # Set the ValueProvider
  test_anim.set_param("pulse_size", specific_provider)
  
  # Call get_param_value() - should call get_pulse_size(), not get_value()
  var result = test_anim.get_param_value("pulse_size", 1000)
  
  assert(result == 10, "Should return the specific method result (5 * 2)")
  assert(specific_provider.get_pulse_size_called == 1, "Should call get_pulse_size() once")
  assert(specific_provider.get_value_called == 0, "Should NOT call get_value()")
  
  print("✓ get_param_value() with specific method test passed")
end

# Test that get_param_value() returns static values unchanged
def test_get_param_value_with_static_value()
  print("Testing get_param_value() with static value...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Register a parameter
  test_anim.register_param("static_param", {"default": 0})
  
  # Set a static value
  test_anim.set_param("static_param", 123)
  
  # Call get_param_value() - should return the static value
  var result = test_anim.get_param_value("static_param", 1000)
  
  assert(result == 123, "Should return the static value unchanged")
  
  print("✓ get_param_value() with static value test passed")
end

# Run all tests
def run_get_param_value_tests()
  print("=== get_param_value() Enhancement Tests ===")
  
  try
    test_get_param_value_with_color_provider()
    test_get_param_value_with_generic_provider()
    test_get_param_value_with_specific_method()
    test_get_param_value_with_static_value()
    
    print("=== All get_param_value() tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_get_param_value_tests = run_get_param_value_tests

run_get_param_value_tests()

return run_get_param_value_tests