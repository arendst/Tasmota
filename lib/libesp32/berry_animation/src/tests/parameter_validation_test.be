# Test suite for parameter validation system
#
# This test verifies that the parameter validation system correctly accepts
# ValueProvider instances for integer and real parameters.

import animation

# Test that parameters accept ValueProviders and integers only
def test_parameter_accepts_value_providers()
  print("Testing parameter validation with ValueProviders...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.animation_engine(strip)
  
  # Create a test animation using new constructor pattern
  var test_anim = animation.animation(engine)
  test_anim.priority = 10
  test_anim.duration = 0
  test_anim.loop = false
  test_anim.opacity = 255
  test_anim.name = "test"
  
  # Test with static integer value (using existing 'opacity' parameter with range 0-255)
  assert(test_anim.set_param("opacity", 42) == true, "Should accept static integer")
  assert(test_anim.get_param("opacity", 0) == 42, "Should return static integer")
  
  # Test with StaticValueProvider
  var static_provider = animation.static_value(engine)
  static_provider.value = 123
  assert(test_anim.set_param("opacity", static_provider) == true, "Should accept StaticValueProvider")
  
  # Test with OscillatorValueProvider
  var oscillator = animation.oscillator_value(engine)
  oscillator.min_value = 0
  oscillator.max_value = 255
  oscillator.duration = 1000
  oscillator.form = animation.SAWTOOTH
  assert(test_anim.set_param("opacity", oscillator) == true, "Should accept OscillatorValueProvider")
  
  # Test that invalid types are rejected (no type conversion)
  assert(test_anim.set_param("opacity", "invalid") == false, "Should reject string")
  assert(test_anim.set_param("opacity", true) == false, "Should reject boolean")
  assert(test_anim.set_param("opacity", 3.14) == false, "Should reject real")
  
  print("✓ Parameter validation with ValueProviders test passed")
end

# Test that loop parameter handles boolean values correctly
def test_loop_boolean_validation()
  print("Testing loop boolean validation...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.animation_engine(strip)
  
  # Create a test animation
  var test_anim = animation.animation(engine)
  
  # Test loop with boolean values (should be accepted since loop expects bool)
  assert(test_anim.set_param("loop", true) == true, "Should accept boolean true for loop")
  assert(test_anim.loop == true, "Should store boolean true")
  
  assert(test_anim.set_param("loop", false) == true, "Should accept boolean false for loop")
  assert(test_anim.loop == false, "Should store boolean false")
  
  # Test loop with integer values (should be rejected since loop expects bool)
  assert(test_anim.set_param("loop", 1) == false, "Should reject integer 1 for loop")
  assert(test_anim.set_param("loop", 0) == false, "Should reject integer 0 for loop")
  
  # Test loop with other invalid types
  assert(test_anim.set_param("loop", "true") == false, "Should reject string for loop")
  assert(test_anim.set_param("loop", 3.14) == false, "Should reject real for loop")
  
  print("✓ Loop boolean validation test passed")
end

# Test range validation
def test_range_validation()
  print("Testing range validation...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.animation_engine(strip)
  
  # Create a test animation
  var test_anim = animation.animation(engine)
  
  # Test valid range values (using existing 'opacity' parameter with range 0-255)
  assert(test_anim.set_param("opacity", 50) == true, "Should accept value within range")
  assert(test_anim.set_param("opacity", 0) == true, "Should accept minimum value")
  assert(test_anim.set_param("opacity", 255) == true, "Should accept maximum value")
  
  # Test invalid range values
  assert(test_anim.set_param("opacity", -1) == false, "Should reject value below minimum")
  assert(test_anim.set_param("opacity", 256) == false, "Should reject value above maximum")
  
  print("✓ Range validation test passed")
end

# Test range validation is skipped for ValueProviders
def test_range_validation_with_providers()
  print("Testing range validation with ValueProviders...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.animation_engine(strip)
  
  # Create a test animation
  var test_anim = animation.animation(engine)
  
  # Test that static values are still range-validated (using existing 'opacity' parameter with range 0-255)
  assert(test_anim.set_param("opacity", 50) == true, "Should accept value within range")
  assert(test_anim.set_param("opacity", 0) == true, "Should accept minimum value")
  assert(test_anim.set_param("opacity", 255) == true, "Should accept maximum value")
  assert(test_anim.set_param("opacity", -1) == false, "Should reject value below minimum")
  assert(test_anim.set_param("opacity", 256) == false, "Should reject value above maximum")
  
  # Test that ValueProviders bypass range validation
  # (since they provide dynamic values that can't be validated at set time)
  var oscillator = animation.oscillator_value(engine)
  oscillator.min_value = -50  # Outside range
  oscillator.max_value = 300  # Outside range
  oscillator.duration = 1000
  assert(test_anim.set_param("opacity", oscillator) == true, "Should accept ValueProvider even if it might produce out-of-range values")
  
  print("✓ Range validation with ValueProviders test passed")
end

# Test type validation
def test_type_validation()
  print("Testing type validation...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.animation_engine(strip)
  
  # Create a test class with different parameter types
  class TestClass : animation.parameterized_object
    static var PARAMS = {
      "int_param": {"default": 42},                    # Default type is "int"
      "explicit_int_param": {"type": "int", "default": 10},
      "string_param": {"type": "string", "default": "hello"},
      "bool_param": {"type": "bool", "default": true},
      "instance_param": {"type": "instance", "default": nil},
      "any_param": {"type": "any", "default": nil}
    }
    
    def init(engine)
      super(self).init(engine)
    end
  end
  
  var test_obj = TestClass(engine)
  
  # Test int parameter (default type)
  assert(test_obj.set_param("int_param", 123) == true, "Should accept int for int_param")
  assert(test_obj.set_param("int_param", "string") == false, "Should reject string for int_param")
  assert(test_obj.set_param("int_param", true) == false, "Should reject bool for int_param")
  assert(test_obj.set_param("int_param", 3.14) == false, "Should reject real for int_param")
  
  # Test explicit int parameter
  assert(test_obj.set_param("explicit_int_param", 456) == true, "Should accept int for explicit_int_param")
  assert(test_obj.set_param("explicit_int_param", "string") == false, "Should reject string for explicit_int_param")
  
  # Test string parameter
  assert(test_obj.set_param("string_param", "world") == true, "Should accept string for string_param")
  assert(test_obj.set_param("string_param", 123) == false, "Should reject int for string_param")
  assert(test_obj.set_param("string_param", true) == false, "Should reject bool for string_param")
  
  # Test bool parameter
  assert(test_obj.set_param("bool_param", true) == true, "Should accept true for bool_param")
  assert(test_obj.set_param("bool_param", false) == true, "Should accept false for bool_param")
  assert(test_obj.set_param("bool_param", 1) == false, "Should reject int for bool_param")
  assert(test_obj.set_param("bool_param", "true") == false, "Should reject string for bool_param")
  
  # Test instance parameter
  var test_instance = TestClass(engine)
  assert(test_obj.set_param("instance_param", test_instance) == true, "Should accept instance for instance_param")
  assert(test_obj.set_param("instance_param", 123) == false, "Should reject int for instance_param")
  assert(test_obj.set_param("instance_param", "string") == false, "Should reject string for instance_param")
  
  # Test any parameter (should accept any type)
  assert(test_obj.set_param("any_param", 123) == true, "Should accept int for any_param")
  assert(test_obj.set_param("any_param", "string") == true, "Should accept string for any_param")
  assert(test_obj.set_param("any_param", true) == true, "Should accept bool for any_param")
  assert(test_obj.set_param("any_param", test_instance) == true, "Should accept instance for any_param")
  assert(test_obj.set_param("any_param", 3.14) == true, "Should accept real for any_param")
  
  # Test that ValueProviders bypass type validation
  var static_provider = animation.static_value(engine)
  static_provider.value = 42  # Use a valid value for the provider itself
  assert(test_obj.set_param("int_param", static_provider) == true, "Should accept ValueProvider for any type")
  assert(test_obj.set_param("string_param", static_provider) == true, "Should accept ValueProvider for any type")
  assert(test_obj.set_param("bool_param", static_provider) == true, "Should accept ValueProvider for any type")
  
  print("✓ Type validation test passed")
end

# Run all tests
def run_parameter_validation_tests()
  print("=== Parameter Validation System Tests ===")
  
  try
    test_parameter_accepts_value_providers()
    test_loop_boolean_validation()
    test_range_validation()
    test_range_validation_with_providers()
    test_type_validation()
    
    print("=== All parameter validation tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_parameter_validation_tests = run_parameter_validation_tests

run_parameter_validation_tests()

return run_parameter_validation_tests