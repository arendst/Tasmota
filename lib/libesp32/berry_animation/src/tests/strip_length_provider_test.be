# Test file for StripLengthProvider class
#
# This file contains tests for the StripLengthProvider class which provides
# access to the LED strip length as a dynamic value provider.
#
# Command to run test is:
#    ./berry -s -g -m lib/libesp32/berry_animation/src -e "import tasmota" lib/libesp32/berry_animation/src/tests/strip_length_provider_test.be

import animation

print("Testing StripLengthProvider...")

# Create a mock LED strip for testing
class MockStrip
  var _length
  
  def init(length)
    self._length = length
  end
  
  def length()
    return self._length
  end
  
  def set_pixel_color(index, color)
    # Mock implementation - do nothing
  end
  
  def show()
    # Mock implementation - do nothing
  end
  
  def clear()
    # Mock implementation - do nothing
  end
  
  def can_show()
    return true
  end
end

# Test basic functionality
def test_basic_functionality()
  print("  Testing basic functionality...")
  
  # Test with different strip lengths
  var test_lengths = [10, 30, 60, 144]
  
  for length : test_lengths
    # Create mock strip and engine
    var strip = MockStrip(length)
    var engine = animation.create_engine(strip)
    
    # Create StripLengthProvider
    var provider = animation.strip_length(engine)
    
    # Test the provider
    var result = provider.produce_value("length", 1000)
    assert(result == length, f"Expected {length}, got {result}")
    
    # Test that parameter name doesn't matter
    var result2 = provider.produce_value("width", 2000)
    assert(result2 == length, f"Expected {length}, got {result2}")
    
    # Test that time doesn't matter
    var result3 = provider.produce_value("size", nil)
    assert(result3 == length, f"Expected {length}, got {result3}")
  end
  
  print("    ✓ Basic functionality tests passed")
end

# Test string representation
def test_string_representation()
  print("  Testing string representation...")
  
  var strip = MockStrip(42)
  var engine = animation.create_engine(strip)
  var provider = animation.strip_length(engine)
  
  var str_repr = str(provider)
  assert(str_repr == "StripLengthProvider(length=42)", f"Unexpected string representation: {str_repr}")
  
  print("    ✓ String representation test passed")
end

# Test error handling
def test_error_handling()
  print("  Testing error handling...")
  
  # Test with nil engine (should raise error during construction)
  try
    var provider_nil = animation.strip_length(nil)
    assert(false, "Should have raised an error with nil engine")
  except "value_error"
    # Expected behavior
  except .. as e
    assert(false, f"Unexpected error: {e}")
  end
  
  print("    ✓ Error handling test passed")
end

# Test integration with animation system
def test_integration()
  print("  Testing integration with animation system...")
  
  var strip = MockStrip(20)
  var engine = animation.create_engine(strip)
  var provider = animation.strip_length(engine)
  
  # Test that it's recognized as a value provider
  assert(animation.is_value_provider(provider), "Should be recognized as a value provider")
  
  # Test that it can be used as a parameter value
  var solid_anim = animation.solid(engine)
  solid_anim.color = 0xFF0000FF
  
  # This should work without errors (though the animation won't use strip_length directly)
  var length_value = provider.produce_value("test", engine.time_ms)
  assert(length_value == 20, f"Expected 20, got {length_value}")
  
  print("    ✓ Integration test passed")
end

# Test consistency with engine properties
def test_engine_consistency()
  print("  Testing consistency with engine properties...")
  
  var strip = MockStrip(100)
  var engine = animation.create_engine(strip)
  var provider = animation.strip_length(engine)
  
  # Test that provider returns same value as engine properties
  var provider_length = provider.produce_value("length", 0)
  var engine_width = engine.width
  var engine_strip_length = engine.get_strip_length()
  
  assert(provider_length == engine_width, f"Provider length {provider_length} != engine width {engine_width}")
  assert(provider_length == engine_strip_length, f"Provider length {provider_length} != engine strip length {engine_strip_length}")
  
  print("    ✓ Engine consistency test passed")
end

# Run all tests
def run_all_tests()
  test_basic_functionality()
  test_string_representation()
  test_error_handling()
  test_integration()
  test_engine_consistency()
  
  print("All StripLengthProvider tests passed!")
  return true
end

# Execute tests
run_all_tests()