# Test suite for StaticValueProvider class
#
# This test verifies that the StaticValueProvider class works correctly
# and follows the parameterized class specification.

import string
import animation

# Test the basic StaticValueProvider interface
def test_static_value_provider_interface()
  print("Testing StaticValueProvider interface...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var provider = animation.static_value(engine)
  
  # Test default methods
  assert(provider.produce_value("test", 1000) == nil, "Default produce_value should return nil (no value set)")
  provider.update(1000)  # update() does not return a value
  
  # Test setting a value
  provider.value = 42
  assert(provider.produce_value("test", 1000) == 42, "Should return set value")
  assert(provider.produce_value("test", 2000) == 42, "Should return same value regardless of time")
  
  print("✓ StaticValueProvider interface test passed")
end

# Test with different value types
def test_static_value_provider_types()
  print("Testing StaticValueProvider with different types...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  # Test with integer
  var int_provider = animation.static_value(engine)
  int_provider.value = 123
  assert(int_provider.produce_value("test", 1000) == 123, "Should handle integer values")
  
  # Test with string
  var str_provider = animation.static_value(engine)
  str_provider.value = "hello"
  assert(str_provider.produce_value("test", 1000) == "hello", "Should handle string values")
  
  # Test with color (hex value)
  var color_provider = animation.static_value(engine)
  color_provider.value = 0xFF00FF00
  assert(color_provider.produce_value("test", 1000) == 0xFF00FF00, "Should handle color values")
  
  print("✓ StaticValueProvider types test passed")
end

# Test produce_value method with different parameter names
def test_produce_value_method()
  print("Testing produce_value method...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var provider = animation.static_value(engine)
  provider.value = 99
  
  # Test produce_value with various parameter names - should return same value
  assert(provider.produce_value("pulse_size", 1000) == 99, "produce_value should return static value for pulse_size")
  assert(provider.produce_value("pos", 1000) == 99, "produce_value should return static value for pos")
  assert(provider.produce_value("color", 1000) == 99, "produce_value should return static value for color")
  assert(provider.produce_value("any_param", 2000) == 99, "produce_value should return static value for any param name")
  
  print("✓ produce_value method test passed")
end

# Test comparison operators
def test_comparison_operators()
  print("Testing comparison operators...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var provider = animation.static_value(engine)
  provider.value = 50
  
  # Test all comparison operators
  assert(provider < 100, "Should be less than 100")
  assert(provider > 25, "Should be greater than 25")
  assert(provider <= 50, "Should be less than or equal to 50")
  assert(provider >= 50, "Should be greater than or equal to 50")
  assert(provider == 50, "Should be equal to 50")
  assert(provider != 25, "Should not be equal to 25")
  
  print("✓ Comparison operators test passed")
end

# Test parameterized object integration
def test_parameterized_object_integration()
  print("Testing ParameterizedObject integration...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var provider = animation.static_value(engine)
  
  # Test that it has the engine reference
  assert(provider.engine != nil, "Provider should have engine reference")
  assert(provider.engine == engine, "Provider should have correct engine reference")
  
  # Test parameter system methods exist
  assert(type(provider.set_param) == "function", "Should have set_param method")
  assert(type(provider.get_param) == "function", "Should have get_param method")
  
  # Test parameter setting via parameter system
  assert(provider.set_param("value", 777) == true, "Should be able to set value via parameter system")
  assert(provider.get_param("value", nil) == 777, "Should retrieve value via parameter system")
  assert(provider.value == 777, "Virtual member should reflect parameter value")
  
  print("✓ ParameterizedObject integration test passed")
end

# Test value changes
def test_value_changes()
  print("Testing value changes...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var provider = animation.static_value(engine)
  
  # Test initial state
  assert(provider.produce_value("test", 1000) == nil, "Initial value should be nil")
  
  # Test setting and changing values
  provider.value = 10
  assert(provider.produce_value("test", 1000) == 10, "Should return first set value")
  
  provider.value = 20
  assert(provider.produce_value("test", 1000) == 20, "Should return updated value")
  
  provider.value = "changed"
  assert(provider.produce_value("test", 1000) == "changed", "Should handle type changes")
  
  print("✓ Value changes test passed")
end

# Test string representation
def test_string_representation()
  print("Testing string representation...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var provider = animation.static_value(engine)
  provider.value = 42
  
  var str_repr = str(provider)
  assert(string.find(str_repr, "StaticValueProvider") >= 0, "String representation should contain class name")
  assert(string.find(str_repr, "42") >= 0, "String representation should contain the value")
  
  print("✓ String representation test passed")
end

# Run all tests
def run_static_value_provider_tests()
  print("=== StaticValueProvider Tests ===")
  
  try
    test_static_value_provider_interface()
    test_static_value_provider_types()
    test_produce_value_method()
    test_comparison_operators()
    test_parameterized_object_integration()
    test_value_changes()
    test_string_representation()
    
    print("=== All StaticValueProvider tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_static_value_provider_tests = run_static_value_provider_tests

run_static_value_provider_tests()

return run_static_value_provider_tests