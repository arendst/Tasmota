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
  
  # Test strip_length arithmetic expression (should fail compilation due to dangerous pattern)
  def test_strip_length_arithmetic_expression()
    var dsl_code = "set strip_len3 = (strip_length() + 1) / 2"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "strip_length in arithmetic expression should cause compilation to fail due to dangerous pattern"
    end
    
    # Check that the error message mentions the dangerous pattern
    if string.find(error_message, "cannot be used in computed expressions") == -1
      raise "error_message_error", f"Error message should mention computed expressions restriction, got: {error_message}"
    end
  end
  
  # Test complex strip_length arithmetic (should fail compilation due to dangerous pattern)
  def test_strip_length_complex_arithmetic()
    var dsl_code = "set complex = (strip_length() + 5) * 2 - strip_length() / 4"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "Complex strip_length arithmetic should cause compilation to fail due to dangerous pattern"
    end
    
    # Check that the error message mentions the dangerous pattern
    if string.find(error_message, "cannot be used in computed expressions") == -1
      raise "error_message_error", f"Error message should mention computed expressions restriction, got: {error_message}"
    end
  end
  
  # Test mixed user variables and strip_length (should fail due to dangerous pattern)
  def test_mixed_variables_and_strip_length()
    var dsl_code = "set val1 = 10\nset mixed = val1 + strip_length() * 2"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "Mixed variables and strip_length should cause compilation to fail due to dangerous pattern"
    end
    
    # Check that the error message mentions the dangerous pattern
    if string.find(error_message, "cannot be used in computed expressions") == -1
      raise "error_message_error", f"Error message should mention computed expressions restriction, got: {error_message}"
    end
  end
  
  # Test strip_length in property assignment (currently allowed due to anonymous function wrapper)
  def test_strip_length_in_property_assignment()
    var dsl_code = "animation test = solid(color=red)\ntest.opacity = strip_length() / 2"
    
    var compilation_failed = false
    var error_message = ""

    try
      var berry_code = animation_dsl.compile(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "strip_length in property assignment should compile (anonymous function wrapper bypasses dangerous pattern detection)"
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
  
  # Test edge case: strip_length with parentheses (should fail due to dangerous pattern)
  def test_strip_length_with_parentheses()
    var dsl_code = "set result = (strip_length()) * 2"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "strip_length with parentheses in arithmetic should cause compilation to fail due to dangerous pattern"
    end
    
    # Check that the error message mentions the dangerous pattern
    if string.find(error_message, "cannot be used in computed expressions") == -1
      raise "error_message_error", f"Error message should mention computed expressions restriction, got: {error_message}"
    end
  end
  
  # Test edge case: strip_length in nested expressions (should fail due to dangerous pattern)
  def test_strip_length_nested_expressions()
    var dsl_code = "set nested = ((strip_length() + 1) * 2) / (strip_length() - 1)"
    
    var compilation_failed = false
    var error_message = ""
    
    try
      var berry_code = animation_dsl.compile(dsl_code)
      if berry_code == nil
        compilation_failed = true
      end
    except "dsl_compilation_error" as e, msg
      compilation_failed = true
      error_message = msg
    end
    
    if !compilation_failed
      raise "validation_error", "Nested strip_length expressions should cause compilation to fail due to dangerous pattern"
    end
    
    # Check that the error message mentions the dangerous pattern
    if string.find(error_message, "cannot be used in computed expressions") == -1
      raise "error_message_error", f"Error message should mention computed expressions restriction, got: {error_message}"
    end
  end
  
  # Test that strip_length works in non-arithmetic contexts (direct assignment without closure)
  def test_strip_length_non_arithmetic_contexts()
    var dsl_code = "animation test = solid(color=red)\ntest.opacity = strip_length()"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "strip_length in non-arithmetic context should compile successfully"
    end
    
    # In property assignment context, strip_length should be assigned directly without closure wrapping
    # since it's a value provider instance without additional computation
    if string.find(berry_code, "def (engine)") != -1
      raise "generation_error", "strip_length in property assignment should NOT use anonymous function wrapper when used alone"
    end
    
    if string.find(berry_code, "animation.strip_length(engine)") == -1
      raise "generation_error", "Should contain direct strip_length call"
    end
    
    # Should not contain animation.resolve since it's not in a computed expression
    if string.find(berry_code, "animation.resolve") != -1
      raise "generation_error", "strip_length should not be wrapped with resolve in this context"
    end
  end
  
  # Test the safe pattern: separate strip_length() call from computation
  def test_strip_length_safe_pattern()
    var dsl_code = "set strip_len = strip_length()\nset computed = (strip_len + 1) / 2"
    
    var berry_code = animation_dsl.compile(dsl_code)
    if berry_code == nil
      raise "compilation_error", "Safe strip_length pattern should compile successfully"
    end
    
    # Check that strip_len is created as a direct call
    if string.find(berry_code, "var strip_len_ = animation.strip_length(engine)") == -1
      raise "generation_error", "strip_len should be created as direct call"
    end
    
    # Check that computed uses closure with animation.resolve for the variable
    if string.find(berry_code, "create_closure_value") == -1
      raise "generation_error", "Computed expression should create closure"
    end
    
    if string.find(berry_code, "animation.resolve(strip_len_)") == -1
      raise "generation_error", "User variable strip_len_ should be wrapped with animation.resolve()"
    end
    
    # Should NOT contain direct strip_length calls in the computed expression
    var computed_line_start = string.find(berry_code, "var computed_ = ")
    if computed_line_start != -1
      var computed_line_end = string.find(berry_code, "\n", computed_line_start)
      if computed_line_end == -1
        computed_line_end = size(berry_code)
      end
      var computed_line = berry_code[computed_line_start..computed_line_end-1]
      
      if string.find(computed_line, "animation.strip_length(engine)") != -1
        raise "generation_error", "Computed expression should not contain direct strip_length calls"
      end
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
      ["Strip Length Nested Expressions", / -> self.test_strip_length_nested_expressions()],
      ["Strip Length Non-Arithmetic Contexts", / -> self.test_strip_length_non_arithmetic_contexts()],
      ["Strip Length Safe Pattern", / -> self.test_strip_length_safe_pattern()]
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