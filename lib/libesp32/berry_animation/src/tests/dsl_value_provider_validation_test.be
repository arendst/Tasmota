# DSL Value Provider Validation Test
# Tests that value provider parameters are validated during DSL transpilation

import animation
import animation_dsl
import string

# Test class to verify value provider parameter validation
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
      "strip length 30\n"
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
      "strip length 30\n"
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
      "strip length 30\n"
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
      "strip length 30\n"
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
  
  # Run all tests
  def run_all_tests()
    print("Running DSL Value Provider Validation Tests...")
    
    var total_tests = 0
    var passed_tests = 0
    
    # Test cases
    var tests = [
      ["Valid Value Provider Parameters", / -> self.test_valid_value_provider_parameters()],
      ["Invalid Value Provider Parameter", / -> self.test_invalid_value_provider_parameter()],
      ["Nonexistent Value Provider", / -> self.test_nonexistent_value_provider()],
      ["Nested Value Providers", / -> self.test_nested_value_providers()]
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
      print("✓ All DSL value provider validation tests passed!")
      return true
    else
      print("✗ Some DSL value provider validation tests failed!")
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