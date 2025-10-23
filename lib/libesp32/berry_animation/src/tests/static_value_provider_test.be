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
  assert(provider.get_value(1000) == nil, "Default get_value should return nil (no value set)")
  assert(provider.update(1000) == false, "Update should return false")
  
  # Test setting a value
  provider.value = 42
  assert(provider.get_value(1000) == 42, "Should return set value")
  assert(provider.get_value(2000) == 42, "Should return same value regardless of time")
  
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
  assert(int_provider.get_value(1000) == 123, "Should handle integer values")
  
  # Test with string
  var str_provider = animation.static_value(engine)
  str_provider.value = "hello"
  assert(str_provider.get_value(1000) == "hello", "Should handle string values")
  
  # Test with color (hex value)
  var color_provider = animation.static_value(engine)
  color_provider.value = 0xFF00FF00
  assert(color_provider.get_value(1000) == 0xFF00FF00, "Should handle color values")
  
  print("✓ StaticValueProvider types test passed")
end

# Test universal get_XXX methods via member() construct
def test_universal_get_methods()
  print("Testing universal get_XXX methods...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  var provider = animation.static_value(engine)
  provider.value = 99
  
  # Test various get_XXX methods
  var get_pulse_size = provider.("get_pulse_size")
  assert(type(get_pulse_size) == "function", "Should return function for get_pulse_size")
  assert(get_pulse_size(1000) == 99, "get_pulse_size should return static value")
  
  var get_pos = provider.("get_pos")
  assert(type(get_pos) == "function", "Should return function for get_pos")
  assert(get_pos(1000) == 99, "get_pos should return static value")
  
  var get_color = provider.("get_color")
  assert(type(get_color) == "function", "Should return function for get_color")
  assert(get_color(1000) == 99, "get_color should return static value")
  
  # Test that non-get methods return undefined
  try
    var other_method = provider.("some_other_method")
    # Should return undefined module, not a function
    assert(type(other_method) != "function", "Non-get methods should not return functions")
  except .. as e
    # Exception is also acceptable
  end
  
  print("✓ Universal get_XXX methods test passed")
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
  assert(provider.get_value(1000) == nil, "Initial value should be nil")
  
  # Test setting and changing values
  provider.value = 10
  assert(provider.get_value(1000) == 10, "Should return first set value")
  
  provider.value = 20
  assert(provider.get_value(1000) == 20, "Should return updated value")
  
  provider.value = "changed"
  assert(provider.get_value(1000) == "changed", "Should handle type changes")
  
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
    test_universal_get_methods()
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