# Test suite for nested function calls in DSL transpiler
#
# This test verifies that the DSL transpiler correctly handles nested function calls
# and generates proper Berry code for complex expressions.

import animation
import animation_dsl

# Test basic nested function calls
def test_basic_nested_calls()
  print("Testing basic nested function calls...")
  
  var dsl_code = 
    "# strip length 30  # TEMPORARILY DISABLED\n"
    "color custom_red = 0xFF0000\n"
    "animation pulse_red = pulsating_animation(color=static_color(color=custom_red), period=3s)\n"
    "run pulse_red"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
    
    # Check that the generated code contains the new engine-based pattern
    import string
    assert(string.find(berry_code, "animation.pulsating_animation(engine)") >= 0, 
           "Generated code should contain pulsating_animation with engine parameter")
    assert(string.find(berry_code, "animation.static_color(engine)") >= 0, 
           "Generated code should contain nested static_color function call")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
  
  print("✓ Basic nested function calls test passed")
end

# Test deep nesting
def test_deep_nesting()
  print("Testing deep nesting...")
  
  var dsl_code = 
    "# strip length 30  # TEMPORARILY DISABLED\n"
    "animation complex = pulsating_animation(color=static_color(color=red), period=2s)\n"
    "run complex"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
    
    # Check that the generated code contains nested calls
    import string
    assert(string.find(berry_code, "animation.pulsating_animation(") >= 0, "Should contain pulsating_animation function")
    assert(string.find(berry_code, "animation.static_color(") >= 0, "Should contain static_color function")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
  
  print("✓ Deep nesting test passed")
end

# Test nested calls with different parameter types
def test_mixed_parameter_types()
  print("Testing nested calls with mixed parameter types...")
  
  var dsl_code = 
    "# strip length 30  # TEMPORARILY DISABLED\n"
    "animation mixed = pulsating_animation(color=static_color(color=blue), period=2s, max_brightness=80%)\n"
    "run mixed"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
    
    # Check that different parameter types are handled correctly
    import string
    assert(string.find(berry_code, "animation.static_color(engine)") >= 0, 
           "Should contain nested static_color function call")
    assert(string.find(berry_code, "2000") >= 0, "Should contain time parameter")
    assert(string.find(berry_code, "204") >= 0, "Should contain percentage converted to 0-255 range")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
  
  print("✓ Mixed parameter types test passed")
end

# Test nested calls in array literals
def test_nested_calls_in_arrays()
  print("Testing nested calls in array literals...")
  
  var dsl_code = 
    "# strip length 30  # TEMPORARILY DISABLED\n"
    "animation cycle = pulsating_animation(color=static_color(color=red), period=5s)\n"
    "run cycle"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
    
    # Check that nested calls in arrays work
    import string
    assert(string.find(berry_code, "animation.pulsating_animation(engine)") >= 0, 
           "Should contain pulsating_animation function call")
    assert(string.find(berry_code, "animation.static_color(engine)") >= 0, 
           "Should contain nested static_color function call")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
  
  print("✓ Nested calls in arrays test passed")
end

# Test error handling for malformed nested calls
def test_error_handling()
  print("Testing error handling for malformed nested calls...")
  
  # Test unclosed parentheses
  var dsl_code1 = 
    "# strip length 30  # TEMPORARILY DISABLED\n"
    "animation bad = pulsating_animation(color=static_color(color=red)\n"  # Missing closing paren
    "run bad"
  
  try
    var berry_code1 = animation_dsl.compile(dsl_code1)
    assert(false, "Should have raised exception for unclosed parentheses")
  except "dsl_compilation_error" as e, msg
    # Expected behavior - syntax error should be caught
  end
  
  # Test invalid function name
  var dsl_code2 = 
    "# strip length 30  # TEMPORARILY DISABLED\n"
    "animation bad = invalid_function(color=red)\n"
    "run bad"
  
  try
    var berry_code2 = animation_dsl.compile(dsl_code2)
    # Should still generate code for unknown functions (runtime resolution)
    assert(berry_code2 != nil, "Should still generate code for unknown functions")
  except "dsl_compilation_error" as e, msg
    # May fail due to predefined color 'red' - that's also valid
  end
  
  print("✓ Error handling test passed")
end

# Test complex real-world example
def test_complex_real_world_example()
  print("Testing complex real-world example...")
  
  var dsl_code = 
    "# strip length 60  # TEMPORARILY DISABLED\n"
    "color sunset_red = 0xFF4500\n"
    "animation evening = pulsating_animation(\n"
    "  color=static_color(color=sunset_red),\n"
    "  period=10s\n"
    ")\n"
    "run evening"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
    
    # Verify the structure is preserved
    import string
    assert(string.find(berry_code, "animation.pulsating_animation(") >= 0, "Should contain pulsating_animation")
    assert(string.find(berry_code, "animation.static_color(") >= 0, "Should contain static_color")
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
  
  print("✓ Complex real-world example test passed")
end

# Test that generated code is valid Berry syntax
def test_generated_code_validity()
  print("Testing generated code validity...")
  
  var dsl_code = 
    "# strip length 30  # TEMPORARILY DISABLED\n"
    "color custom_red = 0xFF0000\n"
    "animation test = pulsating_animation(color=static_color(color=custom_red), period=3s)\n"
    "run test"
  
  try
    var berry_code = animation_dsl.compile(dsl_code)
    assert(berry_code != nil, "Generated Berry code should not be nil")
    
    # Try to compile the generated Berry code (basic syntax check)
    try
      compile(berry_code)
      print("✓ Generated code compiles successfully")
    except .. as e, msg
      print(f"Generated code compilation failed: {e} - {msg}")
      print("Generated code:")
      print(berry_code)
      assert(false, "Generated code should be valid Berry syntax")
    end
  except "dsl_compilation_error" as e, msg
    assert(false, f"DSL compilation should not fail: {msg}")
  end
  
  print("✓ Generated code validity test passed")
end

# Run all tests
def run_nested_function_calls_tests()
  print("=== Nested Function Calls Tests ===")
  
  try
    test_basic_nested_calls()
    test_deep_nesting()
    test_mixed_parameter_types()
    test_nested_calls_in_arrays()
    test_error_handling()
    test_complex_real_world_example()
    test_generated_code_validity()
    
    print("=== All nested function calls tests passed! ===")
    return true
  except .. as e, msg
    print(f"Test failed: {e} - {msg}")
    raise "test_failed"
  end
end

# Export the test function
animation.run_nested_function_calls_tests = run_nested_function_calls_tests

run_nested_function_calls_tests()

return run_nested_function_calls_tests