# Test suite for ValueProvider system
#
# This test verifies that the ValueProvider system works correctly
# with both static values and dynamic providers, and that time_ms
# is correctly passed to all provider methods.

import animation

# Test the basic ValueProvider interface
def test_value_provider_interface()
  print("Testing ValueProvider interface...")
  
  var provider = animation.value_provider()
  
  # Test default methods
  assert(provider.get_value(1000) == nil, "Default get_value should return nil")
  assert(provider.update(1000) == false, "Default update should return false")
  
  print("✓ ValueProvider interface test passed")
end

# Test the StaticValueProvider
def test_static_value_provider()
  print("Testing StaticValueProvider...")
  
  # Test with integer value
  var int_provider = animation.static_value_provider(42)
  assert(int_provider.get_value(1000) == 42, "Should return static integer value")
  assert(int_provider.update(1000) == false, "Update should return false")
  
  # Test with string value
  var str_provider = animation.static_value_provider("hello")
  assert(str_provider.get_value(2000) == "hello", "Should return static string value")
  
  # Test member() construct for get_XXX methods
  var pulse_size_method = int_provider.("get_pulse_size")
  assert(type(pulse_size_method) == "function", "Should return function for get_pulse_size")
  assert(pulse_size_method(1000) == 42, "get_pulse_size should return static value")
  
  var pos_method = int_provider.("get_pos")
  assert(type(pos_method) == "function", "Should return function for get_pos")
  assert(pos_method(1000) == 42, "get_pos should return static value")
  
  # Test non-get methods should use default behavior
  try
    var other_method = int_provider.("some_other_method")
    assert(false, "member should have failed")
  except "attribute_error"
    # all good, exception is expected
  end
  
  print("✓ StaticValueProvider test passed")
end

# Test the helper functions in Animation base class
def test_animation_helpers()
  print("Testing Animation helper methods...")
  
  # Create a simple animation for testing
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Register a test parameter
  test_anim.register_param("test_param", {"default": 0})
  
  # Test with static value
  assert(test_anim.set_param("test_param", 123) == true, "Should set static value")
  var retrieved = test_anim.get_param("test_param", 1000)
  assert(retrieved == 123, "Should retrieve static value")
  
  # Test with value provider
  var provider = animation.static_value_provider(456)
  assert(test_anim.set_param("test_param", provider) == true, "Should set provider")
  retrieved = test_anim.get_param_value("test_param", 1000)
  assert(retrieved == 456, "Should retrieve value from provider")
  
  # Test that time_ms is correctly passed to providers
  class TimeAwareProvider : animation.value_provider
    var last_time_received
    
    def init()
      self.last_time_received = nil
    end
    
    def get_value(time_ms)
      self.last_time_received = time_ms
      return time_ms / 10  # Return a value based on time
    end
    
    def get_test_param(time_ms)
      self.last_time_received = time_ms
      return time_ms / 5  # Different calculation for specific method
    end
  end
  
  var time_provider = TimeAwareProvider()
  test_anim.set_param("test_param", time_provider)
  
  # Test generic get_value method (when specific method doesn't exist)
  test_anim.register_param("unknown_param", {"default": 0})
  test_anim.set_param("unknown_param", time_provider)
  var result = test_anim.get_param_value("unknown_param", 2000)
  assert(time_provider.last_time_received == 2000, "Should pass time_ms to get_value")
  assert(result == 200, "Should return time-based value from get_value")
  
  # Test specific get_test_param method
  result = test_anim.get_param_value("test_param", 1500)
  assert(time_provider.last_time_received == 1500, "Should pass time_ms to get_test_param")
  assert(result == 300, "Should return time-based value from get_test_param")
  
  print("✓ Animation helper methods test passed")
end

# Test with a custom value provider
def test_custom_value_provider()
  print("Testing custom ValueProvider...")
  
  # Create a simple time-based provider
  class TimeBasedProvider : animation.value_provider
    var multiplier
    
    def init(multiplier)
      self.multiplier = multiplier != nil ? multiplier : 1
    end
    
    def get_value(time_ms)
      return (time_ms / 100) * self.multiplier  # Changes every 100ms
    end
    
    def get_pulse_size(time_ms)
      var value = self.get_value(time_ms)
      return value > 0 ? value : 1  # Ensure positive
    end
  end
  
  var provider = TimeBasedProvider(2)
  
  # Test at different times
  assert(provider.get_value(0) == 0, "Should return 0 at time 0")
  assert(provider.get_value(100) == 2, "Should return 2 at time 100")
  assert(provider.get_value(500) == 10, "Should return 10 at time 500")
  
  # Test specific method
  assert(provider.get_pulse_size(100) == 2, "get_pulse_size should return 2")
  assert(provider.get_pulse_size(0) == 1, "get_pulse_size should return 1 (minimum)")
  
  print("✓ Custom ValueProvider test passed")
end

# Test is_value_provider function
def test_is_value_provider()
  print("Testing is_value_provider function...")
  
  var static_provider = animation.static_value_provider(42)
  var base_provider = animation.value_provider()
  
  assert(animation.is_value_provider(static_provider) == true, "StaticValueProvider should be detected")
  assert(animation.is_value_provider(base_provider) == true, "ValueProvider should be detected")
  assert(animation.is_value_provider(42) == false, "Integer should not be detected")
  assert(animation.is_value_provider("hello") == false, "String should not be detected")
  assert(animation.is_value_provider(nil) == false, "nil should not be detected")
  
  print("✓ is_value_provider test passed")
end

# Test ColorProvider inheritance from ValueProvider
def test_color_provider_inheritance()
  print("Testing ColorProvider inheritance...")
  
  # Test that ColorProviders are also ValueProviders
  var solid_color = animation.solid_color_provider(0xFF00FF00)  # Green
  var base_color = animation.color_provider()
  
  assert(animation.is_value_provider(solid_color) == true, "SolidColorProvider should be a ValueProvider")
  assert(animation.is_color_provider(solid_color) == true, "SolidColorProvider should be a ColorProvider")
  assert(animation.is_value_provider(base_color) == true, "ColorProvider should be a ValueProvider")
  assert(animation.is_color_provider(base_color) == true, "ColorProvider should be a ColorProvider")
  
  # Test that color providers work with the parameter system
  var test_anim = animation.animation(10, 0, false, "test")
  test_anim.register_param("color", {"default": 0xFFFFFFFF})
  
  # Test with color provider
  assert(test_anim.set_param_value("color", solid_color) == true, "Should set color provider")
  var retrieved = test_anim.get_param_value("color", 1000)
  assert(retrieved == 0xFF00FF00, "Should retrieve color from provider via get_color()")
  
  # Test that get_value() delegates to get_color()
  assert(solid_color.get_value(1000) == 0xFF00FF00, "get_value() should delegate to get_color()")
  assert(solid_color.get_color(1000) == 0xFF00FF00, "get_color() should return the color")
  
  print("✓ ColorProvider inheritance test passed")
end

# Run all tests
def run_value_provider_tests()
  print("=== ValueProvider System Tests ===")
  
  try
    test_value_provider_interface()
    test_static_value_provider()
    test_animation_helpers()
    test_custom_value_provider()
    test_is_value_provider()
    test_color_provider_inheritance()
    
    print("=== All ValueProvider tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_value_provider_tests = run_value_provider_tests

run_value_provider_tests()

return run_value_provider_tests