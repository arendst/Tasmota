# Test suite for parameter validation system
#
# This test verifies that the parameter validation system correctly accepts
# ValueProvider instances for integer and real parameters.

import animation

# Test that parameters accept ValueProviders and integers only
def test_parameter_accepts_value_providers()
  print("Testing parameter validation with ValueProviders...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Register a parameter (no type needed, only integers accepted)
  test_anim.register_param("test_param", {"default": 0, "min": 0, "max": 255})
  
  # Test with static integer value
  assert(test_anim.set_param("test_param", 42) == true, "Should accept static integer")
  assert(test_anim.get_param("test_param", 0) == 42, "Should return static integer")
  
  # Test with SolidColorProvider (ColorProvider extends ValueProvider)
  var color_provider = animation.solid_color_provider(0xFF00FF00)
  assert(test_anim.set_param("test_param", color_provider) == true, "Should accept ColorProvider")
  
  # Test with StaticValueProvider
  var static_provider = animation.static_value_provider(123)
  assert(test_anim.set_param("test_param", static_provider) == true, "Should accept StaticValueProvider")
  
  # Test with OscillatorValueProvider
  var oscillator = animation.oscillator_value_provider(0, 255, 1000, 1)  # SAWTOOTH
  assert(test_anim.set_param("test_param", oscillator) == true, "Should accept OscillatorValueProvider")
  
  # Test that invalid types are rejected (no type conversion)
  assert(test_anim.set_param("test_param", "invalid") == false, "Should reject string")
  assert(test_anim.set_param("test_param", true) == false, "Should reject boolean")
  assert(test_anim.set_param("test_param", 3.14) == false, "Should reject real")
  
  print("✓ Parameter validation with ValueProviders test passed")
end

# Test that set_loop method handles boolean conversion
def test_loop_boolean_conversion()
  print("Testing loop boolean conversion...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Test set_loop with boolean values
  test_anim.set_loop(true)
  assert(test_anim.loop == 1, "Should convert true to 1")
  
  test_anim.set_loop(false)
  assert(test_anim.loop == 0, "Should convert false to 0")
  
  # Test set_loop with integer values
  test_anim.set_loop(1)
  assert(test_anim.loop == 1, "Should accept integer 1")
  
  test_anim.set_loop(0)
  assert(test_anim.loop == 0, "Should accept integer 0")
  
  print("✓ Loop boolean conversion test passed")
end

# Test range validation
def test_range_validation()
  print("Testing range validation...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Register a parameter with range constraints
  test_anim.register_param("test_range", {"default": 50, "min": 0, "max": 100})
  
  # Test valid range values
  assert(test_anim.set_param("test_range", 50) == true, "Should accept value within range")
  assert(test_anim.set_param("test_range", 0) == true, "Should accept minimum value")
  assert(test_anim.set_param("test_range", 100) == true, "Should accept maximum value")
  
  # Test invalid range values
  assert(test_anim.set_param("test_range", -1) == false, "Should reject value below minimum")
  assert(test_anim.set_param("test_range", 101) == false, "Should reject value above maximum")
  
  print("✓ Range validation test passed")
end

# Test range validation is skipped for ValueProviders
def test_range_validation_with_providers()
  print("Testing range validation with ValueProviders...")
  
  # Create a test animation
  var test_anim = animation.animation(10, 0, false, "test")
  
  # Register a parameter with range constraints
  test_anim.register_param("test_range", {"default": 50, "min": 0, "max": 100})
  
  # Test that static values are still range-validated
  assert(test_anim.set_param("test_range", 50) == true, "Should accept value within range")
  assert(test_anim.set_param("test_range", 0) == true, "Should accept minimum value")
  assert(test_anim.set_param("test_range", 100) == true, "Should accept maximum value")
  assert(test_anim.set_param("test_range", -1) == false, "Should reject value below minimum")
  assert(test_anim.set_param("test_range", 101) == false, "Should reject value above maximum")
  
  # Test that ValueProviders bypass range validation
  # (since they provide dynamic values that can't be validated at set time)
  var oscillator = animation.oscillator_value_provider(-50, 150, 1000, 1)  # Outside range
  assert(test_anim.set_param("test_range", oscillator) == true, "Should accept ValueProvider even if it might produce out-of-range values")
  
  print("✓ Range validation with ValueProviders test passed")
end

# Run all tests
def run_parameter_validation_tests()
  print("=== Parameter Validation System Tests ===")
  
  try
    test_parameter_accepts_value_providers()
    test_loop_boolean_conversion()
    test_range_validation()
    test_range_validation_with_providers()
    
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