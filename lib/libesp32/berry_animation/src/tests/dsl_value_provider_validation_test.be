# DSL Value Provider Validation Test
# Tests that value provider parameters are validated during DSL transpilation
# AND tests the fix for strip_length arithmetic expressions
#
# This test suite covers:
# 1. Original value provider parameter validation
# 2. strip_length() simple assignment (should remain unchanged)
# 3. strip_length() in arithmetic expressions (should be wrapped with animation.resolve())
# 4. Complex arithmetic with multiple strip_length() calls
# 5. Mixed user variables and strip_length() calls
# 6. Property assignments with strip_length() arithmetic
# 7. Edge cases: parentheses, nested expressions, multiple calls
# 8. Regression testing to ensure existing functionality still works

import animation
import animation_dsl
import string

# Test class to verify value provider parameter validation and arithmetic expression fixes
class DSLValueProviderValidationTest
  var test_results
  
  def init()
    self.test_results = []
  end
  
  # Helper method to run a test case
  def run_test(test_name, test_func)
    try
      test_func()
      self.test_results.push(f"✓ {test_name}")
      return true
    except .. as e, msg
      self.test_results.push(f"✗ {test_name}: {msg}")
      return false
    end
  end
  
  # Test valid value provider parameters
  def test_valid_value_provider_parameters()
    var dsl_code = 
      "# strip length 30  # TEMPORARILY DISABLED\n"
      "animation test = pulsating_animation(color=0xFF0000FF, min_brightness=oscillator_value(min_value=0, max_value=100))\n"
      "run test"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Valid value provider parameters should compile successfully"
    end
    
    # Check that the generated code contains the expected value provider
    if string.find(berry_code, "oscillator_value(engine)") == -1
      raise "generation_error", "Generated code should contain oscillator_value instantiation"
    end
  end
  
  # Test invalid value provider parameter
  def test_invalid_value_provider_parameter()
    var dsl_code = 
      "# strip length 30  # TEMPORARILY DISABLED\n"
      "animation test = pulsating_animation(color=0xFF0000FF, min_brightness=oscillator_value(min_value=0, invalid_param=123))\n"
      "run test"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "Invalid value provider parameter should cause compilation to fail"
    end
    
    # Check that the error message mentions the invalid parameter
    if string.find(error_message, "invalid_param") == -1
      raise "error_message_error", f"Error message should mention 'invalid_param', got: {error_message}"
    end
  end
  
  # Test nonexistent value provider
  def test_nonexistent_value_provider()
    var dsl_code = 
      "# strip length 30  # TEMPORARILY DISABLED\n"
      "animation test = pulsating_animation(color=0xFF0000FF, min_brightness=nonexistent_provider(param=123))\n"
      "run test"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "Nonexistent value provider should cause compilation to fail"
    end
    
    # Check that the error message mentions the nonexistent provider
    if string.find(error_message, "nonexistent_provider") == -1
      raise "error_message_error", f"Error message should mention 'nonexistent_provider', got: {error_message}"
    end
  end
  
  # Test nested value providers
  def test_nested_value_providers()
    var dsl_code = 
      "# strip length 30  # TEMPORARILY DISABLED\n"
      "animation test = pulsating_animation(color=color_cycle(palette=[0xFF0000FF, 0xFF00FF00], cycle_period=oscillator_value(min_value=1000, bad_param=456)))\n"
      "run test"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile_dsl(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "Invalid parameter in nested value provider should cause compilation to fail"
    end
    
    # Check that the error message mentions the invalid parameter
    if string.find(error_message, "bad_param") == -1
      raise "error_message_error", f"Error message should mention 'bad_param', got: {error_message}"
    end
  end
  
  # Test strip_length simple assignment (should remain unchanged)
  def test_strip_length_simple_assignment()
    var dsl_code = "set strip_len = strip_length()"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "Simple strip_length assignment should compile successfully"
    end
    
    # Check that it generates direct assignment without closure
    if string.find(berry_code, "var strip_len_ = animation.strip_length(engine)") == -1
      raise "generation_error", "Simple assignment should generate direct call without closure"
    end
    
    # Should NOT contain create_closure_value for simple assignment
    if string.find(berry_code, "create_closure_value") != -1
      raise "generation_error", "Simple assignment should not create closure"
    end
  end
  
  # Test strip_length arithmetic expression (should be wrapped with animation.resolve)
  def test_strip_length_arithmetic_expression()
    var dsl_code = "set strip_len3 = (strip_length() + 1) / 2"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "strip_length arithmetic expression should compile successfully"
    end
    
    # Check that it generates closure with animation.resolve wrapper
    if string.find(berry_code, "create_closure_value") == -1
      raise "generation_error", "Arithmetic expression should create closure"
    end
    
    # Most importantly, check that strip_length is wrapped with animation.resolve
    if string.find(berry_code, "animation.resolve(animation.strip_length(engine))") == -1
      raise "generation_error", "strip_length in arithmetic should be wrapped with animation.resolve()"
    end
    
    # Check the complete expected pattern
    var expected_pattern = "def (engine) return (animation.resolve(animation.strip_length(engine)) + 1) / 2 end"
    if string.find(berry_code, expected_pattern) == -1
      raise "generation_error", f"Generated code should contain expected pattern: {expected_pattern}"
    end
  end
  
  # Test complex strip_length arithmetic
  def test_strip_length_complex_arithmetic()
    var dsl_code = "set complex = (strip_length() + 5) * 2 - strip_length() / 4"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "Complex strip_length arithmetic should compile successfully"
    end
    
    # Check that both strip_length calls are wrapped with animation.resolve
    var resolve_count = 0
    var pos = 0
    while true
      var found_pos = string.find(berry_code, "animation.resolve(animation.strip_length(engine))", pos)
      if found_pos == -1
        break
      end
      resolve_count += 1
      pos = found_pos + 1
    end
    
    if resolve_count != 2
      raise "generation_error", f"Expected 2 animation.resolve() calls for strip_length, found {resolve_count}"
    end
  end
  
  # Test mixed user variables and strip_length
  def test_mixed_variables_and_strip_length()
    var dsl_code = "set val1 = 10\nset mixed = val1 + strip_length() * 2"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "Mixed variables and strip_length should compile successfully"
    end
    
    # Check that both val1_ and strip_length are properly resolved
    if string.find(berry_code, "animation.resolve(val1_)") == -1
      raise "generation_error", "User variable val1_ should be wrapped with animation.resolve()"
    end
    
    if string.find(berry_code, "animation.resolve(animation.strip_length(engine))") == -1
      raise "generation_error", "strip_length should be wrapped with animation.resolve()"
    end
  end
  
  # Test strip_length in property assignment
  def test_strip_length_in_property_assignment()
    var dsl_code = "animation test = solid(color=red)\ntest.opacity = strip_length() / 2"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "strip_length in property assignment should compile successfully"
    end
    
    # Check that property assignment creates closure with resolved strip_length
    if string.find(berry_code, "test_.opacity = animation.create_closure_value") == -1
      raise "generation_error", "Property assignment with arithmetic should create closure"
    end
    
    if string.find(berry_code, "animation.resolve(animation.strip_length(engine))") == -1
      raise "generation_error", "strip_length in property assignment should be wrapped with animation.resolve()"
    end
  end
  
  # Test that fix doesn't break existing functionality
  def test_no_regression_with_regular_expressions()
    var dsl_code = "set val1 = 10\nset val2 = val1 * 2 + 5"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "Regular arithmetic expressions should still work"
    end
    
    # Check that val1_ is properly resolved but no strip_length calls exist
    if string.find(berry_code, "animation.resolve(val1_)") == -1
      raise "generation_error", "User variable should still be resolved"
    end
    
    # Should not contain any strip_length calls
    if string.find(berry_code, "strip_length") != -1
      raise "generation_error", "Should not contain strip_length calls in this test"
    end
  end
  
  # Test edge case: strip_length with parentheses
  def test_strip_length_with_parentheses()
    var dsl_code = "set result = (strip_length()) * 2"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "strip_length with parentheses should compile successfully"
    end
    
    # Check that strip_length is still properly resolved even with extra parentheses
    if string.find(berry_code, "animation.resolve(animation.strip_length(engine))") == -1
      raise "generation_error", "strip_length with parentheses should be wrapped with animation.resolve()"
    end
  end
  
  # Test edge case: multiple strip_length calls in same expression
  def test_multiple_strip_length_calls()
    var dsl_code = "set ratio = strip_length() / (strip_length() + 10)"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "Multiple strip_length calls should compile successfully"
    end
    
    # Count the number of animation.resolve(animation.strip_length(engine)) calls
    var resolve_count = 0
    var pos = 0
    while true
      var found_pos = string.find(berry_code, "animation.resolve(animation.strip_length(engine))", pos)
      if found_pos == -1
        break
      end
      resolve_count += 1
      pos = found_pos + 1
    end
    
    if resolve_count != 2
      raise "generation_error", f"Expected 2 resolved strip_length calls, found {resolve_count}"
    end
  end
  
  # Test edge case: strip_length in nested expressions
  def test_strip_length_nested_expressions()
    var dsl_code = "set nested = ((strip_length() + 1) * 2) / (strip_length() - 1)"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "Nested strip_length expressions should compile successfully"
    end
    
    # Both strip_length calls should be resolved
    var resolve_count = 0
    var pos = 0
    while true
      var found_pos = string.find(berry_code, "animation.resolve(animation.strip_length(engine))", pos)
      if found_pos == -1
        break
      end
      resolve_count += 1
      pos = found_pos + 1
    end
    
    if resolve_count != 2
      raise "generation_error", f"Expected 2 resolved strip_length calls in nested expression, found {resolve_count}"
    end
  end
  
  # Test that the fix doesn't affect strip_length when not in arithmetic
  def test_strip_length_non_arithmetic_contexts()
    var dsl_code = "animation test = solid(color=red)\ntest.opacity = strip_length()"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "strip_length in non-arithmetic context should compile successfully"
    end
    
    # In non-arithmetic context, strip_length should be used directly without resolve
    if string.find(berry_code, "test_.opacity = animation.strip_length(engine)") == -1
      raise "generation_error", "strip_length in non-arithmetic context should be used directly"
    end
    
    # Should not contain animation.resolve for this case
    if string.find(berry_code, "animation.resolve(animation.strip_length(engine))") != -1
      raise "generation_error", "strip_length in non-arithmetic context should not be wrapped with resolve"
    end
  end
  
  # Run all tests
  def run_all_tests()
    print("Running DSL Value Provider Validation and Strip Length Arithmetic Tests...")
    
    var total_tests = 0
    var passed_tests = 0
    
    # Test cases
    var tests = [
      ["Valid Value Provider Parameters", / -> self.test_valid_value_provider_parameters()],
      ["Invalid Value Provider Parameter", / -> self.test_invalid_value_provider_parameter()],
      ["Nonexistent Value Provider", / -> self.test_nonexistent_value_provider()],
      ["Nested Value Providers", / -> self.test_nested_value_providers()],
      ["Strip Length Simple Assignment", / -> self.test_strip_length_simple_assignment()],
      ["Strip Length Arithmetic Expression", / -> self.test_strip_length_arithmetic_expression()],
      ["Strip Length Complex Arithmetic", / -> self.test_strip_length_complex_arithmetic()],
      ["Mixed Variables and Strip Length", / -> self.test_mixed_variables_and_strip_length()],
      ["Strip Length in Property Assignment", / -> self.test_strip_length_in_property_assignment()],
      ["No Regression with Regular Expressions", / -> self.test_no_regression_with_regular_expressions()],
      ["Strip Length with Parentheses", / -> self.test_strip_length_with_parentheses()],
      ["Multiple Strip Length Calls", / -> self.test_multiple_strip_length_calls()],
      ["Strip Length Nested Expressions", / -> self.test_strip_length_nested_expressions()],
      ["Strip Length Non-Arithmetic Contexts", / -> self.test_strip_length_non_arithmetic_contexts()]
    ]
    
    for test : tests
      total_tests += 1
      if self.run_test(test[0], test[1])
        passed_tests += 1
      end
    end
    
    # Print results
    print(f"\nTest Results:")
    for result : self.test_results
      print(f"  {result}")
    end
    
    print(f"\nSummary: {passed_tests}/{total_tests} tests passed")
    
    if passed_tests == total_tests
      print("✓ All DSL value provider validation and strip length arithmetic tests passed!")
      return true
    else
      print("✗ Some DSL value provider validation and strip length arithmetic tests failed!")
      raise "test_failed"
    end
  end
end

# Run tests
var test_runner = DSLValueProviderValidationTest()
test_runner.run_all_tests()

# Export for use in other test files
return {
  "DSLValueProviderValidationTest": DSLValueProviderValidationTest
}