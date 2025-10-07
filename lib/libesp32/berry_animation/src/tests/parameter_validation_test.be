# Test suite for parameter validation system
#
# This test verifies that the parameter validation system correctly accepts
# ValueProvider instances for integer and real parameters.

import animation
import global

import "./core/param_encoder" as encode_constraints

# Test that parameters accept ValueProviders and integers only
def test_parameter_accepts_value_providers()
  print("Testing parameter validation with ValueProviders...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
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
  
  print("✓ Parameter validation with ValueProviders test passed")
end

# Test that loop parameter handles boolean values correctly
def test_loop_boolean_validation()
  print("Testing loop boolean validation...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
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
  assert(test_anim.set_param("loop", 3.14) == false, "Should reject real for loop (boolean parameter)")
  
  print("✓ Loop boolean validation test passed")
end

# Test range validation
def test_range_validation()
  print("Testing range validation...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  # Create a test animation
  var test_anim = animation.animation(engine)
  
  # Test valid range values (using existing 'opacity' parameter with range 0-255)
  assert(test_anim.set_param("opacity", 50) == true, "Should accept value within range")
  assert(test_anim.set_param("opacity", 0) == true, "Should accept minimum value")
  assert(test_anim.set_param("opacity", 255) == true, "Should accept maximum value")
   
  print("✓ Range validation test passed")
end

# Test range validation is skipped for ValueProviders
def test_range_validation_with_providers()
  print("Testing range validation with ValueProviders...")
  
  # Create engine for testing
  var strip = global.Leds()
  var engine = animation.create_engine(strip)
  
  # Create a test animation
  var test_anim = animation.animation(engine)
  
  # Test that static values are still range-validated (using existing 'opacity' parameter with range 0-255)
  assert(test_anim.set_param("opacity", 50) == true, "Should accept value within range")
  assert(test_anim.set_param("opacity", 0) == true, "Should accept minimum value")
  assert(test_anim.set_param("opacity", 255) == true, "Should accept maximum value")
  
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
  var engine = animation.create_engine(strip)
  
  # Create a test class with different parameter types
  class TestClass : animation.parameterized_object
    static var PARAMS = encode_constraints({
      "int_param": {"default": 42},                    # Default type is "int"
      "explicit_int_param": {"type": "int", "default": 10},
      "string_param": {"type": "string", "default": "hello"},
      "bool_param": {"type": "bool", "default": true},
      "instance_param": {"type": "instance", "default": nil},
      "any_param": {"type": "any", "default": nil}
    })
    
    def init(engine)
      super(self).init(engine)
    end
  end
  
  var test_obj = TestClass(engine)
  
  # Test int parameter (default type)
  assert(test_obj.set_param("int_param", 123) == true, "Should accept int for int_param")
  assert(test_obj.set_param("int_param", "string") == false, "Should reject string for int_param")
  assert(test_obj.set_param("int_param", true) == false, "Should reject bool for int_param")
  assert(test_obj.set_param("int_param", 3.14) == true, "Should accept real for int_param (recent change)")
  
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

# Test DSL compile-time parameter validation
def test_dsl_parameter_validation()
  print("Testing DSL compile-time parameter validation...")
  
  import animation_dsl
  
  # Test valid animation parameter
  var valid_dsl = "animation red_eye = beacon_animation(color = red)\n" +
                  "red_eye.back_color = blue"
  
  var result = animation_dsl.compile(valid_dsl)
  assert(result != nil, "Valid parameter should compile successfully")
  
  # Test invalid animation parameter
  var invalid_dsl = "animation red_eye = beacon_animation(color = red)\n" +
                    "red_eye.invalid_param = 123"
  
  try
    animation_dsl.compile(invalid_dsl)
    assert(false, "Invalid parameter should cause compilation error")
  except .. as e
    # Expected - invalid parameter should be caught
    assert(true, "Invalid parameter correctly rejected")
  end
  
  # Test valid color provider parameter
  var valid_color_dsl = "color solid_red = static_color(color = red)\n" +
                        "solid_red.color = blue"
  
  var result2 = animation_dsl.compile(valid_color_dsl)
  assert(result2 != nil, "Valid color provider parameter should compile successfully")
  
  # Test invalid color provider parameter
  var invalid_color_dsl = "color solid_red = static_color(color = red)\n" +
                          "solid_red.invalid_param = 123"
  
  try
    animation_dsl.compile(invalid_color_dsl)
    assert(false, "Invalid color provider parameter should cause compilation error")
  except .. as e
    # Expected - invalid parameter should be caught
    assert(true, "Invalid color provider parameter correctly rejected")
  end
  
  # Test unknown objects skip validation (no error)
  var unknown_dsl = "unknown_object.some_param = 123"
  var result3 = animation_dsl.compile(unknown_dsl)
  assert(result3 != nil, "Unknown objects should not cause validation errors")
  
  print("✓ DSL compile-time parameter validation test passed")
end

# Test DSL object reference validation
def test_dsl_object_reference_validation()
  print("Testing DSL object reference validation...")
  
  import animation_dsl
  
  # Test valid run statement
  var valid_run = "animation red_eye = beacon_animation(color = red)\n" +
                  "run red_eye"
  
  var result = animation_dsl.compile(valid_run)
  assert(result != nil, "Valid run statement should compile successfully")
  
  # Test invalid run statement (undefined object)
  var invalid_run = "animation red_eye = beacon_animation(color = red)\n" +
                    "run undefined_animation"
  
  try
    animation_dsl.compile(invalid_run)
    assert(false, "Invalid run statement should cause compilation error")
  except .. as e
    # Expected - undefined reference should be caught
    assert(true, "Undefined reference in run statement correctly rejected")
  end
  
  # Test valid sequence with play statement
  var valid_sequence = "animation red_eye = beacon_animation(color = red)\n" +
                       "sequence demo {\n" +
                       "  play red_eye for 5s\n" +
                       "  wait 1s\n" +
                       "}"
  
  var result2 = animation_dsl.compile(valid_sequence)
  assert(result2 != nil, "Valid sequence should compile successfully")
  
  # Test invalid sequence with undefined play reference
  var invalid_sequence = "animation red_eye = beacon_animation(color = red)\n" +
                         "sequence demo {\n" +
                         "  play undefined_animation for 5s\n" +
                         "  wait 1s\n" +
                         "}"
  
  try
    animation_dsl.compile(invalid_sequence)
    assert(false, "Invalid sequence should cause compilation error")
  except .. as e
    # Expected - undefined reference should be caught
    assert(true, "Undefined reference in sequence play statement correctly rejected")
  end
  
  print("✓ DSL object reference validation test passed")
end

# Test DSL sequence symbol table registration
def test_dsl_sequence_symbol_table_registration()
  print("Testing DSL sequence symbol table registration...")
  
  import animation_dsl
  
  # Test 1: Valid sequence should be registered and runnable
  var valid_sequence_dsl = "animation red_anim = beacon_animation(color = red)\n" +
                           "sequence demo {\n" +
                           "  play red_anim for 2s\n" +
                           "}\n" +
                           "run demo"
  
  var result = animation_dsl.compile(valid_sequence_dsl)
  assert(result != nil, "Valid sequence should compile successfully")
  
  # Test 2: Sequence with invalid animation reference should fail at sequence processing
  var invalid_anim_sequence_dsl = "animation red_anim = nonexistent_function(color = red)\n" +
                                  "sequence demo {\n" +
                                  "  play red_anim for 2s\n" +
                                  "}\n" +
                                  "run demo"
  
  try
    animation_dsl.compile(invalid_anim_sequence_dsl)
    assert(false, "Invalid animation reference should cause compilation error")
  except .. as e
    # Expected - invalid animation should be caught
    assert(true, "Invalid animation reference correctly rejected")
  end
  
  # Test 3: Sequence with undefined identifier should fail
  var undefined_identifier_dsl = "sequence demo {\n" +
                                 "  play undefined_anim for 2s\n" +
                                 "}\n" +
                                 "run demo"
  
  try
    animation_dsl.compile(undefined_identifier_dsl)
    assert(false, "Undefined identifier in sequence should cause compilation error")
  except .. as e
    # Expected - undefined identifier should be caught
    assert(true, "Undefined identifier in sequence correctly rejected")
  end
  
  print("✓ DSL sequence symbol table registration test passed")
end

# Test DSL symbol table mixed types handling
def test_dsl_symbol_table_mixed_types()
  print("Testing DSL symbol table mixed types handling...")
  
  import animation_dsl
  
  # Test 1: Valid property assignment on animation (instance in symbol table)
  var animation_property_dsl = "animation red_anim = beacon_animation(color = red)\n" +
                               "red_anim.back_color = blue"
  
  var result1 = animation_dsl.compile(animation_property_dsl)
  assert(result1 != nil, "Animation property assignment should work")
  
  # Test 2: Valid property assignment on color provider (instance in symbol table)
  var color_property_dsl = "color solid_red = static_color(color = red)\n" +
                           "solid_red.color = blue"
  
  var result2 = animation_dsl.compile(color_property_dsl)
  assert(result2 != nil, "Color provider property assignment should work")
  
  # Test 3: Invalid property assignment on sequence (string in symbol table)
  var sequence_property_dsl = "animation red_anim = beacon_animation(color = red)\n" +
                              "sequence demo {\n" +
                              "  play red_anim for 2s\n" +
                              "}\n" +
                              "demo.invalid_property = 123"
  
  try
    animation_dsl.compile(sequence_property_dsl)
    assert(false, "Sequence property assignment should cause compilation error")
  except .. as e
    # Expected - sequence property assignment should be rejected
    assert(true, "Sequence property assignment correctly rejected")
  end
  
  # Test 4: Mixed symbol table with sequences and instances
  var mixed_dsl = "animation red_anim = beacon_animation(color = red)\n" +
                  "color solid_blue = static_color(color = blue)\n" +
                  "sequence demo {\n" +
                  "  play red_anim for 2s\n" +
                  "}\n" +
                  "red_anim.back_color = solid_blue\n" +
                  "run demo"
  
  var result4 = animation_dsl.compile(mixed_dsl)
  assert(result4 != nil, "Mixed symbol table operations should work")
  
  print("✓ DSL symbol table mixed types handling test passed")
end

# Test DSL identifier reference symbol table registration
def test_dsl_identifier_reference_symbol_table()
  print("Testing DSL identifier reference symbol table registration...")
  
  import animation_dsl
  
  # Test 1: Animation reference should be added to symbol table
  var animation_ref_dsl = "animation solid_red = solid(color=red)\n" +
                          "animation red_anim = solid_red\n" +
                          "sequence demo {\n" +
                          "  play red_anim for 2s\n" +
                          "}\n" +
                          "run demo"
  
  var result1 = animation_dsl.compile(animation_ref_dsl)
  assert(result1 != nil, "Animation reference should compile successfully")
  
  # Test 2: Parameter validation on referenced animation
  var param_validation_dsl = "animation solid_red = solid(color=red)\n" +
                             "animation red_anim = solid_red\n" +
                             "red_anim.color = blue"
  
  var result2 = animation_dsl.compile(param_validation_dsl)
  assert(result2 != nil, "Parameter validation on referenced animation should work")
  
  # Test 3: Color provider reference should be added to symbol table
  var color_ref_dsl = "color base_red = static_color(color=red)\n" +
                      "color my_red = base_red\n" +
                      "animation red_anim = solid(color=my_red)\n" +
                      "my_red.color = blue"
  
  var result3 = animation_dsl.compile(color_ref_dsl)
  assert(result3 != nil, "Color provider reference should work")
  
  # Test 4: Invalid parameter on referenced animation should fail
  var invalid_param_dsl = "animation solid_red = solid(color=red)\n" +
                          "animation red_anim = solid_red\n" +
                          "red_anim.invalid_param = 123"
  
  try
    animation_dsl.compile(invalid_param_dsl)
    assert(false, "Invalid parameter on referenced animation should cause compilation error")
  except .. as e
    # Expected - invalid parameter should be caught
    assert(true, "Invalid parameter on referenced animation correctly rejected")
  end
  
  print("✓ DSL identifier reference symbol table registration test passed")
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
    test_dsl_parameter_validation()
    test_dsl_object_reference_validation()
    test_dsl_sequence_symbol_table_registration()
    test_dsl_symbol_table_mixed_types()
    test_dsl_identifier_reference_symbol_table()
    
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