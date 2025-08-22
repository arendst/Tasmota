# Test suite for get_param_value() method enhancement
#
# This test verifies that the enhanced get_param_value() method correctly
# handles ColorProviders with optimal produce_value() calls.

import animation

# Create a mock engine for testing
class MockEngine
  var time_ms
  
  def init()
    self.time_ms = 1000  # Fixed time for testing
  end
end

var mock_engine = MockEngine()

# Test that get_param_value() calls produce_value() for ColorProviders
def test_get_param_value_with_color_provider()
  print("Testing get_param_value() with ColorProvider...")
  
  # Create a test animation using new constructor pattern
  var test_anim = animation.animation(mock_engine)
  test_anim.priority = 10
  test_anim.duration = 0
  test_anim.loop = false
  test_anim.opacity = 255
  test_anim.name = "test"
  
  # Create a ColorProvider that we can track calls on
  class TrackingColorProvider : animation.color_provider
    var color
    var produce_value_called
    
    def init(engine, color)
      super(self).init(engine)
      self.color = color
      self.produce_value_called = 0
    end
    
    def produce_value(name, time_ms)
      self.produce_value_called += 1
      return self.color
    end
  end
  
  var tracking_provider = TrackingColorProvider(mock_engine, 0xFF00FF00)  # Green
  
  # Set the ColorProvider (using the 'color' parameter that exists in base Animation)
  test_anim.color = tracking_provider
  
  # Call get_param_value() - should call produce_value()
  var result = test_anim.get_param_value("color", 1000)
  
  assert(result == 0xFF00FF00, "Should return the color value")
  assert(tracking_provider.produce_value_called == 1, "Should call produce_value() once")
  
  print("✓ get_param_value() with ColorProvider test passed")
end

# Test that get_param_value() calls produce_value() for generic ValueProviders
def test_get_param_value_with_generic_provider()
  print("Testing get_param_value() with generic ValueProvider...")
  
  # Create a test animation using new constructor pattern
  var test_anim = animation.animation(mock_engine)
  test_anim.priority = 10
  test_anim.duration = 0
  test_anim.loop = false
  test_anim.opacity = 255
  test_anim.name = "test"
  
  # Create a generic ValueProvider that we can track calls on
  class TrackingValueProvider : animation.value_provider
    var value
    var produce_value_called
    
    def init(engine, value)
      super(self).init(engine)
      self.value = value
      self.produce_value_called = 0
    end
    
    def produce_value(name, time_ms)
      self.produce_value_called += 1
      return self.value
    end
  end
  
  var tracking_provider = TrackingValueProvider(mock_engine, 42)
  
  # Set the ValueProvider (using the 'priority' parameter that exists in base Animation)
  test_anim.priority = tracking_provider
  
  # Call get_param_value() - should call produce_value()
  var result = test_anim.get_param_value("priority", 1000)
  
  assert(result == 42, "Should return the value")
  assert(tracking_provider.produce_value_called == 1, "Should call produce_value() once")
  
  print("✓ get_param_value() with generic ValueProvider test passed")
end

# Test that get_param_value() calls produce_value() method consistently
def test_get_param_value_with_context_aware_provider()
  print("Testing get_param_value() with context-aware ValueProvider...")
  
  # Create a test animation using new constructor pattern
  var test_anim = animation.animation(mock_engine)
  test_anim.priority = 10
  test_anim.duration = 0
  test_anim.loop = false
  test_anim.opacity = 255
  test_anim.name = "test"
  
  # Create a ValueProvider that returns different values based on parameter name
  class ContextAwareProvider : animation.value_provider
    var base_value
    var produce_value_called
    var last_param_name
    
    def init(engine, base_value)
      super(self).init(engine)
      self.base_value = base_value
      self.produce_value_called = 0
      self.last_param_name = nil
    end
    
    def produce_value(name, time_ms)
      self.produce_value_called += 1
      self.last_param_name = name
      if name == "duration"
        return self.base_value * 2  # Different calculation for duration
      else
        return self.base_value
      end
    end
  end
  
  var context_provider = ContextAwareProvider(mock_engine, 5)
  
  # Set the ValueProvider (using the 'duration' parameter that exists in base Animation)
  test_anim.duration = context_provider
  
  # Call get_param_value() - should call produce_value() with parameter name
  var result = test_anim.get_param_value("duration", 1000)
  
  assert(result == 10, "Should return the context-aware result (5 * 2)")
  assert(context_provider.produce_value_called == 1, "Should call produce_value() once")
  assert(context_provider.last_param_name == "duration", "Should pass parameter name to produce_value()")
  
  print("✓ get_param_value() with context-aware provider test passed")
end

# Test that get_param_value() returns static values unchanged
def test_get_param_value_with_static_value()
  print("Testing get_param_value() with static value...")
  
  # Create a test animation using new constructor pattern
  var test_anim = animation.animation(mock_engine)
  test_anim.priority = 10
  test_anim.duration = 0
  test_anim.loop = false
  test_anim.opacity = 255
  test_anim.name = "test"
  
  # Set a static value (using the 'opacity' parameter that exists in base Animation)
  test_anim.opacity = 123
  
  # Call get_param_value() - should return the static value
  var result = test_anim.get_param_value("opacity", 1000)
  
  assert(result == 123, "Should return the static value unchanged")
  
  print("✓ get_param_value() with static value test passed")
end

# Run all tests
def run_get_param_value_tests()
  print("=== get_param_value() Enhancement Tests ===")
  
  try
    test_get_param_value_with_color_provider()
    test_get_param_value_with_generic_provider()
    test_get_param_value_with_context_aware_provider()
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