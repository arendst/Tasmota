# DSL Parameter Validation Test
# Tests the new parameter validation feature in the DSL transpiler

import animation
import animation_dsl
import string

# Test class to verify parameter validation during DSL transpilation
class DSLParameterValidationTest
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
  
  # Test valid parameters - should compile without errors
  def test_valid_parameters()
    var dsl_code = 
      "# strip length 30  # TEMPORARILY DISABLED\n"
      "animation breathe_test = breathe_animation(color=0xFF0000FF, period=2000, min_brightness=50)\n"
      "run breathe_test"
    
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "Valid parameters should compile successfully"
    end
    
    # Check that the generated code contains the expected parameter assignments
    if string.find(berry_code, "breathe_test_.color = 0xFF0000FF") == -1
      raise "generation_error", "Generated code should contain color parameter assignment"
    end
    
    if string.find(berry_code, "breathe_test_.period = 2000") == -1
      raise "generation_error", "Generated code should contain period parameter assignment"
    end
    
    if string.find(berry_code, "breathe_test_.min_brightness = 50") == -1
      raise "generation_error", "Generated code should contain min_brightness parameter assignment"
    end
  end
  
  # Test invalid parameter - should fail compilation with specific error
  def test_invalid_parameter()
    var dsl_code = 
      "# strip length 30  # TEMPORARILY DISABLED\n"
      "animation breathe_test = breathe_animation(color=0xFF0000FF, invalid_param=123)\n"
      "run breathe_test"
    
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
      raise "validation_error", "Invalid parameter should cause compilation to fail"
    end
    
    # Check that the error message mentions the invalid parameter
    if string.find(error_message, "invalid_param") == -1
      raise "error_message_error", f"Error message should mention 'invalid_param', got: {error_message}"
    end
  end
  
  # Test mixed valid and invalid parameters
  def test_mixed_parameters()
    var dsl_code = 
      "# strip length 30  # TEMPORARILY DISABLED\n"
      "animation breathe_test = breathe_animation(color=0xFF0000FF, period=2000, nonexistent_param=456)\n"
      "run breathe_test"
    
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
      raise "validation_error", "Invalid parameter should cause compilation to fail even with valid parameters present"
    end
    
    # Check that the error message mentions the invalid parameter
    if string.find(error_message, "nonexistent_param") == -1
      raise "error_message_error", f"Error message should mention 'nonexistent_param', got: {error_message}"
    end
  end
  
  # Test nested function calls with invalid parameters
  def test_nested_function_invalid_parameters()
    var dsl_code = 
      "# strip length 30  # TEMPORARILY DISABLED\n"
      "animation main_anim = pulsating_animation(color=breathe_animation(color=0xFF0000FF, bad_param=789))\n"
      "run main_anim"
    
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
      raise "validation_error", "Invalid parameter in nested function should cause compilation to fail"
    end
    
    # Check that the error message mentions the invalid parameter
    if string.find(error_message, "bad_param") == -1
      raise "error_message_error", f"Error message should mention 'bad_param', got: {error_message}"
    end
  end
  
  # Test that user functions are not validated (they use old positional style)
  def test_user_function_not_validated()
    # First register a user function
    animation.register_user_function("my_custom_anim", def(engine, param1, param2) 
      return animation.breathe_animation(engine)
    end)
    
    var dsl_code = 
      "# strip length 30  # TEMPORARILY DISABLED\n"
      "animation custom_test = my_custom_anim(123, 456)\n"
      "run custom_test"
    
    # This should compile successfully because user functions use positional parameters
    var berry_code = animation_dsl.compile_dsl(dsl_code)
    
    if berry_code == nil
      raise "compilation_error", "User functions should compile without parameter validation"
    end
    
    # Check that it generates the expected user function call
    if string.find(berry_code, "animation.get_user_function('my_custom_anim')") == -1
      raise "generation_error", "Generated code should contain user function call"
    end
  end
  
  # Test multiple animations with different parameter validation results
  def test_multiple_animations_validation()
    var dsl_code = 
      "# strip length 30  # TEMPORARILY DISABLED\n"
      "animation valid_anim = breathe_animation(color=0xFF0000FF, period=2000)\n"
      "animation invalid_anim = breathe_animation(color=0xFF00FF00, wrong_param=999)\n"
      "run valid_anim"
    
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
      raise "validation_error", "Invalid parameter should cause compilation to fail"
    end
    
    # Check that the error message mentions the invalid parameter
    if string.find(error_message, "wrong_param") == -1
      raise "error_message_error", f"Error message should mention 'wrong_param', got: {error_message}"
    end
  end
  
  # Run all tests
  def run_all_tests()
    print("Running DSL Parameter Validation Tests...")
    
    var total_tests = 0
    var passed_tests = 0
    
    # Test cases
    var tests = [
      ["Valid Parameters", / -> self.test_valid_parameters()],
      ["Invalid Parameter", / -> self.test_invalid_parameter()],
      ["Mixed Parameters", / -> self.test_mixed_parameters()],
      ["Nested Function Invalid Parameters", / -> self.test_nested_function_invalid_parameters()],
      ["User Function Not Validated", / -> self.test_user_function_not_validated()],
      ["Multiple Animations Validation", / -> self.test_multiple_animations_validation()]
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
      print("✓ All DSL parameter validation tests passed!")
      return true
    else
      print("✗ Some DSL parameter validation tests failed!")
      return false
    end
  end
end

# Run tests if this file is executed directly
# Note: Berry doesn't have __name__ equivalent, so we'll run tests by default
var test_runner = DSLParameterValidationTest()
test_runner.run_all_tests()

# Export for use in other test files
return {
  "DSLParameterValidationTest": DSLParameterValidationTest
}